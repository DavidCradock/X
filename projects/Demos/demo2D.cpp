#include "PCH.h"
#include "demo2D.h"
#include "demo2Dstate0.h"
#include "demo2Dstate1.h"
#include "demo2Dstate2.h"
#include "demo2Dstate3.h"
#include "demo2Dstate4.h"

namespace X
{
	void CStateDemo2D::onEnter(void)
	{
		// Use the resource loading screen
		x->pLoadingScreen->onInit(3);

		// Create GUI shared between states
		CGUIContainer* pCont = x->pGUI->addContainer("State Selection.");
		pCont->setDimensions(150, 190);
		pCont->setPosition(999999, (x->pResource->getBackbufferDims().y * 0.5f) - (pCont->getDimensions().y * 0.5f));
		float fYpos = 5;
		pCont->addButton("state0", 0, fYpos, 150, 30, "State 0")->mpTooltip->setAsText("Click me to switch to state zero."); fYpos += 40;
		pCont->addButton("state1", 0, fYpos, 150, 30, "State 1")->mpTooltip->setAsText("Click me to switch to state one."); fYpos += 40;
		pCont->addButton("state2", 0, fYpos, 150, 30, "State 2")->mpTooltip->setAsText("Click me to switch to state two."); fYpos += 40;
		pCont->addButton("state3", 0, fYpos, 150, 30, "State 3")->mpTooltip->setAsText("Click me to switch to state three."); fYpos += 40;
		pCont->addButton("state4", 0, fYpos, 150, 30, "State 4")->mpTooltip->setAsText("Click me to switch to state four."); fYpos += 40;

		// Create all resources for each of the states

		// State0
		// Top down creature rotation images
		x->pResource->addTexture2DAtlas("Scene0Creatures", "data/Demos/Demo2D/images/creature_top_down/", true, 1);

		// State1
		// Tank body and turret image
		std::vector<std::string> vstrImageFilenames;
		vstrImageFilenames.push_back("data/Demos/Demo2D/images/tank_body.png");
		vstrImageFilenames.push_back("data/Demos/Demo2D/images/tank_turret.png");
		x->pResource->addTexture2DAtlas("Scene1Tank", vstrImageFilenames, true, 1);

		// State2

		// State3
	
		// State4
		x->pResource->addTexture2DAtlas("map_atlas", "data/x/textures/map_tiles/", true, 10);

		// TEMP SAVE atlas
		CImageAtlasPacker* pAtlasImagePacker = x->pResource->getTexture2DAtlas("map_atlas")->getImageAtlasPacker();
		pAtlasImagePacker->getAtlasImage(0)->saveAsPNG("ATLAS.png");



		// Create the application states
		CDemo2DState0* pState0 = new CDemo2DState0;	_mFSM.addState("state0", pState0);
		CDemo2DState1* pState1 = new CDemo2DState1;	_mFSM.addState("state1", pState1);
		CDemo2DState2* pState2 = new CDemo2DState2;	_mFSM.addState("state2", pState2);
		CDemo2DState3* pState3 = new CDemo2DState3;	_mFSM.addState("state3", pState3);
		CDemo2DState4* pState4 = new CDemo2DState4;	_mFSM.addState("state4", pState4);

		// Switch to state
		_mFSM.switchToState("state4");

		// End of loading screen
		x->pLoadingScreen->onInitEnd();
	}

	void CStateDemo2D::onExit(void)
	{
		x->pGUI->removeContainer("State Selection.");
		x->pResource->removeTexture2DAtlas("Scene0Creatures");
		x->pResource->removeTexture2DAtlas("Scene1Tank");
		x->pResource->removeTexture2DAtlas("map_atlas");
		_mFSM.removeAllStates();
	}

	void CStateDemo2D::onActive(CFiniteStateMachine* pFSM)
	{
		CGUIContainer* pCont = x->pGUI->getContainer("State Selection.");
		CGUIButton* pBut;

		pBut = pCont->getButton("state0");
		if (pBut->getClicked())
		{
			_mFSM.switchToState("state0");
			x->pGUI->setContainerAsActive("State Selection.");
		}
		pBut = pCont->getButton("state1");
		if (pBut->getClicked())
		{
			_mFSM.switchToState("state1");
			x->pGUI->setContainerAsActive("State Selection.");
		}
		pBut = pCont->getButton("state2");
		if (pBut->getClicked())
		{
			_mFSM.switchToState("state2");
			x->pGUI->setContainerAsActive("State Selection.");
		}
		pBut = pCont->getButton("state3");
		if (pBut->getClicked())
		{
			_mFSM.switchToState("state3");
			x->pGUI->setContainerAsActive("State Selection.");
		}
		pBut = pCont->getButton("state4");
		if (pBut->getClicked())
		{
			_mFSM.switchToState("state4");
			x->pGUI->setContainerAsActive("State Selection.");
		}

		// Update current state
		_mFSM.update();

		// Deal with fullscreen toggling
		if (bFullscreenToggle)
		{
			bFullscreenToggle = false;
			CGUIContainer* pCont = x->pGUI->getContainer("State Selection.");
			pCont->setPosition(999999, (x->pResource->getBackbufferDims().y * 0.5f) - (pCont->getDimensions().y * 0.5f));

			CDemo2DState0* pState0 = (CDemo2DState0*)_mFSM.getState("state0");	pState0->bFullscreenToggle = true;
			CDemo2DState1* pState1 = (CDemo2DState1*)_mFSM.getState("state1");	pState1->bFullscreenToggle = true;
			CDemo2DState2* pState2 = (CDemo2DState2*)_mFSM.getState("state2");	pState2->bFullscreenToggle = true;
			CDemo2DState3* pState3 = (CDemo2DState3*)_mFSM.getState("state3");	pState3->bFullscreenToggle = true;
			CDemo2DState4* pState4 = (CDemo2DState4*)_mFSM.getState("state4");	pState4->bFullscreenToggle = true;
		}
	}
}