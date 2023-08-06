#include "PCH.h"
#include "2DLayer.h"
#include "log.h"
#include "resourceManager.h"

namespace X
{
	C2DLayer::C2DLayer()
	{
		_mbVisible = true;
	}

	C2DLayer::~C2DLayer()
	{
		removeAllEntities();
		removeAllEntityRots();
		removeAllParticleSystems();
	}

	void C2DLayer::setVisible(bool bVisible)
	{
		_mbVisible = bVisible;
	}

	bool C2DLayer::getVisible(void) const
	{
		return _mbVisible;
	}

	C2DEntity* C2DLayer::addEntity(const std::string& strUniqueName, const std::string& strResourceTexture2DAtlasName)
	{
		// Attempt to find if the object name already exists
		std::map<std::string, C2DEntity*>::iterator it = _mmapEntities.find(strUniqueName);
		ThrowIfTrue(it != _mmapEntities.end(), "C2DLayer::addEntity(\"" + strUniqueName + "\") failed. The object already exists.");

		// Allocate memory for new object
		C2DEntity* pNew = new C2DEntity(strResourceTexture2DAtlasName);
		ThrowIfFalse(pNew, "C2DLayer::addEntity(\"" + strUniqueName + "\") failed. Unable to allocate memory.");

		// Make sure the named atlas resource exists, otherwise throw an exception
		SCResourceManager* pRM = SCResourceManager::getPointer();
		ThrowIfFalse(pRM->getTexture2DAtlasExists(strResourceTexture2DAtlasName), "C2DLayer::addEntity(\"" + strUniqueName + ", " + strResourceTexture2DAtlasName + "\") failed. named atlas resource doesn't exist.");
		
		// Add object to hash map
		_mmapEntities[strUniqueName] = pNew;
		return pNew;
	}

	bool C2DLayer::getEntityExists(const std::string& strUniqueName) const
	{
		if (_mmapEntities.find(strUniqueName) == _mmapEntities.end())
			return false;
		return true;
	}

	C2DEntity* C2DLayer::getEntity(const std::string& strUniqueName) const
	{
		// Attempt to find if the name already exists
		std::map<std::string, C2DEntity*>::iterator it = _mmapEntities.find(strUniqueName);
		ThrowIfTrue(it == _mmapEntities.end(), "C2DLayer::getEntity(\"" + strUniqueName + "\") failed. Object name doesn't exist.");
		return it->second;
	}

	C2DEntity* C2DLayer::getEntity(unsigned int uiIndex) const
	{
		// Make sure given index is valid
		ThrowIfTrue(uiIndex >= _mmapEntities.size(), "C2DLayer::getEntity(" + std::to_string(uiIndex) + ") failed. Invalid index given.");
		std::map<std::string, C2DEntity*>::iterator it = _mmapEntities.begin();
		unsigned int ui = 0;
		while (ui < uiIndex)
		{
			ui++;
			it++;
		}
		return it->second;
	}

	void C2DLayer::removeEntity(const std::string& strUniqueName)
	{
		// Attempt to find if the layer name already exists
		std::map<std::string, C2DEntity*>::iterator it = _mmapEntities.find(strUniqueName);
		ThrowIfTrue(it == _mmapEntities.end(), "C2DLayer::removeEntity(\"" + strUniqueName + "\") failed. The object doesn't exist.");

		// De-allocate memory
		delete it->second;

		// Remove from hash map
		_mmapEntities.erase(it);
	}

	void C2DLayer::removeEntity(unsigned int uiIndex)
	{
		// Make sure given index is valid
		ThrowIfTrue(uiIndex >= _mmapEntities.size(), "C2DLayer::removeEntity(" + std::to_string(uiIndex) + ") failed. Invalid index given.");
		std::map<std::string, C2DEntity*>::iterator it = _mmapEntities.begin();
		unsigned int ui = 0;
		while (ui < uiIndex)
		{
			ui++;
			it++;
		}
		// De-allocate memory for the object
		delete it->second;

		// Remove object from hash map
		_mmapEntities.erase(it);
	}

