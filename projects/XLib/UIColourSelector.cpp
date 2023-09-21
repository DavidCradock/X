#include "PCH.h"
#include "UIColourSelector.h"
#include "UIContainer.h"
#include "singletons.h"
#include "stringUtils.h"
#include "UITooltip.h"

namespace X
{
	CUIColourSelector::CUIColourSelector(CUIContainer* pContainer, const std::string& strWidgetName, float fPosX, float fPosY)
	{
		_mpContainer = pContainer;
		_mbVisible = true;
		_mColour.set(1, 1, 1, 1);
		_mvPosition.set(fPosX, fPosY);

		_mstrScrollbarNames[EValue::brightness] = pContainer->getName() + "_" + strWidgetName + "scrollbar_brightness";
		_mstrScrollbarNames[EValue::colourA] = pContainer->getName() + "_" + strWidgetName + "scrollbar_colA";
		_mstrScrollbarNames[EValue::colourB] = pContainer->getName() + "_" + strWidgetName + "scrollbar_colB";
		_mstrScrollbarNames[EValue::colourG] = pContainer->getName() + "_" + strWidgetName + "scrollbar_colG";
		_mstrScrollbarNames[EValue::colourR] = pContainer->getName() + "_" + strWidgetName + "scrollbar_colR";
		_mstrScrollbarNames[EValue::hue] = pContainer->getName() + "_" + strWidgetName + "scrollbar_hue";
		_mstrScrollbarNames[EValue::saturation] = pContainer->getName() + "_" + strWidgetName + "scrollbar_sat";

		_mstrTextEditNames[EValue::brightness] = pContainer->getName() + "_" + strWidgetName + "textedit_brightness";
		_mstrTextEditNames[EValue::colourA] = pContainer->getName() + "_" + strWidgetName + "textedit_colA";
		_mstrTextEditNames[EValue::colourB] = pContainer->getName() + "_" + strWidgetName + "textedit_colB";
		_mstrTextEditNames[EValue::colourG] = pContainer->getName() + "_" + strWidgetName + "textedit_colG";
		_mstrTextEditNames[EValue::colourR] = pContainer->getName() + "_" + strWidgetName + "textedit_colR";
		_mstrTextEditNames[EValue::hue] = pContainer->getName() + "_" + strWidgetName + "textedit_hue";
		_mstrTextEditNames[EValue::saturation] = pContainer->getName() + "_" + strWidgetName + "textedit_sat";

		_mstrTextNames[EValue::brightness] = pContainer->getName() + "_" + strWidgetName + "text_brightness";
		_mstrTextNames[EValue::colourA] = pContainer->getName() + "_" + strWidgetName + "text_colA";
		_mstrTextNames[EValue::colourB] = pContainer->getName() + "_" + strWidgetName + "text_colB";
		_mstrTextNames[EValue::colourG] = pContainer->getName() + "_" + strWidgetName + "text_colG";
		_mstrTextNames[EValue::colourR] = pContainer->getName() + "_" + strWidgetName + "text_colR";
		_mstrTextNames[EValue::hue] = pContainer->getName() + "_" + strWidgetName + "text_hue";
		_mstrTextNames[EValue::saturation] = pContainer->getName() + "_" + strWidgetName + "text_sat";

		for (int i = 0; i < 7; i++)
		{
			_mpScrollbar[i] = _mpContainer->scrollbarAdd(_mstrScrollbarNames[i], 0, 0, (int)_mvDimensions.x, 40, 0.01f);
			_mpTextEdit[i] = _mpContainer->textEditAdd(_mstrTextEditNames[i], 0, 0, 40, 40, "0");
			_mpText[i] = _mpContainer->textAdd(_mstrTextNames[i], 0, 0, 150, 40, "X");
			_mpText[i]->setTextColour(false, CColour(1, 1, 1, 0.90f));
			_mpText[i]->setCentered(true);
		}

		_mstrColourImageName = pContainer->getName() + "_" + strWidgetName + "colour_image";
		_mpColourImage = _mpContainer->imageAdd(_mstrColourImageName, 0, 0, 100, 100);
		_mpColourImage->setTextureFromFile(x->pResource->defaultRes.texture2DFromFile_default_white);

		_updateWidgetDims();
		setColour(CColour());
		_mpContainer->computeScrollbars();

		// Tooltips
		_mpScrollbar[EValue::brightness]->pTooltip->setText("Scroll here to adjust the brightness value of the colour.");
		_mpScrollbar[EValue::colourA]->pTooltip->setText("Scroll here to adjust the alpha value of the colour.");
		_mpScrollbar[EValue::colourB]->pTooltip->setText("Scroll here to adjust the blue intensity of the colour.");
		_mpScrollbar[EValue::colourG]->pTooltip->setText("Scroll here to adjust the green intensity of the colour.");
		_mpScrollbar[EValue::colourR]->pTooltip->setText("Scroll here to adjust the red intensity of the colour.");
		_mpScrollbar[EValue::hue]->pTooltip->setText("Scroll here to adjust the hue value of the colour.");
		_mpScrollbar[EValue::saturation]->pTooltip->setText("Edit here to adjust the saturation value of the colour.");
		_mpTextEdit[EValue::brightness]->pTooltip->setText("Edit here to adjust the brightness value of the colour.");
		_mpTextEdit[EValue::colourA]->pTooltip->setText("Edit here to adjust the alpha value of the colour.");
		_mpTextEdit[EValue::colourB]->pTooltip->setText("Edit here to adjust the blue intensity of the colour.");
		_mpTextEdit[EValue::colourG]->pTooltip->setText("Edit here to adjust the green intensity of the colour.");
		_mpTextEdit[EValue::colourR]->pTooltip->setText("Edit here to adjust the red intensity of the colour.");
		_mpTextEdit[EValue::hue]->pTooltip->setText("Edit here to adjust the hue value of the colour.");
		_mpTextEdit[EValue::saturation]->pTooltip->setText("Edit here to adjust the saturation value of the colour.");
		_mpColourImage->pTooltip->setText("This shows the resulting colour calculated from all the widgets above me.");

	}

