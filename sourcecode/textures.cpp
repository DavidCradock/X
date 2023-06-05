#include "PCH.h"
#include "textures.h"
#include "log.h"

namespace X
{
	Texture::Texture(const std::string& strTextureFilename)
	{
		mstrTextureFilename = strTextureFilename;
	}

	Texture::~Texture(void)
	{
	}

	void Texture::load(void)
	{
		// Make sure filenames exist
		ThrowIfFalse(bool(mstrTextureFilename.size() > 0), "Texture::load() failed as filename of the file holding the texture is not set.");
	}

	void Texture::unload(void)
	{

	}

	TextureManager::TextureManager()
	{
		addNewGroup("default");
	}

	unsigned int TextureManager::getNumGroups(void)
	{
		return (unsigned int)mmapGroup.size();
	}

	unsigned int TextureManager::getNumResInGroup(const std::string& strGroupName)
	{
		if (!groupExists(strGroupName))
		{
			std::string err("TextureManager::getNumResInGroup(\"");
			err.append(strGroupName);
			err.append("\") failed. The group doesn't exist.");
			ThrowIfTrue(1, err);
		}
		std::map<std::string, Group*>::iterator itg = mmapGroup.find(strGroupName);
		return (unsigned int)itg->second->mmapResource.size();;
	}

	unsigned int TextureManager::getNumResInGroupLoaded(const std::string& strGroupName)
	{
		if (!groupExists(strGroupName))
		{
			std::string err("TextureManager::getNumResInGroupLoaded(\"");
			err.append(strGroupName);
			err.append("\") failed. The group doesn't exist.");
			ThrowIfTrue(1, err);
		}
		std::map<std::string, Group*>::iterator itg = mmapGroup.find(strGroupName);

		unsigned int iResLoadedTotal = 0;
		std::map<std::string, Resource*>::iterator it = itg->second->mmapResource.begin();
		while (it != itg->second->mmapResource.end())
		{
			if (it->second->bLoaded)
				++iResLoadedTotal;
			it++;
		}
		return iResLoadedTotal;
	}

	const std::string& TextureManager::getGroupName(unsigned int iGroupIndex)
	{
		if (iGroupIndex >= mmapGroup.size())
		{
			std::string err("TextureManager::getGroupName(");

			err.append(std::to_string(iGroupIndex));
			err.append(") failed. Invalid index given. Number of groups is ");
			err.append(std::to_string(getNumGroups()));
			ThrowIfTrue(1, err);
		}
		std::map<std::string, Group*>::iterator itg = mmapGroup.begin();
		unsigned int i = 0;
		while (i < iGroupIndex)
		{
			itg++;
			++i;
		}
		return itg->first;
	}

	void TextureManager::addNewGroup(const std::string& strNewGroupName)
	{
		if (groupExists(strNewGroupName))
		{
			return;
		}
		Group* pNewGroup = new Group;
		mmapGroup[strNewGroupName] = pNewGroup;
	}

	bool TextureManager::groupExists(const std::string& strGroupName)
	{
		std::map<std::string, Group*>::iterator it = mmapGroup.find(strGroupName);
		if (it == mmapGroup.end())
			return false;
		return true;
	}

	void TextureManager::loadGroup(const std::string& strGroupName)
	{
		// Group doesn't exist?
		if (!groupExists(strGroupName))
		{
			std::string err("TextureManager::loadGroup(\"");
			err.append(strGroupName);
			err.append("\") failed. As the given named group doesn't exist.");
			ThrowIfTrue(1, err);
		}

		// Load any unloaded resources within the group
		std::map<std::string, Group*>::iterator itg = mmapGroup.find(strGroupName);

		// For each resource in this group...
		std::map<std::string, Resource*>::iterator it = itg->second->mmapResource.begin();
		while (it != itg->second->mmapResource.end())
		{
			if (!it->second->bLoaded)
			{
				it->second->pResource->load();
				it->second->bLoaded = true;
			}
			it++;
		}
	}

