#include "PCH.h"
#include "SMSimple.h"
#include "log.h"
#include "resourceManager.h"
#include "window.h"
#include "stringUtils.h"

namespace X
{
	CSceneManagerSimple::CSceneManagerSimple()
	{
		miNumPointLights = 0;
		_mbShadowsCastFromDirectionalLight = true;
		setDirectionalLightColour(CColour(1.0f, 1.0f, 1.0f, 1.0f));
		setDirectionalLightDir(CVector3f(1.0f, -1.0f, 1.0f));

		// Add default camera
		addCamera("camera");

		// Enable automatic shadow matrix computation
		setShadowMatrixAutomatic();
	}

	CSceneManagerSimple::~CSceneManagerSimple()
	{
		removeAllCameras();
	}

	CSMCamera* CSceneManagerSimple::addCamera(const std::string& strName, const std::string& strFramebufferTargetResourceName)
	{
		// If resource already exists
		std::map<std::string, CSMCamera*>::iterator it = _mmapCameras.find(strName);
		ThrowIfTrue(it != _mmapCameras.end(), "CSceneManagerSimple::addCamera(" + strName + ") failed. The named object already exists.");
		CSMCamera* pNew = new CSMCamera;
		ThrowIfFalse(pNew, "CSceneManagerSimple::addCamera(" + strName + ") failed. Could not allocate memory for the new object.");
		_mmapCameras[strName] = pNew;
		pNew->setSMFramebufferTarget(strFramebufferTargetResourceName);
		return pNew;
	}

	CSMCamera* CSceneManagerSimple::getCamera(const std::string& strName) const
	{
		std::map<std::string, CSMCamera*>::iterator it = _mmapCameras.find(strName);
		ThrowIfTrue(it == _mmapCameras.end(), "CSceneManagerSimple::getCamera(" + strName + ") failed. The named object doesn't exist.");
		return it->second;
	}

	CSMCamera* CSceneManagerSimple::getCamera(int iCameraNumber) const
	{
		if (iCameraNumber < 0 || iCameraNumber >= _mmapCameras.size())
		{
			std::string strErr = "CSceneManagerSimple::getCamera(";
			StringUtils::appendInt(strErr, iCameraNumber);
			strErr += ") failed. The given camera number was invalid.";
			ThrowIfTrue(1, strErr);
		}
		std::map<std::string, CSMCamera*>::iterator it = _mmapCameras.begin();
		int iIndex = 0;
		while (iIndex < iCameraNumber)
		{
			iIndex++;
			it++;
		}
		return it->second;
	}

	int CSceneManagerSimple::getNumCameras(void) const
	{
		return (int)_mmapCameras.size();
	}

	void CSceneManagerSimple::removeCamera(const std::string& strName)
	{
		std::map<std::string, CSMCamera*>::iterator it = _mmapCameras.find(strName);
		if (it == _mmapCameras.end())
			return;
		delete it->second;
		_mmapCameras.erase(it);
	}

	void CSceneManagerSimple::removeAllCameras(void)
	{
		std::map<std::string, CSMCamera*>::iterator it = _mmapCameras.begin();
		while (it != _mmapCameras.end())
		{
			delete it->second;
			_mmapCameras.erase(it);
			it = _mmapCameras.begin();
		}
	}

