#include "PCH.h"
#include "applicationDevelopment.h"
#include "audioManager.h"
#include "log.h"
#include "utilities.h"
#include "input.h"
#include "window.h"
#include "resourceManager.h"

namespace X
{
	void ApplicationDevelopment::initOnce(void)
	{
		ResourceManager* pRM = ResourceManager::getPointer();
		ResourceVertexbuffer* pVB = pRM->addVertexbuffer("TEST");
		pVB->addQuad(
			glm::vec2(100, 100),	// Position
			glm::vec2(100, 100),	// Dims
			glm::vec4(1, 1, 1, 1));	// Colour
		pVB->update();
	}

	void ApplicationDevelopment::onStart(void)
	{
	}

	void ApplicationDevelopment::onStop(void)
	{

	}

	bool ApplicationDevelopment::onUpdate(void)
	{

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