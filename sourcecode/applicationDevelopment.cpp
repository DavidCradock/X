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

		// Create a vertex buffer
		ResourceVertexbuffer* pVB = pRM->addVertexbuffer("TEST");
//		pVB->convertObj("obj/susan.obj");
		pVB->addFromFile("geometry/susan.geom");
		pVB->update();

		// Create vertex/fragment shader
		ResourceShader* pShader = pRM->addShader("default", "shaders/default.vs", "shaders/default.fs");
		
		// Load in a texture
		ResourceTexture2D* pTex = pRM->addTexture2D("textures/susan_DefaultMaterial_BaseColor.png", "textures/susan_DefaultMaterial_BaseColor.png", true);

		// Create a framebuffer
		ResourceFramebuffer* pFB = pRM->addFramebuffer("default", Window::getPointer()->getWidth(), Window::getPointer()->getHeight());

		// Create some fonts using the resource manager
//		pRM->buildFontFiles("fonts/arial", "arial", 26, true, false, false, false, false);

		// Add some fonts
		pRM->addFont("arial_26", "fonts/arial_26");
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
		ResourceVertexbuffer* pVB = pRM->getVertexbuffer("TEST");	// Vertex buffer
		ResourceTexture2D* pTex = pRM->getTexture2D("textures/susan_DefaultMaterial_BaseColor.png");		// Texture
		ResourceShader* pShader = pRM->getShader("default");		// Shader
		ResourceFramebuffer* pFB = pRM->getFramebuffer("default");	// Framebuffer

		// Timer delta
		
		static float fInc = 0.0f;
		fInc += timer.getSecondsPast() * kPi * 0.1f;

		// Setup a camera
		Camera camera;
//		camera.setProjectionAsOrthographic(Window::getPointer()->getWidth(), Window::getPointer()->getHeight());
		camera.setProjectionAsPerspective(55.0f, (float)Window::getPointer()->getWidth(), (float)Window::getPointer()->getHeight(), 0.1f, 1000.0f);
		camera.setViewAsLookat(glm::vec3(sinf(fInc)*2.0f, .0f, cosf(fInc)*2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		// Draw stuff
		pShader->bind();
		pShader->setMat4("transform", camera.getViewProjectionMatrix());
		pTex->bind(0);
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		pVB->draw(false);
		pTex->unbind(0);
		pShader->unbind();

		// Render some text
		std::string strFPS = "FPS: ";
		strFPS += std::to_string(timer.getFPSAveraged());
		pRM->getFont("arial_26")->print(strFPS, 0, 0, Window::getPointer()->getWidth(), Window::getPointer()->getHeight(), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

		// Escape key to exit
		if (InputManager::getPointer()->key.pressed(KC_ESCAPE))
			return false;
		// Toggle fullscreen
		if (InputManager::getPointer()->key.once(KC_F1))
		{
			Window::getPointer()->toggleFullscreen();
		}
		return true;
	}
}