#include "PCH.h"
#include "UIDefConStatistics.h"
#include "UIManager.h"
#include "singletons.h"
#include "stringUtils.h"

namespace X
{
	CUIDefConStatistics::CUIDefConStatistics()
	{

	}

	void CUIDefConStatistics::initialise(void)
	{
		CUIDefaultContainers* pDefCons = x->pUI->getDefaultContainers();

		CUIWindow* pWindow = x->pUI->windowAdd(pDefCons->names.statistics, true);
		pWindow->setVisible(false);

		pWindow->setDimensions(450.0f, 290.0f);
		pWindow->setPosition(99999, 0);

		fAddValueToLinegraphDataset = 0.0f;

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
		bvsyncEnabled = x->pWindow->getVSyncEnabled();
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
//		fYpos += 10;
//		CUIButton* pButton = pWindow->buttonAdd("Close", 225.0f - 50.0f, fYpos, 100.0f, 30.0f); fYpos += 30.0f;
		// Tooltip
//		pButton->pTooltip->setEnabled(true);
//		pButton->pTooltip->textAdd("tooltipText", 0, 0, 500, 100, "Click here to close this statistics window.");
//		pButton->mpTooltip->setAsText("Close window.");
	}

	void CUIDefConStatistics::update(float fTimeDeltaSecs)
	{
		CUIDefaultContainers* pDefCons = x->pUI->getDefaultContainers();

		// Is the window visible? If not, simply return
		CUIWindow* pWindow = x->pUI->windowGet(pDefCons->names.statistics);
		if (!pWindow->getVisible())
			return;

		const CUITheme::SSettings* pThemeSettings = pWindow->themeGetSettings();
		CResourceFont* pFont = x->pResource->getFont(pThemeSettings->fonts.text);

		CUILineGraph* pLineGraph = pWindow->lineGraphGet("Stats_FPS_Linegraph");
		CUIText* pText;

		timer.update();

		// Add new value to linegraph data set
		fAddValueToLinegraphDataset += fTimeDeltaSecs;
		if (fAddValueToLinegraphDataset >= 0.1f)
		{
			fAddValueToLinegraphDataset = 0.0f;
			CUILineGraphDataSet* pDataSetFPS = pLineGraph->getDataset("FPS");
			CUILineGraphDataSet* pDataSetFPSAVR = pLineGraph->getDataset("FPSAVR");
			pDataSetFPS->addValue(timer.getFPS());
			pDataSetFPSAVR->addValue(timer.getFPSAveraged());

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
			vNewPos.x = pWindow->getDimensions().x - pFont->getTextWidth(str) - 20;
			pText->setPosition(vNewPos);
			pText->setDimensions(pFont->getTextWidth(str) + 20, pFont->getTextHeight());

			str = "FPS Min: " + std::to_string((int)fMin);
			pText = pWindow->textGet("Text_FPSMin");
			pText->setText(str);

			// Update text values, "Text_FPSMax" and  "Text_FPSMin" have been updated above
			std::string strTxt;
			strTxt = "FPS: ";
			StringUtils::appendFloat(strTxt, timer.getFPS(), 1);
			pText = pWindow->textGet("Text_FPS");
			pText->setText(strTxt);
			// Also set position
			vNewPos = pText->getPosition();
			vNewPos.x = 225 - (0.5f * pFont->getTextWidth(strTxt));
			pText->setPosition(vNewPos);


			strTxt = "FPS Average: ";
			StringUtils::appendFloat(strTxt, timer.getFPSAveraged(), 1);
			strTxt += " (";
			StringUtils::appendFloat(strTxt, timer.getFPSAveragedTimeUntilNextUpdate(), 1);
			strTxt += ")";
			pWindow->textGet("Text_FPSAVR")->setText(strTxt);
			// Also update colour based on whether it's above or below refresh rate
			CColour col(0.0f, 1.0f, 0.0f, 1.0f);
			if (timer.getFPSAveraged() < (float)x->pWindow->getRefreshRate() - 1)
				col.set(1.0f, 0.0f, 0.0f, 1.0f);
			pWindow->textGet("Text_FPSAVR")->setTextColour(false, col);

			// VSync text
			if (bvsyncEnabled != x->pWindow->getVSyncEnabled())
			{
				bvsyncEnabled = x->pWindow->getVSyncEnabled();
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
	}
}