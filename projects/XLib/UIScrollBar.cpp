#include "PCH.h"
#include "UIScrollBar.h"
#include "utilities.h"
#include "UIContainer.h"
#include "singletons.h"
#include "UITheme.h"

namespace X
{
	CUIScrollbar::CUIScrollbar()
	{
		setDimensions(CVector2f(200, 48));
		setPosition(CVector2f(0, 0));
		setVisible(true);

		_mfScrollbarPosition = 0.5f;
		_mbTabBeingMoved = false;
		_mfTabRatio = 0.5f;
		
		_mbOrientationIsHorizontal = true;
	}

	CUIScrollbar::~CUIScrollbar()
	{

	}

	void CUIScrollbar::setDimensions(float fX, float fY)
	{
		_mvDimensions.x = fX;
		_mvDimensions.y = fY;
	}

	void CUIScrollbar::setDimensions(const CVector2f& vDimensions)
	{
		_mvDimensions = vDimensions;
	}

	CVector2f CUIScrollbar::getDimensions(void) const
	{
		return _mvDimensions;
	}

	void CUIScrollbar::setPosition(float fX, float fY)
	{
		_mvPosition.x = fX;
		_mvPosition.y = fY;
	}

	void CUIScrollbar::setPosition(const CVector2f& vPosition)
	{
		_mvPosition = vPosition;
	}

	CVector2f CUIScrollbar::getPosition(void) const
	{
		return _mvPosition;
	}

	void CUIScrollbar::setVisible(bool bVisible)
	{
		_mbVisible = bVisible;
	}

	bool CUIScrollbar::getVisible(void) const
	{
		return _mbVisible;
	}

	void CUIScrollbar::render(CUIContainer* pContainer, CResourceVertexBufferCPT2* pVB)
	{
		const CUITheme::SSettings* pThemeSettings = pContainer->themeGetSettings();

		// Add geometry for the 9 grid cells
		x->pUI->_addWidgetGridGeometry(
			_mvPosition,
			_mvDimensions,
			pThemeSettings->images.scrollbarBG,
			pThemeSettings->colours.scrollbarBG,
			pContainer,
			pVB);
	}

