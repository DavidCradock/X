#include "PCH.h"
#include "2DEntityComplex.h"
#include "log.h"
#include "2DLayer.h"

namespace X
{
	C2DEntityComplex::C2DEntityComplex()
	{
		_mfParentRotationDegrees = 0.0;
	}

	C2DEntityComplex::~C2DEntityComplex()
	{
		removeAllLayers();
	}

	void C2DEntityComplex::render(void)
	{
		SCResourceManager* pRM = SCResourceManager::getPointer();
		CResourceTriangle* pTri = pRM->getTriangle("X:default");
		pTri->removeGeom();

		// For each layer in this EntityComplex object
		for (unsigned int uiLayerZorder = 0; uiLayerZorder < _mvecLayerNameZOrder.size(); ++uiLayerZorder)
		{
			// Get layers, in z order , starting with the one at the back
			C2DLayerComplex* pLayer = _mmapLayers[_mvecLayerNameZOrder[uiLayerZorder]];

			// For each entity in layer
			unsigned int uiPreviouslyBoundAtlasImageNumber = 999999;	// Used to reduce rebinding of same atlas texture
			std::string strPreviouslyBoundAtlasName;					// Used to reduce rebinding of same atlas texture
			std::map<std::string, C2DEntity*>::iterator itEntity = pLayer->_mmapEntities.begin();
			CVector2f v2fPos;	// Used by to compute actual entity position based on scale
			CVector2f v2fDims;	// Used by to compute actual entity position based on scale
			while (itEntity != pLayer->_mmapEntities.end())
			{
				// Make sure the images have been set, if not, throw an exception
				ThrowIfFalse(itEntity->second->_mbImagesAreSet, "C2DEntityComplex::render() failed. Entity: " + itEntity->first + " has not had either setImagesSingle() or setImagesMultiple() called.");

				// Bind correct atlas texture if not previously bound
				bool bBindTexture = false;
				if (strPreviouslyBoundAtlasName != itEntity->second->_mstrResourceTexture2DAtlasName)
				{
					strPreviouslyBoundAtlasName = itEntity->second->_mstrResourceTexture2DAtlasName;
					bBindTexture = true;
				}
				if (uiPreviouslyBoundAtlasImageNumber != itEntity->second->_mvImageDetails[itEntity->second->_muiCurrentFrameNumber].uiAtlasImage)
				{
					uiPreviouslyBoundAtlasImageNumber = itEntity->second->_mvImageDetails[itEntity->second->_muiCurrentFrameNumber].uiAtlasImage;
					bBindTexture = true;
				}
				if (bBindTexture)
				{
					// Get CResourceTexture2DAtlas the entity is set to use
					CResourceTexture2DAtlas* pAtlas = pRM->getTexture2DAtlas(itEntity->second->_mstrResourceTexture2DAtlasName);
					// Bind the atlas texture containing the entity's currently set frame number's image
					pAtlas->bindAtlas(0, itEntity->second->_mvImageDetails[itEntity->second->_muiCurrentFrameNumber].uiAtlasImage);
				}

				// Now get the entity's frame information for texture coordinates and entity position and add to CResourceTriangle* pTri
				// But before doing that, we need to send any existing data to be rendered IF the atlas texture has changed
				if (bBindTexture)	// If the atlas texture has changed
				{
					pTri->update();
					pTri->draw(false);
					pTri->removeGeom();
				}

				// Get position and dimensions of entity
				v2fPos = itEntity->second->_mv2fPosition;
				v2fDims = itEntity->second->_mvImageDetails[itEntity->second->_muiCurrentFrameNumber].v2fDimensions * itEntity->second->_mv2fScale.x;

				// If the entity is NOT the "PARENT" entity, then we need to set it's position, rotation, offset from the parent
				if (itEntity->first != "PARENT")
				{
					// Get parent entity
					C2DEntity* pParentEntity = _findParentEntity();

					// Rotate entity position based upon PARENT rotation
					v2fPos.rotate(-_mfParentRotationDegrees);

					// Offset position of entity by PARENT entity's position	
					v2fPos += pParentEntity->_mv2fPosition;
					v2fPos -= v2fDims * 0.5f;
				}
				else  // Is the "PARENT" entity
				{
					// Offset position from top left to centre of entity
					v2fPos -= v2fDims * 0.5f;
				}

				pTri->addQuad2D(
					v2fPos,															// Position
					v2fDims,														// Dimensions
					itEntity->second->_mColour,										// Colour
					itEntity->second->_mvImageDetails[itEntity->second->_muiCurrentFrameNumber].sTexCoords.bottom_left,		// Texture coordinates
					itEntity->second->_mvImageDetails[itEntity->second->_muiCurrentFrameNumber].sTexCoords.bottom_right,	// Texture coordinates
					itEntity->second->_mvImageDetails[itEntity->second->_muiCurrentFrameNumber].sTexCoords.top_right,		// Texture coordinates
					itEntity->second->_mvImageDetails[itEntity->second->_muiCurrentFrameNumber].sTexCoords.top_left);		// Texture coordinates
				itEntity++;
			}
			// Send remaining vertex data to GPU to be rendered
			pTri->update();
			pTri->draw(false);
			pTri->removeGeom();

		}	// Each layer
	}

