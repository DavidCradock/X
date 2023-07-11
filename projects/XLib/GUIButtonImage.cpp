#include "PCH.h"
#include "GUIButtonImage.h"
#include "GUIManager.h"
#include "resourceManager.h"
#include "window.h"
#include "input.h"
#include "audioManager.h"
#include "GUITooltip.h"

namespace X
{
	CGUIButtonImage::CGUIButtonImage()
	{
		_mfCurrentImageCol[0] = _mfCurrentImageCol[1] = _mfCurrentImageCol[2] = _mfCurrentImageCol[3] = 0.0f;
		_mState = state::up;
		_mStatePrevious = state::up;
		_mbClicked = false;
		mpTooltip = new CGUITooltip;
	}

	CGUIButtonImage::~CGUIButtonImage()
	{
		delete mpTooltip;
	}

	void CGUIButtonImage::render(void* pParentContainer, const std::string& strFramebufferToSampleFrom)
	{
		CGUIContainer* pContainer = (CGUIContainer*)pParentContainer;
		SCGUIManager* pGUIManager = SCGUIManager::getPointer();
		CGUITheme* pTheme = pGUIManager->getTheme(pContainer->mstrThemename);
		CColour col;
		renderBackground(pParentContainer, strFramebufferToSampleFrom, pTheme->mImages.buttonImageBGColour, pTheme->mImages.buttonImageBGNormal, col);

		// Get required resources needed to render
		SCGUIManager* pGUI = SCGUIManager::getPointer();
		SCResourceManager* pRM = SCResourceManager::getPointer();
		SCWindow* pWindow = SCWindow::getPointer();
		CResourceTriangle* pTri = pRM->getTriangle("X:gui");
		CResourceShader* pShader = pRM->getShader("X:pos_col_tex");
		//SCInputManager* pInput = SCInputManager::getPointer();

		pShader->bind();

		// Setup the projection matrix as orthographic
		glm::mat4 matProjection = glm::ortho(0.0f, float(pWindow->getWidth()), float(pWindow->getHeight()), 0.0f, -1.0f, 1.0f);
		pShader->setMat4("transform", matProjection);

		// Tell OpenGL, for each sampler, to which texture unit it belongs to
		pShader->setInt("texture0", 0);

		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);

		// Get textures
		CResourceTexture2DFromFile* pTexColour = pRM->getTexture2DFromFile(_mstrTextureUp);
		if (state::over == _mState)
			pTexColour = pRM->getTexture2DFromFile(_mstrTextureOver);
		else if (state::down == _mState)
			pTexColour = pRM->getTexture2DFromFile(_mstrTextureDown);

		glm::vec2 vTexDimsPoint3 = pTexColour->mvDimensions * 0.3333333f;
		glm::vec2 vTexDimsPoint6 = pTexColour->mvDimensions * 0.6666666f;

		// Bind textures
		pTexColour->bind(0);

		// Render the image
		pTri->removeGeom();
		pTri->addQuad2D(
			glm::vec2(pContainer->mfPositionX + mfPositionX, pContainer->mfPositionY + mfPositionY),				// Position
			glm::vec2(mfWidth, mfHeight),																			// Dimensions
			glm::vec4(_mfCurrentImageCol[0], _mfCurrentImageCol[1], _mfCurrentImageCol[2], _mfCurrentImageCol[3]),	// Vertex colour
			glm::vec2(0.0f, 1.0f),
			glm::vec2(1.0f, 1.0f),
			glm::vec2(1.0f, 0.0f),
			glm::vec2(0.0f, 0.0f));

		pTri->update();
		pTri->draw();

