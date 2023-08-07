#include "PCH.h"
#include "demoOctTree.h"

namespace X
{
	void CStateDemoOctTree::onEnter(void)
	{
		// Use the resource loading screen
		SCResourceLoadingScreen* pLS = SCResourceLoadingScreen::getPointer();
		pLS->onInit(1);

		// Set oct tree settings
		_mOctTree.init(1);

		// Create resources used by scene manager
		SCResourceManager* pRM = SCResourceManager::getPointer();
		pRM->addTexture2DFromFile("ground", "data/DemoOctTree/textures/ground.png");

		// Cube triangle
		CResourceVertexBufferBNT* pVB = pRM->addVertexBufferBNT("cube1x1x1");
		pVB->addCube(CVector3f(0.0f, 0.0f, 0.0f), CVector3f(1.0f, 1.0f, 1.0f));
		pVB->update();

		// Cube line
		CResourceVertexBufferLine* pLine = pRM->addVertexBufferLine("axis");
		pLine->setDrawModeAsLineList();
		CResourceVertexBufferLine::Vertex v;
		pLine->addAxis(CVector3f(0.0f, 0.0f, 0.0f), 2.0f);
		pLine->update();

		// Ground triangle
		pVB = pRM->addVertexBufferBNT("ground");
		pVB->addCube(CVector3f(0.0f, -0.5f, 0.0f), CVector3f(64.0f, 1.0f, 64.0f), CVector2f(32.0f, 32.0f));
		pVB->update();

		// Create materials
		_mSM.addMaterial("mat0_default");
		_mSM.addMaterial("mat1_ground", 0.05f, "ground");
		
		// Setup camera
		CSMCamera* pCamera = _mSM.getCamera(0);
		pCamera->setModeOrbit(CVector3f(0, 0, 0), 1.5f, 100.0f, 150.0f, 0.005f, 0.005f, 0.005f, false);
		pCamera->setModeFPS(CVector3f(0.0f, 2.0f, 10.0f), -90.0f);

		// Set light stuff
		CVector3f vLightDir(-1, -1, -1);
		vLightDir.normalise();
		_mSM.setDirectionalLightDir(vLightDir);

		// Set shadow stuff
//		_mSM.setShadowMatrixManual();

		// Add entity vertex buffer
		CVector3f vEntityPos(0.0f, 0.5f, 0.0f);
		CSMEntityVertexBuffer* pEntityTri = _mSM.addEntityVertexBuffer("cube", "cube1x1x1", "mat0_default");
		pEntityTri->setWorldPosition(vEntityPos);

		// Add entity line
		CSMEntityLine* pEntityLine = _mSM.addEntityLine("axis", "axis");
		pEntityLine->setWorldPosition(vEntityPos);

		// The above two entities to the oct tree
		_mOctTree.addEntity("entity", vEntityPos);

		// Add ground entity
		_mSM.addEntityVertexBuffer("ent_ground", "ground", "mat1_ground");

		// Now create 50 entities which rotate and move around randomly
		for (int i = 0; i < 50; i++)
		{
			std::string strEntityName = "Ent_";
			StringUtils::appendInt(strEntityName, i);
			SEntity entity;
			entity.strName = strEntityName;
			entity.vPos.set(randf(-10.0f, 10.0f), randf(-10.0f, 10.0f), randf(-10.0f, 10.0f));
			entity.vRot.set(randf(-1.0f, 1.0f), randf(-1.0f, 1.0f), randf(-1.0f, 1.0f));
			entity.pEntLine = _mSM.addEntityLine(strEntityName, "axis");
			entity.pEntTri = _mSM.addEntityVertexBuffer(strEntityName, "cube1x1x1", "mat0_default");
			entity.pEntLine->setWorldPosition(entity.vPos);
			entity.pEntTri->setWorldPosition(entity.vPos);

			// Compute random rotation
			CVector3f vRotAxis(randf(-1.0f, 1.0f), randf(-1.0f, 1.0f), randf(-1.0f, 1.0f));
			vRotAxis.normalise();
			float fRotAngle = randf(-k2Pi, k2Pi);
			entity.pEntLine->setRotation(vRotAxis, fRotAngle);
			entity.pEntTri->setRotation(vRotAxis, fRotAngle);
			_mvEntities.push_back(entity);

			// Add to oct tree
			_mOctTree.addEntity(strEntityName, entity.vPos);
		}

		// Create invisible GUI container showing controls
		SCGUIManager* pGUI = SCGUIManager::getPointer();
		CGUIContainer* pCont = pGUI->addContainer("controls");
		CGUITheme* pTheme = pCont->getTheme();
		pCont->setBehaviour(false);
		float fTextHeight = pRM->getFont(pTheme->mFonts.text)->getTextHeight(1.0f);
		float fYpos = 0;
		pCont->addText("txt0", 5, fYpos, "Controls...")->setColour(false);	fYpos += fTextHeight;
		pCont->addText("txt1", 5, fYpos, "W and S = move along forward vector of entity.")->setColour(false);	fYpos += fTextHeight;
		pCont->addText("txt2", 5, fYpos, "A and D = move along right vector of entity.")->setColour(false);	fYpos += fTextHeight;
		pCont->addText("txt3", 5, fYpos, "R and F = move along up vector of entity.")->setColour(false);	fYpos += fTextHeight;
		pCont->addText("txt4", 5, fYpos, "Shift key increases speed of translation of entity.")->setColour(false);	fYpos += fTextHeight;
		pCont->addText("txt5", 5, fYpos, "Y and H = Pitch. Rotate along right vector of entity.")->setColour(false);	fYpos += fTextHeight;
		pCont->addText("txt6", 5, fYpos, "G and J = Yaw. Rotate along up vector of entity.")->setColour(false);	fYpos += fTextHeight;
		pCont->addText("txt7", 5, fYpos, "I and K = Roll. Rotate along forward vector of entity.")->setColour(false);	fYpos += fTextHeight;
		pCont->addText("txt8", 5, fYpos, "Entity Position: X=0.0 | Y=0.0  | Z=0.0")->setColour(false); fYpos += fTextHeight;
		pCont->addText("txt9", 5, fYpos, "Entity Rotation Degrees: Pitch=0.0 | Yaw=0.0  | Roll=0.0")->setColour(false); fYpos += fTextHeight;
		pCont->addText("txt10", 5, fYpos, "Camera Position: ")->setColour(false); fYpos += fTextHeight;

		// End of loading screen
		pLS->onInitEnd();

		// Debug CFrustum
		CSMCamera camTemp;
		camTemp.setProjectionAsPerspective(60.0f);
		camTemp.setViewAsIdentity();
		_mTestFrustum.computeFromViewProjection(camTemp.getViewMatrix(), camTemp.getProjectionMatrix());
		_mTestFrustum.isPointInside(CVector3f(0, 0, 5));
	}

