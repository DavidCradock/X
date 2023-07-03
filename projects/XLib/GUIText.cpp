#include "PCH.h"
#include "GUIText.h"
#include "GUIManager.h"
#include "resourceManager.h"
#include "window.h"
#include "GUITooltip.h"
#include "input.h"

namespace X
{
	GUIText::GUIText()
	{
		_mbUseThemeColour = true;
		mpTooltip = new GUITooltip;
	}

	GUIText::~GUIText()
	{
		delete mpTooltip;
	}

	void GUIText::render(void* pParentContainer)
	{
		GUIContainer* pContainer = (GUIContainer*)pParentContainer;
		GUIManager* pGUI = GUIManager::getPointer();
		ResourceManager* pRM = ResourceManager::getPointer();
		Window* pWindow = Window::getPointer();
		GUITheme* pTheme = pGUI->getTheme(pContainer->mstrThemename);
	
		int iRTDims[2];
		iRTDims[0] = int(pWindow->getWidth());
		iRTDims[1] = int(pWindow->getHeight());
		ResourceFont* pFont = pRM->getFont(pTheme->mFonts.text);

		GUIColour col;
		if (_mbUseThemeColour)
		{
			col = pTheme->mColours.text;
		}
		else
		{
			col = _mColour;
		}
		pFont->print(mstrText,			// The text
			int(pContainer->mfPositionX + mfPositionX),		// X position
			int(pContainer->mfPositionY + mfPositionY),		// Y position
			iRTDims[0], iRTDims[1],	// Render target dims
			1.0f,	// Scaling
			col.get());	// Colour
	}

	void GUIText::renderForTooltip(void* pParentContainer, unsigned int uiTooltipFramebufferWidth, unsigned int uiTooltipFramebufferHeight)
	{
		GUIContainer* pContainer = (GUIContainer*)pParentContainer;
		GUIManager* pGUI = GUIManager::getPointer();
		ResourceManager* pRM = ResourceManager::getPointer();
		GUITheme* pTheme = pGUI->getTheme(pContainer->mstrThemename);

		int iRTDims[2];
		iRTDims[0] = int(uiTooltipFramebufferWidth);
		iRTDims[1] = int(uiTooltipFramebufferHeight);
		ResourceFont* pFont = pRM->getFont(pTheme->mFonts.text);

		GUIColour col;
		if (_mbUseThemeColour)
		{
			col = pTheme->mColours.text;
		}
		else
		{
			col = _mColour;
		}
		pFont->print(mstrText,		// The text
			int(mfPositionX),		// X position
			int(mfPositionY),		// Y position
			iRTDims[0], iRTDims[1],	// Render target dims
			1.0f,	// Scaling
			col.get());	// Colour
	}

	// Called from GUIContainer to update this object
	void GUIText::update(void* pParentContainer, bool bParentContainerAcceptingMouseClicks)
	{
		// Update this object's tooltip
		InputManager* pInput = InputManager::getPointer();
		glm::vec2 vMousePos = pInput->mouse.getCursorPos();
		GUIContainer* pContainer = (GUIContainer*)pParentContainer;
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
		GUITooltip* pTooltip = (GUITooltip*)mpTooltip;
		pTooltip->update(pParentContainer, (GUIBaseObject*)this, bMouseOver);
	}


	void GUIText::setColour(bool bUseDefaultThemeColour, float fRed, float fGreen, float fBlue, float fAlpha)
	{
		_mbUseThemeColour = bUseDefaultThemeColour;
		if (!bUseDefaultThemeColour)
		{
			_mColour.set(fRed, fGreen, fBlue, fAlpha);
		}
	}
}