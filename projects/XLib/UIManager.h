#pragma once
#include "PCH.h"
#include "singleton.h"
#include "UITheme.h"
#include "UIContainer.h"
#include "UIWindow.h"
#include "templateManagerNoRefLockable.h"
#include "timer.h"

namespace X
{
	// Here's where we go for everything UserInterface related.
	//
	// The user interface has a few nice features...
	// The cursor is used as a light source to create a nice bump mapped look to the user interface.
	// The windows of the UI can be semi transparent.
	// Audio playback for various UI events such as button clicks.
	// 
	// Position coordinates and dimensions.
	// Position coordinates and dimensions are all specified in pixels.
	// Everything is offset from the top left corner of the screen, where coordinates of 0, 0 represent the top left corner of the screen.
	// The X coordinate is from the left edge of the screen, starting at 0, increasing as we go across the screen.
	// The Y coordinate is from the top edge of the screen, starting at 0, increasing as we go down the screen.
	// All widget positions should be zero or greater
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
	// progress bars, static images, animated images, frame buffer boxes which render the
	// contents of a framebuffer, draggable images/framebuffers which can be dragged between "draggable dock" widgets, line graphs, expandable menus
	// with support for holding various widgets such as buttons, text and images, for each item in the menu.
	// We also have a taskbar, scrollbars which may be horizontal or vertical.
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
	// We have colour and transparency and normal maps for lighting effects where the mouse can be a light source.
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
		friend class CUIButton;
		friend class CUIScrollbar;
	public:
		SCUIManager();

		// Updates and renders the UI
		void render(void);

		// Returns which window name or container the mouse cursor is over.
		// If the cursor isn't over any windows, or containers, an empty string is returned.
		// Windows are checked first, aka, they are infront of any containers.
		// Both containers and windows have to be visible.
		std::string getMouseIsOverWhichContainer(void);

		// Resets all colours and time based values for widgets and windows
		// This is called from the application manager during initialisation.
		void reset(void);

		// Called from SCApplicationManager::onWindowToggleFullscreen()
		void onToggleFullscreen(void);

		/************************************************************************************************************************************************************/
		/* Container related */
		/************************************************************************************************************************************************************/

		// Adds a new container for use by the UI and returns a pointer to it
		// The new container is initialised to use the default theme.
		// If the named container already exists, a pointer to that container is returned.
		// If bLocked is true, this object is not removed upon calling the various remove methods, unless they are passed true to their bForceRemoveLocked parameter.
		CUIContainer* containerAdd(const std::string& strName, bool bLocked = false);

		// Returns true if the named container exists
		bool containerExists(const std::string& strName) const;

		// Returns a pointer to a container.
		// If the named object doesn't exist, an exception occurs.
		CUIContainer* containerGet(const std::string& strName) const;

		// Returns a pointer to the container at the given index
		// If the index is invalid, an exception occurs.
		CUIContainer* containerGet(size_t index) const;

		// Returns the name of a container at the specified index.
		// If an invalid index is given, an exception occurs.
		std::string containerGetName(size_t index) const;

		// Returns the number of containers
		size_t containerGetNumber(void) const;

		// Removes the named container.
		// If the named object doesn't exist, this silently fails.
		// If the named object is locked, it will not be removed unless bForceRemoveLocked is true.
		void containerRemove(const std::string& strName, bool bForceRemoveLocked = false);

		// Removes all containers.
		// If objects are locked, they will not be removed unless bForceRemoveLocked is true.
		void containerRemoveAll(bool bForceRemoveLocked = false);

		/************************************************************************************************************************************************************/
		/* Theme related */
		/************************************************************************************************************************************************************/

		// Adds a new theme for use by the UI and returns a pointer to it
		// The new theme is initialised to use the default settings and image files.
		// If the named theme already exists, a pointer to that theme is returned.
		// If bLocked is true, this object is not removed upon calling the various remove methods, unless they are passed true to their bForceRemoveLocked parameter.
		CUITheme* themeAdd(const std::string& strName, bool bLocked = false);

		// Returns true if the named theme exists
		bool themeExists(const std::string& strName) const;

		// Returns a pointer to a named theme.
		// If the named object doesn't exist, an exception occurs.
		CUITheme* themeGet(const std::string& strName) const;

		// Returns a pointer to the theme at the given index
		// If the index is invalid, an exception occurs.
		CUITheme* themeGet(size_t index) const;

		// Returns the name of a theme at the specified index.
		// If an invalid index is given, an exception occurs.
		std::string themeGetName(size_t index) const;

		// Returns the number of themes
		size_t themeGetNumber(void) const;

