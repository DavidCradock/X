#include "PCH.h"
#include "UITheme.h"
#include "stringUtils.h"
#include "logging.h"
#include "singletons.h"

namespace X
{
	CUITheme::CUITheme()
	{
		_setSettingsToDefault();
		_mbResourcesAdded = false;
	}

	void CUITheme::load(const std::string& strFilename, bool bCallAddAllResources)
	{
		removeAllResources();

		// Make sure filename given has the required extension and set to lowercase
		std::string filename = StringUtils::addFilenameExtension(".theme", strFilename);

		// Open file in binary mode
		std::ifstream file;
		file.open(filename, std::ifstream::in | std::ifstream::binary);
		ThrowIfFalse(file.is_open(), "CUITheme::load(" + filename + ") failed to open file for reading.");

		// Audio
		_helperReadAudio(file, _mSettings.audio.buttonClicked);
		_helperReadAudio(file, _mSettings.audio.buttonImageClicked);
		_helperReadAudio(file, _mSettings.audio.textEditActivate);
		_helperReadAudio(file, _mSettings.audio.textEditBackspace);
		_helperReadAudio(file, _mSettings.audio.textEditNoMoreCharSpace);
		_helperReadAudio(file, _mSettings.audio.textEditReturn);
		_helperReadAudio(file, _mSettings.audio.textEditTextAdd);

		// Colours
		x->pFile->read(file, _mSettings.colours);

		// Floats
		x->pFile->read(file, _mSettings.floats);

		// Fonts
		x->pFile->readString(file, _mSettings.fonts.button);
		x->pFile->readString(file, _mSettings.fonts.text);
		x->pFile->readString(file, _mSettings.fonts.textEdit);
		x->pFile->readString(file, _mSettings.fonts.textScroll);
		x->pFile->readString(file, _mSettings.fonts.window);

		// Images
		_helperReadImageType(file, _mSettings.images.buttonBG);
		_helperReadImageType(file, _mSettings.images.buttonImageBG);
		_helperReadImageType(file, _mSettings.images.checkboxBGNormal);
		_helperReadImageType(file, _mSettings.images.checkboxBGOFF);
		_helperReadImageType(file, _mSettings.images.checkboxBGON);
		_helperReadImageType(file, _mSettings.images.lineGraphBG);
		_helperReadImageType(file, _mSettings.images.progressBarBG);
		_helperReadImageType(file, _mSettings.images.progressBarFiller);
		_helperReadImageType(file, _mSettings.images.sliderBG);
		_helperReadImageType(file, _mSettings.images.sliderTab);
		_helperReadImageType(file, _mSettings.images.textEditBG);
		_helperReadImageType(file, _mSettings.images.textScrollBG);
		_helperReadImageType(file, _mSettings.images.tooltipBG);

		// Theme name
		x->pFile->readString(file, _mSettings.themeName);

		// Image directory
		x->pFile->readString(file, _mSettings.imageDir);

		ThrowIfTrue(file.fail(), "CUITheme::load(" + filename + ") failed. Error occurred whilst loading file.");
		file.close();

		if (bCallAddAllResources)
			addAllResources();
	}

