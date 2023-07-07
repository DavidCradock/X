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
		std::string strName;			// The filename which the image was created from.
		unsigned int uiAtlasImage;		// The atlas image number which the image is located in.
		struct STexCoords
		{
			CVector2f top_left;
			CVector2f top_right;
		};
		STexCoords sv2fTexCoord;	// The texture coordinates within the atlas image of the image
		
	};

	// Image atlas class used to create image atlas from multiple images.
	// We specify a maximum size that an individual atlas image may be (Good for setting max dims to max supported texture size of GPU)
	// We also provide a vector of strings holding each individual image's filename.
	// Each individual image can have different dimensions.
	// When packing the images, we can allow or disallow rotation of images to allow better fitting of the images.
	// Once the pack is complete, depending upon the number of images and the maximum set size of the atlas images,
	// there may be more than one atlas image.
	// After the pack is complete, we can then access each image by name and obtain which atlas image it is stored in and we can also
	// obtain the texture coordinates which can be used to render each individual image.
	class CImageAtlasPacker
	{
	public:
		CImageAtlasPacker();
		~CImageAtlasPacker();

		void createAtlas(const std::vector<std::string>& vImages);
		
		std::vector<CImage*> _mvAtlasImages;		// Holds each atlas image which contain all the added images.


	};
}