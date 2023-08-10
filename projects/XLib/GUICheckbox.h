#pragma once
#include "PCH.h"
#include "GUIBaseObject.h"
#include "timer.h"

namespace X
{
	class CGUITooltip;

	class CGUICheckbox : public CGUIBaseObject
	{
	public:
		CGUICheckbox();
		~CGUICheckbox();

		// Called from CGUIContainer to render this object
		void render(void *pParentContainer, const std::string& strFramebufferToSampleFrom);

		// Called from CGUIContainer to update this object
		void update(void* pParentContainer, bool bParentContainerAcceptingMouseClicks);

		// Returns true if the checkbox has changed states, by being clicked upon
		bool getClicked(void) const;

		// Returns the state of the checkbox, ON or OFF
		bool getState(void) const;

		// Sets the state of the checkbox, ON or OFF
		void setState(bool bOn);

		// The tooltip for this object.
		// By default, it is disabled. Use this object to enable and setup the tooltip
		CGUITooltip* mpTooltip;
	private:
		CTimer _mTimer;
		float _mfFade;		// Current fade between the on and off images

		bool _mbClicked;
		bool _mbStateON;	// The current state of the checkbox ON or OFF
	};
}