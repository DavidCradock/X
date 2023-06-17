#pragma once
#include "application.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
	X::Application* pApplication = new X::Application;
	X::ApplicationManager* pAppManager = X::ApplicationManager::getPointer();
	pAppManager->addApp("AppDevelopment", pApplication);
	pAppManager->mainLoop();
	return 0;
}