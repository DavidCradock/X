#include "PCH.h"
#include "imageAtlas.h"
#include "logging.h"
#include "stringUtils.h"

namespace X
{
	void CImageAtlasDetails::write(std::ofstream& file)
	{
		ThrowIfFalse(file.is_open(), "CImageAtlasDetails::write() failed. The given ofstream is not open.");

		file.write((char*)&sTexCoords.bottom_left, sizeof(CVector2f));
		file.write((char*)&sTexCoords.bottom_right, sizeof(CVector2f));
		file.write((char*)&sTexCoords.top_left, sizeof(CVector2f));
		file.write((char*)&sTexCoords.top_right, sizeof(CVector2f));
		file.write((char*)&v2fDimensions, sizeof(CVector2f));
		StringUtils::stringWrite(strImageFilename, file);
		file.write((char*)&uiAtlasImage, sizeof(unsigned int));
		file.write((char*)&bRotated, sizeof(bool));

		ThrowIfFalse(file.good(), "CImageAtlasDetails::write() failed. The ofstream is not good.");
	}

	void CImageAtlasDetails::read(std::ifstream& file)
	{
		ThrowIfFalse(file.is_open(), "CImageAtlasDetails::read() failed. The given ifstream is not open.");

		file.read((char*)&sTexCoords.bottom_left, sizeof(CVector2f));
		file.read((char*)&sTexCoords.bottom_right, sizeof(CVector2f));
		file.read((char*)&sTexCoords.top_left, sizeof(CVector2f));
		file.read((char*)&sTexCoords.top_right, sizeof(CVector2f));
		file.read((char*)&v2fDimensions, sizeof(CVector2f));
		StringUtils::stringRead(strImageFilename, file);
		file.read((char*)&uiAtlasImage, sizeof(unsigned int));
		file.read((char*)&bRotated, sizeof(bool));

		ThrowIfFalse(file.good(), "CImageAtlasDetails::read() failed. The ifstream is not good.");
	}

	CImageAtlasPacker::CImageAtlasPacker()
	{
	}

	CImageAtlasPacker::~CImageAtlasPacker()
	{
		reset();
	}