	CUIColourSelector::~CUIColourSelector()
	{
		for (int i = 0; i < 7; i++)
		{
			_mpContainer->scrollbarRemove(_mstrScrollbarNames[i]);
			_mpContainer->textEditRemove(_mstrTextEditNames[i]);
		}

		_mpContainer->imageRemove(_mstrColourImageName);
	}

	CVector2f CUIColourSelector::getDimensions(void) const
	{
		return _mvDimensions;
	}

	void CUIColourSelector::setPosition(float fX, float fY)
	{
		_mvPosition.x = fX;
		_mvPosition.y = fY;
		_mpContainer->computeScrollbars();
	}

	void CUIColourSelector::setPosition(int iX, int iY)
	{
		setPosition(float(iX), float(iY));
	}

	void CUIColourSelector::setPosition(const CVector2f& vPosition)
	{
		setPosition(vPosition.x, vPosition.y);
	}

	CVector2f CUIColourSelector::getPosition(void) const
	{
		return _mvPosition;
	}

	void CUIColourSelector::setVisible(bool bVisible)
	{
		_mbVisible = bVisible;
		_mpContainer->computeScrollbars();
	}

	bool CUIColourSelector::getVisible(void) const
	{
		return _mbVisible;
	}

	void CUIColourSelector::renderNonBG(void)
	{
		if (!_mbVisible)
			return;
	}

	void CUIColourSelector::renderTooltip(void)
	{
		for (int i = 0; i < 7; i++)
		{
			_mpScrollbar[i]->pTooltip->render();
			_mpTextEdit[i]->pTooltip->render();
			_mpText[i]->pTooltip->render();
		}
		_mpColourImage->pTooltip->render();
	}

