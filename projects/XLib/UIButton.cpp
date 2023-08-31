#include "PCH.h"
#include "UIButton.h"
#include "utilities.h"
#include "UIContainer.h"
#include "singletons.h"
#include "UITheme.h"

namespace X
{
	CUIButton::CUIButton()
	{
		setDimensions(CVector2f(200, 48));
		setPosition(CVector2f(0, 0));
		setVisible(true);
		_mState = state::up;
		_mbClicked = false;
	}

	CUIButton::~CUIButton()
	{

	}

	void CUIButton::setDimensions(float fX, float fY)
	{
		_mvDimensions.x = fX;
		_mvDimensions.y = fY;
	}

	void CUIButton::setDimensions(const CVector2f& vDimensions)
	{
		_mvDimensions = vDimensions;
	}

	CVector2f CUIButton::getDimensions(void) const
	{
		return _mvDimensions;
	}

	void CUIButton::setPosition(float fX, float fY)
	{
		_mvPosition.x = fX;
		_mvPosition.y = fY;
	}

	void CUIButton::setPosition(const CVector2f& vPosition)
	{
		_mvPosition = vPosition;
	}

	CVector2f CUIButton::getPosition(void) const
	{
		return _mvPosition;
	}

	void CUIButton::setVisible(bool bVisible)
	{
		_mbVisible = bVisible;
	}

	bool CUIButton::getVisible(void) const
	{
		return _mbVisible;
	}

	void CUIButton::render(CUIContainer* pContainer, CResourceVertexBufferCPT2* pVB)
	{
		const CUITheme::SSettings* pThemeSettings = pContainer->themeGetSettings();

		// Add geometry for the 9 grid cells
		x->pUI->_addWidgetGridGeometry(
			_mvPosition,
			_mvDimensions,
			pThemeSettings->images.buttonBG,
			_mColourBG,
			pContainer,
			pVB);
	}

	void CUIButton::renderFonts(CUIContainer* pContainer)
	{
		CUITheme* pTheme = pContainer->themeGet();
		const CUITheme::SSettings* pThemeSettings = pTheme->getSettings();
		CResourceFont* pFont = x->pResource->getFont(pThemeSettings->fonts.button);
		
		CVector2f vTextPos = pContainer->getWidgetAreaTLCornerPosition();
		vTextPos.x += _mvDimensions.x * 0.5f;
		vTextPos.y += _mvDimensions.y * 0.5f;
		pFont->printCentered(_mstrText, (int)vTextPos.x, (int)vTextPos.y, x->pWindow->getWidth(), x->pWindow->getHeight(), 1.0f, _mColourText);
	}

