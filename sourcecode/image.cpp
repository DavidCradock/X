#include "PCH.h"
#include "image.h"
#include "log.h"

namespace X
{
	Image::Image()
	{
		pData = 0;
		dataSize = 0;
		free();
	}

	Image::~Image()
	{
		free();
	}

	void Image::free(void)
	{
		if (pData)
		{
			delete[] pData;
			pData = NULL;
			dataSize = 0;
		}
		width = height = numChannels = 0;
	}

	void Image::createBlank(unsigned int iWidth, unsigned int iHeight, unsigned short iNumChannels)
	{
		free();
		ThrowIfTrue(iWidth < 1, "Image::createBlank() failed as given width < 1.");
		ThrowIfTrue(iHeight < 1, "Image::createBlank() failed as given height < 1.");
		ThrowIfTrue(iNumChannels < 3, "Image::createBlank() failed as given number of channels < 1. (Only 3 or 4 is valid)");
		ThrowIfTrue(iNumChannels > 4, "Image::createBlank() failed as given number of channels > 4. (Only 3 or 4 is valid)");

		width = iWidth;
		height = iHeight;
		numChannels = iNumChannels;
		dataSize = width * height * numChannels;
		pData = new unsigned char[dataSize];
		ThrowIfTrue(!pData, "Image::createBlank() failed to allocate memory.");

		// Zero out the new memory all to zero
		for (int i = 0; i < dataSize; ++i)
		{
			pData[i] = 0;
		}
	}

	bool Image::load(const std::string& strFilename, bool bFlipForOpenGL)
	{
		free();
		stbi_set_flip_vertically_on_load(bFlipForOpenGL);
		stbi_uc* pixels = stbi_load(strFilename.c_str(), &width, &height, &numChannels, STBI_rgb_alpha);
		if (!pixels)
			return false;

		dataSize = width * height * 4;
		pData = new unsigned char[dataSize];
		memcpy(pData, pixels, static_cast<size_t>(dataSize));
		stbi_image_free(pixels);
		return true;
	}

	bool Image::loadInfo(const std::string& strFilename, int& iWidth, int& iHeight, int& componentCount)
	{
		// To query the width, height and component count of an image without having to
		// decode the full file, you can use the stbi_info family of functions:
		//
		//   int x,y,n,ok;
		//   ok = stbi_info(filename, &x, &y, &n);
		//   // returns ok=1 and sets x, y, n if image is a supported format,
		//   // 0 otherwise.
		return (bool)stbi_info(strFilename.c_str(), &iWidth, &iHeight, &componentCount);
	}

	void Image::fill(unsigned char ucRed, unsigned char ucGreen, unsigned char ucBlue, unsigned char ucAlpha)
	{
		ThrowIfTrue(!pData, "Image::fill() failed. Image not yet created.");

		int i = 0;

		// 3 Colour channels
		if (3 == numChannels)
		{
			while (i < dataSize)
			{
				pData[i] = ucRed;
				pData[i+1] = ucGreen;
				pData[i+2] = ucBlue;
				i += numChannels;
			}
		}

		// 4 colour channels
		if (4 == numChannels)
		{
			while (i < dataSize)
			{
				pData[i] = ucRed;
				pData[i+1] = ucGreen;
				pData[i+2] = ucBlue;
				pData[i+3] = ucAlpha;
				i += numChannels;
			}
		}
	}

	bool Image::getDimsArePowerOfTwo(void)
	{
		int iX = 1;
		int iY = 1;
		while (iX < width)
			iX *= 2;
		while (iY < height)
			iY *= 2;
		if (iX != width || iY != height)
			return false;
		return true;
	}

	void Image::swapRedAndBlue(void)
	{
		ThrowIfTrue(!pData, "Image::swapRedAndBlue() failed. Image not yet created.");

		int i = 0;
		int i2;
		unsigned char chTemp;
		while (i < dataSize)
		{
			i2 = i+2;
			chTemp = pData[i];
			pData[i] = pData[i2];
			pData[i2] = chTemp;
			i += numChannels;
		}
	}

	void Image::saveAsBMP(const std::string& strFilename, bool bFlipOnSave = false)
	{
		ThrowIfTrue(!pData, "Image::saveAsBMP() failed. Image not yet created.");
		stbi_flip_vertically_on_write(bFlipOnSave); // flag is non-zero to flip data vertically
		ThrowIfTrue(!stbi_write_bmp(strFilename.c_str(), width, height, numChannels, pData), "Image::saveAsBMP() failed. Image failed to be written.");
	}

