#include "PCH.h"
#include "GUIImageAnimated.h"
#include "GUIManager.h"
#include "GUITooltip.h"
#include "singletons.h"
#include "resourceManager.h"
#include "input.h"

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
		CResourceVertexBufferCPT* pVB = x->pResource->getVertexBufferCPT("X:default");
		CResourceShader* pShader = x->pResource->getShader("X:VBCPT");

		pShader->bind();

		// Setup the matrices
		CMatrix matWorld, matView;
		CMatrix matProjection;
		matProjection.setProjectionOrthographic(0.0f, float(x->pWindow->getWidth()), 0.0f, float(x->pWindow->getHeight()), -1.0f, 1.0f);
		pShader->setMat4("matrixWorld", matWorld);
		pShader->setMat4("matrixView", matView);
		pShader->setMat4("matrixProjection", matProjection);

		// Tell OpenGL, for each sampler, to which texture unit it belongs to
		pShader->setInt("texture0", 0);

		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);

		// Get textures
		CResourceTexture2DAtlas* pTexColour = x->pResource->getTexture2DAtlas(_mstrResourceTexture2DAtlasName);

		// Bind textures
		pTexColour->bind(0, (int)_mfCurrentFrame);

		// Get texture coordinates for current frame
		CVector2f vTCMin, vTCMax;
		pTexColour->getTextureCoords((int)_mfCurrentFrame, vTCMin, vTCMax);

		// Render the image
		pVB->removeGeom();
		pVB->addQuad2D(
			CVector2f(pContainer->mfPositionX + mfPositionX, pContainer->mfPositionY + mfPositionY),	// Position
			CVector2f(mfWidth, mfHeight),			// Dimensions
			mColour,							// Vertex colour
			CVector2f(vTCMin.x, vTCMax.y),
			CVector2f(vTCMax.x, vTCMax.y),
			CVector2f(vTCMax.x, vTCMin.y),
			CVector2f(vTCMin.x, vTCMin.y));

		pVB->update();
		pVB->render();

		pTexColour->unbindAll();	// Unbind textures
		pShader->unbind();	// Unbind the GUI shader
		glDisable(GL_BLEND);

	}

	void CGUIImageAnimated::update(void* pParentContainer, bool bParentContainerAcceptingMouseClicks)
	{
		CResourceTexture2DAtlas* pTex = x->pResource->getTexture2DAtlas(_mstrResourceTexture2DAtlasName);

		_mTimer.update();
		float fSecPast = _mTimer.getSecondsPast();
		_mfCurrentFrame += fSecPast * _mfFramesPerSecond;
		float fNumFrames = (float)pTex->getNumImages();
		while (_mfCurrentFrame >= fNumFrames)
			_mfCurrentFrame -= fNumFrames;

		// Update this object's tooltip
		CVector2f vMousePos = x->pInput->mouse.getCursorPos();
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