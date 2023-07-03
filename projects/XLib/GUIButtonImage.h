#pragma once
#include "PCH.h"
#include "GUIBaseObject.h"
#include "timer.h"

namespace X
{
	class GUIButtonImage : public GUIBaseObject
	{
	public:
		friend class GUIContainer;
		GUIButtonImage();
		~GUIButtonImage();

		// Called from GUIContainer to render this object
		void render(void* pParentContainer, const std::string& strFramebufferToSampleFrom);

		// Called from GUIContainer to update this object
		void update(void* pParentContainer, bool bParentContainerAcceptingMouseClicks);

		// Returns true if the button was clicked upon
		bool getClicked(void);

		// The tooltip for this object.
		// By default, it is disabled. Use this object to enable and setup the tooltip
		// We've had to set this as a void pointer due to header inclusion restrictions.
		// Type cast this to a GUITooltip* to use. GUITooltip* pTT = (GUITooltip*)pWidget->mpTooltip;
		void* mpTooltip;
	private:
		std::string _mstrTextureDown;	// The down state image to be rendered on top of this button
		std::string _mstrTextureOver;	// The over state image to be rendered on top of this button
		std::string _mstrTextureUp;		// The up state image to be rendered on top of this button
		Timer _mTimer;
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