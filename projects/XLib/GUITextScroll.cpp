#include "PCH.h"
#include "GUITextScroll.h"
#include "GUIManager.h"
#include "resourceManager.h"
#include "window.h"
#include "input.h"
#include "GUITooltip.h"
#include "utilities.h"

namespace X
{
	CGUITextScroll::CGUITextScroll()
	{
		_mbFBNeedsUpdating = true;
		_mfPreviousSliderTabPos = -1.0f;
		_mSlider.setTabPos(0.0f);
		mpTooltip = new CGUITooltip;
	}

	CGUITextScroll::~CGUITextScroll()
	{
		delete mpTooltip;

		// Also remove the framebuffer used from the resource manager
		SCResourceManager* pRM = SCResourceManager::getPointer();
		pRM->removeFramebuffer(_mstrFBName);
	}

	void CGUITextScroll::render(void* pParentContainer, const std::string& strFramebufferToSampleFrom)
	{
		CGUIContainer* pContainer = (CGUIContainer*)pParentContainer;
		SCGUIManager* pGUIManager = SCGUIManager::getPointer();
		CGUITheme* pTheme = pContainer->getTheme();
		CColour col;
		renderBackground(pParentContainer, strFramebufferToSampleFrom, pTheme->mImages.textEditBGColour, pTheme->mImages.textScrollBGNormal, col);
		SCResourceManager* pRM = SCResourceManager::getPointer();
		CResourceFramebuffer* pTexColour = pRM->getFramebuffer(_mstrFBName);

		// Does the frame buffer need updating (Like when the OpenGL context has been lost)
		if (pTexColour->mbNeedsUpdating)
		{
			pTexColour->mbNeedsUpdating = false;
			_mbFBNeedsUpdating = true;
		}
		// Update the framebuffer if needed
		if (_mbFBNeedsUpdating)
			_renderFramebuffer(pParentContainer);

		// Get required resources needed to render
		SCWindow* pWindow = SCWindow::getPointer();
		CResourceVertexBuffer* pVB = pRM->getVertexBuffer("X:default");
		CResourceShader* pShader = pRM->getShader("X:pos_col_tex");

		// Render the frame buffer over the top of the background
		pShader->bind();

		// Setup the projection matrix as orthographic
		CMatrix matProjection;
		matProjection.setProjectionOrthographic(0.0f, float(pWindow->getWidth()), 0.0f, float(pWindow->getHeight()), -1.0f, 1.0f);
		pShader->setMat4("transform", matProjection);

		// Tell OpenGL, for each sampler, to which texture unit it belongs to
		pShader->setInt("texture0", 0);

		glDisable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);

		CVector2f vBGdims = pRM->getTexture2DFromFile(pTheme->mImages.textScrollBGColour)->mvDimensions;
		CVector2f vBGDimsPoint3 = vBGdims * 0.3333333f;
		CVector2f vBGDimsPoint6 = vBGdims * 0.6666666f;

		// Bind textures
		pTexColour->bindAsTexture(0);

		// Render the image
		pVB->removeGeom();
		pVB->addQuad2D(
			CVector2f(pContainer->mfPositionX + mfPositionX + vBGDimsPoint3.x, pContainer->mfPositionY + mfPositionY + vBGDimsPoint3.y),	// Position
			CVector2f(mfWidth - vBGDimsPoint6.x - pTheme->mfTextScrollSliderWidth, mfHeight - vBGDimsPoint6.y),								// Dimensions
			CColour(1.0f, 1.0f, 1.0f, 1.0f),		// Vertex colour
			CVector2f(0.0f, 0.0f),
			CVector2f(1.0f, 0.0f),
			CVector2f(1.0f, 1.0f),
			CVector2f(0.0f, 1.0f));

		pVB->update();
		pVB->render();

		pTexColour->unbindTexture();	// Unbind textures
		pShader->unbind();				// Unbind the GUI shader

