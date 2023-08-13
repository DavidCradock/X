#pragma once
#include "PCH.h"
#include "imageAtlas.h"
#include "resourceTexture2DAtlas.h"

namespace X
{
	// An image type used by C2DMap
	// Holds information about a tile's image or images (frames)such as...
	// Image details (texture coordinates & dimensions)
	// Also deals with animation of the frames by calling update()
	class C2DMapImageType
	{
		friend class C2DMap;
	public:
		C2DMapImageType(const std::string& strResourceTexture2DAtlasName);
		~C2DMapImageType();

		// Updates the current frame image number and loops it
		void update(float fTimeDeltaSeconds);

		// Sets frame rate of this image type in frames per second
		// The default is 30 fps.
		// Can be set to zero.
		void setFramerate(float fFramesPerSecond);

		// Gets currently set framerate in frames per second.
		float getFramerate(void) const;

		// Sets current frame number for this image type
		// If an invalid frame number is given, an exception occurs
		void setCurrentFrameNumber(int iFrameNumber);

		// Returns current frame number this image type is using
		int getCurrentFrameNumber(void) const;

		// Returns total number of frames for this image type
		int getNumberOfFrames(void) const;

		// Adds an image to use as a frame of animation for this type.
		void addFrameImage(const std::string& strImageFilenameInAtlas);

		// Returns the name of the current image frame
		std::string getCurrentFrameImageName(void) const;
		
		// Returns the dimensions of the current image frame (They should all be the same)
		CVector2f getCurrentFrameImageDimensions(void) const;

		// Returns the texture coordinates within the texture atlas for the current image frame
		CImageAtlasDetails::STexCoords getCurrentFrameImageTextureCoords(void) const;
	private:
		CResourceTexture2DAtlas* _mpAtlas;					// Holds pointer of the CResourceTexture2DAtlas this image type is using.
		std::string _mstrResourceTexture2DAtlasName;		// Holds name of the CResourceTexture2DAtlas this image type is using.
		float _mfCurrentFrame;								// Used to hold current frame for this image type

		// Stuff loaded/saved in files
		std::vector<CImageAtlasDetails> _mvecImageFrames;	// Details about each image inside the atlas for each frame of animation.
		float _mfAnimRateFPS;								// Rate at which frames progress in frames per second.
	};
}