#include "PCH.h"
#include "resourceTexture2D.h"

namespace X
{

	ResourceTexture2D::ResourceTexture2D(const std::string& strImageFilename)
	{
		_mstrImageFilename = strImageFilename;
		onGLContextCreated();
	}

	ResourceTexture2D::~ResourceTexture2D()
	{
		onGLContextToBeDestroyed();
	}

	void ResourceTexture2D::onGLContextCreated(void)
	{

	}

	void ResourceTexture2D::onGLContextToBeDestroyed(void)
	{

	}

}