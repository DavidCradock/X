#pragma once
#include "PCH.h"
#include "GUIBaseObject.h"
#include "timer.h"

namespace X
{
	class CGUITooltip;

	class CGUIButton : public CGUIBaseObject
	{
		friend class CGUIContainer;
	public:
		CGUIButton();
		~CGUIButton();

		// Called from CGUIContainer to render this object
		void render(void *pParentContainer, const std::string& strFramebufferToSampleFrom);

		// Called from CGUIContainer to update this object
		void update(void* pParentContainer, bool bParentContainerAcceptingMouseClicks);

		std::string mstrText;	// The text to be rendered on top of this button

		// Returns true if the button was clicked upon
		bool getClicked(void) const;

		// The tooltip for this object.
		// By default, it is disabled. Use this object to enable and setup the tooltip
		CGUITooltip* mpTooltip;
	private:
		CTimer _mTimer;
		float _mfCurrentTextCol[4];	// Current text colour (fading between up/over/down states)
		float _mfCurrentBGCol[4];	// Current BG colour (fading between up/over/down states)
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