#include "PCH.h"
#include "GUIContainer.h"
#include "input.h"
#include "resourceManager.h"
#include "GUIManager.h"
#include "window.h"
#include "log.h"
#include "GUITheme.h"

namespace X
{
	CGUIContainer::CGUIContainer()
	{
		mstrThemename = "default";
		_mbContainerIsWindow = true;
		_mbWindowBeingMoved = false;
		_mbVisible = true;
		_mTextColour = SCGUIManager::getPointer()->getTheme("default")->mColours.containerTitlebarTextNotInFocus;
	}

	CGUIContainer::~CGUIContainer()
	{
		removeAll();
	}

	void CGUIContainer::render(const std::string& strFramebufferToSampleFrom)
	{
		if (!_mbVisible)
			return;

		// Render this container (If it's set as a window)
		_renderContainer(strFramebufferToSampleFrom);
	
		// Render each button
		std::map<std::string, CGUIButton*>::iterator itButton = _mmapButtons.begin();
		while (itButton != _mmapButtons.end())
		{
			itButton->second->render(this, strFramebufferToSampleFrom);
			itButton++;
		}

		// Render each text edit
		std::map<std::string, CGUITextEdit*>::iterator itTextEdit = _mmapTextEdits.begin();
		while (itTextEdit != _mmapTextEdits.end())
		{
			itTextEdit->second->render(this, strFramebufferToSampleFrom);
			itTextEdit++;
		}

		// Render each sliders
		std::map<std::string, CGUISlider*>::iterator itSlider = _mmapSliders.begin();
		while (itSlider != _mmapSliders.end())
		{
			itSlider->second->render(this, strFramebufferToSampleFrom);
			itSlider++;
		}

		// Render each line graph
		std::map<std::string, CGUILineGraph*>::iterator itLineGraph = _mmapLineGraphs.begin();
		while (itLineGraph != _mmapLineGraphs.end())
		{
			itLineGraph->second->render(this, strFramebufferToSampleFrom);
			itLineGraph++;
		}

		// Render each progress bar
		std::map<std::string, CGUIProgressBar*>::iterator itProgressBar = _mmapProgressBars.begin();
		while (itProgressBar != _mmapProgressBars.end())
		{
			itProgressBar->second->render(this, strFramebufferToSampleFrom);
			itProgressBar++;
		}

		// Render each image
		std::map<std::string, CGUIImage*>::iterator itImage = _mmapImages.begin();
		while (itImage != _mmapImages.end())
		{
			itImage->second->render(this, strFramebufferToSampleFrom);
			itImage++;
		}

		// Render each animated image
		std::map<std::string, CGUIImageAnimated*>::iterator itImageAnimated = _mmapImageAnimateds.begin();
		while (itImageAnimated != _mmapImageAnimateds.end())
		{
			itImageAnimated->second->render(this, strFramebufferToSampleFrom);
			itImageAnimated++;
		}

		// Images framebuffer
		std::map<std::string, CGUIImageFramebuffer*>::iterator itImageFB = _mmapImageFramebuffers.begin();
		while (itImageFB != _mmapImageFramebuffers.end())
		{
			itImageFB->second->render(this, strFramebufferToSampleFrom);
			itImageFB++;
		}

		// Render each text scroll object
		std::map<std::string, CGUITextScroll*>::iterator itTextScroll = _mmapTextScrolls.begin();
		while (itTextScroll != _mmapTextScrolls.end())
		{
			itTextScroll->second->render(this, strFramebufferToSampleFrom);
			itTextScroll++;
		}

		// Render each of the button images
		std::map<std::string, CGUIButtonImage*>::iterator itButtonImage = _mmapButtonImages.begin();
		while (itButtonImage != _mmapButtonImages.end())
		{
			itButtonImage->second->render(this, strFramebufferToSampleFrom);
			itButtonImage++;
		}

		// Images depth buffer
		std::map<std::string, CGUIImageDepthbuffer*>::iterator itImageDB = _mmapImageDepthbuffers.begin();
		while (itImageDB != _mmapImageDepthbuffers.end())
		{
			itImageDB->second->render(this, strFramebufferToSampleFrom);
			itImageDB++;
		}

		// Render each text
		std::map<std::string, CGUIText*>::iterator itText = _mmapTexts.begin();
		while (itText != _mmapTexts.end())
		{
			itText->second->render(this);
			itText++;
		}
	}

	void CGUIContainer::renderTooltips(const std::string& strFramebufferToSampleFrom)
	{
		// Render each button's tooltips
		std::map<std::string, CGUIButton*>::iterator itButton = _mmapButtons.begin();
		while (itButton != _mmapButtons.end())
		{
			itButton->second->mpTooltip->render(this, strFramebufferToSampleFrom);
			itButton++;
		}

		// Render each text's tooltip
		std::map<std::string, CGUIText*>::iterator itText = _mmapTexts.begin();
		while (itText != _mmapTexts.end())
		{
			itText->second->mpTooltip->render(this, strFramebufferToSampleFrom);
			itText++;
		}

		// Render each text edit's tooltip
		std::map<std::string, CGUITextEdit*>::iterator itTextEdit = _mmapTextEdits.begin();
		while (itTextEdit != _mmapTextEdits.end())
		{
			itTextEdit->second->mpTooltip->render(this, strFramebufferToSampleFrom);
			itTextEdit++;
		}

		// Render each slider's tooltip
		std::map<std::string, CGUISlider*>::iterator itSlider = _mmapSliders.begin();
		while (itSlider != _mmapSliders.end())
		{
			itSlider->second->mpTooltip->render(this, strFramebufferToSampleFrom);
			itSlider++;
		}

		// Render each line graph's tooltip
		std::map<std::string, CGUILineGraph*>::iterator itLineGraph = _mmapLineGraphs.begin();
		while (itLineGraph != _mmapLineGraphs.end())
		{
			itLineGraph->second->mpTooltip->render(this, strFramebufferToSampleFrom);
			itLineGraph++;
		}

		// Render each progress bar's tooltip
		std::map<std::string, CGUIProgressBar*>::iterator itProgressBar = _mmapProgressBars.begin();
		while (itProgressBar != _mmapProgressBars.end())
		{
			itProgressBar->second->mpTooltip->render(this, strFramebufferToSampleFrom);
			itProgressBar++;
		}

		// Render each image's tooltip
		std::map<std::string, CGUIImage*>::iterator itImage = _mmapImages.begin();
		while (itImage != _mmapImages.end())
		{
			itImage->second->mpTooltip->render(this, strFramebufferToSampleFrom);
			itImage++;
		}

		// Render each animated image's tooltip
		std::map<std::string, CGUIImageAnimated*>::iterator itImageAnimated = _mmapImageAnimateds.begin();
		while (itImageAnimated != _mmapImageAnimateds.end())
		{
			itImageAnimated->second->mpTooltip->render(this, strFramebufferToSampleFrom);
			itImageAnimated++;
		}

		// Images framebuffer's tooltip
		std::map<std::string, CGUIImageFramebuffer*>::iterator itImageFB = _mmapImageFramebuffers.begin();
		while (itImageFB != _mmapImageFramebuffers.end())
		{
			itImageFB->second->mpTooltip->render(this, strFramebufferToSampleFrom);
			itImageFB++;
		}

		// Render each text scroll object's tooltip
		std::map<std::string, CGUITextScroll*>::iterator itTextScroll = _mmapTextScrolls.begin();
		while (itTextScroll != _mmapTextScrolls.end())
		{
			itTextScroll->second->mpTooltip->render(this, strFramebufferToSampleFrom);
			itTextScroll++;
		}

		// Render each of the button images's tooltip
		std::map<std::string, CGUIButtonImage*>::iterator itButtonImage = _mmapButtonImages.begin();
		while (itButtonImage != _mmapButtonImages.end())
		{
			itButtonImage->second->mpTooltip->render(this, strFramebufferToSampleFrom);
			itButtonImage++;
		}

		// Images depth buffer's tooltip
		std::map<std::string, CGUIImageDepthbuffer*>::iterator itImageDB = _mmapImageDepthbuffers.begin();
		while (itImageDB != _mmapImageDepthbuffers.end())
		{
			itImageDB->second->mpTooltip->render(this, strFramebufferToSampleFrom);
			itImageDB++;
		}
	}

