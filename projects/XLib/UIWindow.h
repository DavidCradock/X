#pragma once
#include "PCH.h"
#include "UIContainer.h"

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
	private:
		void _renderBorders(void);

		// Whether this window is in focus or not.
		bool _mbInFocus;

	};
}