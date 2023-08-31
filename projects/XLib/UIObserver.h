#pragma once
#include "PCH.h"

namespace X
{
	/*
	how to store a pointer to an instance's method


	There are two ways to store a pointer to an instance method in C++:
	Using the .* operator: The .* operator is a member access operator that can be used to access a member function of an object.
	The syntax for using the .* operator to store a pointer to an instance method is:


	typename object_type::*method_pointer = &object.method_name;


	where object_type is the type of the object, method_name is the name of the method, and method_pointer is the pointer to the method.
	Using the ->* operator: The ->* operator is a member access operator that can also be used to access a member function of an object.
	The syntax for using the ->* operator to store a pointer to an instance method is:


	typename object_type::*method_pointer = object.*method_name;


	where object_type is the type of the object, method_name is the name of the method, and method_pointer is the pointer to the method.
	Which operator you use is a matter of personal preference.
	The .* operator is slightly more concise, but the ->* operator is more flexible because it can be used with objects that are passed by reference.
	Here is an example of how to store a pointer to an instance method using the .* operator:


	class MyClass {
	public:
	  void method() { std::cout << "The method method() is called" << std::endl; }
	};

	int main() {
	  MyClass my_class;

	  // Store a pointer to the method method() in the variable method_pointer.
	  void (MyClass::*method_pointer)() = &MyClass::method;

	  // Call the method through the pointer.
	  (my_class.*method_pointer)();

	  return 0;
	}


	Here is an example of how to store a pointer to an instance method using the ->* operator:


	class MyClass {
	public:
	  void method() { std::cout << "The method method() is called" << std::endl; }
	};

	int main() {
	  MyClass my_class;

	  // Store a pointer to the method method() in the variable method_pointer.
	  void (MyClass::*method_pointer)() = my_class.*method;

	  // Call the method through the pointer.
	  (my_class.*method_pointer)();

	  return 0;
	}
		*/
	// An observer base class used by the various widgets of the UI.
	// We derive a class from this base class like so...
	// class CMyObserverForExitButton : public CUIObserver
	// {
	//	public:
	//		void update(void)
	//		{
	//			// Do stuff, like PostQuitMessage or something
	//		}
	// }
	//
	// Then we create an object of this class...
	// CMyObserverForExitButton myObserver;
	//
	// Then we register this object to a UI button
	// pButton->addObserver(&myObserver);
	//
	// Now, when the button is clicked, it'll call myObserver::update()
	class CUIObserver
	{
	public:
	};
	/*
	In a widget which uses this mechanism
	*/
}