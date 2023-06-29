#include "PCH.h"
#include "window.h"
#include "log.h"
#include "openGLExtensions.h"
#include "input.h"
#include "utilities.h"
// Managers (for toggleFullscreen)
#include "resourceManager.h"

namespace X
{
	LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		return Window::getPointer()->MsgProc(hwnd, msg, wParam, lParam);
	}

	Window::Window(void)
	{
		mhInstance = GetModuleHandle(NULL);
		mhWindowHandle = NULL;
		mhGLRenderContext = NULL;
		mhDeviceContext = NULL;
		mWindowClass = WNDCLASS{};
		mbVsyncEnabled = true;
		mbWindowFullscreen = false;
		muiWindowWidth = 640;
		muiWindowHeight = 480;
		mv4ClearColour = glm::vec4(0.05f, 0.05f, 0.05f, 0.5f);
	}

	void Window::createWindow(std::string strWindowTitle)
	{
		Log* pLog = Log::getPointer();
		pLog->add("Window::createWindow() called.");

		// Store passed parameters
		mstrWindowTitle = strWindowTitle;

		// Get module instance
		mhInstance = GetModuleHandle(NULL);

		// Register window class
		mWindowClass.cbClsExtra = 0;
		mWindowClass.cbWndExtra = 0;
		mWindowClass.hCursor = LoadCursor(0, IDC_ARROW);					// Default mouse cursor
		mWindowClass.hIcon = LoadIcon(0, IDI_APPLICATION);					// Default icon for application
		mWindowClass.hInstance = mhInstance;								// Application instance (Retrieved above)
		mWindowClass.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);	// No background brush required
		mWindowClass.lpfnWndProc = MainWndProc;								// Window procedure which passes everything onto this class's window procedure method
		mWindowClass.lpszClassName = L"XWindowClassName";
		mWindowClass.lpszMenuName = 0;										// No menu wanted
		mWindowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;			// Redraw on window move/resize and allocate a unique device context for each window in the class.
		ThrowIfFalse(RegisterClass(&mWindowClass), "Window::createWindow() failed to register window class.");

		// Window style (Depends on whether we're doing windowed or fullscreen
		DWORD dwExtendedStyle;
		DWORD dwStyle;
		if (mbWindowFullscreen)
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
		muiWindowWidth = (unsigned int)dmCurrent.dmPelsWidth;
		muiWindowHeight = (unsigned int)dmCurrent.dmPelsHeight;
		// If we're in windowed mode, reduce the size a little
		if (!mbWindowFullscreen)
		{
			muiWindowWidth -= unsigned int(float(muiWindowWidth) * 0.2f);
			muiWindowHeight -= unsigned int(float(muiWindowHeight) * 0.2f);
		}

		// Get a RECT to hold actual dimensions of the window so that, depending upon our set style,
		// the actual renderable area will be the size we request as the borders and such usually overlap a 
		// portion of the renderable area.
		RECT R = { 0, 0, (LONG)muiWindowWidth, (LONG)muiWindowHeight };
		AdjustWindowRectEx(&R, dwStyle, false, dwExtendedStyle);
		int width = R.right - R.left;
		int height = R.bottom - R.top;

		// Compute window position, centered if windowed mode.
		RECT rectDesktop;
		GetClientRect(GetDesktopWindow(), &rectDesktop);
		int iPosX = 0;
		int iPosY = 0;
		if (!mbWindowFullscreen)
		{
			iPosX = (rectDesktop.right / 2) - (width / 2);
			iPosY = (rectDesktop.bottom / 2) - (height / 2);
		}

		// Create application window
		pLog->add("Window::createWindow() creating window.");
		mhWindowHandle = CreateWindowExW(
			dwExtendedStyle,
			L"XWindowClassName",
			StringToWString(mstrWindowTitle).c_str(),
			dwStyle,
			iPosX, iPosY,	// Position
			width, height,
			0,	// Window parent
			0,	// Menu
			mhInstance,
			0);	// Do not pass anything to WM_CREATE
		ThrowIfFalse(mhWindowHandle, "Window::createWindow() failed to create window.");

		// Attempt to get the window's device context
		pLog->add("Window::createWindow() obtaining window's device context.");
		mhDeviceContext = GetDC(mhWindowHandle);
		ThrowIfFalse(mhDeviceContext, "Window::createWindow() failed to get window's device context.");

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
		pLog->add("Window::createWindow() choosing a compatible pixel format.");
		GLuint pixelFormat = ChoosePixelFormat(mhDeviceContext, &pfd);
		ThrowIfFalse(pixelFormat, "Window::createWindow() failed to choose a suitable pixel format for the window's device context.");

		// Attempt to set the pixel format
		pLog->add("Window::createWindow() setting pixel format.");
		ThrowIfFalse(SetPixelFormat(mhDeviceContext, pixelFormat, &pfd), "Window::createWindow() failed to set the choosen pixel format for the window.");

		// Create a temporary context so we can obtain required function pointer to create a more modern core profile rendering context
		pLog->add("Window::createWindow() creating temporary OpenGL rendering context to be able to obtain wglCreateContextAttibsARB function pointer.");
		HGLRC hglrcTemp = wglCreateContext(mhDeviceContext);
		wglMakeCurrent(mhDeviceContext, hglrcTemp);	// Make it current
		PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
		ThrowIfFalse(wglCreateContextAttribsARB, "Window::createWindow() failed to get function pointer for wglCreateContextAttribsARB.");

		// Set the version we require
		int attribs[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
		WGL_CONTEXT_MINOR_VERSION_ARB, 3,
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0};

		// Attempt to create requested rendering context
		pLog->add("Window::createWindow() creating OpenGL core profile rendering context.");
		mhGLRenderContext = wglCreateContextAttribsARB(mhDeviceContext, 0, attribs);
		ThrowIfFalse(mhGLRenderContext, "Window::createWindow() failed to create OpenGL rendering context for the window.");

		// Attempt to make the OpenGL rendering context the calling thread's current rendering context.
		pLog->add("Window::createWindow() making rendering context the calling thread's current one.");
		ThrowIfFalse(wglMakeCurrent(mhDeviceContext, mhGLRenderContext), "Window::createWindow() failed to make a specified OpenGL rendering context the calling thread's current rendering context.");

		// Delete the temporary context as no longer needed.
		pLog->add("Window::createWindow() deleting the temporary OpenGL rendering context.");
		wglDeleteContext(hglrcTemp);
		hglrcTemp = NULL;

		// Show the window
		ShowWindow(mhWindowHandle, SW_SHOW);

		// Bring the thread that created the specified window into the foreground and activates the window.
		// Keyboard input is directed to the window, and various visual cues are changed for the user.
		// The system assigns a slightly higher priority to the thread that created the foreground window than it does to other threads.
		SetForegroundWindow(mhWindowHandle);

		// Sets the keyboard focus to the specified window
		SetFocus(mhWindowHandle);

		// Resize the OpenGL viewport
		pLog->add("Window::createWindow() resizing OpenGL viewport.");
		_resizeOpenGLViewport(muiWindowWidth, muiWindowHeight);

		pLog->add("Window::createWindow() has created the window.");

		// Initialise input manager
		InputManager* pInputManager = InputManager::getPointer();
		pInputManager->init(mhWindowHandle);

		// Obtain OpenGL extensions
		setupOpenGLExtensions(false);

		// Set Vsync (Needs extensions setup)
		setVsync(mbVsyncEnabled);
	}

	void Window::destroyWindow(void)
	{
		Log* pLog = Log::getPointer();
		pLog->add("Window::destroyWindow() called.");

		// Shutdown input manager
		InputManager* pInputManager = InputManager::getPointer();
		pInputManager->shutdown();

		// Reset screen mode
		if (mbWindowFullscreen)
		{
			pLog->add("Window::destroyWindow() changing display mode back to normal.");
			ShowCursor(true);
		}

		// Release OpenGL rendering context
		if (mhGLRenderContext)
		{
			pLog->add("Window::destroyWindow() releasing OpenGL rendering context.");
			// Detach OpenGL rendering context from device context
			ThrowIfTrue(!wglMakeCurrent(NULL, NULL), "Window::destroyWindow() unable to detach OpenGL rendering context from device context.");

			// Delete rendering context
			ThrowIfTrue(!wglDeleteContext(mhGLRenderContext), "Window::destroyWindow() unable to delete OpenGL rendering context.");
			mhGLRenderContext = NULL;
		}
		// Release window's device context
		if (mhDeviceContext)
		{
			pLog->add("Window::destroyWindow() releasing window's device context.");
			ThrowIfTrue(!ReleaseDC(mhWindowHandle, mhDeviceContext), "Window::destroyWindow() unable to release the window's device context.");
			mhDeviceContext = NULL;
		}

		// Close window
		if (mhWindowHandle)
		{
			pLog->add("Window::destroyWindow() closing and destroying the window.");
			ThrowIfTrue(!CloseWindow(mhWindowHandle), "Window::destroyWindow() unable to close window.");
			ThrowIfTrue(!DestroyWindow(mhWindowHandle), "Window::destroyWindow() unable to destroy window.");
			mhWindowHandle = NULL;
		}

		// Unregister window class
		pLog->add("Window::destroyWindow() unregistering window class.");
		ThrowIfTrue(!UnregisterClassW(L"XWindowClassName", mhInstance), "Window::destroyWindow() unable to unregister window class.");
		mhInstance = NULL;
		pLog->add("Window::destroyWindow() complete.");
	}

	LRESULT Window::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		Log* pLog = Log::getPointer();

		switch (msg)
		{
		case WM_ACTIVATE:	// WM_ACTIVATE is sent when the window is activated or deactivated.
			if (LOWORD(wParam) == WA_INACTIVE)
			{
//				mbWindowMinimized = true;
				// Minimize the window if we're in fullscreen mode to prevent
				// the window from constantly showing over everything.
				if (mbWindowFullscreen)
					ShowWindow(mhWindowHandle, SW_MINIMIZE);
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
//				pLog->add("Window::MsgProc() SC_SCREENSAVE or SC_MONITORPOWER received.");
				return 0;			// Prevent these things from happening
			}
			break;
		}
		case WM_CLOSE:				// Window is being closed
