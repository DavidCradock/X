#pragma once
#include "PCH.h"
#include "GUIBaseObject.h"
#include "colour.h"

namespace X
{
	class CGUIImageFramebuffer : public CGUIBaseObject
	{
		friend class CGUIContainer;
		friend class CGUITooltip;
	public:
		CGUIImageFramebuffer();
		~CGUIImageFramebuffer();

		// Called from CGUIContainer to render this object
		void render(void* pParentContainer, const std::string& strFramebufferToSampleFrom);

		// Called from CGUIContainer to update this object
		void update(void* pParentContainer, bool bParentContainerAcceptingMouseClicks);

		CColour mColour;

		// The tooltip for this object.
		// By default, it is disabled. Use this object to enable and setup the tooltip
		// We've had to set this as a void pointer due to header inclusion restrictions.
		// Type cast this to a CGUITooltip* to use. CGUITooltip* pTT = (CGUITooltip*)pWidget->mpTooltip;
		void* mpTooltip;
	private:
		std::string _mstrFBname;	// Name of the framebuffer stored in SCResourceManager
	};
}