#include "PCH.h"
#include "GUITextEdit.h"
#include "GUIManager.h"
#include "resourceManager.h"
#include "window.h"
#include "input.h"
#include "audioManager.h"

namespace X
{
	GUITextEdit::GUITextEdit()
	{
		_mState = state::inactive;
		_mfAddFlashingCursor = 0.0f;
		_muiMaxChars = 16;
		_mbIntegerInputOnly = false;
		_mbWasActiveEnterPressed = false;
		_mfuncOnEnterPressed = NULL;
	}

	void GUITextEdit::render(void* pParentContainer, const std::string& strFramebufferToSampleFrom)
	{
		GUIContainer* pContainer = (GUIContainer*)pParentContainer;

		// Get required resources needed to render
		GUIManager* pGUI = GUIManager::getPointer();
		ResourceManager* pRM = ResourceManager::getPointer();
		Window* pWindow = Window::getPointer();
		ResourceTriangle* pTri = pRM->getTriangle("X:gui");
		ResourceShader* pShader = pRM->getShader("X:gui");
		GUITheme* pTheme = pGUI->getTheme(pContainer->mstrThemename);
		InputManager* pInput = InputManager::getPointer();

		pShader->bind();

		// Setup the projection matrix as orthographic
		glm::mat4 matProjection = glm::ortho(0.0f, float(pWindow->getWidth()), float(pWindow->getHeight()), 0.0f, -1.0f, 1.0f);
		pShader->setMat4("transform", matProjection);

		// Tell OpenGL, for each sampler, to which texture unit it belongs to
		pShader->setInt("texture0_colour", 0);
		pShader->setInt("texture1_normal", 1);
		pShader->setInt("texture2_reflection", 2);
		pShader->setInt("texture3_background", 3);
		pShader->setFloat("fBlurAmount", pTheme->mfBlurAmount);
		pShader->setFloat("fNormalAmount", pTheme->mfNormalAmount);
		pShader->setFloat("fReflectionAmount", pTheme->mfReflectionAmount);
		pShader->setFloat("fMouseCursorDistance", pTheme->mfMouseCursorDistance);

		// Set mouse position, inverting Y position
		glm::vec2 vMousePos = pInput->mouse.getCursorPos();
		vMousePos.y = float(pWindow->getHeight()) - vMousePos.y;
		pShader->setVec2("v2MousePos", vMousePos);

		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);

		// Get textures
		ResourceTexture2D* pTexColour = pRM->getTexture2D(pTheme->mImages.textEditColour);
		ResourceTexture2D* pTexNormal = pRM->getTexture2D(pTheme->mImages.textEditNormal);
		ResourceTexture2D* pTexReflection = pRM->getTexture2D(pTheme->mImages.reflection);
		ResourceFramebuffer* pFBSample = pRM->getFramebuffer(strFramebufferToSampleFrom);

		// Bind textures
		pTexColour->bind(0);
		pTexNormal->bind(1);
		pTexReflection->bind(2);
		pFBSample->bindAsTexture(3);

