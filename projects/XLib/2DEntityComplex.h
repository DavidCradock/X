#pragma once
#include "PCH.h"
#include "2DLayerComplex.h"

namespace X
{
	// This is an object which should have a main parent entity added to any added layer and this must be named "PARENT"
	// If this entity isn't added, an exception occurs during rendering.
	// The position, rotation and scale of the "PARENT" entity is the actual position of the whole object and all other added entities,
	// in whatever layer, offset and rotate themselves in relation to this parent entity.
	// For example, we create a couple of layers and add an entity named "PARENT" to the backmost layer, this could be a spaceship hull or something.
	// Then we add more entities, perhaps to represent guns in the layer above the parent one.
	// Rotating the "PARENT" entity will then correctly position the gun entities as expected.
	class C2DEntityComplex
	{
	public:
		friend class SC2DRenderer;

		C2DEntityComplex();
		~C2DEntityComplex();

		// Called from SC2DRenderer::render() to render this entity.
		// It assumes the shader has has been bound and the view/projection matrix have already been set
		void render(void);

		// Set the position of the "PARENT" entity
		// This sets the top left position of the entity, but in the rendering code, this if offset
		// by half the entity's dimensions, so the this position is the center of the entity.
		void setPosition(const CVector2f& vPosition);

		// Sets rotation of the "PARENT" entity in degress
		// This does NOT set the animation frame, use setFrameBasedOnAngle() for that
		void setRotation(float fRotationDegrees);

		// Sets rotation of the "PARENT" entity given a direction vector
		// This does NOT set the animation frame, use setFrameBasedOnDirection() for that
		void setRotation(CVector2f vDirection);

		// Returns the currently set position of the "PARENT" entity
		CVector2f getPosition(void) const;

		// Set the scale of the "PARENT" entity (a default of 1.0f is same size as the texture used to render it)
		void setScale(float fScaleX, float fScaleY);

		// Get the scale of the "PARENT" entity
		CVector2f getScale(void) const;

		// Set the colour multiplier for the "PARENT" entity (default = white, full opacity)
		void setColour(const CColour& colour);

		// Returns the currently set colour multiplier for the "PARENT" entity
		CColour getColour(void) const;

		// Sets the "PARENT" entity to use a single image stored within the texture atlas to use whilst rendering itself
		// If an invalid image name is given, an exception occurs
		// Resets current frame number to 0
		void setImagesSingle(const std::string& strImageFilenameInAtlas);

		// Sets the "PARENT" entity to use multiple images stored within the texture atlas, specifying each image name in order.
		// If an invalid image name is given, an exception occurs.
		// Resets current frame number to 0
		void setImagesMultiple(const std::vector<std::string>& strImageFilenamesInAtlas);

		// Returns the "PARENT" entity's current animation frame number
		unsigned int getCurrentFrameNumber(void) const;

		// Sets the "PARENT" entity's current animation frame number
		// If invalid framenumber is given, an exception occurs
		void setCurrentFrameNumber(unsigned int uiFrameNumber);

		// Returns the number of image names in the atlas texture used by the "PARENT" entity
		unsigned int getNumFrames(void) const;

		// This sets the current frame number of the "PARENT" entity based on angle of entity
		// Expects an angle between 0.0f-359.99f the value will be clamped
		// This makes some assumptions, that the atlas being used by this entity holds nothing but images
		// which represent the entity images rendered as a rotating sprite, say for example 359 individual
		// images with the entity rendered pointing up on frame 0 and rotating clockwise throughout all the
		// other images. If this is not the case, expect weird results.
		void setFrameBasedOnAngle(float fRotationDegrees);

		// This sets the current frame number of the "PARENT" entity based on direction vector of entity
		// This makes some assumptions, that the atlas being used by this entity holds nothing but images
		// which represent the entity images rendered as a rotating sprite, say for example 359 individual
		// images with the entity rendered pointing up on frame 0 and rotating clockwise throughout all the
		// other images. If this is not the case, expect weird results.
		void setFrameBasedOnDirection(CVector2f vDir);

