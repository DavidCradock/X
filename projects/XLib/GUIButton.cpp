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
		ResourceTexture2D* pTexColour = pRM->getTexture2D(pTheme->mImages.buttonBGColour);
		ResourceTexture2D* pTexNormal = pRM->getTexture2D(pTheme->mImages.buttonBGNormal);
		ResourceTexture2D* pTexReflection = pRM->getTexture2D(pTheme->mImages.reflection);
		ResourceFramebuffer* pFBSample = pRM->getFramebuffer(strFramebufferToSampleFrom);

		// Bind textures
		pTexColour->bind(0);
		pTexNormal->bind(1);
		pTexReflection->bind(2);
		pFBSample->bindAsTexture(3);

		// Render the button centre
		glm::vec2 vTexDimsDiv3 = pTexColour->mvDimensions * 0.3333333f;
		pTri->removeGeom();
		pTri->addQuad2D(
		glm::vec2(
			pContainer->mfPositionX + mfPositionX + vTexDimsDiv3.x,		// Position X
			pContainer->mfPositionY + mfPositionY + vTexDimsDiv3.y),	// Position Y
			glm::vec2(mfWidth, mfHeight),								// Dimensions
			glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),							// Vertex colour
			mTC.centre.BL,
			mTC.centre.BR,
			mTC.centre.TR,
			mTC.centre.TL);

		// Render the left edge
		pTri->addQuad2D(
			glm::vec2(
				pContainer->mfPositionX + mfPositionX,						// Position X
				pContainer->mfPositionY + mfPositionY + vTexDimsDiv3.y),	// Position Y
			glm::vec2(vTexDimsDiv3.x, mfHeight),							// Dimensions
			glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),								// Vertex colour
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
				pContainer->mfPositionX + mfPositionX + vTexDimsDiv3.x,				// Position X
				pContainer->mfPositionY + mfPositionY + mfHeight + vTexDimsDiv3.y),	// Position Y
			glm::vec2(mfWidth, vTexDimsDiv3.y),										// Dimensions
			glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),										// Vertex colour
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
		ResourceFont* pFont = pRM->getFont(pTheme->mFonts.button);

		pFont->printCentered(mstrText,			// The text
			int(pContainer->mfPositionX + mfPositionX + (mfWidth / 2) + vTexDimsDiv3.x),		// X position
			int(pContainer->mfPositionY + mfPositionY + (mfHeight / 2) + vTexDimsDiv3.y),		// Y position
			iRTDims[0], iRTDims[1],	// Render target dims
			1.0f,	// Scaling
			glm::vec4(_mfCurrentTextCol[0], _mfCurrentTextCol[1], _mfCurrentTextCol[2], _mfCurrentTextCol[3]));	// Colour

	}

	void GUIButton::update(void* pParentContainer, bool bParentContainerAcceptingMouseClicks)
	{
		GUIManager* pGUIMan = GUIManager::getPointer();
		InputManager* pInput = InputManager::getPointer();
		ResourceManager* pResMan = ResourceManager::getPointer();
		GUIContainer* pContainer = (GUIContainer*)pParentContainer;
		glm::vec2 vMousePos = pInput->mouse.getCursorPos();
		_mTimer.update();
		float fSecondsPast = _mTimer.getSecondsPast();
		if (fSecondsPast > 0.1f)
			fSecondsPast = 0.1f;
		GUITheme* pTheme = pGUIMan->getTheme(pContainer->mstrThemename);	// Get parent container's theme
		ResourceTexture2D* pColourTex = pResMan->getTexture2D(pTheme->mImages.buttonBGColour);
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