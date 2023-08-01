#pragma once
#include "PCH.h"
#include "vector3f.h"
#include "colour.h"

namespace X
{
	// A directional light which is added to the various scene managers
	class CSMLightDirectional
	{
	public:
		CSMLightDirectional();

		CVector3f mvDirection;	// Direction of the light, in world space
		CColour mvColour;		// Colour of the light, defaults to white
	};
}