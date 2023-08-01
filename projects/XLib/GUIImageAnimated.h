#pragma once
#include "PCH.h"
#include "GUIBaseObject.h"
#include "colour.h"
#include "timer.h"

namespace X
{
	class CGUITooltip;

	class CGUIImageAnimated : public CGUIBaseObject
	{
		friend class CGUIContainer;
		friend class CGUITooltip;
	public:
		CGUIImageAnimated();
		~CGUIImageAnimated();

		// Called from CGUIContainer to render this object
		void render(void* pParentContainer, const std::string& strFramebufferToSampleFrom);

		// Called from CGUIContainer to update this object
		void update(void* pParentContainer, bool bParentContainerAcceptingMouseClicks);

		// Sets the playback rate of the animation
		void setFramesPerSecond(float fFramesPerSecond);
		CColour mColour;

		// The tooltip for this object.
		// By default, it is disabled. Use this object to enable and setup the tooltip
		CGUITooltip* mpTooltip;
	private:
		CTimer _mTimer;
		float _mfFramesPerSecond;
		float _mfCurrentFrame;
		std::string _mstrResourceTexture2DAtlasName;	// Name of the CResourceTexture2DAtlas stored in SCResourceManager

	};
}