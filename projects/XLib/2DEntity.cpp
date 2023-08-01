#include "PCH.h"
#include "2DEntity.h"
#include "log.h"

namespace X
{
	C2DEntity::C2DEntity(const std::string& strResourceTexture2DAtlasName)
	{
		_mstrResourceTexture2DAtlasName = strResourceTexture2DAtlasName;
		_muiCurrentFrameNumber = 0;
		_mv2fPosition.setZero();
		_mv2fScale.set(1, 1);
		_mColour.set(1.0f, 1.0f, 1.0f, 1.0f);
		_mbImagesAreSet = false;
		_mbVisible = true;
	}

	C2DEntity::~C2DEntity()
	{

	}

	void C2DEntity::setPosition(const CVector2f& vPosition)
	{
		_mv2fPosition = vPosition;
	}

	CVector2f C2DEntity::getPosition(void) const
	{
		return _mv2fPosition;
	}

	void C2DEntity::setScale(float fScaleX, float fScaleY)
	{
		_mv2fScale.set(fScaleX, fScaleY);
	}

	CVector2f C2DEntity::getScale(void) const
	{
		return _mv2fScale;
	}

	void C2DEntity::setColour(const CColour& colour)
	{
		_mColour = colour;
	}

	CColour C2DEntity::getColour(void) const
	{
		return _mColour;
	}

	void C2DEntity::setImagesSingle(const std::string& strImageFilenameInAtlas)
	{
		// Make sure the set atlas exists
		SCResourceManager* pRM = SCResourceManager::getPointer();
		ThrowIfFalse(pRM->getTexture2DAtlasExists(_mstrResourceTexture2DAtlasName), "C2DEntity::setSingleImage() failed. The entity's currently set C2DTextureAtlas resource of " + _mstrResourceTexture2DAtlasName + " couldn't be found.");
		// Make sure the named image exists within the atlas
		CResourceTexture2DAtlas* pAtlas = pRM->getTexture2DAtlas(_mstrResourceTexture2DAtlasName);
		ThrowIfFalse(pAtlas->getImageNameExists(strImageFilenameInAtlas), "C2DEntity::setSingleImage() failed. The named image of " + strImageFilenameInAtlas + " does not exist within the entity's currently set texture atlas resource.");

		// If we get here, everything exists, set the entity to use the image...
		_mbUseAnimationFrames = false;
		_mvstrImageNames.clear();
		_mvstrImageNames.push_back(strImageFilenameInAtlas);
		_mvImageDetails.clear();
		_mvImageDetails.push_back(pAtlas->getImageDetails(strImageFilenameInAtlas));
		_muiCurrentFrameNumber = 0;
		_mbImagesAreSet = true;
	}

	void C2DEntity::setImagesMultiple(const std::vector<std::string>& strImageFilenamesInAtlas)
	{
		// Make sure the set atlas exists
		SCResourceManager* pRM = SCResourceManager::getPointer();
		ThrowIfFalse(pRM->getTexture2DAtlasExists(_mstrResourceTexture2DAtlasName), "C2DEntity::setImagesMultiple() failed. The entity's currently set C2DTextureAtlas resource of " + _mstrResourceTexture2DAtlasName + " couldn't be found.");
		CResourceTexture2DAtlas* pAtlas = pRM->getTexture2DAtlas(_mstrResourceTexture2DAtlasName);

		_mbUseAnimationFrames = true;
		_mvstrImageNames.clear();	// Clear stored image names
		_mvImageDetails.clear();
		_muiCurrentFrameNumber = 0;
		_mbImagesAreSet = true;
		for (unsigned int ui = 0; ui < strImageFilenamesInAtlas.size(); ui++)
		{
			// Make sure the named images exist within the atlas
			ThrowIfFalse(pAtlas->getImageNameExists(strImageFilenamesInAtlas[ui]), "C2DEntity::setImagesMultiple() failed. The named image of " + strImageFilenamesInAtlas[ui] + " does not exist within the entity's currently set texture atlas resource.");
			// Add the image name to this entity's vector
			_mvstrImageNames.push_back(strImageFilenamesInAtlas[ui]);

			// Store each image's details here so we don't have to look this up each program loop
			_mvImageDetails.push_back(pAtlas->getImageDetails(strImageFilenamesInAtlas[ui]));
		}
	}

	unsigned int C2DEntity::getCurrentFrameNumber(void) const
	{
		return _muiCurrentFrameNumber;
	}

	void C2DEntity::setCurrentFrameNumber(unsigned int uiFrameNumber)
	{
		ThrowIfTrue(uiFrameNumber > _mvstrImageNames.size(), "C2DEntity::setCurrentFrameNumber() given invalid frame number.");
		_muiCurrentFrameNumber = uiFrameNumber;
	}

