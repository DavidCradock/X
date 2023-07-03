#include "PCH.h"
#include "GUITooltip.h"
#include "log.h"
#include "resourceManager.h"
#include "GUIManager.h"
#include "input.h"

#include "window.h"

namespace X
{
	GUITooltip::GUITooltip()
	{
		_mbEnabled = false;
		_mColour.set(1.0f, 1.0f, 1.0f, 0.0f);
		_mfTooltipDelay = 0.0f;
	}

	GUITooltip::~GUITooltip()
	{

	}

	void GUITooltip::render(void* pParentContainer, const std::string& strFramebufferToSampleFrom)
	{
		// If colour alpha is zero, do not render
		if (_mColour.alpha <= 0.0f)
			return;

		// Get pointers to stuff we need
		ResourceManager* pRM = ResourceManager::getPointer();
		GUIContainer* pContainer = (GUIContainer*)pParentContainer;	// Needed to get theme
		GUIManager* pGUI = GUIManager::getPointer();
		GUITheme* pTheme = pGUI->getTheme(pContainer->mstrThemename);
		ResourceFramebuffer* pBGFB = pRM->getFramebuffer("X:backbuffer_FB");
		ResourceFramebuffer* pFB = pRM->getFramebuffer("X:guitooltipFB");
		ResourceTexture2D* pTexColour = pRM->getTexture2D(pTheme->mImages.tooltipBGColour);
		glm::vec2 vTexDimsPoint3 = pTexColour->mvDimensions * 0.3333333f;
		glm::vec2 vTexDimsPoint6 = pTexColour->mvDimensions * 0.6666666f;

		// Tooltip framebuffer: set size if needed, clear it and set as render target
		// We need to make the framebuffer bigger than just the mfWidth and mfHeight, as we need to accomodate borders
		pFB->resize(unsigned int(mfWidth + vTexDimsPoint6.x), unsigned int(mfHeight + vTexDimsPoint6.y));
		pFB->bindAsRenderTarget(true, false);

		// Render the background of the tooltip
		ResourceTriangle* pTri = pRM->getTriangle("X:gui");
		ResourceShader* pShader = pRM->getShader("X:pos_col_tex");
		pShader->bind();

		// Setup the projection matrix as orthographic
		glm::mat4 matProjection = glm::ortho(0.0f, float(pFB->getWidth()), float(pFB->getHeight()), 0.0f, -1.0f, 1.0f);
		pShader->setMat4("transform", matProjection);

		// Tell OpenGL, for each sampler, to which texture unit it belongs to
		pShader->setInt("texture0", 0);

		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);

		// Bind textures
		pTexColour->bind(0);

		// Render the centre
		glm::vec2 vPos = glm::vec2(0.0f, 0.0f);
		GUIColour colWhite;
		pTri->removeGeom();
		pTri->addQuad2D(vTexDimsPoint3,		// Position
			glm::vec2(mfWidth, mfHeight),	// Dimensions
			colWhite.get(),
			mTC.centre.BL,
			mTC.centre.BR,
			mTC.centre.TR,
			mTC.centre.TL);

		// Render the left edge
		pTri->addQuad2D(
			glm::vec2(0.0f,							// Position X
				vTexDimsPoint3.y),					// Position Y
			glm::vec2(vTexDimsPoint3.x, mfHeight),	// Dimensions
			colWhite.get(),
			mTC.left.BL,
			mTC.left.BR,
			mTC.left.TR,
			mTC.left.TL);

		// Render the right edge
		pTri->addQuad2D(
			glm::vec2(mfWidth + vTexDimsPoint3.x,	// Position X
				vTexDimsPoint3.y),					// Position Y
			glm::vec2(vTexDimsPoint3.x, mfHeight),	// Dimensions
			colWhite.get(),
			mTC.right.BL,
			mTC.right.BR,
			mTC.right.TR,
			mTC.right.TL);

		// Render the top edge
		pTri->addQuad2D(
			glm::vec2(
				vTexDimsPoint3.x,					// Position X
				0.0f),								// Position Y
			glm::vec2(mfWidth, vTexDimsPoint3.y),	// Dimensions
			colWhite.get(),
			mTC.top.BL,
			mTC.top.BR,
			mTC.top.TR,
			mTC.top.TL);

