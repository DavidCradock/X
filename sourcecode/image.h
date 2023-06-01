#pragma once
#include "PCH.h"

namespace X
{
	// A class for creating/loading/saving/modifying 2D images
	// Can read the following formats...
	// JPEG baseline & progressive (12 bpc/arithmetic not supported, same as stock IJG lib)
	// PNG 1 / 2 / 4 / 8 / 16 - bit - per - channel
	// TGA(not sure what subset, if a subset)
	// BMP non - 1bpp, non - RLE
	// PSD(composited view only, no extra channels, 8 / 16 bit - per - channel)
	// GIF(*comp always reports as 4 - channel)
	// HDR(radiance rgbE format)
	// PIC(Softimage PIC)
	// PNM(PPM and PGM binary only)
	class Image
	{
	public:
		Image();
		~Image();

		// Free image and memory
		void free(void);

		// Create a blank image.
		// If already created, the previous image is freed
		// Each channel contains black
		// Acceptable number of channels may be 3 or 4
		// Both dimensions must be at least 1
		// iWidth The width of the image.
		// iHeight The height of the image.
		// Memory consumption examples...
		//  Each pixel is either RGB or RGBA and each is a byte, so and RGBA image is 4 bytes per pixel.
		//  An image of dimensions 512x512x4 is 1,048,576 bytes or 1024 KBytes or 1 MByte
		//  An image of dimensions 1024x1024x4 is 4,194,304 bytes or 4096 KB or 4 MB
		//  An image of dimensions 4096x4096x4 is 64MB
		//  An image of dimensions 16384x16384x4 is 1024MB AKA 1GB
		void createBlank(unsigned int iWidth, unsigned int iHeight, unsigned short iNumChannels);

		// Attempts to load the image data from a file stored on disk.
		// Depending upon the file name extension, determines the file type and loads it in.
		// If the image couldn't be loaded, false is returned, else true
		// The image is freed at the start of this method
		// Loads image from file using the stb_image library
		bool load(const std::string& strFilename, bool bFlipForOpenGL = false);

		// Attempts to read only the image width and height from the given filename, which is faster than loading the whole thing in.
		// Returns true if the image data was loaded, else false.
		bool loadInfo(const std::string& strFilename, int& iWidth, int& iHeight, int& componentCount);

		// Fills the image with the given colour values.
		// If the image only contains 3 colour channels, the alpha component is ignored. 
		void fill(unsigned char ucRed, unsigned char ucGreen, unsigned char ucBlue, unsigned char ucAlpha = 255);

		// Return pointer to image data for manual modification. 
		// Please, BE CAREFULL if you're using this method to directly access the image data.
		// returns The pointer to the image's data. 
		unsigned char* getData(void) { return pData; }

		// Get size of image data in bytes
		// returns The size of the image data 
		unsigned int getDataSize(void) const { return dataSize; }

		// Get width of image
		// returns The width of the image. 
		unsigned int getWidth(void) const { return width; }

		// Get height of image
		// returns The height of the image. 
		unsigned int getHeight(void) const { return height; }

		// Get number of channels
		// returns The number of channels of the image. 
		unsigned int getNumChannels(void) const { return numChannels; }

		// Returns whether this texture's width and height are to power of two
		bool getDimsArePowerOfTwo(void);

		// Sets pixel at given coordinate to given values.
		// Due to additional overhead of this method due to function calling and bounds checking,
		// if you're wanting to modify many pixels, it's best to use the "unsafe" getData() method. 
		// iX The X position of the pixel. Where a value of 0 = left edge of image.
		// iY The Y position of the pixel. Where a value of 0 = bottom edge of image.
		// r The colour to set the pixel to (Red).
		// g The colour to set the pixel to (Green). 
		// b The colour to set the pixel to (Blue). 
		// a The colour to set the pixel to (Alpha). 
		inline void setPixel(int iX, int iY, unsigned char r, unsigned char g, unsigned char b, unsigned char a);

