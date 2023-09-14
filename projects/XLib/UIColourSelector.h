#pragma once
#include "PCH.h"
#include "resourceTexture2DAtlas.h"
#include "resourceVertexBufferCPT2.h"
#include "UITheme.h"

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
		CUIColourSelector(CUIContainer* pContainer, const std::string& strWidgetName);
		~CUIColourSelector();

		// Sets the dimensions of the widget.
		void setDimensions(float fX, float fY);

		// Sets the dimensions of the widget.
		void setDimensions(int iX, int iY);

		// Sets the dimensions of the widget.
		void setDimensions(const CVector2f& vDimensions);

		// Returns the dimensions of the widget.
		CVector2f getDimensions(void) const;

		// Returns the minimum dimensions of this widget based upon the container's currently set theme.
		// As this widget has no background, this returns a vector of (64, 64)
		CVector2f getDimensionsMinimum(void) const;

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
		
		// Sets the current brightness value of the colour wheel
		// Clamped between 0 and 1
		void setBrightness(float fBrightness);
		
		// Sets the width of the brightness selector
		// Default is 20
		void setBrightnessWidth(int iWidth);
	private:
		// Common amoung widgets
		CVector2f _mvDimensions;			// Dimensions of the widget
		CVector2f _mvPosition;				// Position of the widget in relation to it's container.
		bool _mbVisible;					// Whether this widget is visible or not.
		CUIContainer* _mpContainer;			// The container this widget belongs to. Set in constructor

		// Widget specific
		// The name of the resource stored in SCResourceManager. Set in constructor.
		std::string _mstrResourceNameWheel;
		std::string _mstrResourceNameBrightness;
		float _mfBrightness;	// Currently set brightness value (Default is 1.0f)
		int _miBrightnessWidth;	// Width of the brightness selector

		// Redraw and update the image textures
		void _updateTextures(void);
	};
}