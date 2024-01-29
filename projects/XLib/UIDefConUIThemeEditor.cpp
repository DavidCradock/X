#include "PCH.h"
#include "UIDefConUIThemeEditor.h"
#include "UIManager.h"
#include "singletons.h"
#include "UITooltip.h"

namespace X
{
	CUIDefConUIThemeEditor::CUIDefConUIThemeEditor()
	{

	}

	void uiClickedMainWindowHelpButton(void)
	{
		CUIDefaultContainers* pDefCons = x->pUI->getDefaultContainers();
		x->pUI->windowGet(pDefCons->names.uiThemeEditor + "_Help")->setVisible(true, true);
	}

	void uiClickedMainWindowWidgetPreviewButton(void)
	{
		CUIDefaultContainers* pDefCons = x->pUI->getDefaultContainers();
		x->pUI->windowGet(pDefCons->names.uiThemeEditor + "_widget_preview")->setVisible(true, true);
	}

	void uiClicked_ambientLight(void)
	{
		CUIDefaultContainers* pDefCons = x->pUI->getDefaultContainers();
		CUIWindow* pWindowWidgetPreview = x->pUI->windowGet(pDefCons->names.uiThemeEditor + "_widget_preview");
		CUIWindow* pWindowMain = x->pUI->windowGet(pDefCons->names.uiThemeEditor);
		// Edit next two lines for this CUIImage object
		std::string strText = "ambientLight";
		pDefCons->themeEditor.pCurrentlySelectedThemeColour = &pWindowWidgetPreview->themeGetSettings()->colours.ambientLight;
		pWindowMain->textGet("Current colour")->setText(strText);
		pDefCons->themeEditor.pCurrentlySelectedThemeColourImageWidget = pWindowMain->imageGet(strText);
		pDefCons->themeEditor.pCurrentlySelectedThemeColourImageWidget->setColour(*pDefCons->themeEditor.pCurrentlySelectedThemeColour);
		pWindowMain->colourSelectorGet("colourSelector")->setColour(*pDefCons->themeEditor.pCurrentlySelectedThemeColour);
	}

	void uiClicked_buttonBGDown(void)
	{
		CUIDefaultContainers* pDefCons = x->pUI->getDefaultContainers();
		CUIWindow* pWindowWidgetPreview = x->pUI->windowGet(pDefCons->names.uiThemeEditor + "_widget_preview");
		CUIWindow* pWindowMain = x->pUI->windowGet(pDefCons->names.uiThemeEditor);
		// Edit next two lines for this CUIImage object
		std::string strText = "buttonBGDown";
		pDefCons->themeEditor.pCurrentlySelectedThemeColour = &pWindowWidgetPreview->themeGetSettings()->colours.buttonBGDown;
		pWindowMain->textGet("Current colour")->setText(strText);
		pDefCons->themeEditor.pCurrentlySelectedThemeColourImageWidget = pWindowMain->imageGet(strText);
		pDefCons->themeEditor.pCurrentlySelectedThemeColourImageWidget->setColour(*pDefCons->themeEditor.pCurrentlySelectedThemeColour);
		pWindowMain->colourSelectorGet("colourSelector")->setColour(*pDefCons->themeEditor.pCurrentlySelectedThemeColour);
	}

	void uiClicked_buttonBGOver(void)
	{
		CUIDefaultContainers* pDefCons = x->pUI->getDefaultContainers();
		CUIWindow* pWindowWidgetPreview = x->pUI->windowGet(pDefCons->names.uiThemeEditor + "_widget_preview");
		CUIWindow* pWindowMain = x->pUI->windowGet(pDefCons->names.uiThemeEditor);
		// Edit next two lines for this CUIImage object
		std::string strText = "buttonBGOver";
		pDefCons->themeEditor.pCurrentlySelectedThemeColour = &pWindowWidgetPreview->themeGetSettings()->colours.buttonBGOver;
		pWindowMain->textGet("Current colour")->setText(strText);
		pDefCons->themeEditor.pCurrentlySelectedThemeColourImageWidget = pWindowMain->imageGet(strText);
		pDefCons->themeEditor.pCurrentlySelectedThemeColourImageWidget->setColour(*pDefCons->themeEditor.pCurrentlySelectedThemeColour);
		pWindowMain->colourSelectorGet("colourSelector")->setColour(*pDefCons->themeEditor.pCurrentlySelectedThemeColour);
	}

	void uiClicked_buttonBGUp(void)
	{
		CUIDefaultContainers* pDefCons = x->pUI->getDefaultContainers();
		CUIWindow* pWindowWidgetPreview = x->pUI->windowGet(pDefCons->names.uiThemeEditor + "_widget_preview");
		CUIWindow* pWindowMain = x->pUI->windowGet(pDefCons->names.uiThemeEditor);
		// Edit next two lines for this CUIImage object
		std::string strText = "buttonBGUp";
		pDefCons->themeEditor.pCurrentlySelectedThemeColour = &pWindowWidgetPreview->themeGetSettings()->colours.buttonBGUp;
		pWindowMain->textGet("Current colour")->setText(strText);
		pDefCons->themeEditor.pCurrentlySelectedThemeColourImageWidget = pWindowMain->imageGet(strText);
		pDefCons->themeEditor.pCurrentlySelectedThemeColourImageWidget->setColour(*pDefCons->themeEditor.pCurrentlySelectedThemeColour);
		pWindowMain->colourSelectorGet("colourSelector")->setColour(*pDefCons->themeEditor.pCurrentlySelectedThemeColour);
	}
	
	void uiClicked_buttonImageBGDown(void)
	{
		CUIDefaultContainers* pDefCons = x->pUI->getDefaultContainers();
		CUIWindow* pWindowWidgetPreview = x->pUI->windowGet(pDefCons->names.uiThemeEditor + "_widget_preview");
		CUIWindow* pWindowMain = x->pUI->windowGet(pDefCons->names.uiThemeEditor);
		// Edit next two lines for this CUIImage object
		std::string strText = "buttonImageBGDown";
		pDefCons->themeEditor.pCurrentlySelectedThemeColour = &pWindowWidgetPreview->themeGetSettings()->colours.buttonImageBGDown;
		pWindowMain->textGet("Current colour")->setText(strText);
		pDefCons->themeEditor.pCurrentlySelectedThemeColourImageWidget = pWindowMain->imageGet(strText);
		pDefCons->themeEditor.pCurrentlySelectedThemeColourImageWidget->setColour(*pDefCons->themeEditor.pCurrentlySelectedThemeColour);
		pWindowMain->colourSelectorGet("colourSelector")->setColour(*pDefCons->themeEditor.pCurrentlySelectedThemeColour);
	}

