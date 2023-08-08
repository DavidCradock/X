#pragma once
#include "PCH.h"

namespace X
{
	class CDemo2DState3 : public CFiniteStateBase
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
		CTimer _mTimer;
		std::vector<CVector2f> _mvEntityPosition;	// Each line entity's position
		std::vector<CVector2f> _mvEntityVelocity;	// Each line entity's velocity
		std::vector<float> _mvEntityRotationAmount;	// Each line entity's amount of rotation
	};
}