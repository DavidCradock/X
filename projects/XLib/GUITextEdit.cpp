#include "PCH.h"
#include "GUITextEdit.h"
#include "GUIManager.h"
#include "resourceManager.h"
#include "window.h"
#include "input.h"
#include "audioManager.h"
#include "GUITooltip.h"

namespace X
{
	CGUITextEdit::CGUITextEdit()
	{
		_mState = state::inactive;
		_mfAddFlashingCursor = 0.0f;
		_muiMaxChars = 16;
		_mbIntegerInputOnly = false;
		_mbWasActiveEnterPressed = false;
		_mfuncOnEnterPressed = NULL;
		mpTooltip = new CGUITooltip;
	}

	CGUITextEdit::~CGUITextEdit()
	{
		delete mpTooltip;
	}

	void CGUITextEdit::render(void* pParentContainer, const std::string& strFramebufferToSampleFrom)
	{
		CGUIContainer* pContainer = (CGUIContainer*)pParentContainer;
		SCGUIManager* pGUIManager = SCGUIManager::getPointer();
		CGUITheme* pTheme = pContainer->getTheme();
		CColour col;
		renderBackground(pParentContainer, strFramebufferToSampleFrom, pTheme->mImages.textEditBGColour, pTheme->mImages.textEditBGNormal, col);

		// Get required resources needed to render
		SCResourceManager* pRM = SCResourceManager::getPointer();
		SCWindow* pWindow = SCWindow::getPointer();
		CResourceTexture2DFromFile* pTex = pRM->getTexture2DFromFile(pTheme->mImages.textEditBGColour);
		CVector2f vTexDimsPoint3 = pTex->mvDimensions * 0.3333333f;
		CVector2f vTexDimsPoint6 = pTex->mvDimensions * 0.6666666f;

		// Now render the font stuff
		int iRTDims[2];
		iRTDims[0] = int(pWindow->getWidth());
		iRTDims[1] = int(pWindow->getHeight());
		CResourceFont* pFont = pRM->getFont(pTheme->mFonts.textEdit);

		// Simply update to determine whether we add the flashing character when the state is active
		std::string strFinalText = mstrText;
		if (state::active == _mState)
		{
			if (_mfAddFlashingCursor >= 1.0f)
				strFinalText += "_";
		}


		// Enable stencil mask
		glEnable(GL_SCISSOR_TEST);
		int iScissorX = int(pContainer->mfPositionX + mfPositionX + vTexDimsPoint3.x);
		int iScissorY = iRTDims[1] - int(pContainer->mfPositionY + mfPositionY + mfHeight - vTexDimsPoint3.y + pTheme->mOffsets.textEditText.iOffsetY);
		glScissor(iScissorX, iScissorY, GLsizei(mfWidth - vTexDimsPoint6.x), (GLsizei)mfHeight);

		// Compute offset of text, based upon whether it fits in the text edit box or not
		int iOffsetX = 0;
		std::string strTextTemp = mstrText + "_";	// We use this to compute width to prevent moving forward and backwards of text when cursor flashes.
		int iTextWidth = (int)pFont->getTextWidth(strTextTemp, 1.0f);
		if (iTextWidth > (int)mfWidth - vTexDimsPoint6.x)
		{
			iOffsetX = int(mfWidth - iTextWidth - vTexDimsPoint6.x);
		}

		// Reset the offset so that the beginning of the text is displayed, IF the text edit box is not active
		if (_mState == state::inactive)
			iOffsetX = 0;

		pFont->print(strFinalText,			// The text
			int(iOffsetX + pContainer->mfPositionX + mfPositionX + vTexDimsPoint3.x + pTheme->mOffsets.textEditText.iOffsetX),	// X position
			int(pContainer->mfPositionY + mfPositionY + vTexDimsPoint3.y + pTheme->mOffsets.textEditText.iOffsetY),				// Y position
			iRTDims[0], iRTDims[1],	// Render target dims
			1.0f,					// Scaling
			_mTextColour);			// Colour

		glDisable(GL_SCISSOR_TEST);
	}

	void CGUITextEdit::update(void* pParentContainer, bool bParentContainerAcceptingMouseClicks)
	{
		_mbWasActiveEnterPressed = false;

		SCGUIManager* pGUIMan = SCGUIManager::getPointer();
		SCInputManager* pInput = SCInputManager::getPointer();
		SCResourceManager* pResMan = SCResourceManager::getPointer();
		CGUIContainer* pContainer = (CGUIContainer*)pParentContainer;
		CGUITheme* pTheme = pContainer->getTheme();
		SCAudioManager* pAudio = SCAudioManager::getPointer();
		CVector2f vMousePos = pInput->mouse.getCursorPos();
		_mTimer.update();
		float fSecondsPast = _mTimer.getSecondsPast();
		if (fSecondsPast > 0.1f)
			fSecondsPast = 0.1f;
		_mfAddFlashingCursor += fSecondsPast * pTheme->mfTextEditFlashSpeed;
		while (_mfAddFlashingCursor >= 2.0f)
			_mfAddFlashingCursor -= 2.0f;

		
		CResourceTexture2DFromFile* pColourTex = pResMan->getTexture2DFromFile(pTheme->mImages.textEditBGColour);
		CVector2f vTexDimsDiv3 = pColourTex->mvDimensions * 0.3333333f;
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
			if (pInput->key.once(KC_RETURN) || pInput->key.once(KC_NUMPADENTER))
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
						else
						{
							pAudio->getEmitter(pTheme->mAudio.textEditNoMoreCharSpace.strSampleName)->play(pGUIMan->getAudioVol() * pTheme->mAudio.textEditNoMoreCharSpace.fVolume, pTheme->mAudio.textEditNoMoreCharSpace.fPitch);
						}
					}
					if (pInput->key.repeat(KC_SPACE))
					{
						if (mstrText.length() < _muiMaxChars)
						{
							mstrText += " ";
							pAudio->getEmitter(pTheme->mAudio.textEditTextAdd.strSampleName)->play(pGUIMan->getAudioVol() * pTheme->mAudio.textEditTextAdd.fVolume, pTheme->mAudio.textEditTextAdd.fPitch);
						}
						else
						{
							pAudio->getEmitter(pTheme->mAudio.textEditNoMoreCharSpace.strSampleName)->play(pGUIMan->getAudioVol()* pTheme->mAudio.textEditNoMoreCharSpace.fVolume, pTheme->mAudio.textEditNoMoreCharSpace.fPitch);
						}
					}
				}
			}
		}

		// Update this object's tooltip
		mpTooltip->update(pParentContainer, (CGUIBaseObject*)this, bMouseOver);
	}
	
	void CGUITextEdit::setMaxChars(unsigned int iMaxChars)
	{
		_muiMaxChars = iMaxChars;
	}

	void CGUITextEdit::setIntegerInputOnly(bool bAllowIntegersOnly)
	{
		_mbIntegerInputOnly = bAllowIntegersOnly;
	}

	void CGUITextEdit::_checkIsNumber(bool bResetToZero)
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

	bool CGUITextEdit::getEnterPressed(void) const
	{
		return _mbWasActiveEnterPressed;
	}

	void CGUITextEdit::setOnEnter(void (*func)(const std::string& text))
	{
		_mfuncOnEnterPressed = func;
	}
}