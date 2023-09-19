#pragma once
#include "PCH.h"
#include "resourceTexture2DAtlas.h"
#include "resourceVertexBufferCPT2.h"
#include "UITheme.h"

namespace X
{
	class CUIContainer;

	// A scrollbar with a draggable tab.
	// The scrollbar can be either horizontal or vertical.
	// The orientation of the scrollbar is automatically calculated based upon the scrollbar's dimensions.
	// So if the width is greater than the height, it's a horizontal scrollbar, else vertical.
	// The scrollbar has a tab which is draggable via the mouse cursor and has a position value which ranges from 0.0f to 1.0f and can be obtained with a call to getTabPos()
	// If the scrollbar is horizontal and if the tab is to the far left, this will be 0.0f, if far right, 1.0f
	// If the scrollbar is vertical and if the tab is at the bottom, this will be 0.0f, if top, 1.0f
	// The tab also has a "ratio" value which can be set with a call to setTabRatio() and should be between 0.0f and 1.0f
	// This value is multiplied by the width/height of the scrollbar and determines the width/height of the tab.
	// So if the ratio is set to 0.5f and the width of a horizontal scrollbar is 100 pixels, the tab's width will be 100*0.5 = 50 pixels.
	class CUIScrollbar
	{
		friend class CUIContainer;
	public:
		CUIScrollbar(CUIContainer *pContainer);
		~CUIScrollbar();

		// Sets the dimensions of the widget.
		void setDimensions(float fX, float fY);

		// Sets the dimensions of the widget.
		void setDimensions(int iX, int iY);

		// Sets the dimensions of the widget.
		void setDimensions(const CVector2f& vDimensions);

		// Returns the dimensions of the widget.
		CVector2f getDimensions(void) const;

		// Returns the minimum dimensions of this widget based upon the container's currently set theme.
		CVector2f getDimensionsMinimum(void) const;

		// Sets the position of this widget in relation to it's container.
		// Both axis should be at least zero.
		void setPosition(float fX, float fY);

		// Sets the position of this widget in relation to it's container.
		// Both axis should be at least zero.
		void setPosition(int iX, int iY);

		// Sets the position of this widget in relation to it's container.
		// Both axis should be at least zero.
		void setPosition(const CVector2f& vPosition);

		// Returns the position of the widget in relation to it's container.
		CVector2f getPosition(void) const;

		// Sets whether this widget is visible or not.
		void setVisible(bool bVisible);

		// Returns whether this widget is visible or not.
		bool getVisible(void) const;

		// Render this widget
		void renderBG(CResourceVertexBufferCPT2* pVB, bool bUseWidgetScrollbarOffset = true);

		// Update this object
		void update(float fTimeDeltaSec, bool bUseWidgetScrollbarOffset = true);

		// Resets all colours and time based values for the widget
		void reset(void);

		/******************************************************************* Widget specific *******************************************************************/

		// Sets tab position, clamped to 0-1 range
		void setTabPos(float fPos);

		// Gets tab position 0-1 range
		float getTabPos(void) const;

		// Set tab ratio.
		// This is multiplied by the scrollbar dimensions(depending upon orientation) to obtain tab dimensions
		void setTabRatio(float fRatio);

		// Returns currently set tab ratio
		// This is multiplied by the scrollbar dimensions(depending upon orientation) to obtain tab dimensions
		float getTabRatio(void) const;

		// Returns scrollbar's orientation (Which is based upon it's dimensions)
		bool getOrientationHorizontal(void) const;

		// Return computed tab dimensions
		CVector2f getTabDims(void) const;

		// Set the C function which'll be called while the scrollbar's tab is being moved.
		// To use, create a function with the following signature...
		// void MyFunction(void);
		// Then set it to be called with the following syntax...
		// pSomeUIScrollbar->setFunctionOnTabBeingMoved(MyFunction);
		// Pass 0 or NULL here to remove the function
		void setFunctionOnTabBeingMoved(void (*function)(void));

		// Set the C function which'll be called once the scrollbar's tab has finished being moved.
		// To use, create a function with the following signature...
		// void MyFunction(void);
		// Then set it to be called with the following syntax...
		// pSomeUIScrollbar->setFunctionOnTabMoved(MyFunction);
		// Pass 0 or NULL here to remove the function
		void setFunctionOnTabMoved(void (*function)(void));

		// Returns true if the tab is being moved
		bool getTabBeingMoved(void) const;
	private:
		// Common amoung widgets
		CVector2f _mvDimensions;			// Dimensions of the widget
		CVector2f _mvPosition;				// Position of the widget in relation to it's container.
		bool _mbVisible;					// Whether this widget is visible or not.
		CUIContainer* _mpContainer;			// The container this widget belongs to. Set in constructor

		// Widget specific
		float _mfTabPosition;				// 0-1 position of tab
		float _mfTabRatio;					// Multiplied by scrollbar dims(depending upon orientation) to obtain tab dimensions
		bool _mbTabBeingMoved;				// Whether the tab is being moved or not
		CVector2f _mvTabPos;				// Computed in update() and used in render(). Tab position
		CVector2f _mvTabDims;				// Computed in update() and used in render(). Tab dimensions
		CColour _mTabColour;				// For fading between over/not over colours of tab

		// Function pointer which can be set with setFunctionOnTabMoved() which gets called when the scrollbar's tab has moved
		void (*_mfuncOnTabBeingMoved)(void);

		// Function pointer which can be set with setFunctionOnTabMoved() which gets called once the scrollbar's tab has finished being moved
		void (*_mfuncOnTabMoved)(void);
	};
}