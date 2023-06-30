#pragma once
#include "PCH.h"
#include "GUIBaseObject.h"

namespace X
{
	class GUIImageAnimated : public GUIBaseObject
	{
		friend class GUIContainer;
	public:
		// Called from GUIContainer to render this object
		void render(void* pParentContainer, const std::string& strFramebufferToSampleFrom);

		// Called from GUIContainer to update this object
		void update(void* pParentContainer, bool bParentContainerAcceptingMouseClicks);

		GUIColour mColour;
	private:
		std::string _mstrResourceTexture2DAnimationName;	// Name of the ResourceTexture2DAnimation stored in ResourceManager

	};
}