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

		// Create benchmark information GUI
		_initGUI();

		// Initialise stuff for each benchmark
		_initBenchmark1();
		_initBenchmark2();
		_initBenchmark3();
		_initShowResults();

		// Set initial state of the application
		_mState = AppState::welcomeScreen;
	}

	void CApplication::onStart(void)
	{
	}

	void CApplication::onStop(void)
	{
	}

	bool CApplication::onUpdate(void)
	{
		SCGUIManager* pGUI = SCGUIManager::getPointer();
		CGUIContainer* pCont = 0;
		CGUIButton* pButton = 0;

		timer.update();

		// Update stuff depending upon application state
		if (AppState::welcomeScreen == _mState)
		{
			// Get pointer to the begin benchmark button and wait for it to be clicked.
			pCont = pGUI->getContainer("WelcomeScreen");
			pButton = pCont->getButton("Begin");
			if (pButton->getClicked())
			{
				// Button clicked, begin the benchmarks...
				// Close the WelcomeScreen container
				pCont->setVisible(false);

				// Set visible, the "BenchmarkInfo" container and set "BenchmarkName" text object's text
				pCont = pGUI->getContainer("BenchmarkInfo");
				pCont->setVisible(true);
				pCont->getText("BenchmarkName")->setText("Benchmark 1: 2D sprite rendering.");

				// Set 2D world to be visible
				SC2DRenderer::getPointer()->getWorld("World1")->setVisible(true);

				// Set next state
				_mState = AppState::benchmark1;
			}
		}
		else if (AppState::benchmark1 == _mState)
		{
			// Update benchmark until it is done
			if (_updateBenchmark1())
			{
				// Benchmark has finished, proceed to next state
				// Set "BenchmarkName" text of "BenchmarkInfo" container to show next benchmark's details
				pGUI->getContainer("BenchmarkInfo")->getText("BenchmarkName")->setText("Benchmark 2: Something else.");

				// Set next state
				_mState = AppState::benchmark2;
			}
		}
		else if (AppState::benchmark2 == _mState)
		{
			// Update benchmark until it is done
			if (_updateBenchmark2())
			{
				// Benchmark has finished, proceed to next state
				// Set "BenchmarkName" text of "BenchmarkInfo" container to show next benchmark's details
				pGUI->getContainer("BenchmarkInfo")->getText("BenchmarkName")->setText("Benchmark 3: Something else.");

				// Set next state
				_mState = AppState::benchmark3;
			}
		}
		else if (AppState::benchmark3 == _mState)
		{
			// Update benchmark until it is done
			if (_updateBenchmark3())
			{
				// Benchmark has finished, proceed to next state
				// Set "BenchmarkName" text of "BenchmarkInfo" container to show next benchmark's details
				pGUI->getContainer("BenchmarkInfo")->getText("BenchmarkName")->setText("Benchmarks complete! Showing results.");

				// Set next state
				_mState = AppState::showResults;
			}
		}
		else if (AppState::showResults == _mState)
		{

		}

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

	void CApplication::_initGUI(void)
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

		// Create non-window container
		pCont = pGUI->addContainer("BenchmarkInfo");
		pCont->setBehaviour(false);
		pCont->setVisible(false);

		// Add some text stating which benchmark we're performing
		CGUIText* pText = pCont->addText("BenchmarkName", 0, 0, "Performing benchmark 1: 2D sprite rendering.");
		pText->setColour(false, 1.0f, 1.0f, 1.0f, 0.9f);
	}

	void CApplication::_initBenchmark1(void)
	{
		// Setup 2D renderer
		SC2DRenderer* p2D = SC2DRenderer::getPointer();
		C2DWorld* pWorld = p2D->addWorld("World1");
		pWorld->setVisible(false);
		C2DCamera* pCamera = pWorld->addCamera("Cam1");
		C2DLayer* pLayer = pWorld->addLayer("Layer1");
		SCResourceManager* pRM = SCResourceManager::getPointer();
		std::vector<std::string> vstrImages = X::getFilesInDir("data/DemoBenchmark/images/disc/");
		CResourceTexture2DAtlas* pAtlas = pRM->addTexture2DAtlas("Atlas1", vstrImages);

		// Create scaled up centered entity
		C2DEntity* pEntity = pLayer->addEntity("Entity:0", "Atlas1");
		SCWindow* pWindow = SCWindow::getPointer();
		pEntity->setPosition(CVector2r(pWindow->getWidth() / 2, pWindow->getHeight() / 2));
		pEntity->setScale(2.0f, 2.0f);
		pEntity->setImagesMultiple(vstrImages);
		
		// Set variables
		b1.fCentreSpriteRotation = 0.0f;
	}

	bool CApplication::_updateBenchmark1(void)
	{
		SC2DRenderer* p2D = SC2DRenderer::getPointer();
		C2DWorld* pWorld = p2D->getWorld("World1");
		C2DLayer* pLayer = pWorld->getLayer("Layer1");
		C2DEntity* pEntity = pLayer->getEntity("Entity:0");
		b1.fCentreSpriteRotation += timer.getSecondsPast() * 45.0f;
		while (b1.fCentreSpriteRotation >= 360)
			b1.fCentreSpriteRotation -= 360;
		pEntity->setFrameBasedOnAngle(b1.fCentreSpriteRotation);

		return false;
	}

	void CApplication::_initBenchmark2(void)
	{
	}

	bool CApplication::_updateBenchmark2(void)
	{
		return false;
	}

	void CApplication::_initBenchmark3(void)
	{
	}

	bool CApplication::_updateBenchmark3(void)
	{
		return false;
	}

	void CApplication::_initShowResults(void)
	{
	}

	bool CApplication::_updateShowResults(void)
	{
		return false;
	}
}