	void CImageAtlasPacker::createAtlasImages(const std::vector<std::string>& vstrImages, unsigned int uiMaxAtlasImageWidth, unsigned int uiMaxAtlasImageHeight, bool bAllowRotationOfImages, unsigned int uiImageSpacing)
	{
		ThrowIfTrue(0 == vstrImages.size(), "CImageAtlasPacker::createAtlasImages() failed. The passed vector<string> containing individual image filenames contains no data.");
		ThrowIfTrue(0 == uiMaxAtlasImageWidth || 0 == uiMaxAtlasImageHeight, "CImageAtlasPacker::createAtlasImages() failed. uiMaxAtlasImageWidth or uiMaxAtlasImageHeight was set to zero.");

		// Reset, freeing atlas images if they exist
		reset();

		// Store each image filename and as we go, create and load each individual image into memory
		std::vector<std::string> vStrImageFilenames;
		std::vector<CImage*> vImages;
		for (unsigned int ui = 0; ui < vstrImages.size(); ui++)
		{
			vStrImageFilenames.push_back(vstrImages[ui]);

			// Allocate memory for individual image
			CImage* pNewImage = new CImage;
			ThrowIfFalse(pNewImage, "CImageAtlasPacker::createAtlasImages() failed. Unable to allocate memory for individual image.");
			vImages.push_back(pNewImage);

			// Load each individual image
			ThrowIfFalse(vImages[ui]->load(vStrImageFilenames[ui], false), "CImageAtlasPacker::createAtlasImages() failed. Unable to load image from file: " + vStrImageFilenames[ui] + ".");

			// We check to see if the loaded image's dimensions are not the same as the first one and throw an exception
			bool bNotSameDims = vImages[ui]->getWidth() != vImages[0]->getWidth() || vImages[ui]->getHeight() != vImages[0]->getHeight();
			if (bNotSameDims)
			{
				std::string strTxt = "CImageAtlasPacker::createAtlasImages() failed. All images added to an atlas must have the same dimensions.";
				strTxt += "The first image dims: " + std::to_string(vImages[0]->getWidth()) + "x" + std::to_string(vImages[0]->getHeight());
				strTxt += " The current image dims: " + std::to_string(vImages[ui]->getWidth()) + "x" + std::to_string(vImages[ui]->getHeight());
				strTxt += ".";
				ThrowIfTrue(1, strTxt);
			}
		}

		// Determine maximum width/height of the individual images and make sure, including image spacing, they can all fit within the specified max atlas image width/height
		unsigned int uiMaxImageWidth = 0;
		unsigned int uiMaxImageHeight = 0;
		unsigned int uiImageWidth;
		unsigned int uiImageHeight;
		unsigned int uiImageSpacingTimes2 = uiImageSpacing * 2;
		for (unsigned int ui = 0; ui < vImages.size(); ui++)
		{
			uiImageWidth = vImages[ui]->getWidth() + uiImageSpacingTimes2;
			uiImageHeight = vImages[ui]->getHeight() + uiImageSpacingTimes2;
			if (uiMaxImageWidth < uiImageWidth)
				uiMaxImageWidth = uiImageWidth;
			if (uiMaxImageHeight < uiImageHeight)
				uiMaxImageHeight = uiImageHeight;
		}
		ThrowIfTrue(uiMaxImageWidth > uiMaxAtlasImageWidth, "CImageAtlasPacker::createAtlasImages() failed. An individual image's width(" + std::to_string(uiMaxImageWidth) + "), including spacing of " + std::to_string(uiImageSpacing) + " is greater than the specified max atlas image's width(" + std::to_string(uiMaxAtlasImageWidth) + ".");
		ThrowIfTrue(uiMaxImageHeight > uiMaxAtlasImageHeight, "CImageAtlasPacker::createAtlasImages() failed. An individual image's height(" + std::to_string(uiMaxImageHeight) + "), including spacing of " + std::to_string(uiImageSpacing) + " is greater than the specified max atlas image's height(" + std::to_string(uiMaxAtlasImageHeight) + ".");

		// Create each image detail for each individual image, determining whether we should rotate the image or not
		// Also, rotate the images that should be
		for (unsigned int ui = 0; ui < vImages.size(); ui++)
		{
			CImageAtlasDetails imageDetail;
			imageDetail.bRotated = false;
			imageDetail.strImageFilename = vStrImageFilenames[ui];
			imageDetail.uiAtlasImage = 0;
			imageDetail.v2fDimensions = vImages[ui]->getDimensions();
			if (bAllowRotationOfImages)
			{
				// Rotate image so that it's height is greatest
				if (imageDetail.v2fDimensions.x > imageDetail.v2fDimensions.y)
				{
					imageDetail.bRotated = true;
					vImages[ui]->rotateClockwise();
					imageDetail.v2fDimensions = vImages[ui]->getDimensions();
				}
			}
			_mvImageDetails.push_back(imageDetail);
		}

		// Bubble sort temp vectors by their image's width
		// vectors which we have to sort...
		// std::vector<std::string> vStrImageFilenames;		// Holds each individual image's filename 
		// std::vector<CImage*> vImages;					// Holds each individual image's image data
		// std::vector<CImageAtlasDetails> _mvImageDetails;	// Holds each individual image's details
		std::string strFilenameTemp;
		CImage* pImageTemp;
		CImageAtlasDetails imageAtlasDetailsTemp;
		for (int i = 0; i < (int)vImages.size(); ++i)
		{
			for (int j = 0; j < (int)vImages.size(); ++j)
			{
				// Swap by largest first
				if (_mvImageDetails[j].v2fDimensions.x < _mvImageDetails[i].v2fDimensions.x)
				{
					// Temporarily store image, filename and image details
					// Assign i to temp
					pImageTemp = vImages[i];
					strFilenameTemp = vStrImageFilenames[i];
					imageAtlasDetailsTemp = _mvImageDetails[i];
					// Assign j to i
					vImages[i] = vImages[j];
					vStrImageFilenames[i] = vStrImageFilenames[j];
					_mvImageDetails[i] = _mvImageDetails[j];
					// Assign temp to j
					vImages[j] = pImageTemp;
					vStrImageFilenames[j] = strFilenameTemp;
					_mvImageDetails[j] = imageAtlasDetailsTemp;
				}
			}
		}

		// Bubble sort temp vectors by their image's height
		for (int i = 0; i < (int)vImages.size(); ++i)
		{
			for (int j = 0; j < (int)vImages.size(); ++j)
			{
				// Swap by largest first
				if (_mvImageDetails[j].v2fDimensions.y < _mvImageDetails[i].v2fDimensions.y)
				{
					// Temporarily store image, filename and image details
					// Assign i to temp
					pImageTemp = vImages[i];
					strFilenameTemp = vStrImageFilenames[i];
					imageAtlasDetailsTemp = _mvImageDetails[i];
					// Assign j to i
					vImages[i] = vImages[j];
					vStrImageFilenames[i] = vStrImageFilenames[j];
					_mvImageDetails[i] = _mvImageDetails[j];
					// Assign temp to j
					vImages[j] = pImageTemp;
					vStrImageFilenames[j] = strFilenameTemp;
					_mvImageDetails[j] = imageAtlasDetailsTemp;
				}
			}
		}
		// Now, vStrImageFilenames, vImages and _mvImageDetails contain the filename and images sorted by greatest width, then height first

		// Now we have to following arrays filled...
		// std::vector<std::string> vStrImageFilenames;		// Holds each individual image's filename 
		// std::vector<CImage*> vImages;					// Holds each individual image's image data
		// std::vector<CImageAtlasDetails> _mvImageDetails;	// Holds each individual image's details
		// _mvImageDetails[X].sTexCoords is not computed yet, we'll temporarily use this to hold position of each individual image inside of texture atlas
		// and then convert to actual texture coordinates below.

		// We now need to compute each image's position within the large atlas image and once done, we'll have the dimensions of the atlas image/s and the number required.
		std::vector<CVector2f> vv2fAtlasDims;	// Will hold computed dims of each atlas image
		vv2fAtlasDims.push_back(CVector2f(0.0f, (float)vImages[0]->getHeight()));
		int iCurAtlasImage = 0;
		CVector2f v2fCurrentPositionInAtlas(0.0f, 0.0f);
		float fSpacing = (float)uiImageSpacing;
		float fSpacingTimesTwo = fSpacing * 2.0f;
		
		// We go from left to right for each row, adding images one at a time, then when we can no longer add any more
		// images along the X axis, we move down a row and add more images starting from the left again.
		for (unsigned int ui = 0; ui < vImages.size(); ui++)
		{
			// Compute position of current image in atlas
			CVector2f vImageDims = vImages[ui]->getDimensions();
			_mvImageDetails[ui].sTexCoords.top_left.x = fSpacing + v2fCurrentPositionInAtlas.x;
			_mvImageDetails[ui].sTexCoords.top_left.y = fSpacing + v2fCurrentPositionInAtlas.y;
			_mvImageDetails[ui].sTexCoords.bottom_right.x = fSpacingTimesTwo + v2fCurrentPositionInAtlas.x + vImageDims.x;
			_mvImageDetails[ui].sTexCoords.bottom_right.y = fSpacingTimesTwo + v2fCurrentPositionInAtlas.y + vImageDims.y;

			// If the image position fits in current atlas row
			if (_mvImageDetails[ui].sTexCoords.bottom_right.x <= (float)uiMaxAtlasImageWidth)
			{
				// Image fits, fill in the rest of it's information
				_mvImageDetails[ui].sTexCoords.top_right.x = _mvImageDetails[ui].sTexCoords.bottom_right.x;
				_mvImageDetails[ui].sTexCoords.top_right.y = _mvImageDetails[ui].sTexCoords.top_left.y;
				_mvImageDetails[ui].sTexCoords.bottom_left.x = _mvImageDetails[ui].sTexCoords.top_left.x;
				_mvImageDetails[ui].sTexCoords.bottom_left.y = _mvImageDetails[ui].sTexCoords.bottom_right.y;

				// Update maximum atlas size
				if (vv2fAtlasDims[iCurAtlasImage].x < _mvImageDetails[ui].sTexCoords.bottom_right.x)
					vv2fAtlasDims[iCurAtlasImage].x = _mvImageDetails[ui].sTexCoords.bottom_right.x;
				if (vv2fAtlasDims[iCurAtlasImage].y < _mvImageDetails[ui].sTexCoords.bottom_right.y)
					vv2fAtlasDims[iCurAtlasImage].y = _mvImageDetails[ui].sTexCoords.bottom_right.y;

				// Move position in atlas right
				v2fCurrentPositionInAtlas.x += fSpacingTimesTwo + vImageDims.x;

				// Set atlas image index for current image
				_mvImageDetails[ui].uiAtlasImage = iCurAtlasImage;
			}
			else // This image doesn't fit along current row, so start a new one
			{
				// Move position in atlas down by max image height in row
				v2fCurrentPositionInAtlas.x = 0.0f;
				v2fCurrentPositionInAtlas.y = vv2fAtlasDims[iCurAtlasImage].y - fSpacing;

				// Compute position of current image in atlas
				_mvImageDetails[ui].sTexCoords.top_left.x = fSpacing + v2fCurrentPositionInAtlas.x;
				_mvImageDetails[ui].sTexCoords.top_left.y = fSpacing + v2fCurrentPositionInAtlas.y;
				_mvImageDetails[ui].sTexCoords.bottom_right.x = fSpacingTimesTwo + v2fCurrentPositionInAtlas.x + vImageDims.x;
				_mvImageDetails[ui].sTexCoords.bottom_right.y = fSpacingTimesTwo + v2fCurrentPositionInAtlas.y + vImageDims.y;

				// If the image position fits in current atlas
				if (_mvImageDetails[ui].sTexCoords.bottom_right.y <= (float)uiMaxAtlasImageHeight)
				{
					// Image fits, fill in the rest of it's information
					_mvImageDetails[ui].sTexCoords.top_right.x = _mvImageDetails[ui].sTexCoords.bottom_right.x;
					_mvImageDetails[ui].sTexCoords.top_right.y = _mvImageDetails[ui].sTexCoords.top_left.y;
					_mvImageDetails[ui].sTexCoords.bottom_left.x = _mvImageDetails[ui].sTexCoords.top_left.x;
					_mvImageDetails[ui].sTexCoords.bottom_left.y = _mvImageDetails[ui].sTexCoords.bottom_right.y;

					// Update maximum atlas size
					if (vv2fAtlasDims[iCurAtlasImage].x < _mvImageDetails[ui].sTexCoords.bottom_right.x)
						vv2fAtlasDims[iCurAtlasImage].x = _mvImageDetails[ui].sTexCoords.bottom_right.x;
					if (vv2fAtlasDims[iCurAtlasImage].y < _mvImageDetails[ui].sTexCoords.bottom_right.y)
						vv2fAtlasDims[iCurAtlasImage].y = _mvImageDetails[ui].sTexCoords.bottom_right.y;

					// Move position in atlas right
					v2fCurrentPositionInAtlas.x += fSpacingTimesTwo + vImageDims.x;

					// Set atlas image index for current image
					_mvImageDetails[ui].uiAtlasImage = iCurAtlasImage;
				}
				else // This image doesn't fit in this atlas and maximum dims of the atlas have been reached
				{
					iCurAtlasImage++;
					vv2fAtlasDims.push_back(CVector2f(0.0f, (float)vImages[ui]->getHeight()));
					v2fCurrentPositionInAtlas.setZero();

					// Add image to new atlas image
					_mvImageDetails[ui].sTexCoords.top_left.x = fSpacing + v2fCurrentPositionInAtlas.x;
					_mvImageDetails[ui].sTexCoords.top_left.y = fSpacing + v2fCurrentPositionInAtlas.y;
					_mvImageDetails[ui].sTexCoords.bottom_right.x = fSpacingTimesTwo + v2fCurrentPositionInAtlas.x + vImageDims.x;
					_mvImageDetails[ui].sTexCoords.bottom_right.y = fSpacingTimesTwo + v2fCurrentPositionInAtlas.y + vImageDims.y;
					_mvImageDetails[ui].sTexCoords.top_right.x = _mvImageDetails[ui].sTexCoords.bottom_right.x;
					_mvImageDetails[ui].sTexCoords.top_right.y = _mvImageDetails[ui].sTexCoords.top_left.y;
					_mvImageDetails[ui].sTexCoords.bottom_left.x = _mvImageDetails[ui].sTexCoords.top_left.x;
					_mvImageDetails[ui].sTexCoords.bottom_left.y = _mvImageDetails[ui].sTexCoords.bottom_right.y;

					// Update maximum atlas size
					if (vv2fAtlasDims[iCurAtlasImage].x < _mvImageDetails[ui].sTexCoords.bottom_right.x)
						vv2fAtlasDims[iCurAtlasImage].x = _mvImageDetails[ui].sTexCoords.bottom_right.x;
					if (vv2fAtlasDims[iCurAtlasImage].y < _mvImageDetails[ui].sTexCoords.bottom_right.y)
						vv2fAtlasDims[iCurAtlasImage].y = _mvImageDetails[ui].sTexCoords.bottom_right.y;

					// Move position in atlas right
					v2fCurrentPositionInAtlas.x += fSpacingTimesTwo + vImageDims.x;

					// Set atlas image index for current image
					_mvImageDetails[ui].uiAtlasImage = iCurAtlasImage;
				}
			}
		}

		// Now we get here...
		// iCurAtlasImage holds number of atlas images needed
		// vv2fAtlasDims[X] holds each atlas image's dimensions
		// _mvImageDetails[X].sTexCoords holds position in atlas of each individual image
		
		// Create each atlas image
		for (unsigned int ui = 0; ui < (unsigned int)iCurAtlasImage + 1; ui++)
		{
			CImage* pNewImage = new CImage;
			ThrowIfFalse(pNewImage, "CImageAtlasPacker::createAtlasImages() failed. Unable to allocate memory for atlas image.");
			pNewImage->createBlank((unsigned int)vv2fAtlasDims[ui].x, (unsigned int)vv2fAtlasDims[ui].y, 4);
			_mvAtlasImages.push_back(pNewImage);
		}

		// Now copy each image into atlas image
		for (unsigned int ui = 0; ui < vImages.size(); ui++)
		{
			vImages[ui]->copyRectTo(*_mvAtlasImages[_mvImageDetails[ui].uiAtlasImage],
				0, 0,
				(int)vImages[ui]->getWidth(),
				(int)vImages[ui]->getHeight(),
				(int)_mvImageDetails[ui].sTexCoords.top_left.x,
				(int)_mvImageDetails[ui].sTexCoords.top_left.y);

			// Now compute texture coordinates
			CVector2f vAtlasDims = _mvAtlasImages[_mvImageDetails[ui].uiAtlasImage]->getDimensions();
			// Prevent divide by zero
			ThrowIfTrue(vAtlasDims.x < 1.0f, "CImageAtlasPacker::createAtlasImages() failed. Atlas width less than 1.");
			ThrowIfTrue(vAtlasDims.y < 1.0f, "CImageAtlasPacker::createAtlasImages() failed. Atlas height less than 1.");
			CVector2f vAtlasDimsRecip(1.0f / vAtlasDims.x, 1.0f / vAtlasDims.y);

			// Compute correct image positions, not including the spacing
			// From above, top left position is correct for x and y.
			// All others include spacing and need to be recalculated.
			_mvImageDetails[ui].sTexCoords.bottom_right.x = _mvImageDetails[ui].sTexCoords.top_left.x + _mvImageDetails[ui].v2fDimensions.x;
			_mvImageDetails[ui].sTexCoords.bottom_right.y = _mvImageDetails[ui].sTexCoords.top_left.y + _mvImageDetails[ui].v2fDimensions.y;
			_mvImageDetails[ui].sTexCoords.top_right.x = _mvImageDetails[ui].sTexCoords.top_left.x + _mvImageDetails[ui].v2fDimensions.x;
			_mvImageDetails[ui].sTexCoords.top_right.y = _mvImageDetails[ui].sTexCoords.top_left.y;
			_mvImageDetails[ui].sTexCoords.bottom_left.x = _mvImageDetails[ui].sTexCoords.top_left.x;
			_mvImageDetails[ui].sTexCoords.bottom_left.y = _mvImageDetails[ui].sTexCoords.top_left.y + _mvImageDetails[ui].v2fDimensions.y;

			// Now convert from pixel postion in image to 0.0f - 1.0f
			_mvImageDetails[ui].sTexCoords.bottom_left.x *= vAtlasDimsRecip.x;
			_mvImageDetails[ui].sTexCoords.bottom_left.y *= vAtlasDimsRecip.y;
			_mvImageDetails[ui].sTexCoords.bottom_right.x *= vAtlasDimsRecip.x;
			_mvImageDetails[ui].sTexCoords.bottom_right.y *= vAtlasDimsRecip.y;
			_mvImageDetails[ui].sTexCoords.top_left.x *= vAtlasDimsRecip.x;
			_mvImageDetails[ui].sTexCoords.top_left.y *= vAtlasDimsRecip.y;
			_mvImageDetails[ui].sTexCoords.top_right.x *= vAtlasDimsRecip.x;
			_mvImageDetails[ui].sTexCoords.top_right.y *= vAtlasDimsRecip.y;
		}

		// Now we're done, we setup the hashmap for fast lookup of image details by individual image names
		for (unsigned int ui = 0; ui < _mvImageDetails.size(); ui++)
		{
			_mmapImageDetails[_mvImageDetails[ui].strImageFilename] = _mvImageDetails[ui];
		}
		
		// Free each individual loaded image
		for (unsigned int ui = 0; ui < vImages.size(); ui++)
		{
			delete vImages[ui];
		}
		vImages.clear();
	}