	void uiClicked_buttonImageBGOver(void)
	{
		CUIDefaultContainers* pDefCons = x->pUI->getDefaultContainers();
		CUIWindow* pWindowWidgetPreview = x->pUI->windowGet(pDefCons->names.uiThemeEditor + "_widget_preview");
		CUIWindow* pWindowMain = x->pUI->windowGet(pDefCons->names.uiThemeEditor);
		// Edit next two lines for this CUIImage object
		std::string strText = "buttonImageBGOver";
		pDefCons->themeEditor.pCurrentlySelectedThemeColour = &pWindowWidgetPreview->themeGetSettings()->colours.buttonImageBGOver;
		pWindowMain->textGet("Current colour")->setText(strText);
		pDefCons->themeEditor.pCurrentlySelectedThemeColourImageWidget = pWindowMain->imageGet(strText);
		pDefCons->themeEditor.pCurrentlySelectedThemeColourImageWidget->setColour(*pDefCons->themeEditor.pCurrentlySelectedThemeColour);
		pWindowMain->colourSelectorGet("colourSelector")->setColour(*pDefCons->themeEditor.pCurrentlySelectedThemeColour);
	}

	void uiClicked_buttonImageBGUp(void)
	{
		CUIDefaultContainers* pDefCons = x->pUI->getDefaultContainers();
		CUIWindow* pWindowWidgetPreview = x->pUI->windowGet(pDefCons->names.uiThemeEditor + "_widget_preview");
		CUIWindow* pWindowMain = x->pUI->windowGet(pDefCons->names.uiThemeEditor);
		// Edit next two lines for this CUIImage object
		std::string strText = "buttonImageBGUp";
		pDefCons->themeEditor.pCurrentlySelectedThemeColour = &pWindowWidgetPreview->themeGetSettings()->colours.buttonImageBGUp;
		pWindowMain->textGet("Current colour")->setText(strText);
		pDefCons->themeEditor.pCurrentlySelectedThemeColourImageWidget = pWindowMain->imageGet(strText);
		pDefCons->themeEditor.pCurrentlySelectedThemeColourImageWidget->setColour(*pDefCons->themeEditor.pCurrentlySelectedThemeColour);
		pWindowMain->colourSelectorGet("colourSelector")->setColour(*pDefCons->themeEditor.pCurrentlySelectedThemeColour);
	}

	void uiClicked_buttonImageTextDown(void)
	{
		CUIDefaultContainers* pDefCons = x->pUI->getDefaultContainers();
		CUIWindow* pWindowWidgetPreview = x->pUI->windowGet(pDefCons->names.uiThemeEditor + "_widget_preview");
		CUIWindow* pWindowMain = x->pUI->windowGet(pDefCons->names.uiThemeEditor);
		// Edit next two lines for this CUIImage object
		std::string strText = "buttonImageTextDown";
		pDefCons->themeEditor.pCurrentlySelectedThemeColour = &pWindowWidgetPreview->themeGetSettings()->colours.buttonImageTextDown;
		pWindowMain->textGet("Current colour")->setText(strText);
		pDefCons->themeEditor.pCurrentlySelectedThemeColourImageWidget = pWindowMain->imageGet(strText);
		pDefCons->themeEditor.pCurrentlySelectedThemeColourImageWidget->setColour(*pDefCons->themeEditor.pCurrentlySelectedThemeColour);
		pWindowMain->colourSelectorGet("colourSelector")->setColour(*pDefCons->themeEditor.pCurrentlySelectedThemeColour);
	}

	void uiClicked_buttonImageTextOver(void)
	{
		CUIDefaultContainers* pDefCons = x->pUI->getDefaultContainers();
		CUIWindow* pWindowWidgetPreview = x->pUI->windowGet(pDefCons->names.uiThemeEditor + "_widget_preview");
		CUIWindow* pWindowMain = x->pUI->windowGet(pDefCons->names.uiThemeEditor);
		// Edit next two lines for this CUIImage object
		std::string strText = "buttonImageTextOver";
		pDefCons->themeEditor.pCurrentlySelectedThemeColour = &pWindowWidgetPreview->themeGetSettings()->colours.buttonImageTextOver;
		pWindowMain->textGet("Current colour")->setText(strText);
		pDefCons->themeEditor.pCurrentlySelectedThemeColourImageWidget = pWindowMain->imageGet(strText);
		pDefCons->themeEditor.pCurrentlySelectedThemeColourImageWidget->setColour(*pDefCons->themeEditor.pCurrentlySelectedThemeColour);
		pWindowMain->colourSelectorGet("colourSelector")->setColour(*pDefCons->themeEditor.pCurrentlySelectedThemeColour);
	}

	void uiClicked_buttonImageTextUp(void)
	{
		CUIDefaultContainers* pDefCons = x->pUI->getDefaultContainers();
		CUIWindow* pWindowWidgetPreview = x->pUI->windowGet(pDefCons->names.uiThemeEditor + "_widget_preview");
		CUIWindow* pWindowMain = x->pUI->windowGet(pDefCons->names.uiThemeEditor);
		// Edit next two lines for this CUIImage object
		std::string strText = "buttonImageTextUp";
		pDefCons->themeEditor.pCurrentlySelectedThemeColour = &pWindowWidgetPreview->themeGetSettings()->colours.buttonImageTextUp;
		pWindowMain->textGet("Current colour")->setText(strText);
		pDefCons->themeEditor.pCurrentlySelectedThemeColourImageWidget = pWindowMain->imageGet(strText);
		pDefCons->themeEditor.pCurrentlySelectedThemeColourImageWidget->setColour(*pDefCons->themeEditor.pCurrentlySelectedThemeColour);
		pWindowMain->colourSelectorGet("colourSelector")->setColour(*pDefCons->themeEditor.pCurrentlySelectedThemeColour);
	}

	void uiClicked_buttonTextDown(void)
	{
		CUIDefaultContainers* pDefCons = x->pUI->getDefaultContainers();
		CUIWindow* pWindowWidgetPreview = x->pUI->windowGet(pDefCons->names.uiThemeEditor + "_widget_preview");
		CUIWindow* pWindowMain = x->pUI->windowGet(pDefCons->names.uiThemeEditor);
		// Edit next two lines for this CUIImage object
		std::string strText = "buttonTextDown";
		pDefCons->themeEditor.pCurrentlySelectedThemeColour = &pWindowWidgetPreview->themeGetSettings()->colours.buttonTextDown;
		pWindowMain->textGet("Current colour")->setText(strText);
		pDefCons->themeEditor.pCurrentlySelectedThemeColourImageWidget = pWindowMain->imageGet(strText);
		pDefCons->themeEditor.pCurrentlySelectedThemeColourImageWidget->setColour(*pDefCons->themeEditor.pCurrentlySelectedThemeColour);
		pWindowMain->colourSelectorGet("colourSelector")->setColour(*pDefCons->themeEditor.pCurrentlySelectedThemeColour);
	}

