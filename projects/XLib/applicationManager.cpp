#include "PCH.h"
#include "applicationManager.h"
//#include "log.h"
//#include "window.h"
//#include "input.h"
//#include "resourceManager.h"
//#include "GUIManager.h"
//#include "2DRenderer.h"
#include "stringUtils.h"
//#include "resourceLoadingScreen.h"
//#include "settings.h"
#include "singletons.h"

// Include each application


namespace X
{
	SCApplicationManager::SCApplicationManager()
	{
		SCLog* pLog = SCLog::getPointer();
		pLog->add("SCApplicationManager::SCApplicationManager() called.");
	}

	void SCApplicationManager::mainLoop(void)
	{
		try
		{
			// Here's the program's main loop called from WinMain.

			// Get a pointer to the global x SCSingletons class object to initialise all the singleton classes in the correct order
			x = SCSingletons::getPointer();

			x->pLog->add("SCApplicationManager::mainLoop() called.");

			// Create objects to each of the application classes which inherit from CApplicationBase
//			ApplicationGUIThemeEditor* pAppGUIThemeEditor = new ApplicationGUIThemeEditor;
//			ThrowIfTrue(0 == pAppGUIThemeEditor, "SCApplicationManager::mainLoop() failed to allocate ApplicationGUIThemeEditor.");

			// Add each application to this manager
//			addApp("GUIThemeEditor", pAppGUIThemeEditor);

			CResourceFramebuffer* pBGFB = x->pResource->getFramebuffer("X:backbuffer_FB");

			// Create default GUI containers
			x->pGUI->_createDefaultContainers();

			// Now call each application's initOnce method
			callAllApps_initOnce();

			// Call currently set application's onStart method
			std::string strLog = "SCApplicationManager::mainLoop() calling " + _mstrCurrentApp + "'s onStart()";
			x->pLog->add(strLog);
			std::map<std::string, CApplicationBase*>::iterator it = _mApplications.find(_mstrCurrentApp);
			ThrowIfTrue(it == _mApplications.end(), "SCApplicationManager::mainLoop() unable to find the current application called " + _mstrCurrentApp);
			it->second->onStart();

			// Enter main loop...
			x->pLog->add("SCApplicationManager::mainLoop() is entering main loop...");
			_mTimer.update();
			
			// Check window messages and if WM_QUIT occurs, end execution and shutdown
			while (x->pWindow->checkMessages())
			{
				_mTimer.update();

				// Clear the backbuffer
				x->pWindow->clearBackbuffer();

				// Bind the X:backbuffer_FB to render target
				pBGFB->bindAsRenderTarget(true, true);	// Both clear and resize to window dims

				if (!callCurrentApp_onUpdate())
				{
					break;	// Application wants to close
				}

				// Update and render the 2DRenderer to the "X:backbuffer_FB" and any various other framebuffers each of it's cameras are set to render to
				x->p2dRenderer->render();

				// Update and render the GUI to the "X:backbuffer_FB" framebuffer, using the "X:backbuffer_FB" as the sample source
				x->pGUI->render("X:backbuffer_FB");

				// Unbind the X:backbuffer_FB and render to the back buffer
				pBGFB->unbindAsRenderTarget();
				
				// Now render the "X:backbuffer_FB" to the backbuffer
				glEnable(GL_BLEND);
				glDisable(GL_DEPTH_TEST);
				pBGFB->renderTo2DQuad(0, 0, x->pWindow->getWidth(), x->pWindow->getHeight());
				glDisable(GL_BLEND);

				// Swap buffers
				x->pWindow->swapBuffers();
			}

			// Last current app's onStop method
			x->pLog->add("SCApplicationManager::mainLoop() is shutting down...");
			it = _mApplications.find(_mstrCurrentApp);
			ThrowIfTrue(it == _mApplications.end(), "unable to find the current application called " + _mstrCurrentApp);
			it->second->onStop();

			// Now close the window
			x->pWindow->destroyWindow();
		}
		catch (CException &exception)
		{
			x->pLog->add("Total runtime: " + _mTimer.getClock());
			x->pLog->add(exception.mstrException, true);
			std::wstring strw = StringUtils::stringToWide(exception.mstrException);
			MessageBox(x->pWindow->getWindowHandle(), strw.c_str(), L"Sorry, an exception has been thrown...", MB_OK);
			__debugbreak();
		}
		x->pLog->add("Total runtime: " + _mTimer.getClock());
	}

	void SCApplicationManager::addApp(const std::string& applicationName, CApplicationBase* pTheApplication)
	{
		std::string strLog = "SCApplicationManager::addApp(" + applicationName + ") called.";
		SCLog::getPointer()->add(strLog);

		// Make sure it doesn't already exist
		std::map<std::string, CApplicationBase*>::iterator it = _mApplications.find(applicationName);
		ThrowIfTrue(it != _mApplications.end(), "SCApplicationManager::addApp(" + applicationName + " failed. The application name already exists");

		// If we get here, we can add it
		_mApplications[applicationName] = pTheApplication;

		// If no other applications have been added, set this one up to be the currently set application
		if (_mstrCurrentApp.length() == 0)
		{
			_mstrCurrentApp = applicationName;
		}
	}