	// Containers are updated in order of ZOrder, with the front most being updated first
	bool CGUIContainer::update(bool bMouseIsOverContainerWhichIsAboveThisOne)
	{
		if (!_mbVisible)
			return false;

		SCResourceManager* pRM = SCResourceManager::getPointer();
		SCGUIManager* pGUI = SCGUIManager::getPointer();
		CGUITheme* pTheme = pGUI->getTheme(mstrThemename);
		SCWindow* pWindow = SCWindow::getPointer();
		CVector2f vWindowDims = pWindow->getDimensions();

		SCInputManager* pInput = SCInputManager::getPointer();
		CVector2f vMousePos = pInput->mouse.getCursorPos();
		CVector2f vTexDimsDiv3 = pRM->getTexture2DFromFile(pTheme->mImages.containerBGColour)->getDimensions() * 0.3333333f;

		// Determine whether mouse cursor is over this container
		bool bMouseOver = false;
		if (!bMouseIsOverContainerWhichIsAboveThisOne)	// Only perform check if mouse is NOT over a container which is above this one
		{
			if (_mbContainerIsWindow)
			{
				if (vMousePos.x >= mfPositionX - vTexDimsDiv3.x)
					if (vMousePos.x <= mfPositionX + mfWidth + vTexDimsDiv3.x)
						if (vMousePos.y >= mfPositionY - vTexDimsDiv3.y)
							if (vMousePos.y <= mfPositionY + mfHeight + vTexDimsDiv3.y)
							{
								// Mouse cursor is over this container
								bMouseOver = true;
							}
			}
			else
			{
				if (vMousePos.x >= mfPositionX)
					if (vMousePos.x <= mfPositionX + mfWidth)
						if (vMousePos.y >= mfPositionY)
							if (vMousePos.y <= mfPositionY + mfHeight)
							{
								// Mouse cursor is over this container
								bMouseOver = true;
							}
			}
		}

		// The mouse is over this window, we may accept mouse clicks on this container's items
		if (bMouseOver)
		{
			// Dragging of window?
			if (_mbContainerIsWindow)
			{
				if (pInput->mouse.leftButtonOnce())
				{
					// Set colour of all other containers
					std::map<std::string, CGUIContainer*>::iterator itCont = pGUI->_mmapContainers.begin();
					while (itCont != pGUI->_mmapContainers.end())
					{
						itCont->second->_mTextColour = pTheme->mColours.containerTitlebarTextNotInFocus;
						itCont++;
					}
					// Set colour of titlebar text
					_mTextColour = pTheme->mColours.containerTitlebarTextInFocus;
					
					// Only move window if mouse is over titlebar
					if (vMousePos.y > mfPositionY - vTexDimsDiv3.y && vMousePos.y < mfPositionY)
					{
						if (!pGUI->getWindowBeingMoved())
						{
							_mbWindowBeingMoved = true;
							pGUI->setWindowBeingMoved(true);
						}
					}
				}
			}
		}
		else  // The mouse is not over this container, or it is, but there is another container above this one so only update time based stuff, not clickable stuff.
		{

		}

		// Stop dragging if mouse button is no longer down
		if (_mbWindowBeingMoved)
		{
			if (!pInput->mouse.leftButDown())
			{
				_mbWindowBeingMoved = false;
				pGUI->setWindowBeingMoved(false);
			}
		}

		if (_mbWindowBeingMoved)
		{
			mfPositionX += pInput->mouse.getMouseDeltaGUI().x;
			mfPositionY += pInput->mouse.getMouseDeltaGUI().y;
		}

		// Clamp the container's position so that it's borders are viewable IF the container is set as a window
		if (_mbContainerIsWindow)
		{
			// Get a texture so we can determine dimensions
			CResourceTexture2DFromFile* pTexColour = pRM->getTexture2DFromFile(pTheme->mImages.containerBGColour);
			CVector2f vTexDimsDiv3 = pTexColour->getDimensions() * 0.3333333f;
			if (mfPositionX < vTexDimsDiv3.x)
				mfPositionX = vTexDimsDiv3.x;
			else if (mfPositionX + mfWidth + vTexDimsDiv3.x > vWindowDims.x)
				mfPositionX = vWindowDims.x - mfWidth - vTexDimsDiv3.x;
			if (mfPositionY < vTexDimsDiv3.x)
				mfPositionY = vTexDimsDiv3.x;
			else if (mfPositionY + mfHeight + vTexDimsDiv3.y >= vWindowDims.y)
				mfPositionY = vWindowDims.y - mfHeight - vTexDimsDiv3.y;

		}

		// Now update each of the widgets
		// Compute whether container is accepting mouse clicks or not
		bool bContainerAcceptingMouseClicks = bMouseOver;
		if (_mbWindowBeingMoved)
			bContainerAcceptingMouseClicks = false;

		// Buttons
		std::map<std::string, CGUIButton*>::iterator itButton = _mmapButtons.begin();
		while (itButton != _mmapButtons.end())
		{
			itButton->second->update(this, bContainerAcceptingMouseClicks);
			itButton++;
		}

		// Text
		std::map<std::string, CGUIText*>::iterator itText = _mmapTexts.begin();
		while (itText != _mmapTexts.end())
		{
			itText->second->update(this, bContainerAcceptingMouseClicks);
			itText++;
		}

		// Text edit
		std::map<std::string, CGUITextEdit*>::iterator itTextEdit = _mmapTextEdits.begin();
		while (itTextEdit != _mmapTextEdits.end())
		{
			itTextEdit->second->update(this, bContainerAcceptingMouseClicks);
			itTextEdit++;
		}

		// Sliders
		std::map<std::string, CGUISlider*>::iterator itSlider = _mmapSliders.begin();
		while (itSlider != _mmapSliders.end())
		{
			itSlider->second->update(this, bContainerAcceptingMouseClicks);
			itSlider++;
		}

		// LineGraphs
		std::map<std::string, CGUILineGraph*>::iterator itLineGraph = _mmapLineGraphs.begin();
		while (itLineGraph != _mmapLineGraphs.end())
		{
			itLineGraph->second->update(this, bContainerAcceptingMouseClicks);
			itLineGraph++;
		}

		// Progress bars
		std::map<std::string, CGUIProgressBar*>::iterator itProgressBar = _mmapProgressBars.begin();
		while (itProgressBar != _mmapProgressBars.end())
		{
			itProgressBar->second->update(this, bContainerAcceptingMouseClicks);
			itProgressBar++;
		}

		// Images
		std::map<std::string, CGUIImage*>::iterator itImage = _mmapImages.begin();
		while (itImage != _mmapImages.end())
		{
			itImage->second->update(this, bContainerAcceptingMouseClicks);
			itImage++;
		}

		// Images animated
		std::map<std::string, CGUIImageAnimated*>::iterator itImageAnimated = _mmapImageAnimateds.begin();
		while (itImageAnimated != _mmapImageAnimateds.end())
		{
			itImageAnimated->second->update(this, bContainerAcceptingMouseClicks);
			itImageAnimated++;
		}

		// Images framebuffer
		std::map<std::string, CGUIImageFramebuffer*>::iterator itImageFB = _mmapImageFramebuffers.begin();
		while (itImageFB != _mmapImageFramebuffers.end())
		{
			itImageFB->second->update(this, bContainerAcceptingMouseClicks);
			itImageFB++;
		}

		// Text scroll objects
		std::map<std::string, CGUITextScroll*>::iterator itTextScroll = _mmapTextScrolls.begin();
		while (itTextScroll != _mmapTextScrolls.end())
		{
			itTextScroll->second->update(this, bContainerAcceptingMouseClicks);
			itTextScroll++;
		}

		// Button images
		std::map<std::string, CGUIButtonImage*>::iterator itButtonImage = _mmapButtonImages.begin();
		while (itButtonImage != _mmapButtonImages.end())
		{
			itButtonImage->second->update(this, bContainerAcceptingMouseClicks);
			itButtonImage++;
		}

		// Images depth buffer
		std::map<std::string, CGUIImageDepthbuffer*>::iterator itImageDB = _mmapImageDepthbuffers.begin();
		while (itImageDB != _mmapImageDepthbuffers.end())
		{
			itImageDB->second->update(this, bContainerAcceptingMouseClicks);
			itImageDB++;
		}

		return bMouseOver;
	}

