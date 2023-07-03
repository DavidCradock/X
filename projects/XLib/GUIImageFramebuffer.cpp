#include "PCH.h"
#include "GUIImageFramebuffer.h"
#include "GUIManager.h"
#include "resourceManager.h"
#include "window.h"
#include "input.h"
#include "GUITooltip.h"

namespace X
{
	GUIImageFramebuffer::GUIImageFramebuffer()
	{
		mpTooltip = new GUITooltip;
	}

	GUIImageFramebuffer::~GUIImageFramebuffer()
	{
		delete mpTooltip;
	}

	void GUIImageFramebuffer::render(void* pParentContainer, const std::string& strFramebufferToSampleFrom)
	{
		GUIContainer* pContainer = (GUIContainer*)pParentContainer;

		// Get required resources needed to render
		ResourceManager* pRM = ResourceManager::getPointer();
		Window* pWindow = Window::getPointer();
		ResourceTriangle* pTri = pRM->getTriangle("X:gui");
		ResourceShader* pShader = pRM->getShader("X:pos_col_tex");

		pShader->bind();

		// Setup the projection matrix as orthographic
		glm::mat4 matProjection = glm::ortho(0.0f, float(pWindow->getWidth()), float(pWindow->getHeight()), 0.0f, -1.0f, 1.0f);
		pShader->setMat4("transform", matProjection);

		// Tell OpenGL, for each sampler, to which texture unit it belongs to
		pShader->setInt("texture0", 0);

		glDisable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);

		// Get textures
		ResourceFramebuffer* pTexColour = pRM->getFramebuffer(_mstrFBname);

		// Bind textures
		pTexColour->bindAsTexture(0);

		// Render the image
		pTri->removeGeom();
		pTri->addQuad2D(
			glm::vec2(pContainer->mfPositionX + mfPositionX, pContainer->mfPositionY + mfPositionY),	// Position
			glm::vec2(mfWidth, mfHeight),			// Dimensions
			mColour.get(),							// Vertex colour
			glm::vec2(0.0f, 0.0f),
			glm::vec2(1.0f, 0.0f),
			glm::vec2(1.0f, 1.0f),
			glm::vec2(0.0f, 1.0f));

		pTri->update();
		pTri->draw();

		pTexColour->unbindTexture();	// Unbind textures
		pShader->unbind();	// Unbind the GUI shader
//		glDisable(GL_BLEND);

	}

	void GUIImageFramebuffer::update(void* pParentContainer, bool bParentContainerAcceptingMouseClicks)
	{
		// Update this object's tooltip
		InputManager* pInput = InputManager::getPointer();
		glm::vec2 vMousePos = pInput->mouse.getCursorPos();
		GUIContainer* pContainer = (GUIContainer*)pParentContainer;
		bool bMouseOver = false;
		if (bParentContainerAcceptingMouseClicks)
		{
			// Determine if mouse cursor is over
			if (vMousePos.x > pContainer->mfPositionX + mfPositionX)
				if (vMousePos.x < pContainer->mfPositionX + mfPositionX + mfWidth)
					if (vMousePos.y > pContainer->mfPositionY + mfPositionY)
						if (vMousePos.y < pContainer->mfPositionY + mfPositionY + mfHeight)
							bMouseOver = true;
		}
		GUITooltip* pTooltip = (GUITooltip*)mpTooltip;
		pTooltip->update(pParentContainer, (GUIBaseObject*)this, bMouseOver);
	}
}