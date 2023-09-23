#include "PCH.h"
#include "UIDefConProfiling.h"
#include "UIManager.h"
#include "singletons.h"
#include "stringUtils.h"
#include "profiler.h"

namespace X
{
	CUIDefConProfiling::CUIDefConProfiling()
	{

	}

	void CUIDefConProfiling::initialise(void)
	{
		CUIDefaultContainers* pDefCons = x->pUI->getDefaultContainers();

		CUIWindow* pWindow = x->pUI->windowAdd(pDefCons->names.profiling, true);
		pWindow->setVisible(false);
		pWindow->setDimensions(900, 400);
		pWindow->setPosition(0.0f, 20.0f);
		pWindow->setToFrontAndInFocus();

		fUpdateDelay = 0.0f;
		fUpdateDelaySeconds = 1.0f;
		bPaused = false;
		CUIButton* pButton = pWindow->buttonAdd("pause", 800 - 5, 0, 100, 24);
		CUIText* pText = pWindow->textAdd("paused", 700, 0, 100, 30, "Running");
		pText->setTextColour(false, CColour(0.0f, 1.0f, 0.0f, 1.0f));
	}

	void CUIDefConProfiling::update(float fTimeDeltaSecs)
	{
		CUIDefaultContainers* pDefCons = x->pUI->getDefaultContainers();

		// Is the window visible? If not, simply return
		CUIWindow* pWindow = x->pUI->windowGet(pDefCons->names.profiling);
		if (!pWindow->getVisible())
			return;

		// Deal with update delay
		fUpdateDelay -= fTimeDeltaSecs;
		bool bUpdateText = false;
		if (fUpdateDelay <= 0.0f)
		{
			fUpdateDelay = fUpdateDelaySeconds;

			if (!bPaused)
				bUpdateText = true;
		}

		if (pWindow->buttonGet("pause")->getClicked())
		{
			bPaused = !bPaused;
			CUIText* pText = pWindow->textGet("paused");
			if (bPaused)
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
			if (bPaused)
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
}