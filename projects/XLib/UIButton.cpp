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

	void CUIButton::render(CUIContainer* pContainer, bool bContainerIsWindow, CUITheme* pTheme, CResourceVertexBufferCPT2* pVB)
	{
		const CUITheme::SSettings* pThemeSettings = pTheme->getSettings();
//		CResourceTexture2DAtlas* pAtlas = pTheme->getTextureAtlas();

		// Add geometry for the 9 grid cells
		x->pUI->_addGridGeometry(_mvPosition, _mvDimensions, pThemeSettings->images.buttonBG, pContainer, bContainerIsWindow, pTheme, pVB);

	}

	void CUIButton::renderFonts(CUIContainer* pContainer, bool bContainerIsWindow, CUITheme* pTheme)
	{
		const CUITheme::SSettings* pThemeSettings = pTheme->getSettings();
		CResourceFont* pFont = x->pResource->getFont(pThemeSettings->fonts.button);
		
		CColour colour;
		colour = pThemeSettings->colours.buttonBGUp;
		CVector2f vTextPos = pContainer->getPosition();
		vTextPos += _mvPosition;
		vTextPos.x += _mvDimensions.x * 0.5f;
		vTextPos.y += _mvDimensions.y * 0.5f;
		if (bContainerIsWindow)
		{
			CResourceTexture2DAtlas* pAtlas = pTheme->getTextureAtlas();
			CImageAtlasDetails idTL = pAtlas->getImageDetails(pThemeSettings->images.windowBG.colour.cornerTL);
			vTextPos += idTL.vDims;
		}
		pFont->printCentered(_mstrText, (int)vTextPos.x, (int)vTextPos.y, x->pWindow->getWidth(), x->pWindow->getHeight(), 1.0f, colour);
	}

	void CUIButton::update(float fTimeDeltaSec, CUIContainer* pContainer, bool bContainerIsWindow, CUITheme* pTheme)
	{
		// Return name of the window, or container which the mouse cursor is over
		std::string strMouseIsOver = x->pUI->getMouseIsOver();
		bool bAcceptingMouseInput = false;
		// And if that window or container is this object's container, we're accepting mouse input
		if (strMouseIsOver == pContainer->getName())
			bAcceptingMouseInput = true;

		CVector2f vMousePos = x->pInput->mouse.getCursorPos();

	//	CResourceTexture2DAtlas* pAtlas = pTheme->getTextureAtlas();

		// Update this object's tooltip
//		mpTooltip->update(pParentContainer, (CGUIBaseObject*)this, bMouseOver);
	}

	void CUIButton::setText(const std::string& strText)
	{
		_mstrText = strText;
	}

	std::string CUIButton::getText(void) const
	{
		return _mstrText;
	}
}