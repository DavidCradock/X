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
	CGUISlider::CGUISlider()
	{
		_mfSliderPosition = 0.5f;
		_mTabColour.set(1.0f, 1.0f, 1.0f, 1.0f);
		_mbTabBeingMoved = false;
		_mfTabRatio = 0.5f;
		mpTooltip = new CGUITooltip;
	}

	CGUISlider::~CGUISlider()
	{
		delete mpTooltip;
	}

	void CGUISlider::render(void* pParentContainer, const std::string& strFramebufferToSampleFrom)
	{
		CGUIContainer* pContainer = (CGUIContainer*)pParentContainer;
		SCGUIManager* pGUIManager = SCGUIManager::getPointer();
		CGUITheme* pTheme = pContainer->getTheme();
		CColour col;
		renderBackground(pParentContainer, strFramebufferToSampleFrom, pTheme->mImages.sliderBGColour, pTheme->mImages.sliderBGNormal, col);

		// Get required resources needed to render the tab
		SCResourceManager* pRM = SCResourceManager::getPointer();
		SCWindow* pWindow = SCWindow::getPointer();
		CResourceVertexBuffer* pVB = pRM->getVertexBuffer("X:default");
		CResourceShader* pShader = pRM->getShader("X:gui");
		SCInputManager* pInput = SCInputManager::getPointer();

		pShader->bind();

		// Setup the projection matrix as orthographic
		CMatrix matProjection;
		matProjection.setProjectionOrthographic(0.0f, float(pWindow->getWidth()), 0.0f, float(pWindow->getHeight()), -1.0f, 1.0f);
		pShader->setMat4("transform", matProjection);

		// Tell OpenGL, for each sampler, to which texture unit it belongs to
		pShader->setInt("texture0_colour", 0);
		pShader->setInt("texture1_normal", 1);
		pShader->setInt("texture2_reflection", 2);
		pShader->setInt("texture3_background", 3);
		pShader->setFloat("fNormalAmount", pTheme->mfNormalAmount);
		pShader->setFloat("fReflectionAmount", pTheme->mfReflectionAmount);
		pShader->setFloat("fMouseCursorDistance", pTheme->mfMouseCursorDistance);

		// Set mouse position, inverting Y position
		CVector2f vMousePos = pInput->mouse.getCursorPos();
		vMousePos.y = float(pWindow->getHeight()) - vMousePos.y;
		pShader->setVec2("v2MousePos", vMousePos);

		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);

		// Get textures
		CResourceTexture2DFromFile* pTexColour = pRM->getTexture2DFromFile(pTheme->mImages.sliderTabColour);
		CResourceTexture2DFromFile* pTexNormal = pRM->getTexture2DFromFile(pTheme->mImages.sliderTabNormal);
		CResourceTexture2DFromFile* pTexReflection = pRM->getTexture2DFromFile(pTheme->mImages.reflection);
		CResourceFramebuffer* pFBSample = pRM->getFramebuffer(strFramebufferToSampleFrom);

		// Bind textures
		pTexColour->bind(0);
		pTexNormal->bind(1);
		pTexReflection->bind(2);
		pFBSample->bindAsTexture(3);


		CVector2f vTexDimsPoint3 = pTexColour->mvDimensions * 0.3333333f;
		CVector2f vTexDimsPoint6 = pTexColour->mvDimensions * 0.6666666f;

		if (_mbOrientationIsHorizontal)
		{
			// Render the slider tab centre
			pVB->removeGeom();
			pVB->addQuad2D(
				CVector2f(
					_mfTabPos[0],							// Position X
					_mfTabPos[1]),							// Position Y
				CVector2f(_mfTabDims[0], _mfTabDims[1]),	// Dimensions
				_mTabColour,								// Vertex colour
				mTC.centre.BL,
				mTC.centre.BR,
				mTC.centre.TR,
				mTC.centre.TL);

			// Render the left edge
			pVB->addQuad2D(
				CVector2f(
					_mfTabPos[0] - vTexDimsPoint3.x,						// Position X
					_mfTabPos[1]),											// Position Y
				CVector2f(vTexDimsPoint3.x, mfHeight - vTexDimsPoint6.y),	// Dimensions
				_mTabColour,												// Vertex colour
				mTC.left.BL,
				mTC.left.BR,
				mTC.left.TR,
				mTC.left.TL);

			// Render the right edge
			pVB->addQuad2D(
				CVector2f(
					_mfTabPos[0] + _mfTabDims[0],							// Position X
					_mfTabPos[1]),											// Position Y
				CVector2f(vTexDimsPoint3.x, mfHeight - vTexDimsPoint6.y),	// Dimensions
				_mTabColour,												// Vertex colour
				mTC.right.BL,
				mTC.right.BR,
				mTC.right.TR,
				mTC.right.TL);

			// Render the top edge
			pVB->addQuad2D(
				CVector2f(
					_mfTabPos[0],							// Position X
					_mfTabPos[1] - vTexDimsPoint3.y),		// Position Y
				CVector2f(_mfTabDims[0], vTexDimsPoint3.y),	// Dimensions
				_mTabColour,								// Vertex colour
				mTC.top.BL,
				mTC.top.BR,
				mTC.top.TR,
				mTC.top.TL);

			// Render the bottom edge
			pVB->addQuad2D(
				CVector2f(
					_mfTabPos[0],							// Position X
					_mfTabPos[1] + _mfTabDims[1]),			// Position Y
				CVector2f(_mfTabDims[0], vTexDimsPoint3.y),	// Dimensions
				_mTabColour,								// Vertex colour
				mTC.bottom.BL,
				mTC.bottom.BR,
				mTC.bottom.TR,
				mTC.bottom.TL);

			// Render the top left corner
			pVB->addQuad2D(
				CVector2f(
					_mfTabPos[0] - vTexDimsPoint3.x,			// Position X
					_mfTabPos[1] - vTexDimsPoint3.y),			// Position Y
				CVector2f(vTexDimsPoint3.x, vTexDimsPoint3.y),	// Dimensions
				_mTabColour,									// Vertex colour
				mTC.topLeft.BL,
				mTC.topLeft.BR,
				mTC.topLeft.TR,
				mTC.topLeft.TL);

			// Render the top right corner
			pVB->addQuad2D(
				CVector2f(
					_mfTabPos[0] + _mfTabDims[0],				// Position X
					_mfTabPos[1] - vTexDimsPoint3.y),			// Position Y
				CVector2f(vTexDimsPoint3.x, vTexDimsPoint3.y),	// Dimensions
				_mTabColour,									// Vertex colour
				mTC.topRight.BL,
				mTC.topRight.BR,
				mTC.topRight.TR,
				mTC.topRight.TL);

			// Render the bottom left corner
			pVB->addQuad2D(
				CVector2f(
					_mfTabPos[0] - vTexDimsPoint3.x,			// Position X
					_mfTabPos[1] + _mfTabDims[1]),				// Position Y
				CVector2f(vTexDimsPoint3.x, vTexDimsPoint3.y),	// Dimensions
				_mTabColour,									// Vertex colour
				mTC.bottomLeft.BL,
				mTC.bottomLeft.BR,
				mTC.bottomLeft.TR,
				mTC.bottomLeft.TL);

			// Render the bottom right corner
			pVB->addQuad2D(
				CVector2f(
					_mfTabPos[0] + _mfTabDims[0],				// Position X
					_mfTabPos[1] + _mfTabDims[1]),				// Position Y
				CVector2f(vTexDimsPoint3.x, vTexDimsPoint3.y),	// Dimensions
				_mTabColour,									// Vertex colour
				mTC.bottomRight.BL,
				mTC.bottomRight.BR,
				mTC.bottomRight.TR,
				mTC.bottomRight.TL);

		}
		else  // Vertical
		{
			// Render the slider tab centre
			pVB->removeGeom();
			pVB->addQuad2D(
				CVector2f(
					_mfTabPos[0],							// Position X
					_mfTabPos[1]),							// Position Y
				CVector2f(_mfTabDims[0], _mfTabDims[1]),	// Dimensions
				_mTabColour,								// Vertex colour
				mTC.centre.BL,
				mTC.centre.BR,
				mTC.centre.TR,
				mTC.centre.TL);

			// Render the left edge
			pVB->addQuad2D(
				CVector2f(
					_mfTabPos[0] - vTexDimsPoint3.x,		// Position X
					_mfTabPos[1]),							// Position Y
				CVector2f(vTexDimsPoint3.x, _mfTabDims[1]),	// Dimensions
				_mTabColour,								// Vertex colour
				mTC.left.BL,
				mTC.left.BR,
				mTC.left.TR,
				mTC.left.TL);

			// Render the right edge
			pVB->addQuad2D(
				CVector2f(
					_mfTabPos[0] + _mfTabDims[0],			// Position X
					_mfTabPos[1]),							// Position Y
				CVector2f(vTexDimsPoint3.x, _mfTabDims[1]),	// Dimensions
				_mTabColour,								// Vertex colour
				mTC.right.BL,
				mTC.right.BR,
				mTC.right.TR,
				mTC.right.TL);

			// Render the top edge
			pVB->addQuad2D(
				CVector2f(
					_mfTabPos[0],							// Position X
					_mfTabPos[1] - vTexDimsPoint3.y),		// Position Y
				CVector2f(_mfTabDims[0], vTexDimsPoint3.y),	// Dimensions
				_mTabColour,								// Vertex colour
				mTC.top.BL,
				mTC.top.BR,
				mTC.top.TR,
				mTC.top.TL);

			// Render the bottom edge
			pVB->addQuad2D(
				CVector2f(
					_mfTabPos[0],							// Position X
					_mfTabPos[1] + _mfTabDims[1]),			// Position Y
				CVector2f(_mfTabDims[0], vTexDimsPoint3.y),	// Dimensions
				_mTabColour,								// Vertex colour
				mTC.bottom.BL,
				mTC.bottom.BR,
				mTC.bottom.TR,
				mTC.bottom.TL);

			// Render the top left corner
			pVB->addQuad2D(
				CVector2f(
					_mfTabPos[0] - vTexDimsPoint3.x,			// Position X
					_mfTabPos[1] - vTexDimsPoint3.y),			// Position Y
				CVector2f(vTexDimsPoint3.x, vTexDimsPoint3.y),	// Dimensions
				_mTabColour,									// Vertex colour
				mTC.topLeft.BL,
				mTC.topLeft.BR,
				mTC.topLeft.TR,
				mTC.topLeft.TL);

			// Render the top right corner
			pVB->addQuad2D(
				CVector2f(
					_mfTabPos[0] + _mfTabDims[0],				// Position X
					_mfTabPos[1] - vTexDimsPoint3.y),			// Position Y
				CVector2f(vTexDimsPoint3.x, vTexDimsPoint3.y),	// Dimensions
				_mTabColour,									// Vertex colour
				mTC.topRight.BL,
				mTC.topRight.BR,
				mTC.topRight.TR,
				mTC.topRight.TL);

			// Render the bottom left corner
			pVB->addQuad2D(
				CVector2f(
					_mfTabPos[0] - vTexDimsPoint3.x,			// Position X
					_mfTabPos[1] + _mfTabDims[1]),				// Position Y
				CVector2f(vTexDimsPoint3.x, vTexDimsPoint3.y),	// Dimensions
				_mTabColour,									// Vertex colour
				mTC.bottomLeft.BL,
				mTC.bottomLeft.BR,
				mTC.bottomLeft.TR,
				mTC.bottomLeft.TL);

			// Render the bottom right corner
			pVB->addQuad2D(
				CVector2f(
					_mfTabPos[0] + _mfTabDims[0],				// Position X
					_mfTabPos[1] + _mfTabDims[1]),				// Position Y
				CVector2f(vTexDimsPoint3.x, vTexDimsPoint3.y),	// Dimensions
				_mTabColour,									// Vertex colour
				mTC.bottomRight.BL,
				mTC.bottomRight.BR,
				mTC.bottomRight.TR,
				mTC.bottomRight.TL);
		}
		pVB->update();
		pVB->render();

		pTexColour->unbindAll();	// Unbind textures
		pShader->unbind();	// Unbind the GUI shader
		glDisable(GL_BLEND);

	}

	void CGUISlider::update(void* pParentContainer, bool bParentContainerAcceptingMouseClicks)
	{
		SCGUIManager* pGUIMan = SCGUIManager::getPointer();
		SCInputManager* pInput = SCInputManager::getPointer();
		SCResourceManager* pRM = SCResourceManager::getPointer();
		CGUIContainer* pContainer = (CGUIContainer*)pParentContainer;
		CVector2f vMousePos = pInput->mouse.getCursorPos();
		_mTimer.update();
		float fSecondsPast = _mTimer.getSecondsPast();
		if (fSecondsPast > 0.1f)
			fSecondsPast = 0.1f;
		CGUITheme* pTheme = pContainer->getTheme();
		CResourceTexture2DFromFile* pTexColour = pRM->getTexture2DFromFile(pTheme->mImages.sliderTabColour);
		CVector2f vSliderTabTexDimsPoint3 = pTexColour->mvDimensions * 0.3333333f;
		CVector2f vSliderTabTexDimsPoint6 = pTexColour->mvDimensions * 0.6666666f;

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
					CVector2f vMouseDelta = pInput->mouse.getMouseDeltaGUI();
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
		mpTooltip->update(pParentContainer, (CGUIBaseObject*)this, bMouseOver);
	}

	void CGUISlider::setTabPos(float fPos)
	{
		clamp(fPos, 0.0f, 1.0f);
		_mfSliderPosition = fPos;
	}

	float CGUISlider::getTabPos(void) const
	{
		if (_mbOrientationIsHorizontal)
			return _mfSliderPosition;
		else
			return 1.0f - _mfSliderPosition;	// So that when tab is at bottom, 0 is the result and 1 when at top
	}

	void CGUISlider::setTabRatio(float fRatio)
	{
		// Must be less than 1, otherwise the tab can be same width/height as the slider
		// and if this occurs, we could get a divide by zero error in the update method with the following code...
		//float fTabTotalMovementAmount = mfWidth - _mfTabDims[0];
		//fTabPosOffset += vMouseDelta.x * (1.0f / fTabTotalMovementAmount);
		clamp(fRatio, 0.001f, 0.9999f);
		_mfTabRatio = fRatio;
	}

	float CGUISlider::getTabRatio(void) const
	{
		return _mfTabRatio;
	}

	bool CGUISlider::getOrientationHorizontal(void) const
	{
		return _mbOrientationIsHorizontal;
	}

	CVector2f CGUISlider::getTabDims(void) const
	{
		return CVector2f(_mfTabDims[0], _mfTabDims[1]);
	}

}