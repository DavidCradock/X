#pragma once
#include "PCH.h"
#include "GUIBaseObject.h"
#include "colour.h"
#include "GUISlider.h"

namespace X
{
	class CGUITooltip;

	class CGUITextScroll : public CGUIBaseObject
	{
		friend class CGUIContainer;
		friend class CGUISubContainer;
	public:
		CGUITextScroll();
		~CGUITextScroll();

		// Called from CGUIContainer to render this object
		void render(void* pParentContainer, const std::string& strFramebufferToSampleFrom);

		// Called from CGUIContainer to update this object
		void update(void* pParentContainer, bool bParentContainerAcceptingMouseClicks);

		// Modifies this object's text which re-renders the text to a framebuffer.
		// If there's a lot of text, please don't call this each program loop willy-nilly, thanks :)
		void setText(const std::string& strText);

		// The tooltip for this object.
		// By default, it is disabled. Use this object to enable and setup the tooltip
		CGUITooltip* mpTooltip;
	private:
		std::string _mstrFBName;		// Name of framebuffer in SCResourceManager used for rendering this objects text to.
		bool _mbFBNeedsUpdating;		// Flag to re-render the frame buffer containing the text if needed.
		std::string _mstrText;			// The text to be rendered on top of this scroll text object
		CGUISlider _mSlider;			// Holds the slider which is used to scroll through the text
		float _mfPreviousSliderTabPos;	// Holds previous tab position to see if we need to update the render buffer

		// Renders the framebuffer containing the text
		void _renderFramebuffer(void* pParentContainer);
	};
}