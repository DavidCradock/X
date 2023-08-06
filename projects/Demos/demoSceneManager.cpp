#include "PCH.h"
#include "demoSceneManager.h"

namespace X
{
	void CStateDemoSceneManager::onEnter(void)
	{
		// Use the resource loading screen
		SCResourceLoadingScreen* pLS = SCResourceLoadingScreen::getPointer();
		pLS->onInit(4);

		timer.setAveragedFPSRate(1);	// Once every X seconds

		// Load some stuff in and setup simple scene manager
		_initSceneManager();

		// End of loading screen
		pLS->onInitEnd();
	}

	void CStateDemoSceneManager::onExit(void)
	{
		SCGUIManager* pGUI = SCGUIManager::getPointer();
		SCResourceManager* pRM = SCResourceManager::getPointer();
		pRM->removeVertexBufferBNT("cube");
		pRM->removeVertexBufferBNT("icosphere_radius_0.01");
		pRM->removeVertexBufferBNT("groundplane");
		pRM->removeVertexBufferLine("line");
		pRM->removeTexture2DFromFile("data/DemoSceneManager/textures/cube_BaseColor.png");
		pRM->removeTexture2DFromFile("data/DemoSceneManager/textures/cube_Roughness.png");
		pRM->removeTexture2DFromFile("data/DemoSceneManager/textures/cube_Normal.png");
		pRM->removeTexture2DFromFile("data/DemoSceneManager/textures/groundplane.png");
		

//		mSceneManagerSimple.removeAllCameras();
		mSceneManagerSimple.removeAllEnititiesTriangle();
		mSceneManagerSimple.removeAllEntitiesLine();
		mSceneManagerSimple.removeAllMaterials();
	}

	void CStateDemoSceneManager::onActive(CFiniteStateMachine* pFSM)
	{
		// Get pointers to needed managers
		SCInputManager* pInput = SCInputManager::getPointer();
		SCResourceManager* pRM = SCResourceManager::getPointer();

		// Timer delta
		timer.update();

		// Update and render scene manager to a framebuffer
		mSceneManagerSimple.render();

		// Render the debug shadow map onto the screen
		SCWindow* pWindow = SCWindow::getPointer();
		CResourceDepthbuffer* pDepthbuffer = pRM->getDepthbuffer("X:shadows");
		pDepthbuffer->renderTo2DQuad(pWindow->getWidth() - 512, 0, 512, 512);

		float fInc = timer.getSecondsPast();

		SCGUIManager* pGUIMan = SCGUIManager::getPointer();

		// Update line entity
		CSMEntityLine* pEntityLine = mSceneManagerSimple.getEntityLine("line");

		// Translation
		if (pInput->key.pressed(KC_W))	pEntityLine->translateWorld(CVector3f(0.0f, 0.0f, 1.0f * fInc));
		if (pInput->key.pressed(KC_S))	pEntityLine->translateWorld(CVector3f(0.0f, 0.0f, -1.0f * fInc));
		if (pInput->key.pressed(KC_A))	pEntityLine->translateWorld(CVector3f(-1.0f * fInc, 0.0f, 0.0f));
		if (pInput->key.pressed(KC_D))	pEntityLine->translateWorld(CVector3f(1.0f * fInc, 0.0f, 0.0f));
		if (pInput->key.pressed(KC_R))	pEntityLine->translateWorld(CVector3f(0.0f, 1.0f * fInc, 0.0f));
		if (pInput->key.pressed(KC_F))	pEntityLine->translateWorld(CVector3f(0.0f, -1.0f * fInc, 0.0f));
		// Scale
		if (pInput->key.pressed(KC_T))
		{
			CVector3f v3EntityScale = pEntityLine->getScale();
			v3EntityScale.x += fInc;
			v3EntityScale.y += fInc;
			v3EntityScale.z += fInc;
			pEntityLine->setScale(v3EntityScale);
		}
		if (pInput->key.pressed(KC_G))
		{
			CVector3f v3EntityScale = pEntityLine->getScale();
			v3EntityScale.x -= fInc;
			v3EntityScale.y -= fInc;
			v3EntityScale.z -= fInc;
			pEntityLine->setScale(v3EntityScale);
		}
		// Rotation
		if (pInput->key.pressed(KC_Y))
		{

		}
		if (pInput->key.pressed(KC_H))
		{

		}
		if (pInput->key.pressed(KC_U))
		{

		}
		if (pInput->key.pressed(KC_J))
		{

		}
		if (pInput->key.pressed(KC_I))
		{

		}
		if (pInput->key.pressed(KC_K))
		{

		}

		// Update entity rotation
		for (int i = 0; i < 100; ++i)
		{
			// Temp rotation
			_mvEntityRot[i].x += deg2rad(45.0f) * timer.getSecondsPast();
			_mvEntityRot[i].y += deg2rad(45.0f) * timer.getSecondsPast();
			_mvEntityRot[i].z += deg2rad(45.0f) * timer.getSecondsPast();

			std::string strEntity = "entity_" + std::to_string(i);
			CSMEntityTriangle* pEntity = mSceneManagerSimple.getEntityTriangle(strEntity);

			CQuaternion qRot;
			qRot.setFromEuler(_mvEntityRot[i].x, _mvEntityRot[i].y, _mvEntityRot[i].z);
			pEntity->setRotation(qRot);
		}

	}

