#pragma once
#include "PCH.h"
#include "GUIBaseObject.h"
#include "colour.h"

namespace X
{
	class CGUIText : public CGUIBaseObject
	{
	public:
		CGUIText();
		~CGUIText();

		// Called from CGUIContainer to render this object
		void render(void* pParentContainer);

		// Called from CGUITooltip::render()
		void renderForTooltip(void* pParentContainer, unsigned int uiTooltipFramebufferWidth, unsigned int uiTooltipFramebufferHeight);

		// Called from CGUIContainer to update this object
		void update(void* pParentContainer, bool bParentContainerAcceptingMouseClicks);

		std::string mstrText;	// Text to be rendered

		// Sets the colour to use for this text, or use the theme colour
		void setColour(bool bUseDefaultThemeColour = false, float fRed = 1.0f, float fGreen = 1.0f, float fBlue = 1.0f, float fAlpha = 1.0f);

		// The tooltip for this object.
		// By default, it is disabled. Use this object to enable and setup the tooltip
		// We've had to set this as a void pointer due to header inclusion restrictions.
		// Type cast this to a CGUITooltip* to use. CGUITooltip* pTT = (CGUITooltip*)pWidget->mpTooltip;
		void* mpTooltip;
	private:

		bool _mbUseThemeColour;
		CColour _mColour;
	};
}