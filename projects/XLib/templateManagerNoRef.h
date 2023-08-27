#pragma once
#include "PCH.h"
#include "logging.h"

namespace X
{
	// Template class to manage pointers to objects of a class of the given type.
	template <typename T> class CManagerNoRef
	{
	public:
		// Constructor
		CManagerNoRef()
		{
		}

		// Destructor
		~CManagerNoRef()
		{
			removeAll();
		}

		// Adds a new uniquely named object.
		// If the named object already exists, this simply returns the object's pointer
		T* add(const std::string& strObjectName);

		// Attempts to remove the named object.
		// If the named object does not exist, an exception occurs.
		void remove(const std::string& strObjectName);

		// Returns whether the named object exists or not
		bool exists(const std::string& strObjectName);

		// Removes all objects.
		void removeAll(void);

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
		};
		std::map<std::string, SObject*> _mmapObjects;
	};

	template <class T>
	T* CManagerNoRef<T>::add(const std::string& strObjectName)
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
		ThrowIfFalse(pNewObject, "CManagerNoRef::add(\"" + strObjectName + "\") failed. Unable to allocate memory for new object.");
		pNewObject->pObject = new T;
		ThrowIfFalse(pNewObject->pObject, "CManagerNoRef::add(\"" + strObjectName + "\") failed. Unable to allocate memory for new object.");
		_mmapObjects[strObjectName] = pNewObject;
		return pNewObject->pObject;
	}

	template <class T>
	void CManagerNoRef<T>::remove(const std::string& strObjectName)
	{
		// Find object
		auto it = _mmapObjects.find(strObjectName);

		// If object doesn't exist, throw exception
		ThrowIfTrue(_mmapObjects.end() == it, "CManagerNoRef::remove(\"" + strObjectName + "\") failed. Object does not exist.");

		// Remove object
		delete it->second->pObject;
		delete it->second;
		_mmapObjects.erase(it);
	}

	template <class T>
	bool CManagerNoRef<T>::exists(const std::string& strObjectName)
	{
		auto it = _mmapObjects.find(strObjectName);
		return (_mmapObjects.end() != it);
	}

	template <class T>
	void CManagerNoRef<T>::removeAll(void)
	{
		// Remove all objects
		auto it = _mmapObjects.begin();
		while (it != _mmapObjects.end())
		{
			delete it->second->pObject;
			delete it->second;
			_mmapObjects.erase(it);
			it = _mmapObjects.begin();
		}
	}

	template <class T>
	size_t CManagerNoRef<T>::getNumber(void)
	{
		return _mmapObjects.size();
	}

	template <class T>
	std::string CManagerNoRef<T>::getName(size_t index)
	{
		ThrowIfTrue(index < 0 || index >= _mmapObjects.size(), "CManagerNoRef::getName(\"" + std::to_string(index) + "\") failed. Invalid index value given.");
		auto it = _mmapObjects.begin();
		for (size_t i = 0; i < index; i++)
		{
			it++;
		}
		return it->first;
	}

	template <class T>
	T* CManagerNoRef<T>::get(size_t index)
	{
		ThrowIfTrue(index < 0 || index >= _mmapObjects.size(), "CManagerNoRef::get(\"" + std::to_string(index) + "\") failed. Invalid index value given.");

		auto it = _mmapObjects.begin();
		for (size_t i = 0; i < index; i++)
		{
			it++;
		}
		return it->second->pObject;
	}

	template <class T>
	T* CManagerNoRef<T>::get(const std::string& strObjectName)
	{
		auto it = _mmapObjects.find(strObjectName);
		ThrowIfTrue(_mmapObjects.end() == it, "CManagerNoRef::get(\"" + strObjectName + "\") failed. Object does not exist.");
		return it->second->pObject;
	}
}