	unsigned int C2DEntity::getNumFrames(void) const
	{
		return (unsigned int)_mvstrImageNames.size();
	}

	void C2DEntity::setFrameBasedOnAngle(float fRotationDegrees)
	{
		clamp(fRotationDegrees, 0.0f, 359.9999f);
		float fTemp = fRotationDegrees / 360.0f;	// Zero to one
		_muiCurrentFrameNumber = unsigned int(fTemp * float(_mvstrImageNames.size()));
		ThrowIfTrue(_muiCurrentFrameNumber >= (unsigned int)_mvstrImageNames.size(), "C2DEntity::setFrameBasedOnAngle() computed wrong frame number.");
	}

	void C2DEntity::setFrameBasedOnDirection(CVector2f vDir)
	{
		float fDegrees = vDir.getAngleDegrees360();
		setFrameBasedOnAngle(fDegrees);
	}

	void C2DEntity::setVisible(bool bVisible)
	{
		_mbVisible = bVisible;
	}

	bool C2DEntity::getVisible(void) const
	{
		return _mbVisible;
	}

	void C2DEntity::render(
		std::string& strPreviouslyBoundAtlasName,
		unsigned int& uiPreviouslyBoundAtlasImageNumber,
		CResourceTriangle* pTri,
		unsigned int& uiNumTextureBindingsPerLoop)
	{
		// If this entity is set as invisible, do nuffin'
		if (!_mbVisible)
			return;

		// Make sure the images this entity uses have been set, if not, throw an exception
		ThrowIfFalse(_mbImagesAreSet, "C2DEntity::render() failed. Entity has not had either setImagesSingle() or setImagesMultiple() called.");

		// Determine whether texture atlas has changed since rendering of any previous entity
		bool bNeedToBindTexture = false;
		if (strPreviouslyBoundAtlasName != _mstrResourceTexture2DAtlasName)
		{
			strPreviouslyBoundAtlasName = _mstrResourceTexture2DAtlasName;
			bNeedToBindTexture = true;
		}
		// Determine whether texture atlas's texture number has changed since rendering of any previous entity as texture atlases can have multiple textures
		if (uiPreviouslyBoundAtlasImageNumber != _mvImageDetails[_muiCurrentFrameNumber].uiAtlasImage)
		{
			uiPreviouslyBoundAtlasImageNumber = _mvImageDetails[_muiCurrentFrameNumber].uiAtlasImage;
			bNeedToBindTexture = true;
		}

		// We need to send any existing vertex data to be rendered IF the atlas texture is to be changed
		if (bNeedToBindTexture)	// If the atlas texture has changed
		{
			pTri->update();
			pTri->draw(false);
			pTri->removeGeom();
		}

		// Bind the texture AFTER rendering the above vertices, otherwise the above vertices will use the new texture
		if (bNeedToBindTexture)
		{
			SCResourceManager* pRM = SCResourceManager::getPointer();
			// Get CResourceTexture2DAtlas the entity is set to use
			CResourceTexture2DAtlas* pAtlas = pRM->getTexture2DAtlas(_mstrResourceTexture2DAtlasName);
			// Bind the atlas texture containing the entity's currently set frame number's image
			pAtlas->bindAtlas(0, _mvImageDetails[_muiCurrentFrameNumber].uiAtlasImage);
			uiNumTextureBindingsPerLoop++;
		}

		CVector2f v2fDims = _mvImageDetails[_muiCurrentFrameNumber].v2fDimensions;
		CVector2f v2fPos = _mv2fPosition;

		// Scale sprite if needed
		if (_mv2fScale.x != 1.0f || _mv2fScale.y != 1.0f)
		{
			v2fDims.x *= _mv2fScale.x;
			v2fDims.y *= _mv2fScale.y;
		}

		// Offset position from top left to center of entity
		v2fPos -= v2fDims * 0.5f;

		pTri->addQuad2D(
			v2fPos,											// Position
			v2fDims,										// Dimensions
			_mColour,										// Colour
			_mvImageDetails[_muiCurrentFrameNumber].sTexCoords.bottom_left,		// Texture coordinates
			_mvImageDetails[_muiCurrentFrameNumber].sTexCoords.bottom_right,	// Texture coordinates
			_mvImageDetails[_muiCurrentFrameNumber].sTexCoords.top_right,		// Texture coordinates
			_mvImageDetails[_muiCurrentFrameNumber].sTexCoords.top_left);		// Texture coordinates
	}
}