#include "PCH.h"
#include "resourceDepthbuffer.h"
#include "openGLExtensions.h"
#include "log.h"
#include "resourceManager.h"
#include "window.h"

namespace X
{

	CResourceDepthbuffer::CResourceDepthbuffer(unsigned int iWidth, unsigned int iHeight)
	{
		_muiWidth = iWidth;
		_muiHeight = iHeight;
		_muiFramebufferID = 0;
		_muiTextureID = 0;
		onGLContextCreated();
	}

	CResourceDepthbuffer::~CResourceDepthbuffer()
	{
		onGLContextToBeDestroyed();
	}

	void CResourceDepthbuffer::onGLContextCreated(void)
	{
		// Create the framebuffer object and bind it
		glGenFramebuffers(1, &_muiFramebufferID);
		glBindFramebuffer(GL_FRAMEBUFFER, _muiFramebufferID);

		// Create a texture which will be attached to the framebuffer object for colour information
		glGenTextures(1, &_muiTextureID);
		glBindTexture(GL_TEXTURE_2D, _muiTextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, _muiWidth, _muiHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _muiTextureID, 0);

		// No need for colour, as we only want depth information
		// So we need to tell OpenGL that we won't be rendering colour
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);

		ThrowIfTrue(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE, "CResourceDepthbuffer::onGLContextCreated() failed to create a complete framebuffer.");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}

	void CResourceDepthbuffer::onGLContextToBeDestroyed(void)
	{
		if (0 != _muiTextureID)
		{
			glDeleteTextures(1, &_muiTextureID);
			//_muiTextureID = 0;
		}
		if (0 != _muiFramebufferID)
		{
			glDeleteFramebuffers(1, &_muiFramebufferID);
			//_muiFramebufferID = 0;
		}
	}

	void CResourceDepthbuffer::bindAsRenderTarget(bool bClearbuffer) const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _muiFramebufferID);
		if (bClearbuffer)
			glClear(GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, _muiWidth, _muiHeight);
	}

	void CResourceDepthbuffer::unbindAsRenderTarget(void) const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		SCWindow* pWnd = SCWindow::getPointer();
		glViewport(0, 0, pWnd->getWidth(), pWnd->getHeight());
	}

	void CResourceDepthbuffer::bindAsTexture(unsigned int uiTextureUnit) const
	{
		switch (uiTextureUnit)
		{
		case 0:
			glActiveTexture(GL_TEXTURE0);
			break;
		case 1:
			glActiveTexture(GL_TEXTURE1);
			break;
		case 2:
			glActiveTexture(GL_TEXTURE2);
			break;
		case 3:
			glActiveTexture(GL_TEXTURE3);
			break;
		case 4:
			glActiveTexture(GL_TEXTURE4);
			break;
		case 5:
			glActiveTexture(GL_TEXTURE5);
			break;
		case 6:
			glActiveTexture(GL_TEXTURE6);
			break;
		case 7:
			glActiveTexture(GL_TEXTURE7);
			break;
		}
		glBindTexture(GL_TEXTURE_2D, _muiTextureID);
	}

	void CResourceDepthbuffer::unbindTexture(unsigned int uiTextureUnit) const
	{
		switch (uiTextureUnit)
		{
		case 0:
			glActiveTexture(GL_TEXTURE0);
			break;
		case 1:
			glActiveTexture(GL_TEXTURE1);
			break;
		case 2:
			glActiveTexture(GL_TEXTURE2);
			break;
		case 3:
			glActiveTexture(GL_TEXTURE3);
			break;
		case 4:
			glActiveTexture(GL_TEXTURE4);
			break;
		case 5:
			glActiveTexture(GL_TEXTURE5);
			break;
		case 6:
			glActiveTexture(GL_TEXTURE6);
			break;
		case 7:
			glActiveTexture(GL_TEXTURE7);
			break;
		}
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	unsigned int CResourceDepthbuffer::getWidth(void) const
	{
		return _muiWidth;
	}

	unsigned int CResourceDepthbuffer::getHeight(void) const
	{
		return _muiHeight;
	}

	glm::vec2 CResourceDepthbuffer::getDimensions(void) const
	{
		glm::vec2 vDims;
		vDims.x = float(_muiWidth);
		vDims.y = float(_muiHeight);
		return vDims;
	}

	void CResourceDepthbuffer::resize(unsigned int uiNewWidth, unsigned int uiNewHeight)
	{
		ThrowIfTrue(uiNewWidth == 0 || uiNewHeight == 0, "CResourceDepthbuffer::resize() given a dimension of size zero.");
		onGLContextToBeDestroyed();
		_muiWidth = uiNewWidth;
		_muiHeight = uiNewHeight;
		onGLContextCreated();
	}

	void CResourceDepthbuffer::renderTo2DQuad(unsigned int uiPosX, unsigned int uiPosY, unsigned int uiWidth, unsigned int uiHeight)
	{
		SCResourceManager* pRM = SCResourceManager::getPointer();
		CResourceTriangle* pTri = pRM->getTriangle("X:debug");
		CResourceShader* pShader = pRM->getShader("X:depthbuffer_debug");
		SCWindow* pWindow = SCWindow::getPointer();
		
		// Setup triangle geometry
		pTri->removeGeom();
		pTri->addQuad2D(glm::vec2(float(uiPosX), float(uiPosY)), glm::vec2(float(uiWidth), float(uiHeight)),
			glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),	// Colour
			glm::vec2(0,0), glm::vec2(1,0), glm::vec2(1,1), glm::vec2(0,1));	// Texture coordinates
		pTri->update();
		
		glm::mat4 matrixProjection = glm::ortho(0.0f, float(pWindow->getWidth()), float(pWindow->getHeight()), 0.0f, -1.0f, 1.0f);
		pShader->bind();
		pShader->setMat4("transform", matrixProjection);
		bindAsTexture(0);
		glDisable(GL_DEPTH_TEST);
		pTri->draw();
		pShader->unbind();
		unbindTexture(0);
	}
}