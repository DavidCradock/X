#include "PCH.h"
#include "GUITheme.h"
#include "stringUtils.h"
#include "log.h"
#include "resourceManager.h"
#include "audioManager.h"

namespace X
{
	CGUITheme::CGUITheme()
	{
		mstrThemeName = "Default theme";

		// Images
		mImages.containerBGColour = "data/X/GUI/default/containerBGcolour.png";
		mImages.containerBGNormal = "data/X/GUI/default/containerBGnormal.png";
		mImages.buttonBGColour = "data/X/GUI/default/buttonBGcolour.png";
		mImages.buttonBGNormal = "data/X/GUI/default/buttonBGnormal.png";
		mImages.reflection = "data/X/GUI/default/reflection.png";
		mImages.textEditBGColour = "data/X/GUI/default/texteditBGcolour.png";
		mImages.textEditBGNormal = "data/X/GUI/default/texteditBGnormal.png";
		mImages.sliderBGColour = "data/X/GUI/default/sliderBGcolour.png";
		mImages.sliderBGNormal = "data/X/GUI/default/sliderBGnormal.png";
		mImages.sliderTabColour = "data/X/GUI/default/slider_tab_colour.png";
		mImages.sliderTabNormal = "data/X/GUI/default/slider_tab_normal.png";
		mImages.lineGraphBGColour = "data/X/GUI/default/linegraphBGcolour.png";
		mImages.lineGraphBGNormal = "data/X/GUI/default/linegraphBGnormal.png";
		mImages.tooltipBGColour = "data/X/GUI/default/tooltipBGcolour.png";
		mImages.progressBarBGColour = "data/X/GUI/default/progressBarBGcolour.png";
		mImages.progressBarBGNormal = "data/X/GUI/default/progressBarBGnormal.png";
		mImages.progressBarFillerColour = "data/X/GUI/default/progressBarFillercolour.png";
		mImages.progressBarFillerNormal = "data/X/GUI/default/progressBarFillernormal.png";
		mImages.textScrollBGColour = "data/X/GUI/default/textscrollBGcolour.png";
		mImages.textScrollBGNormal = "data/X/GUI/default/textscrollBGnormal.png";
		mImages.buttonImageBGColour = "data/X/GUI/default/buttonImageBGcolour.png";
		mImages.buttonImageBGNormal = "data/X/GUI/default/buttonImageBGnormal.png";

		// Fonts
		mFonts.containerTitle = "data/X/fonts/satoshi_22";
		mFonts.button = "data/X/fonts/satoshi_22";
		mFonts.text = "data/X/fonts/satoshi_22";
		mFonts.textEdit = "data/X/fonts/satoshi_22";
		mFonts.textScroll = "data/X/fonts/satoshi_22";

		// Colours
		mColours.containerTitlebarTextInFocus.set(0.9f, 0.9f, 0.9f, 1.0f);
		mColours.containerTitlebarTextNotInFocus.set(0.9f, 0.9f, 0.9f, 0.5f);
		mColours.buttonTextDown.set(0.9f, 0.9f, 0.9f, 1.0f);
		mColours.buttonTextOver.set(0.9f, 0.9f, 0.9f, 0.75f);
		mColours.buttonTextUp.set(0.9f, 0.9f, 0.9f, 0.5f);
		mColours.buttonBGDown.set(1.0f, 1.0f, 1.0f, 0.5f);
		mColours.buttonBGOver.set(0.9f, 0.9f, 0.9f, 1.0f);
		mColours.buttonBGUp.set(1.0f, 1.0f, 1.0f, 1.0f);
		mColours.text.set(0.7f, 0.7f, 0.7f, 1.0f);
		mColours.textEditActive.set(0.0f, 0.0f, 0.0f, 1.0f);
		mColours.textEditInactive.set(0.0f, 0.0f, 0.0f, 0.75f);
		mColours.sliderTabNotOver.set(1.0f, 1.0f, 1.0f, 0.5f);
		mColours.sliderTabOver.set(1.0f, 0.5f, 0.0f, 1.0f);
		mColours.progressBarFiller.set(1.0f, 1.0f, 1.0f, 1.0f);
		mColours.buttonImageDown.set(1.0f, 1.0f, 1.0f, 1.0f);
		mColours.buttonImageOver.set(1.0f, 1.0f, 1.0f, 0.75f);
		mColours.buttonImageUp.set(1.0f, 1.0f, 1.0f, 0.5f);
		mColours.textScrollBackground.set(0.1f, 0.1f, 0.1f, 1.0f);
		mColours.textScrollText.set(0.7f, 0.7f, 0.7f, 1.0f);
		mColours.tooltipText.set(0.7f, 0.7f, 0.7f, 1.0f);

		// Floats
		mfNormalAmount = 0.1f;
		mfReflectionAmount = 0.025f;
		mfMouseCursorDistance = 50.0f;
		mfButtonFadeSpeed = 5.0f;
		mfTextEditFlashSpeed = 4.0f;
		mfSliderTabFadeSpeed = 5.0f;
		mfTextScrollSliderWidth = 20.0f;
		mfButtonImageTextFadeSpeed = 5.0f;
		mfTooltipFadeSpeed = 2.0f;

		// Offsets
		mOffsets.containerTitlebarText.iOffsetX = 1;
		mOffsets.containerTitlebarText.iOffsetY = 10;
		mOffsets.textEditText.iOffsetX = 1;
		mOffsets.textEditText.iOffsetY = 1;

		// Audio
		mAudio.buttonClicked.strSampleName = "data/X/gui/default/click.wav";			mAudio.buttonClicked.fPitch = 1.0f;				mAudio.buttonClicked.fVolume = 0.8f;
		mAudio.textEditTextAdd.strSampleName = "data/X/gui/default/click2.wav";			mAudio.textEditTextAdd.fPitch = 1.0f;			mAudio.textEditTextAdd.fVolume = 0.8f;
		mAudio.textEditBackspace.strSampleName = "data/X/gui/default/backspace.wav";	mAudio.textEditBackspace.fPitch = 1.0f;			mAudio.textEditBackspace.fVolume = 0.2f;
		mAudio.textEditReturn.strSampleName = "data/X/gui/default/return.wav";			mAudio.textEditReturn.fPitch = 1.0f;			mAudio.textEditReturn.fVolume = 0.8f;
		mAudio.textEditActivate.strSampleName = "data/X/gui/default/activate.wav";		mAudio.textEditActivate.fPitch = 1.0f;			mAudio.textEditActivate.fVolume = 0.8f;
		mAudio.textEditNoMoreCharSpace.strSampleName = "data/X/gui/default/error.wav";	mAudio.textEditNoMoreCharSpace.fPitch = 1.0f;	mAudio.textEditNoMoreCharSpace.fVolume = 0.2f;
		mAudio.buttonImageClicked.strSampleName = "data/X/gui/default/click3.wav";		mAudio.buttonImageClicked.fPitch = 1.0f;		mAudio.buttonImageClicked.fVolume = 0.8f;
	}