	void TextureManager::loadGroupSingle(const std::string& strGroupName)
	{
		// Group doesn't exist?
		if (!groupExists(strGroupName))
		{
			std::string err("TextureManager::loadGroupSingle(\"");
			err.append(strGroupName);
			err.append("\") failed. As the given named group doesn't exist.");
			ThrowIfTrue(1, err);
		}

		// Load any unloaded resources within the group
		std::map<std::string, Group*>::iterator itg = mmapGroup.find(strGroupName);

		// For each resource in this group...
		std::map<std::string, Resource*>::iterator it = itg->second->mmapResource.begin();
		while (it != itg->second->mmapResource.end())
		{
			if (!it->second->bLoaded)
			{
				it->second->pResource->load();
				it->second->bLoaded = true;
				return;	// We've changed a resource from unloaded to loaded state, aka, we've loaded a resource, simply return.
			}
			it++;
		}
	}

	void TextureManager::unloadGroup(const std::string& strGroupName)
	{
		// Group doesn't exist?
		if (!groupExists(strGroupName))
		{
			std::string err("TextureManager::unloadGroup(\"");
			err.append(strGroupName);
			err.append("\") failed. As the given named group doesn't exist.");
			ThrowIfTrue(1, err);
		}

		// Unload any loaded resources within the group
		std::map<std::string, Group*>::iterator itg = mmapGroup.find(strGroupName);

		// For each resource in this group...
		std::map<std::string, Resource*>::iterator it = itg->second->mmapResource.begin();
		while (it != itg->second->mmapResource.end())
		{
			if (it->second->bLoaded)
			{
				it->second->pResource->unload();
				it->second->bLoaded = false;
			}
			it++;
		}
	}

	Texture* TextureManager::add(const std::string& strNewResourceName, const std::string& strTextureFilename, const std::string& strGroupName)
	{
		// Group doesn't exist?
		if (!groupExists(strGroupName))
		{
			std::string err("TextureManager::add(\"");
			err.append(strNewResourceName);
			err.append("\", \"");
			err.append(strGroupName);
			err.append("\") failed. As the given named group of \"");
			err.append(strGroupName);
			err.append("\" which the new resource was to be placed into, doesn't exist.");
			ThrowIfTrue(1, err);
		}

		// Resource already exists in the group?
		std::map<std::string, Group*>::iterator itg = mmapGroup.find(strGroupName);								// Get iterator to the group (we know it exists)
		std::map<std::string, Resource*>::iterator it = itg->second->mmapResource.find(strNewResourceName);		// Try to find the named resource in the group
		if (itg->second->mmapResource.end() != it)
		{
			it->second->uiReferenceCount++;
			return it->second->pResource;
		}

		// If we get here, we have got to create, then add the resource to the existing named group
		Resource* pNewRes = new Resource;
		pNewRes->pResource = new Texture(strTextureFilename);
		pNewRes->uiReferenceCount = 1;
		pNewRes->bLoaded = false;
		itg->second->mmapResource[strNewResourceName] = pNewRes;
		return pNewRes->pResource;
	}

	Texture* TextureManager::get(const std::string& strResourceName, const std::string& strGroupName)
	{
		// Group doesn't exist?
		if (!groupExists(strGroupName))
		{
			std::string err("TextureManager::get(\"");
			err.append(strResourceName);
			err.append("\", \"");
			err.append(strGroupName);
			err.append("\") failed. As the given named group of \"");
			err.append(strGroupName);
			err.append("\" doesn't exist.");
			ThrowIfTrue(1, err);
		}

		// Resource doesn't exist in the group?
		std::map<std::string, Group*>::iterator itg = mmapGroup.find(strGroupName);							// Get iterator to the group (we know it exists)
		std::map<std::string, Resource*>::iterator it = itg->second->mmapResource.find(strResourceName);	// Try to find the named resource in the group
		if (itg->second->mmapResource.end() == it)
		{
			std::string err("TextureManager::get(\"");
			err.append(strResourceName);
			err.append("\", \"");
			err.append(strGroupName);
			err.append("\") failed. Although the given named group of \"");
			err.append(strGroupName);
			err.append("\" exists, the named resource couldn't be found.");
			ThrowIfTrue(1, err);
		}

		// Is the resource in an unloaded state?
		if (!it->second->bLoaded)
		{
			// Load it
			it->second->pResource->load();
			it->second->bLoaded = true;
		}
		// Return the resource pointer...
		return it->second->pResource;
	}

