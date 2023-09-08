#pragma once
#include "PCH.h"
#include "resourceBase.h"

namespace X
{
	class CResourceMouseCursor : public CResourceBase
	{
	public:
		CResourceMouseCursor(const std::string& strFilename);
		~CResourceMouseCursor();

		// Create all OpenGL context dependent objects when an OpenGL context exists.
		void onGLContextCreated(void);

		// Free all OpenGL context dependent objects before the OpenGL context is destroyed.
		void onGLContextToBeDestroyed(void);
	private:
		std::string _mstrFilename;

	};
}