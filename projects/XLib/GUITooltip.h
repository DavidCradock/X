#pragma once
#include "PCH.h"
#include "GUIBaseObject.h"
namespace X
{
	// Hovering over widgets may show a tooltip, which may be as simple as a line of text, or could hold for example, text, animated images,
	// progress bars and more. All the widgets which are non-interactable can be placed inside a tooltip of a widget.
	// As creating a fancy tooltip with many widgets could be complicated and time consuming to program, there are helper functions
	// which create various "tooltip templates" such as "just text" or "left aligned text box with right animated image".
	class GUITooltip : public GUIBaseObject
	{
	public:
		GUITooltip();
	};
}