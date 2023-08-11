#include "PCH.h"
#include "GUIManager.h"
#include "stringUtils.h"
#include "singletons.h"

namespace X
{
	SCGUIManager::SCGUIManager()
	{
		SCLog* pLog = SCLog::getPointer();
		pLog->add("SCGUIManager::SCGUIManager() called.");

		_mfScale = 1.0f;
		// Add default theme
		CGUITheme* pTheme = addTheme("default");
		pTheme->loadTextures();
		pTheme->addFontsToManager();

		SCAudioManager::getPointer()->addNewSampleGroup("gui");
		pTheme->addAudioToManager();
		setAudioVol(1.0f);
		_mbWindowBeingMoved = false;
		_mfTooltipDelaySeconds = 1.0f;
		setTooltipOffset(32.0f, 0.0f);
	}

	void SCGUIManager::render(const std::string& strFramebufferToSampleFrom)
	{
		// Update the timer object
		_mTimer.update();

		// Update each container
		// Go through each container, starting with the one at the front first
		CGUIContainer* pContainer;
		std::list<std::string>::iterator it = _mlistContainerZOrder.end();
		bool bMouseIsOverContainerWhichIsAboveThisOne = false;
		
		// Holds the name of a container which wishes to be moved on top
		// We store this and then after we've gone through all containers, move it to the top, to prevent messing with the list whilst updating
		std::string strWindowWantsToBePlacedInFront;
		while (it != _mlistContainerZOrder.begin())
		{
			it--;
			pContainer = getContainer(*it);

			if (pContainer->update(bMouseIsOverContainerWhichIsAboveThisOne))
			{
				// If clicked upon and mouse over this container, mark as wanting to be placed on top
				if (x->pInput->mouse.leftButDown())
				{
					strWindowWantsToBePlacedInFront = *it;
				}
				bMouseIsOverContainerWhichIsAboveThisOne = true;
			}
		}
		
		// Move container that has been clicked upon to the front
		if (strWindowWantsToBePlacedInFront.length())
		{
			moveContainerToFront(strWindowWantsToBePlacedInFront);
		}

		// Render each container
		// Set the "X:gui" framebuffer as the render target
		x->pResource->getFramebuffer("X:backbuffer")->unbindAsRenderTarget();
		CResourceFramebuffer* pFBGUITarget = x->pResource->getFramebuffer("X:gui");
		// Resize to window dims if not the same
		if (pFBGUITarget->getDimensions().x != x->pWindow->getDimensions().x ||
			pFBGUITarget->getDimensions().y != x->pWindow->getDimensions().y)
		{
			pFBGUITarget->resize(x->pWindow->getWidth(), x->pWindow->getHeight());
		}
		// Clear and bind the render target
		pFBGUITarget->bindAsRenderTarget(true, false);

		// Go through each container, starting with the one at the back first
		it = _mlistContainerZOrder.begin();
		
		while (it != _mlistContainerZOrder.end())
		{
			pContainer = getContainer(*it);
			
			// Render the container
			pContainer->render(strFramebufferToSampleFrom);
			
			it++;
		}

		// Render tooltips
		// Go through each container, starting with the one at the back first
		it = _mlistContainerZOrder.begin();
		while (it != _mlistContainerZOrder.end())
		{
			pContainer = getContainer(*it);
			pContainer->renderTooltips(strFramebufferToSampleFrom);
			it++;
		}
		
		// Update the default containers if they are shown
		// The default containers are statistics etc
		_updateDefaultContainers();

		// Rebind correct framebuffer
		pFBGUITarget->unbindAsRenderTarget();
		x->pResource->getFramebuffer("X:backbuffer")->bindAsRenderTarget(false, false);
	}

	void SCGUIManager::setScale(float fScalingValue)
	{
		_mfScale = fScalingValue;
	}

	float SCGUIManager::getScale(void) const
	{
		return _mfScale;
	}

	CGUITheme* SCGUIManager::addTheme(const std::string& strName)
	{
		// Attempt to find by name
		std::map<std::string, CGUITheme*>::iterator it = _mmapThemes.find(strName);
		// If it already exists, return a pointer to that.
		if (it != _mmapThemes.end())
			return it->second;

		// Allocate the new theme
		CGUITheme* pNew = new CGUITheme;
		ThrowIfFalse(pNew, "SCGUIManager::addTheme(" + strName + ") failed. Unable to allocate memory for the new theme.");

		// Place in the hashmap
		_mmapThemes[strName] = pNew;
		return pNew;
	}

