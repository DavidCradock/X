#pragma once
#include "PCH.h"
#include "singleton.h"
#include "inputJoystick.h"
#include "inputKeyboard.h"
#include "inputMouse.h"

namespace X
{
	// This is responsible for obtaining with input from various input devices such
	// as keyboards, mice, joysticks and steering wheels.
	class SCInputManager : public CSingleton<SCInputManager>
	{
	public:

		SCInputManager();

		// Initialise all input devices
		void init(HWND hApplicationWindow);

		// Updates all input states
		void update(bool fullscreen, unsigned int screenWidth, unsigned int screenHeight);

		// Shutdown all input devices
		void shutdown(void);

		CInputJoystick joy;	// Joystick object
		CInputKeyboard key;	// Keyboard object
		CInputMouse	mouse;	// Mouse object
	private:

		LPDIRECTINPUT8 directInput;	// Main DirectX input object
		HINSTANCE instance;		// Handle to application's instance
		HWND windowHandle;			// Handle to application's window

	};

}