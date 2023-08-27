#include "PCH.h"
#include "UIBaseWidget.h"

namespace X
{
	CUIBaseWidget::CUIBaseWidget()
	{
		setPosition(CVector2f(0, 0));
		setDimensions(CVector2f(128, 128));
		setVisible(true);
	}

	void CUIBaseWidget::setPosition(const CVector2f& vPosition)
	{
		_mvPosition = vPosition;
	}

	CVector2f CUIBaseWidget::getPosition(void) const
	{
		return _mvPosition;
	}

	void CUIBaseWidget::setDimensions(const CVector2f& vDimensions)
	{
		_mvDimensions = vDimensions;
	}

	CVector2f CUIBaseWidget::getDimensions(void) const
	{
		return _mvDimensions;
	}

	void CUIBaseWidget::setVisible(bool bVisible)
	{
		_mbVisible = bVisible;
	}

	bool CUIBaseWidget::getVisible(void) const
	{
		return _mbVisible;
	}
}