	CGUITheme* SCGUIManager::getTheme(const std::string& strName) const
	{
		std::map<std::string, CGUITheme*>::iterator it = _mmapThemes.find(strName);
		ThrowIfTrue(it == _mmapThemes.end(), "SCGUIManager::getTheme(" + strName + ") failed. Theme doesn't exist.");
		return it->second;
	}

	bool SCGUIManager::getThemeExists(const std::string& strName) const
	{
		std::map<std::string, CGUITheme*>::iterator it = _mmapThemes.find(strName);
		return it != _mmapThemes.end();
	}

	void SCGUIManager::removeTheme(const std::string& strName)
	{
		std::map<std::string, CGUITheme*>::iterator it = _mmapThemes.find(strName);
		if (it == _mmapThemes.end())
			return;	// Doesn't exist.
		delete it->second;
		_mmapThemes.erase(it);
	}

	void SCGUIManager::removeAllThemes(void)
	{
		std::map<std::string, CGUITheme*>::iterator it = _mmapThemes.begin();
		while (it != _mmapThemes.end())
		{
			delete it->second;
			it++;
		}
		_mmapThemes.clear();
	}

	int SCGUIManager::getNumberOfThemes(void) const
	{
		return (int)_mmapThemes.size();
	}

	std::string SCGUIManager::getThemeName(int iIndex) const
	{
		ThrowIfTrue(iIndex < 0 || iIndex >= (int)_mmapThemes.size(), "SCGUIManager::getThemeName() failed. Invalid index of " + std::to_string(iIndex) + " was given.");
		std::map<std::string, CGUITheme*>::iterator it = _mmapThemes.begin();
		int iCount = 0;
		while (iCount < iIndex)
			it++;
		return it->first;
	}

	void SCGUIManager::setThemeForAll(const std::string& strName)
	{
		// Make sure the given theme name exists
		ThrowIfFalse(getThemeExists(strName), "SCGUIManager::setThemeForAll() failed. The given theme name of " + strName + " doesn't exist.");

		// Set each container to use the given theme
		std::map<std::string, CGUIContainer*>::iterator it = _mmapContainers.begin();
		while (it != _mmapContainers.end())
		{
			it->second->mstrThemename = strName;
			it++;
		}
	}

	CGUIContainer* SCGUIManager::addContainer(const std::string& strName, bool bLocked)
	{
		// Add container name to the Z order list
		_mlistContainerZOrder.push_back(strName);

		// Attempt to find by name
		std::map<std::string, CGUIContainer*>::iterator it = _mmapContainers.find(strName);
		// If it already exists, return a pointer to that.
		if (it != _mmapContainers.end())
			return it->second;

		// Allocate the new object
		CGUIContainer* pNew = new CGUIContainer;
		ThrowIfFalse(pNew, "SCGUIManager::addContainer(" + strName + ") failed. Unable to allocate memory for the new container.");

		pNew->_mbLocked = bLocked;

		pNew->mstrTitleText = strName;

		// Store name of container inside the container, as it's used to create unique names for things such as the framebuffers used by the CGUITextScroll object.
		pNew->_mstrName = strName;

		// Place in the hashmap
		_mmapContainers[strName] = pNew;

		// Set this containers titlebar text to active and all other to inactive
		// Set colour of all other containers
		std::map<std::string, CGUIContainer*>::iterator itCont = _mmapContainers.begin();
		CGUITheme* pTheme = getTheme(pNew->mstrThemename);

		while (itCont != _mmapContainers.end())
		{
			itCont->second->_mTextColour = pTheme->mColours.containerTitlebarTextNotInFocus;
			itCont++;
		}

		// Set colour of titlebar text
		pNew->_mTextColour = pTheme->mColours.containerTitlebarTextInFocus;
		return pNew;
	}

	CGUIContainer* SCGUIManager::getContainer(const std::string& strName) const
	{
		std::map<std::string, CGUIContainer*>::iterator it = _mmapContainers.find(strName);
		ThrowIfTrue(it == _mmapContainers.end(), "SCGUIManager::getContainer(" + strName + ") failed. Container doesn't exist.");
		return it->second;
	}

	bool SCGUIManager::getContainerExists(const std::string& strName) const
	{
		std::map<std::string, CGUIContainer*>::iterator it = _mmapContainers.find(strName);
		return it != _mmapContainers.end();
	}

