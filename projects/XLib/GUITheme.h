#pragma once
#include "PCH.h"

namespace X
{
	// This class holds all settings for a single GUI theme.
	// They are created and accessed by the GUIManager.
	// Each of the widgets and containers(if set as a window) use a default theme, more themes can be created and switched between.
	// A theme is quite complex, but is designed so that it loads fast.
	// Each theme needs the following images...
	// colour:		This holds the RGB colour as well as the alpha channel for transparency.
	// normal:		This holds a normal map used for lighting effects.
	// blur:		This holds the amount to blur the background (If a scenemanager renders to the default framebuffer, this will be used as the
	//				background) in the R channel.
	// glow:		For when a widget has the mouse over, a semi transparent image will fade in over the top.
	//
	// reflection: This is simply an image (or perhaps a framebuffer) which moves as the widgets move (due to their parent container being moved)
	// 
	// As well as the above images, there are other settings which may be modified such as text colour for various widget states (for example,
	// a button's up/over/down/clicked states.
	class GUITheme
	{
	public:

	};
}