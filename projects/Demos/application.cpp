#include "PCH.h"
#include "application.h"
#include "resource.h"
#include "demo2D.h"
#include "demoNeuralNets.h"
#include "demoOctTree.h"
#include "demoPhysics.h"
#include "demoQuadTree.h"
#include "demoSceneManager.h"

namespace X
{
	void CApplication::initOnce(void)
	{
		// Use the resource loading screen
		SCResourceLoadingScreen* pLS = SCResourceLoadingScreen::getPointer();
		pLS->onInit(0);
		pLS->setFadeOut(1.0f);

		// Set window title bar text and set icon
		SCWindow* pWindow = SCWindow::getPointer();
		pWindow->setText("X: Demos. F1: Toggle fullscreen. F2: Toggle Vsync. F3: Toggle statistics window.");
		pWindow->setIcon(IDI_ICON1);
		// Set mouse cursor
		SCInputManager::getPointer()->mouse.setMouseCursorImage("data/X/cursors/new_default.ani");

		// Create the demo states
		CStateDemo2D* pStateDemo2D = new CStateDemo2D;								_mFSM.addState("demo2D", pStateDemo2D);
		CStateDemoNeuralNets* pStateNeuralNets = new CStateDemoNeuralNets;			_mFSM.addState("demoNeuralNets", pStateNeuralNets);
		CStateDemoOctTree* pStateOctTree = new CStateDemoOctTree;					_mFSM.addState("demoOctTree", pStateOctTree);
		CStateDemoPhysics* pStateDemoPhysics = new CStateDemoPhysics;				_mFSM.addState("demoPhysics", pStateDemoPhysics);
		CStateDemoQuadTree* pStateQuadTree = new CStateDemoQuadTree;				_mFSM.addState("demoQuadTree", pStateQuadTree);
		CStateDemoSceneManager* pStateSceneManager = new CStateDemoSceneManager;	_mFSM.addState("demoSceneManager", pStateSceneManager);

		// Create GUI to switch between states
		SCGUIManager* pGUI = SCGUIManager::getPointer();
		CGUIContainer* pCont = pGUI->addContainer("DemoStates");
		pCont->setDimensions(640, 480);
		pCont->setPositionCentreWindow();
		std::string strTxt;
		strTxt += "Welcome to the demo application.\n";
		strTxt += "This contains all the demos which act as unit tests for X.\n";
		strTxt += "You may enter a demo state with the buttons below.\n";
		strTxt += "Once a demo state has been entered, you can bring up this window again with the \"Demo States\" button found at the lower right corner of the screen.\n";
		CGUITextScroll* pTextScroll = pCont->addTextScroll("DemoStatesTextScroll", 0, 0, 640, 200, strTxt);
		CGUIButton* pBut;
		float fPosY = 210.0f;
		pBut = pCont->addButton("2D", 0, fPosY, 200, 30, "2D"); fPosY += 40;
		pBut = pCont->addButton("Neural Networks", 0, fPosY, 200, 30, "Neural Networks"); fPosY += 40;
		pBut = pCont->addButton("Oct Tree", 0, fPosY, 200, 30, "Oct Tree"); fPosY += 40;
		pBut = pCont->addButton("Physics", 0, fPosY, 200, 30, "Physics"); fPosY += 40;
		pBut = pCont->addButton("Quad Tree", 0, fPosY, 200, 30, "Quad Tree"); fPosY += 40;
		pBut = pCont->addButton("Scene Manager", 0, fPosY, 200, 30, "Scene Manager"); fPosY += 40;

		// Add button in lower right to show demo states container
		pCont = pGUI->addContainer("DemoStatesButton");
		pCont->setBehaviour(false);
		pCont->setDimensions(150, 40);
		pCont->setPosition(float(pWindow->getWidth() - 150), float(pWindow->getHeight() - 40));
		pCont->addButton("DemoStatesShow", 0, 0, 150, 40, "Demo States");

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
		// Get pointers to needed managers
		SCInputManager* pInput = SCInputManager::getPointer();
		SCGUIManager* pGUI = SCGUIManager::getPointer();

		// Check DemoState window
		CGUIContainer* pCont = pGUI->getContainer("DemoStates");
		if (pCont->getVisible())
		{
			CGUIButton* pBut;
			pBut = pCont->getButton("2D");
			if (pBut->getClicked())
			{
				_mFSM.switchToState("demo2D");
				pCont->setVisible(false);
			}
			pBut = pCont->getButton("Neural Networks");
			if (pBut->getClicked())
			{
				_mFSM.switchToState("demoNeuralNets");
				pCont->setVisible(false);
			}
			pBut = pCont->getButton("Oct Tree");
			if (pBut->getClicked())
			{
				_mFSM.switchToState("demoOctTree");
				pCont->setVisible(false);
			}
			pBut = pCont->getButton("Physics");
			if (pBut->getClicked())
			{
				_mFSM.switchToState("demoPhysics");
				pCont->setVisible(false);
			}
			pBut = pCont->getButton("Quad Tree");
			if (pBut->getClicked())
			{
				_mFSM.switchToState("demoQuadTree");
				pCont->setVisible(false);
			}
			pBut = pCont->getButton("Scene Manager");
			if (pBut->getClicked())
			{
				_mFSM.switchToState("demoSceneManager");
				pCont->setVisible(false);
			}
		}

		// Check DemoStatesShow
		if (pGUI->getContainer("DemoStatesButton")->getButton("DemoStatesShow")->getClicked())
		{
			pGUI->getContainer("DemoStates")->setVisible(true);
		}

		_mFSM.update();

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