#pragma once
#include "PCH.h"
#include "colour.h"
#include "audioManager.h"

namespace X
{
	// A user interface theme.
	// These are created and used by the SCUIManager and all of the UI widgets and window.
	// Each of the widget containers are initially set to use a default theme, more themes can be created and then switched between at runtime.
	// Themes may be set per widget container/window.
	// 
	// Each theme needs the following types of images...
	// colour:		RGBA. This holds the RGB colour as well as the alpha channel for transparency.
	// height:		Only R is used. This holds the height of a pixel and is used to generate the normal map...
	// normal:		RGB. This holds a normal map used for lighting effects. It is generated automatically from the height image.
	// 
	// So, when creating a theme, we need to create the colour and height images.
	// 
	// Images needed by most widgets.
	// A widget needs 18 individual images (9 for the colour and alpha and another 9 for the height)
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
	// The major one being that a window's top bar can be taller than the bottom edge of a window.
	// As well as the above images, there are other theme settings which may be modified such as text colour for various widget states (for example,
	// a button's up/over/down/clicked states.
	class CUITheme
	{
	public:
		struct SSettings;
	
		// Constructor which sets everything to the default theme
		CUITheme();

		// Loads the theme with all it's settings from a file on disk previously saved with save()
		// The filename given, if it doesn't have the ".theme" extension, it is added.
		// If the file couldn't be opened, an exception occurs.
		// If this theme's resources have previously been added to the various managers, they are removed before loading commences.
		// If bCallAddAllResources is true, after loading the settings, the resources are added to the various managers, ready for use.
		void load(const std::string& strFilename, bool bCallAddAllResources = true);

		// Saves the theme with all it's settings to a file on disk for loading with load()
		// The filename is saved with the ".theme" extension. If it isn't at the end of the given filename, it is added.
		// If the file couldn't be opened, an exception occurs.
		void save(const std::string& strFilename);

		// Adds all resources stored in _mSettings to the various managers.
		// This removes any previously added resources.
		void addAllResources(void);

		// Removes all resources stored in _mSettings from the various managers.
		// If the resources haven't been added, this does nothing.
		void removeAllResources(void);
		
		// Returns a pointer to the _mSettings object so we can modify any of the settings for the theme.
		// This calls removeAllResources()
		SSettings* getSettings(void);

		// Each of the 9 cells in the 3x3 grid
		struct SImagesGrid
		{
			std::string centre;			// Centre image filename, repeated or mirror repeated along both X and Y.
			std::string cornerBL;		// Corner image filename, bottom left.
			std::string cornerBR;		// Corner image filename, bottom right.
			std::string cornerTL;		// Corner image filename, top left.
			std::string cornerTR;		// Corner image filename, top right.
			std::string edgeB;			// Edge image filename, bottom, repeated or mirror repeated along X.
			std::string edgeL;			// Edge image filename, left, repeated or mirror repeated along Y.
			std::string edgeR;			// Edge image filename, right, repeated or mirror repeated along Y.
			std::string edgeT;			// Edge image filename, top, repeated or mirror repeated along X.
		};

		// The filename for each type of image (Colour and height)
		struct SImageType
		{
			SImagesGrid colour;			// RGB for colour and A for transparency
			SImagesGrid height;			// Height used for generating normal maps. (Not stored in atlas)
			SImagesGrid normal;			// Normal used for lighting
		};

		// Struct holding each widget's and window's image filenames
		struct SImageNamesWidget
		{
			SImageType buttonBG;			// Image names for CUIButton's background
			SImageType buttonImageBG;		// Image names for CUIButtonImage's background
			SImageType checkboxBGNormal;	// Image names for CUICheckbox's normal state
			SImageType checkboxBGON;		// Image names for CUICheckbox's ON state
			SImageType checkboxBGOFF;		// Image names for CUICheckbox's OFF state
			SImageType lineGraphBG;			// Image names for CUILineGraph's background
			SImageType progressBarBG;		// Image names for CUIProgressBar's background
			SImageType progressBarFiller;	// Image names for CUIProgressBar's filler
			SImageType sliderBG;			// Image names for CUISlider's background
			SImageType sliderTab;			// Image names for CUISlider's tab
			SImageType textEditBG;			// Image names for CUITextEdit's background.
			SImageType textScrollBG;		// Image names for CUITextScroll's background.
			SImageType tooltipBG;			// Image names for CUITooltip's background.
		};

		// Structure to hold the names of all the fonts used by this theme
		struct SFonts
		{
			std::string button;						// Font used when rendering a button's text
			std::string text;						// Font used when rendering a UIText widget.
			std::string textEdit;					// Font used when rendering the text of a UITextEdit widget.
			std::string textScroll;					// Font used when rendering the text of a UITextScroll widget's text.
			std::string window;						// Font used when rendering a window's titlebar text.
		};

