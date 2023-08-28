#include "PCH.h"
#include "2DParticleSystem.h"
#include "singletons.h"

namespace X
{
	C2DParticleSystem::C2DParticleSystem()
	{
		// Create default particle type
		C2DParticleType* pType = addParticleType("default");
		//pType->	// Just use defaults set in constructor

		// Create gravity affector
		C2DParticleAffector* pAffector = addAffector("gravity");
		//pAffector->	// Just use defaults set in constructor

		// Create emitter and disable it
//		C2DParticleEmitter* pEmitter = addEmitter("default");
//		pEmitter->

		_mbPaused = false;

		_mSettings.strTextureAtlasName = "X:default_particle";
	}

	void C2DParticleSystem::render(const CMatrix& matrixView, const CMatrix& matrixProjection)
	{
		_mTimer.update();
		float fTimeDeltaSeconds = _mTimer.getSecondsPast();
		if (_mbPaused)
			fTimeDeltaSeconds = 0.0f;

		// Update everything

		// For each particle emitter, emit new particles...
		std::map<std::string, C2DParticleEmitter*>::iterator itEmitter = _mmapEmitters.begin();
		while (itEmitter != _mmapEmitters.end())
		{
			itEmitter->second->_update(fTimeDeltaSeconds);
			itEmitter++;
		}

		// If there's no particles, return.
		if (!_mvParticles.size())
			return;

		// Update all the particles
		CVector2f vForce;
		for (int iParticle = 0; iParticle < _mvParticles.size(); iParticle++)
		{
			// If particle is dead
			if (_mvParticles[iParticle].fAge > 1.0f)
				continue;

			// Update particle's age
			_mvParticles[iParticle].fAge += fTimeDeltaSeconds * _mvParticles[iParticle].fAgingRate;

			// If it is now dead, add to dead list
			if (_mvParticles[iParticle].fAge > 1.0f)
			{
				_mvIndiciesToDeadParticles.push_back(iParticle);
				continue;
			}

			// Update particle's mass based upon it's age
			_mvParticles[iParticle].fMass = interpolate(_mvParticles[iParticle].fMassBirth, _mvParticles[iParticle].fMassDeath, _mvParticles[iParticle].fAge);

			// Update particle's velocity
			std::map<std::string, C2DParticleAffector*>::iterator itAffector = _mmapAffectors.begin();
			while (itAffector != _mmapAffectors.end())
			{
				// This affector is directional
				if (C2DParticleAffector::Type::directional == itAffector->second->_mType)
				{
					vForce.x = itAffector->second->_mvForce.x / _mvParticles[iParticle].fMass;
					vForce.x *= fTimeDeltaSeconds;
					vForce.y = itAffector->second->_mvForce.y / _mvParticles[iParticle].fMass;
					vForce.y *= fTimeDeltaSeconds;
					_mvParticles[iParticle].vVelocity += vForce;
				}
				itAffector++;
			}

			// Update particle's position
			_mvParticles[iParticle].vPosition.x += _mvParticles[iParticle].vVelocity.x * fTimeDeltaSeconds;
			_mvParticles[iParticle].vPosition.y -= _mvParticles[iParticle].vVelocity.y * fTimeDeltaSeconds;

			// Update particle's radius based upon it's age
			_mvParticles[iParticle].fRadius = interpolate(_mvParticles[iParticle].fRadiusAtBirth, _mvParticles[iParticle].fRadiusAtDeath, _mvParticles[iParticle].fAge);
		}
		// Render all the particles

		// Get resources needed to render
		CResourceTexture2DAtlas* pAtlas = x->pResource->getTexture2DAtlas(_mSettings.strTextureAtlasName);
		CResourceVertexBufferCPT2* pVB = x->pResource->getVertexBufferCPT2("X:default");
		CResourceShader* pShader = x->pResource->getShader("X:VBCPT2");

		CMatrix matrixWorld;
		// Bind shader and set some uniforms
		pShader->bind();
		pShader->setInt("texture0", 0);
		pShader->setInt("texture1", 1);
		pShader->setMat4("matrixWorld", matrixWorld);
		pShader->setMat4("matrixView", matrixView);
		pShader->setMat4("matrixProjection", matrixProjection);

		// Bind texture atlas to texture units
		pAtlas->bindAtlas(0, 0);
		pAtlas->bindAtlas(1, 0);

		

		glDisable(GL_DEPTH_TEST);

		CResourceVertexBufferCPT2::Vertex vBL, vBR, vTR, vTL;
		CVector2f vDims;
		CColour colour;
		CVector2f vTexCoordsA;
		CVector2f vTexCoordsB;
		
		for (int iParticle = 0; iParticle < _mvParticles.size(); iParticle++)
		{
			// Clear vertex buffer
			pVB->removeGeom();

			// Get image details for this particle at birth and death
			CImageAtlasDetails imageDetailsBirth = pAtlas->getImageDetails(_mvParticles[iParticle].pType->stageBirth.strTextureAtlasImageName);
			CImageAtlasDetails imageDetailsDeath = pAtlas->getImageDetails(_mvParticles[iParticle].pType->stageDeath.strTextureAtlasImageName);
			
			// Set texture coordinates within texture atlas for both sets
			vBL.texCoord = imageDetailsBirth.sTexCoords.vBL;
			vBR.texCoord = imageDetailsBirth.sTexCoords.vBR;
			vTL.texCoord = imageDetailsBirth.sTexCoords.vTL;
			vTR.texCoord = imageDetailsBirth.sTexCoords.vTR;
			vBL.texCoord2 = imageDetailsDeath.sTexCoords.vBL;
			vBR.texCoord2 = imageDetailsDeath.sTexCoords.vBR;
			vTL.texCoord2 = imageDetailsDeath.sTexCoords.vTL;
			vTR.texCoord2 = imageDetailsDeath.sTexCoords.vTR;

			// Set amount of each texture to use for the shader
			pShader->setFloat("fTextureContribution", _mvParticles[iParticle].fAge);

			// Compute colour based upon age
			colour = _mvParticles[iParticle].colourAtBirth.interpolate(_mvParticles[iParticle].colourAtDeath, _mvParticles[iParticle].fAge);

			// Compute position
			vBL.position.x = _mvParticles[iParticle].vPosition.x - _mvParticles[iParticle].fRadius;
			vBL.position.y = _mvParticles[iParticle].vPosition.y - _mvParticles[iParticle].fRadius;
			vBR.position.x = _mvParticles[iParticle].vPosition.x + _mvParticles[iParticle].fRadius;
			vBR.position.y = _mvParticles[iParticle].vPosition.y - _mvParticles[iParticle].fRadius;
			vTR.position.x = _mvParticles[iParticle].vPosition.x + _mvParticles[iParticle].fRadius;
			vTR.position.y = _mvParticles[iParticle].vPosition.y + _mvParticles[iParticle].fRadius;
			vTL.position.x = _mvParticles[iParticle].vPosition.x - _mvParticles[iParticle].fRadius;
			vTL.position.y = _mvParticles[iParticle].vPosition.y + _mvParticles[iParticle].fRadius;

			// Add the geometry
			vDims.x = _mvParticles[iParticle].fRadius * 2.0f;
			vDims.y = vDims.x;
			pVB->addQuad2D(
				_mvParticles[iParticle].vPosition,
				vDims,
				colour,
				vBL.texCoord, vBR.texCoord, vTR.texCoord, vTL.texCoord,
				vBL.texCoord2, vBR.texCoord2, vTR.texCoord2, vTL.texCoord2);

			pVB->update();
			pVB->render();
		}
		

		pAtlas->unbindAll();
		pShader->unbind();
	}

