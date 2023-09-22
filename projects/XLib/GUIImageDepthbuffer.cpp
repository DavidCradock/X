#include "PCH.h"
#include "GUIImageDepthbuffer.h"
#include "GUIManager.h"
#include "GUITooltip.h"
#include "singletons.h"
#include "resourceManager.h"
#include "input.h"

namespace X
{
	CGUIImageDepthbuffer::CGUIImageDepthbuffer()
	{
		mpTooltip = new CGUITooltip;
	}

	CGUIImageDepthbuffer::~CGUIImageDepthbuffer()
	{
		delete mpTooltip;
	}

	void CGUIImageDepthbuffer::render(void* pParentContainer, const std::string& strFramebufferToSampleFrom)
	{
		CGUIContainer* pContainer = (CGUIContainer*)pParentContainer;

		// Get required resources needed to render
		CResourceVertexBufferCPT* pVB = x->pResource->getVertexBufferCPT("X:default");
		CResourceShader* pShader = x->pResource->getShader("X:depthbuffer_debug");

		pShader->bind();

		// Setup the projection matrix as orthographic
		CMatrix matProjection;
		matProjection.setProjectionOrthographic(0.0f, float(x->pWindow->getWidth()), 0.0f, float(x->pWindow->getHeight()), -1.0f, 1.0f);
		pShader->setMat4("transform", matProjection);

		// Tell OpenGL, for each sampler, to which texture unit it belongs to
		pShader->setInt("texture0", 0);

		glDisable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);

		// Get textures
		CResourceDepthbuffer* pTexColour = x->pResource->getDepthbuffer(_mstrDBname);

		// Bind textures
		pTexColour->bindAsTexture(0);

		// Render the image
		pVB->removeGeom();
		pVB->addQuad2D(
			CVector2f(pContainer->mfPositionX + mfPositionX, pContainer->mfPositionY + mfPositionY),	// Position
			CVector2f(mfWidth, mfHeight),	// Dimensions
			mColour,						// Vertex colour
			CVector2f(0.0f, 0.0f),
			CVector2f(1.0f, 0.0f),
			CVector2f(1.0f, 1.0f),
			CVector2f(0.0f, 1.0f));

		pVB->update();
		pVB->render();

		pTexColour->unbindTexture();	// Unbind textures
		pShader->unbind();	// Unbind the GUI shader
//		glDisable(GL_BLEND);

	}

	void CGUIImageDepthbuffer::update(void* pParentContainer, bool bParentContainerAcceptingMouseClicks)
	{
		// Update this object's tooltip
		CVector2f vMousePos = x->pInput->mouse.getCursorPos();
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
		mpTooltip->update(pParentContainer, (CGUIBaseObject*)this, bMouseOver);
	}
}