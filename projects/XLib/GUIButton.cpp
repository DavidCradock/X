#include "PCH.h"
#include "GUIButton.h"
#include "GUIManager.h"
#include "resourceManager.h"
#include "window.h"
#include "input.h"
#include "audioManager.h"
#include "GUITooltip.h"

namespace X
{
	CGUIButton::CGUIButton()
	{
		_mfCurrentTextCol[0] = _mfCurrentTextCol[1] = _mfCurrentTextCol[2] = _mfCurrentTextCol[3] = 0.0f;
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
		SCGUIManager* pGUIManager = SCGUIManager::getPointer();
		CGUITheme* pTheme = pContainer->getTheme();
		CColour col;
		renderBackground(pParentContainer, strFramebufferToSampleFrom, pTheme->mImages.buttonBGColour, pTheme->mImages.buttonBGNormal, col);

		// Get required resources needed to render
		SCResourceManager* pRM = SCResourceManager::getPointer();
		SCWindow* pWindow = SCWindow::getPointer();

		// Now render the font stuff
		int iRTDims[2];
		iRTDims[0] = int(pWindow->getWidth());
		iRTDims[1] = int(pWindow->getHeight());
		CResourceFont* pFont = pRM->getFont(pTheme->mFonts.button);

		pFont->printCentered(mstrText,										// The text
			int(pContainer->mfPositionX + mfPositionX + (mfWidth / 2)),		// X position
			int(pContainer->mfPositionY + mfPositionY + (mfHeight / 2)),	// Y position
			iRTDims[0], iRTDims[1],											// Render target dims
			1.0f,															// Scaling
			CColour(_mfCurrentTextCol[0], _mfCurrentTextCol[1], _mfCurrentTextCol[2], _mfCurrentTextCol[3]));	// Colour
	}

	void CGUIButton::update(void* pParentContainer, bool bParentContainerAcceptingMouseClicks)
	{
		SCGUIManager* pGUIMan = SCGUIManager::getPointer();
		SCInputManager* pInput = SCInputManager::getPointer();
		CGUIContainer* pContainer = (CGUIContainer*)pParentContainer;
		CVector2f vMousePos = pInput->mouse.getCursorPos();
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
				if (pInput->mouse.leftButDown())
				{
					_mState = state::down;
				}
			}
		}
		else  // Not accepting mouse clicks
		{
			_mState = state::up;
		}

		// Depending upon state, compute text colour
		float fCol[4];
		if (state::up == _mState)
		{
			fCol[0] = pTheme->mColours.buttonTextUp.red;
			fCol[1] = pTheme->mColours.buttonTextUp.green;
			fCol[2] = pTheme->mColours.buttonTextUp.blue;
			fCol[3] = pTheme->mColours.buttonTextUp.alpha;

			for (int iCol = 0; iCol < 4; ++iCol)
			{
				if (_mfCurrentTextCol[iCol] < fCol[iCol])
				{
					_mfCurrentTextCol[iCol] += fSecondsPast * pTheme->mfButtonTextFadeSpeed;
					if (_mfCurrentTextCol[iCol] > fCol[iCol])
						_mfCurrentTextCol[iCol] = fCol[iCol];
				}
				else if (_mfCurrentTextCol[iCol] > fCol[iCol])
				{
					_mfCurrentTextCol[iCol] -= fSecondsPast * pTheme->mfButtonTextFadeSpeed;
					if (_mfCurrentTextCol[iCol] < fCol[iCol])
						_mfCurrentTextCol[iCol] = fCol[iCol];
				}
			}
		}
		else if (state::over == _mState)
		{
			fCol[0] = pTheme->mColours.buttonTextOver.red;
			fCol[1] = pTheme->mColours.buttonTextOver.green;
			fCol[2] = pTheme->mColours.buttonTextOver.blue;
			fCol[3] = pTheme->mColours.buttonTextOver.alpha;

			for (int iCol = 0; iCol < 4; ++iCol)
			{
				if (_mfCurrentTextCol[iCol] < fCol[iCol])
				{
					_mfCurrentTextCol[iCol] += fSecondsPast * pTheme->mfButtonTextFadeSpeed;
					if (_mfCurrentTextCol[iCol] > fCol[iCol])
						_mfCurrentTextCol[iCol] = fCol[iCol];
				}
				else if (_mfCurrentTextCol[iCol] > fCol[iCol])
				{
					_mfCurrentTextCol[iCol] -= fSecondsPast * pTheme->mfButtonTextFadeSpeed;
					if (_mfCurrentTextCol[iCol] < fCol[iCol])
						_mfCurrentTextCol[iCol] = fCol[iCol];
				}
			}
		}
		else if (state::down == _mState)
		{
			fCol[0] = pTheme->mColours.buttonTextDown.red;
			fCol[1] = pTheme->mColours.buttonTextDown.green;
			fCol[2] = pTheme->mColours.buttonTextDown.blue;
			fCol[3] = pTheme->mColours.buttonTextDown.alpha;

			for (int iCol = 0; iCol < 4; ++iCol)
			{
				if (_mfCurrentTextCol[iCol] < fCol[iCol])
				{
					_mfCurrentTextCol[iCol] += fSecondsPast * pTheme->mfButtonTextFadeSpeed;
					if (_mfCurrentTextCol[iCol] > fCol[iCol])
						_mfCurrentTextCol[iCol] = fCol[iCol];
				}
				else if (_mfCurrentTextCol[iCol] > fCol[iCol])
				{
					_mfCurrentTextCol[iCol] -= fSecondsPast * pTheme->mfButtonTextFadeSpeed;
					if (_mfCurrentTextCol[iCol] < fCol[iCol])
						_mfCurrentTextCol[iCol] = fCol[iCol];
				}
			}
		}

		_mbClicked = false;
		if (state::down == _mStatePrevious)
		{
			if (!pInput->mouse.leftButDown())
			{
				_mbClicked = true;
				SCAudioManager::getPointer()->getEmitter(pTheme->mAudio.buttonClicked.strSampleName)->play(pGUIMan->getAudioVol() * pTheme->mAudio.buttonClicked.fVolume, pTheme->mAudio.buttonClicked.fPitch);
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