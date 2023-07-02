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

		// Called from GUIContainer to render this object
		void render(void* pParentContainer, const std::string& strFramebufferToSampleFrom);

		// Called from GUIContainer to update this object
		void update(void* pParentContainer, bool bParentContainerAcceptingMouseClicks);

		// Returns true if the button was clicked upon
		bool getClicked(void);

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