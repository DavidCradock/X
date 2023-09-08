#pragma once
#include "PCH.h"
#include "singleton.h"
#include "GUIProgressBar.h"
#include "GUIContainer.h"

namespace X
{
	// Here's the loading screen class which can be shown during application initialisation.
	// By default, it is disabled.
	// It not only deals with each of the resources which take time to initialise from the SCResourceManager,
	// but also from any other resources, such as the SCAudioManager too.
	// Not all resources use this, resources which are generated really quickly like depth buffers are ignored.
	//
	// To use the loading screen, in our application class's onInit() method, where we normally load resources,
	// we first get a pointer to this class and then set all the options for the loading screen with all the various
	// methods available and then we call onInit() which accepts the total number of resources
	// which are to be loaded. Then we add our resources to the various resource managers as normal.
	// Then, at the end of the onInit() method of our application, we call onInitEnd().
	// 
	// To obtain the total number of resources to load, we could go through our code and manually count how many
	// resources to load, but who wants to do that!? Not me.
	// The solution, is to run to app once, passing a value less than 1 to the number of resources to load to the onInit()
	// method and then the loading screen shows how many resources were loaded and tada!
	// The program has counted the total number of resources for us, which we then pass to the onInit() method.
	//
	// Each addSomeResource method calls this class's loadingResource() method, passing in the name of the resource
	// type (such as "Texture") and the name of the actual resource
	//
	// Please see below for all the options which may be set before calling the onInit() method.
	class SCResourceLoadingScreen : public CSingleton<SCResourceLoadingScreen>
	{
	public:
		// Constructor, sets values to defaults and loads the resources for the loading screen in, ready for use.
		SCResourceLoadingScreen();

		// Called by the various resource managers each time a resource is added, just prior to loading/calculating that resource.
		void loadingResource(const std::string& strResourceTypeName, const std::string& strResourceName);
		
		// Call this from our application's onInit() method, AFTER calling all the various methods which set how the loading
		// screen should behave/look like.
		// Calling this method, enables the loading screen.
		void onInit(int iTotalNumberOfResourcesToLoad = 0);

		// Must be called at the end of our application's onInit() method one all resources are loaded.
		void onInitEnd(void);

		// Sets the loading screen's small text font filename to use during rendering, it also loads it in ready for use
		void setFontSmall(const std::string& strFilename);

		// Sets the loading screen's large text font filename to use during rendering, it also loads it in ready for use
		void setFontLarge(const std::string& strFilename);

		// Sets the loading screen's background image filename to use during rendering, it also loads it in ready for use
		void setBackground(const std::string& strFilename);

		// Sets whether the loading screen fades in.
		// Set this to zero to disable fading in, else the number of seconds in which it takes to fade in.
		// By default, this is set to 0.0
		void setFadeIn(float fFadeInTimeSeconds = 0.0f);

		// Sets whether the loading screen fades out once loading is finished or not.
		// Set this to zero to disable fading out, else the number of seconds in which it takes to fade out.
		// By default, this is set to 1.0
		void setFadeOut(float fFadeOutTimeSeconds = 1.0f);

		// Sets the GUI progress bar position and dimensions
		// By default, position is set to (pWindow->getDimensions().x * 0.5f) - 300.0f, pWindow->getDimensions().y * 0.5f
		// and dimensions set to 600.0f, 50.0f
		void setProgressBar(const CVector2f& vPosition, const CVector2f& vDimensions);

		// Sets position of the information text shown whilst loading
		// Be default,
		// vLine1Position is set to pWindow->getDimensions().x * 0.5f, (pWindow->getDimensions().y * 0.5f) + 130.0f
		// vLine2Position is set to pWindow->getDimensions().x * 0.5f, (pWindow->getDimensions().y * 0.5f) + 170.0f
		void setInfoTextPos(const CVector2f& vLine1Position, const CVector2f& vLine2Position);

		// Toggles whether to show "Resources loaded" text during fade out
		// Defaults to true
		void setShowResourcesLoadedTextDuringFadeOut(bool bShowText);

		// Toggles whether to show progress bar during fade out
		// Defaults to false
		void setShowProgressBarDuringFadeOut(bool bShowProgressBar);

		// Sets colours for large and small text.
		// The alpha is ignored.
		// Default colours for both are white.
		void setTextColours(const CColour& largeTextColour, const CColour& smallTextColour);

		// Sets position of "Loading Complete" text during fade out
		// Be default this is set to...
		// pWindow->getDimensions().x * 0.5f, pWindow->getDimensions().y * 0.5f
		void setTextLoadingCompletePos(const CVector2f& vPosition);

	private:
		// Called from both loadingResources() and onInitEnd() to render the loading screen
		// To differentiate between being called by loadingResource() and onInitEnd(),
		// when being called from onInitEnd() the strings will be empty.
		// iState is either 0 for being called from onInit() to fade in, 1 for being called from loadingResource() or 2 for
		// being called from onInitEnd();
		void _renderLoadingScreen(const std::string& strResourceTypeName, const std::string& strResourceName, int iState);

		// Settings for the loading screen
		std::string _mstrFontSmall;					// The filename of a font to use when rendering text to the loading screen.
		std::string _mstrFontLarge;					// The filename of a font to use when rendering text to the loading screen.
		std::string _mstrBackgroundImage;			// The filename of an image to display in the background of the loading screen.
		float _mfFadeOutTimeSeconds;				// The number of seconds the loading screen takes to fade out.
		float _mfFadeInTimeSeconds;					// The number of seconds the loading screen takes to fade in.
		CVector2f _mvProgressBarPosition;			// The progress bar's position
		CVector2f _mvProgressBarDims;				// The progress bar's dimensions
		CVector2f _mvInfoTextLine1Pos;
		CVector2f _mvInfoTextLine2Pos;
		bool _mbShowResLoadedTextOnFadeOut;	// Whether to show the "Resources loaded x/y" during fade out or not
		bool _mbShowProgressBarOnFadeOut;	// Whether to show the progress bar during fade out or not
		CColour _mColTextLarge;				// Large text colour
		CColour _mColTextSmall;				// Small text colour
		CVector2f _mvLoadingCompleteTextPos;// Position of "Loading Complete" text on fade out

		bool _mbEnabled;					// Whether the loading screen is enabled or not, if not, then all the calls to loadingResource() do nothing.
		int _miTotalNumResourcesToLoad;		// Passed to onInit()
		int _miNumResourcesLoaded;			// Number of times the loadingResource() method has been called, if the loading screen is enabled.
		bool _mbVsyncStatePriorToLoading;	// We store the currently set VSync state in onInit() and set it back again in onInitEnd()
		float _mfFadeOutCountdown;			// Used to fade out in onInitEnd();
		float _mfFadeInCountdown;			// Used to fade in, in onInit();
		std::string _mstrInitialMouseCursorFilename;	// The mouse cursor which was set prior to changing it to busy

		// Used by onInit and onInitEnd to store and restore all GUI container visibility states
		struct SContainerState
		{
			CGUIContainer* pContainer;
			bool bVisibility;
		};
		std::vector<SContainerState> _mvecContainerVisibilityStates;
	};
}