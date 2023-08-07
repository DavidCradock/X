#pragma once
#include "PCH.h"
#include "vector2f.h"
#include "colour.h"
#include "resourceManager.h"

namespace X
{
	// A C2DEntityRot is an entity which holds an individual entity to be rendered to a layer and holds it's
	// position, scale and image stored in a texture atlas to use whilst rendering itself.
	// A C2DEntity is fast to render lots of entities, but has a limitation.
	// The limitation is that the entity can not be rotated.
	// If we wish to use entities which can be rotated, see the C2DEntityRot object.
	// We can however fake rotation by rendering 360 or so images in Blender, each representing the entity at each angle of rotation, adding each of those
	// images to a texture atlas and setting which image to use based on the angle of rotation we wish to render the entity.
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
		void setPosition(const CVector2f& vPosition);

		// Returns the currently set position of the entity
		CVector2f getPosition(void) const;

		// Set the scale of the sprite (a default of 1.0f is same size as the texture used to render it)
		void setScale(float fScaleX, float fScaleY);

		// Get the scale of the sprite
		CVector2f getScale(void) const;

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

		// Although this entity type doesn't actually support rotation (For speed reasons),
		// this sets the current frame number based on the angle of the entity.
		// This is because to "simulate" rotation, we render out many images of the object at various amounts
		// of rotation and store all these images inside the texture atlas. We then set which image to use when
		// rendering, based upon the angle we wish the entity to be rendered at.
		// 
		// Expects an angle between 0.0f-359.99f the value will be clamped
		// This makes some assumptions, that the atlas being used by this entity holds nothing but images
		// which represent the entity images rendered as a rotating sprite, say for example 359 individual
		// images with the entity rendered pointing up on frame 0 and rotating clockwise throughout all the
		// other images. If this is not the case, expect weird results.
		void setFrameBasedOnAngle(float fRotationDegrees);

		// Although this entity type doesn't actually support rotation (For speed reasons),
		// this sets the current frame number based on direction vector of entity.
		// This is because to "simulate" rotation, we render out many images of the object at various amounts
		// of rotation and store all these images inside the texture atlas. We then set which image to use when
		// rendering, based upon the direction we wish the entity to be rendered at.
		//
		// The CVector2f is normalized for us within.
		// This makes some assumptions, that the atlas being used by this entity holds nothing but images
		// which represent the entity images rendered as a rotating sprite, say for example 359 individual
		// images with the entity rendered pointing up on frame 0 and rotating clockwise throughout all the
		// other images. If this is not the case, expect weird results.
		void setFrameBasedOnDirection(CVector2f vDir);

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
			unsigned int& uiNumTextureBindingsPerLoop);
	private:
		CVector2f _mv2fPosition;						// Position of this entity in the world
		CVector2f _mv2fScale;							// Scale of this entity
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