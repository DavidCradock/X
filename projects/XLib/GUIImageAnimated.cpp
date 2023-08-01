#include "PCH.h"
#include "GUIImageAnimated.h"
#include "GUIManager.h"
#include "resourceManager.h"
#include "window.h"
#include "input.h"
#include "GUITooltip.h"

namespace X
{
	CGUIImageAnimated::CGUIImageAnimated()
	{
		_mfFramesPerSecond = 60.0f;
		_mfCurrentFrame = 0.0f;
		mpTooltip = new CGUITooltip;
	}

	CGUIImageAnimated::~CGUIImageAnimated()
	{
		delete mpTooltip;
	}

	void CGUIImageAnimated::render(void* pParentContainer, const std::string& strFramebufferToSampleFrom)
	{
		CGUIContainer* pContainer = (CGUIContainer*)pParentContainer;

		// Get required resources needed to render
		SCResourceManager* pRM = SCResourceManager::getPointer();
		SCWindow* pWindow = SCWindow::getPointer();
		CResourceTriangle* pTri = pRM->getTriangle("X:default");
		CResourceShader* pShader = pRM->getShader("X:pos_col_tex");

		pShader->bind();

		// Setup the projection matrix as orthographic
		CMatrix matProjection;
		matProjection.setProjectionOrthographic(0.0f, float(pWindow->getWidth()), 0.0f, float(pWindow->getHeight()), -1.0f, 1.0f);
		pShader->setMat4("transform", matProjection);

		// Tell OpenGL, for each sampler, to which texture unit it belongs to
		pShader->setInt("texture0", 0);

		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);

		// Get textures
		CResourceTexture2DAtlas* pTexColour = pRM->getTexture2DAtlas(_mstrResourceTexture2DAtlasName);

		// Bind textures
		pTexColour->bind(0, (int)_mfCurrentFrame);

		// Get texture coordinates for current frame
		CVector2f vTCMin, vTCMax;
		pTexColour->getTextureCoords((int)_mfCurrentFrame, vTCMin, vTCMax);

		// Render the image
		pTri->removeGeom();
		pTri->addQuad2D(
			CVector2f(pContainer->mfPositionX + mfPositionX, pContainer->mfPositionY + mfPositionY),	// Position
			CVector2f(mfWidth, mfHeight),			// Dimensions
			mColour,							// Vertex colour
			CVector2f(vTCMin.x, vTCMax.y),
			CVector2f(vTCMax.x, vTCMax.y),
			CVector2f(vTCMax.x, vTCMin.y),
			CVector2f(vTCMin.x, vTCMin.y));

		pTri->update();
		pTri->draw();

		pTexColour->unbindAll();	// Unbind textures
		pShader->unbind();	// Unbind the GUI shader
		glDisable(GL_BLEND);

	}

	void CGUIImageAnimated::update(void* pParentContainer, bool bParentContainerAcceptingMouseClicks)
	{
		SCResourceManager* pRM = SCResourceManager::getPointer();
		CResourceTexture2DAtlas* pTex = pRM->getTexture2DAtlas(_mstrResourceTexture2DAtlasName);

		_mTimer.update();
		float fSecPast = _mTimer.getSecondsPast();
		_mfCurrentFrame += fSecPast * _mfFramesPerSecond;
		float fNumFrames = (float)pTex->getNumImages();
		while (_mfCurrentFrame >= fNumFrames)
			_mfCurrentFrame -= fNumFrames;

		// Update this object's tooltip
		SCInputManager* pInput = SCInputManager::getPointer();
		CVector2f vMousePos = pInput->mouse.getCursorPos();
		CGUIContainer* pContainer = (CGUIContainer*)pParentContainer;
		bool bMouseOver = false;
		if (bParentContainerAcceptingMouseClicks)
		{
			// Determine if mouse cursor is over
			if (vMousePos.x > pContainer->mfPositionX + mfPositionX)
				if (vMousePos.x < pContainer->mfPositionX + mfPositionX + mfWidth)
					if (vMousePos.y > pContainer->mfPositionY + mfPositionY)
						if (vMousePos.y < pContainer->mfPositionY + mfPositionY + mfHeight)
							bMouseOver = true;
		}
		mpTooltip->update(pParentContainer, (CGUIBaseObject*)this, bMouseOver);
	}

	void CGUIImageAnimated::setFramesPerSecond(float fFramesPerSecond)
	{
		_mfFramesPerSecond = fFramesPerSecond;
	}
}