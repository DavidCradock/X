#pragma once
#include "PCH.h"
#include "image.h"
#include "singleton.h"

namespace X
{
	// Image atlas class used to create image atlas from multiple images.
	// We specify a maximum size that an individual atlas image may be (Good for setting max dims to max supported texture size of GPU)
	// We also provide a vector of strings holding each individual image's filename.
	// Each individual image can have different dimensions.

	class SCImageAtlasPacker : public CSingleton<SCImageAtlasPacker>
	{
	public:
		SCImageAtlasPacker();
		~SCImageAtlasPacker();

		void createAtlas(const std::vector<std::string>& vImages);
		
	};
}