#include "PCH.h"
#include "GUIButton.h"
#include "GUIManager.h"
#include "GUITooltip.h"
#include "singletons.h"

namespace X
{
	CGUIButton::CGUIButton()
	{
		_mfCurrentTextCol[0] = _mfCurrentTextCol[1] = _mfCurrentTextCol[2] = _mfCurrentTextCol[3] = 0.0f;
		_mfCurrentBGCol[0] = _mfCurrentBGCol[1] = _mfCurrentBGCol[2] = _mfCurrentBGCol[3] = 0.0f;
		_mState = state::up;
		_mStatePrevious = state::up;
		_mbClicked = false;
		mpTooltip = new CGUITooltip;
	}

	CGUIButton::~CGUIButton()
	{
		delete mpTooltip;
	}

	void CGUIButton::render(void* pParentContainer, const std::string& strFramebufferToSampleFrom)
	{
		CGUIContainer* pContainer = (CGUIContainer*)pParentContainer;
		CGUITheme* pTheme = pContainer->getTheme();
		CColour col(_mfCurrentBGCol[0], _mfCurrentBGCol[1], _mfCurrentBGCol[2], _mfCurrentBGCol[3]);
		renderBackground(pParentContainer, strFramebufferToSampleFrom, pTheme->mImages.buttonBGColour, pTheme->mImages.buttonBGNormal, col);

		// Now render the font stuff
		int iRTDims[2];
		iRTDims[0] = int(x->pWindow->getWidth());
		iRTDims[1] = int(x->pWindow->getHeight());
		CResourceFont* pFont = x->pResource->getFont(pTheme->mFonts.button);

		pFont->printCentered(mstrText,										// The text
			int(pContainer->mfPositionX + mfPositionX + (mfWidth / 2)),		// X position
			int(pContainer->mfPositionY + mfPositionY + (mfHeight / 2)),	// Y position
			iRTDims[0], iRTDims[1],											// Render target dims
			1.0f,															// Scaling
			CColour(_mfCurrentTextCol[0], _mfCurrentTextCol[1], _mfCurrentTextCol[2], _mfCurrentTextCol[3]));	// Colour
	}

