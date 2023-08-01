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
		pLS->onInit(6);

		// Set window title bar text and set icon
		SCWindow* pWindow = SCWindow::getPointer();
		pWindow->setText("X: DemoNeuralNets. F1: Toggle fullscreen. F2: Toggle Vsync. F3: Toggle statistics window.");
		pWindow->setIcon(IDI_ICON1);
		// Set mouse cursor
		SCInputManager::getPointer()->mouse.setMouseCursorImage("data/X/cursors/new_default.ani");

		// Create resources used by the states

		// Used in state0 and state1
		std::vector<std::string> vecstrImageNamesEntity = StringUtils::getFilesInDir("data/DemoNeuralNets/images/creature_top_down/");
		std::vector<std::string> vecstrImageNamesFood = StringUtils::getFilesInDir("data/DemoNeuralNets/images/food/");
		std::vector<std::string> vecstrImageNamesTiles = StringUtils::getFilesInDir("data/DemoNeuralNets/images/floor_tiles/");
		// Used in state1
		std::vector<std::string> vecstrImageNamesEntityEvil = StringUtils::getFilesInDir("data/DemoNeuralNets/images/creature_top_down_evil/");

		// Create texture atlases and entities
		SCResourceManager* pRM = SCResourceManager::getPointer();
		pRM->addTexture2DAtlas("creature_top_down", vecstrImageNamesEntity);
		pRM->addTexture2DAtlas("food", vecstrImageNamesFood);
		pRM->addTexture2DAtlas("tiles", vecstrImageNamesTiles);
		pRM->addTexture2DAtlas("creature_top_down_evil", vecstrImageNamesEntityEvil);

		// Create textures
		pRM->addTexture2DFromFile("critter_diffuse", "data/DemoNeuralNets/textures/critter_diffuse.png", true);
		pRM->addTexture2DFromFile("food_diffuse", "data/DemoNeuralNets/textures/food_diffuse.png", true);

		// Create geometry
		CResourceTriangle* pTri;
		// Critter
		pTri = pRM->addTriangle("critter");
//		pTri->convertObj("data/DemoNeuralNets/geometry/critter.obj");
		pTri->addFromFile("data/DemoNeuralNets/geometry/critter.geom");
		// Food
		pTri = pRM->addTriangle("food");
//		pTri->convertObj("data/DemoNeuralNets/geometry/food.obj");
		pTri->addFromFile("data/DemoNeuralNets/geometry/food.geom");
		// Back "plane" (Has to be a solid object for shadows to render correctly, so it's actually a cube)
		pTri = pRM->addTriangle("background");
		pTri->addCube(CVector3f(0.0f, 0.0f, -0.5f), CVector3f(500.0f, 500.0f, 1.0f));
		pTri->update();

		// Setup finite state machine
		// Create and add all states
		CState0* pStateZero = new CState0;	_mFSM.addState("state0", pStateZero);
		CState1* pStateOne = new CState1;	_mFSM.addState("state1", pStateOne);
		CState2* pStateTwo = new CState2;	_mFSM.addState("state2", pStateTwo);
		CState3* pStateThree = new CState3;	_mFSM.addState("state3", pStateThree);

		// Switch to initial state
		_mFSM.switchToState("state0");

		// Setup GUI stuff which is shared between states
		SCGUIManager* pGUI = SCGUIManager::getPointer();
		CGUIContainer* pCont = pGUI->addContainer("Application state selection.");
		pCont->setDimensions(450, 300);
		// Add state buttons
		float fYpos = 30;
		// Add state 0 button
		CGUIButton* pButton = pCont->addButton("state0", 0, fYpos, 100, 30, "State 0");	pButton->mpTooltip->setAsText("Switch to state zero.");	fYpos += 35;
		// Add state 1 button
		pButton = pCont->addButton("state1", 0, fYpos, 100, 30, "State 1");				pButton->mpTooltip->setAsText("Switch to state one.");	fYpos += 35;
		// Add state 2 button
		pButton = pCont->addButton("state2", 0, fYpos, 100, 30, "State 2");				pButton->mpTooltip->setAsText("Switch to state two.");	fYpos += 35;
		// Add state 3 button
		pButton = pCont->addButton("state3", 0, fYpos, 100, 30, "State 3");				pButton->mpTooltip->setAsText("Switch to state three.");	fYpos += 35;

		// Add text scroll describing each state
		std::string strTxt = "Use these buttons to select which state the application is in.\n \n";
		strTxt += "State0.\nThis state shows training of a neural network whereby some networks try to eat as much food as possible.\n \n";
		strTxt += "State1.\nThis state adds red evil critters which try to eat the smaller critters which still just eat food.\n \n";
		strTxt += "State2.\nThis is the same as state 0, but using the 3D renderer instead of the 2D one.";
		pCont->addTextScroll("desc", 110, 0, 340, 300, strTxt);

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
		SCInputManager* pInput = SCInputManager::getPointer();

		// Timer delta
		timer.update();

		// Update state machine
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