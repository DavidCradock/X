#include "PCH.h"
#include "rendererManager.h"
#include "logging.h"

namespace X
{
	RendererManager::RendererManager()
	{
		renderer = 0;
	}

	RendererManager::~RendererManager()
	{

	}

	Renderer* RendererManager::initOpenGL(void)
	{
		ThrowIfTrue(renderer, "RendererManager::initOpenGL() failed. Renderer already initialised.");
		renderer = new RendererOpenGL;
		ThrowIfMemoryNotAllocated(renderer);
		return renderer;
	}

	Renderer* RendererManager::initVulkan(void)
	{
		ThrowIfTrue(renderer, "RendererManager::initVulkan() failed. Renderer already initialised.");
		renderer = new RendererVulkan;
		ThrowIfMemoryNotAllocated(renderer);
		return renderer;
	}
	Renderer* RendererManager::get(void)
	{
		ThrowIfFalse(renderer, "RendererManager::get() failed. Renderer is not initialised.");
		return renderer;
	}
}