#pragma once
#include "PCH.h"
#include "vector2f.h"
#include "UITheme.h"
#include "UIContainer.h"

namespace X
{
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

		// Sets whether this container is visible or not
		void setVisible(bool bVisible);

		// Returns whether this container is visible or not
		bool getVisible(void) const;

		// Renders this widget's 9 grid cells representing the background
		void renderBackground(CUIContainer* pOwningContainer, CUITheme::SImageNamesWidget& widgetTextureNames);

	private:
		CVector2f _mvDimensions;	// Dimensions of the widget
		CVector2f _mvPosition;		// Position of the widget in relation to it's container.
		bool _mbVisible;			// Whether this widget is shown or not
	};
}