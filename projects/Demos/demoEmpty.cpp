#include "PCH.h"
#include "demoEmpty.h"

namespace X
{
	void CStateDemoEmpty::onEnter(void)
	{
	}

	void CStateDemoEmpty::onExit(void)
	{
	}

	void CStateDemoEmpty::onActive(CFiniteStateMachine* pFSM)
	{
		// Deal with fullscreen toggling
		if (bFullscreenToggle)
		{
			bFullscreenToggle = false;

		}
	}
}