		// Render the slider
		_mSlider.render(pParentContainer, strFramebufferToSampleFrom);
	}

	void CGUITextScroll::update(void* pParentContainer, bool bParentContainerAcceptingMouseClicks)
	{
		CGUIContainer* pContainer = (CGUIContainer*)pParentContainer;
		SCGUIManager* pGUIManager = SCGUIManager::getPointer();
		CGUITheme* pTheme = pContainer->getTheme();

		// Update the slider
		_mSlider.mfPositionX = mfPositionX + mfWidth - pTheme->mfTextScrollSliderWidth;
		_mSlider.mfPositionY = mfPositionY;
		_mSlider.mfWidth = pTheme->mfTextScrollSliderWidth;
		_mSlider.mfHeight = mfHeight;

		_mSlider.update(pParentContainer, bParentContainerAcceptingMouseClicks);

		float fSliderTabPos = _mSlider.getTabPos();
		if (fSliderTabPos != _mfPreviousSliderTabPos)
		{
			_mfPreviousSliderTabPos = fSliderTabPos;
			_mbFBNeedsUpdating = true;
		}

		// Update this object's tooltip
		SCInputManager* pInput = SCInputManager::getPointer();
		CVector2f vMousePos = pInput->mouse.getCursorPos();
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
		// Update tooltip
		mpTooltip->update(pParentContainer, (CGUIBaseObject*)this, bMouseOver);

		// Also accept mousewheel movement to move the slider if mouse cursor is over the text/slider
		if (pInput->mouse.deltaZ())
		{
			if (bMouseOver)
			{
				float fTabPosOffset = 0;
				if (_mSlider.getOrientationHorizontal())
				{
					float fMouseWheel = -pInput->mouse.deltaZ() * 12.0f;	// Positive for scroll up, negative for scroll down
					float fTabTotalMovementAmount = _mSlider.mfWidth - _mSlider.getTabDims().x;
					if (areFloatsEqual(fTabTotalMovementAmount, 0.0f))	// Prevent divide by zero
						fTabTotalMovementAmount = 0.0001f;
					fTabPosOffset += fMouseWheel * (1.0f / fTabTotalMovementAmount);
				}
				else
				{
					float fMouseWheel = -pInput->mouse.deltaZ() * 12.0f;	// Positive for scroll up, negative for scroll down
					float fTabTotalMovementAmount = _mSlider.mfHeight - _mSlider.getTabDims().y;
					if (areFloatsEqual(fTabTotalMovementAmount, 0.0f))	// Prevent divide by zero
						fTabTotalMovementAmount = 0.0001f;
					fTabPosOffset += fMouseWheel * (1.0f / fTabTotalMovementAmount);
				}
				float fSliderTabPos = _mSlider.getTabPos();
				fSliderTabPos += fTabPosOffset;
				_mSlider.setTabPos(fSliderTabPos);
				if (fSliderTabPos != _mfPreviousSliderTabPos)
				{
					_mfPreviousSliderTabPos = fSliderTabPos;
					_mbFBNeedsUpdating = true;
				}
			}
		}
	}

	void CGUITextScroll::setText(const std::string& strText)
	{
		_mstrText = strText;
		_mbFBNeedsUpdating = true;
	}

	void CGUITextScroll::_renderFramebuffer(void* pParentContainer)
	{
		CGUIContainer* pContainer = (CGUIContainer*)pParentContainer;
		SCResourceManager* pRM = SCResourceManager::getPointer();
		SCGUIManager* pGUIManager = SCGUIManager::getPointer();
		CGUITheme* pTheme = pContainer->getTheme();
		CResourceFont* pFont = pRM->getFont(pTheme->mFonts.textScroll);
		CResourceFramebuffer* pFB = pRM->getFramebuffer(_mstrFBName);
		CResourceTexture2DFromFile* pTex = pRM->getTexture2DFromFile(pTheme->mImages.textScrollBGColour);

		// Unbind the render target which the GUI is currently rendering to.
		CResourceFramebuffer* pFBGUITarget = pRM->getFramebuffer("X:backbuffer_FB");
		pFBGUITarget->unbindAsRenderTarget();

		// Resize framebuffer if needed to make room for the slider
		unsigned int uiFBWidth = pFB->getWidth();
		unsigned int uiRequiredFBWidth = unsigned int (mfWidth - pTheme->mfTextScrollSliderWidth - (pTex->mvDimensions.x * 0.6666666f));
		if (uiFBWidth != uiRequiredFBWidth)
			pFB->resize(uiRequiredFBWidth, unsigned int (mfHeight - (pTex->mvDimensions.y * 0.6666666f)));

		// Set framebuffer as render target and clear it
		pFB->bindAsRenderTarget(true, false, pTheme->mColours.textScrollBackground);

		// Do not render, but obtain each line of text and the total height of all text which is rendered.
		std::vector<std::string> vstrLines;
		int iTextTotalHeight;
		pFont->printInRectNewline(false, _mstrText, 0, 0, (int)pFB->getWidth(), (int)pFB->getHeight(), vstrLines, iTextTotalHeight, "\n", 1.0f);

		// If the text fits inside the height of the TextScroll object, prevent scrolling
		if (float(iTextTotalHeight <= mfHeight))
			_mSlider.setTabPos(0.0f);

		// Now compute text position based on slider offset
		float fSliderPos = 1.0f - _mSlider.getTabPos();
		float fTotalMovement = float(iTextTotalHeight) -mfHeight;
		fTotalMovement += pFont->getTextHeight();
		float fTextPosY = fSliderPos * fTotalMovement;
		fTextPosY *= -0.5f;

		// Prevent divide by zero
		float fTabRatio = 1;
		if (iTextTotalHeight > 0)
			fTabRatio = mfHeight / float(iTextTotalHeight);
		_mSlider.setTabRatio(fTabRatio);

		pFont->printInRectNewline(true, _mstrText, 0, (int)fTextPosY, (int)pFB->getWidth(), (int)pFB->getHeight(), vstrLines, iTextTotalHeight, "\n", 1.0f, pTheme->mColours.textScrollText);
		pFB->unbindAsRenderTarget();
		_mbFBNeedsUpdating = false;

		// Remember to re-bind the GUI framebuffer back again
		pFBGUITarget->bindAsRenderTarget(false, false);
	}
}