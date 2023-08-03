#include "PCH.h"
#include "application.h"
#include "resource.h"
#include "state0.h"
#include "state1.h"
#include "state2.h"
#include "state3.h"

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
		pCont->setDimensions(150, 150);
		pCont->setPosition(999999, 999999);
		float fYpos = 5;
		pCont->addButton("state0", 0, fYpos, 150, 30, "State 0")->mpTooltip->setAsText("Click me to switch to state zero."); fYpos += 40;
		pCont->addButton("state1", 0, fYpos, 150, 30, "State 1")->mpTooltip->setAsText("Click me to switch to state one."); fYpos += 40;
		pCont->addButton("state2", 0, fYpos, 150, 30, "State 2")->mpTooltip->setAsText("Click me to switch to state two."); fYpos += 40;
		pCont->addButton("state3", 0, fYpos, 150, 30, "State 3")->mpTooltip->setAsText("Click me to switch to state three."); fYpos += 40;

		// Create all resources for each of the states
		SCResourceManager* pRM = SCResourceManager::getPointer();

		// State0
		// Top down creature rotation images
		std::vector<std::string> vstrImageFilenames = StringUtils::getFilesInDir("data/Demo2D/images/creature_top_down/");	// Get filenames 
		pRM->addTexture2DAtlas("Scene0Creatures", vstrImageFilenames, true, 1);												// Create texture atlas

		// State1
		// Tank body and turret image
		vstrImageFilenames.clear();
		vstrImageFilenames.push_back("data/Demo2D/images/tank_body.png");
		vstrImageFilenames.push_back("data/Demo2D/images/tank_turret.png");
		pRM->addTexture2DAtlas("Scene1Tank", vstrImageFilenames, true, 1);

		// State2
		
		// Create the application states
		CState0* pState0 = new CState0;	_mFSM.addState("state0", pState0);
		CState1* pState1 = new CState1;	_mFSM.addState("state1", pState1);
		CState2* pState2 = new CState2;	_mFSM.addState("state2", pState2);
		CState3* pState3 = new CState3;	_mFSM.addState("state3", pState3);
		_mFSM.switchToState("state1");

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
		{
			_mFSM.switchToState("state0");
			pGUI->setContainerAsActive("State Selection.");
		}
		pBut = pCont->getButton("state1");
		if (pBut->getClicked())
		{
			_mFSM.switchToState("state1");
			pGUI->setContainerAsActive("State Selection.");
		}
		pBut = pCont->getButton("state2");
		if (pBut->getClicked())
		{
			_mFSM.switchToState("state2");
			pGUI->setContainerAsActive("State Selection.");
		}
		pBut = pCont->getButton("state3");
		if (pBut->getClicked())
		{
			_mFSM.switchToState("state3");
			pGUI->setContainerAsActive("State Selection.");
		}

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