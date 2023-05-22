#pragma once
#include "PCH.h"

namespace X
{
	// A Vulkan graphics pipeline
	class GraphicsPipeline
	{
	public:
		GraphicsPipeline();
		~GraphicsPipeline();

		// Used by manager to load the resource, ready for use.
		void load(void);

		// Used by manager to unload the resource, freeing memory.
		void unload(void);

	private:

		
	};
}