	C2DLayerComplex* C2DEntityComplex::addLayer(const std::string& strUniqueName)
	{
		// Attempt to find if the object name already exists
		std::map<std::string, C2DLayerComplex*>::iterator it = _mmapLayers.find(strUniqueName);
		ThrowIfTrue(it != _mmapLayers.end(), "C2DEntityComplex::addLayer(\"" + strUniqueName + "\") failed. The object already exists.");

		// Allocate memory for new object
		C2DLayerComplex* pNew = new C2DLayerComplex;
		ThrowIfFalse(pNew, "C2DEntityComplex::addLayer(\"" + strUniqueName + "\") failed. Unable to allocate memory.");

		// Add object to hash map
		_mmapLayers[strUniqueName] = pNew;

		// Add object name to z order vector
		_mvecLayerNameZOrder.push_back(strUniqueName);

		return pNew;
	}

	bool C2DEntityComplex::getLayerExists(const std::string& strUniqueName) const
	{
		if (_mmapLayers.find(strUniqueName) == _mmapLayers.end())
			return false;
		return true;
	}

	C2DLayerComplex* C2DEntityComplex::getLayer(const std::string& strUniqueName) const
	{
		// Attempt to find if the layer name already exists
		std::map<std::string, C2DLayerComplex*>::iterator it = _mmapLayers.find(strUniqueName);
		ThrowIfTrue(it == _mmapLayers.end(), "C2DEntityComplex::getLayer(\"" + strUniqueName + "\") failed. Object name doesn't exist!");
		return it->second;
	}

	C2DLayerComplex* C2DEntityComplex::getLayer(unsigned int uiIndex) const
	{
		// Make sure given index is valid
		ThrowIfTrue(uiIndex >= _mmapLayers.size(), "C2DEntityComplex::getLayer(" + std::to_string(uiIndex) + ") failed. Invalid index given.");
		std::map<std::string, C2DLayerComplex*>::iterator it = _mmapLayers.begin();
		unsigned int ui = 0;
		while (ui < uiIndex)
		{
			ui++;
			it++;
		}
		return it->second;
	}

