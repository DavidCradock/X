#include "PCH.h"
#include "imageAtlas.h"
#include "log.h"
#include "GUIManager.h"	// For debugging text access

namespace X
{
	CImageAtlasPacker::CImageAtlasPacker()
	{
	}

	CImageAtlasPacker::~CImageAtlasPacker()
	{
		_reset();
	}

	void CImageAtlasPacker::createAtlasImages(const std::vector<std::string>& vstrImages, unsigned int uiMaxAtlasImageWidth, unsigned int uiMaxAtlasImageHeight, bool bAllowRotationOfImages, unsigned int uiImageSpacing)
	{
		ThrowIfTrue(0 == vstrImages.size(), "CImageAtlasPacker::createAtlasImages() failed. The passed vector<string> containing individual image filenames contains no data.");
		ThrowIfTrue(0 == uiMaxAtlasImageWidth || 0 == uiMaxAtlasImageHeight, "CImageAtlasPacker::createAtlasImages() failed. uiMaxAtlasImageWidth or uiMaxAtlasImageHeight was set to zero.");

		// Reset, freeing atlas images if they exist
		_reset();

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
		}

		// Determine maximum width/height of the individual images and make sure, including image spacing, they can all fit within the specified max atlas image widht/height
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
		ThrowIfTrue(uiMaxImageWidth > uiMaxAtlasImageWidth, "CImageAtlasPacker::createAtlasImages() failed.An individual image's width(" + std::to_string(uiMaxImageWidth) + "), including spacing of " + std::to_string(uiImageSpacing) + " is greater than the specified max atlas image's width(" + std::to_string(uiMaxAtlasImageWidth) + ".");
		ThrowIfTrue(uiMaxImageHeight > uiMaxAtlasImageHeight, "CImageAtlasPacker::createAtlasImages() failed.An individual image's height(" + std::to_string(uiMaxImageHeight) + "), including spacing of " + std::to_string(uiImageSpacing) + " is greater than the specified max atlas image's height(" + std::to_string(uiMaxAtlasImageHeight) + ".");

		// Debug text
		SCGUIManager::getPointer()->getContainer("Debug")->getText("Text: 0")->mstrText = "Hello!";
		// Free each individual loaded image
		for (unsigned int ui = 0; ui < vImages.size(); ui++)
		{
			delete vImages[ui];
		}
		vImages.clear();
	}

	unsigned int CImageAtlasPacker::getNumAtlasImages(void)
	{
		return (unsigned int)_mvAtlasImages.size();
	}

	CImage* CImageAtlasPacker::getAtlasImage(unsigned int uiIndex)
	{
		ThrowIfTrue(0 == _mvAtlasImages.size(), "CImageAtlasPacker::getAtlasImage(" + std::to_string(uiIndex) + ") failed. There are no atlas images.");
		ThrowIfTrue(uiIndex >= _mvAtlasImages.size(), "CImageAtlasPacker::getAtlasImage(" + std::to_string(uiIndex) + ") failed. Invalid atlas index given.");
		return _mvAtlasImages[uiIndex];
	}

	std::vector<CImageAtlasDetails> CImageAtlasPacker::getImageDetails(void)
	{
		ThrowIfFalse(_mvImageDetails.size(), "CImageAtlasPacker::getImageDetails() failed. No image data currently exists.");
		return _mvImageDetails;
	}

	void CImageAtlasPacker::_reset(void)
	{
		for (unsigned int ui = 0; ui < _mvAtlasImages.size(); ui++)
		{
			delete _mvAtlasImages[ui];
		}
	}
}