	void CGUITheme::load(const std::string& strFilename)
	{
		// Make sure filename given has the required extension and set to lowercase
		std::string filename = StringUtils::addFilenameExtension(".theme", strFilename);

		// Open file in text mode
		std::ifstream file;
		file.open(filename, std::ofstream::in);
		ThrowIfFalse(file.is_open(), "CGUITheme::load(" + filename + ") failed to open file for reading.");

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
		_readImageInfo(file, mImages.containerBGColour);
		_readImageInfo(file, mImages.containerBGNormal);
		_readImageInfo(file, mImages.buttonBGColour);
		_readImageInfo(file, mImages.buttonBGNormal);
		_readImageInfo(file, mImages.reflection);
		_readImageInfo(file, mImages.textEditBGColour);
		_readImageInfo(file, mImages.textEditBGNormal);
		_readImageInfo(file, mImages.sliderBGColour);
		_readImageInfo(file, mImages.sliderBGNormal);
		_readImageInfo(file, mImages.sliderTabColour);
		_readImageInfo(file, mImages.sliderTabNormal);
		_readImageInfo(file, mImages.lineGraphBGColour);
		_readImageInfo(file, mImages.lineGraphBGNormal);
		_readImageInfo(file, mImages.tooltipBGColour);
		_readImageInfo(file, mImages.progressBarBGColour);
		_readImageInfo(file, mImages.progressBarBGNormal);
		_readImageInfo(file, mImages.progressBarFillerColour);
		_readImageInfo(file, mImages.progressBarFillerNormal);
		_readImageInfo(file, mImages.textScrollBGColour);
		_readImageInfo(file, mImages.textScrollBGNormal);
		_readImageInfo(file, mImages.buttonImageBGColour);
		_readImageInfo(file, mImages.buttonImageBGNormal);

		// Fonts
		getline(file, strLine);	// Empty line
		getline(file, strLine);	//"\nFonts\n"
		_readFontInfo(file, mFonts.containerTitle);
		_readFontInfo(file, mFonts.button);
		_readFontInfo(file, mFonts.text);
		_readFontInfo(file, mFonts.textEdit);
		_readFontInfo(file, mFonts.textScroll);

		// Colours
		getline(file, strLine);	// Empty line
		getline(file, strLine);	//"\nColours\n"
		_readColourInfo(file, mColours.containerTitlebarTextInFocus);
		_readColourInfo(file, mColours.containerTitlebarTextNotInFocus);
		_readColourInfo(file, mColours.buttonTextDown);
		_readColourInfo(file, mColours.buttonTextOver);
		_readColourInfo(file, mColours.buttonTextUp);
		_readColourInfo(file, mColours.buttonBGDown);
		_readColourInfo(file, mColours.buttonBGOver);
		_readColourInfo(file, mColours.buttonBGUp);
		_readColourInfo(file, mColours.text);
		_readColourInfo(file, mColours.textEditActive);
		_readColourInfo(file, mColours.textEditInactive);
		_readColourInfo(file, mColours.sliderTabNotOver);
		_readColourInfo(file, mColours.sliderTabOver);
		_readColourInfo(file, mColours.progressBarFiller);
		_readColourInfo(file, mColours.buttonImageDown);
		_readColourInfo(file, mColours.buttonImageOver);
		_readColourInfo(file, mColours.buttonImageUp);
		_readColourInfo(file, mColours.textScrollBackground);
		_readColourInfo(file, mColours.textScrollText);
		_readColourInfo(file, mColours.tooltipText);

		// Floats
		getline(file, strLine);	// End of line
		getline(file, strLine);	// Empty line
		getline(file, strLine);	//"\nEffects\n"
		_readFloatInfo(file, mfNormalAmount);
		_readFloatInfo(file, mfReflectionAmount);
		_readFloatInfo(file, mfMouseCursorDistance);
		_readFloatInfo(file, mfButtonFadeSpeed);
		_readFloatInfo(file, mfTextEditFlashSpeed);
		_readFloatInfo(file, mfSliderTabFadeSpeed);
		_readFloatInfo(file, mfTextScrollSliderWidth);
		_readFloatInfo(file, mfButtonImageTextFadeSpeed);
		_readFloatInfo(file, mfTooltipFadeSpeed);

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
		_readAudioSampleInfo(file, mAudio.textEditNoMoreCharSpace);
		_readAudioSampleInfo(file, mAudio.buttonImageClicked);

		ThrowIfTrue(file.fail(), "CGUITheme::load(" + filename + ") failed whilst loading file.");
		file.close();
	}

