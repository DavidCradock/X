#pragma once
#include "PCH.h"
#include "image.h"
#include "vector2f.h"

namespace X
{
	// Holds information about each packed image inside an atlas image
	class CImageAtlasDetails
	{
	public:
		struct STexCoords
		{
			CVector2f top_left;
			CVector2f top_right;
			CVector2f bottom_left;
			CVector2f bottom_right;
		};
		STexCoords sTexCoords;			// The texture coordinates within the atlas image of the image
		CVector2f v2fDimensions;		// The dimensions of the image
		std::string strImageFilename;	// The filename which the image was created from.
		unsigned int uiAtlasImage;		// The atlas image number which the image is located in.
		bool bRotated;					// Whether the image was rotated clockwise to fit better or not.
	};

	// Image atlas class used to pack many smaller images into as few as possible larger atlas images.
	class CImageAtlasPacker
	{
	public:
		CImageAtlasPacker();
		~CImageAtlasPacker();

		// This creates texture atlas images.
		// We provide a vector of strings holding each individual image's filename.
		// Each individual image can have different dimensions.
		// We specify a maximum width and height that an individual atlas image may be.
		// We specift whether to allow or disallow rotation of images to allow better fitting of the images.
		// If rotation is enabled, the computed texture coordinates are rotated accordingly.
		// After calling this, we can call the various other methods to get at the information
		// If the vector of strings is empty, an exception occurs
		// If an individual image's dimensions, including spacing is greater than th max atlas dimensions, an exception occurs.
		void createAtlasImages(
			const std::vector<std::string>& vstrImages,	// A vector of strings holding each individual image's filename which to add to the atlas image/s
			unsigned int uiMaxAtlasImageWidth,			// The maximum allowed width of the created atlas image/s
			unsigned int uiMaxAtlasImageHeight,			// The maximum allowed height of the created atlas image/s
			bool bAllowRotationOfImages,				// Whether to allow rotation of individual images to obtain greated packing efficiency or not.
			unsigned int uiImageSpacing = 1				// Amount of spacing between individual images.
		);

		// Returns the number of generated atlas images holding the individual images
		unsigned int getNumAtlasImages(void);

		// Returns a pointer to the atlas image at the specified index
		// If an invalid index is given, or no atlases exist, an exception occurs
		CImage* getAtlasImage(unsigned int uiIndex);

		// Returns a vector of CImageAtlasDetails containing each image's details
		// If no image data exists, an exception occurs.
		std::vector<CImageAtlasDetails> getImageDetails(void);
	private:
		std::vector<CImage*> _mvAtlasImages;				// Holds each atlas image which contain all the added images.
		std::vector<CImageAtlasDetails> _mvImageDetails;	// Holds each image's details

		// Resets everything, freeing computed data and atlas images.
		void _reset(void);

	};
}