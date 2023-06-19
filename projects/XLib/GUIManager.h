#pragma once
#include "PCH.h"
#include "singleton.h"
namespace X
{
	// Here's where we go for everything GUI related.
	// It works on the concept of a container, which holds various GUI widgets and can be set to act as a moveable window or not.
	// 
	// Containers:
	// A container by default is simple an empty, err, container used to add various widgets to such as buttons.
	// A container can be set to be a window which can be dragged around the screen, closed and minimized with z-order supoort.
	// If a container is set as a window, it has dimensions. Width and height. When setting a container's dimensions, the dimensions
	// given are the area inside the container where widgets may be added, not including the container's borders.
	// Again, if set as a window, it may or may not have a close, minimize, maximize button and title bar text.
	// 
	// Widgets:
	// A container hold widgets. Widgets include buttons, static text, editable text boxes, text boxes with vertical scroll bar,
	// generic buttons using the theme, image buttons which use a custom set image, progress bars, static images, animated images, 
	// frame buffer boxes which render the contents of a framebuffer (Scene managers render to framebuffers and these can be used),
	// draggable images/framebuffers which can be dragged between "draggable storage" widgets, line graphs, expandable menus with
	// support for holding widgets such as buttons, text and images, for each item in the menu. We also have taskbars.
	// We also have sliders which may be horizontal or vertical, have some text next to them to show the value they represent and
	// can also have "notches" whereby when the draggable tab is moved, can "snap" to one of these notches.
	//
	// Taskbars:
	// A taskbar is a container with special, automatically controlled behaviour.
	// They hold minimized windows and may contain additional widgets which you may add yourself. Ideas include static text which
	// displays the time and maybe also a notification area, much like on the window managers of our beloved operating systems.
	// 
	// Tooltips:
	// Hovering over widgets may show a tooltip, which may be as simple as a line of text, or could hold for example, a complete GUI container
	// with text, animated images, progress bars etc. All the widgets which are non-interactable can be placed inside a tooltip of a widget.
	// As creating a fancy container with many widgets for use as a widget's tooltip could be complicated and time consuming to program,
	// there are helper functions which create various "tooltip templates" such as "just text" or "left aligned text box with right animated image".
	// 
	// Rendering features:
	// We have colour and transparency, background blur normal maps for lighting effects such as the mouse being a light source and glow.
	// 
	// Audio features:
	// Only one feature, we can playback sounds when things get clicked, dropped, closed etc.
	// 
	// Themes:
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
	// 
	// Scaling:
	// We have implemented text scaling which works quite well when using a ResourceFont which has been created from a large font (such as a size
	// of around 200 or so) and this text scales down quite well. But as we know, scaling bitmap images up to a larger size introduces
	// bluriness, so when creating fonts for the GUI to use, create them huge as the GUI only allows scaling down in size of text.
	// Do the same for each of the required images (colour/normal/blur/etc). Make them larger than needed for optimal appearance.
	// The GUI does allow scaling up however, but it's best to make the images and fonts large enough, for say an 8k display. At least this way,
	// we have some future proofing.
	// Scaling of the GUI is set with a single float value. Values should be 1.0f, 0.5f, 0.25f, 0.125f for optimal appearance, but can be set to
	// any value.
	// 
	// Implementation details:
	// Containers are Z sorted and rendered one at a time, along with their widgets.
	// Then the tooltips are rendered next.
	// Where possible, certain things are rendered to framebuffers which are only updated if needed to prevent rendering of stuff every frame. 
	// A great use of this is with text which is quite demanding and if the text doesn't change, there's no point rendering it each frame update.
	// As themes contain many images per widget and there are many widgets, there are many images/textures to deal with.
	// When creating a theme, we keep things simple for us by creating a theme from many individual images.
	// Having many individual images is slow for rendering, so when creating a theme, the GUI code organises everything into a tidy package which
	// is both fast to load and fast to render. All colour images are stored in a single texture, all normal images in a single texture etc.
	// This way, when rendering stuff (except fonts), we only have to bind one set of textures.
	// 
	class GUIManager : public Singleton<GUIManager>
	{
	public:

	};
}