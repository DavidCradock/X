#pragma once
#include "PCH.h"
#include "vector2f.h"
#include "2DParticleType.h"

namespace X
{
	class C2DParticleSystem;

	// A particle emitter used with the C2DParticleSystem class
	// There can be multiple emitters and they are responsible for inserting particles into the system.
	// They have a position, a max distance from position, birth rate and particle type.
	class C2DParticleEmitter
	{
		friend class C2DParticleSystem;
	public:
		// Constructor, sets to default
		C2DParticleEmitter(C2DParticleSystem* pSystem);

		
	private:
		// Update this emitter.
		// Called from C2DParticleSystem
		void _update(float fTimeDeltaSeconds);

		C2DParticleSystem* _mpPS;				// The particle system this object belongs to.
		CVector2f _mvPosition;					// Position of emitter
		CVector2f _mvMinVelocity;				// Minimum velocity of a newly emitted particle
		CVector2f _mvMaxVelocity;				// Maximum velocity of a newly emitted particle
		float _mfTimeUntilNextEmission;			// When less than 0.0f, emit a particle
		bool _mbEmitting;						// Emitting particles or not?
		float _mfBirthRatePerSecond;			// Number of particles born per second
		float _mfMinRangeFromPos;				// Minimum distance from position the new particles will be created at.
		float _mfMaxRangeFromPos;				// Maximum distance from position the new particles will be created at.
		std::string _mstrParticleTypeName;		// The name of the particle type stored in C2DParticle system this emitter gives to it's particles
		C2DParticleType* _mpParticleType;		// The pointer to the particle type stored in C2DParticle system this emitter gives to it's particles

	};
}