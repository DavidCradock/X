#pragma once
#include "PCH.h"
#include "UIContainer.h"
#include "rect.h"

namespace X
{
	// A window object for the user interface.
	// A window is a CUIContainer object with the addition of borders, titlebar and titlebar text
	// and can be dragged around the screen.
	class CUIWindow : public CUIContainer
	{
		friend class SCUIManager;
	public:
		CUIWindow();

		~CUIWindow();

		// Updates this container
		void update(float fTimeDeltaSec);

		// Render this container and each of it's objects
		void render(void);

		// Returns the dims of this window (not just the widget area),
		// including the borders of this window's currently set theme.
		CVector2f getDimsIncludingTheme(void);

		// Returns a CRect which represents the area of the window's titlebar
		CRect getTitlebarArea(void);

		// Returns the string of text used when printing the titlebar text.
		// Initially, this is set to the name of the window by the manager.
		// Use setTitlebarText() to change this.
		std::string getTitlebarText(void);

		// Sets the string of text used when printing the titlebar text.
		// Initially, this is set to the name of the window by the manager.
		void setTitlebarText(const std::string& strTitlebarText);
	private:
		void _renderBorders(void);

		// Whether this window is in focus or not.
		bool _mbInFocus;

		// The text string used when printing the titlebar text.
		// Initially, this is set to the name of the window by the manager.
		std::string _mstrTitlebarText;

	};
}