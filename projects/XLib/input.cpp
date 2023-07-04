#include "PCH.h"
#include "input.h"
#include "log.h"

namespace X
{

	SCInputManager::SCInputManager()
	{
		directInput = NULL;
	}

	void SCInputManager::init(HWND hApplicationWindow)
	{
		instance = GetModuleHandle(NULL);
		windowHandle = hApplicationWindow;
		ThrowIfTrue(!instance, "SCInputManager::init() failed. Unable to obtain application instance handle.");

		// Init main directX input object
		if (DI_OK != DirectInput8Create(instance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, NULL))
			ThrowIfTrue(1, "SCInputManager::init() failed. Unable to create main DirectX input object.");

		// Init joystick
		if (!joy.init(directInput, windowHandle))// Error only returned if joystick detected, but init failed.
			ThrowIfTrue(1, "SCInputManager::init() failed. Joystick detected, but unable to initialise it.");

		// Init keyboard
		if (!key.init(directInput, windowHandle))
			ThrowIfTrue(1, "SCInputManager::init() failed. Unable to initialise keyboard.");

		// Init mouse
		if (!mouse.init(directInput, windowHandle))
			ThrowIfTrue(1, "SCInputManager::init() failed. Unable to initialise mouse.");
	}

	void SCInputManager::shutdown(void)
	{
		joy.release();
		key.release();
		mouse.release();
		if (directInput)
		{
			directInput->Release();
			directInput = NULL;
		}
	}

	void SCInputManager::update(bool fullscreen, unsigned int screenWidth, unsigned int screenHeight)
	{
		joy.update();
		key.update();
		mouse.update(fullscreen, screenWidth, screenHeight);
	}
}