#pragma once
#include "PCH.h"

// This file contains comments to help explain threading and possibly helper code to help deal with multithreading.
// 
// Benefits of threading:
// Improved performance : Leverage your hardware's capabilities by running tasks concurrently.
// Responsiveness : Maintain UI responsiveness while performing CPU - intensive tasks in the background.
// Simplified code : Modern C++ threading tools have a cleaner and more expressive syntax compared to older approaches.
// Portable applications : Write thread - safe code once and run it on different platforms without modification if using the modern C++ 11 or above stuff.
// 
// Use modern C++ features such as...
// std::thread which makes it so we no longer have to use platform specific API calls such as pthreads or Boost.threads.
// Lookup Tasks: C++20 introduces tasks, providing a higher-level abstraction for scheduling and managing work units.
// Lookup Parallel algorithms: The standard library offers parallel algorithms like std::async and std::parallel_for for efficient use of multicore processors.
//
// Creating threads
// 
// Example 1 using function pointers...
// void someFunction(const std::vector<int> &v, int someVar)
// {
//		// Do stuff here
// }
// 
// // Create a vector and fill it with stuff (omitted)
// std::vector<int> v;
// 
// // Create the thread
// std::thread t1(someFunction, std::ref(v), 7);
//
// Wait for the thread to finish 
// t1.join();
// 
// // All parameters are passed by value, unless we wrap them in std::ref(theVariable)
// // Threads created this way, do not have return values.
// // If we wish to return something, we need to store it in one of the funcction's parameters passed by reference.
// 
// Example 2 using functors (Function objects)...
// // First, what is a functor?
// // A functor is a class or struct which acts like a function.
// // Overloads the function call operator (): This allows you to invoke the functor as if it were a function.
// // Can hold member variables: Functors can store data internally, giving them capabilities beyond simple functions.
// // Here's an example of a functor...
// struct Multiplier
// {
//  int factor;
//  int operator()(int x)
//  {
//	 return x * factor;  // Apply the multiplication using the stored factor
//  }
// };
//
// int main()
// {
//	 Multiplier triple{ 3 };  // Create a functor that multiplies by 3
//	 int result = triple(5);  // Call the functor like a function
//	 // result will be 15
// }
// 
// // Now we know what a functor is, let's see how to create a thread using one...
// 
// class CSomeClassFunctor
// {
//  public:
// 	 void operator()(const std::vector<int>& v,	unsigned int beginIndex, unsigned int endIndex)
//	 {
//		_acm = 0;
//		for (unsigned int i = beginIndex; i < endIndex; ++i)
//		{
//			_acm += v[i];
//		}
//	}
//	unsigned long long _acm;
// };
// 
// {
// CAccumulatorFunctor3 accumulator1 = CAccumulatorFunctor3();
// CAccumulatorFunctor3 accumulator2 = CAccumulatorFunctor3();
// std::thread t1(std::ref(accumulator1), std::ref(v), 0, v.size() / 2);
// std::thread t2(std::ref(accumulator2), std::ref(v), v.size() / 2, v.size());
// t1.join();
// t2.join();
// std::cout << "acm1: " << accumulator1._acm << endl;
// std::cout << "acm2: " << accumulator2._acm << endl;
// std::cout << "accumulator1._acm + accumulator2._acm : " << accumulator1._acm + accumulator2._acm << endl;
// }
// 
// To get the maximum number of cores you can call: std::thread::hardware_concurrency()
// 
// 
// Mutexes (Syncronization primitive)
// These help prevent corruption of data when being accessed by multiple threads.
// The idea is to lock a mutex inside a thread, then access/modify the data, then unlock the mutex when done.
// This guarantees that the data is only being read/written to by one thread at a time.
// 
// Here's an example using a mutex, manually using lock and unlock...
// std::mutex g_display_mutex;
// thread_function()
// {
// 	 g_display_mutex.lock();
//	 std::thread::id this_id = std::this_thread::get_id();
//	 std::cout << "My thread id is: " << this_id << endl;
//	 g_display_mutex.unlock();
// }
// This is not recommended as we have to remember to call unlock and if we forget, can cause issues.
// Instead use...
//
// Here's an example using lock guards
// std::mutex g_display_mutex;
// thread_function()
// {
//	 std::lock_guard<std::mutex> guard(g_display_mutex);
//	 std::thread::id this_id = std::this_thread::get_id();
//	 std::cout << "From thread " << this_id << endl;
// }
// This way, lock and unlock are automatically called for us when the function exits.
// There's no need to remember to call unlock.

namespace X
{

}