	void CGUIContainer::setBehaviour(bool bContainerIsWindow)
	{
		_mbContainerIsWindow = bContainerIsWindow;
	}

	std::string CGUIContainer::getThemeName(void) const
	{
		return mstrThemename;
	}

	CGUITheme* CGUIContainer::getTheme(void) const
	{
		return SCGUIManager::getPointer()->getTheme(mstrThemename);
	}

	void CGUIContainer::setThemeName(const std::string& strThemeToUse)
	{
		mstrThemename = strThemeToUse;
	}

	void CGUIContainer::setVisible(bool bVisible)
	{
		_mbVisible = bVisible;

		// If set to invisible, we need to reset all tooltips for this container

		// Buttons
		std::map<std::string, CGUIButton*>::iterator itButton = _mmapButtons.begin();
		while (itButton != _mmapButtons.end())
		{
			itButton->second->mpTooltip->resetFade();
			itButton++;
		}

		// Text
		std::map<std::string, CGUIText*>::iterator itText = _mmapTexts.begin();
		while (itText != _mmapTexts.end())
		{
			itText->second->mpTooltip->resetFade();
			itText++;
		}

		// Text edit
		std::map<std::string, CGUITextEdit*>::iterator itTextEdit = _mmapTextEdits.begin();
		while (itTextEdit != _mmapTextEdits.end())
		{
			itTextEdit->second->mpTooltip->resetFade();
			itTextEdit++;
		}

		// Sliders
		std::map<std::string, CGUISlider*>::iterator itSlider = _mmapSliders.begin();
		while (itSlider != _mmapSliders.end())
		{
			itSlider->second->mpTooltip->resetFade();
			itSlider++;
		}

		// LineGraphs
		std::map<std::string, CGUILineGraph*>::iterator itLineGraph = _mmapLineGraphs.begin();
		while (itLineGraph != _mmapLineGraphs.end())
		{
			itLineGraph->second->mpTooltip->resetFade();
			itLineGraph++;
		}

		// Progress bars
		std::map<std::string, CGUIProgressBar*>::iterator itProgressBar = _mmapProgressBars.begin();
		while (itProgressBar != _mmapProgressBars.end())
		{
			itProgressBar->second->mpTooltip->resetFade();
			itProgressBar++;
		}

		// Images
		std::map<std::string, CGUIImage*>::iterator itImage = _mmapImages.begin();
		while (itImage != _mmapImages.end())
		{
			itImage->second->mpTooltip->resetFade();
			itImage++;
		}

		// Images animated
		std::map<std::string, CGUIImageAnimated*>::iterator itImageAnimated = _mmapImageAnimateds.begin();
		while (itImageAnimated != _mmapImageAnimateds.end())
		{
			itImageAnimated->second->mpTooltip->resetFade();
			itImageAnimated++;
		}

		// Images framebuffer
		std::map<std::string, CGUIImageFramebuffer*>::iterator itImageFB = _mmapImageFramebuffers.begin();
		while (itImageFB != _mmapImageFramebuffers.end())
		{
			itImageFB->second->mpTooltip->resetFade();
			itImageFB++;
		}

		// Text scroll objects
		std::map<std::string, CGUITextScroll*>::iterator itTextScroll = _mmapTextScrolls.begin();
		while (itTextScroll != _mmapTextScrolls.end())
		{
			itTextScroll->second->mpTooltip->resetFade();
			itTextScroll++;
		}

		// Button images
		std::map<std::string, CGUIButtonImage*>::iterator itButtonImage = _mmapButtonImages.begin();
		while (itButtonImage != _mmapButtonImages.end())
		{
			itButtonImage->second->mpTooltip->resetFade();
			itButtonImage++;
		}

		// Images depth buffer
		std::map<std::string, CGUIImageDepthbuffer*>::iterator itImageDB = _mmapImageDepthbuffers.begin();
		while (itImageDB != _mmapImageDepthbuffers.end())
		{
			itImageDB->second->mpTooltip->resetFade();
			itImageDB++;
		}
	}

	bool CGUIContainer::getVisible(void) const
	{
		return _mbVisible;
	}

	const std::string& CGUIContainer::getName(void) const
	{
		return _mstrName;
	}

