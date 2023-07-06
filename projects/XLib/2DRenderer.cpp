#include "PCH.h"
#include "2DRenderer.h"
#include "log.h"

namespace X
{
	SC2DRenderer::SC2DRenderer()
	{

	}

	SC2DRenderer::~SC2DRenderer()
	{
		removeAllWorlds();
	}

	C2DWorld* SC2DRenderer::addWorld(const std::string& strUniqueName)
	{
		// Attempt to find if the object name already exists
		std::map<std::string, C2DWorld*>::iterator it = _mmapWorlds.find(strUniqueName);
		ThrowIfTrue(it != _mmapWorlds.end(), "SC2DRenderer::addWorld(\"" + strUniqueName + "\") failed. The object already exists.");

		// Allocate memory for new object
		C2DWorld* pNew = new C2DWorld;
		ThrowIfFalse(pNew, "SC2DRenderer::addWorld(\"" + strUniqueName + "\") failed. Unable to allocate memory.");

		// Add object to hash map
		_mmapWorlds[strUniqueName] = pNew;
		return pNew;
	}

	bool SC2DRenderer::getWorldExists(const std::string& strUniqueName)
	{
		if (_mmapWorlds.find(strUniqueName) == _mmapWorlds.end())
			return false;
		return true;
	}

	C2DWorld* SC2DRenderer::getWorld(const std::string& strUniqueName)
	{
		// Attempt to find if the world name already exists
		std::map<std::string, C2DWorld*>::iterator it = _mmapWorlds.find(strUniqueName);
		ThrowIfTrue(it == _mmapWorlds.end(), "SC2DRenderer::getWorld(\"" + strUniqueName + "\") failed. Object name doesn't exist!");
		return it->second;
	}

	C2DWorld* SC2DRenderer::getWorld(unsigned int uiIndex)
	{
		// Make sure given index is valid
		ThrowIfTrue(uiIndex >= _mmapWorlds.size(), "SC2DRenderer::getWorld(" + std::to_string(uiIndex) + ") failed. Invalid index given.");
		std::map<std::string, C2DWorld*>::iterator it = _mmapWorlds.begin();
		unsigned int ui = 0;
		while (ui < uiIndex)
		{
			ui++;
			it++;
		}
		return it->second;
	}

	void SC2DRenderer::removeWorld(const std::string& strUniqueName)
	{
		// Attempt to find if the name already exists
		std::map<std::string, C2DWorld*>::iterator it = _mmapWorlds.find(strUniqueName);
		ThrowIfTrue(it == _mmapWorlds.end(), "SC2DRenderer::removeWorld(\"" + strUniqueName + "\") failed. The object doesn't exist.");

		// De-allocate memory for the object
		delete it->second;

		// Remove object from hash map
		_mmapWorlds.erase(it);
	}

	void SC2DRenderer::removeWorld(unsigned int uiIndex)
	{
		// Make sure given index is valid
		ThrowIfTrue(uiIndex >= _mmapWorlds.size(), "SC2DRenderer::removeWorld(" + std::to_string(uiIndex) + ") failed. Invalid index given.");
		std::map<std::string, C2DWorld*>::iterator it = _mmapWorlds.begin();
		unsigned int ui = 0;
		while (ui < uiIndex)
		{
			ui++;
			it++;
		}
		// De-allocate memory for the object
		delete it->second;

		// Remove object from hash map
		_mmapWorlds.erase(it);
	}

	void SC2DRenderer::removeAllWorlds(void)
	{
		// Remove all layers
		std::map<std::string, C2DWorld*>::iterator it = _mmapWorlds.begin();
		while (it != _mmapWorlds.end())
		{
			delete it->second;
			_mmapWorlds.erase(it);
			it = _mmapWorlds.begin();
		}
	}

	int SC2DRenderer::getNumWorlds(void)
	{
		return int(_mmapWorlds.size());
	}
}