	void CStateDemoSceneManager::_initSceneManager(void)
	{
		// Set camera mode
		CSMCamera* pCamera = mSceneManagerSimple.getCamera(0);
		pCamera->setModeOrbit(
			CVector3f(0.0f, 0.0f, 0.0f),	// Orbit point
			1.5f,							// Min distance from point
			10.0f,							// Current distance from point
			100.0f,							// Max distance from point
			0.005f,							// Sensitivity X
			0.005f,							// Sensitivity Y
			0.005f,							// Sensitivity wheel
			true);							// Limit Y angle to > 0.0f

		// Create needed triangles
		SCResourceManager* pRM = SCResourceManager::getPointer();
		// Cube
		CResourceVertexBufferBNT* pVB = pRM->addVertexBufferBNT("cube");
		pVB->addFromFile("data/DemoSceneManager/geometry/cube.geom", true);
		// Point lights to show where they are
		pVB = pRM->addVertexBufferBNT("icosphere_radius_0.01");
		pVB->addFromFile("data/DemoSceneManager/geometry/icosphere_radius_0.01.geom", true);
		// Ground plane
		pVB = pRM->addVertexBufferBNT("groundplane");
		pVB->addFromFile("data/DemoSceneManager/geometry/ground_plane_200x200x1.geom", true);

		// Load in a textures
		// Cubes
		pRM->addTexture2DFromFile("data/DemoSceneManager/textures/cube_BaseColor.png", "data/DemoSceneManager/textures/cube_BaseColor.png", true);
		pRM->addTexture2DFromFile("data/DemoSceneManager/textures/cube_Roughness.png", "data/DemoSceneManager/textures/cube_Roughness.png", true);
		pRM->addTexture2DFromFile("data/DemoSceneManager/textures/cube_Normal.png", "data/DemoSceneManager/textures/cube_Normal.png", true);
		// Point light entities
		pRM->addTexture2DFromFile("data/DemoSceneManager/textures/groundplane.png", "data/DemoSceneManager/textures/groundplane.png", true);

		// Create materials
		mSceneManagerSimple.addMaterial("mat_cubes", 0.05f, "data/DemoSceneManager/textures/cube_BaseColor.png", "data/DemoSceneManager/textures/cube_Roughness.png", 0.75f, "data/DemoSceneManager/textures/cube_Normal.png", "X:default_emission");
		mSceneManagerSimple.addMaterial("mat_white", 0.05f, "X:default_white", "X:default_white", 0.5f, "X:default_normal", "X:default_white");
		mSceneManagerSimple.addMaterial(
			"mat_groundplane",						// Material name
			0.25f,									// Ambient strength
			"data/DemoSceneManager/textures/groundplane.png",	// Diffuse texture
			"X:default_roughness",					// Roughness texture (grey)
			1.0f,									// Specular strength
			"X:default_normal",						// Normal texture (float)
			"X:default_emission");					// Emission texture (black)

		CSMEntityTriangle* pEntity = mSceneManagerSimple.addEntityTriangle("centre", "cube", "mat_cubes");
		pEntity->setWorldPosition(CVector3f(0.0f, 0.0f, 0.0f));

		for (int i = 0; i < 100; ++i)
		{
			// Temp rotation
			_mvEntityRot[i].x += float(i) * 1.0f;
			_mvEntityRot[i].y += float(i) * 1.0f;
			_mvEntityRot[i].z += float(i) * 1.0f;

			std::string strEntity = "entity_" + std::to_string(i);
			pEntity = mSceneManagerSimple.addEntityTriangle(strEntity, "cube", "mat_cubes");
			pEntity->setWorldPosition(CVector3f(randf(-19.0f, 19.0f), 0.5f, randf(-19.0f, 19.0f)));
		}
		// Ground plane
		pEntity = mSceneManagerSimple.addEntityTriangle("groundplane", "groundplane", "mat_groundplane");

		// Directional light settings
		mSceneManagerSimple.setDirectionalLightColour(CColour(1.0f, 1.0f, 1.0f, 1.0f));
		mSceneManagerSimple.setDirectionalLightDir(CVector3f(1.0f, -1.0f, 1.0f));

		// Set point light settings
		mSceneManagerSimple.miNumPointLights = 0;
		mSceneManagerSimple.mvLightPoint[0].mvPosition = CVector3f(2.0f, 2.0f, 10.0f);
		mSceneManagerSimple.mvLightPoint[0].mvColour = CColour(0.7f, 0.7f, 0.5f, 1.0f);
		mSceneManagerSimple.mvLightPoint[1].mvPosition = CVector3f(-2.0f, 2.0f, 2.0f);
		mSceneManagerSimple.mvLightPoint[1].mvColour = CColour(0.0f, 1.0f, 0.0f, 1.0f);
		mSceneManagerSimple.mvLightPoint[2].mvPosition = CVector3f(2.0f, -2.0f, 2.0f);
		mSceneManagerSimple.mvLightPoint[2].mvColour = CColour(0.0f, 0.0f, 1.0f, 1.0f);
		mSceneManagerSimple.mvLightPoint[3].mvPosition = CVector3f(2.0f, 2.0f, -2.0f);
		mSceneManagerSimple.mvLightPoint[3].mvColour = CColour(1.0f, 1.0f, 1.0f, 1.0f);
		// Add icosphere_radius_0.01.geom to position of each light
		for (int i = 0; i < mSceneManagerSimple.miNumPointLights; i++)
		{
			std::string strEntity = "point_light_" + std::to_string(i);
			pEntity = mSceneManagerSimple.addEntityTriangle(strEntity, "icosphere_radius_0.01", "mat_white");
			CVector3f vPos;
			vPos.x = mSceneManagerSimple.mvLightPoint[i].mvPosition.x;
			vPos.y = mSceneManagerSimple.mvLightPoint[i].mvPosition.y;
			vPos.z = mSceneManagerSimple.mvLightPoint[i].mvPosition.z;
			pEntity->setWorldPosition(vPos);
		}

		// Now we're done with adding triangle entities.
		// Let's add some line entities
		CResourceVertexBufferLine* pLine = pRM->addVertexBufferLine("line");
		pLine->setDrawModeAsLineList();
		CResourceVertexBufferLine::Vertex lineVertex;
		// + Y
		lineVertex.colour = CColour(0.0f, 1.0f, 0.0f, 1.0f);
		lineVertex.position = CVector3f(0.0f, 0.0f, 0.0f);		pLine->addLinePoint(lineVertex);
		lineVertex.position = CVector3f(0.0f, 1.0f, 0.0f);		pLine->addLinePoint(lineVertex);
		// + X
		lineVertex.colour = CColour(1.0f, 0.0f, 0.0f, 1.0f);
		lineVertex.position = CVector3f(0.0f, 0.0f, 0.0f);		pLine->addLinePoint(lineVertex);
		lineVertex.position = CVector3f(1.0f, 0.0f, 0.0f);		pLine->addLinePoint(lineVertex);
		// + Z
		lineVertex.colour = CColour(0.0f, 0.0f, 1.0f, 1.0f);
		lineVertex.position = CVector3f(0.0f, 0.0f, 0.0f);		pLine->addLinePoint(lineVertex);
		lineVertex.position = CVector3f(0.0f, 0.0f, 1.0f);		pLine->addLinePoint(lineVertex);
		pLine->update();

		CSMEntityLine* pEntityLine = mSceneManagerSimple.addEntityLine("line", "line", "X:default_white");
		pEntityLine->setWorldPosition(CVector3f(0.0f, 1.0f, 0.0f));
	}
}