#pragma once
#include "PCH.h"
#include "sceneManagerLightDirectional.h"
#include "SceneManagerEntityVertexbuffer.h"
#include "camera.h"

namespace X
{
	// A simple brute force scene manager which has no spatial partitioning.
	// It shouldn't really be used for anything as other scene managers are more efficient.
	// This is here for development of other areas of code.
	// It does sort renderable object by shader and textures though
	class SceneManagerSimple
	{
	public:
		SceneManagerSimple();

		// Render all the entities in this scene
		void render(void);

		Camera mCamera;

		SceneManagerLightDirectional mvLightDirectional;	// The directional light for this scene

		// Adds a new uniquely named vertex buffer entity to this scene amd returns a pointer to it if needed
		// If the named entity already exists, an exception occurs
		SceneManagerEntityVertexbuffer* addEntityVertexbuffer(
			const std::string& strEntityName,									// The unique name of this entity
			const std::string& strVertexbufferName,								// The vertex buffer resource located in the ResourceManager used when rendering this entity
			const std::string& strTextureNameDiffuse = "X:default_diffuse",		// The texture resource located in the ResourceManager used for the diffuse colour
			const std::string& strTextureNameRoughness = "X:default_roughness",	// The texture resource located in the ResourceManager used for the roughness
			const std::string& strTextureNameNormal = "X:default_normal",		// The texture resource located in the ResourceManager used for the normal map
			const std::string & strTextureNameEmission = "X:default_emission"	// The texture resource located in the ResourceManager used for the emission
		);

		// Returns whether the named vertex buffer entity exists or not
		bool getEntityVertexbufferExists(const std::string& strEntityName);

		// Attempts to remove the named entity from the scene
		// If the entity doesn't exist, this silently fails
		void removeEntityVertexbuffer(const std::string& strEntityName);

		// Removes all entites of type vertex buffer
		void removeAllEnititiesVertexbuffer(void);

		std::map<std::string, SceneManagerEntityVertexbuffer*> mmapEnititiesVertexbuffer;	// Each named vertex buffer entity in this scene.
	};
}