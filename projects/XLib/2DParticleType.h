#pragma once
#include "PCH.h"
#include "colour.h"

namespace X
{
	// A particle type used with the C2DParticleSystem class
	class C2DParticleType
	{
	public:

		// Default constructor, sets to default.
		C2DParticleType();

		// What type is this particle type
		enum Type
		{
			attractor,
			gravity,
			repellor
		};
		Type _mType;

		// A struct to represent a single stage in a particle's life span
		struct SStage
		{

		};


	};
}