#include "PCH.h"
#include "GUIProgressBar.h"
#include "GUIManager.h"
#include "resourceManager.h"
#include "utilities.h"
#include "window.h"
#include "input.h"
#include "GUITooltip.h"

namespace X
{
	CGUIProgressBar::CGUIProgressBar()
	{
		_mfProgress = 0.5f;
		mpTooltip = new CGUITooltip;
		_mfAlpha = 1.0f;
	}

	CGUIProgressBar::~CGUIProgressBar()
	{
		delete mpTooltip;
	}

	void CGUIProgressBar::render(void* pParentContainer, const std::string& strFramebufferToSampleFrom)
	{
		CGUIContainer* pContainer = (CGUIContainer*)pParentContainer;
		SCGUIManager* pGUIManager = SCGUIManager::getPointer();
		CGUITheme* pTheme = pContainer->getTheme();
		CColour col;
		col.alpha *= _mfAlpha;
		renderBackground(pParentContainer, strFramebufferToSampleFrom, pTheme->mImages.progressBarBGColour, pTheme->mImages.progressBarBGNormal, col);

		// Get required resources needed to render the tab
		SCResourceManager* pRM = SCResourceManager::getPointer();
		SCWindow* pWindow = SCWindow::getPointer();
		CResourceVertexBuffer* pVB = pRM->getVertexBuffer("X:default");
		CResourceShader* pShader = pRM->getShader("X:gui");
		SCInputManager* pInput = SCInputManager::getPointer();

		CColour colFiller = pTheme->mColours.progressBarFiller;
		colFiller.alpha *= _mfAlpha;
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
		CResourceTexture2DFromFile* pTexColour = pRM->getTexture2DFromFile(pTheme->mImages.progressBarFillerColour);
		CResourceTexture2DFromFile* pTexNormal = pRM->getTexture2DFromFile(pTheme->mImages.progressBarFillerNormal);
		CResourceTexture2DFromFile* pTexReflection = pRM->getTexture2DFromFile(pTheme->mImages.reflection);
		CResourceFramebuffer* pFBSample = pRM->getFramebuffer(strFramebufferToSampleFrom);

		// Bind textures
		pTexColour->bind(0);
		pTexNormal->bind(1);
		pTexReflection->bind(2);
		pFBSample->bindAsTexture(3);

		CVector2f vTexDimsPoint3 = pTexColour->mvDimensions * 0.3333333f;
		CVector2f vTexDimsPoint6 = pTexColour->mvDimensions * 0.6666666f;
		CVector2f vFillerPos;
		CVector2f vFillerDims;
		
		if (_mbOrientationIsHorizontal)
		{
			vFillerPos.x = pContainer->mfPositionX + mfPositionX + vTexDimsPoint3.x;
			vFillerPos.y = pContainer->mfPositionY + mfPositionY + vTexDimsPoint3.y;
			vFillerDims.x = _mfProgress * (mfWidth - vTexDimsPoint6.x);
			vFillerDims.y = mfHeight - vTexDimsPoint6.y;

			// Render the filler centre
			pVB->removeGeom();
			pVB->addQuad2D(
				CVector2f(
					vFillerPos.x,							// Position X
					vFillerPos.y),							// Position Y
				vFillerDims,								// Dimensions
				colFiller,									// Vertex colour
				mTC.centre.BL,
				mTC.centre.BR,
				mTC.centre.TR,
				mTC.centre.TL);

			// Render the left edge
			pVB->addQuad2D(
				CVector2f(
					vFillerPos.x - vTexDimsPoint3.x,						// Position X
					vFillerPos.y),											// Position Y
				CVector2f(vTexDimsPoint3.x, mfHeight - vTexDimsPoint6.y),	// Dimensions
				colFiller,													// Vertex colour
				mTC.left.BL,
				mTC.left.BR,
				mTC.left.TR,
				mTC.left.TL);

			// Render the right edge
			pVB->addQuad2D(
				CVector2f(
					vFillerPos.x + vFillerDims.x,							// Position X
					vFillerPos.y),											// Position Y
				CVector2f(vTexDimsPoint3.x, mfHeight - vTexDimsPoint6.y),	// Dimensions
				colFiller,													// Vertex colour
				mTC.right.BL,
				mTC.right.BR,
				mTC.right.TR,
				mTC.right.TL);

			// Render the top edge
			pVB->addQuad2D(
				CVector2f(
					vFillerPos.x,							// Position X
					vFillerPos.y - vTexDimsPoint3.y),		// Position Y
				CVector2f(vFillerDims.x, vTexDimsPoint3.y),	// Dimensions
				colFiller,									// Vertex colour
				mTC.top.BL,
				mTC.top.BR,
				mTC.top.TR,
				mTC.top.TL);

			// Render the bottom edge
			pVB->addQuad2D(
				CVector2f(
					vFillerPos.x,							// Position X
					vFillerPos.y + vFillerDims.y),			// Position Y
				CVector2f(vFillerDims.x, vTexDimsPoint3.y),	// Dimensions
				colFiller,									// Vertex colour
				mTC.bottom.BL,
				mTC.bottom.BR,
				mTC.bottom.TR,
				mTC.bottom.TL);

			// Render the top left corner
			pVB->addQuad2D(
				CVector2f(
					vFillerPos.x - vTexDimsPoint3.x,			// Position X
					vFillerPos.y - vTexDimsPoint3.y),			// Position Y
				CVector2f(vTexDimsPoint3.x, vTexDimsPoint3.y),	// Dimensions
				colFiller,										// Vertex colour
				mTC.topLeft.BL,
				mTC.topLeft.BR,
				mTC.topLeft.TR,
				mTC.topLeft.TL);

			// Render the top right corner
			pVB->addQuad2D(
				CVector2f(
					vFillerPos.x + vFillerDims.x,				// Position X
					vFillerPos.y - vTexDimsPoint3.y),			// Position Y
				CVector2f(vTexDimsPoint3.x, vTexDimsPoint3.y),	// Dimensions
				colFiller,										// Vertex colour
				mTC.topRight.BL,
				mTC.topRight.BR,
				mTC.topRight.TR,
				mTC.topRight.TL);

			// Render the bottom left corner
			pVB->addQuad2D(
				CVector2f(
					vFillerPos.x - vTexDimsPoint3.x,			// Position X
					vFillerPos.y + vFillerDims.y),				// Position Y
				CVector2f(vTexDimsPoint3.x, vTexDimsPoint3.y),	// Dimensions
				colFiller,										// Vertex colour
				mTC.bottomLeft.BL,
				mTC.bottomLeft.BR,
				mTC.bottomLeft.TR,
				mTC.bottomLeft.TL);

			// Render the bottom right corner
			pVB->addQuad2D(
				CVector2f(
					vFillerPos.x + vFillerDims.x,				// Position X
					vFillerPos.y + vFillerDims.y),				// Position Y
				CVector2f(vTexDimsPoint3.x, vTexDimsPoint3.y),	// Dimensions
				colFiller,										// Vertex colour
				mTC.bottomRight.BL,
				mTC.bottomRight.BR,
				mTC.bottomRight.TR,
				mTC.bottomRight.TL);

		}
		else  // Vertical
		{
			vFillerPos.x = pContainer->mfPositionX + mfPositionX + vTexDimsPoint3.x;
			vFillerPos.y = pContainer->mfPositionY + mfPositionY + mfHeight - vTexDimsPoint3.y;
			vFillerDims.x = mfWidth - vTexDimsPoint6.x;
			vFillerDims.y = -_mfProgress * (mfHeight - vTexDimsPoint6.x);

			// Render the slider tab centre
			pVB->removeGeom();
			pVB->addQuad2D(
				CVector2f(
					vFillerPos.x,							// Position X
					vFillerPos.y),							// Position Y
				CVector2f(vFillerDims.x, vFillerDims.y),	// Dimensions
				colFiller,									// Vertex colour
				mTC.centre.BL,
				mTC.centre.BR,
				mTC.centre.TR,
				mTC.centre.TL);

			// Render the left edge
			pVB->addQuad2D(
				CVector2f(
					vFillerPos.x - vTexDimsPoint3.x,		// Position X
					vFillerPos.y),							// Position Y
				CVector2f(vTexDimsPoint3.x, vFillerDims.y),	// Dimensions
				colFiller,									// Vertex colour
				mTC.left.BL,
				mTC.left.BR,
				mTC.left.TR,
				mTC.left.TL);

			// Render the right edge
			pVB->addQuad2D(
				CVector2f(
					vFillerPos.x + vFillerDims.x,			// Position X
					vFillerPos.y),							// Position Y
				CVector2f(vTexDimsPoint3.x, vFillerDims.y),	// Dimensions
				colFiller,									// Vertex colour
				mTC.right.BL,
				mTC.right.BR,
				mTC.right.TR,
				mTC.right.TL);

			// Render the top edge
			pVB->addQuad2D(
				CVector2f(
					vFillerPos.x,										// Position X
					vFillerPos.y + vFillerDims.y - vTexDimsPoint3.y),	// Position Y
				CVector2f(vFillerDims.x, vTexDimsPoint3.y),				// Dimensions
				colFiller,												// Vertex colour
				mTC.top.BL,
				mTC.top.BR,
				mTC.top.TR,
				mTC.top.TL);

			// Render the bottom edge
			pVB->addQuad2D(
				CVector2f(
					vFillerPos.x,							// Position X
					vFillerPos.y),							// Position Y
				CVector2f(vFillerDims.x, vTexDimsPoint3.y),	// Dimensions
				colFiller,									// Vertex colour
				mTC.bottom.BL,
				mTC.bottom.BR,
				mTC.bottom.TR,
				mTC.bottom.TL);

			// Render the top left corner
			pVB->addQuad2D(
				CVector2f(
					vFillerPos.x - vTexDimsPoint3.x,					// Position X
					vFillerPos.y + vFillerDims.y - vTexDimsPoint3.y),	// Position Y
				CVector2f(vTexDimsPoint3.x, vTexDimsPoint3.y),			// Dimensions
				colFiller,												// Vertex colour
				mTC.topLeft.BL,
				mTC.topLeft.BR,
				mTC.topLeft.TR,
				mTC.topLeft.TL);

			// Render the top right corner
			pVB->addQuad2D(
				CVector2f(
					vFillerPos.x + vFillerDims.x,						// Position X
					vFillerPos.y + vFillerDims.y - vTexDimsPoint3.y),	// Position Y
				CVector2f(vTexDimsPoint3.x, vTexDimsPoint3.y),			// Dimensions
				colFiller,												// Vertex colour
				mTC.topRight.BL,
				mTC.topRight.BR,
				mTC.topRight.TR,
				mTC.topRight.TL);
		
			// Render the bottom left corner
			pVB->addQuad2D(
				CVector2f(
					vFillerPos.x - vTexDimsPoint3.x,			// Position X
					vFillerPos.y),								// Position Y
				CVector2f(vTexDimsPoint3.x, vTexDimsPoint3.y),	// Dimensions
				colFiller,										// Vertex colour
				mTC.bottomLeft.BL,
				mTC.bottomLeft.BR,
				mTC.bottomLeft.TR,
				mTC.bottomLeft.TL);

			// Render the bottom right corner
			pVB->addQuad2D(
				CVector2f(
					vFillerPos.x + vFillerDims.x,				// Position X
					vFillerPos.y),								// Position Y
				CVector2f(vTexDimsPoint3.x, vTexDimsPoint3.y),	// Dimensions
				colFiller,										// Vertex colour
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

	void CGUIProgressBar::update(void* pParentContainer, bool bParentContainerAcceptingMouseClicks)
	{
		// Compute orientation
		_mbOrientationIsHorizontal = false;
		if (mfWidth > mfHeight)
		{
			_mbOrientationIsHorizontal = true;
		}

		// Update this object's tooltip
		SCInputManager* pInput = SCInputManager::getPointer();
		CVector2f vMousePos = pInput->mouse.getCursorPos();
		CGUIContainer* pContainer = (CGUIContainer*)pParentContainer;
		bool bMouseOver = false;
		if (bParentContainerAcceptingMouseClicks)
		{
			// Determine if mouse cursor is over
			if (vMousePos.x > pContainer->mfPositionX + mfPositionX)
				if (vMousePos.x < pContainer->mfPositionX + mfPositionX + mfWidth)
					if (vMousePos.y > pContainer->mfPositionY + mfPositionY)
						if (vMousePos.y < pContainer->mfPositionY + mfPositionY + mfHeight)
							bMouseOver = true;
		}
		mpTooltip->update(pParentContainer, (CGUIBaseObject*)this, bMouseOver);
	}

	void CGUIProgressBar::setProgress(float fPos)
	{
		clamp(fPos, 0.0f, 1.0f);
		_mfProgress = fPos;
	}

	float CGUIProgressBar::getProgress(void) const
	{
		return _mfProgress;
	}

	void CGUIProgressBar::setAlpha(float fAlpha)
	{
		_mfAlpha = fAlpha;
	}
}