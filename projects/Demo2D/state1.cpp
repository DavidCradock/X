#include "PCH.h"
#include "state1.h"

namespace X
{
	void CState1::onEnter(void)
	{
		// Create state window
		SCGUIManager* pGUI = SCGUIManager::getPointer();
		CGUIContainer* pCont = pGUI->addContainer("state1");
		pCont->setDimensions(640, 240);
		pCont->setPosition(0, 0);
		pCont->mstrTitleText = "State one.";
		// Add text scroll
		std::string strTxt;
		strTxt += "Welcome to state 1.\n";
		strTxt += "This state shows a single world and a single camera with two layers\n";
		strTxt += "The lowest layer, the one that's rendered first and is therefore in the background, is an image of a tank's main body.\n";
		strTxt += "The next layer above that is an image of a tank's turret\n";
		strTxt += "Controls...\n";
		strTxt += "The cursor keys move the camera.\n";
		strTxt += "The WSAD keys move and rotate entity.\n \n";
		strTxt += "Details...\n";
		pCont->addTextScroll("TextScroll1", 0, 0, 640, 240, strTxt);
	}

	void CState1::onExit(void)
	{
		SCGUIManager* pGUI = SCGUIManager::getPointer();
		pGUI->removeContainer("state1");
	}

	void CState1::onActive(CFiniteStateMachine* pFSM)
	{

	}
}