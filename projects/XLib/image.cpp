#include "PCH.h"
#include "image.h"
#include "log.h"
#include "vector3f.h"

namespace X
{
	CImage::CImage()
	{
		pData = 0;
		dataSize = 0;
		free();
	}

	CImage::~CImage()
	{
		free();
	}

	CImage& CImage::operator=(const CImage& other)
	{
		// Guard against self assignment
		if (this == &other)
			return *this;

		other.copyTo(*this);
		return *this;
	}


	void CImage::free(void)
	{
		if (pData)
		{
			delete[] pData;
			pData = NULL;
			dataSize = 0;
		}
		width = height = numChannels = 0;
	}

	void CImage::createBlank(unsigned int iWidth, unsigned int iHeight, unsigned short iNumChannels)
	{
		free();
		ThrowIfTrue(iWidth < 1, "CImage::createBlank() failed as given width < 1.");
		ThrowIfTrue(iHeight < 1, "CImage::createBlank() failed as given height < 1.");
		ThrowIfTrue(iNumChannels < 3, "CImage::createBlank() failed as given number of channels < 1. (Only 3 or 4 is valid)");
		ThrowIfTrue(iNumChannels > 4, "CImage::createBlank() failed as given number of channels > 4. (Only 3 or 4 is valid)");

		width = iWidth;
		height = iHeight;
		numChannels = iNumChannels;
		dataSize = width * height * numChannels;
		pData = new unsigned char[dataSize];
		ThrowIfTrue(!pData, "CImage::createBlank() failed to allocate memory.");

		// Zero out the new memory all to zero
		for (unsigned int i = 0; i < dataSize; ++i)
		{
			pData[i] = 0;
		}
	}

	bool CImage::load(const std::string& strFilename, bool bFlipForOpenGL)
	{
		free();
		STB::stbi_set_flip_vertically_on_load(bFlipForOpenGL);

		// Get number of channels in the image file
		int iDims[2];
		int iNumChannels = 3;
		loadInfo(strFilename, iDims[0], iDims[1], iNumChannels);
		STB::stbi_uc* pixels = 0;
		if (4 == iNumChannels)
			pixels = STB::stbi_load(strFilename.c_str(), &width, &height, &numChannels, STB::STBI_rgb_alpha);
		else if (3 == iNumChannels)
			pixels = STB::stbi_load(strFilename.c_str(), &width, &height, &numChannels, STB::STBI_rgb);
		else if (1 == iNumChannels)
			pixels = STB::stbi_load(strFilename.c_str(), &width, &height, &numChannels, 1);

		if (!pixels)
			return false;

		// If number of channels is 1, then we convert that 1 channel to 3 and duplicate the R to G and B
		if (1 == iNumChannels)
		{
			numChannels = 3;
		}

		// Compute size and allocate
		dataSize = width * height * numChannels;
		pData = new unsigned char[dataSize];

		if (1 != iNumChannels)
			memcpy(pData, pixels, static_cast<size_t>(dataSize));
		else // We need to copy the R to G and B
		{
			unsigned int iPixelIndex = 0;
			for (unsigned int i = 0; i < dataSize; i += 3)
			{
				pData[i] = pixels[iPixelIndex];
				pData[i+1] = pixels[iPixelIndex];
				pData[i+2] = pixels[iPixelIndex];
				iPixelIndex++;
			}
		}
		STB::stbi_image_free(pixels);
		return true;
	}

	bool CImage::loadInfo(const std::string& strFilename, int& iWidth, int& iHeight, int& componentCount)
	{
		// To query the width, height and component count of an image without having to
		// decode the full file, you can use the stbi_info family of functions:
		//
		//   int x,y,n,ok;
		//   ok = stbi_info(filename, &x, &y, &n);
		//   // returns ok=1 and sets x, y, n if image is a supported format,
		//   // 0 otherwise.
		return (bool)STB::stbi_info(strFilename.c_str(), &iWidth, &iHeight, &componentCount);
	}

