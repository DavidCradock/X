#pragma once
#include "PCH.h"
#include "singletons.h"
#include "applicationBase.h"
#include "timer.h"

namespace X
{
	// Application manager class to handle all apps which are derived from CApplicationBase found in "applicationBase.h"
	// By default, the current application to be run each loop is the AppDevelopment application
	// This can be changed by calling x->pAppMan->switchToApp("YourApplicationName");
	// The purpose of this manager is so that we can write tools, editors and be able to switch between them.
	// It also helps me to test the resource loading/unloading which each app uses to make sure everything is
	// being intialised/shutdown properly.
	// It's also nice to have the entire codebase's management handled within.
	class SCApplicationManager : public CSingleton<SCApplicationManager>
	{
	public:
		SCApplicationManager();

		// This is the main loop of the entire program, call this from WinMain.
		// This creates each of the applications, adds them to this manager and performs initialisation, updating and shutdown
		void mainLoop(void);

		// Add a new application class object pointer derived from the CApplicationBase class to the manager
		// If the name of the application already exists, an exception occurs.
		// The first time this is called, that application is set as currently active.
		void addApp(const std::string& applicationName, CApplicationBase* pTheApplication);

		// Return the number of applications added to this manager
		size_t getNumApps(void) const;

		// Return application name at specified index into map
		// If invalid index given, a critical error occurs
		const std::string& getAppName(unsigned int index) const;

		// Switches from the current application, to the named one.
		// If invalid application name is given, an exception is thrown
		// This'll call onStop() of the currently set application, then call onStart() of the new one.
		// If the class name given is already set as current, it's call onStop() then onStart() of that class, kinda like a reset.
		void switchToApp(const std::string& applicationToSwitchTo);

		// Will switch to the next app, calling onStop() of current application and onStart() of the next
		void switchToNextApp(void);

		// Returns the name of the currently set application
		std::string getCurrentAppName(void) const;

		// Returns the amount of seconds the named application has been active/running for
		// If an invalid application name is given, an exception occurs
		float getApplicationRuntime(const std::string& applicationName) const;

		// Called by SCWindow::toggleFullscreen after the window and resources have been restored.
		// This then calls each application's onWindowToggleFullscreen() method if it is defined.
		// It's params are the new state of the window.
		void onWindowToggleFullscreen(bool bFullscreen, int iWindowWidth, int iWindowHeight);

		// Exits the main loop and shutdown
		void shutdown(void);

		// Toggles rendering of a debug grid of lines.
		void debugShowGrid(bool bShowGrid, int iSpacingX = 10, int iSpacingY = 10);

		// Returns whether debug grid is enabled or not
		bool debugGridShown(void) const;

		// Returns currently set debug grid spacing values
		CVector2f debugGridSpacing(void) const;

	private:
		// Calls all added applications' initOnce() methods
		void callAllApps_initOnce(void);

		// Calls currently set application's onUpdate() method
		// Returns false if the application wants to shutdown
		bool callCurrentApp_onUpdate(void);

		// Calls currently set application's onFrameEnd() method
		void callCurrentApp_onFrameEnd(void);

		mutable std::map<std::string, CApplicationBase*> _mApplications;	// Hash map which holds each named application
		std::string _mstrCurrentApp;										// Application name of the current application.
		CTimer _mTimer;														// Used to update application running time.

		// debugShowGrid
		void _debugGridRender(void);	// Renders the debug grid
		bool _mbDebugGridShow;			// Whether the render the debug grid or not
		float _mfDebugGridSpacing[2];		// Spacing values for the debug grid
		float _mfDebugGridTextAlpha;	// Set to 1 on call to debugShowGrid
	};
}