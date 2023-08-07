#pragma once
#include "PCH.h"

namespace X
{
	class CDemo2DState1 : public CFiniteStateBase
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

	private:
		CTimer timer;
		float mfEntityBodyRotationRadians[500];
		float mfEntityBodyRotationRadiansDir[500];

		float mfEntityTurretRotationRadians[500];
		float mfEntityTurretRotationRadiansDir[500];
		
	};
}