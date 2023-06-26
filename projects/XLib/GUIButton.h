#pragma once
#include "PCH.h"
#include "GUIBaseObject.h"
#include "timer.h"

namespace X
{
	class GUIButton : public GUIBaseObject
	{
	public:
		GUIButton();

		// Called from GUIContainer to render this object
		void render(void *pParentContainer, const std::string& strFramebufferToSampleFrom);

		// Called from GUIContainer to update this object
		void update(void* pParentContainer, bool bParentContainerAcceptingMouseClicks);

		std::string mstrText;	// The text to be rendered on top of this button

		// Returns true if the button was clicked upon
		bool getClicked(void);
	private:
		Timer _mTimer;
		float _mfCurrentTextCol[4];	// Current text colour (fading between up/over/down states)
		enum state
		{
			up,
			over,
			down
		};
		state _mState;
		state _mStatePrevious;
		bool _mbClicked;
	};
}