	void C2DParticleSystem::removeAll(void)
	{
		_mvParticles.clear();
		_mvIndiciesToDeadParticles.clear();
		removeAllAffectors();
		removeAllEmitters();
		removeAllParticleTypes();
	}

	void C2DParticleSystem::setTextureAtlas(const std::string strName)
	{
		_mSettings.strTextureAtlasName = strName;
	}

	C2DParticleAffector* C2DParticleSystem::addAffector(const std::string& strName)
	{
		// Attempt to find by name
		std::map<std::string, C2DParticleAffector*>::iterator it = _mmapAffectors.find(strName);
		// If it already exists, return a pointer to that.
		if (it != _mmapAffectors.end())
			return it->second;

		// Allocate the new object
		C2DParticleAffector* pNew = new C2DParticleAffector;
		ThrowIfFalse(pNew, "C2DParticleSystem::addAffector(" + strName + ") failed. Unable to allocate memory for the new affector.");

		// Place in the hashmap
		_mmapAffectors[strName] = pNew;
		return pNew;
	}

	C2DParticleAffector* C2DParticleSystem::getAffector(const std::string& strName) const
	{
		std::map<std::string, C2DParticleAffector*>::iterator it = _mmapAffectors.find(strName);
		ThrowIfTrue(it == _mmapAffectors.end(), "C2DParticleSystem::getAffector(" + strName + ") failed. Affector doesn't exist.");
		return it->second;
	}

