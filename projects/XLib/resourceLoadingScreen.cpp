#include "PCH.h"
#include "resourceLoadingScreen.h"
#include "GUIManager.h"
#include "stringUtils.h"
#include "singletons.h"

namespace X
{
	SCResourceLoadingScreen::SCResourceLoadingScreen()
	{
		SCLog* pLog = SCLog::getPointer();
		pLog->add("SCResourceLoadingScreen::SCResourceLoadingScreen() called.");

		SCWindow* pWindow = SCWindow::getPointer();

		_mbEnabled = false;
		_miNumResourcesLoaded = 0;
		setFadeIn();
		setFadeOut();
		_mstrMouseCursorBusyFilename = "data/X/cursors/new_default_busy.ani";
		_mvProgressBarPosition.set((pWindow->getDimensions().x * 0.5f) - 300.0f, (pWindow->getDimensions().y * 0.5f));
		_mvProgressBarDims.set(600.0f, 50.0f);
		_mvInfoTextLine1Pos.set((pWindow->getDimensions().x * 0.5f), (pWindow->getDimensions().y * 0.5f) + 130.0f);
		_mvInfoTextLine2Pos.set((pWindow->getDimensions().x * 0.5f), (pWindow->getDimensions().y * 0.5f) + 170.0f);
		_mbShowResLoadedTextOnFadeOut = true;
		_mbShowProgressBarOnFadeOut = false;
		_mColTextLarge.set(1.0f, 1.0f, 1.0f, 1.0f);
		_mColTextSmall.set(1.0f, 1.0f, 1.0f, 1.0f);
		_mvLoadingCompleteTextPos.set(pWindow->getDimensions().x * 0.5f, pWindow->getDimensions().y * 0.5f);

		// Default resources
		_mstrFontSmall = "data/X/fonts/satoshi_22";
		_mstrFontLarge = "data/X/fonts/tanker_80";
		_mstrBackgroundImage = "data/X/textures/default_loading_screenBG.png";
	}

	void SCResourceLoadingScreen::loadingResource(const std::string& strResourceTypeName, const std::string& strResourceName)
	{
		if (!_mbEnabled)
			return;

		// Increase resources loaded count
		_miNumResourcesLoaded++;

		// Update GUI progress bar
		float fProgress = (float)_miTotalNumResourcesToLoad - (float)_miNumResourcesLoaded;	// _miTotalNumResourcesToLoad to 0
		// Prevent divide by zero
		if (!_miTotalNumResourcesToLoad)
			fProgress = 0.0f;
		else
		{
			fProgress /= _miTotalNumResourcesToLoad;	// 1 to 0
			fProgress = 1.0f - fProgress;				// 0 to 1
		}
		x->pGUI->getContainer("LoadingScreen")->getProgressBar("progress")->setProgress(fProgress);

		// Render the loading screen.
		_renderLoadingScreen(strResourceTypeName, strResourceName, 1);

		// During the onInit() method of our application, the mouse cursor may be set.
		// Check this, store it's name to restore to onInitEnd() and re-set the busy cursor
		if (x->pWindow->getSetMouseCursorFilename() != _mstrMouseCursorBusyFilename)	// If cursor has been changed
		{
			_mstrInitialMouseCursorFilename = x->pWindow->getSetMouseCursorFilename();	// Store the new cursor name
			// Set cursor back to busy
			x->pWindow->setMouseCursorImage(_mstrMouseCursorBusyFilename);
		}
	}

