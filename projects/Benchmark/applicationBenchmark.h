#pragma once
#include "../XLib/X.h"

namespace X
{
	// Finite state machine state classes
	class CState01_Welcome : public CFiniteStateBase
	{
	public:
		void onEnter(void);	// This is called when this state is first entered.
		void onActive(CFiniteStateMachine* pFSM);	// This is called whilst the state is active.
		void onExit(void);	// This is called when this state is left.
		CTimer timer;
		float fTimeDelayUntilAutostart;
	};

	class CState02_2DSprites : public CFiniteStateBase
	{
	public:
		void onEnter(void);	// This is called when this state is first entered.
		void onActive(CFiniteStateMachine* pFSM);	// This is called whilst the state is active.
		void onExit(void);	// This is called when this state is left.

		CTimer timer;
		float fCentreSpriteRotation;
		std::vector<std::string> vstrImages;	// Each image name for entity
		float fSecondsNoEntitiesAdded;			// To detect when benchmark has reached the end
	};

	class CState99_Results : public CFiniteStateBase
	{
	public:
		void onEnter(void);	// This is called when this state is first entered.
		void onActive(CFiniteStateMachine* pFSM);	// This is called whilst the state is active.
		void onExit(void);	// This is called when this state is left.
	};

	class CApplication : public CApplicationBase
	{
	public:
		// Override this if you need it, by default, it does nothing.
		// This method is called only once upon initialisation after window and other stuff has been initialised
		void initOnce(void);

		// Override this if you need it, by default, it does nothing.
		// This is called each time the application is set as current from the application manager.
		// AKA, we're starting this application
		void onStart(void);

		// Override this if you need it, by default, it does nothing.
		// This is called each time the application is the current application and some other application is set as current from the application manager.
		// AKA, we're stopping this application
		void onStop(void);

		// Pure virtual, so we need to override this
		// It is called once per program loop for the currently set application
		// Return false if the app wants to shutdown
		bool onUpdate(void);
	private:
		
		CFiniteStateMachine _mFSM;	// Finite state machine holding each of the states for the application
	};
}