	void Image::saveAsJPG(const std::string& strFilename, bool bFlipOnSave, int iQuality)
	{
		ThrowIfTrue(!pData, "Image::saveAsJPG() failed. Image not yet created.");
		stbi_flip_vertically_on_write(bFlipOnSave); // flag is non-zero to flip data vertically
		ThrowIfTrue(!stbi_write_jpg(strFilename.c_str(), width, height, numChannels, pData, iQuality), "Image::saveAsJPG() failed. Image failed to be written.");
	}

	void Image::saveAsPNG(const std::string& strFilename, bool bFlipOnSave)
	{
		ThrowIfTrue(!pData, "Image::saveAsPNG() failed. Image not yet created.");
		stbi_flip_vertically_on_write(bFlipOnSave); // flag is non-zero to flip data vertically
		ThrowIfTrue(!stbi_write_png(strFilename.c_str(), width, height, numChannels, pData, width * numChannels), "Image::saveAsPNG failed. Image failed to be written.");
	}

	void Image::saveAsTGA(const std::string& strFilename, bool bFlipOnSave)
	{
		ThrowIfTrue(!pData, "Image::saveAsTGA() failed. Image not yet created.");
		stbi_flip_vertically_on_write(bFlipOnSave); // flag is non-zero to flip data vertically
		ThrowIfTrue(!stbi_write_tga(strFilename.c_str(), width, height, numChannels, pData), "Image::saveAsTGA() failed. Image failed to be written.");
	}

	void Image::flipVertically(void)
	{
		ThrowIfTrue(!pData, "Image::flipVertically() failed. Image not yet created.");

		// Size of a row
		unsigned int iRowSize = width * numChannels;

		// Allocate new flipped image
		unsigned char *pNewImageStartAddress = new unsigned char[dataSize];
		unsigned char *pNewImage = pNewImageStartAddress;
		ThrowIfTrue(0==pNewImage, "Image::flipVertically() failed to allocate memory.");

		// Get pointer to current image
		unsigned char *pOldImage = pData;
		// Increment old image pointer to point to last row
		pOldImage += iRowSize * (height - 1);

		// Copy each row into new image
		unsigned int iRowSizeBytes = iRowSize * sizeof(unsigned char);
		for (int iRow=0; iRow<height; ++iRow)
		{
			memcpy(pNewImage, pOldImage, iRowSizeBytes);
			// Adjust pointers
			pNewImage += iRowSizeBytes;
			pOldImage -= iRowSizeBytes;
		}
		// Now pNewImage contains flipped image data
		delete[] pData;	// Delete old image data
		pData = pNewImageStartAddress;	// Make image data point to the new data
	}

	bool Image::invert(bool bInvertColour, bool bInvertAlpha)
	{
		ThrowIfTrue(!pData, "Image::invert() failed. Image not yet created.");

		int i = 0;
		int iIndex;
		if (bInvertColour)
		{
			while (i < dataSize)
			{
				iIndex = i;
				pData[iIndex] = 255 - pData[iIndex]; ++iIndex;
				pData[iIndex] = 255 - pData[iIndex]; ++iIndex;
				pData[iIndex] = 255 - pData[iIndex];
				i += numChannels;
			}
		}

		if (numChannels == 4 && bInvertAlpha)
		{
			i = 3;
			while (i < dataSize)
			{
				pData[i] = 255 - pData[i];
				i += numChannels;
			}
		}
		return true;
	}

	bool Image::greyscaleSimple(void)
	{
		ThrowIfTrue(!pData, "Image::greyscaleSimple() failed. Image not yet created.");

		int i = 0;
		float f1Over3 = 1.0f / 3.0f;
		float fTmp;
		unsigned char cTmp;
		while (i < dataSize)
		{
			fTmp = float(pData[i]);
			fTmp += float(pData[i+1]);
			fTmp += float(pData[i+2]);
			fTmp *= f1Over3;
			cTmp = (unsigned char)fTmp;
			pData[i] = cTmp;
			pData[i+1] = cTmp;
			pData[i+2] = cTmp;
			i += numChannels;
		}
		return true;
	}


