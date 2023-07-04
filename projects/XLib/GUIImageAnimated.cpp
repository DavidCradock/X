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
		CWindow* pWindow = CWindow::getPointer();
		CResourceTriangle* pTri = pRM->getTriangle("X:gui");
		CResourceShader* pShader = pRM->getShader("X:pos_col_tex");

		pShader->bind();

		// Setup the projection matrix as orthographic
		glm::mat4 matProjection = glm::ortho(0.0f, float(pWindow->getWidth()), float(pWindow->getHeight()), 0.0f, -1.0f, 1.0f);
		pShader->setMat4("transform", matProjection);

		// Tell OpenGL, for each sampler, to which texture unit it belongs to
		pShader->setInt("texture0", 0);

		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);

		// Get textures
		CResourceTexture2DAnimation* pTexColour = pRM->getTexture2DAnimation(_mstrResourceTexture2DAnimationName);

		// Bind textures
		pTexColour->bind(0, (int)_mfCurrentFrame);

		// Get texture coordinates for current frame
		glm::vec2 vTCMin, vTCMax;
		pTexColour->getTextureCoords((int)_mfCurrentFrame, vTCMin, vTCMax);

		// Render the image
		pTri->removeGeom();
		pTri->addQuad2D(
			glm::vec2(pContainer->mfPositionX + mfPositionX, pContainer->mfPositionY + mfPositionY),	// Position
			glm::vec2(mfWidth, mfHeight),			// Dimensions
			mColour.get(),							// Vertex colour
			glm::vec2(vTCMin.x, vTCMax.y),
			glm::vec2(vTCMax.x, vTCMax.y),
			glm::vec2(vTCMax.x, vTCMin.y),
			glm::vec2(vTCMin.x, vTCMin.y));

		pTri->update();
		pTri->draw();

		pTexColour->unbindAll();	// Unbind textures
		pShader->unbind();	// Unbind the GUI shader
		glDisable(GL_BLEND);

	}

	void CGUIImageAnimated::update(void* pParentContainer, bool bParentContainerAcceptingMouseClicks)
	{
		SCResourceManager* pRM = SCResourceManager::getPointer();
		CResourceTexture2DAnimation* pTex = pRM->getTexture2DAnimation(_mstrResourceTexture2DAnimationName);

		_mTimer.update();
		float fSecPast = _mTimer.getSecondsPast();
		_mfCurrentFrame += fSecPast * _mfFramesPerSecond;
		float fNumFrames = (float)pTex->getNumFrames();
		while (_mfCurrentFrame >= fNumFrames)
			_mfCurrentFrame -= fNumFrames;

		// Update this object's tooltip
		SCInputManager* pInput = SCInputManager::getPointer();
		glm::vec2 vMousePos = pInput->mouse.getCursorPos();
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
		CGUITooltip* pTooltip = (CGUITooltip*)mpTooltip;
		pTooltip->update(pParentContainer, (CGUIBaseObject*)this, bMouseOver);

	}

	void CGUIImageAnimated::setFramesPerSecond(float fFramesPerSecond)
	{
		_mfFramesPerSecond = fFramesPerSecond;
	}
}