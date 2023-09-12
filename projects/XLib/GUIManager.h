#pragma once
#include "PCH.h"
#include "singleton.h"
#include "GUIBaseObject.h"
#include "GUIButton.h"
#include "GUIButtonImage.h"
#include "colour.h"
#include "GUIContainer.h"
#include "GUIImage.h"
#include "GUIImageAnimated.h"
#include "GUIImageDepthbuffer.h"
#include "GUIImageFramebuffer.h"
#include "GUILineGraph.h"
#include "GUIProgressBar.h"
#include "GUISlider.h"
#include "GUIText.h"
#include "GUITextEdit.h"
#include "GUITheme.h"
#include "GUITooltip.h"
#include "timer.h"
#include "utilities.h"
#include "resourceFont.h"

namespace X
{
	// Here's where we go for everything GUI related.
	// It works on the concept of a container, which holds various GUI widgets and can be set to act as a moveable window or not.
	// 
	// Containers:
	// A container by default set to behave as a window which can be dragged around the screen, closed and minimized with z-order support.
	// A container can also be set to be an empty container without all the window decorations, invisible by itself, used to add various widgets to.
	// This behaviour is set with a call to CGUIContainer::setBehaviour()
	// If a container is set as a window, it has width and height dimensions. When setting a container's dimensions, the dimensions
	// given, are the area inside the container where widgets may be added, not including the container's borders.
	// If set as a window, it may or may not have title bar text.
	// Containers can also have tabs, which enable a container to switch between a sub container which has it's own set of widgets.
	//
	// Widgets:
	// A container holds widgets. Widgets include buttons, image buttons which use a custom set image, static text, editable text boxes,
	// text boxes with vertical scroll bar, progress bars, static images, animated images, 
	// frame buffer boxes which render the contents of a framebuffer (Scene managers render to framebuffers and these can be used),
	// draggable images/framebuffers which can be dragged between "draggable dock" widgets, line graphs, expandable menus with
	// support for holding widgets such as buttons, text and images, for each item in the menu. We also have taskbars.
	// We also have sliders which may be horizontal or vertical.
	// I may also add more such as a colour selection widget.
	// 
	// Taskbars:
	// A taskbar is a container with special, automatically controlled behaviour.
	// They hold minimized windows and may contain additional widgets which you may add yourself. Ideas include static text which
	// displays the time and maybe also a notification area, much like on the window managers of our beloved operating systems.
	// 
	// Tooltips:
	// Hovering over widgets may show a tooltip, which may be as simple as a line of text, or could hold for example, a complete GUI container
	// with text, animated images, progress bars etc. All the widgets which are non-interactable can be placed inside a tooltip of a widget.
	// As creating a fancy container with many widgets for use as a widget's tooltip could be complicated and time consuming to program,
	// there are helper functions which create various "tooltip templates" such as "just text" or "left aligned text box with right animated image".
	// 
	// Rendering features:
	// We have colour and transparency, and normal maps for lighting effects such as the mouse being a light source and glow.
	// 
	// Audio features:
	// Only one feature, we can playback sounds when things get clicked, dropped, closed etc.
	// 
	// Themes:
	// Each of the widgets and containers(if set as a window) use a default theme, more themes can be created and switched between.
	// Themes may be set per container.
	// A theme is quite complex, but is designed so that it loads fast.
	// Each theme needs the following images...
	// colour:		RGBA. This holds the RGB colour as well as the alpha channel for transparency.
	// normal:		RGB. This holds a normal map used for lighting effects.
	// reflection:	RGB. This is simply an image (or perhaps a framebuffer) which moves as the widgets move (due to their parent container being moved)
	// 
	// As well as the above images, there are other settings which may be modified such as text colour for various widget states (for example,
	// a button's up/over/down/clicked states.
	// 
	// Scaling:
	// We have implemented text scaling which works quite well when using a CResourceFont which has been created from a large font (such as a size
	// of around 200 or so) and this text scales down quite well. But as we know, scaling bitmap images up to a larger size introduces
	// bluriness, so when creating fonts for the GUI to use, create them huge as to maintain decent appearence.
	// Do the same for each of the required images (colour/normal/blur/etc). Make them larger than needed for optimal appearance.
	// The GUI does allow scaling up however, but it's best to make the images and fonts large enough, for say an 8k display. At least this way,
	// we have some future proofing.
	// Scaling of the GUI is set with a single float value. Values should be 1.0f, 0.5f, 0.25f, 0.125f for optimal appearance, but can be set to
	// any value.
	//
	// There are also some default containers which can be shown (hidden by default) and they are named as follows...
	// X:Default:Statistics		// This shows FPS counter, also has a graph
	// X:Default:FontGenerator	// A container with functionality to generate font files from fonts stored in the operating system
	// X:Default:Settings		// Settings which are stored in SCSettings can be fiddled with in this container.
	// They are created in SCGUIManager::_createDefaultContainers()
	// 
	// Implementation details:
	// Containers are Z sorted and rendered one at a time, along with their widgets.
	// Then the tooltips are rendered next.
	// Where possible, certain things are rendered to framebuffers which are only updated if needed to prevent rendering of stuff every frame. 
	// A great use of this is with text which is quite demanding and if the text doesn't change, there's no point rendering it each frame update.
	// As themes contain many images per widget and there are many widgets, there are many images/textures to deal with.
	// When creating a theme, we keep things simple for us by creating a theme from many individual images.
	class SCGUIManager : public CSingleton<SCGUIManager>
	{
	public:
		friend class CGUIContainer;
		friend class SCApplicationManager;
		SCGUIManager();

		// Updates and renders the GUI
		void render(const std::string& strFramebufferToSampleFrom = "X:backbuffer");

