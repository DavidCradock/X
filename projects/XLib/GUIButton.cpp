#include "PCH.h"
#include "GUIButton.h"
#include "GUIManager.h"
#include "resourceManager.h"
#include "window.h"
#include "input.h"
#include "audioManager.h"

namespace X
{
	GUIButton::GUIButton()
	{
		_mfCurrentTextCol[0] = _mfCurrentTextCol[1] = _mfCurrentTextCol[2] = _mfCurrentTextCol[3] = 0.0f;
		_mState = state::up;
		_mStatePrevious = state::up;
		_mbClicked = false;
	}

	void GUIButton::render(void* pParentContainer, const std::string& strFramebufferToSampleFrom)
	{
		GUIContainer* pContainer = (GUIContainer*)pParentContainer;
		GUIManager* pGUIManager = GUIManager::getPointer();
		GUITheme* pTheme = pGUIManager->getTheme(pContainer->mstrThemename);
		renderBackground(pParentContainer, strFramebufferToSampleFrom, pTheme->mImages.buttonBGColour, pTheme->mImages.buttonBGNormal);

		// Get required resources needed to render
//		GUIManager* pGUI = GUIManager::getPointer();
		ResourceManager* pRM = ResourceManager::getPointer();
		Window* pWindow = Window::getPointer();
//		ResourceTriangle* pTri = pRM->getTriangle("X:gui");
//		ResourceShader* pShader = pRM->getShader("X:gui");
//		GUITheme* pTheme = pGUI->getTheme(pContainer->mstrThemename);
//		InputManager* pInput = InputManager::getPointer();



		// Now render the font stuff
		int iRTDims[2];
		iRTDims[0] = int(pWindow->getWidth());
		iRTDims[1] = int(pWindow->getHeight());
		ResourceFont* pFont = pRM->getFont(pTheme->mFonts.button);

		pFont->printCentered(mstrText,			// The text
			int(pContainer->mfPositionX + mfPositionX + (mfWidth / 2)),		// X position
			int(pContainer->mfPositionY + mfPositionY + (mfHeight / 2)),		// Y position
			iRTDims[0], iRTDims[1],	// Render target dims
			1.0f,	// Scaling
			glm::vec4(_mfCurrentTextCol[0], _mfCurrentTextCol[1], _mfCurrentTextCol[2], _mfCurrentTextCol[3]));	// Colour

	}

	void GUIButton::update(void* pParentContainer, bool bParentContainerAcceptingMouseClicks)
	{
		GUIManager* pGUIMan = GUIManager::getPointer();
		InputManager* pInput = InputManager::getPointer();
		GUIContainer* pContainer = (GUIContainer*)pParentContainer;
		glm::vec2 vMousePos = pInput->mouse.getCursorPos();
		_mTimer.update();
		float fSecondsPast = _mTimer.getSecondsPast();
		if (fSecondsPast > 0.1f)
			fSecondsPast = 0.1f;
		GUITheme* pTheme = pGUIMan->getTheme(pContainer->mstrThemename);	// Get parent container's theme
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
				AudioManager::getPointer()->getEmitter(pTheme->mAudio.buttonClicked.strSampleName)->play(pGUIMan->getAudioVol() * pTheme->mAudio.buttonClicked.fVolume, pTheme->mAudio.buttonClicked.fPitch);
			}
		}
		// Store current state to detect mouse clicks
		_mStatePrevious = _mState;
	}

	bool GUIButton::getClicked(void)
	{
		return _mbClicked;
	}
}