	void CUIScrollbar::update(float fTimeDeltaSec, CUIContainer* pContainer)
	{
		CUITheme* pTheme = pContainer->themeGet();
		CResourceTexture2DAtlas* pAtlas = pTheme->getTextureAtlas();

		// Return name of the window, or container which the mouse cursor is over
		std::string strMouseIsOver = x->pUI->getMouseIsOver();
		bool bAcceptingMouseInput = false;
		// And if that window or container is this object's container, we're accepting mouse input
		if (strMouseIsOver == pContainer->getName())
			bAcceptingMouseInput = true;

		CVector2f vMousePos = x->pInput->mouse.getCursorPos();

		// Compute orientation
		_mbOrientationIsHorizontal = false;
		if (_mvDimensions.x > _mvDimensions.y)
		{
			_mbOrientationIsHorizontal = true;
		}

		// Compute tab dims
		const CUITheme::SSettings* pThemeSettings = pTheme->getSettings();
		CImageAtlasDetails imageDetailsCornerTR = pAtlas->getImageDetails(pThemeSettings->images.scrollbarTab.colour.cornerTR);
		CImageAtlasDetails imageDetailsCornerBL = pAtlas->getImageDetails(pThemeSettings->images.scrollbarTab.colour.cornerBL);

		if (_mbOrientationIsHorizontal)
		{
			_mfTabDims[0] = _mfTabRatio * (_mvDimensions.x - imageDetailsCornerTR.vDims.x - imageDetailsCornerBL.vDims.x);
			_mfTabDims[1] = _mvDimensions.y - imageDetailsCornerTR.vDims.y - imageDetailsCornerBL.vDims.y;
		}
		else
		{
			_mfTabDims[0] = _mvDimensions.x - imageDetailsCornerTR.vDims.x - imageDetailsCornerBL.vDims.x;
			_mfTabDims[1] = _mfTabRatio * (_mvDimensions.y - imageDetailsCornerTR.vDims.y - imageDetailsCornerBL.vDims.y);
		}

		// Compute tab position
		if (_mbOrientationIsHorizontal)
		{
			float fTabTotalMovementAmount = _mvDimensions.x - _mfTabDims[0] - imageDetailsCornerTR.vDims.x - imageDetailsCornerBL.vDims.x;
			float fCentrePosOfScrollbar = pContainer->getPosition().x + _mvPosition.x + (_mvDimensions.x * 0.5f) - (_mfTabDims[0] * 0.5f);

			// Convert _mfScrollbarPosition from 0 to 1 to -0.5 to 0.5f
			float fTabOffset = _mfScrollbarPosition - 0.5f;

			// Position offset
			fTabOffset *= fTabTotalMovementAmount;

			_mfTabPos[0] = fCentrePosOfScrollbar + fTabOffset;
			_mfTabPos[1] = pContainer->getPosition().y + _mvPosition.y + imageDetailsCornerBL.vDims.y;
		}
		else // Vertical
		{
			float fTabTotalMovementAmount = _mvDimensions.y - _mfTabDims[1] - imageDetailsCornerTR.vDims.y - imageDetailsCornerBL.vDims.y;
			float fCentrePosOfScrollbar = pContainer->getPosition().y + _mvPosition.y + (_mvDimensions.y * 0.5f) - (_mfTabDims[1] * 0.5f);

			// Convert _mfScrollbarPosition from 0 to 1 to -0.5 to 0.5f
			float fTabOffset = _mfScrollbarPosition - 0.5f;

			// Position offset
			fTabOffset *= fTabTotalMovementAmount;

			_mfTabPos[0] = pContainer->getPosition().x + _mvPosition.x + imageDetailsCornerTR.vDims.x;
			_mfTabPos[1] = fCentrePosOfScrollbar + fTabOffset;
		}

		bool bMouseOver = false;
		if (bAcceptingMouseInput)
		{
			// Determine if mouse cursor is over
			if (vMousePos.x > _mfTabPos[0] - imageDetailsCornerTR.vDims.x)
				if (vMousePos.x < _mfTabPos[0] + _mfTabDims[0] + imageDetailsCornerBL.vDims.x)
					if (vMousePos.y > _mfTabPos[1])
						if (vMousePos.y < _mfTabPos[1] + _mfTabDims[1] + imageDetailsCornerTR.vDims.y)
							bMouseOver = true;
			if (bMouseOver)
			{
				if (x->pInput->mouse.leftButtonOnce())
				{
					_mbTabBeingMoved = true;
				}

				// Move the scrollbar position with keys
				if (x->pInput->key.repeat(KC_LEFT))
					_mfScrollbarPosition -= 0.001f;
				else if (x->pInput->key.repeat(KC_RIGHT))
					_mfScrollbarPosition += 0.001f;
			}

			if (_mbTabBeingMoved)
			{
				if (!x->pInput->mouse.leftButDown())
					_mbTabBeingMoved = false;
				else
				{
					// Move tab position
					// As tab position is in range of 0-1, we need to compute movement based upon mouse delta and scrollbar dims
					CVector2f vMouseDelta = x->pInput->mouse.getMouseDeltaGUI();
					float fTabPosOffset = 0;
					if (_mbOrientationIsHorizontal)
					{
						float fTabTotalMovementAmount = _mvDimensions.x - _mfTabDims[0];
						if (areFloatsEqual(fTabTotalMovementAmount, 0.0f))	// Prevent divide by zero
							fTabTotalMovementAmount = 0.0001f;
						fTabPosOffset += vMouseDelta.x * (1.0f / fTabTotalMovementAmount);
					}
					else
					{
						float fTabTotalMovementAmount = _mvDimensions.y - _mfTabDims[1];
						if (areFloatsEqual(fTabTotalMovementAmount, 0.0f))	// Prevent divide by zero
							fTabTotalMovementAmount = 0.0001f;
						fTabPosOffset += vMouseDelta.y * (1.0f / fTabTotalMovementAmount);
					}
					_mfScrollbarPosition += fTabPosOffset;
					clamp(_mfScrollbarPosition, 0.0f, 1.0f);
				}
			}
		}
		else  // Not accepting mouse clicks
		{

		}

		// Compute tab colour based on whether the mouse is over or not
		float fCol[4];
		fCol[0] = _mTabColour.red;
		fCol[1] = _mTabColour.green;
		fCol[2] = _mTabColour.blue;
		fCol[3] = _mTabColour.alpha;
		float fColTheme[4];
		if (bMouseOver)
		{
			fColTheme[0] = pThemeSettings->colours.scrollbarTabOver.red;
			fColTheme[1] = pThemeSettings->colours.scrollbarTabOver.green;
			fColTheme[2] = pThemeSettings->colours.scrollbarTabOver.blue;
			fColTheme[3] = pThemeSettings->colours.scrollbarTabOver.alpha;

			for (int iCol = 0; iCol < 4; iCol++)
			{
				if (fCol[iCol] < fColTheme[iCol])
				{
					fCol[iCol] += fTimeDeltaSec * pThemeSettings->floats.scrollbarTabFadeSpeed;
					if (fCol[iCol] > fColTheme[iCol])
						fCol[iCol] = fColTheme[iCol];
				}
				else if (_mTabColour.red > fColTheme[iCol])
				{
					fCol[iCol] -= fTimeDeltaSec * pThemeSettings->floats.scrollbarTabFadeSpeed;
					if (fCol[iCol] < fColTheme[iCol])
						fCol[iCol] = fColTheme[iCol];
				}
			}
		}
		else // Mouse not over
		{
			fColTheme[0] = pThemeSettings->colours.scrollbarTabNotOver.red;
			fColTheme[1] = pThemeSettings->colours.scrollbarTabNotOver.green;
			fColTheme[2] = pThemeSettings->colours.scrollbarTabNotOver.blue;
			fColTheme[3] = pThemeSettings->colours.scrollbarTabNotOver.alpha;

			for (int iCol = 0; iCol < 4; iCol++)
			{
				if (fCol[iCol] < fColTheme[iCol])
				{
					fCol[iCol] += fTimeDeltaSec * pThemeSettings->floats.scrollbarTabFadeSpeed;
					if (fCol[iCol] > fColTheme[iCol])
						fCol[iCol] = fColTheme[iCol];
				}
				else if (_mTabColour.red > fColTheme[iCol])
				{
					fCol[iCol] -= fTimeDeltaSec * pThemeSettings->floats.scrollbarTabFadeSpeed;
					if (fCol[iCol] < fColTheme[iCol])
						fCol[iCol] = fColTheme[iCol];
				}
			}
		}
		// Store colour
		_mTabColour.set(fCol[0], fCol[1], fCol[2], fCol[3]);

		// Update this object's tooltip
//		mpTooltip->update(pParentContainer, (CGUIBaseObject*)this, bMouseOver);
	}