	void CStateDemoOctTree::onExit(void)
	{
		SCGUIManager* pGUI = SCGUIManager::getPointer();
		pGUI->removeContainer("controls");

		SCResourceManager* pRM = SCResourceManager::getPointer();
		pRM->removeTexture2DFromFile("ground");
		pRM->removeVertexBufferCPT("cube1x1x1");
		pRM->removeVertexBufferLine("axis");
		pRM->removeVertexBufferCPT("ground");

		_mSM.removeAllMaterials();
//		_mSM.removeAllCameras();
		_mSM.removeAllEnititiesVertexBuffer();
		_mSM.removeAllEntitiesLine();

		_mvEntities.clear();
	}

	void CStateDemoOctTree::onActive(CFiniteStateMachine* pFSM)
	{
		SCInputManager* pInput = SCInputManager::getPointer();

		// Timer delta
		timer.update();

		// Update entity
		float fTransForward = 0.0f;
		float fTransRight = 0.0f;
		float fTransUp = 0.0f;
		bool bTranslate = false;
		bool bRotate = false;
		float fMultiplier = 1.0f;
		float fRotPitch = 0.0f;
		float fRotYaw = 0.0f;
		float fRotRoll = 0.0f;
		if (pInput->key.pressed(KC_LSHIFT) || pInput->key.pressed(KC_RSHIFT)) { fMultiplier = 10.0f; }
		if (pInput->key.pressed(KC_W)) { fTransForward = fMultiplier * timer.getSecondsPast();	bTranslate = true; }
		if (pInput->key.pressed(KC_S)) { fTransForward = -fMultiplier * timer.getSecondsPast();	bTranslate = true; }
		if (pInput->key.pressed(KC_A)) { fTransRight = -fMultiplier * timer.getSecondsPast();	bTranslate = true; }
		if (pInput->key.pressed(KC_D)) { fTransRight = fMultiplier * timer.getSecondsPast();	bTranslate = true; }
		if (pInput->key.pressed(KC_R)) { fTransUp = fMultiplier * timer.getSecondsPast();		bTranslate = true; }
		if (pInput->key.pressed(KC_F)) { fTransUp = -fMultiplier * timer.getSecondsPast();		bTranslate = true; }

		if (pInput->key.pressed(KC_Y)) { fRotPitch = fMultiplier * timer.getSecondsPast();	bRotate = true; }
		if (pInput->key.pressed(KC_H)) { fRotPitch = -fMultiplier * timer.getSecondsPast();	bRotate = true; }
		if (pInput->key.pressed(KC_G)) { fRotYaw = -fMultiplier * timer.getSecondsPast();	bRotate = true; }
		if (pInput->key.pressed(KC_J)) { fRotYaw = fMultiplier * timer.getSecondsPast();	bRotate = true; }
		if (pInput->key.pressed(KC_I)) { fRotRoll = -fMultiplier * timer.getSecondsPast();	bRotate = true; }
		if (pInput->key.pressed(KC_K)) { fRotRoll = fMultiplier * timer.getSecondsPast();	bRotate = true; }

		// Apply translation to cube and axis entities
		if (bTranslate)
		{
			CSMEntityLine* pEntityLine = _mSM.getEntityLine("axis");
			CSMEntityVertexBuffer* pEntityTri = _mSM.getEntityVertexBuffer("cube");
			pEntityLine->translateLocal(fTransForward, fTransUp, fTransRight);
			pEntityTri->translateLocal(fTransForward, fTransUp, fTransRight);

			// Update GUI text
			CGUIContainer* pCont = SCGUIManager::getPointer()->getContainer("controls");
			std::string strTxt;
			CVector3f vPos = pEntityLine->getWorldPosition();
			strTxt = "Entity Position: X="; StringUtils::appendFloat(strTxt, vPos.x, 1);
			strTxt += " | Y="; StringUtils::appendFloat(strTxt, vPos.y, 1);
			strTxt += " | Z="; StringUtils::appendFloat(strTxt, vPos.z, 1);
			pCont->getText("txt8")->setText(strTxt);

			// Update oct tree
			_mOctTree.setEntityPosition("entity", pEntityLine->getWorldPosition());
		}
		// Apply rotation to cube and axis entities
		if (bRotate)
		{
			CSMEntityLine* pEntityLine = _mSM.getEntityLine("axis");
			CSMEntityVertexBuffer* pEntityTri = _mSM.getEntityVertexBuffer("cube");
			pEntityLine->rotateLocal(fRotPitch, fRotYaw, fRotRoll);
			pEntityTri->rotateLocal(fRotPitch, fRotYaw, fRotRoll);

			// Update GUI text
			CGUIContainer* pCont = SCGUIManager::getPointer()->getContainer("controls");
			std::string strTxt;
			CVector3f vEuler = pEntityLine->getEuler();
			strTxt = "Entity Rotation: Pitch="; StringUtils::appendFloat(strTxt, rad2deg(vEuler.x), 1);
			strTxt += " | Yaw="; StringUtils::appendFloat(strTxt, rad2deg(vEuler.y), 1);
			strTxt += " | Roll="; StringUtils::appendFloat(strTxt, rad2deg(vEuler.z), 1);
			pCont->getText("txt9")->setText(strTxt);

			// Update oct tree
			_mOctTree.setEntityPosition("entity", pEntityLine->getWorldPosition());
		}

		// Now update the entities which rotate and move around randomly
		for (int i = 0; i < (int)_mvEntities.size(); i++)
		{
			// Compute rotation
			CVector3f vRot;
			vRot.x = _mvEntities[i].vRot.x * timer.getSecondsPast() * 1.0f;
			vRot.y = _mvEntities[i].vRot.y * timer.getSecondsPast() * 1.0f;
			vRot.z = _mvEntities[i].vRot.z * timer.getSecondsPast() * 1.0f;

			// Compute translation
			CVector3f vTrans;
			vTrans.x = _mvEntities[i].vRot.x * timer.getSecondsPast() * 10.0f;

			// Apply transformations to the line and tri entities
			_mvEntities[i].pEntTri->rotateLocal(vRot);
			_mvEntities[i].pEntTri->translateLocal(vTrans);
			_mvEntities[i].pEntLine->rotateLocal(vRot);
			_mvEntities[i].pEntLine->translateLocal(vTrans);

			// Wrap entities if they exceed a certain distance from origin
			CVector3f vPos = _mvEntities[i].pEntTri->getWorldPosition();
			if (vPos.x < -32.0f)
			{
				vPos.x = 32.0f;
				_mvEntities[i].pEntTri->setWorldPosition(vPos);
				_mvEntities[i].pEntLine->setWorldPosition(vPos);
			}
			if (vPos.x > 32.0f)
			{
				vPos.x = -32.0f;
				_mvEntities[i].pEntTri->setWorldPosition(vPos);
				_mvEntities[i].pEntLine->setWorldPosition(vPos);
			}
			if (vPos.y < 0.5f)
			{
				vPos.y = 32.0f;
				_mvEntities[i].pEntTri->setWorldPosition(vPos);
				_mvEntities[i].pEntLine->setWorldPosition(vPos);
			}
			if (vPos.y > 32.0f)
			{
				vPos.y = 0.5f;
				_mvEntities[i].pEntTri->setWorldPosition(vPos);
				_mvEntities[i].pEntLine->setWorldPosition(vPos);
			}
			if (vPos.z < -32.0f)
			{
				vPos.z = 32.0f;
				_mvEntities[i].pEntTri->setWorldPosition(vPos);
				_mvEntities[i].pEntLine->setWorldPosition(vPos);
			}
			if (vPos.z > 32.0f)
			{
				vPos.z = -32.0f;
				_mvEntities[i].pEntTri->setWorldPosition(vPos);
				_mvEntities[i].pEntLine->setWorldPosition(vPos);
			}

			// Update position stored in struct
			_mvEntities[i].vPos = _mvEntities[i].pEntLine->getWorldPosition();

			// Update oct tree
			_mOctTree.setEntityPosition(_mvEntities[i].strName, _mvEntities[i].vPos);
		}

		// Render the scene manager
		_mSM.render();

		// Debug render the oct tree
		CSMCamera* pCamera = _mSM.getCamera(0);
		_mOctTree.debugRender(*pCamera);

		// Render shadow depth buffer
		_mSM.debugRenderShadowmap(512);

		// Update GUI stuff
		CGUIContainer* pCont = SCGUIManager::getPointer()->getContainer("controls");
		std::string strTxt;
		strTxt = "Camera Position: ";
		StringUtils::appendCVector3f(strTxt, pCamera->getPosition());
		pCont->getText("txt10")->setText(strTxt);
	}
}