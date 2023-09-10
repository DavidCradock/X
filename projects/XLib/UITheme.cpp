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

	/************************************************************************************************************************************************************/
	/* All methods below this section need to be modified if we're adding a new widget type */
	/************************************************************************************************************************************************************/
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
//		_helperReadAudio(file, _mSettings.audio.buttonClicked);
//		_helperReadAudio(file, _mSettings.audio.buttonImageClicked);
//		_helperReadAudio(file, _mSettings.audio.textEditActivate);
//		_helperReadAudio(file, _mSettings.audio.textEditBackspace);
//		_helperReadAudio(file, _mSettings.audio.textEditNoMoreCharSpace);
//		_helperReadAudio(file, _mSettings.audio.textEditReturn);
//		_helperReadAudio(file, _mSettings.audio.textEditTextAdd);

		// Colours
		x->pFile->read(file, _mSettings.colours);

		// Floats
		x->pFile->read(file, _mSettings.floats);

		// Fonts
		x->pFile->readString(file, _mSettings.fonts.button);
		x->pFile->readString(file, _mSettings.fonts.buttonImage);
		x->pFile->readString(file, _mSettings.fonts.text);
//		x->pFile->readString(file, _mSettings.fonts.textEdit);
//		x->pFile->readString(file, _mSettings.fonts.textScroll);
		x->pFile->readString(file, _mSettings.fonts.windowTitlebar);

		// Images
		_helperReadImageType(file, _mSettings.images.buttonBG);
//		_helperReadImageType(file, _mSettings.images.buttonImageBG);
//		_helperReadImageType(file, _mSettings.images.checkboxBGNormal);
//		_helperReadImageType(file, _mSettings.images.checkboxBGOFF);
//		_helperReadImageType(file, _mSettings.images.checkboxBGON);
//		_helperReadImageType(file, _mSettings.images.lineGraphBG);
		_helperReadImageType(file, _mSettings.images.progressBarBG);
		_helperReadImageType(file, _mSettings.images.progressBarFiller);
		_helperReadImageType(file, _mSettings.images.scrollbarBG);
		_helperReadImageType(file, _mSettings.images.scrollbarTab);
//		_helperReadImageType(file, _mSettings.images.textEditBG);
//		_helperReadImageType(file, _mSettings.images.textScrollBG);
//		_helperReadImageType(file, _mSettings.images.tooltipBG);
		_helperReadImageType(file, _mSettings.images.windowBG);

		// Mouse cursors
		x->pFile->readString(file, _mSettings.cursors.busy);
		x->pFile->readString(file, _mSettings.cursors.normal);
		x->pFile->readString(file, _mSettings.cursors.resize_LtoR);
		x->pFile->readString(file, _mSettings.cursors.resize_TLtoBR);
		x->pFile->readString(file, _mSettings.cursors.resize_TRtoBL);
		x->pFile->readString(file, _mSettings.cursors.resize_TtoB);

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
//		_helperWriteAudio(file, _mSettings.audio.buttonClicked);
//		_helperWriteAudio(file, _mSettings.audio.buttonImageClicked);
//		_helperWriteAudio(file, _mSettings.audio.textEditActivate);
//		_helperWriteAudio(file, _mSettings.audio.textEditBackspace);
//		_helperWriteAudio(file, _mSettings.audio.textEditNoMoreCharSpace);
//		_helperWriteAudio(file, _mSettings.audio.textEditReturn);
//		_helperWriteAudio(file, _mSettings.audio.textEditTextAdd);

		// Colours
		x->pFile->write(file, _mSettings.colours);

		// Floats
		x->pFile->write(file, _mSettings.floats);

		// Fonts
		x->pFile->writeString(file, _mSettings.fonts.button);
		x->pFile->writeString(file, _mSettings.fonts.buttonImage);
		x->pFile->writeString(file, _mSettings.fonts.text);
//		x->pFile->writeString(file, _mSettings.fonts.textEdit);
//		x->pFile->writeString(file, _mSettings.fonts.textScroll);
		x->pFile->writeString(file, _mSettings.fonts.windowTitlebar);

		// Images
		_helperWriteImageType(file, _mSettings.images.buttonBG);
//		_helperWriteImageType(file, _mSettings.images.buttonImageBG);
//		_helperWriteImageType(file, _mSettings.images.checkboxBGNormal);
//		_helperWriteImageType(file, _mSettings.images.checkboxBGOFF);
//		_helperWriteImageType(file, _mSettings.images.checkboxBGON);
//		_helperWriteImageType(file, _mSettings.images.lineGraphBG);
		_helperWriteImageType(file, _mSettings.images.progressBarBG);
		_helperWriteImageType(file, _mSettings.images.progressBarFiller);
		_helperWriteImageType(file, _mSettings.images.scrollbarBG);
		_helperWriteImageType(file, _mSettings.images.scrollbarTab);