	void C2DEntityComplex::removeLayer(const std::string& strUniqueName)
	{
		// Attempt to find if the name already exists
		std::map<std::string, C2DLayerComplex*>::iterator it = _mmapLayers.find(strUniqueName);
		ThrowIfTrue(it == _mmapLayers.end(), "C2DEntityComplex::removeLayer(\"" + strUniqueName + "\") failed. The name doesn't exist.");

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

	void C2DEntityComplex::removeLayer(unsigned int uiIndex)
	{
		// Make sure given index is valid
		ThrowIfTrue(uiIndex >= _mmapLayers.size(), "C2DEntityComplex::removeLayer(" + std::to_string(uiIndex) + ") failed. Invalid index given.");
		std::map<std::string, C2DLayerComplex*>::iterator it = _mmapLayers.begin();
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

	void C2DEntityComplex::removeAllLayers(void)
	{
		// Remove all names from z order vector
		_mvecLayerNameZOrder.clear();

		// Remove all layers
		std::map<std::string, C2DLayerComplex*>::iterator it = _mmapLayers.begin();
		while (it != _mmapLayers.end())
		{
			delete it->second;
			_mmapLayers.erase(it);
			it = _mmapLayers.begin();
		}
	}

	unsigned int C2DEntityComplex::getNumLayers(void) const
	{
		return unsigned int(_mmapLayers.size());
	}

	std::string C2DEntityComplex::getLayerNameAtZorder(unsigned int uiZorder) const
	{
		// Make sure valid index given
		ThrowIfTrue(uiZorder >= (unsigned int)_mvecLayerNameZOrder.size(), "C2DEntityComplex::getLayerNameAtZorder(" + std::to_string(uiZorder) + ") failed. Invalid iZorder value given.");
		return _mvecLayerNameZOrder[uiZorder];
	}

	std::string C2DEntityComplex::getLayerNameAtIndex(unsigned int uiIndex) const
	{
		// Make sure valid index given
		ThrowIfTrue(uiIndex >= (unsigned int)_mmapLayers.size(), "C2DEntityComplex::getLayerNameAtIndex(" + std::to_string(uiIndex) + ") failed. Invalid index value given.");
		std::map<std::string, C2DLayerComplex*>::iterator it = _mmapLayers.begin();
		unsigned int ui = 0;
		while (ui < uiIndex)
		{
			ui++;
			it++;
		}
		return it->first;
	}

	int C2DEntityComplex::getLayerZorder(const std::string& strLayerName) const
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
		ThrowIfTrue(-1 == iCurrentIndex, "C2DEntityComplex::getLayerZorder(\"" + strLayerName + "\") failed. The given name couldn't be found.");
		return iCurrentIndex;
	}

	void C2DEntityComplex::moveLayerToFrontByOne(const std::string& strLayerName)
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
		ThrowIfTrue(-1 == iCurrentIndex, "C2DEntityComplex::moveLayerUpOne(\"" + strLayerName + "\") failed. The given name couldn't be found.");

		// If it's already at the front, do nothing
		if ((int)_mvecLayerNameZOrder.size() - 1 == iCurrentIndex)
			return;

		// Move it to front by one...
		_mvecLayerNameZOrder[iCurrentIndex] = _mvecLayerNameZOrder[iCurrentIndex + 1];
		_mvecLayerNameZOrder[iCurrentIndex + 1] = strLayerName;
	}

	void C2DEntityComplex::moveLayerToBackByOne(const std::string& strLayerName)
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
		ThrowIfTrue(-1 == iCurrentIndex, "C2DEntityComplex::moveLayerToBackByOne(\"" + strLayerName + "\") failed. The given name couldn't be found.");

		// If it's already at the back, do nothing
		if (0 == iCurrentIndex)
			return;

