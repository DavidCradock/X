#pragma once
#include "PCH.h"
#include "colour.h"

namespace X
{
	// A particle type used with the C2DParticleSystem class
	// A particle type stores settings which are used to render a particle throughout it's life.
	// An emitter is set to emit particles of a given type.
	// A type has two stages which represent a particle's appearance at birth and at death and these are interpolated between
	// Each stage has colour, radius and image used for their apperance and also have physical properties such as mass.
	// Upon death a particle may be set to spawn another particle using a new type
	class C2DParticleType
	{
	public:
		// Default constructor, sets to default values.
		C2DParticleType();


		C2DParticleType* mpTypeToSpawnOnDeath;	// Type of the particle emitted upon particle's death, may be NULL

	};
}