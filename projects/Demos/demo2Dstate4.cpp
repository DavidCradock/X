#include "PCH.h"
#include "demo2Dstate4.h"

namespace X
{
	void CDemo2DState4::onEnter(void)
	{
		// Create state window
		CGUIContainer* pCont = x->pGUI->addContainer("state4");
		pCont->setDimensions(640, 240);
		pCont->setPosition(0, 0);
		pCont->mstrTitleText = "State four.";
		// Add text scroll
		std::string strTxt;
		strTxt += "Welcome to state 4.\n";
		strTxt += "This state shows C2DMap rendering.\n";
		strTxt += "Controls...\n";
		strTxt += "Cursor keys move the camera.\n";
		strTxt += " \nDetails...\n";
		pCont->addTextScroll("TextScroll4", 0, 0, 640, 240, strTxt);
		// Add text
		CGUIText* pText = pCont->addText("campos", 0, 280, "");

		// Create entity's random position and velocity
		CVector2f vWndDims = x->pWindow->getDimensions();
		CVector2f vWndDimsHalf = vWndDims * 0.5f;

		// Setup 2D renderer
		C2DWorld* pWorld = x->p2dRenderer->addWorld("world");
		C2DCamera* pCamera = pWorld->addCamera("camera");
		pCamera->setPosition(-vWndDimsHalf.x, -vWndDimsHalf.y);

		// Create layer which will hold the map
		C2DLayer* pLayer = pWorld->addLayer("layer");

		// Add the map
		C2DMap* pMap = pLayer->addMap("map", "map_atlas", 100, 100);

		// Add a addImageType
		C2DMapImageType* pImageType = pMap->addImageType("tiles");
		pImageType->addFrameImage("data/x/textures/map_tiles/tiles.png");
		pImageType->setFramerate(0.0f);

		// Set all tiles in the map to use the above image type
		pMap->setAllTilesImageType("tiles");
	}

	void CDemo2DState4::onExit(void)
	{
		x->pGUI->removeContainer("state4");
		x->p2dRenderer->removeWorld("world");
	}

	void CDemo2DState4::onActive(CFiniteStateMachine* pFSM)
	{
		CVector2f vWndDims = x->pWindow->getDimensions();
		CVector2f vWndDimsHalf = vWndDims * 0.5f;

		_mTimer.update();
		float fTimeDelta = _mTimer.getSecondsPast();

		C2DWorld* pWorld = x->p2dRenderer->getWorld("world");
		C2DCamera* pCamera = pWorld->getCamera("camera");
		C2DLayer* pLayer = pWorld->getLayer("layer");

		CVector2f vCamPos = pCamera->getPosition();
		if (x->pInput->key.pressed(KC_UP))
			vCamPos.y -= _mTimer.getSecondsPast() * 250.0f;
		if (x->pInput->key.pressed(KC_DOWN))
			vCamPos.y += _mTimer.getSecondsPast() * 250.0f;
		if (x->pInput->key.pressed(KC_LEFT))
			vCamPos.x -= _mTimer.getSecondsPast() * 250.0f;
		if (x->pInput->key.pressed(KC_RIGHT))
			vCamPos.x += _mTimer.getSecondsPast() * 250.0f;
		pCamera->setPosition(vCamPos);

		// Update GUI
		CGUIContainer* pCont = x->pGUI->getContainer("state4");
		CGUIText* pText = pCont->getText("campos");
		std::string strTxt;
		strTxt = "Camera Position: ";
		StringUtils::appendCVector3f(strTxt, CVector3f(vCamPos.x, vCamPos.y, 0.0f), 1);
		pText->setText(strTxt);

//		x->p2dRenderer->render();
	}
}