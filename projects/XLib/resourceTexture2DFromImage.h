#pragma once
#include "PCH.h"
#include "resourceBase.h"
#include "image.h"

namespace X
{
	class CResourceTexture2DFromImage : public CResourceBase
	{
	public:
		CResourceTexture2DFromImage(const CImage& image);
		~CResourceTexture2DFromImage();

		// Create all OpenGL context dependent objects when an OpenGL context exists.
		void onGLContextCreated(void);

		// Free all OpenGL context dependent objects before the OpenGL context is destroyed.
		void onGLContextToBeDestroyed(void);

		// Binds the texture to the given texture unit
		// uiTextureUnit should be between 0-7
		void bind(unsigned int uiTextureUnit = 0) const;

		// Unbinds texturing for the specified texture unit
		// uiTextureUnit should be between 0-7
		void unbind(unsigned int uiTextureUnit = 0) const;

		// Unbind all texture units
		void unbindAll(void) const;

		// Update the stored image data with the image given and update texture
		// If passed image contains no data, an exception occurs
		void update(const CImage& image);

		CImage _mImage;				// Image holding the textures image data
		unsigned int _muiTextureID;	// OpenGL texture ID
		glm::vec2 mvDimensions;		// Dimensions of the texture
	};
}