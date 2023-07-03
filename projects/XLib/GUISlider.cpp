#include "PCH.h"
#include "GUISlider.h"
#include "GUIManager.h"
#include "resourceManager.h"
#include "window.h"
#include "input.h"
#include "audioManager.h"
#include "utilities.h"
#include "GUITooltip.h"

namespace X
{
	GUISlider::GUISlider()
	{
		_mfSliderPosition = 0.5f;
		_mTabColour.set(1.0f, 1.0f, 1.0f, 1.0f);
		_mbTabBeingMoved = false;
		_mfTabRatio = 0.5f;
		mpTooltip = new GUITooltip;
	}

	GUISlider::~GUISlider()
	{
		delete mpTooltip;
	}

	void GUISlider::render(void* pParentContainer, const std::string& strFramebufferToSampleFrom)
	{
		GUIContainer* pContainer = (GUIContainer*)pParentContainer;
		GUIManager* pGUIManager = GUIManager::getPointer();
		GUITheme* pTheme = pGUIManager->getTheme(pContainer->mstrThemename);
		renderBackground(pParentContainer, strFramebufferToSampleFrom, pTheme->mImages.sliderBGColour, pTheme->mImages.sliderBGNormal);

		// Get required resources needed to render the tab
		ResourceManager* pRM = ResourceManager::getPointer();
		Window* pWindow = Window::getPointer();
		ResourceTriangle* pTri = pRM->getTriangle("X:gui");
		ResourceShader* pShader = pRM->getShader("X:gui");
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
		ResourceTexture2D* pTexColour = pRM->getTexture2D(pTheme->mImages.sliderTabColour);
		ResourceTexture2D* pTexNormal = pRM->getTexture2D(pTheme->mImages.sliderTabNormal);
		ResourceTexture2D* pTexReflection = pRM->getTexture2D(pTheme->mImages.reflection);
		ResourceFramebuffer* pFBSample = pRM->getFramebuffer(strFramebufferToSampleFrom);

		// Bind textures
		pTexColour->bind(0);
		pTexNormal->bind(1);
		pTexReflection->bind(2);
		pFBSample->bindAsTexture(3);


		glm::vec2 vTexDimsPoint3 = pTexColour->mvDimensions * 0.3333333f;
		glm::vec2 vTexDimsPoint6 = pTexColour->mvDimensions * 0.6666666f;

		if (_mbOrientationIsHorizontal)
		{
			// Render the slider tab centre
			pTri->removeGeom();
			pTri->addQuad2D(
				glm::vec2(
					_mfTabPos[0],							// Position X
					_mfTabPos[1]),							// Position Y
				glm::vec2(_mfTabDims[0], _mfTabDims[1]),	// Dimensions
				_mTabColour.get(),							// Vertex colour
				mTC.centre.BL,
				mTC.centre.BR,
				mTC.centre.TR,
				mTC.centre.TL);

			// Render the left edge
			pTri->addQuad2D(
				glm::vec2(
					_mfTabPos[0] - vTexDimsPoint3.x,						// Position X
					_mfTabPos[1]),											// Position Y
				glm::vec2(vTexDimsPoint3.x, mfHeight - vTexDimsPoint6.y),	// Dimensions
				_mTabColour.get(),											// Vertex colour
				mTC.left.BL,
				mTC.left.BR,
				mTC.left.TR,
				mTC.left.TL);

			// Render the right edge
			pTri->addQuad2D(
				glm::vec2(
					_mfTabPos[0] + _mfTabDims[0],							// Position X
					_mfTabPos[1]),											// Position Y
				glm::vec2(vTexDimsPoint3.x, mfHeight - vTexDimsPoint6.y),	// Dimensions
				_mTabColour.get(),											// Vertex colour
				mTC.right.BL,
				mTC.right.BR,
				mTC.right.TR,
				mTC.right.TL);

			// Render the top edge
			pTri->addQuad2D(
				glm::vec2(
					_mfTabPos[0],							// Position X
					_mfTabPos[1] - vTexDimsPoint3.y),		// Position Y
				glm::vec2(_mfTabDims[0], vTexDimsPoint3.y),	// Dimensions
				_mTabColour.get(),							// Vertex colour
				mTC.top.BL,
				mTC.top.BR,
				mTC.top.TR,
				mTC.top.TL);

			// Render the bottom edge
			pTri->addQuad2D(
				glm::vec2(
					_mfTabPos[0],							// Position X
					_mfTabPos[1] + _mfTabDims[1]),			// Position Y
				glm::vec2(_mfTabDims[0], vTexDimsPoint3.y),	// Dimensions
				_mTabColour.get(),							// Vertex colour
				mTC.bottom.BL,
				mTC.bottom.BR,
				mTC.bottom.TR,
				mTC.bottom.TL);

			// Render the top left corner
			pTri->addQuad2D(
				glm::vec2(
					_mfTabPos[0] - vTexDimsPoint3.x,			// Position X
					_mfTabPos[1] - vTexDimsPoint3.y),			// Position Y
				glm::vec2(vTexDimsPoint3.x, vTexDimsPoint3.y),	// Dimensions
				_mTabColour.get(),								// Vertex colour
				mTC.topLeft.BL,
				mTC.topLeft.BR,
				mTC.topLeft.TR,
				mTC.topLeft.TL);

			// Render the top right corner
			pTri->addQuad2D(
				glm::vec2(
					_mfTabPos[0] + _mfTabDims[0],				// Position X
					_mfTabPos[1] - vTexDimsPoint3.y),			// Position Y
				glm::vec2(vTexDimsPoint3.x, vTexDimsPoint3.y),	// Dimensions
				_mTabColour.get(),								// Vertex colour
				mTC.topRight.BL,
				mTC.topRight.BR,
				mTC.topRight.TR,
				mTC.topRight.TL);

			// Render the bottom left corner
			pTri->addQuad2D(
				glm::vec2(
					_mfTabPos[0] - vTexDimsPoint3.x,			// Position X
					_mfTabPos[1] + _mfTabDims[1]),				// Position Y
				glm::vec2(vTexDimsPoint3.x, vTexDimsPoint3.y),	// Dimensions
				_mTabColour.get(),								// Vertex colour
				mTC.bottomLeft.BL,
				mTC.bottomLeft.BR,
				mTC.bottomLeft.TR,
				mTC.bottomLeft.TL);

			// Render the bottom right corner
			pTri->addQuad2D(
				glm::vec2(
					_mfTabPos[0] + _mfTabDims[0],				// Position X
					_mfTabPos[1] + _mfTabDims[1]),				// Position Y
				glm::vec2(vTexDimsPoint3.x, vTexDimsPoint3.y),	// Dimensions
				_mTabColour.get(),								// Vertex colour
				mTC.bottomRight.BL,
				mTC.bottomRight.BR,
				mTC.bottomRight.TR,
				mTC.bottomRight.TL);

		}
		else  // Vertical
		{
			// Render the slider tab centre
			pTri->removeGeom();
			pTri->addQuad2D(
				glm::vec2(
					_mfTabPos[0],							// Position X
					_mfTabPos[1]),							// Position Y
				glm::vec2(_mfTabDims[0], _mfTabDims[1]),	// Dimensions
				_mTabColour.get(),							// Vertex colour
				mTC.centre.BL,
				mTC.centre.BR,
				mTC.centre.TR,
				mTC.centre.TL);

			// Render the left edge
			pTri->addQuad2D(
				glm::vec2(
					_mfTabPos[0] - vTexDimsPoint3.x,		// Position X
					_mfTabPos[1]),							// Position Y
				glm::vec2(vTexDimsPoint3.x, _mfTabDims[1]),	// Dimensions
				_mTabColour.get(),							// Vertex colour
				mTC.left.BL,
				mTC.left.BR,
				mTC.left.TR,
				mTC.left.TL);

			// Render the right edge
			pTri->addQuad2D(
				glm::vec2(
					_mfTabPos[0] + _mfTabDims[0],			// Position X
					_mfTabPos[1]),							// Position Y
				glm::vec2(vTexDimsPoint3.x, _mfTabDims[1]),	// Dimensions
				_mTabColour.get(),							// Vertex colour
				mTC.right.BL,
				mTC.right.BR,
				mTC.right.TR,
				mTC.right.TL);

			// Render the top edge
			pTri->addQuad2D(
				glm::vec2(
					_mfTabPos[0],							// Position X
					_mfTabPos[1] - vTexDimsPoint3.y),		// Position Y
				glm::vec2(_mfTabDims[0], vTexDimsPoint3.y),	// Dimensions
				_mTabColour.get(),							// Vertex colour
				mTC.top.BL,
				mTC.top.BR,
				mTC.top.TR,
				mTC.top.TL);

			// Render the bottom edge
			pTri->addQuad2D(
				glm::vec2(
					_mfTabPos[0],							// Position X
					_mfTabPos[1] + _mfTabDims[1]),			// Position Y
				glm::vec2(_mfTabDims[0], vTexDimsPoint3.y),	// Dimensions
				_mTabColour.get(),							// Vertex colour
				mTC.bottom.BL,
				mTC.bottom.BR,
				mTC.bottom.TR,
				mTC.bottom.TL);

			// Render the top left corner
			pTri->addQuad2D(
				glm::vec2(
					_mfTabPos[0] - vTexDimsPoint3.x,			// Position X
					_mfTabPos[1] - vTexDimsPoint3.y),			// Position Y
				glm::vec2(vTexDimsPoint3.x, vTexDimsPoint3.y),	// Dimensions
				_mTabColour.get(),								// Vertex colour
				mTC.topLeft.BL,
				mTC.topLeft.BR,
				mTC.topLeft.TR,
				mTC.topLeft.TL);

			// Render the top right corner
			pTri->addQuad2D(
				glm::vec2(
					_mfTabPos[0] + _mfTabDims[0],				// Position X
					_mfTabPos[1] - vTexDimsPoint3.y),			// Position Y
				glm::vec2(vTexDimsPoint3.x, vTexDimsPoint3.y),	// Dimensions
				_mTabColour.get(),								// Vertex colour
				mTC.topRight.BL,
				mTC.topRight.BR,
				mTC.topRight.TR,
				mTC.topRight.TL);

			// Render the bottom left corner
			pTri->addQuad2D(
				glm::vec2(
					_mfTabPos[0] - vTexDimsPoint3.x,			// Position X
					_mfTabPos[1] + _mfTabDims[1]),				// Position Y
				glm::vec2(vTexDimsPoint3.x, vTexDimsPoint3.y),	// Dimensions
				_mTabColour.get(),								// Vertex colour
				mTC.bottomLeft.BL,
				mTC.bottomLeft.BR,
				mTC.bottomLeft.TR,
				mTC.bottomLeft.TL);

			// Render the bottom right corner
			pTri->addQuad2D(
				glm::vec2(
					_mfTabPos[0] + _mfTabDims[0],				// Position X
					_mfTabPos[1] + _mfTabDims[1]),				// Position Y
				glm::vec2(vTexDimsPoint3.x, vTexDimsPoint3.y),	// Dimensions
				_mTabColour.get(),								// Vertex colour
				mTC.bottomRight.BL,
				mTC.bottomRight.BR,
				mTC.bottomRight.TR,
				mTC.bottomRight.TL);
		}
		pTri->update();
		pTri->draw();

		pTexColour->unbindAll();	// Unbind textures
		pShader->unbind();	// Unbind the GUI shader
		glDisable(GL_BLEND);

	}

