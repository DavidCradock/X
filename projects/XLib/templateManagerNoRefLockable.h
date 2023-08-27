#pragma once
#include "PCH.h"
#include "logging.h"

namespace X
{
	// Template class to manage pointers to objects of a class of the given type.
	// This class has the ability to "lock" resources when they are added.
	// Locked objects are not removed upon the various remove methods unless they are passed true to their "bForceRemoveLocked" parameter.
	// If you don't need this functionality, you can use the CManager or CManagerNoRef template classes instead.
	template <typename T> class CManagerNoRefLockable
	{
	public:
		// Constructor
		CManagerNoRefLockable()
		{
		}

		// Destructor
		~CManagerNoRefLockable()
		{
			removeAll(true);
		}

		// Adds a new uniquely named object.
		// If the named object already exists, this simply returns the object's pointer
		// If bLocked is set to true, this affects the various remove method's behaviour.
		// If calling a remove method on a named object which is set as locked, it will not
		// be removed unless that remove method is passed true to it's bForceRemoveLocked parameter.
		T* add(const std::string& strObjectName, bool bLocked = false);

		// Attempts to remove the named object.
		// If the named object does not exist, an exception occurs.
		// If the named object is set as locked, it will not be removed unless bForceRemoveLocked is true.
		void remove(const std::string& strObjectName, bool bForceRemoveLocked = false);

		// Returns whether the named object exists or not
		bool exists(const std::string& strObjectName);

		// Removes all objects.
		// bForceRemoveLocked, if true will remove objects regardless of their locked status, else, only unlocked resources are removed.
		void removeAll(bool bForceRemoveLocked = false);

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
			bool bLocked;
		};
		std::map<std::string, SObject*> _mmapObjects;
	};

	template <class T>
	T* CManagerNoRefLockable<T>::add(const std::string& strObjectName, bool bLocked)
	{
		// Find object
		auto it = _mmapObjects.find(strObjectName);

		// If object exists, return object
		if (_mmapObjects.end() != it)
		{
			return it->second->pObject;
		}

		// Create new object and return it
		SObject* pNewObject = new SObject;
		ThrowIfFalse(pNewObject, "CManagerNoRefLockable::add(\"" + strObjectName + "\") failed. Unable to allocate memory for new object.");
		pNewObject->bLocked = bLocked;
		pNewObject->pObject = new T;
		ThrowIfFalse(pNewObject->pObject, "CManagerNoRefLockable::add(\"" + strObjectName + "\") failed. Unable to allocate memory for new object.");
		_mmapObjects[strObjectName] = pNewObject;
		return pNewObject->pObject;
	}

	template <class T>
	void CManagerNoRefLockable<T>::remove(const std::string& strObjectName, bool bForceRemoveLocked)
	{
		// Find object
		auto it = _mmapObjects.find(strObjectName);

		// If object doesn't exist, throw exception
		ThrowIfTrue(_mmapObjects.end() == it, "CManagerNoRefLockable::remove(\"" + strObjectName + "\") failed. Object does not exist.");

		// If object is locked, simply return
		if (it->second->bLocked && bForceRemoveLocked == false)
			return;

		// Remove object
		delete it->second->pObject;
		delete it->second;
		_mmapObjects.erase(it);
	}

	template <class T>
	bool CManagerNoRefLockable<T>::exists(const std::string& strObjectName)
	{
		auto it = _mmapObjects.find(strObjectName);
		return (_mmapObjects.end() != it);
	}

	template <class T>
	void CManagerNoRefLockable<T>::removeAll(bool bForceRemoveLocked)
	{
		// Remove all objects
		if (bForceRemoveLocked)
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

		// Go through map of objects, check locked status of each and get names for objects which should be removed
		std::vector<std::string> vecstrObjectsToDelete;
		auto it = _mmapObjects.begin();
		while (_mmapObjects.end() != it)
		{
			if (!it->second->bLocked)
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
	size_t CManagerNoRefLockable<T>::getNumber(void)
	{
		return _mmapObjects.size();
	}

	template <class T>
	std::string CManagerNoRefLockable<T>::getName(size_t index)
	{
		ThrowIfTrue(index < 0 || index >= _mmapObjects.size(), "CManagerNoRefLockable::getName(\"" + std::to_string(index) + "\") failed. Invalid index value given.");
		auto it = _mmapObjects.begin();
		for (size_t i = 0; i < index; index++)
		{
			it++;
		}
		return it->first;
	}

	template <class T>
	T* CManagerNoRefLockable<T>::get(size_t index)
	{
		ThrowIfTrue(index < 0 || index >= _mmapObjects.size(), "CManagerNoRefLockable::get(\"" + std::to_string(index) + "\") failed. Invalid index value given.");

		auto it = _mmapObjects.begin();
		for (size_t i = 0; i < index; index++)
		{
			it++;
		}
		return it->second->pObject;
	}

	template <class T>
	T* CManagerNoRefLockable<T>::get(const std::string& strObjectName)
	{
		auto it = _mmapObjects.find(strObjectName);
		ThrowIfTrue(_mmapObjects.end() == it, "CManagerNoRefLockable::get(\"" + strObjectName + "\") failed. Object does not exist.");
		return it->second->pObject;
	}
}