		// Scaling of the GUI is set with a single float value. Values should be 1.0f, 0.5f, 0.25f, 0.125f for optimal appearance, but can be set to any value.
		void setScale(float fScalingValue);

		// Returns the currently set scaling value of the entire GUI
		float getScale(void) const;

		// Adds a new theme for use by the GUI and returns a pointer to it
		// If the named theme already exists, a pointer to that theme is returned.
		CGUITheme* addTheme(const std::string& strName);

		// Returns a pointer to a named theme.
		// If the named object doesn't exist, an exception occurs.
		CGUITheme* getTheme(const std::string& strName) const;

		// Returns true if the named theme exists
		bool getThemeExists(const std::string& strName) const;

		// Removes the named theme.
		// If the named object doesn't exist, this silently fails.
		void removeTheme(const std::string& strName);

		// Removes all themes.
		void removeAllThemes(void);

		// Returns the number of themes
		int getNumberOfThemes(void) const;

		// Returns the name of a theme at the specified index.
		// If an invalid index is given, an exception occurs.
		std::string getThemeName(int iIndex) const;

		// Sets all GUI objects to use the named theme.
		// If the passed theme name doesn't exist, an exception occurs
		void setThemeForAll(const std::string& strName);

		// Adds a new container and returns a pointer to it
		// If the named container already exists, a pointer to that object is returned.
		// If bLocked is true, this container will NOT be removed when calling any of the remove container methods. This is to prevent removal of the default containers.
		CGUIContainer* addContainer(const std::string& strName, bool bLocked = false);

		// Returns a pointer to a named container.
		// If the named container doesn't exist, an exception occurs.
		CGUIContainer* getContainer(const std::string& strName) const;

		// Returns true if the named container exists
		bool getContainerExists(const std::string& strName) const;

		// Removes the named container.
		// If the named container doesn't exist, this silently fails.
		// If bLocked was true when this container was added with addContainer, then this container is not removed.
		void removeContainer(const std::string& strName);

		// Removes all containers.
		// Any containers added where bLocked was true when the container was added are not removed.
		void removeAllContainers(void);

		// Returns the number of containers
		int getNumberOfContainers(void) const;

		// Returns the name of a container at the specified index.
		// If an invalid index is given, an exception occurs.
		std::string getContainerName(int iIndex) const;

		// Moves the named container's ZOrder so that it at the top/in front
		// If the named container doesn't exist, this does nothing.
		void moveContainerToFront(const std::string& strContainerName);

		// Returns true is a window is being moved
		bool getWindowBeingMoved(void) const;

		// Sets whether a window is being moved 
		void setWindowBeingMoved(bool bWindowBeingMoved);

		// Sets overall volume of the audio playback for the GUI
		// Clamped between 0.0f and 1.0f
		void setAudioVol(float fVol = 1.0f);

		// Returns overall volume of the audio playback for the GUI
		float getAudioVol(void) const;

		// Sets the delay in seconds until a tooltip will begin to fade in when the mouse is over a widget
		// Default is one second
		void setTooltipDelay(float fSeconds);

		// Returns the currently set delay in seconds until a tooltip will begin to fade int when the mouse is over a widget
		float getTooltipDelay(void) const;

		// Sets tooltip offset from mouse cursor position
		void setTooltipOffset(float fOffsetX, float fOffsetY);

		// Gets tooltip offset from mouse cursor position
		CVector2f getTooltipOffset(void) const;

		// Sets the named container as active, setting it's titlebar text to the correct colour stored in theme and all other containers to inactive
		// If the named container doesn't exist, an exception occurs
		void setContainerAsActive(const std::string& strContainerName);
	private:
		CVector2f _mv2TooltipOffset;									// Offset of tooltip from cursor position
		float _mfTooltipDelaySeconds;									// Number of seconds until a tooltip will begin to fade in
		float _mfScale;													// Scaling value used for GUI scaling.
		mutable std::map<std::string, CGUITheme*> _mmapThemes;			// A hashmap holding each named theme.
		mutable std::map<std::string, CGUIContainer*> _mmapContainers;	// A hashmap holding each named container.
		std::list<std::string>	_mlistContainerZOrder;					// Holds each container name, in order of their Z order where the front most container is last in the list.
		CTimer _mTimer;													// Timer object used for time based stuff.
		bool _mbWindowBeingMoved;										// This is used to prevent multiple windows being moved
		float _mfAudioVol;												// Audio volume 0-1 range

		// Creates the default containers which are hidden by default
		// Called from SCApplicationManager mainloop
		void _createDefaultContainers(void);

		// Updates the default containers if they are shown
		// Called from render
		void _updateDefaultContainers(void);

		struct SDefaultContainerStatistics
		{
			float fAddValueToLinegraphDataset;
		};
		SDefaultContainerStatistics _mDefContStatistics;
		void _createDefaultContainerStatistics(void);
		void _updateDefaultContainerStatistics(void);

		struct SDefaultContainerFontGenerator
		{
			CResourceFont* pFont;
			int iHeight;
			int iWeight;
			bool bAntiAliasingOn;
			bool bItalicsOn;
			bool bUnderliningOn;
			bool bStrikeoutOn;
			std::string strFontName;
		};
		SDefaultContainerFontGenerator _mDefContFontGen;
		void _createDefaultContainerFontGenerator(void);
		void _updateDefaultContainerFontGenerator(void);
		void _defaultContainerFontGeneratorBuildFont(void);

		struct SDefaultContainerSettings
		{
		};
		SDefaultContainerSettings _mDefContSettings;
		void _createDefaultContainerSettings(void);
		void _updateDefaultContainerSettings(void);
	};
}