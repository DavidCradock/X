#include "PCH.h"
#include "window.h"
#include "openGLExtensions.h"
#include "stringUtils.h"
#include "singletons.h"
#include "logging.h"
#include "settings.h"
#include "input.h"
#include "resourceManager.h"
#include "applicationManager.h"

namespace X
{
	LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		return SCWindow::getPointer()->MsgProc(hwnd, msg, wParam, lParam);
	}

	SCWindow::SCWindow(void)
	{
		SCLog* pLog = SCLog::getPointer();
		pLog->add("SCWindow::SCWindow() called.");

		_mhInstance = GetModuleHandle(NULL);
		_mhWindowHandle = NULL;
		_mhGLRenderContext = NULL;
		_mhDeviceContext = NULL;
		_mWindowClass = WNDCLASS{};
		_mClearColour.set(0.0f, 0.0f, 0.0f, 0.0f);
		_muiWindowWidth = 640;
		_muiWindowHeight = 480;

		// Set settings from settings file
		SCSettings* pSettings = SCSettings::getPointer();
		_mbVsyncEnabled = pSettings->getWindowVSync();
		_mbWindowFullscreen = pSettings->getWindowFullscreen();
	}

	void SCWindow::createWindow(std::string strWindowTitle)
	{
		// Called from SCSingletons() so can't use x->
		SCLog* pLog = SCLog::getPointer();
		pLog->add("SCWindow::createWindow() called.");

		// Store passed parameters
		_mstrWindowTitle = strWindowTitle;

		// Get module instance
		_mhInstance = GetModuleHandle(NULL);

		// Register window class
		_mWindowClass.cbClsExtra = 0;
		_mWindowClass.cbWndExtra = 0;
		_mWindowClass.hCursor = LoadCursor(0, IDC_ARROW);					// Default mouse cursor
		_mWindowClass.hIcon = LoadIcon(0, IDI_APPLICATION);					// Default icon for application
		_mWindowClass.hInstance = _mhInstance;								// Application instance (Retrieved above)
		_mWindowClass.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);	// No background brush required
		_mWindowClass.lpfnWndProc = MainWndProc;								// Window procedure which passes everything onto this class's window procedure method
		_mWindowClass.lpszClassName = L"XWindowClassName";
		_mWindowClass.lpszMenuName = 0;										// No menu wanted
		_mWindowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;			// Redraw on window move/resize and allocate a unique device context for each window in the class.
		ThrowIfFalse(RegisterClass(&_mWindowClass), "SCWindow::createWindow() failed to register window class.");

		// Window style (Depends on whether we're doing windowed or fullscreen
		DWORD dwExtendedStyle;
		DWORD dwStyle;
		if (_mbWindowFullscreen)
		{
			// If fullscreen, 
			dwExtendedStyle = WS_EX_APPWINDOW;		// Forces a top-level window onto the taskbar when the window is visible.
			dwStyle = WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;	// No borders or buttons or titlebars, nothing. Also prevent stuff from rendering into the window
		}
		else
		{
			dwExtendedStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;	// The window has a border with a raised edge.
			dwStyle = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;	// Has titlebar, border, caption, min/max/close buttons. Also prevent stuff from rendering into the window
		}

		// First, determine the dimensions of the window, depending upon whether we're going windowed or fullscreen mode
		// Remember, we're not changing display modes here
		// Get current desktop dimensions
		DEVMODE dmCurrent{};
		EnumDisplaySettings(NULL,	// Current device which this thread is running on
			ENUM_CURRENT_SETTINGS,
			&dmCurrent);
		_muiWindowWidth = (unsigned int)dmCurrent.dmPelsWidth;
		_muiWindowHeight = (unsigned int)dmCurrent.dmPelsHeight;
		// If we're in windowed mode, reduce the size a little
		if (!_mbWindowFullscreen)
		{
			_muiWindowWidth -= unsigned int(float(_muiWindowWidth) * 0.2f);
			_muiWindowHeight -= unsigned int(float(_muiWindowHeight) * 0.2f);
		}

		// Get a RECT to hold actual dimensions of the window so that, depending upon our set style,
		// the actual renderable area will be the size we request as the borders and such usually overlap a 
		// portion of the renderable area.
		RECT R = { 0, 0, (LONG)_muiWindowWidth, (LONG)_muiWindowHeight };
		AdjustWindowRectEx(&R, dwStyle, false, dwExtendedStyle);
		int width = R.right - R.left;
		int height = R.bottom - R.top;

		// Compute window position, centered if windowed mode.
		RECT rectDesktop;
		GetClientRect(GetDesktopWindow(), &rectDesktop);
		int iPosX = 0;
		int iPosY = 0;
		if (!_mbWindowFullscreen)
		{
			iPosX = (rectDesktop.right / 2) - (width / 2);
			iPosY = (rectDesktop.bottom / 2) - (height / 2);
		}

		// Create application window
		pLog->add("SCWindow::createWindow() creating window.");
		_mhWindowHandle = CreateWindowExW(
			dwExtendedStyle,
			L"XWindowClassName",
			StringUtils::stringToWide(_mstrWindowTitle).c_str(),
			dwStyle,
			iPosX, iPosY,	// Position
			width, height,
			0,	// Window parent
			0,	// Menu
			_mhInstance,
			0);	// Do not pass anything to WM_CREATE
		ThrowIfFalse(_mhWindowHandle, "SCWindow::createWindow() failed to create window.");

		// Attempt to get the window's device context
		pLog->add("SCWindow::createWindow() obtaining window's device context.");
		_mhDeviceContext = GetDC(_mhWindowHandle);
		ThrowIfFalse(_mhDeviceContext, "SCWindow::createWindow() failed to get window's device context.");

		// Set details we need for the pixel format
		static  PIXELFORMATDESCRIPTOR pfd =
		{
			sizeof(PIXELFORMATDESCRIPTOR),
			1,						// Version
			PFD_DRAW_TO_WINDOW |	// The buffer can draw to a window or device surface.
			PFD_SUPPORT_OPENGL |	// The buffer supports OpenGL drawing.
			PFD_DOUBLEBUFFER,		// The buffer is double-buffered
			PFD_TYPE_RGBA,			// RGBA pixels. Each pixel has four components in this order: red, green, blue, and alpha.
			32,						// Colour depth
			0, 0, 0, 0, 0, 0,		// Colour bits
			0,						// No alpha buffer
			0,						// Shift bits
			0,						// No accumulation buffer
			0, 0, 0, 0,				// Accumulation bits
			16,						// Bits for depth buffer
			0,						// No stencil buffer
			0,						// No auxiliary buffer
			PFD_MAIN_PLANE,			// Main drawing layer. Ignored. Earlier implementations of OpenGL used this member, but it is no longer used.
			0,						// Reserved
			0, 0, 0					// Layer Masks Ignored
		};

		// Attempt to get pixel format for the window using the above settings
		pLog->add("SCWindow::createWindow() choosing a compatible pixel format.");
		GLuint pixelFormat = ChoosePixelFormat(_mhDeviceContext, &pfd);
		ThrowIfFalse(pixelFormat, "SCWindow::createWindow() failed to choose a suitable pixel format for the window's device context.");

		// Attempt to set the pixel format
		pLog->add("SCWindow::createWindow() setting pixel format.");
		ThrowIfFalse(SetPixelFormat(_mhDeviceContext, pixelFormat, &pfd), "SCWindow::createWindow() failed to set the choosen pixel format for the window.");

		// Create a temporary context so we can obtain required function pointer to create a more modern core profile rendering context
		pLog->add("SCWindow::createWindow() creating temporary OpenGL rendering context to be able to obtain wglCreateContextAttibsARB function pointer.");
		HGLRC hglrcTemp = wglCreateContext(_mhDeviceContext);
		wglMakeCurrent(_mhDeviceContext, hglrcTemp);	// Make it current
		PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
		ThrowIfFalse(wglCreateContextAttribsARB, "SCWindow::createWindow() failed to get function pointer for wglCreateContextAttribsARB.");

		// Set the version we require
		int attribs[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
		WGL_CONTEXT_MINOR_VERSION_ARB, 3,
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0};

		// Attempt to create requested rendering context
		pLog->add("SCWindow::createWindow() creating OpenGL core profile rendering context.");
		_mhGLRenderContext = wglCreateContextAttribsARB(_mhDeviceContext, 0, attribs);
		ThrowIfFalse(_mhGLRenderContext, "SCWindow::createWindow() failed to create OpenGL rendering context for the window.");

		// Attempt to make the OpenGL rendering context the calling thread's current rendering context.
		pLog->add("SCWindow::createWindow() making rendering context the calling thread's current one.");
		ThrowIfFalse(wglMakeCurrent(_mhDeviceContext, _mhGLRenderContext), "SCWindow::createWindow() failed to make a specified OpenGL rendering context the calling thread's current rendering context.");

		// Delete the temporary context as no longer needed.
		pLog->add("SCWindow::createWindow() deleting the temporary OpenGL rendering context.");
		wglDeleteContext(hglrcTemp);
		hglrcTemp = NULL;

		// Show the window
		ShowWindow(_mhWindowHandle, SW_SHOW);

		// Bring the thread that created the specified window into the foreground and activates the window.
		// Keyboard input is directed to the window, and various visual cues are changed for the user.
		// The system assigns a slightly higher priority to the thread that created the foreground window than it does to other threads.
		SetForegroundWindow(_mhWindowHandle);

		// Sets the keyboard focus to the specified window
		SetFocus(_mhWindowHandle);

		// Resize the OpenGL viewport
		pLog->add("SCWindow::createWindow() resizing OpenGL viewport.");
		_resizeOpenGLViewport(_muiWindowWidth, _muiWindowHeight);

		pLog->add("SCWindow::createWindow() has created the window.");

		// Initialise input manager
		SCInputManager* pInputManager = SCInputManager::getPointer();
		pInputManager->init(_mhWindowHandle);

		// Obtain OpenGL extensions
		setupOpenGLExtensions(false);

		// Set Vsync (Needs extensions setup)
		setVsync(_mbVsyncEnabled);
	}

	void SCWindow::destroyWindow(void)
	{
		x->pLog->add("SCWindow::destroyWindow() called.");

		// Shutdown input manager
		x->pInput->shutdown();

		// Reset screen mode
		if (_mbWindowFullscreen)
		{
			x->pLog->add("SCWindow::destroyWindow() changing display mode back to normal.");
			ShowCursor(true);
		}

		// Release OpenGL rendering context
		if (_mhGLRenderContext)
		{
			x->pLog->add("SCWindow::destroyWindow() releasing OpenGL rendering context.");
			// Detach OpenGL rendering context from device context
			ThrowIfTrue(!wglMakeCurrent(NULL, NULL), "SCWindow::destroyWindow() unable to detach OpenGL rendering context from device context.");

			// Delete rendering context
			ThrowIfTrue(!wglDeleteContext(_mhGLRenderContext), "SCWindow::destroyWindow() unable to delete OpenGL rendering context.");
			_mhGLRenderContext = NULL;
		}
		// Release window's device context
		if (_mhDeviceContext)
		{
			x->pLog->add("SCWindow::destroyWindow() releasing window's device context.");
			ThrowIfTrue(!ReleaseDC(_mhWindowHandle, _mhDeviceContext), "SCWindow::destroyWindow() unable to release the window's device context.");
			_mhDeviceContext = NULL;
		}

		// Close window
		if (_mhWindowHandle)
		{
			x->pLog->add("SCWindow::destroyWindow() closing and destroying the window.");
			ThrowIfTrue(!CloseWindow(_mhWindowHandle), "SCWindow::destroyWindow() unable to close window.");
			ThrowIfTrue(!DestroyWindow(_mhWindowHandle), "SCWindow::destroyWindow() unable to destroy window.");
			_mhWindowHandle = NULL;
		}

		// Unregister window class
		x->pLog->add("SCWindow::destroyWindow() unregistering window class.");
		ThrowIfTrue(!UnregisterClassW(L"XWindowClassName", _mhInstance), "SCWindow::destroyWindow() unable to unregister window class.");
		_mhInstance = NULL;
		x->pLog->add("SCWindow::destroyWindow() complete.");
	}

	LRESULT SCWindow::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
		case WM_ACTIVATE:	// WM_ACTIVATE is sent when the window is activated or deactivated.
			if (LOWORD(wParam) == WA_INACTIVE)
			{
//				mbWindowMinimized = true;
				// Minimize the window if we're in fullscreen mode to prevent
				// the window from constantly showing over everything.
				if (_mbWindowFullscreen)
					ShowWindow(_mhWindowHandle, SW_MINIMIZE);
			}
			else
			{
//				mbWindowMinimized = false;
			}
			return 0;

		case WM_SYSCOMMAND:			// System commands
		{
			switch (wParam)
			{
			case SC_SCREENSAVE:		// Screensaver is trying to start
			case SC_MONITORPOWER:	// Monitor is trying to enter power saving mode
//				pLog->add("SCWindow::MsgProc() SC_SCREENSAVE or SC_MONITORPOWER received.");
				return 0;			// Prevent these things from happening
			}
			break;
		}
		case WM_CLOSE:				// Window is being closed
