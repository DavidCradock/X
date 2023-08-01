#include "PCH.h"
#include "2DEntityRot.h"
#include "log.h"

namespace X
{
	C2DEntityRot::C2DEntityRot(const std::string& strResourceTexture2DAtlasName)
	{
		_mstrResourceTexture2DAtlasName = strResourceTexture2DAtlasName;
		_muiCurrentFrameNumber = 0;
		_mv2fPosition.setZero();
		_mv2fScale.set(1, 1);
		_mColour.set(1.0f, 1.0f, 1.0f, 1.0f);
		_mbImagesAreSet = false;
		_mbVisible = true;
		_mfRotationRadians = 0.0f;
	}

	C2DEntityRot::~C2DEntityRot()
	{

	}

	void C2DEntityRot::setPosition(const CVector2f& vPosition)
	{
		_mv2fPosition = vPosition;
	}

	CVector2f C2DEntityRot::getPosition(void) const
	{
		return _mv2fPosition;
	}

	void C2DEntityRot::setScale(float fScaleX, float fScaleY)
	{
		_mv2fScale.set(fScaleX, fScaleY);
	}

	CVector2f C2DEntityRot::getScale(void) const
	{
		return _mv2fScale;
	}

	void C2DEntityRot::setRotationDegrees(float fAngleDegrees)
	{
		_mfRotationRadians = deg2rad(fAngleDegrees);
	}

	void C2DEntityRot::setRotationRadians(float fAngleRadians)
	{
		_mfRotationRadians = fAngleRadians;
	}

	float C2DEntityRot::getRotationDegrees(void)
	{
		return rad2deg(_mfRotationRadians);
	}

	float C2DEntityRot::getRotationRadians(void)
	{
		return _mfRotationRadians;
	}

	void C2DEntityRot::setColour(const CColour& colour)
	{
		_mColour = colour;
	}

	CColour C2DEntityRot::getColour(void) const
	{
		return _mColour;
	}

	void C2DEntityRot::setImagesSingle(const std::string& strImageFilenameInAtlas)
	{
		// Make sure the set atlas exists
		SCResourceManager* pRM = SCResourceManager::getPointer();
		ThrowIfFalse(pRM->getTexture2DAtlasExists(_mstrResourceTexture2DAtlasName), "C2DEntityRot::setSingleImage() failed. The entity's currently set C2DTextureAtlas resource of " + _mstrResourceTexture2DAtlasName + " couldn't be found.");
		// Make sure the named image exists within the atlas
		CResourceTexture2DAtlas* pAtlas = pRM->getTexture2DAtlas(_mstrResourceTexture2DAtlasName);
		ThrowIfFalse(pAtlas->getImageNameExists(strImageFilenameInAtlas), "C2DEntityRot::setSingleImage() failed. The named image of " + strImageFilenameInAtlas + " does not exist within the entity's currently set texture atlas resource.");

		// If we get here, everything exists, set the entity to use the image...
		_mbUseAnimationFrames = false;
		_mvstrImageNames.clear();
		_mvstrImageNames.push_back(strImageFilenameInAtlas);
		_mvImageDetails.clear();
		_mvImageDetails.push_back(pAtlas->getImageDetails(strImageFilenameInAtlas));
		_muiCurrentFrameNumber = 0;
		_mbImagesAreSet = true;
	}

	void C2DEntityRot::setImagesMultiple(const std::vector<std::string>& strImageFilenamesInAtlas)
	{
		// Make sure the set atlas exists
		SCResourceManager* pRM = SCResourceManager::getPointer();
		ThrowIfFalse(pRM->getTexture2DAtlasExists(_mstrResourceTexture2DAtlasName), "C2DEntityRot::setImagesMultiple() failed. The entity's currently set C2DTextureAtlas resource of " + _mstrResourceTexture2DAtlasName + " couldn't be found.");
		CResourceTexture2DAtlas* pAtlas = pRM->getTexture2DAtlas(_mstrResourceTexture2DAtlasName);

		_mbUseAnimationFrames = true;
		_mvstrImageNames.clear();	// Clear stored image names
		_mvImageDetails.clear();
		_muiCurrentFrameNumber = 0;
		_mbImagesAreSet = true;
		for (unsigned int ui = 0; ui < strImageFilenamesInAtlas.size(); ui++)
		{
			// Make sure the named images exist within the atlas
			ThrowIfFalse(pAtlas->getImageNameExists(strImageFilenamesInAtlas[ui]), "C2DEntityRot::setImagesMultiple() failed. The named image of " + strImageFilenamesInAtlas[ui] + " does not exist within the entity's currently set texture atlas resource.");
			// Add the image name to this entity's vector
			_mvstrImageNames.push_back(strImageFilenamesInAtlas[ui]);

			// Store each image's details here so we don't have to look this up each program loop
			_mvImageDetails.push_back(pAtlas->getImageDetails(strImageFilenamesInAtlas[ui]));
		}
	}

	unsigned int C2DEntityRot::getCurrentFrameNumber(void) const
	{
		return _muiCurrentFrameNumber;
	}

	void C2DEntityRot::setCurrentFrameNumber(unsigned int uiFrameNumber)
	{
		ThrowIfTrue(uiFrameNumber > _mvstrImageNames.size(), "C2DEntityRot::setCurrentFrameNumber() given invalid frame number.");
		_muiCurrentFrameNumber = uiFrameNumber;
	}

	unsigned int C2DEntityRot::getNumFrames(void) const
	{
		return (unsigned int)_mvstrImageNames.size();
	}

	void C2DEntityRot::setVisible(bool bVisible)
	{
		_mbVisible = bVisible;
	}

	bool C2DEntityRot::getVisible(void) const
	{
		return _mbVisible;
	}

	void C2DEntityRot::render(void)
	{

	}
}