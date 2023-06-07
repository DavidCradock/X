#pragma once
#include "PCH.h"
#include "resourceBase.h"

namespace X
{
	class ResourceVertexbuffer : public ResourceBase
	{
	public:
		ResourceVertexbuffer();
		~ResourceVertexbuffer();

		// Create all OpenGL context dependent objects when an OpenGL context exists.
		void onGLContextCreated(void);

		// Free all OpenGL context dependent objects before the OpenGL context is destroyed.
		void onGLContextToBeDestroyed(void);

	};
}