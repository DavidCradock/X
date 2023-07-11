#include "application.h"
#include "resource.h"

namespace X
{
	void CApplication::initOnce(void)
	{
		// Set window title bar text and set icon
		SCWindow* pWindow = SCWindow::getPointer();
		pWindow->setText("X DemoSceneManager. F1: Toggle fullscreen. F2: Toggle Vsync. F3: Toggle statistics window.");
		pWindow->setIcon(IDI_ICON1);

		// Show "X:Default:Statistics" container
		SCGUIManager* pGUI = SCGUIManager::getPointer();
		pGUI->getContainer("X:Default:Statistics")->setVisible(true);

		timer.setAveragedFPSRate(1);	// Once every X seconds

		// Load some stuff in and setup simple scene manager
		_initSceneManager();
	}

	void CApplication::onStart(void)
	{
	}

	void CApplication::onStop(void)
	{

	}

	bool CApplication::onUpdate(void)
	{
		// Get pointers to needed managers
		SCInputManager* pInputManager = SCInputManager::getPointer();
		SCResourceManager* pRM = SCResourceManager::getPointer();

		// Timer delta
		timer.update();

		// Update and render scene manager to a framebuffer
		mSceneManagerSimple.mCamera.update();
		mSceneManagerSimple.render();

		// Render the debug shadow map onto the screen
//		CResourceDepthbuffer* pDepthbuffer = pRM->getDepthbuffer("X:shadows");
//		pDepthbuffer->renderToBackbuffer(pWindow->getWidth() - 512, 0, 512, 512);

		float fInc = timer.getSecondsPast();

		SCGUIManager* pGUIMan = SCGUIManager::getPointer();

		// Update line entity
		CSMEntityLine* pEntityLine = mSceneManagerSimple.getEntityLine("line");

		// Translation
		if (pInputManager->key.pressed(KC_W))	pEntityLine->translateWorld(glm::vec3(0.0f, 0.0f, 1.0f * fInc));
		if (pInputManager->key.pressed(KC_S))	pEntityLine->translateWorld(glm::vec3(0.0f, 0.0f, -1.0f * fInc));
		if (pInputManager->key.pressed(KC_A))	pEntityLine->translateWorld(glm::vec3(-1.0f * fInc, 0.0f, 0.0f));
		if (pInputManager->key.pressed(KC_D))	pEntityLine->translateWorld(glm::vec3(1.0f * fInc, 0.0f, 0.0f));
		if (pInputManager->key.pressed(KC_R))	pEntityLine->translateWorld(glm::vec3(0.0f, 1.0f * fInc, 0.0f));
		if (pInputManager->key.pressed(KC_F))	pEntityLine->translateWorld(glm::vec3(0.0f, -1.0f * fInc, 0.0f));
		// Scale
		if (pInputManager->key.pressed(KC_T))
		{
			glm::vec3 v3EntityScale = pEntityLine->getScale();
			v3EntityScale.x += fInc;
			v3EntityScale.y += fInc;
			v3EntityScale.z += fInc;
			pEntityLine->setScale(v3EntityScale);
		}
		if (pInputManager->key.pressed(KC_G))
		{
			glm::vec3 v3EntityScale = pEntityLine->getScale();
			v3EntityScale.x -= fInc;
			v3EntityScale.y -= fInc;
			v3EntityScale.z -= fInc;
			pEntityLine->setScale(v3EntityScale);
		}
		// Rotation
		if (pInputManager->key.pressed(KC_Y))
		{

		}
		if (pInputManager->key.pressed(KC_H))
		{

		}
		if (pInputManager->key.pressed(KC_U))
		{

		}
		if (pInputManager->key.pressed(KC_J))
		{

		}
		if (pInputManager->key.pressed(KC_I))
		{

		}
		if (pInputManager->key.pressed(KC_K))
		{

		}

		// Update entity rotation
		for (int i = 0; i < 100; ++i)
		{
			// Temp rotation
			_mvEntityRot[i].x += float(i) * timer.getSecondsPast();
			_mvEntityRot[i].y += float(i) * timer.getSecondsPast();
			_mvEntityRot[i].z += float(i) * timer.getSecondsPast();

			std::string strEntity = "entity_" + std::to_string(i);
			CSMEntityTriangle* pEntity = mSceneManagerSimple.getEntityTriangle(strEntity);
			pEntity->setRotation(_mvEntityRot[i].x, _mvEntityRot[i].y, _mvEntityRot[i].z);
		}

		// Toggle shadows
		if (pInputManager->key.once(KC_F3))
			mSceneManagerSimple.setShadowsEnabled(!mSceneManagerSimple.getShadowsEnabled());
		// Escape key to exit
		if (pInputManager->key.pressed(KC_ESCAPE))
			return false;
		// Toggle fullscreen
		if (pInputManager->key.once(KC_F1))
		{
			SCWindow::getPointer()->toggleFullscreen();
		}
		// Toggle vertical sync
		if (pInputManager->key.once(KC_F2))
			SCWindow::getPointer()->setVsync(!SCWindow::getPointer()->getVSyncEnabled());
		return true;
	}

