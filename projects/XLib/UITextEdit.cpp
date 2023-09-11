#include "PCH.h"
#include "UITextEdit.h"
#include "utilities.h"
#include "UIContainer.h"
#include "singletons.h"
#include "UITheme.h"

namespace X
{
	CUITextEdit::CUITextEdit(CUIContainer* pContainer, const std::string& strWidgetName)
	{
		_mpContainer = pContainer;
		_mvDimensions.set(200, 48);
		_mbVisible = true;

		_mState = state::inactive;
		_mfAddFlashingCursor = 0.0f;
		_muiMaxChars = 16;
		_mbIntegerInputOnly = false;
		_mbWasActiveEnterPressed = false;
		_mfuncOnEnterPressed = NULL;

	}

	CUITextEdit::~CUITextEdit()
	{
	}

	void CUITextEdit::setDimensions(float fX, float fY)
	{
		_mvDimensions.x = fX;
		_mvDimensions.y = fY;
		_mpContainer->computeScrollbars();
	}

	void CUITextEdit::setDimensions(const CVector2f& vDimensions)
	{
		_mvDimensions = vDimensions;
		_mpContainer->computeScrollbars();
	}

	CVector2f CUITextEdit::getDimensions(void) const
	{
		return _mvDimensions;
	}

	void CUITextEdit::setPosition(float fX, float fY)
	{
		_mvPosition.x = fX;
		_mvPosition.y = fY;
		_mpContainer->computeScrollbars();
	}

	void CUITextEdit::setPosition(const CVector2f& vPosition)
	{
		_mvPosition = vPosition;
		_mpContainer->computeScrollbars();
	}

	CVector2f CUITextEdit::getPosition(void) const
	{
		return _mvPosition;
	}

	void CUITextEdit::setVisible(bool bVisible)
	{
		_mbVisible = bVisible;
		_mpContainer->computeScrollbars();
	}

	bool CUITextEdit::getVisible(void) const
	{
		return _mbVisible;
	}

	void CUITextEdit::render(CResourceVertexBufferCPT2* pVB)
	{
		if (!_mbVisible)
			return;

		const CUITheme::SSettings* pThemeSettings = _mpContainer->themeGetSettings();

		CColour col;
		// Add geometry for the 9 grid cells
		x->pUI->_helperAddWidgetGridGeometry(
			_mvPosition + _mpContainer->getWidgetOffset(),
			_mvDimensions,
			pThemeSettings->images.textEditBG,
			col,
			_mpContainer,
			pVB);
	}

	void CUITextEdit::renderFonts(void)
	{
		CUITheme* pTheme = _mpContainer->themeGet();
		const CUITheme::SSettings* pThemeSettings = pTheme->getSettings();
		
		// Get required resources needed to render
		CResourceTexture2DAtlas* pAtlas = pTheme->getTextureAtlas();
		CImageAtlasDetails idTL = pAtlas->getImageDetails(pThemeSettings->images.textEditBG.colour.cornerTL);
		CImageAtlasDetails idBR = pAtlas->getImageDetails(pThemeSettings->images.textEditBG.colour.cornerBR);

		// Now render the font stuff
		int iRTDims[2];
		iRTDims[0] = int(x->pWindow->getWidth());
		iRTDims[1] = int(x->pWindow->getHeight());
		CResourceFont* pFont = x->pResource->getFont(pThemeSettings->fonts.textEdit);

		// Simply update to determine whether we add the flashing character when the state is active
		std::string strFinalText = _mstrText;
		if (state::active == _mState)
		{
			if (_mfAddFlashingCursor >= 1.0f)
				strFinalText += "_";
		}

		CVector2f vTextPosTL = _mpContainer->getWidgetAreaTLCornerPosition();
		vTextPosTL += _mvPosition;
		vTextPosTL += _mpContainer->getWidgetOffset();
		vTextPosTL += idTL.vDims;
		CVector2f vTextPosBR = _mpContainer->getWidgetAreaTLCornerPosition();
		vTextPosBR += _mvPosition;
		vTextPosBR += _mpContainer->getWidgetOffset();
		vTextPosBR += _mvDimensions;
		vTextPosBR -= idBR.vDims;

		// Enable stencil mask
		glEnable(GL_SCISSOR_TEST);
		int iScissorX = int(vTextPosTL.x);
		int iScissorY = iRTDims[1] - int(vTextPosBR.y);
		glScissor(iScissorX, iScissorY, GLsizei(_mvDimensions.x - idTL.vDims.x - idBR.vDims.x), (GLsizei)_mvDimensions.y);

		// Compute offset of text, based upon whether it fits in the text edit box or not
		int iOffsetX = 0;
		std::string strTextTemp = _mstrText + "_";	// We use this to compute width to prevent moving forward and backwards of text when cursor flashes.
		int iTextWidth = (int)pFont->getTextWidth(strTextTemp, 1.0f);
		if (iTextWidth > int(_mvDimensions.x - idTL.vDims.x - idBR.vDims.x))
		{
			//iOffsetX = int(mfWidth - iTextWidth - vTexDimsPoint6.x);
			iOffsetX = int(_mvDimensions.x - iTextWidth - idTL.vDims.x - idBR.vDims.x);
		}

		// Reset the offset so that the beginning of the text is displayed, IF the text edit box is not active
		if (_mState == state::inactive)
			iOffsetX = 0;

		pFont->print(strFinalText,			// The text
			int(iOffsetX + vTextPosTL.x),	// X position
			int(vTextPosTL.y),				// Y position
			iRTDims[0], iRTDims[1],	// Render target dims
			1.0f,					// Scaling
			_mTextColour);			// Colour

		glDisable(GL_SCISSOR_TEST);
	}

