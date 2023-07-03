#include "PCH.h"
#include "GUITextScroll.h"
#include "GUIManager.h"
#include "resourceManager.h"
#include "window.h"
#include "input.h"
#include "GUITooltip.h"

namespace X
{
	GUITextScroll::GUITextScroll()
	{
		_mbFBNeedsUpdating = true;
		_mfPreviousSliderTabPos = -1.0f;
		_mSlider.setTabPos(0.0f);
		mpTooltip = new GUITooltip;
	}

	GUITextScroll::~GUITextScroll()
	{
		delete mpTooltip;
	}

	void GUITextScroll::render(void* pParentContainer, const std::string& strFramebufferToSampleFrom)
	{
		GUIContainer* pContainer = (GUIContainer*)pParentContainer;
		GUIManager* pGUIManager = GUIManager::getPointer();
		GUITheme* pTheme = pGUIManager->getTheme(pContainer->mstrThemename);
		GUIColour col;
		renderBackground(pParentContainer, strFramebufferToSampleFrom, pTheme->mImages.textEditBGColour, pTheme->mImages.textScrollBGNormal, col);
		ResourceManager* pRM = ResourceManager::getPointer();
		ResourceFramebuffer* pTexColour = pRM->getFramebuffer(_mstrFBName);

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
		
		Window* pWindow = Window::getPointer();
		ResourceTriangle* pTri = pRM->getTriangle("X:gui");
		ResourceShader* pShader = pRM->getShader("X:pos_col_tex");

		// Render the frame buffer over the top of the background
		pShader->bind();

		// Setup the projection matrix as orthographic
		glm::mat4 matProjection = glm::ortho(0.0f, float(pWindow->getWidth()), float(pWindow->getHeight()), 0.0f, -1.0f, 1.0f);
		pShader->setMat4("transform", matProjection);

		// Tell OpenGL, for each sampler, to which texture unit it belongs to
		pShader->setInt("texture0", 0);

		glDisable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);

		glm::vec2 vBGdims = pRM->getTexture2D(pTheme->mImages.textScrollBGColour)->mvDimensions;
		glm::vec2 vBGDimsPoint3 = vBGdims * 0.3333333f;
		glm::vec2 vBGDimsPoint6 = vBGdims * 0.6666666f;

		// Bind textures
		pTexColour->bindAsTexture(0);

		// Render the image
		pTri->removeGeom();
		pTri->addQuad2D(
			glm::vec2(pContainer->mfPositionX + mfPositionX + vBGDimsPoint3.x, pContainer->mfPositionY + mfPositionY + vBGDimsPoint3.y),	// Position
			glm::vec2(mfWidth - vBGDimsPoint6.x - pTheme->mfTextScrollSliderWidth, mfHeight - vBGDimsPoint6.y),								// Dimensions
			glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),		// Vertex colour
			glm::vec2(0.0f, 0.0f),
			glm::vec2(1.0f, 0.0f),
			glm::vec2(1.0f, 1.0f),
			glm::vec2(0.0f, 1.0f));

		pTri->update();
		pTri->draw();

		pTexColour->unbindTexture();	// Unbind textures
		pShader->unbind();				// Unbind the GUI shader

		// Render the slider
		_mSlider.render(pParentContainer, strFramebufferToSampleFrom);
	}

	void GUITextScroll::update(void* pParentContainer, bool bParentContainerAcceptingMouseClicks)
	{
		GUIContainer* pContainer = (GUIContainer*)pParentContainer;
		GUIManager* pGUIManager = GUIManager::getPointer();
		GUITheme* pTheme = pGUIManager->getTheme(pContainer->mstrThemename);

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
		InputManager* pInput = InputManager::getPointer();
		glm::vec2 vMousePos = pInput->mouse.getCursorPos();
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
		GUITooltip* pTooltip = (GUITooltip*)mpTooltip;
		pTooltip->update(pParentContainer, (GUIBaseObject*)this, bMouseOver);
	}

	void GUITextScroll::setText(const std::string& strText)
	{
		_mstrText = strText;
		_mbFBNeedsUpdating = true;
	}

	void GUITextScroll::setTextColour(float fRed, float fGreen, float fBlue, float fAlpha)
	{
		_mTextColour.set(fRed, fGreen, fBlue, fAlpha);
		_mbFBNeedsUpdating = true;
	}

	void GUITextScroll::_renderFramebuffer(void* pParentContainer)
	{
		GUIContainer* pContainer = (GUIContainer*)pParentContainer;
		ResourceManager* pRM = ResourceManager::getPointer();
		GUIManager* pGUIManager = GUIManager::getPointer();
		GUITheme* pTheme = pGUIManager->getTheme(pContainer->mstrThemename);
		ResourceFont* pFont = pRM->getFont(pTheme->mFonts.textScroll);
		ResourceFramebuffer* pFB = pRM->getFramebuffer(_mstrFBName);
		ResourceTexture2D* pTex = pRM->getTexture2D(pTheme->mImages.textScrollBGColour);

		// Unbind the render target which the GUI is currently rendering to.
		ResourceFramebuffer* pFBGUITarget = pRM->getFramebuffer("X:backbuffer_FB");
		pFBGUITarget->unbindAsRenderTarget();

		// Resize framebuffer if needed to make room for the slider
		unsigned int uiFBWidth = pFB->getWidth();
		unsigned int uiRequiredFBWidth = unsigned int (mfWidth - pTheme->mfTextScrollSliderWidth - (pTex->mvDimensions.x * 0.6666666f));
		if (uiFBWidth != uiRequiredFBWidth)
			pFB->resize(uiRequiredFBWidth, unsigned int (mfHeight - (pTex->mvDimensions.y * 0.6666666f)));

		// Set framebuffer as render target and clear it
		pFB->bindAsRenderTarget(true, false);

		// Do not render, but obtain each line of text and the total height of all text which is rendered.
		std::vector<std::string> vstrLines;
		int iTextTotalHeight;
		pFont->printInRect(false, _mstrText, 0, 0, (int)pFB->getWidth(), (int)pFB->getHeight(), vstrLines, iTextTotalHeight, 1.0f, _mTextColour.get());

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

		pFont->printInRect(true, _mstrText, 0, (int)fTextPosY, (int)pFB->getWidth(), (int)pFB->getHeight(), vstrLines, iTextTotalHeight, 1.0f, _mTextColour.get());
		pFB->unbindAsRenderTarget();
		_mbFBNeedsUpdating = false;

		// Remember to re-bind the GUI framebuffer back again
		pFBGUITarget->bindAsRenderTarget(false, false);
	}
}