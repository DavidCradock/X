#pragma once
#include "PCH.h"
#include "resourceTexture2DAtlas.h"
#include "resourceVertexBufferCPT2.h"
#include "UIImage.h"
#include "UITheme.h"
#include "UIScrollBar.h"
#include "UITextEdit.h"
#include "colour.h"

namespace X
{
	class CUIContainer;

	// A colour selector widget for being able to select a colour
	// This adds several uniquely named widgets which are controlled by this one, scrollbars, text and text edit widgets.
	class CUIColourSelector
	{
		friend class CUIContainer;
	public:
		CUIColourSelector(CUIContainer* pContainer, const std::string& strWidgetName, float fPosX, float fPosY);
		~CUIColourSelector();

		// Returns the dimensions of the widget.
		CVector2f getDimensions(void) const;

		// Sets the position of this widget in relation to it's container.
		// Both axis should be at least zero.
		void setPosition(float fX, float fY);

		// Sets the position of this widget in relation to it's container.
		// Both axis should be at least zero.
		void setPosition(int iX, int iY);

		// Sets the position of this widget in relation to it's container.
		// Both axis should be at least zero.
		void setPosition(const CVector2f& vPosition);

		// Returns the position of the widget in relation to it's container.
		CVector2f getPosition(void) const;

		// Sets whether this widget is visible or not.
		void setVisible(bool bVisible);

		// Returns whether this widget is visible or not.
		bool getVisible(void) const;

		// Render this widget's non-background items
		void renderNonBG(void);

		// Update this widget
		void update(float fTimeDeltaSec);

		/******************************************************************* Widget specific *******************************************************************/
		
		// Sets the colour represented by this widget.
		// Sets RGBA sliders and text edit widgets to the given colour and then
		// sets the HSB sliders and text edit widgets from the values stored in RGB sliders.
		void setColour(const CColour& colour);

		// Returns the colour of the widget
		CColour getColour(void) const;
	private:
		// Common amoung widgets
		CVector2f _mvDimensions;			// Dimensions of the widget
		CVector2f _mvPosition;				// Position of the widget in relation to it's container.
		bool _mbVisible;					// Whether this widget is visible or not.
		CUIContainer* _mpContainer;			// The container this widget belongs to. Set in constructor

		// Widget specific
		CColour _mColour;				// Colour represented by this widget

		enum EValue
		{
			colourR,
			colourG,
			colourB,
			colourA,
			hue,
			saturation,
			brightness
		};
		std::string _mstrScrollbarNames[7];	// Name of each scrollbar widget
		std::string _mstrTextEditNames[7];	// Name of each textedit widget
		std::string _mstrTextNames[7];		// Name of each text widget
		std::string _mstrColourImageName;	// Name of the CUIImage widget used to show colour
		CUIScrollbar* _mpScrollbar[7];		// Pointer to each scrollbar widget
		CUITextEdit* _mpTextEdit[7];		// Pointer to each text edit widget
		CUIText* _mpText[7];				// Pointer to each text widget
		CUIImage* _mpColourImage;			// Pointer to CUIImage widget

		void _updateWidgetDims(void);

		// Sets RGB sliders and their text from values in HSB sliders
		void _helperSetRGBFromHSB(void);

		// Sets HSB sliders and their text from values in RGB sliders
		void _helperSetHSBFromRGB(void);
	};
}