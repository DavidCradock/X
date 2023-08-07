#pragma once
#include "PCH.h"

namespace X
{
	class CStateDemoInstancing : public CFiniteStateBase
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
		// Timing object
		CTimer _mTimer;

		// For toggling between the two rendering modes
		bool _mbInstancedEnabled;

		// Holds positions of all the objects being rendered
		std::vector<CVector2f> _mvecPosition;		// For the non-indexed vertex buffer
		std::vector<CVector3f> _mvecPositionIndex;	// For the indexed vertex buffer

		// Stores random velocity of all the objects being rendered
		std::vector<CVector2f> _mvecVelocity;
	};
}