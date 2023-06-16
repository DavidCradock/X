#include "PCH.h"
#include "applicationDevelopment.h"
#include "audioManager.h"
#include "log.h"
#include "utilities.h"
#include "input.h"
#include "window.h"
#include "resourceManager.h"
#include "camera.h"


namespace X
{
	void ApplicationDevelopment::initOnce(void)
	{
		timer.setAveragedFPSRate(1);	// Once every X seconds

		// Load some stuff in and setup simple scene manager
		_initSceneManager();

		ResourceManager* pRM = ResourceManager::getPointer();

		// Create some fonts using the resource manager
//		pRM->buildFontFiles("fonts/tahoma", "tahoma", 100, true, false, false, false, false);

		// Add some fonts
		pRM->addFont("arial_26", "fonts/arial_26");
		pRM->addFont("tahoma_100", "fonts/tahoma_100");

	}

	void ApplicationDevelopment::onStart(void)
	{
	}

	void ApplicationDevelopment::onStop(void)
	{

	}

	bool ApplicationDevelopment::onUpdate(void)
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

		// Render more text with various scaling values
		float fYpos = 30.0f;
		for (float fScale = 0.1f; fScale < 1.1f; fScale += 0.1f)
		{
			std::string strText = "Text scale: " + std::format("{:.1f}", fScale) + " hmm, OK.";
			pRM->getFont("tahoma_100")->print(strText, 0, fYpos, Window::getPointer()->getWidth(), Window::getPointer()->getHeight(), fScale, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
			fYpos += 100.0f * fScale;
		}

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

	void ApplicationDevelopment::_initSceneManager(void)
	{
		// Convert and create geom files
		//ResourceVertexbuffer vb;
		//vb.convertObj("geometry/cube.obj");	// Convert .obj file to .geom file
		//vb.convertObj("geometry/blob.obj");	// Convert .obj file to .geom file
		//vb.convertObj("geometry/icosphere_radius_0.01.obj");	// Convert .obj file to .geom file

		// Create needed vertex buffers
		ResourceManager* pRM = ResourceManager::getPointer();
		// Cube
		ResourceVertexbuffer* pVB = pRM->addVertexbuffer("cube");
		pVB->addFromFile("geometry/cube.geom", true);
		// Blob
		pVB = pRM->addVertexbuffer("blob");
		pVB->addFromFile("geometry/blob.geom", true);
		// Point lights to show where they are
		pVB = pRM->addVertexbuffer("icosphere_radius_0.01");
		pVB->addFromFile("geometry/icosphere_radius_0.01.geom", true);
		// Ground plane
		pVB = pRM->addVertexbuffer("groundplane");
		pVB->addGroundplane(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(100.0f, 100.0f));
		pVB->update();

		// Load in a textures
		// Cubes
		pRM->addTexture2D("textures/cube_BaseColor.png", "textures/cube_BaseColor.png", true);
		pRM->addTexture2D("textures/cube_Roughness.png", "textures/cube_Roughness.png", true);
		pRM->addTexture2D("textures/cube_Normal.png", "textures/cube_Normal.png", true);
		// Blob
//		pRM->addTexture2D("textures/blob_diffuse.png", "textures/blob_diffuse.png", true);
//		pRM->addTexture2D("textures/blob_roughness.png", "textures/blob_roughness.png", true);
//		pRM->addTexture2D("textures/blob_normal.png", "textures/blob_normal.png", true);
		// Point light entities
		pRM->addTexture2D("textures/groundplane.png", "textures/groundplane.png", true);

		// Scene manager
		mSceneManagerSimple.mCamera.setModeOrbit();	// Use defaults
		SceneManagerEntityVertexbuffer* pEntity = mSceneManagerSimple.addEntityVertexbuffer("centre", "cube", 0.05f, "textures/cube_BaseColor.png", "textures/cube_Roughness.png", 0.25f, "textures/cube_Normal.png");
		pEntity->matrixWorld = glm::translate(pEntity->matrixWorld, glm::vec3(0.0f, 0.5f, 0.0f));
		for (int i = 0; i < 100; ++i)
		{
			std::string strEntity = "entity_" + std::to_string(i);
			pEntity = mSceneManagerSimple.addEntityVertexbuffer(strEntity, "cube", 0.05f, "textures/cube_BaseColor.png", "textures/cube_Roughness.png", 0.25f, "textures/cube_Normal.png");
			pEntity->matrixWorld = glm::translate(pEntity->matrixWorld, glm::vec3(randf(-25.0f, 25.0f), randf(0.5f, 0.5f), randf(-25.0f, 25.0f)));
		}
		// Ground plane
		pEntity = mSceneManagerSimple.addEntityVertexbuffer("groundplane", "groundplane", 0.5f, "textures/groundplane.png", "X:default_roughness", 0.25f);

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
			pEntity = mSceneManagerSimple.addEntityVertexbuffer(
				strEntity, // Enitity name
				"icosphere_radius_0.01", // Vertex buffer name
				0.05f, // Ambient strength
				"X:default_white", "X:default_white", 0.5f, "X:default_normal", "X:default_white");
			pEntity->matrixWorld = glm::translate(pEntity->matrixWorld, mSceneManagerSimple.mvLightPoint[i].mvPosition);
		}

		// Now we're done with adding vertexbuffer entities.
		// Let's add some vertexbufferLine entities
		ResourceVertexbufferLine* pVertexBufferLine = pRM->addVertexbufferLine("line_up");
		ResourceVertexbufferLine::Vertex lineVertex;
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
				SceneManagerEntityVertexbufferLine* pEntityLine = mSceneManagerSimple.addEntityVertexbufferLine(strEntityName, "line_up", "X:default_white");
				pEntityLine->matrixWorld = glm::translate(pEntityLine->matrixWorld, glm::vec3(fX, 0.0f, fZ));
			}
		}
	}
}