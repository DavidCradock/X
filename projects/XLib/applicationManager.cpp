#include "PCH.h"
#include "applicationManager.h"
#include "log.h"
#include "window.h"
#include "input.h"
#include "resourceManager.h"

// Include each application


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
//			ApplicationGUIThemeEditor* pAppGUIThemeEditor = new ApplicationGUIThemeEditor;
//			ThrowIfTrue(0 == pAppGUIThemeEditor, "ApplicationManager::mainLoop() failed to allocate ApplicationGUIThemeEditor.");

			// Add each application to this manager
//			addApp("GUIThemeEditor", pAppGUIThemeEditor);

			// Create window
			Window* pWindow = Window::getPointer();
			pWindow->createWindow("X - F1: Toggle fullscreen. F2: Toggle Vsync. F3: Toggle shadows.");
			
			// Get pointer to input manager
			InputManager* pInputManager = InputManager::getPointer();
			
			// Set mouse cursor to be located in the centre of the window
			pInputManager->mouse.setMousePos(pWindow->getWidth() / 2, pWindow->getHeight() / 2);

			// Add default resources used in various places
			pLog->add("ApplicationManager::mainLoop() adding default resources to ResourceManager.");
			addDefaultResources();

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

			ResourceManager* pRM = ResourceManager::getPointer();

			// Check window messages and if WM_QUIT occurs, end execution and shutdown
			while (pWindow->checkMessages())
			{
				mTimer.update();

				// Get the default scenemanager framebuffer and resize to size of window if needed
				ResourceFramebuffer *pFB = pRM->getFramebuffer("X:scenemanager");
				int iWindowDims[2];
				iWindowDims[0] = pWindow->getWidth();
				iWindowDims[1] = pWindow->getHeight();
				if ((int)pFB->getWidth() != iWindowDims[0] || (int)pFB->getHeight() != iWindowDims[1])
				{
					pFB->resize(iWindowDims[0], iWindowDims[1]);
				}

				// Clear the backbuffer
				pWindow->clearBackbuffer();

				if (!callCurrentApp_onUpdate())
				{
					break;	// Application wants to close
				}

				// Swap buffers
				pWindow->swapBuffers();
			}

			// Last current app's onStop method
			pLog->add("ApplicationManager::mainLoop() is shutting down...");
			it = mApplications.find(mstrCurrentApp);
			ThrowIfTrue(it == mApplications.end(), "unable to find the current application called " + mstrCurrentApp);
			it->second->onStop();

			// Now close the window
			pWindow->destroyWindow();
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

	void ApplicationManager::addDefaultResources(void)
	{
		// NOTE:
		// When adding resources here, remember to add the resource to the comments at the top of ResourceManager class in resourceManager.h

		ResourceManager* pRM = ResourceManager::getPointer();

		// Shaders
		pRM->addShader("X:font", "data/X/shaders/font.vert", "data/X/shaders/font.frag");
		pRM->addShader("X:line", "data/X/shaders/line.vert", "data/X/shaders/line.frag");
		pRM->addShader("X:DRNE", "data/X/shaders/DRNE.vert", "data/X/shaders/DRNE.frag");
		pRM->addShader("X:DRNE_noshadows", "data/X/shaders/DRNE_noshadows.vert", "data/X/shaders/DRNE_noshadows.frag");
		pRM->addShader("X:pos_col_tex", "data/X/shaders/pos_col_tex.vert", "data/X/shaders/pos_col_tex.frag");
		pRM->addShader("X:depthbuffer_debug", "data/X/shaders/depthbuffer_debug.vert", "data/X/shaders/depthbuffer_debug.frag");
		pRM->addShader("X:shadowdepthmap", "data/X/shaders/shadow_depthmap.vert", "data/X/shaders/shadow_depthmap.frag");
		pRM->addShader("X:gui", "data/X/shaders/gui.vert", "data/X/shaders/gui.frag");

		// Textures
		pRM->addTexture2D("X:default_particle", "data/X/textures/particle0.png");
		pRM->addTexture2D("X:default_white", "data/X/textures/default_white.png");
		pRM->addTexture2D("X:default_diffuse", "data/X/textures/default_diffuse.png");
		pRM->addTexture2D("X:default_emission", "data/X/textures/default_emission.png");
		pRM->addTexture2D("X:default_normal", "data/X/textures/default_normal.png");
		pRM->addTexture2D("X:default_roughness", "data/X/textures/default_roughness.png");

		// Depth buffers
		pRM->addDepthbuffer("X:shadows", 2048, 2048);

		// Triangle resources
		pRM->addTriangle("X:debug");
		pRM->addTriangle("X:gui");

		// Framebuffers
		pRM->addFramebuffer("X:scenemanager", 512, 512);	// Dims are set each program loop to match the window's dimensions
		pRM->addFramebuffer("X:gui", 512, 512);				// Dims are set each program loop to match the window's dimensions
	}
}