		// Render the bottom edge
		pTri->addQuad2D(
			glm::vec2(
				vTexDimsPoint3.x,					// Position X
				mfHeight + vTexDimsPoint3.y),		// Position Y
			glm::vec2(mfWidth, vTexDimsPoint3.y),	// Dimensions
			colWhite.get(),
			mTC.bottom.BL,
			mTC.bottom.BR,
			mTC.bottom.TR,
			mTC.bottom.TL);

		// Render the top left corner
		pTri->addQuad2D(
			glm::vec2(
				0.0f,										// Position X
				0.0f),										// Position Y
			glm::vec2(vTexDimsPoint3.x, vTexDimsPoint3.y),	// Dimensions
			colWhite.get(),
			mTC.topLeft.BL,
			mTC.topLeft.BR,
			mTC.topLeft.TR,
			mTC.topLeft.TL);

		// Render the top right corner
		pTri->addQuad2D(
			glm::vec2(
				mfWidth + vTexDimsPoint3.x,					// Position X
				0.0f),										// Position Y
			glm::vec2(vTexDimsPoint3.x, vTexDimsPoint3.y),	// Dimensions
			colWhite.get(),
			mTC.topRight.BL,
			mTC.topRight.BR,
			mTC.topRight.TR,
			mTC.topRight.TL);

		// Render the bottom left corner
		pTri->addQuad2D(
			glm::vec2(
				0.0f,										// Position X
				mfHeight + vTexDimsPoint3.y),				// Position Y
			glm::vec2(vTexDimsPoint3.x, vTexDimsPoint3.y),	// Dimensions
			colWhite.get(),
			mTC.bottomLeft.BL,
			mTC.bottomLeft.BR,
			mTC.bottomLeft.TR,
			mTC.bottomLeft.TL);

		// Render the bottom right corner
		pTri->addQuad2D(
			glm::vec2(
				mfWidth + vTexDimsPoint3.x,					// Position X
				mfHeight + vTexDimsPoint3.y),				// Position Y
			glm::vec2(vTexDimsPoint3.x, vTexDimsPoint3.y),	// Dimensions
			colWhite.get(),
			mTC.bottomRight.BL,
			mTC.bottomRight.BR,
			mTC.bottomRight.TR,
			mTC.bottomRight.TL);

		pTri->update();
		pTri->draw();

		pTexColour->unbindAll();	// Unbind textures
		pShader->unbind();	// Unbind the GUI shader

		// Render each text
		std::map<std::string, GUIText*>::iterator itText = _mmapTexts.begin();
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
		// ResourceFont turns these off
		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);

		// Render each line graph
		std::map<std::string, GUILineGraph*>::iterator itLineGraph = _mmapLineGraphs.begin();
		while (itLineGraph != _mmapLineGraphs.end())
		{
//			itLineGraph->second->render(pParentContainer, strFramebufferToSampleFrom);
			itLineGraph++;
		}

		// Render each progress bar
		std::map<std::string, GUIProgressBar*>::iterator itProgressBar = _mmapProgressBars.begin();
		while (itProgressBar != _mmapProgressBars.end())
		{
//			itProgressBar->second->render(pParentContainer, strFramebufferToSampleFrom);
			itProgressBar++;
		}

		// Render each image
		std::map<std::string, GUIImage*>::iterator itImage = _mmapImages.begin();
		while (itImage != _mmapImages.end())
		{
//			itImage->second->render(pParentContainer, strFramebufferToSampleFrom);
			itImage++;
		}

		// Render each animated image
		std::map<std::string, GUIImageAnimated*>::iterator itImageAnimated = _mmapImageAnimateds.begin();
		while (itImageAnimated != _mmapImageAnimateds.end())
		{
//			itImageAnimated->second->render(pParentContainer, strFramebufferToSampleFrom);
			itImageAnimated++;
		}

		// Images framebuffer
		std::map<std::string, GUIImageFramebuffer*>::iterator itImageFB = _mmapImageFramebuffers.begin();
		while (itImageFB != _mmapImageFramebuffers.end())
		{
//			itImageFB->second->render(pParentContainer, strFramebufferToSampleFrom);
			itImageFB++;
		}

		// Images depth buffer
		std::map<std::string, GUIImageDepthbuffer*>::iterator itImageDB = _mmapImageDepthbuffers.begin();
		while (itImageDB != _mmapImageDepthbuffers.end())
		{
//			itImageDB->second->render(pParentContainer, strFramebufferToSampleFrom);
			itImageDB++;
		}

		// Unbind framebuffer which now holds all objects and render it
		pFB->unbindAsRenderTarget();

		// Rebind the GUI framebuffer as render target
		pBGFB->bindAsRenderTarget(false, false);

