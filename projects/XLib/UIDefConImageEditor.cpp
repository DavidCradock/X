#include "PCH.h"
#include "UIDefConImageEditor.h"
#include "UIManager.h"
#include "singletons.h"
#include "UITooltip.h"
#include "stringUtils.h"

namespace X
{
	void onClickedNewImageCreate(void)
	{
		CUIDefaultContainers* pDefCons = x->pUI->getDefaultContainers();
		CUIWindow* pWindowNewImage = x->pUI->windowAdd(pDefCons->names.imageEditor + "new_image");
		CUITextEdit* pTE = pWindowNewImage->textEditGet("new_image_width");
		std::string strWidth = pTE->getText();
		pTE = pWindowNewImage->textEditGet("new_image_height");
		std::string strHeight = pTE->getText();
		int iWidth = StringUtils::stringToInt(strWidth);
		int iHeight = StringUtils::stringToInt(strHeight);
		if (iWidth < 1 || iWidth > 65535)
			iWidth = 320;
		if (iHeight < 1 || iHeight > 65535)
			iHeight = 240;
		//pDefCons->imageEditor.
	}

	CUIDefConImageEditor::CUIDefConImageEditor()
	{

	}

	void CUIDefConImageEditor::initialise(void)
	{
		CUIDefaultContainers* pDefCons = x->pUI->getDefaultContainers();

		// Main window
		CUIWindow* pWindow = x->pUI->windowAdd(pDefCons->names.imageEditor, true);
		pWindow->setVisible(false);
		pWindow->setDimensions(600, 400);
		pWindow->setPosition(200, 200);
		CUIButton* pButton = pWindow->buttonAdd("new", 0, 0, 200, 35);
		pButton->pTooltip->setText("Create a new image with the currently set dimensions.");

		// New image window
		CUIWindow* pWindowNewImage = x->pUI->windowAdd(pDefCons->names.imageEditor + "new_image", true);
		pWindowNewImage->setVisible(false);
		pWindowNewImage->setTitlebarText("New Image");
		pWindowNewImage->setDimensions(250, 180);
		CUIText* pText = pWindowNewImage->textAdd("width", 0, 10, 150, 40, "Width:");
		CUITextEdit* pTE = pWindowNewImage->textEditAdd("new_image_width", 150, 10, 100, 35, "640");
		pTE->pTooltip->setText("Width of the new image in pixels.\nValid range: 1 to 65536");
		pTE->setIntegerInputOnly(true);
		pTE->setMaxChars(5);
		pText = pWindowNewImage->textAdd("height", 0, 50, 150, 40, "Height:");
		pTE = pWindowNewImage->textEditAdd("new_image_height", 150, 50, 100, 35, "480");
		pTE->pTooltip->setText("Height of the new image in pixels.\nValid range: 1 to 65536");
		pTE->setIntegerInputOnly(true);
		pTE->setMaxChars(5);
		pText = pWindowNewImage->textAdd("numchannels", 0, 90, 150, 40, "NumChannels:");
		pTE = pWindowNewImage->textEditAdd("new_image_channels", 150, 90, 100, 35, "4");
		pTE->pTooltip->setText("The number of channels of the new image in pixels.\nValid range: 3 to 4");
		pTE->setIntegerInputOnly(true);
		pTE->setMaxChars(5);
		pButton = pWindowNewImage->buttonAdd("Create", 10, 140, 100, 40);
		pButton->pTooltip->setText("Create the new image with the above settings.");
		pButton->setFunctionOnClicked(onClickedNewImageCreate);
		pButton = pWindowNewImage->buttonAdd("Cancel", 140, 140, 100, 40);
		pButton->pTooltip->setText("Whoops, I've changed my mind.");
	}

	void CUIDefConImageEditor::update(float fTimeDeltaSecs)
	{
		CUIDefaultContainers* pDefCons = x->pUI->getDefaultContainers();

		// Is the window visible? If not, simply return
		CUIWindow* pWindow = x->pUI->windowGet(pDefCons->names.imageEditor);
		if (!pWindow->getVisible())
			return;
	}
}