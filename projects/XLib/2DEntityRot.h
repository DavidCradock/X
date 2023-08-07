#pragma once
#include "PCH.h"
#include "vector2f.h"
#include "colour.h"
#include "resourceManager.h"

namespace X
{
	// A C2DEntityRot is an entity which holds an individual entity to be rendered to a layer and holds it's
	// position, scale and image stored in a texture atlas to use whilst rendering itself.
	// This also contains rotation.
	// A C2DEntityRot is similar to a C2DEntity except that it can be rotated.
	// Because of this ability, these entities are slower to render in large batches than the C2DEntity object.
	// Does the rotation get performed on the CPU or the GPU?
	// If performing rotation on the CPU, this allows batching before sending to the GPU.
	// If performing rotation on the GPU, no batching is possible, but spares the CPU for other tasks.
	// As CPU cycles are precious, I decided to go with performing the rotation on the GPU.
	class C2DEntityRot
	{
	public:
		// strResourceTexture2DAtlasName is the name of the CResourceTexture2DAtlas added to SCResourceManager which contains this entity's image data.
		C2DEntityRot(const std::string& strResourceTexture2DAtlasName);
		~C2DEntityRot();

		// Set the position of the sprite entity
		// This sets the top left position of the entity, but in the rendering code, this if offset
		// by half the entity's dimensions, so the this position is the center of the entity.
		void setPosition(const CVector2f& vPosition);

		// Returns the currently set position of the entity
		CVector2f getPosition(void) const;

		// Set the scale of the sprite (a default of 1.0f is same size as the texture used to render it)
		void setScale(float fScaleX, float fScaleY);

		// Get the scale of the sprite
		CVector2f getScale(void) const;

		// Set the rotation of this entity in degrees
		// Working in degrees is slower than in radians as there's a conversion done.
		void setRotationDegrees(float fAngleDegrees);

		// Set the rotation of this entity in radians
		// Working in radians is faster than degrees as there's no conversion to do.
		void setRotationRadians(float fAngleRadians);

		// Get the rotation of this entity in degrees
		// Working in degrees is slower than in radians as there's a conversion done.
		float getRotationDegrees(void);

		// Get the rotation of this entity in radians
		// Working in radians is faster than degrees as there's no conversion to do.
		float getRotationRadians(void);

		// Set the colour multiplier for the sprite entity (default = white, full opacity)
		void setColour(const CColour& colour);

		// Returns the currently set colour multiplier for the sprite entity
		CColour getColour(void) const;

		// Sets the entity to use a single image stored within the texture atlas to use whilst rendering itself
		// If an invalid image name is given, an exception occurs
		// Resets current frame number to 0
		void setImagesSingle(const std::string& strImageFilenameInAtlas);

		// Sets the entity to use multiple images stored within the texture atlas, specifying each image name in order.
		// If an invalid image name is given, an exception occurs.
		// Resets current frame number to 0
		void setImagesMultiple(const std::vector<std::string>& strImageFilenamesInAtlas);

		// Returns this entity's current animation frame number
		unsigned int getCurrentFrameNumber(void) const;

		// Sets this entity's current animation frame number
		// If invalid framenumber is given, an exception occurs
		void setCurrentFrameNumber(unsigned int uiFrameNumber);

		// Returns the number of image names in the atlas texture
		unsigned int getNumFrames(void) const;

		// Sets whether this entity is rendered or not.
		// By default, they are visible
		void setVisible(bool bVisible);

		// Returns whether this entity is currently set to be visible and rendered or not
		bool getVisible(void) const;

		// Renders this entity.
		// It's called from the SC2DRenderer::render() method.
		void render(
			std::string& strPreviouslyBoundAtlasName,
			unsigned int& uiPreviouslyBoundAtlasImageNumber,
			CResourceVertexBufferCPT* pVB,
			CResourceShader* pShader,
			unsigned int& uiNumTextureBindingsPerLoop);
	private:
		CVector2f _mv2fPosition;						// Position of this entity in the world
		CVector2f _mv2fScale;							// Scale of this entity
		float _mfRotationRadians;						// Rotation of the entity in radians
		CColour _mColour;								// Colour used for the vertex colour of this entity.
		std::string _mstrResourceTexture2DAtlasName;	// Name of the CResourceTexture2DAtlas added to SCResourceManager which holds this entity's image data
		unsigned int _muiCurrentFrameNumber;			// Current frame number of this entity
		bool _mbUseAnimationFrames;						// Whether to use a single image or multiple stored in the texture atlas.
		std::vector<std::string> _mvstrImageNames;		// A vector of strings holding each image name stored in the texture atlas to use.
		bool _mbImagesAreSet;							// Keeps track of whether either setImagesSingle() or setImagesMultiple() has been called and
														// checked in render() to throw an exception stating that they haven't
		bool _mbVisible;								// Whether this entity is visible and rendered or not

		/* A vector of CImageAtlasDetails, holding each image's atlas details including...
			sTexCoords The texture coordinates within the atlas image of the image
			v2fDimensions The dimensions of the image
			strImageFilename The filename which the image was created from.
			uiAtlasImage The atlas image number which the image is located in.
			bRotated Whether the image was rotated clockwise to fit better or not. */
		std::vector<CImageAtlasDetails> _mvImageDetails;

		// The layer this entity belongs to.
		// This is set in the constructor and is used in various methods such as setPosition,
		// so that the entity's position is updated in the layer's quad tree
		//C2DLayer* _mpLayerOwner;
	};
}