	void CGUITheme::save(const std::string& strFilename)
	{
		// Make sure filename given has the required extension and set to lowercase
		std::string filename = StringUtils::addFilenameExtension(".theme", strFilename);

		// Open file in text mode, deleting contents if existed before
		std::ofstream file;
		file.open(filename, std::ofstream::out | std::ofstream::trunc);
		ThrowIfFalse(file.is_open(), "CGUITheme::save(" + filename + ") failed to open file for writing.");

		// Filename
		file << filename + "\n";

		// Theme name
		file << "ThemeName: " + mstrThemeName + "\n";

		// Images
		file << "\nImages\n";
		_writeImageInfo(file, "ContainerBGColour", mImages.containerBGColour);
		_writeImageInfo(file, "ContainerBGNormal", mImages.containerBGNormal);
		_writeImageInfo(file, "ButtonBGColour", mImages.buttonBGColour);
		_writeImageInfo(file, "ButtonBGNormal", mImages.buttonBGNormal);
		_writeImageInfo(file, "Reflection", mImages.reflection);
		_writeImageInfo(file, "TextEditBGColour", mImages.textEditBGColour);
		_writeImageInfo(file, "TextEditBGNormal", mImages.textEditBGNormal);
		_writeImageInfo(file, "SliderBGColour", mImages.sliderBGColour);
		_writeImageInfo(file, "SliderBGNormal", mImages.sliderBGNormal);
		_writeImageInfo(file, "SliderTabColour", mImages.sliderTabColour);
		_writeImageInfo(file, "SliderTabNormal", mImages.sliderTabNormal);
		_writeImageInfo(file, "LineGraphBGColour", mImages.lineGraphBGColour);
		_writeImageInfo(file, "LineGraphBGNormal", mImages.lineGraphBGNormal);
		_writeImageInfo(file, "TooltipBGColour", mImages.tooltipBGColour);
		_writeImageInfo(file, "ProgressBarBGColour", mImages.progressBarBGColour);
		_writeImageInfo(file, "ProgressBarBGNormal", mImages.progressBarBGNormal);
		_writeImageInfo(file, "ProgressBarFillerColour", mImages.progressBarFillerColour);
		_writeImageInfo(file, "ProgressBarFillerNormal", mImages.progressBarFillerNormal);
		_writeImageInfo(file, "TextScrollBGColour", mImages.textScrollBGColour);
		_writeImageInfo(file, "TextScrollBGNormal", mImages.textScrollBGNormal);
		_writeImageInfo(file, "ButtonImageBGColour", mImages.buttonImageBGColour);
		_writeImageInfo(file, "ButtonImageBGNormal", mImages.buttonImageBGNormal);

		// Fonts
		file << "\nFonts\n";
		_writeFontInfo(file, "containerTitle", mFonts.containerTitle);
		_writeFontInfo(file, "button", mFonts.button);
		_writeFontInfo(file, "text", mFonts.text);
		_writeFontInfo(file, "textEdit", mFonts.textEdit);
		_writeFontInfo(file, "textEdit", mFonts.textScroll);

		// Colours
		file << "\nColours\n";
		_writeColourInfo(file, "containerTitlebarTextInFocus", mColours.containerTitlebarTextInFocus);
		_writeColourInfo(file, "containerTitlebarTextNotInFocus", mColours.containerTitlebarTextNotInFocus);
		_writeColourInfo(file, "buttonTextDown", mColours.buttonTextDown);
		_writeColourInfo(file, "buttonTextOver", mColours.buttonTextOver);
		_writeColourInfo(file, "buttonTextUp", mColours.buttonTextUp);
		_writeColourInfo(file, "buttonTextDown", mColours.buttonBGDown);
		_writeColourInfo(file, "buttonTextOver", mColours.buttonBGOver);
		_writeColourInfo(file, "buttonTextUp", mColours.buttonBGUp);
		_writeColourInfo(file, "text", mColours.text);
		_writeColourInfo(file, "textEditActive", mColours.textEditActive);
		_writeColourInfo(file, "textEditInactive", mColours.textEditInactive);
		_writeColourInfo(file, "textEditInactive", mColours.sliderTabNotOver);
		_writeColourInfo(file, "textEditInactive", mColours.sliderTabOver);
		_writeColourInfo(file, "progreebarFiller", mColours.progressBarFiller);
		_writeColourInfo(file, "buttonImageDown", mColours.buttonImageDown);
		_writeColourInfo(file, "buttonImageOver", mColours.buttonImageOver);
		_writeColourInfo(file, "buttonImageUp", mColours.buttonImageUp);
		_writeColourInfo(file, "textScrollBackground", mColours.textScrollBackground);
		_writeColourInfo(file, "textScrollText", mColours.textScrollText);
		_writeColourInfo(file, "buttonImageUp", mColours.tooltipText);

		// Floats
		file << "\nFloats\n";
		_writeFloatInfo(file, "normalAmount", mfNormalAmount);
		_writeFloatInfo(file, "reflectionAmount", mfReflectionAmount);
		_writeFloatInfo(file, "mouseCursorDistance", mfMouseCursorDistance);
		_writeFloatInfo(file, "buttonFadeSpeed", mfButtonFadeSpeed);
		_writeFloatInfo(file, "textEditCursorFlashSpeed", mfTextEditFlashSpeed);
		_writeFloatInfo(file, "sliderTabFadeSpeed", mfSliderTabFadeSpeed);
		_writeFloatInfo(file, "textScrollSliderWidth", mfTextScrollSliderWidth);
		_writeFloatInfo(file, "buttonImageFadeSpeed", mfButtonImageTextFadeSpeed);
		_writeFloatInfo(file, "tooltipFadeSpeed", mfTooltipFadeSpeed);
		
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
		_writeAudioSampleInfo(file, "textEditNoMoreCharSpace", mAudio.textEditNoMoreCharSpace);
		_writeAudioSampleInfo(file, "buttonImageClicked", mAudio.buttonImageClicked);

		// Make sure there were no errors
		ThrowIfTrue(file.fail(), "CGUITheme::save(" + filename + ") failed whilst saving file.");
		file.close();
	}

