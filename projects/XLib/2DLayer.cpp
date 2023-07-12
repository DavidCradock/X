#include "PCH.h"
#include "2DLayer.h"
#include "log.h"
#include "resourceManager.h"

namespace X
{
	C2DLayer::C2DLayer()
	{
	}

	C2DLayer::~C2DLayer()
	{
		removeAllEntitys();
		removeAllEntityComplexs();
	}

	C2DEntity* C2DLayer::addEntity(const std::string& strUniqueName, const std::string& strResourceTexture2DAtlasName)
	{
		// Attempt to find if the object name already exists
		std::map<std::string, C2DEntity*>::iterator it = _mmapEntitys.find(strUniqueName);
		ThrowIfTrue(it != _mmapEntitys.end(), "C2DLayer::addEntity(\"" + strUniqueName + "\") failed. The object already exists.");

		// Allocate memory for new object
		C2DEntity* pNew = new C2DEntity(strResourceTexture2DAtlasName);
		ThrowIfFalse(pNew, "C2DLayer::addEntity(\"" + strUniqueName + "\") failed. Unable to allocate memory.");

		// Make sure the named atlas resource exists, otherwise throw an exception
		SCResourceManager* pRM = SCResourceManager::getPointer();
		ThrowIfFalse(pRM->getTexture2DAtlasExists(strResourceTexture2DAtlasName), "C2DLayer::addEntity(\"" + strUniqueName + ", " + strResourceTexture2DAtlasName + "\") failed. named atlas resource doesn't exist.");
		
		// Add object to hash map
		_mmapEntitys[strUniqueName] = pNew;
		return pNew;
	}

	bool C2DLayer::getEntityExists(const std::string& strUniqueName) const
	{
		if (_mmapEntitys.find(strUniqueName) == _mmapEntitys.end())
			return false;
		return true;
	}

	C2DEntity* C2DLayer::getEntity(const std::string& strUniqueName) const
	{
		// Attempt to find if the name already exists
		std::map<std::string, C2DEntity*>::iterator it = _mmapEntitys.find(strUniqueName);
		ThrowIfTrue(it == _mmapEntitys.end(), "C2DLayer::getEntity(\"" + strUniqueName + "\") failed. Object name doesn't exist.");
		return it->second;
	}

	C2DEntity* C2DLayer::getEntity(unsigned int uiIndex) const
	{
		// Make sure given index is valid
		ThrowIfTrue(uiIndex >= _mmapEntitys.size(), "C2DLayer::getEntity(" + std::to_string(uiIndex) + ") failed. Invalid index given.");
		std::map<std::string, C2DEntity*>::iterator it = _mmapEntitys.begin();
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
		std::map<std::string, C2DEntity*>::iterator it = _mmapEntitys.find(strUniqueName);
		ThrowIfTrue(it == _mmapEntitys.end(), "C2DLayer::removeEntity(\"" + strUniqueName + "\") failed. The object doesn't exist.");

		// De-allocate memory
		delete it->second;

		// Remove from hash map
		_mmapEntitys.erase(it);
	}

	void C2DLayer::removeEntity(unsigned int uiIndex)
	{
		// Make sure given index is valid
		ThrowIfTrue(uiIndex >= _mmapEntitys.size(), "C2DLayer::removeEntity(" + std::to_string(uiIndex) + ") failed. Invalid index given.");
		std::map<std::string, C2DEntity*>::iterator it = _mmapEntitys.begin();
		unsigned int ui = 0;
		while (ui < uiIndex)
		{
			ui++;
			it++;
		}
		// De-allocate memory for the object
		delete it->second;

		// Remove object from hash map
		_mmapEntitys.erase(it);
	}

	void C2DLayer::removeAllEntitys(void)
	{
		// Remove all objects
		std::map<std::string, C2DEntity*>::iterator it = _mmapEntitys.begin();
		while (it != _mmapEntitys.end())
		{
			delete it->second;
			_mmapEntitys.erase(it);
			it = _mmapEntitys.begin();
		}
	}

