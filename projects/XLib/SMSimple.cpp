#include "PCH.h"
#include "SMSimple.h"
#include "log.h"
#include "resourceManager.h"
#include "window.h"

namespace X
{
	SceneManagerSimple::SceneManagerSimple()
	{
		mCamera.setViewAsLookat(glm::vec3(10.0f, 10.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		miNumPointLights = 0;

		// Shadow map stuff
		mbShadowsCastFromDirectionalLight = true;
		glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 100.0f);
		glm::mat4 lightView = glm::lookAt(glm::vec3(0.0f, 90.0f, 0.0f),	// Eye
			glm::vec3(0.0f, 0.0f, 0.0f),	// Target
			glm::vec3(0.0f, 1.0f, 0.0f));	// Up
		mmatShadowsDirectionalLightViewProj = lightProjection * lightView;
	}

	void SceneManagerSimple::render(void)
	{
		// Camera
		Window* pWindow = Window::getPointer();
		mCamera.setProjectionAsPerspective(65.0f, (float)pWindow->getWidth(), (float)pWindow->getHeight(), 1.0f, 10000.0f);

		// Render depth map for directional light
		if (mbShadowsCastFromDirectionalLight)
			_renderDepthmapForDirectionalLight();

		// Render the triangle entities
		_renderTriangleEntities();

		// Render the line entities
		_renderLineEntities();


	}

	SMMaterial* SceneManagerSimple::addMaterial(
		const std::string strMaterialName,				// The unique name of this material
		float fAmbientStrength,							// Ambient strength
		const std::string& strTextureNameDiffuse,		// The texture resource located in the ResourceManager used for the diffuse colour
		const std::string& strTextureNameRoughness,		// The texture resource located in the ResourceManager used for the roughness
		float fSpecularStrength,						// Specular strength
		const std::string& strTextureNameNormal,		// The texture resource located in the ResourceManager used for the normal map
		const std::string& strTextureNameEmission		// The texture resource located in the ResourceManager used for the emission
	)
	{
		// Make sure the entity doesn't already exist
		ThrowIfTrue(getMaterialExists(strMaterialName), "SceneManagerSimple::addMaterial(" + strMaterialName + ") failed. The named material already exists.");

		// Allocate the new entity
		SMMaterial* pNewMaterial = new SMMaterial;
		ThrowIfFalse(pNewMaterial, "SceneManagerSimple::addMaterial(" + strMaterialName + ") failed. Unable to allocate memory for the new material.");

		// Set passed values
		pNewMaterial->mstrTextureNameDiffuse = strTextureNameDiffuse;
		pNewMaterial->mstrTextureNameEmission = strTextureNameEmission;
		pNewMaterial->mstrTextureNameNormalmap = strTextureNameNormal;
		pNewMaterial->mstrTextureNameRoughness = strTextureNameRoughness;
		pNewMaterial->mfSpecularStrength = fSpecularStrength;
		pNewMaterial->mfAmbientStrength = fAmbientStrength;

		// Place in the hashmap
		mmapMaterials[strMaterialName] = pNewMaterial;
		return pNewMaterial;
	}

	SMMaterial* SceneManagerSimple::getMaterial(const std::string& strMaterialName)
	{
		std::map<std::string, SMMaterial*>::iterator it = mmapMaterials.find(strMaterialName);
		ThrowIfTrue(it == mmapMaterials.end(), "SceneManagerSimple::getMaterial(" + strMaterialName + ") failed. Material doesn't exist.");
		return it->second;
	}

	bool SceneManagerSimple::getMaterialExists(const std::string& strMaterialName)
	{
		std::map<std::string, SMMaterial*>::iterator it = mmapMaterials.find(strMaterialName);
		return it != mmapMaterials.end();
	}

	void SceneManagerSimple::removeMaterial(const std::string& strMaterialName)
	{
		std::map<std::string, SMMaterial*>::iterator it = mmapMaterials.find(strMaterialName);
		if (it == mmapMaterials.end())
			return;	// Doesn't exist.
		delete it->second;
		mmapMaterials.erase(it);
	}

	void SceneManagerSimple::removeAllMaterials(void)
	{
		std::map<std::string, SMMaterial*>::iterator it = mmapMaterials.begin();
		while (it != mmapMaterials.end())
		{
			delete it->second;
			it++;
		}
		mmapMaterials.clear();
	}

	SMEntityTriangle* SceneManagerSimple::addEntityTriangle(
		const std::string& strEntityName,			// The unique name of this entity
		const std::string& strTriangleName,			// The triangle resource located in the ResourceManager used when rendering this entity
		const std::string& strMaterialName)			// Material name (added to scene manager) which this entity uses for rendering
	{
		// Make sure the entity doesn't already exist
		ThrowIfTrue(getEntityTriangleExists(strEntityName), "SceneManagerSimple::addEntityTriangle(" + strEntityName + ") failed. The named entity already exists.");

		// Allocate the new entity
		SMEntityTriangle* pNewEntity = new SMEntityTriangle;
		ThrowIfFalse(pNewEntity, "SceneManagerSimple::addEntityTriangle(" + strEntityName + ") failed. Unable to allocate memory for the new entity.");

		// Set passed values
		pNewEntity->mstrTriangleName = strTriangleName;
		pNewEntity->mstrMaterialName = strMaterialName;

		// Place in the hashmap
		mmapEntitiesTriangles[strEntityName] = pNewEntity;
		return pNewEntity;
	}

	SMEntityTriangle* SceneManagerSimple::getEntityTriangle(const std::string& strEntityName)
	{
		std::map<std::string, SMEntityTriangle*>::iterator it = mmapEntitiesTriangles.find(strEntityName);
		ThrowIfTrue(it == mmapEntitiesTriangles.end(), "SceneManagerSimple::getEntityTriangle(" + strEntityName + ") failed. Entity doesn't exist.");
		return it->second;
	}

	bool SceneManagerSimple::getEntityTriangleExists(const std::string& strEntityName)
	{
		std::map<std::string, SMEntityTriangle*>::iterator it = mmapEntitiesTriangles.find(strEntityName);
		return it != mmapEntitiesTriangles.end();
	}

	void SceneManagerSimple::removeEntityTriangle(const std::string& strEntityName)
	{
		std::map<std::string, SMEntityTriangle*>::iterator it = mmapEntitiesTriangles.find(strEntityName);
		if (it == mmapEntitiesTriangles.end())
			return;	// Doesn't exist.
		delete it->second;
		mmapEntitiesTriangles.erase(it);
	}

	void SceneManagerSimple::removeAllEnititiesTriangle(void)
	{
		std::map<std::string, SMEntityTriangle*>::iterator it = mmapEntitiesTriangles.begin();
		while (it != mmapEntitiesTriangles.end())
		{
			delete it->second;
			it++;
		}
		mmapEntitiesTriangles.clear();
	}

	SMEntityLine* SceneManagerSimple::addEntityLine(const std::string& strEntityName, const std::string& strLineName,	const std::string& strTextureName)
	{
		// Make sure the entity doesn't already exist
		ThrowIfTrue(getEntityLineExists(strEntityName), "SceneManagerSimple::addEntityLine(" + strEntityName + ") failed. The named entity already exists.");

		// Allocate the new entity
		SMEntityLine* pNewEntity = new SMEntityLine;
		ThrowIfFalse(pNewEntity, "SceneManagerSimple::addEntityLine(" + strEntityName + ") failed. Unable to allocate memory for the new entity.");

		// Set passed values
		pNewEntity->mstrLineName = strLineName;
		pNewEntity->mstrTextureName = strTextureName;

		// Place in the hashmap
		mmapEntitiesLine[strEntityName] = pNewEntity;
		return pNewEntity;
	}

	SMEntityLine* SceneManagerSimple::getEntityLine(const std::string& strEntityName)
	{
		std::map<std::string, SMEntityLine*>::iterator it = mmapEntitiesLine.find(strEntityName);
		ThrowIfTrue(it == mmapEntitiesLine.end(), "SceneManagerSimple::getEntityLine(" + strEntityName + ") failed. Entity doesn't exist.");
		return it->second;
	}

	bool SceneManagerSimple::getEntityLineExists(const std::string& strEntityName)
	{
		std::map<std::string, SMEntityLine*>::iterator it = mmapEntitiesLine.find(strEntityName);
		return it != mmapEntitiesLine.end();
	}

	void SceneManagerSimple::removeEntityLine(const std::string& strEntityName)
	{
		std::map<std::string, SMEntityLine*>::iterator it = mmapEntitiesLine.find(strEntityName);
		if (it == mmapEntitiesLine.end())
			return;	// Doesn't exist.
		delete it->second;
		mmapEntitiesLine.erase(it);
	}

	void SceneManagerSimple::removeAllEntitiesLine(void)
	{
		std::map<std::string, SMEntityLine*>::iterator it = mmapEntitiesLine.begin();
		while (it != mmapEntitiesLine.end())
		{
			delete it->second;
			it++;
		}
		mmapEntitiesLine.clear();
	}

	void SceneManagerSimple::_renderTriangleEntities(void)
	{
		ResourceManager* pRM = ResourceManager::getPointer();
		ResourceShader* pShader = pRM->getShader("X:shader_DRNE");		// Shader used to render the vertex buffer entities
		ResourceTriangle* pResTri;
		ResourceTexture2D* pTexDiffuse = 0;
		ResourceTexture2D* pTexRoughness = 0;
		ResourceTexture2D* pTexNormal = 0;
		ResourceTexture2D* pTexEmission = 0;

		pShader->bind();

		// Tell OpenGL, for each sampler, to which texture unit it belongs to
		pShader->setInt("texture0_diffuse", 0);
		pShader->setInt("texture1_roughness", 1);
		pShader->setInt("texture2_normal", 2);
		pShader->setInt("texture3_emission", 3);

		// Set blending/depth testing
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);

		// Culling
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		// Set projection and view matrixs from the camera
		pShader->setMat4("matrixProjection", mCamera.matrixProjection);
		pShader->setMat4("matrixView", mCamera.matrixView);

		// Set directional light uniforms
		pShader->setVec3("v3LightDirectionalColour", mvLightDirectional.mvColour);
		pShader->setVec3("v3LightDirectionalDirection", mvLightDirectional.mvDirection);

		// Set point light uniforms
		pShader->setInt("iLightPointNumber", miNumPointLights);	// Number of point lights to use
		if (miNumPointLights > 0)	// If at least 1 point light is to be used
		{
			// Set 1st point light settings
			pShader->setVec3("pointLights[0].v3Colour", mvLightPoint[0].mvColour);
			pShader->setVec3("pointLights[0].v3Position", mvLightPoint[0].mvPosition);
			pShader->setFloat("pointLights[0].fSpecularAttenuationLinear", mvLightPoint[0].mfSpecularAttenuationLinear);
			pShader->setFloat("pointLights[0].fSpecularAttenuationQuadratic", mvLightPoint[0].mfSpecularAttenuationQuadratic);
			if (miNumPointLights > 1)	// If at least 2 point lights are to be used
			{
				// Set 2nd point light settings
				pShader->setVec3("pointLights[1].v3Colour", mvLightPoint[1].mvColour);
				pShader->setVec3("pointLights[1].v3Position", mvLightPoint[1].mvPosition);
				pShader->setFloat("pointLights[1].fSpecularAttenuationLinear", mvLightPoint[1].mfSpecularAttenuationLinear);
				pShader->setFloat("pointLights[1].fSpecularAttenuationQuadratic", mvLightPoint[1].mfSpecularAttenuationQuadratic);
				if (miNumPointLights > 2)	// If at least 3 point lights are to be used
				{
					// Set 3rd point light settings
					pShader->setVec3("pointLights[2].v3Colour", mvLightPoint[2].mvColour);
					pShader->setVec3("pointLights[2].v3Position", mvLightPoint[2].mvPosition);
					pShader->setFloat("pointLights[2].fSpecularAttenuationLinear", mvLightPoint[2].mfSpecularAttenuationLinear);
					pShader->setFloat("pointLights[2].fSpecularAttenuationQuadratic", mvLightPoint[2].mfSpecularAttenuationQuadratic);
					if (miNumPointLights > 3)	// If at least 4 point lights are to be used
					{
						// Set 4th point light settings
						pShader->setVec3("pointLights[3].v3Colour", mvLightPoint[3].mvColour);
						pShader->setVec3("pointLights[3].v3Position", mvLightPoint[3].mvPosition);
						pShader->setFloat("pointLights[3].fSpecularAttenuationLinear", mvLightPoint[3].mfSpecularAttenuationLinear);
						pShader->setFloat("pointLights[3].fSpecularAttenuationQuadratic", mvLightPoint[3].mfSpecularAttenuationQuadratic);
					}
				}
			}
		}

		// Set camera uniforms
		// Get the world matrix and then invert it, so it's actually in world space instead of reversed,
		// Then get the position from that inverted matrix
		glm::mat4 matViewInverse = glm::inverse(mCamera.matrixView);
		glm::vec3 cameraWorldPos = matViewInverse * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		pShader->setVec3("v3CameraPositionWorld", cameraWorldPos);

		// Vertex buffer entities
		std::map<std::string, SMEntityTriangle*>::iterator it = mmapEntitiesTriangles.begin();
		while (it != mmapEntitiesTriangles.end())
		{
			// Get entity material
			SMMaterial* pMaterial = getMaterial(it->second->mstrMaterialName);

			// Get vertex buffer and textures used by each entity
			pResTri = pRM->getTriangle(it->second->mstrTriangleName);
			pTexDiffuse = pRM->getTexture2D(pMaterial->mstrTextureNameDiffuse);
			pTexRoughness = pRM->getTexture2D(pMaterial->mstrTextureNameRoughness);
			pTexNormal = pRM->getTexture2D(pMaterial->mstrTextureNameNormalmap);
			pTexEmission = pRM->getTexture2D(pMaterial->mstrTextureNameEmission);

			// Bind each texture to each sampler unit
			pTexDiffuse->bind(0);
			pTexRoughness->bind(1);
			pTexNormal->bind(2);
			pTexEmission->bind(3);

			// Set world matrix (Projection and View set above)
			pShader->setMat4("matrixWorld", it->second->getWorldMatrix());

			// Ambient and specular uniforms
			pShader->setFloat("fAmbientStrength", pMaterial->mfAmbientStrength);
			pShader->setFloat("fSpecularStrength", pMaterial->mfSpecularStrength);

			// Render the vertex buffer
			pResTri->draw(false);
			it++;
		}

		// Unbind everything (We check if not NULL, as perhaps, there are no entities added to scene manager, in which case, no texture will be retrieved above
		if (pTexDiffuse)	pTexDiffuse->unbind(0);
		if (pTexRoughness)	pTexRoughness->unbind(1);
		if (pTexNormal)		pTexNormal->unbind(2);
		if (pTexEmission)	pTexEmission->unbind(3);

		glDisable(GL_CULL_FACE);

		pShader->unbind();
	}

