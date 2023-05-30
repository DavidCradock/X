#include "PCH.h"
#include "applicationManager.h"
#include "log.h"
#include "window.h"
#include "input.h"
// Include each application
#include "applicationDevelopment.h"
#include "applicationGame.h"

namespace X
{
	void ApplicationManager::mainLoop(void)
	{
		try
		{
			// Here's the program's main loop called from WinMain.

			// Clear the log file
			Log* pLog = Log::getPointer();
			pLog->clear();
			pLog->add("ApplicationManager::mainLoop() called.");

			// Create objects to each of the application classes which inherit from ApplicationBase
			ApplicationDevelopment* pAppDevelopment = new ApplicationDevelopment;
			ThrowIfTrue(0 == pAppDevelopment, "ApplicationManager::mainLoop() failed to allocate ApplicationDevelopment.");

			ApplicationGame* pAppGame = new ApplicationGame;
			ThrowIfTrue(0 == pAppGame, "ApplicationManager::mainLoop() failed to allocate ApplicationGame.");

			// Add each application to this manager
			addApp("Development Application", pAppDevelopment);
			addApp("Game", pAppGame);

			// Create window
			Window* pWindow = Window::getPointer();
			pWindow->initialise("X", 1024, 768, false);

			// Initialise input manager
			InputManager* pInputManager = InputManager::getPointer();
			pInputManager->init(pWindow->getWindowHandle());

			// Now call each application's initOnce method
			callAllApps_initOnce();

			// Call currently set application's onStart method
			std::string strLog = "ApplicationManager::mainLoop() calling " + mstrCurrentApp + "'s onStart()";
			pLog->add(strLog);
			std::map<std::string, ApplicationBase*>::iterator it = mApplications.find(mstrCurrentApp);
			ThrowIfTrue(it == mApplications.end(), "ApplicationManager::mainLoop() unable to find the current application called " + mstrCurrentApp);
			it->second->onStart();

			// Enter main loop...
			pLog->add("ApplicationManager::mainLoop() is entering main loop...");
			mTimer.update();

			while (pWindow->update())
			{
				mTimer.update();
				pInputManager->update(pWindow->getWindowFullscreen(), pWindow->getWindowWidth(), pWindow->getWindowHeight());

				if (!callCurrentApp_onUpdate())
				{
					break;	// Application wants to close
				}
			}

			// Last current app's onStop method
			pLog->add("ApplicationManager::mainLoop() is shutting down...");
			it = mApplications.find(mstrCurrentApp);
			ThrowIfTrue(it == mApplications.end(), "unable to find the current application called " + mstrCurrentApp);
			it->second->onStop();

			// Shutdown input manager
			pInputManager->shutdown();

			// Now close the window
			pWindow->shutdown();
		}
		catch (Exception &exception)
		{
			Log::getPointer()->add("Total runtime: " + mTimer.getClock());
			Log::getPointer()->add(exception.mstrException, true);
			MessageBox(Window::getPointer()->getWindowHandle(), StringToWString(exception.mstrException).c_str(), L"Sorry, an exception has been thrown...", MB_OK);
			__debugbreak();
		}
		Log::getPointer()->add("Total runtime: " + mTimer.getClock());
	}

	void ApplicationManager::addApp(const std::string& applicationName, ApplicationBase* pTheApplication)
	{
		Log *pLog = Log::getPointer();
		std::string strLog = "ApplicationManager::addApp(" + applicationName + ") called.";
		Log::getPointer()->add(strLog);

		// Make sure it doesn't already exist
		std::map<std::string, ApplicationBase*>::iterator it = mApplications.find(applicationName);
		ThrowIfTrue(it != mApplications.end(), "ApplicationManager::addApp(" + applicationName + " failed. The application name already exists");

		// If we get here, we can add it
		mApplications[applicationName] = pTheApplication;

		// If no other applications have been added, set this one up to be the currently set application
		if (mstrCurrentApp.length() == 0)
		{
			mstrCurrentApp = applicationName;
		}
	}

