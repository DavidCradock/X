#pragma once
#include "PCH.h"
#include "singleton.h"
#include "UIContainer.h"
#include "UITheme.h"
#include "UIWindow.h"
#include "templateManager.h"

namespace X
{
	// Here's where we go for everything UserInterface related.
	//
	// The user interface has a few nice features...
	// The cursor is used as a light source to create a nice bump mapped look to the user interface.
	// The windows of the UI can be semi transparent and blur the background of the current state of the back buffer's frame buffer.
	// The UI uses the messaging classes instead of function pointers for us to determine UI events such as button clicks, slider movement etc.
	// Audio playback for various UI events such as button clicks.
	// 
	// Position coordinates and dimensions.
	// Position coordinates and dimensions are all specified in pixels.
	// Everything is offset from the top left corner of the screen, where coordinates of 0, 0 represent the top left corner of the screen.
	// The X coordinate is from the left edge of the screen, starting at 0, increasing as we go across the screen.
	// The Y coordinate is from the top edge of the screen, starting at 0, increasing as we go down the screen.
	//
	// The user interface consists of the following components...
	// 
	// Widgets:
	// A widget is the name given to almost all individual types of objects which represent an object in the user interface.
	// There are many types of widgets such as buttons, text, images and more.
	// 
	// Widget containers:
	// A widget container is just what it sounds like. It's a container to hold one or more widgets.
	// A container itself is invisible.
	// A container has a screen position (representing it's top left corner's position) and dimensions.
	// If a widget is added to the container and the widget's position is set so that the widget is outside of the range of the container's dimensions,
	// then the container displays scroll bars which the user can interact with to scroll the container's contents so that a widget is visible.
	// 
	// Windows:
	// A window has a widget container which is used to add widgets to.
	// This widget container's position is set to the position of the window.
	// Windows can be moved around the screen and have a z-order used when being rendered.
	// A window has borders, a titlebar and possibly minimize, maximize and close buttons.
	// When specifying a window's dimensions, the window's widget container's dimensions are set to the dimensions given.
	// This means that when specifying a window's dimensions, the window itself may be slightly larger than the dimensions specified due to the extra
	// pixels used by the window's borders and title bar. This size increase is dependent upon which theme the window is set to be using.
	// 
	// Widgets (More information):
	// Widgets include buttons, image buttons which use a custom set image, static text, editable text boxes,
	// text boxes with vertical scroll bar, progress bars, static images, animated images, frame buffer boxes which render the
	// contents of a framebuffer, draggable images/framebuffers which can be dragged between "draggable dock" widgets, line graphs, expandable menus
	// with support for holding various widgets such as buttons, text and images, for each item in the menu.
	// We also have a taskbar, sliders which may be horizontal or vertical.
	// I may also add more such as a colour selection widget.
	// 
	// Taskbars:
	// A taskbar is a container with special, automatically controlled behaviour.
	// They hold minimized windows and may contain additional widgets which you may add yourself. Ideas include static text which
	// displays the time and maybe also a notification area, much like on the window managers of our beloved operating systems.
	// 
	// Tooltips:
	// Hovering over widgets may show a tooltip, which may be as simple as a line of text, or could hold for example, a complete UI container
	// with text, animated images, progress bars etc. All the widgets which are non-interactable can be placed inside a tooltip of a widget.
	// As creating a fancy container with many widgets for use as a widget's tooltip could be complicated and time consuming to program,
	// there are helper functions which create various "tooltip templates" such as "just text" or "left aligned text box with right animated image".
	// 
	// Rendering features:
	// We have colour and transparency with blurred background, and normal maps for lighting effects where the mouse can be a light source.
	// 
	// Audio features:
	// Only one feature, we can playback sounds when things get clicked, dropped, closed etc.
	// 
	// Themes:
	// Each of the widgets containers use a default theme, more themes can be created and then switched between at runtime.
	// Themes may be set per container.
	// Themes require many individual images which are created in an external application such as Blender.
	// Most widgets need 18 images each, there's many widgets, so that's a lot of images to create, it's worth it because it allows quite a bit of flexibility.
	// See CUITheme for more information on themes.
	// 
	// There are also some default windows which can be shown (hidden by default) and they are named as follows...
	// X:Default:Statistics		// This shows FPS counter, also has a graph
	// X:Default:FontGenerator	// A container with functionality to generate font files from fonts stored in the operating system
	// X:Default:Settings		// Settings which are stored in SCSettings can be fiddled with in this container.
	// They are created in SUIManager::_createDefaultContainers()
	// 
	// Implementation details:
	// Containers are Z sorted and rendered one at a time, along with their widgets.
	// Then the tooltips are rendered next.
	// Where possible, certain things are rendered to framebuffers which are only updated if needed to prevent rendering of stuff every frame. 
	// A great use of this is with text which is quite demanding and if the text doesn't change, there's no point rendering it each frame update.
	// As themes contain many images per widget and there are many widgets, there are many images/textures to deal with.
	// These are all stored on disk as seperate files and are added to a single texture atlas during startup to reduce texture rebinding all over the place.
	class SCUIManager : public CSingleton<SCUIManager>
	{
	public:
		SCUIManager();

		// Updates and renders the UI
		void render(const std::string& strFramebufferToSampleFrom = "X:backbuffer");

		// Adds a new theme for use by the UI and returns a pointer to it
		// The new theme is initialised to use the default settings and image files.
		// If the named theme already exists, a pointer to that theme is returned.
		CUITheme* themeAdd(const std::string& strName);

		// Returns a pointer to a named theme.
		// If the named object doesn't exist, an exception occurs.
		CUITheme* themeGet(const std::string& strName) const;

		// Returns true if the named theme exists
		bool themeExists(const std::string& strName) const;

		// Removes the named theme.
		// If the named object doesn't exist, this silently fails.
		void themeRemove(const std::string& strName);

		// Removes all themes.
		void themeRemoveAll(void);

		// Returns the number of themes
		size_t themeGetNumber(void) const;

		// Returns the name of a theme at the specified index.
		// If an invalid index is given, an exception occurs.
		std::string themeGetName(int iIndex) const;

		// Sets all UI objects to use the named theme.
		// If the passed theme name doesn't exist, an exception occurs
		void setThemeForAll(const std::string& strName);
	private:
		mutable CManager<CUITheme> _mmanThemes;
//		mutable std::map<std::string, CUITheme*> _mmapThemes;			// A hashmap holding each named theme.
	};
}