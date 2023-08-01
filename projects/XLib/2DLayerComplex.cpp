#include "PCH.h"
#include "2DLayerComplex.h"
#include "log.h"
#include "resourceManager.h"

namespace X
{
	C2DLayerComplex::C2DLayerComplex()
	{
	}

	C2DLayerComplex::~C2DLayerComplex()
	{
		removeAllEntities();
	}

	C2DEntity* C2DLayerComplex::addEntity(const std::string& strUniqueName, const std::string& strResourceTexture2DAtlasName)
	{
		// Attempt to find if the object name already exists
		std::map<std::string, C2DEntity*>::iterator it = _mmapEntities.find(strUniqueName);
		ThrowIfTrue(it != _mmapEntities.end(), "C2DLayerComplex::addEntity(\"" + strUniqueName + "\") failed. The object already exists.");

		// Allocate memory for new object
		C2DEntity* pNew = new C2DEntity(strResourceTexture2DAtlasName);
		ThrowIfFalse(pNew, "C2DLayerComplex::addEntity(\"" + strUniqueName + "\") failed. Unable to allocate memory.");

		// Make sure the named atlas resource exists, otherwise throw an exception
		SCResourceManager* pRM = SCResourceManager::getPointer();
		ThrowIfFalse(pRM->getTexture2DAtlasExists(strResourceTexture2DAtlasName), "C2DLayerComplex::addEntity(\"" + strUniqueName + ", " + strResourceTexture2DAtlasName + "\") failed. named atlas resource doesn't exist.");
		
		// Add object to hash map
		_mmapEntities[strUniqueName] = pNew;
		return pNew;
	}

	bool C2DLayerComplex::getEntityExists(const std::string& strUniqueName) const
	{
		if (_mmapEntities.find(strUniqueName) == _mmapEntities.end())
			return false;
		return true;
	}

	C2DEntity* C2DLayerComplex::getEntity(const std::string& strUniqueName) const
	{
		// Attempt to find if the name already exists
		std::map<std::string, C2DEntity*>::iterator it = _mmapEntities.find(strUniqueName);
		ThrowIfTrue(it == _mmapEntities.end(), "C2DLayerComplex::getEntity(\"" + strUniqueName + "\") failed. Object name doesn't exist.");
		return it->second;
	}

	C2DEntity* C2DLayerComplex::getEntity(unsigned int uiIndex) const
	{
		// Make sure given index is valid
		ThrowIfTrue(uiIndex >= _mmapEntities.size(), "C2DLayerComplex::getEntity(" + std::to_string(uiIndex) + ") failed. Invalid index given.");
		std::map<std::string, C2DEntity*>::iterator it = _mmapEntities.begin();
		unsigned int ui = 0;
		while (ui < uiIndex)
		{
			ui++;
			it++;
		}
		return it->second;
	}

	void C2DLayerComplex::removeEntity(const std::string& strUniqueName)
	{
		// Attempt to find if the layer name already exists
		std::map<std::string, C2DEntity*>::iterator it = _mmapEntities.find(strUniqueName);
		ThrowIfTrue(it == _mmapEntities.end(), "C2DLayerComplex::removeEntity(\"" + strUniqueName + "\") failed. The object doesn't exist.");

		// De-allocate memory
		delete it->second;

		// Remove from hash map
		_mmapEntities.erase(it);
	}

	void C2DLayerComplex::removeEntity(unsigned int uiIndex)
	{
		// Make sure given index is valid
		ThrowIfTrue(uiIndex >= _mmapEntities.size(), "C2DLayerComplex::removeEntity(" + std::to_string(uiIndex) + ") failed. Invalid index given.");
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

	void C2DLayerComplex::removeAllEntities(void)
	{
		// Remove all objects
		std::map<std::string, C2DEntity*>::iterator it = _mmapEntities.begin();
		while (it != _mmapEntities.end())
		{
			delete it->second;
			_mmapEntities.erase(it);
			it = _mmapEntities.begin();
		}
	}

	unsigned int C2DLayerComplex::getNumEntities(void) const
	{
		return int(_mmapEntities.size());
	}
}