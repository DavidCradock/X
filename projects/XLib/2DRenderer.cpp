#include "PCH.h"
#include "2DRenderer.h"
#include "log.h"
#include "resourceManager.h"

namespace X
{
	SC2DRenderer::SC2DRenderer()
	{

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

	bool SC2DRenderer::getWorldExists(const std::string& strUniqueName)
	{
		if (_mmapWorlds.find(strUniqueName) == _mmapWorlds.end())
			return false;
		return true;
	}

	C2DWorld* SC2DRenderer::getWorld(const std::string& strUniqueName)
	{
		// Attempt to find if the world name already exists
		std::map<std::string, C2DWorld*>::iterator it = _mmapWorlds.find(strUniqueName);
		ThrowIfTrue(it == _mmapWorlds.end(), "SC2DRenderer::getWorld(\"" + strUniqueName + "\") failed. Object name doesn't exist!");
		return it->second;
	}

	C2DWorld* SC2DRenderer::getWorld(unsigned int uiIndex)
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

	int SC2DRenderer::getNumWorlds(void)
	{
		return int(_mmapWorlds.size());
	}

	void SC2DRenderer::render(void)
	{
		// Get required resources needed to render stuff
		SCResourceManager* pRM = SCResourceManager::getPointer();
		CResourceTriangle* pTri = pRM->getTriangle("X:2D");
		CResourceShader* pShader = pRM->getShader("X:2D");
		//CResourceShader* pShader = pRM->getShader("X:pos_col_tex");

		pTri->removeGeom();

		// Tell OpenGL, for each sampler, to which texture unit it belongs to
		pShader->bind();
		pShader->setInt("texture0", 0);

		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);

		// For each world
		std::map<std::string, C2DWorld*>::iterator itWorld = _mmapWorlds.begin();
		while (itWorld != _mmapWorlds.end())
		{
			// For each camera in world
			std::map<std::string, C2DCamera*>::iterator itCamera = itWorld->second->_mmapCameras.begin();
			while (itCamera != itWorld->second->_mmapCameras.end())
			{
				// Get framebuffer to render to
				CResourceFramebuffer* pFB = pRM->getFramebuffer(itCamera->second->_mstrFramebufferTarget);
				
				// Set framebuffer as render target
				// Only clear the framebuffer if it isn't "X:backbuffer_fb"
				if (itCamera->second->_mstrFramebufferTarget != "X:backbuffer_FB")
					pFB->bindAsRenderTarget(true, false);
				else
					pFB->bindAsRenderTarget(false, false);

				// Set projection matrix
				glm::vec2 vFBDims = pFB->getDimensions();
				glm::mat4 matrixProjection = glm::ortho(0.0f, vFBDims.x, vFBDims.y, 0.0f, -1.0f, 1.0f);
				pShader->setMat4("matrixProjection", matrixProjection);

				// Set matrix view from camera
				glm::mat4 matrixView = glm::mat4(1.0f);
				glm::vec3 v3CameraPos = itCamera->second->getPositionGLM();
				v3CameraPos.x *= -1.0f;
				v3CameraPos.y *= -1.0f;
				matrixView = glm::translate(matrixView, v3CameraPos);
				pShader->setMat4("matrixView", matrixView);

				// For each layer in world
				for (unsigned int uiLayerZorder = 0; uiLayerZorder < itWorld->second->_mvecLayerNameZOrder.size(); ++uiLayerZorder)
				{
					// Get layers, in z order , starting with the one at the back
					C2DLayer* pLayer = itWorld->second->_mmapLayers[itWorld->second->_mvecLayerNameZOrder[uiLayerZorder]];

					// For each entity in layer
					unsigned int uiPreviouslyBoundAtlasImageNumber = 999999;	// Used to reduce rebinding of same atlas texture
					std::string strPreviouslyBoundAtlasName;					// Used to reduce rebinding of same atlas texture
					std::map<std::string, C2DEntity*>::iterator itEntity = pLayer->_mmapEntitys.begin();
					while (itEntity != pLayer->_mmapEntitys.end())
					{
						// Bind correct atlas texture if not previously bound
						bool bBindTexture = false;
						if (strPreviouslyBoundAtlasName != itEntity->second->_mstrResourceTexture2DAtlasName)
						{
							strPreviouslyBoundAtlasName = itEntity->second->_mstrResourceTexture2DAtlasName;
							bBindTexture = true;
						}
						if (uiPreviouslyBoundAtlasImageNumber != itEntity->second->_mvImageDetails[itEntity->second->_muiCurrentFrameNumber].uiAtlasImage)
						{
							uiPreviouslyBoundAtlasImageNumber = itEntity->second->_mvImageDetails[itEntity->second->_muiCurrentFrameNumber].uiAtlasImage;
							bBindTexture = true;
						}
						if (bBindTexture)
						{
							// Get CResourceTexture2DAtlas the entity is set to use
							CResourceTexture2DAtlas* pAtlas = pRM->getTexture2DAtlas(itEntity->second->_mstrResourceTexture2DAtlasName);
							// Bind the atlas texture containing the entity's currently set frame number's image
							pAtlas->bindAtlas(0, itEntity->second->_mvImageDetails[itEntity->second->_muiCurrentFrameNumber].uiAtlasImage);
						}
						
						// Now get the entity's frame information for texture coordinates and entity position and add to CResourceTriangle* pTri
						// But before doing that, we need to send any existing data to be rendered IF the atlas texture has changed
						if (bBindTexture)	// If the atlas texture has changed
						{
							pTri->update();
							pTri->draw(false);
							pTri->removeGeom();
						}

						pTri->addQuad2D(
							itEntity->second->_mv2rPosition,															// Position
							itEntity->second->_mvImageDetails[itEntity->second->_muiCurrentFrameNumber].v2rDimensions,	// Dimensions
							itEntity->second->_mColour,																	// Colour
							itEntity->second->_mvImageDetails[itEntity->second->_muiCurrentFrameNumber].sTexCoords.bottom_left,		// Texture coordinates
							itEntity->second->_mvImageDetails[itEntity->second->_muiCurrentFrameNumber].sTexCoords.bottom_right,	// Texture coordinates
							itEntity->second->_mvImageDetails[itEntity->second->_muiCurrentFrameNumber].sTexCoords.top_right,		// Texture coordinates
							itEntity->second->_mvImageDetails[itEntity->second->_muiCurrentFrameNumber].sTexCoords.top_left);		// Texture coordinates
						itEntity++;
					}
					// Send remaining vertex data to GPU to be rendered
					pTri->update();
					pTri->draw(false);
					pTri->removeGeom();

					// For each complex entity in layer
					std::map<std::string, C2DEntityComplex*>::iterator itEntityComplex = pLayer->_mmapEntityComplexs.begin();
					while (itEntityComplex != pLayer->_mmapEntityComplexs.end())
					{

						itEntityComplex++;
					}

				}	// Each layer

				itCamera++;
			}	// Each camera in world
			itWorld++;
		}	// Each world


		// Reset framebuffer to render to to the "X:backbuffer_FB"
		CResourceFramebuffer* pFB = pRM->getFramebuffer("X:backbuffer_FB");
		pFB->bindAsRenderTarget(false, false);

		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);

	}
}