	void SCResourceLoadingScreen::onInit(int iTotalNumberOfResourcesToLoad)
	{
		_mbEnabled = true;

		// Save current vsync state (for setting it back in onInitEnd() and disable it now.
		_mbVsyncStatePriorToLoading = x->pWindow->getVSyncEnabled();
		if (_mbVsyncStatePriorToLoading)
			x->pWindow->setVsync(false);

		// Store number of resources to load
		_miTotalNumResourcesToLoad = iTotalNumberOfResourcesToLoad;

		// Setup loading screen GUI container for progress bar
		CGUIContainer* pCont = x->pGUI->addContainer("LoadingScreen");
		pCont->setBehaviour(false);
		pCont->setVisible(true);
		CGUIProgressBar* pPB = pCont->addProgressBar("progress", _mvProgressBarPosition.x, _mvProgressBarPosition.y, _mvProgressBarDims.x, _mvProgressBarDims.y);
		_mvecContainerVisibilityStates.clear();
		
		// Deal with mouse cursor switch to busy
		// Get previously set name of mouse cursor from SCWindow for restoration afterwards
		_mstrInitialMouseCursorFilename = x->pWindow->getSetMouseCursorFilename();
		// Set cursor to busy
		x->pWindow->setMouseCursorImage(_mstrMouseCursorBusyFilename);

		// Fade in
		// Fade out..
		_mfFadeInCountdown = _mfFadeInTimeSeconds;
		CTimer timer;
		timer.update();
		while (_mfFadeInCountdown > 0.0f)
		{
			timer.update();
			_mfFadeInCountdown -= timer.getSecondsPast();
			_renderLoadingScreen("", "", 0);
		}
	}

	void SCResourceLoadingScreen::onInitEnd(void)
	{
		_mbEnabled = false;

		// Turn vync to it's state it was before onInit() was called, which disables it.
		x->pWindow->setVsync(_mbVsyncStatePriorToLoading);

		// Set cursor to what it was set to before all this began
		// During the onInit() method of our application, the mouse cursor may be set.
		if (x->pWindow->getSetMouseCursorFilename() != _mstrMouseCursorBusyFilename)	// If cursor has been changed
		{
			_mstrInitialMouseCursorFilename = x->pWindow->getSetMouseCursorFilename();	// Store the new cursor name
		}

		x->pWindow->setMouseCursorImage(_mstrInitialMouseCursorFilename);

		// Fade out..
		_mfFadeOutCountdown = _mfFadeOutTimeSeconds;
		CTimer timer;
		timer.update();
		while (_mfFadeOutCountdown > 0.0f)
		{
			timer.update();
			_mfFadeOutCountdown -= timer.getSecondsPast();
			_renderLoadingScreen("", "", 2);
		}

		// Restore visibility state for all GUI containers
		for (int i = 0; i < (int)_mvecContainerVisibilityStates.size(); i++)
			_mvecContainerVisibilityStates[i].pContainer->setVisible(_mvecContainerVisibilityStates[i].bVisibility);
		_mvecContainerVisibilityStates.clear();

		// Also remove container containing progress bar
		x->pGUI->removeContainer("LoadingScreen");

		// Reset number of resources loaded
		_miNumResourcesLoaded = 0;
	}

