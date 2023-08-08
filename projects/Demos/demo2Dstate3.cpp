#include "PCH.h"
#include "demo2Dstate3.h"

namespace X
{
	void CDemo2DState3::onEnter(void)
	{
		// Create state window
		SCGUIManager* pGUI = SCGUIManager::getPointer();
		SCWindow* pWindow = SCWindow::getPointer();
		CGUIContainer* pCont = pGUI->addContainer("state3");
		pCont->setDimensions(640, 240);
		pCont->setPosition(0, 0);
		pCont->mstrTitleText = "State three.";
		// Add text scroll
		std::string strTxt;
		strTxt += "Welcome to state 3.\n";
		strTxt += "This state shows C2DEntityLine rendering.\n";
		strTxt += "Controls...\n";
		strTxt += "Cursor keys move the camera.\n";
		strTxt += " \nDetails...\n";
		pCont->addTextScroll("TextScroll3", 0, 0, 640, 240, strTxt);

		// Create entity's random position and velocity
		CVector2f vWndDims = pWindow->getDimensions();
		CVector2f vWndDimsHalf = vWndDims * 0.5f;
		// Set each entity's position and velocity
		for (int i = 0; i < 100; i++)
		{
			CVector2f vRandomPos(randf(-vWndDimsHalf.x, vWndDimsHalf.x), randf(-vWndDimsHalf.y, vWndDimsHalf.y));
			_mvEntityPosition.push_back(vRandomPos);
			CVector2f vRandomVel(randf(-100, 100), randf(-100, 100));
			_mvEntityVelocity.push_back(vRandomVel);

			_mvEntityRotationAmount.push_back(randf(-k2Pi, k2Pi));
		}

		// Setup 2D renderer
		SC2DRenderer* p2D = SC2DRenderer::getPointer();
		C2DWorld* pWorld = p2D->addWorld("world");
		C2DCamera* pCamera = pWorld->addCamera("camera");
		pCamera->setPosition(-vWndDimsHalf.x, -vWndDimsHalf.y);

		C2DLayer* pLayer = pWorld->addLayer("layer");
		for (int i = 0; i < _mvEntityPosition.size(); i++)
		{
			std::string strName = "Ent: " + std::to_string(i);
			C2DEntityLine* pEntity = pLayer->addEntityLine(strName);
			if (i % 2 == 0)
				pEntity->setAsCircle(randf(10, 20), randInt(5, 10));
			else
				pEntity->setAsQuad(randf(10, 20), randf(10, 20));
		}
		
	}

	void CDemo2DState3::onExit(void)
	{
		SCGUIManager* pGUI = SCGUIManager::getPointer();
		pGUI->removeContainer("state3");

		// Cleanup 2D renderer
		SC2DRenderer* p2D = SC2DRenderer::getPointer();
		p2D->removeWorld("world");
	}

	void CDemo2DState3::onActive(CFiniteStateMachine* pFSM)
	{
		SCInputManager* pInput = SCInputManager::getPointer();
		SCWindow* pWindow = SCWindow::getPointer();
		CVector2f vWndDims = pWindow->getDimensions();
		CVector2f vWndDimsHalf = vWndDims * 0.5f;

		_mTimer.update();
		float fTimeDelta = _mTimer.getSecondsPast();

		SC2DRenderer* p2D = SC2DRenderer::getPointer();
		C2DWorld* pWorld = p2D->getWorld("world");
		C2DCamera* pCamera = pWorld->getCamera("camera");
		C2DLayer* pLayer = pWorld->getLayer("layer");
		C2DEntityLine* pEntityLine;
		// Update and wrap entity positions based upon their velocity
		for (int i = 0; i < _mvEntityPosition.size(); i++)
		{
			_mvEntityPosition[i].x += fTimeDelta * _mvEntityVelocity[i].x;
			_mvEntityPosition[i].y += fTimeDelta * _mvEntityVelocity[i].y;

			if (_mvEntityPosition[i].x < -vWndDimsHalf.x)
				_mvEntityPosition[i].x = vWndDimsHalf.x;
			else if (_mvEntityPosition[i].x > vWndDimsHalf.x)
				_mvEntityPosition[i].x = -vWndDimsHalf.x;

			if (_mvEntityPosition[i].y < -vWndDimsHalf.y)
				_mvEntityPosition[i].y = vWndDimsHalf.y;
			else if (_mvEntityPosition[i].y > vWndDimsHalf.y)
				_mvEntityPosition[i].y = -vWndDimsHalf.y;

			pEntityLine = pLayer->getEntityLine(i);
			pEntityLine->setPosition(_mvEntityPosition[i]);

			pEntityLine->setRotationRadians(pEntityLine->getRotationRadians() + (_mvEntityRotationAmount[i] * fTimeDelta));
			
		}

		CVector2f vCamPos = pCamera->getPosition();
		if (pInput->key.pressed(KC_UP))
			vCamPos.y -= _mTimer.getSecondsPast() * 250.0f;
		if (pInput->key.pressed(KC_DOWN))
			vCamPos.y += _mTimer.getSecondsPast() * 250.0f;
		if (pInput->key.pressed(KC_LEFT))
			vCamPos.x -= _mTimer.getSecondsPast() * 250.0f;
		if (pInput->key.pressed(KC_RIGHT))
			vCamPos.x += _mTimer.getSecondsPast() * 250.0f;
		pCamera->setPosition(vCamPos);

		p2D->render();
	}
}