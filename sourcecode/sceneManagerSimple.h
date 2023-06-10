#pragma once
#include "PCH.h"

namespace X
{
	// A simple brute force scene manager which has no spatial partitioning.
	// It shouldn't really be used for anything as other scene managers are more efficient.
	// This is here for development of other areas of code.
	// It does sort renderable object by shader and textures though
	class SceneManagerSimple
	{
	public:

	};
}