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

		// Sets the text string to be rendered over the top of the button
		void setText(const std::string& strText);

		// Returns the text string that's used to render the text over this button.
		std::string getText(void) const;

		// Returns whether this button has been clicked upon or not.
		// This is OK for a quick and dirty approach to checking button clicks, but if we have
		// many buttons, they'll be a lot of if then statements in the calling code and things start
		// to get inefficient. Instead of using this, use observers please.
		bool getClicked(void) const;

		// Sets a method of a class object to call when the button is clicked.
		// Example usage:
		// 
		// Have a class which has a method which will be called when the button is clicked upon...
		//		class SomeClass
		//		{
		//			public:
		//				void buttonHasBeenClicked(void);
		//		}
		// Create an object of that class...
		//		SomeClass someClass;
		// 
		// Create a user interface container to contain the button and create the button...
		//		SCUIManager* pUI = SCUIManager::getPointer();
		//		CUIContainer* pContainer = pUI->containerAdd("A_Container");
		//		CUIButton* pButton = pContainer->buttonAdd("TheButton");
		// 
		// Now finally set the class object's method to be called when the button is clicked upon...
		//		pButton->setCallOnClicked(someClass.*buttonHasBeenClicked);
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