#pragma once
#include "PCH.h"
#include "GUIBaseObject.h"
#include "GUIColour.h"

namespace X
{
	class GUIText : public GUIBaseObject
	{
	public:
		GUIText();

		// Called from GUIContainer to render this object
		void render(void* pParentContainer);

		std::string mstrText;	// Text to be rendered

		// Sets the colour to use for this text, or use the theme colour
		void setColour(bool bUseDefaultThemeColour = false, float fRed = 1.0f, float fGreen = 1.0f, float fBlue = 1.0f, float fAlpha = 1.0f);
	private:

		bool _mbUseThemeColour;
		GUIColour _mColour;
	};
}