#include "PCH.h"
#include "resourceFramebuffer.h"

namespace X
{

	ResourceFramebuffer::ResourceFramebuffer(unsigned int iWidth, unsigned int iHeight)
	{
		_muiWidth = iWidth;
		_muiHeight = iHeight;
		onGLContextCreated();
	}

	ResourceFramebuffer::~ResourceFramebuffer()
	{
		onGLContextToBeDestroyed();
	}

	void ResourceFramebuffer::onGLContextCreated(void)
	{

	}

	void ResourceFramebuffer::onGLContextToBeDestroyed(void)
	{

	}

}