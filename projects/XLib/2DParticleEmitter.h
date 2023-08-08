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

		// Sets the position of the emitter
		// Default is 0, 0
		void setPosition(const CVector2f& pNewPosition);

		// Returns currently set position of the emitter
		// Default is 0, 0
		CVector2f getPosition(void) const;

		// Sets minimum velocity a newly spawned particle could have
		// Default is -50.0f, -50.0f
		void setMinimumVelocity(const CVector2f& vMinimumVelocity);

		// Get currently set minimum velocity a newly spawned particle could have
		// Default is -50.0f, -50.0f
		CVector2f getMinimumVelocity(void) const;

		// Sets maximum velocity a newly spawned particle could have
		// Default is 50.0f, 50.0f
		void setMaximumVelocity(const CVector2f& vMaximumVelocity);

		// Get currently set maximum velocity a newly spawned particle could have
		// Default is 50.0f, 50.0f
		CVector2f getMaximumVelocity(void) const;

		// Sets whether this emitter is emitting particles or not
		// Default is true
		void setEmitting(bool bEmittingParticles);

		// Returns whether this emitter is emitting particles or not
		// Default is true
		bool getEmitting(void) const;

		// Sets the birth rate of particles
		// Default is 50
		void setBirthRate(float fNumberOfParticlesPerSecond);

		// Returns the birth rate of particles in number of particles per second
		// Default is 50
		float getBirthRate(void) const;

		// Sets the minimum distance a particle may be emitted from the emitter's position.
		// Default is 0
		void setMinimumDistance(float fMinimumDistance);

		// Gets the minimum distance a particle may be emitted from the emitter's position.
		// Default is 0
		float getMinimumDistance(void) const;

		// Sets the maximum distance a particle may be emitted from the emitter's position.
		// Default is 50
		void setMaximumDistance(float fMaximumDistance);

		// Gets the maximum distance a particle may be emitted from the emitter's position.
		// Default is 50
		float getMaximumDistance(void) const;

		// Sets the name and pointer to the 2DParticleType stored in the 2DParticleSystem which this emitter gives to new particles.
		// If the name of the type doesn't exist, an exception occurs
		// Default is "default"
		void setParticleType(const std::string& strParticleTypeName);

		// Returns the name of the 2DParticleType stored in the 2DParticleSystem which this emitter gives to new particles.
		// Default is "default"
		std::string getParticleType(void) const;

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