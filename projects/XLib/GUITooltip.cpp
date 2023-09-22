#include "PCH.h"
#include "GUITooltip.h"
#include "GUIManager.h"
#include "singletons.h"
#include "resourceManager.h"
#include "input.h"
#include "logging.h"

namespace X
{
	CGUITooltip::CGUITooltip()
	{
		_mbEnabled = false;
		_mColour.set(1.0f, 1.0f, 1.0f, 0.0f);
		_mfTooltipDelay = 0.0f;
	}

	CGUITooltip::~CGUITooltip()
	{

	}

	void CGUITooltip::render(void* pParentContainer, const std::string& strFramebufferToSampleFrom)
	{
		// If colour alpha is zero, do not render
		if (_mColour.alpha <= 0.0f)
			return;

		// Get pointers to stuff we need
		CGUIContainer* pContainer = (CGUIContainer*)pParentContainer;	// Needed to get theme
		CGUITheme* pTheme = pContainer->getTheme();
		CResourceFramebuffer* pFBGUITarget = x->pResource->getFramebuffer("X:gui");
		CResourceFramebuffer* pFB = x->pResource->getFramebuffer("X:guitooltipFB");
		CResourceTexture2DFromFile* pTexColour = x->pResource->getTexture2DFromFile(pTheme->mImages.tooltipBGColour);
		CVector2f vTexDimsPoint3 = pTexColour->getDimensions() * 0.3333333f;
		CVector2f vTexDimsPoint6 = pTexColour->getDimensions() * 0.6666666f;

		// Tooltip framebuffer: set size if needed, clear it and set as render target
		// We need to make the framebuffer bigger than just the mfWidth and mfHeight, as we need to accommodate borders
		pFB->resize(unsigned int(mfWidth + vTexDimsPoint6.x), unsigned int(mfHeight + vTexDimsPoint6.y));
		pFB->bindAsRenderTarget(true, false);

		// Render the background of the tooltip
		CResourceVertexBufferCPT* pVB = x->pResource->getVertexBufferCPT("X:default");
		CResourceShader* pShader = x->pResource->getShader("X:VBCPT");
		pShader->bind();

		// Setup the matrices
		CMatrix matWorld, matView;
		CMatrix matProjection;
		matProjection.setProjectionOrthographic(0.0f, float(pFB->getWidth()), 0.0f, float(pFB->getHeight()), -1.0f, 1.0f);
		pShader->setMat4("matrixWorld", matWorld);
		pShader->setMat4("matrixView", matView);
		pShader->setMat4("matrixProjection", matProjection);

		// Tell OpenGL, for each sampler, to which texture unit it belongs to
		pShader->setInt("texture0", 0);

		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);

		// Bind textures
		pTexColour->bind(0);

		// Render the centre
		CVector2f vPos(0.0f, 0.0f);
		CColour colWhite;
		pVB->removeGeom();

		pVB->addQuad2D(vTexDimsPoint3,		// Position
			CVector2f(mfWidth, mfHeight),	// Dimensions
			colWhite,
			mTC.centre.BL,
			mTC.centre.BR,
			mTC.centre.TR,
			mTC.centre.TL);

		// Render the left edge
		pVB->addQuad2D(
			CVector2f(0.0f,							// Position X
				vTexDimsPoint3.y),					// Position Y
			CVector2f(vTexDimsPoint3.x, mfHeight),	// Dimensions
			colWhite,
			mTC.left.BL,
			mTC.left.BR,
			mTC.left.TR,
			mTC.left.TL);

		// Render the right edge
		pVB->addQuad2D(
			CVector2f(mfWidth + vTexDimsPoint3.x,	// Position X
				vTexDimsPoint3.y),					// Position Y
			CVector2f(vTexDimsPoint3.x, mfHeight),	// Dimensions
			colWhite,
			mTC.right.BL,
			mTC.right.BR,
			mTC.right.TR,
			mTC.right.TL);

		// Render the top edge
		pVB->addQuad2D(
			CVector2f(
				vTexDimsPoint3.x,					// Position X
				0.0f),								// Position Y
			CVector2f(mfWidth, vTexDimsPoint3.y),	// Dimensions
			colWhite,
			mTC.top.BL,
			mTC.top.BR,
			mTC.top.TR,
			mTC.top.TL);

		// Render the bottom edge
		pVB->addQuad2D(
			CVector2f(
				vTexDimsPoint3.x,					// Position X
				mfHeight + vTexDimsPoint3.y),		// Position Y
			CVector2f(mfWidth, vTexDimsPoint3.y),	// Dimensions
			colWhite,
			mTC.bottom.BL,
			mTC.bottom.BR,
			mTC.bottom.TR,
			mTC.bottom.TL);

