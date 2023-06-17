#include "PCH.h"
#include "resourceFramebuffer.h"
#include "openGLExtensions.h"
#include "log.h"

namespace X
{

	ResourceFramebuffer::ResourceFramebuffer(unsigned int iWidth, unsigned int iHeight)
	{
		_muiWidth = iWidth;
		_muiHeight = iHeight;
		_muiFramebufferID = 0;
		_muiTextureID = 0;
		_muiRenderbufferID = 0;
		onGLContextCreated();
	}

	ResourceFramebuffer::~ResourceFramebuffer()
	{
		onGLContextToBeDestroyed();
	}

	void ResourceFramebuffer::onGLContextCreated(void)
	{
		// Create the framebuffer object and bind it
		glGenFramebuffers(1, &_muiFramebufferID);
		glBindFramebuffer(GL_FRAMEBUFFER, _muiFramebufferID);

		// Create a texture which will be attached to the framebuffer object for colour information
		glGenTextures(1, &_muiTextureID);
		glBindTexture(GL_TEXTURE_2D, _muiTextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _muiWidth, _muiHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _muiTextureID, 0);

		// Create renderbuffer object for depth and stencil
		glGenRenderbuffers(1, &_muiRenderbufferID);
		glBindRenderbuffer(GL_RENDERBUFFER, _muiRenderbufferID);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _muiWidth, _muiHeight);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _muiRenderbufferID);

		ThrowIfTrue(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE, "ResourceFramebuffer::onGLContextCreated() failed to create a complete framebuffer.");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}

	void ResourceFramebuffer::onGLContextToBeDestroyed(void)
	{
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

	void ResourceFramebuffer::bindAsRenderTarget(void)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _muiFramebufferID);
	}

	void ResourceFramebuffer::unbindAsRenderTarget(void)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void ResourceFramebuffer::bindAsTexture(unsigned int uiTextureUnit)
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

	void ResourceFramebuffer::unbindTexture(unsigned int uiTextureUnit)
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
}