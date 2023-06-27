#pragma once
#include "PCH.h"
#include "GUIBaseObject.h"
#include "timer.h"
#include "GUIColour.h"

namespace X
{
	class GUISlider : public GUIBaseObject
	{
	public:
		friend class GUIContainer;
		GUISlider();

		// Called from GUIContainer to render this object
		void render(void* pParentContainer, const std::string& strFramebufferToSampleFrom);

		// Called from GUIContainer to update this object
		void update(void* pParentContainer, bool bParentContainerAcceptingMouseClicks);

		// Sets tab position 0-1 range
		void setTabPos(float fPos);

		// Gets tab position 0-1 range
		float getTabPos(void);

		// Set tab ratio.
		// This is multiplied by the slider dimensions(depending upon orientation) to obtain tab dimensions
		void setTabRatio(float fRatio);
	private:
		Timer _mTimer;
		float _mfSliderPosition;	// 0-1 position of slider
		float _mfTabRatio;			// Multiplied by slider dims(depending upon orientation) to obtain tab dimensions
		GUIColour _mTabColour;		// For fading between over/not over colours of tab
		bool _mbTabBeingMoved;		// Whether the tab is being moved or not

		float _mfTabPos[2];			// Computed in update() and used in render(). Tab position
		float _mfTabDims[2];		// Computed in update() and used in render(). Tab dimensions
	};
}