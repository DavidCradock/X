#pragma once
#include "PCH.h"
#include "GUIBaseObject.h"
#include "GUIColour.h"
#include "GUISlider.h"

namespace X
{
	class GUITextScroll : public GUIBaseObject
	{
		friend class GUIContainer;
	public:
		GUITextScroll();
		~GUITextScroll();

		// Called from GUIContainer to render this object
		void render(void* pParentContainer, const std::string& strFramebufferToSampleFrom);

		// Called from GUIContainer to update this object
		void update(void* pParentContainer, bool bParentContainerAcceptingMouseClicks);

		// Modifies this object's text which re-renders the text to a framebuffer.
		// If there's a lot of text, please don't call this each program loop willy-nilly, thanks :)
		void setText(const std::string& strText);

		// Modifies this object's text colour which re-renders the text to a framebuffer.
		// If there's a lot of text, please don't call this each program loop willy-nilly, thanks :)
		void setTextColour(float fRed, float fGreen, float fBlue, float fAlpha);

		// The tooltip for this object.
		// By default, it is disabled. Use this object to enable and setup the tooltip
		// We've had to set this as a void pointer due to header inclusion restrictions.
		// Type cast this to a GUITooltip* to use. GUITooltip* pTT = (GUITooltip*)pWidget->mpTooltip;
		void* mpTooltip;
	private:
		std::string _mstrFBName;	// Name of framebuffer in ResourceManager used for rendering this objects text to.
		bool _mbFBNeedsUpdating;	// Flag to re-render the frame buffer containing the text if needed.
		std::string _mstrText;		// The text to be rendered on top of this scroll text object
		GUIColour _mTextColour;		// Colour of this object's text
		GUISlider _mSlider;			// Holds the slider which is used to scroll through the text
		float _mfPreviousSliderTabPos;	// Holds previous tab position to see if we need to update the render buffer

		// Renders the framebuffer containing the text
		void _renderFramebuffer(void* pParentContainer);
	};
}