	void CImage::saveAsBMP(const std::string& strFilename, bool bFlipOnSave) const
	{
		ThrowIfTrue(!pData, "CImage::saveAsBMP() failed. Image not yet created.");
		STB::stbi_flip_vertically_on_write(bFlipOnSave); // flag is non-zero to flip data vertically
		ThrowIfTrue(!STB::stbi_write_bmp(strFilename.c_str(), width, height, numChannels, pData), "CImage::saveAsBMP() failed. Image failed to be written.");
	}

	void CImage::saveAsJPG(const std::string& strFilename, bool bFlipOnSave, int iQuality) const
	{
		ThrowIfTrue(!pData, "CImage::saveAsJPG() failed. Image not yet created.");
		STB::stbi_flip_vertically_on_write(bFlipOnSave); // flag is non-zero to flip data vertically
		ThrowIfTrue(!STB::stbi_write_jpg(strFilename.c_str(), width, height, numChannels, pData, iQuality), "CImage::saveAsJPG() failed. Image failed to be written.");
	}

	void CImage::saveAsPNG(const std::string& strFilename, bool bFlipOnSave) const
	{
		ThrowIfTrue(!pData, "CImage::saveAsPNG() failed. Image not yet created.");
		STB::stbi_flip_vertically_on_write(bFlipOnSave); // flag is non-zero to flip data vertically
		ThrowIfTrue(!STB::stbi_write_png(strFilename.c_str(), width, height, numChannels, pData, width * numChannels), "CImage::saveAsPNG failed. Image failed to be written.");
	}

	void CImage::saveAsTGA(const std::string& strFilename, bool bFlipOnSave) const
	{
		ThrowIfTrue(!pData, "CImage::saveAsTGA() failed. Image not yet created.");
		STB::stbi_flip_vertically_on_write(bFlipOnSave); // flag is non-zero to flip data vertically
		ThrowIfTrue(!STB::stbi_write_tga(strFilename.c_str(), width, height, numChannels, pData), "CImage::saveAsTGA() failed. Image failed to be written.");
	}