		// Move it back one...
		_mvecLayerNameZOrder[iCurrentIndex] = _mvecLayerNameZOrder[iCurrentIndex - 1];
		_mvecLayerNameZOrder[iCurrentIndex - 1] = strLayerName;
	}

	void C2DEntityComplex::moveLayerToBack(const std::string& strLayerName)
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
		ThrowIfFalse(bFound, "C2DEntityComplex::moveLayerToBack(\"" + strLayerName + "\") failed. The given name couldn't be found.");
		while (getLayerZorder(strLayerName) != 0)
		{
			moveLayerToBackByOne(strLayerName);
		}
	}

	void C2DEntityComplex::moveLayerToFront(const std::string& strLayerName)
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
		ThrowIfFalse(bFound, "C2DEntityComplex::moveLayerToFront(\"" + strLayerName + "\") failed. The given name couldn't be found.");
		while (getLayerZorder(strLayerName) != getNumLayers() - 1)
		{
			moveLayerToFrontByOne(strLayerName);
		}
	}

	void C2DEntityComplex::moveLayerBehind(const std::string& strLayerName, const std::string& strLayerNameOther)
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
		ThrowIfFalse(bFound, "C2DEntityComplex::moveLayerBehind(\"" + strLayerName + "\", \"" + strLayerNameOther + "\") failed. The given name couldn't be found. (" + strLayerName + ")");
		bFound = false;
		for (int i = 0; i < (int)_mvecLayerNameZOrder.size(); ++i)
		{
			if (_mvecLayerNameZOrder[i] == strLayerNameOther)
			{
				bFound = true;
				break;
			}
		}
		ThrowIfFalse(bFound, "C2DEntityComplex::moveLayerBehind(\"" + strLayerName + "\", \"" + strLayerNameOther + "\") failed. The given name couldn't be found. (" + strLayerNameOther + ")");

		// If this layer is before other, then move until it's just before
		while (getLayerZorder(strLayerName) < getLayerZorder(strLayerNameOther))
			moveLayerToFrontByOne(strLayerName);

		// If this layer is after other, then move until it's just before
		while (getLayerZorder(strLayerName) > getLayerZorder(strLayerNameOther))
			moveLayerToBackByOne(strLayerName);
	}

	void C2DEntityComplex::moveLayerInfront(const std::string& strLayerName, const std::string& strLayerNameOther)
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
		ThrowIfFalse(bFound, "C2DEntityComplex::moveLayerInfront(\"" + strLayerName + "\", \"" + strLayerNameOther + "\") failed. The given name couldn't be found. (" + strLayerName + ")");
		bFound = false;
		for (int i = 0; i < (int)_mvecLayerNameZOrder.size(); ++i)
		{
			if (_mvecLayerNameZOrder[i] == strLayerNameOther)
			{
				bFound = true;
				break;
			}
		}
		ThrowIfFalse(bFound, "C2DEntityComplex::moveLayerInfront(\"" + strLayerName + "\", \"" + strLayerNameOther + "\") failed. The given name couldn't be found. (" + strLayerNameOther + ")");

		// If this layer is after other, then move until it's just before
		while (getLayerZorder(strLayerName) > getLayerZorder(strLayerNameOther))
			moveLayerToBackByOne(strLayerName);

		// If this layer is before other, then move until it's just before
		while (getLayerZorder(strLayerName) < getLayerZorder(strLayerNameOther))
			moveLayerToFrontByOne(strLayerName);
	}

	C2DEntity* C2DEntityComplex::_findParentEntity(void) const
	{
		C2DEntity* pParentEntity = 0;
		// For each layer
		std::map<std::string, C2DLayerComplex*>::iterator it = _mmapLayers.begin();
		while (it != _mmapLayers.end())
		{
			if (it->second->getEntityExists("PARENT"))
			{
				pParentEntity = it->second->getEntity("PARENT");
				break;
			}
			it++;
		}

		ThrowIfFalse(pParentEntity, "C2DEntityComplex::_findParentEntity() failed to find the entity named \"PARENT\" in any of the layers.");
		return pParentEntity;
	}

	void C2DEntityComplex::setPosition(const CVector2f& vPosition)
	{
		C2DEntity* pParentEntity = _findParentEntity();
		pParentEntity->setPosition(vPosition);
	}

	CVector2f C2DEntityComplex::getPosition(void) const
	{
		C2DEntity* pParentEntity = _findParentEntity();
		return pParentEntity->getPosition();
	}

	void C2DEntityComplex::setScale(float fScaleX, float fScaleY)
	{
		C2DEntity* pParentEntity = _findParentEntity();
		pParentEntity->setScale(fScaleX, fScaleY);	
	}

	CVector2f C2DEntityComplex::getScale(void) const
	{
		C2DEntity* pParentEntity = _findParentEntity();
		return pParentEntity->getScale();
	}

	void C2DEntityComplex::setColour(const CColour& colour)
	{
		C2DEntity* pParentEntity = _findParentEntity();
		pParentEntity->setColour(colour);
	}

	CColour C2DEntityComplex::getColour(void) const
	{
		C2DEntity* pParentEntity = _findParentEntity();
		return pParentEntity->getColour();
	}

	void C2DEntityComplex::setImagesSingle(const std::string& strImageFilenameInAtlas)
	{
		C2DEntity* pParentEntity = _findParentEntity();

		// Make sure the set atlas exists
		SCResourceManager* pRM = SCResourceManager::getPointer();
		ThrowIfFalse(pRM->getTexture2DAtlasExists(pParentEntity->_mstrResourceTexture2DAtlasName), "C2DEntityComplex::setSingleImage() failed. The entity's currently set C2DTextureAtlas resource of " + pParentEntity->_mstrResourceTexture2DAtlasName + " couldn't be found.");
		// Make sure the named image exists within the atlas
		CResourceTexture2DAtlas* pAtlas = pRM->getTexture2DAtlas(pParentEntity->_mstrResourceTexture2DAtlasName);
		ThrowIfFalse(pAtlas->getImageNameExists(strImageFilenameInAtlas), "C2DEntityComplex::setSingleImage() failed. The named image of " + strImageFilenameInAtlas + " does not exist within the entity's currently set texture atlas resource.");

		// If we get here, everything exists, set the entity to use the image...
		pParentEntity->_mbUseAnimationFrames = false;
		pParentEntity->_mvstrImageNames.clear();
		pParentEntity->_mvstrImageNames.push_back(strImageFilenameInAtlas);
		pParentEntity->_mvImageDetails.clear();
		pParentEntity->_mvImageDetails.push_back(pAtlas->getImageDetails(strImageFilenameInAtlas));
		pParentEntity->_muiCurrentFrameNumber = 0;
		pParentEntity->_mbImagesAreSet = true;
	}

	void C2DEntityComplex::setImagesMultiple(const std::vector<std::string>& strImageFilenamesInAtlas)
	{
		C2DEntity* pParentEntity = _findParentEntity();

		// Make sure the set atlas exists
		SCResourceManager* pRM = SCResourceManager::getPointer();
		ThrowIfFalse(pRM->getTexture2DAtlasExists(pParentEntity->_mstrResourceTexture2DAtlasName), "C2DEntityComplex::setImagesMultiple() failed. The entity's currently set C2DTextureAtlas resource of " + pParentEntity->_mstrResourceTexture2DAtlasName + " couldn't be found.");
		CResourceTexture2DAtlas* pAtlas = pRM->getTexture2DAtlas(pParentEntity->_mstrResourceTexture2DAtlasName);

		pParentEntity->_mbUseAnimationFrames = true;
		pParentEntity->_mvstrImageNames.clear();	// Clear stored image names
		pParentEntity->_mvImageDetails.clear();
		pParentEntity->_muiCurrentFrameNumber = 0;
		pParentEntity->_mbImagesAreSet = true;
		for (unsigned int ui = 0; ui < strImageFilenamesInAtlas.size(); ui++)
		{
			// Make sure the named images exist within the atlas
			ThrowIfFalse(pAtlas->getImageNameExists(strImageFilenamesInAtlas[ui]), "C2DEntityComplex::setImagesMultiple() failed. The named image of " + strImageFilenamesInAtlas[ui] + " does not exist within the entity's currently set texture atlas resource.");
			// Add the image name to this entity's vector
			pParentEntity->_mvstrImageNames.push_back(strImageFilenamesInAtlas[ui]);

			// Store each image's details here so we don't have to look this up each program loop
			pParentEntity->_mvImageDetails.push_back(pAtlas->getImageDetails(strImageFilenamesInAtlas[ui]));
		}
	}

	unsigned int C2DEntityComplex::getCurrentFrameNumber(void) const
	{
		C2DEntity* pParentEntity = _findParentEntity();
		return pParentEntity->_muiCurrentFrameNumber;
	}

	void C2DEntityComplex::setCurrentFrameNumber(unsigned int uiFrameNumber)
	{
		C2DEntity* pParentEntity = _findParentEntity();

		ThrowIfTrue(uiFrameNumber > pParentEntity->_mvstrImageNames.size(), "C2DEntityComplex::setCurrentFrameNumber() given invalid frame number.");
		pParentEntity->_muiCurrentFrameNumber = uiFrameNumber;
	}

	unsigned int C2DEntityComplex::getNumFrames(void) const
	{
		C2DEntity* pParentEntity = _findParentEntity();
		return (unsigned int)pParentEntity->_mvstrImageNames.size();
	}

	void C2DEntityComplex::setFrameBasedOnAngle(float fRotationDegrees)
	{
		C2DEntity* pParentEntity = _findParentEntity();
		clamp(fRotationDegrees, 0.0f, 359.9999f);
		float fTemp = fRotationDegrees / 360.0f;	// Zero to one
		pParentEntity->_muiCurrentFrameNumber = unsigned int(fTemp * float(pParentEntity->_mvstrImageNames.size()));
		ThrowIfTrue(pParentEntity->_muiCurrentFrameNumber >= (unsigned int)pParentEntity->_mvstrImageNames.size(), "C2DEntityComplex::setFrameBasedOnAngle() computed wrong frame number.");
	}

	void C2DEntityComplex::setFrameBasedOnDirection(CVector2f vDir)
	{
		C2DEntity* pParentEntity = _findParentEntity();
		float fDegrees = vDir.getAngleDegrees360();
		setFrameBasedOnAngle(fDegrees);
	}

	void C2DEntityComplex::setRotation(float fRotationDegrees)
	{
		if (fRotationDegrees > 360)
			fRotationDegrees = 360;
		else if (fRotationDegrees < 0.0)
			fRotationDegrees = 0.0;
		_mfParentRotationDegrees = fRotationDegrees;
	}

	void C2DEntityComplex::setRotation(CVector2f vDirection)
	{
		_mfParentRotationDegrees = vDirection.getAngleDegrees360();
	}
}