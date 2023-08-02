#include "PCH.h"
#include "2DRenderer.h"
#include "log.h"
#include "resourceManager.h"

namespace X
{
	SC2DRenderer::SC2DRenderer()
	{
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

		// Get required resources needed to render stuff
		SCResourceManager* pRM = SCResourceManager::getPointer();
		CResourceTriangle* pTri = pRM->getTriangle("X:default");
		CResourceShader* pShaderEntity = pRM->getShader("X:2DEntity");
		CResourceShader* pShaderEntityRot = pRM->getShader("X:2DEntityRot");

		pTri->removeGeom();

		// Tell OpenGL, for each sampler, to which texture unit it belongs to
		pShaderEntity->bind();
		pShaderEntity->setInt("texture0", 0);

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
				CResourceFramebuffer* pFB = pRM->getFramebuffer(itCamera->second->getFramebufferTargetName());
				
				// Set framebuffer as render target
				// Only clear the framebuffer if it isn't "X:backbuffer_fb"
				if (itCamera->second->getFramebufferTargetName() != "X:backbuffer_FB")
					pFB->bindAsRenderTarget(true, false);
				else
					pFB->bindAsRenderTarget(false, false);

				// Set projection matrix
				CVector2f vFBDims = pFB->getDimensions();
				CMatrix matrixProjection;
				matrixProjection.setProjectionOrthographic(0.0f, vFBDims.x, 0.0f, vFBDims.y, -1.0f, 1.0f);
				pShaderEntity->setMat4("matrixProjection", matrixProjection);

				// Set matrix view from camera
				CMatrix matrixView;
				CVector2f v2CameraPos = itCamera->second->getPosition();
				CVector3f v3CameraPos(v2CameraPos.x, v2CameraPos.y, 0.0f);
				v3CameraPos.x *= -1.0f;
				v3CameraPos.y *= -1.0f;
				matrixView.setTranslation(v3CameraPos);
				pShaderEntity->setMat4("matrixView", matrixView);

				// For each layer in world
				for (unsigned int uiLayerZorder = 0; uiLayerZorder < itWorld->second->_mvecLayerNameZOrder.size(); ++uiLayerZorder)
				{
					// Get layers, in z order , starting with the one at the back
					C2DLayer* pLayer = itWorld->second->_mmapLayers[itWorld->second->_mvecLayerNameZOrder[uiLayerZorder]];

					// Only render the layer if it is set to be
					if (!pLayer->_mbVisible)
						continue;

					// For each C2DEntity in layer
					unsigned int uiPreviouslyBoundAtlasImageNumber = 999999;	// Used to reduce rebinding of same atlas texture
					std::string strPreviouslyBoundAtlasName;					// Used to reduce rebinding of same atlas texture
					std::map<std::string, C2DEntity*>::iterator itEntity = pLayer->_mmapEntities.begin();
					while (itEntity != pLayer->_mmapEntities.end())
					{
						itEntity->second->render(
							strPreviouslyBoundAtlasName,
							uiPreviouslyBoundAtlasImageNumber,
							pTri,
							_muiNumTextureBindingsPerLoop);
						itEntity++;
					}
					// Send remaining vertex data to GPU to be rendered
					pTri->update();
					pTri->draw(false);
					pTri->removeGeom();

					// For each C2DEntityRot in layer
					std::map<std::string, C2DEntityRot*>::iterator itEntityRot = pLayer->_mmapEntityRots.begin();
					while (itEntityRot != pLayer->_mmapEntityRots.end())
					{
						itEntityRot->second->render();
						itEntityRot++;
					}

				}	// Each layer

				itCamera++;
			}	// Each camera in world
			itWorld++;
		}	// Each world


		// Reset framebuffer to render to the "X:backbuffer_FB"
		CResourceFramebuffer* pFB = pRM->getFramebuffer("X:backbuffer_FB");
		pFB->bindAsRenderTarget(false, false);

		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);

	}

	unsigned int SC2DRenderer::getNumberTextureRebindingsPerLoop(void) const
	{
		return _muiNumTextureBindingsPerLoop;
	}
}