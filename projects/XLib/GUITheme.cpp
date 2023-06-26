#include "PCH.h"
#include "GUITheme.h"
#include "utilities.h"
#include "log.h"
#include "resourceManager.h"
#include "audioManager.h"

namespace X
{
	GUITheme::GUITheme()
	{
		mstrThemeName = "Default theme";

		// Images
		mImages.containerColour = "data/X/GUI/default/container_colour.png";
		mImages.containerNormal = "data/X/GUI/default/container_normal.png";
		mImages.buttonColour = "data/X/GUI/default/button_colour.png";
		mImages.buttonNormal = "data/X/GUI/default/button_normal.png";
		mImages.reflection = "data/X/GUI/default/reflection.png";
		mImages.textEditColour = "data/X/GUI/default/textedit_colour.png";
		mImages.textEditNormal = "data/X/GUI/default/textedit_normal.png";
		mImages.sliderBackColour = "data/X/GUI/default/slider_back_colour.png";
		mImages.sliderBackNormal = "data/X/GUI/default/slider_back_normal.png";
		mImages.sliderTabColour = "data/X/GUI/default/slider_tab_colour.png";
		mImages.sliderTabNormal = "data/X/GUI/default/slider_tab_normal.png";

		// Fonts
		mFonts.containerTitle = "data/X/fonts/ultimateserial-medium-regular_20";
		mFonts.button = "data/X/fonts/ultimateserial-medium-regular_20";
		mFonts.text = "data/X/fonts/ultimateserial-medium-regular_20";
		mFonts.textEdit = "data/X/fonts/ultimateserial-medium-regular_20";

		// Colours
		mColours.containerTitlebarTextInFocus.set(1.0f, 1.0f, 1.0f, 1.0f);
		mColours.containerTitlebarTextNotInFocus.set(0.9f, 0.9f, 0.9f, 0.5f);
		mColours.buttonTextDown.set(1.0f, 1.0f, 1.0f, 1.0f);
		mColours.buttonTextOver.set(1.0f, 1.0f, 1.0f, 0.75f);
		mColours.buttonTextUp.set(1.0f, 1.0f, 1.0f, 0.5f);
		mColours.text.set(1.0f, 1.0f, 1.0f, 0.75f);
		mColours.textEditActive.set(1.0f, 1.0f, 1.0f, 1.0f);
		mColours.textEditInactive.set(1.0f, 1.0f, 1.0f, 0.75f);
		mColours.sliderTabNotOver.set(1.0f, 1.0f, 1.0f, 0.5f);
		mColours.sliderTabOver.set(1.0f, 1.0f, 1.0f, 1.0f);

		// Floats
		mfBlurAmount = 1.0f;
		mfNormalAmount = 0.1f;
		mfReflectionAmount = 0.025f;
		mfMouseCursorDistance = 50.0f;
		mfButtonTextFadeSpeed = 5.0f;
		mfTextEditFlashSpeed = 4.0f;
		mfSliderTabFadeSpeed = 5.0f;

		// Offsets
		mOffsets.containerTitlebarText.iOffsetX = 1;
		mOffsets.containerTitlebarText.iOffsetY = 10;
		mOffsets.textEditText.iOffsetX = 1;
		mOffsets.textEditText.iOffsetY = 1;

		// Audio
		mAudio.buttonClicked.strSampleName = "data/X/gui/default/click.wav";			mAudio.buttonClicked.fPitch = 1.0f;		mAudio.buttonClicked.fVolume = 0.8f;
		mAudio.textEditTextAdd.strSampleName = "data/X/gui/default/click2.wav";			mAudio.textEditTextAdd.fPitch = 1.0f;	mAudio.textEditTextAdd.fVolume = 0.8f;
		mAudio.textEditBackspace.strSampleName = "data/X/gui/default/backspace.wav";	mAudio.textEditBackspace.fPitch = 1.0f;	mAudio.textEditBackspace.fVolume = 0.2f;
		mAudio.textEditReturn.strSampleName = "data/X/gui/default/return.wav";			mAudio.textEditReturn.fPitch = 1.0f;	mAudio.textEditReturn.fVolume = 0.8f;
		mAudio.textEditActivate.strSampleName = "data/X/gui/default/activate.wav";		mAudio.textEditActivate.fPitch = 1.0f;	mAudio.textEditActivate.fVolume = 0.8f;
	}

