#include "PCH.h"
#include "applicationBase.h"
#include "applicationManager.h"

namespace X
{
	ApplicationBase::ApplicationBase()
	{
		mfApplicationSecondsRunning = 0.0f;
	}

	void ApplicationBase::initOnce(void)
	{
	}

	void ApplicationBase::onStart(void)
	{
	}

	void ApplicationBase::onStop(void)
	{
	}
}