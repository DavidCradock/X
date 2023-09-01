#pragma once
#include "PCH.h"
#include "resourceTexture2DAtlas.h"
#include "resourceVertexBufferCPT2.h"
#include "UITheme.h"

namespace X
{
	class CUIContainer;

	class CUIButton
	{
	public:
		CUIButton();
		~CUIButton();

		// Sets the dimensions of the widget.
		void setDimensions(float fX, float fY);

		// Sets the dimensions of the widget.
		void setDimensions(const CVector2f& vDimensions);

		// Returns the dimensions of the widget.
		CVector2f getDimensions(void) const;

		// Sets the position of this widget in relation to it's container.
		void setPosition(float fX, float fY);

		// Sets the position of this widget in relation to it's container.
		void setPosition(const CVector2f& vPosition);

		// Returns the position of the widget in relation to it's container.
		CVector2f getPosition(void) const;

		// Sets whether this widget is visible or not.
		void setVisible(bool bVisible);

		// Returns whether this widget is visible or not.
		bool getVisible(void) const;

		// Render this object's non-font stuff
		void render(CUIContainer* pContainer, CResourceVertexBufferCPT2* pVB);

		// Render this object's font stuff
		void renderFonts(CUIContainer* pContainer);
	
		// Update this object
		void update(float fTimeDeltaSec, CUIContainer* pContainer);

		// Resets all colours and time based values for the widget
		void reset(CUIContainer* pContainer);

		// Sets the text string to be rendered over the top of the button
		void setText(const std::string& strText);

		// Returns the text string that's used to render the text over this button.
		std::string getText(void) const;

		// Returns whether this button has been clicked upon or not.
		// This is OK for a quick and dirty approach to checking button clicks, but if we have
		// many buttons, they'll be a lot of if then statements in the calling code and things start
		// to get inefficient.
		// Don't forget to check if the widget's container is visible, otherwise there's no point checking
		// to see if the widget has changed state as it will not have.
		// Instead of using this, use observers or function pointers for optimal performance.
		bool getClicked(void) const;

	private:
		CVector2f _mvDimensions;			// Dimensions of the widget
		CVector2f _mvPosition;				// Position of the widget in relation to it's container.
		bool _mbVisible;					// Whether this widget is visible or not.
		std::string _mstrText;				// Text string to be rendered
		CColour _mColourBG;					// Current BG colour (fading between up/over/down states)
		CColour _mColourText;				// Current text colour (fading between up/over/down states)
		
		// The three states of a button
		enum state
		{
			up,		// Mouse is not over the button
			over,	// Mouse is over the button
			down	// Mouse is over the button and left mouse button is pressed
		};
		state _mState;				// The current state of the button
		bool _mbClicked;			// Holds whether the button has been clicked upon.

		// Helper method to move a colour towards a target colour
		void _helperColourAdjust(CColour& colourAdjust, const CColour& colourTarget, float fTimeDeltaSecs, const CUITheme::SSettings* pSettings);
	};
}