		// Add a new named layer
		// When adding a new object, it will be placed "above" all previously added objects, making it's entities rendered last aka above all other objects.
		// If the object name already exists, an exception occurs
		// Returns a pointer to the newly added object
		C2DLayerComplex* addLayer(const std::string& strUniqueName);

		// Returns true if an object exists, else false
		bool getLayerExists(const std::string& strUniqueName) const;

		// Returns a pointer to a previously added named object if it exists, else an exception occurs
		C2DLayerComplex* getLayer(const std::string& strUniqueName) const;

		// Returns a pointer to a previously added named object if it exists, else an exception occurs
		C2DLayerComplex* getLayer(unsigned int uiIndex) const;

		// Removes an object (and all the entities which have been added to it)
		// If the object's name given doesn't exist, an exception occurs
		void removeLayer(const std::string& strUniqueName);

		// Removes an object
		// If an invalid index is given, an exception occurs
		void removeLayer(unsigned int uiIndex);

		// Removes all object (and all the entities which have been added to them)
		void removeAllLayers(void);

		// Returns the total number of added objects
		unsigned int getNumLayers(void) const;

		// Returns the name of the layer at specified z order
		// A z order of 0 would give you the back most layer
		// Use getNumLayers() to get the number of layers
		// If invalid z order given, an exception occurs
		std::string getLayerNameAtZorder(unsigned int uiZorder) const;

		// Returns the name of the layer at specified index
		// Use getNumLayers() to get the number of layers
		// If invalid index given, an exception occurs
		std::string getLayerNameAtIndex(unsigned int uiIndex) const;

		// Given a name of an existing layer, returns it's order of rendering
		// If it's at the back, this would be 0
		// If it's at the front, this would be the total number of added layers - 1
		// If the layer couldn't be found, an exception occurs
		int getLayerZorder(const std::string& strLayerName) const;

		// Given a name of an existing layer (strLayerName), moves it closer to the front by one
		// If the layer name doesn't exist, an exception occurs
		void moveLayerToFrontByOne(const std::string& strLayerName);

		// Given a name of an existing layer (strLayerName), moves it closer to the back by one
		// If the layer name doesn't exist, an exception occurs
		void moveLayerToBackByOne(const std::string& strLayerName);

		// Given a name of an existing layer (strLayerName), moves it completely to the back
		// If the layer name doesn't exist, an exception occurs
		void moveLayerToBack(const std::string& strLayerName);

		// Given a name of an existing layer (strLayerName), moves it completely to the front
		// If the layer name doesn't exist, an exception occurs
		void moveLayerToFront(const std::string& strLayerName);

		// Given a name of an existing layer (strLayerName) and another existing layer (strLayerNameOther), moves strLayerName so that it is behind strLayerNameOther
		// If either of the the layer names don't exist, an exception occurs
		void moveLayerBehind(const std::string& strLayerName, const std::string& strLayerNameOther);

		// Given a name of an existing layer (strLayerName) and another existing layer (strLayerNameOther), moves strLayerName so that it is in front of strLayerNameOther
		// If either of the the layer names don't exist, an exception occurs
		void moveLayerInfront(const std::string& strLayerName, const std::string& strLayerNameOther);

	private:
		mutable std::map<std::string, C2DLayerComplex*> _mmapLayers;	// Each named layer which holds sprite entities
		std::vector<std::string> _mvecLayerNameZOrder;					// Holds names of each layer, in their rendering order (first added = first rendered)
		float _mfParentRotationDegrees;									// Holds the rotation of the "PARENT" entity in degrees

		// Goes through all the layers, finds the entity named "PARENT" and returns a pointer to it.
		// If the "PARENT" entity couldn't be found, an exception occurs.
		C2DEntity* _findParentEntity(void) const;
	};
}