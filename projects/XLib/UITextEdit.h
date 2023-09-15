#pragma once
#include "PCH.h"
#include "resourceTexture2DAtlas.h"
#include "resourceVertexBufferCPT2.h"
#include "UITheme.h"
#include "timer.h"

namespace X
{
	class CUIContainer;
	class CUIText;

	// A clickable text edit widget where we can edit the text inside of it
	class CUITextEdit
	{
		friend class CUIContainer;
	public:
		CUITextEdit(CUIContainer* pContainer, const std::string& strWidgetName);
		~CUITextEdit();

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
		void renderBG(CResourceVertexBufferCPT2* pVB);

		// Render this widget's non-background items
		void renderNonBG(void);
	
		// Update this widget
		void update(float fTimeDeltaSec);

		// Resets all colours and time based values for the widget
		void reset(void);

		/******************************************************************* Widget specific *******************************************************************/

		// Sets the text in this object
		void setText(const std::string& strText);

		// Returns the text stored in this object
		std::string getText(void) const;

		// Maximum number of characters the edit box is allowed
		void setMaxChars(unsigned int iMaxChars);

		// Only allow input of numbers?
		void setIntegerInputOnly(bool bAllowNumbersOnly);

		// Returns true if the text edit was active and is now in-active via enter key
		bool getEnterPressed(void) const;

		// Set the C function which'll be called once the text edit box is active and enter key is pressed
		// To use, create a function with the following syntax...
		// void MyFunc(const std::string &strText)
		// { 
		// // do stuff here
		// // text contains the text within the text edit box
		// }
		// CGUITextEdit *p = x->pGUI->getTextEdit("MyTextEdit");
		// p->setOnEnter(MyFunc);
		void setOnEnter(void (*func)(const std::string& text));

		// Returns true if the text has changed
		bool getHasTextChanged(void);
	private:
		// Common amoung widgets
		CVector2f _mvDimensions;			// Dimensions of the widget
		CVector2f _mvPosition;				// Position of the widget in relation to it's container.
		bool _mbVisible;					// Whether this widget is visible or not.
		CUIContainer* _mpContainer;			// The container this widget belongs to. Set in constructor

		// Widget specific
		CTimer _mTimer;
		enum state
		{
			active,
			inactive
		};
		std::string _mstrText;			// The text string of this object
		state _mState;
		CColour _mTextColour;			// Current colour of text based on state
		float _mfAddFlashingCursor;		// value is incremented and if above 1, adds an additional character to the text edit when active
		unsigned int _muiMaxChars;		// Maximum number of characters this text edit can hold
		bool _mbIntegerInputOnly;		// Only allow number input?
		bool _mbWasActiveEnterPressed;	// Is only true for one program loop

		// Checks to see if the text contains a number and if not, resets it to zero
		void _checkIsNumber(bool bResetToZero);

		// Function pointer which can be set with setOnEnter() which gets called when the text edit box was active and the enter key has been pressed.
		void (*_mfuncOnEnterPressed)(const std::string& strText);

		std::string _mstrTextPrev;			// Previous text value, for computing _mbTextHasChanged
		bool _mbTextHasChanged;
	};
}