#pragma once
#include "PCH.h"
#include "vector2f.h"
#include "2DParticleType.h"

namespace X
{
	// A particle emitter used with the C2DParticleSystem class
	// There can be multiple emitters and they are responsible for inserting particles into the system.
	// They have a position, a max distance from position, birth rate and particle type.
	class C2DParticleEmitter
	{
	public:
		CVector2f vPosition;		// Position of emitter
		float fBirthRatePerSecond;	// Number of particles born per second
		float fMaxRangeFromPos;		// Maximum distance from position the new particles will be created at.
		std::string strParticleTypeName;	// The name of the particle type stored in C2DParticle system this emitter gives to it's particles
		C2DParticleType* pParticleType;		// The pointer to the particle type stored in C2DParticle system this emitter gives to it's particles

	private:
	};
}