#include "PCH.h"
#include "sceneManagerLightDirectional.h"

namespace X
{
	SceneManagerLightDirectional::SceneManagerLightDirectional()
	{
		mvDirection.x = 0.5f;
		mvDirection.y = 0.5f;
		mvDirection.z = -0.5f;
		mvDirection = glm::normalize(mvDirection);

		mvColour = glm::vec3(1.0f, 0.5f, 0.5f);
	}
}