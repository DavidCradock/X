#include "PCH.h"
#include "GUISubContainer.h"
#include "singletons.h"

namespace X
{
	CGUISubContainer::CGUISubContainer()
	{
		_mPreviousDims.set(-1, -1);
		_mbFBNeedsUpdating = true;
	}
	
	CGUISubContainer::~CGUISubContainer()
	{
		// Remove the framebuffer used from the resource manager
		x->pResource->removeFramebuffer(_mstrFBName);
	}

	void CGUISubContainer::render(void* pParentContainer, const std::string& strFramebufferToSampleFrom)
	{
		if (!_mbVisible)
			return;

		// Temp debug render borders of the sub container
		CResourceShader* pShaderLine = x->pResource->getShader(x->pResource->defaultRes.shader_VBCPT);
		CResourceVertexBufferLine* pVBLine = x->pResource->getVertexBufferLine(x->pResource->defaultRes.vertexbufferLine_default);
		CResourceTexture2DFromFile* pTexture = x->pResource->getTexture2DFromFile(x->pResource->defaultRes.texture2DFromFile_default_white);
		CGUIContainer* pContainer = (CGUIContainer*)pParentContainer;
		CGUITheme* pTheme = pContainer->getTheme();
		pShaderLine->bind();
		CMatrix matWorld, matView;
		CMatrix matProjection;
		matProjection.setProjectionOrthographic(0.0f, float(x->pWindow->getWidth()), 0.0f, float(x->pWindow->getHeight()), -1.0f, 1.0f);
		pShaderLine->setMat4("matrixWorld", matWorld);
		pShaderLine->setMat4("matrixView", matView);
		pShaderLine->setMat4("matrixProjection", matProjection);
		pShaderLine->setInt("texture0", 0);	// Tell OpenGL, for each sampler, to which texture unit it belongs to
		pTexture->bind();
		glDisable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		pVBLine->removeGeom();

		CVector2f vPos(pContainer->getPosition().x + (mfWidth * 0.5f), pContainer->getPosition().y + (mfHeight * 0.5f));
		vPos.x += mfPositionX;
		vPos.y += mfPositionY;
		CVector2f vDims(mfWidth, mfHeight);
		pVBLine->addQuad(vPos, mfWidth, mfHeight);
		pVBLine->update();
		pVBLine->render();
		pTexture->unbind();
		pShaderLine->unbind();

		CResourceFramebuffer* pFramebuffer = x->pResource->getFramebuffer(_mstrFBName);

		// Does the frame buffer need updating (Like when the OpenGL context has been lost)
		if (pFramebuffer->mbNeedsUpdating)
		{
			pFramebuffer->mbNeedsUpdating = false;
			_mbFBNeedsUpdating = true;
		}
		// Update the framebuffer if needed
		if (_mbFBNeedsUpdating)
			_renderFramebuffer(pParentContainer, strFramebufferToSampleFrom);

		// Get required resources needed to render
		CResourceVertexBufferCPT* pVB = x->pResource->getVertexBufferCPT("X:default");
		CResourceShader* pShader = x->pResource->getShader("X:VBCPT");

		// Render the frame buffer over the top of the background
		pShader->bind();

		// Setup the matrices
//		CMatrix matWorld, matView;
//		CMatrix matProjection;
//		matProjection.setProjectionOrthographic(0.0f, float(x->pWindow->getWidth()), 0.0f, float(x->pWindow->getHeight()), -1.0f, 1.0f);
		pShader->setMat4("matrixWorld", matWorld);
		pShader->setMat4("matrixView", matView);
		pShader->setMat4("matrixProjection", matProjection);

		// Tell OpenGL, for each sampler, to which texture unit it belongs to
		pShader->setInt("texture0", 0);

		glDisable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Bind textures
		pFramebuffer->bindAsTexture(0);

		// Render the image
		pVB->removeGeom();
		pVB->addQuad2D(
			CVector2f(pContainer->mfPositionX + mfPositionX, pContainer->mfPositionY + mfPositionY),	// Position
			CVector2f(mfWidth, mfHeight),			// Dimensions
			CColour(1.0f, 1.0f, 1.0f, 1.0f),		// Vertex colour
			CVector2f(0.0f, 0.0f),
			CVector2f(1.0f, 0.0f),
			CVector2f(1.0f, 1.0f),
			CVector2f(0.0f, 1.0f));

		pVB->update();
		pVB->render();

		pFramebuffer->unbindTexture();	// Unbind textures
		pShader->unbind();				// Unbind the GUI shader
	}

