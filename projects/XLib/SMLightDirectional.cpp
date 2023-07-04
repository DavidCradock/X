#include "PCH.h"
#include "SMLightDirectional.h"

namespace X
{
	CSMLightDirectional::CSMLightDirectional()
	{
		mvDirection.x = 0.0f;
		mvDirection.y = -1.0f;
		mvDirection.z = 0.0f;
		mvDirection = glm::normalize(mvDirection);

		mvColour = glm::vec3(1.0f, 1.0f, 1.0f);
	}
}