//		_helperWriteImageType(file, _mSettings.images.textEditBG);
//		_helperWriteImageType(file, _mSettings.images.textScrollBG);
//		_helperWriteImageType(file, _mSettings.images.tooltipBG);
		_helperWriteImageType(file, _mSettings.images.windowBG);

		// Mouse cursors
		x->pFile->writeString(file, _mSettings.cursors.busy);
		x->pFile->writeString(file, _mSettings.cursors.normal);
		x->pFile->writeString(file, _mSettings.cursors.resize_LtoR);
		x->pFile->writeString(file, _mSettings.cursors.resize_TLtoBR);
		x->pFile->writeString(file, _mSettings.cursors.resize_TRtoBL);
		x->pFile->writeString(file, _mSettings.cursors.resize_TtoB);

		// Theme name
		x->pFile->writeString(file, _mSettings.themeName);

		// Image directory
		x->pFile->writeString(file, _mSettings.imageDir);

		ThrowIfTrue(file.fail(), "CUITheme::save(" + filename + ") failed. Error occurred whilst saving file.");
		file.close();
	}

	void CUITheme::addAllResources(bool bOverwriteNormalImages)
	{
		removeAllResources();

		std::string strTmp;

		// Audio
		SCAudioManager* pAM = SCAudioManager::getPointer();
		pAM->addNewSampleGroup("UI");
//		strTmp = _mSettings.audio.buttonClicked.sampleName;				pAM->addSample(strTmp, "UI");	pAM->addEmitter(strTmp, strTmp, 4, "UI");
//		strTmp = _mSettings.audio.buttonImageClicked.sampleName;		pAM->addSample(strTmp, "UI");	pAM->addEmitter(strTmp, strTmp, 4, "UI");
//		strTmp = _mSettings.audio.textEditActivate.sampleName;			pAM->addSample(strTmp, "UI");	pAM->addEmitter(strTmp, strTmp, 4, "UI");
//		strTmp = _mSettings.audio.textEditBackspace.sampleName;			pAM->addSample(strTmp, "UI");	pAM->addEmitter(strTmp, strTmp, 4, "UI");
//		strTmp = _mSettings.audio.textEditNoMoreCharSpace.sampleName;	pAM->addSample(strTmp, "UI");	pAM->addEmitter(strTmp, strTmp, 4, "UI");
//		strTmp = _mSettings.audio.textEditReturn.sampleName;			pAM->addSample(strTmp, "UI");	pAM->addEmitter(strTmp, strTmp, 4, "UI");
//		strTmp = _mSettings.audio.textEditTextAdd.sampleName;			pAM->addSample(strTmp, "UI");	pAM->addEmitter(strTmp, strTmp, 4, "UI");
		pAM->loadSampleGroup("UI");

		// Fonts
		x->pResource->addFont(_mSettings.fonts.button, _mSettings.fonts.button, false);
		x->pResource->addFont(_mSettings.fonts.buttonImage, _mSettings.fonts.buttonImage, false);
		x->pResource->addFont(_mSettings.fonts.text, _mSettings.fonts.text, false);
//		x->pResource->addFont(_mSettings.fonts.textEdit, _mSettings.fonts.textEdit, false);
//		x->pResource->addFont(_mSettings.fonts.textScroll, _mSettings.fonts.textScroll, false);
		x->pResource->addFont(_mSettings.fonts.windowTitlebar, _mSettings.fonts.windowTitlebar, false);

		// Mouse cursors
		x->pResource->addMouseCursor(_mSettings.cursors.busy, _mSettings.cursors.busy, false);
		x->pResource->addMouseCursor(_mSettings.cursors.normal, _mSettings.cursors.normal, false);
		x->pResource->addMouseCursor(_mSettings.cursors.resize_LtoR, _mSettings.cursors.resize_LtoR, false);
		x->pResource->addMouseCursor(_mSettings.cursors.resize_TLtoBR, _mSettings.cursors.resize_TLtoBR, false);
		x->pResource->addMouseCursor(_mSettings.cursors.resize_TRtoBL, _mSettings.cursors.resize_TRtoBL, false);
		x->pResource->addMouseCursor(_mSettings.cursors.resize_TtoB, _mSettings.cursors.resize_TtoB, false);

		// Build/rebuild normal images
		_helperBuildNormalImages(bOverwriteNormalImages);

		// Texture atlas
		strTmp = "UI_THEME_" + _mSettings.themeName;
		x->pResource->addTexture2DAtlas(strTmp, _mSettings.imageDir, true, 0, false, false/*nearest or linear filtering*/);

		_mbResourcesAdded = true;

		// Possibly throw an exception, make sure all image dimensions are OK
		_helperCheckImageDimsAreOK();
	}

	void CUITheme::removeAllResources(void)
	{
		if (!_mbResourcesAdded)
			return;

		std::string strTmp;

		// Audio
		SCAudioManager* pAM = SCAudioManager::getPointer();
		pAM->unloadSampleGroup("UI");
//		strTmp = _mSettings.audio.buttonClicked.sampleName;				pAM->removeSample(strTmp, "UI");	pAM->removeEmitter(strTmp);
//		strTmp = _mSettings.audio.buttonImageClicked.sampleName;		pAM->removeSample(strTmp, "UI");	pAM->removeEmitter(strTmp);
//		strTmp = _mSettings.audio.textEditActivate.sampleName;			pAM->removeSample(strTmp, "UI");	pAM->removeEmitter(strTmp);
//		strTmp = _mSettings.audio.textEditBackspace.sampleName;			pAM->removeSample(strTmp, "UI");	pAM->removeEmitter(strTmp);
//		strTmp = _mSettings.audio.textEditNoMoreCharSpace.sampleName;	pAM->removeSample(strTmp, "UI");	pAM->removeEmitter(strTmp);
//		strTmp = _mSettings.audio.textEditReturn.sampleName;			pAM->removeSample(strTmp, "UI");	pAM->removeEmitter(strTmp);
//		strTmp = _mSettings.audio.textEditTextAdd.sampleName;			pAM->removeSample(strTmp, "UI");	pAM->removeEmitter(strTmp);

		// Fonts
		x->pResource->removeFont(_mSettings.fonts.button);
		x->pResource->removeFont(_mSettings.fonts.buttonImage);
		x->pResource->removeFont(_mSettings.fonts.text);
//		x->pResource->removeFont(_mSettings.fonts.textEdit);
//		x->pResource->removeFont(_mSettings.fonts.textScroll);
		x->pResource->removeFont(_mSettings.fonts.windowTitlebar);

		// Mouse cursors
		x->pResource->removeMouseCursor(_mSettings.cursors.busy);
		x->pResource->removeMouseCursor(_mSettings.cursors.normal);
		x->pResource->removeMouseCursor(_mSettings.cursors.resize_LtoR);
		x->pResource->removeMouseCursor(_mSettings.cursors.resize_TLtoBR);
		x->pResource->removeMouseCursor(_mSettings.cursors.resize_TRtoBL);
		x->pResource->removeMouseCursor(_mSettings.cursors.resize_TtoB);

		// Texture atlas
		strTmp = "UI_THEME_" + _mSettings.themeName;
		x->pResource->removeTexture2DAtlas(strTmp);

		_mbResourcesAdded = false;
	}

	void CUITheme::_setSettingsToDefault(void)
	{
		// Audio
//		_helperSetAudio(_mSettings.audio.buttonClicked, "data/X/UI/default/buttonClicked.wav", 1.0f, 0.8f);
//		_helperSetAudio(_mSettings.audio.buttonImageClicked, "data/X/UI/default/buttonImageClicked.wav", 1.0f, 0.8f);
//		_helperSetAudio(_mSettings.audio.textEditActivate, "data/X/UI/default/textEditActivate.wav", 1.0f, 0.8f);
//		_helperSetAudio(_mSettings.audio.textEditBackspace, "data/X/UI/default/textEditBackspace.wav", 1.0f, 0.2f);
//		_helperSetAudio(_mSettings.audio.textEditNoMoreCharSpace, "data/X/UI/default/textEditNoMoreCharSpace.wav", 1.0f, 0.2f);
//		_helperSetAudio(_mSettings.audio.textEditReturn, "data/X/UI/default/textEditReturn.wav", 1.0f, 0.8f);
//		_helperSetAudio(_mSettings.audio.textEditTextAdd, "data/X/UI/default/textEditTextAdd.wav", 1.0f, 0.8f);

		// Colours
		_mSettings.colours.ambientLight.set(0.7f, 0.7f, 0.7f, 1.0f);
		_mSettings.colours.buttonBGDown.set(1.0f, 1.0f, 1.0f, 1.0f);
		_mSettings.colours.buttonBGOver.set(0.9f, 0.9f, 0.9f, 1.0f);
		_mSettings.colours.buttonBGUp.set(0.8f, 0.8f, 0.8f, 1.0f);
		_mSettings.colours.buttonTextDown.set(1.0f, 1.0f, 1.0f, 1.0f);
		_mSettings.colours.buttonTextOver.set(0.9f, 0.9f, 0.9f, 1.0f);
		_mSettings.colours.buttonTextUp.set(0.8f, 0.8f, 0.8f, 1.0f);
		_mSettings.colours.buttonImageBGDown.set(1.0f, 1.0f, 1.0f, 1.0f);
		_mSettings.colours.buttonImageBGOver.set(0.9f, 0.9f, 0.9f, 1.0f);
		_mSettings.colours.buttonImageBGUp.set(0.8f, 0.8f, 0.8f, 1.0f);
		_mSettings.colours.buttonImageTextDown.set(1.0f, 1.0f, 1.0f, 1.0f);
		_mSettings.colours.buttonImageTextOver.set(0.9f, 0.9f, 0.9f, 1.0f);
		_mSettings.colours.buttonImageTextUp.set(0.8f, 0.8f, 0.8f, 1.0f);
		_mSettings.colours.mouseLight.set(0.3f, 0.3f, 0.3f, 1.0f);
		_mSettings.colours.progressBarBG.set(1.0f, 1.0f, 1.0f, 1.0f);
		_mSettings.colours.progressBarFiller.set(1.0f, 1.0f, 1.0f, 0.5f);
		_mSettings.colours.scrollbarBG.set(1.0f, 1.0f, 1.0f, 1.0f);
		_mSettings.colours.scrollbarTabNotOver.set(1.0f, 1.0f, 1.0f, 0.5f);
		_mSettings.colours.scrollbarTabOver.set(1.0f, 1.0f, 1.0f, 1.0f);
		_mSettings.colours.text.set(1.0f, 1.0f, 1.0f, 1.0f);
//		_mSettings.colours.textEditActive.set(1.0f, 1.0f, 1.0f, 1.0f);
//		_mSettings.colours.textEditInactive.set(1.0f, 1.0f, 1.0f, 1.0f);
//		_mSettings.colours.textScrollBG.set(1.0f, 1.0f, 1.0f, 1.0f);
//		_mSettings.colours.textScrollText.set(1.0f, 1.0f, 1.0f, 1.0f);
//		_mSettings.colours.tooltipText.set(1.0f, 1.0f, 1.0f, 1.0f);
		_mSettings.colours.windowBGFocused.set(1.0f, 1.0f, 1.0f, 0.9f);
		_mSettings.colours.windowBGNotFocused.set(1.0f, 1.0f, 1.0f, 0.8f);
		_mSettings.colours.windowTitlebarTextFocused.set(1.0f, 1.0f, 1.0f, 1.0f);
		_mSettings.colours.windowTitlebarTextNotFocused.set(1.0f, 1.0f, 1.0f, 0.8f);

		// Floats
		_mSettings.floats.buttonFadeSpeed = 5.0f;
		_mSettings.floats.buttonImageFadeSpeed = 5.0f;
//		_mSettings.floats.checkboxFadeSpeed = 1.0f;
		_mSettings.floats.normalMouseCursorDistance = 50.0f;
		_mSettings.floats.scrollbarTabFadeSpeed = 5.0f;
//		_mSettings.floats.textEditFlashSpeed = 4.0f;
//		_mSettings.floats.textScrollScrollbarWidth = 20.0f;
//		_mSettings.floats.tooltipFadeSpeedSeconds = 2.0f;
		_mSettings.floats.windowResizeHandleOffsetX = 12.0f;
		_mSettings.floats.windowResizeHandleOffsetY = 12.0f;
		_mSettings.floats.windowScrollbarHorizontalHeight = 24.0f;
		_mSettings.floats.windowScrollbarVerticalWidth = 24.0f;

		// Fonts
		_mSettings.fonts.button = "data/X/UI/default/satoshi_22";
		_mSettings.fonts.buttonImage = "data/X/UI/default/satoshi_22";
		_mSettings.fonts.text = "data/X/UI/default/satoshi_22";
//		_mSettings.fonts.textEdit = "data/X/UI/default/satoshi_22";
//		_mSettings.fonts.textScroll = "data/X/UI/default/satoshi_22";
		_mSettings.fonts.windowTitlebar = "data/X/UI/default/satoshi_22";

		// Image file names
		_helperSetNames(_mSettings.images.buttonBG, "data/X/UI/default/images/genericBG");
//		_helperSetNames(_mSettings.images.buttonImageBG, "data/X/UI/default/images/buttonImageBG");
//		_helperSetNames(_mSettings.images.checkboxBGNormal, "data/X/UI/default/images/checkboxBGNormal");
//		_helperSetNames(_mSettings.images.checkboxBGOFF, "data/X/UI/default/images/checkboxBGOFF");
//		_helperSetNames(_mSettings.images.checkboxBGON, "data/X/UI/default/images/checkboxBGON");
//		_helperSetNames(_mSettings.images.lineGraphBG, "data/X/UI/default/images/lineGraphBG");
		_helperSetNames(_mSettings.images.progressBarBG, "data/X/UI/default/images/genericBG");
		_helperSetNames(_mSettings.images.progressBarFiller, "data/X/UI/default/images/genericBG");
		_helperSetNames(_mSettings.images.scrollbarBG, "data/X/UI/default/images/scrollbarBG");
		_helperSetNames(_mSettings.images.scrollbarTab, "data/X/UI/default/images/scrollbarTab");
//		_helperSetNames(_mSettings.images.textEditBG, "data/X/UI/default/images/textEditBG");
//		_helperSetNames(_mSettings.images.textScrollBG, "data/X/UI/default/images/textScrollBG");
//		_helperSetNames(_mSettings.images.tooltipBG, "data/X/UI/default/images/tooltipBG");
		_helperSetNames(_mSettings.images.windowBG, "data/X/UI/default/images/windowBG");

		// Mouse cursors
		_mSettings.cursors.busy				= "data/X/cursors/default_busy.ani";
		_mSettings.cursors.normal			= "data/X/cursors/default_normal.ani";
		_mSettings.cursors.resize_LtoR		= "data/X/cursors/default_resize_LtoR.ani";
		_mSettings.cursors.resize_TLtoBR	= "data/X/cursors/default_resize_TLtoBR.ani";
		_mSettings.cursors.resize_TRtoBL	= "data/X/cursors/default_resize_TRtoBL.ani";
		_mSettings.cursors.resize_TtoB		= "data/X/cursors/default_resize_TtoB.ani";

		// Theme name
		_mSettings.themeName = "default";

		// Image dir
		_mSettings.imageDir = "data/X/UI/default/images/";
	}

	void CUITheme::_helperCheckImageDimsAreOK(void)
	{
		ThrowIfFalse(_mbResourcesAdded, "CUITheme::_helperCheckImageDimsAreOK() failed. Resources have not been added to resource managers yet.");
		// Note: The second parameter for the below method calls can be anything, it's used during creating of the exception text.
		_helperCheckImageDimsAreOK(_mSettings.images.buttonBG, "buttonBG");
//		_helperCheckImageDimsAreOK(_mSettings.images.buttonImageBG, "buttonImageBG");
//		_helperCheckImageDimsAreOK(_mSettings.images.checkboxBGNormal, "checkboxBGNormal");
//		_helperCheckImageDimsAreOK(_mSettings.images.checkboxBGOFF, "checkboxBGOFF");
//		_helperCheckImageDimsAreOK(_mSettings.images.checkboxBGON, "checkboxBGON");
//		_helperCheckImageDimsAreOK(_mSettings.images.lineGraphBG, "lineGraphBG");
		_helperCheckImageDimsAreOK(_mSettings.images.progressBarBG, "progressBarBG");
		_helperCheckImageDimsAreOK(_mSettings.images.progressBarFiller, "progressBarFiller");
		_helperCheckImageDimsAreOK(_mSettings.images.scrollbarBG, "scrollbarBG");
		_helperCheckImageDimsAreOK(_mSettings.images.scrollbarTab, "scrollbarTab");
//		_helperCheckImageDimsAreOK(_mSettings.images.textEditBG, "textEditBG");
//		_helperCheckImageDimsAreOK(_mSettings.images.textScrollBG, "textScrollBG");
//		_helperCheckImageDimsAreOK(_mSettings.images.tooltipBG, "tooltipBG");
		_helperCheckImageDimsAreOK(_mSettings.images.windowBG, "windowBG");
	}

	void CUITheme::_helperBuildNormalImages(bool bOverwriteNormalImages)
	{
		_helperBuildNormalImages(_mSettings.images.buttonBG, bOverwriteNormalImages);
//		_helperBuildNormalImages(_mSettings.images.buttonImageBG, bOverwriteNormalImages);
//		_helperBuildNormalImages(_mSettings.images.checkboxBGNormal, bOverwriteNormalImages);
//		_helperBuildNormalImages(_mSettings.images.checkboxBGOFF, bOverwriteNormalImages);
//		_helperBuildNormalImages(_mSettings.images.checkboxBGON, bOverwriteNormalImages);
//		_helperBuildNormalImages(_mSettings.images.lineGraphBG, bOverwriteNormalImages);
		_helperBuildNormalImages(_mSettings.images.progressBarBG, bOverwriteNormalImages);
		_helperBuildNormalImages(_mSettings.images.progressBarFiller, bOverwriteNormalImages);
		_helperBuildNormalImages(_mSettings.images.scrollbarBG, bOverwriteNormalImages);
		_helperBuildNormalImages(_mSettings.images.scrollbarTab, bOverwriteNormalImages);
//		_helperBuildNormalImages(_mSettings.images.textEditBG, bOverwriteNormalImages);
//		_helperBuildNormalImages(_mSettings.images.textScrollBG, bOverwriteNormalImages);
//		_helperBuildNormalImages(_mSettings.images.tooltipBG, bOverwriteNormalImages);
		_helperBuildNormalImages(_mSettings.images.windowBG, bOverwriteNormalImages);
	}

	/************************************************************************************************************************************************************/
	/* All methods below this section DO NOT need to be modified if we're adding a new widget type */
	/************************************************************************************************************************************************************/

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

	const CUITheme::SSettings* CUITheme::getSettings(void)
	{
		return &_mSettings;
	}

	CUITheme::SSettings* CUITheme::getSettingsForModification(void)
	{
		removeAllResources();
		return &_mSettings;
	}

	CResourceTexture2DAtlas* CUITheme::getTextureAtlas(void)
	{
		ThrowIfFalse(_mbResourcesAdded, "CUITheme::getTextureAtlas() failed. Resources have not been added to the various managers yet.");
		std::string strTextureAtlasName = "UI_THEME_" + _mSettings.themeName;
		ThrowIfFalse(x->pResource->getTexture2DAtlasExists(strTextureAtlasName), "CUITheme::getTextureAtlas() failed. The texture atlas named \"" + strTextureAtlasName + "\" does not exist.");
		return x->pResource->getTexture2DAtlas(strTextureAtlasName);
	}

	bool CUITheme::getResourcesAdded(void)
	{
		return _mbResourcesAdded;
	}

	void CUITheme::_helperSetAudio(AudioSampleInfo& audio, const std::string& strSampleName, float fPitch, float fVolume)
	{
		audio.pitch = fPitch;
		audio.sampleName = strSampleName;
		audio.volume = fVolume;
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

	void CUITheme::_helperCheckImageDimsAreOK(SImageType& imageType, const std::string& strWidgetName)
	{
		CResourceTexture2DAtlas* pAtlas = getTextureAtlas();
		std::string strErr;
		CImageAtlasDetails imageDetails[3];

		// Top row height, colour image data
		imageDetails[0] = pAtlas->getImageDetails(imageType.colour.cornerTL);
		imageDetails[1] = pAtlas->getImageDetails(imageType.colour.edgeT);
		imageDetails[2] = pAtlas->getImageDetails(imageType.colour.cornerTR);
		if (imageDetails[0].vDims.y != imageDetails[1].vDims.y ||
			imageDetails[0].vDims.y != imageDetails[2].vDims.y)
		{
			strErr = "CUITheme::_helperCheckImageDimsAreOK() for widget " + strWidgetName + " failed. Images for";
			strErr += "colour image data do not have the same height.\nOffending images: ";
			strErr += imageType.colour.cornerTL + ", " + imageType.colour.edgeT + ", " + imageType.colour.cornerTR + ".";
			ThrowIfTrue(1, strErr);
		}
		// Bottom row height, colour image data
		imageDetails[0] = pAtlas->getImageDetails(imageType.colour.cornerBL);
		imageDetails[1] = pAtlas->getImageDetails(imageType.colour.edgeB);
		imageDetails[2] = pAtlas->getImageDetails(imageType.colour.cornerBR);
		if (imageDetails[0].vDims.y != imageDetails[1].vDims.y ||
			imageDetails[0].vDims.y != imageDetails[2].vDims.y)
		{
			strErr = "CUITheme::_helperCheckImageDimsAreOK() for widget " + strWidgetName + " failed. Images for";
			strErr += "colour image data do not have the same height.\nOffending images: ";
			strErr += imageType.colour.cornerBL + ", " + imageType.colour.edgeB + ", " + imageType.colour.cornerBR + ".";
			ThrowIfTrue(1, strErr);
		}

		// Left edge width, colour image data
		imageDetails[0] = pAtlas->getImageDetails(imageType.colour.cornerBL);
		imageDetails[1] = pAtlas->getImageDetails(imageType.colour.edgeL);
		imageDetails[2] = pAtlas->getImageDetails(imageType.colour.cornerTL);
		if (imageDetails[0].vDims.x != imageDetails[1].vDims.x ||
			imageDetails[0].vDims.x != imageDetails[2].vDims.x)
		{
			strErr = "CUITheme::_helperCheckImageDimsAreOK() for widget " + strWidgetName + " failed. Images for";
			strErr += "colour image data do not have the same width.\nOffending images: ";
			strErr += imageType.colour.cornerBL + ", " + imageType.colour.edgeL + ", " + imageType.colour.cornerTL + ".";
			ThrowIfTrue(1, strErr);
		}

		// Right edge width, colour image data
		imageDetails[0] = pAtlas->getImageDetails(imageType.colour.cornerBR);
		imageDetails[1] = pAtlas->getImageDetails(imageType.colour.edgeR);
		imageDetails[2] = pAtlas->getImageDetails(imageType.colour.cornerTR);
		if (imageDetails[0].vDims.x != imageDetails[1].vDims.x ||
			imageDetails[0].vDims.x != imageDetails[2].vDims.x)
		{
			strErr = "CUITheme::_helperCheckImageDimsAreOK() for widget " + strWidgetName + " failed. Images for";
			strErr += "colour image data do not have the same width.\nOffending images: ";
			strErr += imageType.colour.cornerBR + ", " + imageType.colour.edgeR + ", " + imageType.colour.cornerTR + ".";
			ThrowIfTrue(1, strErr);
		}

		// Top row height, height image data
		imageDetails[0] = pAtlas->getImageDetails(imageType.height.cornerTL);
		imageDetails[1] = pAtlas->getImageDetails(imageType.height.edgeT);
		imageDetails[2] = pAtlas->getImageDetails(imageType.height.cornerTR);
		if (imageDetails[0].vDims.y != imageDetails[1].vDims.y ||
			imageDetails[0].vDims.y != imageDetails[2].vDims.y)
		{
			strErr = "CUITheme::_helperCheckImageDimsAreOK() for widget " + strWidgetName + " failed. Images for";
			strErr += "height image data do not have the same height.\nOffending images: ";
			strErr += imageType.height.cornerTL + ", " + imageType.height.edgeT + ", " + imageType.height.cornerTR + ".";
			ThrowIfTrue(1, strErr);
		}
		// Bottom row height, height image data
		imageDetails[0] = pAtlas->getImageDetails(imageType.height.cornerBL);
		imageDetails[1] = pAtlas->getImageDetails(imageType.height.edgeB);
		imageDetails[2] = pAtlas->getImageDetails(imageType.height.cornerBR);
		if (imageDetails[0].vDims.y != imageDetails[1].vDims.y ||
			imageDetails[0].vDims.y != imageDetails[2].vDims.y)
		{
			strErr = "CUITheme::_helperCheckImageDimsAreOK() for widget " + strWidgetName + " failed. Images for";
			strErr += "height image data do not have the same height.\nOffending images: ";
			strErr += imageType.height.cornerBL + ", " + imageType.height.edgeB + ", " + imageType.height.cornerBR + ".";
			ThrowIfTrue(1, strErr);
		}

		// Left edge width, height image data
		imageDetails[0] = pAtlas->getImageDetails(imageType.height.cornerBL);
		imageDetails[1] = pAtlas->getImageDetails(imageType.height.edgeL);
		imageDetails[2] = pAtlas->getImageDetails(imageType.height.cornerTL);
		if (imageDetails[0].vDims.x != imageDetails[1].vDims.x ||
			imageDetails[0].vDims.x != imageDetails[2].vDims.x)
		{
			strErr = "CUITheme::_helperCheckImageDimsAreOK() for widget " + strWidgetName + " failed. Images for";
			strErr += "height image data do not have the same width.\nOffending images: ";
			strErr += imageType.height.cornerBL + ", " + imageType.height.edgeL + ", " + imageType.height.cornerTL + ".";
			ThrowIfTrue(1, strErr);
		}

		// Right edge width, height image data
		imageDetails[0] = pAtlas->getImageDetails(imageType.height.cornerBR);
		imageDetails[1] = pAtlas->getImageDetails(imageType.height.edgeR);
		imageDetails[2] = pAtlas->getImageDetails(imageType.height.cornerTR);
		if (imageDetails[0].vDims.x != imageDetails[1].vDims.x ||
			imageDetails[0].vDims.x != imageDetails[2].vDims.x)
		{
			strErr = "CUITheme::_helperCheckImageDimsAreOK() for widget " + strWidgetName + " failed. Images for";
			strErr += "height image data do not have the same width.\nOffending images: ";
			strErr += imageType.height.cornerBR + ", " + imageType.height.edgeR + ", " + imageType.height.cornerTR + ".";
			ThrowIfTrue(1, strErr);
		}

		// Top row height, normal image data
		imageDetails[0] = pAtlas->getImageDetails(imageType.normal.cornerTL);
		imageDetails[1] = pAtlas->getImageDetails(imageType.normal.edgeT);
		imageDetails[2] = pAtlas->getImageDetails(imageType.normal.cornerTR);
		if (imageDetails[0].vDims.y != imageDetails[1].vDims.y ||
			imageDetails[0].vDims.y != imageDetails[2].vDims.y)
		{
			strErr = "CUITheme::_helperCheckImageDimsAreOK() for widget " + strWidgetName + " failed. Images for";
			strErr += "normal image data do not have the same height.\nOffending images: ";
			strErr += imageType.normal.cornerTL + ", " + imageType.normal.edgeT + ", " + imageType.normal.cornerTR + ".";
			ThrowIfTrue(1, strErr);
		}
		// Bottom row height, normal image data
		imageDetails[0] = pAtlas->getImageDetails(imageType.normal.cornerBL);
		imageDetails[1] = pAtlas->getImageDetails(imageType.normal.edgeB);
		imageDetails[2] = pAtlas->getImageDetails(imageType.normal.cornerBR);
		if (imageDetails[0].vDims.y != imageDetails[1].vDims.y ||
			imageDetails[0].vDims.y != imageDetails[2].vDims.y)
		{
			strErr = "CUITheme::_helperCheckImageDimsAreOK() for widget " + strWidgetName + " failed. Images for";
			strErr += "normal image data do not have the same height.\nOffending images: ";
			strErr += imageType.normal.cornerBL + ", " + imageType.normal.edgeB + ", " + imageType.normal.cornerBR + ".";
			ThrowIfTrue(1, strErr);
		}

		// Left edge width, normal image data
		imageDetails[0] = pAtlas->getImageDetails(imageType.normal.cornerBL);
		imageDetails[1] = pAtlas->getImageDetails(imageType.normal.edgeL);
		imageDetails[2] = pAtlas->getImageDetails(imageType.normal.cornerTL);
		if (imageDetails[0].vDims.x != imageDetails[1].vDims.x ||
			imageDetails[0].vDims.x != imageDetails[2].vDims.x)
		{
			strErr = "CUITheme::_helperCheckImageDimsAreOK() for widget " + strWidgetName + " failed. Images for";
			strErr += "normal image data do not have the same width.\nOffending images: ";
			strErr += imageType.normal.cornerBL + ", " + imageType.normal.edgeL + ", " + imageType.normal.cornerTL + ".";
			ThrowIfTrue(1, strErr);
		}

		// Right edge width, normal image data
		imageDetails[0] = pAtlas->getImageDetails(imageType.normal.cornerBR);
		imageDetails[1] = pAtlas->getImageDetails(imageType.normal.edgeR);
		imageDetails[2] = pAtlas->getImageDetails(imageType.normal.cornerTR);
		if (imageDetails[0].vDims.x != imageDetails[1].vDims.x ||
			imageDetails[0].vDims.x != imageDetails[2].vDims.x)
		{
			strErr = "CUITheme::_helperCheckImageDimsAreOK() for widget " + strWidgetName + " failed. Images for";
			strErr += "normal image data do not have the same width.\nOffending images: ";
			strErr += imageType.normal.cornerBR + ", " + imageType.normal.edgeR + ", " + imageType.normal.cornerTR + ".";
			ThrowIfTrue(1, strErr);
		}
	}

	void CUITheme::_helperBuildNormalImages(SImageType& imageType, bool bOverwriteNormalImages)
	{
		CImage imageHeight;
		CImage imageNormal;
		// For each of the 9 images in the grid, if the normal file doesn't exist, or we're overwriting, load the height image, create normal image and save.
		if (!x->pFile->getFileExists(imageType.normal.centre) || bOverwriteNormalImages)
		{
			imageHeight.load(imageType.height.centre);		imageHeight.normalmap(imageNormal);	imageNormal.saveAsPNG(imageType.normal.centre);
		}
		if (!x->pFile->getFileExists(imageType.normal.cornerBL) || bOverwriteNormalImages)
		{
			imageHeight.load(imageType.height.cornerBL);	imageHeight.normalmap(imageNormal);	imageNormal.saveAsPNG(imageType.normal.cornerBL);
		}
		if (!x->pFile->getFileExists(imageType.normal.cornerBR) || bOverwriteNormalImages)
		{
			imageHeight.load(imageType.height.cornerBR);	imageHeight.normalmap(imageNormal);	imageNormal.saveAsPNG(imageType.normal.cornerBR);
		}
		if (!x->pFile->getFileExists(imageType.normal.cornerTL) || bOverwriteNormalImages)
		{
			imageHeight.load(imageType.height.cornerTL);	imageHeight.normalmap(imageNormal);	imageNormal.saveAsPNG(imageType.normal.cornerTL);
		}
		if (!x->pFile->getFileExists(imageType.normal.cornerTR) || bOverwriteNormalImages)
		{
			imageHeight.load(imageType.height.cornerTR);	imageHeight.normalmap(imageNormal);	imageNormal.saveAsPNG(imageType.normal.cornerTR);
		}
		if (!x->pFile->getFileExists(imageType.normal.edgeB) || bOverwriteNormalImages)
		{
			imageHeight.load(imageType.height.edgeB);	imageHeight.normalmap(imageNormal);	imageNormal.saveAsPNG(imageType.normal.edgeB);
		}
		if (!x->pFile->getFileExists(imageType.normal.edgeL) || bOverwriteNormalImages)
		{
			imageHeight.load(imageType.height.edgeL);	imageHeight.normalmap(imageNormal);	imageNormal.saveAsPNG(imageType.normal.edgeL);
		}
		if (!x->pFile->getFileExists(imageType.normal.edgeR) || bOverwriteNormalImages)
		{
			imageHeight.load(imageType.height.edgeR);	imageHeight.normalmap(imageNormal);	imageNormal.saveAsPNG(imageType.normal.edgeR);
		}
		if (!x->pFile->getFileExists(imageType.normal.edgeT) || bOverwriteNormalImages)
		{
			imageHeight.load(imageType.height.edgeT);	imageHeight.normalmap(imageNormal);	imageNormal.saveAsPNG(imageType.normal.edgeT);
		}
	}
}