	void uiClicked_buttonTextOver(void)
	{
		CUIDefaultContainers* pDefCons = x->pUI->getDefaultContainers();
		CUIWindow* pWindowWidgetPreview = x->pUI->windowGet(pDefCons->names.uiThemeEditor + "_widget_preview");
		CUIWindow* pWindowMain = x->pUI->windowGet(pDefCons->names.uiThemeEditor);
		// Edit next two lines for this CUIImage object
		std::string strText = "buttonTextOver";
		pDefCons->themeEditor.pCurrentlySelectedThemeColour = &pWindowWidgetPreview->themeGetSettings()->colours.buttonTextOver;
		pWindowMain->textGet("Current colour")->setText(strText);
		pDefCons->themeEditor.pCurrentlySelectedThemeColourImageWidget = pWindowMain->imageGet(strText);
		pDefCons->themeEditor.pCurrentlySelectedThemeColourImageWidget->setColour(*pDefCons->themeEditor.pCurrentlySelectedThemeColour);
		pWindowMain->colourSelectorGet("colourSelector")->setColour(*pDefCons->themeEditor.pCurrentlySelectedThemeColour);
	}

	void uiClicked_buttonTextUp(void)
	{
		CUIDefaultContainers* pDefCons = x->pUI->getDefaultContainers();
		CUIWindow* pWindowWidgetPreview = x->pUI->windowGet(pDefCons->names.uiThemeEditor + "_widget_preview");
		CUIWindow* pWindowMain = x->pUI->windowGet(pDefCons->names.uiThemeEditor);
		// Edit next two lines for this CUIImage object
		std::string strText = "buttonTextUp";
		pDefCons->themeEditor.pCurrentlySelectedThemeColour = &pWindowWidgetPreview->themeGetSettings()->colours.buttonTextUp;
		pWindowMain->textGet("Current colour")->setText(strText);
		pDefCons->themeEditor.pCurrentlySelectedThemeColourImageWidget = pWindowMain->imageGet(strText);
		pDefCons->themeEditor.pCurrentlySelectedThemeColourImageWidget->setColour(*pDefCons->themeEditor.pCurrentlySelectedThemeColour);
		pWindowMain->colourSelectorGet("colourSelector")->setColour(*pDefCons->themeEditor.pCurrentlySelectedThemeColour);
	}

	void uiClicked_checkboxBGOff(void)
	{
		CUIDefaultContainers* pDefCons = x->pUI->getDefaultContainers();
		CUIWindow* pWindowWidgetPreview = x->pUI->windowGet(pDefCons->names.uiThemeEditor + "_widget_preview");
		CUIWindow* pWindowMain = x->pUI->windowGet(pDefCons->names.uiThemeEditor);
		// Edit next two lines for this CUIImage object
		std::string strText = "checkboxBGOff";
		pDefCons->themeEditor.pCurrentlySelectedThemeColour = &pWindowWidgetPreview->themeGetSettings()->colours.checkboxBGOff;
		pWindowMain->textGet("Current colour")->setText(strText);
		pDefCons->themeEditor.pCurrentlySelectedThemeColourImageWidget = pWindowMain->imageGet(strText);
		pDefCons->themeEditor.pCurrentlySelectedThemeColourImageWidget->setColour(*pDefCons->themeEditor.pCurrentlySelectedThemeColour);
		pWindowMain->colourSelectorGet("colourSelector")->setColour(*pDefCons->themeEditor.pCurrentlySelectedThemeColour);
	}

	void uiClicked_checkboxBGOffOver(void)
	{
		CUIDefaultContainers* pDefCons = x->pUI->getDefaultContainers();
		CUIWindow* pWindowWidgetPreview = x->pUI->windowGet(pDefCons->names.uiThemeEditor + "_widget_preview");
		CUIWindow* pWindowMain = x->pUI->windowGet(pDefCons->names.uiThemeEditor);
		// Edit next two lines for this CUIImage object
		std::string strText = "checkboxBGOffOver";
		pDefCons->themeEditor.pCurrentlySelectedThemeColour = &pWindowWidgetPreview->themeGetSettings()->colours.checkboxBGOffOver;
		pWindowMain->textGet("Current colour")->setText(strText);
		pDefCons->themeEditor.pCurrentlySelectedThemeColourImageWidget = pWindowMain->imageGet(strText);
		pDefCons->themeEditor.pCurrentlySelectedThemeColourImageWidget->setColour(*pDefCons->themeEditor.pCurrentlySelectedThemeColour);
		pWindowMain->colourSelectorGet("colourSelector")->setColour(*pDefCons->themeEditor.pCurrentlySelectedThemeColour);
	}

	void uiClicked_checkboxBGOn(void)
	{
		CUIDefaultContainers* pDefCons = x->pUI->getDefaultContainers();
		CUIWindow* pWindowWidgetPreview = x->pUI->windowGet(pDefCons->names.uiThemeEditor + "_widget_preview");
		CUIWindow* pWindowMain = x->pUI->windowGet(pDefCons->names.uiThemeEditor);
		// Edit next two lines for this CUIImage object
		std::string strText = "checkboxBGOn";
		pDefCons->themeEditor.pCurrentlySelectedThemeColour = &pWindowWidgetPreview->themeGetSettings()->colours.checkboxBGOn;
		pWindowMain->textGet("Current colour")->setText(strText);
		pDefCons->themeEditor.pCurrentlySelectedThemeColourImageWidget = pWindowMain->imageGet(strText);
		pDefCons->themeEditor.pCurrentlySelectedThemeColourImageWidget->setColour(*pDefCons->themeEditor.pCurrentlySelectedThemeColour);
		pWindowMain->colourSelectorGet("colourSelector")->setColour(*pDefCons->themeEditor.pCurrentlySelectedThemeColour);
	}

	void uiClicked_checkboxBGOnOver(void)
	{
		CUIDefaultContainers* pDefCons = x->pUI->getDefaultContainers();
		CUIWindow* pWindowWidgetPreview = x->pUI->windowGet(pDefCons->names.uiThemeEditor + "_widget_preview");
		CUIWindow* pWindowMain = x->pUI->windowGet(pDefCons->names.uiThemeEditor);
		// Edit next two lines for this CUIImage object
		std::string strText = "checkboxBGOnOver";
		pDefCons->themeEditor.pCurrentlySelectedThemeColour = &pWindowWidgetPreview->themeGetSettings()->colours.checkboxBGOnOver;
		pWindowMain->textGet("Current colour")->setText(strText);
		pDefCons->themeEditor.pCurrentlySelectedThemeColourImageWidget = pWindowMain->imageGet(strText);
		pDefCons->themeEditor.pCurrentlySelectedThemeColourImageWidget->setColour(*pDefCons->themeEditor.pCurrentlySelectedThemeColour);
		pWindowMain->colourSelectorGet("colourSelector")->setColour(*pDefCons->themeEditor.pCurrentlySelectedThemeColour);
	}

