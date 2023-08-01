#include "PCH.h"
#include "GUIText.h"
#include "GUIManager.h"
#include "resourceManager.h"
#include "window.h"
#include "GUITooltip.h"
#include "input.h"

namespace X
{
	CGUIText::CGUIText()
	{
		_mbUseThemeColour = true;
		mpTooltip = new CGUITooltip;
	}

	CGUIText::~CGUIText()
	{
		delete mpTooltip;
	}

	void CGUIText::render(void* pParentContainer)
	{
		CGUIContainer* pContainer = (CGUIContainer*)pParentContainer;
		SCGUIManager* pGUI = SCGUIManager::getPointer();
		SCResourceManager* pRM = SCResourceManager::getPointer();
		SCWindow* pWindow = SCWindow::getPointer();
		CGUITheme* pTheme = pContainer->getTheme();

		int iRTDims[2];
		iRTDims[0] = int(pWindow->getWidth());
		iRTDims[1] = int(pWindow->getHeight());
		CResourceFont* pFont = pRM->getFont(pTheme->mFonts.text);

		CColour col;
		if (_mbUseThemeColour)
		{
			col = pTheme->mColours.text;
		}
		else
		{
			col = _mColour;
		}
		pFont->print(_mstrText,			// The text
			int(pContainer->mfPositionX + mfPositionX),		// X position
			int(pContainer->mfPositionY + mfPositionY),		// Y position
			iRTDims[0], iRTDims[1],	// Render target dims
			1.0f,	// Scaling
			col);	// Colour
	}

	void CGUIText::renderForTooltip(void* pParentContainer, unsigned int uiTooltipFramebufferWidth, unsigned int uiTooltipFramebufferHeight)
	{
		CGUIContainer* pContainer = (CGUIContainer*)pParentContainer;
		SCGUIManager* pGUI = SCGUIManager::getPointer();
		SCResourceManager* pRM = SCResourceManager::getPointer();
		CGUITheme* pTheme = pContainer->getTheme();

		int iRTDims[2];
		iRTDims[0] = int(uiTooltipFramebufferWidth);
		iRTDims[1] = int(uiTooltipFramebufferHeight);
		CResourceFont* pFont = pRM->getFont(pTheme->mFonts.text);

		CColour col;
		if (_mbUseThemeColour)
		{
			col = pTheme->mColours.tooltipText;
		}
		else
		{
			col = _mColour;
		}
		std::vector<std::string> vecLines;
		int iHeight;
		pFont->printInRectNewline(true, _mstrText, (int)mfPositionX, (int)mfPositionY, iRTDims[0], iRTDims[1], vecLines, iHeight, "\n", 1.0f, col);
	}

	void CGUIText::update(void* pParentContainer, bool bParentContainerAcceptingMouseClicks)
	{
		// Update this object's tooltip
		SCInputManager* pInput = SCInputManager::getPointer();
		CVector2f vMousePos = pInput->mouse.getCursorPos();
		CGUIContainer* pContainer = (CGUIContainer*)pParentContainer;
		bool bMouseOver = false;
		if (bParentContainerAcceptingMouseClicks)
		{
			// Determine if mouse cursor is over
			if (vMousePos.x > pContainer->mfPositionX + mfPositionX)
				if (vMousePos.x < pContainer->mfPositionX + mfPositionX + mfWidth)
					if (vMousePos.y > pContainer->mfPositionY + mfPositionY)
						if (vMousePos.y < pContainer->mfPositionY + mfPositionY + mfHeight)
							bMouseOver = true;
		}
		mpTooltip->update(pParentContainer, (CGUIBaseObject*)this, bMouseOver);
	}

	void CGUIText::setText(const std::string& strText)
	{
		_mstrText = strText;
	}

	std::string CGUIText::getText(void) const
	{
		return _mstrText;
	}

	void CGUIText::setColour(bool bUseDefaultThemeColour, const CColour& colour)
	{
		_mbUseThemeColour = bUseDefaultThemeColour;
		if (!bUseDefaultThemeColour)
		{
			_mColour = colour;
		}
	}
}