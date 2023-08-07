#pragma once
#include "PCH.h"
#include "resourceBase.h"
#include "vector2f.h"
#include "colour.h"

namespace X
{
	class CResourceTexture2DFromFile : public CResourceBase
	{
	public:
		CResourceTexture2DFromFile(const std::string& strImageFilename, bool bFlipYaxis = false);
		~CResourceTexture2DFromFile();

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

		// Renders the contents of this texture to a 2D quad with specified position and dimensions
		void renderTo2DQuad(int iPosX, int iPosY, int iWidth, int iHeight, CColour colour = CColour());

		// Returns dimensions of the texture
		CVector2f getDimensions(void) const;
	private:
		std::string _mstrImageFilename;
		unsigned int _muiTextureID;
		bool _mbFlipYaxis;	// Whether to flip the image data in during loading or not.
		CVector2f _mvDimensions;	// Dimensions of the texture
	};
}