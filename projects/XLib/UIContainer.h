#pragma once
#include "PCH.h"
#include "UIBaseWidget.h"
#include "UIScrollBar.h"
#include "UITheme.h"
#include "templateManagerNoRef.h"

namespace X
{
	// A container object for the user interface.
	// A container object allows us to add all the various UI widgets to it.
	// A container is a CUIBaseWidget which stores position, dimensions and visibility.
	class CUIContainer : public CUIBaseWidget
	{
		friend class SCUIManager;
	public:
		CUIContainer();

		~CUIContainer();

		// Updates this container
		void update(float fTimeDeltaSec, bool bIsWindow = false);

		// Render this container and each of it's objects
		void render(bool bIsWindow = false);

		// Render this container's object's tooltips
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
		std::string _mstrThemename;	// Theme name used by this container ("default" upon construction)
		CManagerNoRef<CUIScrollbar> _mmanWidgetScrollbars;		// Manager holding each CUIScrollbar widget

		// Name of this container, set from SCUIManager during call to containerAdd() or windowAdd()
		std::string _mstrName;
	};
}