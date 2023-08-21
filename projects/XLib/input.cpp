#include "PCH.h"
#include "input.h"
#include "logging.h"

namespace X
{

	SCInputManager::SCInputManager()
	{
		SCLog* pLog = SCLog::getPointer();
		pLog->add("SCInputManager::SCInputManager() called.");

		_mDirectInput = NULL;
	}

	void SCInputManager::init(HWND hApplicationWindow)
	{
		_mInstance = GetModuleHandle(NULL);
		_mWindowHandle = hApplicationWindow;
		ThrowIfTrue(!_mInstance, "SCInputManager::init() failed. Unable to obtain application instance handle.");

		// Init main directX input object
		if (DI_OK != DirectInput8Create(_mInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&_mDirectInput, NULL))
			ThrowIfTrue(1, "SCInputManager::init() failed. Unable to create main DirectX input object.");

		// Init joystick
		if (!joy.init(_mDirectInput, _mWindowHandle))// Error only returned if joystick detected, but init failed.
			ThrowIfTrue(1, "SCInputManager::init() failed. Joystick detected, but unable to initialise it.");

		// Init keyboard
		if (!key.init(_mDirectInput, _mWindowHandle))
			ThrowIfTrue(1, "SCInputManager::init() failed. Unable to initialise keyboard.");

		// Init mouse
		if (!mouse.init(_mDirectInput, _mWindowHandle))
			ThrowIfTrue(1, "SCInputManager::init() failed. Unable to initialise mouse.");
	}

	void SCInputManager::shutdown(void)
	{
		joy.release();
		key.release();
		mouse.release();
		if (_mDirectInput)
		{
			_mDirectInput->Release();
			_mDirectInput = NULL;
		}
	}

	void SCInputManager::update(bool fullscreen, unsigned int screenWidth, unsigned int screenHeight)
	{
		joy.update();
		key.update();
		mouse.update(fullscreen, screenWidth, screenHeight);
	}
}