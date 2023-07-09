#include "PCH.h"
#include "2DEntity.h"
#include "log.h"

namespace X
{
	C2DEntity::C2DEntity(const std::string& strResourceTexture2DAtlasName)
	{
		_mstrResourceTexture2DAtlasName = strResourceTexture2DAtlasName;
		_muiCurrentFrameNumber = 0;
		_mv2rPosition.setZero();
		_mv2rScale.set(1, 1);
		_mColour.set(1.0f, 1.0f, 1.0f, 1.0f);
		_mrRotationDegrees = 0;
	}

	C2DEntity::~C2DEntity()
	{

	}

	void C2DEntity::setPosition(const CVector2r& vPosition)
	{
		_mv2rPosition = vPosition;
	}

	CVector2r C2DEntity::getPosition(void)
	{
		return _mv2rPosition;
	}

	glm::vec3 C2DEntity::getPositionGLMV3(void)
	{
		glm::vec3 vPos;
		vPos.x = (float)_mv2rPosition.x;
		vPos.y = (float)_mv2rPosition.y;
		vPos.z = 0.0f;
		return vPos;
	}

	glm::vec2 C2DEntity::getPositionGLMV2(void)
	{
		glm::vec2 vPos;
		vPos.x = (float)_mv2rPosition.x;
		vPos.y = (float)_mv2rPosition.y;
		return vPos;
	}

	void C2DEntity::setRotationDeg(real rRotationDegress)
	{
		_mrRotationDegrees = rRotationDegress;
	}

	real C2DEntity::getRotationDeg(void)
	{
		return _mrRotationDegrees;
	}

	void C2DEntity::setScale(real rScaleX, real rScaleY)
	{
		_mv2rScale.set(rScaleX, rScaleY);
	}

	CVector2r C2DEntity::getScale(void)
	{
		return _mv2rScale;
	}

	void C2DEntity::setColour(const CColour& colour)
	{
		_mColour = colour;
	}

	CColour C2DEntity::getColour(void)
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

	unsigned int C2DEntity::getCurrentFrameNumber(void)
	{
		return _muiCurrentFrameNumber;
	}

	void C2DEntity::setCurrentFrameNumber(unsigned int uiFrameNumber)
	{
		ThrowIfTrue(uiFrameNumber > _mvstrImageNames.size(), "C2DEntity::setCurrentFrameNumber() given invalid frame number.");
		_muiCurrentFrameNumber = uiFrameNumber;
	}
}