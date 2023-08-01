#pragma once
#include "PCH.h"

namespace X
{
	class CResourceBase
	{
	public:
		// Create all OpenGL context dependent objects when an OpenGL context exists.
		// As this is purely virtual, we need to implement this in each derived class otherwise the compiler won't be able to instantiate the abstract class.
		virtual void onGLContextCreated(void) = 0;

		// Free all OpenGL context dependent objects before the OpenGL context is destroyed.
		// As this is purely virtual, we need to implement this in each derived class otherwise the compiler won't be able to instantiate the abstract class.
		virtual void onGLContextToBeDestroyed(void) = 0;
	};
}