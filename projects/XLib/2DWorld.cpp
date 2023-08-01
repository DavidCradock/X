#include "PCH.h"
#include "2DWorld.h"
#include "log.h"

namespace X
{
	C2DWorld::C2DWorld()
	{
		_mbVisible = true;
	}

	C2DWorld::~C2DWorld()
	{
		removeAllCameras();
		removeAllLayers();
	}

	C2DLayer* C2DWorld::addLayer(const std::string& strUniqueName)
	{
		// Attempt to find if the object name already exists
		std::map<std::string, C2DLayer*>::iterator it = _mmapLayers.find(strUniqueName);
		ThrowIfTrue(it != _mmapLayers.end(), "C2DWorld::addLayer(\"" + strUniqueName + "\") failed. The object already exists.");

		// Allocate memory for new object
		C2DLayer* pNew = new C2DLayer;
		ThrowIfFalse(pNew, "C2DWorld::addLayer(\"" + strUniqueName + "\") failed. Unable to allocate memory.");

		// Add object to hash map
		_mmapLayers[strUniqueName] = pNew;

		// Add object name to z order vector
		_mvecLayerNameZOrder.push_back(strUniqueName);

		return pNew;
	}

	bool C2DWorld::getLayerExists(const std::string& strUniqueName) const
	{
		if (_mmapLayers.find(strUniqueName) == _mmapLayers.end())
			return false;
		return true;
	}

	C2DLayer* C2DWorld::getLayer(const std::string& strUniqueName) const
	{
		// Attempt to find if the layer name already exists
		std::map<std::string, C2DLayer*>::iterator it = _mmapLayers.find(strUniqueName);
		ThrowIfTrue(it == _mmapLayers.end(), "C2DWorld::getLayer(\"" + strUniqueName + "\") failed. Object name doesn't exist!");
		return it->second;
	}

	C2DLayer* C2DWorld::getLayer(unsigned int uiIndex) const
	{
		// Make sure given index is valid
		ThrowIfTrue(uiIndex >= _mmapLayers.size(), "C2DWorld::getLayer(" + std::to_string(uiIndex) + ") failed. Invalid index given.");
		std::map<std::string, C2DLayer*>::iterator it = _mmapLayers.begin();
		unsigned int ui = 0;
		while (ui < uiIndex)
		{
			ui++;
			it++;
		}
		return it->second;
	}

	void C2DWorld::removeLayer(const std::string& strUniqueName)
	{
		// Attempt to find if the name already exists
		std::map<std::string, C2DLayer*>::iterator it = _mmapLayers.find(strUniqueName);
		ThrowIfTrue(it == _mmapLayers.end(), "C2DWorld::removeLayer(\"" + strUniqueName + "\") failed. The name doesn't exist.");

		// De-allocate memory for the layer
		delete it->second;

		// Remove layer from hash map
		_mmapLayers.erase(it);

		// Remove layer name from z order vector
		std::vector<std::string>::iterator itv = _mvecLayerNameZOrder.begin();
		for (int i = 0; i < (int)_mvecLayerNameZOrder.size(); ++i)
		{
			if (_mvecLayerNameZOrder[i] == strUniqueName)
			{
				itv = _mvecLayerNameZOrder.erase(itv);
				break;
			}
			itv++;
		}
	}

	void C2DWorld::removeLayer(unsigned int uiIndex)
	{
		// Make sure given index is valid
		ThrowIfTrue(uiIndex >= _mmapLayers.size(), "C2DWorld::removeLayer(" + std::to_string(uiIndex) + ") failed. Invalid index given.");
		std::map<std::string, C2DLayer*>::iterator it = _mmapLayers.begin();
		unsigned int ui = 0;
		while (ui < uiIndex)
		{
			ui++;
			it++;
		}
		// Temporarily store name (For removal from zorder vector)
		std::string strIndexName = it->first;

		// De-allocate memory for the object
		delete it->second;

		// Remove object from hash map
		_mmapLayers.erase(it);

		// Remove layer name from z order vector
		std::vector<std::string>::iterator itv = _mvecLayerNameZOrder.begin();
		for (int i = 0; i < (int)_mvecLayerNameZOrder.size(); ++i)
		{
			if (_mvecLayerNameZOrder[i] == strIndexName)
			{
				itv = _mvecLayerNameZOrder.erase(itv);
				break;
			}
			itv++;
		}
	}

