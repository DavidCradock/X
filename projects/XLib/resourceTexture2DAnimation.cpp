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
		_packImagesIntoLargeImages(vecStrImageFilenames, bFlipYaxis);

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

	void ResourceTexture2DAnimation::bind(unsigned int uiTextureUnit, unsigned int uiFrameNumber)
	{
		ThrowIfTrue(uiFrameNumber >= _mvAnimationFrames.size(), "ResourceTexture2DAnimation::bind() given invalid frame number.");

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

	unsigned int ResourceTexture2DAnimation::getNumFrames(void)
	{
		return unsigned int(_mvAnimationFrames.size());
	}

	void ResourceTexture2DAnimation::_packImagesIntoLargeImages(const std::vector<std::string>& vecStrImageFilenames, bool bFlipYaxis)
	{
		// Make sure the vector is valid
		ThrowIfTrue(0 == vecStrImageFilenames.size(), "ResourceTexture2DAnimation::_packImagesIntoLargeImages() failed. Given vector of strings has no data.");

		// Load first image in the vector and determine dimensions
		Image image;
		int iImageWidth, iImageHeight, iImageChannels;
		ThrowIfFalse(image.loadInfo(vecStrImageFilenames[0], iImageWidth, iImageHeight, iImageChannels), "ResourceTexture2DAnimation::_packImagesIntoLargeImages() failed. Unable to determined dimensions of first image.");

		// Get maximum texture dimensions supported by gfx hardware and check that the first image dims are <= to that
		int iMaxTextureDims = Window::getPointer()->getMaxTextureSize();
		ThrowIfTrue(iImageWidth > iMaxTextureDims || iImageHeight > iMaxTextureDims, "ResourceTexture2DAnimation::_packImagesIntoLargeImages() failed. Image size is greater than maximum texture size supported by gfx hardware.");

		// Compute total number of textures needed and their dimensions
//		int iMaxNumImagesAlongX = iMaxTextureDims / iImageWidth;
//		int iMaxNumImagesAlongY = iMaxTextureDims / iImageHeight;
//		int iMaxNumImagesPerLargeImage = iMaxNumImagesAlongX * iMaxNumImagesAlongY;
//		int iTotalNumberOfImages = vecStrImageFilenames.size();
		
		// Load in each individual image and whilst doing so, compute large texture dimensions and amount needed
		std::vector<Image*> vImages;	// Holds each individual image
		AnimationFrame animFrame;		// Holds texture number and positions (positions are converted to texture coordinates when we know large image dimensions)
		animFrame.uiTextureNumber = 0;
		animFrame.vTCMin = glm::vec2(0.0f, 0.0f);
		animFrame.vTCMax = glm::vec2((float)iImageWidth, (float)iImageHeight);
		glm::vec2 vMaxLargeImageDims;
		glm::vec2 vCurLargeImageDims = glm::vec2(0.0f, (float)iImageHeight);
		for (unsigned int iImage = 0; iImage < vecStrImageFilenames.size(); iImage++)
		{
			// Create new image and store in vImages
			Image* pNewImage = new Image;
			ThrowIfFalse(pNewImage, "ResourceTexture2DAnimation::_packImagesIntoLargeImages() failed. Unable to allocate memory for image " + vecStrImageFilenames[iImage] + ".");
			vImages.push_back(pNewImage);

			// Load in the image
			ThrowIfFalse(vImages[iImage]->load(vecStrImageFilenames[iImage], bFlipYaxis), "ResourceTexture2DAnimation::_packImagesIntoLargeImages() failed. Unable to load image " + vecStrImageFilenames[iImage] + " in.");

			// Make sure it's the same dimensions as the first image
			ThrowIfTrue((int)vImages[iImage]->getWidth() != iImageWidth || (int)vImages[iImage]->getHeight() != iImageHeight, "ResourceTexture2DAnimation::_packImagesIntoLargeImages() failed. Image " + vecStrImageFilenames[iImage] + " has different dimensions of first image.");

			// Make sure the number of channels is the same as the first image
			ThrowIfTrue((int)vImages[iImage]->getNumChannels() != iImageChannels, "ResourceTexture2DAnimation::_packImagesIntoLargeImages() failed. Image " + vecStrImageFilenames[iImage] + " has different number of channels than first image.");

			// Compute dimensions of large textures and number
			// If this new image fits within iMaxTextureDims
			if (animFrame.vTCMax.x <= iMaxTextureDims)
			{
				// Add animation frame with current image positions in pixels
				_mvAnimationFrames.push_back(animFrame);
				animFrame.vTCMin.x += (float)iImageWidth;
				animFrame.vTCMax.x += (float)iImageWidth;
				vCurLargeImageDims.x += (float)iImageWidth;
			}
			else
			{
				// Move to next Y postion in large image
				animFrame.vTCMin.x = 0.0f;
				animFrame.vTCMax.x = (float)iImageWidth;
				animFrame.vTCMin.y += (float)iImageHeight;
				animFrame.vTCMax.y += (float)iImageHeight;
				

				// If animation with new current positions on new row fits within iMaxTextureDims
				if (animFrame.vTCMax.y <= iMaxTextureDims)
				{
					// Add animation frame with current image positions in pixels
					_mvAnimationFrames.push_back(animFrame);

					// Move position to next image
					animFrame.vTCMin.x += (float)iImageWidth;
					animFrame.vTCMax.x += (float)iImageWidth;
					vCurLargeImageDims.y += (float)iImageHeight;
				}
				else // We need a new large image
				{
					animFrame.vTCMin.x = 0.0f;
					animFrame.vTCMax.x = (float)iImageWidth;
					animFrame.vTCMin.y = 0.0f;
					animFrame.vTCMax.y = (float)iImageHeight;
					animFrame.uiTextureNumber++;

					vMaxLargeImageDims = vCurLargeImageDims;
					vCurLargeImageDims = glm::vec2(0.0f, (float)iImageHeight);
				}
			}
		}
		// animFrame.uiTextureNumber now holds the total number of images needed - 1
		// vMaxLargeImageDims now holds the largest image dims needed
		// vCurLargeImageDims now holds the last image's dims needed
		
		// Create each of the large images
		Image img;
		int iNumImages = _mvAnimationFrames[_mvAnimationFrames.size() - 1].uiTextureNumber + 1;
		for (int iLargeImage = 0; iLargeImage < iNumImages - 1; iLargeImage++)
		{
			_mvLargeImages.push_back(img);
			_mvLargeImages[iLargeImage].createBlank(vMaxLargeImageDims.x, vMaxLargeImageDims.y, iImageChannels);
		}
		_mvLargeImages.push_back(img);
		_mvLargeImages[iNumImages - 1].createBlank(vCurLargeImageDims.x, vCurLargeImageDims.y, iImageChannels);


		// Convert stored values in animFrame to texture coordinates (0-1) and
		// copy each individual image into the correct position of correct large image
		for (unsigned int iImage = 0; iImage < vecStrImageFilenames.size(); iImage++)
		{
			_mvAnimationFrames[iImage].uiTextureNumber
			animFrame.vTCMin.x = 0.1f;

		}


		// Free all temporary images
		for (unsigned int iImage = 0; iImage < vecStrImageFilenames.size(); iImage++)
		{
			delete vImages[iImage];
		}

	}


}