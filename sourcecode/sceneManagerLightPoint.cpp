#include "PCH.h"
#include "sceneManagerLightPoint.h"

namespace X
{
	SceneManagerLightPoint::SceneManagerLightPoint()
	{
		mvPosition.x = 0.0f;
		mvPosition.y = 0.0f;
		mvPosition.z = 0.0f;

		mvColour = glm::vec3(1.0f, 1.0f, 1.0f);
	}
}