	void uiClicked_linegraphBG(void)
	{
		CUIDefaultContainers* pDefCons = x->pUI->getDefaultContainers();
		CUIWindow* pWindowWidgetPreview = x->pUI->windowGet(pDefCons->names.uiThemeEditor + "_widget_preview");
		CUIWindow* pWindowMain = x->pUI->windowGet(pDefCons->names.uiThemeEditor);
		// Edit next two lines for this CUIImage object
		std::string strText = "linegraphBG";
		pDefCons->themeEditor.pCurrentlySelectedThemeColour = &pWindowWidgetPreview->themeGetSettings()->colours.linegraphBG;
		pWindowMain->textGet("Current colour")->setText(strText);
		pDefCons->themeEditor.pCurrentlySelectedThemeColourImageWidget = pWindowMain->imageGet(strText);
		pDefCons->themeEditor.pCurrentlySelectedThemeColourImageWidget->setColour(*pDefCons->themeEditor.pCurrentlySelectedThemeColour);
		pWindowMain->colourSelectorGet("colourSelector")->setColour(*pDefCons->themeEditor.pCurrentlySelectedThemeColour);
	}

	void uiClicked_mouseLight(void)
	{
		CUIDefaultContainers* pDefCons = x->pUI->getDefaultContainers();
		CUIWindow* pWindowWidgetPreview = x->pUI->windowGet(pDefCons->names.uiThemeEditor + "_widget_preview");
		CUIWindow* pWindowMain = x->pUI->windowGet(pDefCons->names.uiThemeEditor);
		// Edit next two lines for this CUIImage object
		std::string strText = "mouseLight";
		pDefCons->themeEditor.pCurrentlySelectedThemeColour = &pWindowWidgetPreview->themeGetSettings()->colours.mouseLight;
		pWindowMain->textGet("Current colour")->setText(strText);
		pDefCons->themeEditor.pCurrentlySelectedThemeColourImageWidget = pWindowMain->imageGet(strText);
		pDefCons->themeEditor.pCurrentlySelectedThemeColourImageWidget->setColour(*pDefCons->themeEditor.pCurrentlySelectedThemeColour);
		pWindowMain->colourSelectorGet("colourSelector")->setColour(*pDefCons->themeEditor.pCurrentlySelectedThemeColour);
	}

	void uiClicked_progressBarBG(void)
	{
		CUIDefaultContainers* pDefCons = x->pUI->getDefaultContainers();
		CUIWindow* pWindowWidgetPreview = x->pUI->windowGet(pDefCons->names.uiThemeEditor + "_widget_preview");
		CUIWindow* pWindowMain = x->pUI->windowGet(pDefCons->names.uiThemeEditor);
		// Edit next two lines for this CUIImage object
		std::string strText = "progressBarBG";
		pDefCons->themeEditor.pCurrentlySelectedThemeColour = &pWindowWidgetPreview->themeGetSettings()->colours.progressBarBG;
		pWindowMain->textGet("Current colour")->setText(strText);
		pDefCons->themeEditor.pCurrentlySelectedThemeColourImageWidget = pWindowMain->imageGet(strText);
		pDefCons->themeEditor.pCurrentlySelectedThemeColourImageWidget->setColour(*pDefCons->themeEditor.pCurrentlySelectedThemeColour);
		pWindowMain->colourSelectorGet("colourSelector")->setColour(*pDefCons->themeEditor.pCurrentlySelectedThemeColour);
	}

	void uiClicked_progressBarFiller(void)
	{
		CUIDefaultContainers* pDefCons = x->pUI->getDefaultContainers();
		CUIWindow* pWindowWidgetPreview = x->pUI->windowGet(pDefCons->names.uiThemeEditor + "_widget_preview");
		CUIWindow* pWindowMain = x->pUI->windowGet(pDefCons->names.uiThemeEditor);
		// Edit next two lines for this CUIImage object
		std::string strText = "progressBarFiller";
		pDefCons->themeEditor.pCurrentlySelectedThemeColour = &pWindowWidgetPreview->themeGetSettings()->colours.progressBarFiller;
		pWindowMain->textGet("Current colour")->setText(strText);
		pDefCons->themeEditor.pCurrentlySelectedThemeColourImageWidget = pWindowMain->imageGet(strText);
		pDefCons->themeEditor.pCurrentlySelectedThemeColourImageWidget->setColour(*pDefCons->themeEditor.pCurrentlySelectedThemeColour);
		pWindowMain->colourSelectorGet("colourSelector")->setColour(*pDefCons->themeEditor.pCurrentlySelectedThemeColour);
	}

	void uiClicked_scrollbarBG(void)
	{
		CUIDefaultContainers* pDefCons = x->pUI->getDefaultContainers();
		CUIWindow* pWindowWidgetPreview = x->pUI->windowGet(pDefCons->names.uiThemeEditor + "_widget_preview");
		CUIWindow* pWindowMain = x->pUI->windowGet(pDefCons->names.uiThemeEditor);
		// Edit next two lines for this CUIImage object
		std::string strText = "scrollbarBG";
		pDefCons->themeEditor.pCurrentlySelectedThemeColour = &pWindowWidgetPreview->themeGetSettings()->colours.scrollbarBG;
		pWindowMain->textGet("Current colour")->setText(strText);
		pDefCons->themeEditor.pCurrentlySelectedThemeColourImageWidget = pWindowMain->imageGet(strText);
		pDefCons->themeEditor.pCurrentlySelectedThemeColourImageWidget->setColour(*pDefCons->themeEditor.pCurrentlySelectedThemeColour);
		pWindowMain->colourSelectorGet("colourSelector")->setColour(*pDefCons->themeEditor.pCurrentlySelectedThemeColour);
	}

	void uiClicked_scrollbarTabNotOver(void)
	{
		CUIDefaultContainers* pDefCons = x->pUI->getDefaultContainers();
		CUIWindow* pWindowWidgetPreview = x->pUI->windowGet(pDefCons->names.uiThemeEditor + "_widget_preview");
		CUIWindow* pWindowMain = x->pUI->windowGet(pDefCons->names.uiThemeEditor);
		// Edit next two lines for this CUIImage object
		std::string strText = "scrollbarTabNotOver";
		pDefCons->themeEditor.pCurrentlySelectedThemeColour = &pWindowWidgetPreview->themeGetSettings()->colours.scrollbarTabNotOver;
		pWindowMain->textGet("Current colour")->setText(strText);
		pDefCons->themeEditor.pCurrentlySelectedThemeColourImageWidget = pWindowMain->imageGet(strText);
		pDefCons->themeEditor.pCurrentlySelectedThemeColourImageWidget->setColour(*pDefCons->themeEditor.pCurrentlySelectedThemeColour);
		pWindowMain->colourSelectorGet("colourSelector")->setColour(*pDefCons->themeEditor.pCurrentlySelectedThemeColour);
	}

