#pragma once
#include "PCH.h"
#include "GUIBaseObject.h"
#include "GUIColour.h"
#include "timer.h"

namespace X
{
	class GUIImageAnimated : public GUIBaseObject
	{
		friend class GUIContainer;
		friend class GUITooltip;
	public:
		GUIImageAnimated();
		~GUIImageAnimated();

		// Called from GUIContainer to render this object
		void render(void* pParentContainer, const std::string& strFramebufferToSampleFrom);

		// Called from GUIContainer to update this object
		void update(void* pParentContainer, bool bParentContainerAcceptingMouseClicks);

		// Sets the playback rate of the animation
		void setFramesPerSecond(float fFramesPerSecond);
		GUIColour mColour;

		// The tooltip for this object.
		// By default, it is disabled. Use this object to enable and setup the tooltip
		// We've had to set this as a void pointer due to header inclusion restrictions.
		// Type cast this to a GUITooltip* to use. GUITooltip* pTT = (GUITooltip*)pWidget->mpTooltip;
		void* mpTooltip;
	private:
		Timer _mTimer;
		float _mfFramesPerSecond;
		float _mfCurrentFrame;
		std::string _mstrResourceTexture2DAnimationName;	// Name of the ResourceTexture2DAnimation stored in ResourceManager

	};
}