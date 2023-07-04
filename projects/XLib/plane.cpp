#include "PCH.h"
#include "plane.h"

namespace X
{
	CPlane::CPlane()
	{
		mvDirection = glm::vec3(0.0f, 1.0f, 0.0f);
		mfDistanceToOrigin = 0.0f;
	}
}