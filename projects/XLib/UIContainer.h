#pragma once
#include "PCH.h"
#include "UIBaseWidget.h"
#include "UIScrollBar.h"
#include "UITheme.h"

namespace X
{
	// A container object for the user interface.
	// A container object allows us to add all the various UI widgets to it.
	// A container is a CUIBaseWidget which stores position, dimensions and visibility.
	class CUIContainer : public CUIBaseWidget
	{
	public:
		CUIContainer();

	private:

	};
}