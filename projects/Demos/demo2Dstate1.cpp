#include "PCH.h"
#include "demo2Dstate1.h"

namespace X
{
	void CDemo2DState1::onEnter(void)
	{
		SC2DRenderer* p2DRenderer = SC2DRenderer::getPointer();		// Obtain pointer to 2D renderer
		C2DWorld* pWorld = p2DRenderer->addWorld("Scene1World");	// Add a new world to contain everything
		C2DLayer* pLayer0 = pWorld->addLayer("Scene1Layer0");		// Add a new layer to the world
		C2DLayer* pLayer1 = pWorld->addLayer("Scene1Layer1");		// Add a new layer to the world
		C2DCamera* pCamera = pWorld->addCamera("Scene1Camera");		// Add a new camera to the world
		SCWindow* pWindow = SCWindow::getPointer();
		SCResourceManager* pRM = SCResourceManager::getPointer();
		SCGUIManager* pGUI = SCGUIManager::getPointer();

		// Set filename of each image for the top down tank
		std::string strTankBody = "data/Demos/Demo2D/images/tank_body.png";
		std::string strTankTurret = "data/Demos/Demo2D/images/tank_turret.png";

		// Add 1000 entities (500 body and 500 turret)
		std::string strEntityName;
		C2DEntityRot* pEntityBody;
		C2DEntityRot* pEntityTurret;
		for (int i = 0; i < 500; i++)
		{
			strEntityName = "EntityBody: " + std::to_string(i);					// Create unique name for the entity
			pEntityBody = pLayer0->addEntityRot(strEntityName, "Scene1Tank");	// Add entity to the layer
			pEntityBody->setImagesSingle(strTankBody);							// Set which image the entity will be using

			// Set random position
			CVector2f vPos(randf(0, (float)pWindow->getWidth()), randf(0, (float)pWindow->getHeight()));
			pEntityBody->setPosition(vPos);

			// Set scale
			pEntityBody->setScale(0.5, 0.5);

			// Set random initial rotation
			mfEntityBodyRotationRadians[i] = randf(0, k2Pi);

			// Set random rotation amount
			mfEntityBodyRotationRadiansDir[i] = randf(-k2Pi, k2Pi);

			strEntityName = "EntityTurret: " + std::to_string(i);				// Create unique name for the entity
			pEntityTurret = pLayer1->addEntityRot(strEntityName, "Scene1Tank");	// Add entity to the layer
			pEntityTurret->setImagesSingle(strTankTurret);						// Set which image the entity will be using

			// Set random position
			pEntityTurret->setPosition(vPos);

			// Set scale
			pEntityTurret->setScale(0.5, 0.5);

			// Set random initial rotation
			mfEntityTurretRotationRadians[i] = randf(0, k2Pi);

			// Set random rotation amount
			mfEntityTurretRotationRadiansDir[i] = randf(-k2Pi, k2Pi);
		}

		// Create state window
		CGUIContainer* pCont = pGUI->addContainer("state1");
		pCont->setDimensions(640, 640);
		pCont->setPosition(0, 0);
		pCont->mstrTitleText = "State one.";

		// Add text scroll
		std::string strTxt;
		strTxt += "Welcome to state 1!\n";
		strTxt += "This state shows a single world and a single camera with two layers\n";
		strTxt += "The lowest layer contains images of a tank body rotating around a random amount.\n";
		strTxt += "The highest layer contains images of a tank turret rotating independently of the tank body, but their position is the same.\n";
		strTxt += "Controls...\n";
		strTxt += "The cursor keys move the camera.\n \n";
		strTxt += "Details...\n";
		strTxt += "As CEntityRot objects cannot be batched before being sent to the GPU as they each have their own world matrix which is used for the rotations, ";
		strTxt += "they are a lot slower to render. But that's OK, they're not meant to be used for rendering things such as particle systems! No, we use particle systems for those lol.\n";
		strTxt += "The added ability to be rotated is worth it.\n";
		strTxt += "Try switching between state 0 and 1 and notice the framerate differences.\n";
		pCont->addTextScroll("TextScroll1", 0, 0, 640, 640, strTxt);
	}

	void CDemo2DState1::onExit(void)
	{
		SC2DRenderer* p2D = SC2DRenderer::getPointer();
		SCGUIManager* pGUI = SCGUIManager::getPointer();
		p2D->removeWorld("Scene1World");
		pGUI->removeContainer("state1");
	}

	void CDemo2DState1::onActive(CFiniteStateMachine* pFSM)
	{
		// Timer delta
		timer.update();

		SC2DRenderer* p2D = SC2DRenderer::getPointer();
		SCGUIManager* pGUI = SCGUIManager::getPointer();
		SCInputManager* pInput = SCInputManager::getPointer();

		// Rotate entities
		C2DWorld* pWorld = p2D->getWorld("Scene1World");
		C2DLayer* pLayer0 = pWorld->getLayer("Scene1Layer0");
		C2DLayer* pLayer1 = pWorld->getLayer("Scene1Layer1");
		std::string strEntityName;
		C2DEntityRot* pEntity;
		for (int i = 0; i < 500; i++)
		{
			strEntityName = "EntityBody: " + std::to_string(i);
			pEntity = pLayer0->getEntityRot(strEntityName);
			mfEntityBodyRotationRadians[i] += timer.getSecondsPast() * mfEntityBodyRotationRadiansDir[i];
			while (mfEntityBodyRotationRadians[i] >= k2Pi)
				mfEntityBodyRotationRadians[i] -= k2Pi;
			while (mfEntityBodyRotationRadians[i] < 0.0f)
				mfEntityBodyRotationRadians[i] += k2Pi;
			pEntity->setRotationRadians(mfEntityBodyRotationRadians[i]);

			strEntityName = "EntityTurret: " + std::to_string(i);
			pEntity = pLayer1->getEntityRot(strEntityName);
			mfEntityTurretRotationRadians[i] += timer.getSecondsPast() * mfEntityTurretRotationRadiansDir[i];
			while (mfEntityTurretRotationRadians[i] >= k2Pi)
				mfEntityTurretRotationRadians[i] -= k2Pi;
			while (mfEntityTurretRotationRadians[i] < 0.0f)
				mfEntityTurretRotationRadians[i] += k2Pi;
			pEntity->setRotationRadians(mfEntityTurretRotationRadians[i]);
		}

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