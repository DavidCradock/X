#include "PCH.h"
#include "UIDefaultContainers.h"
#include "UIWindow.h"
#include "singletons.h"

namespace X
{
	CUIDefaultContainers::CUIDefaultContainers()
	{
		_mbInitialised = false;
		// Set names of each default container
		names.fontGenerator = "X:Default:FontGenerator";
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

		// Call each default container's init method
		_initFontGenerator();
		_initProfiling();
		_initSettings();
		_initStatistics();
		_initUIThemeEditor();
	}

	void CUIDefaultContainers::update(float fTimeDeltaSecs)
	{
		// Call each default container's update method
		_updateFontGenerator(fTimeDeltaSecs);
		_updateProfiling(fTimeDeltaSecs);
		_updateSettings(fTimeDeltaSecs);
		_updateStatistics(fTimeDeltaSecs);
		_updateUIThemeEditor(fTimeDeltaSecs);
	}

	/************************************************************************************************************************************************************/
	/* Font generator */
	/************************************************************************************************************************************************************/

	void CUIDefaultContainers::_initFontGenerator(void)
	{
		CUIWindow* pWindow = x->pUI->windowAdd(names.fontGenerator, true);
		pWindow->setVisible(false);
	}

	void CUIDefaultContainers::_updateFontGenerator(float fTimeDeltaSec)
	{
		// Is the window visible? If not, simply return
		CUIWindow* pWindow = x->pUI->windowGet(names.fontGenerator);
		if (!pWindow->getVisible())
			return;

	}

	/************************************************************************************************************************************************************/
	/* Profiling */
	/************************************************************************************************************************************************************/

	void CUIDefaultContainers::_initProfiling(void)
	{
		CUIWindow* pWindow = x->pUI->windowAdd(names.profiling, true);
//		pWindow->setVisible(false);
		pWindow->setDimensions(600, 200);
	}

	void CUIDefaultContainers::_updateProfiling(float fTimeDeltaSec)
	{
		// Is the window visible? If not, simply return
		CUIWindow* pWindow = x->pUI->windowGet(names.profiling);
		if (!pWindow->getVisible())
			return;

	}

	/************************************************************************************************************************************************************/
	/* Settings */
	/************************************************************************************************************************************************************/

	void CUIDefaultContainers::_initSettings(void)
	{
		CUIWindow* pWindow = x->pUI->windowAdd(names.settings, true);
		pWindow->setVisible(false);
	}

	void CUIDefaultContainers::_updateSettings(float fTimeDeltaSec)
	{
		// Is the window visible? If not, simply return
		CUIWindow* pWindow = x->pUI->windowGet(names.settings);
		if (!pWindow->getVisible())
			return;

	}

	/************************************************************************************************************************************************************/
	/* Statistics */
	/************************************************************************************************************************************************************/

	void CUIDefaultContainers::_initStatistics(void)
	{
		CUIWindow* pWindow = x->pUI->windowAdd(names.statistics, true);
		pWindow->setVisible(false);
	}

	void CUIDefaultContainers::_updateStatistics(float fTimeDeltaSec)
	{
		// Is the window visible? If not, simply return
		CUIWindow* pWindow = x->pUI->windowGet(names.statistics);
		if (!pWindow->getVisible())
			return;

	}

	/************************************************************************************************************************************************************/
	/* UI Theme Editor */
	/************************************************************************************************************************************************************/

	void CUIDefaultContainers::_initUIThemeEditor(void)
	{
		CUIWindow* pWindow = x->pUI->windowAdd(names.uiThemeEditor, true);
		pWindow->setVisible(false);
	}

	void CUIDefaultContainers::_updateUIThemeEditor(float fTimeDeltaSec)
	{
		// Is the window visible? If not, simply return
		CUIWindow* pWindow = x->pUI->windowGet(names.uiThemeEditor);
		if (!pWindow->getVisible())
			return;

	}
}