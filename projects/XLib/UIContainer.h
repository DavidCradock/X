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
	// A container is a CUIBaseWidget which stores position, dimensions and visibility.
	class CUIContainer
	{
		friend class SCUIManager;
	public:
		CUIContainer();

		~CUIContainer();

		// Sets the position of this container.
		// This is the top left position of the widget area, where widgets can be placed.
		void setPosition(const CVector2f& vPosition);

		// Returns the position of this container.
		// This is the top left position of the widget area, where widgets can be placed.
		CVector2f getPosition(void) const;

		// Sets the dimensions of this container.
		// This is the dimensions of the area available for widgets to be added to.
		// The actual container's dimensions are dependant upon the theme it's currently using.
		// The actual dimensions are the dimensions given here PLUS the width/height of the
		// horizontal and vertical scrollbars which are added to the bottom and right edge of the
		// container for scrolling of widgets into view if their positions are greater than the
		// dimensions given here.
		void setDimensions(const CVector2f& vDimensions);

		// Returns the dimensions of this container.
		// This is the dimensions of the area available for widgets to be added to, not including
		// the additional dimensions added for the container's two scroll bars.
		CVector2f getDimensions(void) const;

		// Sets whether this container is visible or not
		void setVisible(bool bVisible);

		// Returns whether this container is visible or not
		bool getVisible(void) const;

		// Updates this container
		void update(float fTimeDeltaSec, bool bIsWindow = false);

		// Render this container's widgets
		void render(bool bIsWindow = false);

		// Render this container's widget's tooltips
		void renderTooltips(void);

		// Get currently set theme name used by this container
		std::string themeNameGet(void) const;

		// Returns a pointer to the theme this container uses
		CUITheme* themeGet(void) const;

		// Set theme this container uses for it's widgets and scrollbars
		void themeNameSet(const std::string& strThemeToUse);

		// Removes all widgets from this container
		void widgetRemoveAll(void);

		// Returns the name of this container given to it when it was added via SCUIManager::containerAdd() or SCUIManager::windowAdd()
		std::string getName(void);

		/************************************************************************************************************************************************************/
		/* Buttons */
		/************************************************************************************************************************************************************/

		// Add button to this container and return a pointer to it
		// If the name already exists, an exception occurs
		CUIButton* buttonAdd(const std::string& strName, float fPosX, float fPosY, float fWidth, float fHeight);

		// Returns a pointer to the named object
		// If the object doesn't exist, an exception occurs
		CUIButton* buttonbarGet(const std::string& strName);

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
		CVector2f _mvDimensions;	// Dimensions of the container
		CVector2f _mvPosition;		// Position of the container.
		bool _mbVisible;			// Whether this container is shown or not

		std::string _mstrThemename;	// Theme name used by this container ("default" upon construction)
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

		// Updates the two scrollbar positions and dimensions and such.
		// Called if the theme or dimensions change.
		void _computeScrollbars(void);
	};
}