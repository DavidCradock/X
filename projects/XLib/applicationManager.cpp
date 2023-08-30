#include "PCH.h"
#include "applicationManager.h"
#include "stringUtils.h"
#include "singletons.h"

// Include each application


namespace X
{
	SCApplicationManager::SCApplicationManager()
	{
		SCLog::getPointer()->add("SCApplicationManager::SCApplicationManager() called.");

		// Debug grid
		_mbDebugGridShow = false;
		_mfDebugGridSpacing[0] = _mfDebugGridSpacing[1] = 24;
		_mfDebugGridTextAlpha = 0;
	}

	void SCApplicationManager::mainLoop(void)
	{
		try
		{
			// Get a pointer to the global x SCSingletons class object to initialise all the singleton classes in the correct order
			x = SCSingletons::getPointer();

			// Here's the program's main loop called from WinMain.
			x->pLog->add("SCApplicationManager::mainLoop() called.");

			// Create objects to each of the application classes which inherit from CApplicationBase
//			ApplicationGUIThemeEditor* pAppGUIThemeEditor = new ApplicationGUIThemeEditor;
//			ThrowIfTrue(0 == pAppGUIThemeEditor, "SCApplicationManager::mainLoop() failed to allocate ApplicationGUIThemeEditor.");

			// Add each application to this manager
//			addApp("GUIThemeEditor", pAppGUIThemeEditor);

			// Set backbuffer to size of window
			CResourceFramebuffer* pBGFB = x->pResource->getFramebuffer("X:backbuffer");
			pBGFB->resizeToWindowDimsScaled();

			// Create default UI theme
			CUITheme* pTheme = x->pUI->themeAdd("default", true);
			pTheme->addAllResources(true);

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

				// Bind the X:backbuffer to render target
				pBGFB->bindAsRenderTarget(true, true);	// Both clear and resize to window dims

				if (!callCurrentApp_onUpdate())
				{
					break;	// Application wants to close
				}

				// Unbind the X:backbuffer and render to the back buffer
				pBGFB->unbindAsRenderTarget();

				// Update and render the 2DRenderer to the "X:backbuffer" and any various other framebuffers each of it's cameras are set to render to
				x->p2dRenderer->render();

				// Update and render the GUI to the "X:gui" framebuffer, using the "X:backbuffer" as the sample source
				x->pGUI->render("X:backbuffer");

				// Update and render the UI to the "X:ui" framebuffer
				x->pUI->render();

				// Now render the "X:backbuffer" to the backbuffer
				glEnable(GL_BLEND);
				glDisable(GL_DEPTH_TEST);
				x->pResource->getFramebuffer(x->pResource->defaultRes.framebuffer_backbuffer_FB)->renderTo2DQuad(0, 0, x->pWindow->getWidth(), x->pWindow->getHeight());
				
				// Now render the "X:gui" to the backbuffer
				x->pResource->getFramebuffer(x->pResource->defaultRes.framebuffer_gui)->renderTo2DQuad(0, 0, x->pWindow->getWidth(), x->pWindow->getHeight());

				// Now render the "X::ui" framebuffer to the backbuffer
				x->pResource->getFramebuffer(x->pResource->defaultRes.framebuffer_ui)->renderTo2DQuad(0, 0, x->pWindow->getWidth(), x->pWindow->getHeight());

				glDisable(GL_BLEND);

				// Render debug grid
				if (_mbDebugGridShow)
					_debugGridRender();

				// Swap buffers
				x->pWindow->swapBuffers();
			}

			// Last current app's onStop method
			x->pLog->add("SCApplicationManager::mainLoop() is shutting down...");
			it = _mApplications.find(_mstrCurrentApp);
			ThrowIfTrue(it == _mApplications.end(), "Unable to find the current application called " + _mstrCurrentApp);
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

	void SCApplicationManager::shutdown(void)
	{
		PostQuitMessage(0);
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

	void SCApplicationManager::onWindowToggleFullscreen(bool bFullscreen, int iWindowWidth, int iWindowHeight)
	{
		std::map<std::string, CApplicationBase*>::iterator it = _mApplications.begin();
		while (it != _mApplications.end())
		{
			it->second->onWindowToggleFullscreen(bFullscreen, iWindowWidth, iWindowHeight);
			it++;
		}
	}

	void SCApplicationManager::debugShowGrid(bool bShowGrid, int iSpacingX, int iSpacingY)
	{
		_mbDebugGridShow = bShowGrid;
		if (iSpacingX < 2)
			iSpacingX = 2;
		if (iSpacingY < 2)
			iSpacingY = 2;
		_mfDebugGridSpacing[0] = float(iSpacingX);
		_mfDebugGridSpacing[1] = float(iSpacingY);
		_mfDebugGridTextAlpha = 1.0f;
	}

	bool SCApplicationManager::debugGridShown(void) const
	{
		return _mbDebugGridShow;
	}

	CVector2f SCApplicationManager::debugGridSpacing(void) const
	{
		CVector2f vSpacing;
		vSpacing.x = float(_mfDebugGridSpacing[0]);
		vSpacing.y = float(_mfDebugGridSpacing[1]);
		return vSpacing;
	}

	void SCApplicationManager::_debugGridRender(void)
	{
		if (!_mbDebugGridShow)
			return;

		// Obtain required resources needed to render the node's as lines.
		CResourceVertexBufferLine* pLine = x->pResource->getVertexBufferLine(x->pResource->defaultRes.vertexbufferLine_default);
		CResourceShader* pShader = x->pResource->getShader(x->pResource->defaultRes.shader_VBCPT);
		CResourceTexture2DFromFile* pTexture = x->pResource->getTexture2DFromFile(x->pResource->defaultRes.texture2DFromFile_default_white);

		// Setup orthographic projection matrix
		CMatrix matrixWorld;
		CMatrix matrixView;
		CMatrix matrixProjection;
		matrixProjection.setProjectionOrthographic(0.0f, x->pWindow->getDimensions().x, 0.0f, x->pWindow->getDimensions().y);

		// Bind shader and set uniforms
		pShader->bind();
		pShader->setMat4("matrixWorld", matrixWorld);
		pShader->setMat4("matrixView", matrixView);
		pShader->setMat4("matrixProjection", matrixProjection);

		// Tell OpenGL, for each sampler, to which texture unit it belongs to
		pShader->setInt("texture0", 0);

		// And bind the texture...
		pTexture->bind();

		// Set the line vertex buffer rendering mode
		pLine->setDrawModeAsLineList();
		pLine->removeGeom();

		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Setup vertex we'll be using to render with
		CResourceVertexBufferLine::Vertex vertex;
		vertex.colour.set(1.0f, 1.0f, 1.0f, 0.2f);
		for (float fX = 0; fX < x->pWindow->getDimensions().x; fX += _mfDebugGridSpacing[0])
		{
			// Vertical line going from top to bottom
			vertex.position.x = fX;
			vertex.position.y = 0.0f;
			pLine->addLinePoint(vertex);

			vertex.position.y = x->pWindow->getDimensions().y;
			pLine->addLinePoint(vertex);
		}
		for (float fY = 0; fY < x->pWindow->getDimensions().y; fY += _mfDebugGridSpacing[1])
		{
			// Horizontal line going from left to right
			vertex.position.x = 0.0f;
			vertex.position.y = fY;
			pLine->addLinePoint(vertex);

			vertex.position.x = x->pWindow->getDimensions().x;
			pLine->addLinePoint(vertex);
		}

		// Send geometry to be rendered
		pLine->update();
		pLine->render();

		// Cleanup
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		pTexture->unbind();
		pShader->unbind();

		// Render text to show debug grid settings
		_mfDebugGridTextAlpha -= _mTimer.getSecondsPast() * 1000.0f;
		static float temp = 0;

		if (_mfDebugGridTextAlpha < 0.0f)
			_mfDebugGridTextAlpha = 0.0f;
		if (_mfDebugGridTextAlpha > 0.0f)
		{
			CResourceFont* pFont = x->pResource->getFont(x->pResource->defaultRes.font_default);
			std::string strText = "Grid settings.";
			CVector2f vScreenDims = x->pWindow->getDimensions();
			CVector2f vScreenTextPos = x->pWindow->getDimensions() * 0.5f;
			pFont->printCentered(strText, (int)vScreenTextPos.x, (int)vScreenTextPos.y, (int)vScreenDims.x, (int)vScreenDims.y, 1.0f, CColour(1.0f, 1.0f, 1.0f, _mfDebugGridTextAlpha));
			vScreenTextPos.y += pFont->getTextHeight();
			strText = "Spacing X and Y: " + std::to_string(int(_mfDebugGridSpacing[0]));
			pFont->printCentered(strText, (int)vScreenTextPos.x, (int)vScreenTextPos.y, (int)vScreenDims.x, (int)vScreenDims.y, 1.0f, CColour(1.0f, 1.0f, 1.0f, _mfDebugGridTextAlpha));

			
		}
	}
}