	void SCGUIManager::removeContainer(const std::string& strName)
	{
		std::map<std::string, CGUIContainer*>::iterator it = _mmapContainers.find(strName);
		if (it == _mmapContainers.end())
			return;	// Doesn't exist.
		
		if (it->second->_mbLocked)	// If it was locked upon being added, do not remove this container
			return;

		// Delete the container
		delete it->second;
		_mmapContainers.erase(it);

		// Remove container name from the Z order list
		std::list<std::string>::iterator itlist = std::find(_mlistContainerZOrder.begin(), _mlistContainerZOrder.end(), strName);
		if (itlist != _mlistContainerZOrder.end())
		{
			_mlistContainerZOrder.erase(itlist);
		}
	}

	void SCGUIManager::removeAllContainers(void)
	{
		std::map<std::string, CGUIContainer*>::iterator it = _mmapContainers.begin();
		while (it != _mmapContainers.end())
		{
			// If it was locked upon being added, do not remove this container
			if (it->second->_mbLocked)
			{
				it++;
				continue;
			}

			// Remove the container
			removeContainer(it->first);
			it = _mmapContainers.begin();
		}
	}

	int SCGUIManager::getNumberOfContainers(void) const
	{
		return (int)_mmapContainers.size();
	}

	std::string SCGUIManager::getContainerName(int iIndex) const
	{
		ThrowIfTrue(iIndex < 0 || iIndex >= (int)_mmapContainers.size(), "SCGUIManager::getContainerName() failed. Invalid index of " + std::to_string(iIndex) + " was given.");
		std::map<std::string, CGUIContainer*>::iterator it = _mmapContainers.begin();
		int iCount = 0;
		while (iCount < iIndex)
		{
			it++;
			iCount++;
		}
		return it->first;
	}

	void SCGUIManager::moveContainerToFront(const std::string& strContainerName)
	{
		std::list<std::string>::iterator itlist = std::find(_mlistContainerZOrder.begin(), _mlistContainerZOrder.end(), strContainerName);
		if (itlist != _mlistContainerZOrder.end())
		{
			_mlistContainerZOrder.erase(itlist);
			_mlistContainerZOrder.push_back(strContainerName);
		}
	}

	bool SCGUIManager::getWindowBeingMoved(void) const
	{
		return _mbWindowBeingMoved;
	}

	void SCGUIManager::setWindowBeingMoved(bool bWindowBeingMoved)
	{
		_mbWindowBeingMoved = bWindowBeingMoved;
	}

	void SCGUIManager::setAudioVol(float fVol)
	{
		clamp(fVol, 0.0f, 1.0f);
		_mfAudioVol = fVol;
	}

	float SCGUIManager::getAudioVol(void) const
	{
		return _mfAudioVol;
	}

	void SCGUIManager::_createDefaultContainers(void)
	{
		_createDefaultContainerStatistics();
		_createDefaultContainerFontGenerator();
		_createDefaultContainerSettings();
	}

	void SCGUIManager::_updateDefaultContainers(void)
	{
		CGUIContainer* pCont;

		pCont = getContainer("X:Default:Statistics");
		if (pCont->getVisible())
			_updateDefaultContainerStatistics();

		pCont = getContainer("X:Default:FontGenerator");
		if (pCont->getVisible())
			_updateDefaultContainerFontGenerator();

		pCont = getContainer("X:Default:Settings");
		if (pCont->getVisible())
			_updateDefaultContainerSettings();
	}

	void SCGUIManager::_createDefaultContainerStatistics(void)
	{
		CGUIContainer* pCont = addContainer("X:Default:Statistics", true);
		CGUITheme* pTheme = pCont->getTheme();
		CResourceFont* pFont = x->pResource->getFont(pTheme->mFonts.text);
		float fTextHeight = pFont->getTextHeight(1.0f);

		pCont->setDimensions(450.0f, 330.0f);
		pCont->setPosition(100000.0f, 0.0f);
		pCont->setVisible(false);
		pCont->mstrTitleText = "Statistics";

		CGUILineGraph* pLineGraph = pCont->addLineGraph("Stats_FPS_Linegraph", 0, 0, 450, 120);
		CColour col;
		col.set(1.0f, 1.0f, 1.0f, 0.5f);	pLineGraph->addDataset("FPS", col);
		col.set(1.0f, 1.0f, 1.0f, 1.0f);	pLineGraph->addDataset("FPSAVR", col);
		float fYpos = 140.0f;

		pCont->addText("Text_FPSMin", 0, fYpos, "FPSMin: ");
		pCont->addText("Text_FPS", 0, fYpos, "FPS: ");
		pCont->addText("Text_FPSMax", 0, fYpos, "FPSMax: ");
		fYpos += fTextHeight;
		
		pCont->addText("Text_FPSAVR", 0, fYpos, "FPSAVR: ");
		fYpos += fTextHeight;

		_mDefContStatistics.fAddValueToLinegraphDataset = 0.0f;

		// VSync enabled/disabled and desktop Hz
		pCont->addText("VSyncONOFF", 0, fYpos, "VSync: ON");	fYpos += fTextHeight;
		pCont->addText("Text_Mem1", 0, fYpos, "");	fYpos += fTextHeight;
		pCont->addText("Text_Mem2", 0, fYpos, "");	fYpos += fTextHeight;

		// CPU info
		pCont->addText("CPU_info", 0, fYpos, ""); fYpos += fTextHeight;

		// Close button
		fYpos += 10;
		CGUIButton* pButton = pCont->addButton("Close", 225 - 50, fYpos, 100, 30, "Close"); fYpos += 30.0f;
		pButton->mpTooltip->setAsText("Close window.");
	}

