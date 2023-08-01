#pragma once
#include "PCH.h"

namespace X
{
	// Base class to derive application classes from
	class CApplicationBase
	{
	public:
		// Initialises running time to zero
		CApplicationBase();

		// Override this if you need it, by default, it does nothing (Virtual method).
		// This method is called only once upon initialisation after window and other stuff has been initialised
		virtual void initOnce(void);

		// Override this if you need it, by default, it does nothing (Virtual method).
		// This is called each time the application is set as current from the application manager.
		// AKA, we're starting this application
		// We should add all resources needed for the application to the various managers here
		virtual void onStart(void);

		// Override this if you need it, by default, it does nothing (Virtual method).
		// This is called each time the application is the current application and some other application is set as current from the application manager.
		// AKA, we're stopping this application
		// We should remove all resources needed for the application from the various managers here
		virtual void onStop(void);

		// Pure virtual, so we need to override this.
		// It is called once per program loop for the currently set application
		// return false here if the application wants to close.
		virtual bool onUpdate(void) = 0;
	
		float mfApplicationSecondsRunning;	// Number of seconds the application has been running. Updated by SCApplicationManager
	protected:
	};
}