	void CGUIContainer::_renderContainer(const std::string& strFramebufferToSampleFrom)
	{
		if (!_mbVisible)
			return;

		if (!_mbContainerIsWindow)
			return;	// No point rendering anything

		// Get required resources needed to render
		SCGUIManager* pGUI = SCGUIManager::getPointer();
		SCResourceManager* pRM = SCResourceManager::getPointer();
		SCWindow* pWindow = SCWindow::getPointer();
		CResourceVertexBufferCPT* pVB = pRM->getVertexBufferCPT("X:default");
		CResourceShader* pShader = pRM->getShader("X:gui");
		CGUITheme* pTheme = pGUI->getTheme(mstrThemename);
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

		// Get textures and background sample framebuffer
		CResourceTexture2DFromFile* pTexColour = pRM->getTexture2DFromFile(pTheme->mImages.containerBGColour);
		CResourceTexture2DFromFile* pTexNormal = pRM->getTexture2DFromFile(pTheme->mImages.containerBGNormal);
		CResourceTexture2DFromFile* pTexReflection = pRM->getTexture2DFromFile(pTheme->mImages.reflection);
		CResourceFramebuffer* pFBSample = pRM->getFramebuffer(strFramebufferToSampleFrom);

		// Bind textures
		pTexColour->bind(0);
		pTexNormal->bind(1);
		pTexReflection->bind(2);
		pFBSample->bindAsTexture(3);

		// Render the container centre
		pVB->removeGeom();
		pVB->addQuad2D(
			CVector2f(mfPositionX, mfPositionY),	// Position
			CVector2f(mfWidth, mfHeight),			// Dimensions
			CColour(1.0f, 1.0f, 1.0f, 1.0f),		// Vertex colour
			mTC.centre.BL,
			mTC.centre.BR,
			mTC.centre.TR,
			mTC.centre.TL);

		// Render the left edge
		CVector2f vTexDimsDiv3 = pTexColour->getDimensions() * 0.3333333f;
		pVB->addQuad2D(
			CVector2f(mfPositionX - vTexDimsDiv3.x, mfPositionY),	// Position
			CVector2f(vTexDimsDiv3.x, mfHeight),					// Dimensions
			CColour(1.0f, 1.0f, 1.0f, 1.0f),						// Vertex colour
			mTC.left.BL,
			mTC.left.BR,
			mTC.left.TR,
			mTC.left.TL);

		// Render the right edge
		pVB->addQuad2D(
			CVector2f(mfPositionX + mfWidth, mfPositionY),	// Position
			CVector2f(vTexDimsDiv3.x, mfHeight),			// Dimensions
			CColour(1.0f, 1.0f, 1.0f, 1.0f),				// Vertex colour
			mTC.right.BL,
			mTC.right.BR,
			mTC.right.TR,
			mTC.right.TL);

		// Render the top edge
		pVB->addQuad2D(
			CVector2f(mfPositionX, mfPositionY - vTexDimsDiv3.y),	// Position
			CVector2f(mfWidth, vTexDimsDiv3.y),						// Dimensions
			CColour(1.0f, 1.0f, 1.0f, 1.0f),						// Vertex colour
			mTC.top.BL,
			mTC.top.BR,
			mTC.top.TR,
			mTC.top.TL);

		// Render the bottom edge
		pVB->addQuad2D(
			CVector2f(mfPositionX, mfPositionY + mfHeight),		// Position
			CVector2f(mfWidth, vTexDimsDiv3.y),					// Dimensions
			CColour(1.0f, 1.0f, 1.0f, 1.0f),					// Vertex colour
			mTC.bottom.BL,
			mTC.bottom.BR,
			mTC.bottom.TR,
			mTC.bottom.TL);

		// Render the top left corner
		pVB->addQuad2D(
			CVector2f(mfPositionX - vTexDimsDiv3.x, mfPositionY - vTexDimsDiv3.y),	// Position
			CVector2f(vTexDimsDiv3.x, vTexDimsDiv3.y),								// Dimensions
			CColour(1.0f, 1.0f, 1.0f, 1.0f),										// Vertex colour
			mTC.topLeft.BL,
			mTC.topLeft.BR,
			mTC.topLeft.TR,
			mTC.topLeft.TL);

		// Render the top right corner
		pVB->addQuad2D(
			CVector2f(mfPositionX + mfWidth, mfPositionY - vTexDimsDiv3.y),	// Position
			CVector2f(vTexDimsDiv3.x, vTexDimsDiv3.y),						// Dimensions
			CColour(1.0f, 1.0f, 1.0f, 1.0f),								// Vertex colour
			mTC.topRight.BL,
			mTC.topRight.BR,
			mTC.topRight.TR,
			mTC.topRight.TL);

		// Render the bottom left corner
		pVB->addQuad2D(
			CVector2f(mfPositionX - vTexDimsDiv3.x, mfPositionY + mfHeight),	// Position
			CVector2f(vTexDimsDiv3.x, vTexDimsDiv3.y),							// Dimensions
			CColour(1.0f, 1.0f, 1.0f, 1.0f),									// Vertex colour
			mTC.bottomLeft.BL,
			mTC.bottomLeft.BR,
			mTC.bottomLeft.TR,
			mTC.bottomLeft.TL);

		// Render the bottom right corner
		pVB->addQuad2D(
			CVector2f(mfPositionX + mfWidth, mfPositionY + mfHeight),	// Position
			CVector2f(vTexDimsDiv3.x, vTexDimsDiv3.y),					// Dimensions
			CColour(1.0f, 1.0f, 1.0f, 1.0f),							// Vertex colour
			mTC.bottomRight.BL,
			mTC.bottomRight.BR,
			mTC.bottomRight.TR,
			mTC.bottomRight.TL);

		pVB->update();
		pVB->render();

		pTexColour->unbindAll();	// Unbind textures
		pShader->unbind();	// Unbind the GUI shader
		glDisable(GL_BLEND);

		// Container title
		int iRTDims[2];
		iRTDims[0] = int(pWindow->getWidth());
		iRTDims[1] = int(pWindow->getHeight());
		CResourceFont* pFont = pRM->getFont(pTheme->mFonts.containerTitle);
		pFont->print(mstrTitleText,
			int(mfPositionX) + pTheme->mOffsets.containerTitlebarText.iOffsetX,
			int(mfPositionY - vTexDimsDiv3.y) + pTheme->mOffsets.containerTitlebarText.iOffsetY,
			iRTDims[0], iRTDims[1],
			1.0f,
			_mTextColour);
	}

	CGUIButton* CGUIContainer::addButton(const std::string& strName, float fPosX, float fPosY, float fWidth, float fHeight, const std::string& strText)
	{
		// If resource already exists
		std::map<std::string, CGUIButton*>::iterator it = _mmapButtons.find(strName);
		ThrowIfTrue(it != _mmapButtons.end(), "CGUIContainer::addButton(" + strName + ") failed. The named object already exists.");
		CGUIButton* pNewRes = new CGUIButton;
		ThrowIfFalse(pNewRes, "CGUIContainer::addButton(" + strName + ") failed. Could not allocate memory for the new object.");
		pNewRes->mfPositionX = fPosX;
		pNewRes->mfPositionY = fPosY;
		pNewRes->mfWidth = fWidth;
		pNewRes->mfHeight = fHeight;
		pNewRes->mstrText = strText;
		_mmapButtons[strName] = pNewRes;
		return pNewRes;
	}

	CGUIButton* CGUIContainer::getButton(const std::string& strName) const
	{
		std::map<std::string, CGUIButton*>::iterator it = _mmapButtons.find(strName);
		ThrowIfTrue(it == _mmapButtons.end(), "CGUIContainer::getButton(" + strName + ") failed. The named object doesn't exist.");
		return it->second;
	}

	void CGUIContainer::removeButton(const std::string& strName)
	{
		std::map<std::string, CGUIButton*>::iterator it = _mmapButtons.find(strName);
		if (it == _mmapButtons.end())
			return;
		delete it->second;
		_mmapButtons.erase(it);
	}

	void CGUIContainer::removeAllButtons(void)
	{
		std::map<std::string, CGUIButton*>::iterator it = _mmapButtons.begin();
		while (it != _mmapButtons.end())
		{
			delete it->second;
			_mmapButtons.erase(it);
			it = _mmapButtons.begin();
		}
	}