	void CGUITheme::loadTextures(void)
	{
		SCResourceManager* pRM = SCResourceManager::getPointer();
		pRM->addTexture2DFromFile(mImages.containerBGColour, mImages.containerBGColour);
		pRM->addTexture2DFromFile(mImages.containerBGNormal, mImages.containerBGNormal);
		pRM->addTexture2DFromFile(mImages.buttonBGColour, mImages.buttonBGColour);
		pRM->addTexture2DFromFile(mImages.buttonBGNormal, mImages.buttonBGNormal);
		pRM->addTexture2DFromFile(mImages.reflection, mImages.reflection);
		pRM->addTexture2DFromFile(mImages.textEditBGColour, mImages.textEditBGColour);
		pRM->addTexture2DFromFile(mImages.textEditBGNormal, mImages.textEditBGNormal);
		pRM->addTexture2DFromFile(mImages.sliderBGColour, mImages.sliderBGColour);
		pRM->addTexture2DFromFile(mImages.sliderBGNormal, mImages.sliderBGNormal);
		pRM->addTexture2DFromFile(mImages.sliderTabColour, mImages.sliderTabColour);
		pRM->addTexture2DFromFile(mImages.sliderTabNormal, mImages.sliderTabNormal);
		pRM->addTexture2DFromFile(mImages.lineGraphBGColour, mImages.lineGraphBGColour);
		pRM->addTexture2DFromFile(mImages.lineGraphBGNormal, mImages.lineGraphBGNormal);
		pRM->addTexture2DFromFile(mImages.tooltipBGColour, mImages.tooltipBGColour);
		pRM->addTexture2DFromFile(mImages.progressBarBGColour, mImages.progressBarBGColour);
		pRM->addTexture2DFromFile(mImages.progressBarBGNormal, mImages.progressBarBGNormal);
		pRM->addTexture2DFromFile(mImages.progressBarFillerColour, mImages.progressBarFillerColour);
		pRM->addTexture2DFromFile(mImages.progressBarFillerNormal, mImages.progressBarFillerNormal);
		pRM->addTexture2DFromFile(mImages.textScrollBGColour, mImages.textScrollBGColour);
		pRM->addTexture2DFromFile(mImages.textScrollBGNormal, mImages.textScrollBGNormal);
		pRM->addTexture2DFromFile(mImages.buttonImageBGColour, mImages.buttonImageBGColour);
		pRM->addTexture2DFromFile(mImages.buttonImageBGNormal, mImages.buttonImageBGNormal);
	}