	void CUIScrollbar::reset(CUIContainer* pContainer)
	{

	}

	void CUIScrollbar::setTabPos(float fPos)
	{
		clamp(fPos, 0.0f, 1.0f);
		_mfScrollbarPosition = fPos;
	}

	float CUIScrollbar::getTabPos(void) const
	{
		if (_mbOrientationIsHorizontal)
			return _mfScrollbarPosition;
		else
			return 1.0f - _mfScrollbarPosition;	// So that when tab is at bottom, 0 is the result and 1 when at top
	}

	void CUIScrollbar::setTabRatio(float fRatio)
	{
		// Must be less than 1, otherwise the tab can be same width/height as the scrollbar
		// and if this occurs, we could get a divide by zero error in the update method with the following code...
		//float fTabTotalMovementAmount = mfWidth - _mfTabDims[0];
		//fTabPosOffset += vMouseDelta.x * (1.0f / fTabTotalMovementAmount);
		clamp(fRatio, 0.001f, 0.9999f);
		_mfTabRatio = fRatio;
	}

	float CUIScrollbar::getTabRatio(void) const
	{
		return _mfTabRatio;
	}

	bool CUIScrollbar::getOrientationHorizontal(void) const
	{
		return _mbOrientationIsHorizontal;
	}

	CVector2f CUIScrollbar::getTabDims(void) const
	{
		return CVector2f(_mfTabDims[0], _mfTabDims[1]);
	}
}