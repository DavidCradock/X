#include "PCH.h"
#include "demo2Dstate4.h"

namespace X
{
	void CDemo2DState4::onEnter(void)
	{
		_mpMapTilePrev = 0;

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
		strTxt += "Cursor or WSAD keys to move the camera.\n";
		strTxt += "Either shift key increases camera's movement speed.\n";
		strTxt += " \nDetails...\n";
		strTxt += "Moving the mouse over a tile will highlight it and with a left mouse click, will set it to the currently set tile brush.\n";
		strTxt += "Create a new map by clicking on the \"New Map\" button in the \"Editing\" window after setting the desired number of tiles in the text edit boxes above the button.\n";
		strTxt += "\n";
//		pCont->addTextScroll("TextScroll4", 0, 0, 640, 240, strTxt);

		// Add text
		CGUIText* pText = pCont->addText("campos", 0, 250, "");
		pText = pCont->addText("numtiles", 0, 290, "");

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

		pImageType = pMap->addImageType("water_anim");
		for (int i = 1; i < 61; i++)
		{
			strTxt = StringUtils::blenderAnimFilename("data/x/textures/map_tiles/water_anim", ".png", i);
			pImageType->addFrameImage(strTxt);
		}

		// Set all tiles in the map to use the above image type
		pMap->setAllTilesImageType("water_anim");

		// GUI stuff for editing the map
		pCont = x->pGUI->addContainer("state4_edit");
		pCont->mstrTitleText = "Map editing";
		pCont->setDimensions(320, 640);
		// tile_index_at_cursor
		pCont->addText("tile_index_at_cursor", 0, 0, "");
		pCont->addText("Num Tiles X:", 0, 40, "Number of tiles X:");
		CGUITextEdit* pTextEdit;
		pTextEdit = pCont->addTextEdit("numtilesx", 185, 37, 100, 40, "100");
		pTextEdit->setIntegerInputOnly(true);
		pCont->addText("Num Tiles Y:", 0, 90, "Number of tiles Y:");
		pCont->addTextEdit("numtilesy", 185, 82, 100, 40, "100");
		pTextEdit->setIntegerInputOnly(true);
		pCont->addButton("newmap", 0, 120, 320, 40, "New Map");
		
		// Save, load, filename
		pCont->addButton("load", 0, 160, 150, 40, "Load Map");
		pCont->addButton("save", 170, 160, 150, 40, "Save Map");
		pCont->addTextEdit("filename", 0, 210, 320, 40, "data/demos/demo2d/maps/map_test.map");

		// Load in textures for the GUI to use as images
		// and create image buttons for them and add them to the sub container, which we create next...

		std::vector<std::string> vecstrTiles = StringUtils::getFilesInDir("data/x/textures/map_tiles/");
		CVector2f vButtonPos(0, 260);
		for (size_t i = 0; i < vecstrTiles.size(); i++)
		{
			x->pResource->addTexture2DFromFile(vecstrTiles[i], vecstrTiles[i]);
			std::string strButName = "but_";
			strButName += vecstrTiles[i];
			//CGUIButtonImage* pButImage = pCont->addButtonImage(strButName, vButtonPos.x, vButtonPos.y, vecstrTiles[i], vecstrTiles[i], vecstrTiles[i], 32, 32);
//			CGUIButtonImage* pButImage = pSubCont->addButtonImage(strButName, vButtonPos.x, vButtonPos.y, vecstrTiles[i], vecstrTiles[i], vecstrTiles[i], 32, 32);
			vButtonPos.y += 40;
			if (vButtonPos.y > pCont->getDimensions().y)
			{
				vButtonPos.y = 260;
				vButtonPos.x += 40;
			}
		}
		
		// Images from texture atlas
		for (int i = 0; i < (int)x->pResource->getTexture2DAtlas(pMap->getAtlasResourceName())->getNumImages(); i++)
		{
			
		}

		// New map, are you sure confirm window
		pCont = x->pGUI->addContainer("RUSURE");
		pCont->setDimensions(200, 40);
		pCont->mstrTitleText = "Are you sure?";
		pCont->setPositionCentreWindow();
		pCont->addButton("NO", 0, 10, 90, 40, "No")->mpTooltip->setAsText("No, don't do it!");
		pCont->addButton("YES", 110, 10, 90, 40, "Yes")->mpTooltip->setAsText("Yes please.");
		pCont->setVisible(false);

		// Now add some debug line entities to a layer above the map layer
		pLayer = pWorld->addLayer("debug");
		C2DEntityLine* pEntityLine = pLayer->addEntityLine("0_0");		pEntityLine->setAsCircle(100, 36);
		CResourceFramebuffer* pBackbuffer = x->pResource->getFramebuffer(x->pResource->defaultRes.framebuffer_backbuffer_FB);
		CVector2f vBackbufferDims = pBackbuffer->getDimensions();
		pEntityLine = pLayer->addEntityLine("W_0");	pEntityLine->setAsCircle(100, 36);	pEntityLine->setPosition(CVector2f(vBackbufferDims.x, 0));
		pEntityLine = pLayer->addEntityLine("W_H");	pEntityLine->setAsCircle(100, 36);	pEntityLine->setPosition(CVector2f(vBackbufferDims.x, vBackbufferDims.y));
		pEntityLine = pLayer->addEntityLine("0_H");	pEntityLine->setAsCircle(100, 36);	pEntityLine->setPosition(CVector2f(0, vBackbufferDims.y));

		_mTimer.update();
	}

