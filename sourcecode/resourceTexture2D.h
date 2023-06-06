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

		std::string _mstrImageFilename;
	};
}