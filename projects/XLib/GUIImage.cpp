#include "PCH.h"
#include "GUIImage.h"
#include "GUIManager.h"
#include "resourceManager.h"
#include "window.h"
#include "input.h"
#include "GUITooltip.h"

namespace X
{
	CGUIImage::CGUIImage()
	{
		mpTooltip = new CGUITooltip;
		_mbImageIsFromFile = false;
	}

	CGUIImage::~CGUIImage()
	{
		delete mpTooltip;
	}

	void CGUIImage::render(void* pParentContainer, const std::string& strFramebufferToSampleFrom)
	{
		CGUIContainer* pContainer = (CGUIContainer*)pParentContainer;

		// Get required resources needed to render
		SCResourceManager* pRM = SCResourceManager::getPointer();
		SCWindow* pWindow = SCWindow::getPointer();
		CResourceTriangle* pTri = pRM->getTriangle("X:gui");
		CResourceShader* pShader = pRM->getShader("X:pos_col_tex");

		pShader->bind();

		// Setup the projection matrix as orthographic
		glm::mat4 matProjection = glm::ortho(0.0f, float(pWindow->getWidth()), float(pWindow->getHeight()), 0.0f, -1.0f, 1.0f);
		pShader->setMat4("transform", matProjection);

		// Tell OpenGL, for each sampler, to which texture unit it belongs to
		pShader->setInt("texture0", 0);

		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);

		// Get textures
		CResourceTexture2DFromFile* pTexFileColour = 0;
		CResourceTexture2DFromImage* pTexImageColour = 0;
		if (_mbImageIsFromFile)
		{
			pTexFileColour = pRM->getTexture2DFromFile(_mstrTextureResourceName);
			// Bind textures
			pTexFileColour->bind(0);
		}
		else
		{
			pTexImageColour = pRM->getTexture2DFromImage(_mstrTextureResourceName);
			// Bind textures
			pTexImageColour->bind(0);
		}

		// Render the image
		pTri->removeGeom();
		pTri->addQuad2D(
			glm::vec2(pContainer->mfPositionX + mfPositionX, pContainer->mfPositionY + mfPositionY),	// Position
			glm::vec2(mfWidth, mfHeight),			// Dimensions
			mColour.get(),							// Vertex colour
			glm::vec2(0.0f, 1.0f),
			glm::vec2(1.0f, 1.0f),
			glm::vec2(1.0f, 0.0f),
			glm::vec2(0.0f, 0.0f));

		pTri->update();
		pTri->draw();

		// Unbind textures
		if (_mbImageIsFromFile)
			pTexFileColour->unbindAll();
		else
			pTexImageColour->unbindAll();

		pShader->unbind();	// Unbind the GUI shader
		glDisable(GL_BLEND);

	}

	void CGUIImage::update(void* pParentContainer, bool bParentContainerAcceptingMouseClicks)
	{
		// Update this object's tooltip
		SCInputManager* pInput = SCInputManager::getPointer();
		glm::vec2 vMousePos = pInput->mouse.getCursorPos();
		CGUIContainer* pContainer = (CGUIContainer*)pParentContainer;
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
		CGUITooltip* pTooltip = (CGUITooltip*)mpTooltip;
		pTooltip->update(pParentContainer, (CGUIBaseObject*)this, bMouseOver);

	}

}