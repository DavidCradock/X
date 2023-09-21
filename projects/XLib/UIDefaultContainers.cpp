#include "PCH.h"
#include "UIDefaultContainers.h"
#include "UIWindow.h"
#include "singletons.h"
#include "stringUtils.h"
#include "UITooltip.h"

namespace X
{
	CUIDefaultContainers::CUIDefaultContainers()
	{
		_mbInitialised = false;
		// Set names of each default container
		names.console = "X:Default:Console";
		names.fontGenerator = "X:Default:FontGenerator";
		names.imageEditor = "X:Default:ImageEditor";
		names.profiling = "X:Default:Profiling";
		names.settings = "X:Default:Settings";
		names.statistics = "X:Default:Statistics";
		names.uiThemeEditor = "X:Default:UIThemeEditor";
		names.uiThemeEditorUIWidgetExamples = "X:Default:UIThemeEditorWidgetExamples";
	}

	void CUIDefaultContainers::initialise(void)
	{
		if (_mbInitialised)
			return;
		_mbInitialised = true;

		// Call each default container's init method
		_initConsole();
		_initFontGenerator();
		_initImageEditor();
		_initProfiling();
		_initSettings();
		_initStatistics();
		_initUIThemeEditor();
		_initUIThemeEditorWidgetExamples();
	}

	void CUIDefaultContainers::update(float fTimeDeltaSecs)
	{
		// Call each default container's update method
		_updateConsole(fTimeDeltaSecs);
		_updateFontGenerator(fTimeDeltaSecs);
		_updateImageEditor(fTimeDeltaSecs);
		_updateProfiling(fTimeDeltaSecs);
		_updateSettings(fTimeDeltaSecs);
		_updateStatistics(fTimeDeltaSecs);
		_updateUIThemeEditor(fTimeDeltaSecs);
		_updateUIThemeEditorWidgetExamples(fTimeDeltaSecs);
	}

	/************************************************************************************************************************************************************/
	/* Console */
	/************************************************************************************************************************************************************/

	void CUIDefaultContainers::_initConsole(void)
	{
		CUIContainer* pContainer = x->pUI->containerAdd(names.console, true);
		pContainer->setVisible(false);
		pContainer->setDimensions(x->pWindow->getDimensions());
		pContainer->setPosition(0, 0);

		pContainer->textAdd("Title", 0.0f, 0.0f, x->pWindow->getDimensions().x, 20.0f, "Console...");

	}

