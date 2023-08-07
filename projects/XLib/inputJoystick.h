#pragma once
#include "PCH.h"

namespace X
{

	// Joystick Input class, use SCInputManager to access it's "joy" member.
	// There is an object of this class called "joy" which is initialised and updated in the SCInputManager class.
	// Use that object to access information about the device. 
	class CInputJoystick
	{
	public:
		CInputJoystick(void);
		~CInputJoystick(void);

		// Init joystick device
		// Only returns an error if the given main device is invalid or
		// joystick is detected, but init failed as not all systems have joysticks attached. 
		bool init(LPDIRECTINPUT8 pMainDirectXinputDevice, HWND hApplicationWindow);

		// Updates joystick state
		void update(void);

		// Free everything
		void release(void);

		// Returns true if a joystick has been detected.
		inline bool getJoystickDetected(void) const;

		// Returns true if force feedback is supported.
		inline bool getForcefeedbackDetected(void) const;

		// Returns position along X axis +-1000
		inline long axisX(void) const;

		// Returns position along X axis +-1000
		inline long axisY(void) const;

		// Returns rotation of rudder +-1000
		inline long axisZ(void) const;

		// Returns position along slider 1 +-1000
		inline long axisS1(void) const;

		// Returns position along slider 2 +-1000
		inline long axisS2(void) const;

		// Returns 1st POW hat position. -1 for central position, else degrees from "north" position
		inline DWORD axisPOW1(void) const;

		// Returns 2nd POW hat position. -1 for central position, else degrees from "north" position
		inline DWORD axisPOW2(void) const;

		// Returns non zero number if the specified button is pressed. (upto 128 (0 to 127) buttons)
		inline int buttonDown(unsigned int iButNum = 0) const;

		// Returns number of buttons
		inline int getNumButtons(void) const;

		// Returns number of view hats
		inline int getNumPOVS(void) const;

		// Returns number of axis
		inline int getNumAxis(void) const;

		LPDIRECTINPUT8			directInput;			// Pointer to main DirectX input object
		LPDIRECTINPUTDEVICE8	device;					// DirectX input device object for the joystick
	private:
		bool _mbJoystickDetected;
		bool _mbForcefeedbackDetected;
		DIJOYSTATE2 _mjs;	// Joystick states
		int _miNumButtons;
		int _miNumPOVS;
		int _miNumAxis;
	};

}