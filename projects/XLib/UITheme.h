#pragma once
#include "PCH.h"

namespace X
{
	// Each of the widget containers use a default theme, more themes can be created and then switched between at runtime.
	// Themes may be set per widget container.
	// Each theme needs the following types of images...
	// colour:		RGBA. This holds the RGB colour as well as the alpha channel for transparency.
	// height:		Only R is used. This holds the height of a pixel and is used to generate the normal map...
	// normal:		RGB. This holds a normal map used for lighting effects. It is generated automatically from the height image.
	// 
	// So, when creating a theme, we need to create the colour and height images.
	// 
	// Images needed by most widgets.
	// A widget needs 9 individual images (9 for the colour and another 9 for the height for a total of 18)
	// There are 3 images per row and there are 3 columns, all representing a 3x3 grid.
	// This grid consists of 4 corners, 4 edges and a centre.
	// The 4 corners are top left, top right, bottom left and bottom right.
	// The 4 edges are left, right, top and bottom.
	// The 1 centre is the centre.
	// Each image in each row must have the same height.
	// Each image in each column must have the same width.
	// So for example, the top left corner, top right corner and top edge must have the same height.
	// The left and right edges and centre are repeated or mirror repeated (depending upon theme's settings) along the Y axis.
	// The top and bottom edges and centre are repeated or mirror repeated (depending upon theme's settings) along the X axis.
	// Some widgets need additional images, see the default theme to determine what these are.
	// So yeah, that's a shit ton of images needed by a theme which could introduce major texture rebinding hell.
	// Luckily, the theme loader combines all of these images into a single texture atlas during initialisation.
	// Although it's a lot of images to create, it's worth it because it allows quite a bit of flexibility.
	// 
	// As well as the above images, there are other theme settings which may be modified such as text colour for various widget states (for example,
	// a button's up/over/down/clicked states.
	class CUITheme
	{
	public:

	private:
	};
}