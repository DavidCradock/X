#include "PCH.h"
#include "inputJoystick.h"
#include "input.h"

namespace X
{

	// Function prototypes for joysticks
	BOOL CALLBACK enumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, void* pContext)
	{
		HRESULT hr;

		// Obtain an interface to the enumerated joystick.
		hr = SCInputManager::getPointer()->joy.directInput->CreateDevice(pdidInstance->guidInstance, &SCInputManager::getPointer()->joy.device, NULL);

		// If it failed, then we can't use this joystick. (Maybe the user unplugged
		// it while we were in the middle of enumerating it.)
		if (FAILED(hr))
			return DIENUM_CONTINUE;

		// Stop enumeration. Note: we're just taking the first joystick we get. You
		// could store all the enumerated joysticks and let the user pick.
		return DIENUM_STOP;
	}

	CInputJoystick::CInputJoystick(void)
	{
		device = NULL;
	}

	CInputJoystick::~CInputJoystick(void)
	{
	}

	bool CInputJoystick::init(LPDIRECTINPUT8 pMainDirectXinputDevice, HWND hApplicationWindow)
	{
		_mbJoystickDetected = true;
		_mbForcefeedbackDetected = true;
		_miNumButtons = 0;
		_miNumPOVS = 0;
		_miNumAxis = 0;
		HRESULT hr;

		directInput = pMainDirectXinputDevice;
		if (!directInput)
		{
			_mbJoystickDetected = false;
			_mbForcefeedbackDetected = false;
			return false;
		}

		// Enumerate available joysticks and create object for each one found
		// Check for force feedback devices first
		if (FAILED(hr = directInput->EnumDevices(DI8DEVCLASS_GAMECTRL, enumJoysticksCallback, NULL, DIEDFL_FORCEFEEDBACK | DIEDFL_ATTACHEDONLY)))
		{
			// No force feedback, check for normal joystick
			_mbForcefeedbackDetected = false;
			if (FAILED(hr = directInput->EnumDevices(DI8DEVCLASS_GAMECTRL, enumJoysticksCallback, NULL, DIEDFL_ATTACHEDONLY)))
			{
				_mbJoystickDetected = false;
				return true;
			}
		}

		// Do we have a joystick?
		if (device == NULL)
		{
			_mbJoystickDetected = false;
			_mbForcefeedbackDetected = false;
			return true;
		}

		// Set joystick data format
		hr = device->SetDataFormat(&c_dfDIJoystick2);
		if (FAILED(hr))
		{
			_mbJoystickDetected = false;
			_mbForcefeedbackDetected = false;
			return false;
		}

		// Set joystick behavior
		hr = device->SetCooperativeLevel(hApplicationWindow, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
		if (FAILED(hr))
		{
			_mbJoystickDetected = false;
			_mbForcefeedbackDetected = false;
			return false;
		}

		// Get joystick capabilities
		DIDEVCAPS	capsJ;
		capsJ.dwSize = sizeof(capsJ);
		hr = device->GetCapabilities(&capsJ);
		if (FAILED(hr))
		{
			_mbJoystickDetected = false;
			_mbForcefeedbackDetected = false;
			return false;
		}

		// Set joystick capability members
		_miNumButtons = capsJ.dwButtons;
		_miNumPOVS = capsJ.dwPOVs;
		_miNumAxis = capsJ.dwAxes;

		// Set ranges -1000 to 1000
		DIPROPRANGE diprg;
		diprg.diph.dwSize = sizeof(DIPROPRANGE);
		diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		diprg.diph.dwHow = DIPH_DEVICE;
		diprg.diph.dwObj = 0;
		diprg.lMin = -1000;
		diprg.lMax = +1000;
		device->SetProperty(DIPROP_RANGE, &diprg.diph);

		// Turn off auto center if force feedback
		if (_mbForcefeedbackDetected == true)
		{
			DIPROPDWORD DIPropAutoCenter;
			DIPropAutoCenter.diph.dwSize = sizeof(DIPropAutoCenter);
			DIPropAutoCenter.diph.dwHeaderSize = sizeof(DIPROPHEADER);
			DIPropAutoCenter.diph.dwHow = DIPH_DEVICE;
			DIPropAutoCenter.diph.dwObj = 0;
			DIPropAutoCenter.dwData = DIPROPAUTOCENTER_OFF;
			hr = device->SetProperty(DIPROP_AUTOCENTER, &DIPropAutoCenter.diph);
		}

		// Acquire joystick
		hr = device->Acquire();
		ZeroMemory(&_mjs, sizeof(DIJOYSTATE2));
		return true;
	}

	void CInputJoystick::update(void)
	{
		HRESULT hr;
		if (device == NULL)
			return;

		// Poll joystick, ready for reading
		if (DIERR_INPUTLOST == device->Poll())
		{
			// Device lost!
			device->Acquire();
		}

		// Read state of joystick
		hr = device->GetDeviceState(sizeof(DIJOYSTATE2), &_mjs);
		if (FAILED(hr))
			return;

		// Adjust values for hats
		if (_mjs.rgdwPOV[0] != -1)
			_mjs.rgdwPOV[0] /= 100;
	}

	void CInputJoystick::release(void)
	{
		if (device)	// Joystick object exists?
		{
			device->Unacquire();
			device->Release();
			device = NULL;
		}
	}

	bool CInputJoystick::getJoystickDetected(void) const
	{
		return _mbJoystickDetected;
	}

	bool CInputJoystick::getForcefeedbackDetected(void) const
	{ 
		return _mbForcefeedbackDetected;
	}

	long CInputJoystick::axisX(void) const
	{ 
		return _mjs.lX;
	}

	long CInputJoystick::axisY(void) const
	{
		return _mjs.lY;
	}

	long CInputJoystick::axisZ(void) const
	{
		return _mjs.lRz;
	}

	long CInputJoystick::axisS1(void) const
	{
		return _mjs.rglSlider[0];
	}

	long CInputJoystick::axisS2(void) const 
	{
		return _mjs.rglSlider[1];
	}

	DWORD CInputJoystick::axisPOW1(void) const
	{
		return _mjs.rgdwPOV[0];
	}

	DWORD CInputJoystick::axisPOW2(void) const 
	{
		return _mjs.rgdwPOV[1];
	}

	int CInputJoystick::buttonDown(unsigned int iButNum) const 
	{
		if (iButNum > 127)iButNum = 127;
		return int(_mjs.rgbButtons[iButNum]);
	}

	int CInputJoystick::getNumButtons(void) const
	{
		return _miNumButtons;
	}

	int CInputJoystick::getNumPOVS(void) const
	{
		return _miNumPOVS;
	}

	int CInputJoystick::getNumAxis(void) const
	{
		return _miNumAxis;
	}
}