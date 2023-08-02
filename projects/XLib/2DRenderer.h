#pragma once
#include "PCH.h"
#include "singleton.h"
#include "2DWorld.h"

namespace X
{
	// SC2DRenderer is a 2D renderer singleton class.
	// This is responsible for rendering 2D objects.
	// Physics and other such things such as AI is kept seperate from this. This is solely for rendering of 2D objects.
	// 2D objects can be simple diffuse coloured (with vertex colours)
	// 
	// The manager works with multiple objects including worlds, cameras, layers and entities
	// Each object is stored in a hashmap, so they can be accessed via a unique name for each object and
	// take advantage of fast sorting of the objects.
	// 
	// We go here to create everything 2D related, except the CResourceTexture2DAtlas objects (which the entities use),
	// which are created with the SCResourceManager->addTexture2DAtlas() method.
	// 
	// When rendering an animation sequence of .png files in Blender, to make sure the images have alpha channel,
	// not only goto render properties, film and enable transparent, but also make sure in the compositor 
	// tab, that the alpha from render layers node is linked to alpha of composite node.
	// 
	// Usage example to get a single entity rendered...
	// // First, create the C2DTexture2DAtlas resource which the entity will use for it's image data.
	// SCResourceManager* pResourceManager = SCResourceManager::getPointer();	// Obtain pointer to the resource manager.
	// Fill in a std::vector<std::string> to hold all the images the entity will use, in this case, just one single image.
	// std::vector<std::string> vstrImageFilenames;
	// vstrImageFilenames.push_back("data/Demo2D/images/creature_top_down.png");
	// // Now create the texture atlas with the resource manager...
	// pResourceManager->addTexture2DAtlas("MyTextureAtlas", vstrImageFilenames, true, 1);
	// SC2DRenderer* p2DRenderer = SC2DRenderer::getPointer();					// Obtain pointer to this object
	// C2DWorld* pWorld = p2DRenderer->addWorld("MyWorld");						// Add a new world to contain everything
	// C2DLayer* pLayer = pWorld->addLayer("MyLayer");							// Add a new layer to the world
	// C2DCamera* pCamera = pWorld->addCamera("MyCamera");						// Add a new camera to the world
	// Add the entity to the layer, of the world, specifying which texture atlas it will get it's image data from...
	// C2DEntity* pEntity = pLayer->addEntity("MyEntity", "MyTextureAtlas");
	// Set which image in the texture atlas the entity will use...
	// pEntity->setImagesSingle("data/Demo2D/images/creature_top_down.png");
	// Optionally, set the entity's position...
	// pEntity->setPosition(CVector2r(1.0f, 2.0f));
	// Done!
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
		bool getWorldExists(const std::string& strUniqueName) const;

		// Returns a pointer to a previously added named object if it exists, else an exception occurs
		C2DWorld* getWorld(const std::string& strUniqueName) const;

		// Returns a pointer to a previously added named object if it exists, else an exception occurs
		C2DWorld* getWorld(unsigned int uiIndex) const;

		// Removes an object
		// If the object's name given doesn't exist, an exception occurs
		void removeWorld(const std::string& strUniqueName);

		// Removes an object
		// If an invalid index is given, an exception occurs
		void removeWorld(unsigned int uiIndex);

		// Removes all objects
		void removeAllWorlds(void);

		// Returns the total number of added objects
		int getNumWorlds(void) const;

		// Updates and renders everything
		// Called from SCApplicationManager::mainLoop before GUI rendering
		void render(void);

		// Returns the total number of atlas texture bindings per loop
		unsigned int getNumberTextureRebindingsPerLoop(void) const;
	private:
		mutable std::map<std::string, C2DWorld*> _mmapWorlds;	// Each named world
		unsigned int _muiNumTextureBindingsPerLoop;				// Holds number of times textures are bound per loop
	};
}