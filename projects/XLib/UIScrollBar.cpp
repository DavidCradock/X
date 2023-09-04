#include "PCH.h"
#include "UIScrollBar.h"
#include "utilities.h"
#include "UIContainer.h"
#include "singletons.h"
#include "UITheme.h"

namespace X
{
	CUIScrollbar::CUIScrollbar(CUIContainer* pContainer)
	{
		_mpContainer = pContainer;
		_mvDimensions.set(200, 48);
		_mbVisible = true;
		_mfTabPosition = 0.5f;
		_mbTabBeingMoved = false;
		_mfTabRatio = 0.5f;
	}

	CUIScrollbar::~CUIScrollbar()
	{

	}

	void CUIScrollbar::setDimensions(float fX, float fY)
	{
		_mvDimensions.x = fX;
		_mvDimensions.y = fY;
		_mpContainer->_computeScrollbars();

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
		_mpContainer->_computeScrollbars();
	}

	void CUIScrollbar::setPosition(const CVector2f& vPosition)
	{
		_mvPosition = vPosition;
		_mpContainer->_computeScrollbars();
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

	void CUIScrollbar::render(CResourceVertexBufferCPT2* pVB)
	{
		const CUITheme::SSettings* pThemeSettings = _mpContainer->themeGetSettings();

		// Add geometry for the 9 grid cells of the scrollbar's background
		if (_mbVisible)
		{
			x->pUI->_helperAddWidgetGridGeometry(
				_mvPosition + _mpContainer->getWidgetOffset(),
				_mvDimensions,
				pThemeSettings->images.scrollbarBG,
				pThemeSettings->colours.scrollbarBG,
				_mpContainer,
				pVB);

			// Add geometry for the 9 grid cells of the scrollbar's tab
			x->pUI->_helperAddWidgetGridGeometry(
				_mvTabPos + _mpContainer->getWidgetOffset(),
				_mvTabDims,
				pThemeSettings->images.scrollbarTab,
				_mTabColour,
				_mpContainer,
				pVB);
		}
	}

	void CUIScrollbar::update(float fTimeDeltaSec)
	{
		// Determine whether this widget's container is accepting mouse input
		bool bAcceptingMouseInput = false;
		// Get name of the window, or container which the mouse cursor is over
		std::string strMouseIsOver = x->pUI->getMouseIsOverWhichContainer();
		// And if that window or container is this object's container, we're accepting mouse input
		if (strMouseIsOver == _mpContainer->getName())
			bAcceptingMouseInput = true;

		// Compute orientation
		_mbOrientationIsHorizontal = false;
		if (_mvDimensions.x > _mvDimensions.y)
		{
			_mbOrientationIsHorizontal = true;
		}

		// Compute tab dims
		if (_mbOrientationIsHorizontal)
		{
			_mvTabDims.x = _mfTabRatio * _mvDimensions.x;
			_mvTabDims.y = _mvDimensions.y;
		}
		else
		{
			_mvTabDims.x = _mvDimensions.x;
			_mvTabDims.y = _mfTabRatio * _mvDimensions.y;
		}
 
		// Compute tab position
		CUITheme* pTheme = _mpContainer->themeGet();
		const CUITheme::SSettings* pThemeSettings = pTheme->getSettings();
		if (_mbOrientationIsHorizontal)
		{
			float fTabTotalMovementAmount = _mvDimensions.x - _mvTabDims.x;		// Total amount of movement possible
			float fCentrePosOfScrollbar = _mpContainer->getWidgetAreaTLCornerPosition().x + _mvPosition.x + (_mvDimensions.x * 0.5f) - (_mvTabDims.x * 0.5f);
			float fTabOffset = _mfTabPosition - 0.5f;	// Convert from 0 to 1 to -0.5 to 0.5f
			fTabOffset *= fTabTotalMovementAmount;		// Position offset
			_mvTabPos.x = fCentrePosOfScrollbar + fTabOffset;
			_mvTabPos.y = _mpContainer->getWidgetAreaTLCornerPosition().y + _mvPosition.y;
		}
		else // Vertical
		{
			float fTabTotalMovementAmount = _mvDimensions.y - _mvTabDims.y;		// Total amount of movement possible
			float fCentrePosOfScrollbar = _mpContainer->getWidgetAreaTLCornerPosition().y + _mvPosition.y + (_mvDimensions.y * 0.5f) - (_mvTabDims.y * 0.5f);
			float fTabOffset = _mfTabPosition - 0.5f;	// Convert from 0 to 1 to -0.5 to 0.5f
			fTabOffset *= fTabTotalMovementAmount;		// Position offset
			_mvTabPos.x = _mpContainer->getWidgetAreaTLCornerPosition().x + _mvPosition.x;
			_mvTabPos.y = fCentrePosOfScrollbar + fTabOffset;
		}

		// _mvTabPos now holds screen position (not including _mpContainer->getWidgetOffset())
		_mvTabPos += _mpContainer->getWidgetOffset();

		// We'll convert this back to position relative to container once all calculations
		// have been performed, so that rendering of the thing is correct, near the bottom of this method.

		// Set _mbTabBeingMoved to true if container is accepting input, mouse is over and mouse is clicked
		bool bMouseOver = false;
		if (bAcceptingMouseInput)
		{
			CVector2f vMousePos = x->pInput->mouse.getCursorPos();
			CRect rctTab;
			rctTab.miMinX = (int)_mvTabPos.x;
			rctTab.miMaxX = (int)_mvTabPos.x + (int)_mvTabDims.x;
			rctTab.miMinY = (int)_mvTabPos.y;
			rctTab.miMaxY = (int)_mvTabPos.y + (int)_mvTabDims.y;

			// Determine if mouse cursor is over
			if (rctTab.doesPositionFitWithin(vMousePos))
				bMouseOver = true;
			if (bMouseOver)
			{
				if (x->pInput->mouse.leftButtonOnce())
				{
					_mbTabBeingMoved = true;
				}
			}
		}

		// Regardless of whether container is accepting input, or mouse is over, but only if
		// the tab has been set to being moved
		if (_mbTabBeingMoved)
		{
			if (!x->pInput->mouse.leftButDown())
				_mbTabBeingMoved = false;
			else
			{
				// Left mouse is down and tab is being moved
			
				// Move tab position
				// As tab position is in range of 0-1, we need to compute movement based upon mouse delta and scrollbar dims
				CVector2f vMouseDelta = x->pInput->mouse.getMouseDeltaGUI();
				float fTabPosOffset = 0;
				if (_mbOrientationIsHorizontal)
				{
					float fTabTotalMovementAmount = _mvDimensions.x - _mvTabDims.x;
					if (areFloatsEqual(fTabTotalMovementAmount, 0.0f))	// Prevent divide by zero
						fTabTotalMovementAmount = 0.0001f;
					fTabPosOffset += vMouseDelta.x * (1.0f / fTabTotalMovementAmount);

					// Offset tab position (should be in range of 0 to 1) by the computed offset
					_mfTabPosition += fTabPosOffset;

					// If the newly computed tab position is out of range, limit mouse movement
					if (_mfTabPosition < 0.0f || _mfTabPosition > 1.0f)
					{
						CVector2f vMousePos = x->pInput->mouse.getCursorPos();
						vMousePos -= x->pInput->mouse.getMouseDeltaGUI();
						x->pInput->mouse.setMousePos(vMousePos);
						clamp(_mfTabPosition, 0.0f, 1.0f);
					}
				}
				else
				{
					float fTabTotalMovementAmount = _mvDimensions.y - _mvTabDims.x;
					if (areFloatsEqual(fTabTotalMovementAmount, 0.0f))	// Prevent divide by zero
						fTabTotalMovementAmount = 0.0001f;
					fTabPosOffset += vMouseDelta.y * (1.0f / fTabTotalMovementAmount);

					// Offset tab position (should be in range of 0 to 1) by the computed offset
					_mfTabPosition += fTabPosOffset;

					// If the newly computed tab position is out of range, limit mouse movement
					if (_mfTabPosition < 0.0f || _mfTabPosition > 1.0f)
					{
						CVector2f vMousePos = x->pInput->mouse.getCursorPos();
						vMousePos -= x->pInput->mouse.getMouseDeltaGUI();
						x->pInput->mouse.setMousePos(vMousePos);
						clamp(_mfTabPosition, 0.0f, 1.0f);
					}
				}
			}
		}	// Tab is being moved

		_mvTabPos -= _mpContainer->getWidgetOffset();

		// Convert screen position back to local so that rendering is correct
		_mvTabPos -= _mpContainer->getWidgetAreaTLCornerPosition();

		// Compute tab colour based on whether the mouse is over or not
		if (bMouseOver)
			x->pUI->_helperColourAdjust(_mTabColour, pThemeSettings->colours.scrollbarTabOver, fTimeDeltaSec, pThemeSettings->floats.scrollbarTabFadeSpeed);
		else
			x->pUI->_helperColourAdjust(_mTabColour, pThemeSettings->colours.scrollbarTabNotOver, fTimeDeltaSec, pThemeSettings->floats.scrollbarTabFadeSpeed);

		// Update this object's tooltip
//		mpTooltip->update(pParentContainer, (CGUIBaseObject*)this, bMouseOver);
	}

	void CUIScrollbar::reset(void)
	{
		CUITheme* pTheme = _mpContainer->themeGet();
		const CUITheme::SSettings* pThemeSettings = pTheme->getSettings();
		_mTabColour = pThemeSettings->colours.scrollbarTabNotOver;

		_mbTabBeingMoved = false;
	}

	void CUIScrollbar::setTabPos(float fPos)
	{
		clamp(fPos, 0.0f, 1.0f);
		_mfTabPosition = fPos;
	}

	float CUIScrollbar::getTabPos(void) const
	{
		if (_mbOrientationIsHorizontal)
			return _mfTabPosition;
		else
			return 1.0f - _mfTabPosition;	// So that when tab is at bottom, 0 is the result and 1 when at top
	}

	void CUIScrollbar::setTabRatio(float fRatio)
	{
		// Must be less than 1, otherwise the tab can be same width/height as the scrollbar
		// and if this occurs, we could get a divide by zero error
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
		return _mvTabDims;
	}
}