	void CDemo2DState4::onExit(void)
	{
		x->pGUI->removeContainer("state4");
		x->pGUI->removeContainer("state4_edit");
		x->pGUI->removeContainer("RUSURE");
		x->p2dRenderer->removeWorld("world");
		_mpMapTilePrev = 0;
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
		if (_mpMapTilePrev)
			_mpMapTilePrev->setColour(CColour());
		if (pMapTile)	// Might not've found a tile if mouse cursor isn't over any
			pMapTile->setColour(CColour(1.0f, 0.5f, 0.5f, 1.0f));
		_mpMapTilePrev = pMapTile;
		
		// Move camera
		CVector2f vCamPos = pCamera->getPosition();
		float fCameraMultiplier = 250.0f;
		if (x->pInput->key.pressed(KC_LSHIFT) || x->pInput->key.pressed(KC_RSHIFT))
			fCameraMultiplier *= 10.0f;
		if (x->pInput->key.pressed(KC_UP) || x->pInput->key.pressed(KC_W))
			vCamPos.y -= _mTimer.getSecondsPast() * fCameraMultiplier;
		if (x->pInput->key.pressed(KC_DOWN) || x->pInput->key.pressed(KC_S))
			vCamPos.y += _mTimer.getSecondsPast() * fCameraMultiplier;
		if (x->pInput->key.pressed(KC_LEFT) || x->pInput->key.pressed(KC_A))
			vCamPos.x -= _mTimer.getSecondsPast() * fCameraMultiplier;
		if (x->pInput->key.pressed(KC_RIGHT) || x->pInput->key.pressed(KC_D))
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

		// GUI stuff for editing the map
		pCont = x->pGUI->getContainer("state4_edit");
		// tile_index_at_cursor
		pText = pCont->getText("tile_index_at_cursor");
		int iTileIndexX, iTileIndexY;
		pMap->getTileIndicesAtScreenPosition(pCamera->getPosition(), x->pInput->mouse.getCursorPos(), iTileIndexX, iTileIndexY);
		strTxt = "Map tile index at mouse: ";
		StringUtils::appendInt(strTxt, iTileIndexX);
		strTxt += ", ";
		StringUtils::appendInt(strTxt, iTileIndexY);
		pText->setText(strTxt);

		// New map
		if (pCont->getButton("newmap")->getClicked())
			x->pGUI->getContainer("RUSURE")->setVisible(true);

		pCont = x->pGUI->getContainer("RUSURE");
		CGUIButton* pButton;
		if (pCont->getVisible())
		{
			pButton = pCont->getButton("YES");
			if (pButton->getClicked())
			{
				pCont->setVisible(false);
				// Create new map
				pLayer->removeMap("map");
				int iNumTilesX = std::stoi(x->pGUI->getContainer("state4_edit")->getTextEdit("numtilesx")->getText());
				int iNumTilesY = std::stoi(x->pGUI->getContainer("state4_edit")->getTextEdit("numtilesy")->getText());
				if (iNumTilesX < 1)
					iNumTilesX = 1;
				if (iNumTilesY < 1)
					iNumTilesY = 1;
				pMap = pLayer->addMap("map", "map_atlas", iNumTilesX, iNumTilesY);

				_mpMapTilePrev = 0;

				// Add image types
				C2DMapImageType* pImageType = pMap->addImageType("tiles");
				pImageType->addFrameImage("data/x/textures/map_tiles/tiles.png");
				pImageType->setFramerate(0.0f);

				pImageType = pMap->addImageType("water_anim");
				for (int i = 1; i < 61; i++)
				{
					strTxt = StringUtils::blenderAnimFilename("data/x/textures/map_tiles/water_anim", ".png", i);
					pImageType->addFrameImage(strTxt);
				}

				// Set all tiles in the map to use the above image type
				pMap->setAllTilesImageType("water_anim");
			}
			pButton = pCont->getButton("NO");
			if (pButton->getClicked())
			{
				pCont->setVisible(false);
			}
		}


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