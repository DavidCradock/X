#pragma once
#include "PCH.h"
#include "SMLightDirectional.h"
#include "SMLightPoint.h"
#include "SMEntityTriangle.h"
#include "SMEntityLine.h"
#include "SMCamera.h"
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

		SMCamera mCamera;	// The scene manager's camera

		SMLightDirectional mvLightDirectional;	// The directional light for this scene
		SMLightPoint mvLightPoint[4];			// The point lights for this scene
		int miNumPointLights;					// The number of point lights to use. Can be between 0 and 4

		// Adds a new uniquely named material which the entity vertex buffers use to render theirselves with.
		// If the named material already exists, an exception occurs.
		SMMaterial* addMaterial(
			const std::string strMaterialName,		// The unique name of this material
			float fAmbientStrength = 0.05f,										// Ambient strength
			const std::string& strTextureNameDiffuse = "X:default_diffuse",		// The texture resource located in the ResourceManager used for the diffuse colour
			const std::string& strTextureNameRoughness = "X:default_roughness",	// The texture resource located in the ResourceManager used for the roughness
			float fSpecularStrength = 0.15f,									// Specular strength
			const std::string& strTextureNameNormal = "X:default_normal",		// The texture resource located in the ResourceManager used for the normal map
			const std::string& strTextureNameEmission = "X:default_emission"	// The texture resource located in the ResourceManager used for the emission
		);
		
		// Returns a pointer to the named material
		// If the named material doesn't exist, an exception occurs
		SMMaterial* getMaterial(const std::string& strMaterialName);

		// Returns whether the named material exists or not
		bool getMaterialExists(const std::string& strMaterialName);

		// Attempts to remove the named material from the scene manager
		// If the material doesn't exist, this silently fails
		void removeMaterial(const std::string& strMaterialName);

		// Removes all materials
		void removeAllMaterials(void);


		// Adds a new uniquely named triangle entity to this scene and returns a pointer to it if needed
		// If the named entity already exists, an exception occurs
		SMEntityTriangle* addEntityTriangle(
			const std::string& strEntityName,				// The unique name of this entity
			const std::string& strTriangleName,				// The triangle resource located in the ResourceManager used when rendering this entity
			const std::string& strMaterialName				// The material in the scene manager to use whilst rendering the entity
		);

		// Returns a pointer to the named triangle entity.
		// If the enitity doesn't exist, an exception occurs.
		SMEntityTriangle* getEntityTriangle(const std::string& strEntityName);

		// Returns whether the named triangle entity exists or not
		bool getEntityTriangleExists(const std::string& strEntityName);

		// Attempts to remove the named entity from the scene
		// If the entity doesn't exist, this silently fails
		void removeEntityTriangle(const std::string& strEntityName);

		// Removes all entites of type triangle
		void removeAllEnititiesTriangle(void);

		// Adds a new uniquely named line entity to this scene and returns a pointer to it if needed
		// If the named entity already exists, an exception occurs
		SMEntityLine* addEntityLine(
			const std::string& strEntityName,						// The unique name of this entity
			const std::string& strLineName,							// The line resource located in the ResourceManager used when rendering this entity
			const std::string& strTextureName = "X:default_white"		// The texture resource located in the ResourceManager used for the colour
		);

		// Returns a pointer to the named line entity.
		// If the enitity doesn't exist, an exception occurs.
		SMEntityLine* getEntityLine(const std::string& strEntityName);

		// Returns whether the named line entity exists or not
		bool getEntityLineExists(const std::string& strEntityName);

		// Attempts to remove the named entity from the scene
		// If the entity doesn't exist, this silently fails
		void removeEntityLine(const std::string& strEntityName);

		// Removes all entites of type line
		void removeAllEntitiesLine(void);

	private:
		std::map<std::string, SMEntityTriangle*>	mmapEntitiesTriangles;	// Each named triangle entity in this scene.
		std::map<std::string, SMEntityLine*>		mmapEntitiesLine;		// Each named line entity in this scene.
		std::map<std::string, SMMaterial*>			mmapMaterials;			// Each named material in this scene.

		// Renders the triangle entities contained in this scene
		void _renderTriangleEntities(void);

		// Renders the line entities contained in this scene
		void _renderLineEntities(void);
	};
}