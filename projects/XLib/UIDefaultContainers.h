#pragma once
#include "PCH.h"
#include "timer.h"
#include "UIDefConConsole.h"
#include "UIDefConDefaults.h"
#include "UIDefConFontGenerator.h"
#include "UIDefConImageEditor.h"
#include "UIDefConMapEditor.h"
#include "UIDefConProfiling.h"
#include "UIDefConSettings.h"
#include "UIDefConStatistics.h"
#include "UIDefConUIThemeEditor.h"

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
			// Holds the name of the default container called "X:Default:Console"
			// A text based console/terminal for executing various commands.
			std::string console;

			// Holds the name of the default container called "X:Default:Defaults"
			// A window to show and hide all of the default containers found here.
			std::string defaults;

			// Holds the name of the default container called "X:Default:FontGenerator"
			// A container with functionality to generate font files from fonts stored in the operating system which
			// can then be loaded by the SCResourceManager for the font resources.
			std::string fontGenerator;
			
			// Holds the name of the default container called "X:Default:ImageEditor"
			// A window with functionality to create/load/save/edit/display images.
			std::string imageEditor;

			// Holds the name of the default container called "X:Default:MapEditor"
			std::string mapEditor;

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

		CUIDefConConsole console;
		CUIDefConDefaults defaults;
		CUIDefConFontGenerator fontGenerator;
		CUIDefConImageEditor imageEditor;
		CUIDefConMapEditor mapEditor;
		CUIDefConProfiling profiling;
		CUIDefConSettings settings;
		CUIDefConStatistics statistics;
		CUIDefConUIThemeEditor themeEditor;

	private:
		// Is set to true once initialise() has been called.
		bool _mbInitialised;

		
	};
}