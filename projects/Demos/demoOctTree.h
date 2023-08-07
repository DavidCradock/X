#pragma once
#include "PCH.h"

namespace X
{
	class CStateDemoOctTree : public CFiniteStateBase
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

		CSceneManagerSimple _mSM;
		COctTree _mOctTree;

		struct SEntity
		{
			CVector3f vPos;
			CVector3f vRot;
			std::string strName;
			CSMEntityLine* pEntLine;
			CSMEntityVertexBuffer* pEntTri;
		};
		std::vector<SEntity> _mvEntities;

		CFrustum _mTestFrustum;		// Test frustum
	};
}