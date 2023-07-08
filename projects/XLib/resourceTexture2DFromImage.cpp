#include "PCH.h"
#include "resourceTexture2DFromImage.h"
#include "image.h"
#include "log.h"

namespace X
{
	CResourceTexture2DFromImage::CResourceTexture2DFromImage(const CImage& image)
	{
		ThrowIfTrue(0 == image.getWidth() || 0 == image.getHeight(), "CResourceTexture2DFromImage::CResourceTexture2DFromImage() failed. Passed image has zero dimensions.");
		image.copyTo(_mImage);
		_muiTextureID = 0;
		onGLContextCreated();
	}

	CResourceTexture2DFromImage::~CResourceTexture2DFromImage()
	{
		onGLContextToBeDestroyed();
		_mImage.free();
	}

	void CResourceTexture2DFromImage::onGLContextCreated(void)
	{
		ThrowIfTrue(0 == _mImage.getWidth() || 0 == _mImage.getHeight(), "CResourceTexture2DFromImage::onGLContextCreated() failed. Passed image has zero dimensions.");
		mvDimensions.x = (float)_mImage.getWidth();
		mvDimensions.y = (float)_mImage.getHeight();
		glGenTextures(1, &_muiTextureID);
		glBindTexture(GL_TEXTURE_2D, _muiTextureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (3== _mImage.getNumChannels())
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _mImage.getWidth(), _mImage.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, _mImage.getData());
		else  // We'll assume 4
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _mImage.getWidth(), _mImage.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, _mImage.getData());

		glGenerateMipmap(GL_TEXTURE_2D);
	}

	void CResourceTexture2DFromImage::onGLContextToBeDestroyed(void)
	{
		glDeleteTextures(1, &_muiTextureID);
		_muiTextureID = 0;
	}

	void CResourceTexture2DFromImage::bind(unsigned int uiTextureUnit)
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

	void CResourceTexture2DFromImage::unbind(unsigned int uiTextureUnit)
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

	void CResourceTexture2DFromImage::unbindAll(void)
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

	void CResourceTexture2DFromImage::update(const CImage& image)
	{
		ThrowIfTrue(0 == image.getWidth() || 0 == image.getHeight(), "CResourceTexture2DFromImage::update() failed. Passed image has zero dimensions.");
		image.copyTo(_mImage);
		onGLContextToBeDestroyed();
		onGLContextCreated();
	}
}