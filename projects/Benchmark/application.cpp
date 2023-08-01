#include "application.h"
#include "resource.h"

namespace X
{
	class CResults
	{
	public:
		void save(void)
		{
			// Open file in text mode, deleting contents if existed before
			std::ofstream file;
			file.open("benchmark_results.txt", std::ofstream::out | std::ofstream::trunc);
			ThrowIfFalse(file.is_open(), "CResults::save() failed to open benchmark_results.txt for writing.");

			// Filename
			file << "benchmark_results.txt\n";

			std::string str;
			str = "Benchmark01: " + std::to_string(iBenchmark1_NumSprites) + "\n";	file << str;

			// Make sure there were no errors
			ThrowIfTrue(file.fail(), "CResults::save() failed whilst saving file.");
			file.close();
		}

		void load(void)
		{
			std::ifstream file;
			file.open("benchmark_results.txt", std::ofstream::in);
			// If unable to load from file, just set all results to defaults
			if (!file.is_open())
			{
				iBenchmark1_NumSprites = 0;
				return;
			}

			// Filename
			std::string strWord;
			std::string strLine;
			char space;
			getline(file, strLine);

			// Benchmark 01
			file >> strWord;	// "Benchmark01:"
			space = file.get();	// Space
			file >> iBenchmark1_NumSprites;

			ThrowIfTrue(file.fail(), "CResults::load() failed whilst loading file.");
			file.close();
		}
		unsigned int iBenchmark1_NumSprites;
	};
	CResults gResults;

