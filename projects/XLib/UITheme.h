#pragma once
#include "PCH.h"
#include "colour.h"
#include "audioManager.h"
#include "resourceManager.h"

namespace X
{
	// A user interface theme.
	// These are created and used by the SCUIManager and all of the UI widgets and window.
	// Each of the widget containers are initially set to use a default theme, more themes can be created and then switched between at runtime.
	// Themes may be set per container/window.
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
		// This also creates the normal images from the height images if they do not exist.
		// Passing true to bOverwriteNormalImages will rebuild the normal images regardless of whether they exist or not
		// May throw an exception if the images' dimensions are invalid (See _helperCheckImageDimsAreOK())
		void addAllResources(bool bOverwriteNormalImages = false);

		// Removes all resources stored in _mSettings from the various managers.
		// If the resources haven't been added, this does nothing.
		void removeAllResources(void);
		
		// Returns a pointer to the _mSettings object so we can access any of the settings for the theme.
		SSettings* getSettings(void);

		// Returns a pointer to the _mSettings object so we can modify them.
		// This removes all the resources from the various managers by calling removeAllResources() before returning.
		// Call this method, then update any settings, then do NOT forget to call addAllResources() again afterwards
		SSettings* getSettingsForModification(void);

		// Returns a pointer to this theme's texture atlas
		// If the resources haven't been loaded yet, an exception occurs
		// To check if the resources have been loaded, call getResourcesAdded()
		CResourceTexture2DAtlas* getTextureAtlas(void);