		// Render the top left corner
		pVB->addQuad2D(
			CVector2f(
				0.0f,										// Position X
				0.0f),										// Position Y
			CVector2f(vTexDimsPoint3.x, vTexDimsPoint3.y),	// Dimensions
			colWhite,
			mTC.topLeft.BL,
			mTC.topLeft.BR,
			mTC.topLeft.TR,
			mTC.topLeft.TL);

		// Render the top right corner
		pVB->addQuad2D(
			CVector2f(
				mfWidth + vTexDimsPoint3.x,					// Position X
				0.0f),										// Position Y
			CVector2f(vTexDimsPoint3.x, vTexDimsPoint3.y),	// Dimensions
			colWhite,
			mTC.topRight.BL,
			mTC.topRight.BR,
			mTC.topRight.TR,
			mTC.topRight.TL);

		// Render the bottom left corner
		pVB->addQuad2D(
			CVector2f(
				0.0f,										// Position X
				mfHeight + vTexDimsPoint3.y),				// Position Y
			CVector2f(vTexDimsPoint3.x, vTexDimsPoint3.y),	// Dimensions
			colWhite,
			mTC.bottomLeft.BL,
			mTC.bottomLeft.BR,
			mTC.bottomLeft.TR,
			mTC.bottomLeft.TL);

		// Render the bottom right corner
		pVB->addQuad2D(
			CVector2f(
				mfWidth + vTexDimsPoint3.x,					// Position X
				mfHeight + vTexDimsPoint3.y),				// Position Y
			CVector2f(vTexDimsPoint3.x, vTexDimsPoint3.y),	// Dimensions
			colWhite,
			mTC.bottomRight.BL,
			mTC.bottomRight.BR,
			mTC.bottomRight.TR,
			mTC.bottomRight.TL);

		pVB->update();
		pVB->render();

		pTexColour->unbindAll();	// Unbind textures
		pShader->unbind();	// Unbind the GUI shader

		// Render each text
		std::map<std::string, CGUIText*>::iterator itText = _mmapTexts.begin();
		while (itText != _mmapTexts.end())
		{
			// Offset object by vTexDimsPoint3
			itText->second->mfPositionX += vTexDimsPoint3.x;
			itText->second->mfPositionY += vTexDimsPoint3.y;
			itText->second->renderForTooltip(pParentContainer, (unsigned int)pFB->getWidth(), (unsigned int)pFB->getHeight());
			itText->second->mfPositionX -= vTexDimsPoint3.x;
			itText->second->mfPositionY -= vTexDimsPoint3.y;

			itText++;
		}
		// CResourceFont turns these off
		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);

		// Render each line graph
		std::map<std::string, CGUILineGraph*>::iterator itLineGraph = _mmapLineGraphs.begin();
		while (itLineGraph != _mmapLineGraphs.end())
		{
//			itLineGraph->second->render(pParentContainer, strFramebufferToSampleFrom);
			itLineGraph++;
		}

		// Render each progress bar
		std::map<std::string, CGUIProgressBar*>::iterator itProgressBar = _mmapProgressBars.begin();
		while (itProgressBar != _mmapProgressBars.end())
		{
//			itProgressBar->second->render(pParentContainer, strFramebufferToSampleFrom);
			itProgressBar++;
		}

		// Render each image
		std::map<std::string, CGUIImage*>::iterator itImage = _mmapImages.begin();
		while (itImage != _mmapImages.end())
		{
//			itImage->second->render(pParentContainer, strFramebufferToSampleFrom);
			itImage++;
		}

		// Render each animated image
		std::map<std::string, CGUIImageAnimated*>::iterator itImageAnimated = _mmapImageAnimateds.begin();
		while (itImageAnimated != _mmapImageAnimateds.end())
		{
//			itImageAnimated->second->render(pParentContainer, strFramebufferToSampleFrom);
			itImageAnimated++;
		}

		// Images framebuffer
		std::map<std::string, CGUIImageFramebuffer*>::iterator itImageFB = _mmapImageFramebuffers.begin();
		while (itImageFB != _mmapImageFramebuffers.end())
		{
//			itImageFB->second->render(pParentContainer, strFramebufferToSampleFrom);
			itImageFB++;
		}

		// Images depth buffer
		std::map<std::string, CGUIImageDepthbuffer*>::iterator itImageDB = _mmapImageDepthbuffers.begin();
		while (itImageDB != _mmapImageDepthbuffers.end())
		{
//			itImageDB->second->render(pParentContainer, strFramebufferToSampleFrom);
			itImageDB++;
		}

		// Unbind framebuffer which now holds all objects and render it
		pFB->unbindAsRenderTarget();

		// Rebind the GUI framebuffer as render target
		pFBGUITarget->bindAsRenderTarget(false, false);