	void CUIButton::update(float fTimeDeltaSec, CUIContainer* pContainer)
	{
		// Determine whether the mouse cursor is over this widget's container or not.
		// This is all to prevent doing expensive checks if we don't need to.
		bool bContainerHasMouseOver = false;
		std::string strMouseIsOver = x->pUI->getMouseIsOver();	// Get the name of the container/window that the mouse cursor is over, if any.
		if (strMouseIsOver == pContainer->getName())
			bContainerHasMouseOver = true;

		// Set state of button prior to checks below
		_mState = state::up;

		// And do this again but for _mbClicked
		_mbClicked = false;

		if (bContainerHasMouseOver)
		{
			// We'll be needing the mouse cursor screen position
			CVector2f vMousePos = x->pInput->mouse.getCursorPos();

			// Compute a CRect which represents this widget's actual screen area
			CRect rctWidget;	// This will hold actual screen position for the widget's four corners.
			CVector2f vWidgetAreaTLPos = pContainer->getWidgetAreaTLCornerPosition();
			rctWidget.miMinX = int(vWidgetAreaTLPos.x + _mvPosition.x);
			rctWidget.miMinY = int(vWidgetAreaTLPos.y + _mvPosition.y);
			rctWidget.miMaxX = rctWidget.miMinX + int(_mvDimensions.x);
			rctWidget.miMaxY = rctWidget.miMinY + int(_mvDimensions.y);

			// Determine whether the mouse cursor is over the button or not
			bool bMouseOver = false;
			if (rctWidget.doesPositionFitWithin(vMousePos))
				bMouseOver = true;

			// Determine current state of the button (up/over/down)
			if (bMouseOver)
			{
				_mState = state::over;
				if (x->pInput->mouse.leftButDown())
				{
					_mState = state::down;
				}
				// Determine whether the button has been clicked upon.
				if (x->pInput->mouse.leftButtonOnce())
				{
					_mbClicked = true;
				}
			}
		}	// if (bContainerHasMouseOver)
	
		// Depending upon state, compute text and BG colours
		const CUITheme::SSettings* pSettings = pContainer->themeGetSettings();
		CColour colTargetBG;	// Will hold the colour of the button's current state for it's background
		CColour colTargetText;	// Same again, but for the text colour.
		if (_mState == state::up)
		{
			colTargetBG = pSettings->colours.buttonBGUp;
			colTargetText = pSettings->colours.buttonTextUp;
		}
		else if (_mState == state::over)
		{
			colTargetBG = pSettings->colours.buttonBGOver;
			colTargetText = pSettings->colours.buttonTextOver;
		}
		else
		{
			colTargetBG = pSettings->colours.buttonBGDown;
			colTargetText = pSettings->colours.buttonTextDown;
		}
		_helperColourAdjust(_mColourBG, colTargetBG, fTimeDeltaSec, pSettings);
		_helperColourAdjust(_mColourText, colTargetText, fTimeDeltaSec, pSettings);
	}

	void CUIButton::setText(const std::string& strText)
	{
		_mstrText = strText;
	}

	std::string CUIButton::getText(void) const
	{
		return _mstrText;
	}

	void CUIButton::_helperColourAdjust(CColour& colourAdjust, const CColour& colourTarget, float fTimeDeltaSecs, const CUITheme::SSettings* pSettings)
	{
		if (colourAdjust.red < colourTarget.red)
		{
			colourAdjust.red += fTimeDeltaSecs * pSettings->floats.buttonFadeSpeed;
			if (colourAdjust.red > colourTarget.red)
				colourAdjust.red = colourTarget.red;
		}
		else
		{
			colourAdjust.red -= fTimeDeltaSecs * pSettings->floats.buttonFadeSpeed;
			if (colourAdjust.red < colourTarget.red)
				colourAdjust.red = colourTarget.red;
		}

		if (colourAdjust.green < colourTarget.green)
		{
			colourAdjust.green += fTimeDeltaSecs * pSettings->floats.buttonFadeSpeed;
			if (colourAdjust.green > colourTarget.green)
				colourAdjust.green = colourTarget.green;
		}
		else
		{
			colourAdjust.green -= fTimeDeltaSecs * pSettings->floats.buttonFadeSpeed;
			if (colourAdjust.green < colourTarget.green)
				colourAdjust.green = colourTarget.green;
		}

		if (colourAdjust.blue < colourTarget.blue)
		{
			colourAdjust.blue += fTimeDeltaSecs * pSettings->floats.buttonFadeSpeed;
			if (colourAdjust.blue > colourTarget.blue)
				colourAdjust.blue = colourTarget.blue;
		}
		else
		{
			colourAdjust.blue -= fTimeDeltaSecs * pSettings->floats.buttonFadeSpeed;
			if (colourAdjust.blue < colourTarget.blue)
				colourAdjust.blue = colourTarget.blue;
		}

		if (colourAdjust.alpha < colourTarget.alpha)
		{
			colourAdjust.alpha += fTimeDeltaSecs * pSettings->floats.buttonFadeSpeed;
			if (colourAdjust.alpha > colourTarget.alpha)
				colourAdjust.alpha = colourTarget.alpha;
		}
		else
		{
			colourAdjust.alpha -= fTimeDeltaSecs * pSettings->floats.buttonFadeSpeed;
			if (colourAdjust.alpha < colourTarget.alpha)
				colourAdjust.alpha = colourTarget.alpha;
		}
	}
}