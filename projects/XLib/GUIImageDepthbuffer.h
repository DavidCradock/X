#pragma once
#include "PCH.h"
#include "GUIBaseObject.h"
#include "GUIColour.h"

namespace X
{
	class GUIImageDepthbuffer : public GUIBaseObject
	{
		friend class GUIContainer;
		friend class GUITooltip;
	public:
		GUIImageDepthbuffer();
		~GUIImageDepthbuffer();

		// Called from GUIContainer to render this object
		void render(void* pParentContainer, const std::string& strFramebufferToSampleFrom);

		// Called from GUIContainer to update this object
		void update(void* pParentContainer, bool bParentContainerAcceptingMouseClicks);

		GUIColour mColour;

		// The tooltip for this object.
		// By default, it is disabled. Use this object to enable and setup the tooltip
		// We've had to set this as a void pointer due to header inclusion restrictions.
		// Type cast this to a GUITooltip* to use. GUITooltip* pTT = (GUITooltip*)pWidget->mpTooltip;
		void* mpTooltip;
	private:
		std::string _mstrDBname;	// Name of the depthbuffer stored in ResourceManager
	};
}