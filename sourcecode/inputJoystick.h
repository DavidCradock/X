#pragma once
#include "PCH.h"

namespace X
{

	// Joystick Input class, use InputManager to access it's "joy" member.
	// There is an object of this class called "joy" which is initialised and updated in the InputManager class.
	// Use that object to access information about the device. 
	class InputJoystick
	{
	public:
		InputJoystick(void);
		~InputJoystick(void);

		// Init joystick device
		// Only returns an error if the given main device is invalid or
		// joystick is detected, but init failed as not all systems have joysticks attached. 
		bool init(LPDIRECTINPUT8 pMainDirectXinputDevice, HWND hApplicationWindow);

		// Updates joystick state
		void update(void);

		// Free everything
		void release(void);

		// Returns true if a joystick has been detected.
		inline bool getJoystickDetected(void) { return joystickDetected; }

		// Returns true if force feedback is supported.
		inline bool getForcefeedbackDetected(void) { return forcefeedbackDetected; }

		// Returns position along X axis +-1000
		inline long axisX(void) { return _mjs.lX; }

		// Returns position along X axis +-1000
		inline long axisY(void) { return _mjs.lY; }

		// Returns rotation of rudder +-1000
		inline long axisZ(void) { return _mjs.lRz; }

		// Returns position along slider 1 +-1000
		inline long axisS1(void) { return _mjs.rglSlider[0]; }

		// Returns position along slider 2 +-1000
		inline long axisS2(void) { return _mjs.rglSlider[1]; }

		// Returns 1st POW hat position. -1 for central position, else degrees from "north" position
		inline DWORD axisPOW1(void) { return _mjs.rgdwPOV[0]; }

		// Returns 2nd POW hat position. -1 for central position, else degrees from "north" position
		inline DWORD axisPOW2(void) { return _mjs.rgdwPOV[1]; }

		// Returns non zero number if the specified button is pressed. (upto 128 (0 to 127) buttons)
		inline int buttonDown(unsigned int iButNum = 0) { if (iButNum > 127)iButNum = 127;	return int(_mjs.rgbButtons[iButNum]); }

		// Returns number of buttons
		inline int getNumButtons(void) { return numButtons; }

		// Returns number of view hats
		inline int getNumPOVS(void) { return numPOVS; }

		// Returns number of axis
		inline int getNumAxis(void) { return numAxis; }

		LPDIRECTINPUT8			directInput;			// Pointer to main DirectX input object
		LPDIRECTINPUTDEVICE8	device;	// DirectX input device object for the joystick
	private:
		bool joystickDetected;
		bool forcefeedbackDetected;
		DIJOYSTATE2 _mjs;	// Joystick states
		int numButtons;
		int numPOVS;
		int numAxis;
	};

}