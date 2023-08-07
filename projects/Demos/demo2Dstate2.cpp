#include "PCH.h"
#include "demo2Dstate2.h"

namespace X
{
	void CDemo2DState2::onEnter(void)
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
		strTxt += "This state shows particle systems in action.\n";
		strTxt += "Controls...\n";
		strTxt += "Details...\n";
		pCont->addTextScroll("TextScroll2", 0, 0, 640, 240, strTxt);

		// Add a particle system to the SC2DRenderer
		SC2DRenderer* p2D = SC2DRenderer::getPointer();
		C2DWorld* pWorld = p2D->addWorld("world");
		C2DCamera* pCamera = pWorld->addCamera("camera");
		SCWindow* pWindow = SCWindow::getPointer();
		CVector2f vWndDims = pWindow->getDimensions();
		CVector2f vCameraPosition(vWndDims.x * -0.5f, vWndDims.y * -0.5f);
		pCamera->setPosition(vCameraPosition);
		C2DLayer* pLayer = pWorld->addLayer("layer0");
		C2DParticleSystem* pParticleSystem = pLayer->addParticleSystem("particleSystem0");

	}

	void CDemo2DState2::onExit(void)
	{
		SCGUIManager* pGUI = SCGUIManager::getPointer();
		pGUI->removeContainer("state2");

		// Remove world
		SC2DRenderer::getPointer()->removeWorld("world");
	}

	void CDemo2DState2::onActive(CFiniteStateMachine* pFSM)
	{
		// Timer delta
		timer.update();

		SC2DRenderer* p2D = SC2DRenderer::getPointer();
		SCInputManager* pInput = SCInputManager::getPointer();

		// Rotate entities
		C2DWorld* pWorld = p2D->getWorld("world");
		C2DLayer* pLayer0 = pWorld->getLayer("layer0");

		// Now move camera
		C2DCamera* pCamera = pWorld->getCamera(0);
		CVector2f vCamPos = pCamera->getPosition();
		if (pInput->key.pressed(KC_UP))
			vCamPos.y -= timer.getSecondsPast() * 150.0f;
		if (pInput->key.pressed(KC_DOWN))
			vCamPos.y += timer.getSecondsPast() * 150.0f;
		if (pInput->key.pressed(KC_LEFT))
			vCamPos.x -= timer.getSecondsPast() * 150.0f;
		if (pInput->key.pressed(KC_RIGHT))
			vCamPos.x += timer.getSecondsPast() * 150.0f;
		pCamera->setPosition(vCamPos);

	
	}
}