//			pLog->add("Window::MsgProc() WM_CLOSE received. PostQuitMessage(0) called.");
			PostQuitMessage(0);		// We'll check this during our message peeking. Message will be WM_QUIT
			return 0;
		case WM_SIZE:				// WM_SIZE is sent when the user resizes the window.
//			pLog->add("Window::MsgProc() WM_SIZE received. Calling _resizeOpenGLViewport().");
			_resizeOpenGLViewport(LOWORD(lParam), HIWORD(lParam));	// Resize stuff
			return 0;
		}
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	bool Window::checkMessages(void)
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

		// Update the input manager
		InputManager* pInputManager = InputManager::getPointer();
		pInputManager->update(mbWindowFullscreen, muiWindowWidth, muiWindowHeight);
		return true;
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

	const std::string& Window::getText(void)
	{
		return mstrWindowTitle;
	}

	void Window::setText(const std::string& strText)
	{
		SetWindowTextW(mhWindowHandle, StringToWString(strText).c_str());
	}

	unsigned int Window::getWidth(void)
	{
		return muiWindowWidth;
	}

	unsigned int Window::getHeight(void)
	{
		return muiWindowHeight;
	}

	bool Window::getFullscreen(void)
	{
		return mbWindowFullscreen;
	}

	bool Window::getVSyncEnabled(void)
	{
		return mbVsyncEnabled;
	}

	void Window::_resizeOpenGLViewport(unsigned int uiNewWidth, unsigned int uiNewHeight)
	{
		Log::getPointer()->add("Window::_resizeOpenGLViewport( " + std::to_string(uiNewWidth) + ", " + std::to_string(uiNewHeight) + ") called.");
		if (uiNewWidth < 1)
			uiNewWidth = 1;
		if (uiNewHeight < 1)
			uiNewHeight = 1;

		// Set OpenGL viewport size
		glViewport(0, 0, uiNewWidth, uiNewHeight);
		muiWindowWidth = uiNewWidth;
		muiWindowHeight = uiNewHeight;
	}

	void Window::clearBackbuffer(void)
	{
		glClearColor(mv4ClearColour.r, mv4ClearColour.g, mv4ClearColour.b, mv4ClearColour.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	void Window::swapBuffers(void)
	{
		SwapBuffers(mhDeviceContext);
	}

	void Window::setVsync(bool bEnabled)
	{
		mbVsyncEnabled = bEnabled;
		wglSwapIntervalEXT(mbVsyncEnabled);
	}

	void Window::toggleFullscreen(void)
	{
		// With the resource manager, call the method which removes all resources from the GPU which require an OpenGL context
		ResourceManager* pResMan = ResourceManager::getPointer();
		pResMan->onGLContextToBeDestroyed();

		// Now destroy and recreate the window, toggling fullscreen mode whilst doing so
		destroyWindow();
		mbWindowFullscreen = !mbWindowFullscreen;
		createWindow(mstrWindowTitle);

		// Now call the method which recreates all resources to the GPU which require an OpenGL context, putting everything back again to the original state.
		pResMan->onGLContextRecreated();

	}

	glm::vec2 Window::getDimensions(void)
	{
		glm::vec2 vDims;
		vDims.x = float(muiWindowWidth);
		vDims.y = float(muiWindowHeight);
		return vDims;
	}

	int Window::getMaxTextureSize(void)
	{
		int value;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &value);
		return value;
	}
}