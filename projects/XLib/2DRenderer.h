#pragma once
#include "PCH.h"
#include "singleton.h"
#include "2DCamera.h"
#include "2DEntity.h"
#include "2DImageData.h"
#include "2DLayer.h"
#include "2DWorld.h"

namespace X
{
	// SC2DRenderer is a 2D renderer singleton class.
	// This is responsible for rendering 2D objects.
	// Physics and other such things such as AI is kept seperate from this. This is solely for rendering of 2D objects.
	// 2D objects can be simple diffuse coloured (with vertex colours) or diffuse with normal maps for fancy lighting.
	// We can create parent/child relationships between individual objects to create correct rotation/position of child objects.
	// 
	// The manager works with multiple objects including worlds, cameras, layers and entities
	// Each object is stored in a hashmap, so they can be accessed via a unique name for each object and
	// take advantage of fast sorting of the objects.
	// 
	// Here is more information about each of the objects...
	// 
	// SC2DRenderer:
	// We go here to create everything 2D related, including image data.
	// 
	// C2DImageData:
	// This holds image data used to render each entity.
	// Imagedata is added from single image files which have RGB colour information and alpha for transparency.
	// Aswell as RGBA colour information, we can also have normal maps for lighting effects.
	// Images are added from multiple image files into a single ImageData object to reduce texture binding.
	// Imagedata can hold a single image or multiple images for animation.
	// 
	// C2DWorld:
	// The top most object is a C2DWorld, we create at least one of these to hold all other objects.
	// We can have multiple worlds.
	// A world needs at least one camera to be able to render the world to a framebuffer.
	// A world should have at least one layer with which to contain the various entities.
	// 
	// C2DCamera:
	// A camera has a position inside a C2DWorld and is used to render it's view of the world into a framebuffer
	// which can then be rendered onto the back buffer or rendered inside a CGUIContainer.
	// A camera also has a "zoom/height" value. This scales sprites and gives the impression of zooming in/out to/from the world.
	//
	// C2DLayer:
	// A layer stores entities and each layer is ordered by rendering order so that we can have some control over which entities get rendered
	// on top of others.
	// 
	// C2DEntity:
	// This is a simple object which holds the position of the entity within the world, a rotation, non-uniform scale, the imagedata used to 
	// render itself as well as it's current animation frame (if it is animated)
	// 
	// C2DEntityComplex
	// This holds multiple entities which have a parent/child relationship.
	// This is so that we can create for example, a spaceship parent object with the image of a spaceship and then we can add child entities
	// (for example, guns) with their position offset from the parent's origin so that when rotating the parent entity, the child entities
	// translate, rotate and scale along with their parent entity.
	// These also have layers, so we can control the order of rendering.
	//
	class SC2DRenderer : public CSingleton<SC2DRenderer>
	{
	public:
		SC2DRenderer();
		~SC2DRenderer();

		// Add a new named object
		// If the object name already exists, an exception occurs
		// Returns a pointer to the newly added object
		C2DWorld* addWorld(const std::string& strUniqueName);

		// Returns true if an object exists, else false
		bool getWorldExists(const std::string& strUniqueName);

		// Returns a pointer to a previously added named object if it exists, else an exception occurs
		C2DWorld* getWorld(const std::string& strUniqueName);

		// Returns a pointer to a previously added named object if it exists, else an exception occurs
		C2DWorld* getWorld(unsigned int uiIndex);

		// Removes an object
		// If the object's name given doesn't exist, an exception occurs
		void removeWorld(const std::string& strUniqueName);

		// Removes an object
		// If an invalid index is given, an exception occurs
		void removeWorld(unsigned int uiIndex);

		// Removes all objects
		void removeAllWorlds(void);

		// Returns the total number of added objects
		int getNumWorlds(void);

	private:
		std::map<std::string, C2DWorld*> _mmapWorlds;		// Each named world
	};
}