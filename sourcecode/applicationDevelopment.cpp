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
		pVB->addQuad(
			glm::vec2(100, 100),	// Position
			glm::vec2(512, 512),	// Dims
			glm::vec4(1, 1, 1, 1));	// Colour
		pVB->update();

		// Create vertex/fragment shader
		ResourceShader* pShader = pRM->addShader("default", "shaders/default.vs", "shaders/default.fs");
		
		// Load in a texture
		ResourceTexture2D* pTex = pRM->addTexture2D("default", "textures/default.png");

		// Create a framebuffer
		ResourceFramebuffer* pFB = pRM->addFramebuffer("default", Window::getPointer()->getWidth(), Window::getPointer()->getHeight());
	}

	void ApplicationDevelopment::onStart(void)
	{
	}

	void ApplicationDevelopment::onStop(void)
	{

	}

	bool ApplicationDevelopment::onUpdate(void)
	{
		// Get resources
		ResourceManager* pRM = ResourceManager::getPointer();		// Resource manager
		ResourceVertexbuffer* pVB = pRM->getVertexbuffer("TEST");	// Vertex buffer
		ResourceTexture2D* pTex = pRM->getTexture2D("default");		// Texture
		ResourceShader* pShader = pRM->getShader("default");		// Shader
		ResourceFramebuffer* pFB = pRM->getFramebuffer("default");	// Framebuffer

		// Setup a camera
		Camera camera;
		camera.setProjectionAsOrthographic(Window::getPointer()->getWidth(), Window::getPointer()->getHeight());

		// Recreate vertex buffer for fun
		static Timer timer;
		timer.update();
		static float fInc = 0.0f;
		fInc += timer.getSecondsPast() * 100.0f;
		if (fInc > 2048)
			fInc -= 2048;
		pVB->removeGeom();
		pVB->addQuad(
			glm::vec2(100, 100),	// Pos
			glm::vec2(fInc, fInc),	// Dims
			glm::vec4(1,1,1,1));	// Colour
		pVB->update();

		// Draw stuff
		pShader->bind();
		pShader->setMat4("transform", camera.matrixProjection);
		pTex->bind(0);

		pVB->draw(false);


		pTex->unbind(0);
		pShader->unbind();
//		float fWidth = (float)Window::getPointer()->getWidth();
//		float fHeight = (float)Window::getPointer()->getHeight();
//		glm::mat4 mat = glm::ortho(0.0f, fWidth, fHeight, 0.0f, -1.0f, 1.0f);
//		pShader->setMat4("transform", mat);


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