	void CGUITheme::unloadTextures(void)
	{
		SCResourceManager* pRM = SCResourceManager::getPointer();
		pRM->removeTexture2DFromFile(mImages.containerBGColour);
		pRM->removeTexture2DFromFile(mImages.containerBGNormal);
		pRM->removeTexture2DFromFile(mImages.buttonBGColour);
		pRM->removeTexture2DFromFile(mImages.buttonBGNormal);
		pRM->removeTexture2DFromFile(mImages.reflection);
		pRM->removeTexture2DFromFile(mImages.textEditBGColour);
		pRM->removeTexture2DFromFile(mImages.textEditBGNormal);
		pRM->removeTexture2DFromFile(mImages.sliderBGColour);
		pRM->removeTexture2DFromFile(mImages.sliderBGNormal);
		pRM->removeTexture2DFromFile(mImages.sliderTabColour);
		pRM->removeTexture2DFromFile(mImages.sliderTabNormal);
		pRM->removeTexture2DFromFile(mImages.lineGraphBGColour);
		pRM->removeTexture2DFromFile(mImages.lineGraphBGNormal);
		pRM->removeTexture2DFromFile(mImages.tooltipBGColour);
		pRM->removeTexture2DFromFile(mImages.progressBarBGColour);
		pRM->removeTexture2DFromFile(mImages.progressBarBGNormal);
		pRM->removeTexture2DFromFile(mImages.progressBarFillerColour);
		pRM->removeTexture2DFromFile(mImages.progressBarFillerNormal);
		pRM->removeTexture2DFromFile(mImages.textScrollBGColour);
		pRM->removeTexture2DFromFile(mImages.textScrollBGNormal);
		pRM->removeTexture2DFromFile(mImages.buttonImageBGColour);
		pRM->removeTexture2DFromFile(mImages.buttonImageBGNormal);
	}

