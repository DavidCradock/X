#pragma once
#include "../XLib/X.h"

namespace X
{
	// 
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
		CTimer timer;

		// Create benchmark information GUI and results container.
		void _initGUI(void);

		// Each application state
		enum AppState
		{
			welcomeScreen,
			benchmark1,
			benchmark2,
			benchmark3,
			showResults
		};
		// Holds the state of the application, starting at "welcomeScreen" for showing the welcome screen,
		// then incrementing for each test until finally, we're at the final state which shows the results.
		AppState _mState;

		// Setup benchmarks
		void _initBenchmark1(void);
		void _initBenchmark2(void);
		void _initBenchmark3(void);
		void _initShowResults(void);

		// Update benchmarks
		// Each return true, once the benchmark has complete
		bool _updateBenchmark1(void);
		bool _updateBenchmark2(void);
		bool _updateBenchmark3(void);
		bool _updateShowResults(void);

		struct SBenchmark1
		{
			float fCentreSpriteRotation;
		};
		SBenchmark1 b1;	// Holds variables for benchmark1

	};
}