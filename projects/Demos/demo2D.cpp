#include "PCH.h"
#include "demo2D.h"
#include "demo2Dstate0.h"
#include "demo2Dstate1.h"
#include "demo2Dstate2.h"
#include "demo2Dstate3.h"

namespace X
{
	void CStateDemo2D::onEnter(void)
	{
		// Use the resource loading screen
		SCResourceLoadingScreen* pLS = SCResourceLoadingScreen::getPointer();
		pLS->onInit(3);

		// Set window title bar text and set icon
		SCWindow* pWindow = SCWindow::getPointer();

		// Create GUI shared between states
		SCGUIManager* pGUI = SCGUIManager::getPointer();
		CGUIContainer* pCont = pGUI->addContainer("State Selection.");
		pCont->setDimensions(150, 150);
		pCont->setPosition(999999, float(pWindow->getHeight())-220);
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
		CDemo2DState0* pState0 = new CDemo2DState0;	_mFSM.addState("state0", pState0);
		CDemo2DState1* pState1 = new CDemo2DState1;	_mFSM.addState("state1", pState1);
		CDemo2DState2* pState2 = new CDemo2DState2;	_mFSM.addState("state2", pState2);
		CDemo2DState3* pState3 = new CDemo2DState3;	_mFSM.addState("state3", pState3);
		_mFSM.switchToState("state2");

		// End of loading screen
		pLS->onInitEnd();
	}

	void CStateDemo2D::onExit(void)
	{
		SCGUIManager* pGUI = SCGUIManager::getPointer();
		pGUI->removeContainer("State Selection.");
		SCResourceManager* pRM = SCResourceManager::getPointer();
		pRM->removeTexture2DAtlas("Scene0Creatures");
		pRM->removeTexture2DAtlas("Scene1Tank");
		_mFSM.removeAllStates();
	}

	void CStateDemo2D::onActive(CFiniteStateMachine* pFSM)
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
	}
}