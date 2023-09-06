#pragma once
#include "PCH.h"
#include "resourceTexture2DAtlas.h"
#include "resourceVertexBufferCPT2.h"
#include "UITheme.h"

namespace X
{
	class CUIContainer;

	// Text using a framebuffer to increase rendering performance.
	// Can accept text with multiple lines. Use "\n" inside the string to create a new line.
	// Even if no new line code is used, the text is wrapped so that no words go outside of the
	// widget's set position + dimensions.
	// The text will not go outside of the widget's position + dimensions due to both scissor testing and being rendered to a framebuffer.
	// By default, when rendering, the widget uses the colour set by the owning container's theme but can be set with setColour()
	class CUIText
	{
//		friend class CUIContainer;
	public:
		CUIText(CUIContainer* pContainer, const std::string& strWidgetName);
		~CUIText();

		// Sets the dimensions of the widget.
		void setDimensions(float fX, float fY);

		// Sets the dimensions of the widget.
		void setDimensions(const CVector2f& vDimensions);

		// Returns the dimensions of the widget.
		CVector2f getDimensions(void) const;

		// Sets the position of this widget in relation to it's container.
		// Both axis should be at least zero. I wish there was an unsigned float, but oh well :)
		void setPosition(float fX, float fY);

		// Sets the position of this widget in relation to it's container.
		// Both axis should be at least zero. I wish there was an unsigned float, but oh well :)
		void setPosition(const CVector2f& vPosition);

		// Returns the position of the widget in relation to it's container.
		CVector2f getPosition(void) const;

		// Sets whether this widget is visible or not.
		void setVisible(bool bVisible);

		// Returns whether this widget is visible or not.
		bool getVisible(void) const;

		// Render this object
		void render(void);

		/******************************************************************* Widget specific *******************************************************************/

		// Sets the text string to be rendered
		void setText(const std::string& strText);

		// Returns the text string that's used to render
		std::string getText(void) const;

		// Sets whether to use the theme's text colour or the colour given here
		void setTextColour(bool bUseThemeColour, const CColour& colour = CColour(1, 1, 1, 1));

		// Sets the colour to clear the framebuffer to.
		// By default, this is CColour(0,0,0,0);
		// It's usefull to set to a non-transparent colour if we wish to add the text to a container and make the text more readable.
		// It's also usefull for debugging as the entire framebuffer is set to the given colour.
		void setBackgroundColour(const CColour& colour = CColour(0, 0, 0, 0));

		// Sets whether to use the theme's font or to use the named one
		// The named font should be added to the SCResourceManager with a call to it's addFont() method.
		// If the named font resource doesn't exist, an exception occurs.
		void setFont(bool bUseThemeFont, const std::string& strFontResourceToUse = "");

		// Sets this widget's text to be centered at it's set position.
		// Otherwise (the default), the text is rendered with the position representing the top left corner of the text's rect.
		// If text is centered, then it is rendered as a single line.
		// If text is centered, this calls _helperComputeCenteredPosDims()
		void setCentered(bool bCenterTheText);

		// Sets the framebuffer to need updating
		void setFramebufferNeedsUpdating(void);
	private:
		// Common amoung widgets
		CVector2f _mvDimensions;			// Dimensions of the widget
		CVector2f _mvPosition;				// Position of the widget in relation to it's container.
		bool _mbVisible;					// Whether this widget is visible or not.
		CUIContainer* _mpContainer;			// The container this widget belongs to. Set in constructor

		// Widget specific
		std::string _mstrText;				// Text string to be rendered

		CColour _mColourText;				// Current text colour (if not using the text colour from the theme)
		bool _mbUseThemeColour;				// Whether to use the theme's text colour or the one given to setColour()

		CColour _mColourFramebufferClear;	// The colour used when clearing the framebuffer for this widget.
		
		std::string _mstrFontResourceName;	// Font resource to use for the text rendering, if _mvUseThemeFont is false.
		bool _mbUseThemeFont;				// Whether to use the theme's font or the one given to setFont()

		bool _mbFramebufferNeedsUpdating;	// If any settings changed, we need to re-render this widget's framebuffer
		std::string _mstrFramebufferName;	// The unique name of this widget's framebuffer. Computed in constructor

		// Centering members
		bool _mbCentered;					// Whether this widget is to be centered.
		CVector2f _mvNonCenteredPos;		// Holds the position passed to setPosition methods and is used to compute and store the actual position in _mvPosition
		CVector2f _mvNonCenteredDims;		// Holds the dimensions passed to setDimensions methods and is used to compute and store the actual dimensions in _mvDimensions
		// Using the values stored in _mvNonCenteredPos/Dims, if _mvCentered is true, computes and stores actual pos/dims in _mvPosition and _mvDimensions.
		void _helperComputeCenteredPosDims(void);
	};
}