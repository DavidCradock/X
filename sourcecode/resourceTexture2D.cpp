#include "PCH.h"
#include "resourceTexture2D.h"
#include "image.h"
#include "log.h"

namespace X
{

	ResourceTexture2D::ResourceTexture2D(const std::string& strImageFilename)
	{
		_mstrImageFilename = strImageFilename;
		_muiTextureID = 0;
		onGLContextCreated();
	}

	ResourceTexture2D::~ResourceTexture2D()
	{
		onGLContextToBeDestroyed();
	}

	void ResourceTexture2D::onGLContextCreated(void)
	{
		Image image;
		ThrowIfFalse(image.load(_mstrImageFilename, false), "ResourceTexture2D::onGLContextCreated() failed to load image from file containing image data.");
		glGenTextures(1, &_muiTextureID);
		glBindTexture(GL_TEXTURE_2D, _muiTextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.getWidth(), image.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, image.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	void ResourceTexture2D::onGLContextToBeDestroyed(void)
	{

	}

}