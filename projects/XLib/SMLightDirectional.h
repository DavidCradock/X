#pragma once
#include "PCH.h"

namespace X
{
	// A directional light which is added to the various scene managers
	class SceneManagerLightDirectional
	{
	public:
		SceneManagerLightDirectional();

		glm::vec3 mvDirection;	// Direction of the light, in world space
		glm::vec3 mvColour;		// Colour of the light, defaults to white
	};
}