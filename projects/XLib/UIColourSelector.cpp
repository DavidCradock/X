#include "PCH.h"
#include "UIColourSelector.h"
#include "UIContainer.h"
#include "singletons.h"
#include "stringUtils.h"

namespace X
{
	CUIColourSelector::CUIColourSelector(CUIContainer* pContainer, const std::string& strWidgetName, float fPosX, float fPosY, int iColourWheelRadius)
	{
		_mpContainer = pContainer;
		
		_mbVisible = true;
		_miCircleRadius = 10;

		_mvDimensions.x = (float)iColourWheelRadius * 2.0f;
		_mvDimensions.y = (float)iColourWheelRadius * 2.0f;
		_mColour.set(1, 1, 1, 1);
		_mstrResourceNameWheel = "CUIColourSelectorImageWheel_" + pContainer->getName() + "_" + strWidgetName;
		_mstrResourceNameCircle = "CUIColourSelectorImageCircle_" + pContainer->getName() + "_" + strWidgetName;
		

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

		CUIText* pText;
		for (int i = 0; i < 7; i++)
		{
			_mpContainer->scrollbarAdd(_mstrScrollbarNames[i], 0, 0, iColourWheelRadius * 2, 40, 0.01f);
			_mpContainer->textEditAdd(_mstrTextEditNames[i], 0, 0, 40, 40, "0");
			pText = _mpContainer->textAdd(_mstrTextNames[i], 0, 0, 150, 40, "X");
			pText->setTextColour(false, CColour(1, 1, 1, 0.90f));
			pText->setCentered(true);
		}
		_updateTextures();
		setTextEditDims();
	}

	CUIColourSelector::~CUIColourSelector()
	{
		x->pResource->removeTexture2DFromImage(_mstrResourceNameWheel);
		x->pResource->removeTexture2DFromImage(_mstrResourceNameCircle);

		for (int i = 0; i < 7; i++)
		{
			_mpContainer->scrollbarRemove(_mstrScrollbarNames[i]);
			_mpContainer->textEditRemove(_mstrTextEditNames[i]);
		}
	}

	void CUIColourSelector::setDimensions(float fColourWheelRadius)
	{
		_mvDimensions.x = fColourWheelRadius;
		_mvDimensions.y = fColourWheelRadius;

		_updateTextures();
		setColour(_mColour);	// Also update selector positions
		_updateScrollbarsAndTextEdits();
		_mpContainer->computeScrollbars();
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

//		const CUITheme::SSettings* pThemeSettings = _mpContainer->themeGetSettings();

		if (_mstrResourceNameWheel.size())
		{
			CVector2f vPos = _mpContainer->getWidgetAreaTLCornerPosition() + _mvPosition + _mpContainer->getWidgetOffset();
			x->pRenderer->blendEnable();

			// Render the wheel
			CResourceTexture2DFromImage* pImage = x->pResource->getTexture2DFromImage(_mstrResourceNameWheel);
			CVector2f vWheelDims = pImage->getDimensions();
			pImage->renderTo2DQuad(
				(int)vPos.x,
				(int)vPos.y,
				(int)vWheelDims.x,
				(int)vWheelDims.y, CColour());
		
			x->pRenderer->blendDisable();
		}
	}

	void CUIColourSelector::update(float fTimeDeltaSec)
	{
		CUIScrollbar* pSB;
		CUITextEdit* pTE;
		std::string strTxt;

		for (int i = 0; i < 7; i++)
		{
			// Scrollbar
			pSB = _mpContainer->scrollbarGet(_mstrScrollbarNames[i]);
			if (pSB->getTabBeingMoved())
			{
				strTxt.clear();
				StringUtils::appendFloat(strTxt, pSB->getTabPos(), 3);
				pTE = _mpContainer->textEditGet(_mstrTextEditNames[i]);
				pTE->setText(strTxt);
			}

			// Text edit
			pTE = _mpContainer->textEditGet(_mstrTextEditNames[i]);
			if (pTE->getHasTextChanged())
			{
				float fValue = StringUtils::stringToFloat(pTE->getText());
				pSB = _mpContainer->scrollbarGet(_mstrScrollbarNames[i]);
				pSB->setTabPos(fValue);
			}
		}
	}

