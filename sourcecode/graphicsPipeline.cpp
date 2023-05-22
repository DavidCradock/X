#include "PCH.h"
#include "graphicsPipeline.h"
#include "log.h"

namespace X
{
	GraphicsPipeline::GraphicsPipeline(void)
	{
		loaded = false;
		refCount = 0;
	}

	GraphicsPipeline::~GraphicsPipeline(void)
	{
	}

	bool GraphicsPipeline::getLoaded(void)
	{
		return loaded;
	}

	void GraphicsPipeline::load(void)
	{

	}

	void GraphicsPipeline::unload(void)
	{

	}
}