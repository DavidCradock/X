#include "application.h"

namespace X
{
	void Application::initOnce(void)
	{
		// Set window title bar text
		Window::getPointer()->setText("X Demo2D. F1: Toggle fullscreen. F2: Toggle Vsync. F3: Toggle statistics window.");

		// Show frame rate statistics
		GUIManager::getPointer()->getContainer("X:Default:Statistics")->setVisible(true);

	}

	void Application::onStart(void)
	{
	}

	void Application::onStop(void)
	{

	}

	bool Application::onUpdate(void)
	{
		InputManager* pInputManager = InputManager::getPointer();

		// Timer delta
		timer.update();

		// Escape key to exit
		if (pInputManager->key.pressed(KC_ESCAPE))
			return false;
		// Toggle fullscreen
		if (pInputManager->key.once(KC_F1))
		{
			Window::getPointer()->toggleFullscreen();
		}
		// Toggle vertical sync
		if (pInputManager->key.once(KC_F2))
			Window::getPointer()->setVsync(!Window::getPointer()->getVSyncEnabled());
		// Toggle statistics window
		if (pInputManager->key.once(KC_F3))
		{
			GUIContainer* pStatsCont = GUIManager::getPointer()->getContainer("X:Default:Statistics");
			pStatsCont->setVisible(!pStatsCont->getVisible());
		}
		return true;
	}

}