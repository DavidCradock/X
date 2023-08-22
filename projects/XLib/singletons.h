#pragma once
#include "PCH.h"
#include "singleton.h"
#include "2DRenderer.h"
#include "applicationManager.h"
#include "audioManager.h"
#include "GUIManager.h"
#include "input.h"
#include "logging.h"
#include "messaging.h"
#include "physicsManager.h"
#include "resourceLoadingScreen.h"
#include "resourceManager.h"
#include "window.h"
#include "settings.h"

namespace X
{
	// A class to get pointers to all singletons found throughout the codebase.
	// We have a global object available declared below which everything, if including this header,
	// can use to gain access to all singleton objects.
	// We have this class as it stores all global pointers in one place and also creates things in the
	// correct order as certain objects depend upon others being initialised.
	// 
	class SCSingletons : public CSingleton<SCSingletons>
	{
	public:
		// Obtains pointers to all singletons
		SCSingletons();

		// A pointer to the SC2DRenderer singleton class.
		SC2DRenderer* p2dRenderer;

		// A pointer to the SCApplicationManager singleton class.
		// Application manager class to handle all apps which are derived from CApplicationBase found in "applicationBase.h"
		SCApplicationManager* pAppMan;

		// A pointer to the SCAudioManager singleton class.
		// Playback of audio samples which are loaded from .wav files.
		SCAudioManager* pAudio;

		// A pointer to the SCGUIManager singleton class.
		// DEPRECATED
		SCGUIManager* pGUI;

		// A pointer to the SCInputManager singleton class.
		// This is responsible for obtaining input from various input devices such
		// as keyboards, mice, joysticks and steering wheels.
		SCInputManager* pInput;

		// A pointer to the SCLog singleton class.
		// Logging of information to "log.txt" file, storage of each added log entry into memory
		// and throwing of exceptions with the help of ThrowIfTrue() and ThrowIfFalse() macros.
		SCLog* pLog;

		// A pointer to the SCLoadingScreen singleton class.
		SCResourceLoadingScreen* pLoadingScreen;

		// A pointer to the SCMessageSystem singleton class.
		SCMessageSystem* pMessageSystem;

		// A pointer to the SCPhysicsManager singleton class.
		// NOT IMPLEMENTED
		SCPhysicsManager* pPhysics;

		// A pointer to the SCResourceManager singleton class.
		// This resource manager holds various resources which require an OpenGL context and is 
		// responsible for re-creating them if the context is lost.
		// You'll find all sorts of things in here such as textures, vertex buffers, framebuffers,
		// fonts and shaders.
		SCResourceManager* pResource;

		// A pointer to the SCSettings singleton class.
		// The SCSettings class which holds all global settings.
		// These settings are things which are usually accessed from a program's settings interface and include such things as..
		// Shadow map resolution, backbuffer resolution, global audio volumn and more.
		SCSettings* pSettings;

		// A pointer to the SCWindow singleton class.
		// Is responsible for creating/updating/shutting down the application's window.
		// We can get window dimensions, get/set V sync state, clear and swap the back buffer,
		// set mouse cursor images and more.
		SCWindow* pWindow;
	};

	// Global object used to get access to the pointers of all singleton objects.
	// It's first accessed via SCApplicationManager::mainLoop() at the top.
	extern SCSingletons* x;
}