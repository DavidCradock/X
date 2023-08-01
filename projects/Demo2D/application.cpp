#include "PCH.h"
#include "application.h"
#include "resource.h"
#include "state0.h"
#include "state1.h"

namespace X
{
	void CApplication::initOnce(void)
	{
		// Use the resource loading screen
		SCResourceLoadingScreen* pLS = SCResourceLoadingScreen::getPointer();
		pLS->onInit(3);

		// Set window title bar text and set icon
		SCWindow* pWindow = SCWindow::getPointer();
		pWindow->setText("X: Demo2D. F1: Toggle fullscreen. F2: Toggle Vsync. F3: Toggle statistics window.");
		pWindow->setIcon(IDI_ICON1);
		// Set mouse cursor
		SCInputManager::getPointer()->mouse.setMouseCursorImage("data/X/cursors/new_default.ani");

		// Show frame rate statistics
		SCGUIManager::getPointer()->getContainer("X:Default:Statistics")->setVisible(true);

		// Create GUI shared between states
		SCGUIManager* pGUI = SCGUIManager::getPointer();
		CGUIContainer* pCont = pGUI->addContainer("State Selection.");
		pCont->setDimensions(150, 200);
		pCont->setPosition(999999, 999999);
		float fYpos = 5;
		pCont->addButton("state0", 0, fYpos, 150, 30, "State 0")->mpTooltip->setAsText("Click me to switch to state zero."); fYpos += 40;
		pCont->addButton("state1", 0, fYpos, 150, 30, "State 1")->mpTooltip->setAsText("Click me to switch to state one."); fYpos += 40;

		// Create all resources for each of the states
		SCResourceManager* pRM = SCResourceManager::getPointer();

		// State0
		// Top down creature rotation images
		std::vector<std::string> vstrImageFilenames = StringUtils::getFilesInDir("data/Demo2D/images/creature_top_down/");	// Get filenames 
		pRM->addTexture2DAtlas("Scene0Creatures", vstrImageFilenames, true, 1);												// Create texture atlas
		// Space ship hull rotation images
		vstrImageFilenames = StringUtils::getFilesInDir("data/Demo2D/images/spaceship_hull/");								// Get filenames 
		pRM->addTexture2DAtlas("Scene0SpaceshipHull", vstrImageFilenames, true, 1);												// Create texture atlas
		// Space ship turret rotation images
		vstrImageFilenames = StringUtils::getFilesInDir("data/Demo2D/images/spaceship_turret/");							// Get filenames 
		pRM->addTexture2DAtlas("Scene0SpaceshipTurrets", vstrImageFilenames, true, 1);											// Create texture atlas

		// Create the application states
		CState0* pState0 = new CState0;	_mFSM.addState("state0", pState0);
		CState1* pState1 = new CState1;	_mFSM.addState("state1", pState1);
		_mFSM.switchToState("state0");

		// End of loading screen
		pLS->onInitEnd();
	}

	void CApplication::onStart(void)
	{
	}

	void CApplication::onStop(void)
	{
	}

	bool CApplication::onUpdate(void)
	{
		SCGUIManager* pGUI = SCGUIManager::getPointer();
		CGUIContainer* pCont = pGUI->getContainer("State Selection.");
		CGUIButton* pBut;

		pBut = pCont->getButton("state0");
		if (pBut->getClicked())
			_mFSM.switchToState("state0");

		pBut = pCont->getButton("state1");
		if (pBut->getClicked())
			_mFSM.switchToState("state1");

		// Update current state
		_mFSM.update();

		SCInputManager* pInput = SCInputManager::getPointer();

		// Escape key to exit
		if (pInput->key.pressed(KC_ESCAPE))

			return false;
		// Toggle fullscreen
		if (pInput->key.once(KC_F1))
			SCWindow::getPointer()->toggleFullscreen();

		// Toggle vertical sync
		if (pInput->key.once(KC_F2))
			SCWindow::getPointer()->setVsync(!SCWindow::getPointer()->getVSyncEnabled());

		// Toggle statistics window
		if (pInput->key.once(KC_F3))
		{
			CGUIContainer* pStatsCont = SCGUIManager::getPointer()->getContainer("X:Default:Statistics");
			pStatsCont->setVisible(!pStatsCont->getVisible());
		}
		return true;
	}

}