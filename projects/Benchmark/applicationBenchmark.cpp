#include "applicationBenchmark.h"

namespace X
{
	void CApplication::initOnce(void)
	{
	}

	void CApplication::onStart(void)
	{
	}

	void CApplication::onStop(void)
	{

	}

	bool CApplication::onUpdate(void)
	{
		// Escape key to exit
		InputManager* pInputManager = InputManager::getPointer();
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
		return true;
	}
}