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

		ResourceManager* pRM = ResourceManager::getPointer();

		//pVB->convertObj("geometry/icosphere_radius_0.01.obj");	// Convert .obj file to .geom file

		// Create needed vertex buffers
		// Cube
		ResourceVertexbuffer* pVB = pRM->addVertexbuffer("cube");
		pVB->addFromFile("geometry/cube.geom", true);
		// Blob
		pVB = pRM->addVertexbuffer("blob");
		pVB->addFromFile("geometry/blob.geom", true);
		// Point lights to show where they are
		pVB = pRM->addVertexbuffer("icosphere_radius_0.01");
		pVB->addFromFile("geometry/icosphere_radius_0.01.geom", true);

		// Load in a textures
		// Cubes
		pRM->addTexture2D("textures/cube_BaseColor.png", "textures/cube_BaseColor.png", true);
		pRM->addTexture2D("textures/cube_Roughness.png", "textures/cube_Roughness.png", true);
		pRM->addTexture2D("textures/cube_Normal.png", "textures/cube_Normal.png", true);
		// Blob
		pRM->addTexture2D("textures/blob_diffuse.png", "textures/blob_diffuse.png", true);
		pRM->addTexture2D("textures/blob_roughness.png", "textures/blob_roughness.png", true);
		pRM->addTexture2D("textures/blob_normal.png", "textures/blob_normal.png", true);
		// Point light entities
		pRM->addTexture2D("textures/white.png", "textures/white.png", true);

		// Create a framebuffer
		ResourceFramebuffer* pFB = pRM->addFramebuffer("default", Window::getPointer()->getWidth(), Window::getPointer()->getHeight());

		// Create some fonts using the resource manager
//		pRM->buildFontFiles("fonts/arial", "arial", 26, true, false, false, false, false);

		// Add some fonts
		pRM->addFont("arial_26", "fonts/arial_26");

		// Scene manager
		mSceneManagerSimple.mCamera.setModeOrbit();	// Use defaults
		SceneManagerEntityVertexbuffer* pEnitity = mSceneManagerSimple.addEntityVertexbuffer("centre", "blob", 0.05f, "textures/blob_diffuse.png", "textures/blob_roughness.png", 0.5f, "textures/blob_normal.png");
//		pEnitity->matrixWorld = glm::translate(pEnitity->matrixWorld, glm::vec3(randf(-25.0f, 25.0f), randf(-25.0f, 25.0f), randf(-25.0f, 25.0f)));
		for (int i = 0; i < 1000; ++i)
		{
			std::string strEntity = "entity_" + std::to_string(i);
			pEnitity = mSceneManagerSimple.addEntityVertexbuffer(strEntity, "cube", 0.05f, "textures/cube_BaseColor.png", "textures/cube_Roughness.png", 0.5f, "textures/cube_Normal.png");
			pEnitity->matrixWorld = glm::translate(pEnitity->matrixWorld, glm::vec3(randf(-25.0f, 25.0f), randf(-25.0f, 25.0f), randf(-25.0f, 25.0f)));
		}
		// Set point light settings
		mSceneManagerSimple.miNumPointLights = 4;
		mSceneManagerSimple.mvLightPoint[0].mvPosition = glm::vec3(2.0f, 2.0f, 2.0f);
		mSceneManagerSimple.mvLightPoint[0].mvColour = glm::vec3(1.0f, 0.0f, 0.0f);
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
			pEnitity = mSceneManagerSimple.addEntityVertexbuffer(
				strEntity, // Enitity name
				"icosphere_radius_0.01", // Vertex buffer name
				0.05f, // Ambient strength
				"textures/white.png", "textures/white.png", 0.5f, "X:default_normal", "textures/white.png");
			pEnitity->matrixWorld = glm::translate(pEnitity->matrixWorld, mSceneManagerSimple.mvLightPoint[i].mvPosition);
		}
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

//		mSceneManagerSimple.render();
//		mSceneManagerSimple.mCamera.update();

		// Render some text
		std::string strFPS = "FPS: ";
		strFPS += std::format("{:.2f}", timer.getFPSAveraged());
		pRM->getFont("arial_26")->print(strFPS, 0, 0, Window::getPointer()->getWidth(), Window::getPointer()->getHeight(), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

		// Temp render lots of text to benchmark and improve performance
		// Framerate in before optimization: release build fullscreen= 950fps
		// Framerate in after optimization:  release build fullscreen= 1100 before removing normal, 1140 after removing normal
		std::string strLines;
		strLines = "This is a line of text which contains about 280 characters to render. This is to test performance aka profiling with the hope of being able to increase performance slightly. I'm just trying to write some random stuff here to fill up the screen so we can really push the font resource.";

		ResourceFont* pFont = pRM->getFont("arial_26");
		int iDims[2];
		iDims[0] = Window::getPointer()->getWidth();
		iDims[1] = Window::getPointer()->getHeight();
		int iYpos = 30;
		for (int i = 0; i < 69; i++)
		{
			pFont->print(strLines, 100, iYpos, iDims[0], iDims[1]);
			iYpos += 30;
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
}