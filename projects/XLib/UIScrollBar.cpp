#include "PCH.h"
#include "UIScrollBar.h"
#include "utilities.h"
#include "UIContainer.h"
#include "singletons.h"
#include "UITheme.h"
#include "UITooltip.h"
#include "UIManager.h"
#include "input.h"

namespace X
{
	CUIScrollbar::CUIScrollbar(CUIContainer* pContainer)
	{
		_mpContainer = pContainer;
		_mvDimensions = getDimensionsMinimum();
		_mbVisible = true;
		_mfTabPosition = 0.5f;
		_mbTabBeingMoved = false;
		_mfTabRatio = 0.5f;

		_mfuncOnTabBeingMoved = 0;
		_mfuncOnTabMoved = 0;

		pTooltip = new CUITooltip(pContainer);
		ThrowIfMemoryNotAllocated(pTooltip);
	}

	CUIScrollbar::~CUIScrollbar()
	{
		delete pTooltip;
	}

	void CUIScrollbar::setDimensions(float fX, float fY)
	{
		_mvDimensions.x = fX;
		_mvDimensions.y = fY;
		_mpContainer->computeScrollbars();
	}

	void CUIScrollbar::setDimensions(int iX, int iY)
	{
		setDimensions(float(iX), float(iY));
	}

	void CUIScrollbar::setDimensions(const CVector2f& vDimensions)
	{
		setDimensions(vDimensions.x, vDimensions.y);
	}

	CVector2f CUIScrollbar::getDimensions(void) const
	{
		return _mvDimensions;
	}

	CVector2f CUIScrollbar::getDimensionsMinimum(void) const
	{
		CUITheme* pTheme = _mpContainer->themeGet();
		CResourceTexture2DAtlas* pAtlas = pTheme->getTextureAtlas();
		const CUITheme::SSettings* pThemeSettings = _mpContainer->themeGetSettings();

		CVector2f vMinimumDims;
		CImageAtlasDetails* pID = pAtlas->getImageDetailsPointer(pThemeSettings->images.scrollbarBG.colour.cornerTL);
		vMinimumDims = pID->vDims;
		pID = pAtlas->getImageDetailsPointer(pThemeSettings->images.scrollbarBG.colour.cornerBR);
		vMinimumDims += pID->vDims;
		pID = pAtlas->getImageDetailsPointer(pThemeSettings->images.scrollbarBG.colour.edgeT);
		vMinimumDims.x += pID->vDims.x;
		pID = pAtlas->getImageDetailsPointer(pThemeSettings->images.scrollbarBG.colour.edgeR);
		vMinimumDims.y += pID->vDims.y;
		return vMinimumDims;
	}

	void CUIScrollbar::setPosition(float fX, float fY)
	{
		_mvPosition.x = fX;
		_mvPosition.y = fY;
		_mpContainer->computeScrollbars();
	}

	void CUIScrollbar::setPosition(int iX, int iY)
	{
		setPosition(float(iX), float(iY));
	}

	void CUIScrollbar::setPosition(const CVector2f& vPosition)
	{
		setPosition(vPosition.x, vPosition.y);
	}

	CVector2f CUIScrollbar::getPosition(void) const
	{
		return _mvPosition;
	}

	void CUIScrollbar::setVisible(bool bVisible)
	{
		_mbVisible = bVisible;
		if (!_mbVisible)
			reset();
		_mpContainer->computeScrollbars();
	}

	bool CUIScrollbar::getVisible(void) const
	{
		return _mbVisible;
	}

	void CUIScrollbar::renderBG(CResourceVertexBufferCPT2* pVB, bool bUseWidgetScrollbarOffset)
	{
		if (!_mbVisible)
			return;

		CUITheme::SSettings* pThemeSettings = _mpContainer->themeGetSettings();

		if (_mbVisible)
		{
			// Add geometry for the 9 grid cells of the scrollbar's background
			CVector2f vPos = _mvPosition;
			if (bUseWidgetScrollbarOffset)
				vPos += _mpContainer->getWidgetOffset();

			x->pUI->_helperAddWidgetBGGeometry(
				vPos,
				_mvDimensions,
				pThemeSettings->images.scrollbarBG,
				pThemeSettings->colours.scrollbarBG,
				_mpContainer,
				pVB);

			// Add geometry for the 9 grid cells of the scrollbar's tab
			vPos = _mvTabPos;
			if (bUseWidgetScrollbarOffset)
				vPos += _mpContainer->getWidgetOffset();
			x->pUI->_helperAddWidgetBGGeometry(
				vPos,
				_mvTabDims,
				pThemeSettings->images.scrollbarTab,
				_mTabColour,
				_mpContainer,
				pVB);
		}
	}