	const std::string& ApplicationManager::getAppName(unsigned int index)
	{
		Log* pLog = Log::getPointer();
		std::string strLog = "ApplicationManager::getAppName() with index " + std::to_string(index) + " called.";
		Log::getPointer()->add(strLog);

		ThrowIfTrue(index >= mApplications.size(), "ApplicationManager::getAppName(" + std::to_string(index) + ") failed. Invalid index given, maximum number of added applications is " + std::to_string(mApplications.size()));

		std::map<std::string, ApplicationBase*>::iterator it = mApplications.begin();
		for (unsigned int i = 0; i < index; ++i)
		{
			it++;
		}
		return it->first;
	}

	void ApplicationManager::switchToApp(const std::string& applicationToSwitchTo)
	{
		Log* pLog = Log::getPointer();
		std::string strLog = "ApplicationManager::switchToApp(" + applicationToSwitchTo + " called.";
		Log::getPointer()->add(strLog);

		// Attempt to find the application we're trying to switch to
		std::map<std::string, ApplicationBase*>::iterator itNewApp = mApplications.find(applicationToSwitchTo);
		ThrowIfTrue(itNewApp == mApplications.end(), "ApplicationManager::switchToApp(\"" + applicationToSwitchTo + "\") failed. Application by given name doesn't exist.");

		// Attempt to find the application that's currently set as current
		std::map<std::string, ApplicationBase*>::iterator itOldApp = mApplications.find(mstrCurrentApp);
		ThrowIfTrue(itOldApp == mApplications.end(), "ApplicationManager::switchToApp(" + applicationToSwitchTo + " failed. Prior to changing to the new application, the old application called " + mstrCurrentApp + " doesn't exist which is mental!");

		// Change current app name and call the various methods for each
		mstrCurrentApp = applicationToSwitchTo;
		itOldApp->second->onStop();
		itNewApp->second->onStart();
	}

	void ApplicationManager::switchToNextApp(void)
	{
		Log* pLog = Log::getPointer();
		std::string strLog = "ApplicationManager::SwitchToNextApp() called.";
		Log::getPointer()->add(strLog);

		// If no apps are currently set
		ThrowIfTrue(0 == mstrCurrentApp.size(), "ApplicationManager::switchToNextApp() failed. There is no currently set application name.");

		// If only one app exists
		ThrowIfTrue(1 == mApplications.size(), "ApplicationManager::switchToNextApp() failed. There is only one application which exists.");

		// Find index of current application
		int indexOfCurrentApp = -1;
		for (int i = 0; i < getNumApps(); ++i)
		{
			if (mstrCurrentApp == getAppName(i))
			{
				indexOfCurrentApp = i;
				break;
			}
		}
		ThrowIfTrue(-1 == indexOfCurrentApp, "ApplicationManager::switchToNextApp() failed. Unable to find currently set application.");

		// Determine index of next app (wrap around)
		int indexOfNextApp = indexOfCurrentApp + 1;
		if (indexOfNextApp > mApplications.size() - 1)
			indexOfNextApp = 0;
		switchToApp(getAppName(indexOfNextApp));
	}

	void ApplicationManager::callAllApps_initOnce(void)
	{
		Log* pLog = Log::getPointer();
		std::string strLog = "ApplicationManager::callAllApps_initOnce() called.";
		Log::getPointer()->add(strLog);

		std::map<std::string, ApplicationBase*>::iterator it = mApplications.begin();
		while (it != mApplications.end())
		{
			it->second->initOnce();
			it++;
		}
	}

	bool ApplicationManager::callCurrentApp_onUpdate(void)
	{
		mTimer.update();	// For application runtime
		std::map<std::string, ApplicationBase*>::iterator it = mApplications.find(mstrCurrentApp);
		ThrowIfTrue(it == mApplications.end(), "ApplicationManager::callCurrentApp_onUpdate(" + mstrCurrentApp + " failed. Unable to find the named application");

		// Update application runtime
		it->second->mfApplicationSecondsRunning += mTimer.getSecondsPast();
		return it->second->onUpdate();
	}

	float ApplicationManager::getApplicationRuntime(const std::string& applicationName)
	{
		std::map<std::string, ApplicationBase*>::iterator it = mApplications.find(applicationName);
		ThrowIfTrue(it == mApplications.end(), "ApplicationManager::getApplicationRuntime(" + applicationName + " failed. Unable to find the named application");
		return it->second->mfApplicationSecondsRunning;
	}
}
