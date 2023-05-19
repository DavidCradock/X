#pragma once
#include "PCH.h"
#include "applicationManager.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
	X::ApplicationManager::getPointer()->mainLoop();
	return 0;
}