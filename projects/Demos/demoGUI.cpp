#include "PCH.h"
#include "demoGUI.h"

namespace X
{
	void CStateDemoGUI::onEnter(void)
	{
		// Create GUI container which has buttons which show/hide the default containers
		CGUIContainer* pCont = x->pGUI->addContainer("demoGUI");
		pCont->setDimensions(320, 320);
		pCont->setPositionCentreWindow();
		std::string strTxt;
		strTxt = "Welcome to the GUI demo application.\n";
		strTxt += "This demo shows every feature of the Graphical User Interface.\n";
		strTxt += "Several GUI containers are created during initialisation and can be toggled visible or not with the buttons below.\n";
//		strTxt += "\n";
//		strTxt += "\n";
//		strTxt += "\n";
//		strTxt += "\n";
		CGUITextScroll* pTS = pCont->addTextScroll("demoGUITS", 0, 0, 320, 170, strTxt);
		pCont->addButton("Statistics", 0, 180, 320, 40, "Statistics.")->mpTooltip->setAsText("Click me to toggle the statistics window as visible or not.");
		pCont->addButton("FontGenerator", 0, 230, 320, 40, "Font Generator.")->mpTooltip->setAsText("Click me to toggle the font generator window as visible or not.");
		pCont->addButton("Settings", 0, 280, 320, 40, "Settings.")->mpTooltip->setAsText("Click me to toggle the settings window as visiable or not.");
	}

	void CStateDemoGUI::onExit(void)
	{
		x->pGUI->removeContainer("demoGUI");
	}

	void CStateDemoGUI::onActive(CFiniteStateMachine* pFSM)
	{
		CGUIContainer* pCont = x->pGUI->getContainer("demoGUI");
		CGUIButton* pBut = pCont->getButton("Statistics");
		if (pBut->getClicked())
		{
			CGUIContainer* pCont2 = x->pGUI->getContainer("X:Default:Statistics");
			if (pCont2->getVisible())
				pCont2->setVisible(false);
			else
				pCont2->setVisible(true);
		}
		pBut = pCont->getButton("FontGenerator");
		if (pBut->getClicked())
		{
			CGUIContainer* pCont2 = x->pGUI->getContainer("X:Default:FontGenerator");
			if (pCont2->getVisible())
				pCont2->setVisible(false);
			else
				pCont2->setVisible(true);
		}
		pBut = pCont->getButton("Settings");
		{
//			CGUIContainer* pCont2 = x->pGUI->getContainer("X:Default:SettingsNOTIMP");
//			if (pCont2->getVisible())
//				pCont2->setVisible(false);
//			else
//				pCont2->setVisible(true);
		}

	}
}