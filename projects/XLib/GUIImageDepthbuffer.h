#pragma once
#include "PCH.h"
#include "GUIBaseObject.h"
#include "GUIColour.h"

namespace X
{
	class GUIImageDepthbuffer : public GUIBaseObject
	{
		friend class GUIContainer;
	public:
		// Called from GUIContainer to render this object
		void render(void* pParentContainer, const std::string& strFramebufferToSampleFrom);

		// Called from GUIContainer to update this object
		void update(void* pParentContainer, bool bParentContainerAcceptingMouseClicks);

		GUIColour mColour;
	private:
		std::string _mstrDBname;	// Name of the depthbuffer stored in ResourceManager
	};
}