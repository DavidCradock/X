#include "PCH.h"
#include "resourceTexture2DAnimation.h"
#include "image.h"
#include "log.h"
#include "window.h"

namespace X
{

	ResourceTexture2DAnimation::ResourceTexture2DAnimation(const std::vector<std::string>& vecStrImageFilenames, bool bFlipYaxis)
	{
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
		// For each of the large images, create a texture
		for (int i = 0; i < _mvLargeTextureIDs.size(); i++)
		{
			glGenTextures(1, &_mvLargeTextureIDs[i]);
			glBindTexture(GL_TEXTURE_2D, _mvLargeTextureIDs[i]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			if (3 == _mvLargeImages[i]->getNumChannels())
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _mvLargeImages[i]->getWidth(), _mvLargeImages[i]->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, _mvLargeImages[i]->getData());
			else  // We'll assume 4
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _mvLargeImages[i]->getWidth(), _mvLargeImages[i]->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, _mvLargeImages[i]->getData());
			glGenerateMipmap(GL_TEXTURE_2D);
		}
	}

	void ResourceTexture2DAnimation::onGLContextToBeDestroyed(void)
	{
		// For each of the large images, delete texture
		for (int i = 0; i < _mvLargeTextureIDs.size(); i++)
		{
			glDeleteTextures(1, &_mvLargeTextureIDs[i]);
			_mvLargeTextureIDs[i] = 0;
		}
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
		glBindTexture(GL_TEXTURE_2D, _mvLargeTextureIDs[_mvAnimationFrames[uiFrameNumber].uiTextureNumber]);
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

	void ResourceTexture2DAnimation::getTextureCoords(unsigned int uiFrameNumber, glm::vec2& vTCMin, glm::vec2& vTCMax)
	{
		ThrowIfTrue(uiFrameNumber >= _mvAnimationFrames.size(), "ResourceTexture2DAnimation::getTextureCoords() given invalid frame number.");
		vTCMin = _mvAnimationFrames[uiFrameNumber].vTCMin;
		vTCMax = _mvAnimationFrames[uiFrameNumber].vTCMax;
	}

	void ResourceTexture2DAnimation::_packImagesIntoLargeImages(const std::vector<std::string>& vecStrImageFilenames, bool bFlipYaxis)
	{
		// Make sure the vector is valid
		ThrowIfTrue(0 == vecStrImageFilenames.size(), "ResourceTexture2DAnimation::_packImagesIntoLargeImages() failed. Given vector of strings has no data.");

		// Load first image in the vector and determine dimensions
		Image image;
		int iImageWidth, iImageHeight, iImageChannels;
		ThrowIfFalse(image.loadInfo(vecStrImageFilenames[0], iImageWidth, iImageHeight, iImageChannels), "ResourceTexture2DAnimation::_packImagesIntoLargeImages() failed. Unable to determined dimensions of first image.");

		// Set dimensions of a single texture
		mvDimensions.x = (float)iImageWidth;
		mvDimensions.y = (float)iImageHeight;

		// Get maximum texture dimensions supported by gfx hardware and check that the first image dims are <= to that
		int iMaxTextureDims = Window::getPointer()->getMaxTextureSize();
		ThrowIfTrue(iImageWidth > iMaxTextureDims || iImageHeight > iMaxTextureDims, "ResourceTexture2DAnimation::_packImagesIntoLargeImages() failed. Image size is greater than maximum texture size supported by gfx hardware.");

		// Load in each individual image and whilst doing so, compute large texture dimensions and amount needed
		std::vector<Image*> vImages;	// Holds each individual image
		AnimationFrame animFrame;		// Holds texture number and positions (positions are converted to texture coordinates when we know large image dimensions)
		animFrame.uiTextureNumber = 0;
		animFrame.vTCMin = glm::vec2(0.0f, 0.0f);
		animFrame.vTCMax = glm::vec2((float)iImageWidth, (float)iImageHeight);
		std::vector<glm::vec2> vecLargeImageDims;
		vecLargeImageDims.push_back(glm::vec2(0, (float)iImageHeight));
		int iCurLargeTex = 0;

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

			// If this new image fits within iMaxTextureDims
			if (animFrame.vTCMax.x <= iMaxTextureDims)
			{
				// Add animation frame with current image positions in pixels
				_mvAnimationFrames.push_back(animFrame);
				if (vecLargeImageDims[iCurLargeTex].x < animFrame.vTCMax.x)
					vecLargeImageDims[iCurLargeTex].x = animFrame.vTCMax.x;
				animFrame.vTCMin.x += (float)iImageWidth;
				animFrame.vTCMax.x += (float)iImageWidth;
			}
			else // Doesn't fit along X 
			{
				// Move to next Y position in large image
				animFrame.vTCMin.x = 0.0f;
				animFrame.vTCMax.x = (float)iImageWidth;
				animFrame.vTCMin.y += (float)iImageHeight;
				animFrame.vTCMax.y += (float)iImageHeight;
				
				// If animation with new current positions on new row fits within iMaxTextureDims
				if (animFrame.vTCMax.y <= iMaxTextureDims)
				{
					// Add animation frame with current image positions in pixels
					_mvAnimationFrames.push_back(animFrame);
					vecLargeImageDims[iCurLargeTex].y = animFrame.vTCMax.y;
					// Move position to next image
					animFrame.vTCMin.x += (float)iImageWidth;
					animFrame.vTCMax.x += (float)iImageWidth;
				}
				else // We need a new large image
				{
					animFrame.vTCMin.x = 0.0f;
					animFrame.vTCMax.x = (float)iImageWidth;
					animFrame.vTCMin.y = 0.0f;
					animFrame.vTCMax.y = (float)iImageHeight;
					animFrame.uiTextureNumber++;
					_mvAnimationFrames.push_back(animFrame);
					vecLargeImageDims.push_back(glm::vec2((float)iImageWidth, (float)iImageHeight));
					iCurLargeTex++;
				}
			}
		}
		// animFrame.uiTextureNumber now holds the total number of images needed - 1
		
		// Create each of the large images
		int iNumImages = (int)vecLargeImageDims.size();
		for (int iLargeImage = 0; iLargeImage < iNumImages; iLargeImage++)
		{
			Image* pNewImage = new Image;
			ThrowIfFalse(pNewImage, "ResourceTexture2DAnimation::_packImagesIntoLargeImages() failed to allocated memory for large image.");
			_mvLargeImages.push_back(pNewImage);
			_mvLargeImages[iLargeImage]->createBlank((unsigned int)vecLargeImageDims[iLargeImage].x, (unsigned int)vecLargeImageDims[iLargeImage].y, (unsigned short)iImageChannels);
		}

		// Convert stored values in animFrame to texture coordinates (0-1) and
		// copy each individual image into the correct position of correct large image
		for (unsigned int iImage = 0; iImage < _mvAnimationFrames.size(); iImage++)
		{
			// Copy small image into large image
			vImages[iImage]->copyRectTo(
				*_mvLargeImages[_mvAnimationFrames[iImage].uiTextureNumber],
				0, 0,
				iImageWidth, iImageHeight,
				(int)_mvAnimationFrames[iImage].vTCMin.x,
				(int)_mvAnimationFrames[iImage].vTCMin.y);

			// Now convert pixel positions into texture coordinates
			glm::vec2 v2TexelSize;
			v2TexelSize.x = 1.0f / _mvLargeImages[_mvAnimationFrames[iImage].uiTextureNumber]->getWidth();
			v2TexelSize.y = 1.0f / _mvLargeImages[_mvAnimationFrames[iImage].uiTextureNumber]->getHeight();

			_mvAnimationFrames[iImage].vTCMin.x *= v2TexelSize.x;
			_mvAnimationFrames[iImage].vTCMin.y *= v2TexelSize.y;
			_mvAnimationFrames[iImage].vTCMax.x *= v2TexelSize.x;
			_mvAnimationFrames[iImage].vTCMax.y *= v2TexelSize.y;
		}

		// Free all temporary images
		for (unsigned int iImage = 0; iImage < vecStrImageFilenames.size(); iImage++)
		{
			delete vImages[iImage];
		}

		// Create texture IDs for each of the large images
		for (int i = 0; i < _mvLargeImages.size(); i++)
		{
			_mvLargeTextureIDs.push_back(0);

			// TEMP Save each large image
//			std::string strName = "Image_" + std::to_string(i) + ".png";
//			_mvLargeImages[i]->saveAsPNG(strName);
		}
	}
}