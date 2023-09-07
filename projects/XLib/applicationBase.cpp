#include "PCH.h"
#include "applicationBase.h"
#include "applicationManager.h"

namespace X
{
	CApplicationBase::CApplicationBase()
	{
		mfApplicationSecondsRunning = 0.0f;
	}

	void CApplicationBase::initOnce(void)
	{
	}

	void CApplicationBase::onStart(void)
	{
	}

	void CApplicationBase::onStop(void)
	{
	}

	void CApplicationBase::onWindowToggleFullscreen(bool bFullscreen, int iWindowWidth, int iWindowHeight)
	{
	}

	void CApplicationBase::onFrameEnd(void)
	{

	}
}