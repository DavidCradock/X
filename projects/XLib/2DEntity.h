#pragma once
#include "PCH.h"
#include "vector2r.h"
#include "colour.h"
#include "resourceManager.h"

namespace X
{
	// This is an object which holds the position of the entity within the world is is assigned to, non-uniform scale, the CResourceTexture2DAtlas used to 
	// render itself as well as it's current animation frame (if it is animated)
	class C2DEntity
	{
	public:
		friend class SC2DRenderer;
		friend class C2DEntityComplex;

		// strResourceTexture2DAtlasName is the name of the CResourceTexture2DAtlas added to SCResourceManager which contains this entity's image data.
		C2DEntity(const std::string &strResourceTexture2DAtlasName);
		~C2DEntity();

		// Set the position of the sprite entity
		// This sets the top left position of the entity, but in the rendering code, this if offset
		// by half the entity's dimensions, so the this position is the center of the entity.
		void setPosition(const CVector2r& vPosition);

		// Returns the currently set position of the entity
		CVector2r getPosition(void) const;

		// Returns the currently set position of the entity
		glm::vec3 getPositionGLMV3(void) const;

		// Returns the currently set position of the entity
		glm::vec2 getPositionGLMV2(void) const;

		// Set the scale of the sprite (a default of 1.0f is same size as the texture used to render it)
		void setScale(real rScaleX, real rScaleY);

		// Get the scale of the sprite
		CVector2r getScale(void) const;

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

		// This sets the current frame number based on angle of entity
		// Expects an angle between 0.0f-359.99f the value will be clamped
		// This makes some assumptions, that the atlas being used by this entity holds nothing but images
		// which represent the entity images rendered as a rotating sprite, say for example 359 individual
		// images with the entity rendered pointing up on frame 0 and rotating clockwise throughout all the
		// other images. If this is not the case, expect weird results.
		void setFrameBasedOnAngle(float fRotationDegrees);

		// This sets the current frame number based on direction vector of entity
		// This makes some assumptions, that the atlas being used by this entity holds nothing but images
		// which represent the entity images rendered as a rotating sprite, say for example 359 individual
		// images with the entity rendered pointing up on frame 0 and rotating clockwise throughout all the
		// other images. If this is not the case, expect weird results.
		void setFrameBasedOnDirection(CVector2r vDir);

		// Sets whether this entity is rendered or not.
		// By default, they are visible
		void setVisible(bool bVisible);

		// Returns whether this entity is currently set to be visible and rendered or not
		bool getVisible(void) const;
	private:
		CVector2r _mv2rPosition;						// Position of this entity in the world
		CVector2r _mv2rScale;							// Scale of this entity
		CColour _mColour;								// Colour used for the vertex colour of this entity.
		std::string _mstrResourceTexture2DAtlasName;	// Name of the CResourceTexture2DAtlas added to SCResourceManager which holds this entity's image data
		unsigned int _muiCurrentFrameNumber;			// Current frame number of this entity
		bool _mbUseAnimationFrames;						// Whether to use a single image or multiple stored in the texture atlas.
		std::vector<std::string> _mvstrImageNames;		// A vector of strings holding each image name stored in the texture atlas to use.
		bool _mbImagesAreSet;							// Keeps track of whether either setImagesSingle() or setImagesMultiple() has been called and
														// checked in SC2DRenderer to throw an exception stating that they haven't
		bool _mbVisible;								// Whether this layer is visible and rendered or not

		/* A vector of CImageAtlasDetails, holding each image's atlas details including...
			sTexCoords The texture coordinates within the atlas image of the image
			v2fDimensions The dimensions of the image
			strImageFilename The filename which the image was created from.
			uiAtlasImage The atlas image number which the image is located in.
			bRotated Whether the image was rotated clockwise to fit better or not. */
		std::vector<CImageAtlasDetails> _mvImageDetails;
	};
}