	void GUISlider::update(void* pParentContainer, bool bParentContainerAcceptingMouseClicks)
	{
		GUIManager* pGUIMan = GUIManager::getPointer();
		InputManager* pInput = InputManager::getPointer();
		ResourceManager* pRM = ResourceManager::getPointer();
		GUIContainer* pContainer = (GUIContainer*)pParentContainer;
		glm::vec2 vMousePos = pInput->mouse.getCursorPos();
		_mTimer.update();
		float fSecondsPast = _mTimer.getSecondsPast();
		if (fSecondsPast > 0.1f)
			fSecondsPast = 0.1f;
		GUITheme* pTheme = pGUIMan->getTheme(pContainer->mstrThemename);	// Get parent container's theme
		ResourceTexture2D* pTexColour = pRM->getTexture2D(pTheme->mImages.sliderTabColour);
		glm::vec2 vSliderTabTexDimsPoint3 = pTexColour->mvDimensions * 0.3333333f;
		glm::vec2 vSliderTabTexDimsPoint6 = pTexColour->mvDimensions * 0.6666666f;

		// Compute slider orientation
		_mbOrientationIsHorizontal = false;
		if (mfWidth > mfHeight)
		{
			_mbOrientationIsHorizontal = true;
		}

		// Compute tab dims
		if (_mbOrientationIsHorizontal)
		{
			_mfTabDims[0] = _mfTabRatio * (mfWidth - vSliderTabTexDimsPoint6.x);
			_mfTabDims[1] = mfHeight - vSliderTabTexDimsPoint6.y;
		}
		else
		{
			_mfTabDims[0] = mfWidth - vSliderTabTexDimsPoint6.x;
			_mfTabDims[1] = _mfTabRatio * (mfHeight - vSliderTabTexDimsPoint6.y);
		}

		// Compute tab position
		if (_mbOrientationIsHorizontal)
		{
			float fTabTotalMovementAmount = mfWidth - _mfTabDims[0] - vSliderTabTexDimsPoint6.x;
			float fCentrePosOfSlider = pContainer->mfPositionX + mfPositionX + (mfWidth * 0.5f) - (_mfTabDims[0] * 0.5f);

			// Convert _mfSliderPosition from 0 to 1 to -0.5 to 0.5f
			float fTabOffset = _mfSliderPosition - 0.5f;

			// Position offset
			fTabOffset *= fTabTotalMovementAmount;

			_mfTabPos[0] = fCentrePosOfSlider + fTabOffset;
			_mfTabPos[1] = pContainer->mfPositionY + mfPositionY + vSliderTabTexDimsPoint3.y;
		}
		else // Vertical
		{
			float fTabTotalMovementAmount = mfHeight - _mfTabDims[1] - vSliderTabTexDimsPoint6.y;
			float fCentrePosOfSlider = pContainer->mfPositionY + mfPositionY + (mfHeight * 0.5f) - (_mfTabDims[1] * 0.5f);

			// Convert _mfSliderPosition from 0 to 1 to -0.5 to 0.5f
			float fTabOffset = _mfSliderPosition - 0.5f;

			// Position offset
			fTabOffset *= fTabTotalMovementAmount;

			_mfTabPos[0] = pContainer->mfPositionX + mfPositionX + vSliderTabTexDimsPoint3.x;
			_mfTabPos[1] = fCentrePosOfSlider + fTabOffset;
		}

		bool bMouseOver = false;
		if (bParentContainerAcceptingMouseClicks)
		{
			// Determine if mouse cursor is over
			if (vMousePos.x > _mfTabPos[0] - vSliderTabTexDimsPoint3.x)
				if (vMousePos.x < _mfTabPos[0] + _mfTabDims[0] + vSliderTabTexDimsPoint3.x)
					if (vMousePos.y > _mfTabPos[1])
						if (vMousePos.y < _mfTabPos[1] + _mfTabDims[1] + vSliderTabTexDimsPoint3.y)
							bMouseOver = true;
			if (bMouseOver)
			{
				if (pInput->mouse.leftButtonOnce())
				{
					_mbTabBeingMoved = true;
				}
			}
			
			if (_mbTabBeingMoved)
			{
				if (!pInput->mouse.leftButDown())
					_mbTabBeingMoved = false;
				else
				{
					// Move tab position
					// As tab position is in range of 0-1, we need to compute movement based upon mouse delta and slider dims
					glm::vec2 vMouseDelta = pInput->mouse.getMouseDeltaGUI();
					float fTabPosOffset = 0;
					if (_mbOrientationIsHorizontal)
					{
						float fTabTotalMovementAmount = mfWidth - _mfTabDims[0];
						if (areFloatsEqual(fTabTotalMovementAmount, 0.0f))	// Prevent divide by zero
							fTabTotalMovementAmount = 0.0001f;
						fTabPosOffset += vMouseDelta.x * (1.0f / fTabTotalMovementAmount);
					}
					else
					{
						float fTabTotalMovementAmount = mfHeight - _mfTabDims[1];
						if (areFloatsEqual(fTabTotalMovementAmount, 0.0f))	// Prevent divide by zero
							fTabTotalMovementAmount = 0.0001f;
						fTabPosOffset += vMouseDelta.y * (1.0f / fTabTotalMovementAmount);
					}
					_mfSliderPosition += fTabPosOffset;
					clamp(_mfSliderPosition, 0.0f, 1.0f);
				}
			}
		}
		else  // Not accepting mouse clicks
		{

		}

		// Compute tab colour based on whether the mouse is over or not
		float fCol[4];
		fCol[0] = _mTabColour.red;
		fCol[1] = _mTabColour.green;
		fCol[2] = _mTabColour.blue;
		fCol[3] = _mTabColour.alpha;
		float fColTheme[4];
		if (bMouseOver)
		{
			fColTheme[0] = pTheme->mColours.sliderTabOver.red;
			fColTheme[1] = pTheme->mColours.sliderTabOver.green;
			fColTheme[2] = pTheme->mColours.sliderTabOver.blue;
			fColTheme[3] = pTheme->mColours.sliderTabOver.alpha;

			for (int iCol = 0; iCol < 4; iCol++)
			{
				if (fCol[iCol] < fColTheme[iCol])
				{
					fCol[iCol] += fSecondsPast * pTheme->mfSliderTabFadeSpeed;
					if (fCol[iCol] > fColTheme[iCol])
						fCol[iCol] = fColTheme[iCol];
				}
				else if (_mTabColour.red > fColTheme[iCol])
				{
					fCol[iCol] -= fSecondsPast * pTheme->mfSliderTabFadeSpeed;
					if (fCol[iCol] < fColTheme[iCol])
						fCol[iCol] = fColTheme[iCol];
				}
			}
		}
		else // Mouse not over
		{
			fColTheme[0] = pTheme->mColours.sliderTabNotOver.red;
			fColTheme[1] = pTheme->mColours.sliderTabNotOver.green;
			fColTheme[2] = pTheme->mColours.sliderTabNotOver.blue;
			fColTheme[3] = pTheme->mColours.sliderTabNotOver.alpha;

			for (int iCol = 0; iCol < 4; iCol++)
			{
				if (fCol[iCol] < fColTheme[iCol])
				{
					fCol[iCol] += fSecondsPast * pTheme->mfSliderTabFadeSpeed;
					if (fCol[iCol] > fColTheme[iCol])
						fCol[iCol] = fColTheme[iCol];
				}
				else if (_mTabColour.red > fColTheme[iCol])
				{
					fCol[iCol] -= fSecondsPast * pTheme->mfSliderTabFadeSpeed;
					if (fCol[iCol] < fColTheme[iCol])
						fCol[iCol] = fColTheme[iCol];
				}
			}
		}
		// Store colour
		_mTabColour.set(fCol[0], fCol[1], fCol[2], fCol[3]);

		// Update this object's tooltip
		GUITooltip* pTooltip = (GUITooltip*)mpTooltip;
		pTooltip->update(pParentContainer, (GUIBaseObject*)this, bMouseOver);
	}

	void GUISlider::setTabPos(float fPos)
	{
		clamp(fPos, 0.0f, 1.0f);
		_mfSliderPosition = fPos;
	}

	float GUISlider::getTabPos(void)
	{
		if (_mbOrientationIsHorizontal)
			return _mfSliderPosition;
		else
			return 1.0f - _mfSliderPosition;	// So that when tab is at bottom, 0 is the result and 1 when at top
	}

	void GUISlider::setTabRatio(float fRatio)
	{
		// Must be less than 1, otherwise the tab can be same width/height as the slider
		// and if this occurs, we could get a divide by zero error in the update method with the following code...
		//float fTabTotalMovementAmount = mfWidth - _mfTabDims[0];
		//fTabPosOffset += vMouseDelta.x * (1.0f / fTabTotalMovementAmount);
		clamp(fRatio, 0.001f, 0.9999f);
		_mfTabRatio = fRatio;
	}
}