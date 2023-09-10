#include "PCH.h"
#include "UIButtonImage.h"
#include "utilities.h"
#include "UIContainer.h"
#include "singletons.h"
#include "UITheme.h"

namespace X
{
	CUIButtonImage::CUIButtonImage(CUIContainer* pContainer, const std::string& strTextureFromFileResourceNameUp, const std::string& strTextureFromFileResourceNameOver, const std::string& strTextureFromFileResourceNameDown)
	{
		_mpContainer = pContainer;
		_mvDimensions.set(200, 48);
		_mbVisible = true;
		_mState = state::up;
		_mbClicked = false;

		_mfuncOnClicked = 0;

		_mstrTextureUp = strTextureFromFileResourceNameUp;
		_mstrTextureOver = strTextureFromFileResourceNameOver;
		_mstrTextureDown = strTextureFromFileResourceNameDown;
	}

	CUIButtonImage::~CUIButtonImage()
	{

	}

	void CUIButtonImage::setDimensions(float fX, float fY)
	{
		if (fX >= 1)
			_mvDimensions.x = fX;
		else
			_mvDimensions.x = x->pResource->getTexture2DFromFile(_mstrTextureUp)->getDimensions().x;
		
		if (fY >= 1)
			_mvDimensions.y = fY;
		else
			_mvDimensions.x = x->pResource->getTexture2DFromFile(_mstrTextureUp)->getDimensions().y;
		_mpContainer->computeScrollbars();
	}

	void CUIButtonImage::setDimensions(const CVector2f& vDimensions)
	{
		if (vDimensions.x >= 1)
			_mvDimensions.x = vDimensions.x;
		else
			_mvDimensions.x = x->pResource->getTexture2DFromFile(_mstrTextureUp)->getDimensions().x;

		if (vDimensions.y >= 1)
			_mvDimensions.y = vDimensions.y;
		else
			_mvDimensions.x = x->pResource->getTexture2DFromFile(_mstrTextureUp)->getDimensions().y;
		_mpContainer->computeScrollbars();
	}

	CVector2f CUIButtonImage::getDimensions(void) const
	{
		return _mvDimensions;
	}

	void CUIButtonImage::setPosition(float fX, float fY)
	{
		_mvPosition.x = fX;
		_mvPosition.y = fY;
		_mpContainer->computeScrollbars();
	}

	void CUIButtonImage::setPosition(const CVector2f& vPosition)
	{
		_mvPosition = vPosition;
		_mpContainer->computeScrollbars();
	}

	CVector2f CUIButtonImage::getPosition(void) const
	{
		return _mvPosition;
	}

	void CUIButtonImage::setVisible(bool bVisible)
	{
		_mbVisible = bVisible;
		_mpContainer->computeScrollbars();
	}

	bool CUIButtonImage::getVisible(void) const
	{
		return _mbVisible;
	}

	void CUIButtonImage::render(void)
	{
		// Render image
		CVector2f vWidgetOffset = _mpContainer->getWidgetOffset();
		CVector2f vPos = vWidgetOffset + _mvPosition;
		if (state::up == _mState)
			x->pResource->getTexture2DFromFile(_mstrTextureUp)->renderTo2DQuad((int)vPos.x, (int)vPos.y, (int)_mvDimensions.x, (int)_mvDimensions.y, _mColourBG);
		else if (state::over == _mState)
			x->pResource->getTexture2DFromFile(_mstrTextureOver)->renderTo2DQuad((int)vPos.x, (int)vPos.y, (int)_mvDimensions.x, (int)_mvDimensions.y, _mColourBG);
		else if (state::down == _mState)
			x->pResource->getTexture2DFromFile(_mstrTextureDown)->renderTo2DQuad((int)vPos.x, (int)vPos.y, (int)_mvDimensions.x, (int)_mvDimensions.y, _mColourBG);

		// Render font
		CUITheme* pTheme = _mpContainer->themeGet();
		const CUITheme::SSettings* pThemeSettings = pTheme->getSettings();
		CResourceFont* pFont = x->pResource->getFont(pThemeSettings->fonts.buttonImage);

		CVector2f vTextPos = _mpContainer->getWidgetAreaTLCornerPosition();
		vTextPos += _mvPosition;
		vTextPos += _mpContainer->getWidgetOffset();
		vTextPos.x += _mvDimensions.x * 0.5f;
		vTextPos.y += _mvDimensions.y * 0.5f;
		pFont->printCentered(_mstrText, (int)vTextPos.x, (int)vTextPos.y, x->pWindow->getWidth(), x->pWindow->getHeight(), 1.0f, _mColourText);
	}

	void CUIButtonImage::update(float fTimeDeltaSec)
	{
		// Determine whether the mouse cursor is over this widget's container or not.
		// This is all to prevent doing expensive checks if we don't need to.
		bool bContainerHasMouseOver = false;
		std::string strMouseIsOver = x->pUI->getMouseIsOverWhichContainer();	// Get the name of the container/window that the mouse cursor is over, if any.
		if (strMouseIsOver == _mpContainer->getName())
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
			CVector2f vWidgetAreaTLPos = _mpContainer->getWidgetAreaTLCornerPosition() + _mpContainer->getWidgetOffset();
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

					// Call function pointer if it has been set
					if (_mfuncOnClicked)
						_mfuncOnClicked();
				}
			}
		}	// if (bContainerHasMouseOver)
	
		// Depending upon state, compute text and BG colours
		const CUITheme::SSettings* pSettings = _mpContainer->themeGetSettings();
		CColour colTargetBG;
		CColour colTargetText;
		if (_mState == state::up)
		{
			colTargetBG = pSettings->colours.buttonImageBGUp;
			colTargetText = pSettings->colours.buttonImageTextUp;
		}
		else if (_mState == state::over)
		{
			colTargetBG = pSettings->colours.buttonImageBGOver;
			colTargetText = pSettings->colours.buttonImageTextOver;
		}
		else
		{
			colTargetBG = pSettings->colours.buttonImageBGDown;
			colTargetText = pSettings->colours.buttonImageTextDown;
		}
		x->pUI->_helperColourAdjust(_mColourBG, colTargetBG, fTimeDeltaSec, pSettings->floats.buttonImageFadeSpeed);
		x->pUI->_helperColourAdjust(_mColourText, colTargetText, fTimeDeltaSec, pSettings->floats.buttonImageFadeSpeed);
	}

	void CUIButtonImage::reset(void)
	{
		const CUITheme::SSettings* pSettings = _mpContainer->themeGetSettings();
		_mColourBG = pSettings->colours.buttonBGUp;
		_mColourText = pSettings->colours.buttonTextUp;
	}

	/******************************************************************* Widget specific *******************************************************************/

	void CUIButtonImage::setText(const std::string& strText)
	{
		_mstrText = strText;
	}

	std::string CUIButtonImage::getText(void) const
	{
		return _mstrText;
	}

	bool CUIButtonImage::getClicked(void) const
	{
		return _mbClicked;
	}

	void CUIButtonImage::setFunctionOnClicked(void (*function)(void))
	{
		_mfuncOnClicked = function;
	}
}