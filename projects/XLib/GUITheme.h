#pragma once
#include "PCH.h"
#include "GUIColour.h"

namespace X
{
	// This class holds all settings for a single GUI theme.
	// They are created and accessed by the GUIManager.
	// Each of the widgets and containers(if set as a window) use a default theme, more themes can be created and switched between.
	// Themes may be set per container.
	// A theme is quite complex, but is designed so that it loads fast.
	// Each theme needs the following images...
	// colour:		This holds the RGB colour as well as the alpha channel for transparency.
	// normal:		This holds a normal map used for lighting effects.
	// reflection:  This is simply an image (or perhaps a framebuffer) which moves as the widgets move (due to their parent container being moved)
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

		// Adds the theme's currently set textures into ResourceManager so that they're ready to use.
		// If you need to change any of the theme's texture names, please call unloadTextures() before doing so.
		void loadTextures(void);

		// Removes the theme's currently set textures out of the ResourceManager.
		// If you need to change any of the theme's texture names, please call this method before doing so.
		void unloadTextures(void);

		// Adds the theme's currently set fonts into ResourceManager so that they're ready to use.
		// If you need to change any of the theme's font names, please call unloadFonts() before doing so.
		void addFontsToManager(void);

		// Removes the theme's currently set fonts out of the ResourceManager.
		// If you need to change any of the theme's font names, please call this method before doing so.
		void removeFontsFromManager(void);

		// Adds the theme's currently set audio into AudioManager so that they're ready to use.
		// If you need to change any of the theme's audio names, please call unloadAudio() before doing so.
		void addAudioToManager(void);

		// Removes the theme's currently set audio out of the AudioManager.
		// If you need to change any of the theme's audio names, please call this method before doing so.
		void removeAudioFromManager(void);

		std::string mstrThemeName;		// Holds the name of the theme.

		// Structure to hold the names of all the images used for this theme
		struct Images
		{
			std::string containerBGColour;	// The image name holding the colour data for a container
			std::string containerBGNormal;	// The image name holding the normal data for a container
			std::string buttonBGColour;		// The image name holding the colour data for a button
			std::string buttonBGNormal;		// The image name holding the normal data for a button
			std::string reflection;			// The image name holding the reflection data for the GUI.
			std::string textEditBGColour;
			std::string textEditBGNormal;
			std::string sliderBGColour;
			std::string sliderBGNormal;
			std::string sliderTabColour;
			std::string sliderTabNormal;
			std::string lineGraphBGColour;
			std::string lineGraphBGNormal;
			std::string tooltipBGColour;
			std::string tooltipBGNormal;
		};
		Images mImages;		// Holds the names of each image file used by the theme.

		// Structure to hold the names of all the fonts used by this theme
		struct Fonts
		{
			std::string containerTitle;	// GUIContainer font
			std::string button;			// GUIButton font
			std::string text;			// GUIText font
			std::string textEdit;
		};
		Fonts mFonts;

		// Structure to hold colours used for this theme
		struct Colours
		{
			GUIColour containerTitlebarTextInFocus;		// Colour of a container's titlebar text when the container is set to be a window and is in focus.
			GUIColour containerTitlebarTextNotInFocus;	// Colour of a container's titlebar text when the container is set to be a window and is not in focus.
			GUIColour buttonTextDown;					// Colour of a button's text when mouse is over and clicked
			GUIColour buttonTextOver;					// Colour of a button's text when mouse is over
			GUIColour buttonTextUp;						// Colour of a button's text when mouse is not over
			GUIColour text;								// GUIText font colour
			GUIColour textEditInactive;
			GUIColour textEditActive;
			GUIColour sliderTabOver;
			GUIColour sliderTabNotOver;
		};
		Colours mColours;	// Holds all the colours used by the theme

		// Offsets of various items
		struct Offset
		{
			int iOffsetX;
			int iOffsetY;
		};
		struct Offsets
		{
			Offset containerTitlebarText;
			Offset textEditText;
		};
		Offsets mOffsets;

		// Audio samples
		struct AudioSampleInfo
		{
			std::string strSampleName;
			float fVolume;
			float fPitch;
		};
		struct Audio
		{
			AudioSampleInfo buttonClicked;
			AudioSampleInfo textEditTextAdd;
			AudioSampleInfo textEditBackspace;
			AudioSampleInfo textEditReturn;
			AudioSampleInfo textEditActivate;
			AudioSampleInfo textEditNoMoreCharSpace;
		};
		Audio mAudio;

		// Floats
		float mfBlurAmount;				// 1.0f = max blur, 0.0f = none
		float mfNormalAmount;			// 1.0f = max normal, 0.0f = none
		float mfReflectionAmount;		// 1.0f = max reflection, 0.0f = none
		float mfMouseCursorDistance;	// Distance the mouse cursor is the fragments when computing the bump mapping
		float mfButtonTextFadeSpeed;	// Rate at which button text colours interpolate between
		float mfTextEditFlashSpeed;		// Rate at which the additional character that flashes when a text edit is active.
		float mfSliderTabFadeSpeed;		// Rate at which slider's tab colours interpolate between

	private:
		// Writes out image info
		void _writeImageInfo(std::ofstream& stream, const std::string& strDescription, const std::string& strImagename);

		// Reads in image info
		void _readImageInfo(std::ifstream& stream, std::string& strImagename);

		// Writes out font info
		void _writeFontInfo(std::ofstream& stream, const std::string& strDescription, const std::string& strFontname);

		// Reads in font info
		void _readFontInfo(std::ifstream& stream, std::string& strFontname);

		// Writes out colour info
		void _writeColourInfo(std::ofstream& stream, const std::string& strDescription, const GUIColour& colour);

		// Reads in colour info
		void _readColourInfo(std::ifstream& stream, GUIColour& colour);

		// Writes out offset info
		void _writeOffsetInfo(std::ofstream& stream, const std::string& strDescription, const Offset& offset);

		// Reads in offset info
		void _readOffsetInfo(std::ifstream& stream, Offset& offset);

		// Writes out AudioSampleInfo
		void _writeAudioSampleInfo(std::ofstream& stream, const std::string& strDescription, const AudioSampleInfo& asi);

		// Reads in AudioSampleInfo
		void _readAudioSampleInfo(std::ifstream& stream, AudioSampleInfo& asi);

		// Writes out float info
		void _writeFloatInfo(std::ofstream& stream, const std::string& strDescription, const float& fValue);

		// Reads in float info
		void _readFloatInfo(std::ifstream& stream, float& fValue);
	};
}