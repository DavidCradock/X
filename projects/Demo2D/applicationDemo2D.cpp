#include "applicationDemo2D.h"
#include "resource.h"

namespace X
{
	void CApplication::initOnce(void)
	{
		// Set window title bar text and set icon
		SCWindow* pWindow = SCWindow::getPointer();
		pWindow->setText("X Demo2D. F1: Toggle fullscreen. F2: Toggle Vsync. F3: Toggle statistics window.");
		pWindow->setIcon(IDI_ICON1);

		// Show frame rate statistics
		SCGUIManager::getPointer()->getContainer("X:Default:Statistics")->setVisible(true);

		// Create Debug GUI
		SCGUIManager* pGUI = SCGUIManager::getPointer();
		CGUIContainer* pCont = pGUI->addContainer("Debug");
		pCont->setDimensions(640, 480);
		pCont->setPositionCentreWindow();
		pCont->setPosition(pCont->mfPositionX + 640, pCont->mfPositionY);

		for (int i = 0; i < 100; i += 20)
		{
			std::string strText = "Text: " + std::to_string(i);
			pCont->addText(strText, 0, (float)i, strText);
		}
//		pCont->setVisible(false);

		// First, create the C2DTexture2DAtlas resource which the entity will use for it's image data.
		SCResourceManager* pResourceManager = SCResourceManager::getPointer();	// Obtain pointer to the resource manager.
		// Fill in a std::vector<std::string> to hold all the images the entity will use, in this case, just one single image.
		std::vector<std::string> vstrImageFilenames;
		vstrImageFilenames = getFilesInDir("data/Demo2D/images/creature_top_down/");
		// Now create the texture atlas with the resource manager...
		pResourceManager->addTexture2DAtlas("MyTextureAtlas", vstrImageFilenames, true, 1);
		SC2DRenderer* p2DRenderer = SC2DRenderer::getPointer();					// Obtain pointer to this object
		C2DWorld* pWorld = p2DRenderer->addWorld("MyWorld");						// Add a new world to contain everything
		C2DLayer* pLayer = pWorld->addLayer("MyLayer");							// Add a new layer to the world
		C2DCamera* pCamera = pWorld->addCamera("MyCamera");						// Add a new camera to the world
		// Add the entity to the layer, of the world, specifying which texture atlas it will get it's image data from...
		C2DEntity* pEntity = pLayer->addEntity("MyEntity", "MyTextureAtlas");
		// Set which image in the texture atlas the entity will use...
		pEntity->setImagesMultiple(vstrImageFilenames);
		// Optionally, set the entity's position...
		// pEntity->setPosition(CVector2r(1.0f, 2.0f));
		
		// Now add lots of entitys
		for (int i = 0; i < 1000; i++)
		{
			std::string strEntityName = "Entity: " + std::to_string(i);
			pEntity = pLayer->addEntity(strEntityName, "MyTextureAtlas");
			pEntity->setImagesMultiple(vstrImageFilenames);
			CVector2r vPos(randf(0, (float)pWindow->getWidth()), randf(0, (float)pWindow->getHeight()));
			pEntity->setPosition(vPos);
			pEntity->setScale(0.5, 0.5);
			mfEntityDegrees[i] = 0;
		}

		// Now add controllable entity on a new layer above the layer with 1000 enititys
		pLayer = pWorld->addLayer("Layer2");
		pEntity = pLayer->addEntity("EntityControllable", "MyTextureAtlas");
		pEntity->setImagesMultiple(vstrImageFilenames);
		mvEntityDir.set(0.0f, 1.0f);
		mvEntityPos.set(500, 500);
	}

	void CApplication::onStart(void)
	{
	}

	void CApplication::onStop(void)
	{

	}

	bool CApplication::onUpdate(void)
	{
		SCInputManager* pInputManager = SCInputManager::getPointer();

		// Timer delta
		timer.update();
		
		// Move entity
		static float fInc = 0.0f;
		fInc += timer.getSecondsPast() * 100.0f;
		if (fInc > 100)
			fInc = 0;
		CVector2r vSpritePos;
		vSpritePos.x = 500 + fInc;
		vSpritePos.y = 500 + (sinf(fInc * 0.1f) * 10.0f);
		SC2DRenderer* p2D = SC2DRenderer::getPointer();
		p2D->getWorld(0)->getLayer(0)->getEntity(0)->setPosition(vSpritePos);
		SCGUIManager* pGUI = SCGUIManager::getPointer();
		CGUIContainer* pCont = pGUI->getContainer("Debug");
		pCont->getText("Text: 0")->mstrText = "SpritePosition: x=" + std::to_string(vSpritePos.x) + "y=" + std::to_string(vSpritePos.y);
		pCont->getText("Text: 20")->mstrText = "getNumberTextureRebindingsPerLoop() = " + std::to_string(p2D->getNumberTextureRebindingsPerLoop());

		// Increase animation frames for each sprite
		SC2DRenderer* p2DRenderer = SC2DRenderer::getPointer();
		C2DWorld* pWorld = p2DRenderer->getWorld("MyWorld");
		C2DLayer* pLayer = pWorld->getLayer("MyLayer");
		
		for (int i = 0; i < 1000; i++)
		{
			std::string strEntityName = "Entity: " + std::to_string(i);
			C2DEntity* pEntity = pLayer->getEntity(strEntityName);
			mfEntityDegrees[i] += timer.getSecondsPast() * 45.0f;
			while (mfEntityDegrees[i] >= 360)
				mfEntityDegrees[i] -= 360;
			pEntity->setFrameBasedOnAngle(mfEntityDegrees[i]);
		}

		// Now deal with controllable entity
		pLayer = pWorld->addLayer("Layer2");
		C2DEntity* pEntity = pLayer->getEntity("EntityControllable");
		pEntity->setPosition(mvEntityPos);
		pEntity->setFrameBasedOnDirection(mvEntityDir);

		// Escape key to exit
		if (pInputManager->key.pressed(KC_ESCAPE))
			return false;
		// Toggle fullscreen
		if (pInputManager->key.once(KC_F1))
		{
			SCWindow::getPointer()->toggleFullscreen();
		}
		// Toggle vertical sync
		if (pInputManager->key.once(KC_F2))
			SCWindow::getPointer()->setVsync(!SCWindow::getPointer()->getVSyncEnabled());
		// Toggle statistics window
		if (pInputManager->key.once(KC_F3))
		{
			CGUIContainer* pStatsCont = SCGUIManager::getPointer()->getContainer("X:Default:Statistics");
			pStatsCont->setVisible(!pStatsCont->getVisible());
		}
		return true;
	}

}