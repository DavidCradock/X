#include "PCH.h"
#include "UICheckbox.h"
#include "utilities.h"
#include "UIContainer.h"
#include "singletons.h"
#include "UITheme.h"

namespace X
{
	CUICheckbox::CUICheckbox(CUIContainer* pContainer)
	{
		_mpContainer = pContainer;
		_mvDimensions.set(200, 48);
		_mbVisible = true;
		_mState = state::up;
		_mbClicked = false;

		_mfuncOnClicked = 0;
	}

	CUICheckbox::~CUICheckbox()
	{

	}

	void CUICheckbox::setDimensions(float fX, float fY)
	{
		_mvDimensions.x = fX;
		_mvDimensions.y = fY;
		_mpContainer->computeScrollbars();
	}

	void CUICheckbox::setDimensions(int iX, int iY)
	{
		setDimensions(float(iX), float(iY));
	}

	void CUICheckbox::setDimensions(const CVector2f& vDimensions)
	{
		setDimensions(vDimensions.x, vDimensions.y);
	}

	CVector2f CUICheckbox::getDimensions(void) const
	{
		return _mvDimensions;
	}

	CVector2f CUICheckbox::getDimensionsMinimum(void) const
	{
		CUITheme* pTheme = _mpContainer->themeGet();
		CResourceTexture2DAtlas* pAtlas = pTheme->getTextureAtlas();
		const CUITheme::SSettings* pThemeSettings = _mpContainer->themeGetSettings();

		CVector2f vMinimumDims;
		CImageAtlasDetails* pID = pAtlas->getImageDetailsPointer(pThemeSettings->images.checkboxBGOff.colour.cornerTL);
		vMinimumDims = pID->vDims;
		pID = pAtlas->getImageDetailsPointer(pThemeSettings->images.checkboxBGOff.colour.cornerBR);
		vMinimumDims += pID->vDims;
		pID = pAtlas->getImageDetailsPointer(pThemeSettings->images.checkboxBGOff.colour.edgeT);
		vMinimumDims.x += pID->vDims.x;
		pID = pAtlas->getImageDetailsPointer(pThemeSettings->images.checkboxBGOff.colour.edgeR);
		vMinimumDims.y += pID->vDims.y;
		return vMinimumDims;
	}

	void CUICheckbox::setPosition(float fX, float fY)
	{
		_mvPosition.x = fX;
		_mvPosition.y = fY;
		_mpContainer->computeScrollbars();
	}

	void CUICheckbox::setPosition(int iX, int iY)
	{
		setPosition(float(iX), float(iY));
	}

	void CUICheckbox::setPosition(const CVector2f& vPosition)
	{
		setPosition(vPosition.x, vPosition.y);
	}

	CVector2f CUICheckbox::getPosition(void) const
	{
		return _mvPosition;
	}

	void CUICheckbox::setVisible(bool bVisible)
	{
		_mbVisible = bVisible;
		_mpContainer->computeScrollbars();
	}

	bool CUICheckbox::getVisible(void) const
	{
		return _mbVisible;
	}

	void CUICheckbox::renderBG(CResourceVertexBufferCPT2* pVB)
	{
		if (!_mbVisible)
			return;

		CUITheme::SSettings* pThemeSettings = _mpContainer->themeGetSettings();

		// Add geometry for the 9 grid cells
		x->pUI->_helperAddWidgetGridGeometry(
			_mvPosition + _mpContainer->getWidgetOffset(),
			_mvDimensions,
			pThemeSettings->images.buttonBG,
			_mColourBG,
			_mpContainer,
			pVB);
	}

	void CUICheckbox::renderNonBG(void)
	{
		if (!_mpContainer->getVisible())
			return;

		CUITheme* pTheme = _mpContainer->themeGet();
		const CUITheme::SSettings* pThemeSettings = pTheme->getSettings();
		CResourceFont* pFont = x->pResource->getFont(pThemeSettings->fonts.button);
		
		CVector2f vTextPos = _mpContainer->getWidgetAreaTLCornerPosition();
		vTextPos += _mvPosition;
		vTextPos += _mpContainer->getWidgetOffset();
		vTextPos.x += _mvDimensions.x * 0.5f;
		vTextPos.y += _mvDimensions.y * 0.5f;
		pFont->printCentered(_mstrText, (int)vTextPos.x, (int)vTextPos.y, x->pWindow->getWidth(), x->pWindow->getHeight(), 1.0f, _mColourText);
	}

	void CUICheckbox::update(float fTimeDeltaSec)
	{
//		if (!_mpContainer->getVisible())
//			return;

		// Determine whether the mouse cursor is over this widget's container or not.
		// This is all to prevent doing expensive checks if we don't need to.
		bool bContainerHasMouseOver = false;
		std::string strMouseIsOver = x->pUI->getMouseIsOverWhichContainer();	// Get the name of the container/window that the mouse cursor is over, if any.
		if (strMouseIsOver == _mpContainer->getName())
			bContainerHasMouseOver = true;

		const CUITheme::SSettings* pThemeSettings = _mpContainer->themeGetSettings();

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

					x->pAudio->getEmitter(pThemeSettings->audio.buttonClicked.sampleName)->play(x->pSettings->getUIVolume() * pThemeSettings->audio.buttonClicked.volume, pThemeSettings->audio.buttonClicked.pitch);
				}
			}
		}	// if (bContainerHasMouseOver)
	
		// Depending upon state, compute text and BG colours
		const CUITheme::SSettings* pSettings = _mpContainer->themeGetSettings();
		CColour colTargetBG;
		CColour colTargetText;
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
		x->pUI->_helperColourAdjust(_mColourBG, colTargetBG, fTimeDeltaSec, pSettings->floats.buttonFadeSpeed);
		x->pUI->_helperColourAdjust(_mColourText, colTargetText, fTimeDeltaSec, pSettings->floats.buttonFadeSpeed);
	}

	void CUICheckbox::reset(void)
	{
		const CUITheme::SSettings* pSettings = _mpContainer->themeGetSettings();
		_mColourBG = pSettings->colours.buttonBGUp;
		_mColourText = pSettings->colours.buttonTextUp;
	}

	/******************************************************************* Widget specific *******************************************************************/

	void CUICheckbox::setText(const std::string& strText)
	{
		_mstrText = strText;
	}

	std::string CUICheckbox::getText(void) const
	{
		return _mstrText;
	}

	bool CUICheckbox::getClicked(void) const
	{
		return _mbClicked;
	}

	void CUICheckbox::setFunctionOnClicked(void (*function)(void))
	{
		_mfuncOnClicked = function;
	}
}