	void CUITheme::save(const std::string& strFilename)
	{
		// Make sure filename given has the required extension and set to lowercase
		std::string filename = StringUtils::addFilenameExtension(".theme", strFilename);

		// Open file in binary mode
		std::ofstream file;
		file.open(filename, std::ofstream::out | std::ofstream::binary);
		ThrowIfFalse(file.is_open(), "CUITheme::save(" + filename + ") failed to open file for writing.");

		// Audio
		_helperWriteAudio(file, _mSettings.audio.buttonClicked);
		_helperWriteAudio(file, _mSettings.audio.buttonImageClicked);
		_helperWriteAudio(file, _mSettings.audio.textEditActivate);
		_helperWriteAudio(file, _mSettings.audio.textEditBackspace);
		_helperWriteAudio(file, _mSettings.audio.textEditNoMoreCharSpace);
		_helperWriteAudio(file, _mSettings.audio.textEditReturn);
		_helperWriteAudio(file, _mSettings.audio.textEditTextAdd);

		// Colours
		x->pFile->write(file, _mSettings.colours);

		// Floats
		x->pFile->write(file, _mSettings.floats);

		// Fonts
		x->pFile->writeString(file, _mSettings.fonts.button);
		x->pFile->writeString(file, _mSettings.fonts.text);
		x->pFile->writeString(file, _mSettings.fonts.textEdit);
		x->pFile->writeString(file, _mSettings.fonts.textScroll);
		x->pFile->writeString(file, _mSettings.fonts.window);

		// Images
		_helperWriteImageType(file, _mSettings.images.buttonBG);
		_helperWriteImageType(file, _mSettings.images.buttonImageBG);
		_helperWriteImageType(file, _mSettings.images.checkboxBGNormal);
		_helperWriteImageType(file, _mSettings.images.checkboxBGOFF);
		_helperWriteImageType(file, _mSettings.images.checkboxBGON);
		_helperWriteImageType(file, _mSettings.images.lineGraphBG);
		_helperWriteImageType(file, _mSettings.images.progressBarBG);
		_helperWriteImageType(file, _mSettings.images.progressBarFiller);
		_helperWriteImageType(file, _mSettings.images.sliderBG);
		_helperWriteImageType(file, _mSettings.images.sliderTab);
		_helperWriteImageType(file, _mSettings.images.textEditBG);
		_helperWriteImageType(file, _mSettings.images.textScrollBG);
		_helperWriteImageType(file, _mSettings.images.tooltipBG);

		// Theme name
		x->pFile->writeString(file, _mSettings.themeName);

		// Image directory
		x->pFile->writeString(file, _mSettings.imageDir);

		ThrowIfTrue(file.fail(), "CUITheme::save(" + filename + ") failed. Error occurred whilst saving file.");
		file.close();
	}

	void CUITheme::addAllResources(void)
	{
		removeAllResources();

		std::string strTmp;

		// Audio
		SCAudioManager* pAM = SCAudioManager::getPointer();
		pAM->addNewSampleGroup("UI");
		strTmp = _mSettings.audio.buttonClicked.sampleName;				pAM->addSample(strTmp, "UI");	pAM->addEmitter(strTmp, strTmp, 4, "UI");
		strTmp = _mSettings.audio.buttonImageClicked.sampleName;		pAM->addSample(strTmp, "UI");	pAM->addEmitter(strTmp, strTmp, 4, "UI");
		strTmp = _mSettings.audio.textEditActivate.sampleName;			pAM->addSample(strTmp, "UI");	pAM->addEmitter(strTmp, strTmp, 4, "UI");
		strTmp = _mSettings.audio.textEditBackspace.sampleName;			pAM->addSample(strTmp, "UI");	pAM->addEmitter(strTmp, strTmp, 4, "UI");
		strTmp = _mSettings.audio.textEditNoMoreCharSpace.sampleName;	pAM->addSample(strTmp, "UI");	pAM->addEmitter(strTmp, strTmp, 4, "UI");
		strTmp = _mSettings.audio.textEditReturn.sampleName;			pAM->addSample(strTmp, "UI");	pAM->addEmitter(strTmp, strTmp, 4, "UI");
		strTmp = _mSettings.audio.textEditTextAdd.sampleName;			pAM->addSample(strTmp, "UI");	pAM->addEmitter(strTmp, strTmp, 4, "UI");
		pAM->loadSampleGroup("UI");

		// Fonts
		x->pResource->addFont(_mSettings.fonts.button, _mSettings.fonts.button, false);
		x->pResource->addFont(_mSettings.fonts.text, _mSettings.fonts.text, false);
		x->pResource->addFont(_mSettings.fonts.textEdit, _mSettings.fonts.textEdit, false);
		x->pResource->addFont(_mSettings.fonts.textScroll, _mSettings.fonts.textScroll, false);
		x->pResource->addFont(_mSettings.fonts.window, _mSettings.fonts.window, false);

		// Texture atlas
		strTmp = "UI_THEME_" + _mSettings.themeName;
		x->pResource->addTexture2DAtlas(strTmp, _mSettings.imageDir, true, 0, false, true);

		_mbResourcesAdded = true;
	}