	void SCGUIManager::_updateDefaultContainerStatistics(void)
	{
		CGUIContainer* pCont = getContainer("X:Default:Statistics");
		CGUITheme* pTheme = x->pGUI->getTheme(pCont->mstrThemename);
		CResourceFont* pFont = x->pResource->getFont(pTheme->mFonts.text);

		CGUILineGraph* pLineGraph = pCont->getLineGraph("Stats_FPS_Linegraph");
		CGUIText* pText;

		// Add new value to linegraph data set
		_mDefContStatistics.fAddValueToLinegraphDataset += _mTimer.getSecondsPast();
		if (_mDefContStatistics.fAddValueToLinegraphDataset >= 0.1f)
		{
			_mDefContStatistics.fAddValueToLinegraphDataset = 0.0f;
			CGUILineGraphDataSet* pDataSetFPS = pLineGraph->getDataset("FPS");
			CGUILineGraphDataSet* pDataSetFPSAVR = pLineGraph->getDataset("FPSAVR");
			pDataSetFPS->addValue(_mTimer.getFPS());
			pDataSetFPSAVR->addValue(_mTimer.getFPSAveraged());

			while (pDataSetFPS->getNumValues() > 60)
				pDataSetFPS->removeValue();
			while (pDataSetFPSAVR->getNumValues() > 60)
				pDataSetFPSAVR->removeValue();

			// Set min and max text
			float fMaxFPS = pDataSetFPS->getHighestValue();
			float fMaxFPSAVR = pDataSetFPSAVR->getHighestValue();
			float fMax = fMaxFPS;
			if (fMax < fMaxFPSAVR)
				fMax = fMaxFPSAVR;

			float fMinFPS = pDataSetFPS->getLowestValue();
			float fMinFPSAVR = pDataSetFPSAVR->getLowestValue();
			float fMin = fMinFPS;
			if (fMin < fMinFPSAVR)
				fMin = fMinFPSAVR;

			std::string str = "FPS Max: " + std::to_string((int)fMax);
			
			pText = pCont->getText("Text_FPSMax");
			pText->setText(str);
			// Set position
			CVector2f vNewPos = pText->getPosition();
			vNewPos.x = 450 - pFont->getTextWidth(str);
			pText->setPosition(vNewPos);

			str = "FPS Min: " + std::to_string((int)fMin);
			pText = pCont->getText("Text_FPSMin");
			pText->setText(str);
		}

		// Update text values, "Text_FPSMax" and  "Text_FPSMin" have been updated above
		std::string strTxt;
		strTxt = "FPS: ";
		StringUtils::appendFloat(strTxt, _mTimer.getFPS(), 1);
		pText = pCont->getText("Text_FPS");
		pText->setText(strTxt);
		// Also set position
		CVector2f vNewPos = pText->getPosition();
		vNewPos.x = 225 - (0.5f * pFont->getTextWidth(strTxt));
		pText->setPosition(vNewPos);

		strTxt = "FPS Average: ";
		StringUtils::appendFloat(strTxt, _mTimer.getFPSAveraged(), 1);
		strTxt += " (";
		StringUtils::appendFloat(strTxt, _mTimer.getFPSAveragedTimeUntilNextUpdate(), 1);
		strTxt += ")";
		pCont->getText("Text_FPSAVR")->setText(strTxt);
		// Also update colour based on whether it's above or below refresh rate
		CColour col(0.0f, 1.0f, 0.0f, 1.0f);
		if (_mTimer.getFPSAveraged() < (float)x->pWindow->getRefreshRate() - 1)
			col.set(1.0f, 0.0f, 0.0f, 1.0f);
		pCont->getText("Text_FPSAVR")->setColour(false, col);

		// VSync text
		if (x->pWindow->getVSyncEnabled())
		{
			strTxt = "VSync: On. Desktop Hz: ";
			StringUtils::appendUInt(strTxt, x->pWindow->getRefreshRate());
			pCont->getText("VSyncONOFF")->setText(strTxt);
		}
		else
		{
			strTxt = "VSync: Off. Desktop Hz: ";
			StringUtils::appendUInt(strTxt, x->pWindow->getRefreshRate());
			pCont->getText("VSyncONOFF")->setText(strTxt);
		}

		// CPU info
		strTxt = "CPU Logical Cores Count: ";
		StringUtils::appendInt(strTxt, getCPULogicalCores());
		pCont->getText("CPU_info")->setText(strTxt);

		// Memory text
		SMemInfo memInfo;
		getMemInfo(memInfo);
		pCont->getText("Text_Mem1")->setText("MemProcessWorkingSetSize: " + std::to_string(((memInfo.proc.iWorkingSetSize / 1024) / 1024)) + "MB");
		pCont->getText("Text_Mem2")->setText("MemProcessiPrivateUsage: " + std::to_string((((memInfo.proc.iPrivateUsage) / 1024) / 1024)) + "MB");

		// Close button
		CGUIButton* pButton = pCont->getButton("Close");
		if (pButton->getClicked())
			pCont->setVisible(false);
	}