	bool C2DParticleSystem::getAffectorExists(const std::string& strName) const
	{
		std::map<std::string, C2DParticleAffector*>::iterator it = _mmapAffectors.find(strName);
		return it != _mmapAffectors.end();
	}

	void C2DParticleSystem::removeAffector(const std::string& strName)
	{
		std::map<std::string, C2DParticleAffector*>::iterator it = _mmapAffectors.find(strName);
		if (it == _mmapAffectors.end())
			return;	// Doesn't exist.
		delete it->second;
		_mmapAffectors.erase(it);
	}

	void C2DParticleSystem::removeAllAffectors(void)
	{
		std::map<std::string, C2DParticleAffector*>::iterator it = _mmapAffectors.begin();
		while (it != _mmapAffectors.end())
		{
			delete it->second;
			it++;
		}
		_mmapAffectors.clear();
	}

	int C2DParticleSystem::getNumberOfAffectors(void) const
	{
		return (int)_mmapAffectors.size();
	}

	std::string C2DParticleSystem::getAffectorName(int iIndex) const
	{
		ThrowIfTrue(iIndex < 0 || iIndex >= (int)_mmapAffectors.size(), "C2DParticleSystem::getAffectorName() failed. Invalid index of " + std::to_string(iIndex) + " was given.");
		std::map<std::string, C2DParticleAffector*>::iterator it = _mmapAffectors.begin();
		int iCount = 0;
		while (iCount < iIndex)
		{
			it++;
			iCount++;
		}
		return it->first;
	}

	C2DParticleEmitter* C2DParticleSystem::addEmitter(const std::string& strName)
	{
		// Attempt to find by name
		std::map<std::string, C2DParticleEmitter*>::iterator it = _mmapEmitters.find(strName);
		// If it already exists, return a pointer to that.
		if (it != _mmapEmitters.end())
			return it->second;

		// Allocate the new object
		C2DParticleEmitter* pNew = new C2DParticleEmitter(this);
		ThrowIfFalse(pNew, "C2DParticleSystem::addEmitter(" + strName + ") failed. Unable to allocate memory for the new emitter.");

		// Place in the hashmap
		_mmapEmitters[strName] = pNew;
		return pNew;
	}

	C2DParticleEmitter* C2DParticleSystem::getEmitter(const std::string& strName) const
	{
		std::map<std::string, C2DParticleEmitter*>::iterator it = _mmapEmitters.find(strName);
		ThrowIfTrue(it == _mmapEmitters.end(), "C2DParticleSystem::getEmitter(" + strName + ") failed. Emitter doesn't exist.");
		return it->second;
	}

