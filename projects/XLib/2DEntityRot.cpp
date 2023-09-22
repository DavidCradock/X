#include "PCH.h"
#include "2DEntityRot.h"
#include "singletons.h"
#include "logging.h"

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
		ThrowIfFalse(x->pResource->getTexture2DAtlasExists(_mstrResourceTexture2DAtlasName), "C2DEntityRot::setSingleImage() failed. The entity's currently set C2DTextureAtlas resource of " + _mstrResourceTexture2DAtlasName + " couldn't be found.");
		// Make sure the named image exists within the atlas
		CResourceTexture2DAtlas* pAtlas = x->pResource->getTexture2DAtlas(_mstrResourceTexture2DAtlasName);
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
		ThrowIfFalse(x->pResource->getTexture2DAtlasExists(_mstrResourceTexture2DAtlasName), "C2DEntityRot::setImagesMultiple() failed. The entity's currently set C2DTextureAtlas resource of " + _mstrResourceTexture2DAtlasName + " couldn't be found.");
		CResourceTexture2DAtlas* pAtlas = x->pResource->getTexture2DAtlas(_mstrResourceTexture2DAtlasName);

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

	void C2DEntityRot::render(
		std::string& strPreviouslyBoundAtlasName,
		unsigned int& uiPreviouslyBoundAtlasImageNumber,
		CResourceVertexBufferCPTInst* pVBI,
		CResourceShader* pShader,
		unsigned int& uiNumTextureBindingsPerLoop)
	{
		// If this entity is set as invisible, do nuffin'
		if (!_mbVisible)
			return;

		// Make sure the images this entity uses have been set, if not, throw an exception
		ThrowIfFalse(_mbImagesAreSet, "C2DEntityRot::render() failed. Entity has not had either setImagesSingle() or setImagesMultiple() called.");

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

		// Bind the texture (And also create geometry with correct texture coordinates
		if (bNeedToBindTexture)
		{
			// Get CResourceTexture2DAtlas the entity is set to use
			CResourceTexture2DAtlas* pAtlas = x->pResource->getTexture2DAtlas(_mstrResourceTexture2DAtlasName);
			// Bind the atlas texture containing the entity's currently set frame number's image
			pAtlas->bindAtlas(0, _mvImageDetails[_muiCurrentFrameNumber].uiAtlasImage);
			uiNumTextureBindingsPerLoop++;

			// Render any existing data
			pVBI->render();

			// Build geometry with correct texture coordinates
			pVBI->removeAll();
			pVBI->addQuad2D(
				CVector2f(-0.5f, -0.5f),
				CVector2f(1, 1),
				_mColour,
				_mvImageDetails[_muiCurrentFrameNumber].sTexCoords.vBL,
				_mvImageDetails[_muiCurrentFrameNumber].sTexCoords.vBR,
				_mvImageDetails[_muiCurrentFrameNumber].sTexCoords.vTR,
				_mvImageDetails[_muiCurrentFrameNumber].sTexCoords.vTL);
			pVBI->update();
		}

		// Create rotation matrix for this entity
		CMatrix matrixRotation;
		matrixRotation.setFromAxisAngleRadians(CVector3f(0.0f, 0.0f, 1.0f), _mfRotationRadians);

		// Create scale matrix for this entity
		CMatrix matrixScale;
		CVector3f v3fScale(_mv2fScale.x, _mv2fScale.y, 1.0f);
		v3fScale.x *= _mvImageDetails[_muiCurrentFrameNumber].vDims.x;
		v3fScale.y *= _mvImageDetails[_muiCurrentFrameNumber].vDims.y;
		matrixScale.setScale(v3fScale);

		// Create translation matrix for this entity
		CMatrix matrixTranslation;
		CVector3f v3fPosition = v3fScale * -0.5f;	// Offset position from top left to center of entity
		matrixTranslation.setTranslation(_mv2fPosition.x, _mv2fPosition.y, 0.0f);

		// Create world matrix
		CMatrix matrixWorld = matrixTranslation * matrixRotation *matrixScale;

		// Add instance data to the instanced vertex buffer
		pVBI->addInstanceMatrix(matrixWorld);

	}
}