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
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.getWidth(), image.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, image.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	void ResourceTexture2D::onGLContextToBeDestroyed(void)
	{
		glDeleteTextures(1, &_muiTextureID);

	}

	void ResourceTexture2D::bind(unsigned int uiTextureUnit)
	{
		glActiveTextureARB(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _muiTextureID);
	}
}