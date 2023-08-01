#pragma once
#include "PCH.h"

namespace X
{
	// Singleton class to inherit from.
	// Usage:
	// Create a class from this template class with something like...
	// class MyClass : public CSingleton<MyClass>
	// From then on, whenever you create an object of MyClass like normal, it only has one instance, all variables are shared.
	// To create a pointer to the singleton...
	// MyClass *pMyClass = MyClass::getPointer();
	// Then you can access all the public stuff with something like...
	// pMyClass->someMethod();
	template <class T>
	class CSingleton
	{
	public:
		virtual ~CSingleton()
		{
			if (pInstance)
			{
				delete pInstance;
				pInstance = 0;
			}
		}

		// Obtain pointer to class object
		// If not yet created, the object will be created here and it's constructor called 
		static T* getPointer(void)
		{
			if (!pInstance)
			{
				pInstance = new T();
			}
			return pInstance;
		}

	protected:
		static T* pInstance;
	};
	template <class T> T* CSingleton<T>::pInstance = 0;
}