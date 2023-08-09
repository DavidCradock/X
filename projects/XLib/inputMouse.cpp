#include "PCH.h"
#include "inputMouse.h"
#include "singletons.h"

namespace X
{

	CInputMouse::CInputMouse(void)
	{
		windowHandle = NULL;
		lpDIDeviceM = NULL;
		leftButOnce[0] = false;
		leftButOnce[1] = false;
		midButOnce[0] = false;
		midButOnce[1] = false;
		rightButOnce[0] = false;
		rightButOnce[1] = false;
		mouseLeft = false;
		mouseMiddle = false;
		mouseRight = false;
		mouseX = 0;
		mouseY = 0;
		mouseXold = 0;
		mouseYold = 0;
		mouseDeltaX = 0.0f;
		mouseDeltaY = 0.0f;
		mouseDeltaZ = 0.0f;
	}

	CInputMouse::~CInputMouse(void)
	{
	}

	bool CInputMouse::init(LPDIRECTINPUT8 pMainDirectXinputDevice, HWND hApplicationWindow)
	{
		windowHandle = hApplicationWindow;

		lpDI = pMainDirectXinputDevice;
		if (!lpDI)
			return false;

		// Create mouse device
		lpDI->CreateDevice(GUID_SysMouse, &lpDIDeviceM, NULL);
		if (lpDIDeviceM == NULL)
			return false;

		// Set mouse data format
		HRESULT hr = lpDIDeviceM->SetDataFormat(&c_dfDIMouse);
		if (FAILED(hr))
			return false;

		// Set mouse behavior
		hr = lpDIDeviceM->SetCooperativeLevel(hApplicationWindow, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
		//	hr = _mlpDIDeviceM->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND);
		if (FAILED(hr))
			return false;

		// Acquire the Mouse
		hr = lpDIDeviceM->Acquire();
		if (FAILED(hr))
			return false;

		return true;	// Success!
	}

	void CInputMouse::update(bool fullscreen, unsigned int screenWidth, unsigned int screenHeight)
	{
		DIMOUSESTATE mouseState;	// Struct to store state of mouse
		HRESULT hr = lpDIDeviceM->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&mouseState);
		if (FAILED(hr))
		{
			if (hr = DIERR_INPUTLOST)
			{
				hr = lpDIDeviceM->Acquire();
				if (FAILED(hr))
				{
					// Device lost
					mouseLeft = false;
					mouseRight = false;
					mouseMiddle = false;
					leftButOnce[0] = leftButOnce[1] = false;
					rightButOnce[0] = rightButOnce[1] = false;
					midButOnce[0] = midButOnce[1] = false;
					return;
				}
			}
		}

		// Left mouse button down?
		if (mouseState.rgbButtons[0] == 0x80)	mouseLeft = true;
		else	mouseLeft = false;

		// Middle mouse button down?
		if (mouseState.rgbButtons[2] == 0x80)	mouseMiddle = true;
		else	mouseMiddle = false;

		// Right mouse button down?
		if (mouseState.rgbButtons[1] == 0x80)	mouseRight = true;
		else	mouseRight = false;

		// Update button once stuff
		if (mouseLeft) // Is mouse button down?
		{
			if (leftButOnce[1] == false) // Wasn't down on last update
			{
				leftButOnce[1] = true;
				leftButOnce[0] = true;
			}
			else  // Was down last time
			{
				leftButOnce[0] = false;  // Return value
			}
		}
		else
		{
			// No longer pressed
			leftButOnce[1] = false;
			leftButOnce[0] = false;
		}
		if (mouseRight) // Is mouse button down?
		{
			if (rightButOnce[1] == false) // Wasn't down on last update
			{
				rightButOnce[1] = true;
				rightButOnce[0] = true;
			}
			else  // Was down last time
			{
				rightButOnce[0] = false;  // Return value
			}
		}
		else
		{
			// No longer pressed
			rightButOnce[1] = false;
			rightButOnce[0] = false;
		}
		if (mouseMiddle) // Is mouse button down?
		{
			if (midButOnce[1] == false) // Wasn't down on last update
			{
				midButOnce[1] = true;
				midButOnce[0] = true;
			}
			else  // Was down last time
			{
				midButOnce[0] = false;  // Return value
			}
		}
		else
		{
			// No longer pressed
			midButOnce[1] = false;
			midButOnce[0] = false;
		}

		// Update members
		mouseDeltaX = (float)mouseState.lX;					// Change in position along X
		mouseDeltaY = (float)mouseState.lY;					// Change in position along Y
		mouseDeltaZ = (float)mouseState.lZ;					// Change in position along Z (Wheel)

		// Store old position...
		mouseXold = mouseX;
		mouseYold = mouseY;

		// Cursor stuff
		POINT point;
		GetCursorPos(&point);
		if (fullscreen)
		{
			mouseX = (float)point.x;
			mouseY = (float)point.y;
		}
		else
		{
			WINDOWINFO wndInfo;
			GetWindowInfo(windowHandle, &wndInfo);

			mouseX = (float)point.x;
			mouseX -= wndInfo.rcClient.left;
			mouseY = (float)point.y;
			mouseY -= wndInfo.rcClient.top;
		}
	}

	void CInputMouse::release(void)
	{
		if (lpDIDeviceM)	// Mouse object exists?
		{
			lpDIDeviceM->Unacquire();
			lpDIDeviceM->Release();
			lpDIDeviceM = NULL;
		}
	}

	void CInputMouse::setMousePos(int ix, int iy)
	{
		mouseX = float(ix);
		mouseY = float(iy);
		mouseXold = mouseX;
		mouseYold = mouseY;
		mouseDeltaX = mouseDeltaY = 0;

		// Get window dimensions (Not including borders or menu bar, only client area)
		WINDOWINFO wiWindow;
		GetWindowInfo(windowHandle, &wiWindow);
		// Modify the rcWindow value to take into account the app's title bar
		AdjustWindowRect(&wiWindow.rcWindow, WS_POPUPWINDOW | WS_CAPTION, false);
		POINT mPos;
		mPos.x = wiWindow.rcClient.left + (int)ix;
		mPos.y = wiWindow.rcClient.top + (int)iy;
		SetCursorPos(mPos.x, mPos.y);
	}

	void CInputMouse::setMouseCursorImage(const std::string& strAniFilename)
	{
		x->pWindow->setMouseCursorImage(strAniFilename);
	}
}