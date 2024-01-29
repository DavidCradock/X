#include "PCH.h"
#include "UIDefConMapEditor.h"
#include "UIManager.h"
#include "singletons.h"
#include "2DRenderer.h"
#include "stringUtils.h"
#include "input.h"

namespace X
{
	void onClickedMainWindowClose(void)
	{
		C2DWorld* pWorld = x->p2dRenderer->getWorld("world1");
		pWorld->setVisible(false);
	}

	CUIDefConMapEditor::CUIDefConMapEditor()
	{

	}

	void CUIDefConMapEditor::initialise(void)
	{
		CUIDefaultContainers* pDefCons = x->pUI->getDefaultContainers();
		CUIWindow* pWindow = x->pUI->windowAdd(pDefCons->names.mapEditor, true);
		pWindow->setVisible(false);
		pWindow->setDimensions(640, 480);
		pWindow->setPosition(0, 0);
		pWindow->setPositionCentre();
		pWindow->setFunctionOnCloseButtonClicked(onClickedMainWindowClose);

		x->pResource->addTexture2DAtlas("map_atlas", "data/x/textures/map_tiles/", true, 10, false, true);

		_mpMapTilePrev = 0;

		CUIText* pText = pWindow->textAdd("campos", 0, 250, 200, 50, "campos");
		pText = pWindow->textAdd("numtiles", 0, 290, 200, 50, "numtiles");

		// Setup 2D renderer
		C2DWorld* pWorld = x->p2dRenderer->addWorld("world1");
		C2DCamera* pCamera = pWorld->addCamera("camera");
		C2DLayer* pLayer = pWorld->addLayer("layer");
		C2DMap* pMap = pLayer->addMap("map", "map_atlas", 100, 100);
		C2DMapImageType* pImageType = pMap->addImageType("tiles");
		pImageType->addFrameImage("data/x/textures/map_tiles/tiles.png");
		pImageType->setFramerate(0.0f);

		pWorld->setVisible(false);

		pImageType = pMap->addImageType("water_anim");
		for (int i = 1; i < 61; i++)
		{
			std::string strTxt = StringUtils::blenderAnimFilename("data/x/textures/map_tiles/water_anim", ".png", i);
			pImageType->addFrameImage(strTxt);
		}

		// Set all tiles in the map to use the above image type
		pMap->setAllTilesImageType("water_anim");

		

		// Load in textures for the UI to use as images
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
//			if (vButtonPos.y > pCont->getDimensions().y)
//			{
//				vButtonPos.y = 260;
//				vButtonPos.x += 40;
//			}
		}

	}

	void CUIDefConMapEditor::update(float fTimeDeltaSecs)
	{
		CUIDefaultContainers* pDefCons = x->pUI->getDefaultContainers();

		// Is the container visible? If not, simply return
		CUIWindow* pWindow = x->pUI->windowGet(pDefCons->names.mapEditor);
		if (!pWindow->getVisible())
			return;

		C2DWorld* pWorld = x->p2dRenderer->getWorld("world1");
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
	}
}