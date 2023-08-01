#include "PCH.h"
#include "application.h"
#include "resource.h"

namespace X
{
	void CApplication::initOnce(void)
	{
		// Use the resource loading screen
		SCResourceLoadingScreen* pLS = SCResourceLoadingScreen::getPointer();
		pLS->onInit(3);

		// Set window title bar text and set icon
		SCWindow* pWindow = SCWindow::getPointer();
		pWindow->setText("X: Demo2D. F1: Toggle fullscreen. F2: Toggle Vsync. F3: Toggle statistics window.");
		pWindow->setIcon(IDI_ICON1);
		// Set mouse cursor
		SCInputManager::getPointer()->mouse.setMouseCursorImage("data/X/cursors/new_default.ani");

		// Show frame rate statistics
		SCGUIManager::getPointer()->getContainer("X:Default:Statistics")->setVisible(true);

		// Create GUI
		SCGUIManager* pGUI = SCGUIManager::getPointer();
		CGUIContainer* pCont = pGUI->addContainer("Demo2D");
		pCont->setDimensions(500, 480);
		pCont->setPosition(0, 0);

		// Add some text objects
		for (int i = 0; i < 100; i += 20)
		{
			std::string strText = "Text: " + std::to_string(i);
			pCont->addText(strText, 0, (float)i, strText);
		}

		// Add text scroll showing controls
		std::string strTextScroll;
		strTextScroll += "Welcome to Demo2D.\nThe cursor keys move the camera.\nThe WSAD keys move and rotate entity.\nThere are 1000 scaled down entities in the background on the lowest layer, then the controllable entity is rendered on a layer above that one.";
		pCont->addTextScroll("TextScroll", 20, 120, 480, 200, strTextScroll);
//		pCont->setVisible(false);

		// First, create the C2DTexture2DAtlas resource which the entity will use for it's image data.
		// Obtain pointer to the resource manager.
		SCResourceManager* pResourceManager = SCResourceManager::getPointer();

		// Fill in a std::vector<std::string> to hold all the images the entity we will
		std::vector<std::string> vstrImageFilenames = StringUtils::getFilesInDir("data/Demo2D/images/creature_top_down/");

		// Now create the texture atlas with the resource manager...
		pResourceManager->addTexture2DAtlas("MyTextureAtlas", vstrImageFilenames, true, 1);
		
		// Obtain pointer to 2D renderer
		SC2DRenderer* p2DRenderer = SC2DRenderer::getPointer();

		// Add a new world to contain everything
		C2DWorld* pWorld = p2DRenderer->addWorld("MyWorld");

		// Add a new layer to the world
		C2DLayer* pLayer = pWorld->addLayer("MyLayer");

		// Add a new camera to the world
		C2DCamera* pCamera = pWorld->addCamera("MyCamera");

		// Now add lots of entities
		for (int i = 0; i < 1000; i++)
		{
			// Create unique name for the entity
			std::string strEntityName = "Entity: " + std::to_string(i);

			// Add entity to the layer
			C2DEntity* pEntity = pLayer->addEntity(strEntityName, "MyTextureAtlas");

			// Set which images the entity will be using
			pEntity->setImagesMultiple(vstrImageFilenames);

			// Set random position
			CVector2f vPos(randf(0, (float)pWindow->getWidth()), randf(0, (float)pWindow->getHeight()));
			pEntity->setPosition(vPos);

			// Set scale
			pEntity->setScale(0.5, 0.5);

			// Set random initial rotation
			mfEntityBackgroundDegrees[i] = randf(0, 360.0f);

			// Set entity rotation direction
			if (randf(0, 1) < 0.5f)
				mfEntityRotDir[i] = -1.0f;
			else
				mfEntityRotDir[i] = 1.0f;
		}

		// Now add controllable complex entity on a new layer above the layer with 1000 entities
		pLayer = pWorld->addLayer("Layer2");

		// Add complex entity to the layer
		C2DEntityComplex* pEntityComplex = pLayer->addEntityComplex("EntityControllable");

		// Add a layer to the complex entity
		C2DLayerComplex* pLayerComplex = pEntityComplex->addLayer("ParentLayer");

		// Setup the texture atlas the "PARENT" entity of the complex entity will use
		vstrImageFilenames = StringUtils::getFilesInDir("data/Demo2D/images/spaceship_hull/");
		pResourceManager->addTexture2DAtlas("SpaceshipHull", vstrImageFilenames, true, 1);
		C2DEntity* pEntity = pLayerComplex->addEntity("PARENT", "SpaceshipHull");
		pEntity->setImagesMultiple(vstrImageFilenames);
		mvEntityDir.set(0.0f, 1.0f);
		mvEntityPos.set((pWindow->getDimensions().x * 0.5f), (pWindow->getDimensions().y * 0.5f));
		mfEntityVel = 0.0f;

		// Now set up child entities of the parent entity (The gun turrets)
		// Setup the texture atlas the gun turret entities of the complex entity will use
		vstrImageFilenames = StringUtils::getFilesInDir("data/Demo2D/images/spaceship_turret/");
		pResourceManager->addTexture2DAtlas("SpaceshipTurrets", vstrImageFilenames, true, 1);

		// Create a new layer in the complex entity for the gun turrets
		pLayerComplex = pEntityComplex->addLayer("TurretLayer");
		// Create a new child entity in the new layer
		pEntity = pLayerComplex->addEntity("Gun1", "SpaceshipTurrets");
		// Set the atlas the gun entity will use
		pEntity->setImagesMultiple(vstrImageFilenames);
		// Scale it down a bit
		pEntity->setScale(0.5f, 0.5f);
		// Set position offset from the "PARENT" entity
		pEntity->setPosition(CVector2f(0, 50));
		
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
		
		SC2DRenderer* p2D = SC2DRenderer::getPointer();
		SCGUIManager* pGUI = SCGUIManager::getPointer();
		CGUIContainer* pCont = pGUI->getContainer("Demo2D");

		// Rotate background entities
		C2DWorld* pWorld = p2D->getWorld("MyWorld");
		C2DLayer* pLayer = pWorld->getLayer("MyLayer");
		for (int i = 0; i < 1000; i++)
		{
			std::string strEntityName = "Entity: " + std::to_string(i);
			C2DEntity* pEntity = pLayer->getEntity(strEntityName);
			mfEntityBackgroundDegrees[i] += timer.getSecondsPast() * 45.0f * mfEntityRotDir[i];
			while (mfEntityBackgroundDegrees[i] >= 360)
				mfEntityBackgroundDegrees[i] -= 360;
			while (mfEntityBackgroundDegrees[i] < 0.0f)
				mfEntityBackgroundDegrees[i] += 360;
			pEntity->setFrameBasedOnAngle(mfEntityBackgroundDegrees[i]);
		}

		// Now deal with controllable entity
		pLayer = pWorld->getLayer("Layer2");
		C2DEntityComplex* pEntityComplex = pLayer->getEntityComplex("EntityControllable");
		pEntityComplex->setFrameBasedOnDirection(mvEntityDir);
		// Set rotation of entity
		pEntityComplex->setRotation(mvEntityDir);

		if (pInput->key.pressed(KC_D))
			mvEntityDir.rotate(timer.getSecondsPast() * 180.0f);
		if (pInput->key.pressed(KC_A))
			mvEntityDir.rotate(timer.getSecondsPast() * -180.0f);
		if (pInput->key.pressed(KC_W))
		{
			mfEntityVel += timer.getSecondsPast() * 1.5f;
			if (mfEntityVel > 2.0f)
				mfEntityVel = 2.0f;
		}
		if (pInput->key.pressed(KC_S))
		{
			mfEntityVel -= timer.getSecondsPast() * 1.5f;
		}
		// Reduce velocity
		mfEntityVel -= timer.getSecondsPast() * 0.85f;
		if (mfEntityVel < 0.0f)
			mfEntityVel = 0.0f;
		// Compute position offset of entity based upon velocity.
		CVector2f vOffset = mvEntityDir * timer.getSecondsPast() * 250.0f * mfEntityVel;
		vOffset.y *= -1.0f;							// Invert Y for correct direction
		mvEntityPos += vOffset;						// Adjust position
		pEntityComplex->setPosition(mvEntityPos);	// Set position

		// Now move camera
		C2DCamera* pCamera = pWorld->getCamera(0);
		CVector2f vCamPos = pCamera->getPosition();
		if (pInput->key.pressed(KC_UP))
			vCamPos.y -= timer.getSecondsPast() * 150.0f;
		if (pInput->key.pressed(KC_DOWN))
			vCamPos.y += timer.getSecondsPast() * 150.0f;
		if (pInput->key.pressed(KC_LEFT))
			vCamPos.x -= timer.getSecondsPast() * 150.0f;
		if (pInput->key.pressed(KC_RIGHT))
			vCamPos.x += timer.getSecondsPast() * 150.0f;
		pCamera->setPosition(vCamPos);

		pCont->getText("Text: 0")->setText("SpritePosition: x=" + std::to_string(mvEntityPos.x) + "y=" + std::to_string(mvEntityPos.y));
		pCont->getText("Text: 20")->setText("SpriteDirection: x=" + std::to_string(mvEntityDir.x) + "y=" + std::to_string(mvEntityDir.y));
		pCont->getText("Text: 40")->setText("getNumberTextureRebindingsPerLoop() = " + std::to_string(p2D->getNumberTextureRebindingsPerLoop()));

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