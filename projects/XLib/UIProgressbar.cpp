#include "PCH.h"
#include "UIProgressbar.h"
#include "utilities.h"
#include "UIContainer.h"
#include "singletons.h"
#include "UITheme.h"

namespace X
{
	CUIProgressbar::CUIProgressbar(CUIContainer* pContainer)
	{
		_mpContainer = pContainer;
		_mvDimensions.set(200, 48);
		_mbVisible = true;
		_mfProgress = 0.5f;
	}

	CUIProgressbar::~CUIProgressbar()
	{

	}

	void CUIProgressbar::setDimensions(float fX, float fY)
	{
		_mvDimensions.x = fX;
		_mvDimensions.y = fY;
		_mpContainer->computeScrollbars();
	}

	void CUIProgressbar::setDimensions(const CVector2f& vDimensions)
	{
		_mvDimensions = vDimensions;
		_mpContainer->computeScrollbars();
	}

	CVector2f CUIProgressbar::getDimensions(void) const
	{
		return _mvDimensions;
	}

	void CUIProgressbar::setPosition(float fX, float fY)
	{
		_mvPosition.x = fX;
		_mvPosition.y = fY;
		_mpContainer->computeScrollbars();
	}

	void CUIProgressbar::setPosition(const CVector2f& vPosition)
	{
		_mvPosition = vPosition;
		_mpContainer->computeScrollbars();
	}

	CVector2f CUIProgressbar::getPosition(void) const
	{
		return _mvPosition;
	}

	void CUIProgressbar::setVisible(bool bVisible)
	{
		_mbVisible = bVisible;
		_mpContainer->computeScrollbars();
	}

	bool CUIProgressbar::getVisible(void) const
	{
		return _mbVisible;
	}

	void CUIProgressbar::render(CResourceVertexBufferCPT2* pVB)
	{
		const CUITheme::SSettings* pThemeSettings = _mpContainer->themeGetSettings();

		// Add geometry for the 9 grid cells
		x->pUI->_helperAddWidgetGridGeometry(
			_mvPosition + _mpContainer->getWidgetOffset(),
			_mvDimensions,
			pThemeSettings->images.progressBarBG,
			pThemeSettings->colours.progressBarBG,
			_mpContainer,
			pVB);
			
		// Compute filler dimensions.
		CVector2f vDims = _mvDimensions;
		if (_mvDimensions.x > _mvDimensions.y)	// Horizontal
		{
			vDims.x *= _mfProgress;
		}
		else // Vertical
		{
			vDims.y *= _mfProgress;
		}

		// Add geometry for the 9 grid cells
		x->pUI->_helperAddWidgetGridGeometry(
			_mvPosition + _mpContainer->getWidgetOffset(),
			vDims,
			pThemeSettings->images.progressBarFiller,
			pThemeSettings->colours.progressBarFiller,
			_mpContainer,
			pVB);
	}


	/******************************************************************* Widget specific *******************************************************************/

	void CUIProgressbar::setProgress(float fProgress)
	{
		_mfProgress = fProgress;
		clamp(_mfProgress, 0.0f, 1.0f);
	}

	float CUIProgressbar::getProgress(void) const
	{
		return _mfProgress;
	}
}