	void CGUISubContainer::update(void* pParentContainer, bool bParentContainerAcceptingMouseClicks)
	{
		// Buttons
		std::map<std::string, CGUIButton*>::iterator itButton = _mmapButtons.begin();
		while (itButton != _mmapButtons.end())
		{
			itButton->second->update(pParentContainer, bParentContainerAcceptingMouseClicks);
			itButton++;
		}

		// Text
		std::map<std::string, CGUIText*>::iterator itText = _mmapTexts.begin();
		while (itText != _mmapTexts.end())
		{
			itText->second->update(pParentContainer, bParentContainerAcceptingMouseClicks);
			itText++;
		}

		// Text edit
		std::map<std::string, CGUITextEdit*>::iterator itTextEdit = _mmapTextEdits.begin();
		while (itTextEdit != _mmapTextEdits.end())
		{
			itTextEdit->second->update(pParentContainer, bParentContainerAcceptingMouseClicks);
			itTextEdit++;
		}

		// Sliders
		std::map<std::string, CGUISlider*>::iterator itSlider = _mmapSliders.begin();
		while (itSlider != _mmapSliders.end())
		{
			itSlider->second->update(pParentContainer, bParentContainerAcceptingMouseClicks);
			itSlider++;
		}

		// LineGraphs
		std::map<std::string, CGUILineGraph*>::iterator itLineGraph = _mmapLineGraphs.begin();
		while (itLineGraph != _mmapLineGraphs.end())
		{
			itLineGraph->second->update(pParentContainer, bParentContainerAcceptingMouseClicks);
			itLineGraph++;
		}

		// Progress bars
		std::map<std::string, CGUIProgressBar*>::iterator itProgressBar = _mmapProgressBars.begin();
		while (itProgressBar != _mmapProgressBars.end())
		{
			itProgressBar->second->update(pParentContainer, bParentContainerAcceptingMouseClicks);
			itProgressBar++;
		}

		// Images
		std::map<std::string, CGUIImage*>::iterator itImage = _mmapImages.begin();
		while (itImage != _mmapImages.end())
		{
			itImage->second->update(pParentContainer, bParentContainerAcceptingMouseClicks);
			itImage++;
		}

		// Images animated
		std::map<std::string, CGUIImageAnimated*>::iterator itImageAnimated = _mmapImageAnimateds.begin();
		while (itImageAnimated != _mmapImageAnimateds.end())
		{
			itImageAnimated->second->update(pParentContainer, bParentContainerAcceptingMouseClicks);
			itImageAnimated++;
		}

		// Images framebuffer
		std::map<std::string, CGUIImageFramebuffer*>::iterator itImageFB = _mmapImageFramebuffers.begin();
		while (itImageFB != _mmapImageFramebuffers.end())
		{
			itImageFB->second->update(pParentContainer, bParentContainerAcceptingMouseClicks);
			itImageFB++;
		}

		// Text scroll objects
		std::map<std::string, CGUITextScroll*>::iterator itTextScroll = _mmapTextScrolls.begin();
		while (itTextScroll != _mmapTextScrolls.end())
		{
			itTextScroll->second->update(pParentContainer, bParentContainerAcceptingMouseClicks);
			itTextScroll++;
		}

		// Button images
		std::map<std::string, CGUIButtonImage*>::iterator itButtonImage = _mmapButtonImages.begin();
		while (itButtonImage != _mmapButtonImages.end())
		{
			itButtonImage->second->update(pParentContainer, bParentContainerAcceptingMouseClicks);
			itButtonImage++;
		}

		// Images depth buffer
		std::map<std::string, CGUIImageDepthbuffer*>::iterator itImageDB = _mmapImageDepthbuffers.begin();
		while (itImageDB != _mmapImageDepthbuffers.end())
		{
			itImageDB->second->update(pParentContainer, bParentContainerAcceptingMouseClicks);
			itImageDB++;
		}

		// Checkboxes
		std::map<std::string, CGUICheckbox*>::iterator itCheckbox = _mmapCheckboxes.begin();
		while (itCheckbox != _mmapCheckboxes.end())
		{
			itCheckbox->second->update(pParentContainer, bParentContainerAcceptingMouseClicks);
			itCheckbox++;
		}

		// Sub containers
		std::map<std::string, CGUISubContainer*>::iterator itSubContainer = _mmapSubContainers.begin();
		while (itSubContainer != _mmapSubContainers.end())
		{
			itSubContainer->second->update(pParentContainer, bParentContainerAcceptingMouseClicks);
			itSubContainer++;
		}
	}

