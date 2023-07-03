#pragma once
#include "PCH.h"
#include "GUIBaseObject.h"

namespace X
{
	class GUIProgressBar : public GUIBaseObject
	{
	public:
		GUIProgressBar();
		~GUIProgressBar();

		// Called from GUIContainer to render this object
		void render(void* pParentContainer, const std::string& strFramebufferToSampleFrom);

		// Called from GUIContainer to update this object
		void update(void* pParentContainer, bool bParentContainerAcceptingMouseClicks);

		// Sets progress 0-1 range
		void setProgress(float fPos);

		// Gets progress 0-1 range
		float getProgress(void);

		// The tooltip for this object.
		// By default, it is disabled. Use this object to enable and setup the tooltip
		// We've had to set this as a void pointer due to header inclusion restrictions.
		// Type cast this to a GUITooltip* to use. GUITooltip* pTT = (GUITooltip*)pWidget->mpTooltip;
		void* mpTooltip;
	private:
		float _mfProgress;	// 0-1 position of progress bar
		bool _mbOrientationIsHorizontal;// Computed in update() and used in render(). Orientation based on dimensions
	};
}