	void GUITheme::load(const std::string& strFilename)
	{
		// Make sure filename given has the required extension and set to lowercase
		std::string filename = addFilenameExtension(".theme", strFilename);

		// Open file in text mode
		std::ifstream file;
		file.open(filename, std::ofstream::in);
		ThrowIfFalse(file.is_open(), "GUITheme::load(" + filename + ") failed to open file for reading.");

		// Filename
		std::string strWord;
		std::string strLine;
		char space;
		getline(file, strLine);

		// Theme name
		file >> strWord;	// "ThemeName:"
		space = file.get();	// Space
		getline(file, mstrThemeName);	// Themename

		// Images
		getline(file, strLine);	// Empty line
		getline(file, strLine);	//"\nImages\n"
		_readImageInfo(file, mImages.containerColour);
		_readImageInfo(file, mImages.containerNormal);
		_readImageInfo(file, mImages.buttonColour);
		_readImageInfo(file, mImages.buttonNormal);
		_readImageInfo(file, mImages.reflection);
		_readImageInfo(file, mImages.textEditColour);
		_readImageInfo(file, mImages.textEditNormal);
		_readImageInfo(file, mImages.sliderBackColour);
		_readImageInfo(file, mImages.sliderBackNormal);
		_readImageInfo(file, mImages.sliderTabColour);
		_readImageInfo(file, mImages.sliderTabNormal);

		// Fonts
		getline(file, strLine);	// Empty line
		getline(file, strLine);	//"\nFonts\n"
		_readFontInfo(file, mFonts.containerTitle);
		_readFontInfo(file, mFonts.button);
		_readFontInfo(file, mFonts.text);
		_readFontInfo(file, mFonts.textEdit);

		// Colours
		getline(file, strLine);	// Empty line
		getline(file, strLine);	//"\nColours\n"
		_readColourInfo(file, mColours.containerTitlebarTextInFocus);
		_readColourInfo(file, mColours.containerTitlebarTextNotInFocus);
		_readColourInfo(file, mColours.buttonTextDown);
		_readColourInfo(file, mColours.buttonTextOver);
		_readColourInfo(file, mColours.buttonTextUp);
		_readColourInfo(file, mColours.text);
		_readColourInfo(file, mColours.textEditActive);
		_readColourInfo(file, mColours.textEditInactive);
		_readColourInfo(file, mColours.sliderTabNotOver);
		_readColourInfo(file, mColours.sliderTabOver);

		// Floats
		getline(file, strLine);	// End of line
		getline(file, strLine);	// Empty line
		getline(file, strLine);	//"\nEffects\n"
		_readFloatInfo(file, mfBlurAmount);
		_readFloatInfo(file, mfNormalAmount);
		_readFloatInfo(file, mfReflectionAmount);
		_readFloatInfo(file, mfMouseCursorDistance);
		_readFloatInfo(file, mfButtonTextFadeSpeed);
		_readFloatInfo(file, mfTextEditFlashSpeed);
		_readFloatInfo(file, mfSliderTabFadeSpeed);
		
		// Offsets
		getline(file, strLine);	// End of line
		getline(file, strLine);	// Empty line
		getline(file, strLine);	//"\nOffsets\n"
		_readOffsetInfo(file, mOffsets.containerTitlebarText);
		_readOffsetInfo(file, mOffsets.textEditText);

		// Audio
		getline(file, strLine);	// End of line
		getline(file, strLine);	// Empty line
		getline(file, strLine);	//"\nAudio\n"
		_readAudioSampleInfo(file, mAudio.buttonClicked);
		_readAudioSampleInfo(file, mAudio.textEditTextAdd);
		_readAudioSampleInfo(file, mAudio.textEditBackspace);
		_readAudioSampleInfo(file, mAudio.textEditReturn);
		_readAudioSampleInfo(file, mAudio.textEditActivate);

		ThrowIfTrue(file.fail(), "GUITheme::load(" + filename + ") failed whilst loading file.");
		file.close();
	}