	void C2DLayer::removeAllEntities(void)
	{
		// Remove all objects
		std::map<std::string, C2DEntity*>::iterator it = _mmapEntities.begin();
		while (it != _mmapEntities.end())
		{
			delete it->second;
			_mmapEntities.erase(it);
			it = _mmapEntities.begin();
		}
		_mmapEntities.clear();
	}

	unsigned int C2DLayer::getNumEntities(void) const
	{
		return int(_mmapEntities.size());
	}

	C2DEntityRot* C2DLayer::addEntityRot(const std::string& strUniqueName, const std::string& strResourceTexture2DAtlasName)
	{
		// Attempt to find if the object name already exists
		std::map<std::string, C2DEntityRot*>::iterator it = _mmapEntityRots.find(strUniqueName);
		ThrowIfTrue(it != _mmapEntityRots.end(), "C2DLayer::addEntityRot(\"" + strUniqueName + "\") failed. The object already exists.");

		// Allocate memory for new object
		C2DEntityRot* pNew = new C2DEntityRot(strResourceTexture2DAtlasName);
		ThrowIfFalse(pNew, "C2DLayer::addEntityRot(\"" + strUniqueName + "\") failed. Unable to allocate memory.");

		// Make sure the named atlas resource exists, otherwise throw an exception
		SCResourceManager* pRM = SCResourceManager::getPointer();
		ThrowIfFalse(pRM->getTexture2DAtlasExists(strResourceTexture2DAtlasName), "C2DLayer::addEntityRot(\"" + strUniqueName + ", " + strResourceTexture2DAtlasName + "\") failed. named atlas resource doesn't exist.");

		// Add object to hash map
		_mmapEntityRots[strUniqueName] = pNew;
		return pNew;
	}

	bool C2DLayer::getEntityRotExists(const std::string& strUniqueName) const
	{
		if (_mmapEntityRots.find(strUniqueName) == _mmapEntityRots.end())
			return false;
		return true;
	}

	C2DEntityRot* C2DLayer::getEntityRot(const std::string& strUniqueName) const
	{
		// Attempt to find if the name already exists
		std::map<std::string, C2DEntityRot*>::iterator it = _mmapEntityRots.find(strUniqueName);
		ThrowIfTrue(it == _mmapEntityRots.end(), "C2DLayer::getEntityRot(\"" + strUniqueName + "\") failed. Object name doesn't exist.");
		return it->second;
	}

	C2DEntityRot* C2DLayer::getEntityRot(unsigned int uiIndex) const
	{
		// Make sure given index is valid
		ThrowIfTrue(uiIndex >= _mmapEntityRots.size(), "C2DLayer::getEntityRot(" + std::to_string(uiIndex) + ") failed. Invalid index given.");
		std::map<std::string, C2DEntityRot*>::iterator it = _mmapEntityRots.begin();
		unsigned int ui = 0;
		while (ui < uiIndex)
		{
			ui++;
			it++;
		}
		return it->second;
	}

	void C2DLayer::removeEntityRot(const std::string& strUniqueName)
	{
		// Attempt to find if the layer name already exists
		std::map<std::string, C2DEntityRot*>::iterator it = _mmapEntityRots.find(strUniqueName);
		ThrowIfTrue(it == _mmapEntityRots.end(), "C2DLayer::removeEntityRot(\"" + strUniqueName + "\") failed. The object doesn't exist.");

		// De-allocate memory
		delete it->second;

		// Remove from hash map
		_mmapEntityRots.erase(it);
	}

	void C2DLayer::removeEntityRot(unsigned int uiIndex)
	{
		// Make sure given index is valid
		ThrowIfTrue(uiIndex >= _mmapEntityRots.size(), "C2DLayer::removeEntityRot(" + std::to_string(uiIndex) + ") failed. Invalid index given.");
		std::map<std::string, C2DEntityRot*>::iterator it = _mmapEntityRots.begin();
		unsigned int ui = 0;
		while (ui < uiIndex)
		{
			ui++;
			it++;
		}
		// De-allocate memory for the object
		delete it->second;

		// Remove object from hash map
		_mmapEntityRots.erase(it);
	}