	void CGUISubContainer::removeAll(void)
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
		removeAllCheckboxes();
		removeAllSubContainers();
	}

	/**************************************************************************************************************************************************
	Button
	**************************************************************************************************************************************************/

	CGUIButton* CGUISubContainer::addButton(const std::string& strName, float fPosX, float fPosY, float fWidth, float fHeight, const std::string& strText)
	{
		// If resource already exists
		std::map<std::string, CGUIButton*>::iterator it = _mmapButtons.find(strName);
		ThrowIfTrue(it != _mmapButtons.end(), "CGUISubContainer::addButton(" + strName + ") failed. The named object already exists.");
		CGUIButton* pNewRes = new CGUIButton;
		ThrowIfFalse(pNewRes, "CGUISubContainer::addButton(" + strName + ") failed. Could not allocate memory for the new object.");
		pNewRes->mfPositionX = fPosX;
		pNewRes->mfPositionY = fPosY;
		pNewRes->mfWidth = fWidth;
		pNewRes->mfHeight = fHeight;
		pNewRes->mstrText = strText;
		_mmapButtons[strName] = pNewRes;
		return pNewRes;
	}

	CGUIButton* CGUISubContainer::getButton(const std::string& strName) const
	{
		std::map<std::string, CGUIButton*>::iterator it = _mmapButtons.find(strName);
		ThrowIfTrue(it == _mmapButtons.end(), "CGUISubContainer::getButton(" + strName + ") failed. The named object doesn't exist.");
		return it->second;
	}

	void CGUISubContainer::removeButton(const std::string& strName)
	{
		std::map<std::string, CGUIButton*>::iterator it = _mmapButtons.find(strName);
		if (it == _mmapButtons.end())
			return;
		delete it->second;
		_mmapButtons.erase(it);
	}

	void CGUISubContainer::removeAllButtons(void)
	{
		std::map<std::string, CGUIButton*>::iterator it = _mmapButtons.begin();
		while (it != _mmapButtons.end())
		{
			delete it->second;
			_mmapButtons.erase(it);
			it = _mmapButtons.begin();
		}
	}

	/**************************************************************************************************************************************************
	Text
	**************************************************************************************************************************************************/

	CGUIText* CGUISubContainer::addText(const std::string& strName, float fPosX, float fPosY, const std::string& strText)
	{
		// If resource already exists
		std::map<std::string, CGUIText*>::iterator it = _mmapTexts.find(strName);
		ThrowIfTrue(it != _mmapTexts.end(), "CGUISubContainer::addText(" + strName + ") failed. The named object already exists.");
		CGUIText* pNewRes = new CGUIText;
		ThrowIfFalse(pNewRes, "CGUISubContainer::addText(" + strName + ") failed. Could not allocate memory for the new object.");
		pNewRes->mfPositionX = fPosX;
		pNewRes->mfPositionY = fPosY;
		pNewRes->setText(strText);
		_mmapTexts[strName] = pNewRes;
		return pNewRes;
	}

	CGUIText* CGUISubContainer::getText(const std::string& strName) const
	{
		std::map<std::string, CGUIText*>::iterator it = _mmapTexts.find(strName);
		ThrowIfTrue(it == _mmapTexts.end(), "CGUISubContainer::getText(" + strName + ") failed. The named object doesn't exist.");
		return it->second;
	}

	void CGUISubContainer::removeText(const std::string& strName)
	{
		std::map<std::string, CGUIText*>::iterator it = _mmapTexts.find(strName);
		if (it == _mmapTexts.end())
			return;
		delete it->second;
		_mmapTexts.erase(it);
	}

	void CGUISubContainer::removeAllText(void)
	{
		std::map<std::string, CGUIText*>::iterator it = _mmapTexts.begin();
		while (it != _mmapTexts.end())
		{
			delete it->second;
			_mmapTexts.erase(it);
			it = _mmapTexts.begin();
		}
	}

	/**************************************************************************************************************************************************
	Text edit
	**************************************************************************************************************************************************/

	CGUITextEdit* CGUISubContainer::addTextEdit(const std::string& strName, float fPosX, float fPosY, float fWidth, float fHeight, const std::string& strText)
	{
		// If resource already exists
		std::map<std::string, CGUITextEdit*>::iterator it = _mmapTextEdits.find(strName);
		ThrowIfTrue(it != _mmapTextEdits.end(), "CGUISubContainer::addTextEdit(" + strName + ") failed. The named object already exists.");
		CGUITextEdit* pNewRes = new CGUITextEdit;
		ThrowIfFalse(pNewRes, "CGUISubContainer::addTextEdit(" + strName + ") failed. Could not allocate memory for the new object.");
		pNewRes->mfPositionX = fPosX;
		pNewRes->mfPositionY = fPosY;
		pNewRes->mfWidth = fWidth;
		pNewRes->mfHeight = fHeight;
		pNewRes->setText(strText);
		_mmapTextEdits[strName] = pNewRes;
		return pNewRes;
	}

	CGUITextEdit* CGUISubContainer::getTextEdit(const std::string& strName) const
	{
		std::map<std::string, CGUITextEdit*>::iterator it = _mmapTextEdits.find(strName);
		ThrowIfTrue(it == _mmapTextEdits.end(), "CGUISubContainer::getTextEdit(" + strName + ") failed. The named object doesn't exist.");
		return it->second;
	}

	void CGUISubContainer::removeTextEdit(const std::string& strName)
	{
		std::map<std::string, CGUITextEdit*>::iterator it = _mmapTextEdits.find(strName);
		if (it == _mmapTextEdits.end())
			return;
		delete it->second;
		_mmapTextEdits.erase(it);
	}

	void CGUISubContainer::removeAllTextEdits(void)
	{
		std::map<std::string, CGUITextEdit*>::iterator it = _mmapTextEdits.begin();
		while (it != _mmapTextEdits.end())
		{
			delete it->second;
			_mmapTextEdits.erase(it);
			it = _mmapTextEdits.begin();
		}
	}

	/**************************************************************************************************************************************************
	Slider
	**************************************************************************************************************************************************/

	CGUISlider* CGUISubContainer::addSlider(const std::string& strName, float fPosX, float fPosY, float fWidth, float fHeight, float fTabRatio)
	{
		// If resource already exists
		std::map<std::string, CGUISlider*>::iterator it = _mmapSliders.find(strName);
		ThrowIfTrue(it != _mmapSliders.end(), "CGUISubContainer::addSlider(" + strName + ") failed. The named object already exists.");
		CGUISlider* pNewRes = new CGUISlider;
		ThrowIfFalse(pNewRes, "CGUISubContainer::addSlider(" + strName + ") failed. Could not allocate memory for the new object.");
		pNewRes->mfPositionX = fPosX;
		pNewRes->mfPositionY = fPosY;
		pNewRes->mfWidth = fWidth;
		pNewRes->mfHeight = fHeight;
		pNewRes->setTabRatio(fTabRatio);
		_mmapSliders[strName] = pNewRes;
		return pNewRes;
	}

	CGUISlider* CGUISubContainer::getSlider(const std::string& strName) const
	{
		std::map<std::string, CGUISlider*>::iterator it = _mmapSliders.find(strName);
		ThrowIfTrue(it == _mmapSliders.end(), "CGUISubContainer::getSlider(" + strName + ") failed. The named object doesn't exist.");
		return it->second;
	}

	void CGUISubContainer::removeSlider(const std::string& strName)
	{
		std::map<std::string, CGUISlider*>::iterator it = _mmapSliders.find(strName);
		if (it == _mmapSliders.end())
			return;
		delete it->second;
		_mmapSliders.erase(it);
	}

	void CGUISubContainer::removeAllSliders(void)
	{
		std::map<std::string, CGUISlider*>::iterator it = _mmapSliders.begin();
		while (it != _mmapSliders.end())
		{
			delete it->second;
			_mmapSliders.erase(it);
			it = _mmapSliders.begin();
		}
	}

	/**************************************************************************************************************************************************
	Line graphs
	**************************************************************************************************************************************************/

	CGUILineGraph* CGUISubContainer::addLineGraph(const std::string& strName, float fPosX, float fPosY, float fWidth, float fHeight)
	{
		// If resource already exists
		std::map<std::string, CGUILineGraph*>::iterator it = _mmapLineGraphs.find(strName);
		ThrowIfTrue(it != _mmapLineGraphs.end(), "CGUISubContainer::addLineGraph(" + strName + ") failed. The named object already exists.");
		CGUILineGraph* pNewRes = new CGUILineGraph;
		ThrowIfFalse(pNewRes, "CGUISubContainer::addLineGraph(" + strName + ") failed. Could not allocate memory for the new object.");
		pNewRes->mfPositionX = fPosX;
		pNewRes->mfPositionY = fPosY;
		pNewRes->mfWidth = fWidth;
		pNewRes->mfHeight = fHeight;
		_mmapLineGraphs[strName] = pNewRes;
		return pNewRes;
	}

	CGUILineGraph* CGUISubContainer::getLineGraph(const std::string& strName) const
	{
		std::map<std::string, CGUILineGraph*>::iterator it = _mmapLineGraphs.find(strName);
		ThrowIfTrue(it == _mmapLineGraphs.end(), "CGUISubContainer::getLineGraph(" + strName + ") failed. The named object doesn't exist.");
		return it->second;
	}

	void CGUISubContainer::removeLineGraph(const std::string& strName)
	{
		std::map<std::string, CGUILineGraph*>::iterator it = _mmapLineGraphs.find(strName);
		if (it == _mmapLineGraphs.end())
			return;
		delete it->second;
		_mmapLineGraphs.erase(it);
	}

	void CGUISubContainer::removeAllLineGraphs(void)
	{
		std::map<std::string, CGUILineGraph*>::iterator it = _mmapLineGraphs.begin();
		while (it != _mmapLineGraphs.end())
		{
			delete it->second;
			_mmapLineGraphs.erase(it);
			it = _mmapLineGraphs.begin();
		}
	}

	/**************************************************************************************************************************************************
	Progress bars
	**************************************************************************************************************************************************/

	CGUIProgressBar* CGUISubContainer::addProgressBar(const std::string& strName, float fPosX, float fPosY, float fWidth, float fHeight)
	{
		// If resource already exists
		std::map<std::string, CGUIProgressBar*>::iterator it = _mmapProgressBars.find(strName);
		ThrowIfTrue(it != _mmapProgressBars.end(), "CGUISubContainer::addProgressBar(" + strName + ") failed. The named object already exists.");
		CGUIProgressBar* pNewRes = new CGUIProgressBar;
		ThrowIfFalse(pNewRes, "CGUISubContainer::addProgressBar(" + strName + ") failed. Could not allocate memory for the new object.");
		pNewRes->mfPositionX = fPosX;
		pNewRes->mfPositionY = fPosY;
		pNewRes->mfWidth = fWidth;
		pNewRes->mfHeight = fHeight;
		_mmapProgressBars[strName] = pNewRes;
		return pNewRes;
	}

	CGUIProgressBar* CGUISubContainer::getProgressBar(const std::string& strName) const
	{
		std::map<std::string, CGUIProgressBar*>::iterator it = _mmapProgressBars.find(strName);
		ThrowIfTrue(it == _mmapProgressBars.end(), "CGUISubContainer::getProgressBar(" + strName + ") failed. The named object doesn't exist.");
		return it->second;
	}

	void CGUISubContainer::removeProgressBar(const std::string& strName)
	{
		std::map<std::string, CGUIProgressBar*>::iterator it = _mmapProgressBars.find(strName);
		if (it == _mmapProgressBars.end())
			return;
		delete it->second;
		_mmapProgressBars.erase(it);
	}

	void CGUISubContainer::removeAllProgressBars(void)
	{
		std::map<std::string, CGUIProgressBar*>::iterator it = _mmapProgressBars.begin();
		while (it != _mmapProgressBars.end())
		{
			delete it->second;
			_mmapProgressBars.erase(it);
			it = _mmapProgressBars.begin();
		}
	}

	/**************************************************************************************************************************************************
	Images
	**************************************************************************************************************************************************/

	CGUIImage* CGUISubContainer::addImage(const std::string& strName, float fPosX, float fPosY, const std::string& strImageFilename, float fWidth, float fHeight)
	{
		// If resource already exists
		std::map<std::string, CGUIImage*>::iterator it = _mmapImages.find(strName);
		ThrowIfTrue(it != _mmapImages.end(), "CGUISubContainer::addImage(" + strName + ") failed. The named object already exists.");
		CGUIImage* pNewRes = new CGUIImage;
		ThrowIfFalse(pNewRes, "CGUISubContainer::addImage(" + strName + ") failed. Could not allocate memory for the new object.");
		pNewRes->mfPositionX = fPosX;
		pNewRes->mfPositionY = fPosY;
		pNewRes->mfWidth = fWidth;
		pNewRes->mfHeight = fHeight;
		pNewRes->_mstrTextureResourceName = strImageFilename;
		pNewRes->_mbImageIsFromFile = true;
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

	CGUIImage* CGUISubContainer::addImageFromImage(const std::string& strName, float fPosX, float fPosY, const std::string& strCResourceTexture2DFromImage, float fWidth, float fHeight)
	{
		// If resource already exists
		std::map<std::string, CGUIImage*>::iterator it = _mmapImages.find(strName);
		ThrowIfTrue(it != _mmapImages.end(), "CGUISubContainer::addImageFromImage(" + strName + ") failed. The named object already exists.");
		CGUIImage* pNewRes = new CGUIImage;
		ThrowIfFalse(pNewRes, "CGUISubContainer::addImageFromImage(" + strName + ") failed. Could not allocate memory for the new object.");
		pNewRes->mfPositionX = fPosX;
		pNewRes->mfPositionY = fPosY;
		pNewRes->mfWidth = fWidth;
		pNewRes->mfHeight = fHeight;
		pNewRes->_mstrTextureResourceName = strCResourceTexture2DFromImage;
		pNewRes->_mbImageIsFromFile = false;
		_mmapImages[strName] = pNewRes;

		// Get image from the resource manager
		CResourceTexture2DFromImage* pTex = x->pResource->getTexture2DFromImage(strCResourceTexture2DFromImage);

		// If a value less than 0 is passed to width/height, set widget to dims of the image
		if (fWidth < 0)
			pNewRes->mfWidth = pTex->getDimensions().x;
		if (fHeight < 0)
			pNewRes->mfHeight = pTex->getDimensions().y;

		return pNewRes;
	}

	CGUIImage* CGUISubContainer::getImage(const std::string& strName) const
	{
		std::map<std::string, CGUIImage*>::iterator it = _mmapImages.find(strName);
		ThrowIfTrue(it == _mmapImages.end(), "CGUISubContainer::getImage(" + strName + ") failed. The named object doesn't exist.");
		return it->second;
	}

	void CGUISubContainer::removeImage(const std::string& strName)
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

	void CGUISubContainer::removeAllImages(void)
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

	/**************************************************************************************************************************************************
	Images animated
	**************************************************************************************************************************************************/

	CGUIImageAnimated* CGUISubContainer::addImageAnimated(const std::string& strName, float fPosX, float fPosY, const std::vector<std::string>& vecStrImageFilenames, float fWidth, float fHeight)
	{
		// If resource already exists
		std::map<std::string, CGUIImageAnimated*>::iterator it = _mmapImageAnimateds.find(strName);
		ThrowIfTrue(it != _mmapImageAnimateds.end(), "CGUISubContainer::addImageAnimated(" + strName + ") failed. The named object already exists.");
		CGUIImageAnimated* pNewRes = new CGUIImageAnimated;
		ThrowIfFalse(pNewRes, "CGUISubContainer::addImageAnimated(" + strName + ") failed. Could not allocate memory for the new object.");
		pNewRes->mfPositionX = fPosX;
		pNewRes->mfPositionY = fPosY;
		pNewRes->mfWidth = fWidth;
		pNewRes->mfHeight = fHeight;
		pNewRes->_mstrResourceTexture2DAtlasName = strName;
		_mmapImageAnimateds[strName] = pNewRes;

		// Add CResourceTexture2DAnimation to the resource manager
		CResourceTexture2DAtlas* pTex = x->pResource->addTexture2DAtlas(strName, vecStrImageFilenames, false, 1);

		// If a value less than 0 is passed to width/height, set widget to dims of the image
		if (fWidth < 0)
			pNewRes->mfWidth = (float)pTex->getImageDims(0).x;
		if (fHeight < 0)
			pNewRes->mfHeight = (float)pTex->getImageDims(0).y;

		return pNewRes;
	}

	CGUIImageAnimated* CGUISubContainer::getImageAnimated(const std::string& strName) const
	{
		std::map<std::string, CGUIImageAnimated*>::iterator it = _mmapImageAnimateds.find(strName);
		ThrowIfTrue(it == _mmapImageAnimateds.end(), "CGUISubContainer::getImageAnimated(" + strName + ") failed. The named object doesn't exist.");
		return it->second;
	}

	void CGUISubContainer::removeImageAnimated(const std::string& strName)
	{
		std::map<std::string, CGUIImageAnimated*>::iterator it = _mmapImageAnimateds.find(strName);
		if (it == _mmapImageAnimateds.end())
			return;

		// Remove CResourceTexture2DAtlas from the resource manager
		x->pResource->removeTexture2DAtlas(it->second->_mstrResourceTexture2DAtlasName);

		delete it->second;
		_mmapImageAnimateds.erase(it);
	}

	void CGUISubContainer::removeAllImageAnimateds(void)
	{
		std::map<std::string, CGUIImageAnimated*>::iterator it = _mmapImageAnimateds.begin();
		while (it != _mmapImageAnimateds.end())
		{
			delete it->second;
			_mmapImageAnimateds.erase(it);
			it = _mmapImageAnimateds.begin();
		}
	}

	/**************************************************************************************************************************************************
	Frame buffers
	**************************************************************************************************************************************************/

	CGUIImageFramebuffer* CGUISubContainer::addImageFramebuffer(const std::string& strName, float fPosX, float fPosY, const std::string& strFBname, float fWidth, float fHeight)
	{
		// If resource already exists
		std::map<std::string, CGUIImageFramebuffer*>::iterator it = _mmapImageFramebuffers.find(strName);
		ThrowIfTrue(it != _mmapImageFramebuffers.end(), "CGUISubContainer::addImageFramebuffer(" + strName + ") failed. The named object already exists.");
		CGUIImageFramebuffer* pNewRes = new CGUIImageFramebuffer;
		ThrowIfFalse(pNewRes, "CGUISubContainer::addImageFramebuffer(" + strName + ") failed. Could not allocate memory for the new object.");
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

	CGUIImageFramebuffer* CGUISubContainer::getImageFramebuffer(const std::string& strName) const
	{
		std::map<std::string, CGUIImageFramebuffer*>::iterator it = _mmapImageFramebuffers.find(strName);
		ThrowIfTrue(it == _mmapImageFramebuffers.end(), "CGUISubContainer::getImageFramebuffer(" + strName + ") failed. The named object doesn't exist.");
		return it->second;
	}

	void CGUISubContainer::removeImageFramebuffer(const std::string& strName)
	{
		std::map<std::string, CGUIImageFramebuffer*>::iterator it = _mmapImageFramebuffers.find(strName);
		if (it == _mmapImageFramebuffers.end())
			return;

		delete it->second;
		_mmapImageFramebuffers.erase(it);
	}

	void CGUISubContainer::removeAllImageFramebuffers(void)
	{
		std::map<std::string, CGUIImageFramebuffer*>::iterator it = _mmapImageFramebuffers.begin();
		while (it != _mmapImageFramebuffers.end())
		{
			delete it->second;
			_mmapImageFramebuffers.erase(it);
			it = _mmapImageFramebuffers.begin();
		}
	}

	/**************************************************************************************************************************************************
	Text scroll
	**************************************************************************************************************************************************/

	CGUITextScroll* CGUISubContainer::addTextScroll(const std::string& strName, float fPosX, float fPosY, float fWidth, float fHeight, const std::string& strText)
	{
		// If resource already exists
		std::map<std::string, CGUITextScroll*>::iterator it = _mmapTextScrolls.find(strName);
		ThrowIfTrue(it != _mmapTextScrolls.end(), "CGUISubContainer::addTextScroll(" + strName + ") failed. The named object already exists.");
		CGUITextScroll* pNewRes = new CGUITextScroll;
		ThrowIfFalse(pNewRes, "CGUISubContainer::addTextScroll(" + strName + ") failed. Could not allocate memory for the new object.");
		pNewRes->mfPositionX = fPosX;
		pNewRes->mfPositionY = fPosY;
		pNewRes->mfWidth = fWidth;
		pNewRes->mfHeight = fHeight;
		pNewRes->_mstrText = strText;
		// Create name of framebuffer
		pNewRes->_mstrFBName = "GUITextScrollFB_" + _mstrName + "_" + strName;

		// Check to see if the framebuffer resource name already exists
		ThrowIfTrue(x->pResource->getFramebufferExists(pNewRes->_mstrFBName), "CGUISubContainer::addTextScroll(" + strName + ") failed. The name must be unique for all TextScroll objects.");

		// Create framebuffer in SCResourceManager
		x->pResource->addFramebuffer(pNewRes->_mstrFBName, unsigned int(fWidth), unsigned int(fHeight));

		_mmapTextScrolls[strName] = pNewRes;
		return pNewRes;
	}

	CGUITextScroll* CGUISubContainer::getTextScroll(const std::string& strName) const
	{
		std::map<std::string, CGUITextScroll*>::iterator it = _mmapTextScrolls.find(strName);
		ThrowIfTrue(it == _mmapTextScrolls.end(), "CGUISubContainer::getTextScroll(" + strName + ") failed. The named object doesn't exist.");
		return it->second;
	}

	void CGUISubContainer::removeTextScroll(const std::string& strName)
	{
		std::map<std::string, CGUITextScroll*>::iterator it = _mmapTextScrolls.find(strName);
		if (it == _mmapTextScrolls.end())
			return;

		// Remove framebuffer resource from resource manager
		x->pResource->removeFramebuffer(it->second->_mstrFBName);

		delete it->second;
		_mmapTextScrolls.erase(it);
	}

	void CGUISubContainer::removeAllTextScrolls(void)
	{
		std::map<std::string, CGUITextScroll*>::iterator it = _mmapTextScrolls.begin();
		while (it != _mmapTextScrolls.end())
		{
			delete it->second;
			_mmapTextScrolls.erase(it);
			it = _mmapTextScrolls.begin();
		}
	}

	/**************************************************************************************************************************************************
	Button image
	**************************************************************************************************************************************************/

	CGUIButtonImage* CGUISubContainer::addButtonImage(const std::string& strName, float fPosX, float fPosY, const std::string& strImageFilenameUp, const std::string& strImageFilenameOver, const std::string& strImageFilenameDown, float fWidth, float fHeight)
	{
		// If resource already exists
		std::map<std::string, CGUIButtonImage*>::iterator it = _mmapButtonImages.find(strName);
		ThrowIfTrue(it != _mmapButtonImages.end(), "CGUISubContainer::addButtonImage(" + strName + ") failed. The named object already exists.");
		CGUIButtonImage* pNewRes = new CGUIButtonImage;
		ThrowIfFalse(pNewRes, "CGUISubContainer::addButtonImage(" + strName + ") failed. Could not allocate memory for the new object.");
		pNewRes->mfPositionX = fPosX;
		pNewRes->mfPositionY = fPosY;
		pNewRes->mfWidth = fWidth;
		pNewRes->mfHeight = fHeight;
		pNewRes->_mstrTextureDown = strImageFilenameDown;
		pNewRes->_mstrTextureOver = strImageFilenameOver;
		pNewRes->_mstrTextureUp = strImageFilenameUp;
		_mmapButtonImages[strName] = pNewRes;

		// Add images to the resource manager
		CResourceTexture2DFromFile* pTex = x->pResource->addTexture2DFromFile(pNewRes->_mstrTextureDown, pNewRes->_mstrTextureDown);
		pTex = x->pResource->addTexture2DFromFile(pNewRes->_mstrTextureOver, pNewRes->_mstrTextureOver);
		pTex = x->pResource->addTexture2DFromFile(pNewRes->_mstrTextureUp, pNewRes->_mstrTextureUp);

		// If a value less than 0 is passed to width/height, set widget to dims of the image
		if (fWidth < 0)
			pNewRes->mfWidth = pTex->getDimensions().x;
		if (fHeight < 0)
			pNewRes->mfHeight = pTex->getDimensions().y;

		return pNewRes;
	}

	CGUIButtonImage* CGUISubContainer::getButtonImage(const std::string& strName) const
	{
		std::map<std::string, CGUIButtonImage*>::iterator it = _mmapButtonImages.find(strName);
		ThrowIfTrue(it == _mmapButtonImages.end(), "CGUISubContainer::getButtonImage(" + strName + ") failed. The named object doesn't exist.");
		return it->second;
	}

	void CGUISubContainer::removeButtonImage(const std::string& strName)
	{
		std::map<std::string, CGUIButtonImage*>::iterator it = _mmapButtonImages.find(strName);
		if (it == _mmapButtonImages.end())
			return;

		// Remove images from the resource manager
		x->pResource->removeTexture2DFromFile(it->second->_mstrTextureDown);
		x->pResource->removeTexture2DFromFile(it->second->_mstrTextureOver);
		x->pResource->removeTexture2DFromFile(it->second->_mstrTextureUp);

		delete it->second;
		_mmapButtonImages.erase(it);
	}

	void CGUISubContainer::removeAllButtonImages(void)
	{
		std::map<std::string, CGUIButtonImage*>::iterator it = _mmapButtonImages.begin();
		while (it != _mmapButtonImages.end())
		{
			delete it->second;
			_mmapButtonImages.erase(it);
			it = _mmapButtonImages.begin();
		}
	}

	/**************************************************************************************************************************************************
	Depth buffers
	**************************************************************************************************************************************************/

	CGUIImageDepthbuffer* CGUISubContainer::addImageDepthbuffer(const std::string& strName, float fPosX, float fPosY, const std::string& strDBname, float fWidth, float fHeight)
	{
		// If resource already exists
		std::map<std::string, CGUIImageDepthbuffer*>::iterator it = _mmapImageDepthbuffers.find(strName);
		ThrowIfTrue(it != _mmapImageDepthbuffers.end(), "CGUISubContainer::addImageDepthbuffer(" + strName + ") failed. The named object already exists.");
		CGUIImageDepthbuffer* pNewRes = new CGUIImageDepthbuffer;
		ThrowIfFalse(pNewRes, "CGUISubContainer::addImageDepthbuffer(" + strName + ") failed. Could not allocate memory for the new object.");
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

	CGUIImageDepthbuffer* CGUISubContainer::getImageDepthbuffer(const std::string& strName) const
	{
		std::map<std::string, CGUIImageDepthbuffer*>::iterator it = _mmapImageDepthbuffers.find(strName);
		ThrowIfTrue(it == _mmapImageDepthbuffers.end(), "CGUISubContainer::getImageDepthbuffer(" + strName + ") failed. The named object doesn't exist.");
		return it->second;
	}

	void CGUISubContainer::removeImageDepthbuffer(const std::string& strName)
	{
		std::map<std::string, CGUIImageDepthbuffer*>::iterator it = _mmapImageDepthbuffers.find(strName);
		if (it == _mmapImageDepthbuffers.end())
			return;

		delete it->second;
		_mmapImageDepthbuffers.erase(it);
	}

	void CGUISubContainer::removeAllImageDepthbuffers(void)
	{
		std::map<std::string, CGUIImageDepthbuffer*>::iterator it = _mmapImageDepthbuffers.begin();
		while (it != _mmapImageDepthbuffers.end())
		{
			delete it->second;
			_mmapImageDepthbuffers.erase(it);
			it = _mmapImageDepthbuffers.begin();
		}
	}

	/**************************************************************************************************************************************************
	Check boxes
	**************************************************************************************************************************************************/

	CGUICheckbox* CGUISubContainer::addCheckbox(const std::string& strName, float fPosX, float fPosY)
	{
		// If resource already exists
		std::map<std::string, CGUICheckbox*>::iterator it = _mmapCheckboxes.find(strName);
		ThrowIfTrue(it != _mmapCheckboxes.end(), "CGUISubContainer::addCheckbox(" + strName + ") failed. The named object already exists.");
		CGUICheckbox* pNewRes = new CGUICheckbox;
		ThrowIfFalse(pNewRes, "CGUISubContainer::addCheckbox(" + strName + ") failed. Could not allocate memory for the new object.");
		pNewRes->mfPositionX = fPosX;
		pNewRes->mfPositionY = fPosY;

		_mmapCheckboxes[strName] = pNewRes;
		return pNewRes;
	}

	CGUICheckbox* CGUISubContainer::getCheckbox(const std::string& strName) const
	{
		std::map<std::string, CGUICheckbox*>::iterator it = _mmapCheckboxes.find(strName);
		ThrowIfTrue(it == _mmapCheckboxes.end(), "CGUISubContainer::getCheckbox(" + strName + ") failed. The named object doesn't exist.");
		return it->second;
	}

	void CGUISubContainer::removeCheckbox(const std::string& strName)
	{
		std::map<std::string, CGUICheckbox*>::iterator it = _mmapCheckboxes.find(strName);
		if (it == _mmapCheckboxes.end())
			return;
		delete it->second;
		_mmapCheckboxes.erase(it);
	}

	void CGUISubContainer::removeAllCheckboxes(void)
	{
		std::map<std::string, CGUICheckbox*>::iterator it = _mmapCheckboxes.begin();
		while (it != _mmapCheckboxes.end())
		{
			delete it->second;
			_mmapCheckboxes.erase(it);
			it = _mmapCheckboxes.begin();
		}
	}

	/**************************************************************************************************************************************************
	Sub containers
	**************************************************************************************************************************************************/

	CGUISubContainer* CGUISubContainer::addSubContainer(const std::string& strName, float fPosX, float fPosY, float fDimsX, float fDimsY)
	{
		// If resource already exists
		std::map<std::string, CGUISubContainer*>::iterator it = _mmapSubContainers.find(strName);
		ThrowIfTrue(it != _mmapSubContainers.end(), "CGUISubContainer::addSubContainer(" + strName + ") failed. The named object already exists.");
		CGUISubContainer* pNewRes = new CGUISubContainer;
		ThrowIfFalse(pNewRes, "CGUISubContainer::addSubContainer(" + strName + ") failed. Could not allocate memory for the new object.");
		pNewRes->mfPositionX = fPosX;
		pNewRes->mfPositionY = fPosY;
		pNewRes->setDimensions(fDimsX, fDimsY);

		// Create name of framebuffer
		pNewRes->_mstrFBName = "GUISubContainerFB_" + _mstrName + "_" + strName;

		// Check to see if the framebuffer resource name already exists
		ThrowIfTrue(x->pResource->getFramebufferExists(pNewRes->_mstrFBName), "CGUISubContainer::addSubContainer(" + strName + ") failed. The name must be unique for all SubContainer objects.");

		// Create framebuffer in SCResourceManager
		x->pResource->addFramebuffer(pNewRes->_mstrFBName, unsigned int(fDimsX), unsigned int(fDimsY));

		_mmapSubContainers[strName] = pNewRes;
		return pNewRes;
	}

	CGUISubContainer* CGUISubContainer::getSubContainer(const std::string& strName) const
	{
		std::map<std::string, CGUISubContainer*>::iterator it = _mmapSubContainers.find(strName);
		ThrowIfTrue(it == _mmapSubContainers.end(), "CGUISubContainer::getSubContainer(" + strName + ") failed. The named object doesn't exist.");
		return it->second;
	}

	void CGUISubContainer::removeSubContainer(const std::string& strName)
	{
		std::map<std::string, CGUISubContainer*>::iterator it = _mmapSubContainers.find(strName);
		if (it == _mmapSubContainers.end())
			return;
		delete it->second;
		_mmapSubContainers.erase(it);
	}

	void CGUISubContainer::removeAllSubContainers(void)
	{
		std::map<std::string, CGUISubContainer*>::iterator it = _mmapSubContainers.begin();
		while (it != _mmapSubContainers.end())
		{
			delete it->second;
			_mmapSubContainers.erase(it);
			it = _mmapSubContainers.begin();
		}
	}

	void CGUISubContainer::_renderFramebuffer(void* pParentContainer, const std::string& strFramebufferToSampleFrom)
	{
		CGUIContainer* pContainer = (CGUIContainer*)pParentContainer;

		// Create fake container object, as we need to set dimensions to that of this subcontainer
		CGUIContainer fakeContainer;
		fakeContainer.setDimensions(mfWidth, mfHeight);
		pContainer = &fakeContainer;

		CResourceFramebuffer* pFB = x->pResource->getFramebuffer(_mstrFBName);

		// Unbind the render target which the GUI is currently rendering to.
		CResourceFramebuffer* pFBGUITarget = x->pResource->getFramebuffer("X:gui");
		pFBGUITarget->unbindAsRenderTarget();

		// Resize the scrollbars and framebuffer render target if the dimensions have changed
		if (_mPreviousDims.x != mfWidth || _mPreviousDims.y != mfHeight)
		{
			_mPreviousDims.set(mfWidth, mfHeight);
			_mSliderV.setDimensions(20, mfHeight - 20);
			_mSliderV.setPosition(mfWidth - 20, 0);
			_mSliderH.setDimensions(mfWidth - 20, 20);
			_mSliderH.setPosition(0, mfHeight - 20);

			pFB->resize((unsigned int)mfWidth, (unsigned int)mfHeight);
		}

		// Set framebuffer as render target and clear it
		pFB->bindAsRenderTarget(true, false, CColour(1.0f, 0.2f, 0.0f, 0.2f));

		// Render stuff to the frame buffer
		// Render each button
		std::map<std::string, CGUIButton*>::iterator itButton = _mmapButtons.begin();
		while (itButton != _mmapButtons.end())
		{
			itButton->second->render(pParentContainer, strFramebufferToSampleFrom);
			itButton++;
		}

		// Render each text edit
		std::map<std::string, CGUITextEdit*>::iterator itTextEdit = _mmapTextEdits.begin();
		while (itTextEdit != _mmapTextEdits.end())
		{
			itTextEdit->second->render(pParentContainer, strFramebufferToSampleFrom);
			itTextEdit++;
		}

		// Render each sliders
		std::map<std::string, CGUISlider*>::iterator itSlider = _mmapSliders.begin();
		while (itSlider != _mmapSliders.end())
		{
			itSlider->second->render(pParentContainer, strFramebufferToSampleFrom);
			itSlider++;
		}

		// Render each line graph
		std::map<std::string, CGUILineGraph*>::iterator itLineGraph = _mmapLineGraphs.begin();
		while (itLineGraph != _mmapLineGraphs.end())
		{
			itLineGraph->second->render(pParentContainer, strFramebufferToSampleFrom);
			itLineGraph++;
		}

		// Render each progress bar
		std::map<std::string, CGUIProgressBar*>::iterator itProgressBar = _mmapProgressBars.begin();
		while (itProgressBar != _mmapProgressBars.end())
		{
			itProgressBar->second->render(pParentContainer, strFramebufferToSampleFrom);
			itProgressBar++;
		}

		// Render each image
		std::map<std::string, CGUIImage*>::iterator itImage = _mmapImages.begin();
		while (itImage != _mmapImages.end())
		{
			itImage->second->render(pParentContainer, strFramebufferToSampleFrom);
			itImage++;
		}

		// Render each animated image
		std::map<std::string, CGUIImageAnimated*>::iterator itImageAnimated = _mmapImageAnimateds.begin();
		while (itImageAnimated != _mmapImageAnimateds.end())
		{
			itImageAnimated->second->render(pParentContainer, strFramebufferToSampleFrom);
			itImageAnimated++;
		}

		// Images framebuffer
		std::map<std::string, CGUIImageFramebuffer*>::iterator itImageFB = _mmapImageFramebuffers.begin();
		while (itImageFB != _mmapImageFramebuffers.end())
		{
			itImageFB->second->render(pParentContainer, strFramebufferToSampleFrom);
			itImageFB++;
		}

		// Render each text scroll object
		std::map<std::string, CGUITextScroll*>::iterator itTextScroll = _mmapTextScrolls.begin();
		while (itTextScroll != _mmapTextScrolls.end())
		{
			itTextScroll->second->render(pParentContainer, strFramebufferToSampleFrom);
			itTextScroll++;
		}

		// Render each of the button images
		std::map<std::string, CGUIButtonImage*>::iterator itButtonImage = _mmapButtonImages.begin();
		while (itButtonImage != _mmapButtonImages.end())
		{
			itButtonImage->second->render(pParentContainer, strFramebufferToSampleFrom);
			itButtonImage++;
		}

		// Images depth buffer
		std::map<std::string, CGUIImageDepthbuffer*>::iterator itImageDB = _mmapImageDepthbuffers.begin();
		while (itImageDB != _mmapImageDepthbuffers.end())
		{
			itImageDB->second->render(pParentContainer, strFramebufferToSampleFrom);
			itImageDB++;
		}

		// Render each checkbox
		std::map<std::string, CGUICheckbox*>::iterator itCheckbox = _mmapCheckboxes.begin();
		while (itCheckbox != _mmapCheckboxes.end())
		{
			itCheckbox->second->render(pParentContainer, strFramebufferToSampleFrom);
			itCheckbox++;
		}

		// Render each sub container
		std::map<std::string, CGUISubContainer*>::iterator itSubContainer = _mmapSubContainers.begin();
		while (itSubContainer != _mmapSubContainers.end())
		{
			itSubContainer->second->render(pParentContainer, strFramebufferToSampleFrom);
			itSubContainer++;
		}

		// Render each text
		std::map<std::string, CGUIText*>::iterator itText = _mmapTexts.begin();
		while (itText != _mmapTexts.end())
		{
			itText->second->render(pParentContainer);
			itText++;
		}

		// Render the sliders
		_mSliderV.render(pParentContainer, strFramebufferToSampleFrom);
		_mSliderH.render(pParentContainer, strFramebufferToSampleFrom);




		pFB->unbindAsRenderTarget();
		_mbFBNeedsUpdating = false;

		// Remember to re-bind the GUI framebuffer back again
		pFBGUITarget->bindAsRenderTarget(false, false);
	}
}