	void GUITheme::save(const std::string& strFilename)
	{
		// Make sure filename given has the required extension and set to lowercase
		std::string filename = addFilenameExtension(".theme", strFilename);

		// Open file in text mode, deleting contents if existed before
		std::ofstream file;
		file.open(filename, std::ofstream::out | std::ofstream::trunc);
		ThrowIfFalse(file.is_open(), "GUITheme::save(" + filename + ") failed to open file for writing.");

		// Filename
		file << filename + "\n";

		// Theme name
		file << "ThemeName: " + mstrThemeName + "\n";

		// Images
		file << "\nImages\n";
		_writeImageInfo(file, "ContainerColour", mImages.containerColour);
		_writeImageInfo(file, "ContainerNormal", mImages.containerNormal);
		_writeImageInfo(file, "ButtonColour", mImages.buttonColour);
		_writeImageInfo(file, "ButtonNormal", mImages.buttonNormal);
		_writeImageInfo(file, "Reflection", mImages.reflection);
		_writeImageInfo(file, "TextEditColour", mImages.textEditColour);
		_writeImageInfo(file, "TextEditNormal", mImages.textEditNormal);
		_writeImageInfo(file, "SliderBackColour", mImages.sliderBackColour);
		_writeImageInfo(file, "SliderBackNormal", mImages.sliderBackNormal);
		_writeImageInfo(file, "SliderTabColour", mImages.sliderTabColour);
		_writeImageInfo(file, "SliderTabNormal", mImages.sliderTabNormal);

		// Fonts
		file << "\nFonts\n";
		_writeFontInfo(file, "containerTitle", mFonts.containerTitle);
		_writeFontInfo(file, "button", mFonts.button);
		_writeFontInfo(file, "text", mFonts.text);
		_writeFontInfo(file, "textEdit", mFonts.textEdit);

		// Colours
		file << "\nColours\n";
		_writeColourInfo(file, "containerTitlebarTextInFocus", mColours.containerTitlebarTextInFocus);
		_writeColourInfo(file, "containerTitlebarTextNotInFocus", mColours.containerTitlebarTextNotInFocus);
		_writeColourInfo(file, "buttonTextDown", mColours.buttonTextDown);
		_writeColourInfo(file, "buttonTextOver", mColours.buttonTextOver);
		_writeColourInfo(file, "buttonTextUp", mColours.buttonTextUp);
		_writeColourInfo(file, "text", mColours.text);
		_writeColourInfo(file, "textEditActive", mColours.textEditActive);
		_writeColourInfo(file, "textEditInactive", mColours.textEditInactive);
		_writeColourInfo(file, "textEditInactive", mColours.sliderTabNotOver);
		_writeColourInfo(file, "textEditInactive", mColours.sliderTabOver);

		// Floats
		file << "\nFloats\n";
		_writeFloatInfo(file, "blurAmount", mfBlurAmount);
		_writeFloatInfo(file, "normalAmount", mfNormalAmount);
		_writeFloatInfo(file, "reflectionAmount", mfReflectionAmount);
		_writeFloatInfo(file, "mouseCursorDistance", mfMouseCursorDistance);
		_writeFloatInfo(file, "buttonTextFadeSpeed", mfButtonTextFadeSpeed);
		_writeFloatInfo(file, "textEditCursorFlashSpeed", mfTextEditFlashSpeed);
		_writeFloatInfo(file, "sliderTabFadeSpeed", mfSliderTabFadeSpeed);
		
		// Offsets
		file << "\nOffsets\n";
		_writeOffsetInfo(file, "ContainerTitlebarText", mOffsets.containerTitlebarText);
		_writeOffsetInfo(file, "TextEditText", mOffsets.textEditText);

		// Audio
		file << "\nAudio\n";
		_writeAudioSampleInfo(file, "buttonClicked", mAudio.buttonClicked);
		_writeAudioSampleInfo(file, "textEditTextAdd", mAudio.textEditTextAdd);
		_writeAudioSampleInfo(file, "textEditBackspace", mAudio.textEditBackspace);
		_writeAudioSampleInfo(file, "textEditReturn", mAudio.textEditReturn);
		_writeAudioSampleInfo(file, "textEditActivate", mAudio.textEditActivate);

		// Make sure there were no errors
		ThrowIfTrue(file.fail(), "GUITheme::save(" + filename + ") failed whilst saving file.");
		file.close();
	}

