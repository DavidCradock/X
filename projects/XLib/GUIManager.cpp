#include "PCH.h"
#include "GUIManager.h"
#include "log.h"

namespace X
{
	GUIManager::GUIManager()
	{
		_mfScale = 1.0f;
	}

	void GUIManager::setScale(float fScalingValue)
	{
		_mfScale = fScalingValue;
	}

	float GUIManager::getScale(void)
	{
		return _mfScale;
	}

	GUITheme* GUIManager::addTheme(const std::string& strName)
	{
		// Attempt to find by name
		std::map<std::string, GUITheme*>::iterator it = _mmapThemes.find(strName);
		// If it already exists, return a pointer to that.
		if (it != _mmapThemes.end())
			return it->second;

		// Allocate the new theme
		GUITheme* pNew = new GUITheme;
		ThrowIfFalse(pNew, "GUIManager::addTheme(" + strName + ") failed. Unable to allocate memory for the new theme.");

		// Place in the hashmap
		_mmapThemes[strName] = pNew;
		return pNew;
	}

	GUITheme* GUIManager::getTheme(const std::string& strName)
	{
		std::map<std::string, GUITheme*>::iterator it = _mmapThemes.find(strName);
		ThrowIfTrue(it == _mmapThemes.end(), "GUIManager::getTheme(" + strName + ") failed. Theme doesn't exist.");
		return it->second;
	}

	bool GUIManager::getThemeExists(const std::string& strName)
	{
		std::map<std::string, GUITheme*>::iterator it = _mmapThemes.find(strName);
		return it != _mmapThemes.end();
	}

	void GUIManager::removeTheme(const std::string& strName)
	{
		std::map<std::string, GUITheme*>::iterator it = _mmapThemes.find(strName);
		if (it == _mmapThemes.end())
			return;	// Doesn't exist.
		delete it->second;
		_mmapThemes.erase(it);
	}

	void GUIManager::removeAllThemes(void)
	{
		std::map<std::string, GUITheme*>::iterator it = _mmapThemes.begin();
		while (it != _mmapThemes.end())
		{
			delete it->second;
			it++;
		}
		_mmapThemes.clear();
	}

	int GUIManager::getNumberOfThemes(void)
	{
		return (int)_mmapThemes.size();
	}

	std::string GUIManager::getThemeName(int iIndex)
	{
		ThrowIfTrue(iIndex < 0 || iIndex >= (int)_mmapThemes.size(), "GUIManager::getThemeName() failed. Invalid index of " + std::to_string(iIndex) + " was given.");
		std::map<std::string, GUITheme*>::iterator it = _mmapThemes.begin();
		int iCount = 0;
		while (iCount < iIndex)
			it++;
		return it->first;
	}

	void GUIManager::setThemeForAll(const std::string& strName)
	{
		ThrowIfTrue(1, "GUIManager::setThemeForAll() failed. This method isn't implemented yet!");
	}


	GUIContainer* GUIManager::addContainer(const std::string& strName)
	{
		// Attempt to find by name
		std::map<std::string, GUIContainer*>::iterator it = _mmapContainers.find(strName);
		// If it already exists, return a pointer to that.
		if (it != _mmapContainers.end())
			return it->second;

		// Allocate the new theme
		GUIContainer* pNew = new GUIContainer;
		ThrowIfFalse(pNew, "GUIManager::addContainer(" + strName + ") failed. Unable to allocate memory for the new container.");

		// Place in the hashmap
		_mmapContainers[strName] = pNew;
		return pNew;
	}

	GUIContainer* GUIManager::getContainer(const std::string& strName)
	{
		std::map<std::string, GUIContainer*>::iterator it = _mmapContainers.find(strName);
		ThrowIfTrue(it == _mmapContainers.end(), "GUIManager::getContainer(" + strName + ") failed. Container doesn't exist.");
		return it->second;
	}

	bool GUIManager::getContainerExists(const std::string& strName)
	{
		std::map<std::string, GUIContainer*>::iterator it = _mmapContainers.find(strName);
		return it != _mmapContainers.end();
	}

	void GUIManager::removeContainer(const std::string& strName)
	{
		std::map<std::string, GUIContainer*>::iterator it = _mmapContainers.find(strName);
		if (it == _mmapContainers.end())
			return;	// Doesn't exist.
		delete it->second;
		_mmapContainers.erase(it);
	}

	void GUIManager::removeAllContainers(void)
	{
		std::map<std::string, GUIContainer*>::iterator it = _mmapContainers.begin();
		while (it != _mmapContainers.end())
		{
			delete it->second;
			it++;
		}
		_mmapContainers.clear();
	}

	int GUIManager::getNumberOfContainers(void)
	{
		return (int)_mmapContainers.size();
	}

	std::string GUIManager::getContainerName(int iIndex)
	{
		ThrowIfTrue(iIndex < 0 || iIndex >= (int)_mmapContainers.size(), "GUIManager::getContainerName() failed. Invalid index of " + std::to_string(iIndex) + " was given.");
		std::map<std::string, GUIContainer*>::iterator it = _mmapContainers.begin();
		int iCount = 0;
		while (iCount < iIndex)
			it++;
		return it->first;
	}
}