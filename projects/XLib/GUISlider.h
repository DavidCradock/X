#pragma once
#include "PCH.h"
#include "GUIBaseObject.h"
#include "timer.h"
#include "colour.h"
#include "vector2f.h"

namespace X
{
	class CGUITooltip;

	class CGUISlider : public CGUIBaseObject
	{
	public:
		friend class CGUIContainer;
		CGUISlider();
		~CGUISlider();

		// Called from CGUIContainer to render this object
		void render(void* pParentContainer, const std::string& strFramebufferToSampleFrom);

		// Called from CGUIContainer to update this object
		void update(void* pParentContainer, bool bParentContainerAcceptingMouseClicks);

		// Sets tab position 0-1 range
		void setTabPos(float fPos);

		// Gets tab position 0-1 range
		float getTabPos(void) const;

		// Set tab ratio.
		// This is multiplied by the slider dimensions(depending upon orientation) to obtain tab dimensions
		void setTabRatio(float fRatio);

		// Returns currently set tab ratio
		// This is multiplied by the slider dimensions(depending upon orientation) to obtain tab dimensions
		float getTabRatio(void) const;

		// Returns slider's orientation (Which is based upon it's dimensions)
		bool getOrientationHorizontal(void) const;

		// Return computed tab dimensions
		CVector2f getTabDims(void) const;

		// The tooltip for this object.
		// By default, it is disabled. Use this object to enable and setup the tooltip
		CGUITooltip* mpTooltip;
	private:
		CTimer _mTimer;
		float _mfSliderPosition;	// 0-1 position of slider
		float _mfTabRatio;			// Multiplied by slider dims(depending upon orientation) to obtain tab dimensions
		CColour _mTabColour;		// For fading between over/not over colours of tab
		bool _mbTabBeingMoved;		// Whether the tab is being moved or not

		bool _mbOrientationIsHorizontal;// Computed in update() and used in render(). Slider orientation based on dimensions
		float _mfTabPos[2];				// Computed in update() and used in render(). Tab position
		float _mfTabDims[2];			// Computed in update() and used in render(). Tab dimensions

	};
}