	void CUIColourSelector::update(float fTimeDeltaSec)
	{
		CUIScrollbar* pSB;
		CUITextEdit* pTE;
		std::string strTxt;
		CColour colour;

		// Scrollbar brightness being moved
		pSB = _mpContainer->scrollbarGet(_mstrScrollbarNames[EValue::brightness]);
		if (pSB->getTabBeingMoved())
		{
			// Set textedit text
			strTxt.clear();
			StringUtils::appendFloat(strTxt, pSB->getTabPos(), 3);
			_mpContainer->textEditGet(_mstrTextEditNames[EValue::brightness])->setText(strTxt);
			// Set RGB scrollbars and textedit
			_helperSetRGBFromHSB();
		}

		// Scrollbar hue being moved
		pSB = _mpContainer->scrollbarGet(_mstrScrollbarNames[EValue::hue]);
		if (pSB->getTabBeingMoved())
		{
			// Set textedit text
			strTxt.clear();
			StringUtils::appendFloat(strTxt, pSB->getTabPos(), 3);
			_mpContainer->textEditGet(_mstrTextEditNames[EValue::hue])->setText(strTxt);
			// Set RGB scrollbars and textedit
			_helperSetRGBFromHSB();
		}

		// Scrollbar saturation being moved
		pSB = _mpContainer->scrollbarGet(_mstrScrollbarNames[EValue::saturation]);
		if (pSB->getTabBeingMoved())
		{
			// Set textedit text
			strTxt.clear();
			StringUtils::appendFloat(strTxt, pSB->getTabPos(), 3);
			_mpContainer->textEditGet(_mstrTextEditNames[EValue::saturation])->setText(strTxt);
			// Set RGB scrollbars and textedit
			_helperSetRGBFromHSB();
		}

		// Scrollbar RED being moved
		pSB = _mpContainer->scrollbarGet(_mstrScrollbarNames[EValue::colourR]);
		if (pSB->getTabBeingMoved())
		{
			// Set textedit text
			strTxt.clear();
			StringUtils::appendFloat(strTxt, pSB->getTabPos(), 3);
			_mpContainer->textEditGet(_mstrTextEditNames[EValue::colourR])->setText(strTxt);
			// Set RGB scrollbars and textedit
			_helperSetHSBFromRGB();
		}

		// Scrollbar GREEN being moved
		pSB = _mpContainer->scrollbarGet(_mstrScrollbarNames[EValue::colourG]);
		if (pSB->getTabBeingMoved())
		{
			// Set textedit text
			strTxt.clear();
			StringUtils::appendFloat(strTxt, pSB->getTabPos(), 3);
			_mpContainer->textEditGet(_mstrTextEditNames[EValue::colourG])->setText(strTxt);
			// Set RGB scrollbars and textedit
			_helperSetHSBFromRGB();
		}

		// Scrollbar BLUE being moved
		pSB = _mpContainer->scrollbarGet(_mstrScrollbarNames[EValue::colourB]);
		if (pSB->getTabBeingMoved())
		{
			// Set textedit text
			strTxt.clear();
			StringUtils::appendFloat(strTxt, pSB->getTabPos(), 3);
			_mpContainer->textEditGet(_mstrTextEditNames[EValue::colourB])->setText(strTxt);
			// Set RGB scrollbars and textedit
			_helperSetHSBFromRGB();
		}

		// Scrollbar ALPHA being moved
		pSB = _mpContainer->scrollbarGet(_mstrScrollbarNames[EValue::colourA]);
		if (pSB->getTabBeingMoved())
		{
			// Set textedit text
			strTxt.clear();
			StringUtils::appendFloat(strTxt, pSB->getTabPos(), 3);
			_mpContainer->textEditGet(_mstrTextEditNames[EValue::colourA])->setText(strTxt);
			_mColour.alpha = pSB->getTabPos();
			_mpColourImage->setColour(_mColour);
		}

		// Text edit brightness has changed
		pTE = _mpContainer->textEditGet(_mstrTextEditNames[EValue::brightness]);
		if (pTE->getHasTextChanged())
		{
			float fValue = StringUtils::stringToFloat(pTE->getText());
			_mpContainer->scrollbarGet(_mstrScrollbarNames[EValue::brightness])->setTabPos(fValue);
			// Set RGB scrollbars and textedit
			_helperSetRGBFromHSB();
		}

		// Text edit hue has changed
		pTE = _mpContainer->textEditGet(_mstrTextEditNames[EValue::hue]);
		if (pTE->getHasTextChanged())
		{
			float fValue = StringUtils::stringToFloat(pTE->getText());
			_mpContainer->scrollbarGet(_mstrScrollbarNames[EValue::hue])->setTabPos(fValue);
			// Set RGB scrollbars and textedit
			_helperSetRGBFromHSB();
		}

		// Text edit saturation has changed
		pTE = _mpContainer->textEditGet(_mstrTextEditNames[EValue::saturation]);
		if (pTE->getHasTextChanged())
		{
			float fValue = StringUtils::stringToFloat(pTE->getText());
			_mpContainer->scrollbarGet(_mstrScrollbarNames[EValue::saturation])->setTabPos(fValue);
			// Set RGB scrollbars and textedit
			_helperSetRGBFromHSB();
		}

		// Text edit RED has changed
		pTE = _mpContainer->textEditGet(_mstrTextEditNames[EValue::colourR]);
		if (pTE->getHasTextChanged())
		{
			float fValue = StringUtils::stringToFloat(pTE->getText());
			_mpContainer->scrollbarGet(_mstrScrollbarNames[EValue::colourR])->setTabPos(fValue);
			// Set HSB scrollbars and textedit
			_helperSetHSBFromRGB();
		}

		// Text edit GREEN has changed
		pTE = _mpContainer->textEditGet(_mstrTextEditNames[EValue::colourG]);
		if (pTE->getHasTextChanged())
		{
			float fValue = StringUtils::stringToFloat(pTE->getText());
			_mpContainer->scrollbarGet(_mstrScrollbarNames[EValue::colourG])->setTabPos(fValue);
			// Set HSB scrollbars and textedit
			_helperSetHSBFromRGB();
		}

		// Text edit BLUE has changed
		pTE = _mpContainer->textEditGet(_mstrTextEditNames[EValue::colourB]);
		if (pTE->getHasTextChanged())
		{
			float fValue = StringUtils::stringToFloat(pTE->getText());
			_mpContainer->scrollbarGet(_mstrScrollbarNames[EValue::colourB])->setTabPos(fValue);
			// Set HSB scrollbars and textedit
			_helperSetHSBFromRGB();
		}

		// Text edit ALPHA has changed
		pTE = _mpContainer->textEditGet(_mstrTextEditNames[EValue::colourA]);
		if (pTE->getHasTextChanged())
		{
			float fValue = StringUtils::stringToFloat(pTE->getText());
			_mpContainer->scrollbarGet(_mstrScrollbarNames[EValue::colourA])->setTabPos(fValue);
			_mColour.alpha = fValue;
			_mpColourImage->setColour(_mColour);
		}

		// Update this widget's, widget's tooltips
		CVector2f vPosOffset = _mpContainer->getWidgetAreaTLCornerPosition() + _mpContainer->getWidgetOffset();
		for (int i = 0; i < 7; i++)
		{
			_mpScrollbar[i]->pTooltip->update(_mpScrollbar[i]->getPosition() + vPosOffset, _mpScrollbar[i]->getDimensions(), fTimeDeltaSec);
			_mpTextEdit[i]->pTooltip->update(_mpTextEdit[i]->getPosition() + vPosOffset, _mpTextEdit[i]->getDimensions(), fTimeDeltaSec);
			_mpText[i]->pTooltip->update(_mpText[i]->getPosition() + vPosOffset, _mpText[i]->getDimensions(), fTimeDeltaSec);
		}
		_mpColourImage->pTooltip->update(_mpColourImage->getPosition() + vPosOffset, _mpColourImage->getDimensions(), fTimeDeltaSec);
	}

