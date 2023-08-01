#pragma once
#include "PCH.h"
#include "vector3f.h"
#include "colour.h"

namespace X
{
	// A point light which is added to the various scene managers
	class CSMLightPoint
	{
	public:
		CSMLightPoint();

		CVector3f mvPosition;	// Position of the light, in world space
		CColour mvColour;		// Colour of the light, defaults to white
		float mfSpecularAttenuationLinear;		// Affects specular attenuation. See comments in .cpp file for more information.
		float mfSpecularAttenuationQuadratic;	// Affects specular attenuation. See comments in .cpp file for more information.
	};
}