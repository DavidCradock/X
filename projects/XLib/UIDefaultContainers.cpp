#include "PCH.h"
#include "UIDefaultContainers.h"
#include "UIWindow.h"
#include "singletons.h"
#include "stringUtils.h"
#include "UITooltip.h"
#include "UIManager.h"
#include "profiler.h"

namespace X
{
	CUIDefaultContainers::CUIDefaultContainers()
	{
		_mbInitialised = false;

		// Set names of each default container
		names.console = "X:Default:Console";
		names.defaults = "X:Default:Defaults";
		names.fontGenerator = "X:Default:FontGenerator";
		names.imageEditor = "X:Default:ImageEditor";
		names.profiling = "X:Default:Profiling";
		names.settings = "X:Default:Settings";
		names.statistics = "X:Default:Statistics";
		names.uiThemeEditor = "X:Default:UIThemeEditor";
	}

	void CUIDefaultContainers::initialise(void)
	{
		if (_mbInitialised)
			return;
		_mbInitialised = true;

		_mConsole.initialise();
		_mDefaults.initialise();
		_mFontGenerator.initialise();
		_mImageEditor.initialise();
		_mProfiling.initialise();
		_mSettings.initialise();
		_mStatistics.initialise();
		_mUIThemeEditor.initialise();
	}

	void CUIDefaultContainers::update(float fTimeDeltaSecs)
	{
		_mConsole.update(fTimeDeltaSecs);
		_mDefaults.update(fTimeDeltaSecs);
		_mFontGenerator.update(fTimeDeltaSecs);
		_mImageEditor.update(fTimeDeltaSecs);
		_mProfiling.update(fTimeDeltaSecs);
		_mSettings.update(fTimeDeltaSecs);
		_mStatistics.update(fTimeDeltaSecs);
		_mUIThemeEditor.update(fTimeDeltaSecs);
	}	
}