	void C2DWorld::removeAllLayers(void)
	{
		// Remove all names from z order vector
		_mvecLayerNameZOrder.clear();

		// Remove all layers
		std::map<std::string, C2DLayer*>::iterator it = _mmapLayers.begin();
		while (it != _mmapLayers.end())
		{
			delete it->second;
			_mmapLayers.erase(it);
			it = _mmapLayers.begin();
		}
	}

	unsigned int C2DWorld::getNumLayers(void) const
	{
		return unsigned int(_mmapLayers.size());
	}

	std::string C2DWorld::getLayerNameAtZorder(unsigned int uiZorder) const
	{
		// Make sure valid index given
		ThrowIfTrue(uiZorder >= (unsigned int)_mvecLayerNameZOrder.size(), "C2DWorld::getLayerNameAtZorder(" + std::to_string(uiZorder) + ") failed. Invalid iZorder value given.");
		return _mvecLayerNameZOrder[uiZorder];
	}

	std::string C2DWorld::getLayerNameAtIndex(unsigned int uiIndex) const
	{
		// Make sure valid index given
		ThrowIfTrue(uiIndex >= (unsigned int)_mmapLayers.size(), "C2DWorld::getLayerNameAtIndex(" + std::to_string(uiIndex) + ") failed. Invalid index value given.");
		std::map<std::string, C2DLayer*>::iterator it = _mmapLayers.begin();
		unsigned int ui = 0;
		while (ui < uiIndex)
		{
			ui++;
			it++;
		}
		return it->first;
	}

	int C2DWorld::getLayerZorder(const std::string& strLayerName) const
	{
		// Find current position of the named layer
		int iCurrentIndex = -1;
		for (int i = 0; i < (int)_mvecLayerNameZOrder.size(); ++i)
		{
			if (_mvecLayerNameZOrder[i] == strLayerName)
			{
				iCurrentIndex = i;
				break;
			}
		}

		// If the layer couldn't be found
		ThrowIfTrue(-1 == iCurrentIndex, "C2DWorld::getLayerZorder(\"" + strLayerName + "\") failed. The given name couldn't be found.");
		return iCurrentIndex;
	}

	void C2DWorld::moveLayerToFrontByOne(const std::string& strLayerName)
	{
		// Find current position of the named layer
		int iCurrentIndex = -1;
		for (int i = 0; i < (int)_mvecLayerNameZOrder.size(); ++i)
		{
			if (_mvecLayerNameZOrder[i] == strLayerName)
			{
				iCurrentIndex = i;
				break;
			}
		}

		// If the layer couldn't be found
		ThrowIfTrue(-1 == iCurrentIndex, "C2DWorld::moveLayerUpOne(\"" + strLayerName + "\") failed. The given name couldn't be found.");

		// If it's already at the front, do nothing
		if ((int)_mvecLayerNameZOrder.size() - 1 == iCurrentIndex)
			return;

		// Move it to front by one...
		_mvecLayerNameZOrder[iCurrentIndex] = _mvecLayerNameZOrder[iCurrentIndex + 1];
		_mvecLayerNameZOrder[iCurrentIndex + 1] = strLayerName;
	}

	void C2DWorld::moveLayerToBackByOne(const std::string& strLayerName)
	{
		// Find current position of the named layer
		int iCurrentIndex = -1;
		for (int i = 0; i < (int)_mvecLayerNameZOrder.size(); ++i)
		{
			if (_mvecLayerNameZOrder[i] == strLayerName)
			{
				iCurrentIndex = i;
				break;
			}
		}

		// If the layer couldn't be found
		ThrowIfTrue(-1 == iCurrentIndex, "C2DWorld::moveLayerToBackByOne(\"" + strLayerName +  "\") failed. The given name couldn't be found.");

		// If it's already at the back, do nothing
		if (0 == iCurrentIndex)
			return;

		// Move it back one...
		_mvecLayerNameZOrder[iCurrentIndex] = _mvecLayerNameZOrder[iCurrentIndex - 1];
		_mvecLayerNameZOrder[iCurrentIndex - 1] = strLayerName;
	}

