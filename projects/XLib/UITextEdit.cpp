#include "PCH.h"
#include "UITextEdit.h"
#include "utilities.h"
#include "UIContainer.h"
#include "UIText.h"
#include "singletons.h"
#include "UITheme.h"

namespace X
{
	CUITextEdit::CUITextEdit(CUIContainer* pContainer, const std::string& strWidgetName)
	{
		_mpContainer = pContainer;
		_mvDimensions = getDimensionsMinimum();
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

	void CUITextEdit::setDimensions(int iX, int iY)
	{
		setDimensions(float(iX), float(iY));
	}

	void CUITextEdit::setDimensions(const CVector2f& vDimensions)
	{
		setDimensions(vDimensions.x, vDimensions.y);
	}

	CVector2f CUITextEdit::getDimensions(void) const
	{
		return _mvDimensions;
	}

	CVector2f CUITextEdit::getDimensionsMinimum(void) const
	{
		CUITheme* pTheme = _mpContainer->themeGet();
		CResourceTexture2DAtlas* pAtlas = pTheme->getTextureAtlas();
		const CUITheme::SSettings* pThemeSettings = _mpContainer->themeGetSettings();

		CVector2f vMinimumDims;
		CImageAtlasDetails* pID = pAtlas->getImageDetailsPointer(pThemeSettings->images.textEditBG.colour.cornerTL);
		vMinimumDims = pID->vDims;
		pID = pAtlas->getImageDetailsPointer(pThemeSettings->images.textEditBG.colour.cornerBR);
		vMinimumDims += pID->vDims;
		pID = pAtlas->getImageDetailsPointer(pThemeSettings->images.textEditBG.colour.edgeT);
		vMinimumDims.x += pID->vDims.x;
		pID = pAtlas->getImageDetailsPointer(pThemeSettings->images.textEditBG.colour.edgeR);
		vMinimumDims.y += pID->vDims.y;
		return vMinimumDims;
	}

	void CUITextEdit::setPosition(float fX, float fY)
	{
		_mvPosition.x = fX;
		_mvPosition.y = fY;
		_mpContainer->computeScrollbars();
	}

	void CUITextEdit::setPosition(int iX, int iY)
	{
		setPosition(float(iX), float(iY));
	}

	void CUITextEdit::setPosition(const CVector2f& vPosition)
	{
		setPosition(vPosition.x, vPosition.y);
	}

	CVector2f CUITextEdit::getPosition(void) const
	{
		return _mvPosition;
	}

	void CUITextEdit::setVisible(bool bVisible)
	{
		_mbVisible = bVisible;
		if (!_mbVisible)
			reset();
		_mpContainer->computeScrollbars();
	}

	bool CUITextEdit::getVisible(void) const
	{
		return _mbVisible;
	}

	void CUITextEdit::renderBG(CResourceVertexBufferCPT2* pVB)
	{
		if (!_mbVisible)
			return;

		CUITheme::SSettings* pThemeSettings = _mpContainer->themeGetSettings();

		CColour col;
		if (_mState == state::active)
			col = pThemeSettings->colours.textEditBGActive;
		else
			col = pThemeSettings->colours.textEditBGInactive;

		// Add geometry for the 9 grid cells
		x->pUI->_helperAddWidgetBGGeometry(
			_mvPosition + _mpContainer->getWidgetOffset(),
			_mvDimensions,
			pThemeSettings->images.textEditBG,
			col,
			_mpContainer,
			pVB);
	}

	void CUITextEdit::renderNonBG(void)
	{
		if (!_mpContainer->getVisible())
			return;

		CUITheme* pTheme = _mpContainer->themeGet();
		const CUITheme::SSettings* pThemeSettings = pTheme->getSettings();
		
		CResourceTexture2DAtlas* pAtlas = pTheme->getTextureAtlas();
		CImageAtlasDetails* idTL = pAtlas->getImageDetailsPointer(pThemeSettings->images.textEditBG.colour.cornerTL);
		CImageAtlasDetails* idBR = pAtlas->getImageDetailsPointer(pThemeSettings->images.textEditBG.colour.cornerBR);

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

		// Compute offset of text, based upon whether it fits in the text edit box or not
		int iOffsetX = 0;
		std::string strTextTemp = _mstrText + "_";	// We use this to compute width to prevent moving forward and backwards of text when cursor flashes.
		int iTextWidth = (int)pFont->getTextWidth(strTextTemp, 1.0f);
		if (iTextWidth > int(_mvDimensions.x - idTL->vDims.x - idBR->vDims.x))
			iOffsetX = int(_mvDimensions.x - iTextWidth - idTL->vDims.x - idBR->vDims.x);

		// Reset the offset so that the beginning of the text is displayed, IF the text edit box is not active
		if (_mState == state::inactive)
			iOffsetX = 0;

		CVector2f vScissorDims;
		vScissorDims.x = _mvDimensions.x - idTL->vDims.x - idBR->vDims.x;
		vScissorDims.y = _mpContainer->getWidgetAreaDimensions().y;
		CVector2f vScissorPos;
		vScissorPos.x = _mpContainer->getWidgetAreaTLCornerPosition().x + _mvPosition.x + idTL->vDims.x;
		vScissorPos.y = (float)x->pWindow->getHeight();	// Top of screen
		vScissorPos.y -= _mpContainer->getWidgetAreaTLCornerPosition().y;
		vScissorPos.y -= _mpContainer->getWidgetAreaDimensions().y;
		//vScissorPos.y -= (_mpContainer->getWidgetAreaTLCornerPosition().y + _mvPosition.y + _mpContainer->getWidgetOffset().y + idTL->vDims.y);
		
		x->pRenderer->scissorTestEnable();
		x->pRenderer->scissorTest((int)vScissorPos.x, (int)vScissorPos.y, (int)vScissorDims.x, (int)vScissorDims.y);

		CColour col;
		if (_mState == state::active)
			col = pThemeSettings->colours.textEditTextActive;
		else
			col = pThemeSettings->colours.textEditTextInactive;

		pFont->print(strFinalText,
			int(iOffsetX + _mpContainer->getWidgetAreaTLCornerPosition().x + _mvPosition.x + idTL->vDims.x),
			int(_mpContainer->getWidgetAreaTLCornerPosition().y + _mvPosition.y + idTL->vDims.y + _mpContainer->getWidgetOffset().y),
			x->pWindow->getWidth(), x->pWindow->getHeight(), 1.0f, col);
		
		x->pRenderer->scissorTestDisable();

//		x->pRenderer->blendEnable();
//		x->pResource->getTexture2DFromFile(x->pResource->defaultRes.texture2DFromFile_default_white)->renderTo2DQuad(
//			(int)vScissorPos.x,
//			x->pWindow->getHeight() - (int)vScissorPos.y,
//			(int)vScissorDims.x, (int)vScissorDims.y, CColour(0.5f, 0.5f, 0.5f, 0.5f));
//		x->pRenderer->blendDisable();
	}

	void CUITextEdit::update(float fTimeDeltaSec)
	{
		if (!_mpContainer->getVisible())
			return;

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

//		CUITheme* pTheme = _mpContainer->themeGet();
//		CResourceTexture2DAtlas* pAtlas = pTheme->getTextureAtlas();
//		CImageAtlasDetails* idTL = pAtlas->getImageDetailsPointer(pThemeSettings->images.textEditBG.colour.cornerTL);

		CVector2f vWidgetTL = _mpContainer->getWidgetAreaTLCornerPosition() + _mpContainer->getWidgetOffset() + _mvPosition;
		CVector2f vWidgetBR = vWidgetTL + _mvDimensions;

		bool bMouseOver = false;
		if (bContainerHasMouseOver)
		{
			// Determine if mouse cursor is over
			if (vMousePos.x > vWidgetTL.x)
				if (vMousePos.x < vWidgetBR.x)
					if (vMousePos.y > vWidgetTL.y)
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
						x->pAudio->getEmitter(pThemeSettings->audio.textEditActivate.sampleName)->play(x->pSettings->getUIVolume() * pThemeSettings->audio.textEditActivate.volume, pThemeSettings->audio.textEditActivate.pitch);
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
		_mbTextHasChanged = false;
		if (state::active == _mState)
		{
			if (x->pInput->key.once(KC_RETURN) || x->pInput->key.once(KC_NUMPADENTER))
			{
				_mState = state::inactive;
				x->pAudio->getEmitter(pThemeSettings->audio.textEditReturn.sampleName)->play(x->pSettings->getUIVolume() * pThemeSettings->audio.textEditReturn.volume, pThemeSettings->audio.textEditReturn.pitch);
				// Check to see if we're only accepting numbers with _mbNumberInputOnly and if so, check the string and empty it if it isn't a number
				if (_mbIntegerInputOnly)
				{
					_checkIsNumber(true);
				}
				_mbWasActiveEnterPressed = true;	// So we can check this ourselves with getEnterPressed()

				// Call C function pointer if set
				if (_mfuncOnEnterPressed != NULL)
					_mfuncOnEnterPressed(_mstrText);

				if (_mstrTextPrev != _mstrText)
				{
					_mstrTextPrev = _mstrText;
					_mbTextHasChanged = true;
				}
			}
			else if (x->pInput->key.anyPressed())
			{
				if (x->pInput->key.repeat(KC_BACK))
				{
					if (_mstrText.length() > 0)
					{
						_mstrText.pop_back();
						x->pAudio->getEmitter(pThemeSettings->audio.textEditBackspace.sampleName)->play(x->pSettings->getUIVolume() * pThemeSettings->audio.textEditBackspace.volume, pThemeSettings->audio.textEditBackspace.pitch);
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
							x->pAudio->getEmitter(pThemeSettings->audio.textEditTextAdd.sampleName)->play(x->pSettings->getUIVolume() * pThemeSettings->audio.textEditTextAdd.volume, pThemeSettings->audio.textEditTextAdd.pitch);
						}
						else
						{
							x->pAudio->getEmitter(pThemeSettings->audio.textEditNoMoreCharSpace.sampleName)->play(x->pSettings->getUIVolume() * pThemeSettings->audio.textEditNoMoreCharSpace.volume, pThemeSettings->audio.textEditNoMoreCharSpace.pitch);
						}
					}
					if (x->pInput->key.repeat(KC_SPACE))
					{
						if (_mstrText.length() < _muiMaxChars)
						{
							_mstrText += " ";
							x->pAudio->getEmitter(pThemeSettings->audio.textEditTextAdd.sampleName)->play(x->pSettings->getUIVolume()* pThemeSettings->audio.textEditTextAdd.volume, pThemeSettings->audio.textEditTextAdd.pitch);
						}
						else
						{
							x->pAudio->getEmitter(pThemeSettings->audio.textEditNoMoreCharSpace.sampleName)->play(x->pSettings->getUIVolume()* pThemeSettings->audio.textEditNoMoreCharSpace.volume, pThemeSettings->audio.textEditNoMoreCharSpace.pitch);
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
		_mState = state::inactive;
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

	bool CUITextEdit::getHasTextChanged(void)
	{
		return _mbTextHasChanged;
	}
}