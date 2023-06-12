#pragma once
#include "PCH.h"

namespace X
{
	// A point light which is added to the various scene managers
	class SceneManagerLightPoint
	{
	public:
		SceneManagerLightPoint();

		glm::vec3 mvPosition;	// Position of the light, in world space
		glm::vec3 mvColour;		// Colour of the light, defaults to white
	};
}