	void CUIScrollbar::renderTooltip(void)
	{
		pTooltip->render();
	}

	void CUIScrollbar::update(float fTimeDeltaSec, bool bUseWidgetScrollbarOffset)
	{
		if (!_mpContainer->getVisible())
			return;

		// Determine whether this widget's container is accepting mouse input
		bool bAcceptingMouseInput = false;
		// Get name of the window, or container which the mouse cursor is over
		std::string strMouseIsOver = x->pUI->getMouseIsOverWhichContainer();
		// And if that window or container is this object's container, we're accepting mouse input
		if (strMouseIsOver == _mpContainer->getName())
			bAcceptingMouseInput = true;

		// Compute orientation
		bool bOrientationHorizontal = false;
		if (_mvDimensions.x > _mvDimensions.y)
		{
			bOrientationHorizontal = true;
		}

		// Compute tab dims
		if (bOrientationHorizontal)
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
		if (bOrientationHorizontal)
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
		if (bUseWidgetScrollbarOffset)
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
			// Call function pointer if set
			if (_mfuncOnTabBeingMoved)
				_mfuncOnTabBeingMoved();

			if (!x->pInput->mouse.leftButDown())
			{
				_mbTabBeingMoved = false;
				// Call function pointer if set
				if (_mfuncOnTabMoved)
					_mfuncOnTabMoved();
			}
			else
			{
				// Left mouse is down and tab is being moved
			
				// Move tab position
				// As tab position is in range of 0-1, we need to compute movement based upon mouse delta and scrollbar dims
				CVector2f vMouseDelta = x->pInput->mouse.getMouseDeltaGUI();
				float fTabPosOffset = 0;
				if (bOrientationHorizontal)
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

		if (bUseWidgetScrollbarOffset)
			_mvTabPos -= _mpContainer->getWidgetOffset();

		// Convert screen position back to local so that rendering is correct
		_mvTabPos -= _mpContainer->getWidgetAreaTLCornerPosition();

		// Compute tab colour based on whether the mouse is over or not
		if (bMouseOver)
			x->pUI->_helperColourAdjust(_mTabColour, pThemeSettings->colours.scrollbarTabOver, fTimeDeltaSec, pThemeSettings->floats.scrollbarTabFadeSpeed);
		else
			x->pUI->_helperColourAdjust(_mTabColour, pThemeSettings->colours.scrollbarTabNotOver, fTimeDeltaSec, pThemeSettings->floats.scrollbarTabFadeSpeed);

		// Update this widget's tooltip
		pTooltip->update(_mpContainer->getWidgetAreaTLCornerPosition() + _mpContainer->getWidgetOffset() + _mvPosition, _mvDimensions, fTimeDeltaSec);
	}

	void CUIScrollbar::reset(void)
	{
		CUITheme* pTheme = _mpContainer->themeGet();
		const CUITheme::SSettings* pThemeSettings = pTheme->getSettings();
		_mTabColour = pThemeSettings->colours.scrollbarTabNotOver;

		_mbTabBeingMoved = false;

		pTooltip->reset();
	}

	void CUIScrollbar::setTabPos(float fPos)
	{
		clamp(fPos, 0.0f, 1.0f);
		_mfTabPosition = fPos;
	}

	float CUIScrollbar::getTabPos(void) const
	{
		if (_mvDimensions.x > _mvDimensions.y)
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
		return _mvDimensions.x > _mvDimensions.y;
	}

	CVector2f CUIScrollbar::getTabDims(void) const
	{
		return _mvTabDims;
	}

	void CUIScrollbar::setFunctionOnTabBeingMoved(void (*function)(void))
	{
		_mfuncOnTabBeingMoved = function;
	}

	void CUIScrollbar::setFunctionOnTabMoved(void (*function)(void))
	{
		_mfuncOnTabMoved = function;
	}

	bool CUIScrollbar::getTabBeingMoved(void) const
	{
		return _mbTabBeingMoved;
	}
}