	void CUITextEdit::update(float fTimeDeltaSec)
	{
		// Determine whether the mouse cursor is over this widget's container or not.
		// This is all to prevent doing expensive checks if we don't need to.
		bool bContainerHasMouseOver = false;
		std::string strMouseIsOver = x->pUI->getMouseIsOverWhichContainer();	// Get the name of the container/window that the mouse cursor is over, if any.
		if (strMouseIsOver == _mpContainer->getName())
			bContainerHasMouseOver = true;

		_mbWasActiveEnterPressed = false;

		const CUITheme::SSettings* pThemeSettings = _mpContainer->themeGetSettings();
		CVector2f vMousePos = x->pInput->mouse.getCursorPos();
		_mTimer.update();
		float fSecondsPast = _mTimer.getSecondsPast();
		if (fSecondsPast > 0.1f)
			fSecondsPast = 0.1f;
		_mfAddFlashingCursor += fSecondsPast * pThemeSettings->floats.textEditFlashSpeed;
		while (_mfAddFlashingCursor >= 2.0f)
			_mfAddFlashingCursor -= 2.0f;

		CUITheme* pTheme = _mpContainer->themeGet();
		CResourceTexture2DAtlas* pAtlas = pTheme->getTextureAtlas();
		CImageAtlasDetails idTL = pAtlas->getImageDetails(pThemeSettings->images.textEditBG.colour.cornerTL);
//		CImageAtlasDetails idBR = pAtlas->getImageDetails(pThemeSettings->images.textEditBG.colour.cornerBR);

		CVector2f vWidgetTL = _mpContainer->getWidgetAreaTLCornerPosition() + _mpContainer->getWidgetOffset() + _mvPosition;
		CVector2f vWidgetBR = vWidgetTL + _mvDimensions;

		bool bMouseOver = false;
		if (bContainerHasMouseOver)
		{
			// Determine if mouse cursor is over
			if (vMousePos.x > vWidgetTL.x)
				if (vMousePos.x < vWidgetBR.x)
					if (vMousePos.y > vWidgetTL.x)
						if (vMousePos.y < vWidgetBR.y)
							bMouseOver = true;

			// Determine current state of the textedit box
			// Mouse clicking on or off to activate/deactivate
			if (bMouseOver)
			{
				if (x->pInput->mouse.leftButtonOnce())
				{
					if (_mState != state::active)
					{
						_mState = state::active;
//						x->pAudio->getEmitter(pTheme->mAudio.textEditActivate.strSampleName)->play(x->pGUI->getAudioVol() * pTheme->mAudio.textEditActivate.fVolume, pTheme->mAudio.textEditActivate.fPitch);
					}
				}
			}
			else
			{
				if (x->pInput->mouse.leftButDown())
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
			_mTextColour = pThemeSettings->colours.textEditTextActive;
		}
		else if (state::inactive == _mState)
		{
			_mTextColour = pThemeSettings->colours.textEditTextInactive;
		}

		// Now add/remove characters to the text based on keyboard input
		if (state::active == _mState)
		{
			if (x->pInput->key.once(KC_RETURN) || x->pInput->key.once(KC_NUMPADENTER))
			{
				_mState = state::inactive;
//				x->pAudio->getEmitter(pTheme->mAudio.textEditReturn.strSampleName)->play(x->pGUI->getAudioVol() * pTheme->mAudio.textEditReturn.fVolume, pTheme->mAudio.textEditReturn.fPitch);
				// Check to see if we're only accepting numbers with _mbNumberInputOnly and if so, check the string and empty it if it isn't a number
				if (_mbIntegerInputOnly)
				{
					_checkIsNumber(true);
				}
				_mbWasActiveEnterPressed = true;	// So we can check this ourselves with getEnterPressed()

				// Call C function pointer if set
				if (_mfuncOnEnterPressed != NULL)
					_mfuncOnEnterPressed(_mstrText);
			}
			else if (x->pInput->key.anyPressed())
			{
				if (x->pInput->key.repeat(KC_BACK))
				{
					if (_mstrText.length() > 0)
					{
						_mstrText.pop_back();
//						x->pAudio->getEmitter(pTheme->mAudio.textEditBackspace.strSampleName)->play(x->pGUI->getAudioVol() * pTheme->mAudio.textEditBackspace.fVolume, pTheme->mAudio.textEditBackspace.fPitch);
					}
				}
				else
				{
					std::string strNewChar = x->pInput->key.once2Char();
					if (strNewChar.length() > 0)
					{
						if (_mstrText.length() < _muiMaxChars)
						{
							_mstrText += strNewChar;
//							x->pAudio->getEmitter(pTheme->mAudio.textEditTextAdd.strSampleName)->play(x->pGUI->getAudioVol() * pTheme->mAudio.textEditTextAdd.fVolume, pTheme->mAudio.textEditTextAdd.fPitch);
						}
						else
						{
//							x->pAudio->getEmitter(pTheme->mAudio.textEditNoMoreCharSpace.strSampleName)->play(x->pGUI->getAudioVol() * pTheme->mAudio.textEditNoMoreCharSpace.fVolume, pTheme->mAudio.textEditNoMoreCharSpace.fPitch);
						}
					}
					if (x->pInput->key.repeat(KC_SPACE))
					{
						if (_mstrText.length() < _muiMaxChars)
						{
							_mstrText += " ";
//							x->pAudio->getEmitter(pTheme->mAudio.textEditTextAdd.strSampleName)->play(x->pGUI->getAudioVol() * pTheme->mAudio.textEditTextAdd.fVolume, pTheme->mAudio.textEditTextAdd.fPitch);
						}
						else
						{
//							x->pAudio->getEmitter(pTheme->mAudio.textEditNoMoreCharSpace.strSampleName)->play(x->pGUI->getAudioVol() * pTheme->mAudio.textEditNoMoreCharSpace.fVolume, pTheme->mAudio.textEditNoMoreCharSpace.fPitch);
						}
					}
				}
			}
		}

		// Update this object's tooltip
//		mpTooltip->update(pParentContainer, (CGUIBaseObject*)this, bMouseOver);
	}

	void CUITextEdit::reset(void)
	{
		const CUITheme::SSettings* pSettings = _mpContainer->themeGetSettings();
//		_mColourBG = pSettings->colours.buttonBGUp;
//		_mColourText = pSettings->colours.buttonTextUp;
	}

	/******************************************************************* Widget specific *******************************************************************/

	void CUITextEdit::setText(const std::string& strText)
	{
		_mstrText = strText;
	}

	std::string CUITextEdit::getText(void) const
	{
		return _mstrText;
	}

	void CUITextEdit::setMaxChars(unsigned int iMaxChars)
	{
		_muiMaxChars = iMaxChars;
	}

	void CUITextEdit::setIntegerInputOnly(bool bAllowIntegersOnly)
	{
		_mbIntegerInputOnly = bAllowIntegersOnly;
	}

	void CUITextEdit::_checkIsNumber(bool bResetToZero)
	{
		if (0 == _mstrText.length())
			return;
		if (std::all_of(_mstrText.begin(), _mstrText.end(), ::isdigit))
			return;
		if (bResetToZero)
		{
			_mstrText = "0";
		}
	}

	bool CUITextEdit::getEnterPressed(void) const
	{
		return _mbWasActiveEnterPressed;
	}

	void CUITextEdit::setOnEnter(void (*func)(const std::string& text))
	{
		_mfuncOnEnterPressed = func;
	}
}