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

		CTimer timer;	timer.update();

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

		timer.update();

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
		SCGUIManager::getPointer()->getContainer("Debug")->getText("Text: 0")->mstrText = "Total num images: " + std::to_string(vImages.size());
		SCGUIManager::getPointer()->getContainer("Debug")->getText("Text: 20")->mstrText = "Time to load all images: " + std::to_string(timer.getSecondsPast()) + " seconds.";

		timer.update();
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
					imageDetail.v2fDimensions = vImages[ui]->getDimensions();	// Update dims
				}
			}
			_mvImageDetails.push_back(imageDetail);
		}
		timer.update();
		// Debug text
		SCGUIManager::getPointer()->getContainer("Debug")->getText("Text: 40")->mstrText = "Time to rotate images: " + std::to_string(timer.getSecondsPast()) + " seconds.";


		timer.update();

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
					// Temporarily store image and filename
					pImageTemp = vImages[i];
					strFilenameTemp = vStrImageFilenames[i];
					// Assign j to i
					vImages[i] = vImages[j];
					vStrImageFilenames[i] = vStrImageFilenames[j];
					// Assign temp to j
					vImages[j] = pImageTemp;
					vStrImageFilenames[j] = strFilenameTemp;
				}
			}
		}
		timer.update();
		// Debug text
		SCGUIManager::getPointer()->getContainer("Debug")->getText("Text: 60")->mstrText = "Time to bubblesort vecs by width: " + std::to_string(timer.getSecondsPast()) + " seconds.";

		timer.update();

		// Bubble sort temp vectors by their image's height
		for (int i = 0; i < (int)vImages.size(); ++i)
		{
			for (int j = 0; j < (int)vImages.size(); ++j)
			{
				// Swap by largest first
				if (_mvImageDetails[j].v2fDimensions.y < _mvImageDetails[i].v2fDimensions.y)
				{
					// Temporarily store image and filename
					pImageTemp = vImages[i];
					strFilenameTemp = vStrImageFilenames[i];
					// Assign j to i
					vImages[i] = vImages[j];
					vStrImageFilenames[i] = vStrImageFilenames[j];
					// Assign temp to j
					vImages[j] = pImageTemp;
					vStrImageFilenames[j] = strFilenameTemp;
				}
			}
		}
		// Now, vStrImageFilenames and vImages contain the filename and images sorted by greatest width, then height first
		timer.update();
		// Debug text
		SCGUIManager::getPointer()->getContainer("Debug")->getText("Text: 80")->mstrText = "Time to bubblesort vecs by height: " + std::to_string(timer.getSecondsPast()) + " seconds.";



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