	void CApplication::_initSceneManager(void)
	{
		// Set camera mode
//		mSceneManagerSimple.mCamera.setModeOrbit(
//			glm::vec3(0.0f, 0.0f, 0.0f),	// Orbit point
//			1.5f,							// Min distance from point
//			10.0f,							// Current distance from point
//			100.0f,							// Max distance from point
//			0.005f,							// Sensitivity X
//			0.005f,							// Sensitivity Y
//			0.005f,							// Sensitivity wheel
//			true);							// Limit Y angle to > 0.0f

		mSceneManagerSimple.mCamera.setModeFPS(
		glm::vec3(0.0f, 5.0f, -5.0f),	// Initial position
		90.0f,	// Initial yaw in degrees
		-25.0f,	// Initial pitch in degrees
		0.5f,	// Mouse X sensitivity
		0.2f,	// Mouse Y sensitivity
		1.0f,	// WS sensitivity
		1.0f,	// AD sensitivity
		1.0f,	// RF sensitivity
		10.0f);	// Shift key multiplier

		// Create needed triangles
		SCResourceManager* pRM = SCResourceManager::getPointer();
		// Cube
		CResourceTriangle* pTri = pRM->addTriangle("cube");
		pTri->addFromFile("data/DemoSceneManager/geometry/cube.geom", true);
		// Point lights to show where they are
		pTri = pRM->addTriangle("icosphere_radius_0.01");
		pTri->addFromFile("data/DemoSceneManager/geometry/icosphere_radius_0.01.geom", true);
		// Ground plane
		pTri = pRM->addTriangle("groundplane");
		pTri->addFromFile("data/DemoSceneManager/geometry/ground_plane_200x200x1.geom", true);

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
		pEntity->setWorldPosition(glm::vec3(0.0f, 0.0f, 0.0f));

		for (int i = 0; i < 100; ++i)
		{
			// Temp rotation
			_mvEntityRot[i].x += float(i) * 1.0f;
			_mvEntityRot[i].y += float(i) * 1.0f;
			_mvEntityRot[i].z += float(i) * 1.0f;

			std::string strEntity = "entity_" + std::to_string(i);
			pEntity = mSceneManagerSimple.addEntityTriangle(strEntity, "cube", "mat_cubes");
			pEntity->setWorldPosition(glm::vec3(randf(-19.0f, 19.0f), 0.5f, randf(-19.0f, 19.0f)));
			pEntity->setRotation(_mvEntityRot[i].x, _mvEntityRot[i].y, _mvEntityRot[i].z);
		}
		// Ground plane
		pEntity = mSceneManagerSimple.addEntityTriangle("groundplane", "groundplane", "mat_groundplane");

		// Directional light settings
		mSceneManagerSimple.setDirectionalLightColour(glm::vec3(1, 1, 1));
		mSceneManagerSimple.setDirectionalLightDir(glm::vec3(1, -1, 1));
		mSceneManagerSimple.setDirectionalLightProjection();

		// Set point light settings
		mSceneManagerSimple.miNumPointLights = 0;
		mSceneManagerSimple.mvLightPoint[0].mvPosition = glm::vec3(2.0f, 2.0f, 10.0f);
		mSceneManagerSimple.mvLightPoint[0].mvColour = glm::vec3(0.7f, 0.7f, 0.5f);
		mSceneManagerSimple.mvLightPoint[1].mvPosition = glm::vec3(-2.0f, 2.0f, 2.0f);
		mSceneManagerSimple.mvLightPoint[1].mvColour = glm::vec3(0.0f, 1.0f, 0.0f);
		mSceneManagerSimple.mvLightPoint[2].mvPosition = glm::vec3(2.0f, -2.0f, 2.0f);
		mSceneManagerSimple.mvLightPoint[2].mvColour = glm::vec3(0.0f, 0.0f, 1.0f);
		mSceneManagerSimple.mvLightPoint[3].mvPosition = glm::vec3(2.0f, 2.0f, -2.0f);
		mSceneManagerSimple.mvLightPoint[3].mvColour = glm::vec3(1.0f, 1.0f, 1.0f);
		// Add icosphere_radius_0.01.geom to position of each light
		for (int i = 0; i < mSceneManagerSimple.miNumPointLights; i++)
		{
			std::string strEntity = "point_light_" + std::to_string(i);
			pEntity = mSceneManagerSimple.addEntityTriangle(strEntity, "icosphere_radius_0.01", "mat_white");
			pEntity->setWorldPosition(mSceneManagerSimple.mvLightPoint[i].mvPosition);
		}

		// Now we're done with adding triangle entities.
		// Let's add some line entities
		CResourceLine* pLine = pRM->addLine("line");
		pLine->setDrawModeAsLineList();
		CResourceLine::Vertex lineVertex;
		// + Y
		lineVertex.colour = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
		lineVertex.position = glm::vec3(0.0f, 0.0f, 0.0f);		pLine->addLinePoint(lineVertex);
		lineVertex.position = glm::vec3(0.0f, 1.0f, 0.0f);		pLine->addLinePoint(lineVertex);
		// + X
		lineVertex.colour = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
		lineVertex.position = glm::vec3(0.0f, 0.0f, 0.0f);		pLine->addLinePoint(lineVertex);
		lineVertex.position = glm::vec3(1.0f, 0.0f, 0.0f);		pLine->addLinePoint(lineVertex);
		// + Z
		lineVertex.colour = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
		lineVertex.position = glm::vec3(0.0f, 0.0f, 0.0f);		pLine->addLinePoint(lineVertex);
		lineVertex.position = glm::vec3(0.0f, 0.0f, 1.0f);		pLine->addLinePoint(lineVertex);
		pLine->update();

		CSMEntityLine* pEntityLine = mSceneManagerSimple.addEntityLine("line", "line", "X:default_white");
		pEntityLine->setWorldPosition(glm::vec3(0.0f, 1.0f, 0.0f));
	}
}