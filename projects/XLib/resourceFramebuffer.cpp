#include "PCH.h"
#include "resourceFramebuffer.h"
#include "openGLExtensions.h"
#include "log.h"
#include "resourceManager.h"
#include "window.h"

namespace X
{

	CResourceFramebuffer::CResourceFramebuffer(unsigned int iWidth, unsigned int iHeight)
	{
		_muiWidth = iWidth;
		_muiHeight = iHeight;
		_muiFramebufferID = 0;
		_muiTextureID = 0;
		_muiRenderbufferID = 0;
		mbNeedsUpdating = false;
		onGLContextCreated();
	}

	CResourceFramebuffer::~CResourceFramebuffer()
	{
		onGLContextToBeDestroyed();
	}

	void CResourceFramebuffer::onGLContextCreated(void)
	{
		// Create the framebuffer object and bind it
		glGenFramebuffers(1, &_muiFramebufferID);
		glBindFramebuffer(GL_FRAMEBUFFER, _muiFramebufferID);

		// Create a texture which will be attached to the framebuffer object for colour information
		glGenTextures(1, &_muiTextureID);
		glBindTexture(GL_TEXTURE_2D, _muiTextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _muiWidth, _muiHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _muiTextureID, 0);

		// Create renderbuffer object for depth and stencil
		glGenRenderbuffers(1, &_muiRenderbufferID);
		glBindRenderbuffer(GL_RENDERBUFFER, _muiRenderbufferID);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _muiWidth, _muiHeight);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _muiRenderbufferID);

		ThrowIfTrue(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE, "CResourceFramebuffer::onGLContextCreated() failed to create a complete framebuffer.");
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}

	void CResourceFramebuffer::onGLContextToBeDestroyed(void)
	{
		mbNeedsUpdating = true;

		if (0 != _muiTextureID)
		{
			glDeleteTextures(1, &_muiTextureID);
			//_muiTextureID = 0;
		}
		if (0 != _muiRenderbufferID)
		{
			glDeleteRenderbuffers(1, &_muiRenderbufferID);
			//_muiRenderbufferID = 0;
		}
		if (0 != _muiFramebufferID)
		{
			glDeleteFramebuffers(1, &_muiFramebufferID);
			//_muiFramebufferID = 0;
		}
	}

	void CResourceFramebuffer::bindAsRenderTarget(bool bClearbuffer,bool bResizeToWindowDimensions)
	{
		if (bResizeToWindowDimensions)
		{
			SCWindow* pWindow = SCWindow::getPointer();
			unsigned int iWindowWidth = (unsigned int)pWindow->getWidth();
			unsigned int iWindowHeight = (unsigned int)pWindow->getHeight();
			if (_muiWidth != iWindowWidth || _muiHeight != iWindowHeight)
			{
				resize(iWindowWidth, iWindowHeight);
			}
		}

		glBindFramebuffer(GL_FRAMEBUFFER, _muiFramebufferID);
		
		if (bClearbuffer)
		{
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		}
		glViewport(0, 0, _muiWidth, _muiHeight);
	}

	void CResourceFramebuffer::unbindAsRenderTarget(void) const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		SCWindow* pWnd = SCWindow::getPointer();
		glViewport(0, 0, pWnd->getWidth(), pWnd->getHeight());
		glFlush();
		glFinish();
	}

	void CResourceFramebuffer::bindAsTexture(unsigned int uiTextureUnit) const
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

	void CResourceFramebuffer::unbindTexture(unsigned int uiTextureUnit) const
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

	unsigned int CResourceFramebuffer::getWidth(void) const
	{
		return _muiWidth;
	}

	unsigned int CResourceFramebuffer::getHeight(void) const
	{
		return _muiHeight;
	}

	CVector2f CResourceFramebuffer::getDimensions(void) const
	{
		CVector2f vDims;
		vDims.x = float(_muiWidth);
		vDims.y = float(_muiHeight);
		return vDims;
	}

	void CResourceFramebuffer::resize(unsigned int uiNewWidth, unsigned int uiNewHeight)
	{
		// If the framebuffer is already the newly specified size, do nother
		if (uiNewWidth == _muiWidth)
			if (uiNewHeight == _muiHeight)
				return;

		ThrowIfTrue(uiNewWidth == 0 || uiNewHeight == 0, "CResourceFramebuffer::resize() given a dimension of size zero.");
		onGLContextToBeDestroyed();
		_muiWidth = uiNewWidth;
		_muiHeight = uiNewHeight;
		onGLContextCreated();
	}

	void CResourceFramebuffer::renderTo2DQuad(int iPosX, int iPosY, int iWidth, int iHeight, CColour colour)
	{
		SCResourceManager* pRM = SCResourceManager::getPointer();
		CResourceTriangle* pTri = pRM->getTriangle("X:default");
		CResourceShader* pShader = pRM->getShader("X:pos_col_tex");
		SCWindow* pWindow = SCWindow::getPointer();

		// Setup triangle geometry
		pTri->removeGeom();
		pTri->addQuad2D(CVector2f(float(iPosX), float(iPosY)), CVector2f(float(iWidth), float(iHeight)),
			colour,	// Colour
			CVector2f(0, 0), CVector2f(1, 0), CVector2f(1, 1), CVector2f(0, 1));	// Texture coordinates
		pTri->update();

		CMatrix matProjection;
		matProjection.setProjectionOrthographic(0.0f, float(pWindow->getWidth()), 0.0f, float(pWindow->getHeight()), -1.0f, 1.0f);
		pShader->bind();
		pShader->setMat4("transform", matProjection);
		pShader->setInt("texture0", 0);

		bindAsTexture(0);
		glDisable(GL_DEPTH_TEST);
//		glEnable(GL_BLEND);
		pTri->draw();
		pShader->unbind();
		unbindTexture(0);
	}
}