	bool TextureManager::getExists(const std::string& strResourceName, const std::string& strGroupName)
	{
		std::map<std::string, Group*>::iterator itg = mmapGroup.find(strGroupName);
		if (itg == mmapGroup.end())
			return false;
		std::map<std::string, Resource*>::iterator it = itg->second->mmapResource.find(strResourceName);
		if (it == itg->second->mmapResource.end())
			return false;
		return true;
	}

	void TextureManager::remove(const std::string& strResourceName, const std::string& strGroupName)
	{
		// Group doesn't exist?
		if (!groupExists(strGroupName))
		{
			std::string err("TextureManager::remove(\"");
			err.append(strResourceName);
			err.append("\", \"");
			err.append(strGroupName);
			err.append("\") failed. As the given named group of \"");
			err.append(strGroupName);
			err.append("\" which the resource is supposed to be in, doesn't exist.");
			ThrowIfTrue(1, err);
		}

		// Resource doesn't exist in the group?
		std::map<std::string, Group*>::iterator itg = mmapGroup.find(strGroupName);							// Get iterator to the group (we know it exists)
		std::map<std::string, Resource*>::iterator it = itg->second->mmapResource.find(strResourceName);	// Try to find the named resource in the group
		if (itg->second->mmapResource.end() == it)
		{
			std::string err("TextureManager::remove(\"");
			err.append(strResourceName);
			err.append("\", \"");
			err.append(strGroupName);
			err.append("\") failed. Although the given named group of \"");
			err.append(strGroupName);
			err.append("\" which the resource is supposed to be in, exists, the named resource couldn't be found.");
			ThrowIfTrue(1, err);
		}

		// If we get here, we've found the resource in the group
		// Decrement it's reference count 
		it->second->uiReferenceCount--;
		// If the reference count is now at zero
		if (it->second->uiReferenceCount <= 0)
		{
			// If it's in a loaded state, unload it
			if (it->second->bLoaded)
			{
				it->second->pResource->unload();
				it->second->bLoaded = false;
			}

			// Destroy the resource
			delete it->second->pResource;
			delete it->second;
			itg->second->mmapResource.erase(it);
		}
	}

	void TextureManager::_onOpenGLContextLost(void)
	{
		// For each group
		std::map<std::string, Group*>::iterator itg = mmapGroup.begin();
		while (itg != mmapGroup.end())
		{
			// For each resource in group
			std::map<std::string, Resource*>::iterator it = itg->second->mmapResource.begin();
			while (it != itg->second->mmapResource.end())
			{
				// Store whether the resource is loaded or not
				it->second->bLoadedContextLostBackup = it->second->bLoaded;
				it++;
			}
			itg++;
		}

		// Now that we've backup whether the resource is in the loaded state or not, unload everything
		unsigned int iNumGroups = getNumGroups();
		for (unsigned int iGroupIndex = 0; iGroupIndex < iNumGroups; iGroupIndex++)
		{
			const std::string& strGroupName = getGroupName(iGroupIndex);
			unloadGroup(strGroupName);
		}

		// Done.
	}

	void TextureManager::_onOpenGLContextRestored(void)
	{
		// Now that the context has been lost, we need to reload all the unloaded resources back in.
		// Hopefully a call to _onOpenGLContextLost() has been previously called to backup all the resources which were previously loaded.
		// Now we go through each group and each resource in each group and reload if they were previously...
		// For each group
		std::map<std::string, Group*>::iterator itg = mmapGroup.begin();
		while (itg != mmapGroup.end())
		{
			// For each resource in group
			std::map<std::string, Resource*>::iterator it = itg->second->mmapResource.begin();
			while (it != itg->second->mmapResource.end())
			{
				// Was it previously loaded?
				if (it->second->bLoadedContextLostBackup)
				{
					it->second->pResource->load();
					it->second->bLoaded = true;
				}
				it++;
			}
			itg++;
		}
	}
}