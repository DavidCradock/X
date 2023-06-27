#include "PCH.h"
#include "GUISlider.h"
#include "GUIManager.h"
#include "resourceManager.h"
#include "window.h"
#include "input.h"
#include "audioManager.h"
#include "utilities.h"

namespace X
{
	GUISlider::GUISlider()
	{
		_mfSliderPosition = 0.5f;
		_mTabColour.set(1.0f, 1.0f, 1.0f, 1.0f);
		_mbTabBeingMoved = false;
	}

	void GUISlider::render(void* pParentContainer, const std::string& strFramebufferToSampleFrom)
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
		ResourceTexture2D* pTexColour = pRM->getTexture2D(pTheme->mImages.sliderBackColour);
		ResourceTexture2D* pTexNormal = pRM->getTexture2D(pTheme->mImages.sliderBackNormal);
		ResourceTexture2D* pTexReflection = pRM->getTexture2D(pTheme->mImages.reflection);
		ResourceFramebuffer* pFBSample = pRM->getFramebuffer(strFramebufferToSampleFrom);

		// Bind textures
		pTexColour->bind(0);
		pTexNormal->bind(1);
		pTexReflection->bind(2);
		pFBSample->bindAsTexture(3);

		// Render the slider centre
		glm::vec2 vTexDimsDiv3 = pTexColour->mvDimensions * 0.3333333f;
		pTri->removeGeom();
		pTri->addQuad2D(
			glm::vec2(
				pContainer->mfPositionX + mfPositionX,		// Position X
				pContainer->mfPositionY + mfPositionY),	// Position Y
			glm::vec2(mfWidth, mfHeight),									// Dimensions
			glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),								// Vertex colour
			mTC.centre.BL,
			mTC.centre.BR,
			mTC.centre.TR,
			mTC.centre.TL);
