#include "PCH.h"
#include "SMSimple.h"
#include "log.h"
#include "resourceManager.h"
#include "window.h"

namespace X
{
	CSceneManagerSimple::CSceneManagerSimple()
	{
		mCamera.setViewAsLookat(glm::vec3(10.0f, 10.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		miNumPointLights = 0;
		mbShadowsCastFromDirectionalLight = true;
		setDirectionalLightProjection(30.0f, 30.0f, 5.0f, 100.0f, glm::vec3(10.0f, 10.0f, 10.0f));
		setDirectionalLightColour(glm::vec3(1.0f, 1.0f, 1.0f));
		setDirectionalLightDir(glm::vec3(1.0f, -1.0f, 1.0f));
	}

	void CSceneManagerSimple::render(const std::string strFramebufferToRenderTo, bool bResizeFramebufferToWindowDims)
	{
		SCResourceManager* pRM = SCResourceManager::getPointer();
		// The application manager has X:backbuffer_FB set to render target, so unbind this first
		CResourceFramebuffer* pBGFB = pRM->getFramebuffer("X:backbuffer_FB");
		pBGFB->unbindAsRenderTarget();

		// Camera
		CWindow* pWindow = CWindow::getPointer();
		mCamera.setProjectionAsPerspective(65.0f, (float)pWindow->getWidth(), (float)pWindow->getHeight(), 1.0f, 10000.0f);

		// Render depth map for directional light
		if (mbShadowsCastFromDirectionalLight)
			_renderDepthmapForDirectionalLight();

		// Bind the framebuffer we're rendering to
		CResourceFramebuffer* pFramebuffer = pRM->getFramebuffer(strFramebufferToRenderTo);
		pFramebuffer->bindAsRenderTarget(true, bResizeFramebufferToWindowDims);

		// Render the triangle entities
		_renderTriangleEntities();

		// Render the line entities
		_renderLineEntities();

		pFramebuffer->unbindAsRenderTarget();

		// Now re-set the X:backbuffer_FB framebuffer to be the render target again
		pBGFB->bindAsRenderTarget(false, false);
	}

	CSMMaterial* CSceneManagerSimple::addMaterial(
		const std::string strMaterialName,				// The unique name of this material
		float fAmbientStrength,							// Ambient strength
		const std::string& strTextureNameDiffuse,		// The texture resource located in the SCResourceManager used for the diffuse colour
		const std::string& strTextureNameRoughness,		// The texture resource located in the SCResourceManager used for the roughness
		float fSpecularStrength,						// Specular strength
		const std::string& strTextureNameNormal,		// The texture resource located in the SCResourceManager used for the normal map
		const std::string& strTextureNameEmission		// The texture resource located in the SCResourceManager used for the emission
	)
	{
		// Make sure the entity doesn't already exist
		ThrowIfTrue(getMaterialExists(strMaterialName), "CSceneManagerSimple::addMaterial(" + strMaterialName + ") failed. The named material already exists.");

		// Allocate the new entity
		CSMMaterial* pNewMaterial = new CSMMaterial;
		ThrowIfFalse(pNewMaterial, "CSceneManagerSimple::addMaterial(" + strMaterialName + ") failed. Unable to allocate memory for the new material.");

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

	CSMMaterial* CSceneManagerSimple::getMaterial(const std::string& strMaterialName)
	{
		std::map<std::string, CSMMaterial*>::iterator it = mmapMaterials.find(strMaterialName);
		ThrowIfTrue(it == mmapMaterials.end(), "CSceneManagerSimple::getMaterial(" + strMaterialName + ") failed. Material doesn't exist.");
		return it->second;
	}

	bool CSceneManagerSimple::getMaterialExists(const std::string& strMaterialName)
	{
		std::map<std::string, CSMMaterial*>::iterator it = mmapMaterials.find(strMaterialName);
		return it != mmapMaterials.end();
	}

	void CSceneManagerSimple::removeMaterial(const std::string& strMaterialName)
	{
		std::map<std::string, CSMMaterial*>::iterator it = mmapMaterials.find(strMaterialName);
		if (it == mmapMaterials.end())
			return;	// Doesn't exist.
		delete it->second;
		mmapMaterials.erase(it);
	}

	void CSceneManagerSimple::removeAllMaterials(void)
	{
		std::map<std::string, CSMMaterial*>::iterator it = mmapMaterials.begin();
		while (it != mmapMaterials.end())
		{
			delete it->second;
			it++;
		}
		mmapMaterials.clear();
	}

	CSMEntityTriangle* CSceneManagerSimple::addEntityTriangle(
		const std::string& strEntityName,			// The unique name of this entity
		const std::string& strTriangleName,			// The triangle resource located in the SCResourceManager used when rendering this entity
		const std::string& strMaterialName)			// Material name (added to scene manager) which this entity uses for rendering
	{
		// Make sure the entity doesn't already exist
		ThrowIfTrue(getEntityTriangleExists(strEntityName), "CSceneManagerSimple::addEntityTriangle(" + strEntityName + ") failed. The named entity already exists.");

		// Allocate the new entity
		CSMEntityTriangle* pNewEntity = new CSMEntityTriangle;
		ThrowIfFalse(pNewEntity, "CSceneManagerSimple::addEntityTriangle(" + strEntityName + ") failed. Unable to allocate memory for the new entity.");

		// Set passed values
		pNewEntity->mstrTriangleName = strTriangleName;
		pNewEntity->mstrMaterialName = strMaterialName;

		// Place in the hashmap
		mmapEntitiesTriangles[strEntityName] = pNewEntity;
		return pNewEntity;
	}

	CSMEntityTriangle* CSceneManagerSimple::getEntityTriangle(const std::string& strEntityName)
	{
		std::map<std::string, CSMEntityTriangle*>::iterator it = mmapEntitiesTriangles.find(strEntityName);
		ThrowIfTrue(it == mmapEntitiesTriangles.end(), "CSceneManagerSimple::getEntityTriangle(" + strEntityName + ") failed. Entity doesn't exist.");
		return it->second;
	}

	bool CSceneManagerSimple::getEntityTriangleExists(const std::string& strEntityName)
	{
		std::map<std::string, CSMEntityTriangle*>::iterator it = mmapEntitiesTriangles.find(strEntityName);
		return it != mmapEntitiesTriangles.end();
	}

	void CSceneManagerSimple::removeEntityTriangle(const std::string& strEntityName)
	{
		std::map<std::string, CSMEntityTriangle*>::iterator it = mmapEntitiesTriangles.find(strEntityName);
		if (it == mmapEntitiesTriangles.end())
			return;	// Doesn't exist.
		delete it->second;
		mmapEntitiesTriangles.erase(it);
	}

	void CSceneManagerSimple::removeAllEnititiesTriangle(void)
	{
		std::map<std::string, CSMEntityTriangle*>::iterator it = mmapEntitiesTriangles.begin();
		while (it != mmapEntitiesTriangles.end())
		{
			delete it->second;
			it++;
		}
		mmapEntitiesTriangles.clear();
	}

	CSMEntityLine* CSceneManagerSimple::addEntityLine(const std::string& strEntityName, const std::string& strLineName,	const std::string& strTextureName)
	{
		// Make sure the entity doesn't already exist
		ThrowIfTrue(getEntityLineExists(strEntityName), "CSceneManagerSimple::addEntityLine(" + strEntityName + ") failed. The named entity already exists.");

		// Allocate the new entity
		CSMEntityLine* pNewEntity = new CSMEntityLine;
		ThrowIfFalse(pNewEntity, "CSceneManagerSimple::addEntityLine(" + strEntityName + ") failed. Unable to allocate memory for the new entity.");

		// Set passed values
		pNewEntity->mstrLineName = strLineName;
		pNewEntity->mstrTextureName = strTextureName;

		// Place in the hashmap
		mmapEntitiesLine[strEntityName] = pNewEntity;
		return pNewEntity;
	}

	CSMEntityLine* CSceneManagerSimple::getEntityLine(const std::string& strEntityName)
	{
		std::map<std::string, CSMEntityLine*>::iterator it = mmapEntitiesLine.find(strEntityName);
		ThrowIfTrue(it == mmapEntitiesLine.end(), "CSceneManagerSimple::getEntityLine(" + strEntityName + ") failed. Entity doesn't exist.");
		return it->second;
	}

	bool CSceneManagerSimple::getEntityLineExists(const std::string& strEntityName)
	{
		std::map<std::string, CSMEntityLine*>::iterator it = mmapEntitiesLine.find(strEntityName);
		return it != mmapEntitiesLine.end();
	}

	void CSceneManagerSimple::removeEntityLine(const std::string& strEntityName)
	{
		std::map<std::string, CSMEntityLine*>::iterator it = mmapEntitiesLine.find(strEntityName);
		if (it == mmapEntitiesLine.end())
			return;	// Doesn't exist.
		delete it->second;
		mmapEntitiesLine.erase(it);
	}

	void CSceneManagerSimple::removeAllEntitiesLine(void)
	{
		std::map<std::string, CSMEntityLine*>::iterator it = mmapEntitiesLine.begin();
		while (it != mmapEntitiesLine.end())
		{
			delete it->second;
			it++;
		}
		mmapEntitiesLine.clear();
	}

	void CSceneManagerSimple::_renderTriangleEntities(void)
	{
		SCResourceManager* pRM = SCResourceManager::getPointer();
		CResourceShader* pShader;
		if (mbShadowsCastFromDirectionalLight)
			pShader = pRM->getShader("X:DRNE");
		else
			pShader = pRM->getShader("X:DRNE_noshadows");

		CResourceTriangle* pResTri;
		CResourceTexture2D* pTexDiffuse = 0;
		CResourceTexture2D* pTexRoughness = 0;
		CResourceTexture2D* pTexNormal = 0;
		CResourceTexture2D* pTexEmission = 0;

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

		// If we're rendering shadows, using the DRNE shader, set additional stuff for that
		CResourceDepthbuffer* pDepthbuffer = pRM->getDepthbuffer("X:shadows");
		if (mbShadowsCastFromDirectionalLight)
		{
			pShader->setMat4("matrixLightViewProjection", mmatShadowsDirectionalLightViewProj);
			pShader->setInt("texture4_depthmap", 4);
			pDepthbuffer->bindAsTexture(4);
		}

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

		// Triangle entities
		std::map<std::string, CSMEntityTriangle*>::iterator it = mmapEntitiesTriangles.begin();
		while (it != mmapEntitiesTriangles.end())
		{
			// Get entity material
			CSMMaterial* pMaterial = getMaterial(it->second->mstrMaterialName);

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

		
		if (mbShadowsCastFromDirectionalLight)
		{
			pDepthbuffer->unbindTexture(4);
		}

		glDisable(GL_CULL_FACE);
		pShader->unbind();
	}

	void CSceneManagerSimple::_renderLineEntities(void)
	{
		SCResourceManager* pRM = SCResourceManager::getPointer();
		CResourceShader* pShader = pRM->getShader("X:line");		// Shader used to render the vertex buffer line entities
		CResourceLine* pLine;
		CResourceTexture2D* pTexColour = 0;

		pShader->bind();

		// Tell OpenGL, for each sampler, to which texture unit it belongs to
		pShader->setInt("texture0_colour", 0);

		// Set blending/depth testing
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);

		// Culling
		glDisable(GL_CULL_FACE);

		// Set projection and view matrixs from the camera
		pShader->setMat4("matrixProjection", mCamera.matrixProjection);
		pShader->setMat4("matrixView", mCamera.matrixView);

		// Vertex buffer line entities
		std::map<std::string, CSMEntityLine*>::iterator it = mmapEntitiesLine.begin();
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

	void CSceneManagerSimple::_renderDepthmapForDirectionalLight(void)
	{
		SCResourceManager* pRM = SCResourceManager::getPointer();
		CResourceShader* pShader = pRM->getShader("X:shadowdepthmap");		// Shader used to render the vertex buffer entities to the depth buffer
		pShader->bind();
		CResourceDepthbuffer* pDepthbuffer = pRM->getDepthbuffer("X:shadows");
		pDepthbuffer->bindAsRenderTarget();
		glClear(GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, pDepthbuffer->getWidth(), pDepthbuffer->getHeight());

		// Set blending/depth testing
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);

		// Culling
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		glm::vec3 vLightTarget = _mv3DirectionLightPosition + mvLightDirectional.mvDirection;
		glm::mat4 lightView = glm::lookAt(
			glm::vec3(_mv3DirectionLightPosition.x, _mv3DirectionLightPosition.y, _mv3DirectionLightPosition.z),	// Eye
			vLightTarget,	// Target
			glm::vec3(0.0f, 1.0f, 0.0f));	// Up
		mmatShadowsDirectionalLightViewProj = _mmatDirectionallightProjection * lightView;
		pShader->setMat4("lightSpace", mmatShadowsDirectionalLightViewProj);

		// Triangle entities
		CResourceTriangle* pResTri;
		std::map<std::string, CSMEntityTriangle*>::iterator it = mmapEntitiesTriangles.begin();
		while (it != mmapEntitiesTriangles.end())
		{
			pResTri = pRM->getTriangle(it->second->mstrTriangleName);
			pShader->setMat4("model", it->second->getWorldMatrix());
			pResTri->draw(false);
			it++;
		}
		glCullFace(GL_BACK);
		glDisable(GL_CULL_FACE);
		pShader->unbind();
		pDepthbuffer->unbindAsRenderTarget();
		// Reset viewport
		CWindow* pWindow = CWindow::getPointer();
		glViewport(0, 0, pWindow->getWidth(), pWindow->getHeight());
	}

	void CSceneManagerSimple::setShadowsEnabled(bool bShadowsEnabled)
	{
		mbShadowsCastFromDirectionalLight = bShadowsEnabled;
	}

	bool CSceneManagerSimple::getShadowsEnabled(void)
	{
		return mbShadowsCastFromDirectionalLight;
	}

	void CSceneManagerSimple::setDirectionalLightProjection(float fProjMatWidth, float fProjMatHeight, float fProjMatNear, float fProjMatFar, glm::vec3 v3LightPosition)
	{
		_mmatDirectionallightProjection = glm::ortho(-fProjMatWidth, fProjMatWidth, -fProjMatHeight, fProjMatHeight, fProjMatNear, fProjMatFar);
		_mv3DirectionLightPosition = v3LightPosition;
	}

	void CSceneManagerSimple::setDirectionalLightDir(glm::vec3 vDirection)
	{
		mvLightDirectional.mvDirection = glm::normalize(vDirection);
	}

	void CSceneManagerSimple::setDirectionalLightColour(glm::vec3 vColour)
	{
		mvLightDirectional.mvColour = vColour;

	}
}