	void C2DLayer::removeAllEntityRots(void)
	{
		// Remove all objects
		std::map<std::string, C2DEntityRot*>::iterator it = _mmapEntityRots.begin();
		while (it != _mmapEntityRots.end())
		{
			delete it->second;
			_mmapEntityRots.erase(it);
			it = _mmapEntityRots.begin();
		}
		_mmapEntityRots.clear();
	}

	unsigned int C2DLayer::getNumEntityRots(void) const
	{
		return int(_mmapEntityRots.size());
	}

	C2DParticleSystem* C2DLayer::addParticleSystem(const std::string& strUniqueName)
	{
		// Attempt to find if the object name already exists
		std::map<std::string, C2DParticleSystem*>::iterator it = _mmapParticleSystems.find(strUniqueName);
		ThrowIfTrue(it != _mmapParticleSystems.end(), "C2DLayer::addParticleSystem(\"" + strUniqueName + "\") failed. The object already exists.");

		// Allocate memory for new object
		C2DParticleSystem* pNew = new C2DParticleSystem();
		ThrowIfFalse(pNew, "C2DLayer::addParticleSystem(\"" + strUniqueName + "\") failed. Unable to allocate memory.");

		// Add object to hash map
		_mmapParticleSystems[strUniqueName] = pNew;
		return pNew;
	}

	bool C2DLayer::getParticleSystemExists(const std::string& strUniqueName) const
	{
		if (_mmapParticleSystems.find(strUniqueName) == _mmapParticleSystems.end())
			return false;
		return true;
	}

	C2DParticleSystem* C2DLayer::getParticleSystem(const std::string& strUniqueName) const
	{
		// Attempt to find if the name already exists
		std::map<std::string, C2DParticleSystem*>::iterator it = _mmapParticleSystems.find(strUniqueName);
		ThrowIfTrue(it == _mmapParticleSystems.end(), "C2DLayer::getParticleSystem(\"" + strUniqueName + "\") failed. Object name doesn't exist.");
		return it->second;
	}

	C2DParticleSystem* C2DLayer::getParticleSystem(unsigned int uiIndex) const
	{
		// Make sure given index is valid
		ThrowIfTrue(uiIndex >= _mmapParticleSystems.size(), "C2DLayer::getParticleSystem(" + std::to_string(uiIndex) + ") failed. Invalid index given.");
		std::map<std::string, C2DParticleSystem*>::iterator it = _mmapParticleSystems.begin();
		unsigned int ui = 0;
		while (ui < uiIndex)
		{
			ui++;
			it++;
		}
		return it->second;
	}

	void C2DLayer::removeParticleSystem(const std::string& strUniqueName)
	{
		// Attempt to find if the layer name already exists
		std::map<std::string, C2DParticleSystem*>::iterator it = _mmapParticleSystems.find(strUniqueName);
		ThrowIfTrue(it == _mmapParticleSystems.end(), "C2DLayer::removeParticleSystem(\"" + strUniqueName + "\") failed. The object doesn't exist.");

		// De-allocate memory
		delete it->second;

		// Remove from hash map
		_mmapParticleSystems.erase(it);
	}

	void C2DLayer::removeParticleSystem(unsigned int uiIndex)
	{
		// Make sure given index is valid
		ThrowIfTrue(uiIndex >= _mmapParticleSystems.size(), "C2DLayer::removeParticleSystem(" + std::to_string(uiIndex) + ") failed. Invalid index given.");
		std::map<std::string, C2DParticleSystem*>::iterator it = _mmapParticleSystems.begin();
		unsigned int ui = 0;
		while (ui < uiIndex)
		{
			ui++;
			it++;
		}
		// De-allocate memory for the object
		delete it->second;

		// Remove object from hash map
		_mmapParticleSystems.erase(it);
	}

	void C2DLayer::removeAllParticleSystems(void)
	{
		// Remove all objects
		std::map<std::string, C2DParticleSystem*>::iterator it = _mmapParticleSystems.begin();
		while (it != _mmapParticleSystems.end())
		{
			delete it->second;
			_mmapParticleSystems.erase(it);
			it = _mmapParticleSystems.begin();
		}
		_mmapParticleSystems.clear();
	}

	unsigned int C2DLayer::getNumParticleSystems(void) const
	{
		return int(_mmapParticleSystems.size());
	}
}