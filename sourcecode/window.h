#pragma once
#include "PCH.h"
#include "singleton.h"

namespace X
{
	// Is responsible for creating/updating/shutting down the application's window.
	// Also initialises/updates/shutsdown the input manager.
	class Window : public Singleton<Window>
	{
	public:
		// Constructor
		Window(void);

		// Creates the window and sets screenmode (If fullscreen)
		void createWindow(std::string strWindowTitle);

		// Updates the window by checking messages, should be called each program loop
		// Is called from ApplicationManager::mainLoop()
		// Returns false if the window wants to close
		bool checkMessages(void);

		// Frees all objects and closes the window.
		void destroyWindow(void);

		// Returns the window's handle
		HWND getWindowHandle(void);

		// Returns the application's HINSTANCE
		HINSTANCE getApplicationInstance(void);

		// Returns window's text
		const std::string& getText(void);

		// Sets the window's text
		void setText(const std::string& strText);

		// Returns window's width
		int getWidth(void);

		// Returns window's height
		int getHeight(void);

		// Returns whether the window is fullscreen or not
		bool getFullscreen(void);

		// Returns whether VSync is enabled 
		bool getVSyncEnabled(void);
	
		// Sets the colour used when clearing the back buffer
		void setClearColour(glm::vec4& clearColour);

		// Swaps back and front buffers and then clears the back buffer
		void swapBuffers(void);

		// Window message procedure
		LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

		// Sets whether VSync is enabled or not
		void setVsync(bool bEnabled);

		// Tobbles between windowed and fullscreen modes
		void toggleFullscreen(void);
	private:
		WNDCLASS mWindowClass;			// Window class used to create the window
		HINSTANCE mhInstance;			// Application instance handle
		HWND mhWindowHandle;			// Window handle
		std::string mstrWindowTitle;	// Window's title text
		int miWindowWidth;				// Width of window
		int miWindowHeight;				// Height of window
		bool mbWindowFullscreen;		// Fullscreen or windowed
		bool mbVsyncEnabled;			// Vsync enabled or not
		glm::vec4 mv4ClearColour;		// The clear colour set by setClearColour();
		HGLRC mhGLRenderContext;		// OpenGL rendering context
		HDC mhDeviceContext;			// Device context for the window

		// Resize the window's OpenGL viewport
		void _resizeOpenGLViewport(int iNewWidth, int iNewHeight);
	};
}