	void CUITheme::removeAllResources(void)
	{
		if (!_mbResourcesAdded)
			return;

		std::string strTmp;

		// Audio
		SCAudioManager* pAM = SCAudioManager::getPointer();
		pAM->unloadSampleGroup("UI");
		strTmp = _mSettings.audio.buttonClicked.sampleName;				pAM->removeSample(strTmp, "UI");	pAM->removeEmitter(strTmp);
		strTmp = _mSettings.audio.buttonImageClicked.sampleName;		pAM->removeSample(strTmp, "UI");	pAM->removeEmitter(strTmp);
		strTmp = _mSettings.audio.textEditActivate.sampleName;			pAM->removeSample(strTmp, "UI");	pAM->removeEmitter(strTmp);
		strTmp = _mSettings.audio.textEditBackspace.sampleName;			pAM->removeSample(strTmp, "UI");	pAM->removeEmitter(strTmp);
		strTmp = _mSettings.audio.textEditNoMoreCharSpace.sampleName;	pAM->removeSample(strTmp, "UI");	pAM->removeEmitter(strTmp);
		strTmp = _mSettings.audio.textEditReturn.sampleName;			pAM->removeSample(strTmp, "UI");	pAM->removeEmitter(strTmp);
		strTmp = _mSettings.audio.textEditTextAdd.sampleName;			pAM->removeSample(strTmp, "UI");	pAM->removeEmitter(strTmp);

		// Fonts
		x->pResource->removeFont(_mSettings.fonts.button);
		x->pResource->removeFont(_mSettings.fonts.text);
		x->pResource->removeFont(_mSettings.fonts.textEdit);
		x->pResource->removeFont(_mSettings.fonts.textScroll);
		x->pResource->removeFont(_mSettings.fonts.window);

		// Texture atlas
		strTmp = "UI_THEME_" + _mSettings.themeName;
		x->pResource->removeTexture2DAtlas(strTmp);

		_mbResourcesAdded = false;
	}

	CUITheme::SSettings* CUITheme::getSettings(void)
	{
		removeAllResources();
		return &_mSettings;
	}