	void uiClicked_scrollbarTabOver(void)
	{
		CUIDefaultContainers* pDefCons = x->pUI->getDefaultContainers();
		CUIWindow* pWindowWidgetPreview = x->pUI->windowGet(pDefCons->names.uiThemeEditor + "_widget_preview");
		CUIWindow* pWindowMain = x->pUI->windowGet(pDefCons->names.uiThemeEditor);
		// Edit next two lines for this CUIImage object
		std::string strText = "scrollbarTabOver";
		pDefCons->themeEditor.pCurrentlySelectedThemeColour = &pWindowWidgetPreview->themeGetSettings()->colours.scrollbarTabOver;
		pWindowMain->textGet("Current colour")->setText(strText);
		pDefCons->themeEditor.pCurrentlySelectedThemeColourImageWidget = pWindowMain->imageGet(strText);
		pDefCons->themeEditor.pCurrentlySelectedThemeColourImageWidget->setColour(*pDefCons->themeEditor.pCurrentlySelectedThemeColour);
		pWindowMain->colourSelectorGet("colourSelector")->setColour(*pDefCons->themeEditor.pCurrentlySelectedThemeColour);
	}

	void uiClicked_text(void)
	{
		CUIDefaultContainers* pDefCons = x->pUI->getDefaultContainers();
		CUIWindow* pWindowWidgetPreview = x->pUI->windowGet(pDefCons->names.uiThemeEditor + "_widget_preview");
		CUIWindow* pWindowMain = x->pUI->windowGet(pDefCons->names.uiThemeEditor);
		// Edit next two lines for this CUIImage object
		std::string strText = "text";
		pDefCons->themeEditor.pCurrentlySelectedThemeColour = &pWindowWidgetPreview->themeGetSettings()->colours.text;
		pWindowMain->textGet("Current colour")->setText(strText);
		pDefCons->themeEditor.pCurrentlySelectedThemeColourImageWidget = pWindowMain->imageGet(strText);
		pDefCons->themeEditor.pCurrentlySelectedThemeColourImageWidget->setColour(*pDefCons->themeEditor.pCurrentlySelectedThemeColour);
		pWindowMain->colourSelectorGet("colourSelector")->setColour(*pDefCons->themeEditor.pCurrentlySelectedThemeColour);
	}

	void uiClicked_textEditBGActive(void)
	{
		CUIDefaultContainers* pDefCons = x->pUI->getDefaultContainers();
		CUIWindow* pWindowWidgetPreview = x->pUI->windowGet(pDefCons->names.uiThemeEditor + "_widget_preview");
		CUIWindow* pWindowMain = x->pUI->windowGet(pDefCons->names.uiThemeEditor);
		// Edit next two lines for this CUIImage object
		std::string strText = "textEditBGActive";
		pDefCons->themeEditor.pCurrentlySelectedThemeColour = &pWindowWidgetPreview->themeGetSettings()->colours.textEditBGActive;
		pWindowMain->textGet("Current colour")->setText(strText);
		pDefCons->themeEditor.pCurrentlySelectedThemeColourImageWidget = pWindowMain->imageGet(strText);
		pDefCons->themeEditor.pCurrentlySelectedThemeColourImageWidget->setColour(*pDefCons->themeEditor.pCurrentlySelectedThemeColour);
		pWindowMain->colourSelectorGet("colourSelector")->setColour(*pDefCons->themeEditor.pCurrentlySelectedThemeColour);
	}

	void uiClicked_textEditBGInactive(void)
	{
		CUIDefaultContainers* pDefCons = x->pUI->getDefaultContainers();
		CUIWindow* pWindowWidgetPreview = x->pUI->windowGet(pDefCons->names.uiThemeEditor + "_widget_preview");
		CUIWindow* pWindowMain = x->pUI->windowGet(pDefCons->names.uiThemeEditor);
		// Edit next two lines for this CUIImage object
		std::string strText = "textEditBGInactive";
		pDefCons->themeEditor.pCurrentlySelectedThemeColour = &pWindowWidgetPreview->themeGetSettings()->colours.textEditBGInactive;
		pWindowMain->textGet("Current colour")->setText(strText);
		pDefCons->themeEditor.pCurrentlySelectedThemeColourImageWidget = pWindowMain->imageGet(strText);
		pDefCons->themeEditor.pCurrentlySelectedThemeColourImageWidget->setColour(*pDefCons->themeEditor.pCurrentlySelectedThemeColour);
		pWindowMain->colourSelectorGet("colourSelector")->setColour(*pDefCons->themeEditor.pCurrentlySelectedThemeColour);
	}

	void uiClicked_textEditTextActive(void)
	{
		CUIDefaultContainers* pDefCons = x->pUI->getDefaultContainers();
		CUIWindow* pWindowWidgetPreview = x->pUI->windowGet(pDefCons->names.uiThemeEditor + "_widget_preview");
		CUIWindow* pWindowMain = x->pUI->windowGet(pDefCons->names.uiThemeEditor);
		// Edit next two lines for this CUIImage object
		std::string strText = "textEditTextActive";
		pDefCons->themeEditor.pCurrentlySelectedThemeColour = &pWindowWidgetPreview->themeGetSettings()->colours.textEditTextActive;
		pWindowMain->textGet("Current colour")->setText(strText);
		pDefCons->themeEditor.pCurrentlySelectedThemeColourImageWidget = pWindowMain->imageGet(strText);
		pDefCons->themeEditor.pCurrentlySelectedThemeColourImageWidget->setColour(*pDefCons->themeEditor.pCurrentlySelectedThemeColour);
		pWindowMain->colourSelectorGet("colourSelector")->setColour(*pDefCons->themeEditor.pCurrentlySelectedThemeColour);
	}

	void uiClicked_textEditTextInactive(void)
	{
		CUIDefaultContainers* pDefCons = x->pUI->getDefaultContainers();
		CUIWindow* pWindowWidgetPreview = x->pUI->windowGet(pDefCons->names.uiThemeEditor + "_widget_preview");
		CUIWindow* pWindowMain = x->pUI->windowGet(pDefCons->names.uiThemeEditor);
		// Edit next two lines for this CUIImage object
		std::string strText = "textEditTextInactive";
		pDefCons->themeEditor.pCurrentlySelectedThemeColour = &pWindowWidgetPreview->themeGetSettings()->colours.textEditTextInactive;
		pWindowMain->textGet("Current colour")->setText(strText);
		pDefCons->themeEditor.pCurrentlySelectedThemeColourImageWidget = pWindowMain->imageGet(strText);
		pDefCons->themeEditor.pCurrentlySelectedThemeColourImageWidget->setColour(*pDefCons->themeEditor.pCurrentlySelectedThemeColour);
		pWindowMain->colourSelectorGet("colourSelector")->setColour(*pDefCons->themeEditor.pCurrentlySelectedThemeColour);
	}

	void uiClicked_tooltipBG(void)
	{
		CUIDefaultContainers* pDefCons = x->pUI->getDefaultContainers();
		CUIWindow* pWindowWidgetPreview = x->pUI->windowGet(pDefCons->names.uiThemeEditor + "_widget_preview");
		CUIWindow* pWindowMain = x->pUI->windowGet(pDefCons->names.uiThemeEditor);
		// Edit next two lines for this CUIImage object
		std::string strText = "tooltipBG";
		pDefCons->themeEditor.pCurrentlySelectedThemeColour = &pWindowWidgetPreview->themeGetSettings()->colours.tooltipBG;
		pWindowMain->textGet("Current colour")->setText(strText);
		pDefCons->themeEditor.pCurrentlySelectedThemeColourImageWidget = pWindowMain->imageGet(strText);
		pDefCons->themeEditor.pCurrentlySelectedThemeColourImageWidget->setColour(*pDefCons->themeEditor.pCurrentlySelectedThemeColour);
		pWindowMain->colourSelectorGet("colourSelector")->setColour(*pDefCons->themeEditor.pCurrentlySelectedThemeColour);
	}