	void CUIColourSelector::reset(void)
	{
		for (int i = 0; i < 7; i++)
		{
			_mpScrollbar[i]->pTooltip->reset();
			_mpTextEdit[i]->pTooltip->reset();
			_mpText[i]->pTooltip->reset();
		}
		_mpColourImage->pTooltip->reset();
	}

	void CUIColourSelector::_helperSetRGBFromHSB(void)
	{
		// Get values from HSB sliders
		float fBrightness = _mpContainer->scrollbarGet(_mstrScrollbarNames[EValue::brightness])->getTabPos();
		float fSaturation = _mpContainer->scrollbarGet(_mstrScrollbarNames[EValue::saturation])->getTabPos();
		float fHue = _mpContainer->scrollbarGet(_mstrScrollbarNames[EValue::hue])->getTabPos();

		CColour colour;
		colour.setFromHSB(fHue, fSaturation, fBrightness);
		colour.alpha = _mColour.alpha;

		// Set values of RGB sliders
		_mpContainer->scrollbarGet(_mstrScrollbarNames[EValue::colourB])->setTabPos(colour.blue);
		_mpContainer->scrollbarGet(_mstrScrollbarNames[EValue::colourG])->setTabPos(colour.green);
		_mpContainer->scrollbarGet(_mstrScrollbarNames[EValue::colourR])->setTabPos(colour.red);

		// Set text of RGB text edits
		std::string strTxt;
		StringUtils::appendFloat(strTxt, colour.blue, 3);
		_mpContainer->textEditGet(_mstrTextEditNames[EValue::colourB])->setText(strTxt);
		strTxt.clear();
		StringUtils::appendFloat(strTxt, colour.green, 3);
		_mpContainer->textEditGet(_mstrTextEditNames[EValue::colourG])->setText(strTxt);
		strTxt.clear();
		StringUtils::appendFloat(strTxt, colour.red, 3);
		_mpContainer->textEditGet(_mstrTextEditNames[EValue::colourR])->setText(strTxt);

		_mpColourImage->setColour(colour);
	}