	/******************************************************************* Widget specific *******************************************************************/

	void CUIColourSelector::setSelectorCircleRadius(int iCircleRadius)
	{
		_miCircleRadius = iCircleRadius;
		_updateTextures();
	}

	void CUIColourSelector::_updateTextures(void)
	{
		// Colour wheel
		CImage image;
		unsigned int uiWheelSize = unsigned int(_mvDimensions.x);
		float fBrightness = _mpContainer->scrollbarGet(_mstrScrollbarNames[EValue::brightness])->getTabPos();
		image.drawColourWheel(uiWheelSize, unsigned char(fBrightness * 255.0f));
		CResourceTexture2DFromImage* pImage;
		if (!x->pResource->getTexture2DFromImageExists(_mstrResourceNameWheel))
		{
			pImage = x->pResource->addTexture2DFromImage(_mstrResourceNameWheel, image);
		}
		else
		{
			pImage = x->pResource->getTexture2DFromImage(_mstrResourceNameWheel);
			pImage->update(image);
		}
		
		// Selector circle
		image.drawCircle(_miCircleRadius * 2, CColour(1, 1, 1, 1), CColour(0, 0, 0, 0));
		if (!x->pResource->getTexture2DFromImageExists(_mstrResourceNameCircle))
		{
			pImage = x->pResource->addTexture2DFromImage(_mstrResourceNameCircle, image);
		}
		else
		{
			pImage = x->pResource->getTexture2DFromImage(_mstrResourceNameCircle);
			pImage->update(image);
		}
	}