	void C2DWorld::moveLayerToBack(const std::string& strLayerName)
	{
		// Make sure the layer name exists
		bool bFound = false;
		for (int i = 0; i < (int)_mvecLayerNameZOrder.size(); ++i)
		{
			if (_mvecLayerNameZOrder[i] == strLayerName)
			{
				bFound = true;
				break;
			}
		}
		ThrowIfFalse(bFound, "C2DWorld::moveLayerToBack(\"" + strLayerName + "\") failed. The given name couldn't be found.");
		while (getLayerZorder(strLayerName) != 0)
		{
			moveLayerToBackByOne(strLayerName);
		}
	}

	void C2DWorld::moveLayerToFront(const std::string& strLayerName)
	{
		// Make sure the layer name exists
		bool bFound = false;
		for (int i = 0; i < (int)_mvecLayerNameZOrder.size(); ++i)
		{
			if (_mvecLayerNameZOrder[i] == strLayerName)
			{
				bFound = true;
				break;
			}
		}
		ThrowIfFalse(bFound, "C2DWorld::moveLayerToFront(\"" + strLayerName + "\") failed. The given name couldn't be found.");
		while (getLayerZorder(strLayerName) != getNumLayers() - 1)
		{
			moveLayerToFrontByOne(strLayerName);
		}
	}

	void C2DWorld::moveLayerBehind(const std::string& strLayerName, const std::string& strLayerNameOther)
	{
		// Make sure both layer names exist
		bool bFound = false;
		for (int i = 0; i < (int)_mvecLayerNameZOrder.size(); ++i)
		{
			if (_mvecLayerNameZOrder[i] == strLayerName)
			{
				bFound = true;
				break;
			}
		}
		ThrowIfFalse(bFound, "C2DWorld::moveLayerBehind(\"" + strLayerName + "\", \"" + strLayerNameOther + "\") failed. The given name couldn't be found. (" + strLayerName + ")");
		bFound = false;
		for (int i = 0; i < (int)_mvecLayerNameZOrder.size(); ++i)
		{
			if (_mvecLayerNameZOrder[i] == strLayerNameOther)
			{
				bFound = true;
				break;
			}
		}
		ThrowIfFalse(bFound, "C2DWorld::moveLayerBehind(\"" + strLayerName + "\", \"" + strLayerNameOther + "\") failed. The given name couldn't be found. (" + strLayerNameOther + ")");

		// If this layer is before other, then move until it's just before
		while (getLayerZorder(strLayerName) < getLayerZorder(strLayerNameOther))
			moveLayerToFrontByOne(strLayerName);

		// If this layer is after other, then move until it's just before
		while (getLayerZorder(strLayerName) > getLayerZorder(strLayerNameOther))
			moveLayerToBackByOne(strLayerName);
	}

	void C2DWorld::moveLayerInfront(const std::string& strLayerName, const std::string& strLayerNameOther)
	{
		// Make sure both layer names exist
		bool bFound = false;
		for (int i = 0; i < (int)_mvecLayerNameZOrder.size(); ++i)
		{
			if (_mvecLayerNameZOrder[i] == strLayerName)
			{
				bFound = true;
				break;
			}
		}
		ThrowIfFalse(bFound, "C2DWorld::moveLayerInfront(\"" + strLayerName + "\", \"" + strLayerNameOther + "\") failed. The given name couldn't be found. (" + strLayerName + ")");
		bFound = false;
		for (int i = 0; i < (int)_mvecLayerNameZOrder.size(); ++i)
		{
			if (_mvecLayerNameZOrder[i] == strLayerNameOther)
			{
				bFound = true;
				break;
			}
		}
		ThrowIfFalse(bFound, "C2DWorld::moveLayerInfront(\"" + strLayerName + "\", \"" + strLayerNameOther + "\") failed. The given name couldn't be found. (" + strLayerNameOther + ")");

		// If this layer is after other, then move until it's just before
		while (getLayerZorder(strLayerName) > getLayerZorder(strLayerNameOther))
			moveLayerToBackByOne(strLayerName);

		// If this layer is before other, then move until it's just before
		while (getLayerZorder(strLayerName) < getLayerZorder(strLayerNameOther))
			moveLayerToFrontByOne(strLayerName);
	}

