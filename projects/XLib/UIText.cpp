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
		setColour(true);
	}

	CUIText::~CUIText()
	{
		x->pResource->removeFramebuffer(_mstrFramebufferName);
	}

	void CUIText::setDimensions(float fX, float fY)
	{
		_mvDimensions.x = fX;
		_mvDimensions.y = fY;
		_mbFramebufferNeedsUpdating = true;
		_mpContainer->computeScrollbars();
	}

	void CUIText::setDimensions(const CVector2f& vDimensions)
	{
		_mvDimensions = vDimensions;
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
		_mpContainer->computeScrollbars();
	}

	void CUIText::setPosition(const CVector2f& vPosition)
	{
		_mvPosition = vPosition;
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
			CResourceFont* pFont = x->pResource->getFont(pThemeSettings->fonts.text);
			pFramebuffer->bindAsRenderTarget(true);
			std::vector<std::string> vstrTemp;
			int iTotalRenderedHeight;
			glDisable(GL_SCISSOR_TEST);
			pFont->printInRectNewline(true, _mstrText, 0, 0, _mvDimensions.x, _mvDimensions.y, vstrTemp, iTotalRenderedHeight, "\n", 1.0f, _mColour);
			glEnable(GL_SCISSOR_TEST);
			pFramebuffer->unbindAsRenderTarget();
		}

		// Compute position
		CVector2f vTextPos = _mpContainer->getWidgetAreaTLCornerPosition();
		vTextPos += _mvPosition;
		vTextPos += _mpContainer->getWidgetOffset();

		glEnable(GL_BLEND);
		pFramebuffer->renderTo2DQuad(vTextPos.x, vTextPos.y, _mvDimensions.x, _mvDimensions.y);
		glDisable(GL_BLEND);
	}

	/******************************************************************* Widget specific *******************************************************************/

	void CUIText::setText(const std::string& strText)
	{
		_mstrText = strText;
		_mbFramebufferNeedsUpdating = true;
	}

	std::string CUIText::getText(void) const
	{
		return _mstrText;
	}

	void CUIText::setColour(bool bUseThemesColour, const CColour& colour)
	{
		_mbUseThemeColour = bUseThemesColour;
		_mColour = colour;
		_mbFramebufferNeedsUpdating = true;
	}
}