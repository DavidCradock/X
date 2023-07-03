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
		~GUISlider();

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

		// The tooltip for this object.
		// By default, it is disabled. Use this object to enable and setup the tooltip
		// We've had to set this as a void pointer due to header inclusion restrictions.
		// Type cast this to a GUITooltip* to use. GUITooltip* pTT = (GUITooltip*)pWidget->mpTooltip;
		void* mpTooltip;
	private:
		Timer _mTimer;
		float _mfSliderPosition;	// 0-1 position of slider
		float _mfTabRatio;			// Multiplied by slider dims(depending upon orientation) to obtain tab dimensions
		GUIColour _mTabColour;		// For fading between over/not over colours of tab
		bool _mbTabBeingMoved;		// Whether the tab is being moved or not

		bool _mbOrientationIsHorizontal;// Computed in update() and used in render(). Slider orientation based on dimensions
		float _mfTabPos[2];				// Computed in update() and used in render(). Tab position
		float _mfTabDims[2];			// Computed in update() and used in render(). Tab dimensions

	};
}