//		glEnable(GL_BLEND);
//		glDisable(GL_DEPTH_TEST);
//		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		pFB->renderTo2DQuad(unsigned int(mfPositionX + vTexDimsPoint3.x), unsigned int(mfPositionY), unsigned int(pFB->getWidth()), unsigned int(pFB->getHeight()), _mColour);
		glDisable(GL_BLEND);
	}

	void GUITooltip::update(void* pParentContainer, GUIBaseObject* pGUIOwner, bool bMouseOverOwner)
	{
		// Set position of tooltip
		InputManager* pInput = InputManager::getPointer();
		GUIManager* pGUIManager = GUIManager::getPointer();
		glm::vec2 vMousePos = pInput->mouse.getCursorPos();
		glm::vec2 vTooltipOffset = pGUIManager->getTooltipOffset();
		mfPositionX = vMousePos.x + vTooltipOffset.x;
		mfPositionY = vMousePos.y + vTooltipOffset.y;

		// Deal with fading in and out
		GUIContainer* pContainer = (GUIContainer*)pParentContainer;
		GUITheme* pTheme = pGUIManager->getTheme(pContainer->mstrThemename);
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
				if (_mfTooltipDelay >= pGUIManager->getTooltipDelay())
				{
					_mfTooltipDelay = pGUIManager->getTooltipDelay();
					_mColour.alpha += fSecondsPast * pTheme->mfTooltipFadeSpeed;
					clamp(_mColour.alpha, 0.0f, 1.0f);
				}
			}
		}

		// Set dimensions of this tooltip so that all objects fit inside of it
		mfWidth = 1;
		mfHeight = 1;
		// Text
		std::map<std::string, GUIText*>::iterator itText = _mmapTexts.begin();
		float fMaxPos[2];
		ResourceManager* pRM = ResourceManager::getPointer();
		ResourceFont* pFont = pRM->getFont(pTheme->mFonts.text);
		while (itText != _mmapTexts.end())
		{
			// We need to set the dimensions of the text object using the current theme
			itText->second->mfWidth = pFont->getTextWidth(itText->second->mstrText);
			itText->second->mfHeight = pFont->getTextHeight(1.0f);

			fMaxPos[0] = itText->second->mfPositionX + itText->second->mfWidth;
			if (fMaxPos[0] > mfWidth)
				mfWidth = fMaxPos[0];
			fMaxPos[1] = itText->second->mfPositionY + itText->second->mfHeight;
			if (fMaxPos[1] > mfHeight)
				mfHeight = fMaxPos[1];
			itText++;
		}

		// Line graph
		std::map<std::string, GUILineGraph*>::iterator itLineGraph = _mmapLineGraphs.begin();
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
		std::map<std::string, GUIProgressBar*>::iterator itProgressBar = _mmapProgressBars.begin();
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

		// Image
		std::map<std::string, GUIImage*>::iterator itImage = _mmapImages.begin();
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
		std::map<std::string, GUIImageAnimated*>::iterator itImageAnimated = _mmapImageAnimateds.begin();
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
		std::map<std::string, GUIImageFramebuffer*>::iterator itImageFB = _mmapImageFramebuffers.begin();
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
		std::map<std::string, GUIImageDepthbuffer*>::iterator itImageDB = _mmapImageDepthbuffers.begin();
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

	void GUITooltip::setEnabled(bool bEnabled)
	{
		_mbEnabled = bEnabled;
	}

	bool GUITooltip::getEnabled(void)
	{
		return _mbEnabled;
	}

	GUIText* GUITooltip::addText(const std::string& strName, float fPosX, float fPosY, const std::string& strText)
	{
		// If resource already exists
		std::map<std::string, GUIText*>::iterator it = _mmapTexts.find(strName);
		ThrowIfTrue(it != _mmapTexts.end(), "GUITooltip::addText(" + strName + ") failed. The named object already exists.");
		GUIText* pNewRes = new GUIText;
		ThrowIfFalse(pNewRes, "GUITooltip::addText(" + strName + ") failed. Could not allocate memory for the new object.");
		pNewRes->mfPositionX = fPosX;
		pNewRes->mfPositionY = fPosY;
		pNewRes->mstrText = strText;
		_mmapTexts[strName] = pNewRes;
		return pNewRes;
	}

	GUIText* GUITooltip::getText(const std::string& strName)
	{
		std::map<std::string, GUIText*>::iterator it = _mmapTexts.find(strName);
		ThrowIfTrue(it == _mmapTexts.end(), "GUITooltip::getText(" + strName + ") failed. The named object doesn't exist.");
		return it->second;
	}

	void GUITooltip::removeText(const std::string& strName)
	{
		std::map<std::string, GUIText*>::iterator it = _mmapTexts.find(strName);
		if (it == _mmapTexts.end())
			return;
		delete it->second;
		_mmapTexts.erase(it);
	}

	void GUITooltip::removeAllText(void)
	{
		std::map<std::string, GUIText*>::iterator it = _mmapTexts.begin();
		while (it != _mmapTexts.end())
		{
			delete it->second;
			_mmapTexts.erase(it);
			it = _mmapTexts.begin();
		}
	}

	GUILineGraph* GUITooltip::addLineGraph(const std::string& strName, float fPosX, float fPosY, float fWidth, float fHeight)
	{
		// If resource already exists
		std::map<std::string, GUILineGraph*>::iterator it = _mmapLineGraphs.find(strName);
		ThrowIfTrue(it != _mmapLineGraphs.end(), "GUITooltip::addLineGraph(" + strName + ") failed. The named object already exists.");
		GUILineGraph* pNewRes = new GUILineGraph;
		ThrowIfFalse(pNewRes, "GUITooltip::addLineGraph(" + strName + ") failed. Could not allocate memory for the new object.");
		pNewRes->mfPositionX = fPosX;
		pNewRes->mfPositionY = fPosY;
		pNewRes->mfWidth = fWidth;
		pNewRes->mfHeight = fHeight;
		_mmapLineGraphs[strName] = pNewRes;
		return pNewRes;
	}

	GUILineGraph* GUITooltip::getLineGraph(const std::string& strName)
	{
		std::map<std::string, GUILineGraph*>::iterator it = _mmapLineGraphs.find(strName);
		ThrowIfTrue(it == _mmapLineGraphs.end(), "GUITooltip::getLineGraph(" + strName + ") failed. The named object doesn't exist.");
		return it->second;
	}

	void GUITooltip::removeLineGraph(const std::string& strName)
	{
		std::map<std::string, GUILineGraph*>::iterator it = _mmapLineGraphs.find(strName);
		if (it == _mmapLineGraphs.end())
			return;
		delete it->second;
		_mmapLineGraphs.erase(it);
	}

	void GUITooltip::removeAllLineGraphs(void)
	{
		std::map<std::string, GUILineGraph*>::iterator it = _mmapLineGraphs.begin();
		while (it != _mmapLineGraphs.end())
		{
			delete it->second;
			_mmapLineGraphs.erase(it);
			it = _mmapLineGraphs.begin();
		}
	}

	GUIProgressBar* GUITooltip::addProgressBar(const std::string& strName, float fPosX, float fPosY, float fWidth, float fHeight)
	{
		// If resource already exists
		std::map<std::string, GUIProgressBar*>::iterator it = _mmapProgressBars.find(strName);
		ThrowIfTrue(it != _mmapProgressBars.end(), "GUITooltip::addProgressBar(" + strName + ") failed. The named object already exists.");
		GUIProgressBar* pNewRes = new GUIProgressBar;
		ThrowIfFalse(pNewRes, "GUITooltip::addProgressBar(" + strName + ") failed. Could not allocate memory for the new object.");
		pNewRes->mfPositionX = fPosX;
		pNewRes->mfPositionY = fPosY;
		pNewRes->mfWidth = fWidth;
		pNewRes->mfHeight = fHeight;
		_mmapProgressBars[strName] = pNewRes;
		return pNewRes;
	}

	GUIProgressBar* GUITooltip::getProgressBar(const std::string& strName)
	{
		std::map<std::string, GUIProgressBar*>::iterator it = _mmapProgressBars.find(strName);
		ThrowIfTrue(it == _mmapProgressBars.end(), "GUITooltip::getProgressBar(" + strName + ") failed. The named object doesn't exist.");
		return it->second;
	}

	void GUITooltip::removeProgressBar(const std::string& strName)
	{
		std::map<std::string, GUIProgressBar*>::iterator it = _mmapProgressBars.find(strName);
		if (it == _mmapProgressBars.end())
			return;
		delete it->second;
		_mmapProgressBars.erase(it);
	}

	void GUITooltip::removeAllProgressBars(void)
	{
		std::map<std::string, GUIProgressBar*>::iterator it = _mmapProgressBars.begin();
		while (it != _mmapProgressBars.end())
		{
			delete it->second;
			_mmapProgressBars.erase(it);
			it = _mmapProgressBars.begin();
		}
	}

	GUIImage* GUITooltip::addImage(const std::string& strName, float fPosX, float fPosY, const std::string& strImageFilename, float fWidth, float fHeight)
	{
		// If resource already exists
		std::map<std::string, GUIImage*>::iterator it = _mmapImages.find(strName);
		ThrowIfTrue(it != _mmapImages.end(), "GUITooltip::addImage(" + strName + ") failed. The named object already exists.");
		GUIImage* pNewRes = new GUIImage;
		ThrowIfFalse(pNewRes, "GUITooltip::addImage(" + strName + ") failed. Could not allocate memory for the new object.");
		pNewRes->mfPositionX = fPosX;
		pNewRes->mfPositionY = fPosY;
		pNewRes->mfWidth = fWidth;
		pNewRes->mfHeight = fHeight;
		pNewRes->_mstrTexturename = strImageFilename;
		_mmapImages[strName] = pNewRes;

		// Add strImageFilename to the resource manager
		ResourceManager* pResMan = ResourceManager::getPointer();
		ResourceTexture2D* pTex = pResMan->addTexture2D(strImageFilename, strImageFilename);

		// If a value less than 0 is passed to width/height, set widget to dims of the image
		if (fWidth < 0)
			pNewRes->mfWidth = pTex->mvDimensions.x;
		if (fHeight < 0)
			pNewRes->mfHeight = pTex->mvDimensions.y;

		return pNewRes;
	}

	GUIImage* GUITooltip::getImage(const std::string& strName)
	{
		std::map<std::string, GUIImage*>::iterator it = _mmapImages.find(strName);
		ThrowIfTrue(it == _mmapImages.end(), "GUITooltip::getImage(" + strName + ") failed. The named object doesn't exist.");
		return it->second;
	}

	void GUITooltip::removeImage(const std::string& strName)
	{
		std::map<std::string, GUIImage*>::iterator it = _mmapImages.find(strName);
		if (it == _mmapImages.end())
			return;

		// Remove _mstrImageFilename from the resource manager
		ResourceManager* pResMan = ResourceManager::getPointer();
		pResMan->removeTexture2D(it->second->_mstrTexturename);

		delete it->second;
		_mmapImages.erase(it);
	}

	void GUITooltip::removeAllImages(void)
	{
		std::map<std::string, GUIImage*>::iterator it = _mmapImages.begin();
		while (it != _mmapImages.end())
		{
			delete it->second;
			_mmapImages.erase(it);
			it = _mmapImages.begin();
		}
	}

	GUIImageAnimated* GUITooltip::addImageAnimated(const std::string& strName, float fPosX, float fPosY, const std::vector<std::string>& vecStrImageFilenames, float fWidth, float fHeight)
	{
		// If resource already exists
		std::map<std::string, GUIImageAnimated*>::iterator it = _mmapImageAnimateds.find(strName);
		ThrowIfTrue(it != _mmapImageAnimateds.end(), "GUITooltip::addImageAnimated(" + strName + ") failed. The named object already exists.");
		GUIImageAnimated* pNewRes = new GUIImageAnimated;
		ThrowIfFalse(pNewRes, "GUITooltip::addImageAnimated(" + strName + ") failed. Could not allocate memory for the new object.");
		pNewRes->mfPositionX = fPosX;
		pNewRes->mfPositionY = fPosY;
		pNewRes->mfWidth = fWidth;
		pNewRes->mfHeight = fHeight;
		pNewRes->_mstrResourceTexture2DAnimationName = strName;
		_mmapImageAnimateds[strName] = pNewRes;

		// Add ResourceTexture2DAnimation to the resource manager
		ResourceManager* pResMan = ResourceManager::getPointer();
		ResourceTexture2DAnimation* pTex = pResMan->addTexture2DAnimation(strName, vecStrImageFilenames);

		// If a value less than 0 is passed to width/height, set widget to dims of the image
		if (fWidth < 0)
			pNewRes->mfWidth = pTex->mvDimensions.x;
		if (fHeight < 0)
			pNewRes->mfHeight = pTex->mvDimensions.y;

		return pNewRes;
	}

	GUIImageAnimated* GUITooltip::getImageAnimated(const std::string& strName)
	{
		std::map<std::string, GUIImageAnimated*>::iterator it = _mmapImageAnimateds.find(strName);
		ThrowIfTrue(it == _mmapImageAnimateds.end(), "GUITooltip::getImageAnimated(" + strName + ") failed. The named object doesn't exist.");
		return it->second;
	}

	void GUITooltip::removeImageAnimated(const std::string& strName)
	{
		std::map<std::string, GUIImageAnimated*>::iterator it = _mmapImageAnimateds.find(strName);
		if (it == _mmapImageAnimateds.end())
			return;

		// Remove ResourceTexture2DAnimation from the resource manager
		ResourceManager* pResMan = ResourceManager::getPointer();
		pResMan->removeTexture2DAnimation(it->second->_mstrResourceTexture2DAnimationName);

		delete it->second;
		_mmapImageAnimateds.erase(it);
	}

	void GUITooltip::removeAllImagesAnimated(void)
	{
		std::map<std::string, GUIImageAnimated*>::iterator it = _mmapImageAnimateds.begin();
		while (it != _mmapImageAnimateds.end())
		{
			delete it->second;
			_mmapImageAnimateds.erase(it);
			it = _mmapImageAnimateds.begin();
		}
	}

	GUIImageFramebuffer* GUITooltip::addImageFramebuffer(const std::string& strName, float fPosX, float fPosY, const std::string& strFBname, float fWidth, float fHeight)
	{
		// If resource already exists
		std::map<std::string, GUIImageFramebuffer*>::iterator it = _mmapImageFramebuffers.find(strName);
		ThrowIfTrue(it != _mmapImageFramebuffers.end(), "GUITooltip::addImageFramebuffer(" + strName + ") failed. The named object already exists.");
		GUIImageFramebuffer* pNewRes = new GUIImageFramebuffer;
		ThrowIfFalse(pNewRes, "GUITooltip::addImageFramebuffer(" + strName + ") failed. Could not allocate memory for the new object.");
		pNewRes->mfPositionX = fPosX;
		pNewRes->mfPositionY = fPosY;
		pNewRes->mfWidth = fWidth;
		pNewRes->mfHeight = fHeight;
		pNewRes->_mstrFBname = strFBname;
		_mmapImageFramebuffers[strName] = pNewRes;

		// Get ResourceFramebuffer from the resource manager to set size of this object
		ResourceManager* pResMan = ResourceManager::getPointer();
		ResourceFramebuffer* pFB = pResMan->getFramebuffer(strFBname);

		// If a value less than 0 is passed to width/height, set widget to dims of the image
		if (fWidth < 0)
			pNewRes->mfWidth = (float)pFB->getWidth();
		if (fHeight < 0)
			pNewRes->mfHeight = (float)pFB->getHeight();

		return pNewRes;
	}

	GUIImageFramebuffer* GUITooltip::getImageFramebuffer(const std::string& strName)
	{
		std::map<std::string, GUIImageFramebuffer*>::iterator it = _mmapImageFramebuffers.find(strName);
		ThrowIfTrue(it == _mmapImageFramebuffers.end(), "GUITooltip::getImageFramebuffer(" + strName + ") failed. The named object doesn't exist.");
		return it->second;
	}

	void GUITooltip::removeImageFramebuffer(const std::string& strName)
	{
		std::map<std::string, GUIImageFramebuffer*>::iterator it = _mmapImageFramebuffers.find(strName);
		if (it == _mmapImageFramebuffers.end())
			return;

		delete it->second;
		_mmapImageFramebuffers.erase(it);
	}

	void GUITooltip::removeAllImageFramebuffers(void)
	{
		std::map<std::string, GUIImageFramebuffer*>::iterator it = _mmapImageFramebuffers.begin();
		while (it != _mmapImageFramebuffers.end())
		{
			delete it->second;
			_mmapImageFramebuffers.erase(it);
			it = _mmapImageFramebuffers.begin();
		}
	}

	GUIImageDepthbuffer* GUITooltip::addImageDepthbuffer(const std::string& strName, float fPosX, float fPosY, const std::string& strDBname, float fWidth, float fHeight)
	{
		// If resource already exists
		std::map<std::string, GUIImageDepthbuffer*>::iterator it = _mmapImageDepthbuffers.find(strName);
		ThrowIfTrue(it != _mmapImageDepthbuffers.end(), "GUITooltip::addImageDepthbuffer(" + strName + ") failed. The named object already exists.");
		GUIImageDepthbuffer* pNewRes = new GUIImageDepthbuffer;
		ThrowIfFalse(pNewRes, "GUITooltip::addImageDepthbuffer(" + strName + ") failed. Could not allocate memory for the new object.");
		pNewRes->mfPositionX = fPosX;
		pNewRes->mfPositionY = fPosY;
		pNewRes->mfWidth = fWidth;
		pNewRes->mfHeight = fHeight;
		pNewRes->_mstrDBname = strDBname;
		_mmapImageDepthbuffers[strName] = pNewRes;

		// Get ResourceFramebuffer from the resource manager to set size of this object
		ResourceManager* pResMan = ResourceManager::getPointer();
		ResourceDepthbuffer* pDB = pResMan->getDepthbuffer(strDBname);

		// If a value less than 0 is passed to width/height, set widget to dims of the image
		if (fWidth < 0)
			pNewRes->mfWidth = (float)pDB->getWidth();
		if (fHeight < 0)
			pNewRes->mfHeight = (float)pDB->getHeight();

		return pNewRes;
	}

	GUIImageDepthbuffer* GUITooltip::getImageDepthbuffer(const std::string& strName)
	{
		std::map<std::string, GUIImageDepthbuffer*>::iterator it = _mmapImageDepthbuffers.find(strName);
		ThrowIfTrue(it == _mmapImageDepthbuffers.end(), "GUITooltip::getImageDepthbuffer(" + strName + ") failed. The named object doesn't exist.");
		return it->second;
	}

	void GUITooltip::removeImageDepthbuffer(const std::string& strName)
	{
		std::map<std::string, GUIImageDepthbuffer*>::iterator it = _mmapImageDepthbuffers.find(strName);
		if (it == _mmapImageDepthbuffers.end())
			return;

		delete it->second;
		_mmapImageDepthbuffers.erase(it);
	}

	void GUITooltip::removeAllImageDepthbuffers(void)
	{
		std::map<std::string, GUIImageDepthbuffer*>::iterator it = _mmapImageDepthbuffers.begin();
		while (it != _mmapImageDepthbuffers.end())
		{
			delete it->second;
			_mmapImageDepthbuffers.erase(it);
			it = _mmapImageDepthbuffers.begin();
		}
	}

	void GUITooltip::removeAll(void)
	{
		removeAllText();
		removeAllLineGraphs();
		removeAllProgressBars();
		removeAllImages();
		removeAllImagesAnimated();
		removeAllImageFramebuffers();
		removeAllImageDepthbuffers();
	}

	void GUITooltip::setAsText(const std::string& strText)
	{
		removeAll();
		_mbEnabled = true;
		addText("Text", 0, 0, strText);
	}
}