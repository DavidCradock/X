#include "PCH.h"
#include "2DRenderer.h"
#include "singletons.h"
#include "logging.h"

namespace X
{
	SC2DRenderer::SC2DRenderer()
	{
		SCLog* pLog = SCLog::getPointer();
		pLog->add("SC2DRenderer::SC2DRenderer() called.");

		_muiNumTextureBindingsPerLoop = 0;
	}

	SC2DRenderer::~SC2DRenderer()
	{
		removeAllWorlds();
	}

	C2DWorld* SC2DRenderer::addWorld(const std::string& strUniqueName)
	{
		// Attempt to find if the object name already exists
		std::map<std::string, C2DWorld*>::iterator it = _mmapWorlds.find(strUniqueName);
		ThrowIfTrue(it != _mmapWorlds.end(), "SC2DRenderer::addWorld(\"" + strUniqueName + "\") failed. The object already exists.");

		// Allocate memory for new object
		C2DWorld* pNew = new C2DWorld;
		ThrowIfFalse(pNew, "SC2DRenderer::addWorld(\"" + strUniqueName + "\") failed. Unable to allocate memory.");

		// Add object to hash map
		_mmapWorlds[strUniqueName] = pNew;
		return pNew;
	}

	bool SC2DRenderer::getWorldExists(const std::string& strUniqueName) const
	{
		if (_mmapWorlds.find(strUniqueName) == _mmapWorlds.end())
			return false;
		return true;
	}

	C2DWorld* SC2DRenderer::getWorld(const std::string& strUniqueName) const
	{
		// Attempt to find if the world name already exists
		std::map<std::string, C2DWorld*>::iterator it = _mmapWorlds.find(strUniqueName);
		ThrowIfTrue(it == _mmapWorlds.end(), "SC2DRenderer::getWorld(\"" + strUniqueName + "\") failed. Object name doesn't exist!");
		return it->second;
	}

	C2DWorld* SC2DRenderer::getWorld(unsigned int uiIndex) const
	{
		// Make sure given index is valid
		ThrowIfTrue(uiIndex >= _mmapWorlds.size(), "SC2DRenderer::getWorld(" + std::to_string(uiIndex) + ") failed. Invalid index given.");
		std::map<std::string, C2DWorld*>::iterator it = _mmapWorlds.begin();
		unsigned int ui = 0;
		while (ui < uiIndex)
		{
			ui++;
			it++;
		}
		return it->second;
	}

	void SC2DRenderer::removeWorld(const std::string& strUniqueName)
	{
		// Attempt to find if the name already exists
		std::map<std::string, C2DWorld*>::iterator it = _mmapWorlds.find(strUniqueName);
		ThrowIfTrue(it == _mmapWorlds.end(), "SC2DRenderer::removeWorld(\"" + strUniqueName + "\") failed. The object doesn't exist.");

		// De-allocate memory for the object
		delete it->second;

		// Remove object from hash map
		_mmapWorlds.erase(it);
	}

	void SC2DRenderer::removeWorld(unsigned int uiIndex)
	{
		// Make sure given index is valid
		ThrowIfTrue(uiIndex >= _mmapWorlds.size(), "SC2DRenderer::removeWorld(" + std::to_string(uiIndex) + ") failed. Invalid index given.");
		std::map<std::string, C2DWorld*>::iterator it = _mmapWorlds.begin();
		unsigned int ui = 0;
		while (ui < uiIndex)
		{
			ui++;
			it++;
		}
		// De-allocate memory for the object
		delete it->second;

		// Remove object from hash map
		_mmapWorlds.erase(it);
	}

	void SC2DRenderer::removeAllWorlds(void)
	{
		// Remove all layers
		std::map<std::string, C2DWorld*>::iterator it = _mmapWorlds.begin();
		while (it != _mmapWorlds.end())
		{
			delete it->second;
			_mmapWorlds.erase(it);
			it = _mmapWorlds.begin();
		}
	}

	int SC2DRenderer::getNumWorlds(void) const
	{
		return int(_mmapWorlds.size());
	}

