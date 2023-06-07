#include "PCH.h"
#include "resourceVertexbuffer.h"

namespace X
{

	ResourceVertexbuffer::ResourceVertexbuffer()
	{
		onGLContextCreated();
	}

	ResourceVertexbuffer::~ResourceVertexbuffer()
	{
		onGLContextToBeDestroyed();
	}

	void ResourceVertexbuffer::onGLContextCreated(void)
	{

	}

	void ResourceVertexbuffer::onGLContextToBeDestroyed(void)
	{

	}

}