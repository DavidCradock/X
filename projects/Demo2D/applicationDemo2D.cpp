#include "applicationDemo2D.h"
#include "resource.h"

namespace X
{
	void CApplication::initOnce(void)
	{
		// Set window title bar text and set icon
		CWindow* pWindow = CWindow::getPointer();
		pWindow->setText("X Demo2D. F1: Toggle fullscreen. F2: Toggle Vsync. F3: Toggle statistics window.");
		pWindow->setIcon(IDI_ICON1);

		// Show frame rate statistics
		SCGUIManager::getPointer()->getContainer("X:Default:Statistics")->setVisible(true);

		SC2DRenderer* p2DRenderer = SC2DRenderer::getPointer();
		//p2DRenderer->addWorld("World");
		C2DWorld world;

		CVector2r v1(0, 0);
		CVector2r v2(1.0, 1.0);
		real rAngle = v1.getDistance(v2);

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
			CWindow::getPointer()->toggleFullscreen();
		}
		// Toggle vertical sync
		if (pInputManager->key.once(KC_F2))
			CWindow::getPointer()->setVsync(!CWindow::getPointer()->getVSyncEnabled());
		// Toggle statistics window
		if (pInputManager->key.once(KC_F3))
		{
			CGUIContainer* pStatsCont = SCGUIManager::getPointer()->getContainer("X:Default:Statistics");
			pStatsCont->setVisible(!pStatsCont->getVisible());
		}
		return true;
	}

}