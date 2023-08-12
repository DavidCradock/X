#pragma once
#include "PCH.h"

namespace X
{
	class CStateDemoSceneManager : public CFiniteStateBase
	{
	public:
		// This is called when this state is first entered.
		void onEnter(void);

		// This is called when this state is left.
		void onExit(void);

		// This is called whilst the state is active.
		// pFSM is a pointer to the CFiniteStateMachine this state is stored in.
		// This is usefull to allow the state to change to another named state by calling pFSM->switchToState()
		void onActive(CFiniteStateMachine* pFSM);

		// Set to true if there has been a fullscreen toggle.
		// Check this in onActive to reposition various things such as GUI containers
		bool bFullscreenToggle;

		CTimer timer;
		CSceneManagerSimple mSceneManagerSimple;	// Simple, no spatial partitioning scene manager
		CVector3f _mvEntityRot[100];
		void _initSceneManager(void);
	};
}