	C2DCamera* C2DWorld::addCamera(const std::string& strUniqueName)
	{
		// Attempt to find if the object name already exists
		std::map<std::string, C2DCamera*>::iterator it = _mmapCameras.find(strUniqueName);
		ThrowIfTrue(it != _mmapCameras.end(), "C2DWorld::addCamera(\"" + strUniqueName + "\") failed. The object already exists.");

		// Allocate memory for new object
		C2DCamera* pNew = new C2DCamera;
		ThrowIfFalse(pNew, "C2DWorld::addCamera(\"" + strUniqueName + "\") failed. Unable to allocate memory.");

		// Add object to hash map
		_mmapCameras[strUniqueName] = pNew;
		return pNew;
	}

	bool C2DWorld::getCameraExists(const std::string& strUniqueName) const
	{
		if (_mmapCameras.find(strUniqueName) == _mmapCameras.end())
			return false;
		return true;
	}

	C2DCamera* C2DWorld::getCamera(const std::string& strUniqueName) const
	{
		// Attempt to find if the name already exists
		std::map<std::string, C2DCamera*>::iterator it = _mmapCameras.find(strUniqueName);
		ThrowIfTrue(it == _mmapCameras.end(), "C2DWorld::getCamera(\"" + strUniqueName + "\") failed. Object name doesn't exist.");
		return it->second;
	}

	C2DCamera* C2DWorld::getCamera(unsigned int uiIndex) const
	{
		// Make sure given index is valid
		ThrowIfTrue(uiIndex >= _mmapCameras.size(), "C2DWorld::getCamera(" + std::to_string(uiIndex) + ") failed. Invalid index given.");
		std::map<std::string, C2DCamera*>::iterator it = _mmapCameras.begin();
		unsigned int ui = 0;
		while (ui < uiIndex)
		{
			ui++;
			it++;
		}
		return it->second;
	}

	void C2DWorld::removeCamera(const std::string& strUniqueName)
	{
		// Attempt to find if the layer name already exists
		std::map<std::string, C2DCamera*>::iterator it = _mmapCameras.find(strUniqueName);
		ThrowIfTrue(it == _mmapCameras.end(), "C2DWorld::removeCamera(\"" + strUniqueName + "\") failed. The object doesn't exist.");

		// De-allocate memory
		delete it->second;

		// Remove from hash map
		_mmapCameras.erase(it);
	}

	void C2DWorld::removeCamera(unsigned int uiIndex)
	{
		// Make sure given index is valid
		ThrowIfTrue(uiIndex >= _mmapCameras.size(), "C2DWorld::removeCamera(" + std::to_string(uiIndex) + ") failed. Invalid index given.");
		std::map<std::string, C2DCamera*>::iterator it = _mmapCameras.begin();
		unsigned int ui = 0;
		while (ui < uiIndex)
		{
			ui++;
			it++;
		}
		// De-allocate memory for the object
		delete it->second;

		// Remove object from hash map
		_mmapCameras.erase(it);
	}

	void C2DWorld::removeAllCameras(void)
	{
		// Remove all objects
		std::map<std::string, C2DCamera*>::iterator it = _mmapCameras.begin();
		while (it != _mmapCameras.end())
		{
			delete it->second;
			_mmapCameras.erase(it);
			it = _mmapCameras.begin();
		}
	}

	int C2DWorld::getNumCameras(void) const
	{
		return int(_mmapCameras.size());
	}

	void C2DWorld::setVisible(bool bVisible)
	{
		_mbVisible = bVisible;
	}

	bool C2DWorld::getVisible(void) const
	{
		return _mbVisible;
	}
}