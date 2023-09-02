#pragma once
#include "PCH.h"
#include "UIButton.h"
#include "UIScrollBar.h"
#include "UITheme.h"
#include "templateManagerNoRef.h"

namespace X
{
	// A container object for the user interface.
	// A container object allows us to add all the various UI widgets to it.
	class CUIContainer
	{
		friend class SCUIManager;
		friend class CUIButton;
		friend class CUIScrollbar;
	public:
		CUIContainer();

		~CUIContainer();

		// This sets _mvContainersWidgetAreaDimensions which is the dimensions of the container's widget area.
		// The actual container's dimensions onscreen will be larger than this if it is a window,
		// as a window has borders and the two scrollbars.
		void setDimensions(float fX, float fY);

		// This sets _mvContainersWidgetAreaDimensions which is the dimensions of the container's widget area.
		// The actual container's dimensions onscreen will be larger than this if it is a window,
		// as a window has borders and the two scrollbars.
		void setDimensions(const CVector2f& vDimensions);

		// Returns the dimensions of this container.
		// This is the dimensions of the area available for widgets to be added to.
		// If widgets are placed so that they are outside of this area, the two scrollbars are shown which allows scrolling of the contents.
		CVector2f getDimensions(void) const;

		// Sets the position of this container.
		// If the container is not a window, then this is the top left position of the widget area, where widgets can be placed.
		// If the container is a window, then this is the position of the window's top left corner of it's borders.
		void setPosition(float fX, float fY);

		// Sets the position of this container.
		// If the container is not a window, then this is the top left position of the widget area, where widgets can be placed.
		// If the container is a window, then this is the position of the window's top left corner of it's borders.
		void setPosition(const CVector2f& vPosition);

		// Returns the position of this container.
		CVector2f getPosition(void) const;

		// Sets whether this container is visible or not
		void setVisible(bool bVisible);

		// Returns whether this container is visible or not
		bool getVisible(void) const;

		// Updates this container
		void update(float fTimeDeltaSec);

		// Render this container's widgets
		void render(void);

		// Render this container's widget's tooltips
		void renderTooltips(void);

		// Resets all colours and time based values for widgets
		void reset(void);

		// Get currently set theme name used by this container
		std::string themeNameGet(void) const;

		// Returns a pointer to the theme this container uses
		CUITheme* themeGet(void) const;

		// Returns a pointer to the theme's settings this container uses.
		const CUITheme::SSettings* themeGetSettings(void) const;

		// Set theme this container uses for it's widgets and scrollbars
		void themeNameSet(const std::string& strThemeToUse);

		// Removes all widgets from this container
		void widgetRemoveAll(void);

		// Returns the name of this container given to it when it was added via SCUIManager::containerAdd() or SCUIManager::windowAdd()
		std::string getName(void);

		// Returns the top left position of the widget area for this container.
		// Used when rendering or updating a widget to determine the actual top left position of the container's widget area.
		// If this container is just a container, then this will be equal to the container's top left position, otherwise,
		// it'll be that + the dimensions of the top left cornder border of the window using this container's currently set theme.
		CVector2f getWidgetAreaTLCornerPosition(void) const;

		/************************************************************************************************************************************************************/
		/* Buttons */
		/************************************************************************************************************************************************************/

		// Add button to this container and return a pointer to it
		// If the name already exists, an exception occurs
		CUIButton* buttonAdd(const std::string& strName, float fPosX, float fPosY, float fWidth, float fHeight);

		// Returns a pointer to the named object
		// If the object doesn't exist, an exception occurs
		CUIButton* buttonGet(const std::string& strName);

		// Removes the named object from the container
		// If the named object doesn't exist, this silently fails
		void buttonRemove(const std::string& strName);

		// Removes all buttons from this container
		void buttonRemoveAll(void);

		/************************************************************************************************************************************************************/
		/* Scrollbars */
		/************************************************************************************************************************************************************/

		// Add scrollbar to this container and return a pointer to it
		// If the name already exists, an exception occurs
		// If the width is greater than height, it is set as a horizontal scrollbar, else vertical.
		// fTabRatio is a value which is multiplied by the width/height(Depending on orientation) of the scrollbar's dims, to obtain tab dimensions
		CUIScrollbar* scrollbarAdd(const std::string& strName, float fPosX, float fPosY, float fWidth, float fHeight, float fTabRatio = 0.05f);

		// Returns a pointer to the named object
		// If the object doesn't exist, an exception occurs
		CUIScrollbar* scrollbarGet(const std::string& strName);

		// Removes the named object from the container
		// If the named object doesn't exist, this silently fails
		void scrollbarRemove(const std::string& strName);

		// Removes all scrollbars from this container
		void scrollbarRemoveAll(void);
	protected:
		// Dimensions of the container's widget area, set by setDimensions();
		CVector2f _mvContainersWidgetAreaDimensions;
		
		// Position of the container.
		CVector2f _mvPosition;

		// Whether this container is shown or not
		bool _mbVisible;

		// Set via CUIContainer or CUIWindow constructors.
		// Used by _computeScrollbars and other areas to
		// determine whether the container is a window or not.
		bool _mbContainerIsWindow;

		// Theme name used by this container ("default" upon construction)
		std::string _mstrThemename;
		CManagerNoRef<CUIButton> _mmanButtons;			// Manager holding each CUIButton widget
		CManagerNoRef<CUIScrollbar> _mmanScrollbars;	// Manager holding each CUIScrollbar widget

		// Name of this container, set from SCUIManager during call to containerAdd() or windowAdd()
		std::string _mstrName;

		// The horizontal scrollbar used for scrolling through all the widgets if their positions exceed
		// the dimensions of the container.
		CUIScrollbar _mScrollbarH;

		// The vertical scrollbar used for scrolling through all the widgets if their positions exceed
		// the dimensions of the container.
		CUIScrollbar _mScrollbarV;

		// Updates the two scrollbar positions and dimensions
		// Called if the theme or dimensions change.
		void _computeScrollbars(void);
	};
}