	void SCResourceLoadingScreen::_renderLoadingScreen(const std::string& strResourceTypeName, const std::string& strResourceName, int iState)
	{
		// Get visibility for all GUI containers, storing each containers' state so we can restore it in onInitEnd().
		// Also disable their visibility here.
		for (int i = 0; i < x->pGUI->getNumberOfContainers(); i++)
		{
			std::string strContainerName = x->pGUI->getContainerName(i);
			SContainerState state;
			state.pContainer = x->pGUI->getContainer(strContainerName);
			state.bVisibility = state.pContainer->getVisible();

			// If the container already exists in the vector, do nothing, as it's previously been added and set to invisible
			bool bContainerAlreadyAdded = false;
			for (int i = 0; i < _mvecContainerVisibilityStates.size(); i++)
			{
				if (_mvecContainerVisibilityStates[i].pContainer == state.pContainer)
					bContainerAlreadyAdded = true;
			}
			// Add state
			if (!bContainerAlreadyAdded)
			{
				_mvecContainerVisibilityStates.push_back(state);
				if (strContainerName != "LoadingScreen")
					state.pContainer->setVisible(false);
			}
		}

		// Obtain loading screen resources
		std::string strTxtResName;
		CResourceFramebuffer* pBGFB = x->pResource->getFramebuffer("X:backbuffer_FB");
		
		// Add required font resource if it doesn't exist
		CResourceFont* pFontSmall;
		strTxtResName = "LOADING_SCREEN: " + _mstrFontSmall;
		if (!x->pResource->getFontExists(strTxtResName))					// If the resource doesn't yet exist
		{
			_mbEnabled = false;
			x->pResource->addFont(strTxtResName, _mstrFontSmall);		// Add it
			_mbEnabled = true;
		}
		pFontSmall = x->pResource->getFont(strTxtResName);

		// Add required font resource if it doesn't exist
		CResourceFont* pFontLarge;
		strTxtResName = "LOADING_SCREEN: " + _mstrFontLarge;
		if (!x->pResource->getFontExists(strTxtResName))					// If the resource doesn't yet exist
		{
			_mbEnabled = false;
			x->pResource->addFont(strTxtResName, _mstrFontLarge);		// Add it
			_mbEnabled = true;
		}
		pFontLarge = x->pResource->getFont(strTxtResName);

		// Add required texture resource if it doesn't exist
		CResourceTexture2DFromFile* pTextureBG;
		strTxtResName = "LOADING_SCREEN: " + _mstrBackgroundImage;
		if (!x->pResource->getTexture2DFromFileExists(strTxtResName))						// If the resource doesn't yet exist
		{
			_mbEnabled = false;
			x->pResource->addTexture2DFromFile(strTxtResName, _mstrBackgroundImage);	// Add it
			_mbEnabled = true;
		}
		pTextureBG = x->pResource->getTexture2DFromFile(strTxtResName);

		// Render the loading screen to the back buffer

		x->pWindow->clearBackbuffer();

		// Bind the X:backbuffer_FB to render target
		pBGFB->bindAsRenderTarget(true, true);	// Both clear and resize to window dims

		// Compute centre of window
		int iWindowCentrePosX = x->pWindow->getWidth() / 2;
		int iWindowCentrePosY = x->pWindow->getHeight() / 2;
		std::string txt;

		// Determine background image's position and dimensions
		int iBGWidth = int(pTextureBG->getDimensions().x);
		int iBGHeight = int(pTextureBG->getDimensions().y);
		int iBGposX = int(iWindowCentrePosX) - (iBGWidth / 2);
		int iBGposY = int(iWindowCentrePosY) - (iBGHeight / 2);
		
		int iTxtPosLargeX = iWindowCentrePosX;
		int iTxtPosLargeY = iWindowCentrePosY - 100;

		// Being called from onInit() during fade in
		if (0 == iState)
		{
			// Compute alpha based upon fade out time
			float fAlpha = _mfFadeInTimeSeconds - _mfFadeInCountdown;	// 0 to _mfFadeInTimeSeconds
			if (_mfFadeInTimeSeconds > 0.0f)
				fAlpha /= _mfFadeInTimeSeconds;	// 0 to 1

			// Render the background texture image
			pTextureBG->renderTo2DQuad(iBGposX, iBGposY, iBGWidth, iBGHeight, CColour(1.0f, 1.0f, 1.0f, fAlpha));

			// Print "Loading" in centre of screen
			pFontLarge->printCentered("Loading", iTxtPosLargeX, iTxtPosLargeY, x->pWindow->getWidth(), x->pWindow->getHeight(), 1.0f, CColour(_mColTextLarge.red, _mColTextLarge.green, _mColTextLarge.blue, fAlpha));

			// Progress bar alpha
			CGUIProgressBar* pPB = x->pGUI->getContainer("LoadingScreen")->getProgressBar("progress");
			pPB->setAlpha(fAlpha);
			pPB->setProgress(0.0f);

			// Render GUI for the progress bar
			x->pGUI->render("X:backbuffer_FB");
		}
		else if (1 == iState)	// Being called from loadingResource()
		{
			// Render the background texture image
			pTextureBG->renderTo2DQuad(iBGposX, iBGposY, iBGWidth, iBGHeight, CColour(1.0f, 1.0f, 1.0f, 1.0f));

			// Print "Loading" in centre of screen
			pFontLarge->printCentered("Loading", iTxtPosLargeX, iTxtPosLargeY, x->pWindow->getWidth(), x->pWindow->getHeight(), 1.0f, CColour(_mColTextLarge.red, _mColTextLarge.green, _mColTextLarge.blue, 1.0f));

			// Print "Loading resource...etc"
			txt = "Loading resource type\"" + strResourceTypeName + "\" named \"" + strResourceName + "\"";
			pFontSmall->printCentered(txt, (int)_mvInfoTextLine1Pos.x, (int)_mvInfoTextLine1Pos.y, x->pWindow->getWidth(), x->pWindow->getHeight(), 1.0f, CColour(_mColTextSmall.red, _mColTextSmall.green, _mColTextSmall.blue, 1.0f));

			// Print "Resources Loaded: X / _miTotalNumResourcesToLoad"
			txt = "Resources Loaded: " + std::to_string(_miNumResourcesLoaded) + " / " + std::to_string(_miTotalNumResourcesToLoad);
			pFontSmall->printCentered(txt, (int)_mvInfoTextLine2Pos.x, (int)_mvInfoTextLine2Pos.y, x->pWindow->getWidth(), x->pWindow->getHeight(), 1.0f, CColour(_mColTextSmall.red, _mColTextSmall.green, _mColTextSmall.blue, 1.0f));

			// Render GUI for the progress bar
			x->pGUI->render("X:backbuffer_FB");
		}
		else  // Being called from onInitEnd() during fade out
		{
			// Compute alpha based upon fade out time
			float fAlpha = _mfFadeOutTimeSeconds - _mfFadeOutCountdown;	// 0 to _mfFadeOutTimeSeconds
			if (_mfFadeOutTimeSeconds > 0.0f)
				fAlpha /= _mfFadeOutTimeSeconds;	// 0 to 1
			fAlpha = 1.0f - fAlpha;	// 1 down to 0

			// Render the background texture image
			pTextureBG->renderTo2DQuad(iBGposX, iBGposY, iBGWidth, iBGHeight, CColour(1.0f, 1.0f, 1.0f, fAlpha));

			// Print "Loading Complete" in centre of screen
			pFontLarge->printCentered("Loading Complete", (int)_mvLoadingCompleteTextPos.x, (int)_mvLoadingCompleteTextPos.y, x->pWindow->getWidth(), x->pWindow->getHeight(), 1.0f, CColour(_mColTextLarge.red, _mColTextLarge.green, _mColTextLarge.blue, fAlpha));

			// Print "Resources Loaded: X / _miTotalNumResourcesToLoad"
			if (_mbShowResLoadedTextOnFadeOut)
			{
				txt = "Resources Loaded: " + std::to_string(_miNumResourcesLoaded) + " / " + std::to_string(_miTotalNumResourcesToLoad);
				pFontSmall->printCentered(txt, (int)_mvInfoTextLine2Pos.x, (int)_mvInfoTextLine2Pos.y, x->pWindow->getWidth(), x->pWindow->getHeight(), 1.0f, CColour(_mColTextSmall.red, _mColTextSmall.green, _mColTextSmall.blue, fAlpha));
			}

			// Progress bar alpha
			CGUIProgressBar* pPB = x->pGUI->getContainer("LoadingScreen")->getProgressBar("progress");
			pPB->setProgress(1.0f);
			if (_mbShowProgressBarOnFadeOut)
				pPB->setAlpha(fAlpha);
			else
				pPB->setAlpha(0.0f);
			
			// Render GUI for the progress bar
			x->pGUI->render("X:backbuffer_FB");
		}

		// Unbind the X:backbuffer_FB and render to the back buffer
		pBGFB->unbindAsRenderTarget();

		// Now render the "X:backbuffer_FB" to the backbuffer
		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		pBGFB->renderTo2DQuad(0, 0, x->pWindow->getWidth(), x->pWindow->getHeight());
		glDisable(GL_BLEND);

		// Swap buffers
		x->pWindow->swapBuffers();

		// Check messages, to update the message loop and make sure the cursor is set
		x->pWindow->checkMessages();
	}

