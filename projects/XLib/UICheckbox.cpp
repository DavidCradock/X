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
		_mvDimensions = getDimensionsMinimum();
		_mbVisible = true;
		_mState = state::off;
		_mbClicked = false;
		_mbOn = false;
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
		if (!_mbVisible)
			reset();
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

		CUITheme::SImageType imageType;
		CColour colour;
		if (state::off == _mState)
		{
			colour = pThemeSettings->colours.checkboxBGOff;
			imageType = pThemeSettings->images.checkboxBGOff;
		}
		else if (state::on == _mState)
		{
			colour = pThemeSettings->colours.checkboxBGOn;
			imageType = pThemeSettings->images.checkboxBGOn;
		}
		else if (state::offOver == _mState)
		{
			colour = pThemeSettings->colours.checkboxBGOffOver;
			imageType = pThemeSettings->images.checkboxBGOffOver;
		}
		else
		{
			colour = pThemeSettings->colours.checkboxBGOnOver;
			imageType = pThemeSettings->images.checkboxBGOnOver;
		}

		x->pUI->_helperAddWidgetBGGeometry(
			_mvPosition + _mpContainer->getWidgetOffset(),
			_mvDimensions,
			imageType,
			colour,
			_mpContainer,
			pVB);
	}

	void CUICheckbox::update(float fTimeDeltaSec)
	{
		if (!_mpContainer->getVisible())
			return;

		bool bContainerHasMouseOver = false;
		std::string strMouseIsOver = x->pUI->getMouseIsOverWhichContainer();
		if (strMouseIsOver == _mpContainer->getName())
			bContainerHasMouseOver = true;

		const CUITheme::SSettings* pThemeSettings = _mpContainer->themeGetSettings();

		if (_mbOn)
			_mState = state::on;
		else
			_mState = state::off;

		_mbClicked = false;

		if (bContainerHasMouseOver)
		{
			CVector2f vMousePos = x->pInput->mouse.getCursorPos();
			CRect rctWidget;
			CVector2f vWidgetAreaTLPos = _mpContainer->getWidgetAreaTLCornerPosition() + _mpContainer->getWidgetOffset();
			rctWidget.miMinX = int(vWidgetAreaTLPos.x + _mvPosition.x);
			rctWidget.miMinY = int(vWidgetAreaTLPos.y + _mvPosition.y);
			rctWidget.miMaxX = rctWidget.miMinX + int(_mvDimensions.x);
			rctWidget.miMaxY = rctWidget.miMinY + int(_mvDimensions.y);

			bool bMouseOver = false;
			if (rctWidget.doesPositionFitWithin(vMousePos))
				bMouseOver = true;

			if (bMouseOver)
			{
				if (_mbOn)
					_mState = state::onOver;
				else
					_mState = state::offOver;

				if (x->pInput->mouse.leftButtonOnce())
				{
					_mbOn = !_mbOn;

					_mbClicked = true;

					if (_mfuncOnClicked)
						_mfuncOnClicked();
				}
			}
		}
	}

	void CUICheckbox::reset(void)
	{
		const CUITheme::SSettings* pSettings = _mpContainer->themeGetSettings();
		if (_mbOn)
			_mState = state::on;
		else
			_mState = state::off;
	}

	/******************************************************************* Widget specific *******************************************************************/

	bool CUICheckbox::getClicked(void) const
	{
		return _mbClicked;
	}

	void CUICheckbox::setFunctionOnClicked(void (*function)(void))
	{
		_mfuncOnClicked = function;
	}
}