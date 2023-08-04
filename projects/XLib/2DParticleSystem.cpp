#include "PCH.h"
#include "2DParticleSystem.h"
#include "log.h"

#include "resourceManager.h"

namespace X
{
	C2DParticleSystem::C2DParticleSystem()
	{

	}

	void C2DParticleSystem::render(void)
	{
		// Update everything

		// If there's no particles, return.
		if (!_mvParticles.size())
			return;
	}

	void C2DParticleSystem::removeAll(void)
	{
		_mvParticles.clear();
		_mvIndiciesToDeadParticles.clear();
		removeAllAffectors();
		removeAllEmitters();
		removeAllParticleTypes();
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
		C2DParticleEmitter* pNew = new C2DParticleEmitter;
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