	CGUIText* CGUIContainer::addText(const std::string& strName, float fPosX, float fPosY, const std::string& strText)
	{
		// If resource already exists
		std::map<std::string, CGUIText*>::iterator it = _mmapTexts.find(strName);
		ThrowIfTrue(it != _mmapTexts.end(), "CGUIContainer::addText(" + strName + ") failed. The named object already exists.");
		CGUIText* pNewRes = new CGUIText;
		ThrowIfFalse(pNewRes, "CGUIContainer::addText(" + strName + ") failed. Could not allocate memory for the new object.");
		pNewRes->mfPositionX = fPosX;
		pNewRes->mfPositionY = fPosY;
		pNewRes->setText(strText);
		_mmapTexts[strName] = pNewRes;
		return pNewRes;
	}

	CGUIText* CGUIContainer::getText(const std::string& strName) const
	{
		std::map<std::string, CGUIText*>::iterator it = _mmapTexts.find(strName);
		ThrowIfTrue(it == _mmapTexts.end(), "CGUIContainer::getText(" + strName + ") failed. The named object doesn't exist.");
		return it->second;
	}

	void CGUIContainer::removeText(const std::string& strName)
	{
		std::map<std::string, CGUIText*>::iterator it = _mmapTexts.find(strName);
		if (it == _mmapTexts.end())
			return;
		delete it->second;
		_mmapTexts.erase(it);
	}

	void CGUIContainer::removeAllText(void)
	{
		std::map<std::string, CGUIText*>::iterator it = _mmapTexts.begin();
		while (it != _mmapTexts.end())
		{
			delete it->second;
			_mmapTexts.erase(it);
			it = _mmapTexts.begin();
		}
	}

	CGUITextEdit* CGUIContainer::addTextEdit(const std::string& strName, float fPosX, float fPosY, float fWidth, float fHeight, const std::string& strText)
	{
		// If resource already exists
		std::map<std::string, CGUITextEdit*>::iterator it = _mmapTextEdits.find(strName);
		ThrowIfTrue(it != _mmapTextEdits.end(), "CGUIContainer::addTextEdit(" + strName + ") failed. The named object already exists.");
		CGUITextEdit* pNewRes = new CGUITextEdit;
		ThrowIfFalse(pNewRes, "CGUIContainer::addTextEdit(" + strName + ") failed. Could not allocate memory for the new object.");
		pNewRes->mfPositionX = fPosX;
		pNewRes->mfPositionY = fPosY;
		pNewRes->mfWidth = fWidth;
		pNewRes->mfHeight = fHeight;
		pNewRes->mstrText = strText;
		_mmapTextEdits[strName] = pNewRes;
		return pNewRes;
	}

	CGUITextEdit* CGUIContainer::getTextEdit(const std::string& strName) const
	{
		std::map<std::string, CGUITextEdit*>::iterator it = _mmapTextEdits.find(strName);
		ThrowIfTrue(it == _mmapTextEdits.end(), "CGUIContainer::getTextEdit(" + strName + ") failed. The named object doesn't exist.");
		return it->second;
	}

	void CGUIContainer::removeTextEdit(const std::string& strName)
	{
		std::map<std::string, CGUITextEdit*>::iterator it = _mmapTextEdits.find(strName);
		if (it == _mmapTextEdits.end())
			return;
		delete it->second;
		_mmapTextEdits.erase(it);
	}

	void CGUIContainer::removeAllTextEdits(void)
	{
		std::map<std::string, CGUITextEdit*>::iterator it = _mmapTextEdits.begin();
		while (it != _mmapTextEdits.end())
		{
			delete it->second;
			_mmapTextEdits.erase(it);
			it = _mmapTextEdits.begin();
		}
	}

	CGUISlider* CGUIContainer::addSlider(const std::string& strName, float fPosX, float fPosY, float fWidth, float fHeight, float fTabRatio)
	{
		// If resource already exists
		std::map<std::string, CGUISlider*>::iterator it = _mmapSliders.find(strName);
		ThrowIfTrue(it != _mmapSliders.end(), "CGUIContainer::addSlider(" + strName + ") failed. The named object already exists.");
		CGUISlider* pNewRes = new CGUISlider;
		ThrowIfFalse(pNewRes, "CGUIContainer::addSlider(" + strName + ") failed. Could not allocate memory for the new object.");
		pNewRes->mfPositionX = fPosX;
		pNewRes->mfPositionY = fPosY;
		pNewRes->mfWidth = fWidth;
		pNewRes->mfHeight = fHeight;
		pNewRes->_mfTabRatio = fTabRatio;
		_mmapSliders[strName] = pNewRes;
		return pNewRes;
	}

	CGUISlider* CGUIContainer::getSlider(const std::string& strName) const
	{
		std::map<std::string, CGUISlider*>::iterator it = _mmapSliders.find(strName);
		ThrowIfTrue(it == _mmapSliders.end(), "CGUIContainer::getSlider(" + strName + ") failed. The named object doesn't exist.");
		return it->second;
	}

	void CGUIContainer::removeSlider(const std::string& strName)
	{
		std::map<std::string, CGUISlider*>::iterator it = _mmapSliders.find(strName);
		if (it == _mmapSliders.end())
			return;
		delete it->second;
		_mmapSliders.erase(it);
	}

	void CGUIContainer::removeAllSliders(void)
	{
		std::map<std::string, CGUISlider*>::iterator it = _mmapSliders.begin();
		while (it != _mmapSliders.end())
		{
			delete it->second;
			_mmapSliders.erase(it);
			it = _mmapSliders.begin();
		}
	}

	CGUILineGraph* CGUIContainer::addLineGraph(const std::string& strName, float fPosX, float fPosY, float fWidth, float fHeight)
	{
		// If resource already exists
		std::map<std::string, CGUILineGraph*>::iterator it = _mmapLineGraphs.find(strName);
		ThrowIfTrue(it != _mmapLineGraphs.end(), "CGUIContainer::addLineGraph(" + strName + ") failed. The named object already exists.");
		CGUILineGraph* pNewRes = new CGUILineGraph;
		ThrowIfFalse(pNewRes, "CGUIContainer::addLineGraph(" + strName + ") failed. Could not allocate memory for the new object.");
		pNewRes->mfPositionX = fPosX;
		pNewRes->mfPositionY = fPosY;
		pNewRes->mfWidth = fWidth;
		pNewRes->mfHeight = fHeight;
		_mmapLineGraphs[strName] = pNewRes;
		return pNewRes;
	}

	CGUILineGraph* CGUIContainer::getLineGraph(const std::string& strName) const
	{
		std::map<std::string, CGUILineGraph*>::iterator it = _mmapLineGraphs.find(strName);
		ThrowIfTrue(it == _mmapLineGraphs.end(), "CGUIContainer::getLineGraph(" + strName + ") failed. The named object doesn't exist.");
		return it->second;
	}

	void CGUIContainer::removeLineGraph(const std::string& strName)
	{
		std::map<std::string, CGUILineGraph*>::iterator it = _mmapLineGraphs.find(strName);
		if (it == _mmapLineGraphs.end())
			return;
		delete it->second;
		_mmapLineGraphs.erase(it);
	}