	void CImage::fill(unsigned char ucRed, unsigned char ucGreen, unsigned char ucBlue, unsigned char ucAlpha)
	{
		ThrowIfTrue(!pData, "CImage::fill() failed. Image not yet created.");

		unsigned int i = 0;

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

	unsigned char* CImage::getData(void) const
	{
		return pData;
	}

	unsigned int CImage::getDataSize(void) const
	{
		return dataSize;
	}

	unsigned int CImage::getWidth(void) const
	{
		return width;
	}

	unsigned int CImage::getHeight(void) const
	{
		return height;
	}

	CVector2f CImage::getDimensions(void) const
	{
		CVector2f vDims((float)width, (float)height);
		return vDims;
	}

	unsigned int CImage::getNumChannels(void) const
	{
		return numChannels;
	}

	bool CImage::getDimsArePowerOfTwo(void) const
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

	void CImage::swapRedAndBlue(void)
	{
		ThrowIfTrue(!pData, "CImage::swapRedAndBlue() failed. Image not yet created.");

		unsigned int i = 0;
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

	void CImage::flipVertically(void)
	{
		ThrowIfTrue(!pData, "CImage::flipVertically() failed. Image not yet created.");

		// Size of a row
		unsigned int iRowSize = width * numChannels;

		// Allocate new flipped image
		unsigned char *pNewImageStartAddress = new unsigned char[dataSize];
		unsigned char *pNewImage = pNewImageStartAddress;
		ThrowIfTrue(0==pNewImage, "CImage::flipVertically() failed to allocate memory.");

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

	void CImage::invert(bool bInvertColour, bool bInvertAlpha)
	{
		ThrowIfTrue(!pData, "CImage::invert() failed. Image not yet created.");

		unsigned int i = 0;
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
	}

	void CImage::greyscaleSimple(void)
	{
		ThrowIfTrue(!pData, "CImage::greyscaleSimple() failed. Image not yet created.");

		unsigned int i = 0;
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
	}


	void CImage::greyscale(float fRedSensitivity, float fGreenSensitivity, float fBlueSensitivity)
	{
		ThrowIfTrue(!pData, "CImage::greyscale() failed. Image not yet created.");

		CVector3f vCol(fRedSensitivity, fGreenSensitivity, fBlueSensitivity);

		unsigned int i = 0;
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
	}

	void CImage::adjustBrightness(int iAmount)
	{
		ThrowIfTrue(!pData, "CImage::adjustBrightness() failed. Image not yet created.");

		unsigned int i = 0;
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
	}

	void CImage::adjustContrast(int iAmount)
	{
		ThrowIfTrue(!pData, "CImage::adjustContrast() failed. Image not yet created.");

		clamp(iAmount, -100, 100);
		double dPixel;
		double d1Over255 = 1.0 / 255.0;
		double dContrast = (100.0 + double(iAmount)) * 0.01; // 0 and 2
		dContrast *= dContrast;	// 0 and 4
		unsigned int i = 0;
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
	}

	void CImage::copyTo(CImage &destImage) const
	{
		ThrowIfTrue(!pData, "CImage::copyTo() failed. Source image not yet created.");

		// If destination image is the same as this one, do nothing
		if (destImage.pData == this->pData)
			return;

		destImage.free();
		destImage.createBlank(width, height, numChannels);
		memcpy(destImage.pData, pData, sizeof(unsigned char) * dataSize);
	}

	void CImage::copyRectTo(CImage &destImage, int iSrcPosX, int iSrcPosY, int iSrcWidth, int iSrcHeight, int iDestPosX, int iDestPosY) const
	{
		// Check that both images have data
		ThrowIfTrue(!pData, "CImage::copyRectTo() failed. Source image not yet created.");
		ThrowIfTrue(!destImage.pData, "CImage::copyRectTo() failed. Destination image not yet created.");

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

	void CImage::copyToAddBorder(CImage& outputImage) const
	{
		ThrowIfTrue(!pData, "CImage::copyToAddBorder() failed. Image data doesn't exist.");

		// Compute new larger dimensions and create the larger image
		int newWidth = width + 2;
		int newHeight = height + 2;
		outputImage.createBlank(newWidth, newHeight, numChannels);

		// Copy this image to the centre of the larger image
		copyRectTo(outputImage, 0, 0, width, height, 1, 1);

		// Now copy the edges of this image to the destination image
		unsigned char r, g, b, a;
		int heightOfOutputImageMinusOne = newHeight - 1;
		// Top and bottom edges
		for (int iX = 0; iX < width; iX++)
		{
			// Top pixel row
			getPixel(iX, 0, r, g, b, a);
			outputImage.setPixel(iX + 1, 0, r, g, b, a);

			// Bottom pixel row
			getPixel(iX, height - 1, r, g, b, a);
			outputImage.setPixel(iX + 1, heightOfOutputImageMinusOne, r, g, b, a);
		}
		int widthOfOutputImageMinusOne = newWidth - 1;
		// Left and right edges
		for (int iY = 0; iY < height; iY++)
		{
			// Left pixel column
			getPixel(0, iY, r, g, b, a);
			outputImage.setPixel(0, iY + 1, r, g, b, a);

			// Right pixel column
			getPixel(width - 1, iY, r, g, b, a);
			outputImage.setPixel(widthOfOutputImageMinusOne, iY + 1, r, g, b, a);
		}
	}

	void CImage::rotateClockwise(void)
	{
		CImage oldImage;
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

	void CImage::edgeDetect(CImage &outputImage, unsigned char r, unsigned char g, unsigned char b)
	{
		ThrowIfTrue(!pData, "CImage::edgeDetect() failed. Image data doesn't exist.");
		ThrowIfTrue(numChannels < 3, "CImage::edgeDetect() failed. Some image data exists, but doesn't have enough colour channels.");

		outputImage.createBlank(width, height, 4);
		int iX = 0;
		int iY = 0;
		while (iX < (int)width)
		{
			while (iY < (int)height)
			{
				if (_isPixelEdge(iX, iY, r, g, b))
					outputImage.setPixel(iX, iY, 255, 255, 255, 255);
				else
					outputImage.setPixel(iX, iY, 0, 0, 0, 0);
				++iY;
			}
			++iX;
			iY = 0;
		}
	}

	void CImage::removeAlphaChannel(void)
	{
		ThrowIfTrue(!pData, "CImage::removeAlphaChannel() failed. Image data doesn't exist.");
		ThrowIfTrue(numChannels != 4, "CImage::removeAlphaChannel() failed. Some image data exists, but the alpha data doesn't exist (Image doesn't hold 4 channels)");

		// Copy this image to a new tmp image
		CImage old;
		copyTo(old);

		// Recreate this one, but with 3 channels
		createBlank(old.getWidth(), old.getHeight(), 3);

		// Copy RGB from old to this...
		unsigned int iIndex = 0;
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

	void CImage::copyAlphaChannelToRGB(void)
	{
		ThrowIfTrue(!pData, "CImage::copyAlphaChannelToRGB() failed. Image data doesn't exist.");
		ThrowIfTrue(numChannels != 4, "CImage::copyAlphaChannelToRGB() failed. Some image data exists, but the alpha data doesn't exist (Image doesn't hold 4 channels)");

		unsigned int iIndex = 0;
		while (iIndex < dataSize)
		{
			pData[iIndex] = pData[iIndex+3];	// Red
			pData[iIndex+1] = pData[iIndex+3];	// Green
			pData[iIndex+2] = pData[iIndex+3];	// Blue
			iIndex+=4;
		}
	}

	void CImage::normalmap(CImage& outputImage, float fScale) const
	{
		ThrowIfTrue(!pData, "CImage::normalmap() failed. Image data doesn't exist.");

		clamp(fScale, 0.0f, 1.0f);

		// Copy this image into a new one so this is left unaffected.
		// This uses the copyToAddBorder() method which adds a border and copies the edge pixels to the new pixels in the border.
		// This makes it so we don't have to mess around with edge cases.
		CImage imageGreyscale;
		copyToAddBorder(imageGreyscale);

		// Greyscale the image
		imageGreyscale.greyscaleSimple();

		// Create output image with the same size as this one
		outputImage.createBlank(width, height, 3);

		// Now loop through greyscale image, computing each normal and storing in the output image.
		unsigned char r[3], g[3], b[3], a;
		float fX, fY, fZ;
		float fLength;
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				// we add +1 to imageGreyscale pixel positions as it has a border

				// Get height values of centre and surrounding pixels
				imageGreyscale.getPixel(x + 1, y + 1, r[0], g[0], b[0], a);	// Current pixel
				imageGreyscale.getPixel(x, y + 1, r[1], g[1], b[1], a);		// Left pixel
				imageGreyscale.getPixel(x + 1, y + 2, r[2], g[2], b[2], a);	// Above pixel
				
				fX = float(r[1] - r[0]) / 255.0f;	// Convert to -1.0f to 1.0f
				fY = float(r[2] - r[0]) / 255.0f;	// ....
				fZ = fScale;

				// Compute length of vector and normalize
				fLength = sqrt((fX * fX) + (fY * fY) + (fZ * fZ));
				if (areFloatsEqual(fLength, 0.0f))	// If length is nearly zero, just set as up vector
				{
					fX = 0.0f;
					fY = 0.0f;
					fZ = fScale;
				}
				else
				{
					fX = fX / fLength;
					fY = fY / fLength;
					fZ = fZ / fLength;
				}

				// Convert from -1, +1 to 0, 255
				fX += 1.0f;	fX *= 127.0f;
				fY += 1.0f;	fY *= 127.0f;
				fZ += 1.0f;	fZ *= 127.0f;
				r[0] = unsigned char(fX);
				g[0] = unsigned char(fY);
				b[0] = unsigned char(fZ);
				outputImage.setPixel(x, y, r[0], g[0], b[0], a);
			}
		}
	}
}