	void SceneManagerSimple::_renderLineEntities(void)
	{
		ResourceManager* pRM = ResourceManager::getPointer();
		ResourceShader* pShader = pRM->getShader("X:shader_line");		// Shader used to render the vertex buffer line entities
		ResourceLine* pLine;
		ResourceTexture2D* pTexColour = 0;

		pShader->bind();

		// Tell OpenGL, for each sampler, to which texture unit it belongs to
		pShader->setInt("texture0_colour", 0);

		// Set blending/depth testing
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);

		// Culling
		glDisable(GL_CULL_FACE);
//		glCullFace(GL_BACK);

		// Set projection and view matrixs from the camera
		pShader->setMat4("matrixProjection", mCamera.matrixProjection);
		pShader->setMat4("matrixView", mCamera.matrixView);

		// Vertex buffer line entities
		std::map<std::string, SMEntityLine*>::iterator it = mmapEntitiesLine.begin();
		while (it != mmapEntitiesLine.end())
		{
			// Get vertex buffer and textures used by each entity
			pLine = pRM->getLine(it->second->mstrLineName);
			pTexColour = pRM->getTexture2D(it->second->mstrTextureName);

			// Bind texture to sampler unit
			pTexColour->bind(0);

			// Set world matrix (Projection and View set above)
			pShader->setMat4("matrixWorld", it->second->getWorldMatrix());

			// Render the vertex buffer
			pLine->draw();
			it++;
		}

		// Unbind everything (We check if not NULL, as perhaps, there are no entities added to scene manager, in which case, no texture will be retrieved above
		if (pTexColour)	pTexColour->unbind(0);

		pShader->unbind();
	}

	void SceneManagerSimple::_renderDepthmapForDirectionalLight(void)
	{

	}
}