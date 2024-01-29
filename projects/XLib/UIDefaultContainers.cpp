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
		names.mapEditor = "X:Default:MapEditor";
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

		console.initialise();
		defaults.initialise();
		fontGenerator.initialise();
		imageEditor.initialise();
		mapEditor.initialise();
		profiling.initialise();
		settings.initialise();
		statistics.initialise();
		themeEditor.initialise();
	}

	void CUIDefaultContainers::update(float fTimeDeltaSecs)
	{
		console.update(fTimeDeltaSecs);
		defaults.update(fTimeDeltaSecs);
		fontGenerator.update(fTimeDeltaSecs);
		imageEditor.update(fTimeDeltaSecs);
		mapEditor.update(fTimeDeltaSecs);
		profiling.update(fTimeDeltaSecs);
		settings.update(fTimeDeltaSecs);
		statistics.update(fTimeDeltaSecs);
		themeEditor.update(fTimeDeltaSecs);
	}	
}