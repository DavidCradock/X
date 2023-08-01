#include "PCH.h"
#include "state2.h"

namespace X
{
	void CState2::onEnter(void)
	{
		// Create state window
		SCGUIManager* pGUI = SCGUIManager::getPointer();
		CGUIContainer* pCont = pGUI->addContainer("state2");
		pCont->setDimensions(640, 240);
		pCont->setPosition(0, 0);
		pCont->mstrTitleText = "State two.";
		// Add text scroll
		std::string strTxt;
		strTxt += "Welcome to state 2.\n";
		strTxt += "This state shows ?\n";
		strTxt += "Controls...\n";
		strTxt += "Details...\n";
		pCont->addTextScroll("TextScroll2", 0, 0, 640, 240, strTxt);
	}

	void CState2::onExit(void)
	{
		SCGUIManager* pGUI = SCGUIManager::getPointer();
		pGUI->removeContainer("state2");
	}

	void CState2::onActive(CFiniteStateMachine* pFSM)
	{

	}
}