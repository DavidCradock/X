#include "PCH.h"
#include "GUIText.h"
#include "GUIManager.h"
#include "resourceManager.h"
#include "window.h"

namespace X
{
	GUIText::GUIText()
	{
		_mbUseThemeColour = true;
	}

	void GUIText::render(void* pParentContainer)
	{
		GUIContainer* pContainer = (GUIContainer*)pParentContainer;
		// Get required resources needed to render
		GUIManager* pGUI = GUIManager::getPointer();
		ResourceManager* pRM = ResourceManager::getPointer();
		Window* pWindow = Window::getPointer();
		GUITheme* pTheme = pGUI->getTheme(pContainer->mstrThemename);
	
		int iRTDims[2];
		iRTDims[0] = int(pWindow->getWidth());
		iRTDims[1] = int(pWindow->getHeight());
		ResourceFont* pFont = pRM->getFont(pTheme->mFonts.button);

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

	void GUIText::setColour(bool bUseDefaultThemeColour, float fRed, float fGreen, float fBlue, float fAlpha)
	{
		_mbUseThemeColour = bUseDefaultThemeColour;
		if (!bUseDefaultThemeColour)
		{
			_mColour.set(fRed, fGreen, fBlue, fAlpha);
		}
	}
}