#include "PCH.h"
#include "demoNeuralNets.h"
#include "DemoNeuralNetsState0.h"
#include "DemoNeuralNetsState1.h"
#include "DemoNeuralNetsState2.h"
#include "DemoNeuralNetsState3.h"

namespace X
{
	void CStateDemoNeuralNets::onEnter(void)
	{
		// Use the resource loading screen
		SCResourceLoadingScreen* pLS = SCResourceLoadingScreen::getPointer();
		pLS->onInit(6);

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
		CResourceVertexBufferBNT* pVB;
		// Critter
		pVB = pRM->addVertexBufferBNT("critter");
		//		pTri->convertObj("data/DemoNeuralNets/geometry/critter.obj");
		pVB->addFromFile("data/DemoNeuralNets/geometry/critter.geom");
		// Food
		pVB = pRM->addVertexBufferBNT("food");
		//		pTri->convertObj("data/DemoNeuralNets/geometry/food.obj");
		pVB->addFromFile("data/DemoNeuralNets/geometry/food.geom");
		// Back "plane" (Has to be a solid object for shadows to render correctly, so it's actually a cube)
		pVB = pRM->addVertexBufferBNT("background");
		pVB->addCube(CVector3f(0.0f, 0.0f, -0.5f), CVector3f(500.0f, 500.0f, 1.0f));
		pVB->update();

		// Setup finite state machine
		// Create and add all states
		CDemoNeuralNetsState0* pStateZero = new CDemoNeuralNetsState0;	_mFSM.addState("state0", pStateZero);
		CDemoNeuralNetsState1* pStateOne = new CDemoNeuralNetsState1;	_mFSM.addState("state1", pStateOne);
		CDemoNeuralNetsState2* pStateTwo = new CDemoNeuralNetsState2;	_mFSM.addState("state2", pStateTwo);
		CDemoNeuralNetsState3* pStateThree = new CDemoNeuralNetsState3;	_mFSM.addState("state3", pStateThree);

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

	void CStateDemoNeuralNets::onExit(void)
	{
		SCGUIManager* pGUI = SCGUIManager::getPointer();
		pGUI->removeContainer("Application state selection.");

		SCResourceManager* pRM = SCResourceManager::getPointer();
		pRM->removeTexture2DAtlas("creature_top_down" );
		pRM->removeTexture2DAtlas("food");
		pRM->removeTexture2DAtlas("tiles");
		pRM->removeTexture2DAtlas("creature_top_down_evil");

		pRM->removeTexture2DFromFile("critter_diffuse");
		pRM->removeTexture2DFromFile("food_diffuse");

		pRM->removeVertexBuffer("critter");
		pRM->removeVertexBuffer("food");
		pRM->removeVertexBuffer("background");

		// Remove states
		_mFSM.removeAllStates();

	}

	void CStateDemoNeuralNets::onActive(CFiniteStateMachine* pFSM)
	{
		SCInputManager* pInput = SCInputManager::getPointer();

		// Timer delta
		timer.update();

		// Update state machine
		_mFSM.update();
	}
}