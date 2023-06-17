#include "PCH.h"
#include "SMLightDirectional.h"

namespace X
{
	SMLightDirectional::SMLightDirectional()
	{
		mvDirection.x = 0.5f;
		mvDirection.y = 0.5f;
		mvDirection.z = -0.5f;
		mvDirection = glm::normalize(mvDirection);

		mvColour = glm::vec3(1.0f, 1.0f, 1.0f);
	}
}