	unsigned int C2DLayer::getNumEntitys(void) const
	{
		return int(_mmapEntitys.size());
	}

	C2DEntityComplex* C2DLayer::addEntityComplex(const std::string& strUniqueName)
	{
		// Attempt to find if the object name already exists
		std::map<std::string, C2DEntityComplex*>::iterator it = _mmapEntityComplexs.find(strUniqueName);
		ThrowIfTrue(it != _mmapEntityComplexs.end(), "C2DLayer::addEntityComplex(\"" + strUniqueName + "\") failed. The object already exists.");

		// Allocate memory for new object
		C2DEntityComplex* pNew = new C2DEntityComplex();
		ThrowIfFalse(pNew, "C2DLayer::addEntityComplex(\"" + strUniqueName + "\") failed. Unable to allocate memory.");

		// Add object to hash map
		_mmapEntityComplexs[strUniqueName] = pNew;
		return pNew;
	}

	bool C2DLayer::getEntityComplexExists(const std::string& strUniqueName) const
	{
		if (_mmapEntityComplexs.find(strUniqueName) == _mmapEntityComplexs.end())
			return false;
		return true;
	}

	C2DEntityComplex* C2DLayer::getEntityComplex(const std::string& strUniqueName) const
	{
		// Attempt to find if the name already exists
		std::map<std::string, C2DEntityComplex*>::iterator it = _mmapEntityComplexs.find(strUniqueName);
		ThrowIfTrue(it == _mmapEntityComplexs.end(), "C2DLayer::getEntityComplex(\"" + strUniqueName + "\") failed. Object name doesn't exist.");
		return it->second;
	}

	C2DEntityComplex* C2DLayer::getEntityComplex(unsigned int uiIndex) const
	{
		// Make sure given index is valid
		ThrowIfTrue(uiIndex >= _mmapEntityComplexs.size(), "C2DLayer::getEntityComplex(" + std::to_string(uiIndex) + ") failed. Invalid index given.");
		std::map<std::string, C2DEntityComplex*>::iterator it = _mmapEntityComplexs.begin();
		unsigned int ui = 0;
		while (ui < uiIndex)
		{
			ui++;
			it++;
		}
		return it->second;
	}

	void C2DLayer::removeEntityComplex(const std::string& strUniqueName)
	{
		// Attempt to find if the layer name already exists
		std::map<std::string, C2DEntityComplex*>::iterator it = _mmapEntityComplexs.find(strUniqueName);
		ThrowIfTrue(it == _mmapEntityComplexs.end(), "C2DLayer::removeEntityComplex(\"" + strUniqueName + "\") failed. The object doesn't exist.");

		// De-allocate memory
		delete it->second;

		// Remove from hash map
		_mmapEntityComplexs.erase(it);
	}

	void C2DLayer::removeEntityComplex(unsigned int uiIndex)
	{
		// Make sure given index is valid
		ThrowIfTrue(uiIndex >= _mmapEntityComplexs.size(), "C2DLayer::removeEntityComplex(" + std::to_string(uiIndex) + ") failed. Invalid index given.");
		std::map<std::string, C2DEntityComplex*>::iterator it = _mmapEntityComplexs.begin();
		unsigned int ui = 0;
		while (ui < uiIndex)
		{
			ui++;
			it++;
		}
		// De-allocate memory for the object
		delete it->second;

		// Remove object from hash map
		_mmapEntityComplexs.erase(it);
	}

	void C2DLayer::removeAllEntityComplexs(void)
	{
		// Remove all objects
		std::map<std::string, C2DEntityComplex*>::iterator it = _mmapEntityComplexs.begin();
		while (it != _mmapEntityComplexs.end())
		{
			delete it->second;
			_mmapEntityComplexs.erase(it);
			it = _mmapEntityComplexs.begin();
		}
	}

	int C2DLayer::getNumEntityComplexs(void) const
	{
		return int(_mmapEntityComplexs.size());
	}
}