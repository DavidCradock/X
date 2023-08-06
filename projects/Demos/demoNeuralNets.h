#pragma once
#include "PCH.h"

namespace X
{
	class CStateDemoNeuralNets : public CFiniteStateBase
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

		CTimer timer;
		CFiniteStateMachine _mFSM;	// Finite state machine to hold each of the application's states
	};
}