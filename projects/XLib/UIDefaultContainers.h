#pragma once
#include "PCH.h"

namespace X
{
	// A class to initialise and update the default containers.
	// The SCUIManager class has an object of this class and uses it to access the init and update methods here
	class CUIDefaultContainers
	{
	public:
		CUIDefaultContainers();

		// Called via SCUIManager::initialiseDefaultContainers() which is called from SCApplicationManager during initialisation
		// Creates all the default containers and sets all variables up.
		void initialise(void);

		// Called via SCUIManager::_update() to update all the default containers.
		void update(float fTimeDeltaSecs);

		// Structure to hold strings containing the names of the default containers.
		// This saves us from having to remember their names.
		struct SNames
		{
			// Holds the name of the default container called "X:Default:FontGenerator"
			// A container with functionality to generate font files from fonts stored in the operating system which
			// can then be loaded by the SCResourceManager for the font resources.
			std::string fontGenerator;
			
			// Holds the name of the default container called "X:Default:Profiling"
			// Shows profiling statistics for the global CProfiler object stored in the global x object, x->pProfiler.
			// We can add more sections to be profiled and they will be added and shown in this container.
			std::string profiling;
			
			// Holds the name of the default container called "X:Default:Settings"
			// Settings which are stored in SCSettings can be fiddled with in this container.
			std::string settings;
			
			// Holds the name of the default container called "X:Default:Statistics"
			// This shows FPS counter, also has a graph
			std::string statistics;

			// Holds the name of the default container called "X:Default:UIThemeEditor"
			std::string uiThemeEditor;
		};
		// Structure holds strings of each default container's name.
		SNames names;
	private:
		// Is set to true once initialise() has been called.
		bool _mbInitialised;

		void _initFontGenerator(void);						// Init method called from initialise() for "X:Default:FontGenerator" default container.
		void _updateFontGenerator(float fTimeDeltaSec);		// Update method called from update() for "X:Default:FontGenerator" default container.

		void _initProfiling(void);							// Init method called from initialise() for "X:Default:Profiling" default container.
		void _updateProfiling(float fTimeDeltaSec);			// Update method called from update() for "X:Default:Profiling" default container.
		// Struct used by profiling default container
		struct SProfiling
		{
			float fUpdateDelaySeconds;		// Number of seconds between updating of text
			float fUpdateDelay;				// When reached zero, we update the text
		};
		SProfiling _mProfiling;

		void _initSettings(void);							// Init method called from initialise() for "X:Default:Settings" default container.
		void _updateSettings(float fTimeDeltaSec);			// Update method called from update() for "X:Default:Settings" default container.

		void _initStatistics(void);							// Init method called from initialise() for "X:Default:Statistics" default container.
		void _updateStatistics(float fTimeDeltaSec);		// Update method called from update() for "X:Default:Statistics" default container.

		void _initUIThemeEditor(void);						// Init method called from initialise() for "X:Default:UIThemeEditor" default container.
		void _updateUIThemeEditor(float fTimeDeltaSec);		// Update method called from update() for "X:Default:UIThemeEditor" default container.
		
	};
}