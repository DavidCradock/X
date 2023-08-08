#pragma once
#include "PCH.h"
#include "colour.h"

namespace X
{
	// A particle type used with the C2DParticleSystem class
	// A particle type stores settings which are used to render a particle throughout it's life.
	// An emitter is set to emit particles of a given type.
	// A type has two stages which represent a particle's appearance at birth and at death and the values
	// within the birth and death stages are interpolated between, moving from birth to death as the particle ages.
	// Each stage has colour, radius, mass and image used for their apperance.
	// Upon death a particle may be set to spawn another particle using a new type, or not.
	// Most settings have a minimum and maximum value which is the range in which a random value is generated within this range.
	// 
	class C2DParticleType
	{
	public:
		// Default constructor, sets to default values.
		C2DParticleType();

		// Holds all values of a particle at a specific stage in it's life.
		struct SLifeStage
		{
			CColour colourMin;	// Minimum colour of a particle (RGBA)
			CColour colourMax;	// Maximum colour of a particle (RGBA)

			float fMassMin;		// Minimum mass of a particle
			float fMassMax;		// Maximum mass of a particle

			float fRadiusMin;	// Minimum radius of a particle
			float fRadiusMax;	// Maximum radius of a particle

			std::string strTextureAtlasImageName;	// Name of the image stored within the texture atlas which is used for a particle.
		};

		SLifeStage stageBirth;			// Holds values of a particle at it's birth
		SLifeStage stageDeath;			// Holds values of a particle at it's death
		float fLifeSpanMinSeconds;		// Minimum random value of a particle's life in seconds
		float fLifeSpanMaxSeconds;		// Maximum random value of a particle's life in seconds

		C2DParticleType* mpTypeToSpawnOnDeath;	// Type of the particle emitted upon particle's death, may be NULL

	};
}