	void uiClicked_tooltipText(void)
	{
		CUIDefaultContainers* pDefCons = x->pUI->getDefaultContainers();
		CUIWindow* pWindowWidgetPreview = x->pUI->windowGet(pDefCons->names.uiThemeEditor + "_widget_preview");
		CUIWindow* pWindowMain = x->pUI->windowGet(pDefCons->names.uiThemeEditor);
		// Edit next two lines for this CUIImage object
		std::string strText = "tooltipText";
		pDefCons->themeEditor.pCurrentlySelectedThemeColour = &pWindowWidgetPreview->themeGetSettings()->colours.tooltipText;
		pWindowMain->textGet("Current colour")->setText(strText);
		pDefCons->themeEditor.pCurrentlySelectedThemeColourImageWidget = pWindowMain->imageGet(strText);
		pDefCons->themeEditor.pCurrentlySelectedThemeColourImageWidget->setColour(*pDefCons->themeEditor.pCurrentlySelectedThemeColour);
		pWindowMain->colourSelectorGet("colourSelector")->setColour(*pDefCons->themeEditor.pCurrentlySelectedThemeColour);
	}

	void uiClicked_windowBGFocused(void)
	{
		CUIDefaultContainers* pDefCons = x->pUI->getDefaultContainers();
		CUIWindow* pWindowWidgetPreview = x->pUI->windowGet(pDefCons->names.uiThemeEditor + "_widget_preview");
		CUIWindow* pWindowMain = x->pUI->windowGet(pDefCons->names.uiThemeEditor);
		// Edit next two lines for this CUIImage object
		std::string strText = "windowBGFocused";
		pDefCons->themeEditor.pCurrentlySelectedThemeColour = &pWindowWidgetPreview->themeGetSettings()->colours.windowBGFocused;
		pWindowMain->textGet("Current colour")->setText(strText);
		pDefCons->themeEditor.pCurrentlySelectedThemeColourImageWidget = pWindowMain->imageGet(strText);
		pDefCons->themeEditor.pCurrentlySelectedThemeColourImageWidget->setColour(*pDefCons->themeEditor.pCurrentlySelectedThemeColour);
		pWindowMain->colourSelectorGet("colourSelector")->setColour(*pDefCons->themeEditor.pCurrentlySelectedThemeColour);
	}

	void uiClicked_windowBGNotFocused(void)
	{
		CUIDefaultContainers* pDefCons = x->pUI->getDefaultContainers();
		CUIWindow* pWindowWidgetPreview = x->pUI->windowGet(pDefCons->names.uiThemeEditor + "_widget_preview");
		CUIWindow* pWindowMain = x->pUI->windowGet(pDefCons->names.uiThemeEditor);
		// Edit next two lines for this CUIImage object
		std::string strText = "windowBGNotFocused";
		pDefCons->themeEditor.pCurrentlySelectedThemeColour = &pWindowWidgetPreview->themeGetSettings()->colours.windowBGNotFocused;
		pWindowMain->textGet("Current colour")->setText(strText);
		pDefCons->themeEditor.pCurrentlySelectedThemeColourImageWidget = pWindowMain->imageGet(strText);
		pDefCons->themeEditor.pCurrentlySelectedThemeColourImageWidget->setColour(*pDefCons->themeEditor.pCurrentlySelectedThemeColour);
		pWindowMain->colourSelectorGet("colourSelector")->setColour(*pDefCons->themeEditor.pCurrentlySelectedThemeColour);
	}

	void uiClicked_windowTitlebarTextFocused(void)
	{
		CUIDefaultContainers* pDefCons = x->pUI->getDefaultContainers();
		CUIWindow* pWindowWidgetPreview = x->pUI->windowGet(pDefCons->names.uiThemeEditor + "_widget_preview");
		CUIWindow* pWindowMain = x->pUI->windowGet(pDefCons->names.uiThemeEditor);
		// Edit next two lines for this CUIImage object
		std::string strText = "windowTitlebarTextFocused";
		pDefCons->themeEditor.pCurrentlySelectedThemeColour = &pWindowWidgetPreview->themeGetSettings()->colours.windowTitlebarTextFocused;
		pWindowMain->textGet("Current colour")->setText(strText);
		pDefCons->themeEditor.pCurrentlySelectedThemeColourImageWidget = pWindowMain->imageGet(strText);
		pDefCons->themeEditor.pCurrentlySelectedThemeColourImageWidget->setColour(*pDefCons->themeEditor.pCurrentlySelectedThemeColour);
		pWindowMain->colourSelectorGet("colourSelector")->setColour(*pDefCons->themeEditor.pCurrentlySelectedThemeColour);
	}

	void uiClicked_windowTitlebarTextNotFocused(void)
	{
		CUIDefaultContainers* pDefCons = x->pUI->getDefaultContainers();
		CUIWindow* pWindowWidgetPreview = x->pUI->windowGet(pDefCons->names.uiThemeEditor + "_widget_preview");
		CUIWindow* pWindowMain = x->pUI->windowGet(pDefCons->names.uiThemeEditor);
		// Edit next two lines for this CUIImage object
		std::string strText = "windowTitlebarTextNotFocused";
		pDefCons->themeEditor.pCurrentlySelectedThemeColour = &pWindowWidgetPreview->themeGetSettings()->colours.windowTitlebarTextNotFocused;
		pWindowMain->textGet("Current colour")->setText(strText);
		pDefCons->themeEditor.pCurrentlySelectedThemeColourImageWidget = pWindowMain->imageGet(strText);
		pDefCons->themeEditor.pCurrentlySelectedThemeColourImageWidget->setColour(*pDefCons->themeEditor.pCurrentlySelectedThemeColour);
		pWindowMain->colourSelectorGet("colourSelector")->setColour(*pDefCons->themeEditor.pCurrentlySelectedThemeColour);
	}

	void CUIDefConUIThemeEditor::initialise(void)
	{
		_initMainWindow();
		_initHelpWindow();
		_initWidgetPreviewWindow();

		// Set currently selected colour
		uiClicked_ambientLight();
	}