		// Returns whether the resources for this theme have been added to the various managers or not
		bool getResourcesAdded(void);

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
//			SImageType buttonImageBG;		// Image names for CUIButtonImage's background
			SImageType checkboxBGOff;		// Image names for CUICheckbox's OFF state
			SImageType checkboxBGOffOver;	// Image names for CUICheckbox's OFF, with mouse over state
			SImageType checkboxBGOn;		// Image names for CUICheckbox's ON state
			SImageType checkboxBGOnOver;	// Image names for CUICheckbox's ON, with mouse over state
			SImageType lineGraphBG;			// Image names for CUILineGraph's background
			SImageType progressBarBG;		// Image names for CUIProgressBar's background
			SImageType progressBarFiller;	// Image names for CUIProgressBar's filler
			SImageType scrollbarBG;			// Image names for CUIScrollbar's background
			SImageType scrollbarTab;		// Image names for CUIScrollbar's tab
			SImageType textEditBG;			// Image names for CUITextEdit's background.
//			SImageType textScrollBG;		// Image names for CUITextScroll's background.
			SImageType tooltipBG;			// Image names for CUITooltip's background.
			SImageType windowBG;			// Image names for CUIWindow's background.
		};

		// Structure to hold the names of all the fonts used by this theme
		struct SFonts
		{
			std::string button;						// Font used when rendering a button's text
			std::string buttonImage;				// Font used when rendering a buttonImage's text
			std::string text;						// Font used when rendering a UIText widget.
			std::string textEdit;					// Font used when rendering the text of a UITextEdit widget.
//			std::string textScroll;					// Font used when rendering the text of a UITextScroll widget's text.
			std::string tooltipText;				// Font used when rendering the text of a UITooltip widget's text.
			std::string windowTitlebar;				// Font used when rendering a window's titlebar text.
		};

		// Structure to hold colours used for this theme
		struct SColours
		{
			CColour ambientLight;						// Colour of the ambient light used during normal mapping lighting.
			CColour buttonBGDown;						// Colour of a button's BG when mouse is over and clicked
			CColour buttonBGOver;						// Colour of a button's BG when mouse is over
			CColour buttonBGUp;							// Colour of a button's BG when mouse is not over
			CColour buttonTextDown;						// Colour of a button's text when mouse is over and clicked
			CColour buttonTextOver;						// Colour of a button's text when mouse is over
			CColour buttonTextUp;						// Colour of a button's text when mouse is not over
			CColour buttonImageBGDown;					// Colour of a buttonImage's BG when mouse is over and clicked
			CColour buttonImageBGOver;					// Colour of a buttonImage's BG when mouse is over
			CColour buttonImageBGUp;					// Colour of a buttonImage's BG when mouse is not over
			CColour buttonImageTextDown;				// Colour of a buttonImage's text when mouse is over and clicked
			CColour buttonImageTextOver;				// Colour of a buttonImage's text when mouse is over
			CColour buttonImageTextUp;					// Colour of a buttonImage's text when mouse is not over
			CColour checkboxBGOff;						// Colour of a CUICheckbox's OFF state
			CColour checkboxBGOffOver;					// Colour of a CUICheckbox's OFF, with mouse over state
			CColour checkboxBGOn;						// Colour of a CUICheckbox's ON state
			CColour checkboxBGOnOver;					// Colour of a CUICheckbox's ON, with mouse over state
			CColour linegraphBG;						// CUILineGraph's background vertex colour
			CColour mouseLight;							// Colour emitted from the mouse cursor
			CColour progressBarBG;						// CUIProgressBar's background vertex colour
			CColour progressBarFiller;					// CUIProgressBar's filler vertex colour
			CColour scrollbarBG;						// CUIScrollbar's  BG colour.
			CColour scrollbarTabNotOver;				// CUIScrollbar's tab colour when mouse is not over
			CColour scrollbarTabOver;					// CUIScrollbar's tab colour when mouse is over
			CColour text;								// CUIText font colour
			CColour textEditBGActive;
			CColour textEditBGInactive;
			CColour textEditTextActive;					// CUITextEdit's text colour when active
			CColour textEditTextInactive;				// CUITextEdit's text colour when inactive
//			CColour textScrollBG;						// CUITextScroll's colour
//			CColour textScrollText;						// CUITextScroll's text colour
			CColour tooltipBG;							// Background colour of a tooltip
			CColour tooltipText;						// Tooltip text colour
			CColour windowBGFocused;					// CUIWindow background vertex colour when focused
			CColour windowBGNotFocused;					// CUIWindow background vertex colour when not focused
			CColour windowTitlebarTextFocused;			// CUIWindow titlebar text colour when focused
			CColour windowTitlebarTextNotFocused;		// CUIWindow titlebar text colour when not focused
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
//			AudioSampleInfo buttonImageClicked;			// Audio sample to play when an image button is clicked
			AudioSampleInfo textEditActivate;			// Audio sample to play when a text edit box is clicked upon and activated
			AudioSampleInfo textEditBackspace;			// Audio sample to play when a text edit box is activated and the backspace key is pressed
			AudioSampleInfo textEditNoMoreCharSpace;	// Audio sample to play when a text edit box is activated, a character key is pressed an the edit box has reached it's character limit.
			AudioSampleInfo textEditReturn;				// Audio sample to play when a text edit box is activated and the return key is pressed.
			AudioSampleInfo textEditTextAdd;			// Audio sample to play when a text edit box is activated and a characted is being added to it's current string.
		};
		
		// Structure to hold the names of all the cursor files for the mouse cursor
		// The SCUIManager uses these to set the operating system's mouse cursor.
		struct SMouseCursors
		{
			std::string busy;							// .ani file to set the mouse cursor to when things are busy
			std::string normal;							// .ani file to set the mouse cursor to when things are normal
			std::string resize_LtoR;					// .ani file to set the mouse cursor to when a UI window maybe/is being resized.
			std::string resize_TLtoBR;					// .ani file to set the mouse cursor to when a UI window maybe/is being resized.
			std::string resize_TRtoBL;					// .ani file to set the mouse cursor to when a UI window maybe/is being resized.
			std::string resize_TtoB;					// .ani file to set the mouse cursor to when a UI window maybe/is being resized.
		};

		// Structure to hold all floating point settings
		struct SFloats
		{
			float buttonFadeSpeed;					// Rate at which button text and BG colours interpolate between
			float buttonImageFadeSpeed;				// Rate at which button text and image colours interpolate between
//			float checkboxFadeSpeed;				// Rate at which the checkbox fades between ON and OFF states
			float colourSelectorTotalWidth;			// Total width of colour selector widget
			float colourSelectorYspacing;			// Spacing between widgets along Y axis
			float normalMouseCursorDistance;		// Distance the mouse cursor is from the fragments when computing the bump mapping.
			float scrollbarTabFadeSpeed;			// Rate at which scrollbar's tab colours interpolate between
			float textEditFlashSpeed;				// Rate at which the additional character that flashes when a text edit is active.
//			float textScrollScrollbarWidth;			// Width of a CUITextScroll object's vertical scrollbar.
			float tooltipFadeSpeed;					// Rate at which tooltips fade in/out
			float windowResizeHandleOffsetX;		// The distance from a window's borders along X where clicking the mouse can begin window resizing.
			float windowResizeHandleOffsetY;		// The distance from a window's borders along Y where clicking the mouse can begin window resizing.
			float windowScrollbarHorizontalHeight;	// Height of a window's horizontal scrollbar
			float windowScrollbarVerticalWidth;		// Width of a window's vertical scrollbar
		};

		// Structure to hold all the CVector2f settings
		struct SVector2fs
		{
			CVector2f colourSelectorTextEditDims;	// Dimensions of the CUIColourSelector widget's text edit widget.
			CVector2f tooltipOffsetFromCursor;		// Offset of tooltips from the position of the mouse cursor
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
			SMouseCursors cursors;		// Holds all the mouse cursor file names used by the theme
			SVector2fs vectors;			// Holds all the CVector2f settings
			std::string themeName;		// Holds the name of the theme.
		};
	private:
		SSettings _mSettings;
		bool _mbResourcesAdded;			// Whether resources have been added to the managers or not

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

		// Helper method to determine whether each of the images have the correct dimensions.
		// In the 3x3 grid...
		// The 3 images in the top row must have the same height.
		// The 3 images in the bottom row must have the same height.
		// The 3 images on the left edge must have the same width.
		// The 3 images on the right edge must have the same width.
		// The centre image can be whatever dimensions it likes.
		// Throws an exception is image dimensions are invalid
		// Note: the image atlas must exist before calling this.
		void _helperCheckImageDimsAreOK(void);

		// Helper method to determine whether the given image type images have the correct dimensions.
		// Called from _helpCheckImageDimsAreOK(void)
		// Throws an exception is image dimensions are invalid
		void _helperCheckImageDimsAreOK(SImageType& imageType, const std::string& strWidgetName);

		// Builds or rebuilds normal map images from height images
		void _helperBuildNormalImages(bool bOverwriteNormalImages);

		// Builds or rebuilds normal map images from height images for the given SImageType
		void _helperBuildNormalImages(SImageType& imageType, bool bOverwriteNormalImages);
	};
}