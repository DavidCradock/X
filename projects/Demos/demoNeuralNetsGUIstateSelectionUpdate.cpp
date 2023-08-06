#include "PCH.h"
#include "DemoNeuralNetsGUIstateSelectionUpdate.h"

namespace X
{
	void DemoNNGUIstateSelectionUpdate(CFiniteStateMachine* pFSM)
	{
		SCGUIManager* pGUI = SCGUIManager::getPointer();
		CGUIContainer* pCont = pGUI->getContainer("Application state selection.");
		CGUIButton* pBut;
		
		pBut = pCont->getButton("state0");
		if (pBut->getClicked())
			pFSM->switchToState("state0");

		pBut = pCont->getButton("state1");
		if (pBut->getClicked())
			pFSM->switchToState("state1");

		pBut = pCont->getButton("state2");
		if (pBut->getClicked())
			pFSM->switchToState("state2");

		pBut = pCont->getButton("state3");
		if (pBut->getClicked())
			pFSM->switchToState("state3");
	}
}