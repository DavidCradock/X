#pragma once
#include "PCH.h"
#include "GUIBaseObject.h"
#include "colour.h"

namespace X
{
	class CGUITooltip;

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

		// Sets the text to be rendered
		void setText(const std::string& strText);

		// Gets the currently set text
		std::string getText(void) const;

		// Sets the colour to use for this text, or use the theme colour
		void setColour(bool bUseDefaultThemeColour = false, const CColour& colour = CColour(1,1,1,1));

		// The tooltip for this object.
		// By default, it is disabled. Use this object to enable and setup the tooltip
		CGUITooltip* mpTooltip;
	private:
		std::string _mstrText;	// Text to be rendered, set by calling setText()
		bool _mbUseThemeColour;
		CColour _mColour;
	};
}