	void GUITheme::loadTextures(void)
	{
		ResourceManager* pRM = ResourceManager::getPointer();
		pRM->addTexture2D(mImages.containerColour, mImages.containerColour);
		pRM->addTexture2D(mImages.containerNormal, mImages.containerNormal);
		pRM->addTexture2D(mImages.buttonColour, mImages.buttonColour);
		pRM->addTexture2D(mImages.buttonNormal, mImages.buttonNormal);
		pRM->addTexture2D(mImages.reflection, mImages.reflection);
		pRM->addTexture2D(mImages.textEditColour, mImages.textEditColour);
		pRM->addTexture2D(mImages.textEditNormal, mImages.textEditNormal);
		pRM->addTexture2D(mImages.sliderBackColour, mImages.sliderBackColour);
		pRM->addTexture2D(mImages.sliderBackNormal, mImages.sliderBackNormal);
		pRM->addTexture2D(mImages.sliderTabColour, mImages.sliderTabColour);
		pRM->addTexture2D(mImages.sliderTabNormal, mImages.sliderTabNormal);
	}

	void GUITheme::unloadTextures(void)
	{
		ResourceManager* pRM = ResourceManager::getPointer();
		pRM->removeTexture2D(mImages.containerColour);
		pRM->removeTexture2D(mImages.containerNormal);
		pRM->removeTexture2D(mImages.buttonColour);
		pRM->removeTexture2D(mImages.buttonNormal);
		pRM->removeTexture2D(mImages.reflection);
		pRM->removeTexture2D(mImages.textEditColour);
		pRM->removeTexture2D(mImages.textEditNormal);
		pRM->removeTexture2D(mImages.sliderBackColour);
		pRM->removeTexture2D(mImages.sliderBackNormal);
		pRM->removeTexture2D(mImages.sliderTabColour);
		pRM->removeTexture2D(mImages.sliderTabNormal);
	}

	void GUITheme::addFontsToManager(void)
	{
		ResourceManager* pRM = ResourceManager::getPointer();
		pRM->addFont(mFonts.button, mFonts.containerTitle);
		pRM->addFont(mFonts.button, mFonts.button);
		pRM->addFont(mFonts.text, mFonts.text);
		pRM->addFont(mFonts.textEdit, mFonts.textEdit);
	}

	void GUITheme::removeFontsFromManager(void)
	{
		ResourceManager* pRM = ResourceManager::getPointer();
		pRM->removeFont(mFonts.containerTitle);
		pRM->removeFont(mFonts.button);
		pRM->removeFont(mFonts.text);
		pRM->removeFont(mFonts.textEdit);
	}

	void GUITheme::addAudioToManager(void)
	{
		AudioManager* pAM = AudioManager::getPointer();
		pAM->addSample(mAudio.buttonClicked.strSampleName, "gui");		pAM->addEmitter(mAudio.buttonClicked.strSampleName, mAudio.buttonClicked.strSampleName, 4, "gui");
		pAM->addSample(mAudio.textEditTextAdd.strSampleName, "gui");	pAM->addEmitter(mAudio.textEditTextAdd.strSampleName, mAudio.textEditTextAdd.strSampleName, 4, "gui");
		pAM->addSample(mAudio.textEditBackspace.strSampleName, "gui");	pAM->addEmitter(mAudio.textEditBackspace.strSampleName, mAudio.textEditBackspace.strSampleName, 4, "gui");
		pAM->addSample(mAudio.textEditReturn.strSampleName, "gui");		pAM->addEmitter(mAudio.textEditReturn.strSampleName, mAudio.textEditReturn.strSampleName, 4, "gui");
		pAM->addSample(mAudio.textEditActivate.strSampleName, "gui");	pAM->addEmitter(mAudio.textEditActivate.strSampleName, mAudio.textEditActivate.strSampleName, 4, "gui");
		
		pAM->loadSampleGroup("gui");
	}