	void CUITheme::_setSettingsToDefault(void)
	{
		// Audio
		_helperSetAudio(_mSettings.audio.buttonClicked, "data/X/UI/default/buttonClicked.wav", 1.0f, 0.8f);
		_helperSetAudio(_mSettings.audio.buttonImageClicked, "data/X/UI/default/buttonImageClicked.wav", 1.0f, 0.8f);
		_helperSetAudio(_mSettings.audio.textEditActivate, "data/X/UI/default/textEditActivate.wav", 1.0f, 0.8f);
		_helperSetAudio(_mSettings.audio.textEditBackspace, "data/X/UI/default/textEditBackspace.wav", 1.0f, 0.2f);
		_helperSetAudio(_mSettings.audio.textEditNoMoreCharSpace, "data/X/UI/default/textEditNoMoreCharSpace.wav", 1.0f, 0.2f);
		_helperSetAudio(_mSettings.audio.textEditReturn, "data/X/UI/default/textEditReturn.wav", 1.0f, 0.8f);
		_helperSetAudio(_mSettings.audio.textEditTextAdd, "data/X/UI/default/textEditTextAdd.wav", 1.0f, 0.8f);

		// Colours
		_mSettings.colours.buttonBGDown.set(1.0f, 1.0f, 1.0f, 1.0f);
		_mSettings.colours.buttonBGOver.set(1.0f, 1.0f, 1.0f, 1.0f);
		_mSettings.colours.buttonBGUp.set(1.0f, 1.0f, 1.0f, 1.0f);
		_mSettings.colours.buttonImageDown.set(1.0f, 1.0f, 1.0f, 1.0f);
		_mSettings.colours.buttonImageOver.set(1.0f, 1.0f, 1.0f, 1.0f);
		_mSettings.colours.buttonImageUp.set(1.0f, 1.0f, 1.0f, 1.0f);
		_mSettings.colours.buttonTextDown.set(1.0f, 1.0f, 1.0f, 1.0f);
		_mSettings.colours.buttonTextOver.set(1.0f, 1.0f, 1.0f, 1.0f);
		_mSettings.colours.buttonTextUp.set(1.0f, 1.0f, 1.0f, 1.0f);
		_mSettings.colours.containerTitlebarTextInFocus.set(1.0f, 1.0f, 1.0f, 1.0f);
		_mSettings.colours.containerTitlebarTextNotInFocus.set(1.0f, 1.0f, 1.0f, 1.0f);
		_mSettings.colours.progressBarFiller.set(1.0f, 1.0f, 1.0f, 1.0f);
		_mSettings.colours.sliderTabNotOver.set(1.0f, 1.0f, 1.0f, 1.0f);
		_mSettings.colours.sliderTabOver.set(1.0f, 1.0f, 1.0f, 1.0f);
		_mSettings.colours.text.set(1.0f, 1.0f, 1.0f, 1.0f);
		_mSettings.colours.textEditActive.set(1.0f, 1.0f, 1.0f, 1.0f);
		_mSettings.colours.textEditInactive.set(1.0f, 1.0f, 1.0f, 1.0f);
		_mSettings.colours.textScrollBG.set(1.0f, 1.0f, 1.0f, 1.0f);
		_mSettings.colours.textScrollText.set(1.0f, 1.0f, 1.0f, 1.0f);
		_mSettings.colours.tooltipText.set(1.0f, 1.0f, 1.0f, 1.0f);

		// Floats
		_mSettings.floats.buttonFadeSpeedSeconds = 5.0f;
		_mSettings.floats.buttonImageTextFadeSpeedSeconds = 5.0f;
		_mSettings.floats.checkboxFadeSpeedSeconds = 1.0f;
		_mSettings.floats.mouseCursorDistance = 50.0f;
		_mSettings.floats.normalAmount = 0.1f;
		_mSettings.floats.sliderTabFadeSpeedSeconds = 5.0f;
		_mSettings.floats.textEditFlashSpeed = 4.0f;
		_mSettings.floats.textScrollSliderWidth = 20.0f;
		_mSettings.floats.tooltipFadeSpeedSeconds = 2.0f;

		// Fonts
		_mSettings.fonts.button = "data/X/UI/default/satoshi_22";
		_mSettings.fonts.text = "data/X/UI/default/satoshi_22";
		_mSettings.fonts.textEdit = "data/X/UI/default/satoshi_22";
		_mSettings.fonts.textScroll = "data/X/UI/default/satoshi_22";
		_mSettings.fonts.window = "data/X/UI/default/satoshi_22";

		// Image file names
		_helperSetNames(_mSettings.images.buttonBG, "data/X/UI/default/images/buttonBG");
		_helperSetNames(_mSettings.images.buttonImageBG, "data/X/UI/default/images/buttonImageBG");
		_helperSetNames(_mSettings.images.checkboxBGNormal, "data/X/UI/default/images/checkboxBGNormal");
		_helperSetNames(_mSettings.images.checkboxBGOFF, "data/X/UI/default/images/checkboxBGOFF");
		_helperSetNames(_mSettings.images.checkboxBGON, "data/X/UI/default/images/checkboxBGON");
		_helperSetNames(_mSettings.images.lineGraphBG, "data/X/UI/default/images/lineGraphBG");
		_helperSetNames(_mSettings.images.progressBarBG, "data/X/UI/default/images/progressBarBG");
		_helperSetNames(_mSettings.images.progressBarFiller, "data/X/UI/default/images/progressBarFiller");
		_helperSetNames(_mSettings.images.sliderBG, "data/X/UI/default/images/sliderBG");
		_helperSetNames(_mSettings.images.sliderTab, "data/X/UI/default/images/sliderTab");
		_helperSetNames(_mSettings.images.textEditBG, "data/X/UI/default/images/textEditBG");
		_helperSetNames(_mSettings.images.textScrollBG, "data/X/UI/default/images/textScrollBG");
		_helperSetNames(_mSettings.images.tooltipBG, "data/X/UI/default/images/tooltipBG");

		// Theme name
		_mSettings.themeName = "Default theme";

		// Image dir
		_mSettings.imageDir = "data/X/UI/default/images/";
	}