	void SCGUIManager::_createDefaultContainerFontGenerator(void)
	{
		CGUIContainer* pCont = addContainer("X:Default:FontGenerator", true);
		CGUITheme* pTheme = pCont->getTheme();
		CResourceFont* pFont = x->pResource->getFont(pTheme->mFonts.text);
		float fTextHeight = pFont->getTextHeight(1.0f);
		CGUIButton* pButton;
		CGUITextEdit* pTextEdit;

		pCont->setDimensions(640.0f, 385.0f);
		pCont->setPosition(100000.0f, 0.0f);
		pCont->setVisible(false);
		pCont->mstrTitleText = "Font Generator";

		float fYpos = 0;

		// Set settings
		_mDefContFontGen.bAntiAliasingOn = true;
		_mDefContFontGen.bItalicsOn = false;
		_mDefContFontGen.bStrikeoutOn = false;
		_mDefContFontGen.bUnderliningOn = false;
		_mDefContFontGen.iHeight = 20;
		_mDefContFontGen.iWeight = 400;
		_mDefContFontGen.strFontName = "arial";
		
		// Text explaining stuff
		CGUITextScroll* pTextScroll = pCont->addTextScroll("X:Default:FontGenerator:TextScroll", 0, 0, 420, 385, "");
		std::string strTxt = "This Font Generator window is used to create, preview and save files for use with the SCResourceManager and it's CResourceFont class.\n \n";
		strTxt += "Start by typing the name of the font installed on the operating system, then set the height and weight in the text edit boxes and toggle anti-aliasing, italics, underling and strikeout with the buttons.\n";
		strTxt += "Whenever you change a setting, the the pair of font files \"font_output.fnt\" and \"font_output.png\" are saved to this application's root directory.\n";
		strTxt += "Happy font creating! :)";
		pTextScroll->setText(strTxt);

		// Text edit box for font name
		pTextEdit = pCont->addTextEdit("textedit:fontname", 440, fYpos, 200, 40, _mDefContFontGen.strFontName);
		pTextEdit->mpTooltip->setAsText("Type the name of the full font installed on your operating system here.");
		pTextEdit->setMaxChars(64);
		fYpos += 45;

		// Text edit box for font height
		strTxt.clear();
		StringUtils::appendInt(strTxt, _mDefContFontGen.iHeight);
		pTextEdit = pCont->addTextEdit("textedit:fontheight", 440, fYpos, 200, 40, strTxt);
		pTextEdit->mpTooltip->setAsText("Type the height of the font here.");
		fYpos += 45;

		// Text edit box for font weight
		strTxt.clear();
		StringUtils::appendInt(strTxt, _mDefContFontGen.iWeight);
		pTextEdit = pCont->addTextEdit("textedit:fontweight", 440, fYpos, 200, 40, strTxt);
		pTextEdit->mpTooltip->setAsText("Type the font weight to use when generating the font here. Values of 400-800 are typical.");
		fYpos += 55;

		// Button to toggle anti-aliasing on/off
		pButton = pCont->addButton("antialiasing", 440, fYpos, 200, 40, "Anti aliasing");
		pButton->mpTooltip->setAsText("Toggle anti-aliasing on/off. (Currently on)");
		fYpos += 50;

		// Button to toggle italic on/off
		pButton = pCont->addButton("italic", 440, fYpos, 200, 40, "Toggle Italics");
		pButton->mpTooltip->setAsText("Toggle italics on/off. (Currently off)");
		fYpos += 50;

		// Button to toggle underline on/off
		pButton = pCont->addButton("underline", 440, fYpos, 200, 40, "Toggle Underlining");
		pButton->mpTooltip->setAsText("Toggle underlining on/off.  (Currently off)");
		fYpos += 50;

		// Button to toggle strikeout on/off
		pButton = pCont->addButton("strikeout", 440, fYpos, 200, 40, "Toggle Strikeout");
		pButton->mpTooltip->setAsText("Toggle strikeout on/off.  (Currently off)");
		fYpos += 50;

		// Close button
		pButton = pCont->addButton("Close", 440, fYpos, 200, 40, "Close");
		pButton->mpTooltip->setAsText("Close window.");
		fYpos += 50;

		// Create the preview font
		_mDefContFontGen.pFont = new CResourceFont("data/X/fonts/satoshi_22");
	}