//		glEnable(GL_BLEND);
//		glDisable(GL_DEPTH_TEST);
//		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


		// Now render the framebuffer which now contains the tooltip.
		// But before we do so, we need to make sure it fits on screen, otherwise reposition as required.

		CVector3f vTooltipPos;
		vTooltipPos.x = mfPositionX + vTexDimsPoint3.x;
		vTooltipPos.y = mfPositionY;
		CVector2f vMousePos = x->pInput->mouse.getCursorPos();
		CVector2f vTargetFBDims = pFBGUITarget->getDimensions();

		// If goes off right edge of render target
		if (vTooltipPos.x + pFB->getWidth() > vTargetFBDims.x)
		{
			// Position so the tooltip's right most edge is to the left of the mouse cursor
			vTooltipPos.x = vTargetFBDims.x - (float)pFB->getWidth();
		}
		// If goes off left edge of render target
		else if (vTooltipPos.x < 0)
		{
			vTooltipPos.x = 0;
			// If the tooltip is large, it may be rendered underneath the cursor which we don't want, 
			// move it down so it's not rendered underneath the cursor
//			vTooltipPosition.y += vTexDimsPoint3.y * 2.0f;
//			vTooltipPos.y += x->pWindow->getMouseCursorDimensions().y;
		}
		// If goes off bottom edge of render target
		if (vTooltipPos.y + pFB->getHeight() > vTargetFBDims.y)
		{
			// Position so the tooltip's bottom most edge is above the mouse cursor
			vTooltipPos.y = vTargetFBDims.y - (float)pFB->getHeight();
		}
		// If goes off top edge of render target
		else if (vTooltipPos.y < 0)
		{
			vTooltipPos.y = 0;
		}
		pFB->renderTo2DQuad(unsigned int(vTooltipPos.x), unsigned int(vTooltipPos.y), unsigned int(pFB->getWidth()), unsigned int(pFB->getHeight()), _mColour);
		glDisable(GL_BLEND);
	}

	void CGUITooltip::update(void* pParentContainer, CGUIBaseObject* pGUIOwner, bool bMouseOverOwner)
	{
		// Set position of tooltip
		CVector2f vMousePos = x->pInput->mouse.getCursorPos();
		CVector2f vTooltipOffset = x->pGUI->getTooltipOffset();
		mfPositionX = vMousePos.x + vTooltipOffset.x;
		mfPositionY = vMousePos.y + vTooltipOffset.y;

		// Deal with fading in and out
		CGUIContainer* pContainer = (CGUIContainer*)pParentContainer;
		CGUITheme* pTheme = pContainer->getTheme();
		_mTimer.update();
		float fSecondsPast = _mTimer.getSecondsPast();
		if (!bMouseOverOwner)
		{
			_mColour.alpha -= fSecondsPast * pTheme->mfTooltipFadeSpeed;
			clamp(_mColour.alpha, 0.0f, 1.0f);
			if (_mColour.alpha == 0.0f)
			{
				_mfTooltipDelay = 0.0f;	// Reset delay
			}
		}
		else if (_mbEnabled)
		{
			if (bMouseOverOwner)
			{
				// Delay tooltip from fading in
				_mfTooltipDelay += fSecondsPast;
				if (_mfTooltipDelay >= x->pGUI->getTooltipDelay())
				{
					_mfTooltipDelay = x->pGUI->getTooltipDelay();
					_mColour.alpha += fSecondsPast * pTheme->mfTooltipFadeSpeed;
					clamp(_mColour.alpha, 0.0f, 1.0f);
				}
			}
		}

		// Set dimensions of this tooltip so that all objects fit inside of it
		mfWidth = 1;
		mfHeight = 1;
		// Text
		std::map<std::string, CGUIText*>::iterator itText = _mmapTexts.begin();
		float fMaxPos[2];
		CResourceFont* pFont = x->pResource->getFont(pTheme->mFonts.text);
		while (itText != _mmapTexts.end())
		{
			// We need to set the dimensions of the text object using the current theme
//			itText->second->mfWidth = pFont->getTextWidth(itText->second->getText());
//			itText->second->mfHeight = pFont->getTextHeight(1.0f);

			// Do not render, but obtain each line of text and the total width and height of all text which is rendered.
			std::vector<std::string> vstrLines;
			int iTextTotalHeight;
			pFont->printInRectNewline(false, itText->second->getText(), 0, 0, (int)x->pWindow->getWidth(), (int)x->pWindow->getHeight(), vstrLines, iTextTotalHeight, "\n", 1.0f);
			int iTextMaxWidth = 0;
			std::vector<std::string>::iterator it = vstrLines.begin();
			while (it != vstrLines.end())
			{
				float fTextLineWidth = pFont->getTextWidth(*it, 1.0f);
				if (iTextMaxWidth < (int)fTextLineWidth)
					iTextMaxWidth = (int)fTextLineWidth;
				it++;
			}
			mfWidth = (float)iTextMaxWidth;
			mfHeight = (float)iTextTotalHeight;

//			fMaxPos[0] = itText->second->mfPositionX + itText->second->mfWidth;
//			if (fMaxPos[0] > mfWidth)
//				mfWidth = fMaxPos[0];
//			fMaxPos[1] = itText->second->mfPositionY + itText->second->mfHeight;
//			if (fMaxPos[1] > mfHeight)
//				mfHeight = fMaxPos[1];
			itText++;
		}

		// Line graph
		std::map<std::string, CGUILineGraph*>::iterator itLineGraph = _mmapLineGraphs.begin();
		while (itLineGraph != _mmapLineGraphs.end())
		{
			fMaxPos[0] = itLineGraph->second->mfPositionX + itLineGraph->second->mfWidth;
			if (fMaxPos[0] > mfWidth)
				mfWidth = fMaxPos[0];
			fMaxPos[1] = itLineGraph->second->mfPositionY + itLineGraph->second->mfHeight;
			if (fMaxPos[1] > mfHeight)
				mfHeight = fMaxPos[1];
			itLineGraph++;
		}

		// Progress bar
		std::map<std::string, CGUIProgressBar*>::iterator itProgressBar = _mmapProgressBars.begin();
		while (itProgressBar != _mmapProgressBars.end())
		{
			fMaxPos[0] = itProgressBar->second->mfPositionX + itProgressBar->second->mfWidth;
			if (fMaxPos[0] > mfWidth)
				mfWidth = fMaxPos[0];
			fMaxPos[1] = itProgressBar->second->mfPositionY + itProgressBar->second->mfHeight;
			if (fMaxPos[1] > mfHeight)
				mfHeight = fMaxPos[1];
			itProgressBar++;
		}

		// CImage
		std::map<std::string, CGUIImage*>::iterator itImage = _mmapImages.begin();
		while (itImage != _mmapImages.end())
		{
			fMaxPos[0] = itImage->second->mfPositionX + itImage->second->mfWidth;
			if (fMaxPos[0] > mfWidth)
				mfWidth = fMaxPos[0];
			fMaxPos[1] = itImage->second->mfPositionY + itImage->second->mfHeight;
			if (fMaxPos[1] > mfHeight)
				mfHeight = fMaxPos[1];
			itImage++;
		}

		// Animated image
		std::map<std::string, CGUIImageAnimated*>::iterator itImageAnimated = _mmapImageAnimateds.begin();
		while (itImageAnimated != _mmapImageAnimateds.end())
		{
			fMaxPos[0] = itImageAnimated->second->mfPositionX + itImageAnimated->second->mfWidth;
			if (fMaxPos[0] > mfWidth)
				mfWidth = fMaxPos[0];
			fMaxPos[1] = itImageAnimated->second->mfPositionY + itImageAnimated->second->mfHeight;
			if (fMaxPos[1] > mfHeight)
				mfHeight = fMaxPos[1];
			itImageAnimated++;
		}

		// Framebuffer
		std::map<std::string, CGUIImageFramebuffer*>::iterator itImageFB = _mmapImageFramebuffers.begin();
		while (itImageFB != _mmapImageFramebuffers.end())
		{
			fMaxPos[0] = itImageFB->second->mfPositionX + itImageFB->second->mfWidth;
			if (fMaxPos[0] > mfWidth)
				mfWidth = fMaxPos[0];
			fMaxPos[1] = itImageFB->second->mfPositionY + itImageFB->second->mfHeight;
			if (fMaxPos[1] > mfHeight)
				mfHeight = fMaxPos[1];
			itImageFB++;
		}

		// Depth buffer
		std::map<std::string, CGUIImageDepthbuffer*>::iterator itImageDB = _mmapImageDepthbuffers.begin();
		while (itImageDB != _mmapImageDepthbuffers.end())
		{
			fMaxPos[0] = itImageDB->second->mfPositionX + itImageDB->second->mfWidth;
			if (fMaxPos[0] > mfWidth)
				mfWidth = fMaxPos[0];
			fMaxPos[1] = itImageDB->second->mfPositionY + itImageDB->second->mfHeight;
			if (fMaxPos[1] > mfHeight)
				mfHeight = fMaxPos[1];
			itImageDB++;
		}
	}

	void CGUITooltip::setEnabled(bool bEnabled)
	{
		_mbEnabled = bEnabled;
	}

	bool CGUITooltip::getEnabled(void) const
	{
		return _mbEnabled;
	}

	CGUIText* CGUITooltip::addText(const std::string& strName, float fPosX, float fPosY, const std::string& strText)
	{
		// If resource already exists
		std::map<std::string, CGUIText*>::iterator it = _mmapTexts.find(strName);
		ThrowIfTrue(it != _mmapTexts.end(), "CGUITooltip::addText(" + strName + ") failed. The named object already exists.");
		CGUIText* pNewRes = new CGUIText;
		ThrowIfFalse(pNewRes, "CGUITooltip::addText(" + strName + ") failed. Could not allocate memory for the new object.");
		pNewRes->mfPositionX = fPosX;
		pNewRes->mfPositionY = fPosY;
		pNewRes->setText(strText);
		_mmapTexts[strName] = pNewRes;
		return pNewRes;
	}

	CGUIText* CGUITooltip::getText(const std::string& strName) const
	{
		std::map<std::string, CGUIText*>::iterator it = _mmapTexts.find(strName);
		ThrowIfTrue(it == _mmapTexts.end(), "CGUITooltip::getText(" + strName + ") failed. The named object doesn't exist.");
		return it->second;
	}

	void CGUITooltip::removeText(const std::string& strName)
	{
		std::map<std::string, CGUIText*>::iterator it = _mmapTexts.find(strName);
		if (it == _mmapTexts.end())
			return;
		delete it->second;
		_mmapTexts.erase(it);
	}

	void CGUITooltip::removeAllText(void)
	{
		std::map<std::string, CGUIText*>::iterator it = _mmapTexts.begin();
		while (it != _mmapTexts.end())
		{
			delete it->second;
			_mmapTexts.erase(it);
			it = _mmapTexts.begin();
		}
	}

	CGUILineGraph* CGUITooltip::addLineGraph(const std::string& strName, float fPosX, float fPosY, float fWidth, float fHeight)
	{
		// If resource already exists
		std::map<std::string, CGUILineGraph*>::iterator it = _mmapLineGraphs.find(strName);
		ThrowIfTrue(it != _mmapLineGraphs.end(), "CGUITooltip::addLineGraph(" + strName + ") failed. The named object already exists.");
		CGUILineGraph* pNewRes = new CGUILineGraph;
		ThrowIfFalse(pNewRes, "CGUITooltip::addLineGraph(" + strName + ") failed. Could not allocate memory for the new object.");
		pNewRes->mfPositionX = fPosX;
		pNewRes->mfPositionY = fPosY;
		pNewRes->mfWidth = fWidth;
		pNewRes->mfHeight = fHeight;
		_mmapLineGraphs[strName] = pNewRes;
		return pNewRes;
	}

	CGUILineGraph* CGUITooltip::getLineGraph(const std::string& strName) const
	{
		std::map<std::string, CGUILineGraph*>::iterator it = _mmapLineGraphs.find(strName);
		ThrowIfTrue(it == _mmapLineGraphs.end(), "CGUITooltip::getLineGraph(" + strName + ") failed. The named object doesn't exist.");
		return it->second;
	}

	void CGUITooltip::removeLineGraph(const std::string& strName)
	{
		std::map<std::string, CGUILineGraph*>::iterator it = _mmapLineGraphs.find(strName);
		if (it == _mmapLineGraphs.end())
			return;
		delete it->second;
		_mmapLineGraphs.erase(it);
	}

	void CGUITooltip::removeAllLineGraphs(void)
	{
		std::map<std::string, CGUILineGraph*>::iterator it = _mmapLineGraphs.begin();
		while (it != _mmapLineGraphs.end())
		{
			delete it->second;
			_mmapLineGraphs.erase(it);
			it = _mmapLineGraphs.begin();
		}
	}

	CGUIProgressBar* CGUITooltip::addProgressBar(const std::string& strName, float fPosX, float fPosY, float fWidth, float fHeight)
	{
		// If resource already exists
		std::map<std::string, CGUIProgressBar*>::iterator it = _mmapProgressBars.find(strName);
		ThrowIfTrue(it != _mmapProgressBars.end(), "CGUITooltip::addProgressBar(" + strName + ") failed. The named object already exists.");
		CGUIProgressBar* pNewRes = new CGUIProgressBar;
		ThrowIfFalse(pNewRes, "CGUITooltip::addProgressBar(" + strName + ") failed. Could not allocate memory for the new object.");
		pNewRes->mfPositionX = fPosX;
		pNewRes->mfPositionY = fPosY;
		pNewRes->mfWidth = fWidth;
		pNewRes->mfHeight = fHeight;
		_mmapProgressBars[strName] = pNewRes;
		return pNewRes;
	}

	CGUIProgressBar* CGUITooltip::getProgressBar(const std::string& strName) const
	{
		std::map<std::string, CGUIProgressBar*>::iterator it = _mmapProgressBars.find(strName);
		ThrowIfTrue(it == _mmapProgressBars.end(), "CGUITooltip::getProgressBar(" + strName + ") failed. The named object doesn't exist.");
		return it->second;
	}

	void CGUITooltip::removeProgressBar(const std::string& strName)
	{
		std::map<std::string, CGUIProgressBar*>::iterator it = _mmapProgressBars.find(strName);
		if (it == _mmapProgressBars.end())
			return;
		delete it->second;
		_mmapProgressBars.erase(it);
	}

	void CGUITooltip::removeAllProgressBars(void)
	{
		std::map<std::string, CGUIProgressBar*>::iterator it = _mmapProgressBars.begin();
		while (it != _mmapProgressBars.end())
		{
			delete it->second;
			_mmapProgressBars.erase(it);
			it = _mmapProgressBars.begin();
		}
	}

	CGUIImage* CGUITooltip::addImage(const std::string& strName, float fPosX, float fPosY, const std::string& strImageFilename, float fWidth, float fHeight)
	{
		// If resource already exists
		std::map<std::string, CGUIImage*>::iterator it = _mmapImages.find(strName);
		ThrowIfTrue(it != _mmapImages.end(), "CGUITooltip::addImage(" + strName + ") failed. The named object already exists.");
		CGUIImage* pNewRes = new CGUIImage;
		ThrowIfFalse(pNewRes, "CGUITooltip::addImage(" + strName + ") failed. Could not allocate memory for the new object.");
		pNewRes->mfPositionX = fPosX;
		pNewRes->mfPositionY = fPosY;
		pNewRes->mfWidth = fWidth;
		pNewRes->mfHeight = fHeight;
		pNewRes->_mstrTextureResourceName = strImageFilename;
		_mmapImages[strName] = pNewRes;

		// Add strImageFilename to the resource manager
		CResourceTexture2DFromFile* pTex = x->pResource->addTexture2DFromFile(strImageFilename, strImageFilename);

		// If a value less than 0 is passed to width/height, set widget to dims of the image
		if (fWidth < 0)
			pNewRes->mfWidth = pTex->getDimensions().x;
		if (fHeight < 0)
			pNewRes->mfHeight = pTex->getDimensions().y;

		return pNewRes;
	}

	CGUIImage* CGUITooltip::getImage(const std::string& strName) const
	{
		std::map<std::string, CGUIImage*>::iterator it = _mmapImages.find(strName);
		ThrowIfTrue(it == _mmapImages.end(), "CGUITooltip::getImage(" + strName + ") failed. The named object doesn't exist.");
		return it->second;
	}

	void CGUITooltip::removeImage(const std::string& strName)
	{
		std::map<std::string, CGUIImage*>::iterator it = _mmapImages.find(strName);
		if (it == _mmapImages.end())
			return;

		// Remove _mstrTextureResourceName from the resource manager
		if (it->second->_mbImageIsFromFile)
		{
			x->pResource->removeTexture2DFromFile(it->second->_mstrTextureResourceName);
		}
		
		delete it->second;
		_mmapImages.erase(it);
	}

	void CGUITooltip::removeAllImages(void)
	{
		std::map<std::string, CGUIImage*>::iterator it = _mmapImages.begin();
		while (it != _mmapImages.end())
		{
			// Remove _mstrTextureResourceName from the resource manager
			if (it->second->_mbImageIsFromFile)
			{
				x->pResource->removeTexture2DFromFile(it->second->_mstrTextureResourceName);
			}
			delete it->second;
			_mmapImages.erase(it);
			it = _mmapImages.begin();
		}
	}

	CGUIImageAnimated* CGUITooltip::addImageAnimated(const std::string& strName, float fPosX, float fPosY, const std::vector<std::string>& vecStrImageFilenames, float fWidth, float fHeight)
	{
		// If resource already exists
		std::map<std::string, CGUIImageAnimated*>::iterator it = _mmapImageAnimateds.find(strName);
		ThrowIfTrue(it != _mmapImageAnimateds.end(), "CGUITooltip::addImageAnimated(" + strName + ") failed. The named object already exists.");
		CGUIImageAnimated* pNewRes = new CGUIImageAnimated;
		ThrowIfFalse(pNewRes, "CGUITooltip::addImageAnimated(" + strName + ") failed. Could not allocate memory for the new object.");
		pNewRes->mfPositionX = fPosX;
		pNewRes->mfPositionY = fPosY;
		pNewRes->mfWidth = fWidth;
		pNewRes->mfHeight = fHeight;
		pNewRes->_mstrResourceTexture2DAtlasName = strName;
		_mmapImageAnimateds[strName] = pNewRes;

		// Add CResourceTexture2DAtlas to the resource manager
		CResourceTexture2DAtlas* pTex = x->pResource->addTexture2DAtlas(strName, vecStrImageFilenames, false, 1);

		// If a value less than 0 is passed to width/height, set widget to dims of the image
		if (fWidth < 0)
			pNewRes->mfWidth = (float)pTex->getImageDims(0).x;
		if (fHeight < 0)
			pNewRes->mfHeight = (float)pTex->getImageDims(0).y;

		return pNewRes;
	}

	CGUIImageAnimated* CGUITooltip::getImageAnimated(const std::string& strName) const
	{
		std::map<std::string, CGUIImageAnimated*>::iterator it = _mmapImageAnimateds.find(strName);
		ThrowIfTrue(it == _mmapImageAnimateds.end(), "CGUITooltip::getImageAnimated(" + strName + ") failed. The named object doesn't exist.");
		return it->second;
	}

	void CGUITooltip::removeImageAnimated(const std::string& strName)
	{
		std::map<std::string, CGUIImageAnimated*>::iterator it = _mmapImageAnimateds.find(strName);
		if (it == _mmapImageAnimateds.end())
			return;

		// Remove CResourceTexture2DAtlas from the resource manager
		x->pResource->removeTexture2DAtlas(it->second->_mstrResourceTexture2DAtlasName);

		delete it->second;
		_mmapImageAnimateds.erase(it);
	}

	void CGUITooltip::removeAllImagesAnimated(void)
	{
		std::map<std::string, CGUIImageAnimated*>::iterator it = _mmapImageAnimateds.begin();
		while (it != _mmapImageAnimateds.end())
		{
			delete it->second;
			_mmapImageAnimateds.erase(it);
			it = _mmapImageAnimateds.begin();
		}
	}

	CGUIImageFramebuffer* CGUITooltip::addImageFramebuffer(const std::string& strName, float fPosX, float fPosY, const std::string& strFBname, float fWidth, float fHeight)
	{
		// If resource already exists
		std::map<std::string, CGUIImageFramebuffer*>::iterator it = _mmapImageFramebuffers.find(strName);
		ThrowIfTrue(it != _mmapImageFramebuffers.end(), "CGUITooltip::addImageFramebuffer(" + strName + ") failed. The named object already exists.");
		CGUIImageFramebuffer* pNewRes = new CGUIImageFramebuffer;
		ThrowIfFalse(pNewRes, "CGUITooltip::addImageFramebuffer(" + strName + ") failed. Could not allocate memory for the new object.");
		pNewRes->mfPositionX = fPosX;
		pNewRes->mfPositionY = fPosY;
		pNewRes->mfWidth = fWidth;
		pNewRes->mfHeight = fHeight;
		pNewRes->_mstrFBname = strFBname;
		_mmapImageFramebuffers[strName] = pNewRes;

		// Get CResourceFramebuffer from the resource manager to set size of this object
		CResourceFramebuffer* pFB = x->pResource->getFramebuffer(strFBname);

		// If a value less than 0 is passed to width/height, set widget to dims of the image
		if (fWidth < 0)
			pNewRes->mfWidth = (float)pFB->getWidth();
		if (fHeight < 0)
			pNewRes->mfHeight = (float)pFB->getHeight();

		return pNewRes;
	}

	CGUIImageFramebuffer* CGUITooltip::getImageFramebuffer(const std::string& strName) const
	{
		std::map<std::string, CGUIImageFramebuffer*>::iterator it = _mmapImageFramebuffers.find(strName);
		ThrowIfTrue(it == _mmapImageFramebuffers.end(), "CGUITooltip::getImageFramebuffer(" + strName + ") failed. The named object doesn't exist.");
		return it->second;
	}

	void CGUITooltip::removeImageFramebuffer(const std::string& strName)
	{
		std::map<std::string, CGUIImageFramebuffer*>::iterator it = _mmapImageFramebuffers.find(strName);
		if (it == _mmapImageFramebuffers.end())
			return;

		delete it->second;
		_mmapImageFramebuffers.erase(it);
	}

	void CGUITooltip::removeAllImageFramebuffers(void)
	{
		std::map<std::string, CGUIImageFramebuffer*>::iterator it = _mmapImageFramebuffers.begin();
		while (it != _mmapImageFramebuffers.end())
		{
			delete it->second;
			_mmapImageFramebuffers.erase(it);
			it = _mmapImageFramebuffers.begin();
		}
	}

	CGUIImageDepthbuffer* CGUITooltip::addImageDepthbuffer(const std::string& strName, float fPosX, float fPosY, const std::string& strDBname, float fWidth, float fHeight)
	{
		// If resource already exists
		std::map<std::string, CGUIImageDepthbuffer*>::iterator it = _mmapImageDepthbuffers.find(strName);
		ThrowIfTrue(it != _mmapImageDepthbuffers.end(), "CGUITooltip::addImageDepthbuffer(" + strName + ") failed. The named object already exists.");
		CGUIImageDepthbuffer* pNewRes = new CGUIImageDepthbuffer;
		ThrowIfFalse(pNewRes, "CGUITooltip::addImageDepthbuffer(" + strName + ") failed. Could not allocate memory for the new object.");
		pNewRes->mfPositionX = fPosX;
		pNewRes->mfPositionY = fPosY;
		pNewRes->mfWidth = fWidth;
		pNewRes->mfHeight = fHeight;
		pNewRes->_mstrDBname = strDBname;
		_mmapImageDepthbuffers[strName] = pNewRes;

		// Get CResourceFramebuffer from the resource manager to set size of this object
		CResourceDepthbuffer* pDB = x->pResource->getDepthbuffer(strDBname);

		// If a value less than 0 is passed to width/height, set widget to dims of the image
		if (fWidth < 0)
			pNewRes->mfWidth = (float)pDB->getWidth();
		if (fHeight < 0)
			pNewRes->mfHeight = (float)pDB->getHeight();

		return pNewRes;
	}

	CGUIImageDepthbuffer* CGUITooltip::getImageDepthbuffer(const std::string& strName) const
	{
		std::map<std::string, CGUIImageDepthbuffer*>::iterator it = _mmapImageDepthbuffers.find(strName);
		ThrowIfTrue(it == _mmapImageDepthbuffers.end(), "CGUITooltip::getImageDepthbuffer(" + strName + ") failed. The named object doesn't exist.");
		return it->second;
	}

	void CGUITooltip::removeImageDepthbuffer(const std::string& strName)
	{
		std::map<std::string, CGUIImageDepthbuffer*>::iterator it = _mmapImageDepthbuffers.find(strName);
		if (it == _mmapImageDepthbuffers.end())
			return;

		delete it->second;
		_mmapImageDepthbuffers.erase(it);
	}

	void CGUITooltip::removeAllImageDepthbuffers(void)
	{
		std::map<std::string, CGUIImageDepthbuffer*>::iterator it = _mmapImageDepthbuffers.begin();
		while (it != _mmapImageDepthbuffers.end())
		{
			delete it->second;
			_mmapImageDepthbuffers.erase(it);
			it = _mmapImageDepthbuffers.begin();
		}
	}

	void CGUITooltip::removeAll(void)
	{
		removeAllText();
		removeAllLineGraphs();
		removeAllProgressBars();
		removeAllImages();
		removeAllImagesAnimated();
		removeAllImageFramebuffers();
		removeAllImageDepthbuffers();
	}

	void CGUITooltip::setAsText(const std::string& strText)
	{
		removeAll();
		_mbEnabled = true;
		addText("Text", 0, 0, strText);
	}

	void CGUITooltip::resetFade(void)
	{
		_mfTooltipDelay = 0.0f;	// Reset delay
		_mColour.alpha = 0.0f;	// Reset fade alpha
	}
}