	size_t SCApplicationManager::getNumApps(void) const
	{
		return _mApplications.size();
	}

	const std::string& SCApplicationManager::getAppName(unsigned int index) const
	{
		std::string strLog = "SCApplicationManager::getAppName() with index " + std::to_string(index) + " called.";
		x->pLog->add(strLog);

		ThrowIfTrue(index >= _mApplications.size(), "SCApplicationManager::getAppName(" + std::to_string(index) + ") failed. Invalid index given, maximum number of added applications is " + std::to_string(_mApplications.size()));

		std::map<std::string, CApplicationBase*>::iterator it = _mApplications.begin();
		for (unsigned int i = 0; i < index; ++i)
		{
			it++;
		}
		return it->first;
	}

	void SCApplicationManager::switchToApp(const std::string& applicationToSwitchTo)
	{
		std::string strLog = "SCApplicationManager::switchToApp(" + applicationToSwitchTo + " called.";
		x->pLog->add(strLog);

		// Attempt to find the application we're trying to switch to
		std::map<std::string, CApplicationBase*>::iterator itNewApp = _mApplications.find(applicationToSwitchTo);
		ThrowIfTrue(itNewApp == _mApplications.end(), "SCApplicationManager::switchToApp(\"" + applicationToSwitchTo + "\") failed. Application by given name doesn't exist.");

		// Attempt to find the application that's currently set as current
		std::map<std::string, CApplicationBase*>::iterator itOldApp = _mApplications.find(_mstrCurrentApp);
		ThrowIfTrue(itOldApp == _mApplications.end(), "SCApplicationManager::switchToApp(" + applicationToSwitchTo + " failed. Prior to changing to the new application, the old application called " + _mstrCurrentApp + " doesn't exist which is mental!");

		// Change current app name and call the various methods for each
		_mstrCurrentApp = applicationToSwitchTo;
		itOldApp->second->onStop();
		itNewApp->second->onStart();
	}

	void SCApplicationManager::switchToNextApp(void)
	{
		std::string strLog = "SCApplicationManager::SwitchToNextApp() called.";
		x->pLog->add(strLog);

		// If no apps are currently set
		ThrowIfTrue(0 == _mstrCurrentApp.size(), "SCApplicationManager::switchToNextApp() failed. There is no currently set application name.");

		// If only one app exists
		ThrowIfTrue(1 == _mApplications.size(), "SCApplicationManager::switchToNextApp() failed. There is only one application which exists.");

		// Find index of current application
		int indexOfCurrentApp = -1;
		for (int i = 0; i < getNumApps(); ++i)
		{
			if (_mstrCurrentApp == getAppName(i))
			{
				indexOfCurrentApp = i;
				break;
			}
		}
		ThrowIfTrue(-1 == indexOfCurrentApp, "SCApplicationManager::switchToNextApp() failed. Unable to find currently set application.");

		// Determine index of next app (wrap around)
		int indexOfNextApp = indexOfCurrentApp + 1;
		if (indexOfNextApp > _mApplications.size() - 1)
			indexOfNextApp = 0;
		switchToApp(getAppName(indexOfNextApp));
	}

	std::string SCApplicationManager::getCurrentAppName(void) const
	{
		return _mstrCurrentApp;
	}

	void SCApplicationManager::callAllApps_initOnce(void)
	{
		std::string strLog = "SCApplicationManager::callAllApps_initOnce() called.";
		x->pLog->add(strLog);

		std::map<std::string, CApplicationBase*>::iterator it = _mApplications.begin();
		while (it != _mApplications.end())
		{
			it->second->initOnce();
			it++;
		}
	}

	bool SCApplicationManager::callCurrentApp_onUpdate(void)
	{
		_mTimer.update();	// For application runtime
		std::map<std::string, CApplicationBase*>::iterator it = _mApplications.find(_mstrCurrentApp);
		ThrowIfTrue(it == _mApplications.end(), "SCApplicationManager::callCurrentApp_onUpdate(" + _mstrCurrentApp + " failed. Unable to find the named application");

		// Update application runtime
		it->second->mfApplicationSecondsRunning += _mTimer.getSecondsPast();
		return it->second->onUpdate();
	}

	float SCApplicationManager::getApplicationRuntime(const std::string& applicationName) const
	{
		std::map<std::string, CApplicationBase*>::iterator it = _mApplications.find(applicationName);
		ThrowIfTrue(it == _mApplications.end(), "SCApplicationManager::getApplicationRuntime(" + applicationName + " failed. Unable to find the named application");
		return it->second->mfApplicationSecondsRunning;
	}
}