	void CUITheme::_helperSetAudio(AudioSampleInfo& audio, const std::string& strSampleName, float fPitch, float fVolume)
	{
		audio.pitch = fPitch;
		audio.sampleName = strSampleName;
		audio.volume = fVolume;
	}

	void CUITheme::_helperSetNames(SImageType& imageType, const std::string& strBaseName)
	{
		imageType.colour.centre = strBaseName + "_colour_centre.png";
		imageType.colour.cornerBL = strBaseName + "_colour_cornerBL.png";
		imageType.colour.cornerBR = strBaseName + "_colour_cornerBR.png";
		imageType.colour.cornerTL = strBaseName + "_colour_cornerTL.png";
		imageType.colour.cornerTR = strBaseName + "_colour_cornerTR.png";
		imageType.colour.edgeB = strBaseName + "_colour_edgeB.png";
		imageType.colour.edgeL = strBaseName + "_colour_edgeL.png";
		imageType.colour.edgeR = strBaseName + "_colour_edgeR.png";
		imageType.colour.edgeT = strBaseName + "_colour_edgeT.png";

		imageType.height.centre = strBaseName + "_height_centre.png";
		imageType.height.cornerBL = strBaseName + "_height_cornerBL.png";
		imageType.height.cornerBR = strBaseName + "_height_cornerBR.png";
		imageType.height.cornerTL = strBaseName + "_height_cornerTL.png";
		imageType.height.cornerTR = strBaseName + "_height_cornerTR.png";
		imageType.height.edgeB = strBaseName + "_height_edgeB.png";
		imageType.height.edgeL = strBaseName + "_height_edgeL.png";
		imageType.height.edgeR = strBaseName + "_height_edgeR.png";
		imageType.height.edgeT = strBaseName + "_height_edgeT.png";

		imageType.normal.centre = strBaseName + "_normal_centre.png";
		imageType.normal.cornerBL = strBaseName + "_normal_cornerBL.png";
		imageType.normal.cornerBR = strBaseName + "_normal_cornerBR.png";
		imageType.normal.cornerTL = strBaseName + "_normal_cornerTL.png";
		imageType.normal.cornerTR = strBaseName + "_normal_cornerTR.png";
		imageType.normal.edgeB = strBaseName + "_normal_edgeB.png";
		imageType.normal.edgeL = strBaseName + "_normal_edgeL.png";
		imageType.normal.edgeR = strBaseName + "_normal_edgeR.png";
		imageType.normal.edgeT = strBaseName + "_normal_edgeT.png";
	}

	void CUITheme::_helperReadAudio(std::ifstream& file, AudioSampleInfo& audio)
	{
		x->pFile->read(file, audio.pitch);
		x->pFile->readString(file, audio.sampleName);
		x->pFile->read(file, audio.volume);
	}
	
