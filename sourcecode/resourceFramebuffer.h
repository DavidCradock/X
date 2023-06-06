#pragma once
#include "PCH.h"
#include "resourceBase.h"

namespace X
{
	class ResourceFramebuffer : public ResourceBase
	{
	public:
		ResourceFramebuffer(unsigned int iWidth, unsigned int iHeight);
		~ResourceFramebuffer();

		// Create all OpenGL context dependent objects when an OpenGL context exists.
		void onGLContextCreated(void);

		// Free all OpenGL context dependent objects before the OpenGL context is destroyed.
		void onGLContextToBeDestroyed(void);

		unsigned int _muiWidth;
		unsigned int _muiHeight;
	};
}