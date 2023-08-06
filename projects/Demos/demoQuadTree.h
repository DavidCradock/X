#pragma once
#include "PCH.h"

namespace X
{
	class CStateDemoQuadTree : public CFiniteStateBase
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
		CQuadTree* _mpQuadTree;
		unsigned int _muiNumEntities;
		std::vector<CVector2f> _mvecEntityPos;
		std::vector<CVector2f> _mvecEntityDir;
		std::vector<std::string> _mvecEntityName;
		CVector3f _mvCameraPosition;

		// The position which we get entities within range of
		CVector2f _mvRangePos;

		// The range from _mvRangePos which we get entities within range of
		float _mfRange;

		void _renderRangeFinder(void);
	};
}