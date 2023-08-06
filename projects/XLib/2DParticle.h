#pragma once
#include "PCH.h"
#include "vector2f.h"
#include "colour.h"

namespace X
{
	class C2DParticleType;

	// A particle system used with the C2DParticleSystem class
	// The stars of the show! (Quite literally if given a star shaped texture :))
	// A particle has it's current position, velocity, mass and it's age as well as it's type
	class C2DParticle
	{
	public:
		CVector2f vPosition;	// Position within the world of this particle
		CVector2f vVelocity;	// Current velocity of this particle.
		float fMass;			// Mass of this particle. Minumum value is 0.1f
		float fMassBirth;		// Mass of this particle at birth, given by the emitter
		float fMassDeath;		// Mass of this particle at death, given by the emitter
		float fAge;				// Current age of the particle. Starts at 0.0f when born and proceeds towards 1.0f when the particle dies.
		float fAgingRate;		// The rate at which the particle ages.
		C2DParticleType* pType;	// Pointer to particle type stored in C2DParticleSystem, given to particle by CParticleEmitter upon birth.
		CColour colourAtBirth;	// Colour this particle was given by the emitter for it's birth
		CColour colourAtDeath;	// Colour this particle was given by the emitter for it's death
		float fRadius;			// Computed during update
		float fRadiusAtBirth;	// Radius of this particle was given by the emitter for it's birth
		float fRadiusAtDeath;	// Radius of this particle was given by the emitter for it's death

	};
}