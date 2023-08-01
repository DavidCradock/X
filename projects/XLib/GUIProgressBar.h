#pragma once
#include "PCH.h"
#include "GUIBaseObject.h"

namespace X
{
	class CGUITooltip;

	class CGUIProgressBar : public CGUIBaseObject
	{
	public:
		CGUIProgressBar();
		~CGUIProgressBar();

		// Called from CGUIContainer to render this object
		void render(void* pParentContainer, const std::string& strFramebufferToSampleFrom);

		// Called from CGUIContainer to update this object
		void update(void* pParentContainer, bool bParentContainerAcceptingMouseClicks);

		// Sets progress 0-1 range
		void setProgress(float fPos);

		// Gets progress 0-1 range
		float getProgress(void) const;

		// The tooltip for this object.
		// By default, it is disabled. Use this object to enable and setup the tooltip
		CGUITooltip* mpTooltip;

		// Sets the alpha, multiplied by the theme's alpha of the progress bar
		// The default is 1.0f
		void setAlpha(float fAlpha);
	private:
		float _mfProgress;	// 0-1 position of progress bar
		bool _mbOrientationIsHorizontal;// Computed in update() and used in render(). Orientation based on dimensions

		float _mfAlpha;	// Alpha value to multiply by the theme's alpha
	};
}