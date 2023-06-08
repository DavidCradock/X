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
		_muiTextureID = 0;
	}

	void ResourceTexture2D::bind(unsigned int uiTextureUnit)
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

	void unbind(unsigned int uiTextureUnit)
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

	void ResourceTexture2D::unbindAll(void)
	{
		glActiveTexture(GL_TEXTURE7);	glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE6);	glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE5);	glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE4);	glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE3);	glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE2);	glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE1);	glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE0);	glBindTexture(GL_TEXTURE_2D, 0);
	}
}