	bool Image::greyscale(float fRedSensitivity, float fGreenSensitivity, float fBlueSensitivity)
	{
		ThrowIfTrue(!pData, "Image::greyscale() failed. Image not yet created.");

		glm::vec3 vCol(fRedSensitivity, fGreenSensitivity, fBlueSensitivity);

		int i = 0;
		float fTmp;
		unsigned char cTmp;
		while (i < dataSize)
		{
			fTmp = 0.0f;
			fTmp = float(pData[i]) * vCol.x;
			fTmp += float(pData[i+1]) * vCol.y;
			fTmp += float(pData[i+2]) * vCol.z;
			cTmp = (unsigned char)fTmp;
			pData[i] = cTmp;
			pData[i+1] = cTmp;
			pData[i+2] = cTmp;
			i += numChannels;
		}
		return true;
	}

	bool Image::adjustBrightness(int iAmount)
	{
		ThrowIfTrue(!pData, "Image::adjustBrightness() failed. Image not yet created.");

		int i = 0;
		int iCol;
		while (i < dataSize)
		{
			iCol = (int)pData[i] + iAmount;
			clamp(iCol, 0, 255);
			pData[i] = unsigned char(iCol);

			iCol = (int)pData[i+1] + iAmount;
			clamp(iCol, 0, 255);
			pData[i+1] = unsigned char(iCol);

			iCol = (int)pData[i+2] + iAmount;
			clamp(iCol, 0, 255);
			pData[i+2] = unsigned char(iCol);
			i += numChannels;
		}
		return true;
	}

	bool Image::adjustContrast(int iAmount)
	{
		ThrowIfTrue(!pData, "Image::adjustContrast() failed. Image not yet created.");

		clamp(iAmount, -100, 100);
		double dPixel;
		double d1Over255 = 1.0 / 255.0;
		double dContrast = (100.0 + double(iAmount)) * 0.01; // 0 and 2
		dContrast *= dContrast;	// 0 and 4
		int i = 0;
		int iIndex;
		while (i < dataSize)
		{
			iIndex = i;
			dPixel = double(pData[iIndex]) * d1Over255;
			dPixel -= 0.5;
			dPixel *= dContrast;
			dPixel += 0.5;
			dPixel *= 255;
			clamp(dPixel, 0.0, 255.0);
			pData[iIndex] = unsigned char(dPixel);
			++iIndex;

			dPixel = double(pData[iIndex]) * d1Over255;
			dPixel -= 0.5;
			dPixel *= dContrast;
			dPixel += 0.5;
			dPixel *= 255;
			clamp(dPixel, 0.0, 255.0);
			pData[iIndex] = unsigned char(dPixel);
			++iIndex;

			dPixel = double(pData[iIndex]) * d1Over255;
			dPixel -= 0.5;
			dPixel *= dContrast;
			dPixel += 0.5;
			dPixel *= 255;
			clamp(dPixel, 0.0, 255.0);
			pData[iIndex] = unsigned char(dPixel);

			i += numChannels;
		}
		return true;
	}

	void Image::copyTo(Image &destImage) const
	{
		ThrowIfTrue(!pData, "Image::copyTo() failed. Source image not yet created.");

		// If destination image is the same as this one, do nothing
		if (destImage.pData == this->pData)
			return;


		destImage.free();
		destImage.createBlank(width, height, numChannels);
		memcpy(destImage.pData, pData, sizeof(unsigned char) * dataSize);
	}