		// Gets pixel values at specified position
		// Due to additional overhead of this method due to function calling and bounds checking,
		// if you're wanting to modify many pixels, it's best to use the "unsafe" getData() method.
		// iX The X position of the pixel. Where a value of 0 = left edge of image.
		// iY The Y position of the pixel. Where a value of 0 = bottom edge of image.
		// r The colour will be held in here (Red). 
		// g The colour will be held in here (Green). 
		// b The colour will be held in here (Blue). 
		// a The colour will be held in here (Alpha). 
		inline void getPixel(int iX, int iY, unsigned char& r, unsigned char& g, unsigned char& b, unsigned char& a);

		// Swap red and blue colour components around
		void swapRedAndBlue(void);

		// Save image as BMP file to disk.
		void saveAsBMP(const std::string& strFilename, bool bFlipOnSave = false);

		// Save image to JPG file to disk
		void saveAsJPG(const std::string& strFilename, bool bFlipOnSave = false, int iQuality = 100);

		// Save image to PNG file to disk
		void saveAsPNG(const std::string& strFilename, bool bFlipOnSave = false);

		// Save image to TGA file to disk
		void saveAsTGA(const std::string& strFilename, bool bFlipOnSave = false);

		// Flip the image vertically
		void flipVertically(void);

		// Inverts the colours of the image, AKA new colour = 255 - current colour
		// returns false if no image data exists 
		bool invert(bool bInvertColour = true, bool bInvertAlpha = false);

		// Converts the image's RGB components to greyscale, simply finding mean average of RGB components
		// Returns false if no image data exists
		bool greyscaleSimple(void);

		// Converts the image's RGB components to greyscale, taking into consideration the average human's eye sensitivity to the individual RGB components.
		// If default params are not used (They approximate the average human's eye sensitivity), they should be of unit length.
		// returns false if no image data exists 
		bool greyscale(float fRedSensitivity = 0.299f,
			float fGreenSensitivity = 0.587f,
			float fBlueSensitivity = 0.144f);

		// Adjusts brightness of colour components
		// Accepted range for iAmount can be between -255 to +255 which would make entire image totally black or white.
		// returns false if no image data exists 
		bool adjustBrightness(int iAmount);

		// Adjusts contrast of the colour components
		// Accepted range for iAmount is between -100 and +100 \n
		// returns false if no image data exists.	
		bool adjustContrast(int iAmount);

		// Copies this image into the one given
		// Silently fails if both this image and the one parsed are actually the same objects, or there is no image data to copy.
		// The destinationImage is totally replaced. 
		void copyTo(Image& destImage) const;

		// Copies a rectangular region from this object, into the one given.
		// Automatic clipping is done so that if the source region doesn't fit into destination, it will be clipped.
		// The destination image can be this image itself IE img.copyRectTo(img)
		// destImage The destination image to copy the rectangular region of this object into
		// iSrcPosX The bottom left position within the source image to copy from (a value of 0 would be left edge)
		// iSrcPosY The bottom left position within the source image to copy from (a value of 0 would be bottom edge)
		// iSrcWidth The width of the region to copy from the source image
		// iSrcHeight The height of the region to copy from the source image
		// iDestPosX The bottom left position within the destination image to copy to
		// iDestPosY The bottom left position within the destination image to copy to
		void copyRectTo(Image& destImage, int iSrcPosX, int iSrcPosY, int iSrcWidth, int iSrcHeight, int iDestPosX, int iDestPosY);

		// Rotates the image 90 degrees clockwise
		void rotateClockwise(void);

		// Edge detection
		// This detects edges of this image and sets the image reference given to hold an image of equal dimensions with values of 255 for RGBA where edges were detected, else values of 0.
		// Edges are detected by using the given colour value which should represent the colour of the image's background
		void edgeDetect(Image& outputImage, unsigned char r, unsigned char g, unsigned char b);

		// Removes the alpha channel of the image, leaving the RGB components
		void removeAlphaChannel(void);

		// Copies the alpha channel to each of the RGB components
		void copyAlphaChannelToRGB(void);

	private:
		unsigned char* pData;
		int dataSize;
		int width;
		int height;
		int numChannels;

