#pragma once
#include "PCH.h"
#include "vector2f.h"

namespace X
{
	// A base class which all user interface containers, widgets and windows are based upon.
	// It simply contains a few things which are common amoung all objects such
	// as position, dimensions and visibility.
	// By default, it's position is 0, 0, dimensions are 128, 128 and visibility set to true.
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
	private:
		CVector2f _mvDimensions;	// Dimensions of the widget
		CVector2f _mvPosition;		// Position of the widget in relation to it's container.
		bool _mbVisible;			// Whether this widget is shown or not
	};
}