	void GUITheme::removeAudioFromManager(void)
	{
		AudioManager* pAM = AudioManager::getPointer();
		pAM->unloadSampleGroup("gui");
		pAM->removeSample(mAudio.buttonClicked.strSampleName, "gui");		pAM->removeEmitter(mAudio.buttonClicked.strSampleName);
		pAM->removeSample(mAudio.textEditTextAdd.strSampleName, "gui");		pAM->removeEmitter(mAudio.textEditTextAdd.strSampleName);
		pAM->removeSample(mAudio.textEditBackspace.strSampleName, "gui");	pAM->removeEmitter(mAudio.textEditBackspace.strSampleName);
		pAM->removeSample(mAudio.textEditReturn.strSampleName, "gui");		pAM->removeEmitter(mAudio.textEditReturn.strSampleName);
		pAM->removeSample(mAudio.textEditActivate.strSampleName, "gui");	pAM->removeEmitter(mAudio.textEditActivate.strSampleName);
	}

	void GUITheme::_writeImageInfo(std::ofstream& stream, const std::string& strDescription, const std::string& strImagename)
	{
		stream << strDescription + ": " + strImagename + "\n";
	}

	void GUITheme::_readImageInfo(std::ifstream& stream, std::string& strImagename)
	{
		std::string strWord;
		char space;
		stream >> strWord;
		space = stream.get();
		getline(stream, strImagename);
	}

	void GUITheme::_writeFontInfo(std::ofstream& stream, const std::string& strDescription, const std::string& strFontname)
	{
		stream << strDescription + ": " + strFontname + "\n";
	}

	void GUITheme::_readFontInfo(std::ifstream& stream, std::string& strFontname)
	{
		std::string strWord;
		char space;
		stream >> strWord;
		space = stream.get();
		getline(stream, strFontname);
	}

	void GUITheme::_writeColourInfo(std::ofstream& stream, const std::string& strDescription, const GUIColour& colour)
	{
		std::string str;
		str = std::to_string(colour.red) + " " + std::to_string(colour.green) + " " + std::to_string(colour.blue) + " " + std::to_string(colour.alpha);
		stream << strDescription + ": " + str + "\n";
	}

	void GUITheme::_readColourInfo(std::ifstream& stream, GUIColour& colour)
	{
		static char space;			// For storing a space character
		static std::string strWord;	// For storing a single word
		stream >> strWord;			// Description text (For example, containerTitlebarTextInFocus:)
		space = stream.get();		// Space character

		// Each colour value
		stream >> colour.red;
		stream >> colour.green;
		stream >> colour.blue;
		stream >> colour.alpha;
	}

	void GUITheme::_writeOffsetInfo(std::ofstream& stream, const std::string& strDescription, const Offset& offset)
	{
		std::string str;
		str = std::to_string(offset.iOffsetX) + " " + std::to_string(offset.iOffsetY);
		stream << strDescription + ": " + str + "\n";
	}

	void GUITheme::_readOffsetInfo(std::ifstream& stream, Offset& offset)
	{
		static char space;			// For storing a space character
		static std::string strWord;	// For storing a single word
		stream >> strWord;			// Description text (For example, containerTitlebarTextInFocus:)
		space = stream.get();		// Space character

		// Each colour value
		stream >> offset.iOffsetX;
		space = stream.get();		// Space character
		stream >> offset.iOffsetY;
	}

	void GUITheme::_writeAudioSampleInfo(std::ofstream& stream, const std::string& strDescription, const AudioSampleInfo& asi)
	{
		stream << strDescription + "Name: " + mAudio.buttonClicked.strSampleName + "\n";
		stream << strDescription + "VolPitch: " << std::to_string(asi.fVolume) + " " + std::to_string(asi.fPitch);
	}

	void GUITheme::_readAudioSampleInfo(std::ifstream& stream, AudioSampleInfo& asi)
	{
		std::string strWord;
		char space;
		stream >> strWord;
		space = stream.get();
		stream >> asi.strSampleName;
		stream >> strWord;
		space = stream.get();
		stream >> asi.fVolume;
		space = stream.get();
		stream >> asi.fPitch;
	}

	void GUITheme::_writeFloatInfo(std::ofstream& stream, const std::string& strDescription, const float& fValue)
	{
		stream << strDescription + ": " + std::to_string(fValue) + "\n";
	}

	void GUITheme::_readFloatInfo(std::ifstream& stream, float& fValue)
	{
		std::string strWord;
		char space;
		stream >> strWord;
		space = stream.get();
		stream >> fValue;
	}
}