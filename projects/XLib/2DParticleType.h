#pragma once
#include "PCH.h"
#include "colour.h"

namespace X
{
	// A particle type used with the C2DParticleSystem class
	// A particle type stores settings which are used to render a particle throughout it's life.
	// An emitter is set to emit particles of a given type.
	// A type can have multiple stages which represent a particle's appearance thoughout specific stages in a particle's life.
	// Each stage has colour, radius and image used for their apperance and also have physical properties such as mass.
	class C2DParticleType
	{
	public:

		// Default constructor, sets to default.
		C2DParticleType();

		

		// A struct to represent a single stage in a particle's life span
		struct SStage
		{
			
		};
		std::vector<SStage> mvStages;	// Vector holding each stage in a particle's life


	};
}