	void CApplication::initOnce(void)
	{
		// Use the resource loading screen
		SCResourceLoadingScreen* pLS = SCResourceLoadingScreen::getPointer();
		pLS->onInit(1);

		// Set window title bar text and set icon
		SCWindow* pWindow = SCWindow::getPointer();
		pWindow->setText("X: Benchmark. F1: Toggle fullscreen. F2: Toggle Vsync. F3: Toggle statistics window.");
		pWindow->setIcon(IDI_ICON1);
		// Set mouse cursor
		SCInputManager::getPointer()->mouse.setMouseCursorImage("data/X/cursors/new_default.ani");

		// Turn off VSync, we're benchmarking!
		pWindow->setVsync(false);

		// Initialise finite state machine and the states for each benchmark
		CState01_Welcome* pState01 = new CState01_Welcome;
		CState02_2DSprites* pState02 = new CState02_2DSprites;
		CState99_Results* pState99 = new CState99_Results;
		_mFSM.addState("state01_welcome", pState01);
		_mFSM.addState("state02_2DSprites", pState02);
		_mFSM.addState("state99_Results", pState99);
		_mFSM.switchToState("state01_welcome");

		// Temp debugging of CStringHelper class
		std::string strMultipleLines = "Line1\nLine2\nLine3\n";
		std::vector<std::string> vstrLines1 = StringUtils::splitString(strMultipleLines, "\n");

		strMultipleLines = "Line1\nLine2\nLine3";
		std::vector<std::string> vstrLines2 = StringUtils::splitString(strMultipleLines, "\n");

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
		// Update the FSM
		_mFSM.update();

		// Escape key to exit
		SCInputManager* pInput = SCInputManager::getPointer();
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

	void CState01_Welcome::onEnter(void)
	{
		SCGUIManager* pGUI = SCGUIManager::getPointer();

		// Add container
		CGUIContainer* pCont = pGUI->addContainer("WelcomeScreen");
		pCont->setDimensions(320, 240);
		pCont->setPositionCentreWindow();

		// Add animated image
		std::vector<std::string> vstrImages = StringUtils::getFilesInDir("data/DemoBenchmark/Images/disc/");
		CGUIImageAnimated* pImageAnim = pCont->addImageAnimated("Anim", -16, -16, vstrImages, 16, 16);

		// Add scroll text
		std::string strTxt;
		strTxt += "Welcome to the X Benchmark application.\n";
		strTxt += "Here we perform various tests to stress the CPU, GPU, memory and storage device which the application is run from.\n";
		strTxt += "After all the tests have been completed, we calculate an arbitrary score and show the results from each of the tests.\n";
		strTxt += "When you're ready to begin, feel free to click the button below, or press space bar or the return key.";
		CGUITextScroll* pTextScroll = pCont->addTextScroll("TextScroll", 0, 0, 320, 200, strTxt);

		// Add begin button
		CGUIButton* pButton = pCont->addButton("Begin", 160 - 150, 210, 300, 40, "Begin Benchmark");
		pButton->mpTooltip->setAsText("Click here to begin the benchmarks! :D");

		fTimeDelayUntilAutostart = 5.0f;
	}

	void CState01_Welcome::onActive(CFiniteStateMachine* pFSM)
	{
		SCInputManager* pInput = SCInputManager::getPointer();

		// Autostart
		timer.update();
		fTimeDelayUntilAutostart -= timer.getSecondsPast();
		if (pInput->mouse.getMouseDeltaGUI().x != 0.0f || pInput->mouse.getMouseDeltaGUI().y != 0.0f)
			fTimeDelayUntilAutostart = 5.0f;

		// Get pointer to the begin benchmark button and wait for it to be clicked.
		SCGUIManager* pGUI = SCGUIManager::getPointer();
		CGUIContainer* pCont = pGUI->getContainer("WelcomeScreen");
		CGUIButton* pButton = pCont->getButton("Begin");
		std::string strTxt = "Begin (Autostart in: ";
		strTxt += std::format("{:.1f}", fTimeDelayUntilAutostart);
		strTxt += ")";
		pButton->mstrText = strTxt;

		if (pButton->getClicked() || pInput->key.pressed(KC_RETURN) || pInput->key.pressed(KC_SPACE) || fTimeDelayUntilAutostart <= 0.0f)
		{
			pFSM->switchToState("state02_2DSprites");
		}
	}

	void CState01_Welcome::onExit(void)
	{
		SCGUIManager* pGUI = SCGUIManager::getPointer();
		pGUI->removeContainer("WelcomeScreen");
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
		pText->setColour(false, CColour(1.0f, 1.0f, 1.0f, 1.0f));
		// Add text to GUI to show number of rendered entities
		pText = pCont->addText("b1_num_entities", 0, 20, "");
		pText->setColour(false, CColour(1.0f, 1.0f, 1.0f, 1.0f));

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
		vstrImages = StringUtils::getFilesInDir("data/DemoBenchmark/images/disc/");
		CResourceTexture2DAtlas* pAtlas = pRM->addTexture2DAtlas("Atlas1", vstrImages);

		// Create scaled up centered entity
		C2DEntity* pEntity = pLayer->addEntity("Entity:0", "Atlas1");
		SCWindow* pWindow = SCWindow::getPointer();
		pEntity->setPosition(CVector2f(pWindow->getWidth() / 2.0f, pWindow->getHeight() / 2.0f));
		pEntity->setScale(1.0f, 1.0f);
		pEntity->setImagesMultiple(vstrImages);

		// Create layer beneath the other one for lots of other entityies
		pLayer = pWorld->addLayer("Layer2");
		pWorld->moveLayerToFront("Layer1");

		// Set variables
		fCentreSpriteRotation = 0.0f;
		fSecondsNoEntitiesAdded = 0.0f;

		timer.update();
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

		// Increment the amount of time no entities have been added.
		fSecondsNoEntitiesAdded += timer.getSecondsPast();

		// Add entity if framerate is above the refresh rate
		SCWindow* pWindow = SCWindow::getPointer();
		pLayer = pWorld->getLayer("Layer2");
		if (timer.getFPS() >= pWindow->getRefreshRate())
		{
			for (unsigned int i = 0; i < 30; i++)
			{
				// Create unique name for the new entity
				std::string strName = "Entity:" + std::to_string(pLayer->getNumEntities());
				pEntity = pLayer->addEntity(strName, "Atlas1");
				pEntity->setImagesMultiple(vstrImages);
				pEntity->setPosition(CVector2f(randf(0.0f, (float)pWindow->getWidth()), randf(0.0f, (float)pWindow->getHeight())));
				float fScale = randf(0.1f, 0.3f);
				pEntity->setScale(fScale, fScale);
				pEntity->setFrameBasedOnAngle(randf(0, 360));

				// If entities have been added, reset fSecondsNoEntitiesAdded
				fSecondsNoEntitiesAdded = 0.0f;
			}
			// Update GUI text
			SCGUIManager* pGUI = SCGUIManager::getPointer();
			CGUIContainer* pCont = pGUI->getContainer("BenchmarkInfo");
			CGUIText* pText = pCont->getText("b1_num_entities");
			std::string strText = "Number of entities: " + std::to_string(pLayer->getNumEntities());
			pText->setText(strText);
		}

		// Proceed to next state if maximum number of entities have been reached
		if (fSecondsNoEntitiesAdded > 5.0f)
		{
			// Save results
			gResults.iBenchmark1_NumSprites = pLayer->getNumEntities();
			pFSM->switchToState("state99_Results");
		}
	}

	void CState02_2DSprites::onExit(void)
	{
		// Cleanup
		SC2DRenderer* p2D = SC2DRenderer::getPointer();
		p2D->removeAllWorlds();
		SCResourceManager* pRM = SCResourceManager::getPointer();
		pRM->removeTexture2DAtlas("Atlas1");

		SCGUIManager* pGUI = SCGUIManager::getPointer();
		CGUIContainer* pCont = pGUI->getContainer("BenchmarkInfo");
		pCont->removeText("b1_num_entities");

		// Hide container
		pCont->setVisible(false);
	}

	void CState99_Results::onEnter(void)
	{
		// Load old results from file (Or if no file, set to defaults)
		CResults resultsOld;
		resultsOld.load();

		// Save current results to file
		gResults.save();

		// Re-enable Vsync
		SCWindow* pWindow = SCWindow::getPointer();
		pWindow->setVsync(true);

		SCGUIManager* pGUI = SCGUIManager::getPointer();

		// Add container
		CGUIContainer* pCont = pGUI->addContainer("Results");
		pCont->setDimensions(800, 600);
		pCont->setPositionCentreWindow();

		// Add animated image
		std::vector<std::string> vstrImages = StringUtils::getFilesInDir("data/DemoBenchmark/Images/disc/");
		CGUIImageAnimated* pImageAnim = pCont->addImageAnimated("Anim", -16, -16, vstrImages, 16, 16);

		// Add text
		CGUIText* pText = pCont->addText("Text1", 0, 0, "Benchmark results: ");
		pText = pCont->addText("Text2", 0, 20, "Benchmark 01: Total number of entities rendered at 60fps: " + std::to_string(gResults.iBenchmark1_NumSprites) + " (Prev: " + std::to_string(resultsOld.iBenchmark1_NumSprites) + ")");

	}

	void CState99_Results::onActive(CFiniteStateMachine* pFSM)
	{
		// Get pointer to the begin benchmark button and wait for it to be clicked.
		SCGUIManager* pGUI = SCGUIManager::getPointer();
	}

	void CState99_Results::onExit(void)
	{
	}
}