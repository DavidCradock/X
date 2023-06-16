#pragma once
#include "PCH.h"
#include "SMLightDirectional.h"
#include "SMLightPoint.h"
#include "SMEntityVertexbuffer.h"
#include "SMEntityVertexbufferLine.h"
#include "camera.h"
#include "SMMaterial.h"

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

		Camera mCamera;	// The scene manager's camera

		SceneManagerLightDirectional mvLightDirectional;	// The directional light for this scene
		SceneManagerLightPoint mvLightPoint[4];				// The point lights for this scene
		int miNumPointLights;								// The number of point lights to use. Can be between 0 and 4

		// Adds a new uniquely named material which the entity vertex buffers use to render theirselves with.
		// If the named material already exists, an exception occurs.
		SceneManagerMaterial* addMaterial(
			const std::string strMaterialName,		// The unique name of this material
			float fAmbientStrength = 0.05f,										// Ambient strength
			const std::string& strTextureNameDiffuse = "X:default_diffuse",		// The texture resource located in the ResourceManager used for the diffuse colour
			const std::string& strTextureNameRoughness = "X:default_roughness",	// The texture resource located in the ResourceManager used for the roughness
			float fSpecularStrength = 0.25f,									// Specular strength
			const std::string& strTextureNameNormal = "X:default_normal",		// The texture resource located in the ResourceManager used for the normal map
			const std::string& strTextureNameEmission = "X:default_emission"	// The texture resource located in the ResourceManager used for the emission
		);
		
		// Returns a pointer to the named material
		// If the named material doesn't exist, an exception occurs
		SceneManagerMaterial* getMaterial(const std::string& strMaterialName);

		// Returns whether the named material exists or not
		bool getMaterialExists(const std::string& strMaterialName);

		// Attempts to remove the named material from the scene manager
		// If the material doesn't exist, this silently fails
		void removeMaterial(const std::string& strMaterialName);

		// Removes all materials
		void removeAllMaterials(void);


		// Adds a new uniquely named vertex buffer entity to this scene and returns a pointer to it if needed
		// If the named entity already exists, an exception occurs
		SceneManagerEntityVertexbuffer* addEntityVertexbuffer(
			const std::string& strEntityName,				// The unique name of this entity
			const std::string& strVertexbufferName,			// The vertex buffer resource located in the ResourceManager used when rendering this entity
			const std::string& strMaterialName				// The material in the scene manager to use whilst rendering the entity
		);

		// Returns a pointer to the named vertex buffer entity.
		// If the enitity doesn't exist, an exception occurs.
		SceneManagerEntityVertexbuffer* getEntityVertexbuffer(const std::string& strEntityName);

		// Returns whether the named vertex buffer entity exists or not
		bool getEntityVertexbufferExists(const std::string& strEntityName);

		// Attempts to remove the named entity from the scene
		// If the entity doesn't exist, this silently fails
		void removeEntityVertexbuffer(const std::string& strEntityName);

		// Removes all entites of type vertex buffer
		void removeAllEnititiesVertexbuffer(void);

		// Adds a new uniquely named vertex buffer line entity to this scene and returns a pointer to it if needed
		// If the named entity already exists, an exception occurs
		SceneManagerEntityVertexbufferLine* addEntityVertexbufferLine(
			const std::string& strEntityName,									// The unique name of this entity
			const std::string& strVertexbufferName,								// The vertex buffer resource located in the ResourceManager used when rendering this entity
			const std::string& strTextureName = "X:default_white"				// The texture resource located in the ResourceManager used for the colour
		);

		// Returns a pointer to the named vertex buffer line entity.
		// If the enitity doesn't exist, an exception occurs.
		SceneManagerEntityVertexbufferLine* getEntityVertexbufferLine(const std::string& strEntityName);

		// Returns whether the named vertex buffer line entity exists or not
		bool getEntityVertexbufferLineExists(const std::string& strEntityName);

		// Attempts to remove the named entity from the scene
		// If the entity doesn't exist, this silently fails
		void removeEntityVertexbufferLine(const std::string& strEntityName);

		// Removes all entites of type vertex buffer
		void removeAllEnititiesVertexbufferLine(void);

	private:
		std::map<std::string, SceneManagerEntityVertexbuffer*>		mmapEntitiesVertexbuffer;		// Each named vertex buffer entity in this scene.
		std::map<std::string, SceneManagerEntityVertexbufferLine*>	mmapEntitiesVertexbufferLine;	// Each named vertex buffer line entity in this scene.
		std::map<std::string, SceneManagerMaterial*>				mmapMaterials;					// Each named material in this scene.

		// Renders the vertex buffer entities contained in this scene
		void _renderVertexbufferEntities(void);

		// Renders the vertex buffer line entities contained in this scene
		void _renderVertexbufferLineEntities(void);
	};
}