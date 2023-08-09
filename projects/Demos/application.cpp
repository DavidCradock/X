#include "PCH.h"
#include "application.h"
#include "resource.h"
#include "demo2D.h"
#include "demoEmpty.h"
#include "demoGUI.h"
#include "demoInstancing.h"
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
		x->pLoadingScreen->onInit(0);
		x->pLoadingScreen->setFadeOut(0.0f);

		// Set window title bar text and set icon
		x->pWindow->setText("X: Demos. F1: Toggle fullscreen. F2: Toggle Vsync. F3: Toggle statistics window.");
		x->pWindow->setIcon(IDI_ICON1);
		// Set mouse cursor
		x->pInput->mouse.setMouseCursorImage("data/X/cursors/new_default.ani");

		// Create GUI to switch between states
		CGUIContainer* pCont = x->pGUI->addContainer("DemoStates");
		pCont->setDimensions(640, 480);
		pCont->setPositionCentreWindow();
		std::string strTxt;
		strTxt += "Welcome to the demo application.\n";
		strTxt += "This contains all the demos which act as unit tests for X.\n";
		strTxt += "You may enter a demo state with the buttons below.\n";
		strTxt += "Once a demo state has been entered, you can bring up this window again with the \"Demo States\" button found at the lower right corner of the screen.\n";
		CGUITextScroll* pTextScroll = pCont->addTextScroll("DemoStatesTextScroll", 0, 0, 640, 200, strTxt);
		CGUIButton* pBut;
		CVector2f vTxtPos(0.0f, 210.0f);
		pBut = pCont->addButton("2D", vTxtPos.x, vTxtPos.y, 200, 30, "2D"); vTxtPos.y += 40;								pBut->mpTooltip->setAsText("2D rendering of stuff.");
		pBut = pCont->addButton("Empty", vTxtPos.x, vTxtPos.y, 200, 30, "Empty"); vTxtPos.y += 40;							pBut->mpTooltip->setAsText("An empty demo which does nothing. I use this as a template when adding new demos.");
		pBut = pCont->addButton("GUI", vTxtPos.x, vTxtPos.y, 200, 30, "GUI"); vTxtPos.y += 40;								pBut->mpTooltip->setAsText("Usage of the Graphical User Interface (GUI).");
		pBut = pCont->addButton("Instancing", vTxtPos.x, vTxtPos.y, 200, 30, "Instancing"); vTxtPos.y += 40;				pBut->mpTooltip->setAsText("Speed ups with instancing.");
		pBut = pCont->addButton("Neural Networks", vTxtPos.x, vTxtPos.y, 200, 30, "Neural Networks"); vTxtPos.y += 40;		pBut->mpTooltip->setAsText("AI Neural networks and their training.");
		pBut = pCont->addButton("Oct Tree", vTxtPos.x, vTxtPos.y, 200, 30, "Oct Tree"); vTxtPos.y += 40;					pBut->mpTooltip->setAsText("Oct tree for culling.");
		pBut = pCont->addButton("Physics", vTxtPos.x, vTxtPos.y, 200, 30, "Physics"); vTxtPos.set(220, 210);				pBut->mpTooltip->setAsText("Testing the physics engine here.");
		pBut = pCont->addButton("Quad Tree", vTxtPos.x, vTxtPos.y, 200, 30, "Quad Tree"); vTxtPos.y += 40;		 			pBut->mpTooltip->setAsText("Quad tree for culling.");
		pBut = pCont->addButton("Scene Manager", vTxtPos.x, vTxtPos.y, 200, 30, "Scene Manager"); 	vTxtPos.y += 40;		pBut->mpTooltip->setAsText("3D Scene manager testing.");

		// Create the demo states
		CStateDemo2D* pStateDemo2D = new CStateDemo2D;
		_mFSM.addState("demo2D", pStateDemo2D);

		CStateDemoEmpty* pStateDemoEmpty = new CStateDemoEmpty;
		_mFSM.addState("demoEmpty", pStateDemoEmpty);

		CStateDemoGUI* pStateDemoGUI = new CStateDemoGUI;
		_mFSM.addState("demoGUI", pStateDemoGUI);

		CStateDemoInstancing* pStateDemoInstancing = new CStateDemoInstancing;
		_mFSM.addState("demoInstancing", pStateDemoInstancing);

		CStateDemoNeuralNets* pStateNeuralNets = new CStateDemoNeuralNets;
		_mFSM.addState("demoNeuralNets", pStateNeuralNets);

		CStateDemoOctTree* pStateOctTree = new CStateDemoOctTree;
		_mFSM.addState("demoOctTree", pStateOctTree);

		CStateDemoPhysics* pStateDemoPhysics = new CStateDemoPhysics;
		_mFSM.addState("demoPhysics", pStateDemoPhysics);

		CStateDemoQuadTree* pStateQuadTree = new CStateDemoQuadTree;
		_mFSM.addState("demoQuadTree", pStateQuadTree);

		CStateDemoSceneManager* pStateSceneManager = new CStateDemoSceneManager;
		_mFSM.addState("demoSceneManager", pStateSceneManager);

		// Add button in lower right to show demo states container
		pCont = x->pGUI->addContainer("DemoStatesButton");
		pCont->setBehaviour(false);
		pCont->setDimensions(150, 40);
		pCont->setPosition(float(x->pWindow->getWidth() - 150), float(x->pWindow->getHeight() - 40));
		pCont->addButton("DemoStatesShow", 0, 0, 150, 40, "Demo States");

		// End of loading screen
		x->pLoadingScreen->onInitEnd();

		// TEMP
		// Switch to state GUI straight away, and hide the Demo states container
		_mFSM.switchToState("demoGUI");
		x->pGUI->getContainer("DemoStates")->setVisible(false);
	}

	void CApplication::onStart(void)
	{
	}

	void CApplication::onStop(void)
	{
	}

	bool CApplication::onUpdate(void)
	{
		// Check DemoState window
		CGUIContainer* pCont = x->pGUI->getContainer("DemoStates");
		if (pCont->getVisible())
		{
			CGUIButton* pBut;
			pBut = pCont->getButton("2D");
			if (pBut->getClicked())
			{
				_mFSM.switchToState("demo2D");
				pCont->setVisible(false);
			}
			pBut = pCont->getButton("Empty");
			if (pBut->getClicked())
			{
				_mFSM.switchToState("demoEmpty");
				pCont->setVisible(false);
			}
			pBut = pCont->getButton("GUI");
			if (pBut->getClicked())
			{
				_mFSM.switchToState("demoGUI");
				pCont->setVisible(false);
			}
			pBut = pCont->getButton("Instancing");
			if (pBut->getClicked())
			{
				_mFSM.switchToState("demoInstancing");
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
		if (x->pGUI->getContainer("DemoStatesButton")->getButton("DemoStatesShow")->getClicked())
		{
			x->pGUI->getContainer("DemoStates")->setVisible(true);
		}

		_mFSM.update();

		// Escape key to exit
		if (x->pInput->key.pressed(KC_ESCAPE))
			return false;

		// Toggle fullscreen
		if (x->pInput->key.once(KC_F1))
			SCWindow::getPointer()->toggleFullscreen();

		// Toggle vertical sync
		if (x->pInput->key.once(KC_F2))
			SCWindow::getPointer()->setVsync(!x->pWindow->getVSyncEnabled());

		// Toggle statistics window
		if (x->pInput->key.once(KC_F3))
		{
			CGUIContainer* pStatsCont = x->pGUI->getContainer("X:Default:Statistics");
			pStatsCont->setVisible(!pStatsCont->getVisible());
		}
		return true;
	}
}