	void SCGUIManager::_updateDefaultContainerFontGenerator(void)
	{
		CGUIContainer* pCont = getContainer("X:Default:FontGenerator");
		int iTotalRenderedHeight;
		
		// Close button
		CGUIButton* pButton = pCont->getButton("Close");
		if (pButton->getClicked())
			pCont->setVisible(false);

		// Fontname textedit
		CGUITextEdit* pTE;
		pTE = pCont->getTextEdit("textedit:fontname");
		if (pTE->getEnterPressed())
		{
			_mDefContFontGen.strFontName = pTE->mstrText;
			_defaultContainerFontGeneratorBuildFont();
		}

		// Fontheight textedit
		pTE = pCont->getTextEdit("textedit:fontheight");
		if (pTE->getEnterPressed())
		{
			if (StringUtils::representsNumber(pTE->mstrText))
			{
				_mDefContFontGen.iHeight = stoi(pTE->mstrText);
				_defaultContainerFontGeneratorBuildFont();
			}
		}

		// Fontweight textedit
		pTE = pCont->getTextEdit("textedit:fontweight");
		if (pTE->getEnterPressed())
		{
			if (StringUtils::representsNumber(pTE->mstrText))
			{
				_mDefContFontGen.iWeight = stoi(pTE->mstrText);
				_defaultContainerFontGeneratorBuildFont();
			}
		}

		// Button to toggle anti-aliasing on/off
		pButton = pCont->getButton("antialiasing");
		if (pButton->getClicked())
		{
			_mDefContFontGen.bAntiAliasingOn = !_mDefContFontGen.bAntiAliasingOn;
			if (_mDefContFontGen.bAntiAliasingOn)
				pButton->mpTooltip->setAsText("Toggle anti-aliasing on/off. (Currently on)");
			else
				pButton->mpTooltip->setAsText("Toggle anti-aliasing on/off. (Currently off)");
			_defaultContainerFontGeneratorBuildFont();
		}

		// Button to toggle italic on/off
		pButton = pCont->getButton("italic");
		if (pButton->getClicked())
		{
			_mDefContFontGen.bItalicsOn = !_mDefContFontGen.bItalicsOn;
			if (_mDefContFontGen.bItalicsOn)
				pButton->mpTooltip->setAsText("Toggle italics on/off. (Currently on)");
			else
				pButton->mpTooltip->setAsText("Toggle italics on/off. (Currently off)");
			_defaultContainerFontGeneratorBuildFont();
		}

		// Button to toggle underline on/off
		pButton = pCont->getButton("underline");
		if (pButton->getClicked())
		{
			_mDefContFontGen.bUnderliningOn = !_mDefContFontGen.bUnderliningOn;
			if (_mDefContFontGen.bUnderliningOn)
				pButton->mpTooltip->setAsText("Toggle underlining on/off. (Currently on)");
			else
				pButton->mpTooltip->setAsText("Toggle underlining on/off. (Currently off)");
			_defaultContainerFontGeneratorBuildFont();
		}

		// Button to toggle strikeout on/off
		pButton = pCont->getButton("strikeout");
		if (pButton->getClicked())
		{
			_mDefContFontGen.bStrikeoutOn = !_mDefContFontGen.bStrikeoutOn;
			if (_mDefContFontGen.bStrikeoutOn)
				pButton->mpTooltip->setAsText("Toggle strikeout on/off. (Currently on)");
			else
				pButton->mpTooltip->setAsText("Toggle strikeout on/off. (Currently off)");
			_defaultContainerFontGeneratorBuildFont();
		}

		// Render preview font
		std::string strTxt;
		strTxt = "Here is the Font Generator window's preview font being rendered with some text, no ipsum stuff here!\n";
		strTxt += "Let's see what numbers look like... 0123456789 and with a space in between... 0 1 2 3 4 5 6 7 8 9\n";
		strTxt += "Not all fonts have these symbols... Shift key + number keys... !\"$%^&*()_+\n";
		strTxt += "Some more symbols...\n";
		strTxt += "- + _ : []{} <--- minus, plus, underscore, equals and four brackets. \n";
		strTxt += ": ; ' @ ' <--- Semi colon, colon, apostrophe, at symbol and tilde.\n";
		strTxt += "< > ? / <--- Less than, more than, question mark and forward slash.\n";
		strTxt += "AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz. <--- Letters lower and uppercase.\n";
		strTxt += ",,,, .... <--- Commas and full periods.\n";

		std::vector<std::string> vecStrLines;
		_mDefContFontGen.pFont->printInRectNewline(
			true,
			strTxt,
			0,
			0,
			x->pWindow->getWidth(),
			x->pWindow->getHeight(),
			vecStrLines,
			iTotalRenderedHeight,
			"\n",
			1.0f,
			CColour());
	}