	void CSceneManagerSimple::render(void)
	{
		SCResourceManager* pRM = SCResourceManager::getPointer();
		// The application manager has X:backbuffer_FB set to render target, so unbind this first
		CResourceFramebuffer* pBGFB = pRM->getFramebuffer("X:backbuffer_FB");
		pBGFB->unbindAsRenderTarget();

		SCWindow* pWindow = SCWindow::getPointer();

		// For each camera
		std::map<std::string, CSMCamera*>::iterator itCamera = _mmapCameras.begin();
		while (itCamera != _mmapCameras.end())
		{
			// If the camera isn't active
			if (!itCamera->second->getActive())
			{
				// Do nothing for this camera and proceed to the next
				itCamera++;
				continue;
			}

			// Update the camera because it might have a mode set
			itCamera->second->update();

			// Render depth map for directional light
			if (_mbShadowsCastFromDirectionalLight)
			{
				_renderDepthmapForDirectionalLight(itCamera->second->getViewMatrix(), itCamera->second->getProjectionMatrix());
			}

			// Get the camera's framebuffer target
			CResourceFramebuffer* pFramebuffer = pRM->getFramebuffer(itCamera->second->getSMFramebufferTarget());
			
			// If the buffer is the back buffer, resize it to the window dimensions
			bool bResizeFramebufferToWindowDims = false;
			if (itCamera->second->getSMFramebufferTarget() == "X:backbuffer_FB")
				bResizeFramebufferToWindowDims = true;

			// Bind the framebuffer we're rendering to
			pFramebuffer->bindAsRenderTarget(true, bResizeFramebufferToWindowDims);

			// Set the camera's perspective matrix to it's set values
			itCamera->second->setProjectionAsPerspective(
				itCamera->second->getFOV(),
				(float)pFramebuffer->getWidth(),
				(float)pFramebuffer->getHeight(),
				itCamera->second->getZNear(),
				itCamera->second->getZFar());

			// Render the triangle entities
			_renderTriangleEntities(itCamera->second);

			// Render the line entities
			_renderLineEntities(itCamera->second);

			pFramebuffer->unbindAsRenderTarget();

			itCamera++;
		}

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
		pNewMaterial->setDiffuseTextureName(strTextureNameDiffuse);
		pNewMaterial->setEmissionTextureName(strTextureNameEmission);
		pNewMaterial->setNormalmapTextureName(strTextureNameNormal);
		pNewMaterial->setRoughnessTextureName(strTextureNameRoughness);
		pNewMaterial->setSpecularStrength(fSpecularStrength);
		pNewMaterial->setAmbientStrength(fAmbientStrength);

		// Place in the hashmap
		mmapMaterials[strMaterialName] = pNewMaterial;
		return pNewMaterial;
	}

	CSMMaterial* CSceneManagerSimple::getMaterial(const std::string& strMaterialName) const
	{
		std::map<std::string, CSMMaterial*>::iterator it = mmapMaterials.find(strMaterialName);
		ThrowIfTrue(it == mmapMaterials.end(), "CSceneManagerSimple::getMaterial(" + strMaterialName + ") failed. Material doesn't exist.");
		return it->second;
	}

	bool CSceneManagerSimple::getMaterialExists(const std::string& strMaterialName) const
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

	CSMEntityTriangle* CSceneManagerSimple::getEntityTriangle(const std::string& strEntityName) const
	{
		std::map<std::string, CSMEntityTriangle*>::iterator it = mmapEntitiesTriangles.find(strEntityName);
		ThrowIfTrue(it == mmapEntitiesTriangles.end(), "CSceneManagerSimple::getEntityTriangle(" + strEntityName + ") failed. Entity doesn't exist.");
		return it->second;
	}

	bool CSceneManagerSimple::getEntityTriangleExists(const std::string& strEntityName) const
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

	CSMEntityLine* CSceneManagerSimple::getEntityLine(const std::string& strEntityName) const
	{
		std::map<std::string, CSMEntityLine*>::iterator it = mmapEntitiesLine.find(strEntityName);
		ThrowIfTrue(it == mmapEntitiesLine.end(), "CSceneManagerSimple::getEntityLine(" + strEntityName + ") failed. Entity doesn't exist.");
		return it->second;
	}

	bool CSceneManagerSimple::getEntityLineExists(const std::string& strEntityName) const
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

