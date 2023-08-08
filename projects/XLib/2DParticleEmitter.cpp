#include "PCH.h"
#include "2DParticleEmitter.h"
#include "2DParticleSystem.h"
#include "utilities.h"
#include "log.h"

namespace X
{
	C2DParticleEmitter::C2DParticleEmitter(C2DParticleSystem *pSystem)
	{
		_mpPS = pSystem;
		_mvPosition.set(0.0f, 0.0f);
		_mfBirthRatePerSecond = 50.0f;
		_mfMinRangeFromPos = 0.0f;
		_mfMaxRangeFromPos = 50.0f;
		_mstrParticleTypeName = "default";
		_mpParticleType = 0;
		_mbEmitting = true;
		_mvMinVelocity.set(-50.0f, -50.0f);
		_mvMaxVelocity.set( 50.0f, 50.0f);
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

				// fAgingRate
				_mpPS->_mvParticles[iIndexToNewParticle].fAgingRate = 1.0f / randf(_mpParticleType->fLifeSpanMinSeconds, _mpParticleType->fLifeSpanMaxSeconds);

				// Mass
				_mpPS->_mvParticles[iIndexToNewParticle].fMassBirth = randf(_mpParticleType->stageBirth.fMassMin, _mpParticleType->stageBirth.fMassMax);
				_mpPS->_mvParticles[iIndexToNewParticle].fMassDeath = randf(_mpParticleType->stageDeath.fMassMin, _mpParticleType->stageDeath.fMassMax);

				// Colour
				_mpPS->_mvParticles[iIndexToNewParticle].colourAtBirth = _mpParticleType->stageBirth.colourMin.interpolate(_mpParticleType->stageBirth.colourMax, randf(0.0f, 1.0f));
				_mpPS->_mvParticles[iIndexToNewParticle].colourAtDeath = _mpParticleType->stageDeath.colourMin.interpolate(_mpParticleType->stageDeath.colourMax, randf(0.0f, 1.0f));
				
				// Radius
				_mpPS->_mvParticles[iIndexToNewParticle].fRadiusAtBirth = interpolate(_mpParticleType->stageBirth.fRadiusMin, _mpParticleType->stageBirth.fRadiusMax, randf(0.0f, 1.0f));
				_mpPS->_mvParticles[iIndexToNewParticle].fRadiusAtDeath = interpolate(_mpParticleType->stageDeath.fRadiusMin, _mpParticleType->stageDeath.fRadiusMax, randf(0.0f, 1.0f));

				// Pointer to the ParticleType stored in C2DParticleSystem
				_mpPS->_mvParticles[iIndexToNewParticle].pType = _mpParticleType;

				// Position
				_mpPS->_mvParticles[iIndexToNewParticle].vPosition = _mvPosition;
				_mpPS->_mvParticles[iIndexToNewParticle].vPosition.x += randf(_mfMinRangeFromPos, _mfMaxRangeFromPos);
				_mpPS->_mvParticles[iIndexToNewParticle].vPosition.y += randf(_mfMinRangeFromPos, _mfMaxRangeFromPos);

				// Velocity
				_mpPS->_mvParticles[iIndexToNewParticle].vVelocity.x = randf(_mvMinVelocity.x, _mvMaxVelocity.x);
				_mpPS->_mvParticles[iIndexToNewParticle].vVelocity.y = randf(_mvMinVelocity.y, _mvMaxVelocity.y);

			}
		}	// _mbEmitting?
	}

	void C2DParticleEmitter::setPosition(const CVector2f& pNewPosition)
	{
		_mvPosition = pNewPosition;
	}

	CVector2f C2DParticleEmitter::getPosition(void) const
	{
		return _mvPosition;
	}

	void C2DParticleEmitter::setMinimumVelocity(const CVector2f& vMinimumVelocity)
	{
		_mvMinVelocity = vMinimumVelocity;
	}

	CVector2f C2DParticleEmitter::getMinimumVelocity(void) const
	{
		return _mvMinVelocity;
	}

	void C2DParticleEmitter::setMaximumVelocity(const CVector2f& vMaximumVelocity)
	{
		_mvMaxVelocity = vMaximumVelocity;
	}

	CVector2f C2DParticleEmitter::getMaximumVelocity(void) const
	{
		return _mvMaxVelocity;
	}

	void C2DParticleEmitter::setEmitting(bool bEmittingParticles)
	{
		_mbEmitting = bEmittingParticles;
	}

	bool C2DParticleEmitter::getEmitting(void) const
	{
		return _mbEmitting;
	}

	void C2DParticleEmitter::setBirthRate(float fNumberOfParticlesPerSecond)
	{
		_mfBirthRatePerSecond = fNumberOfParticlesPerSecond;
	}

	float C2DParticleEmitter::getBirthRate(void) const
	{
		return _mfBirthRatePerSecond;
	}

	void C2DParticleEmitter::setMinimumDistance(float fMinimumDistance)
	{
		_mfMinRangeFromPos = fMinimumDistance;
	}

	float C2DParticleEmitter::getMinimumDistance(void) const
	{
		return _mfMinRangeFromPos;
	}

	void C2DParticleEmitter::setMaximumDistance(float fMaximumDistance)
	{
		_mfMaxRangeFromPos = fMaximumDistance;
	}

	float C2DParticleEmitter::getMaximumDistance(void) const
	{
		return _mfMaxRangeFromPos;
	}

	void C2DParticleEmitter::setParticleType(const std::string& strParticleTypeName)
	{
		ThrowIfFalse(_mpPS->getParticleTypeExists(strParticleTypeName), "C2DParticleEmitter::setParticleType() failed with name of: " + strParticleTypeName + " The type doesn't exist.");
		_mstrParticleTypeName = strParticleTypeName;
		_mpParticleType = _mpPS->getParticleType(strParticleTypeName);
	}

	std::string C2DParticleEmitter::getParticleType(void) const
	{
		return _mstrParticleTypeName;
	}
}