	bool C2DParticleSystem::getEmitterExists(const std::string& strName) const
	{
		std::map<std::string, C2DParticleEmitter*>::iterator it = _mmapEmitters.find(strName);
		return it != _mmapEmitters.end();
	}

	void C2DParticleSystem::removeEmitter(const std::string& strName)
	{
		std::map<std::string, C2DParticleEmitter*>::iterator it = _mmapEmitters.find(strName);
		if (it == _mmapEmitters.end())
			return;	// Doesn't exist.
		delete it->second;
		_mmapEmitters.erase(it);
	}

	void C2DParticleSystem::removeAllEmitters(void)
	{
		std::map<std::string, C2DParticleEmitter*>::iterator it = _mmapEmitters.begin();
		while (it != _mmapEmitters.end())
		{
			delete it->second;
			it++;
		}
		_mmapEmitters.clear();
	}

	int C2DParticleSystem::getNumberOfEmitters(void) const
	{
		return (int)_mmapEmitters.size();
	}

	std::string C2DParticleSystem::getEmitterName(int iIndex) const
	{
		ThrowIfTrue(iIndex < 0 || iIndex >= (int)_mmapEmitters.size(), "C2DParticleSystem::getEmitterName() failed. Invalid index of " + std::to_string(iIndex) + " was given.");
		std::map<std::string, C2DParticleEmitter*>::iterator it = _mmapEmitters.begin();
		int iCount = 0;
		while (iCount < iIndex)
		{
			it++;
			iCount++;
		}
		return it->first;
	}

	C2DParticleType* C2DParticleSystem::addParticleType(const std::string& strName)
	{
		// Attempt to find by name
		std::map<std::string, C2DParticleType*>::iterator it = _mmapTypes.find(strName);
		// If it already exists, return a pointer to that.
		if (it != _mmapTypes.end())
			return it->second;

		// Allocate the new object
		C2DParticleType* pNew = new C2DParticleType;
		ThrowIfFalse(pNew, "C2DParticleSystem::addParticleType(" + strName + ") failed. Unable to allocate memory for the new particle type.");

		// Place in the hashmap
		_mmapTypes[strName] = pNew;
		return pNew;
	}

	C2DParticleType* C2DParticleSystem::getParticleType(const std::string& strName) const
	{
		std::map<std::string, C2DParticleType*>::iterator it = _mmapTypes.find(strName);
		ThrowIfTrue(it == _mmapTypes.end(), "C2DParticleSystem::getParticleType(" + strName + ") failed. Particle type doesn't exist.");
		return it->second;
	}

	bool C2DParticleSystem::getParticleTypeExists(const std::string& strName) const
	{
		std::map<std::string, C2DParticleType*>::iterator it = _mmapTypes.find(strName);
		return it != _mmapTypes.end();
	}

	void C2DParticleSystem::removeParticleType(const std::string& strName)
	{
		std::map<std::string, C2DParticleType*>::iterator it = _mmapTypes.find(strName);
		if (it == _mmapTypes.end())
			return;	// Doesn't exist.
		delete it->second;
		_mmapTypes.erase(it);
	}

	void C2DParticleSystem::removeAllParticleTypes(void)
	{
		std::map<std::string, C2DParticleType*>::iterator it = _mmapTypes.begin();
		while (it != _mmapTypes.end())
		{
			delete it->second;
			it++;
		}
		_mmapTypes.clear();
	}

	int C2DParticleSystem::getNumberOfParticleTypes(void) const
	{
		return (int)_mmapTypes.size();
	}

	std::string C2DParticleSystem::getParticleTypeName(int iIndex) const
	{
		ThrowIfTrue(iIndex < 0 || iIndex >= (int)_mmapTypes.size(), "C2DParticleSystem::getParticleTypeName() failed. Invalid index of " + std::to_string(iIndex) + " was given.");
		std::map<std::string, C2DParticleType*>::iterator it = _mmapTypes.begin();
		int iCount = 0;
		while (iCount < iIndex)
		{
			it++;
			iCount++;
		}
		return it->first;
	}
}