	void SCGUIManager::_defaultContainerFontGeneratorBuildFont(void)
	{
		x->pResource->buildFontFiles(
			"font_output",
			_mDefContFontGen.strFontName,
			_mDefContFontGen.iHeight,
			_mDefContFontGen.iWeight,
			_mDefContFontGen.bAntiAliasingOn,
			_mDefContFontGen.bItalicsOn,
			_mDefContFontGen.bUnderliningOn,
			_mDefContFontGen.bStrikeoutOn,
			false);
		delete _mDefContFontGen.pFont;
		std::string strFontFilename = "font_output";
//		StringUtils::appendInt(strFontFilename, _mDefContFontGen.iHeight);
		_mDefContFontGen.pFont = new CResourceFont(strFontFilename);
	}

	void SCGUIManager::_createDefaultContainerSettings(void)
	{
		CGUIContainer* pCont = addContainer("X:Default:Settings", true);
		pCont->setDimensions(1920 * 0.5f, 1080 * 0.5f);
		pCont->setPositionCentreWindow();
		pCont->setVisible(false);
		pCont->mstrTitleText = "Settings.";
		CGUITheme* pTheme = pCont->getTheme();
		CResourceFont* pFont = x->pResource->getFont(pTheme->mFonts.text);
		float fTextHeight = pFont->getTextHeight(1.0f);
		CGUICheckbox* pCB;
		pCB = pCont->addCheckbox("vsync", 0, 5);
		pCB->setState(x->pWindow->getVSyncEnabled());
		if (x->pWindow->getVSyncEnabled())
		{
			pCB->mpTooltip->setAsText("Toggle between synchronization of the window's backbuffer flipping with that of the display device's refresh rate.\nCurrently ON.");
		}
		else
		{
			pCB->mpTooltip->setAsText("Toggle between synchronization of the window's backbuffer flipping with that of the display device's refresh rate.\nCurrently OFF.");
		}
		CGUIText* pText = pCont->addText("vsync_checkbox_text", 40, 10, "VSync Toggle.");

		CGUISlider* pSlider = pCont->addSlider("backbufferscale", 0, 50, 200, 40);
		pSlider->setTabRatio(0.5f);
		std::string strTxt;
		strTxt = "Adjust the back buffer's dimension scaling.\n";
		strTxt += "A value of 100% would make the back buffer be the same dimensions as the window.\n";
		strTxt += "A value < 100% makes the back buffer smaller than the window, reducing the amount of work the graphics chipset has to do at the expense of reducing the clarity of the final image.\n";
		strTxt += "A value > 100% makes the back buffer larger than the window, increasing the amount of work the graphics chipset has to do, but increasing the quality of the final image.\n";
		pSlider->mpTooltip->setAsText(strTxt);
		strTxt = "Scale: ";
		StringUtils::appendFloat(strTxt, x->pSettings->getBackbufferScale() * 100.0f, 2);
		strTxt += "%";
		CVector2f vPos;
		vPos.x = pSlider->getPosition().x + (pSlider->getDimensions().x * 0.5f) - (pFont->getTextWidth(strTxt) * 0.5f);
		vPos.y = pSlider->getPosition().y + (pSlider->getDimensions().y * 0.5f) - (pFont->getTextHeight() * 0.5f);
		pText = pCont->addText("backbuffer_slider_text", vPos.x, vPos.y, strTxt);
		pSlider->setTabPos(x->pSettings->getBackbufferScale() * 0.5f);	// Slider goes from 0 to 2
	}

