#include "PCH.h"
#include "demoInstancing.h"

namespace X
{
	void CStateDemoInstancing::onEnter(void)
	{
		// Add container which enables toggling between instanced and non-instanced rendering
		SCGUIManager* pGUI = SCGUIManager::getPointer();
		CGUIContainer* pCont = pGUI->addContainer("toggleInstanced");
		pCont->setDimensions(320, 250);
		std::string strTxt;
		strTxt += "Toggle between instanced and non-instanced rendering with the button below.\n";
		strTxt += "Press F3 to show the statistics window and F2 to toggle VSync on or off.";
		pCont->addTextScroll("instanceText", 0, 0, 320, 170, strTxt);
		pCont->addButton("toggleInstanced", 0, 180, 320, 40, "Toggle Instanced")->mpTooltip->setAsText("Toggle beween instanced and non-instanced rendering here.");
		pCont->addText("instancingMode", 0, 230, "Instancing is ON.");
		_mbInstancedEnabled = true;
	}

	void CStateDemoInstancing::onExit(void)
	{
		SCGUIManager* pGUI = SCGUIManager::getPointer();
		pGUI->removeContainer("toggleInstanced");
	}

	void CStateDemoInstancing::onActive(CFiniteStateMachine* pFSM)
	{
		// Toggle instancing
		SCGUIManager* pGUI = SCGUIManager::getPointer();
		CGUIContainer* pCont = pGUI->getContainer("toggleInstanced");
		CGUIButton* pButton = pCont->getButton("toggleInstanced");
		if (pButton->getClicked())
		{
			_mbInstancedEnabled = !_mbInstancedEnabled;
			CGUIText* pText = pCont->getText("instancingMode");
			if (_mbInstancedEnabled)
				pText->setText("Instancing is ON.");
			else
				pText->setText("Instancing is OFF.");
		}

		// Debug CResourceVertexBufferInstanced
		SCResourceManager* pRM = SCResourceManager::getPointer();
		CResourceVertexBufferCPTInst* pVBI = pRM->getVertexBufferCPTInst("X:default");
		CResourceShader* pShader = pRM->getShader("X:VBCPTInst");
		CResourceTexture2DFromFile* pTexture = pRM->getTexture2DFromFile("X:default_white");
		pTexture->bind(0);
		CMatrix matView;
		// Move view to centre of screen
		SCWindow* pWindow = SCWindow::getPointer();
		matView.setTranslation(pWindow->getWidth() * 0.5f, pWindow->getHeight() * 0.5f, 0.0f);
		CMatrix matProj;	
		matProj.setProjectionOrthographic();
		pShader->bind();
		pShader->setInt("texture0", 0);
		pShader->setMat4("viewMatrix", matView);
		pShader->setMat4("projectionMatrix", matProj);
		pVBI->addQuad2D(CVector2f(0.0f, 0.0f), CVector2f(100, 100));

		// Now add instance data
		for (int i = 0; i < 100; i++)
		{
			//pVBI->addIn
		}
		pVBI->update();
		pVBI->render(false);
		pShader->unbind();
		pTexture->unbind(0);
	}
}