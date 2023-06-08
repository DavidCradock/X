#pragma once
#include "PCH.h"
#include "resourceBase.h"

namespace X
{
	class ResourceTexture2D : public ResourceBase
	{
	public:
		ResourceTexture2D(const std::string& strImageFilename);
		~ResourceTexture2D();

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

		std::string _mstrImageFilename;
		unsigned int _muiTextureID;
	};
}