	void CUIColourSelector::_updateScrollbarsAndTextEdits(void)
	{
		CVector2f vPos = _mpContainer->getWidgetAreaTLCornerPosition() + _mvPosition + _mpContainer->getWidgetOffset();
		CResourceTexture2DFromImage* pImage = x->pResource->getTexture2DFromImage(_mstrResourceNameWheel);
		CVector2f vWheelDims = pImage->getDimensions();

		CUITextEdit* pTE;
		pTE = _mpContainer->textEditGet(_mstrTextEditNames[0]);
		pTE->setPosition(_mvDimensions.x - pTE->getDimensions().x, vWheelDims.y);

		pTE = _mpContainer->textEditGet(_mstrTextEditNames[1]);
		pTE->setPosition(_mvDimensions.x - pTE->getDimensions().x, vWheelDims.y + (pTE->getDimensions().y));

		pTE = _mpContainer->textEditGet(_mstrTextEditNames[2]);
		pTE->setPosition(_mvDimensions.x - pTE->getDimensions().x, vWheelDims.y + (pTE->getDimensions().y * 2));

		pTE = _mpContainer->textEditGet(_mstrTextEditNames[3]);
		pTE->setPosition(_mvDimensions.x - pTE->getDimensions().x, vWheelDims.y + (pTE->getDimensions().y * 3));

		pTE = _mpContainer->textEditGet(_mstrTextEditNames[4]);
		pTE->setPosition(_mvDimensions.x - pTE->getDimensions().x, vWheelDims.y + (pTE->getDimensions().y * 4));

		pTE = _mpContainer->textEditGet(_mstrTextEditNames[5]);
		pTE->setPosition(_mvDimensions.x - pTE->getDimensions().x, vWheelDims.y + (pTE->getDimensions().y * 5));

		pTE = _mpContainer->textEditGet(_mstrTextEditNames[6]);
		pTE->setPosition(_mvDimensions.x - pTE->getDimensions().x, vWheelDims.y + (pTE->getDimensions().y * 6));

		CUIScrollbar* pSB;
		pSB = _mpContainer->scrollbarGet(_mstrScrollbarNames[0]);
		pSB->setPosition(0.0f, vWheelDims.y);
		pSB->setDimensions(_mvDimensions.x - pTE->getDimensions().x, pTE->getDimensions().y);
		pSB->setTabRatio(0.2f);

		pSB = _mpContainer->scrollbarGet(_mstrScrollbarNames[1]);
		pSB->setPosition(0.0f, vWheelDims.y + (pTE->getDimensions().y));
		pSB->setDimensions(_mvDimensions.x - pTE->getDimensions().x, pTE->getDimensions().y);
		pSB->setTabRatio(0.2f);

		pSB = _mpContainer->scrollbarGet(_mstrScrollbarNames[2]);
		pSB->setPosition(0.0f, vWheelDims.y + (pTE->getDimensions().y * 2));
		pSB->setDimensions(_mvDimensions.x - pTE->getDimensions().x, pTE->getDimensions().y);
		pSB->setTabRatio(0.2f);

		pSB = _mpContainer->scrollbarGet(_mstrScrollbarNames[3]);
		pSB->setPosition(0.0f, vWheelDims.y + (pTE->getDimensions().y * 3));
		pSB->setDimensions(_mvDimensions.x - pTE->getDimensions().x, pTE->getDimensions().y);
		pSB->setTabRatio(0.2f);

		pSB = _mpContainer->scrollbarGet(_mstrScrollbarNames[4]);
		pSB->setPosition(0.0f, vWheelDims.y + (pTE->getDimensions().y * 4));
		pSB->setDimensions(_mvDimensions.x - pTE->getDimensions().x, pTE->getDimensions().y);
		pSB->setTabRatio(0.2f);

		pSB = _mpContainer->scrollbarGet(_mstrScrollbarNames[5]);
		pSB->setPosition(0.0f, vWheelDims.y + (pTE->getDimensions().y * 5));
		pSB->setDimensions(_mvDimensions.x - pTE->getDimensions().x, pTE->getDimensions().y);
		pSB->setTabRatio(0.2f);

		pSB = _mpContainer->scrollbarGet(_mstrScrollbarNames[6]);
		pSB->setPosition(0.0f, vWheelDims.y + (pTE->getDimensions().y * 6));
		pSB->setDimensions(_mvDimensions.x - pTE->getDimensions().x, pTE->getDimensions().y);
		pSB->setTabRatio(0.2f);

		CUIText* pText;
		float fPosX = _mvDimensions.x - pTE->getDimensions().x - (pSB->getDimensions().x * 0.5f);
		float fPosY = vWheelDims.y + (pTE->getDimensions().y * 0.5f);
		pText = _mpContainer->textGet(_mstrTextNames[0]);
		pText->setPosition(fPosX, fPosY);
		pText->setText("Brightness");
		
		fPosY += pTE->getDimensions().y;
		pText = _mpContainer->textGet(_mstrTextNames[1]);
		pText->setPosition(fPosX, fPosY);
		pText->setText("Red");

		fPosY += pTE->getDimensions().y;
		pText = _mpContainer->textGet(_mstrTextNames[2]);
		pText->setPosition(fPosX, fPosY);
		pText->setText("Green");

		fPosY += pTE->getDimensions().y;
		pText = _mpContainer->textGet(_mstrTextNames[3]);
		pText->setPosition(fPosX, fPosY);
		pText->setText("Blue");

		fPosY += pTE->getDimensions().y;
		pText = _mpContainer->textGet(_mstrTextNames[4]);
		pText->setPosition(fPosX, fPosY);
		pText->setText("Alpha");

		fPosY += pTE->getDimensions().y;
		pText = _mpContainer->textGet(_mstrTextNames[5]);
		pText->setPosition(fPosX, fPosY);
		pText->setText("Hue");

		fPosY += pTE->getDimensions().y;
		pText = _mpContainer->textGet(_mstrTextNames[6]);
		pText->setPosition(fPosX, fPosY);
		pText->setText("Saturation");

	}

	void CUIColourSelector::setTextEditDims(const CVector2f& vDims)
	{
		for (int i = 0; i < 7; i++)
		{
			_mpContainer->textEditGet(_mstrTextEditNames[i])->setDimensions(vDims);
		}
		_updateScrollbarsAndTextEdits();
	}

	void CUIColourSelector::setColour(const CColour& colour)
	{
		_mColour = colour;

		CVector2f vPos = _mpContainer->getWidgetAreaTLCornerPosition() + _mvPosition + _mpContainer->getWidgetOffset();
		CResourceTexture2DFromImage* pImage = x->pResource->getTexture2DFromImage(_mstrResourceNameWheel);
		CVector2f vWheelDims = pImage->getDimensions();

	}
}