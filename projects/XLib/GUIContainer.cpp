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
	GUIContainer::GUIContainer()
	{
		mstrThemename = "default";
		mbContainerIsWindow = true;
		_mbWindowBeingMoved = false;
		_mbVisible = true;
		GUIColour col = GUIManager::getPointer()->getTheme("default")->mColours.containerTitlebarTextNotInFocus;
		_mvTextColour.r = col.red;
		_mvTextColour.g = col.green;
		_mvTextColour.b = col.blue;
		_mvTextColour.a = col.alpha;
	}

	void GUIContainer::render(const std::string& strFramebufferToSampleFrom)
	{
		if (!_mbVisible)
			return;

		// Render this container (If it's set as a window)
		_renderContainer(strFramebufferToSampleFrom);
	
		// Render each button
		std::map<std::string, GUIButton*>::iterator itButton = _mmapButtons.begin();
		while (itButton != _mmapButtons.end())
		{
			itButton->second->render(this, strFramebufferToSampleFrom);
			itButton++;
		}

		// Render each text
		std::map<std::string, GUIText*>::iterator itText = _mmapTexts.begin();
		while (itText != _mmapTexts.end())
		{
			itText->second->render(this);
			itText++;
		}

		// Render each text edit
		std::map<std::string, GUITextEdit*>::iterator itTextEdit = _mmapTextEdits.begin();
		while (itTextEdit != _mmapTextEdits.end())
		{
			itTextEdit->second->render(this, strFramebufferToSampleFrom);
			itTextEdit++;
		}

		// Render each sliders
		std::map<std::string, GUISlider*>::iterator itSlider = _mmapSliders.begin();
		while (itSlider != _mmapSliders.end())
		{
			itSlider->second->render(this, strFramebufferToSampleFrom);
			itSlider++;
		}

		// Render each line graph
		std::map<std::string, GUILineGraph*>::iterator itLineGraph = _mmapLineGraphs.begin();
		while (itLineGraph != _mmapLineGraphs.end())
		{
			itLineGraph->second->render(this, strFramebufferToSampleFrom);
			itLineGraph++;
		}

		// Render each progress bar
		std::map<std::string, GUIProgressBar*>::iterator itProgressBar = _mmapProgressBars.begin();
		while (itProgressBar != _mmapProgressBars.end())
		{
			itProgressBar->second->render(this, strFramebufferToSampleFrom);
			itProgressBar++;
		}

	}

	// Containers are updated in order of ZOrder, with the front most being updated first
	bool GUIContainer::update(bool bMouseIsOverContainerWhichIsAboveThisOne)
	{
		if (!_mbVisible)
			return false;

		ResourceManager* pRM = ResourceManager::getPointer();
		GUIManager* pGUI = GUIManager::getPointer();
		GUITheme* pTheme = pGUI->getTheme(mstrThemename);
		Window* pWindow = Window::getPointer();
		glm::vec2 vWindowDims = pWindow->getDimensions();

		InputManager* pInput = InputManager::getPointer();
		glm::vec2 vMousePos = pInput->mouse.getCursorPos();
		glm::vec2 vTexDimsDiv3 = pRM->getTexture2D(pTheme->mImages.containerBGColour)->mvDimensions * 0.3333333f;

		// Determine whether mouse cursor is over this container
		bool bMouseOver = false;
		if (!bMouseIsOverContainerWhichIsAboveThisOne)	// Only perform check if mouse is NOT over a container which is above this one
		{
			if (mbContainerIsWindow)
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
			if (mbContainerIsWindow)
			{
				if (pInput->mouse.leftButtonOnce())
				{
					// Set colour of all other containers
					std::map<std::string, GUIContainer*>::iterator itCont = pGUI->_mmapContainers.begin();
					while (itCont != pGUI->_mmapContainers.end())
					{
						itCont->second->_mvTextColour = glm::vec4(
							pTheme->mColours.containerTitlebarTextNotInFocus.red,
							pTheme->mColours.containerTitlebarTextNotInFocus.green,
							pTheme->mColours.containerTitlebarTextNotInFocus.blue,
							pTheme->mColours.containerTitlebarTextNotInFocus.alpha);
						itCont++;
					}
					// Set colour of titlebar text
					_mvTextColour = glm::vec4(
						pTheme->mColours.containerTitlebarTextInFocus.red,
						pTheme->mColours.containerTitlebarTextInFocus.green,
						pTheme->mColours.containerTitlebarTextInFocus.blue,
						pTheme->mColours.containerTitlebarTextInFocus.alpha);
					
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
		if (mbContainerIsWindow)
		{
			// Get a texture so we can determine dimensions
			ResourceTexture2D* pTexColour = pRM->getTexture2D(pTheme->mImages.containerBGColour);
			glm::vec2 vTexDimsDiv3 = pTexColour->mvDimensions * 0.3333333f;
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
		std::map<std::string, GUIButton*>::iterator itButton = _mmapButtons.begin();
		while (itButton != _mmapButtons.end())
		{
			itButton->second->update(this, bContainerAcceptingMouseClicks);
			itButton++;
		}

		// Text (We don't need to update anything

		// Text edit
		std::map<std::string, GUITextEdit*>::iterator itTextEdit = _mmapTextEdits.begin();
		while (itTextEdit != _mmapTextEdits.end())
		{
			itTextEdit->second->update(this, bContainerAcceptingMouseClicks);
			itTextEdit++;
		}

		// Sliders
		std::map<std::string, GUISlider*>::iterator itSlider = _mmapSliders.begin();
		while (itSlider != _mmapSliders.end())
		{
			itSlider->second->update(this, bContainerAcceptingMouseClicks);
			itSlider++;
		}

		// LineGraphs
		std::map<std::string, GUILineGraph*>::iterator itLineGraph = _mmapLineGraphs.begin();
		while (itLineGraph != _mmapLineGraphs.end())
		{
			itLineGraph->second->update(this, bContainerAcceptingMouseClicks);
			itLineGraph++;
		}

		// Progress bars
		std::map<std::string, GUIProgressBar*>::iterator itProgressBar = _mmapProgressBars.begin();
		while (itProgressBar != _mmapProgressBars.end())
		{
			itProgressBar->second->update(this, bContainerAcceptingMouseClicks);
			itProgressBar++;
		}

		return bMouseOver;
	}

	GUIButton* GUIContainer::addButton(const std::string& strName, float fPosX, float fPosY, float fWidth, float fHeight, const std::string& strText)
	{
		// If resource already exists
		std::map<std::string, GUIButton*>::iterator it = _mmapButtons.find(strName);
		ThrowIfTrue(it != _mmapButtons.end(), "GUIContainer::addButton(" + strName + ") failed. The named object already exists.");
		GUIButton* pNewRes = new GUIButton;
		ThrowIfFalse(pNewRes, "GUIContainer::addButton(" + strName + ") failed. Could not allocate memory for the new object.");
		pNewRes->mfPositionX = fPosX;
		pNewRes->mfPositionY = fPosY;
		pNewRes->mfWidth = fWidth;
		pNewRes->mfHeight = fHeight;
		pNewRes->mstrText = strText;
		_mmapButtons[strName] = pNewRes;
		return pNewRes;
	}

	GUIButton* GUIContainer::getButton(const std::string& strName)
	{
		std::map<std::string, GUIButton*>::iterator it = _mmapButtons.find(strName);
		ThrowIfTrue(it == _mmapButtons.end(), "GUIContainer::getButton(" + strName + ") failed. The named object doesn't exist.");
		return it->second;
	}

	void GUIContainer::removeButton(const std::string& strName)
	{
		std::map<std::string, GUIButton*>::iterator it = _mmapButtons.find(strName);
		if (it == _mmapButtons.end())
			return;
		delete it->second;
		_mmapButtons.erase(it);
	}

	GUIText* GUIContainer::addText(const std::string& strName, float fPosX, float fPosY, const std::string& strText)
	{
		// If resource already exists
		std::map<std::string, GUIText*>::iterator it = _mmapTexts.find(strName);
		ThrowIfTrue(it != _mmapTexts.end(), "GUIContainer::addText(" + strName + ") failed. The named object already exists.");
		GUIText* pNewRes = new GUIText;
		ThrowIfFalse(pNewRes, "GUIContainer::addText(" + strName + ") failed. Could not allocate memory for the new object.");
		pNewRes->mfPositionX = fPosX;
		pNewRes->mfPositionY = fPosY;
		pNewRes->mstrText = strText;
		_mmapTexts[strName] = pNewRes;
		return pNewRes;
	}

	GUIText* GUIContainer::getText(const std::string& strName)
	{
		std::map<std::string, GUIText*>::iterator it = _mmapTexts.find(strName);
		ThrowIfTrue(it == _mmapTexts.end(), "GUIContainer::getText(" + strName + ") failed. The named object doesn't exist.");
		return it->second;
	}

	void GUIContainer::removeText(const std::string& strName)
	{
		std::map<std::string, GUIText*>::iterator it = _mmapTexts.find(strName);
		if (it == _mmapTexts.end())
			return;
		delete it->second;
		_mmapTexts.erase(it);
	}

	void GUIContainer::setVisible(bool bVisible)
	{
		_mbVisible = bVisible;
	}

	bool GUIContainer::getVisible(void)
	{
		return _mbVisible;
	}

	void GUIContainer::_renderContainer(const std::string& strFramebufferToSampleFrom)
	{
		if (!_mbVisible)
			return;

		if (!mbContainerIsWindow)
			return;	// No point rendering anything

		// Get required resources needed to render
		GUIManager* pGUI = GUIManager::getPointer();
		ResourceManager* pRM = ResourceManager::getPointer();
		Window* pWindow = Window::getPointer();
		ResourceTriangle* pTri = pRM->getTriangle("X:gui");
		ResourceShader* pShader = pRM->getShader("X:gui");
		GUITheme* pTheme = pGUI->getTheme(mstrThemename);
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

		// Get textures and background sample framebuffer
		ResourceTexture2D* pTexColour = pRM->getTexture2D(pTheme->mImages.containerBGColour);
		ResourceTexture2D* pTexNormal = pRM->getTexture2D(pTheme->mImages.containerBGNormal);
		ResourceTexture2D* pTexReflection = pRM->getTexture2D(pTheme->mImages.reflection);
		ResourceFramebuffer* pFBSample = pRM->getFramebuffer(strFramebufferToSampleFrom);

		// Bind textures
		pTexColour->bind(0);
		pTexNormal->bind(1);
		pTexReflection->bind(2);
		pFBSample->bindAsTexture(3);

		// Render the container centre
		pTri->removeGeom();
		pTri->addQuad2D(
			glm::vec2(mfPositionX, mfPositionY),	// Position
			glm::vec2(mfWidth, mfHeight),			// Dimensions
			glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),		// Vertex colour
			mTC.centre.BL,
			mTC.centre.BR,
			mTC.centre.TR,
			mTC.centre.TL);

		// Render the left edge
		glm::vec2 vTexDimsDiv3 = pTexColour->mvDimensions * 0.3333333f;
		pTri->addQuad2D(
			glm::vec2(mfPositionX - vTexDimsDiv3.x, mfPositionY),	// Position
			glm::vec2(vTexDimsDiv3.x, mfHeight),					// Dimensions
			glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),						// Vertex colour
			mTC.left.BL,
			mTC.left.BR,
			mTC.left.TR,
			mTC.left.TL);

		// Render the right edge
		pTri->addQuad2D(
			glm::vec2(mfPositionX + mfWidth, mfPositionY),	// Position
			glm::vec2(vTexDimsDiv3.x, mfHeight),			// Dimensions
			glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),				// Vertex colour
			mTC.right.BL,
			mTC.right.BR,
			mTC.right.TR,
			mTC.right.TL);

		// Render the top edge
		pTri->addQuad2D(
			glm::vec2(mfPositionX, mfPositionY - vTexDimsDiv3.y),	// Position
			glm::vec2(mfWidth, vTexDimsDiv3.y),						// Dimensions
			glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),						// Vertex colour
			mTC.top.BL,
			mTC.top.BR,
			mTC.top.TR,
			mTC.top.TL);

		// Render the bottom edge
		pTri->addQuad2D(
			glm::vec2(mfPositionX, mfPositionY + mfHeight),		// Position
			glm::vec2(mfWidth, vTexDimsDiv3.y),					// Dimensions
			glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),					// Vertex colour
			mTC.bottom.BL,
			mTC.bottom.BR,
			mTC.bottom.TR,
			mTC.bottom.TL);

		// Render the top left corner
		pTri->addQuad2D(
			glm::vec2(mfPositionX - vTexDimsDiv3.x, mfPositionY - vTexDimsDiv3.y),	// Position
			glm::vec2(vTexDimsDiv3.x, vTexDimsDiv3.y),								// Dimensions
			glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),										// Vertex colour
			mTC.topLeft.BL,
			mTC.topLeft.BR,
			mTC.topLeft.TR,
			mTC.topLeft.TL);

		// Render the top right corner
		pTri->addQuad2D(
			glm::vec2(mfPositionX + mfWidth, mfPositionY - vTexDimsDiv3.y),	// Position
			glm::vec2(vTexDimsDiv3.x, vTexDimsDiv3.y),						// Dimensions
			glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),								// Vertex colour
			mTC.topRight.BL,
			mTC.topRight.BR,
			mTC.topRight.TR,
			mTC.topRight.TL);

		// Render the bottom left corner
		pTri->addQuad2D(
			glm::vec2(mfPositionX - vTexDimsDiv3.x, mfPositionY + mfHeight),	// Position
			glm::vec2(vTexDimsDiv3.x, vTexDimsDiv3.y),							// Dimensions
			glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),									// Vertex colour
			mTC.bottomLeft.BL,
			mTC.bottomLeft.BR,
			mTC.bottomLeft.TR,
			mTC.bottomLeft.TL);

		// Render the bottom right corner
		pTri->addQuad2D(
			glm::vec2(mfPositionX + mfWidth, mfPositionY + mfHeight),	// Position
			glm::vec2(vTexDimsDiv3.x, vTexDimsDiv3.y),					// Dimensions
			glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),							// Vertex colour
			mTC.bottomRight.BL,
			mTC.bottomRight.BR,
			mTC.bottomRight.TR,
			mTC.bottomRight.TL);

		pTri->update();
		pTri->draw();

		pTexColour->unbindAll();	// Unbind textures
		pShader->unbind();	// Unbind the GUI shader
		glDisable(GL_BLEND);

		// Container title
		int iRTDims[2];
		iRTDims[0] = int(pWindow->getWidth());
		iRTDims[1] = int(pWindow->getHeight());
		ResourceFont* pFont = pRM->getFont(pTheme->mFonts.containerTitle);
		pFont->print(mstrTitleText,
			int(mfPositionX) + pTheme->mOffsets.containerTitlebarText.iOffsetX,
			int(mfPositionY - vTexDimsDiv3.y) + pTheme->mOffsets.containerTitlebarText.iOffsetY,
			iRTDims[0], iRTDims[1],
			1.0f,
			_mvTextColour);
	}

	GUITextEdit* GUIContainer::addTextEdit(const std::string& strName, float fPosX, float fPosY, float fWidth, float fHeight, const std::string& strText)
	{
		// If resource already exists
		std::map<std::string, GUITextEdit*>::iterator it = _mmapTextEdits.find(strName);
		ThrowIfTrue(it != _mmapTextEdits.end(), "GUIContainer::addTextEdit(" + strName + ") failed. The named object already exists.");
		GUITextEdit* pNewRes = new GUITextEdit;
		ThrowIfFalse(pNewRes, "GUIContainer::addTextEdit(" + strName + ") failed. Could not allocate memory for the new object.");
		pNewRes->mfPositionX = fPosX;
		pNewRes->mfPositionY = fPosY;
		pNewRes->mfWidth = fWidth;
		pNewRes->mfHeight = fHeight;
		pNewRes->mstrText = strText;
		_mmapTextEdits[strName] = pNewRes;
		return pNewRes;
	}

	GUITextEdit* GUIContainer::getTextEdit(const std::string& strName)
	{
		std::map<std::string, GUITextEdit*>::iterator it = _mmapTextEdits.find(strName);
		ThrowIfTrue(it == _mmapTextEdits.end(), "GUIContainer::getTextEdit(" + strName + ") failed. The named object doesn't exist.");
		return it->second;
	}

	void GUIContainer::removeTextEdit(const std::string& strName)
	{
		std::map<std::string, GUITextEdit*>::iterator it = _mmapTextEdits.find(strName);
		if (it == _mmapTextEdits.end())
			return;
		delete it->second;
		_mmapTextEdits.erase(it);
	}

	GUISlider* GUIContainer::addSlider(const std::string& strName, float fPosX, float fPosY, float fWidth, float fHeight, float fTabRatio)
	{
		// If resource already exists
		std::map<std::string, GUISlider*>::iterator it = _mmapSliders.find(strName);
		ThrowIfTrue(it != _mmapSliders.end(), "GUIContainer::addSlider(" + strName + ") failed. The named object already exists.");
		GUISlider* pNewRes = new GUISlider;
		ThrowIfFalse(pNewRes, "GUIContainer::addSlider(" + strName + ") failed. Could not allocate memory for the new object.");
		pNewRes->mfPositionX = fPosX;
		pNewRes->mfPositionY = fPosY;
		pNewRes->mfWidth = fWidth;
		pNewRes->mfHeight = fHeight;
		pNewRes->_mfTabRatio = fTabRatio;
		_mmapSliders[strName] = pNewRes;
		return pNewRes;
	}

	GUISlider* GUIContainer::getSlider(const std::string& strName)
	{
		std::map<std::string, GUISlider*>::iterator it = _mmapSliders.find(strName);
		ThrowIfTrue(it == _mmapSliders.end(), "GUIContainer::getSlider(" + strName + ") failed. The named object doesn't exist.");
		return it->second;
	}

	void GUIContainer::removeSlider(const std::string& strName)
	{
		std::map<std::string, GUISlider*>::iterator it = _mmapSliders.find(strName);
		if (it == _mmapSliders.end())
			return;
		delete it->second;
		_mmapSliders.erase(it);
	}

	GUILineGraph* GUIContainer::addLineGraph(const std::string& strName, float fPosX, float fPosY, float fWidth, float fHeight)
	{
		// If resource already exists
		std::map<std::string, GUILineGraph*>::iterator it = _mmapLineGraphs.find(strName);
		ThrowIfTrue(it != _mmapLineGraphs.end(), "GUIContainer::addLineGraph(" + strName + ") failed. The named object already exists.");
		GUILineGraph* pNewRes = new GUILineGraph;
		ThrowIfFalse(pNewRes, "GUIContainer::addLineGraph(" + strName + ") failed. Could not allocate memory for the new object.");
		pNewRes->mfPositionX = fPosX;
		pNewRes->mfPositionY = fPosY;
		pNewRes->mfWidth = fWidth;
		pNewRes->mfHeight = fHeight;
		_mmapLineGraphs[strName] = pNewRes;
		return pNewRes;
	}

	GUILineGraph* GUIContainer::getLineGraph(const std::string& strName)
	{
		std::map<std::string, GUILineGraph*>::iterator it = _mmapLineGraphs.find(strName);
		ThrowIfTrue(it == _mmapLineGraphs.end(), "GUIContainer::getLineGraph(" + strName + ") failed. The named object doesn't exist.");
		return it->second;
	}

	void GUIContainer::removeLineGraph(const std::string& strName)
	{
		std::map<std::string, GUILineGraph*>::iterator it = _mmapLineGraphs.find(strName);
		if (it == _mmapLineGraphs.end())
			return;
		delete it->second;
		_mmapLineGraphs.erase(it);
	}

	GUIProgressBar* GUIContainer::addProgressBar(const std::string& strName, float fPosX, float fPosY, float fWidth, float fHeight)
	{
		// If resource already exists
		std::map<std::string, GUIProgressBar*>::iterator it = _mmapProgressBars.find(strName);
		ThrowIfTrue(it != _mmapProgressBars.end(), "GUIContainer::addProgressBar(" + strName + ") failed. The named object already exists.");
		GUIProgressBar* pNewRes = new GUIProgressBar;
		ThrowIfFalse(pNewRes, "GUIContainer::addProgressBar(" + strName + ") failed. Could not allocate memory for the new object.");
		pNewRes->mfPositionX = fPosX;
		pNewRes->mfPositionY = fPosY;
		pNewRes->mfWidth = fWidth;
		pNewRes->mfHeight = fHeight;
		_mmapProgressBars[strName] = pNewRes;
		return pNewRes;
	}

	GUIProgressBar* GUIContainer::getProgressBar(const std::string& strName)
	{
		std::map<std::string, GUIProgressBar*>::iterator it = _mmapProgressBars.find(strName);
		ThrowIfTrue(it == _mmapProgressBars.end(), "GUIContainer::getProgressBar(" + strName + ") failed. The named object doesn't exist.");
		return it->second;
	}

	void GUIContainer::removeProgressBar(const std::string& strName)
	{
		std::map<std::string, GUIProgressBar*>::iterator it = _mmapProgressBars.find(strName);
		if (it == _mmapProgressBars.end())
			return;
		delete it->second;
		_mmapProgressBars.erase(it);
	}
}