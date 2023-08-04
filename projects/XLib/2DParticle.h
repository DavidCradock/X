#pragma once
#include "PCH.h"
#include "vector2f.h"

namespace X
{
	// A particle system used with the C2DParticleSystem class
	class C2DParticle
	{
	public:
		CVector2f vPosition;	// Position within the world of this particle
		CVector2f vVelocity;	// Current velocity of this particle.
		float fMass;			// Mass of this particle. Minumum value is 0.1f
		float fAge;				// Current age of the particle. Starts at 0.0f when born and proceeds towards 1.0f when the particle dies.

	};
}