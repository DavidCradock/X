#pragma once
#include "PCH.h"
#include "logging.h"

namespace X
{
	// Template class to manage pointers to objects of a class of the given type.
	// An object has a reference count, so if it is added multiple times, it will not be removed with calls to remove() until the reference count has reached 0.
	// That is unless removeAll() is passed true to it's bForceRemoval parameter in which case, it is.
	// If you do not wish to mess around with references, use the CManagerNoRef template class instead.
	template <typename T> class CManager
	{
	public:
		// Constructor
		CManager()
		{
		}

		// Destructor
		~CManager()
		{
			removeAll(true);
		}

		// Adds a new uniquely named object.
		// If the named object already exists, it's reference count is increased.
		T* add(const std::string& strObjectName);

		// Attempts to remove the named object.
		// If the named object does not exist, an exception occurs.
		// If the named object exists and has been added multiple times, it's reference count is decreased and the object remains.
		void remove(const std::string& strObjectName);

		// Returns whether the named object exists or not
		bool exists(const std::string& strObjectName);

		// Removes all objects.
		// bForceRemoval, if true will remove objects regardless of their reference count, else only objects with a reference count of 0 will be removed.
		void removeAll(bool bForceRemoval = false);

		// Returns the number of objects.
		size_t getNumber(void);

		// Attempts to find and return the name of the object at the specified index.
		// If the given index is invalid, an exception occurs.
		// Use getNumber() to determine valid index range.
		std::string getName(size_t index);

		// Returns a pointer to the object at the specified index.
		// If the given index is invalid, an exception occurs.
		// Use getNumber() to determine valid index range.
		T* get(size_t index);

		// Attempts to find and return a pointer to the named object.
		// If the named object could not be found, an exception occurs.
		// Use exists() to determine whether the object exists to prevent exception from occurring.
		T* get(const std::string& strObjectName);
	private:
		struct SObject
		{
			T* pObject;
			unsigned int iRefCount;
		};
		std::map<std::string, SObject*> _mmapObjects;
	};

	template <class T>
	T* CManager<T>::add(const std::string& strObjectName)
	{
		// Find object
		auto it = _mmapObjects.find(strObjectName);

		// If object exists, increment reference count and return object
		if (_mmapObjects.end() != it)
		{
			it->second->iRefCount++;
			return it->second->pObject;
		}

		// Create new object and return it
		SObject* pNewObject = new SObject;
		ThrowIfFalse(pNewObject, "CManager::add(\"" + strObjectName + "\") failed. Unable to allocate memory for new object.");
		pNewObject->iRefCount = 1;
		pNewObject->pObject = new T;
		ThrowIfFalse(pNewObject->pObject, "CManager::add(\"" + strObjectName + "\") failed. Unable to allocate memory for new object.");
		_mmapObjects[strObjectName] = pNewObject;
		return pNewObject->pObject;
	}

	template <class T>
	void CManager<T>::remove(const std::string& strObjectName)
	{
		// Find object
		auto it = _mmapObjects.find(strObjectName);

		// If object doesn't exist, throw exception
		ThrowIfTrue(_mmapObjects.end() == it, "CManager::remove(\"" + strObjectName + "\") failed. Object does not exist.");

		// Decrement object's reference count
		it->second->iRefCount--;

		// If reference count is still high, simply return
		if (it->second->iRefCount > 0)
			return;

		// Remove object
		delete it->second->pObject;
		delete it->second;
		_mmapObjects.erase(it);
	}

	template <class T>
	bool CManager<T>::exists(const std::string& strObjectName)
	{
		auto it = _mmapObjects.find(strObjectName);
		return (_mmapObjects.end() != it);
	}

	template <class T>
	void CManager<T>::removeAll(bool bForceRemoval)
	{
		// Remove all objects
		if (bForceRemoval)
		{
			auto it = _mmapObjects.begin();
			while (it != _mmapObjects.end())
			{
				delete it->second->pObject;
				delete it->second;
				_mmapObjects.erase(it);
				it = _mmapObjects.begin();
			}
			return;
		}

		// Go through map of objects, decrease reference count of each and get names for objects which should be removed
		std::vector<std::string> vecstrObjectsToDelete;
		auto it = _mmapObjects.begin();
		while (_mmapObjects.end() != it)
		{
			if (it->second->iRefCount == 1)
			{
				vecstrObjectsToDelete.push_back(it->first);
			}
			it++;
		}

		// Remove each object name found
		for (size_t i = 0; i < vecstrObjectsToDelete.size(); i++)
			remove(vecstrObjectsToDelete[i]);
	}

	template <class T>
	size_t CManager<T>::getNumber(void)
	{
		return _mmapObjects.size();
	}

	template <class T>
	std::string CManager<T>::getName(size_t index)
	{
		ThrowIfTrue(index < 0 || index >= _mmapObjects.size(), "CManager::getName(\"" + std::to_string(index) + "\") failed. Invalid index value given.");
		auto it = _mmapObjects.begin();
		for (size_t i = 0; i < index; i++)
		{
			it++;
		}
		return it->first;
	}

	template <class T>
	T* CManager<T>::get(size_t index)
	{
		ThrowIfTrue(index < 0 || index >= _mmapObjects.size(), "CManager::get(\"" + std::to_string(index) + "\") failed. Invalid index value given.");

		auto it = _mmapObjects.begin();
		for (size_t i = 0; i < index; i++)
		{
			it++;
		}
		return it->second->pObject;
	}

	template <class T>
	T* CManager<T>::get(const std::string& strObjectName)
	{
		auto it = _mmapObjects.find(strObjectName);
		ThrowIfTrue(_mmapObjects.end() == it, "CManager::get(\"" + strObjectName + "\") failed. Object does not exist.");
		return it->second->pObject;
	}
}