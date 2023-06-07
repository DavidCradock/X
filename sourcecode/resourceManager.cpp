#include "PCH.h"
#include "resourceManager.h"
#include "log.h"

namespace X
{
	void ResourceManager::onGLContextToBeDestroyed(void)
	{
		// For each of the resources, call their onGLContextToBeDestroyed() methods.

		// Framebuffers
		std::map<std::string, ResourceFramebuffer*>::iterator itFramebuffer = _mmapResFramebuffers.begin();
		while (itFramebuffer != _mmapResFramebuffers.end())
		{
			itFramebuffer->second->onGLContextToBeDestroyed();
			itFramebuffer++;
		}

		// Shaders
		std::map<std::string, ResourceShader*>::iterator itShaders = _mmapResShaders.begin();
		while (itShaders != _mmapResShaders.end())
		{
			itShaders->second->onGLContextToBeDestroyed();
			itShaders++;
		}

		// Texture2Ds
		std::map<std::string, ResourceTexture2D*>::iterator itTextures2D = _mmapResTextures2D.begin();
		while (itTextures2D != _mmapResTextures2D.end())
		{
			itTextures2D->second->onGLContextToBeDestroyed();
			itTextures2D++;
		}

		// Vertexbuffers
		std::map<std::string, ResourceVertexbuffer*>::iterator itVertexbuffer = _mmapResVertexbuffers.begin();
		while (itVertexbuffer != _mmapResVertexbuffers.end())
		{
			itVertexbuffer->second->onGLContextToBeDestroyed();
			itVertexbuffer++;
		}
	}

	void ResourceManager::onGLContextRecreated(void)
	{
		// For each of the resources, call their onGLContextCreated() methods.

		// Framebuffers
		std::map<std::string, ResourceFramebuffer*>::iterator itFramebuffer = _mmapResFramebuffers.begin();
		while (itFramebuffer != _mmapResFramebuffers.end())
		{
			itFramebuffer->second->onGLContextCreated();
			itFramebuffer++;
		}

		// Shaders
		std::map<std::string, ResourceShader*>::iterator itShaders = _mmapResShaders.begin();
		while (itShaders != _mmapResShaders.end())
		{
			itShaders->second->onGLContextCreated();
			itShaders++;
		}

		// Texture 2Ds
		std::map<std::string, ResourceTexture2D*>::iterator itTextures2D = _mmapResTextures2D.begin();
		while (itTextures2D != _mmapResTextures2D.end())
		{
			itTextures2D->second->onGLContextCreated();
			itTextures2D++;
		}

		// Vertexbuffers
		std::map<std::string, ResourceVertexbuffer*>::iterator itVertexbuffer = _mmapResVertexbuffers.begin();
		while (itVertexbuffer != _mmapResVertexbuffers.end())
		{
			itVertexbuffer->second->onGLContextCreated();
			itVertexbuffer++;
		}
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

	ResourceVertexbuffer* ResourceManager::addVertexbuffer(const std::string& strResourceName)
	{
		ResourceVertexbuffer* pNewResource = new ResourceVertexbuffer();
		ThrowIfFalse(pNewResource, "ResourceManager::addVertexbuffer(" + strResourceName + ") failed to allocate memory for new resource.");
		_mmapResVertexbuffers[strResourceName] = pNewResource;
		return pNewResource;
	}

	ResourceVertexbuffer* ResourceManager::getVertexbuffer(const std::string& strResourceName)
	{
		std::map<std::string, ResourceVertexbuffer*>::iterator it = _mmapResVertexbuffers.find(strResourceName);
		ThrowIfTrue(it == _mmapResVertexbuffers.end(), "ResourceManager::getVertexbuffer(" + strResourceName + ") failed. Named resource doesn't exist.");
		return it->second;
	}

	bool ResourceManager::getVertexbufferExists(const std::string& strResourceName)
	{
		return _mmapResVertexbuffers.find(strResourceName) != _mmapResVertexbuffers.end();
	}

	void ResourceManager::removeVertexbuffer(const std::string& strResourceName)
	{
		std::map<std::string, ResourceVertexbuffer*>::iterator it = _mmapResVertexbuffers.find(strResourceName);
		if (it == _mmapResVertexbuffers.end())
			return;	// Doesn't exist.
		delete it->second;
		_mmapResVertexbuffers.erase(it);
	}

}