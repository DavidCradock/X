#include "PCH.h"
#include "demo2Dstate4.h"

namespace X
{
	void CDemo2DState4::onEnter(void)
	{
		// Create state window
		CGUIContainer* pCont = x->pGUI->addContainer("state4");
		pCont->setDimensions(640, 320);
		pCont->setPosition((x->pResource->getBackbufferDims().x * 0.5f) - 320.0f, 0);
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
		CGUIText* pText = pCont->addText("campos", 0, 250, "");
		pText = pCont->addText("numtiles", 0, 290, "");

		// Create entity's random position and velocity
		CVector2f vWndDims = x->pWindow->getDimensions();
		CVector2f vWndDimsHalf = vWndDims * 0.5f;

		// Setup 2D renderer
		C2DWorld* pWorld = x->p2dRenderer->addWorld("world");
		C2DCamera* pCamera = pWorld->addCamera("camera");
//		pCamera->setPosition(-vWndDimsHalf.x, -vWndDimsHalf.y);

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

		// Now add some debug line entities to a layer above the map layer
		pLayer = pWorld->addLayer("debug");
		C2DEntityLine* pEntityLine = pLayer->addEntityLine("0_0");		pEntityLine->setAsCircle(100, 36);
		CResourceFramebuffer* pBackbuffer = x->pResource->getFramebuffer(x->pResource->defaultRes.framebuffer_backbuffer_FB);
		CVector2f vBackbufferDims = pBackbuffer->getDimensions();
		pEntityLine = pLayer->addEntityLine("W_0");	pEntityLine->setAsCircle(100, 36);	pEntityLine->setPosition(CVector2f(vBackbufferDims.x, 0));
		pEntityLine = pLayer->addEntityLine("W_H");	pEntityLine->setAsCircle(100, 36);	pEntityLine->setPosition(CVector2f(vBackbufferDims.x, vBackbufferDims.y));
		pEntityLine = pLayer->addEntityLine("0_H");	pEntityLine->setAsCircle(100, 36);	pEntityLine->setPosition(CVector2f(0, vBackbufferDims.y));

		_mTimer.update();

		// TEMP
		// Write out map
		pMap->save("data/x/maps/map_test.map");
		pMap->load("data/x/maps/map_test.map");
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
		C2DMap* pMap = pLayer->getMap("map");

		// "Select" tile at cursor position and make it red
		C2DMapTile* pMapTile = pMap->getTileAtScreenPosition(pCamera->getPosition(), x->pInput->mouse.getCursorPos());
		static C2DMapTile* pMapTilePrev = pMapTile;
		if (pMapTilePrev)
			pMapTilePrev->setColour(CColour());
		if (pMapTile)	// Might not've found a tile if mouse cursor isn't over any
			pMapTile->setColour(CColour(1.0f, 0.0f, 0.0f, 1.0f));
		pMapTilePrev = pMapTile;
		
		// Move camera
		CVector2f vCamPos = pCamera->getPosition();
		float fCameraMultiplier = 250.0f;
		if (x->pInput->key.pressed(KC_LSHIFT) || x->pInput->key.pressed(KC_RSHIFT))
			fCameraMultiplier *= 10.0f;
		if (x->pInput->key.pressed(KC_UP))
			vCamPos.y -= _mTimer.getSecondsPast() * fCameraMultiplier;
		if (x->pInput->key.pressed(KC_DOWN))
			vCamPos.y += _mTimer.getSecondsPast() * fCameraMultiplier;
		if (x->pInput->key.pressed(KC_LEFT))
			vCamPos.x -= _mTimer.getSecondsPast() * fCameraMultiplier;
		if (x->pInput->key.pressed(KC_RIGHT))
			vCamPos.x += _mTimer.getSecondsPast() * fCameraMultiplier;
		pCamera->setPosition(vCamPos);

		// Update GUI
		CGUIContainer* pCont = x->pGUI->getContainer("state4");
		CGUIText* pText = pCont->getText("campos");
		std::string strTxt;
		strTxt = "Camera Position: ";
		StringUtils::appendCVector3f(strTxt, CVector3f(vCamPos.x, vCamPos.y, 0.0f), 1);
		pText->setText(strTxt);
		pText = pCont->getText("numtiles");
		strTxt = "Number of Tiles Rendered: ";
		int iStats[2];
		pLayer->getMap("map")->getStatsTilesRendered(iStats[0], iStats[1]);
		StringUtils::appendInt(strTxt, iStats[0]);
		strTxt += " / ";
		StringUtils::appendInt(strTxt, iStats[1]);
		pText->setText(strTxt);


//		x->p2dRenderer->render();

		// Deal with fullscreen toggling
		if (bFullscreenToggle)
		{
			bFullscreenToggle = false;
			
			CResourceFramebuffer* pBackbuffer = x->pResource->getFramebuffer(x->pResource->defaultRes.framebuffer_backbuffer_FB);
			CVector2f vBackbufferDims = pBackbuffer->getDimensions();
			C2DWorld* pWorld = x->p2dRenderer->getWorld("world");
			C2DLayer* pLayer = pWorld->getLayer("debug");
			C2DEntityLine* pEntityLine;
			pEntityLine = pLayer->getEntityLine("0_0");	pEntityLine->setPosition(CVector2f(0, 0));
			pEntityLine = pLayer->getEntityLine("W_0");	pEntityLine->setPosition(CVector2f(vBackbufferDims.x, 0));
			pEntityLine = pLayer->getEntityLine("W_H");	pEntityLine->setPosition(CVector2f(vBackbufferDims.x, vBackbufferDims.y));
			pEntityLine = pLayer->getEntityLine("0_H");	pEntityLine->setPosition(CVector2f(0, vBackbufferDims.y));
		}
	}
}