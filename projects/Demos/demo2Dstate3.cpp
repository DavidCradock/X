#include "PCH.h"
#include "demo2Dstate3.h"

namespace X
{
	void CDemo2DState3::onEnter(void)
	{
		// Create state window
		SCGUIManager* pGUI = SCGUIManager::getPointer();
		CGUIContainer* pCont = pGUI->addContainer("state3");
		pCont->setDimensions(640, 240);
		pCont->setPosition(0, 0);
		pCont->mstrTitleText = "State two.";
		// Add text scroll
		std::string strTxt;
		strTxt += "Welcome to state 3.\n";
		strTxt += "This state shows ?\n";
		strTxt += "Controls...\n";
		strTxt += "Details...\n";
		pCont->addTextScroll("TextScroll3", 0, 0, 640, 240, strTxt);
	}

	void CDemo2DState3::onExit(void)
	{
		SCGUIManager* pGUI = SCGUIManager::getPointer();
		pGUI->removeContainer("state3");
	}

	void CDemo2DState3::onActive(CFiniteStateMachine* pFSM)
	{

	}
}