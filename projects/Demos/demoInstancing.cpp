#include "PCH.h"
#include "demoInstancing.h"

namespace X
{
	void CStateDemoInstancing::onEnter(void)
	{
		// Add container which enables toggling between instanced and non-instanced rendering
		CGUIContainer* pCont = x->pGUI->addContainer("toggleInstanced");
		pCont->setDimensions(320, 250);
		std::string strTxt;
		strTxt += "Toggle between instanced and non-instanced rendering with the button below.\n";
		strTxt += "Press F3 to show the statistics window and F2 to toggle VSync on or off.";
		pCont->addTextScroll("instanceText", 0, 0, 320, 170, strTxt);
		pCont->addButton("toggleInstanced", 0, 180, 320, 40, "Toggle Instanced")->mpTooltip->setAsText("Toggle beween instanced and non-instanced rendering here.");
		pCont->addText("instancingMode", 0, 230, "Instancing is ON.");
		_mbInstancedEnabled = true;
		
		// Create positions of each of the objects
		CVector2f vWndDims = x->pWindow->getDimensions() * 0.5f;
		CVector2f vPos2f;
		CVector3f vPos3f;
		for (int i = 0; i < 50000; i++)	// Determine number of objects here!
		{
			vPos2f = CVector2f(randf(-vWndDims.x, vWndDims.x), randf(-vWndDims.y, vWndDims.y));
			// Compute random values and store in the vector for non-indexed
			_mvecPosition.push_back(vPos2f);

			// Copy the non-indexed position and placed in the vector for indexed
			vPos3f.set(vPos2f.x, vPos2f.y, 0.0f);
			_mvecPositionIndex.push_back(vPos3f);

			// Compute random velocity
			_mvecVelocity.push_back(CVector2f(randf(-100.0f, 100.0f), randf(-100.0f, 100.0f)));
		}

		// Add a texture for use when rendering the quads
		x->pResource->addTexture2DFromFile("particle", "data/X/textures/particles/particle0.png");
	}

	void CStateDemoInstancing::onExit(void)
	{
		x->pGUI->removeContainer("toggleInstanced");
		x->pResource->removeTexture2DFromFile("particle");
	}

	void CStateDemoInstancing::onActive(CFiniteStateMachine* pFSM)
	{
		// Toggle instancing
		CGUIContainer* pCont = x->pGUI->getContainer("toggleInstanced");
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

		// Regardless of rendering mode, update position of all objects
		_mTimer.update();
		CVector2f vWndDims = x->pWindow->getDimensions() * 0.5f;
		for (int i = 0; i < (int)_mvecPosition.size(); i++)
		{
//			break;
			// Update position
			_mvecPosition[i] += _mvecVelocity[i] * _mTimer.getSecondsPast();
			
			// Wrap positions
			if (_mvecPosition[i].x < -vWndDims.x)
				_mvecPosition[i].x = vWndDims.x;
			else if (_mvecPosition[i].x > vWndDims.x)
				_mvecPosition[i].x = -vWndDims.x;
			if (_mvecPosition[i].y < -vWndDims.y)
				_mvecPosition[i].y = vWndDims.y;
			else if (_mvecPosition[i].y > vWndDims.y)
				_mvecPosition[i].y = -vWndDims.y;

			_mvecPositionIndex[i].x = _mvecPosition[i].x;
			_mvecPositionIndex[i].y = _mvecPosition[i].y;
		}

		// Render based upon whether instancing is enabled or not

		// Enable blending and disable depth testing
		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);

		// Obtain and bind texture
		CResourceTexture2DFromFile* pTexture = x->pResource->getTexture2DFromFile("particle");
		pTexture->bind(0);
		
		// Create matrices
		CMatrix matView, matWorld, matProj;
		matView.setTranslation(x->pWindow->getWidth() * 0.5f, x->pWindow->getHeight() * 0.5f, 0.0f);// Move view to centre of screen
		matProj.setProjectionOrthographic();

		CResourceShader* pShader;
		if (_mbInstancedEnabled)
		{
			// Setup instanced geometry 
			CResourceVertexBufferCPTInst* pVBI = x->pResource->getVertexBufferCPTInst("X:default");
			pVBI->removeAll();
			pVBI->addQuad2D(CVector2f(0, 0), CVector2f(10, 10));

			// Add each instance matrix for each object
			CMatrix matInstance;
			for (int i = 0; i < (int)_mvecPositionIndex.size(); i++)
			{
//				matInstance.setIdentity();	// No need to set this each time as setTranslation only changes the 3 translation values in the matrix.
				matInstance.setTranslation(_mvecPositionIndex[i]);
				pVBI->addInstanceMatrix(matInstance);
			}
			pVBI->update();

			// Bind and setup shader
			pShader = x->pResource->getShader("X:VBCPTInst");
			pShader->bind();
			pShader->setInt("texture0", 0);
			pShader->setMat4("matrixView", matView);
			pShader->setMat4("matrixProjection", matProj);

			pVBI->render();
		}
		else
		{
			// Setup non-instanced geometry 
			CResourceVertexBufferCPT* pVB = x->pResource->getVertexBufferCPT("X:default");
			pVB->removeGeom();
			// Add each quad for each object
			for (int i = 0; i < (int)_mvecPosition.size(); i++)
			{
				pVB->addQuad2D(_mvecPosition[i], CVector2f(10, 10));
			}
			pVB->update();

			// Bind and setup shader
			pShader = x->pResource->getShader("X:VBCPT");
			pShader->bind();
			pShader->setInt("texture0", 0);
			pShader->setMat4("matrixWorld", matWorld);
			pShader->setMat4("matrixView", matView);
			pShader->setMat4("matrixProjection", matProj);

			pVB->render();
		}

		// Unbind stuff
		pShader->unbind();
		pTexture->unbind(0);
	}
}