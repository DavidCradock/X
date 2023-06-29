#pragma once
#include "PCH.h"
#include "GUIBaseObject.h"

namespace X
{
	class GUIProgressBar : public GUIBaseObject
	{
	public:
		GUIProgressBar();

		// Called from GUIContainer to render this object
		void render(void* pParentContainer, const std::string& strFramebufferToSampleFrom);

		// Called from GUIContainer to update this object
		void update(void* pParentContainer, bool bParentContainerAcceptingMouseClicks);

		// Sets progress 0-1 range
		void setProgress(float fPos);

		// Gets progress 0-1 range
		float getProgress(void);

	private:
		float _mfProgress;	// 0-1 position of progress bar
		bool _mbOrientationIsHorizontal;// Computed in update() and used in render(). Orientation based on dimensions
	};
}