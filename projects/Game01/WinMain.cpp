#include "application.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
	X::CApplication* pApplication = new X::CApplication;
	X::SCApplicationManager* pAppManager = X::SCApplicationManager::getPointer();
	pAppManager->addApp("App", pApplication);
	pAppManager->mainLoop();
	return 0;
}