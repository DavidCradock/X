#include "PCH.h"
#include "2DWorld.h"
#include "log.h"

namespace X
{
	C2DWorld::C2DWorld()
	{

	}

	C2DWorld::~C2DWorld()
	{

	}

	C2DLayer* C2DWorld::addLayer(const std::string& strUniqueName)
	{
		// Attempt to find if the layer name already exists
		std::map<std::string, C2DLayer*>::iterator itlayer = _mmapLayers.find(strUniqueName);
		if (itlayer != _mmapLayers.end())
		{
			std::string err("C2DWorld::addLayer(\"");
			err.append(strUniqueName);
			err.append("\") failed. The layer already exists.");
			ThrowIfTrue(1, err);
		}

		C2DLayer* pNewLayer = new C2DLayer;
		if (!pNewLayer)
			ThrowIfTrue(1, "Memory allocation error.");

		// Add layer to hash map
		_mmapLayers[strUniqueName] = pNewLayer;

		// Add layer name to z order vector
		_mvecLayerNameZOrder.push_back(strUniqueName);

		return pNewLayer;
	}

	bool C2DWorld::layerExists(const std::string& strUniqueName)
	{
		if (_mmapLayers.find(strUniqueName) == _mmapLayers.end())
			return false;
		return true;
	}
	C2DLayer* C2DWorld::getLayer(const std::string& strUniqueName)
	{
		// Attempt to find if the layer name already exists
		std::map<std::string, C2DLayer*>::iterator itlayer = _mmapLayers.find(strUniqueName);
		if (itlayer == _mmapLayers.end())
		{
			std::string err("C2DWorld::getLayer(\"");
			err.append(strUniqueName);
			err.append("\") failed. Layer name doesn't exist!");
			ThrowIfTrue(1, err);
		}
		return itlayer->second;
	}

	void C2DWorld::removeLayer(const std::string& strUniqueName)
	{
		// Attempt to find if the layer name already exists
		std::map<std::string, C2DLayer*>::iterator itlayer = _mmapLayers.find(strUniqueName);
		if (itlayer == _mmapLayers.end())
		{
			std::string err("C2DWorld::removeLayer(\"");
			err.append(strUniqueName);
			err.append("\") failed. The layer doesn't exist.");
			ThrowIfTrue(1, err);
		}

		C2DLayer* pLayer = itlayer->second;
		// De-allocate memory for the layer
		delete pLayer;

		// Remove layer from hash map
		itlayer = _mmapLayers.erase(itlayer);

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

	void C2DWorld::removeAllLayers(void)
	{
		// Get names of each layer
		std::vector<std::string> vNames;
		std::map<std::string, C2DLayer*>::iterator itlayer = _mmapLayers.begin();
		while (itlayer != _mmapLayers.end())
		{
			std::string name = itlayer->first;
			vNames.push_back(name);
			itlayer++;
		}
		for (int i = 0; i < (int)vNames.size(); ++i)
		{
			removeLayer(vNames[i]);
		}
	}

	std::string C2DWorld::getLayerName(int iZorder)
	{
		// Make sure valid index given
		if (iZorder < 0 || iZorder >= (int)_mvecLayerNameZOrder.size())
		{
			std::string err("C2DWorld::getLayerName(");
			err += std::to_string(iZorder);
			err.append(") failed. Invalid iZorder value given!");
			ThrowIfTrue(1, err);
		}
		return _mvecLayerNameZOrder[iZorder];
	}

	int C2DWorld::getLayerZorder(const std::string& strLayerName)
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
			++i;
		}

		// If the layer couldn't be found
		if (-1 == iCurrentIndex)
		{
			std::string err("C2DWorld::getLayerZorder(\"");
			err.append(strLayerName);
			err.append("\") failed. The given layer name couldn't be found.");
			ThrowIfTrue(1, err);
		}
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
			++i;
		}

		// If the layer couldn't be found
		if (-1 == iCurrentIndex)
		{
			std::string err("C2DWorld::moveLayerUpOne(\"");
			err.append(strLayerName);
			err.append("\") failed. The given layer name couldn't be found.");
			ThrowIfTrue(1, err);
		}

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
			++i;
		}

		// If the layer couldn't be found
		if (-1 == iCurrentIndex)
		{
			std::string err("C2DWorld::moveLayerToBackByOne(\"");
			err.append(strLayerName);
			err.append("\") failed. The given layer name couldn't be found.");
			ThrowIfTrue(1, err);
		}

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
		if (!bFound)
		{
			std::string err("C2DWorld::moveLayerToBack(\"");
			err.append(strLayerName);
			err.append("\") failed. The given layer name couldn't be found.");
			ThrowIfTrue(1, err);
		}
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
		if (!bFound)
		{
			std::string err("C2DWorld::moveLayerToFront(\"");
			err.append(strLayerName);
			err.append("\") failed. The given layer name couldn't be found.");
			ThrowIfTrue(1, err);
		}
		while (getLayerZorder(strLayerName) != getLayerCount() - 1)
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
		if (!bFound)
		{
			std::string err("C2DWorld::moveLayerBehind(\"");
			err.append(strLayerName);
			err.append("\", \"");
			err.append(strLayerNameOther);
			err.append("\") failed. The given layer name couldn't be found. (");
			err.append(strLayerName);
			err.append(")");
			ThrowIfTrue(1, err);
		}
		bFound = false;
		for (int i = 0; i < (int)_mvecLayerNameZOrder.size(); ++i)
		{
			if (_mvecLayerNameZOrder[i] == strLayerNameOther)
			{
				bFound = true;
				break;
			}
		}
		if (!bFound)
		{
			std::string err("C2DWorld::moveLayerBehind(\"");
			err.append(strLayerName);
			err.append("\", \"");
			err.append(strLayerNameOther);
			err.append("\") failed. The given layer name couldn't be found. (");
			err.append(strLayerNameOther);
			err.append(")");
			ThrowIfTrue(1, err);
		}

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
		if (!bFound)
		{
			std::string err("C2DWorld::moveLayerInfront(\"");
			err.append(strLayerName);
			err.append("\", \"");
			err.append(strLayerNameOther);
			err.append("\") failed. The given layer name couldn't be found. (");
			err.append(strLayerName);
			err.append(")");
			ThrowIfTrue(1, err);
		}
		bFound = false;
		for (int i = 0; i < (int)_mvecLayerNameZOrder.size(); ++i)
		{
			if (_mvecLayerNameZOrder[i] == strLayerNameOther)
			{
				bFound = true;
				break;
			}
		}
		if (!bFound)
		{
			std::string err("C2DWorld::moveLayerInfront(\"");
			err.append(strLayerName);
			err.append("\", \"");
			err.append(strLayerNameOther);
			err.append("\") failed. The given layer name couldn't be found. (");
			err.append(strLayerNameOther);
			err.append(")");
			ThrowIfTrue(1, err);
		}

		// If this layer is after other, then move until it's just before
		while (getLayerZorder(strLayerName) > getLayerZorder(strLayerNameOther))
			moveLayerToBackByOne(strLayerName);

		// If this layer is before other, then move until it's just before
		while (getLayerZorder(strLayerName) < getLayerZorder(strLayerNameOther))
			moveLayerToFrontByOne(strLayerName);
	}
}