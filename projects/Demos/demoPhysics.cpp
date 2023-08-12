#include "PCH.h"
#include "demoPhysics.h"

namespace X
{
	void CStateDemoPhysics::onEnter(void)
	{

	}

	void CStateDemoPhysics::onExit(void)
	{

	}

	void CStateDemoPhysics::onActive(CFiniteStateMachine* pFSM)
	{
		// Deal with fullscreen toggling
		if (bFullscreenToggle)
		{
			bFullscreenToggle = false;

		}
	}
}