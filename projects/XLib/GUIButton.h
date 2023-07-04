#pragma once
#include "PCH.h"
#include "GUIBaseObject.h"
#include "timer.h"

namespace X
{
	class CGUIButton : public CGUIBaseObject
	{
	public:
		CGUIButton();
		~CGUIButton();

		// Called from CGUIContainer to render this object
		void render(void *pParentContainer, const std::string& strFramebufferToSampleFrom);

		// Called from CGUIContainer to update this object
		void update(void* pParentContainer, bool bParentContainerAcceptingMouseClicks);

		std::string mstrText;	// The text to be rendered on top of this button

		// Returns true if the button was clicked upon
		bool getClicked(void);

		// The tooltip for this object.
		// By default, it is disabled. Use this object to enable and setup the tooltip
		// We've had to set this as a void pointer due to header inclusion restrictions.
		// Type cast this to a CGUITooltip* to use. CGUITooltip* pTT = (CGUITooltip*)pWidget->mpTooltip;
		void* mpTooltip;
	private:
		CTimer _mTimer;
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