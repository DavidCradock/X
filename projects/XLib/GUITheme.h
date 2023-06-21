#pragma once
#include "PCH.h"
#include "GUIColour.h"

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
	// Upon construction, a theme has default settings set.
	class GUITheme
	{
	public:
		// Constructor which sets everything to default theme
		GUITheme();

		// Loads the theme with all it's settings from a file on disk previously saved with save()
		// The filename given, if it doesn't have the ".theme" extension, it is added.
		// If the file couldn't be opened, an exception occurs.
		void load(const std::string& strFilename);

		// Saves the theme with all it's settings to a file on disk for loading with load()
		// The filename is saved with the ".theme" extension. If it isn't at the end of the given filename, it is added.
		// If the file couldn't be opened, an exception occurs.
		void save(const std::string& strFilename);

		std::string mstrThemeName;		// Holds the name of the theme.

		// Structure to hold the names of all the images used for this theme
		struct Images
		{
			std::string containerBlur;		// The image name holding the blur data for a container
			std::string containerColour;	// The image name holding the colour data for a container
			std::string containerGlow;		// The image name holding the glow data for a container
			std::string containerNormal;	// The image name holding the normal data for a container
			std::string reflection;			// The image name holding the reflection data for the GUI.
		};
		Images mImages;		// Holds the names of each image file used by the theme.

		// Structure to hold colours used for this theme
		struct Colours
		{
			GUIColour containerTitlebarTextInFocus;		// Colour of a container's titlebar text when the container is set to be a window and is in focus.
			GUIColour containerTitlebarTextNotInFocus;	// Colour of a container's titlebar text when the container is set to be a window and is not in focus.
		};
		Colours mColours;	// Holds all the colours used by the theme

	private:
		// Converts the stored values of the given colour, to text. Used for writing out to a file.
		std::string _colourToString(GUIColour& colour);

		// Given an ifstream and a colour, reads in the RGBA values. Used to read from a file
		void _colourReadValues(std::ifstream& stream, GUIColour &colour);
	};
}