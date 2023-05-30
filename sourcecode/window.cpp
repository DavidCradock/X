#include "PCH.h"
#include "window.h"
#include "log.h"

namespace X
{
	LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		return Window::getPointer()->MsgProc(hwnd, msg, wParam, lParam);
	}

	Window::Window()
	{
		mhInstance = GetModuleHandle(NULL);
		mhWindowHandle = NULL;
		miWindowWidth = 1024;
		miWindowHeight = 576;
	}

	void Window::initialise(std::string strWindowTitle, int iWindowWidth, int iWindowHeight, bool bFullscreen, bool bVsyncEnabled)
	{
		Log* pLog = Log::getPointer();
		pLog->add("Window::initialise() called.");

		mbVsyncEnabled = bVsyncEnabled;

		mstrWindowTitle = strWindowTitle;

		// Create application window
		mWindowClass.cbClsExtra = 0;
		mWindowClass.cbWndExtra = 0;
		mWindowClass.hCursor = LoadCursor(0, IDC_ARROW);
		mWindowClass.hIcon = LoadIcon(0, IDI_APPLICATION);
		mWindowClass.hInstance = mhInstance;
		mWindowClass.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
		mWindowClass.lpfnWndProc = MainWndProc;	// Window procedure which passes everything onto this class's window procedure method
		mWindowClass.lpszClassName = L"XWindowClassName";
		mWindowClass.lpszMenuName = 0;
		mWindowClass.style = CS_HREDRAW | CS_VREDRAW;

		ThrowIfFalse(RegisterClass(&mWindowClass), "Window::initialise() failed to register window class.");

		RECT R = { 0, 0, miWindowWidth, miWindowHeight };
		AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
		int width = R.right - R.left;
		int height = R.bottom - R.top;

		mhWindowHandle = CreateWindow(L"XWindowClassName", StringToWString(mstrWindowTitle).c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, mhInstance, 0);
		ThrowIfFalse(mhWindowHandle, "Window::initialise() failed to create window.");

		// Center window
		RECT rectDesktop;
		GetClientRect(GetDesktopWindow(), &rectDesktop);
		int iPosLeft = (rectDesktop.right / 2) - (width / 2);
		int iPosTop = (rectDesktop.bottom / 2) - (height / 2);
		SetWindowPos(mhWindowHandle, HWND_TOP, iPosLeft, iPosTop, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		ShowWindow(mhWindowHandle, SW_SHOW);
		UpdateWindow(mhWindowHandle);
		pLog->add("Window::initialise() has created the application window.");
	}

	bool Window::update(void)
	{
		return true;
	}

	void Window::shutdown(void)
	{
		// Close window
		if (mhWindowHandle)
		{
			CloseWindow(mhWindowHandle);
			mhWindowHandle = NULL;
		}
		// Unregister window class
		UnregisterClass(L"XWindowClassName", mhInstance);
	}

	void Window::drawFrame(void)
	{

	}

	LRESULT Window::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
		case WM_ACTIVATE:	// WM_ACTIVATE is sent when the window is activated or deactivated.
			if (LOWORD(wParam) == WA_INACTIVE)
			{
				//mbWindowActive = false;
			}
			else
			{
				//mbWindowActive = true;
			}
			return 0;
		case WM_DESTROY:	// WM_DESTROY is sent when the window is being destroyed.
			PostQuitMessage(0);
			return 0;
		case WM_SIZE:		// WM_SIZE is sent when the user resizes the window.  
			//			miWindowWidth = LOWORD(lParam);
			//			miWindowHeight = HIWORD(lParam);
			return 0;
		case WM_ENTERSIZEMOVE:	// WM_EXITSIZEMOVE is sent when the user grabs the resize bars.
			return 0;
		case WM_EXITSIZEMOVE:	// WM_EXITSIZEMOVE is sent when the user releases the resize bars.
			return 0;

		case WM_GETMINMAXINFO:	// Catch this message so to prevent the window from becoming too small.
			((MINMAXINFO*)lParam)->ptMinTrackSize.x = 1024;
			((MINMAXINFO*)lParam)->ptMinTrackSize.y = 576;
			return 0;
		case WM_DISPLAYCHANGE:
			return 0;
		}


		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	HWND Window::getWindowHandle(void)
	{
		return mhWindowHandle;
	}

	HINSTANCE Window::getApplicationInstance(void)
	{
		return mhInstance;
	}

	void Window::setClearColour(glm::vec4& clearColour)
	{
		mv4ClearColour = clearColour;
	}

	int Window::getWindowWidth(void)
	{
		return miWindowWidth;
	}

	int Window::getWindowHeight(void)
	{
		return miWindowHeight;
	}

	bool Window::getWindowFullscreen(void)
	{
		return mbWindowFullscreen;
	}

	bool Window::getVSyncEnabled(void)
	{
		return mbVsyncEnabled;
	}
}