#pragma once
#include "PCH.h"

namespace X
{
	// A representation of a plane in 3D space.
	// A plane is defined by two things.
	// 1. Distance to the origin
	// 2. A normal vector
	class Plane
	{
	public:
		Plane();

		glm::vec3 mvDirection;
		float mfDistanceToOrigin;
	};
}