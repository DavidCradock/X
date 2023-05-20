#pragma once
#include "PCH.h"
#include "singleton.h"

namespace X
{
	class Window : public Singleton<Window>
	{
	public:
		Window();

		// Creates the window and initialises Vulkan
		void initialise(std::string strWindowTitle, int iWindowWidth, int iWindowHeight);

		// Updates the window 
		bool update(void);

		// Frees all objects and closes the window.
		void shutdown(void);

		// Window message procedure
		LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

		HWND getWindowHandle(void);
	private:
		WNDCLASS mWindowClass;			// Window class used to create the window
		HINSTANCE mhInstance;			// Application instance handle
		HWND mhWindowHandle;			// Window handle
		std::string mstrWindowTitle;	// Window's title text
		int miWindowWidth;				// Width of window
		int miWindowHeight;				// Height of window

		VkInstance mvkInstance;			// Main Vulkan instance
	};
}