#include "applicationBenchmark.h"
#include "resource.h"

namespace X
{
	void CApplication::initOnce(void)
	{
		// Set window title bar text and set icon
		SCWindow* pWindow = SCWindow::getPointer();
		pWindow->setText("X: Benchmark. F1: Toggle fullscreen. F2: Toggle Vsync. F3: Toggle statistics window.");
		pWindow->setIcon(IDI_ICON1);

		// Turn off VSync, we're benchmarking!
		pWindow->setVsync(false);

		// Initialise finite state machine and the states for each benchmark
		CState01_Welcome* pState01 = new CState01_Welcome;
		CState02_2DSprites* pState02 = new CState02_2DSprites;
		_mFSM.addState("state01_welcome", pState01);
		_mFSM.addState("state02_2DSprites", pState02);
		_mFSM.switchToState("state01_welcome");
	}

	void CApplication::onStart(void)
	{
	}

	void CApplication::onStop(void)
	{
	}

	bool CApplication::onUpdate(void)
	{
		// Update the FSM
		_mFSM.update();

		// Escape key to exit
		SCInputManager* pInputManager = SCInputManager::getPointer();
		if (pInputManager->key.pressed(KC_ESCAPE))
			return false;

		// Toggle fullscreen
		if (pInputManager->key.once(KC_F1))
			SCWindow::getPointer()->toggleFullscreen();

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

	void CState01_Welcome::onEnter(void)
	{
		SCGUIManager* pGUI = SCGUIManager::getPointer();

		// Add container
		CGUIContainer* pCont = pGUI->addContainer("WelcomeScreen");
		pCont->setDimensions(320, 240);
		pCont->setPositionCentreWindow();

		// Add animated image
		std::vector<std::string> vstrImages = getFilesInDir("data/DemoBenchmark/Images/disc/");
		CGUIImageAnimated* pImageAnim = pCont->addImageAnimated("Anim", -16, -16, vstrImages, 16, 16);

		// Add scroll text
		std::string strTxt;
		strTxt += "Welcome to the X Benchmark application. Here we perform various tests to stress the CPU, GPU, memory and storage device which the application is run from. ";
		strTxt += "After all the tests have been complete, we calculate an arbitrary score and show the results from each of the tests. ";
		strTxt += "When you're ready to begin, feel free to click the button below! :) ";
		CGUITextScroll* pTextScroll = pCont->addTextScroll("TextScroll", 0, 0, 320, 200, strTxt);

		// Add begin button
		CGUIButton* pButton = pCont->addButton("Begin", 160 - 100, 210, 200, 40, "Begin Benchmark");
		CGUITooltip* pTooltip = (CGUITooltip*)pButton->mpTooltip;
		pTooltip->setAsText("Click here to begin the benchmarks! :D");
	}

	void CState01_Welcome::onActive(CFiniteStateMachine* pFSM)
	{
		// Get pointer to the begin benchmark button and wait for it to be clicked.
		SCGUIManager* pGUI = SCGUIManager::getPointer();
		CGUIContainer* pCont = pGUI->getContainer("WelcomeScreen");
		CGUIButton* pButton = pCont->getButton("Begin");
		if (pButton->getClicked())
		{
			pFSM->switchToState("state02_2DSprites");
		}
	}

	void CState01_Welcome::onExit(void)
	{
		SCGUIManager* pGUI = SCGUIManager::getPointer();
		CGUIContainer* pCont = pGUI->getContainer("WelcomeScreen");

		// Close the WelcomeScreen container
		pCont->setVisible(false);
	}

	void CState02_2DSprites::onEnter(void)
	{
		SCGUIManager* pGUI = SCGUIManager::getPointer();

		// Create non-window container
		CGUIContainer* pCont = pGUI->addContainer("BenchmarkInfo");
		pCont->setBehaviour(false);
		pCont->setVisible(true);

		// Add some text stating which benchmark we're performing
		CGUIText* pText = pCont->addText("BenchmarkName", 0, 0, "Performing benchmark 1: 2D sprite rendering.");
		pText->setColour(false, 1.0f, 1.0f, 1.0f, 1.0f);
		// Add text to GUI to show number of rendered entities
		pText = pCont->addText("b1_num_entities", 0, 20, "");

		// Setup 2D renderer
		SC2DRenderer* p2D = SC2DRenderer::getPointer();
		// Add the world
		C2DWorld* pWorld = p2D->addWorld("World1");
		pWorld->setVisible(true);
		// Add the camera
		C2DCamera* pCamera = pWorld->addCamera("Cam1");
		// Add layer 1 for the centered rotation 2D entity
		C2DLayer* pLayer = pWorld->addLayer("Layer1");
		// Create texture atlas holding each of the 360 images
		SCResourceManager* pRM = SCResourceManager::getPointer();
		vstrImages = X::getFilesInDir("data/DemoBenchmark/images/disc/");
		CResourceTexture2DAtlas* pAtlas = pRM->addTexture2DAtlas("Atlas1", vstrImages);

		// Create scaled up centered entity
		C2DEntity* pEntity = pLayer->addEntity("Entity:0", "Atlas1");
		SCWindow* pWindow = SCWindow::getPointer();
		pEntity->setPosition(CVector2r(pWindow->getWidth() / 2, pWindow->getHeight() / 2));
		pEntity->setScale(1.0f, 1.0f);
		pEntity->setImagesMultiple(vstrImages);

		// Create layer beneath the other one for lots of other entityies
		pLayer = pWorld->addLayer("Layer2");
		pWorld->moveLayerToFront("Layer1");

		// Set variables
		fCentreSpriteRotation = 0.0f;
	}

	void CState02_2DSprites::onActive(CFiniteStateMachine* pFSM)
	{
		timer.update();
		SC2DRenderer* p2D = SC2DRenderer::getPointer();
		C2DWorld* pWorld = p2D->getWorld("World1");
		C2DLayer* pLayer = pWorld->getLayer("Layer1");

		// Rotate the images of the centered entity
		C2DEntity* pEntity = pLayer->getEntity("Entity:0");
		fCentreSpriteRotation += timer.getSecondsPast() * 45.0f;
		while (fCentreSpriteRotation >= 360)
			fCentreSpriteRotation -= 360;
		pEntity->setFrameBasedOnAngle(fCentreSpriteRotation);

		// Add entity if framerate is above the refresh rate
		SCWindow* pWindow = SCWindow::getPointer();
		if (timer.getFPSAveraged() >= pWindow->getRefreshRate())
		{
			// Create unique name for the new entity
			std::string strName = "Entity:" + std::to_string(pLayer->getNumEntities());
			pEntity = pLayer->addEntity(strName, "Atlas1");
			pEntity->setImagesMultiple(vstrImages);
			pEntity->setPosition(CVector2r(randf(0.0f, (float)pWindow->getWidth()), randf(0.0f, (float)pWindow->getHeight())));
			pEntity->setScale(0.5f, 0.5f);

			// Update GUI text
			SCGUIManager* pGUI = SCGUIManager::getPointer();
			CGUIContainer* pCont = pGUI->getContainer("BenchmarkInfo");
			CGUIText* pText = pCont->getText("b1_num_entities");
			std::string strText = "Number of entities: " + std::to_string(pLayer->getNumEntities());
			pText->setText(strText);
		}

		// Rotate each entity
		for (unsigned int ui = 0; ui < pLayer->getNumEntities(); ui++)
		{

		}
	}

	void CState02_2DSprites::onExit(void)
	{

	}

	
}