	void CUITheme::_helperWriteAudio(std::ofstream& file, AudioSampleInfo& audio)
	{
		x->pFile->write(file, audio.pitch);
		x->pFile->writeString(file, audio.sampleName);
		x->pFile->write(file, audio.volume);
	}

	void CUITheme::_helperReadImageType(std::ifstream& file, SImageType& imageType)
	{
		x->pFile->readString(file, imageType.colour.centre);
		x->pFile->readString(file, imageType.colour.cornerBL);
		x->pFile->readString(file, imageType.colour.cornerBR);
		x->pFile->readString(file, imageType.colour.cornerTL);
		x->pFile->readString(file, imageType.colour.cornerTR);
		x->pFile->readString(file, imageType.colour.edgeB);
		x->pFile->readString(file, imageType.colour.edgeL);
		x->pFile->readString(file, imageType.colour.edgeR);
		x->pFile->readString(file, imageType.colour.edgeT);

		x->pFile->readString(file, imageType.height.centre);
		x->pFile->readString(file, imageType.height.cornerBL);
		x->pFile->readString(file, imageType.height.cornerBR);
		x->pFile->readString(file, imageType.height.cornerTL);
		x->pFile->readString(file, imageType.height.cornerTR);
		x->pFile->readString(file, imageType.height.edgeB);
		x->pFile->readString(file, imageType.height.edgeL);
		x->pFile->readString(file, imageType.height.edgeR);
		x->pFile->readString(file, imageType.height.edgeT);

		x->pFile->readString(file, imageType.normal.centre);
		x->pFile->readString(file, imageType.normal.cornerBL);
		x->pFile->readString(file, imageType.normal.cornerBR);
		x->pFile->readString(file, imageType.normal.cornerTL);
		x->pFile->readString(file, imageType.normal.cornerTR);
		x->pFile->readString(file, imageType.normal.edgeB);
		x->pFile->readString(file, imageType.normal.edgeL);
		x->pFile->readString(file, imageType.normal.edgeR);
		x->pFile->readString(file, imageType.normal.edgeT);
	}

	void CUITheme::_helperWriteImageType(std::ofstream& file, SImageType& imageType)
	{
		x->pFile->writeString(file, imageType.colour.centre);
		x->pFile->writeString(file, imageType.colour.cornerBL);
		x->pFile->writeString(file, imageType.colour.cornerBR);
		x->pFile->writeString(file, imageType.colour.cornerTL);
		x->pFile->writeString(file, imageType.colour.cornerTR);
		x->pFile->writeString(file, imageType.colour.edgeB);
		x->pFile->writeString(file, imageType.colour.edgeL);
		x->pFile->writeString(file, imageType.colour.edgeR);
		x->pFile->writeString(file, imageType.colour.edgeT);

		x->pFile->writeString(file, imageType.height.centre);
		x->pFile->writeString(file, imageType.height.cornerBL);
		x->pFile->writeString(file, imageType.height.cornerBR);
		x->pFile->writeString(file, imageType.height.cornerTL);
		x->pFile->writeString(file, imageType.height.cornerTR);
		x->pFile->writeString(file, imageType.height.edgeB);
		x->pFile->writeString(file, imageType.height.edgeL);
		x->pFile->writeString(file, imageType.height.edgeR);
		x->pFile->writeString(file, imageType.height.edgeT);

		x->pFile->writeString(file, imageType.normal.centre);
		x->pFile->writeString(file, imageType.normal.cornerBL);
		x->pFile->writeString(file, imageType.normal.cornerBR);
		x->pFile->writeString(file, imageType.normal.cornerTL);
		x->pFile->writeString(file, imageType.normal.cornerTR);
		x->pFile->writeString(file, imageType.normal.edgeB);
		x->pFile->writeString(file, imageType.normal.edgeL);
		x->pFile->writeString(file, imageType.normal.edgeR);
		x->pFile->writeString(file, imageType.normal.edgeT);
	}
}