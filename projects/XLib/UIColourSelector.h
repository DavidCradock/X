#pragma once
#include "PCH.h"
#include "resourceTexture2DAtlas.h"
#include "resourceVertexBufferCPT2.h"
#include "UITheme.h"
#include "UIScrollBar.h"
#include "UITextEdit.h"

namespace X
{
	class CUIContainer;

	// A colour selector widget for being able to select a colour
	// There are no borders or anything, just a simple texture resource to render.
	// The named texture resource stored in SCResourceManager will be resized to this widget's set dimensions.
	// That doesn't stop you from setting this widget's dimension though :)
	class CUIColourSelector
	{
		friend class CUIContainer;
	public:
		CUIColourSelector(CUIContainer* pContainer, const std::string& strWidgetName, float fPosX, float fPosY, int iColourWheelRadius);
		~CUIColourSelector();

		// Sets the dimensions of the widget.
		// This is the radius of the colour wheel only. Additional widgets are added to the parent container.
		void setDimensions(float fColourWheelRadius);

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
		
		// Sets the radius of the circles showing position in selectors
		// Default is 10
		void setSelectorCircleRadius(int iCircleRadius);

		// Sets the colour represented by this widget.
		// This sets the selection circles to their correct positions
		void setColour(const CColour& colour);

		// Sets the dimensions of the text edit widgets
		void setTextEditDims(const CVector2f& vDims = CVector2f(80, 40));
	private:
		// Common amoung widgets
		CVector2f _mvDimensions;			// Dimensions of the widget
		CVector2f _mvPosition;				// Position of the widget in relation to it's container.
		bool _mbVisible;					// Whether this widget is visible or not.
		CUIContainer* _mpContainer;			// The container this widget belongs to. Set in constructor

		// Widget specific
		// The name of the resources stored in SCResourceManager. Set in constructor.
		std::string _mstrResourceNameWheel;
		std::string _mstrResourceNameCircle;

		int _miCircleRadius;			// Radius of the selector circle
		
		CColour _mColour;				// Colour represented by this widget

		enum EValue
		{
			brightness,
			colourR,
			colourG,
			colourB,
			colourA,
			hue,
			saturation
		};
		std::string _mstrScrollbarNames[7];
		std::string _mstrTextEditNames[7];
		std::string _mstrTextNames[7];
		CUIScrollbar* _mpScrollbar[7];
		CUITextEdit* _mpTextEdit[7];
		CUIText* _mpText[7];

		// Redraw and update the image textures
		void _updateTextures(void);

		// Create/update scrollbars and textedit widgets
		void _updateScrollbarsAndTextEdits(void);

	};
}