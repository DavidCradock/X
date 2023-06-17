#pragma once
#include "PCH.h"

namespace X
{
	// This is an object which is inserted into the various scene managers
	// This is a base class which we derive other classes from, we don't use these directly.
	// This is because all entitys have their position, rotation and scale, but differ by what they render (meshes, particles etc)
	class SMEntityBase
	{
	public:
		SMEntityBase();
		
		glm::mat4 matrixWorld;	// Holds the world transform of this entity (position,
	};
}