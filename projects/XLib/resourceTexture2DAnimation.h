#pragma once
#include "PCH.h"
#include "resourceBase.h"
#include "image.h"

namespace X
{
	// Upon construction, this is given a vector of strings holding each image filename to be used for animation.
	// Each image must be the same size, otherwise an exception occurs.
	// This fits as many of the images into a single texture to reduce texture re-binding
	// The large texture images are stored in memory so we can quickly reload onGLContextCreated()
	// We decided NOT to pre-generate the large textures and store on disk because different gfx chipsets have different max texture dimensions
	// and if we wanted to support older hardware, then multiple versions would have to be created and stored.
	// This is NOT designed for holding and playing back large animation files, it's for animated sprites and such.
	// This is because it stores ALL the image frames in memory and memory can quickly fill up.
	// Say we have an animation with dimensions of 512x512, that's 4*512*512 = 1MB per frame.
	// Say the animation is 10 seconds long, at 60fps, that's 1MB * 60 * 10 = 600MB in size. Too big
	// It's more suitable for shorter animations such as used by sprites, say having 120 or so frames (Which'd be using 256x256 dims, about 32MB)
	class ResourceTexture2DAnimation : public ResourceBase
	{
	public:
		ResourceTexture2DAnimation(const std::vector<std::string>& vecStrImageFilenames, bool bFlipYaxis = false);
		~ResourceTexture2DAnimation();

		// Create all OpenGL context dependent objects when an OpenGL context exists.
		void onGLContextCreated(void);

		// Free all OpenGL context dependent objects before the OpenGL context is destroyed.
		void onGLContextToBeDestroyed(void);

		// Binds the texture to the given texture unit
		// uiTextureUnit should be between 0-7
		void bind(unsigned int uiTextureUnit = 0);

		// Unbinds texturing for the specified texture unit
		// uiTextureUnit should be between 0-7
		void unbind(unsigned int uiTextureUnit = 0);

		// Unbind all texture units
		void unbindAll(void);

		unsigned int _muiTextureID;
		bool _mbFlipYaxis;		// Whether to flip the image data in during loading or not.
		glm::vec2 mvDimensions;	// Dimensions of the texture
	private:

		// Called during construction to load in each of the images contained in the constructor's vecStrImageFilenames
		// This fits as many images into a single texture, then if another is needed, fits the next batch of images into that and so on.
		// It also computes and stores the texture number and texture coordinates for each animation frame.
		void _packImagesIntoLargeTextures(const std::vector<std::string>& vecStrImageFilenames, bool bFlipYaxis);
		
		struct AnimationFrame
		{
			unsigned int uiTextureNumber;	// The texture number which the animation frame is stored within
			glm::vec2 vTCMin;				// The minimum texture coordinates within the numbered texture which this frame is located
			glm::vec2 vTCMax;				// The maximum texture coordinates within the numbered texture which this frame is located
		};
		std::vector<AnimationFrame> _mvAnimationFrames;	// Stores information for each animation frame (texture coordinates and which large texture it's located in)
		std::vector<unsigned int> _mvLargeTextureIDs;	// OpenGL texture ID for each generated large texture holding the smaller images.
		std::vector<Image> _mvLargeImages;				// The images holding multiple images, used for generating the OpenGL texture/s.
	};
}