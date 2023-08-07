#include "PCH.h"
#include "demoInstancing.h"

namespace X
{
	void CStateDemoInstancing::onEnter(void)
	{

	}

	void CStateDemoInstancing::onExit(void)
	{
		SCGUIManager* pGUI = SCGUIManager::getPointer();
	}

	void CStateDemoInstancing::onActive(CFiniteStateMachine* pFSM)
	{
		// Debug CResourceVertexBufferInstanced
		SCResourceManager* pRM = SCResourceManager::getPointer();
		CResourceVertexBufferInstanced* pVBI = pRM->getVertexBufferInstanced("X:default");
		CResourceShader* pShader = pRM->getShader("X:2DParticle");
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