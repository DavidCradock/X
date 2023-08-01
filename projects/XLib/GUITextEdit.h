#pragma once
#include "PCH.h"
#include "GUIBaseObject.h"
#include "timer.h"
#include "colour.h"

namespace X
{
	class CGUITooltip;

	class CGUITextEdit : public CGUIBaseObject
	{
	public:
		CGUITextEdit();
		~CGUITextEdit();

		// Called from CGUIContainer to render this object
		void render(void* pParentContainer, const std::string& strFramebufferToSampleFrom);

		// Called from CGUIContainer to update this object
		void update(void* pParentContainer, bool bParentContainerAcceptingMouseClicks);

		std::string mstrText;	// The text to be rendered on top of this button

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
		// CGUITextEdit *p = SCGUIManager::getPointer()->getTextEdit("MyTextEdit");
		// p->setOnEnter(MyFunc);
		void setOnEnter(void (*func)(const std::string& text));

		// The tooltip for this object.
		// By default, it is disabled. Use this object to enable and setup the tooltip
		CGUITooltip* mpTooltip;
	private:
		CTimer _mTimer;
		enum state
		{
			active,
			inactive
		};
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

	};
}