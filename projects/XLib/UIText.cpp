#include "PCH.h"
#include "UIText.h"
#include "utilities.h"
#include "UIContainer.h"
#include "singletons.h"
#include "UITheme.h"

namespace X
{
	CUIText::CUIText(CUIContainer* pContainer, const std::string& strWidgetName)
	{
		_mpContainer = pContainer;
		_mvDimensions.set(100, 100);
		_mbVisible = true;

		_mbFramebufferNeedsUpdating = true;
		_mstrFramebufferName = "CUITextCont_" + pContainer->getName() + "Widget_" + strWidgetName;
		x->pResource->addFramebuffer(_mstrFramebufferName, 100, 100, false);
		setTextColour(true);
		setBackgroundColour();
		setFont(true);

		// Centering
		_mbCentered = false;
		_mvNonCenteredPos = _mvPosition;
		_mvNonCenteredDims = _mvDimensions;
	}

	CUIText::~CUIText()
	{
		x->pResource->removeFramebuffer(_mstrFramebufferName);
	}

	void CUIText::setDimensions(float fX, float fY)
	{
		_mvDimensions.x = fX;
		_mvDimensions.y = fY;
		_mvNonCenteredDims = _mvDimensions;
		_helperComputeCenteredPosDims();
		_mbFramebufferNeedsUpdating = true;
		_mpContainer->computeScrollbars();
	}

	void CUIText::setDimensions(const CVector2f& vDimensions)
	{
		_mvDimensions = vDimensions;
		_mvNonCenteredDims = _mvDimensions;
		_helperComputeCenteredPosDims();
		_mbFramebufferNeedsUpdating = true;
		_mpContainer->computeScrollbars();
	}

	CVector2f CUIText::getDimensions(void) const
	{
		return _mvDimensions;
	}

	void CUIText::setPosition(float fX, float fY)
	{
		_mvPosition.x = fX;
		_mvPosition.y = fY;
		_mvNonCenteredPos = _mvPosition;
		_helperComputeCenteredPosDims();
		_mbFramebufferNeedsUpdating = true;
		_mpContainer->computeScrollbars();
	}

	void CUIText::setPosition(const CVector2f& vPosition)
	{
		_mvPosition = vPosition;
		_mvNonCenteredPos = _mvPosition;
		_helperComputeCenteredPosDims();
		_mbFramebufferNeedsUpdating = true;
		_mpContainer->computeScrollbars();
	}

	CVector2f CUIText::getPosition(void) const
	{
		return _mvPosition;
	}

	void CUIText::setVisible(bool bVisible)
	{
		_mbVisible = bVisible;
		_mpContainer->computeScrollbars();
	}

	bool CUIText::getVisible(void) const
	{
		return _mbVisible;
	}

	void CUIText::render(void)
	{
		// Get pointer to this widget's frame buffer
		CResourceFramebuffer* pFramebuffer = x->pResource->getFramebuffer(_mstrFramebufferName);

		// Update the frame buffer if needed.
		if (_mbFramebufferNeedsUpdating)
		{
			_mbFramebufferNeedsUpdating = false;
			// Resize framebuffer if needed
			CVector2f vFBDims = pFramebuffer->getDimensions();
			if (vFBDims != _mvDimensions)
			{
				pFramebuffer->resize(unsigned int(_mvDimensions.x), unsigned int(_mvDimensions.y));
			}

			// Render text to framebuffer
			const CUITheme::SSettings* pThemeSettings = _mpContainer->themeGetSettings();
			// Get font resource
			CResourceFont* pFont;
			if (_mbUseThemeFont)
				pFont = x->pResource->getFont(pThemeSettings->fonts.text);
			else
				pFont = x->pResource->getFont(_mstrFontResourceName);

			glDisable(GL_SCISSOR_TEST);
			pFramebuffer->bindAsRenderTarget(true, false, _mColourFramebufferClear);

			// Render normally
			if (!_mbCentered)
			{
				std::vector<std::string> vstrTemp;
				int iTotalRenderedHeight;
				pFont->printInRectNewline(true, _mstrText, 0, 0, (int)_mvDimensions.x, (int)_mvDimensions.y, vstrTemp, iTotalRenderedHeight, "\n", 1.0f, _mColourText);
			}
			else // Render centered (Although we're rendering to framebuffer, so render at 0,0
			{
				pFont->print(_mstrText, 0, 0, (int)_mvDimensions.x, (int)_mvDimensions.y, 1.0f, _mColourText);
			}
			glEnable(GL_SCISSOR_TEST);
			pFramebuffer->unbindAsRenderTarget();

			// Rebind the UI framebuffer
			CResourceFramebuffer* pUIFB = x->pResource->getFramebuffer(x->pResource->defaultRes.framebuffer_ui);
			pUIFB->bindAsRenderTarget(false);
		}

		// Compute position
		CVector2f vTextPos;
		vTextPos = _mpContainer->getWidgetAreaTLCornerPosition();
		vTextPos += _mvPosition;
		vTextPos += _mpContainer->getWidgetOffset();

		glEnable(GL_BLEND);
		pFramebuffer->renderTo2DQuad((int)vTextPos.x, (int)vTextPos.y, (int)_mvDimensions.x, (int)_mvDimensions.y);
		glDisable(GL_BLEND);
	}

	/******************************************************************* Widget specific *******************************************************************/

	void CUIText::setText(const std::string& strText)
	{
		_mstrText = strText;
		_helperComputeCenteredPosDims();
		_mbFramebufferNeedsUpdating = true;
	}

	std::string CUIText::getText(void) const
	{
		return _mstrText;
	}

	void CUIText::setTextColour(bool bUseThemeColour, const CColour& colour)
	{
		_mbUseThemeColour = bUseThemeColour;
		_mColourText = colour;
		_mbFramebufferNeedsUpdating = true;
	}

	void CUIText::setBackgroundColour(const CColour& colour)
	{
		_mColourFramebufferClear = colour;
	}

	void CUIText::setFont(bool bUseThemeFont, const std::string& strFontResourceToUse)
	{
		_mbUseThemeFont = bUseThemeFont;
		_mstrFontResourceName = strFontResourceToUse;
		_helperComputeCenteredPosDims();
		_mbFramebufferNeedsUpdating = true;

		if (!bUseThemeFont)
			ThrowIfFalse(x->pResource->getFontExists(strFontResourceToUse), "CUIText::setFont(\"" + strFontResourceToUse + "\") failed. The named font resource does not exist.");
	}

	void CUIText::setCentered(bool bCenterTheText)
	{
		_mbCentered = bCenterTheText;
		_helperComputeCenteredPosDims();
		_mbFramebufferNeedsUpdating = true;
	}

	void CUIText::_helperComputeCenteredPosDims(void)
	{
		if (_mbCentered)
		{
			CResourceFont* pFont;
			if (_mbUseThemeFont)
				pFont = x->pResource->getFont(_mpContainer->themeGetSettings()->fonts.text);
			else
				pFont = x->pResource->getFont(_mstrFontResourceName);
			CVector2f vTextDims;
			vTextDims.x = pFont->getTextWidth(_mstrText);
			vTextDims.y = pFont->getTextHeight();
			_mvDimensions = vTextDims;
			vTextDims *= 0.5f;
			_mvPosition = _mvNonCenteredPos - vTextDims;
		}
		else// Restore values to original ones
		{
			_mvPosition = _mvNonCenteredPos;
			_mvDimensions = _mvNonCenteredDims;
		}

	}

	void CUIText::setFramebufferNeedsUpdating(void)
	{
		_mbFramebufferNeedsUpdating = true;
	}
}