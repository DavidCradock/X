#include "PCH.h"
#include "sceneManagerSimple.h"
#include "log.h"
#include "resourceManager.h"
#include "window.h"

namespace X
{
	SceneManagerSimple::SceneManagerSimple()
	{
		mCamera.setViewAsLookat(glm::vec3(10.0f, 10.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	}

	void SceneManagerSimple::render(void)
	{
		// Camera
		Window* pWindow = Window::getPointer();
		mCamera.setProjectionAsPerspective(55.0f, (float)pWindow->getWidth(), (float)pWindow->getHeight(), 1.0f, 10000.0f);

		ResourceManager* pRM = ResourceManager::getPointer();	// Resource manager
		ResourceShader* pShader = pRM->getShader("X:DRNE");		// Shader
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

		// Set projection and view matrixs from the camera
		pShader->setMat4("matrixProjection", mCamera.matrixProjection);
		pShader->setMat4("matrixView", mCamera.matrixView);

		// Set light uniforms
		pShader->setVec3("v3LightDirectionalColour", mvLightDirectional.mvColour);
		pShader->setVec3("v3LightDirectionalDirection", mvLightDirectional.mvDirection);
		pShader->setVec3("v3LightPointColour0", mvLightPoint0.mvColour);
		pShader->setVec3("v3LightPointPosition0", mvLightPoint0.mvPosition);

		// Set camera uniforms
		// Get the world matrix and then invert it, so it's actually in world space instead of reversed,
		// Then get the position from that inverted matrix
		glm::mat4 matViewInverse = glm::inverse(mCamera.matrixView);
		glm::vec3 cameraWorldPos = matViewInverse * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		pShader->setVec3("v3CameraPositionWorld", cameraWorldPos);

		// Vertex buffer entities
		std::map<std::string, SceneManagerEntityVertexbuffer*>::iterator it = mmapEnititiesVertexbuffer.begin();
		while (it != mmapEnititiesVertexbuffer.end())
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

		// Unbind everything
		if (pTexDiffuse)	pTexDiffuse->unbind(0);
		if (pTexRoughness)	pTexRoughness->unbind(1);
		if (pTexNormal)		pTexNormal->unbind(2);
		if (pTexEmission)	pTexEmission->unbind(3);
		
		pShader->unbind();
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
		mmapEnititiesVertexbuffer[strEntityName] = pNewEntity;
		return pNewEntity;
	}

	bool SceneManagerSimple::getEntityVertexbufferExists(const std::string& strEntityName)
	{
		std::map<std::string, SceneManagerEntityVertexbuffer*>::iterator it = mmapEnititiesVertexbuffer.find(strEntityName);
		return it != mmapEnititiesVertexbuffer.end();
	}

	void SceneManagerSimple::removeEntityVertexbuffer(const std::string& strEntityName)
	{
		std::map<std::string, SceneManagerEntityVertexbuffer*>::iterator it = mmapEnititiesVertexbuffer.find(strEntityName);
		if (it == mmapEnititiesVertexbuffer.end())
			return;	// Doesn't exist.
		delete it->second;
		mmapEnititiesVertexbuffer.erase(it);
	}

	void SceneManagerSimple::removeAllEnititiesVertexbuffer(void)
	{
		std::map<std::string, SceneManagerEntityVertexbuffer*>::iterator it = mmapEnititiesVertexbuffer.begin();
		while (it != mmapEnititiesVertexbuffer.end())
		{
			delete it->second;
			it++;
		}
		mmapEnititiesVertexbuffer.clear();
	}
}