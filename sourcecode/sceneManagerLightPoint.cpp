#include "PCH.h"
#include "sceneManagerLightPoint.h"

namespace X
{
	SceneManagerLightPoint::SceneManagerLightPoint()
	{
		mvPosition.x = 10.0f;
		mvPosition.y = 10.0f;
		mvPosition.z = 10.0f;

		mvColour = glm::vec3(1.0f, 1.0f, 1.0f);
	}
}