	void CGUIButton::update(void* pParentContainer, bool bParentContainerAcceptingMouseClicks)
	{
		CGUIContainer* pContainer = (CGUIContainer*)pParentContainer;
		CVector2f vMousePos = x->pInput->mouse.getCursorPos();
		_mTimer.update();
		float fSecondsPast = _mTimer.getSecondsPast();
		if (fSecondsPast > 0.1f)
			fSecondsPast = 0.1f;
		CGUITheme* pTheme = pContainer->getTheme();
		bool bMouseOver = false;
		if (bParentContainerAcceptingMouseClicks)
		{
			// Determine if mouse cursor is over
			if (vMousePos.x > pContainer->mfPositionX + mfPositionX)
				if (vMousePos.x < pContainer->mfPositionX + mfPositionX + mfWidth)
					if (vMousePos.y > pContainer->mfPositionY + mfPositionY)
						if (vMousePos.y < pContainer->mfPositionY + mfPositionY + mfHeight)
							bMouseOver = true;
			// Determine current state of the button (up/over/down)
			_mState = state::up;
			if (bMouseOver)
			{
				_mState = state::over;
				if (x->pInput->mouse.leftButDown())
				{
					_mState = state::down;
				}
			}
		}
		else  // Not accepting mouse clicks
		{
			_mState = state::up;
		}

		// Depending upon state, compute text and BG colours
		float fColTxt[4];
		float fColBG[4];
		if (state::up == _mState)
		{
			fColTxt[0] = pTheme->mColours.buttonTextUp.red;
			fColTxt[1] = pTheme->mColours.buttonTextUp.green;
			fColTxt[2] = pTheme->mColours.buttonTextUp.blue;
			fColTxt[3] = pTheme->mColours.buttonTextUp.alpha;
			fColBG[0] = pTheme->mColours.buttonBGUp.red;
			fColBG[1] = pTheme->mColours.buttonBGUp.green;
			fColBG[2] = pTheme->mColours.buttonBGUp.blue;
			fColBG[3] = pTheme->mColours.buttonBGUp.alpha;

			for (int iCol = 0; iCol < 4; ++iCol)
			{
				// Text
				if (_mfCurrentTextCol[iCol] < fColTxt[iCol])
				{
					_mfCurrentTextCol[iCol] += fSecondsPast * pTheme->mfButtonFadeSpeed;
					if (_mfCurrentTextCol[iCol] > fColTxt[iCol])
						_mfCurrentTextCol[iCol] = fColTxt[iCol];
				}
				else if (_mfCurrentTextCol[iCol] > fColTxt[iCol])
				{
					_mfCurrentTextCol[iCol] -= fSecondsPast * pTheme->mfButtonFadeSpeed;
					if (_mfCurrentTextCol[iCol] < fColTxt[iCol])
						_mfCurrentTextCol[iCol] = fColTxt[iCol];
				}
				// BG
				if (_mfCurrentBGCol[iCol] < fColBG[iCol])
				{
					_mfCurrentBGCol[iCol] += fSecondsPast * pTheme->mfButtonFadeSpeed;
					if (_mfCurrentBGCol[iCol] > fColBG[iCol])
						_mfCurrentBGCol[iCol] = fColBG[iCol];
				}
				else if (_mfCurrentBGCol[iCol] > fColBG[iCol])
				{
					_mfCurrentBGCol[iCol] -= fSecondsPast * pTheme->mfButtonFadeSpeed;
					if (_mfCurrentBGCol[iCol] < fColBG[iCol])
						_mfCurrentBGCol[iCol] = fColBG[iCol];
				}
			}
		}
		else if (state::over == _mState)
		{
			fColTxt[0] = pTheme->mColours.buttonTextOver.red;
			fColTxt[1] = pTheme->mColours.buttonTextOver.green;
			fColTxt[2] = pTheme->mColours.buttonTextOver.blue;
			fColTxt[3] = pTheme->mColours.buttonTextOver.alpha;
			fColBG[0] = pTheme->mColours.buttonBGOver.red;
			fColBG[1] = pTheme->mColours.buttonBGOver.green;
			fColBG[2] = pTheme->mColours.buttonBGOver.blue;
			fColBG[3] = pTheme->mColours.buttonBGOver.alpha;

			// Text
			for (int iCol = 0; iCol < 4; ++iCol)
			{
				if (_mfCurrentTextCol[iCol] < fColTxt[iCol])
				{
					_mfCurrentTextCol[iCol] += fSecondsPast * pTheme->mfButtonFadeSpeed;
					if (_mfCurrentTextCol[iCol] > fColTxt[iCol])
						_mfCurrentTextCol[iCol] = fColTxt[iCol];
				}
				else if (_mfCurrentTextCol[iCol] > fColTxt[iCol])
				{
					_mfCurrentTextCol[iCol] -= fSecondsPast * pTheme->mfButtonFadeSpeed;
					if (_mfCurrentTextCol[iCol] < fColTxt[iCol])
						_mfCurrentTextCol[iCol] = fColTxt[iCol];
				}
			}
			// BG
			for (int iCol = 0; iCol < 4; ++iCol)
			{
				if (_mfCurrentBGCol[iCol] < fColBG[iCol])
				{
					_mfCurrentBGCol[iCol] += fSecondsPast * pTheme->mfButtonFadeSpeed;
					if (_mfCurrentBGCol[iCol] > fColBG[iCol])
						_mfCurrentBGCol[iCol] = fColBG[iCol];
				}
				else if (_mfCurrentBGCol[iCol] > fColBG[iCol])
				{
					_mfCurrentBGCol[iCol] -= fSecondsPast * pTheme->mfButtonFadeSpeed;
					if (_mfCurrentBGCol[iCol] < fColBG[iCol])
						_mfCurrentBGCol[iCol] = fColBG[iCol];
				}
			}
		}
		else if (state::down == _mState)
		{
			fColTxt[0] = pTheme->mColours.buttonTextDown.red;
			fColTxt[1] = pTheme->mColours.buttonTextDown.green;
			fColTxt[2] = pTheme->mColours.buttonTextDown.blue;
			fColTxt[3] = pTheme->mColours.buttonTextDown.alpha;
			fColBG[0] = pTheme->mColours.buttonBGDown.red;
			fColBG[1] = pTheme->mColours.buttonBGDown.green;
			fColBG[2] = pTheme->mColours.buttonBGDown.blue;
			fColBG[3] = pTheme->mColours.buttonBGDown.alpha;

			// Text
			for (int iCol = 0; iCol < 4; ++iCol)
			{
				if (_mfCurrentTextCol[iCol] < fColTxt[iCol])
				{
					_mfCurrentTextCol[iCol] += fSecondsPast * pTheme->mfButtonFadeSpeed;
					if (_mfCurrentTextCol[iCol] > fColTxt[iCol])
						_mfCurrentTextCol[iCol] = fColTxt[iCol];
				}
				else if (_mfCurrentTextCol[iCol] > fColTxt[iCol])
				{
					_mfCurrentTextCol[iCol] -= fSecondsPast * pTheme->mfButtonFadeSpeed;
					if (_mfCurrentTextCol[iCol] < fColTxt[iCol])
						_mfCurrentTextCol[iCol] = fColTxt[iCol];
				}
			}
			// BG
			for (int iCol = 0; iCol < 4; ++iCol)
			{
				if (_mfCurrentBGCol[iCol] < fColBG[iCol])
				{
					_mfCurrentBGCol[iCol] += fSecondsPast * pTheme->mfButtonFadeSpeed;
					if (_mfCurrentBGCol[iCol] > fColBG[iCol])
						_mfCurrentBGCol[iCol] = fColBG[iCol];
				}
				else if (_mfCurrentBGCol[iCol] > fColBG[iCol])
				{
					_mfCurrentBGCol[iCol] -= fSecondsPast * pTheme->mfButtonFadeSpeed;
					if (_mfCurrentBGCol[iCol] < fColBG[iCol])
						_mfCurrentBGCol[iCol] = fColBG[iCol];
				}
			}
		}

		_mbClicked = false;
		if (state::down == _mStatePrevious)
		{
			if (!x->pInput->mouse.leftButDown())
			{
				_mbClicked = true;
				x->pAudio->getEmitter(pTheme->mAudio.buttonClicked.strSampleName)->play(x->pGUI->getAudioVol() * pTheme->mAudio.buttonClicked.fVolume, pTheme->mAudio.buttonClicked.fPitch);
			}
		}
		// Store current state to detect mouse clicks
		_mStatePrevious = _mState;

		// Update this object's tooltip
		bool bMouseOverForTT = false;
		if (state::over == _mState || state::down == _mState)
			bMouseOverForTT = true;
		mpTooltip->update(pParentContainer, (CGUIBaseObject*)this, bMouseOver);
	}

	bool CGUIButton::getClicked(void) const
	{
		return _mbClicked;
	}
}