	void CGUITheme::addFontsToManager(void)
	{
		SCResourceManager* pRM = SCResourceManager::getPointer();
		pRM->addFont(mFonts.button, mFonts.containerTitle);
		pRM->addFont(mFonts.button, mFonts.button);
		pRM->addFont(mFonts.text, mFonts.text);
		pRM->addFont(mFonts.textEdit, mFonts.textEdit);
		pRM->addFont(mFonts.textScroll, mFonts.textScroll);
	}

	void CGUITheme::removeFontsFromManager(void)
	{
		SCResourceManager* pRM = SCResourceManager::getPointer();
		pRM->removeFont(mFonts.containerTitle);
		pRM->removeFont(mFonts.button);
		pRM->removeFont(mFonts.text);
		pRM->removeFont(mFonts.textEdit);
		pRM->removeFont(mFonts.textScroll);
	}

	void CGUITheme::addAudioToManager(void)
	{
		SCAudioManager* pAM = SCAudioManager::getPointer();
		pAM->addSample(mAudio.buttonClicked.strSampleName, "gui");				pAM->addEmitter(mAudio.buttonClicked.strSampleName, mAudio.buttonClicked.strSampleName, 4, "gui");
		pAM->addSample(mAudio.textEditTextAdd.strSampleName, "gui");			pAM->addEmitter(mAudio.textEditTextAdd.strSampleName, mAudio.textEditTextAdd.strSampleName, 4, "gui");
		pAM->addSample(mAudio.textEditBackspace.strSampleName, "gui");			pAM->addEmitter(mAudio.textEditBackspace.strSampleName, mAudio.textEditBackspace.strSampleName, 4, "gui");
		pAM->addSample(mAudio.textEditReturn.strSampleName, "gui");				pAM->addEmitter(mAudio.textEditReturn.strSampleName, mAudio.textEditReturn.strSampleName, 4, "gui");
		pAM->addSample(mAudio.textEditActivate.strSampleName, "gui");			pAM->addEmitter(mAudio.textEditActivate.strSampleName, mAudio.textEditActivate.strSampleName, 4, "gui");
		pAM->addSample(mAudio.textEditNoMoreCharSpace.strSampleName, "gui");	pAM->addEmitter(mAudio.textEditNoMoreCharSpace.strSampleName, mAudio.textEditNoMoreCharSpace.strSampleName, 4, "gui");
		pAM->addSample(mAudio.buttonImageClicked.strSampleName, "gui");			pAM->addEmitter(mAudio.buttonImageClicked.strSampleName, mAudio.buttonImageClicked.strSampleName, 4, "gui");

		pAM->loadSampleGroup("gui");
	}