	void CUIDefConUIThemeEditor::_initMainWindow(void)
	{
		CUIDefaultContainers* pDefCons = x->pUI->getDefaultContainers();

		CUIButton* pButton;
		CUIText* pText;
		CUIColourSelector* pCS;

		CUIWindow* pWindow = x->pUI->windowAdd(pDefCons->names.uiThemeEditor, true);
		CUITheme::SSettings* pThemeSettings = pWindow->themeGetSettings();
		pWindow->setVisible(false);
		pWindow->setDimensions(800, 700);
		pWindow->setPositionCentre();

		pButton = pWindow->buttonAdd("Help", 0, 0, 200, 40);
		pButton->setFunctionOnClicked(uiClickedMainWindowHelpButton);
		pButton->pTooltip->setText("Open the help window.");
		
		pButton = pWindow->buttonAdd("Widget Preview", 200, 0, 200, 40);
		pButton->setFunctionOnClicked(uiClickedMainWindowWidgetPreviewButton);
		pButton->pTooltip->setText("Open the user interface editor's widget preview window.");

		int iPosX = 0;
		int iPosY = 50;
		_colourImageAdd("ambientLight", iPosY, pThemeSettings->colours.ambientLight, uiClicked_ambientLight);
		_colourImageAdd("buttonBGDown", iPosY, pThemeSettings->colours.buttonBGDown, uiClicked_buttonBGDown);
		_colourImageAdd("buttonBGOver", iPosY, pThemeSettings->colours.buttonBGOver, uiClicked_buttonBGOver);
		_colourImageAdd("buttonBGUp", iPosY, pThemeSettings->colours.buttonBGUp, uiClicked_buttonBGUp);
		_colourImageAdd("buttonImageBGDown", iPosY, pThemeSettings->colours.buttonImageBGDown, uiClicked_buttonImageBGDown);
		_colourImageAdd("buttonImageBGOver", iPosY, pThemeSettings->colours.buttonImageBGOver, uiClicked_buttonImageBGOver);
		_colourImageAdd("buttonImageBGUp", iPosY, pThemeSettings->colours.buttonImageBGUp, uiClicked_buttonImageBGUp);
		_colourImageAdd("buttonImageTextDown", iPosY, pThemeSettings->colours.buttonImageTextDown, uiClicked_buttonImageTextDown);
		_colourImageAdd("buttonImageTextOver", iPosY, pThemeSettings->colours.buttonImageTextOver, uiClicked_buttonImageTextOver);
		_colourImageAdd("buttonImageTextUp", iPosY, pThemeSettings->colours.buttonImageTextUp, uiClicked_buttonImageTextUp);
		_colourImageAdd("buttonTextDown", iPosY, pThemeSettings->colours.buttonTextDown, uiClicked_buttonTextDown);
		_colourImageAdd("buttonTextOver", iPosY, pThemeSettings->colours.buttonTextOver, uiClicked_buttonTextOver);
		_colourImageAdd("buttonTextUp", iPosY, pThemeSettings->colours.buttonTextUp, uiClicked_buttonTextUp);
		_colourImageAdd("checkboxBGOff", iPosY, pThemeSettings->colours.checkboxBGOff, uiClicked_checkboxBGOff);
		_colourImageAdd("checkboxBGOffOver", iPosY, pThemeSettings->colours.checkboxBGOffOver, uiClicked_checkboxBGOffOver);
		_colourImageAdd("checkboxBGOn", iPosY, pThemeSettings->colours.checkboxBGOn, uiClicked_checkboxBGOn);
		_colourImageAdd("checkboxBGOnOver", iPosY, pThemeSettings->colours.checkboxBGOnOver, uiClicked_checkboxBGOnOver);
		_colourImageAdd("linegraphBG", iPosY, pThemeSettings->colours.linegraphBG, uiClicked_linegraphBG);
		_colourImageAdd("mouseLight", iPosY, pThemeSettings->colours.mouseLight, uiClicked_mouseLight);
		_colourImageAdd("progressBarBG", iPosY, pThemeSettings->colours.progressBarBG, uiClicked_progressBarBG);
		_colourImageAdd("progressBarFiller", iPosY, pThemeSettings->colours.progressBarFiller, uiClicked_progressBarFiller);
		_colourImageAdd("scrollbarBG", iPosY, pThemeSettings->colours.scrollbarBG, uiClicked_scrollbarBG);
		_colourImageAdd("scrollbarTabNotOver", iPosY, pThemeSettings->colours.scrollbarTabNotOver, uiClicked_scrollbarTabNotOver);
		_colourImageAdd("scrollbarTabOver", iPosY, pThemeSettings->colours.scrollbarTabOver, uiClicked_scrollbarTabOver);
		_colourImageAdd("text", iPosY, pThemeSettings->colours.text, uiClicked_text);
		_colourImageAdd("textEditBGActive", iPosY, pThemeSettings->colours.textEditBGActive, uiClicked_textEditBGActive);
		_colourImageAdd("textEditBGInactive", iPosY, pThemeSettings->colours.textEditBGInactive, uiClicked_textEditBGInactive);
		_colourImageAdd("textEditTextActive", iPosY, pThemeSettings->colours.textEditTextActive, uiClicked_textEditTextActive);
		_colourImageAdd("textEditTextInactive", iPosY, pThemeSettings->colours.textEditTextInactive, uiClicked_textEditTextInactive);
		_colourImageAdd("tooltipBG", iPosY, pThemeSettings->colours.tooltipBG, uiClicked_tooltipBG);
		_colourImageAdd("tooltipText", iPosY, pThemeSettings->colours.tooltipText, uiClicked_tooltipText);
		_colourImageAdd("windowBGFocused", iPosY, pThemeSettings->colours.windowBGFocused, uiClicked_windowBGFocused);
		_colourImageAdd("windowBGNotFocused", iPosY, pThemeSettings->colours.windowBGNotFocused, uiClicked_windowBGNotFocused);
		_colourImageAdd("windowTitlebarTextFocused", iPosY, pThemeSettings->colours.windowTitlebarTextFocused, uiClicked_windowTitlebarTextFocused);
		_colourImageAdd("windowTitlebarTextNotFocused", iPosY, pThemeSettings->colours.windowTitlebarTextNotFocused, uiClicked_windowTitlebarTextNotFocused);


		// Colour selector
		pText = pWindow->textAdd("Current colour", 400, 45, 300, 40, "");
		pCS = pWindow->colourSelectorAdd("colourSelector", 400, 80);

		//		pText = pWindow->textAdd("MouseLightDistance", iPosX, iPosY, 300, 40, "MouseLight Distance"); iPosY += 30;
		//		CUIScrollbar* pSB = pWindow->scrollbarAdd("MouseLightDistance", iPosX, iPosY, 500, 30);
		//		pSB->setTabPos(pThemeSettings->floats.normalMouseCursorDistance * 0.01f); iPosY += 50;

	}