	void CUIColourSelector::_helperSetHSBFromRGB(void)
	{
		// Get values from RGB sliders
		CColour colour;
		colour.red = _mpContainer->scrollbarGet(_mstrScrollbarNames[EValue::colourR])->getTabPos();
		colour.green = _mpContainer->scrollbarGet(_mstrScrollbarNames[EValue::colourG])->getTabPos();
		colour.blue = _mpContainer->scrollbarGet(_mstrScrollbarNames[EValue::colourB])->getTabPos();
		colour.alpha = _mColour.alpha;

		float fHue, fSaturation, fBrightness;
		colour.getHSB(fHue, fSaturation, fBrightness);

		// Set values of HSB sliders
		_mpContainer->scrollbarGet(_mstrScrollbarNames[EValue::brightness])->setTabPos(fBrightness);
		_mpContainer->scrollbarGet(_mstrScrollbarNames[EValue::hue])->setTabPos(fHue);
		_mpContainer->scrollbarGet(_mstrScrollbarNames[EValue::saturation])->setTabPos(fSaturation);

		// Set text of HSB text edits
		std::string strTxt;
		StringUtils::appendFloat(strTxt, fBrightness, 3);
		_mpContainer->textEditGet(_mstrTextEditNames[EValue::brightness])->setText(strTxt);
		strTxt.clear();
		StringUtils::appendFloat(strTxt, fHue, 3);
		_mpContainer->textEditGet(_mstrTextEditNames[EValue::hue])->setText(strTxt);
		strTxt.clear();
		StringUtils::appendFloat(strTxt, fSaturation, 3);
		_mpContainer->textEditGet(_mstrTextEditNames[EValue::saturation])->setText(strTxt);

		_mpColourImage->setColour(colour);
	}