	void CGUIContainer::removeAllLineGraphs(void)
	{
		std::map<std::string, CGUILineGraph*>::iterator it = _mmapLineGraphs.begin();
		while (it != _mmapLineGraphs.end())
		{
			delete it->second;
			_mmapLineGraphs.erase(it);
			it = _mmapLineGraphs.begin();
		}
	}

	CGUIProgressBar* CGUIContainer::addProgressBar(const std::string& strName, float fPosX, float fPosY, float fWidth, float fHeight)
	{
		// If resource already exists
		std::map<std::string, CGUIProgressBar*>::iterator it = _mmapProgressBars.find(strName);
		ThrowIfTrue(it != _mmapProgressBars.end(), "CGUIContainer::addProgressBar(" + strName + ") failed. The named object already exists.");
		CGUIProgressBar* pNewRes = new CGUIProgressBar;
		ThrowIfFalse(pNewRes, "CGUIContainer::addProgressBar(" + strName + ") failed. Could not allocate memory for the new object.");
		pNewRes->mfPositionX = fPosX;
		pNewRes->mfPositionY = fPosY;
		pNewRes->mfWidth = fWidth;
		pNewRes->mfHeight = fHeight;
		_mmapProgressBars[strName] = pNewRes;
		return pNewRes;
	}

	CGUIProgressBar* CGUIContainer::getProgressBar(const std::string& strName) const
	{
		std::map<std::string, CGUIProgressBar*>::iterator it = _mmapProgressBars.find(strName);
		ThrowIfTrue(it == _mmapProgressBars.end(), "CGUIContainer::getProgressBar(" + strName + ") failed. The named object doesn't exist.");
		return it->second;
	}

	void CGUIContainer::removeProgressBar(const std::string& strName)
	{
		std::map<std::string, CGUIProgressBar*>::iterator it = _mmapProgressBars.find(strName);
		if (it == _mmapProgressBars.end())
			return;
		delete it->second;
		_mmapProgressBars.erase(it);
	}

	void CGUIContainer::removeAllProgressBars(void)
	{
		std::map<std::string, CGUIProgressBar*>::iterator it = _mmapProgressBars.begin();
		while (it != _mmapProgressBars.end())
		{
			delete it->second;
			_mmapProgressBars.erase(it);
			it = _mmapProgressBars.begin();
		}
	}

	CGUIImage* CGUIContainer::addImage(const std::string& strName, float fPosX, float fPosY, const std::string& strImageFilename, float fWidth, float fHeight)
	{
		// If resource already exists
		std::map<std::string, CGUIImage*>::iterator it = _mmapImages.find(strName);
		ThrowIfTrue(it != _mmapImages.end(), "CGUIContainer::addImage(" + strName + ") failed. The named object already exists.");
		CGUIImage* pNewRes = new CGUIImage;
		ThrowIfFalse(pNewRes, "CGUIContainer::addImage(" + strName + ") failed. Could not allocate memory for the new object.");
		pNewRes->mfPositionX = fPosX;
		pNewRes->mfPositionY = fPosY;
		pNewRes->mfWidth = fWidth;
		pNewRes->mfHeight = fHeight;
		pNewRes->_mstrTextureResourceName = strImageFilename;
		pNewRes->_mbImageIsFromFile = true;
		_mmapImages[strName] = pNewRes;

		// Add strImageFilename to the resource manager
		SCResourceManager* pResMan = SCResourceManager::getPointer();
		CResourceTexture2DFromFile *pTex = pResMan->addTexture2DFromFile(strImageFilename, strImageFilename);

		// If a value less than 0 is passed to width/height, set widget to dims of the image
		if (fWidth < 0)
			pNewRes->mfWidth = pTex->getDimensions().x;
		if (fHeight < 0)
			pNewRes->mfHeight = pTex->getDimensions().y;

		return pNewRes;
	}

	CGUIImage* CGUIContainer::addImageFromImage(const std::string& strName, float fPosX, float fPosY, const std::string& strCResourceTexture2DFromImage, float fWidth, float fHeight)
	{
		// If resource already exists
		std::map<std::string, CGUIImage*>::iterator it = _mmapImages.find(strName);
		ThrowIfTrue(it != _mmapImages.end(), "CGUIContainer::addImageFromImage(" + strName + ") failed. The named object already exists.");
		CGUIImage* pNewRes = new CGUIImage;
		ThrowIfFalse(pNewRes, "CGUIContainer::addImageFromImage(" + strName + ") failed. Could not allocate memory for the new object.");
		pNewRes->mfPositionX = fPosX;
		pNewRes->mfPositionY = fPosY;
		pNewRes->mfWidth = fWidth;
		pNewRes->mfHeight = fHeight;
		pNewRes->_mstrTextureResourceName = strCResourceTexture2DFromImage;
		pNewRes->_mbImageIsFromFile = false;
		_mmapImages[strName] = pNewRes;

		// Get image from the resource manager
		SCResourceManager* pResMan = SCResourceManager::getPointer();
		CResourceTexture2DFromImage* pTex = pResMan->getTexture2DFromImage(strCResourceTexture2DFromImage);

		// If a value less than 0 is passed to width/height, set widget to dims of the image
		if (fWidth < 0)
			pNewRes->mfWidth = pTex->getDimensions().x;
		if (fHeight < 0)
			pNewRes->mfHeight = pTex->getDimensions().y;

		return pNewRes;
	}

	CGUIImage* CGUIContainer::getImage(const std::string& strName) const
	{
		std::map<std::string, CGUIImage*>::iterator it = _mmapImages.find(strName);
		ThrowIfTrue(it == _mmapImages.end(), "CGUIContainer::getImage(" + strName + ") failed. The named object doesn't exist.");
		return it->second;
	}

	void CGUIContainer::removeImage(const std::string& strName)
	{
		std::map<std::string, CGUIImage*>::iterator it = _mmapImages.find(strName);
		if (it == _mmapImages.end())
			return;

		// Remove _mstrTextureResourceName from the resource manager
		if (it->second->_mbImageIsFromFile)
		{
			SCResourceManager* pResMan = SCResourceManager::getPointer();
			pResMan->removeTexture2DFromFile(it->second->_mstrTextureResourceName);
		}

		delete it->second;
		_mmapImages.erase(it);
	}

	void CGUIContainer::removeAllImages(void)
	{
		std::map<std::string, CGUIImage*>::iterator it = _mmapImages.begin();
		SCResourceManager* pResMan = SCResourceManager::getPointer();
		while (it != _mmapImages.end())
		{
			// Remove _mstrTextureResourceName from the resource manager
			if (it->second->_mbImageIsFromFile)
			{
				pResMan->removeTexture2DFromFile(it->second->_mstrTextureResourceName);
			}
			delete it->second;
			_mmapImages.erase(it);
			it = _mmapImages.begin();
		}
	}