	void CUIDefaultContainers::_updateConsole(float fTimeDeltaSec)
	{
		// Is the container visible? If not, simply return
		CUIContainer* pContainer = x->pUI->containerGet(names.console);
		if (!pContainer->getVisible())
			return;

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
	/* Image editor */
	/************************************************************************************************************************************************************/

	void CUIDefaultContainers::_initImageEditor(void)
	{
		CUIWindow* pWindow = x->pUI->windowAdd(names.imageEditor, true);
		pWindow->setVisible(false);
		pWindow->setDimensions(320, 400);
		pWindow->setPosition(200, 200);

	}

	void CUIDefaultContainers::_updateImageEditor(float fTimeDeltaSec)
	{
		// Is the window visible? If not, simply return
		CUIWindow* pWindow = x->pUI->windowGet(names.imageEditor);
		if (!pWindow->getVisible())
			return;

	}

	/************************************************************************************************************************************************************/
	/* Profiling */
	/************************************************************************************************************************************************************/

	void CUIDefaultContainers::_initProfiling(void)
	{
		CUIWindow* pWindow = x->pUI->windowAdd(names.profiling, true);
		pWindow->setVisible(false);
		pWindow->setDimensions(900, 400);
		pWindow->setPosition(0.0f, 20.0f);
		pWindow->setToFrontAndInFocus();

		_mProfiling.fUpdateDelay = 0.0f;
		_mProfiling.fUpdateDelaySeconds = 1.0f;
		_mProfiling.bPaused = false;
		CUIButton* pButton = pWindow->buttonAdd("pause", 800 - 5, 0, 100, 24);
		CUIText* pText = pWindow->textAdd("paused", 700, 0, 100, 30, "Running");
		pText->setTextColour(false, CColour(0.0f, 1.0f, 0.0f, 1.0f));

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

			if (!_mProfiling.bPaused)
				bUpdateText = true;
		}

		if (pWindow->buttonGet("pause")->getClicked())
		{
			_mProfiling.bPaused = !_mProfiling.bPaused;
			CUIText* pText = pWindow->textGet("paused");
			if (_mProfiling.bPaused)
			{
				pText->setText("Paused");
				pText->setTextColour(false, CColour(1.0f, 0.0f, 0.0f, 1.0f));
			}
			else
			{
				pText->setText("Running");
				pText->setTextColour(false, CColour(0.0f, 1.0f, 0.0f, 1.0f));
			}
		}
		if (bUpdateText)
		{
			pWindow->textRemoveAll();
			pWindow->progressbarRemoveAll();

			CUIText* pText = pWindow->textAdd("paused", 700, 0, 100, 30, "");
			if (_mProfiling.bPaused)
			{
				pText->setText("Paused");
				pText->setTextColour(false, CColour(1.0f, 0.0f, 0.0f, 1.0f));
			}
			else
			{
				pText->setText("Running");
				pText->setTextColour(false, CColour(0.0f, 1.0f, 0.0f, 1.0f));
			}

			CVector2f vTextPos(0, 0);
			const CUITheme::SSettings* pThemeSettings = pWindow->themeGetSettings();
			CResourceFont* pFont = x->pResource->getFont(pThemeSettings->fonts.text);
			float fTextHeight = pFont->getTextHeight() + 3;
			CVector2f vWndDims = pWindow->getDimensions();
			std::vector<SProfilerResults> vResults = x->pProfiler->getResults();
			// Add title text
			float fColumnWidth[3];
			fColumnWidth[0] = pFont->getTextWidth("% of \"main\" ");
			fColumnWidth[1] = pFont->getTextWidth("Milliseconds ");
			fColumnWidth[2] = pFont->getTextWidth("Code Section Name ");

			pWindow->textAdd("TitlePercentMain", 0.0f, vTextPos.y, fColumnWidth[0], fTextHeight, "% of \"main\"");
			pWindow->textAdd("TitleMS", 140.0f, vTextPos.y, fColumnWidth[1], fTextHeight, "Milliseconds");
			pWindow->textAdd("TitleSectionName", 300.0f, vTextPos.y, fColumnWidth[2], fTextHeight, "Code Section Name");
			vTextPos.y += fTextHeight;
			for (size_t i = 0; i < vResults.size(); i++)
			{
				// Percent of main
				std::string strTextName = "PercentMain:" + std::to_string(i);
				std::string strLine;
				StringUtils::appendFloat(strLine, vResults[i].fPercentageOfMain, 1);
				strLine += "%";
				pWindow->textAdd(strTextName, 0.0f, vTextPos.y, vWndDims.x, fTextHeight, strLine);

				// Progress bar
				CUIProgressbar* pPB = pWindow->progressbarAdd(strTextName, 0.0f, vTextPos.y, 120.0f, 24.0f);
				pPB->setProgress(vResults[i].fPercentageOfMain * 0.01f);

				// Milliseconds
				strTextName = "Milliseconds:" + std::to_string(i);
				strLine.clear();
				StringUtils::appendDouble(strLine, vResults[i].dAccumulatedTimeSeconds * 1000, 3);
				strLine += "ms";
				pWindow->textAdd(strTextName, 140.0f, vTextPos.y, 160.0f, fTextHeight, strLine);

				// Section name
				// We need to compute how wide the text so we can add the text widget with that size.
				float fTextWidth = pFont->getTextWidth(vResults[i].strSectionName);
				strTextName = "SectionName:" + std::to_string(i);
				pWindow->textAdd(strTextName, 300.0f, vTextPos.y, fTextWidth + 10, fTextHeight, vResults[i].strSectionName);

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

		pWindow->setDimensions(450.0f, 330.0f);
		pWindow->setPosition(99999, 0);

		_mStatistics.fAddValueToLinegraphDataset = 0.0f;

		CUILineGraph* pLineGraph = pWindow->lineGraphAdd("Stats_FPS_Linegraph", 0, 0, 450, 120);
		
		CColour col;
		col.set(0.5f, 0.5f, 0.5f, 1.0f);	pLineGraph->addDataset("FPS", col);
		col.set(1.0f, 1.0f, 1.0f, 1.0f);	pLineGraph->addDataset("FPSAVR", col);

		float fTextHeight = x->pResource->getFont(pWindow->themeGetSettings()->fonts.text)->getTextHeight();
		float fYpos = 140.0f;
		pWindow->textAdd("Text_FPSMin", 0.0f, fYpos, pWindow->getDimensions().x, fTextHeight, "FPSMin: ");
		pWindow->textAdd("Text_FPS", 0.0f, fYpos, 200.0f, fTextHeight, "FPS: ");
		pWindow->textAdd("Text_FPSMax", 0.0f, fYpos, 100.0f, fTextHeight, "FPSMax: ");
		fYpos += fTextHeight;

		pWindow->textAdd("Text_FPSAVR", 0.0f, fYpos, pWindow->getDimensions().x, fTextHeight, "FPSAVR: ");
		fYpos += fTextHeight;

		// VSync enabled/disabled and desktop Hz
		std::string strTxt;
		_mStatistics.bvsyncEnabled = x->pWindow->getVSyncEnabled();
		if (x->pWindow->getVSyncEnabled())
		{
			strTxt = "VSync: On. Desktop Hz: ";
			StringUtils::appendUInt(strTxt, x->pWindow->getRefreshRate());
		}
		else
		{
			strTxt = "VSync: Off. Desktop Hz: ";
			StringUtils::appendUInt(strTxt, x->pWindow->getRefreshRate());
		}
		pWindow->textAdd("VSyncONOFF", 0.0f, fYpos, pWindow->getDimensions().x, fTextHeight, strTxt);	fYpos += fTextHeight;
		
		pWindow->textAdd("Text_Mem1", 0.0f, fYpos, pWindow->getDimensions().x, fTextHeight, "");	fYpos += fTextHeight;
		pWindow->textAdd("Text_Mem2", 0.0f, fYpos, pWindow->getDimensions().x, fTextHeight, "");	fYpos += fTextHeight;

		// CPU info
		pWindow->textAdd("CPU_info", 0.0f, fYpos, pWindow->getDimensions().x, fTextHeight, ""); fYpos += fTextHeight;

		// Close button
		fYpos += 10;
		CUIButton* pButton = pWindow->buttonAdd("Close", 225.0f - 50.0f, fYpos, 100.0f, 30.0f); fYpos += 30.0f;
		// Tooltip
//		pButton->pTooltip->setEnabled(true);
//		pButton->pTooltip->textAdd("tooltipText", 0, 0, 500, 100, "Click here to close this statistics window.");
//		pButton->mpTooltip->setAsText("Close window.");

	}

	void CUIDefaultContainers::_updateStatistics(float fTimeDeltaSec)
	{
		// Is the window visible? If not, simply return
		CUIWindow* pWindow = x->pUI->windowGet(names.statistics);
		if (!pWindow->getVisible())
			return;

		const CUITheme::SSettings* pThemeSettings = pWindow->themeGetSettings();
		CResourceFont* pFont = x->pResource->getFont(pThemeSettings->fonts.text);

		CUILineGraph* pLineGraph = pWindow->lineGraphGet("Stats_FPS_Linegraph");
		CUIText* pText;

		_mStatistics.timer.update();

		// Add new value to linegraph data set
		_mStatistics.fAddValueToLinegraphDataset += fTimeDeltaSec;
		if (_mStatistics.fAddValueToLinegraphDataset >= 0.1f)
		{
			_mStatistics.fAddValueToLinegraphDataset = 0.0f;
			CUILineGraphDataSet* pDataSetFPS = pLineGraph->getDataset("FPS");
			CUILineGraphDataSet* pDataSetFPSAVR = pLineGraph->getDataset("FPSAVR");
			pDataSetFPS->addValue(_mStatistics.timer.getFPS());
			pDataSetFPSAVR->addValue(_mStatistics.timer.getFPSAveraged());

			while (pDataSetFPS->getNumValues() > 30)
				pDataSetFPS->removeValue();
			while (pDataSetFPSAVR->getNumValues() > 30)
				pDataSetFPSAVR->removeValue();

			// Set min and max text
			float fMaxFPS = pDataSetFPS->getHighestValue();
			float fMaxFPSAVR = pDataSetFPSAVR->getHighestValue();
			float fMax = fMaxFPS;
			if (fMax < fMaxFPSAVR)
				fMax = fMaxFPSAVR;

			float fMinFPS = pDataSetFPS->getLowestValue();
			float fMinFPSAVR = pDataSetFPSAVR->getLowestValue();
			float fMin = fMinFPS;
			if (fMin < fMinFPSAVR)
				fMin = fMinFPSAVR;

			std::string str = "FPS Max: " + std::to_string((int)fMax);

			pText = pWindow->textGet("Text_FPSMax");
			pText->setText(str);

			CVector2f vNewPos = pText->getPosition();
			vNewPos.x = pWindow->getDimensions().x - pFont->getTextWidth(str)-20;
			pText->setPosition(vNewPos);
			pText->setDimensions(pFont->getTextWidth(str)+20, pFont->getTextHeight());

			str = "FPS Min: " + std::to_string((int)fMin);
			pText = pWindow->textGet("Text_FPSMin");
			pText->setText(str);

			// Update text values, "Text_FPSMax" and  "Text_FPSMin" have been updated above
			std::string strTxt;
			strTxt = "FPS: ";
			StringUtils::appendFloat(strTxt, _mStatistics.timer.getFPS(), 1);
			pText = pWindow->textGet("Text_FPS");
			pText->setText(strTxt);
			// Also set position
			vNewPos = pText->getPosition();
			vNewPos.x = 225 - (0.5f * pFont->getTextWidth(strTxt));
			pText->setPosition(vNewPos);


			strTxt = "FPS Average: ";
			StringUtils::appendFloat(strTxt, _mStatistics.timer.getFPSAveraged(), 1);
			strTxt += " (";
			StringUtils::appendFloat(strTxt, _mStatistics.timer.getFPSAveragedTimeUntilNextUpdate(), 1);
			strTxt += ")";
			pWindow->textGet("Text_FPSAVR")->setText(strTxt);
			// Also update colour based on whether it's above or below refresh rate
			CColour col(0.0f, 1.0f, 0.0f, 1.0f);
			if (_mStatistics.timer.getFPSAveraged() < (float)x->pWindow->getRefreshRate() - 1)
				col.set(1.0f, 0.0f, 0.0f, 1.0f);
			pWindow->textGet("Text_FPSAVR")->setTextColour(false, col);

			// VSync text
			if (_mStatistics.bvsyncEnabled != x->pWindow->getVSyncEnabled())
			{
				_mStatistics.bvsyncEnabled = x->pWindow->getVSyncEnabled();
				if (x->pWindow->getVSyncEnabled())
				{
					strTxt = "VSync: On. Desktop Hz: ";
					StringUtils::appendUInt(strTxt, x->pWindow->getRefreshRate());
					pWindow->textGet("VSyncONOFF")->setText(strTxt);
				}
				else
				{
					strTxt = "VSync: Off. Desktop Hz: ";
					StringUtils::appendUInt(strTxt, x->pWindow->getRefreshRate());
					pWindow->textGet("VSyncONOFF")->setText(strTxt);
				}
			}

			// CPU info
			strTxt = "CPU Logical Cores Count: ";
			StringUtils::appendInt(strTxt, getCPULogicalCores());
			pWindow->textGet("CPU_info")->setText(strTxt);

			// Memory text
			SMemInfo memInfo;
			getMemInfo(memInfo);
			pWindow->textGet("Text_Mem1")->setText("MemProcessWorkingSetSize: " + std::to_string(((memInfo.proc.iWorkingSetSize / 1024) / 1024)) + "MB");
			pWindow->textGet("Text_Mem2")->setText("MemProcessiPrivateUsage: " + std::to_string((((memInfo.proc.iPrivateUsage) / 1024) / 1024)) + "MB");
		}

		

		// Close button
		CUIButton* pButton = pWindow->buttonGet("Close");
		if (pButton->getClicked())
			pWindow->setVisible(false);
	}

	/************************************************************************************************************************************************************/
	/* UI Theme Editor */
	/************************************************************************************************************************************************************/

	void CUIDefaultContainers::_initUIThemeEditor(void)
	{
		CUIWindow* pWindow = x->pUI->windowAdd(names.uiThemeEditor, true);
		pWindow->setVisible(false);
		pWindow->setDimensions(640, 480);
	}

	void CUIDefaultContainers::_updateUIThemeEditor(float fTimeDeltaSec)
	{
		// Is the window visible? If not, simply return
		CUIWindow* pWindow = x->pUI->windowGet(names.uiThemeEditor);
		if (!pWindow->getVisible())
			return;
	}

	/************************************************************************************************************************************************************/
	/* UI Theme Editor Widget Examples */
	/************************************************************************************************************************************************************/

	void CUIDefaultContainers::_initUIThemeEditorWidgetExamples(void)
	{
		CUIWindow* pWindow = x->pUI->windowAdd(names.uiThemeEditorUIWidgetExamples, true);
		pWindow->setVisible(false);
		pWindow->setDimensions(500, 800);

		int iXpos = 0;
		int iYpos = 0;
		
		pWindow->textAdd("text0", iXpos, iYpos, 200, 40, "CUIButton: ");
		CUIButton* pButton = pWindow->buttonAdd("button0", iXpos + 200, iYpos, 100, 30);
		pButton->pTooltip->setText("Tooltip of a CUIButton");

		iYpos += 50;
		pWindow->textAdd("text1", iXpos, iYpos, 200, 40, "CUIButtonImage: ");
		CUIButtonImage* pBI = pWindow->buttonImageAdd("buttonImage0", iXpos + 200, iYpos, 100, 40,
			x->pResource->defaultRes.texture2DFromFile_default_white,
			x->pResource->defaultRes.texture2DFromFile_default_white,
			x->pResource->defaultRes.texture2DFromFile_default_white);
		pBI->pTooltip->setText("Tooltip of a CUIButtonImage");

		iYpos += 50;
		pWindow->textAdd("text2", iXpos, iYpos, 200, 40, "CUICheckbox: ");
		CUICheckbox* pCB = pWindow->checkboxAdd("Checkbox", 200, iYpos, 40, 40);
		pCB->pTooltip->setText("Tooltip of a CUICheckbox");

		iYpos += 50;
		pWindow->textAdd("text3", iXpos, iYpos, 200, 40, "CUIColourSelector: ");
		CUIColourSelector* pCS = pWindow->colourSelectorAdd("colourSelector", iXpos + 200, iYpos);

		iYpos += 355;
		pWindow->textAdd("text4", iXpos, iYpos, 200, 40, "CUILineGraph: ");
		CUILineGraph* pLG = pWindow->lineGraphAdd("linegraph", iXpos + 200, iYpos, 200, 100);
		CUILineGraphDataSet* pDataSet = pLG->addDataset("Dataset0", CColour(1.0f, 1.0f, 1.0f, 1.0f));
		for (float f = 0; f < 10; f += 0.1f)
			pDataSet->addValue((1.0f+sinf(f)) * 10.0f);
		pLG->pTooltip->setText("Tooltip of a CUILineGraph");

		iYpos += 100;
		pWindow->textAdd("text5", iXpos, iYpos, 200, 40, "CUIProgressbar: ");
		CUIProgressbar* pPB = pWindow->progressbarAdd("progressbar", iXpos + 200, iYpos, 200, 40);
		pPB->pTooltip->setText("Tooltip of a CUIProgressbar");

		iYpos += 40;
		pWindow->textAdd("text6", iXpos, iYpos, 200, 40, "CUIScrollbar: ");
		CUIScrollbar* pSB = pWindow->scrollbarAdd("scrollbar", iXpos + 200, iYpos, 200, 40);
		pSB->pTooltip->setText("Tooltip of a CUIScrollbar");

		iYpos += 40;
		pWindow->textAdd("text7", iXpos, iYpos, 200, 40, "CUIText: ");
		CUIText* pText = pWindow->textAdd("text8", iXpos + 200, iYpos, 200, 40, "CUIText Example.");
		pText->pTooltip->setText("Tooltip of a CUIText");

		iYpos += 40;
		pWindow->textAdd("text9", iXpos, iYpos, 200, 40, "CUITextEdit: ");
		CUITextEdit* pTE = pWindow->textEditAdd("textedit", iXpos + 200, iYpos, 200, 40, "TextEdit");
		pTE->pTooltip->setText("Tooltip of a CUITextEdit");
	}

	void CUIDefaultContainers::_updateUIThemeEditorWidgetExamples(float fTimeDeltaSec)
	{
		// Is the window visible? If not, simply return
		CUIWindow* pWindow = x->pUI->windowGet(names.uiThemeEditorUIWidgetExamples);
		if (!pWindow->getVisible())
			return;

	}
}