	void CUIColourSelector::_updateWidgetDims(void)
	{
		const CUITheme::SSettings* pThemeSettings = _mpContainer->themeGetSettings();
		CVector2f vPos = _mvPosition;

		CUITextEdit* pTE;
		CUIScrollbar* pSB;
		_mvDimensions.x = pThemeSettings->floats.colourSelectorTotalWidth;
		CVector2f vTextEditDims(pThemeSettings->vectors.colourSelectorTextEditDims.x, pThemeSettings->vectors.colourSelectorTextEditDims.y);
		CVector2f vScrollbarDims(pThemeSettings->floats.colourSelectorTotalWidth - pThemeSettings->vectors.colourSelectorTextEditDims.x, pThemeSettings->vectors.colourSelectorTextEditDims.y);
		for (int i = 0; i < 7; i++)
		{
			pTE = _mpContainer->textEditGet(_mstrTextEditNames[i]);
			pTE->setDimensions(vTextEditDims);
			vPos.x = _mvDimensions.x - vTextEditDims.x + _mvPosition.x;
			pTE->setPosition(vPos);
			
			pSB = _mpContainer->scrollbarGet(_mstrScrollbarNames[i]);
			vPos.x = _mvPosition.x;
			pSB->setPosition(vPos);
			pSB->setDimensions(vScrollbarDims);
			pSB->setTabRatio(0.2f);

			vPos.y += pThemeSettings->floats.colourSelectorYspacing;
		}

		_mpColourImage->setDimensions(_mvDimensions.x, 32.0f);
		_mpColourImage->setPosition(_mvPosition.x, vPos.y);

		CUIText* pText;
		vPos.x = _mvPosition.x + _mvDimensions.x - pTE->getDimensions().x - (pSB->getDimensions().x * 0.5f);
		vPos.y = _mvPosition.y + pTE->getDimensions().y / 2;
		pText = _mpContainer->textGet(_mstrTextNames[0]);
		pText->setPosition(vPos);
		pText->setText("Red");
		
		vPos.y += pThemeSettings->floats.colourSelectorYspacing;
		pText = _mpContainer->textGet(_mstrTextNames[1]);
		pText->setPosition(vPos);
		pText->setText("Green");

		vPos.y += pThemeSettings->floats.colourSelectorYspacing;
		pText = _mpContainer->textGet(_mstrTextNames[2]);
		pText->setPosition(vPos);
		pText->setText("Blue");

		vPos.y += pThemeSettings->floats.colourSelectorYspacing;
		pText = _mpContainer->textGet(_mstrTextNames[3]);
		pText->setPosition(vPos);
		pText->setText("Alpha");

		vPos.y += pThemeSettings->floats.colourSelectorYspacing;
		pText = _mpContainer->textGet(_mstrTextNames[4]);
		pText->setPosition(vPos);
		pText->setText("Hue");

		vPos.y += pThemeSettings->floats.colourSelectorYspacing;
		pText = _mpContainer->textGet(_mstrTextNames[5]);
		pText->setPosition(vPos);
		pText->setText("Saturation");

		vPos.y += pThemeSettings->floats.colourSelectorYspacing;
		pText = _mpContainer->textGet(_mstrTextNames[6]);
		pText->setPosition(vPos);
		pText->setText("Brightness");
	}

	void CUIColourSelector::setColour(const CColour& colour)
	{
		_mColour = colour;

		// Set RGBA sliders and textedits from given colour
		std::string strTxt;

		// Scrollbar RED to colour
		_mpContainer->scrollbarGet(_mstrScrollbarNames[EValue::colourR])->setTabPos(colour.red);
		// Set textedit text
		strTxt.clear();
		StringUtils::appendFloat(strTxt, colour.red, 3);
		_mpContainer->textEditGet(_mstrTextEditNames[EValue::colourR])->setText(strTxt);

		// Scrollbar GREEN to colour
		_mpContainer->scrollbarGet(_mstrScrollbarNames[EValue::colourG])->setTabPos(colour.green);
		// Set textedit text
		strTxt.clear();
		StringUtils::appendFloat(strTxt, colour.green, 3);
		_mpContainer->textEditGet(_mstrTextEditNames[EValue::colourG])->setText(strTxt);

		// Scrollbar BLUE to colour
		_mpContainer->scrollbarGet(_mstrScrollbarNames[EValue::colourB])->setTabPos(colour.blue);
		// Set textedit text
		strTxt.clear();
		StringUtils::appendFloat(strTxt, colour.blue, 3);
		_mpContainer->textEditGet(_mstrTextEditNames[EValue::colourB])->setText(strTxt);

		// Scrollbar ALPHA to colour
		_mpContainer->scrollbarGet(_mstrScrollbarNames[EValue::colourA])->setTabPos(colour.alpha);
		// Set textedit text
		strTxt.clear();
		StringUtils::appendFloat(strTxt, colour.alpha, 3);
		_mpContainer->textEditGet(_mstrTextEditNames[EValue::colourA])->setText(strTxt);

		// Set HSB sliders and text edits from values in RGB sliders.
		_helperSetHSBFromRGB();

		_mpColourImage->setColour(colour);
	}

	CColour CUIColourSelector::getColour(void) const
	{
		return _mColour;
	}
}