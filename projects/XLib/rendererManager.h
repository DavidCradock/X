#pragma once
#include "PCH.h"
#include "rendererOpenGL.h"
#include "rendererVulkan.h"

namespace X
{
	class RendererManager
	{
	public:
		RendererManager();
		~RendererManager();

		Renderer* initOpenGL(void);
		Renderer* initVulkan(void);
		Renderer* get(void);
	private:
		Renderer* renderer;
	};
}