		// Used by edgeDetect()
		inline bool isPixelEdge(int iPosX, int iPosY, unsigned char r, unsigned char g, unsigned char b);

	};



	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	// Definition
	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////


	inline void Image::setPixel(int iX, int iY, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
	{
		if (iX >= width)
			return;
		if (iY >= height)
			return;

		unsigned int iIndex = iX + (iY * width);
		iIndex *= numChannels;
		switch (numChannels)
		{
		case 1:
			pData[iIndex] = r;
			break;
		case 3:
			pData[iIndex] = r;
			pData[iIndex + 1] = g;
			pData[iIndex + 2] = b;
			break;
		case 4:
			pData[iIndex] = r;
			pData[iIndex + 1] = g;
			pData[iIndex + 2] = b;
			pData[iIndex + 3] = a;
			break;
		}
	}

	inline void Image::getPixel(int iX, int iY, unsigned char& r, unsigned char& g, unsigned char& b, unsigned char& a)
	{
		if (iX >= width)
			return;
		if (iY >= height)
			return;

		unsigned int iIndex = iX + (iY * width);
		iIndex *= numChannels;
		switch (numChannels)
		{
		case 1:
			r = pData[iIndex];
			break;
		case 3:
			r = pData[iIndex];
			g = pData[iIndex + 1];
			b = pData[iIndex + 2];
			break;
		case 4:
			r = pData[iIndex];
			g = pData[iIndex + 1];
			b = pData[iIndex + 2];
			a = pData[iIndex + 3];
			break;
		}
	}

	inline bool Image::isPixelEdge(int iPosX, int iPosY, unsigned char r, unsigned char g, unsigned char b)
	{
		// Don't check edge pixels of image
		if (iPosX == 0)
			return false;
		if (iPosY == 0)
			return false;
		if (iPosX >= int(width) - 1)
			return false;
		if (iPosY >= int(height) - 1)
			return false;

		// Get center pixel colour values
		unsigned char col[4];
		getPixel(iPosX, iPosY, col[0], col[1], col[2], col[3]);

		// If the center pixel's colour is same as mask, it's not an edge
		if (col[0] == r)
			if (col[1] == g)
				if (col[2] == b)
					return false;

		// If we get here, the center pixel is not the same as the mask colour


		// Get bordering pixels of center pixel
		// If they are the same as the mask, then it's an edge

		// Left
		getPixel(iPosX - 1, iPosY, col[0], col[1], col[2], col[3]);
		if (col[0] == r)
			if (col[1] == g)
				if (col[2] == b)
					return true;
		// Right
		getPixel(iPosX + 1, iPosY, col[0], col[1], col[2], col[3]);
		if (col[0] == r)
			if (col[1] == g)
				if (col[2] == b)
					return true;

		// Top
		getPixel(iPosX, iPosY - 1, col[0], col[1], col[2], col[3]);
		if (col[0] == r)
			if (col[1] == g)
				if (col[2] == b)
					return true;

		// Bottom
		getPixel(iPosX, iPosY + 1, col[0], col[1], col[2], col[3]);
		if (col[0] == r)
			if (col[1] == g)
				if (col[2] == b)
					return true;

		// Top left
		getPixel(iPosX - 1, iPosY - 1, col[0], col[1], col[2], col[3]);
		if (col[0] == r)
			if (col[1] == g)
				if (col[2] == b)
					return true;

		// Top right
		getPixel(iPosX + 1, iPosY - 1, col[0], col[1], col[2], col[3]);
		if (col[0] == r)
			if (col[1] == g)
				if (col[2] == b)
					return true;

		// Bottom left
		getPixel(iPosX - 1, iPosY + 1, col[0], col[1], col[2], col[3]);
		if (col[0] == r)
			if (col[1] == g)
				if (col[2] == b)
					return true;

		// Bottom right
		getPixel(iPosX + 1, iPosY + 1, col[0], col[1], col[2], col[3]);
		if (col[0] == r)
			if (col[1] == g)
				if (col[2] == b)
					return true;

		return false;	// Shouldn't get here, keep compiler happy
	}

}