	unsigned int CImageAtlasPacker::getNumAtlases(void) const
	{
		return (unsigned int)_mvAtlasImages.size();
	}

	CImage* CImageAtlasPacker::getAtlasImage(unsigned int uiIndex) const
	{
		ThrowIfTrue(0 == _mvAtlasImages.size(), "CImageAtlasPacker::getAtlasImage(" + std::to_string(uiIndex) + ") failed. There are no atlas images.");
		ThrowIfTrue(uiIndex >= _mvAtlasImages.size(), "CImageAtlasPacker::getAtlasImage(" + std::to_string(uiIndex) + ") failed. Invalid atlas index given.");
		return _mvAtlasImages[uiIndex];
	}

	std::vector<CImageAtlasDetails> CImageAtlasPacker::getAllImageDetails(void) const
	{
		ThrowIfFalse(_mvImageDetails.size(), "CImageAtlasPacker::getAllImageDetails() failed. No image data currently exists.");
		return _mvImageDetails;
	}

	std::vector<CImageAtlasDetails>* CImageAtlasPacker::getAllImageDetailsPointer(void)
	{
		ThrowIfFalse(_mvImageDetails.size(), "CImageAtlasPacker::getAllImageDetailsPointer() failed. No image data currently exists.");
		return &_mvImageDetails;
	}

	unsigned int CImageAtlasPacker::getNumIndividualImages(void) const
	{
		return unsigned int(_mvImageDetails.size());
	}

	CImageAtlasDetails CImageAtlasPacker::getImageDetails(const std::string& strImageName) const
	{
		std::map<std::string, CImageAtlasDetails>::iterator it = _mmapImageDetails.find(strImageName);
		ThrowIfTrue(it == _mmapImageDetails.end(), "CImageAtlasPacker::getImageDetails(" + strImageName + ") failed. Named image doesn't exist.");
		return it->second;
	}

	void CImageAtlasPacker::reset(void)
	{
		for (unsigned int ui = 0; ui < _mvAtlasImages.size(); ui++)
		{
			delete _mvAtlasImages[ui];
		}
		_mvImageDetails.clear();
		_mmapImageDetails.clear();
	}

	bool CImageAtlasPacker::getImageExists(const std::string& strImageName) const
	{
		std::map<std::string, CImageAtlasDetails>::iterator it = _mmapImageDetails.find(strImageName);
		return (it != _mmapImageDetails.end());
	}
}