		// Structure to hold colours used for this theme
		struct SColours
		{
			CColour buttonBGDown;						// Colour of a button's BG when mouse is over and clicked
			CColour buttonBGOver;						// Colour of a button's BG when mouse is over
			CColour buttonBGUp;							// Colour of a button's BG when mouse is not over
			CColour buttonImageDown;					// CUIButtonImage's colour when mouse is over and clicked
			CColour buttonImageOver;					// CUIButtonImage's colour when mouse is over
			CColour buttonImageUp;						// CUIButtonImage's colour when mouse is not over
			CColour buttonTextDown;						// Colour of a button's text when mouse is over and clicked
			CColour buttonTextOver;						// Colour of a button's text when mouse is over
			CColour buttonTextUp;						// Colour of a button's text when mouse is not over
			CColour containerTitlebarTextInFocus;		// Colour of a container's titlebar text when the container is in focus.
			CColour containerTitlebarTextNotInFocus;	// Colour of a container's titlebar text when the container is not in focus.
			CColour progressBarFiller;					// CUIProgressBar's colour
			CColour sliderTabNotOver;					// CUISlider's tab colour when mouse is not over
			CColour sliderTabOver;						// CUISlider's tab colour when mouse is over
			CColour text;								// CUIText font colour
			CColour textEditActive;						// CUITextEdit's text colour when active
			CColour textEditInactive;					// CUITextEdit's text colour when inactive
			CColour textScrollBG;						// CUITextScroll's colour
			CColour textScrollText;						// CUITextScroll's text colour
			CColour tooltipText;						// Tool tip text colour
		};

		// Audio samples
		struct AudioSampleInfo
		{
			float pitch;								// Pitch of 1.0 is normal pitch of the sample
			float volume;								// Volume of 1.0 is normal volume of the sample
			std::string sampleName;						// The filename of the sample.
		};
		struct SAudio
		{
			AudioSampleInfo buttonClicked;				// Audio sample to play when a button is clicked
			AudioSampleInfo buttonImageClicked;			// Audio sample to play when an image button is clicked
			AudioSampleInfo textEditActivate;			// Audio sample to play when a text edit box is clicked upon and activated
			AudioSampleInfo textEditBackspace;			// Audio sample to play when a text edit box is activated and the backspace key is pressed
			AudioSampleInfo textEditNoMoreCharSpace;	// Audio sample to play when a text edit box is activated, a character key is pressed an the edit box has reached it's character limit.
			AudioSampleInfo textEditReturn;				// Audio sample to play when a text edit box is activated and the return key is pressed.
			AudioSampleInfo textEditTextAdd;			// Audio sample to play when a text edit box is activated and a characted is being added to it's current string.
		};
		
		// Structure to hold all floating point settings
		struct SFloats
		{
			float buttonFadeSpeedSeconds;			// Rate at which button text and BG colours interpolate between
			float buttonImageTextFadeSpeedSeconds;	// Rate at which button image colours interpolate between
			float checkboxFadeSpeedSeconds;			// Rate at which the checkbox fades between ON and OFF states
			float mouseCursorDistance;				// Distance the mouse cursor is from the fragments when computing the bump mapping.
			float normalAmount;						// Strength of normal mapping effect. 1.0f = max normal, 0.0f = none
			float sliderTabFadeSpeedSeconds;		// Rate at which slider's tab colours interpolate between
			float textEditFlashSpeed;				// Rate at which the additional character that flashes when a text edit is active.
			float textScrollSliderWidth;			// Width of a CUITextScroll object's vertical slider.
			float tooltipFadeSpeedSeconds;			// Rate at which tooltips face in/out
		};

		// Struct to hold all settings for the theme
		struct SSettings
		{
			SAudio audio;				// Holds all the audio samples used by the theme
			SColours colours;			// Holds all the colour settings used by the theme
			SFloats floats;				// Holds all floating point number settings used by the theme
			SFonts fonts;				// Holds all the names of all the fonts used by the theme
			SImageNamesWidget images;	// Holds all the names of the images stored in a texture atlas used by this theme.
			std::string imageDir;		// Holds the directory name containing each of the theme's images, for example "data/X/UI/default/images/"
			std::string themeName;		// Holds the name of the theme.
			
		};
	private:
		SSettings _mSettings;
		bool _mbResourcesAdded;			// Whether resources have been added or not

		// Helper method to set all settings in _mSettings to default theme
		void _setSettingsToDefault(void);

		// Helper method to simplify setting of filenames for each widget
		// Example usage...
		// _helperSetGridNames(_mSettings.images.buttonBG, "data/X/UI/default/buttonBG");
		void _helperSetNames(SImageType& imageType, const std::string& strBaseName);

		// Helper method to set audio settings
		void _helperSetAudio(AudioSampleInfo& audio, const std::string& strSampleName, float fPitch, float fVolume);

		// Helper method to read in audio sample info from an already opened ifstream
		void _helperReadAudio(std::ifstream& file, AudioSampleInfo& audio);

		// Helper method to write out audio sample info to an already opened ofstream
		void _helperWriteAudio(std::ofstream& file, AudioSampleInfo& audio);

		// Helper method to read in image filenames for each widget type from already opened ifstream
		void _helperReadImageType(std::ifstream& file, SImageType& imageType);

		// Helper method to write out image filenames for each widget type from already opened ofstream
		void _helperWriteImageType(std::ofstream& file, SImageType& imageType);
	};
}