	void Image::copyRectTo(Image &destImage, int iSrcPosX, int iSrcPosY, int iSrcWidth, int iSrcHeight, int iDestPosX, int iDestPosY)
	{
		// Check that both images have data
		ThrowIfTrue(!pData, "Image::copyRectTo() failed. Source image not yet created.");
		ThrowIfTrue(!destImage.pData, "Image::copyRectTo() failed. Destination image not yet created.");

		// Compute source rect
		int iSrcLeft = iSrcPosX;
		int iSrcBot = iSrcPosY;
		int iSrcRight = iSrcLeft + iSrcWidth;
		int iSrcTop = iSrcBot + iSrcHeight;
		// Compute destination rect
		int iDstLeft = iDestPosX;
		int iDstBot = iDestPosY;
		int iDstRight = iDstLeft + iSrcWidth;
		int iDstTop = iDstBot + iSrcHeight;

		// The above may be invalid due to different sizes, invalid positions, dims etc.
		// Invalid starting positions
		if (iSrcLeft >= width)
			return;
		if (iSrcBot >= height)
			return;
		if (iDstLeft >= destImage.width)
			return;
		if (iDstBot >= destImage.height)
			return;
		// Clamp right and top to edges of their respective images
		clamp(iSrcRight, iSrcLeft, width);
		clamp(iSrcTop, iSrcBot, height);
		clamp(iDstRight, iDstLeft, destImage.width);
		clamp(iDstTop, iDstBot, destImage.height);
		// Compute rect dims for both images
		unsigned int iSrcRectWidth = iSrcRight - iSrcLeft;
		unsigned int iSrcRectHeight = iSrcTop - iSrcBot;
		unsigned int iDstRectWidth = iDstRight - iDstLeft;
		unsigned int iDstRectHeight = iDstTop - iDstBot;
		// Compute smallest rect
		unsigned int iMinWidth = iSrcRectWidth;
		if (iMinWidth > iDstRectWidth)
			iMinWidth = iDstRectWidth;
		unsigned int iMinHeight = iSrcRectHeight;
		if (iMinHeight > iDstRectHeight)
			iMinHeight = iDstRectHeight;
		// If minimum = zero, then do nothing
		if (iMinWidth == 0)
			return;
		if (iMinHeight == 0)
			return;

		unsigned char colTmp[4];
		unsigned int isx, isy;
		unsigned int idx, idy;
		for (unsigned int ix = 0; ix < iMinWidth; ++ix)
		{
			for (unsigned int iy = 0; iy < iMinHeight; ++iy)
			{
				isx = iSrcLeft + ix;
				isy = iSrcBot + iy;
				idx = iDstLeft + ix;
				idy = iDstBot + iy;
				getPixel(isx, isy, colTmp[0], colTmp[1], colTmp[2], colTmp[3]);
				destImage.setPixel(idx, idy, colTmp[0], colTmp[1], colTmp[2], colTmp[3]);
			}
		}
	}

	void Image::rotateClockwise(void)
	{
		Image oldImage;
		copyTo(oldImage);

		unsigned char col[4];
		int idstX;
		int idstY;

		// Non squared?
		if (width != height)
		{
			createBlank(height, width, numChannels);
		}

		for (int isrcX=0; isrcX < oldImage.width; ++isrcX)
		{
			idstY = height - isrcX - 1;
			for (int isrcY=0; isrcY < oldImage.height; ++isrcY)
			{
				idstX = isrcY;
				oldImage.getPixel(isrcX, isrcY, col[0], col[1], col[2], col[3]);
				setPixel(idstX, idstY, col[0], col[1], col[2], col[3]);
			}
		}
	}

	void Image::edgeDetect(Image &outputImage, unsigned char r, unsigned char g, unsigned char b)
	{
		outputImage.createBlank(width, height, 4);
		int iX = 0;
		int iY = 0;
		while (iX < (int)width)
		{
			while (iY < (int)height)
			{
				if (isPixelEdge(iX, iY, r, g, b))
					outputImage.setPixel(iX, iY, 255, 255, 255, 255);
				else
					outputImage.setPixel(iX, iY, 0, 0, 0, 0);
				++iY;
			}
			++iX;
			iY = 0;
		}
	}

	void Image::removeAlphaChannel(void)
	{
		ThrowIfTrue(!pData, "Image::removeAlphaChannel() failed. Image data doesn't exist.");
		ThrowIfTrue(numChannels != 4, "Image::removeAlphaChannel() failed. Some image data exists, but the alpha data doesn't exist (Image doesn't hold 4 channels)");

		// Copy this image to a new tmp image
		Image old;
		copyTo(old);

		// Recreate this one, but with 3 channels
		createBlank(old.getWidth(), old.getHeight(), 3);

		// Copy RGB from old to this...
		int iIndex = 0;
		int iIndexOld = 0;
		while (iIndex < dataSize)
		{
			pData[iIndex] = old.pData[iIndexOld];		// Red
			pData[iIndex+1] = old.pData[iIndexOld+1];	// Green
			pData[iIndex+2] = old.pData[iIndexOld+2];	// Blue
			iIndex+=3;
			iIndexOld+=4;
		}
	}

	void Image::copyAlphaChannelToRGB(void)
	{
		ThrowIfTrue(!pData, "Image::copyAlphaChannelToRGB() failed. Image data doesn't exist.");
		ThrowIfTrue(numChannels != 4, "Image::copyAlphaChannelToRGB() failed. Some image data exists, but the alpha data doesn't exist (Image doesn't hold 4 channels)");

		int iIndex = 0;
		while (iIndex < dataSize)
		{
			pData[iIndex] = pData[iIndex+3];	// Red
			pData[iIndex+1] = pData[iIndex+3];	// Green
			pData[iIndex+2] = pData[iIndex+3];	// Blue
			iIndex+=4;
		}
	}
}