		pTexColour->unbindAll();	// Unbind textures
		pShader->unbind();	// Unbind the GUI shader
		glDisable(GL_BLEND);
	}

	void CGUIButtonImage::update(void* pParentContainer, bool bParentContainerAcceptingMouseClicks)
	{
		SCGUIManager* pGUIMan = SCGUIManager::getPointer();
		SCInputManager* pInput = SCInputManager::getPointer();
		CGUIContainer* pContainer = (CGUIContainer*)pParentContainer;
		glm::vec2 vMousePos = pInput->mouse.getCursorPos();
		_mTimer.update();
		float fSecondsPast = _mTimer.getSecondsPast();
		if (fSecondsPast > 0.1f)
			fSecondsPast = 0.1f;
		CGUITheme* pTheme = pGUIMan->getTheme(pContainer->mstrThemename);	// Get parent container's theme
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
			fCol[0] = pTheme->mColours.buttonImageUp.red;
			fCol[1] = pTheme->mColours.buttonImageUp.green;
			fCol[2] = pTheme->mColours.buttonImageUp.blue;
			fCol[3] = pTheme->mColours.buttonImageUp.alpha;

			for (int iCol = 0; iCol < 4; ++iCol)
			{
				if (_mfCurrentImageCol[iCol] < fCol[iCol])
				{
					_mfCurrentImageCol[iCol] += fSecondsPast * pTheme->mfButtonImageTextFadeSpeed;
					if (_mfCurrentImageCol[iCol] > fCol[iCol])
						_mfCurrentImageCol[iCol] = fCol[iCol];
				}
				else if (_mfCurrentImageCol[iCol] > fCol[iCol])
				{
					_mfCurrentImageCol[iCol] -= fSecondsPast * pTheme->mfButtonImageTextFadeSpeed;
					if (_mfCurrentImageCol[iCol] < fCol[iCol])
						_mfCurrentImageCol[iCol] = fCol[iCol];
				}
			}
		}
		else if (state::over == _mState)
		{
			fCol[0] = pTheme->mColours.buttonImageOver.red;
			fCol[1] = pTheme->mColours.buttonImageOver.green;
			fCol[2] = pTheme->mColours.buttonImageOver.blue;
			fCol[3] = pTheme->mColours.buttonImageOver.alpha;

			for (int iCol = 0; iCol < 4; ++iCol)
			{
				if (_mfCurrentImageCol[iCol] < fCol[iCol])
				{
					_mfCurrentImageCol[iCol] += fSecondsPast * pTheme->mfButtonImageTextFadeSpeed;
					if (_mfCurrentImageCol[iCol] > fCol[iCol])
						_mfCurrentImageCol[iCol] = fCol[iCol];
				}
				else if (_mfCurrentImageCol[iCol] > fCol[iCol])
				{
					_mfCurrentImageCol[iCol] -= fSecondsPast * pTheme->mfButtonImageTextFadeSpeed;
					if (_mfCurrentImageCol[iCol] < fCol[iCol])
						_mfCurrentImageCol[iCol] = fCol[iCol];
				}
			}
		}
		else if (state::down == _mState)
		{
			fCol[0] = pTheme->mColours.buttonImageDown.red;
			fCol[1] = pTheme->mColours.buttonImageDown.green;
			fCol[2] = pTheme->mColours.buttonImageDown.blue;
			fCol[3] = pTheme->mColours.buttonImageDown.alpha;

			for (int iCol = 0; iCol < 4; ++iCol)
			{
				if (_mfCurrentImageCol[iCol] < fCol[iCol])
				{
					_mfCurrentImageCol[iCol] += fSecondsPast * pTheme->mfButtonImageTextFadeSpeed;
					if (_mfCurrentImageCol[iCol] > fCol[iCol])
						_mfCurrentImageCol[iCol] = fCol[iCol];
				}
				else if (_mfCurrentImageCol[iCol] > fCol[iCol])
				{
					_mfCurrentImageCol[iCol] -= fSecondsPast * pTheme->mfButtonImageTextFadeSpeed;
					if (_mfCurrentImageCol[iCol] < fCol[iCol])
						_mfCurrentImageCol[iCol] = fCol[iCol];
				}
			}
		}

		_mbClicked = false;
		if (state::down == _mStatePrevious)
		{
			if (!pInput->mouse.leftButDown())
			{
				_mbClicked = true;
				SCAudioManager::getPointer()->getEmitter(pTheme->mAudio.buttonImageClicked.strSampleName)->play(pGUIMan->getAudioVol() * pTheme->mAudio.buttonImageClicked.fVolume, pTheme->mAudio.buttonImageClicked.fPitch);
			}
		}
		// Store current state to detect mouse clicks
		_mStatePrevious = _mState;

		// Update this object's tooltip
		bool bMouseOverForTT = false;
		if (state::over == _mState || state::down == _mState)
			bMouseOverForTT = true;
		CGUITooltip* pTooltip = (CGUITooltip*)mpTooltip;
		pTooltip->update(pParentContainer, (CGUIBaseObject*)this, bMouseOver);

	}

	bool CGUIButtonImage::getClicked(void) const
	{
		return _mbClicked;
	}
}