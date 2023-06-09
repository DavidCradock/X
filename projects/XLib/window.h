#pragma once
#include "PCH.h"
#include "singleton.h"

namespace X
{
	// Is responsible for creating/updating/shutting down the application's window.
	// Also initialises/updates/shutsdown the input manager.
	class SCWindow : public CSingleton<SCWindow>
	{
	public:
		// Constructor
		SCWindow(void);

		// Creates the window and sets screenmode (If fullscreen)
		void createWindow(std::string strWindowTitle);

		// Updates the window by checking messages, should be called each program loop
		// Is called from SCApplicationManager::mainLoop()
		// Returns false if the window wants to close
		bool checkMessages(void);

		// Frees all objects and closes the window.
		void destroyWindow(void);

		// Returns the window's handle
		HWND getWindowHandle(void) const;

		// Returns the application's HINSTANCE
		HINSTANCE getApplicationInstance(void) const;

		// Returns window's text
		const std::string& getText(void) const;

		// Sets the window's text
		void setText(const std::string& strText);

		// Returns window's width
		unsigned int getWidth(void) const;

		// Returns window's height
		unsigned int getHeight(void) const;

		// Returns whether the window is fullscreen or not
		bool getFullscreen(void) const;

		// Returns whether VSync is enabled 
		bool getVSyncEnabled(void) const;
	
		// Sets the colour used when clearing the back buffer
		void setClearColour(glm::vec4& clearColour);

		// Clears the backbuffer with a call to glClearColor() using the set clear colour with setClearColour()
		void clearBackbuffer(void);

		// Swaps back and front buffers and then clears the back buffer
		void swapBuffers(void);

		// Window message procedure
		LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

		// Sets whether VSync is enabled or not
		void setVsync(bool bEnabled);

		// Toggles between windowed and fullscreen modes
		// This method is currently the only place where the OpenGL context will be lost/destroyed and rebuilt as the window has to be re-created.
		// Because of this, all OpenGL objects depending upon that context need to be recreated.
		// This is done from within this method by calling each of the various managers' _onOpenGLContextLost() methods to store which objects are currently loaded
		// and then the window and OpenGL context is re-created and then a call to each of the various managers' _onOpenGLContextRestored() methods which re-load/create the resources again.
		void toggleFullscreen(void);

		// Returns window dimensions as a glm::vec2
		glm::vec2 getDimensions(void) const;

		// Returns the maximum texture size supported by the OpenGL drivers
		unsigned int getMaxTextureSize(void) const;

		// Sets the icon of the application window using an int from resource.h
		void setIcon(int iIconResource);

		// Returns currently set refresh rate of the display
		unsigned int getRefreshRate(void) const;
		
	private:
		WNDCLASS mWindowClass;			// Window class used to create the window
		HINSTANCE mhInstance;			// Application instance handle
		HWND mhWindowHandle;			// Window handle
		std::string mstrWindowTitle;	// Window's title text
		unsigned int muiWindowWidth;		// Width of window
		unsigned int muiWindowHeight;	// Height of window
		bool mbWindowFullscreen;		// Fullscreen or windowed
		bool mbVsyncEnabled;			// Vsync enabled or not
		glm::vec4 mv4ClearColour;		// The clear colour set by setClearColour();
		HGLRC mhGLRenderContext;		// OpenGL rendering context
		HDC mhDeviceContext;			// Device context for the window

		// Resize the window's OpenGL viewport
		void _resizeOpenGLViewport(unsigned int iNewWidth, unsigned int iNewHeight);
	};
}