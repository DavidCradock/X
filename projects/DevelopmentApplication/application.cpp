#include "application.h"

namespace X
{
	void Application::initOnce(void)
	{
		ResourceManager* pRM = ResourceManager::getPointer();

		timer.setAveragedFPSRate(1);	// Once every X seconds

		// Load some stuff in and setup simple scene manager
		_initSceneManager();

		// Create some fonts using the resource manager
		pRM->buildFontFiles("data/DevApp/fonts/tahoma", "tahoma", 200, true, false, false, false, false);

		// Add some fonts
		pRM->addFont("arial_26", "data/DevApp/fonts/arial_26");
		pRM->addFont("tahoma_200", "data/DevApp/fonts/tahoma_200");

	}

	void Application::onStart(void)
	{
	}

	void Application::onStop(void)
	{

	}

	bool Application::onUpdate(void)
	{
		timer.update();

		// Get resources
		ResourceManager* pRM = ResourceManager::getPointer();		// Resource manager

		// Timer delta
		static float fInc = 0.0f;
		fInc += timer.getSecondsPast() * kPi * 0.1f;

		// Update and render simple scene manager
		mSceneManagerSimple.mCamera.update();
		mSceneManagerSimple.render();

		// Render some text
		std::string strFPS = "FPS: ";
		strFPS += std::format("{:.2f}", timer.getFPSAveraged());
		pRM->getFont("arial_26")->print(strFPS, 0, 0, Window::getPointer()->getWidth(), Window::getPointer()->getHeight(), 1.0f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

		/*		// Render more text with various scaling values
				float fYpos = 30.0f;
				for (float fScale = 0.1f; fScale < 1.0f; fScale += 0.1f)
				{
					std::string strText = "Text scale: " + std::format("{:.1f}", fScale) + " hmm, OK.";
					pRM->getFont("tahoma_200")->print(strText, 0, fYpos, Window::getPointer()->getWidth(), Window::getPointer()->getHeight(), fScale, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
					fYpos += 100.0f * fScale;
				}
				for (float fScale = 1.0f; fScale < 3.5f; fScale += 0.5f)
				{
					std::string strText = "Text scale: " + std::format("{:.1f}", fScale) + " hmm, OK.";
					pRM->getFont("tahoma_200")->print(strText, 0, fYpos, Window::getPointer()->getWidth(), Window::getPointer()->getHeight(), fScale, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
					fYpos += 100.0f * fScale;
				}
		*/
		// Escape key to exit
		InputManager* pInputManager = InputManager::getPointer();
		if (pInputManager->key.pressed(KC_ESCAPE))
			return false;
		// Toggle fullscreen
		if (pInputManager->key.once(KC_F1))
		{
			Window::getPointer()->toggleFullscreen();
		}
		// Toggle vertical sync
		if (pInputManager->key.once(KC_F2))
			Window::getPointer()->setVsync(!Window::getPointer()->getVSyncEnabled());
		return true;
	}

	void Application::_initSceneManager(void)
	{
		// Convert and create geom files
		//ResourceVertexbuffer vb;
		//vb.convertObj("data/DevApp/geometry/cube.obj");	// Convert .obj file to .geom file
		//vb.convertObj("data/DevApp/geometry/blob.obj");	// Convert .obj file to .geom file
		//vb.convertObj("data/DevApp/geometry/icosphere_radius_0.01.obj");	// Convert .obj file to .geom file

		// Create needed triangles
		ResourceManager* pRM = ResourceManager::getPointer();
		// Cube
		ResourceTriangle* pTri = pRM->addTriangle("cube");
		pTri->addFromFile("data/DevApp/geometry/cube.geom", true);
		// Blob
		pTri = pRM->addTriangle("blob");
		pTri->addFromFile("data/DevApp/geometry/blob.geom", true);
		// Point lights to show where they are
		pTri = pRM->addTriangle("icosphere_radius_0.01");
		pTri->addFromFile("data/DevApp/geometry/icosphere_radius_0.01.geom", true);
		// Ground plane
		pTri = pRM->addTriangle("groundplane");
		pTri->addGroundplane(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(100.0f, 100.0f));
		pTri->update();

		// Load in a textures
		// Cubes
		pRM->addTexture2D("data/DevApp/textures/cube_BaseColor.png", "data/DevApp/textures/cube_BaseColor.png", true);
		pRM->addTexture2D("data/DevApp/textures/cube_Roughness.png", "data/DevApp/textures/cube_Roughness.png", true);
		pRM->addTexture2D("data/DevApp/textures/cube_Normal.png", "data/DevApp/textures/cube_Normal.png", true);
		// Blob
//		pRM->addTexture2D("data/DevApp/textures/blob_diffuse.png", "data/DevApp/textures/blob_diffuse.png", true);
//		pRM->addTexture2D("data/DevApp/textures/blob_roughness.png", "data/DevApp/textures/blob_roughness.png", true);
//		pRM->addTexture2D("data/DevApp/textures/blob_normal.png", "data/DevApp/textures/blob_normal.png", true);
		// Point light entities
		pRM->addTexture2D("data/DevApp/textures/groundplane.png", "data/DevApp/textures/groundplane.png", true);

		// Scene manager
		mSceneManagerSimple.mCamera.setModeOrbit();	// Use defaults

		// Create materials
		mSceneManagerSimple.addMaterial("mat_cubes", 0.05f, "data/DevApp/textures/cube_BaseColor.png", "data/DevApp/textures/cube_Roughness.png", 0.25f, "data/DevApp/textures/cube_Normal.png", "X:default_emission");
		mSceneManagerSimple.addMaterial("mat_groundplane", 0.5f, "data/DevApp/textures/groundplane.png", "X:default_roughness", 0.25f);
		mSceneManagerSimple.addMaterial("mat_white", 0.05f, "X:default_white", "X:default_white", 0.5f, "X:default_normal", "X:default_white");

		SMEntityTriangle* pEntity = mSceneManagerSimple.addEntityTriangle("centre", "cube", "mat_cubes");
		pEntity->matrixWorld = glm::translate(pEntity->matrixWorld, glm::vec3(0.0f, 0.5f, 0.0f));
		for (int i = 0; i < 100; ++i)
		{
			std::string strEntity = "entity_" + std::to_string(i);
			pEntity = mSceneManagerSimple.addEntityTriangle(strEntity, "cube", "mat_cubes");
			pEntity->matrixWorld = glm::translate(pEntity->matrixWorld, glm::vec3(randf(-25.0f, 25.0f), randf(0.5f, 0.5f), randf(-25.0f, 25.0f)));
		}
		// Ground plane
		pEntity = mSceneManagerSimple.addEntityTriangle("groundplane", "groundplane", "mat_groundplane");

		// Directional light settings
		mSceneManagerSimple.mvLightDirectional.mvDirection = glm::vec3(0.0f, -1.0f, 0.0f);
		mSceneManagerSimple.mvLightDirectional.mvColour = glm::vec3(0.5f, 0.5f, 0.5f);
		// Set point light settings
		mSceneManagerSimple.miNumPointLights = 1;
		mSceneManagerSimple.mvLightPoint[0].mvPosition = glm::vec3(2.0f, 2.0f, 10.0f);
		mSceneManagerSimple.mvLightPoint[0].mvColour = glm::vec3(0.7f, 0.7f, 0.5f);
		mSceneManagerSimple.mvLightPoint[1].mvPosition = glm::vec3(-2.0f, 2.0f, 2.0f);
		mSceneManagerSimple.mvLightPoint[1].mvColour = glm::vec3(0.0f, 1.0f, 0.0f);
		mSceneManagerSimple.mvLightPoint[2].mvPosition = glm::vec3(2.0f, -2.0f, 2.0f);
		mSceneManagerSimple.mvLightPoint[2].mvColour = glm::vec3(0.0f, 0.0f, 1.0f);
		mSceneManagerSimple.mvLightPoint[3].mvPosition = glm::vec3(2.0f, 2.0f, -2.0f);
		mSceneManagerSimple.mvLightPoint[3].mvColour = glm::vec3(1.0f, 1.0f, 1.0f);
		mSceneManagerSimple.mvLightDirectional.mvColour = glm::vec3(1.0f, 1.0f, 1.0f);
		// Add icosphere_radius_0.01.geom to position of each light
		for (int i = 0; i < mSceneManagerSimple.miNumPointLights; i++)
		{
			std::string strEntity = "point_light_" + std::to_string(i);
			pEntity = mSceneManagerSimple.addEntityTriangle(strEntity, "icosphere_radius_0.01", "mat_white");
			pEntity->matrixWorld = glm::translate(pEntity->matrixWorld, mSceneManagerSimple.mvLightPoint[i].mvPosition);
		}

		// Now we're done with adding triangle entities.
		// Let's add some line entities
		ResourceLine* pVertexBufferLine = pRM->addLine("line_up");
		ResourceLine::Vertex lineVertex;
		lineVertex.colour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		lineVertex.texCoord.x = 0.0f;
		lineVertex.texCoord.y = 0.0f;
		lineVertex.position = glm::vec3(0.0f, 0.0f, 0.0f);
		pVertexBufferLine->addLinePoint(lineVertex);
		lineVertex.position = glm::vec3(0.0f, 1.0f, 0.0f);
		pVertexBufferLine->addLinePoint(lineVertex);
		pVertexBufferLine->update();

		for (float fX = -50.0f; fX < 50.0f; fX += 5.0f)
		{
			for (float fZ = -50.0f; fZ < 50.0f; fZ += 5.0f)
			{
				std::string strEntityName = "line_" + std::to_string(fX) + ":" + std::to_string(fZ);
				SMEntityLine* pEntityLine = mSceneManagerSimple.addEntityLine(strEntityName, "line_up", "X:default_white");
				pEntityLine->matrixWorld = glm::translate(pEntityLine->matrixWorld, glm::vec3(fX, 0.0f, fZ));
			}
		}
	}
}