	void CSceneManagerSimple::_renderTriangleEntities(CSMCamera* pCamera)
	{
		SCResourceManager* pRM = SCResourceManager::getPointer();
		CResourceShader* pShader;
		if (_mbShadowsCastFromDirectionalLight)
			pShader = pRM->getShader("X:DRNE");
		else
			pShader = pRM->getShader("X:DRNE_noshadows");

		CResourceTriangle* pResTri;
		CResourceTexture2DFromFile* pTexDiffuse = 0;
		CResourceTexture2DFromFile* pTexRoughness = 0;
		CResourceTexture2DFromFile* pTexNormal = 0;
		CResourceTexture2DFromFile* pTexEmission = 0;

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
		pShader->setMat4("matrixProjection", pCamera->getProjectionMatrix());
		pShader->setMat4("matrixView", pCamera->getViewMatrix());

		// Set directional light uniforms
		pShader->setVec3("v3LightDirectionalColour", _mvLightDirectional.mvColour);
		pShader->setVec3("v3LightDirectionalDirection", _mvLightDirectional.mvDirection);

		// If we're rendering shadows, using the DRNE shader, set additional stuff for that
		CResourceDepthbuffer* pDepthbuffer = pRM->getDepthbuffer("X:shadows");
		if (_mbShadowsCastFromDirectionalLight)
		{
			pShader->setMat4("matrixLightViewProjection", _mmatShadowsDirectionalLightViewProj);
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
		CMatrix matViewInverse = pCamera->getViewMatrix().inverse();
		CVector3f cameraWorldPos = matViewInverse.multiply(CVector3f(0.0f, 0.0f, 0.0f));
		pShader->setVec3("v3CameraPositionWorld", cameraWorldPos);

		// Triangle entities
		std::map<std::string, CSMEntityTriangle*>::iterator it = mmapEntitiesTriangles.begin();
		while (it != mmapEntitiesTriangles.end())
		{
			// Get entity material
			CSMMaterial* pMaterial = getMaterial(it->second->mstrMaterialName);

			// Get vertex buffer and textures used by each entity
			pResTri = pRM->getTriangle(it->second->mstrTriangleName);
			pTexDiffuse = pRM->getTexture2DFromFile(pMaterial->getDiffuseTextureName());
			pTexRoughness = pRM->getTexture2DFromFile(pMaterial->getRoughnessTextureName());
			pTexNormal = pRM->getTexture2DFromFile(pMaterial->getNormalmapTextureName());
			pTexEmission = pRM->getTexture2DFromFile(pMaterial->getEmissionTextureName());

			// Bind each texture to each sampler unit
			pTexDiffuse->bind(0);
			pTexRoughness->bind(1);
			pTexNormal->bind(2);
			pTexEmission->bind(3);

			// Set world matrix (Projection and View set above)
			pShader->setMat4("matrixWorld", it->second->getWorldMatrix());

			// Ambient and specular uniforms
			pShader->setFloat("fAmbientStrength", pMaterial->getAmbientStrength());
			pShader->setFloat("fSpecularStrength", pMaterial->getSpecularStrength());

			// Render the vertex buffer
			pResTri->draw(false);
			it++;
		}

		// Unbind everything (We check if not NULL, as perhaps, there are no entities added to scene manager, in which case, no texture will be retrieved above
		if (pTexDiffuse)	pTexDiffuse->unbind(0);
		if (pTexRoughness)	pTexRoughness->unbind(1);
		if (pTexNormal)		pTexNormal->unbind(2);
		if (pTexEmission)	pTexEmission->unbind(3);

		if (_mbShadowsCastFromDirectionalLight)
		{
			pDepthbuffer->unbindTexture(4);
		}

		glDisable(GL_CULL_FACE);
		pShader->unbind();
	}

	void CSceneManagerSimple::_renderLineEntities(CSMCamera* pCamera)
	{
		SCResourceManager* pRM = SCResourceManager::getPointer();
		CResourceShader* pShader = pRM->getShader("X:line");		// Shader used to render the vertex buffer line entities
		CResourceLine* pLine;
		CResourceTexture2DFromFile* pTexColour = 0;

		pShader->bind();

		// Tell OpenGL, for each sampler, to which texture unit it belongs to
		pShader->setInt("texture0_colour", 0);

		// Set blending/depth testing
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);

		// Culling
		glDisable(GL_CULL_FACE);

		// Set projection and view matrixs from the camera
		pShader->setMat4("matrixProjection", pCamera->getProjectionMatrix());
		pShader->setMat4("matrixView", pCamera->getViewMatrix());

		// Vertex buffer line entities
		std::map<std::string, CSMEntityLine*>::iterator it = mmapEntitiesLine.begin();
		while (it != mmapEntitiesLine.end())
		{
			// Get vertex buffer and textures used by each entity
			pLine = pRM->getLine(it->second->mstrLineName);
			pTexColour = pRM->getTexture2DFromFile(it->second->mstrTextureName);

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

	void CSceneManagerSimple::_renderDepthmapForDirectionalLight(const CMatrix& cameraViewMatrix, const CMatrix& cameraProjectionMatrix)
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

		// If automatic calculation of the view and projection matrices used when rendering the shadow's depth map is set to true
		CVector3f vLightTarget;
		CMatrix lightView;
		// TODO work on this once I get ray casting in place.
		if (_mbAutomaticShadowMatrix)
		{
			// Compute projection matrix
			float fProjMatrixDims = 30.0f;
			float fProjMatrixNear = 1.0f;
			float fProjMatrixFar = 100.0f;
			_mmatDirectionallightProjection.setProjectionOrthographic(-fProjMatrixDims, fProjMatrixDims, fProjMatrixDims, -fProjMatrixDims, fProjMatrixNear, fProjMatrixFar);

			// Compute light "position"
			CVector3f vCamPos = cameraViewMatrix.getTranslation();
			vCamPos *= -1.0f;
			_mvDirectionalLightPosition = vCamPos;
			_mvDirectionalLightPosition += _mvLightDirectional.mvDirection * (fProjMatrixFar * 0.5f);
			
			// Compute view matrix
			vLightTarget = _mvDirectionalLightPosition + _mvLightDirectional.mvDirection;
			lightView.setViewLookat(
				_mvDirectionalLightPosition,	// Eye
				vLightTarget,					// Target
				CVector3f(0.0f, 1.0f, 0.0f));	// Up
		}
		else  // Manual settings
		{
			vLightTarget = _mvDirectionalLightPosition + _mvLightDirectional.mvDirection;
			lightView.setViewLookat(
				_mvDirectionalLightPosition,	// Eye
				vLightTarget,					// Target
				CVector3f(0.0f, 1.0f, 0.0f));	// Up
		}
		// Combine view and projection matrices for the shadow depth map.
		_mmatShadowsDirectionalLightViewProj = _mmatDirectionallightProjection * lightView;
		pShader->setMat4("lightSpace", _mmatShadowsDirectionalLightViewProj);

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
		SCWindow* pWindow = SCWindow::getPointer();
		glViewport(0, 0, pWindow->getWidth(), pWindow->getHeight());
	}

	void CSceneManagerSimple::setDirectionalLightDir(const CVector3f& vDirection)
	{
		_mvLightDirectional.mvDirection = vDirection;
		_mvLightDirectional.mvDirection.normalise();
	}

	void CSceneManagerSimple::setDirectionalLightColour(CColour colour)
	{
		_mvLightDirectional.mvColour = colour;
	}

	void CSceneManagerSimple::setShadowsEnabled(bool bShadowsEnabled)
	{
		_mbShadowsCastFromDirectionalLight = bShadowsEnabled;
	}

	bool CSceneManagerSimple::getShadowsEnabled(void) const
	{
		return _mbShadowsCastFromDirectionalLight;
	}

	void CSceneManagerSimple::setShadowMatrixManual(float fProjMatWidth, float fProjMatHeight, float fProjMatNear, float fProjMatFar, CVector3f v3LightPosition)
	{
		_mbAutomaticShadowMatrix = false;

		_mmatDirectionallightProjection.setProjectionOrthographic(-fProjMatWidth, fProjMatWidth, fProjMatHeight, -fProjMatHeight, fProjMatNear, fProjMatFar);
		_mvDirectionalLightPosition = v3LightPosition;
	}

	void CSceneManagerSimple::setShadowMatrixAutomatic(void)
	{
		_mbAutomaticShadowMatrix = true;
	}

	void CSceneManagerSimple::debugRenderShadowmap(int iDims, bool bRightEdgeOfScreen, float fAlpha) const
	{
		SCResourceManager* pRM = SCResourceManager::getPointer();
		CResourceDepthbuffer* pDepthbuffer = pRM->getDepthbuffer("X:shadows");
		SCWindow* pWindow = SCWindow::getPointer();
		int iXpos = 0;
		if (bRightEdgeOfScreen)
			iXpos = pWindow->getWidth() - iDims;
		pDepthbuffer->renderTo2DQuad(iXpos, 0, iDims, iDims, CColour(1, 1, 1, fAlpha));
	}
}