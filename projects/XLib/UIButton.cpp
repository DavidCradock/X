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

	}

	CUIButton::~CUIButton()
	{

	}

	void CUIButton::setDimensions(const CVector2f& vDimensions)
	{
		_mvDimensions = vDimensions;
	}

	CVector2f CUIButton::getDimensions(void) const
	{
		return _mvDimensions;
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

	bool CUIButton::getVisible(void)
	{
		return _mbVisible;
	}

	void CUIButton::render(CResourceTexture2DAtlas* pAtlas, CUIContainer* pContainer, bool bContainerIsWindow, CUITheme* pTheme, CResourceVertexBufferCPT2* pVB)
	{
		const CUITheme::SSettings* pThemeSettings = pTheme->getSettings();

		// Add geometry for the 9 grid cells
		x->pUI->_addGridGeometry(_mvPosition, _mvDimensions, pThemeSettings->images.buttonBG, pAtlas, pContainer, bContainerIsWindow, pTheme, pVB);

	}

	void CUIButton::update(float fTimeDeltaSec, CResourceTexture2DAtlas* pAtlas, CUIContainer* pContainer, bool bContainerIsWindow, CUITheme* pTheme)
	{
		// Return name of the window, or container which the mouse cursor is over
		std::string strMouseIsOver = x->pUI->getMouseIsOver();
		bool bAcceptingMouseInput = false;
		// And if that window or container is this object's container, we're accepting mouse input
		if (strMouseIsOver == pContainer->getName())
			bAcceptingMouseInput = true;

		CVector2f vMousePos = x->pInput->mouse.getCursorPos();

		// Update this object's tooltip
//		mpTooltip->update(pParentContainer, (CGUIBaseObject*)this, bMouseOver);
	}


}