#include "PCH.h"
#include "image.h"
#include "logging.h"
#include "vector3f.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

namespace X
{
	CImage::CImage()
	{
		_mpData = 0;
		_muiDataSize = 0;
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
		if (_mpData)
		{
			delete[] _mpData;
			_mpData = NULL;
			_muiDataSize = 0;
		}
		_miWidth = _miHeight = _miNumChannels = 0;
	}

	void CImage::createBlank(unsigned int iWidth, unsigned int iHeight, unsigned short iNumChannels)
	{
		free();
		ThrowIfTrue(iWidth < 1, "CImage::createBlank() failed as given width < 1.");
		ThrowIfTrue(iHeight < 1, "CImage::createBlank() failed as given height < 1.");
		ThrowIfTrue(iNumChannels < 3, "CImage::createBlank() failed as given number of channels < 1. (Only 3 or 4 is valid)");
		ThrowIfTrue(iNumChannels > 4, "CImage::createBlank() failed as given number of channels > 4. (Only 3 or 4 is valid)");

		_miWidth = iWidth;
		_miHeight = iHeight;
		_miNumChannels = iNumChannels;
		_muiDataSize = _miWidth * _miHeight * _miNumChannels;
		_mpData = new unsigned char[_muiDataSize];
		ThrowIfTrue(!_mpData, "CImage::createBlank() failed to allocate memory.");

		// Zero out the new memory all to zero
		for (unsigned int i = 0; i < _muiDataSize; ++i)
		{
			_mpData[i] = 0;
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
			pixels = STB::stbi_load(strFilename.c_str(), &_miWidth, &_miHeight, &_miNumChannels, STB::STBI_rgb_alpha);
		else if (3 == iNumChannels)
			pixels = STB::stbi_load(strFilename.c_str(), &_miWidth, &_miHeight, &_miNumChannels, STB::STBI_rgb);
		else if (1 == iNumChannels)
			pixels = STB::stbi_load(strFilename.c_str(), &_miWidth, &_miHeight, &_miNumChannels, 1);

		if (!pixels)
			return false;

		// If number of channels is 1, then we convert that 1 channel to 3 and duplicate the R to G and B
		if (1 == iNumChannels)
		{
			_miNumChannels = 3;
		}

		// Compute size and allocate
		_muiDataSize = _miWidth * _miHeight * _miNumChannels;
		_mpData = new unsigned char[_muiDataSize];

		if (1 != iNumChannels)
			memcpy(_mpData, pixels, static_cast<size_t>(_muiDataSize));
		else // We need to copy the R to G and B
		{
			unsigned int iPixelIndex = 0;
			for (unsigned int i = 0; i < _muiDataSize; i += 3)
			{
				_mpData[i] = pixels[iPixelIndex];
				_mpData[i+1] = pixels[iPixelIndex];
				_mpData[i+2] = pixels[iPixelIndex];
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
		//   int ix,iy,n,ok;
		//   ok = stbi_info(filename, &ix, &iy, &n);
		//   // returns ok=1 and sets ix, iy, n if image is a supported format,
		//   // 0 otherwise.
		return (bool)STB::stbi_info(strFilename.c_str(), &iWidth, &iHeight, &componentCount);
	}

	void CImage::saveAsBMP(const std::string& strFilename, bool bFlipOnSave) const
	{
		ThrowIfTrue(!_mpData, "CImage::saveAsBMP() failed. Image not yet created.");
		STB::stbi_flip_vertically_on_write(bFlipOnSave); // flag is non-zero to flip data vertically
		ThrowIfTrue(!STB::stbi_write_bmp(strFilename.c_str(), _miWidth, _miHeight, _miNumChannels, _mpData), "CImage::saveAsBMP(\"" + strFilename + "\") failed.Image failed to be written.");
	}

	void CImage::saveAsJPG(const std::string& strFilename, bool bFlipOnSave, int iQuality) const
	{
		ThrowIfTrue(!_mpData, "CImage::saveAsJPG() failed. Image not yet created.");
		STB::stbi_flip_vertically_on_write(bFlipOnSave); // flag is non-zero to flip data vertically
		ThrowIfTrue(!STB::stbi_write_jpg(strFilename.c_str(), _miWidth, _miHeight, _miNumChannels, _mpData, iQuality), "CImage::saveAsJPG(\"" + strFilename + "\") failed.Image failed to be written.");
	}

	void CImage::saveAsPNG(const std::string& strFilename, bool bFlipOnSave) const
	{
		ThrowIfTrue(!_mpData, "CImage::saveAsPNG() failed. Image not yet created.");
		STB::stbi_flip_vertically_on_write(bFlipOnSave); // flag is non-zero to flip data vertically
		ThrowIfTrue(!STB::stbi_write_png(strFilename.c_str(), _miWidth, _miHeight, _miNumChannels, _mpData, _miWidth * _miNumChannels), "CImage::saveAsPNG(\"" + strFilename + "\") failed.Image failed to be written.");
	}

	void CImage::saveAsTGA(const std::string& strFilename, bool bFlipOnSave) const
	{
		ThrowIfTrue(!_mpData, "CImage::saveAsTGA() failed. Image not yet created.");
		STB::stbi_flip_vertically_on_write(bFlipOnSave); // flag is non-zero to flip data vertically
		ThrowIfTrue(!STB::stbi_write_tga(strFilename.c_str(), _miWidth, _miHeight, _miNumChannels, _mpData), "CImage::saveAsTGA(\"" + strFilename + "\") failed.Image failed to be written.");
	}

	void CImage::fill(unsigned char ucRed, unsigned char ucGreen, unsigned char ucBlue, unsigned char ucAlpha)
	{
		ThrowIfTrue(!_mpData, "CImage::fill() failed. Image not yet created.");

		unsigned int i = 0;

		// 3 Colour channels
		if (3 == _miNumChannels)
		{
			while (i < _muiDataSize)
			{
				_mpData[i] = ucRed;
				_mpData[i+1] = ucGreen;
				_mpData[i+2] = ucBlue;
				i += _miNumChannels;
			}
		}

		// 4 colour channels
		if (4 == _miNumChannels)
		{
			while (i < _muiDataSize)
			{
				_mpData[i] = ucRed;
				_mpData[i+1] = ucGreen;
				_mpData[i+2] = ucBlue;
				_mpData[i+3] = ucAlpha;
				i += _miNumChannels;
			}
		}
	}

	unsigned char* CImage::getData(void) const
	{
		return _mpData;
	}

	unsigned int CImage::getDataSize(void) const
	{
		return _muiDataSize;
	}

	unsigned int CImage::getWidth(void) const
	{
		return _miWidth;
	}

	unsigned int CImage::getHeight(void) const
	{
		return _miHeight;
	}

	CVector2f CImage::getDimensions(void) const
	{
		CVector2f vDims((float)_miWidth, (float)_miHeight);
		return vDims;
	}

	unsigned int CImage::getNumChannels(void) const
	{
		return _miNumChannels;
	}

	bool CImage::getDimsArePowerOfTwo(void) const
	{
		int iX = 1;
		int iY = 1;
		while (iX < _miWidth)
			iX *= 2;
		while (iY < _miHeight)
			iY *= 2;
		if (iX != _miWidth || iY != _miHeight)
			return false;
		return true;
	}

	void CImage::swapRedAndBlue(void)
	{
		ThrowIfTrue(!_mpData, "CImage::swapRedAndBlue() failed. Image not yet created.");

		unsigned int i = 0;
		int i2;
		unsigned char chTemp;
		while (i < _muiDataSize)
		{
			i2 = i+2;
			chTemp = _mpData[i];
			_mpData[i] = _mpData[i2];
			_mpData[i2] = chTemp;
			i += _miNumChannels;
		}
	}

	void CImage::flipVertically(void)
	{
		ThrowIfTrue(!_mpData, "CImage::flipVertically() failed. Image not yet created.");

		// Size of a row
		unsigned int iRowSize = _miWidth * _miNumChannels;

		// Allocate new flipped image
		unsigned char *pNewImageStartAddress = new unsigned char[_muiDataSize];
		unsigned char *pNewImage = pNewImageStartAddress;
		ThrowIfTrue(0==pNewImage, "CImage::flipVertically() failed to allocate memory.");

		// Get pointer to current image
		unsigned char *pOldImage = _mpData;
		// Increment old image pointer to point to last row
		pOldImage += iRowSize * (_miHeight - 1);

		// Copy each row into new image
		unsigned int iRowSizeBytes = iRowSize * sizeof(unsigned char);
		for (int iRow=0; iRow<_miHeight; ++iRow)
		{
			memcpy(pNewImage, pOldImage, iRowSizeBytes);
			// Adjust pointers
			pNewImage += iRowSizeBytes;
			pOldImage -= iRowSizeBytes;
		}
		// Now pNewImage contains flipped image data
		delete[] _mpData;	// Delete old image data
		_mpData = pNewImageStartAddress;	// Make image data point to the new data
	}

	void CImage::invert(bool bInvertColour, bool bInvertAlpha)
	{
		ThrowIfTrue(!_mpData, "CImage::invert() failed. Image not yet created.");

		unsigned int i = 0;
		int iIndex;
		if (bInvertColour)
		{
			while (i < _muiDataSize)
			{
				iIndex = i;
				_mpData[iIndex] = 255 - _mpData[iIndex]; ++iIndex;
				_mpData[iIndex] = 255 - _mpData[iIndex]; ++iIndex;
				_mpData[iIndex] = 255 - _mpData[iIndex];
				i += _miNumChannels;
			}
		}

		if (_miNumChannels == 4 && bInvertAlpha)
		{
			i = 3;
			while (i < _muiDataSize)
			{
				_mpData[i] = 255 - _mpData[i];
				i += _miNumChannels;
			}
		}
	}

	void CImage::greyscaleSimple(void)
	{
		ThrowIfTrue(!_mpData, "CImage::greyscaleSimple() failed. Image not yet created.");

		unsigned int i = 0;
		float f1Over3 = 1.0f / 3.0f;
		float fTmp;
		unsigned char cTmp;
		while (i < _muiDataSize)
		{
			fTmp = float(_mpData[i]);
			fTmp += float(_mpData[i+1]);
			fTmp += float(_mpData[i+2]);
			fTmp *= f1Over3;
			cTmp = (unsigned char)fTmp;
			_mpData[i] = cTmp;
			_mpData[i+1] = cTmp;
			_mpData[i+2] = cTmp;
			i += _miNumChannels;
		}
	}


	void CImage::greyscale(float fRedSensitivity, float fGreenSensitivity, float fBlueSensitivity)
	{
		ThrowIfTrue(!_mpData, "CImage::greyscale() failed. Image not yet created.");

		CVector3f vCol(fRedSensitivity, fGreenSensitivity, fBlueSensitivity);

		unsigned int i = 0;
		float fTmp;
		unsigned char cTmp;
		while (i < _muiDataSize)
		{
			fTmp = 0.0f;
			fTmp = float(_mpData[i]) * vCol.x;
			fTmp += float(_mpData[i+1]) * vCol.y;
			fTmp += float(_mpData[i+2]) * vCol.z;
			cTmp = (unsigned char)fTmp;
			_mpData[i] = cTmp;
			_mpData[i+1] = cTmp;
			_mpData[i+2] = cTmp;
			i += _miNumChannels;
		}
	}

	void CImage::adjustBrightness(int iAmount)
	{
		ThrowIfTrue(!_mpData, "CImage::adjustBrightness() failed. Image not yet created.");

		unsigned int i = 0;
		int iCol;
		while (i < _muiDataSize)
		{
			iCol = (int)_mpData[i] + iAmount;
			clamp(iCol, 0, 255);
			_mpData[i] = unsigned char(iCol);

			iCol = (int)_mpData[i+1] + iAmount;
			clamp(iCol, 0, 255);
			_mpData[i+1] = unsigned char(iCol);

			iCol = (int)_mpData[i+2] + iAmount;
			clamp(iCol, 0, 255);
			_mpData[i+2] = unsigned char(iCol);
			i += _miNumChannels;
		}
	}

	void CImage::adjustContrast(int iAmount)
	{
		ThrowIfTrue(!_mpData, "CImage::adjustContrast() failed. Image not yet created.");

		clamp(iAmount, -100, 100);
		double dPixel;
		double d1Over255 = 1.0 / 255.0;
		double dContrast = (100.0 + double(iAmount)) * 0.01; // 0 and 2
		dContrast *= dContrast;	// 0 and 4
		unsigned int i = 0;
		int iIndex;
		while (i < _muiDataSize)
		{
			iIndex = i;
			dPixel = double(_mpData[iIndex]) * d1Over255;
			dPixel -= 0.5;
			dPixel *= dContrast;
			dPixel += 0.5;
			dPixel *= 255;
			clamp(dPixel, 0.0, 255.0);
			_mpData[iIndex] = unsigned char(dPixel);
			++iIndex;

			dPixel = double(_mpData[iIndex]) * d1Over255;
			dPixel -= 0.5;
			dPixel *= dContrast;
			dPixel += 0.5;
			dPixel *= 255;
			clamp(dPixel, 0.0, 255.0);
			_mpData[iIndex] = unsigned char(dPixel);
			++iIndex;

			dPixel = double(_mpData[iIndex]) * d1Over255;
			dPixel -= 0.5;
			dPixel *= dContrast;
			dPixel += 0.5;
			dPixel *= 255;
			clamp(dPixel, 0.0, 255.0);
			_mpData[iIndex] = unsigned char(dPixel);

			i += _miNumChannels;
		}
	}

	void CImage::copyTo(CImage &destImage) const
	{
		ThrowIfTrue(!_mpData, "CImage::copyTo() failed. Source image not yet created.");

		// If destination image is the same as this one, do nothing
		if (destImage._mpData == this->_mpData)
			return;

		destImage.free();
		destImage.createBlank(_miWidth, _miHeight, _miNumChannels);
		memcpy(destImage._mpData, _mpData, sizeof(unsigned char) * _muiDataSize);
	}

	void CImage::copyRectTo(CImage &destImage, int iSrcPosX, int iSrcPosY, int iSrcWidth, int iSrcHeight, int iDestPosX, int iDestPosY) const
	{
		// Check that both images have data
		ThrowIfTrue(!_mpData, "CImage::copyRectTo() failed. Source image not yet created.");
		ThrowIfTrue(!destImage._mpData, "CImage::copyRectTo() failed. Destination image not yet created.");

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
		if (iSrcLeft >= _miWidth)
			return;
		if (iSrcBot >= _miHeight)
			return;
		if (iDstLeft >= destImage._miWidth)
			return;
		if (iDstBot >= destImage._miHeight)
			return;
		// Clamp right and top to edges of their respective images
		clamp(iSrcRight, iSrcLeft, _miWidth);
		clamp(iSrcTop, iSrcBot, _miHeight);
		clamp(iDstRight, iDstLeft, destImage._miWidth);
		clamp(iDstTop, iDstBot, destImage._miHeight);
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
		ThrowIfTrue(!_mpData, "CImage::copyToAddBorder() failed. Image data doesn't exist.");

		// Compute new larger dimensions and create the larger image
		int newWidth = _miWidth + 2;
		int newHeight = _miHeight + 2;
		outputImage.createBlank(newWidth, newHeight, _miNumChannels);

		// Copy this image to the centre of the larger image
		copyRectTo(outputImage, 0, 0, _miWidth, _miHeight, 1, 1);

		// Now copy the edges of this image to the destination image
		unsigned char r, g, b, a;
		int heightOfOutputImageMinusOne = newHeight - 1;
		// Top and bottom edges
		for (int iX = 0; iX < _miWidth; iX++)
		{
			// Top pixel row
			getPixel(iX, 0, r, g, b, a);
			outputImage.setPixel(iX + 1, 0, r, g, b, a);

			// Bottom pixel row
			getPixel(iX, _miHeight - 1, r, g, b, a);
			outputImage.setPixel(iX + 1, heightOfOutputImageMinusOne, r, g, b, a);
		}
		int widthOfOutputImageMinusOne = newWidth - 1;
		// Left and right edges
		for (int iY = 0; iY < _miHeight; iY++)
		{
			// Left pixel column
			getPixel(0, iY, r, g, b, a);
			outputImage.setPixel(0, iY + 1, r, g, b, a);

			// Right pixel column
			getPixel(_miWidth - 1, iY, r, g, b, a);
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
		if (_miWidth != _miHeight)
		{
			createBlank(_miHeight, _miWidth, _miNumChannels);
		}

		for (int isrcX=0; isrcX < oldImage._miWidth; ++isrcX)
		{
			idstY = _miHeight - isrcX - 1;
			for (int isrcY=0; isrcY < oldImage._miHeight; ++isrcY)
			{
				idstX = isrcY;
				oldImage.getPixel(isrcX, isrcY, col[0], col[1], col[2], col[3]);
				setPixel(idstX, idstY, col[0], col[1], col[2], col[3]);
			}
		}
	}

	void CImage::edgeDetect(CImage &outputImage, unsigned char r, unsigned char g, unsigned char b)
	{
		ThrowIfTrue(!_mpData, "CImage::edgeDetect() failed. Image data doesn't exist.");
		ThrowIfTrue(_miNumChannels < 3, "CImage::edgeDetect() failed. Some image data exists, but doesn't have enough colour channels.");

		outputImage.createBlank(_miWidth, _miHeight, 4);
		int iX = 0;
		int iY = 0;
		while (iX < (int)_miWidth)
		{
			while (iY < (int)_miHeight)
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
		ThrowIfTrue(!_mpData, "CImage::removeAlphaChannel() failed. Image data doesn't exist.");
		ThrowIfTrue(_miNumChannels != 4, "CImage::removeAlphaChannel() failed. Some image data exists, but the alpha data doesn't exist (Image doesn't hold 4 channels)");

		// Copy this image to a new tmp image
		CImage old;
		copyTo(old);

		// Recreate this one, but with 3 channels
		createBlank(old.getWidth(), old.getHeight(), 3);

		// Copy RGB from old to this...
		unsigned int iIndex = 0;
		int iIndexOld = 0;
		while (iIndex < _muiDataSize)
		{
			_mpData[iIndex] = old._mpData[iIndexOld];		// Red
			_mpData[iIndex+1] = old._mpData[iIndexOld+1];	// Green
			_mpData[iIndex+2] = old._mpData[iIndexOld+2];	// Blue
			iIndex+=3;
			iIndexOld+=4;
		}
	}

	void CImage::copyAlphaChannelToRGB(void)
	{
		ThrowIfTrue(!_mpData, "CImage::copyAlphaChannelToRGB() failed. Image data doesn't exist.");
		ThrowIfTrue(_miNumChannels != 4, "CImage::copyAlphaChannelToRGB() failed. Some image data exists, but the alpha data doesn't exist (Image doesn't hold 4 channels)");

		unsigned int iIndex = 0;
		while (iIndex < _muiDataSize)
		{
			_mpData[iIndex] = _mpData[iIndex+3];	// Red
			_mpData[iIndex+1] = _mpData[iIndex+3];	// Green
			_mpData[iIndex+2] = _mpData[iIndex+3];	// Blue
			iIndex+=4;
		}
	}

	void CImage::normalmap(CImage& outputImage, float fScale) const
	{
		ThrowIfTrue(!_mpData, "CImage::normalmap() failed. Image data doesn't exist.");

		clamp(fScale, 0.0f, 1.0f);

		// Copy this image into a new one so this is left unaffected.
		// This uses the copyToAddBorder() method which adds a border and copies the edge pixels to the new pixels in the border.
		// This makes it so we don't have to mess around with edge cases.
		CImage imageGreyscale;
		copyToAddBorder(imageGreyscale);

		// Greyscale the image
		imageGreyscale.greyscaleSimple();

		// Create output image with the same size as this one
		outputImage.createBlank(_miWidth, _miHeight, 3);

		// Now loop through greyscale image, computing each normal and storing in the output image.
		unsigned char r[3], g[3], b[3], a;
		float fX, fY, fZ;
		float fLength;
		for (int y = 0; y < _miHeight; y++)
		{
			for (int ix = 0; ix < _miWidth; ix++)
			{
				// we add +1 to imageGreyscale pixel positions as it has a border

				// Get height values of centre and surrounding pixels
				imageGreyscale.getPixel(ix + 1, y + 1, r[0], g[0], b[0], a);	// Current pixel
				imageGreyscale.getPixel(ix, y + 1, r[1], g[1], b[1], a);		// Left pixel
				imageGreyscale.getPixel(ix + 1, y + 2, r[2], g[2], b[2], a);	// Above pixel
				
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
				outputImage.setPixel(ix, y, r[0], g[0], b[0], a);
			}
		}
	}

	void CImage::fillFromOpenGL(const CVector2f& vSourcePosTLCorner)
	{
		if (!_mpData)
			return;
		if (4 == _miNumChannels)
			glReadPixels((int)vSourcePosTLCorner.x, (int)vSourcePosTLCorner.y, _miWidth, _miHeight, GL_RGBA, GL_UNSIGNED_BYTE, _mpData);
		else if (3 == _miNumChannels)
			glReadPixels((int)vSourcePosTLCorner.x, (int)vSourcePosTLCorner.y, _miWidth, _miHeight, GL_RGB, GL_UNSIGNED_BYTE, _mpData);
		flipVertically();
	}

	void CImage::drawColourWheel(unsigned int iWidthAndHeightOfImage, unsigned char ucBrightness)
	{
		ThrowIfTrue(iWidthAndHeightOfImage < 1, "CImage::drawColourWheel() failed. Parsed iWidthAndHeightOfImage must be at least 1");
		createBlank(iWidthAndHeightOfImage, iWidthAndHeightOfImage, 4);

		float fBrightness = float(ucBrightness) / 255.0f;
		CVector2f vCentrePixelPosition;
		vCentrePixelPosition.x = float(iWidthAndHeightOfImage) * 0.5f;
		vCentrePixelPosition.y = vCentrePixelPosition.x;

		CVector2f vCurrentPixelPosition;
		CVector2f vCurrentPixelOffsetFromCentre;
		CColour colour;
		float fCircleRadius = float(iWidthAndHeightOfImage) * 0.5f;
		float fDistanceFromCentre;
		float fSaturation;	// 0.0f = white, 1.0f = full colour
		float fAngleDegrees;
		float fOneOver360 = 1.0f / 360.0f;
		unsigned int iPixelIndex = 0;
		for (unsigned int iPosX = 0; iPosX < (unsigned int)_miWidth; iPosX++)
		{
			vCurrentPixelPosition.x = (float)iPosX;
			for (unsigned int iPosY = 0; iPosY < (unsigned int)_miHeight; iPosY++)
			{
				vCurrentPixelPosition.y = (float)iPosY;
				vCurrentPixelOffsetFromCentre = vCurrentPixelPosition - vCentrePixelPosition;
				fDistanceFromCentre = vCurrentPixelOffsetFromCentre.getMagnitude();
				fSaturation = fCircleRadius - fDistanceFromCentre;
				fSaturation /= fCircleRadius;	// 0 at edge of circle, 1 at centre. Can be < 0 which is outside circle
				fAngleDegrees = vCurrentPixelOffsetFromCentre.getAngleDegrees360();
				fAngleDegrees *= fOneOver360;	// 0 when pixel is north, 0.25 when east etc.
				if (fSaturation < 0.0f)
					colour.set(0.0f, 0.0f, 0.0f, 0.0f);
				else
				{
					colour.setHueColour(fAngleDegrees, fSaturation, fBrightness);
					colour.alpha = 1.0f;
				}
				_mpData[iPixelIndex] = unsigned char(colour.red * 255);
				_mpData[iPixelIndex+1] = unsigned char(colour.green * 255);
				_mpData[iPixelIndex+2] = unsigned char(colour.blue * 255);
				_mpData[iPixelIndex + 3] = unsigned char(colour.alpha * 255);
				iPixelIndex += 4;
			}
		}
	}

	CColour CImage::getColourWheelColour(unsigned int iPositionX, unsigned int iPositionY, unsigned int iWidthAndHeightOfImage, unsigned char ucBrightness)
	{
		ThrowIfTrue(iWidthAndHeightOfImage < 1, "CImage::getColourWheelColour() failed. Parsed iWidthAndHeightOfImage must be at least 1");

		CColour colour;
		CVector2f vCurrentPixelPosition((float)iPositionX, float(iPositionY));
		CVector2f vCentrePixelPosition;
		vCentrePixelPosition.x = float(iWidthAndHeightOfImage) * 0.5f;
		vCentrePixelPosition.y = vCentrePixelPosition.x;
		CVector2f vCurrentPixelOffsetFromCentre = vCurrentPixelPosition - vCentrePixelPosition;
		float fDistanceFromCentre = vCurrentPixelOffsetFromCentre.getMagnitude();
		float fCircleRadius = float(iWidthAndHeightOfImage) * 0.5f;
		float fSaturation = fCircleRadius - fDistanceFromCentre;
		fSaturation /= fCircleRadius;	// 0 at edge of circle, 1 at centre. Can be < 0 which is outside circle
		float fAngleDegrees = vCurrentPixelOffsetFromCentre.getAngleDegrees360();
		fAngleDegrees /= 360.0f;	// 0 when pixel is north, 0.25 when east etc.
		if (fSaturation < 0.0f)
			colour.set(0.0f, 0.0f, 0.0f, 0.0f);
		else
		{
			colour.setHueColour(fAngleDegrees, fSaturation, float(ucBrightness) / 255.0f);
			colour.alpha = 1.0f;
		}
		return colour;
	}

	void CImage::drawGradient(unsigned int iWidth, unsigned int iHeight, unsigned int iNumChannels, const CColour& colour0, const CColour& colour1)
	{
		ThrowIfTrue(iWidth < 1 || iHeight < 1, "CImage::drawGradient() failed. Invalid dimensions given.");
		ThrowIfTrue(iNumChannels < 3 || iNumChannels > 4, "CImage::drawGradient() failed. Number of channels must be either 3 or 4.");
		createBlank(iWidth, iHeight, iNumChannels);
		bool bHorizontal = true;
		if (iHeight > iWidth)
			bHorizontal = false;

		CColour colour;
		unsigned int iPixelIndex = 0;
		if (bHorizontal)
		{
			for (unsigned int iPosX = 0; iPosX < iWidth; iPosX++)
			{
				colour = colour0.interpolate(colour1, float(iPosX) / float(iWidth));
				for (unsigned int iPosY = 0; iPosY < iHeight; iPosY++)
				{
					iPixelIndex = iPosX + (iPosY * _miWidth);
					iPixelIndex *= iNumChannels;
					_mpData[iPixelIndex] = unsigned char(colour.red * 255);
					_mpData[iPixelIndex + 1] = unsigned char(colour.green * 255);
					_mpData[iPixelIndex + 2] = unsigned char(colour.blue * 255);
					if (iNumChannels == 4)
						_mpData[iPixelIndex + 3] = unsigned char(colour.alpha * 255);
				}
			}
		}
		else
		{
			for (unsigned int iPosY = 0; iPosY < iHeight; iPosY++)
			{
				colour = colour0.interpolate(colour1, float(iPosY) / float(iHeight));
				for (unsigned int iPosX = 0; iPosX < iWidth; iPosX++)
				{
					iPixelIndex = iPosX + (iPosY * _miWidth);
					iPixelIndex *= iNumChannels;
					_mpData[iPixelIndex] = unsigned char(colour.red * 255);
					_mpData[iPixelIndex + 1] = unsigned char(colour.green * 255);
					_mpData[iPixelIndex + 2] = unsigned char(colour.blue * 255);
					if (iNumChannels == 4)
						_mpData[iPixelIndex + 3] = unsigned char(colour.alpha * 255);
				}
			}
		}
	}

	void CImage::drawCircle(unsigned int iWidthAndHeightOfImage, const CColour& colourInner, const CColour& colourOuter)
	{
		ThrowIfTrue(iWidthAndHeightOfImage < 1, "CImage::drawCircle() failed. Parsed iWidthAndHeightOfImage must be at least 1");
		createBlank(iWidthAndHeightOfImage, iWidthAndHeightOfImage, 4);

		CVector2f vCentrePixelPosition;
		vCentrePixelPosition.x = float(iWidthAndHeightOfImage) * 0.5f;
		vCentrePixelPosition.y = vCentrePixelPosition.x;

		CVector2f vCurrentPixelPosition;
		CVector2f vCurrentPixelOffsetFromCentre;
		CColour colour;
		float fCircleRadius = float(iWidthAndHeightOfImage) * 0.5f;
		float fDistanceFromCentre;
		float fOneOver360 = 1.0f / 360.0f;
		unsigned int iPixelIndex = 0;
		for (unsigned int iPosX = 0; iPosX < (unsigned int)_miWidth; iPosX++)
		{
			vCurrentPixelPosition.x = (float)iPosX;
			for (unsigned int iPosY = 0; iPosY < (unsigned int)_miHeight; iPosY++)
			{
				vCurrentPixelPosition.y = (float)iPosY;
				vCurrentPixelOffsetFromCentre = vCurrentPixelPosition - vCentrePixelPosition;
				fDistanceFromCentre = fCircleRadius - vCurrentPixelOffsetFromCentre.getMagnitude();
				fDistanceFromCentre /= fCircleRadius;	// 0 at edge of circle, 1 at centre. Can be < 0 which is outside circle
				if (fDistanceFromCentre < 0.0f)
					colour.set(0.0f, 0.0f, 0.0f, 0.0f);
				else
					colour = colourOuter.interpolate(colourInner, fDistanceFromCentre);
				_mpData[iPixelIndex] = unsigned char(colour.red * 255);
				_mpData[iPixelIndex + 1] = unsigned char(colour.green * 255);
				_mpData[iPixelIndex + 2] = unsigned char(colour.blue * 255);
				_mpData[iPixelIndex + 3] = unsigned char(colour.alpha * 255);
				iPixelIndex += 4;
			}
		}
	}
}
