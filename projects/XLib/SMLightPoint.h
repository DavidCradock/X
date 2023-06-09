#pragma once
#include "PCH.h"

namespace X
{
	// A point light which is added to the various scene managers
	class CSMLightPoint
	{
	public:
		CSMLightPoint();

		glm::vec3 mvPosition;	// Position of the light, in world space
		glm::vec3 mvColour;		// Colour of the light, defaults to white
		float mfSpecularAttenuationLinear;		// Affects specular attenuation. See comments in .cpp file for more information.
		float mfSpecularAttenuationQuadratic;	// Affects specular attenuation. See comments in .cpp file for more information.
	};
}