	CGUIImageAnimated* CGUIContainer::addImageAnimated(const std::string& strName, float fPosX, float fPosY, const std::vector<std::string>& vecStrImageFilenames, float fWidth, float fHeight)
	{
		// If resource already exists
		std::map<std::string, CGUIImageAnimated*>::iterator it = _mmapImageAnimateds.find(strName);
		ThrowIfTrue(it != _mmapImageAnimateds.end(), "CGUIContainer::addImageAnimated(" + strName + ") failed. The named object already exists.");
		CGUIImageAnimated* pNewRes = new CGUIImageAnimated;
		ThrowIfFalse(pNewRes, "CGUIContainer::addImageAnimated(" + strName + ") failed. Could not allocate memory for the new object.");
		pNewRes->mfPositionX = fPosX;
		pNewRes->mfPositionY = fPosY;
		pNewRes->mfWidth = fWidth;
		pNewRes->mfHeight = fHeight;
		pNewRes->_mstrResourceTexture2DAtlasName = strName;
		_mmapImageAnimateds[strName] = pNewRes;

		// Add CResourceTexture2DAnimation to the resource manager
		SCResourceManager* pResMan = SCResourceManager::getPointer();
		CResourceTexture2DAtlas* pTex = pResMan->addTexture2DAtlas(strName, vecStrImageFilenames, false, 1);

		// If a value less than 0 is passed to width/height, set widget to dims of the image
		if (fWidth < 0)
			pNewRes->mfWidth = (float)pTex->getImageDims(0).x;
		if (fHeight < 0)
			pNewRes->mfHeight = (float)pTex->getImageDims(0).y;

		return pNewRes;
	}

	CGUIImageAnimated* CGUIContainer::getImageAnimated(const std::string& strName) const
	{
		std::map<std::string, CGUIImageAnimated*>::iterator it = _mmapImageAnimateds.find(strName);
		ThrowIfTrue(it == _mmapImageAnimateds.end(), "CGUIContainer::getImageAnimated(" + strName + ") failed. The named object doesn't exist.");
		return it->second;
	}

	void CGUIContainer::removeImageAnimated(const std::string& strName)
	{
		std::map<std::string, CGUIImageAnimated*>::iterator it = _mmapImageAnimateds.find(strName);
		if (it == _mmapImageAnimateds.end())
			return;

		// Remove CResourceTexture2DAtlas from the resource manager
		SCResourceManager* pResMan = SCResourceManager::getPointer();
		pResMan->removeTexture2DAtlas(it->second->_mstrResourceTexture2DAtlasName);

		delete it->second;
		_mmapImageAnimateds.erase(it);
	}

	void CGUIContainer::removeAllImageAnimateds(void)
	{
		std::map<std::string, CGUIImageAnimated*>::iterator it = _mmapImageAnimateds.begin();
		while (it != _mmapImageAnimateds.end())
		{
			delete it->second;
			_mmapImageAnimateds.erase(it);
			it = _mmapImageAnimateds.begin();
		}
	}

	CGUIImageFramebuffer* CGUIContainer::addImageFramebuffer(const std::string& strName, float fPosX, float fPosY, const std::string& strFBname, float fWidth, float fHeight)
	{
		// If resource already exists
		std::map<std::string, CGUIImageFramebuffer*>::iterator it = _mmapImageFramebuffers.find(strName);
		ThrowIfTrue(it != _mmapImageFramebuffers.end(), "CGUIContainer::addImageFramebuffer(" + strName + ") failed. The named object already exists.");
		CGUIImageFramebuffer* pNewRes = new CGUIImageFramebuffer;
		ThrowIfFalse(pNewRes, "CGUIContainer::addImageFramebuffer(" + strName + ") failed. Could not allocate memory for the new object.");
		pNewRes->mfPositionX = fPosX;
		pNewRes->mfPositionY = fPosY;
		pNewRes->mfWidth = fWidth;
		pNewRes->mfHeight = fHeight;
		pNewRes->_mstrFBname = strFBname;
		_mmapImageFramebuffers[strName] = pNewRes;

		// Get CResourceFramebuffer from the resource manager to set size of this object
		SCResourceManager* pResMan = SCResourceManager::getPointer();
		CResourceFramebuffer* pFB = pResMan->getFramebuffer(strFBname);

		// If a value less than 0 is passed to width/height, set widget to dims of the image
		if (fWidth < 0)
			pNewRes->mfWidth = (float)pFB->getWidth();
		if (fHeight < 0)
			pNewRes->mfHeight = (float)pFB->getHeight();

		return pNewRes;
	}

	CGUIImageFramebuffer* CGUIContainer::getImageFramebuffer(const std::string& strName) const
	{
		std::map<std::string, CGUIImageFramebuffer*>::iterator it = _mmapImageFramebuffers.find(strName);
		ThrowIfTrue(it == _mmapImageFramebuffers.end(), "CGUIContainer::getImageFramebuffer(" + strName + ") failed. The named object doesn't exist.");
		return it->second;
	}

	void CGUIContainer::removeImageFramebuffer(const std::string& strName)
	{
		std::map<std::string, CGUIImageFramebuffer*>::iterator it = _mmapImageFramebuffers.find(strName);
		if (it == _mmapImageFramebuffers.end())
			return;

		delete it->second;
		_mmapImageFramebuffers.erase(it);
	}

	void CGUIContainer::removeAllImageFramebuffers(void)
	{
		std::map<std::string, CGUIImageFramebuffer*>::iterator it = _mmapImageFramebuffers.begin();
		while (it != _mmapImageFramebuffers.end())
		{
			delete it->second;
			_mmapImageFramebuffers.erase(it);
			it = _mmapImageFramebuffers.begin();
		}
	}

	CGUITextScroll* CGUIContainer::addTextScroll(const std::string& strName, float fPosX, float fPosY, float fWidth, float fHeight, const std::string& strText)
	{
		// If resource already exists
		std::map<std::string, CGUITextScroll*>::iterator it = _mmapTextScrolls.find(strName);
		ThrowIfTrue(it != _mmapTextScrolls.end(), "CGUIContainer::addTextScroll(" + strName + ") failed. The named object already exists.");
		CGUITextScroll* pNewRes = new CGUITextScroll;
		ThrowIfFalse(pNewRes, "CGUIContainer::addTextScroll(" + strName + ") failed. Could not allocate memory for the new object.");
		pNewRes->mfPositionX = fPosX;
		pNewRes->mfPositionY = fPosY;
		pNewRes->mfWidth = fWidth;
		pNewRes->mfHeight = fHeight;
		pNewRes->_mstrText = strText;
		// Create name of framebuffer
		pNewRes->_mstrFBName = "GUITextScrollFB_" + _mstrName + "_" + strName;

		// Check to see if the framebuffer resource name already exists
		SCResourceManager* pRM = SCResourceManager::getPointer();
		ThrowIfTrue(pRM->getFramebufferExists(pNewRes->_mstrFBName), "CGUIContainer::addTextScroll(" + strName + ") failed. The name must be unique for all TextScroll objects.");

		// Create framebuffer in SCResourceManager
		pRM->addFramebuffer(pNewRes->_mstrFBName, unsigned int(fWidth), unsigned int(fHeight));

		_mmapTextScrolls[strName] = pNewRes;
		return pNewRes;
	}

	CGUITextScroll* CGUIContainer::getTextScroll(const std::string& strName) const
	{
		std::map<std::string, CGUITextScroll*>::iterator it = _mmapTextScrolls.find(strName);
		ThrowIfTrue(it == _mmapTextScrolls.end(), "CGUIContainer::getTextScroll(" + strName + ") failed. The named object doesn't exist.");
		return it->second;
	}