	void SCResourceLoadingScreen::setFontSmall(const std::string& strFilename)
	{
		// Set this to false until the resource has been added, otherwise weird stuff happens.
		// This is because we need to add the resource to the manager, which in turn will call the loadingResource() method.
		_mbEnabled = false;

		// Remove old resource
		std::string strTxtResName = "LOADING_SCREEN: " + _mstrFontSmall;
		x->pResource->removeFont(strTxtResName);
		
		// Add the new resource
		_mstrFontSmall = strFilename;
		strTxtResName = "LOADING_SCREEN: " + _mstrFontSmall;
		x->pResource->addFont(strTxtResName, strFilename);

		// Now re-enable the loading screen so it may proceed as normal
		_mbEnabled = true;
	}

	void SCResourceLoadingScreen::setFontLarge(const std::string& strFilename)
	{
		// Set this to false until the resource has been added, otherwise weird stuff happens.
		// This is because we need to add the resource to the manager, which in turn will call the loadingResource() method.
		_mbEnabled = false;

		// Remove old resource
		std::string strTxtResName = "LOADING_SCREEN: " + _mstrFontLarge;
		x->pResource->removeFont(strTxtResName);

		// Add the new resource
		_mstrFontLarge = strFilename;
		strTxtResName = "LOADING_SCREEN: " + _mstrFontLarge;
		x->pResource->addFont(strTxtResName, strFilename);

		// Now re-enable the loading screen so it may proceed as normal
		_mbEnabled = true;
	}

