#pragma once
#include "PCH.h"

namespace X
{
	// A Vulkan graphics pipeline
	class GraphicsPipeline
	{
		friend class GraphicsPipelineManager;

	public:
		GraphicsPipeline();
		~GraphicsPipeline();

		bool getLoaded(void);
		void load(void);
		void unload(void);

	private:
		bool loaded;
		unsigned int refCount;		// Used by the manager 
	};
}