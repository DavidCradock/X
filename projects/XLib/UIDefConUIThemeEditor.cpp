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

	void uiClickedAmbientLight(void)
	{
		CUIDefaultContainers* pDefCons = x->pUI->getDefaultContainers();
		CUIWindow* pWindow = x->pUI->windowGet(pDefCons->names.uiThemeEditor);
	}

	void uiClickedMouseLight(void)
	{

	}

	void CUIDefConUIThemeEditor::initialise(void)
	{
		CUIDefaultContainers* pDefCons = x->pUI->getDefaultContainers();

		CUIButton* pButton;
		CUIText* pText;
		CUIScrollbar* pSB;
		CUIColourSelector* pCS;
		CUIImage* pImage;

		// Main window
		CUIWindow* pWindow = x->pUI->windowAdd(pDefCons->names.uiThemeEditor, true);
		CUITheme::SSettings* pThemeSettings = pWindow->themeGetSettings();
		pWindow->setVisible(true);
		pWindow->setDimensions(800, 700);
		pWindow->setPositionCentre();

		pButton = pWindow->buttonAdd("Help", 0, 0, 200, 40);
		pButton->pTooltip->setText("Open the help window.");

		pButton = pWindow->buttonAdd("Widget Preview", 200, 0, 200, 40);
		pButton->pTooltip->setText("Open the user interface editor's widget preview window.");


		// CUIImages and text
		int iPosX = 0;
		int iPosY = 50;
		pImage = pWindow->imageAdd("AmbientLight", iPosX, iPosY, 60, 30);
		pImage->setTextureFromFile(x->pResource->defaultRes.texture2DFromFile_default_white);
		pImage->setColour(pThemeSettings->colours.ambientLight);
		pImage->setFunctionOnClicked(uiClickedAmbientLight);
		pText = pWindow->textAdd("AmbientLight", iPosX + 70, iPosY, 200, 40, "AmbientLight colour"); iPosY += 40;

		pImage = pWindow->imageAdd("MouseLight", iPosX, iPosY, 60, 30);
		pImage->setTextureFromFile(x->pResource->defaultRes.texture2DFromFile_default_white);
		pImage->setColour(pThemeSettings->colours.mouseLight);
		pImage->setFunctionOnClicked(uiClickedMouseLight);
		pText = pWindow->textAdd("MouseLight", iPosX + 70, iPosY, 200, 40, "MouseLight colour"); iPosY += 40;

		// Colour selector
		pText = pWindow->textAdd("Current colour", 400, 40, 300, 40, "Current Selected Colour: MouseLight");
		pCS = pWindow->colourSelectorAdd("colourSelector", 400, 80);

		//		pText = pWindow->textAdd("MouseLightDistance", iPosX, iPosY, 300, 40, "MouseLight Distance"); iPosY += 30;
		//		CUIScrollbar* pSB = pWindow->scrollbarAdd("MouseLightDistance", iPosX, iPosY, 500, 30);
		//		pSB->setTabPos(pThemeSettings->floats.normalMouseCursorDistance * 0.01f); iPosY += 50;




				// Help window
		pWindow = x->pUI->windowAdd(pDefCons->names.uiThemeEditor + "_Help", true);
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

		// Window for widget previews
		pWindow = x->pUI->windowAdd(pDefCons->names.uiThemeEditor + "_widget_preview", true);
		pWindow->setVisible(false);
		pWindow->setDimensions(550, 800);
		pWindow->setPositionCentre();
		vPos = pWindow->getPosition();
		vPos.x -= 700;
		pWindow->setPosition(vPos);
		pWindow->themeNameSet("editor");
		int iXpos = 0;
		int iYpos = 0;

		pWindow->textAdd("text0", iXpos, iYpos, 200, 40, "CUIButton: ");
		pButton = pWindow->buttonAdd("button0", iXpos + 200, iYpos, 100, 30);
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
		pCS = pWindow->colourSelectorAdd("colourSelector", iXpos + 200, iYpos);

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
		pSB = pWindow->scrollbarAdd("scrollbar", iXpos + 200, iYpos, 200, 40);
		pSB->pTooltip->setText("Tooltip of a CUIScrollbar");

		iYpos += 40;
		pWindow->textAdd("text7", iXpos, iYpos, 200, 40, "CUIText: ");
		pText = pWindow->textAdd("text8", iXpos + 200, iYpos, 200, 40, "CUIText Example.");
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

		if (pWindow->buttonGet("Help")->getClicked())
		{
			x->pUI->windowGet(pDefCons->names.uiThemeEditor + "_Help")->setVisible(true, true);
		}
		if (pWindow->buttonGet("Widget Preview")->getClicked())
		{
			x->pUI->windowGet(pDefCons->names.uiThemeEditor + "_widget_preview")->setVisible(true, true);
		}

		CUIWindow* pWindowWidgets = x->pUI->windowGet(pDefCons->names.uiThemeEditor + "_widget_preview");
		CUITheme::SSettings* pThemeSettings = pWindowWidgets->themeGetSettings();

		CUIImage* pImage = pWindow->imageGet("AmbientLight");
		CUIColourSelector* pCS;
		//		pCS = pWindow->colourSelectorGet("AmbientLight");
		//		if (pCS->getSomethingHasChanged())
		//		{
		//			pThemeSettings->colours.ambientLight = pCS->getColour();
		//		}
		//		pCS = pWindow->colourSelectorGet("MouseLight");
		//		if (pCS->getSomethingHasChanged())
		//		{
		//			pThemeSettings->colours.mouseLight = pCS->getColour();
		//		}
		//		CUIScrollbar* pSB = pWindow->scrollbarGet("MouseLightDistance");
		//		if (pSB->getTabBeingMoved())
		//		{
		//			pThemeSettings->floats.normalMouseCursorDistance = pSB->getTabPos() * 1000.0f;
		//		}



		// Widget preview window
		pWindow = x->pUI->windowGet(pDefCons->names.uiThemeEditor + "_widget_preview");
		if (pWindow->getVisible())
		{
		}

	}
}