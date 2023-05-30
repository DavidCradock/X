#pragma once
#include "PCH.h"
#include "singleton.h"

namespace X
{

	class Window : public Singleton<Window>
	{
	public:
		Window();

		// Creates the window and initialises OpenGL
		void initialise(std::string strWindowTitle, int iWindowWidth, int iWindowHeight, bool bFullscreen = false, bool bVsyncEnabled = true);

		// Updates the window 
		bool update(void);

		// Frees all objects and closes the window.
		void shutdown(void);

		// Draws a frame to the backbuffer and presents to the window
		void drawFrame(void);

		// Window message procedure
		LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

		// Returns the window's handle
		HWND getWindowHandle(void);

		// Returns the application's HINSTANCE
		HINSTANCE getApplicationInstance(void);

		// Sets the colour used when clearing the framebuffers
		void setClearColour(glm::vec4& clearColour);

		// Returns window's width
		int getWindowWidth(void);

		// Returns window's height
		int getWindowHeight(void);

		// Returns whether the window is fullscreen or not
		bool getWindowFullscreen(void);

		// Returns whether VSync is enabled or not
		bool getVSyncEnabled(void);
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
		

	};
}