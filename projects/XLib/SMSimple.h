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
	// Shader rendering is a little bit finicky.
	// We need to make sure it's setup right depending upon the scene, the camera position etc.
	// When updating the camera, it's a good idea to call setDirectionalLightProjection() to update the light position etc.
	// Point lights do not cast shadows, it would be too much of a performance hit.
	class CSceneManagerSimple
	{
	public:
		CSceneManagerSimple();

		// Render all the entities in this scene to a framebuffer
		void render(const std::string strFramebufferToRenderTo = "X:backbuffer_FB", bool bResizeFramebufferToWindowDims = false);

		CSMCamera mCamera;						// The scene manager's camera
		
		CSMLightPoint mvLightPoint[4];			// The point lights for this scene
		int miNumPointLights;					// The number of point lights to use. Can be between 0 and 4

		// Adds a new uniquely named material which the entity vertex buffers use to render theirselves with.
		// If the named material already exists, an exception occurs.
		CSMMaterial* addMaterial(
			const std::string strMaterialName,		// The unique name of this material
			float fAmbientStrength = 0.05f,										// Ambient strength
			const std::string& strTextureNameDiffuse = "X:default_diffuse",		// The texture resource located in the SCResourceManager used for the diffuse colour
			const std::string& strTextureNameRoughness = "X:default_roughness",	// The texture resource located in the SCResourceManager used for the roughness
			float fSpecularStrength = 0.5f,										// Specular strength
			const std::string& strTextureNameNormal = "X:default_normal",		// The texture resource located in the SCResourceManager used for the normal map
			const std::string& strTextureNameEmission = "X:default_emission"	// The texture resource located in the SCResourceManager used for the emission
		);
		
		// Returns a pointer to the named material
		// If the named material doesn't exist, an exception occurs
		CSMMaterial* getMaterial(const std::string& strMaterialName) const;

		// Returns whether the named material exists or not
		bool getMaterialExists(const std::string& strMaterialName) const;

		// Attempts to remove the named material from the scene manager
		// If the material doesn't exist, this silently fails
		void removeMaterial(const std::string& strMaterialName);

		// Removes all materials
		void removeAllMaterials(void);

		// Adds a new uniquely named triangle entity to this scene and returns a pointer to it if needed
		// If the named entity already exists, an exception occurs
		CSMEntityTriangle* addEntityTriangle(
			const std::string& strEntityName,				// The unique name of this entity
			const std::string& strTriangleName,				// The triangle resource located in the SCResourceManager used when rendering this entity
			const std::string& strMaterialName				// The material in the scene manager to use whilst rendering the entity
		);

		// Returns a pointer to the named triangle entity.
		// If the enitity doesn't exist, an exception occurs.
		CSMEntityTriangle* getEntityTriangle(const std::string& strEntityName) const;

		// Returns whether the named triangle entity exists or not
		bool getEntityTriangleExists(const std::string& strEntityName) const;

		// Attempts to remove the named entity from the scene
		// If the entity doesn't exist, this silently fails
		void removeEntityTriangle(const std::string& strEntityName);

		// Removes all entites of type triangle
		void removeAllEnititiesTriangle(void);

		// Adds a new uniquely named line entity to this scene and returns a pointer to it if needed
		// If the named entity already exists, an exception occurs
		CSMEntityLine* addEntityLine(
			const std::string& strEntityName,						// The unique name of this entity
			const std::string& strLineName,							// The line resource located in the SCResourceManager used when rendering this entity
			const std::string& strTextureName = "X:default_white"	// The texture resource located in the SCResourceManager used for the colour
		);

		// Returns a pointer to the named line entity.
		// If the enitity doesn't exist, an exception occurs.
		CSMEntityLine* getEntityLine(const std::string& strEntityName) const;

		// Returns whether the named line entity exists or not
		bool getEntityLineExists(const std::string& strEntityName) const;

		// Attempts to remove the named entity from the scene
		// If the entity doesn't exist, this silently fails
		void removeEntityLine(const std::string& strEntityName);

		// Removes all entites of type line
		void removeAllEntitiesLine(void);

		// Set whether to render shadows from the directional light or not
		void setShadowsEnabled(bool bShadowsEnabled = true);

		// Returns whether rendering of shadows from directional light is enabled or not
		bool getShadowsEnabled(void) const;

		// Sets the directional light's parameters for it's projection matrix and position.
		// Even though the directional light doesn't have a position, just a direction, it still needs a position to be able to work.
		void setDirectionalLightProjection(float fProjMatWidth = 30.0f, float fProjMatHeight = 30.0f, float fProjMatNear = 5.0f, float fProjMatFar = 100.0f, glm::vec3 v3LightPosition = glm::vec3(10, 10, 10));

		// Sets the directional light's direction
		void setDirectionalLightDir(glm::vec3 vDirection);

		// Sets the directional light's colour
		void setDirectionalLightColour(glm::vec3 vColour);
	private:
		mutable std::map<std::string, CSMEntityTriangle*>	mmapEntitiesTriangles;	// Each named triangle entity in this scene.
		mutable std::map<std::string, CSMEntityLine*>		mmapEntitiesLine;		// Each named line entity in this scene.
		mutable std::map<std::string, CSMMaterial*>			mmapMaterials;			// Each named material in this scene.
		
		// Variables used for rendering the depth map for the directional light
		bool mbShadowsCastFromDirectionalLight;			// Whether to cast shadows from the directional light
		glm::mat4 mmatShadowsDirectionalLightViewProj;	// The view projection matrix for the directional light
		glm::mat4 _mmatDirectionallightProjection;		// Projection matrix used for rendering the depth buffer for the directional light
		glm::vec3 _mv3DirectionLightPosition;			// Position used during lookat to determine light's position and lookat target
		CSMLightDirectional mvLightDirectional;			// The directional light for this scene
		
		// Renders the shadow depth map for the directional light
		void _renderDepthmapForDirectionalLight(void);

		// Renders the triangle entities contained in this scene
		void _renderTriangleEntities(void);

		// Renders the line entities contained in this scene
		void _renderLineEntities(void);
	};
}