#pragma once
#include "PCH.h"
#include "rendererOpenGL.h"

namespace X
{
	class RendererManager
	{
	public:
		RendererManager();
		~RendererManager();

		Renderer* initOpenGL(void);
		Renderer* get(void);
	private:
		Renderer* renderer;
	};
}