	void SCGUIManager::_updateDefaultContainerSettings(void)
	{
		CGUIContainer* pCont = x->pGUI->getContainer("X:Default:Settings");
		CGUICheckbox* pCB = pCont->getCheckbox("vsync");
		if (pCB->getClicked())
		{
			x->pWindow->setVsync(!x->pWindow->getVSyncEnabled());
			if (x->pWindow->getVSyncEnabled())
			{
				pCB->mpTooltip->setAsText("Toggle between synchronization of the window's backbuffer flipping with that of the display device's refresh rate.\nCurrently ON.");
			}
			else
			{
				pCB->mpTooltip->setAsText("Toggle between synchronization of the window's backbuffer flipping with that of the display device's refresh rate.\nCurrently OFF.");
			}
		}

		CGUITheme* pTheme = pCont->getTheme();
		CResourceFont* pFont = x->pResource->getFont(pTheme->mFonts.text);
		float fTextHeight = pFont->getTextHeight(1.0f);
		CGUISlider* pSlider = pCont->getSlider("backbufferscale");
		std::string strTxt = "Scale: ";
		StringUtils::appendFloat(strTxt, x->pSettings->getBackbufferScale() * 100.0f, 2);
		strTxt += "%";
		CVector2f vPos;
		vPos.x = pSlider->getPosition().x + (pSlider->getDimensions().x * 0.5f) - (pFont->getTextWidth(strTxt) * 0.5f);
		vPos.y = pSlider->getPosition().y + (pSlider->getDimensions().y * 0.5f) - (pFont->getTextHeight() * 0.5f);
		CGUIText* pText = pCont->getText("backbuffer_slider_text");

		// Get scale value
		float fScaleFromTab = pSlider->getTabPos() * 200.0f;	// 0 to 200
		int iScale = (int)fScaleFromTab;
		if (iScale > 98 && iScale < 102)
			iScale = 100;
		fScaleFromTab = (float)iScale;	// Reduce precision
		fScaleFromTab *= 0.01f; // 0 to 2
		if (fScaleFromTab < 0.001f)
			fScaleFromTab = 0.001f;
		if (fScaleFromTab != (int)x->pSettings->getBackbufferScale())
		{
			x->pSettings->setBackbufferScale(fScaleFromTab);
			x->pResource->getFramebuffer("X:backbuffer")->resizeToWindowDimsScaled();
			pText->setText(strTxt);
		}
	}

	void SCGUIManager::setTooltipDelay(float fSeconds)
	{
		_mfTooltipDelaySeconds = fSeconds;
	}

	float SCGUIManager::getTooltipDelay(void) const
	{
		return _mfTooltipDelaySeconds;
	}

	void SCGUIManager::setTooltipOffset(float fOffsetX, float fOffsetY)
	{
		_mv2TooltipOffset.x = fOffsetX;
		_mv2TooltipOffset.y = fOffsetY;
	}

	CVector2f SCGUIManager::getTooltipOffset(void) const
	{
		return _mv2TooltipOffset;
	}

	void SCGUIManager::setContainerAsActive(const std::string& strContainerName)
	{
		// Make sure the container exists
		std::map<std::string, CGUIContainer*>::iterator itCont = _mmapContainers.find(strContainerName);
		ThrowIfTrue(_mmapContainers.end() == itCont, "SCGUIManager:::setContainerAsActive(" + strContainerName + ") failed. The named container doesn't exist.");

		// Go through all containers and set their titlebar text to not in focus
		itCont = _mmapContainers.begin();
		while (itCont != _mmapContainers.end())
		{
			itCont->second->_mTextColour = itCont->second->getTheme()->mColours.containerTitlebarTextNotInFocus;
			itCont++;
		}

		// Set colour of titlebar text
		itCont = _mmapContainers.find(strContainerName);
		itCont->second->_mTextColour = itCont->second->getTheme()->mColours.containerTitlebarTextInFocus;
	}
	
}