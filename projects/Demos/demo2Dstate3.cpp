#include "PCH.h"
#include "demo2Dstate3.h"

namespace X
{
	void CDemo2DState3::onEnter(void)
	{
		// Create state window
		CGUIContainer* pCont = x->pGUI->addContainer("state3");
		pCont->setDimensions(640, 240);
		pCont->setPosition((x->pResource->getBackbufferDims().x * 0.5f) - 320.0f, 0);
		pCont->mstrTitleText = "State three.";
		// Add text scroll
		std::string strTxt;
		strTxt += "Welcome to state 3.\n";
		strTxt += "This state shows C2DEntityLine rendering.\n";
		strTxt += "Controls...\n";
		strTxt += "Cursor keys move the camera.\n";
		strTxt += " \nDetails...\n";
		pCont->addText("TextScroll3", 0, 0, strTxt);

		// Create entity's random position and velocity
		CVector2f vWndDims = x->pWindow->getDimensions();
		CVector2f vWndDimsHalf = vWndDims * 0.5f;
		// Set each entity's position and velocity
		for (int i = 0; i < 200; i++)
		{
			CVector2f vRandomPos(randf(-vWndDimsHalf.x, vWndDimsHalf.x), randf(-vWndDimsHalf.y, vWndDimsHalf.y));
			_mvEntityPosition.push_back(vRandomPos);
			CVector2f vRandomVel(randf(-100, 100), randf(-100, 100));
			_mvEntityVelocity.push_back(vRandomVel);

			float fRandomRotation = 0;
			while (fRandomRotation > -0.5f && fRandomRotation < 0.5f)
			{
				fRandomRotation = randf(-kPi, kPi);
			}
			_mvEntityRotationAmount.push_back(fRandomRotation);
		}

		// Setup 2D renderer
		C2DWorld* pWorld = x->p2dRenderer->addWorld("world");
		C2DCamera* pCamera = pWorld->addCamera("camera");
		pCamera->setPosition(-vWndDimsHalf.x, -vWndDimsHalf.y);

		C2DLayer* pLayer = pWorld->addLayer("layer");
		for (int i = 0; i < _mvEntityPosition.size(); i++)
		{
			std::string strName = "Ent: " + std::to_string(i);
			C2DEntityLine* pEntity = pLayer->addEntityLine(strName);
			if (i % 2 == 0)
				pEntity->setAsCircle(randf(10, 50), randInt(3, 10));
			else
				pEntity->setAsQuad(randf(10, 100), randf(10, 100));
		}
		
		_mTimer.update();
	}

	void CDemo2DState3::onExit(void)
	{
		x->pGUI->removeContainer("state3");
		x->p2dRenderer->removeWorld("world");
	}

	void CDemo2DState3::onActive(CFiniteStateMachine* pFSM)
	{
		CVector2f vWndDims = x->pWindow->getDimensions();
		CVector2f vWndDimsHalf = vWndDims * 0.5f;

		_mTimer.update();
		float fTimeDelta = _mTimer.getSecondsPast();

		C2DWorld* pWorld = x->p2dRenderer->getWorld("world");
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
		if (x->pInput->key.pressed(KC_UP))
			vCamPos.y -= _mTimer.getSecondsPast() * 250.0f;
		if (x->pInput->key.pressed(KC_DOWN))
			vCamPos.y += _mTimer.getSecondsPast() * 250.0f;
		if (x->pInput->key.pressed(KC_LEFT))
			vCamPos.x -= _mTimer.getSecondsPast() * 250.0f;
		if (x->pInput->key.pressed(KC_RIGHT))
			vCamPos.x += _mTimer.getSecondsPast() * 250.0f;
		pCamera->setPosition(vCamPos);

		//x->p2dRenderer->render();

		// Deal with fullscreen toggling
		if (bFullscreenToggle)
		{
			bFullscreenToggle = false;

		}
	}
}