		// Removes the named theme.
		// If the named object doesn't exist, this silently fails.
		// If the named object is locked, it will not be removed unless bForceRemoveLocked is true.
		void themeRemove(const std::string& strName, bool bForceRemoveLocked = false);

		// Removes all themes.
		// If objects are locked, they will not be removed unless bForceRemoveLocked is true.
		void themeRemoveAll(bool bForceRemoveLocked = false);

		// Sets all UI objects to use the named theme.
		// If the passed theme name doesn't exist, an exception occurs
		void themeSetForAll(const std::string& strName);

		/************************************************************************************************************************************************************/
		/* Window related */
		/************************************************************************************************************************************************************/

		// Adds a new window for use by the UI and returns a pointer to it
		// The new window is initialised to use the default theme.
		// If the named window already exists, a pointer to that window is returned.
		// If bLocked is true, this object is not removed upon calling the various remove methods, unless they are passed true to their bForceRemoveLocked parameter.
		CUIWindow* windowAdd(const std::string& strName, bool bLocked = false);

		// Returns true if the named window exists
		bool windowExists(const std::string& strName) const;

		// Returns a pointer to a window.
		// If the named object doesn't exist, an exception occurs.
		CUIWindow* windowGet(const std::string& strName) const;

		// Returns a pointer to the window at the given index
		// If the index is invalid, an exception occurs.
		CUIWindow* windowGet(size_t index) const;

		// Returns the name of a window at the specified index.
		// If an invalid index is given, an exception occurs.
		std::string windowGetName(size_t index) const;

		// Returns the number of windows
		size_t windowGetNumber(void) const;

		// Removes the named window.
		// If the named object doesn't exist, this silently fails.
		// If the named object is locked, it will not be removed unless bForceRemoveLocked is true.
		void windowRemove(const std::string& strName, bool bForceRemoveLocked = false);

		// Removes all windows.
		// If objects are locked, they will not be removed unless bForceRemoveLocked is true.
		void windowRemoveAll(bool bForceRemoveLocked = false);

		// Moves the named window's ZOrder so that it at the top/in front
		// If the named window doesn't exist, this does nothing.
		void windowMoveToFront(const std::string& strName);

		// Returns the name of a window which is currently being moved.
		// Will be empty if no windows are being moved
		std::string windowBeingMoved(void);

		// Sets the name of the window currently being moved.
		void windowBeingMoved(const std::string& strWindowBeingMoved);

		// Sets the named window to be in focus and all others to not be.
		// If the named window doesn't exist, an exception occurs.
		void windowSetFocused(const std::string strWindowName);

		// Sets all windows as unfocused
		void windowSetUnfocusedAll(void);
	private:
		CTimer _mTimer;

		mutable CManagerNoRefLockable<CUIContainer> _mmanContainers;
		mutable CManagerNoRefLockable<CUITheme> _mmanThemes;
		mutable CManagerNoRefLockable<CUIWindow> _mmanWindows;

		// Holds each window name, in their Z order where the front most window
		// is the last one in the list and therefore the last to be rendered.
		std::list<std::string> _mlistWindowZOrder;

		// Called from render to update everything
		void _update(void);

		// Called from _update() to compute if the mouse cursor is over any windows or containers.
		// Stores the name of the window or container the cursor is over in _mstrMouseIsOver
		void _updateMouseIsOver(void);

		// Which window or container the mouse cursor is currently over.
		// May be empty is the mouse cursor isn't over any windows, or containers.
		std::string _mstrMouseIsOver;
		bool _bMouseIsOverAWindow;	// Holds whether when _mstrMouseIsOver.size() > 0, whether the name is a window or container

		// The name of the window which is currently being moved
		std::string _mstrWindowBeingMoved;

		// Add 9 quads to the passed vertex buffer, which represent the 9 grid cell images stored in the passed CUITheme::SImageType.
		void _helperAddWidgetGridGeometry(
			const CVector2f& vPosition,			// The position of the widget's top left corner offset from the container's top left corner of the widget area.
			const CVector2f& vDimensions,		// The total dimensions of the widget
			CUITheme::SImageType imageType,		// The names of the 9 images stored in the theme's atlas texture and is used to obtain the dimensions of each image and it's texture coordinates.
			const CColour& colour,				// The colour used for the vertex colours.
			CUIContainer* pContainer,			// The container which the widget belongs to.
			CResourceVertexBufferCPT2* pVB		// A CResourceVertexBufferCPT2 pointer which the computed vertex positions/texture coordinates will be added to.
		);

		// Helper method to move a colour towards a target colour
		void _helperColourAdjust(CColour& colourToAdjust, const CColour& colourTarget, float fTimeDeltaSecs, float fFadeSpeed);
	};
}