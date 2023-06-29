#include "PCH.h"
#include "resourceTexture2DAnimation.h"
#include "image.h"
#include "log.h"
#include "window.h"

namespace X
{

	ResourceTexture2DAnimation::ResourceTexture2DAnimation(const std::vector<std::string>& vecStrImageFilenames, bool bFlipYaxis)
	{
		_muiTextureID = 0;
		_mbFlipYaxis = bFlipYaxis;

		// Upon construction, we need to load each image in and store inside large textures.
		_packImagesIntoLargeTextures(vecStrImageFilenames, bFlipYaxis);

		onGLContextCreated();
	}

	ResourceTexture2DAnimation::~ResourceTexture2DAnimation()
	{
		onGLContextToBeDestroyed();
	}

	void ResourceTexture2DAnimation::onGLContextCreated(void)
	{
		Image image;
//		ThrowIfFalse(image.load(_mstrImageFilename, _mbFlipYaxis), "ResourceTexture2DAnimation::onGLContextCreated() failed to load image from file (" + _mstrImageFilename + ") containing image data.");
		mvDimensions.x = (float)image.getWidth();
		mvDimensions.y = (float)image.getHeight();
		glGenTextures(1, &_muiTextureID);
		glBindTexture(GL_TEXTURE_2D, _muiTextureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (3==image.getNumChannels())
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.getWidth(), image.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, image.getData());
		else  // We'll assume 4
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.getWidth(), image.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getData());

		glGenerateMipmap(GL_TEXTURE_2D);
	}

	void ResourceTexture2DAnimation::onGLContextToBeDestroyed(void)
	{
		glDeleteTextures(1, &_muiTextureID);
		_muiTextureID = 0;
	}

	void ResourceTexture2DAnimation::bind(unsigned int uiTextureUnit)
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

	void ResourceTexture2DAnimation::unbind(unsigned int uiTextureUnit)
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

	void ResourceTexture2DAnimation::unbindAll(void)
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

	void ResourceTexture2DAnimation::_packImagesIntoLargeTextures(const std::vector<std::string>& vecStrImageFilenames, bool bFlipYaxis)
	{
		// Make sure the vector is valid
		ThrowIfTrue(0 == vecStrImageFilenames.size(), "ResourceTexture2DAnimation::_packImagesIntoLargeTextures() failed. Given vector of strings with no data.");

		// Load first image in the vector and determine dimensions
		Image image;
		int iImageWidth, iImageHeight, iImageChannels;
		ThrowIfFalse(image.loadInfo(vecStrImageFilenames[0], iImageWidth, iImageHeight, iImageChannels), "ResourceTexture2DAnimation::_packImagesIntoLargeTextures() failed. Unable to determind dimensions of first image.");

		// Get maximum texture dimensions supported by gfx hardware and check that the first image dims are <= to that
		int iMaxTextureDims = Window::getPointer()->getMaxTextureSize();
		ThrowIfTrue(iImageWidth > iMaxTextureDims || iImageHeight > iMaxTextureDims, "ResourceTexture2DAnimation::_packImagesIntoLargeTextures() failed. Image size is greated than maximum texture size supported by gfx hardware.");

		// Compute maximum number of image we can fit along the XY axis of the largest of textures
		int iMaxTexturesAlongX = iMaxTextureDims / iImageWidth;
		int iMaxTexturesAlongY = iMaxTextureDims / iImageHeight;

		// Load in each individual image and whilst doing so, compute large texture dimensions and amount needed
		int iLargeTextureWidthNeeded = 0;
		int iLargeTextureHeightNeeded = 0;
		int iNumberLargeTexturesNeeded = 1;
		std::vector<Image*> vImages;
		for (unsigned int iImage = 0; iImage < vecStrImageFilenames.size(); iImage++)
		{
			// Create new image and store in vImages
			Image* pNewImage = new Image;
			ThrowIfFalse(pNewImage, "ResourceTexture2DAnimation::_packImagesIntoLargeTextures() failed. Unable to allocate memory for image " + vecStrImageFilenames[iImage] + ".");
			vImages.push_back(pNewImage);

			// Load in the image
			ThrowIfFalse(vImages[iImage]->load(vecStrImageFilenames[iImage], bFlipYaxis), "ResourceTexture2DAnimation::_packImagesIntoLargeTextures() failed. Unable to load image " + vecStrImageFilenames[iImage] + " in.");

			// Make sure it's the same dimensions as the first image
			ThrowIfTrue((int)vImages[iImage]->getWidth() != iImageWidth || (int)vImages[iImage]->getHeight() != iImageHeight, "ResourceTexture2DAnimation::_packImagesIntoLargeTextures() failed. Image " + vecStrImageFilenames[iImage] + " has different dimensions from first image.");

			// Make sure the number of channels is the same as the first image
			ThrowIfTrue((int)vImages[iImage]->getNumChannels() != iImageChannels, "ResourceTexture2DAnimation::_packImagesIntoLargeTextures() failed. Image " + vecStrImageFilenames[iImage] + " has different number of channels than first image.");

			// Compute dimensions of large textures and number
			iLargeTextureWidthNeeded += iImageWidth;
		}

		// Free all temporary images
		for (unsigned int iImage = 0; iImage < vecStrImageFilenames.size(); iImage++)
		{
			delete vImages[iImage];
		}

	}
}