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
	public:
		GUIImageAnimated();

		// Called from GUIContainer to render this object
		void render(void* pParentContainer, const std::string& strFramebufferToSampleFrom);

		// Called from GUIContainer to update this object
		void update(void* pParentContainer, bool bParentContainerAcceptingMouseClicks);

		// Sets the playback rate of the animation
		void setFramesPerSecond(float fFramesPerSecond);
		GUIColour mColour;
	private:
		Timer _mTimer;
		float _mfFramesPerSecond;
		float _mfCurrentFrame;
		std::string _mstrResourceTexture2DAnimationName;	// Name of the ResourceTexture2DAnimation stored in ResourceManager

	};
}