#include "PCH.h"
#include "UIDefaultContainers.h"
#include "UIWindow.h"
#include "singletons.h"
#include "stringUtils.h"

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
		pWindow->setDimensions(800, 400);
		pWindow->setPositionCentre();
		pWindow->setToFrontAndInFocus();

		_mProfiling.fUpdateDelay = 0.0f;
		_mProfiling.fUpdateDelaySeconds = 1.0f;

	}

	void CUIDefaultContainers::_updateProfiling(float fTimeDeltaSec)
	{
		// Is the window visible? If not, simply return
		CUIWindow* pWindow = x->pUI->windowGet(names.profiling);
		if (!pWindow->getVisible())
			return;

		// Deal with update delay
		_mProfiling.fUpdateDelay -= fTimeDeltaSec;
		bool bUpdateText = false;
		if (_mProfiling.fUpdateDelay <= 0.0f)
		{
			_mProfiling.fUpdateDelay = _mProfiling.fUpdateDelaySeconds;
			bUpdateText = true;
		}

		if (bUpdateText)
		{
			pWindow->textRemoveAll();
			CVector2f vTextPos(0, 0);
			const CUITheme::SSettings* pThemeSettings = pWindow->themeGetSettings();
			CResourceFont* pFont = x->pResource->getFont(pThemeSettings->fonts.text);
			float fTextHeight = pFont->getTextHeight();
			CVector2f vWndDims = pWindow->getDimensions();
			std::vector<SProfilerResults> vResults = x->pProfiler->getResults();
			// Add title text
			pWindow->textAdd("TitlePercentMain", 0, vTextPos.y, 150, fTextHeight, "% of \"main\"");
			pWindow->textAdd("TitleMS", 140, vTextPos.y, 150, fTextHeight, "Milliseconds");
			pWindow->textAdd("TitleSectionName", 300, vTextPos.y, 150, fTextHeight, "Section");
			vTextPos.y += fTextHeight;
			for (size_t i = 0; i < vResults.size(); i++)
			{
				// Percent of main
				std::string strTextName = "PercentMain:" + std::to_string(i);
				std::string strLine;
				StringUtils::appendFloat(strLine, vResults[i].fPercentageOfMain, 1);
				strLine += "%";
				pWindow->textAdd(strTextName, 0, vTextPos.y, vWndDims.x, fTextHeight, strLine);

				// Milliseconds
				strTextName = "Milliseconds:" + std::to_string(i);
				strLine.clear();
				StringUtils::appendDouble(strLine, vResults[i].dAccumulatedTimeSeconds * 1000, 3);
				strLine += "ms";
				pWindow->textAdd(strTextName, 140, vTextPos.y, 160, fTextHeight, strLine);

				// Section name
				// We need to compute how wide the text so we can add the text widget with that size.
				float fTextWidth = pFont->getTextWidth(vResults[i].strSectionName);
				strTextName = "SectionName:" + std::to_string(i);
				pWindow->textAdd(strTextName, 300, vTextPos.y, fTextWidth + 10, fTextHeight, vResults[i].strSectionName);

				vTextPos.y += fTextHeight;
			}
		}
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