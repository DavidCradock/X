#pragma once
#include "PCH.h"
#include "GUIBaseObject.h"
#include "timer.h"

namespace X
{
	class CGUITooltip;

	class CGUIButtonImage : public CGUIBaseObject
	{
	public:
		friend class CGUIContainer;
		CGUIButtonImage();
		~CGUIButtonImage();

		// Called from CGUIContainer to render this object
		void render(void* pParentContainer, const std::string& strFramebufferToSampleFrom);

		// Called from CGUIContainer to update this object
		void update(void* pParentContainer, bool bParentContainerAcceptingMouseClicks);

		// Returns true if the button was clicked upon
		bool getClicked(void) const;

		// The tooltip for this object.
		// By default, it is disabled. Use this object to enable and setup the tooltip
		CGUITooltip* mpTooltip;
	private:
		std::string _mstrTextureDown;	// The down state image to be rendered on top of this button
		std::string _mstrTextureOver;	// The over state image to be rendered on top of this button
		std::string _mstrTextureUp;		// The up state image to be rendered on top of this button
		CTimer _mTimer;
		float _mfCurrentImageCol[4];	// Current image colour (fading between up/over/down states)
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