	void CGUIContainer::removeTextScroll(const std::string& strName)
	{
		std::map<std::string, CGUITextScroll*>::iterator it = _mmapTextScrolls.find(strName);
		if (it == _mmapTextScrolls.end())
			return;

		// Remove framebuffer resource from resource manager
		SCResourceManager* pRM = SCResourceManager::getPointer();
		pRM->removeFramebuffer(it->second->_mstrFBName);

		delete it->second;
		_mmapTextScrolls.erase(it);
	}

	void CGUIContainer::removeAllTextScrolls(void)
	{
		std::map<std::string, CGUITextScroll*>::iterator it = _mmapTextScrolls.begin();
		while (it != _mmapTextScrolls.end())
		{
			delete it->second;
			_mmapTextScrolls.erase(it);
			it = _mmapTextScrolls.begin();
		}
	}

	CGUIButtonImage* CGUIContainer::addButtonImage(const std::string& strName, float fPosX, float fPosY, const std::string& strImageFilenameUp, const std::string& strImageFilenameOver, const std::string& strImageFilenameDown, float fWidth, float fHeight)
	{
		// If resource already exists
		std::map<std::string, CGUIButtonImage*>::iterator it = _mmapButtonImages.find(strName);
		ThrowIfTrue(it != _mmapButtonImages.end(), "CGUIContainer::addButtonImage(" + strName + ") failed. The named object already exists.");
		CGUIButtonImage* pNewRes = new CGUIButtonImage;
		ThrowIfFalse(pNewRes, "CGUIContainer::addButtonImage(" + strName + ") failed. Could not allocate memory for the new object.");
		pNewRes->mfPositionX = fPosX;
		pNewRes->mfPositionY = fPosY;
		pNewRes->mfWidth = fWidth;
		pNewRes->mfHeight = fHeight;
		pNewRes->_mstrTextureDown = strImageFilenameDown;
		pNewRes->_mstrTextureOver = strImageFilenameOver;
		pNewRes->_mstrTextureUp = strImageFilenameUp;
		_mmapButtonImages[strName] = pNewRes;

		// Add images to the resource manager
		SCResourceManager* pResMan = SCResourceManager::getPointer();
		CResourceTexture2DFromFile* pTex = pResMan->addTexture2DFromFile(pNewRes->_mstrTextureDown, pNewRes->_mstrTextureDown);
		pTex = pResMan->addTexture2DFromFile(pNewRes->_mstrTextureOver, pNewRes->_mstrTextureOver);
		pTex = pResMan->addTexture2DFromFile(pNewRes->_mstrTextureUp, pNewRes->_mstrTextureUp);

		// If a value less than 0 is passed to width/height, set widget to dims of the image
		if (fWidth < 0)
			pNewRes->mfWidth = pTex->getDimensions().x;
		if (fHeight < 0)
			pNewRes->mfHeight = pTex->getDimensions().y;

		return pNewRes;
	}

	CGUIButtonImage* CGUIContainer::getButtonImage(const std::string& strName) const
	{
		std::map<std::string, CGUIButtonImage*>::iterator it = _mmapButtonImages.find(strName);
		ThrowIfTrue(it == _mmapButtonImages.end(), "CGUIContainer::getButtonImage(" + strName + ") failed. The named object doesn't exist.");
		return it->second;
	}

	void CGUIContainer::removeButtonImage(const std::string& strName)
	{
		std::map<std::string, CGUIButtonImage*>::iterator it = _mmapButtonImages.find(strName);
		if (it == _mmapButtonImages.end())
			return;

		// Remove images from the resource manager
		SCResourceManager* pResMan = SCResourceManager::getPointer();
		pResMan->removeTexture2DFromFile(it->second->_mstrTextureDown);
		pResMan->removeTexture2DFromFile(it->second->_mstrTextureOver);
		pResMan->removeTexture2DFromFile(it->second->_mstrTextureUp);

		delete it->second;
		_mmapButtonImages.erase(it);
	}

	void CGUIContainer::removeAllButtonImages(void)
	{
		std::map<std::string, CGUIButtonImage*>::iterator it = _mmapButtonImages.begin();
		while (it != _mmapButtonImages.end())
		{
			delete it->second;
			_mmapButtonImages.erase(it);
			it = _mmapButtonImages.begin();
		}
	}

	CGUIImageDepthbuffer* CGUIContainer::addImageDepthbuffer(const std::string& strName, float fPosX, float fPosY, const std::string& strDBname, float fWidth, float fHeight)
	{
		// If resource already exists
		std::map<std::string, CGUIImageDepthbuffer*>::iterator it = _mmapImageDepthbuffers.find(strName);
		ThrowIfTrue(it != _mmapImageDepthbuffers.end(), "CGUIContainer::addImageDepthbuffer(" + strName + ") failed. The named object already exists.");
		CGUIImageDepthbuffer* pNewRes = new CGUIImageDepthbuffer;
		ThrowIfFalse(pNewRes, "CGUIContainer::addImageDepthbuffer(" + strName + ") failed. Could not allocate memory for the new object.");
		pNewRes->mfPositionX = fPosX;
		pNewRes->mfPositionY = fPosY;
		pNewRes->mfWidth = fWidth;
		pNewRes->mfHeight = fHeight;
		pNewRes->_mstrDBname = strDBname;
		_mmapImageDepthbuffers[strName] = pNewRes;

		// Get CResourceFramebuffer from the resource manager to set size of this object
		SCResourceManager* pResMan = SCResourceManager::getPointer();
		CResourceDepthbuffer* pDB = pResMan->getDepthbuffer(strDBname);

		// If a value less than 0 is passed to width/height, set widget to dims of the image
		if (fWidth < 0)
			pNewRes->mfWidth = (float)pDB->getWidth();
		if (fHeight < 0)
			pNewRes->mfHeight = (float)pDB->getHeight();

		return pNewRes;
	}

	CGUIImageDepthbuffer* CGUIContainer::getImageDepthbuffer(const std::string& strName) const
	{
		std::map<std::string, CGUIImageDepthbuffer*>::iterator it = _mmapImageDepthbuffers.find(strName);
		ThrowIfTrue(it == _mmapImageDepthbuffers.end(), "CGUIContainer::getImageDepthbuffer(" + strName + ") failed. The named object doesn't exist.");
		return it->second;
	}

	void CGUIContainer::removeImageDepthbuffer(const std::string& strName)
	{
		std::map<std::string, CGUIImageDepthbuffer*>::iterator it = _mmapImageDepthbuffers.find(strName);
		if (it == _mmapImageDepthbuffers.end())
			return;

		delete it->second;
		_mmapImageDepthbuffers.erase(it);
	}

	void CGUIContainer::removeAllImageDepthbuffers(void)
	{
		std::map<std::string, CGUIImageDepthbuffer*>::iterator it = _mmapImageDepthbuffers.begin();
		while (it != _mmapImageDepthbuffers.end())
		{
			delete it->second;
			_mmapImageDepthbuffers.erase(it);
			it = _mmapImageDepthbuffers.begin();
		}
	}

	void CGUIContainer::removeAll(void)
	{
		removeAllButtons();
		removeAllText();
		removeAllTextEdits();
		removeAllSliders();
		removeAllLineGraphs();
		removeAllProgressBars();
		removeAllImages();
		removeAllImageAnimateds();
		removeAllImageFramebuffers();
		removeAllTextScrolls();
		removeAllButtonImages();
		removeAllImageDepthbuffers();
	}
}