//			pLog->add("SCWindow::MsgProc() WM_CLOSE received. PostQuitMessage(0) called.");
			PostQuitMessage(0);		// We'll check this during our message peeking. Message will be WM_QUIT
			return 0;
		case WM_SIZE:				// WM_SIZE is sent when the user resizes the window.
//			pLog->add("SCWindow::MsgProc() WM_SIZE received. Calling _resizeOpenGLViewport().");
			_resizeOpenGLViewport(LOWORD(lParam), HIWORD(lParam));	// Resize stuff
			return 0;
		}
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	bool SCWindow::checkMessages(void)
	{
		// Check window messages
		MSG msg;
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// Window has been asked to close
			if (msg.message == WM_QUIT)
				return false;
			else
			{
//				TranslateMessage(&msg);	// Translates virtual-key messages into character messages
				DispatchMessage(&msg);	// Dispatches a message to a window procedure
			}
		}
		return true;
	}

	HWND SCWindow::getWindowHandle(void) const
	{
		return _mhWindowHandle;
	}

	HINSTANCE SCWindow::getApplicationInstance(void) const
	{
		return _mhInstance;
	}

	void SCWindow::setClearColour(CColour& clearColour)
	{
		_mClearColour = clearColour;
	}

	const std::string& SCWindow::getText(void) const
	{
		return _mstrWindowTitle;
	}

	void SCWindow::setText(const std::string& strText)
	{
		_mstrWindowTitle = strText;
		SetWindowTextW(_mhWindowHandle, StringUtils::stringToWide(strText).c_str());
	}

	unsigned int SCWindow::getWidth(void) const
	{
		return _muiWindowWidth;
	}

	unsigned int SCWindow::getHeight(void) const
	{
		return _muiWindowHeight;
	}

	CVector2f SCWindow::getDimensions(void) const
	{
		CVector2f vDims;
		vDims.x = float(_muiWindowWidth);
		vDims.y = float(_muiWindowHeight);
		return vDims;
	}

	bool SCWindow::getFullscreen(void) const
	{
		return _mbWindowFullscreen;
	}

	bool SCWindow::getVSyncEnabled(void) const
	{
		return _mbVsyncEnabled;
	}

	void SCWindow::_resizeOpenGLViewport(unsigned int uiNewWidth, unsigned int uiNewHeight)
	{
		SCLog::getPointer()->add("SCWindow::_resizeOpenGLViewport( " + std::to_string(uiNewWidth) + ", " + std::to_string(uiNewHeight) + ") called.");
		if (uiNewWidth < 1)
			uiNewWidth = 1;
		if (uiNewHeight < 1)
			uiNewHeight = 1;

		// Set OpenGL viewport size
		glViewport(0, 0, uiNewWidth, uiNewHeight);
		_muiWindowWidth = uiNewWidth;
		_muiWindowHeight = uiNewHeight;
	}

	void SCWindow::clearBackbuffer(void)
	{
		glClearColor(_mClearColour.red, _mClearColour.green, _mClearColour.blue, _mClearColour.alpha);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	void SCWindow::swapBuffers(void)
	{
		SwapBuffers(_mhDeviceContext);
	}

	void SCWindow::setVsync(bool bEnabled)
	{
		_mbVsyncEnabled = bEnabled;
		wglSwapIntervalEXT(_mbVsyncEnabled);
	}

	void SCWindow::toggleFullscreen(void)
	{
		// With the resource manager, call the method which removes all resources from the GPU which require an OpenGL context
		x->pResource->onGLContextToBeDestroyed();

		// Now destroy and recreate the window, toggling fullscreen mode whilst doing so
		destroyWindow();
		_mbWindowFullscreen = !_mbWindowFullscreen;
		createWindow(_mstrWindowTitle);

		// Now call the method which recreates all resources to the GPU which require an OpenGL context, putting everything back again to the original state.
		x->pResource->onGLContextRecreated();

		// Resize backbuffer
		x->pResource->getBackbuffer()->resizeToWindowDimsScaled();

		// Call SCApplicationManager::onToggleFullscreen to call all app's method too.
		// That method calls SCUIManager::onToggleFullscreen
		x->pAppMan->onWindowToggleFullscreen(_mbWindowFullscreen, _muiWindowWidth, _muiWindowHeight);
	}

	unsigned int SCWindow::getMaxTextureSize(void) const
	{
		int value;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &value);
		return (unsigned int)value;
	}

	void SCWindow::setIcon(int iIconResource)
	{
		HICON hIcon = LoadIcon(_mhInstance, MAKEINTRESOURCE(iIconResource));
//		_ASSERTE(hIcon != 0);
		SendMessage(_mhWindowHandle, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
	}

	unsigned int SCWindow::getRefreshRate(void) const
	{
		DEVMODE dmCurrent;
		EnumDisplaySettings(NULL,	// Current device which this thread is running on
			ENUM_CURRENT_SETTINGS,
			&dmCurrent);
		return (unsigned int)dmCurrent.dmDisplayFrequency;
	}


}