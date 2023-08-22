#pragma once
#include "PCH.h"
#include "singletons.h"

namespace X
{
	// Global object used to get access to the pointers of all singleton objects.
	// It's first accessed via SCApplicationManager::mainLoop() at the top.
	SCSingletons* x;

	SCSingletons::SCSingletons()
	{
		pLog = SCLog::getPointer();
		pLog->clear();
		pLog->add("SCSingletons::SCSingletons() called. Initialising all singletons...");

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

		// GUI depends upon resource manager and therefore window
		pGUI = SCGUIManager::getPointer();

		pLoadingScreen = SCResourceLoadingScreen::getPointer();
	}
}