	void SC2DRenderer::render(void)
	{
		// Reset _muiNumTextureBindingsPerLoop
		// It's used for statistical purposes only
		_muiNumTextureBindingsPerLoop = 0;

		// For entities
		CResourceVertexBufferCPT* pVB = x->pResource->getVertexBufferCPT("X:default");
		CResourceShader* pShaderEntity = x->pResource->getShader("X:VBCPT");
		pVB->removeGeom();

		// For line entities
		CResourceVertexBufferLine* pVBLine = x->pResource->getVertexBufferLine("X:default");
		CResourceShader* pShaderLine = x->pResource->getShader("X:VBCPT");
		CResourceTexture2DFromFile* pTextureLine = x->pResource->getTexture2DFromFile("X:default_white");

		// For instance entities
		CResourceVertexBufferCPTInst* pVBI = x->pResource->getVertexBufferCPTInst("X:default");
		pVBI->removeAll();
		CResourceShader* pShaderEntityRot = x->pResource->getShader("X:VBCPTInst");

		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);

		// For each world
		std::map<std::string, C2DWorld*>::iterator itWorld = _mmapWorlds.begin();
		while (itWorld != _mmapWorlds.end())
		{
			// Only render the world if it is set to be
			if (!itWorld->second->_mbVisible)
			{
				itWorld++;
				continue;
			}

			// For each camera in world
			std::map<std::string, C2DCamera*>::iterator itCamera = itWorld->second->_mmapCameras.begin();
			while (itCamera != itWorld->second->_mmapCameras.end())
			{
				// Only proceed if this camera is enabled
				if (!itCamera->second->getEnabled())
				{
					itCamera++;
					continue;
				}

				// Get framebuffer to render to
				CResourceFramebuffer* pFB = x->pResource->getFramebuffer(itCamera->second->getFramebufferTargetName());
				
				// Set framebuffer as render target
				// Only clear the framebuffer if it isn't "X:backbuffer"
				if (itCamera->second->getFramebufferTargetName() != "X:backbuffer")
					pFB->bindAsRenderTarget(true, false);
				else
					pFB->bindAsRenderTarget(false, false);

				// Set projection matrix
				CVector2f vFBDims = pFB->getDimensions();
				CMatrix matrixProjection;
				matrixProjection.setProjectionOrthographic(0.0f, vFBDims.x, 0.0f, vFBDims.y, -1.0f, 1.0f);

				// Set view matrix from camera
				CMatrix matrixView;
				CVector2f v2CameraPos = itCamera->second->getPosition();
				CVector3f v3CameraPos(v2CameraPos.x, v2CameraPos.y, 0.0f);
				v3CameraPos.x *= -1.0f;
				v3CameraPos.y *= -1.0f;
				matrixView.setTranslation(v3CameraPos);

				// Identity world matrix
				CMatrix matrixWorld;

				// For each layer in world
				for (unsigned int uiLayerZorder = 0; uiLayerZorder < itWorld->second->_mvecLayerNameZOrder.size(); ++uiLayerZorder)
				{
					// Get layers, in z order , starting with the one at the back
					C2DLayer* pLayer = itWorld->second->_mmapLayers[itWorld->second->_mvecLayerNameZOrder[uiLayerZorder]];

					// Only render the layer if it is set to be
					if (!pLayer->_mbVisible)
						continue;

					// For each C2DMap
					std::map<std::string, C2DMap*>::iterator itMap = pLayer->_mmapMaps.begin();
					while (itMap != pLayer->_mmapMaps.end())
					{
						if (itMap->second->getVisible())
							itMap->second->render(*itCamera->second, matrixView, matrixProjection);
						itMap++;
					}

					// Set previously bound stuff to nothing
					unsigned int uiPreviouslyBoundAtlasImageNumber = 999999;		// Used to reduce rebinding of same atlas texture
					std::string strPreviouslyBoundAtlasName;						// Used to reduce rebinding of same atlas texture

					// Bind and setup shader for C2DEntity objects
					pShaderEntity->bind();											// Bind correct shader
					pShaderEntity->setInt("texture0", 0);							// Tell OpenGL, for each sampler, to which texture unit it belongs to
					pShaderEntity->setMat4("matrixWorld", matrixWorld);
					pShaderEntity->setMat4("matrixView", matrixView);				// Set matrix view from camera for shader
					pShaderEntity->setMat4("matrixProjection", matrixProjection);	// Set projection matrix for shader

					// For each C2DEntity in layer
					std::map<std::string, C2DEntity*>::iterator itEntity = pLayer->_mmapEntities.begin();
					while (itEntity != pLayer->_mmapEntities.end())
					{
						itEntity->second->render(
							strPreviouslyBoundAtlasName,
							uiPreviouslyBoundAtlasImageNumber,
							pVB,
							_muiNumTextureBindingsPerLoop);
						itEntity++;
					}
					// Send remaining vertex data to GPU to be rendered
					pVB->update();
					pVB->render(false);
					pVB->removeGeom();
					pShaderEntity->unbind();

					// Bind and setup shader for C2DEntityRot objects
					pShaderEntityRot->bind();
					pShaderEntityRot->setInt("texture0", 0);							// Tell OpenGL, for each sampler, to which texture unit it belongs to
					// We do not set world view as world matrix is given as instance data.
					pShaderEntityRot->setMat4("matrixView", matrixView);				// Set matrix view from camera for shader
					pShaderEntityRot->setMat4("matrixProjection", matrixProjection);	// Set projection matrix for shader
					
					// For each C2DEntityRot in layer
					std::map<std::string, C2DEntityRot*>::iterator itEntityRot = pLayer->_mmapEntityRots.begin();
					while (itEntityRot != pLayer->_mmapEntityRots.end())
					{
						itEntityRot->second->render(
							strPreviouslyBoundAtlasName,
							uiPreviouslyBoundAtlasImageNumber,
							pVBI,
							pShaderEntityRot,
							_muiNumTextureBindingsPerLoop);
						itEntityRot++;
					}
					pVBI->update();
					pVBI->render(false);
					pVBI->removeAll();

					// For each particle system
					std::map<std::string, C2DParticleSystem*>::iterator itParticleSystem = pLayer->_mmapParticleSystems.begin();
					while (itParticleSystem != pLayer->_mmapParticleSystems.end())
					{
						itParticleSystem->second->render(matrixView, matrixProjection);
						itParticleSystem++;
					}

					// Bind and setup shader for C2DEntityLine objects
					pShaderLine->bind();										// Bind correct shader
					pShaderLine->setInt("texture0", 0);							// Tell OpenGL, for each sampler, to which texture unit it belongs to
					pShaderLine->setMat4("matrixView", matrixView);				// Set matrix view from camera for shader
					pShaderLine->setMat4("matrixProjection", matrixProjection);	// Set projection matrix for shader

					// Bind white texture
					pTextureLine->bind();

					// For each C2DEntityLine in layer
					std::map<std::string, C2DEntityLine*>::iterator itEntityLine = pLayer->_mmapEntityLines.begin();
					while (itEntityLine != pLayer->_mmapEntityLines.end())
					{
						itEntityLine->second->render(pVBLine, pShaderLine);
						itEntityLine++;
					}
					// Send remaining vertex data to GPU to be rendered and cleanup
					pVB->update();
					pVB->render(false);
					pVB->removeGeom();
					pShaderLine->unbind();
					pTextureLine->unbind();

					
				}	// Each layer

				itCamera++;
			}	// Each camera in world
			itWorld++;
		}	// Each world


		// Reset framebuffer to render to the "X:backbuffer"
		CResourceFramebuffer* pFB = x->pResource->getFramebuffer("X:backbuffer");
		pFB->bindAsRenderTarget(false, false);

		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);

	}

	unsigned int SC2DRenderer::getNumberTextureRebindingsPerLoop(void) const
	{
		return _muiNumTextureBindingsPerLoop;
	}
}