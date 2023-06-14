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
//		ResourceVertexbuffer* pVB = pRM->getVertexbuffer("TEST");	// Vertex buffer
//		ResourceTexture2D* pTexDiffuse = pRM->getTexture2D("textures/cube_BaseColor.png");
//		ResourceTexture2D* pTexRoughness = pRM->getTexture2D("textures/cube_Roughness.png");
//		ResourceTexture2D* pTexNormal = pRM->getTexture2D("textures/cube_Normal.png");
//		ResourceTexture2D* pTexEmission = pRM->getTexture2D("textures/cube_Normal.png");
//		ResourceShader* pShader = pRM->getShader("X:DRNE");			// Shader
//		ResourceFramebuffer* pFB = pRM->getFramebuffer("default");	// Framebuffer

		// Timer delta
		static float fInc = 0.0f;
		fInc += timer.getSecondsPast() * kPi * 0.1f;

		// Scene manager
		//mSceneManagerSimple.mCamera.setViewAsLookat(glm::vec3(sinf(fInc) * 5.0f, 0.0f, cosf(fInc) * 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		
		mSceneManagerSimple.render();
		mSceneManagerSimple.mCamera.update();

		// Setup a camera
//		Camera camera;
//		camera.setProjectionAsOrthographic(Window::getPointer()->getWidth(), Window::getPointer()->getHeight());
//		camera.setProjectionAsPerspective(55.0f, (float)Window::getPointer()->getWidth(), (float)Window::getPointer()->getHeight(), 0.1f, 1000.0f);
//		camera.setViewAsLookat(glm::vec3(sinf(fInc)*3.0f, 0.0f, cosf(fInc)*2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		// Draw stuff
//		pShader->bind();
		// Set view/projection matrix
//		pShader->setMat4("transform", camera.getViewProjectionMatrix());
		// Tell OpenGL, for each sampler, to which texture unit it belongs to
//		pShader->setInt("texture0", 0);
//		pShader->setInt("texture1", 1);
		// Bind each texture to each sampler unit
//		pTexDiffuse->bind(0);
//		pTexRoughness->bind(1);
		//pTexNormal->bind(2);
//		glDisable(GL_BLEND);
//		glEnable(GL_DEPTH_TEST);
//		pVB->draw(false);
//		pTexDiffuse->unbind(0);
//		pTexRoughness->unbind(1);
		//pTexNormal->unbind(2);
//		pShader->unbind();

		// Render some text
		std::string strFPS = "FPS: ";
		strFPS += std::format("{:.2f}", timer.getFPSAveraged());
		pRM->getFont("arial_26")->print(strFPS, 0, 0, Window::getPointer()->getWidth(), Window::getPointer()->getHeight(), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

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