	void CUIDefConUIThemeEditor::_initHelpWindow(void)
	{
		CUIDefaultContainers* pDefCons = x->pUI->getDefaultContainers();
		CUIWindow* pWindow = x->pUI->windowAdd(pDefCons->names.uiThemeEditor + "_Help", true);
		pWindow->setDimensions(640, 240);
		pWindow->setPositionCentre();
		CVector2f vPos = pWindow->getPosition();
		vPos.y = 0;
		pWindow->setPosition(vPos);
		pWindow->setVisible(false);
		std::string strText;
		strText = "Help for the user interface editor.\n";
		strText += "First up, with great power, comes great responsibility! I heard that somewhere, perhaps in a film and it seems quite apt here.\n";
		strText += "With the user interface editor, we can create new themes for the user interface system.\n";
		strText += "The reason I mentioned the above is because there is no checking of us setting values to stupid values.\n";
		strText += "For example, if we set some text colour to white and the background colour to white aswell, then we won't be able to see the text!\n";
		strText += "So, be carefull :)\n";
		strText += "With that out of the way, here's how to use the thing...\n";
		strText += "Close this help window or move it out of the way and then click on the \"Widget Preview\" button.\n";
		strText += "This will open a window which shows all of the user interface's available widgets.\n";
		strText += "Click on a widget to make it the currently editable widget and once clicked, it's settings will be shown in the main window.\n";
		strText += "You can then edit this widget's various settings.\n";
		pWindow->textAdd("help", 0, 0, 640, 540, strText);
	}

	void CUIDefConUIThemeEditor::_initWidgetPreviewWindow(void)
	{
		CUIDefaultContainers* pDefCons = x->pUI->getDefaultContainers();
		CUIWindow* pWindow = x->pUI->windowAdd(pDefCons->names.uiThemeEditor + "_widget_preview", true);
		pWindow->setVisible(false);
		pWindow->setDimensions(550, 800);
		pWindow->setPositionCentre();
		CVector2f vPos = pWindow->getPosition();
		vPos.x -= 700;
		pWindow->setPosition(vPos);
		pWindow->themeNameSet("editor");
		int iXpos = 0;
		int iYpos = 0;

		pWindow->textAdd("text0", iXpos, iYpos, 200, 40, "CUIButton: ");
		CUIButton* pButton = pWindow->buttonAdd("button0", iXpos + 200, iYpos, 100, 30);
		pButton->pTooltip->setText("Tooltip of a CUIButton");

		iYpos += 50;
		pWindow->textAdd("text1", iXpos, iYpos, 200, 40, "CUIButtonImage: ");
		CUIButtonImage* pBI = pWindow->buttonImageAdd("buttonImage0", iXpos + 200, iYpos, 100, 40,
			x->pResource->defaultRes.texture2DFromFile_default_white,
			x->pResource->defaultRes.texture2DFromFile_default_white,
			x->pResource->defaultRes.texture2DFromFile_default_white);
		pBI->pTooltip->setText("Tooltip of a CUIButtonImage");
		pBI->setText("");

		iYpos += 50;
		pWindow->textAdd("text2", iXpos, iYpos, 200, 40, "CUICheckbox: ");
		CUICheckbox* pCB = pWindow->checkboxAdd("Checkbox", 200, iYpos, 40, 40);
		pCB->pTooltip->setText("Tooltip of a CUICheckbox");

		iYpos += 50;
		pWindow->textAdd("text3", iXpos, iYpos, 200, 40, "CUIColourSelector: ");
		CUIColourSelector* pCS = pWindow->colourSelectorAdd("colourSelector", iXpos + 200, iYpos);

		iYpos += 355;
		pWindow->textAdd("text4", iXpos, iYpos, 200, 40, "CUILineGraph: ");
		CUILineGraph* pLG = pWindow->lineGraphAdd("linegraph", iXpos + 200, iYpos, 200, 100);
		CUILineGraphDataSet* pDataSet = pLG->addDataset("Dataset0", CColour(1.0f, 1.0f, 1.0f, 1.0f));
		pLG->pTooltip->setText("Tooltip of a CUILineGraph");
		for (float f = 0; f < 15; f += 0.3f)
			pDataSet->addValue((1.0f + sinf(f)) * 10.0f);

		iYpos += 100;
		pWindow->textAdd("text5", iXpos, iYpos, 200, 40, "CUIProgressbar: ");
		CUIProgressbar* pPB = pWindow->progressbarAdd("progressbar", iXpos + 200, iYpos, 200, 40);
		pPB->pTooltip->setText("Tooltip of a CUIProgressbar");

		iYpos += 40;
		pWindow->textAdd("text6", iXpos, iYpos, 200, 40, "CUIScrollbar: ");
		CUIScrollbar* pSB = pWindow->scrollbarAdd("scrollbar", iXpos + 200, iYpos, 200, 40);
		pSB->pTooltip->setText("Tooltip of a CUIScrollbar");

		iYpos += 40;
		pWindow->textAdd("text7", iXpos, iYpos, 200, 40, "CUIText: ");
		CUIText* pText = pWindow->textAdd("text8", iXpos + 200, iYpos, 200, 40, "CUIText Example.");
		pText->pTooltip->setText("Tooltip of a CUIText");

		iYpos += 40;
		pWindow->textAdd("text9", iXpos, iYpos, 200, 40, "CUITextEdit: ");
		CUITextEdit* pTE = pWindow->textEditAdd("textedit", iXpos + 200, iYpos, 200, 40, "TextEdit");
		pTE->pTooltip->setText("Tooltip of a CUITextEdit");
	}

	void CUIDefConUIThemeEditor::update(float fTimeDeltaSecs)
	{
		CUIDefaultContainers* pDefCons = x->pUI->getDefaultContainers();

		// Main editor window
		CUIWindow* pWindow = x->pUI->windowGet(pDefCons->names.uiThemeEditor);
		if (!pWindow->getVisible())
			return;


		CUIWindow* pWindowWidgets = x->pUI->windowGet(pDefCons->names.uiThemeEditor + "_widget_preview");
		CUITheme::SSettings* pThemeSettings = pWindowWidgets->themeGetSettings();

		CUIColourSelector* pCS;
		pCS = pWindow->colourSelectorGet("colourSelector");
		if (pCS->getSomethingHasChanged())
		{
			*pCurrentlySelectedThemeColour = pCS->getColour();
			pCurrentlySelectedThemeColourImageWidget->setColour(pCS->getColour());
		}

		// Widget preview window
		pWindow = x->pUI->windowGet(pDefCons->names.uiThemeEditor + "_widget_preview");
		if (pWindow->getVisible())
		{
		}

	}

	void CUIDefConUIThemeEditor::_colourImageAdd(const std::string& strName, int& iPositionY, CColour& colour, void (*function)(void))
	{
		CUIDefaultContainers* pDefCons = x->pUI->getDefaultContainers();
		CUIWindow* pWindow = x->pUI->windowGet(pDefCons->names.uiThemeEditor);
		CUIImage* pImage = pWindow->imageAdd(strName, 0, iPositionY, 60, 30);
		pImage->setTextureFromFile(x->pResource->defaultRes.texture2DFromFile_default_white);
		pImage->setColour(colour);
		pImage->setFunctionOnClicked(function);
		pWindow->textAdd(strName, 70, iPositionY, 350, 40, strName);
		iPositionY += 40;
	}
}