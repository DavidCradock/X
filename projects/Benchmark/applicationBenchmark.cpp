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
		SCInputManager* pInputManager = SCInputManager::getPointer();
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
		return true;
	}
}