	void SCResourceLoadingScreen::setBackground(const std::string& strFilename)
	{
		// Set this to false until the resource has been added, otherwise weird stuff happens.
		// This is because we need to add the resource to the manager, which in turn will call the loadingResource() method.
		_mbEnabled = false;

		// Remove old resource
		std::string strTxtResName = "LOADING_SCREEN: " + _mstrBackgroundImage;
		x->pResource->removeTexture2DFromFile(strTxtResName);

		// Add the new resource
		_mstrBackgroundImage = strFilename;
		strTxtResName = "LOADING_SCREEN: " + _mstrBackgroundImage;
		x->pResource->addTexture2DFromFile(strTxtResName, strFilename);

		// Now re-enable the loading screen so it may proceed as normal
		_mbEnabled = true;
	}

	void SCResourceLoadingScreen::setFadeIn(float fFadeInTimeSeconds)
	{
		_mfFadeInTimeSeconds = fFadeInTimeSeconds;
	}

	void SCResourceLoadingScreen::setFadeOut(float fFadeOutTimeSeconds)
	{
		_mfFadeOutTimeSeconds = fFadeOutTimeSeconds;
	}

	void SCResourceLoadingScreen::setMouseCursorBusyFilename(const std::string strFilename)
	{
		_mstrMouseCursorBusyFilename = strFilename;
	}

	void SCResourceLoadingScreen::setProgressBar(const CVector2f& vPosition, const CVector2f& vDimensions)
	{
		_mvProgressBarPosition = vPosition;
		_mvProgressBarDims = vDimensions;
	}

	void SCResourceLoadingScreen::setInfoTextPos(const CVector2f& vLine1Position, const CVector2f& vLine2Position)
	{
		_mvInfoTextLine1Pos = vLine1Position;
		_mvInfoTextLine2Pos = vLine2Position;
	}

	void SCResourceLoadingScreen::setShowResourcesLoadedTextDuringFadeOut(bool bShowText)
	{
		_mbShowResLoadedTextOnFadeOut = bShowText;
	}
	
	void SCResourceLoadingScreen::setShowProgressBarDuringFadeOut(bool bShowProgressBar)
	{
		_mbShowProgressBarOnFadeOut = bShowProgressBar;
	}

	void SCResourceLoadingScreen::setTextColours(const CColour& largeTextColour, const CColour& smallTextColour)
	{
		_mColTextLarge = largeTextColour;
		_mColTextSmall = smallTextColour;
	}

	void SCResourceLoadingScreen::setTextLoadingCompletePos(const CVector2f& vPosition)
	{
		_mvLoadingCompleteTextPos = vPosition;
	}
}