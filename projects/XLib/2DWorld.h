#pragma once
#include "PCH.h"
#include "2DLayer.h"
#include "2DCamera.h"

namespace X
{
	class C2DWorld
	{
	public:
		C2DWorld();
		~C2DWorld();

		// Add a new named object
		// When adding a new object, it will be placed "above" all previously added objects, making it's entities rendered last aka above all other objects.
		// If the object name already exists, an exception occurs
		// Returns a pointer to the newly added object
		C2DLayer* addLayer(const std::string& strUniqueName);

		// Returns true if an object exists, else false
		bool getLayerExists(const std::string& strUniqueName);

		// Returns a pointer to a previously added named object if it exists, else an exception occurs
		C2DLayer* getLayer(const std::string& strUniqueName);

		// Returns a pointer to a previously added named object if it exists, else an exception occurs
		C2DLayer* getLayer(unsigned int uiIndex);

		// Removes an object (and all the entities which have been added to it)
		// If the object's name given doesn't exist, an exception occurs
		void removeLayer(const std::string& strUniqueName);

		// Removes an object
		// If an invalid index is given, an exception occurs
		void removeLayer(unsigned int uiIndex);

		// Removes all object (and all the entities which have been added to them)
		void removeAllLayers(void);

		// Returns the total number of added objects
		int getNumLayers(void);

		// Returns the name of the layer at specified z order
		// A z order of 0 would give you the back most layer
		// Use getNumLayers() to get the number of layers
		// If invalid z order given, an exception occurs
		std::string getLayerNameAtZorder(unsigned int uiZorder);

		// Returns the name of the layer at specified index
		// Use getNumLayers() to get the number of layers
		// If invalid index given, an exception occurs
		std::string getLayerNameAtIndex(unsigned int uiIndex);

		// Given a name of an existing layer, returns it's order of rendering
		// If it's at the back, this would be 0
		// If it's at the front, this would be the total number of added layers - 1
		// If the layer couldn't be found, an exception occurs
		int getLayerZorder(const std::string& strLayerName);

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

		// Add a new named object
		// If the object name already exists, an exception occurs
		// Returns a pointer to the newly added object
		C2DCamera* addCamera(const std::string& strUniqueName);

		// Returns true if an object exists, else false
		bool getCameraExists(const std::string& strUniqueName);

		// Returns a pointer to a previously added named object if it exists, else an exception occurs
		C2DCamera* getCamera(const std::string& strUniqueName);

		// Returns a pointer to a previously added named object if it exists, else an exception occurs
		C2DCamera* getCamera(unsigned int uiIndex);

		// Removes an object
		// If the object's name given doesn't exist, an exception occurs
		void removeCamera(const std::string& strUniqueName);

		// Removes an object
		// If an invalid index is given, an exception occurs
		void removeCamera(unsigned int uiIndex);

		// Removes all objects
		void removeAllCameras(void);

		// Returns the total number of added objects
		int getNumCameras(void);
	private:
		std::map<std::string, C2DLayer*> _mmapLayers;		// Each named layer which holds sprite entities
		std::vector<std::string> _mvecLayerNameZOrder;		// Holds names of each layer, in their rendering order (first added = first rendered)
		std::map<std::string, C2DCamera*> _mmapCameras;		// Each named camera
	};
}