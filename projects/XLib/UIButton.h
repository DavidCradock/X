#pragma once
#include "PCH.h"
#include "resourceTexture2DAtlas.h"
#include "resourceVertexBufferCPT2.h"
#include "UITheme.h"

namespace X
{
	class CUIContainer;
	class CUITooltip;

	// A clickable button with text rendered on top of it.
	class CUIButton
	{
		friend class CUIContainer;
	public:
		CUIButton(CUIContainer* pContainer);
		~CUIButton();

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

		// Render this widget's background grid of 9 quads
		void renderBG(CResourceVertexBufferCPT2* pVB);

		// Render this widget's non-background items
		void renderNonBG(void);
	
		// Render this widget's tooltip
		void renderTooltip(void);

		// Update this widget and it's tooltip
		void update(float fTimeDeltaSec);

		// Resets all colours and time based values for the widget
		void reset(void);

		// Sets the text string to be rendered over the top of the button
		void setText(const std::string& strText);

		// Returns the text string that's used to render the text over this button.
		std::string getText(void) const;

		// Returns whether this button has been clicked upon or not.
		// This is OK for a quick and dirty approach to checking button clicks, but if we have
		// many buttons, there will be a lot of if then statements in the calling code and things start
		// to get inefficient.
		// If you are using this method, don't forget to check if the widget's container is visible, otherwise
		// there's no point checking to see if the widget has changed state as it will not have.
		// Instead of using this, use observers or function pointers for optimal performance.
		bool getClicked(void) const;

		// Set the C function which'll be called once the button has been clicked upon.
		// To use, create a function with the following signature...
		// void MyFunction(void);
		// Then set it to be called with the following syntax...
		// pSomeButton->setFunctionOnClicked(MyFunction);
		// Pass 0 or NULL here to remove the function
		void setFunctionOnClicked(void (*function)(void));

		// Tooltip object for this widget.
		// Access and use this object to setup and enable the tooltip.
		CUITooltip* pTooltip;
	private:
		// Common amoung widgets
		CVector2f _mvDimensions;			// Dimensions of the widget
		CVector2f _mvPosition;				// Position of the widget in relation to it's container.
		bool _mbVisible;					// Whether this widget is visible or not.
		CUIContainer* _mpContainer;			// The container this widget belongs to. Set in constructor

		// Widget specific
		std::string _mstrText;				// Text string to be rendered
		CColour _mColourBG;					// Current BG colour (fading between up/over/down states)
		CColour _mColourText;				// Current text colour (fading between up/over/down states)
		enum state	// The three states of a button
		{
			up,		// Mouse is not over the button
			over,	// Mouse is over the button
			down	// Mouse is over the button and left mouse button is pressed
		};
		state _mState;				// The current state of the button
		bool _mbClicked;			// Holds whether the button has been clicked upon.

		// Function pointer which can be set with setFunctionOnClicked() which gets called when the button gets clicked upon.
		void (*_mfuncOnClicked)(void);
	};
}