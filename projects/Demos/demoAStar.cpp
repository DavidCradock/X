#include "PCH.h"
#include "demoAStar.h"

namespace X
{
	void CStateDemoAStar::onEnter(void)
	{
	}

	void CStateDemoAStar::onExit(void)
	{
	}

	void CStateDemoAStar::onActive(CFiniteStateMachine* pFSM)
	{
		// Deal with fullscreen toggling
		if (bFullscreenToggle)
		{
			bFullscreenToggle = false;

		}
	}
}