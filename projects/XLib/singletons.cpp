#pragma once
#include "PCH.h"
#include "singletons.h"
#include "2DRenderer.h"
#include "applicationManager.h"
#include "audioManager.h"
#include "fileManager.h"
#include "GUIManager.h"
#include "input.h"
#include "logging.h"
#include "messaging.h"
#include "physicsManager.h"
#include "profiler.h"
#include "rendererManager.h"
#include "resourceLoadingScreen.h"
#include "resourceManager.h"
#include "settings.h"
#include "UIManager.h"
#include "window.h"

namespace X
{
	// Global object used to get access to the pointers of all singleton objects.
	// It's first accessed via SCApplicationManager::mainLoop() at the top.
	SCSingletons* x;

	SCSingletons::SCSingletons()
	{
		pLog = SCLog::getPointer();
		pLog->add("SCSingletons::SCSingletons() called. Initialising all singletons...");

		pFile = SCFileManager::getPointer();

		RendererManager* pRendererManager = new RendererManager;
		ThrowIfMemoryNotAllocated(pRendererManager);
		pRenderer = pRendererManager->initOpenGL();

		pMessageSystem = SCMessageSystem::getPointer();

		pSettings = SCSettings::getPointer();

		// Input depends upon there being a window
		pWindow = SCWindow::getPointer();
		pWindow->createWindow("X - F1: Toggle fullscreen. F2: Toggle Vsync. F3: Toggle shadows.");

		pInput = SCInputManager::getPointer();
		// Set mouse cursor to be located in the centre of the window
		pInput->mouse.setMousePos(pWindow->getWidth() / 2, pWindow->getHeight() / 2);

		// Resource depends upon there being a window
		pResource = SCResourceManager::getPointer();
		pResource->addDefaultResources();// Add default resources used in various places

		pAppMan = SCApplicationManager::getPointer();
		pAudio = SCAudioManager::getPointer();
		p2dRenderer = SC2DRenderer::getPointer();
		pPhysics = SCPhysicsManager::getPointer();

		pProfiler = new CProfiler;
		ThrowIfFalse(pProfiler, "SCSingletons::SCSingletons() failed to allocated memory for CProfiler object.");

		// GUI depends upon resource manager and therefore window
		pGUI = SCGUIManager::getPointer();

		// UI depends upon resource manager and therefore window
		pUI = SCUIManager::getPointer();

		pLoadingScreen = SCResourceLoadingScreen::getPointer();

		pLog->add("SCSingletons::SCSingletons() complete. All singletons initialised.");
	}
}