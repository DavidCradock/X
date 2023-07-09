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

		SC2DRenderer* p2DRenderer = SC2DRenderer::getPointer();
		//p2DRenderer->addWorld("World");
		C2DWorld world;

		// Create Debug GUI
		SCGUIManager* pGUI = SCGUIManager::getPointer();
		CGUIContainer* pCont = pGUI->addContainer("Debug");
		pCont->setDimensions(1600, 1200);
		pCont->setPositionCentreWindow();
		for (int i = 0; i < 100; i += 20)
		{
			std::string strText = "Text: " + std::to_string(i);
			pCont->addText(strText, 0, (float)i, strText);
		}
		pCont->setVisible(true);

		// Add image animation to GUI
		std::vector<std::string> vstrImageFilenames = getFilesInDir("data/DevApp/textures/anim_test_320x140x180/");
		pCont->addImageAnimated("AnimTest", 600, 0, vstrImageFilenames, -1.0f, -1.0f);

		CImageAtlasPacker imageAtlasPacker;
		vstrImageFilenames = getFilesInDir("data/Demo2D/images/CImageAtlasPackerTest/");
		imageAtlasPacker.createAtlasImages(vstrImageFilenames, pWindow->getMaxTextureSize(), pWindow->getMaxTextureSize(), true, 1);
		CImage *pImage = imageAtlasPacker.getAtlasImage(0);
		SCResourceManager::getPointer()->addTexture2DFromImage("AtlasImageResource", *pImage);
		pCont->addImageFromImage("AtlasImage", 0, 100, "AtlasImageResource");
		
		
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