#include "PCH.h"
#include "UIDefConDefaults.h"
#include "UIManager.h"
#include "singletons.h"
#include "UITooltip.h"

namespace X
{
	CUIDefConDefaults::CUIDefConDefaults()
	{

	}

	void CUIDefConDefaults::initialise(void)
	{
		CUIDefaultContainers* pDefCons = x->pUI->getDefaultContainers();
		CUIWindow* pWindow = x->pUI->windowAdd(pDefCons->names.defaults, true);
		pWindow->setVisible(true);
		pWindow->setDimensions(200, 310);
		pWindow->setPosition(99999, 99999);

		int iYpos = 0;
		CUIButton* pBut = pWindow->buttonAdd("Console", 0, iYpos, 200, 40); iYpos += 45;
		pBut->pTooltip->setText("Toggles the \"X:Default:Console\" default container.");

		pBut = pWindow->buttonAdd("FontGenerator", 0, iYpos, 200, 40); iYpos += 45;
		pBut->pTooltip->setText("Toggles the \"X:Default:FontGenerator\" default container.");

		pBut = pWindow->buttonAdd("ImageEditor", 0, iYpos, 200, 40); iYpos += 45;
		pBut->pTooltip->setText("Toggles the \"X:Default:ImageEditor\" default container.");

		pBut = pWindow->buttonAdd("Profiling", 0, iYpos, 200, 40); iYpos += 45;
		pBut->pTooltip->setText("Toggles the \"X:Default:Profiling\" default container.");

		pBut = pWindow->buttonAdd("Settings", 0, iYpos, 200, 40); iYpos += 45;
		pBut->pTooltip->setText("Toggles the \"X:Default:Settings\" default container.");

		pBut = pWindow->buttonAdd("Statistics", 0, iYpos, 200, 40); iYpos += 45;
		pBut->pTooltip->setText("Toggles the \"X:Default:Statistics\" default container.");

		pBut = pWindow->buttonAdd("UIThemeEditor", 0, iYpos, 200, 40); iYpos += 45;
		pBut->pTooltip->setText("Toggles the \"X:Default:UIThemeEditor\" default container.");
	}

	void CUIDefConDefaults::update(float fTimeDeltaSecs)
	{
		CUIDefaultContainers* pDefCons = x->pUI->getDefaultContainers();

		// Is the container visible? If not, simply return
		CUIWindow* pWindow = x->pUI->windowGet(pDefCons->names.defaults);
		if (!pWindow->getVisible())
			return;

		if (pWindow->buttonGet("Console")->getClicked())
		{
			CUIContainer* pCont = x->pUI->containerGet(pDefCons->names.console);
			pCont->setVisible(!pCont->getVisible());
			return;
		}

		if (pWindow->buttonGet("FontGenerator")->getClicked())
		{
			CUIWindow* pWnd = x->pUI->windowGet(pDefCons->names.fontGenerator);
			pWnd->setVisible(!pWnd->getVisible());
			if (pWnd->getVisible())
				pWnd->setToFrontAndInFocus();
			return;
		}

		if (pWindow->buttonGet("ImageEditor")->getClicked())
		{
			CUIWindow* pWnd = x->pUI->windowGet(pDefCons->names.imageEditor);
			pWnd->setVisible(!pWnd->getVisible());
			if (pWnd->getVisible())
				pWnd->setToFrontAndInFocus();
			return;
		}

		if (pWindow->buttonGet("Profiling")->getClicked())
		{
			CUIWindow* pWnd = x->pUI->windowGet(pDefCons->names.profiling);
			pWnd->setVisible(!pWnd->getVisible());
			if (pWnd->getVisible())
				pWnd->setToFrontAndInFocus();
			return;
		}

		if (pWindow->buttonGet("Settings")->getClicked())
		{
			CUIWindow* pWnd = x->pUI->windowGet(pDefCons->names.settings);
			pWnd->setVisible(!pWnd->getVisible());
			if (pWnd->getVisible())
				pWnd->setToFrontAndInFocus();
			return;
		}

		if (pWindow->buttonGet("Statistics")->getClicked())
		{
			CUIWindow* pWnd = x->pUI->windowGet(pDefCons->names.statistics);
			pWnd->setVisible(!pWnd->getVisible());
			if (pWnd->getVisible())
				pWnd->setToFrontAndInFocus();
			return;
		}

		if (pWindow->buttonGet("UIThemeEditor")->getClicked())
		{
			CUIWindow* pWnd = x->pUI->windowGet(pDefCons->names.uiThemeEditor);
			pWnd->setVisible(!pWnd->getVisible());
			if (pWnd->getVisible())
				pWnd->setToFrontAndInFocus();
		}
	}
}