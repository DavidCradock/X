#include "PCH.h"
#include "applicationDevelopment.h"
#include "audioManager.h"
#include "log.h"
#include "utilities.h"
#include "input.h"
#include "geometry.h"
#include "textures.h"
#include "window.h"

namespace X
{
	void ApplicationDevelopment::initOnce(void)
	{
		
		getNumberOfCPUs();
	}

	void ApplicationDevelopment::onStart(void)
	{
	}

	void ApplicationDevelopment::onStop(void)
	{

	}

	bool ApplicationDevelopment::onUpdate(void)
	{
		// Escape key to exit
		if (InputManager::getPointer()->key.pressed(KC_ESCAPE))
			return false;
		if (InputManager::getPointer()->key.once(KC_F1))
			Window::getPointer()->toggleFullscreen();
		return true;
	}
}