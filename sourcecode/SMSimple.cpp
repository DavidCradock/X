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
		miNumPointLights = 3;
	}

	void SceneManagerSimple::render(void)
	{
		// Camera
		Window* pWindow = Window::getPointer();
		mCamera.setProjectionAsPerspective(55.0f, (float)pWindow->getWidth(), (float)pWindow->getHeight(), 1.0f, 10000.0f);

		// First render the vertex buffer entities
		_renderVertexbufferEntities();

		// Next render the vertex buffer line entities
		_renderVertexbufferLineEntities();


	}

	SceneManagerEntityVertexbuffer* SceneManagerSimple::addEntityVertexbuffer(
		const std::string& strEntityName,			// The unique name of this entity
		const std::string& strVertexbufferName,		// The vertex buffer resource located in the ResourceManager used when rendering this entity
		float fAmbientStrength,						//
		const std::string& strTextureNameDiffuse,	// The texture resource located in the ResourceManager used for the diffuse colour
		const std::string& strTextureNameRoughness,	// The texture resource located in the ResourceManager used for the roughness
		float fSpecularStrength,					//
		const std::string& strTextureNameNormal,	// The texture resource located in the ResourceManager used for the normal map
		const std::string& strTextureNameEmission)	// The texture resource located in the ResourceManager used for the emission
	{
		// Make sure the entity doesn't already exist
		ThrowIfTrue(getEntityVertexbufferExists(strEntityName), "SceneManagerSimple::addEntityVertexbuffer(" + strEntityName + ") failed. The named entity already exists.");

		// Allocate the new entity
		SceneManagerEntityVertexbuffer* pNewEntity = new SceneManagerEntityVertexbuffer;
		ThrowIfFalse(pNewEntity, "SceneManagerSimple::addEntityVertexbuffer(" + strEntityName + ") failed. Unable to allocate memory for the new entity.");

		// Set passed values
		pNewEntity->mstrVertexbufferName = strVertexbufferName;
		pNewEntity->mstrTextureNameDiffuse = strTextureNameDiffuse;
		pNewEntity->mstrTextureNameEmission = strTextureNameEmission;
		pNewEntity->mstrTextureNameNormalmap = strTextureNameNormal;
		pNewEntity->mstrTextureNameRoughness = strTextureNameRoughness;
		pNewEntity->mfSpecularStrength = fSpecularStrength;
		pNewEntity->mfAmbientStrength = fAmbientStrength;

		// Place in the hashmap
		mmapEntitiesVertexbuffer[strEntityName] = pNewEntity;
		return pNewEntity;
	}

	bool SceneManagerSimple::getEntityVertexbufferExists(const std::string& strEntityName)
	{
		std::map<std::string, SceneManagerEntityVertexbuffer*>::iterator it = mmapEntitiesVertexbuffer.find(strEntityName);
		return it != mmapEntitiesVertexbuffer.end();
	}

	void SceneManagerSimple::removeEntityVertexbuffer(const std::string& strEntityName)
	{
		std::map<std::string, SceneManagerEntityVertexbuffer*>::iterator it = mmapEntitiesVertexbuffer.find(strEntityName);
		if (it == mmapEntitiesVertexbuffer.end())
			return;	// Doesn't exist.
		delete it->second;
		mmapEntitiesVertexbuffer.erase(it);
	}

	void SceneManagerSimple::removeAllEnititiesVertexbuffer(void)
	{
		std::map<std::string, SceneManagerEntityVertexbuffer*>::iterator it = mmapEntitiesVertexbuffer.begin();
		while (it != mmapEntitiesVertexbuffer.end())
		{
			delete it->second;
			it++;
		}
		mmapEntitiesVertexbuffer.clear();
	}

	SceneManagerEntityVertexbufferLine* SceneManagerSimple::addEntityVertexbufferLine(const std::string& strEntityName, const std::string& strVertexbufferName,	const std::string& strTextureName)
	{
		// Make sure the entity doesn't already exist
		ThrowIfTrue(getEntityVertexbufferLineExists(strEntityName), "SceneManagerSimple::addEntityVertexbufferLine(" + strEntityName + ") failed. The named entity already exists.");

		// Allocate the new entity
		SceneManagerEntityVertexbufferLine* pNewEntity = new SceneManagerEntityVertexbufferLine;
		ThrowIfFalse(pNewEntity, "SceneManagerSimple::addEntityVertexbufferLine(" + strEntityName + ") failed. Unable to allocate memory for the new entity.");

		// Set passed values
		pNewEntity->mstrVertexbufferName = strVertexbufferName;
		pNewEntity->mstrTextureName = strTextureName;

		// Place in the hashmap
		mmapEntitiesVertexbufferLine[strEntityName] = pNewEntity;
		return pNewEntity;
	}

	bool SceneManagerSimple::getEntityVertexbufferLineExists(const std::string& strEntityName)
	{
		std::map<std::string, SceneManagerEntityVertexbufferLine*>::iterator it = mmapEntitiesVertexbufferLine.find(strEntityName);
		return it != mmapEntitiesVertexbufferLine.end();
	}

	void SceneManagerSimple::removeEntityVertexbufferLine(const std::string& strEntityName)
	{
		std::map<std::string, SceneManagerEntityVertexbufferLine*>::iterator it = mmapEntitiesVertexbufferLine.find(strEntityName);
		if (it == mmapEntitiesVertexbufferLine.end())
			return;	// Doesn't exist.
		delete it->second;
		mmapEntitiesVertexbufferLine.erase(it);
	}

	void SceneManagerSimple::removeAllEnititiesVertexbufferLine(void)
	{
		std::map<std::string, SceneManagerEntityVertexbufferLine*>::iterator it = mmapEntitiesVertexbufferLine.begin();
		while (it != mmapEntitiesVertexbufferLine.end())
		{
			delete it->second;
			it++;
		}
		mmapEntitiesVertexbufferLine.clear();
	}

	void SceneManagerSimple::_renderVertexbufferEntities(void)
	{
		ResourceManager* pRM = ResourceManager::getPointer();
		ResourceShader* pShader = pRM->getShader("X:DRNE");		// Shader used to render the vertex buffer entities
		ResourceVertexbuffer* pVB;
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
		std::map<std::string, SceneManagerEntityVertexbuffer*>::iterator it = mmapEntitiesVertexbuffer.begin();
		while (it != mmapEntitiesVertexbuffer.end())
		{
			// Get vertex buffer and textures used by each entity
			pVB = pRM->getVertexbuffer(it->second->mstrVertexbufferName);
			pTexDiffuse = pRM->getTexture2D(it->second->mstrTextureNameDiffuse);
			pTexRoughness = pRM->getTexture2D(it->second->mstrTextureNameRoughness);
			pTexNormal = pRM->getTexture2D(it->second->mstrTextureNameNormalmap);
			pTexEmission = pRM->getTexture2D(it->second->mstrTextureNameEmission);

			// Bind each texture to each sampler unit
			pTexDiffuse->bind(0);
			pTexRoughness->bind(1);
			pTexNormal->bind(2);
			pTexEmission->bind(3);

			// Set world matrix (Projection and View set above)
			pShader->setMat4("matrixWorld", it->second->matrixWorld);

			// Ambient and specular uniforms
			pShader->setFloat("fAmbientStrength", it->second->mfAmbientStrength);
			pShader->setFloat("fSpecularStrength", it->second->mfSpecularStrength);


			// Render the vertex buffer
			pVB->draw(false);
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

	void SceneManagerSimple::_renderVertexbufferLineEntities(void)
	{
		ResourceManager* pRM = ResourceManager::getPointer();
		ResourceShader* pShader = pRM->getShader("X:line");		// Shader used to render the vertex buffer line entities
		ResourceVertexbufferLine* pVB;
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
		std::map<std::string, SceneManagerEntityVertexbufferLine*>::iterator it = mmapEntitiesVertexbufferLine.begin();
		while (it != mmapEntitiesVertexbufferLine.end())
		{
			// Get vertex buffer and textures used by each entity
			pVB = pRM->getVertexbufferLine(it->second->mstrVertexbufferName);
			pTexColour = pRM->getTexture2D(it->second->mstrTextureName);

			// Bind texture to sampler unit
			pTexColour->bind(0);

			// Set world matrix (Projection and View set above)
			pShader->setMat4("matrixWorld", it->second->matrixWorld);

			// Render the vertex buffer
			pVB->draw();
			it++;
		}

		// Unbind everything (We check if not NULL, as perhaps, there are no entities added to scene manager, in which case, no texture will be retrieved above
		if (pTexColour)	pTexColour->unbind(0);

		pShader->unbind();
	}
}