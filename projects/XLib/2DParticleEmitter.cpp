#include "PCH.h"
#include "2DParticleEmitter.h"
#include "2DParticleSystem.h"
#include "utilities.h"

namespace X
{
	C2DParticleEmitter::C2DParticleEmitter(C2DParticleSystem *pSystem)
	{
		_mpPS = pSystem;
		_mvPosition.set(0.0f, 0.0f);
		_mfBirthRatePerSecond = 100.0f;
		_mfMinRangeFromPos = 0.0f;
		_mfMaxRangeFromPos = 50.0f;
		_mstrParticleTypeName = "default";
		_mpParticleType = 0;
		_mbEmitting = true;
		_mvMinVelocity.set(1.0f, 1.0f);
		_mvMaxVelocity.set(100.0f, 100.0f);
		_mfTimeUntilNextEmission = 0.0f;
	}

	void C2DParticleEmitter::_update(float fTimeDeltaSeconds)
	{
		// Get particle type pointer used by this emitter
		if (!_mpParticleType)
		{
			_mpParticleType = _mpPS->getParticleType(_mstrParticleTypeName);
		}

		// Emit new particles
		if (_mbEmitting)
		{
			_mfTimeUntilNextEmission -= fTimeDeltaSeconds;
			while (_mfTimeUntilNextEmission < 0.0f)
			{
				_mfTimeUntilNextEmission += 1.0f / _mfBirthRatePerSecond;

				int iIndexToNewParticle;

				// Any dead particles we can re-use?
				if (_mpPS->_mvIndiciesToDeadParticles.size())
				{
					// Get index into particle vector we can re-use
					iIndexToNewParticle = _mpPS->_mvIndiciesToDeadParticles[_mpPS->_mvIndiciesToDeadParticles.size() - 1];

					// Remove particle index of dead particle
					_mpPS->_mvIndiciesToDeadParticles.pop_back();
				}
				else // No dead particles, create a new one
				{
					iIndexToNewParticle = (int)_mpPS->_mvParticles.size();
					_mpPS->_mvParticles.push_back(C2DParticle());
				}

				// Set particle's initial values

				// Newborn, awww!
				_mpPS->_mvParticles[iIndexToNewParticle].fAge = 0.0f;

				// Mass
				_mpPS->_mvParticles[iIndexToNewParticle].fMass = randf(_mpParticleType->stageBirth.fMassMin, _mpParticleType->stageBirth.fMassMax);

				// Pointer to the ParticleType stored in C2DParticleSystem
				_mpPS->_mvParticles[iIndexToNewParticle].pType = _mpParticleType;

				// Position
				_mpPS->_mvParticles[iIndexToNewParticle].vPosition = _mvPosition;
				_mpPS->_mvParticles[iIndexToNewParticle].vPosition.x += randf(_mfMinRangeFromPos, _mfMaxRangeFromPos);
				_mpPS->_mvParticles[iIndexToNewParticle].vPosition.y += randf(_mfMinRangeFromPos, _mfMaxRangeFromPos);
				// Velocity
				_mpPS->_mvParticles[iIndexToNewParticle].vVelocity.x = randf(_mvMinVelocity.x, _mvMaxVelocity.x);
				_mpPS->_mvParticles[iIndexToNewParticle].vVelocity.y = randf(_mvMinVelocity.y, _mvMaxVelocity.y);
				// Mass
				_mpPS->_mvParticles[iIndexToNewParticle].fMassBirth = randf(_mpParticleType->stageBirth.fMassMin, _mpParticleType->stageBirth.fMassMax);
				_mpPS->_mvParticles[iIndexToNewParticle].fMassDeath = randf(_mpParticleType->stageDeath.fMassMin, _mpParticleType->stageDeath.fMassMax);
				// Colour
				_mpPS->_mvParticles[iIndexToNewParticle].colourAtBirth = _mpParticleType->stageBirth.colourMin.interpolate(_mpParticleType->stageBirth.colourMax, randf(0.0f, 1.0f));
				_mpPS->_mvParticles[iIndexToNewParticle].colourAtDeath = _mpParticleType->stageDeath.colourMin.interpolate(_mpParticleType->stageDeath.colourMax, randf(0.0f, 1.0f));
				// Radius
				_mpPS->_mvParticles[iIndexToNewParticle].fRadiusAtBirth = interpolate(_mpParticleType->stageBirth.fRadiusMin, _mpParticleType->stageBirth.fRadiusMax, randf(0.0f, 1.0f));
				_mpPS->_mvParticles[iIndexToNewParticle].fRadiusAtDeath = interpolate(_mpParticleType->stageDeath.fRadiusMin, _mpParticleType->stageDeath.fRadiusMax, randf(0.0f, 1.0f));
			}
		}	// _mbEmitting?
	}
}