		// Render the centre
		glm::vec2 vTexDimsDiv3 = pTexColour->mvDimensions * 0.3333333f;
		pTri->removeGeom();
		pTri->addQuad2D(
			glm::vec2(
				pContainer->mfPositionX + mfPositionX + vTexDimsDiv3.x,		// Position X
				pContainer->mfPositionY + mfPositionY + vTexDimsDiv3.y),	// Position Y
			glm::vec2(mfWidth, mfHeight),									// Dimensions
			glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),								// Vertex colour
			mTC.centre.BL,
			mTC.centre.BR,
			mTC.centre.TR,
			mTC.centre.TL);

		// Render the left edge
		pTri->addQuad2D(
			glm::vec2(
				pContainer->mfPositionX + mfPositionX,					// Position X
				pContainer->mfPositionY + mfPositionY + vTexDimsDiv3.y),// Position Y
			glm::vec2(vTexDimsDiv3.x, mfHeight),						// Dimensions
			glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),							// Vertex colour
			mTC.left.BL,
			mTC.left.BR,
			mTC.left.TR,
			mTC.left.TL);

		// Render the right edge
		pTri->addQuad2D(
			glm::vec2(
				pContainer->mfPositionX + mfPositionX + mfWidth + vTexDimsDiv3.x,	// Position X
				pContainer->mfPositionY + mfPositionY + vTexDimsDiv3.y),			// Position Y
			glm::vec2(vTexDimsDiv3.x, mfHeight),									// Dimensions
			glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),										// Vertex colour
			mTC.right.BL,
			mTC.right.BR,
			mTC.right.TR,
			mTC.right.TL);

		// Render the top edge
		pTri->addQuad2D(
			glm::vec2(
				pContainer->mfPositionX + mfPositionX + vTexDimsDiv3.x,	// Position X
				pContainer->mfPositionY + mfPositionY),					// Position Y
			glm::vec2(mfWidth, vTexDimsDiv3.y),							// Dimensions
			glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),							// Vertex colour
			mTC.top.BL,
			mTC.top.BR,
			mTC.top.TR,
			mTC.top.TL);

		// Render the bottom edge
		pTri->addQuad2D(
			glm::vec2(
				pContainer->mfPositionX + mfPositionX + vTexDimsDiv3.x,					// Position X
				pContainer->mfPositionY + mfPositionY + mfHeight + vTexDimsDiv3.y),		// Position Y
			glm::vec2(mfWidth, vTexDimsDiv3.y),											// Dimensions
			glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),											// Vertex colour
			mTC.bottom.BL,
			mTC.bottom.BR,
			mTC.bottom.TR,
			mTC.bottom.TL);

		// Render the top left corner
		pTri->addQuad2D(
			glm::vec2(
				pContainer->mfPositionX + mfPositionX,	// Position X
				pContainer->mfPositionY + mfPositionY),	// Position Y
			glm::vec2(vTexDimsDiv3.x, vTexDimsDiv3.y),	// Dimensions
			glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),			// Vertex colour
			mTC.topLeft.BL,
			mTC.topLeft.BR,
			mTC.topLeft.TR,
			mTC.topLeft.TL);

		// Render the top right corner
		pTri->addQuad2D(
			glm::vec2(
				pContainer->mfPositionX + mfPositionX + mfWidth + vTexDimsDiv3.x,	// Position X
				pContainer->mfPositionY + mfPositionY),								// Position Y
			glm::vec2(vTexDimsDiv3.x, vTexDimsDiv3.y),								// Dimensions
			glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),										// Vertex colour
			mTC.topRight.BL,
			mTC.topRight.BR,
			mTC.topRight.TR,
			mTC.topRight.TL);

		// Render the bottom left corner
		pTri->addQuad2D(
			glm::vec2(
				pContainer->mfPositionX + mfPositionX,								// Position X
				pContainer->mfPositionY + mfPositionY + mfHeight + vTexDimsDiv3.y),	// Position Y
			glm::vec2(vTexDimsDiv3.x, vTexDimsDiv3.y),								// Dimensions
			glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),										// Vertex colour
			mTC.bottomLeft.BL,
			mTC.bottomLeft.BR,
			mTC.bottomLeft.TR,
			mTC.bottomLeft.TL);

		// Render the bottom right corner
		pTri->addQuad2D(
			glm::vec2(
				pContainer->mfPositionX + mfPositionX + mfWidth + vTexDimsDiv3.x,	// Position X
				pContainer->mfPositionY + mfPositionY + mfHeight + vTexDimsDiv3.y),	// Position Y
			glm::vec2(vTexDimsDiv3.x, vTexDimsDiv3.y),								// Dimensions
			glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),										// Vertex colour
			mTC.bottomRight.BL,
			mTC.bottomRight.BR,
			mTC.bottomRight.TR,
			mTC.bottomRight.TL);

		pTri->update();
		pTri->draw();

		pTexColour->unbindAll();	// Unbind textures
		pShader->unbind();	// Unbind the GUI shader
		glDisable(GL_BLEND);

		// Now render the font stuff
		int iRTDims[2];
		iRTDims[0] = int(pWindow->getWidth());
		iRTDims[1] = int(pWindow->getHeight());
		ResourceFont* pFont = pRM->getFont(pTheme->mFonts.textEdit);

		// Simple update to determine whether we add the flashing character when the state is active
		std::string strFinalText = mstrText;
		if (state::active == _mState)
		{
			if (_mfAddFlashingCursor >= 1.0f)
				strFinalText += "_";
		}

		// Enable stencil mask
		glEnable(GL_SCISSOR_TEST);
		int iScissorX = int(pContainer->mfPositionX + mfPositionX + vTexDimsDiv3.x);
		int iScissorY = iRTDims[1] - int(pContainer->mfPositionY + mfPositionY + mfHeight + vTexDimsDiv3.y + pTheme->mOffsets.textEditText.iOffsetY);
		glScissor(iScissorX, iScissorY, (GLsizei)mfWidth, (GLsizei)mfHeight);

		// Compute offset of text, based upon whether it fits in the text edit box or not
		int iOffsetX = 0;
		std::string strTextTemp = mstrText + "_";	// We use this to compute width to prevent moving forward and backwards of text when cursor flashes.
		int iTextWidth = (int)pFont->getTextWidth(strTextTemp, 1.0f);
		if (iTextWidth > (int)mfWidth)
		{
			iOffsetX = (int)mfWidth - iTextWidth;
		}

		// Reset the offset so that the beginning of the text is displayed, IF the text edit box is not active
		if (_mState == state::inactive)
			iOffsetX = 0;

		pFont->print(strFinalText,			// The text
			int(iOffsetX + pContainer->mfPositionX + mfPositionX + vTexDimsDiv3.x + pTheme->mOffsets.textEditText.iOffsetX),		// X position
			int(pContainer->mfPositionY + mfPositionY + vTexDimsDiv3.y + pTheme->mOffsets.textEditText.iOffsetY),		// Y position
			iRTDims[0], iRTDims[1],	// Render target dims
			1.0f,	// Scaling
			_mTextColour.get());	// Colour

		glDisable(GL_SCISSOR_TEST);
	}

	void GUITextEdit::update(void* pParentContainer, bool bParentContainerAcceptingMouseClicks)
	{
		_mbWasActiveEnterPressed = false;

		GUIManager* pGUIMan = GUIManager::getPointer();
		InputManager* pInput = InputManager::getPointer();
		ResourceManager* pResMan = ResourceManager::getPointer();
		GUIContainer* pContainer = (GUIContainer*)pParentContainer;
		GUITheme* pTheme = pGUIMan->getTheme(pContainer->mstrThemename);	// Get parent container's theme
		AudioManager* pAudio = AudioManager::getPointer();
		glm::vec2 vMousePos = pInput->mouse.getCursorPos();
		_mTimer.update();
		float fSecondsPast = _mTimer.getSecondsPast();
		if (fSecondsPast > 0.1f)
			fSecondsPast = 0.1f;
		_mfAddFlashingCursor += fSecondsPast * pTheme->mfTextEditFlashSpeed;
		while (_mfAddFlashingCursor >= 2.0f)
			_mfAddFlashingCursor -= 2.0f;

		
		ResourceTexture2D* pColourTex = pResMan->getTexture2D(pTheme->mImages.textEditColour);
		glm::vec2 vTexDimsDiv3 = pColourTex->mvDimensions * 0.3333333f;
		bool bMouseOver = false;
		if (bParentContainerAcceptingMouseClicks)
		{
			// Determine if mouse cursor is over
			if (vMousePos.x > pContainer->mfPositionX + mfPositionX - vTexDimsDiv3.x)
				if (vMousePos.x < pContainer->mfPositionX + mfPositionX + mfWidth + (vTexDimsDiv3.x * 2.0f))
					if (vMousePos.y > pContainer->mfPositionY + mfPositionY)
						if (vMousePos.y < pContainer->mfPositionY + mfPositionY + mfHeight + (vTexDimsDiv3.y * 2.0f))
							bMouseOver = true;

			// Determine current state of the textedit box
			// Mouse clicking on or off to activate/deactivate
			if (bMouseOver)
			{
				if (pInput->mouse.leftButtonOnce())
				{
					if (_mState != state::active)
					{
						_mState = state::active;
						pAudio->getEmitter(pTheme->mAudio.textEditActivate.strSampleName)->play(pGUIMan->getAudioVol() * pTheme->mAudio.textEditActivate.fVolume, pTheme->mAudio.textEditActivate.fPitch);
					}
				}
			}
			else
			{
				if (pInput->mouse.leftButDown())
				{
					_mState = state::inactive;

					// Check to see if we're only accepting numbers with _mbNumberInputOnly and if so, check the string and empty it if it isn't a number
					if (_mbIntegerInputOnly)
					{
						_checkIsNumber(true);
					}
				}
			}
		}
		else  // Not accepting mouse clicks
		{
			_mState = state::inactive;
			// Check to see if we're only accepting numbers with _mbNumberInputOnly and if so, check the string and empty it if it isn't a number
			if (_mbIntegerInputOnly)
			{
				_checkIsNumber(true);
			}
		}

		// Depending upon state, compute text colour
		if (state::active == _mState)
		{
			_mTextColour = pTheme->mColours.textEditActive;
		}
		else if (state::inactive == _mState)
		{
			_mTextColour = pTheme->mColours.textEditInactive;
		}

		// Now add/remove characters to the text based on keyboard input
		if (state::active == _mState)
		{
			if (pInput->key.pressed(KC_RETURN) || pInput->key.once(KC_NUMPADENTER))
			{
				_mState = state::inactive;
				pAudio->getEmitter(pTheme->mAudio.textEditReturn.strSampleName)->play(pGUIMan->getAudioVol() * pTheme->mAudio.textEditReturn.fVolume, pTheme->mAudio.textEditReturn.fPitch);
				// Check to see if we're only accepting numbers with _mbNumberInputOnly and if so, check the string and empty it if it isn't a number
				if (_mbIntegerInputOnly)
				{
					_checkIsNumber(true);
				}
				_mbWasActiveEnterPressed = true;	// So we can check this ourselves with getEnterPressed()

				// Call C function pointer if set
				if (_mfuncOnEnterPressed != NULL)
					_mfuncOnEnterPressed(mstrText);
			}
			else if (pInput->key.anyPressed())
			{
				if (pInput->key.repeat(KC_BACK))
				{
					if (mstrText.length() > 0)
					{
						mstrText.pop_back();
						pAudio->getEmitter(pTheme->mAudio.textEditBackspace.strSampleName)->play(pGUIMan->getAudioVol() * pTheme->mAudio.textEditBackspace.fVolume, pTheme->mAudio.textEditBackspace.fPitch);
					}
				}
				else
				{
					std::string strNewChar = pInput->key.once2Char();
					if (strNewChar.length() > 0)
					{
						if (mstrText.length() < _muiMaxChars)
						{
							mstrText += strNewChar;
							pAudio->getEmitter(pTheme->mAudio.textEditTextAdd.strSampleName)->play(pGUIMan->getAudioVol() * pTheme->mAudio.textEditTextAdd.fVolume, pTheme->mAudio.textEditTextAdd.fPitch);
						}
					}
					if (pInput->key.repeat(KC_SPACE))
					{
						if (mstrText.length() < _muiMaxChars)
						{
							mstrText += " ";
							pAudio->getEmitter(pTheme->mAudio.textEditTextAdd.strSampleName)->play(pGUIMan->getAudioVol() * pTheme->mAudio.textEditTextAdd.fVolume, pTheme->mAudio.textEditTextAdd.fPitch);
						}
					}
				}
			}
		}
	}
	
	void GUITextEdit::setMaxChars(unsigned int iMaxChars)
	{
		_muiMaxChars = iMaxChars;
	}

	void GUITextEdit::setIntegerInputOnly(bool bAllowIntegersOnly)
	{
		_mbIntegerInputOnly = bAllowIntegersOnly;
	}

	void GUITextEdit::_checkIsNumber(bool bResetToZero)
	{
		if (0 == mstrText.length())
			return;
		if (std::all_of(mstrText.begin(), mstrText.end(), ::isdigit))
			return;
		if (bResetToZero)
		{
			mstrText = "0";
		}
	}

	bool GUITextEdit::getEnterPressed(void)
	{
		return _mbWasActiveEnterPressed;
	}

	void GUITextEdit::setOnEnter(void (*func)(const std::string& text))
	{
		_mfuncOnEnterPressed = func;
	}
}