	void CGUITheme::removeAudioFromManager(void)
	{
		SCAudioManager* pAM = SCAudioManager::getPointer();
		pAM->unloadSampleGroup("gui");
		pAM->removeSample(mAudio.buttonClicked.strSampleName, "gui");			pAM->removeEmitter(mAudio.buttonClicked.strSampleName);
		pAM->removeSample(mAudio.textEditTextAdd.strSampleName, "gui");			pAM->removeEmitter(mAudio.textEditTextAdd.strSampleName);
		pAM->removeSample(mAudio.textEditBackspace.strSampleName, "gui");		pAM->removeEmitter(mAudio.textEditBackspace.strSampleName);
		pAM->removeSample(mAudio.textEditReturn.strSampleName, "gui");			pAM->removeEmitter(mAudio.textEditReturn.strSampleName);
		pAM->removeSample(mAudio.textEditActivate.strSampleName, "gui");		pAM->removeEmitter(mAudio.textEditActivate.strSampleName);
		pAM->removeSample(mAudio.textEditNoMoreCharSpace.strSampleName, "gui");	pAM->removeEmitter(mAudio.textEditNoMoreCharSpace.strSampleName);
		pAM->removeSample(mAudio.buttonImageClicked.strSampleName, "gui");		pAM->removeEmitter(mAudio.buttonImageClicked.strSampleName);
	}

	void CGUITheme::_writeImageInfo(std::ofstream& stream, const std::string& strDescription, const std::string& strImagename)
	{
		stream << strDescription + ": " + strImagename + "\n";
	}

	void CGUITheme::_readImageInfo(std::ifstream& stream, std::string& strImagename)
	{
		std::string strWord;
		char space;
		stream >> strWord;
		space = stream.get();
		getline(stream, strImagename);
	}

	void CGUITheme::_writeFontInfo(std::ofstream& stream, const std::string& strDescription, const std::string& strFontname)
	{
		stream << strDescription + ": " + strFontname + "\n";
	}

	void CGUITheme::_readFontInfo(std::ifstream& stream, std::string& strFontname)
	{
		std::string strWord;
		char space;
		stream >> strWord;
		space = stream.get();
		getline(stream, strFontname);
	}

	void CGUITheme::_writeColourInfo(std::ofstream& stream, const std::string& strDescription, const CColour& colour)
	{
		std::string str;
		str = std::to_string(colour.red) + " " + std::to_string(colour.green) + " " + std::to_string(colour.blue) + " " + std::to_string(colour.alpha);
		stream << strDescription + ": " + str + "\n";
	}

	void CGUITheme::_readColourInfo(std::ifstream& stream, CColour& colour)
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

	void CGUITheme::_writeOffsetInfo(std::ofstream& stream, const std::string& strDescription, const Offset& offset)
	{
		std::string str;
		str = std::to_string(offset.iOffsetX) + " " + std::to_string(offset.iOffsetY);
		stream << strDescription + ": " + str + "\n";
	}

	void CGUITheme::_readOffsetInfo(std::ifstream& stream, Offset& offset)
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

	void CGUITheme::_writeAudioSampleInfo(std::ofstream& stream, const std::string& strDescription, const AudioSampleInfo& asi)
	{
		stream << strDescription + "Name: " + mAudio.buttonClicked.strSampleName + "\n";
		stream << strDescription + "VolPitch: " << std::to_string(asi.fVolume) + " " + std::to_string(asi.fPitch) + "\n";
	}

	void CGUITheme::_readAudioSampleInfo(std::ifstream& stream, AudioSampleInfo& asi)
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

	void CGUITheme::_writeFloatInfo(std::ofstream& stream, const std::string& strDescription, const float& fValue)
	{
		stream << strDescription + ": " + std::to_string(fValue) + "\n";
	}

	void CGUITheme::_readFloatInfo(std::ifstream& stream, float& fValue)
	{
		std::string strWord;
		char space;
		stream >> strWord;
		space = stream.get();
		stream >> fValue;
	}
}