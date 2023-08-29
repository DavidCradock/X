#pragma once
#include "PCH.h"
#include "vector2f.h"
#include "UITheme.h"

namespace X
{
	class CResourceTexture2DAtlas;
	class CUIContainer;
	class CResourceVertexBufferCPT2;

	// A base class which all user interface widgets are based upon.
	// It simply contains a few things which are common amoung all widgets
	// By default, it's position is 0, 0 and dimensions are 128, 128.
	class CUIBaseWidget
	{
	public:
		CUIBaseWidget();

		// Sets the position of this widget in relation to it's container.
		void setPosition(const CVector2f& vPosition);

		// Returns the position of the widget in relation to it's container.
		CVector2f getPosition(void) const;

		// Sets the dimensions of the widget.
		void setDimensions(const CVector2f& vDimensions);

		// Returns the dimensions of the widget.
		CVector2f getDimensions(void) const;

	protected:
		CVector2f _mvDimensions;	// Dimensions of the widget
		CVector2f _mvPosition;		// Position of the widget in relation to it's container.

		// Adds geometry to the given vertex buffer for the 9 grid cells
		void _addGridGeometry(CUITheme::SImageType imageType, CResourceTexture2DAtlas* pAtlas, CUIContainer* pContainer, bool bContainerIsWindow, CUITheme* pTheme, CResourceVertexBufferCPT2* pVB);
	};
}