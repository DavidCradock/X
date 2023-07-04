#pragma once
#include "PCH.h"
#include "singleton.h"

namespace X
{
	// This is a single rendered entity which is created and then added to the CRenderQueue for rendering
	class CRenderObject
	{
	public:

		std::string mstrGeometry;				// Name of the geometry stored in the GeometryManager
		std::string mstrGraphicsPipeline;		// Name of the graphics pipeline stored in GraphicsPipelineManager
		std::vector<std::string> mvstrTextures;	// Name or names of the textures stored in the TextureManager
	};

	// This is used by the window to get everything it needs to render for a frame.
	// Other areas of the sourcecode, including the main applications should use this
	// to submit everything that is needed to be rendered for a frame.
	// For something to be rendered, we need these things...
	// 1. Some geometry to render.
	// 2. Some texture to use whilst rendering the geometry
	// 3. GPU shaders used whilst rendering the geometry
	class CRenderQueue : public CSingleton<CRenderQueue>
	{
	public:

		std::vector<CRenderObject> mvRenderObjects;	// Each render object added to this queue
	};

}