/*
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
*/
		pTri->update();
		pTri->draw();

		// Render the tab
		// Get textures
		pTexColour = pRM->getTexture2D(pTheme->mImages.sliderTabColour);
		pTexNormal = pRM->getTexture2D(pTheme->mImages.sliderTabNormal);

		// Bind textures
		pTexColour->bind(0);
		pTexNormal->bind(1);

		// Compute slider orientation
		bool bOrientationIsHorizontal = false;
		if (mfWidth > mfHeight)
		{
			bOrientationIsHorizontal = true;
		}

		// Compute tab position based upon _mfSliderPosition and orientation
		vTexDimsDiv3 = pTexColour->mvDimensions * 0.3333333f;


		if (bOrientationIsHorizontal)
		{
			// Render the slider tab centre
			pTri->removeGeom();
			pTri->addQuad2D(
				glm::vec2(
					_mfTabPos[0],							// Position X
					_mfTabPos[1]),						// Position Y
				glm::vec2(_mfTabDims[0], _mfTabDims[1]),	// Dimensions
				_mTabColour.get(),						// Vertex colour
				mTC.centre.BL,
				mTC.centre.BR,
				mTC.centre.TR,
				mTC.centre.TL);
/*
			// Render the left edge
			pTri->addQuad2D(
				glm::vec2(
					fTabPos[0] - vTexDimsDiv3.x,		// Position X
					fTabPos[1]),						// Position Y
				glm::vec2(vTexDimsDiv3.x, mfHeight),	// Dimensions
				_mTabColour.get(),						// Vertex colour
				mTC.left.BL,
				mTC.left.BR,
				mTC.left.TR,
				mTC.left.TL);

			// Render the right edge
			pTri->addQuad2D(
				glm::vec2(
					fTabPos[0] + fTabDims[0],			// Position X
					fTabPos[1]),						// Position Y
				glm::vec2(vTexDimsDiv3.x, mfHeight),	// Dimensions
				_mTabColour.get(),						// Vertex colour
				mTC.right.BL,
				mTC.right.BR,
				mTC.right.TR,
				mTC.right.TL);

			// Render the top edge
			pTri->addQuad2D(
				glm::vec2(
					fTabPos[0],							// Position X
					fTabPos[1] - vTexDimsDiv3.y),		// Position Y
				glm::vec2(fTabDims[0], vTexDimsDiv3.y),	// Dimensions
				_mTabColour.get(),						// Vertex colour
				mTC.top.BL,
				mTC.top.BR,
				mTC.top.TR,
				mTC.top.TL);

			// Render the bottom edge
			pTri->addQuad2D(
				glm::vec2(
					fTabPos[0],								// Position X
					fTabPos[1] + fTabDims[1]),				// Position Y
				glm::vec2(fTabDims[0], vTexDimsDiv3.y),		// Dimensions
				_mTabColour.get(),							// Vertex colour
				mTC.bottom.BL,
				mTC.bottom.BR,
				mTC.bottom.TR,
				mTC.bottom.TL);

			// Render the top left corner
			pTri->addQuad2D(
				glm::vec2(
					fTabPos[0] - vTexDimsDiv3.x,			// Position X
					fTabPos[1] - vTexDimsDiv3.y),			// Position Y
				glm::vec2(vTexDimsDiv3.x, vTexDimsDiv3.y),	// Dimensions
				_mTabColour.get(),							// Vertex colour
				mTC.topLeft.BL,
				mTC.topLeft.BR,
				mTC.topLeft.TR,
				mTC.topLeft.TL);

			// Render the top right corner
			pTri->addQuad2D(
				glm::vec2(
					fTabPos[0] + fTabDims[0],				// Position X
					fTabPos[1] - vTexDimsDiv3.y),			// Position Y
				glm::vec2(vTexDimsDiv3.x, vTexDimsDiv3.y),	// Dimensions
				_mTabColour.get(),							// Vertex colour
				mTC.topRight.BL,
				mTC.topRight.BR,
				mTC.topRight.TR,
				mTC.topRight.TL);

			// Render the bottom left corner
			pTri->addQuad2D(
				glm::vec2(
					fTabPos[0] - vTexDimsDiv3.x,			// Position X
					fTabPos[1] + fTabDims[1]),				// Position Y
				glm::vec2(vTexDimsDiv3.x, vTexDimsDiv3.y),	// Dimensions
				_mTabColour.get(),							// Vertex colour
				mTC.bottomLeft.BL,
				mTC.bottomLeft.BR,
				mTC.bottomLeft.TR,
				mTC.bottomLeft.TL);

			// Render the bottom right corner
			pTri->addQuad2D(
				glm::vec2(
					fTabPos[0] + fTabDims[0],				// Position X
					fTabPos[1] + fTabDims[1]),				// Position Y
				glm::vec2(vTexDimsDiv3.x, vTexDimsDiv3.y),	// Dimensions
				_mTabColour.get(),							// Vertex colour
				mTC.bottomRight.BL,
				mTC.bottomRight.BR,
				mTC.bottomRight.TR,
				mTC.bottomRight.TL);
*/
		}
		else  // Vertical
		{
			// Render the slider tab centre
			pTri->removeGeom();
			pTri->addQuad2D(
				glm::vec2(
					_mfTabPos[0],							// Position X
					_mfTabPos[1]),						// Position Y
				glm::vec2(_mfTabDims[0], _mfTabDims[1]),	// Dimensions
				_mTabColour.get(),						// Vertex colour
				mTC.centre.BL,
				mTC.centre.BR,
				mTC.centre.TR,
				mTC.centre.TL);
/*
			// Render the left edge
			pTri->addQuad2D(
				glm::vec2(
					fTabPos[0] - vTexDimsDiv3.x,		// Position X
					fTabPos[1]),						// Position Y
				glm::vec2(vTexDimsDiv3.x, fTabDims[1]),	// Dimensions
				_mTabColour.get(),						// Vertex colour
				mTC.left.BL,
				mTC.left.BR,
				mTC.left.TR,
				mTC.left.TL);

			// Render the right edge
			pTri->addQuad2D(
				glm::vec2(
					fTabPos[0] + fTabDims[0],			// Position X
					fTabPos[1]),						// Position Y
				glm::vec2(vTexDimsDiv3.x, fTabDims[1]),	// Dimensions
				_mTabColour.get(),						// Vertex colour
				mTC.right.BL,
				mTC.right.BR,
				mTC.right.TR,
				mTC.right.TL);

			// Render the top edge
			pTri->addQuad2D(
				glm::vec2(
					fTabPos[0],							// Position X
					fTabPos[1] - vTexDimsDiv3.y),		// Position Y
				glm::vec2(fTabDims[0], vTexDimsDiv3.y),	// Dimensions
				_mTabColour.get(),						// Vertex colour
				mTC.top.BL,
				mTC.top.BR,
				mTC.top.TR,
				mTC.top.TL);

			// Render the bottom edge
			pTri->addQuad2D(
				glm::vec2(
					fTabPos[0],								// Position X
					fTabPos[1] + fTabDims[1]),				// Position Y
				glm::vec2(fTabDims[0], vTexDimsDiv3.y),		// Dimensions
				_mTabColour.get(),							// Vertex colour
				mTC.bottom.BL,
				mTC.bottom.BR,
				mTC.bottom.TR,
				mTC.bottom.TL);

			// Render the top left corner
			pTri->addQuad2D(
				glm::vec2(
					fTabPos[0] - vTexDimsDiv3.x,			// Position X
					fTabPos[1] - vTexDimsDiv3.y),			// Position Y
				glm::vec2(vTexDimsDiv3.x, vTexDimsDiv3.y),	// Dimensions
				_mTabColour.get(),							// Vertex colour
				mTC.topLeft.BL,
				mTC.topLeft.BR,
				mTC.topLeft.TR,
				mTC.topLeft.TL);

			// Render the top right corner
			pTri->addQuad2D(
				glm::vec2(
					fTabPos[0] + fTabDims[0],				// Position X
					fTabPos[1] - vTexDimsDiv3.y),			// Position Y
				glm::vec2(vTexDimsDiv3.x, vTexDimsDiv3.y),	// Dimensions
				_mTabColour.get(),							// Vertex colour
				mTC.topRight.BL,
				mTC.topRight.BR,
				mTC.topRight.TR,
				mTC.topRight.TL);

			// Render the bottom left corner
			pTri->addQuad2D(
				glm::vec2(
					fTabPos[0] - vTexDimsDiv3.x,			// Position X
					fTabPos[1] + fTabDims[1]),				// Position Y
				glm::vec2(vTexDimsDiv3.x, vTexDimsDiv3.y),	// Dimensions
				_mTabColour.get(),							// Vertex colour
				mTC.bottomLeft.BL,
				mTC.bottomLeft.BR,
				mTC.bottomLeft.TR,
				mTC.bottomLeft.TL);

			// Render the bottom right corner
			pTri->addQuad2D(
				glm::vec2(
					fTabPos[0] + fTabDims[0],				// Position X
					fTabPos[1] + fTabDims[1]),				// Position Y
				glm::vec2(vTexDimsDiv3.x, vTexDimsDiv3.y),	// Dimensions
				_mTabColour.get(),							// Vertex colour
				mTC.bottomRight.BL,
				mTC.bottomRight.BR,
				mTC.bottomRight.TR,
				mTC.bottomRight.TL);
*/
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
		glm::vec2 vTexDimsDiv3 = pTexColour->mvDimensions * 0.3333333f;


		// Compute slider orientation
		bool bOrientationIsHorizontal = false;
		if (mfWidth > mfHeight)
		{
			bOrientationIsHorizontal = true;
		}

		// Compute tab dims
		if (bOrientationIsHorizontal)
		{
			_mfTabDims[0] = _mfTabRatio * mfWidth;
			_mfTabDims[1] = mfHeight;
		}
		else
		{
			_mfTabDims[0] = mfWidth;
			_mfTabDims[1] = _mfTabRatio * mfHeight;
		}

		// Compute tab position based upon _mfSliderPosition and orientation
		if (bOrientationIsHorizontal)
		{
			_mfTabPos[0] = pContainer->mfPositionX + mfPositionX + (_mfSliderPosition * mfWidth) - (_mfTabDims[0] * 0.5f);

			// We need to offset the tab position slightly so that it doesn't go over the edge due to tab dims
			float fScale = 0.5f - _mfSliderPosition;	// 0.5 to -0.5
			fScale *= 2.0f; // // 1.0f to - 1.0f
			fScale *= _mfTabDims[0] - (vTexDimsDiv3.x * 2.0f);
			_mfTabPos[0] += fScale;// + vTexDimsDiv3.x;

			_mfTabPos[1] = pContainer->mfPositionY + mfPositionY + vTexDimsDiv3.y;
		}
		else
		{
			_mfTabPos[0] = pContainer->mfPositionX + mfPositionX + vTexDimsDiv3.x;
			_mfTabPos[1] = pContainer->mfPositionY + mfPositionY + (_mfSliderPosition * mfHeight) - (_mfTabDims[1] * 0.5f);

			// We need to offset the tab position slightly so that it doesn't go over the edge due to tab dims
			float fScale = 0.5f - _mfSliderPosition;	// 0.5 to -0.5
			fScale *= 2.0f; // // 1.0f to - 1.0f
			fScale *= _mfTabDims[1] - (vTexDimsDiv3.y * 2.0f);
			_mfTabPos[1] += fScale;// +vTexDimsDiv3.y;

		}

		bool bMouseOver = false;
		if (bParentContainerAcceptingMouseClicks)
		{
			// Determine if mouse cursor is over
			if (vMousePos.x > _mfTabPos[0] - vTexDimsDiv3.x)
				if (vMousePos.x < _mfTabPos[0] + _mfTabDims[0] + (vTexDimsDiv3.x * 2.0f))
					if (vMousePos.y > _mfTabPos[1])
						if (vMousePos.y < _mfTabPos[1] + _mfTabDims[1] + (vTexDimsDiv3.y * 2.0f))
							bMouseOver = true;
			if (bMouseOver)
			{
				if (pInput->mouse.leftButDown())
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
					if (bOrientationIsHorizontal)
					{
						fTabPosOffset += vMouseDelta.x / mfWidth;
					}
					else
					{
						fTabPosOffset += vMouseDelta.y / mfHeight;
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


	}

	void GUISlider::setTabPos(float fPos)
	{
		clamp(fPos, 0.0f, 1.0f);
		_mfSliderPosition = fPos;

	}

	float GUISlider::getTabPos(void)
	{
		return _mfSliderPosition;
	}

	void GUISlider::setTabRatio(float fRatio)
	{
		clamp(fRatio, 0.1f, 1.0f);
		_mfTabRatio = fRatio;
	}
}