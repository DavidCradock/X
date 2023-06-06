#include "PCH.h"
#include "resourceManager.h"
#include "log.h"

namespace X
{
	void ResourceManager::onGLContextToBeDestroyed(void)
	{
		// For each of the resources, call their onGLContextToBeDestroyed() methods.

		// Shaders
		std::map<std::string, ResourceShader*>::iterator itShaders = _mmapResShaders.begin();
		while (itShaders != _mmapResShaders.end())
		{
			itShaders->second->onGLContextToBeDestroyed();
			itShaders++;
		}

		// Texture2D
		std::map<std::string, ResourceTexture2D*>::iterator itTextures2D = _mmapResTextures2D.begin();
		while (itTextures2D != _mmapResTextures2D.end())
		{
			itTextures2D->second->onGLContextToBeDestroyed();
			itTextures2D++;
		}

		// Framebuffer
		std::map<std::string, ResourceFramebuffer*>::iterator itFramebuffer = _mmapResFramebuffers.begin();
		while (itFramebuffer != _mmapResFramebuffers.end())
		{
			itShaders->second->onGLContextToBeDestroyed();
			itFramebuffer++;
		}
	}

	
	void ResourceManager::onGLContextRecreated(void)
	{
		// For each of the resources, call their onGLContextCreated() methods.
		std::map<std::string, ResourceShader*>::iterator itShaders = _mmapResShaders.begin();
		while (itShaders != _mmapResShaders.end())
		{
			itShaders->second->onGLContextCreated();
			itShaders++;
		}

		std::map<std::string, ResourceTexture2D*>::iterator itTextures2D = _mmapResTextures2D.begin();
		while (itTextures2D != _mmapResTextures2D.end())
		{
			itTextures2D->second->onGLContextCreated();
			itTextures2D++;
		}

		// Framebuffer
		std::map<std::string, ResourceFramebuffer*>::iterator itFramebuffer = _mmapResFramebuffers.begin();
		while (itFramebuffer != _mmapResFramebuffers.end())
		{
			itShaders->second->onGLContextCreated();
			itFramebuffer++;
		}
	}

	ResourceShader* ResourceManager::addShader(const std::string& strResourceName, const std::string& strVertexProgramFilename, const std::string& strFragmentProgramFilename)
	{
		ResourceShader* pNewResource = new ResourceShader(strVertexProgramFilename, strFragmentProgramFilename);
		ThrowIfFalse(pNewResource, "ResourceManager::addShader(" + strResourceName + ", " + strVertexProgramFilename + ", " + strFragmentProgramFilename + ") failed to allocate memory for new resource.");
		_mmapResShaders[strResourceName] = pNewResource;
		return pNewResource;
	}

	ResourceShader* ResourceManager::getShader(const std::string& strResourceName)
	{
		std::map<std::string, ResourceShader*>::iterator it = _mmapResShaders.find(strResourceName);
		ThrowIfTrue(it == _mmapResShaders.end(), "ResourceManager::getShader(" + strResourceName + ") failed. Named resource doesn't exist.");
		return it->second;
	}

	bool ResourceManager::getShaderExists(const std::string& strResourceName)
	{
		return _mmapResShaders.find(strResourceName) != _mmapResShaders.end();
	}

	void ResourceManager::removeShader(const std::string& strResourceName)
	{
		std::map<std::string, ResourceShader*>::iterator it = _mmapResShaders.find(strResourceName);
		if (it == _mmapResShaders.end())
			return;	// Doesn't exist.
		delete it->second;
		_mmapResShaders.erase(it);
	}

	ResourceTexture2D* ResourceManager::addTexture2D(const std::string& strResourceName, const std::string& strImageFilename)
	{
		ResourceTexture2D* pNewResource = new ResourceTexture2D(strImageFilename);
		ThrowIfFalse(pNewResource, "ResourceManager::addTexture2D(" + strResourceName + ", " + strImageFilename + ") failed to allocate memory for new resource.");
		_mmapResTextures2D[strResourceName] = pNewResource;
		return pNewResource;
	}

	ResourceTexture2D* ResourceManager::getTexture2D(const std::string& strResourceName)
	{
		std::map<std::string, ResourceTexture2D*>::iterator it = _mmapResTextures2D.find(strResourceName);
		ThrowIfTrue(it == _mmapResTextures2D.end(), "ResourceManager::getTexture2D(" + strResourceName + ") failed. Named resource doesn't exist.");
		return it->second;
	}

	bool ResourceManager::getTexture2DExists(const std::string& strResourceName)
	{
		return _mmapResTextures2D.find(strResourceName) != _mmapResTextures2D.end();
	}

	void ResourceManager::removeTexture2D(const std::string& strResourceName)
	{
		std::map<std::string, ResourceTexture2D*>::iterator it = _mmapResTextures2D.find(strResourceName);
		if (it == _mmapResTextures2D.end())
			return;	// Doesn't exist.
		delete it->second;
		_mmapResTextures2D.erase(it);
	}

	ResourceFramebuffer* ResourceManager::addFramebuffer(const std::string& strResourceName, unsigned int uiWidth, unsigned int uiHeight)
	{
		ResourceFramebuffer* pNewResource = new ResourceFramebuffer(uiWidth, uiHeight);
		ThrowIfFalse(pNewResource, "ResourceManager::addFramebuffer(" + strResourceName + ") failed to allocate memory for new resource.");
		_mmapResFramebuffers[strResourceName] = pNewResource;
		return pNewResource;
	}

	ResourceFramebuffer* ResourceManager::getFramebuffer(const std::string& strResourceName)
	{
		std::map<std::string, ResourceFramebuffer*>::iterator it = _mmapResFramebuffers.find(strResourceName);
		ThrowIfTrue(it == _mmapResFramebuffers.end(), "ResourceManager::getFramebuffer(" + strResourceName + ") failed. Named resource doesn't exist.");
		return it->second;
	}

	bool ResourceManager::getFramebufferExists(const std::string& strResourceName)
	{
		return _mmapResFramebuffers.find(strResourceName) != _mmapResFramebuffers.end();
	}

	void ResourceManager::removeFramebuffer(const std::string& strResourceName)
	{
		std::map<std::string, ResourceFramebuffer*>::iterator it = _mmapResFramebuffers.find(strResourceName);
		if (it == _mmapResFramebuffers.end())
			return;	// Doesn't exist.
		delete it->second;
		_mmapResFramebuffers.erase(it);
	}

}