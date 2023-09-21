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
		// This does NOT represent actual screen position.
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

		// Centres the window on the screen
		void setPositionCentre(void);

		// Sets this window to the foreground, aka top most and in focus
		void setToFrontAndInFocus(void);

		// Sets whether this window can be resized or not.
		// By default, window resizing is disabled.
		void setResizable(bool bCanBeResized, float fMinWidth, float fMinHeight, float fMaxWidth, float fMaxHeight);

		// Sets whether this window can be resized or not.
		// By default, window resizing is disabled.
		void setResizable(bool bCanBeResized, const CVector2f& vMinDims, const CVector2f& vMaxDims);

		// Returns whether this window is set to be resizable
		bool getResizable(void) const;

		// Sets given CVector2fs to min and max dims of the resizable window
		// which were set with setResizable()
		void getResizableDims(CVector2f& vMinDims, CVector2f& vMaxDims) const;

		// Sets whether this window shows the close button in the top right of the titlebar or not.
		// By default, this is enabled.
		void showCloseButton(bool bShowCloseButton = true);
	private:
		void _renderBordersAndWindowButtons(void);

		// Whether this window is in focus or not.
		bool _mbInFocus;

		// The text string used when printing the titlebar text.
		// Initially, this is set to the name of the window by the manager.
		std::string _mstrTitlebarText;

		// Called from SCUIManager::onToggleFullscreen()
		void _onToggleFullscreen(void);

		// Resizable members
		bool _mbIsResizable;		// Whether this window can be resized or not (Default is false)
		CVector2f _mvResizeMinDims;	// Minimum dimensions this window can be resized to if _mbIsResizable == true
		CVector2f _mvResizeMaxDims;	// Maximum dimensions this window can be resized to if _mbIsResizable == true

		// Used by getMouseOverResizableArea().
		enum EWindowArea
		{
			mouseOverCornerBL,
			mouseOverCornerBR,
			mouseOverCornerTL,
			mouseOverCornerTR,
			mouseOverEdgeB,
			mouseOverEdgeL,
			mouseOverEdgeR,
			mouseOverEdgeT,
			mouseOverNone
		};

		// Used to detect whether we could resize this window in SCUIManager::_update()
		// If the mouse cursor is over one of the 8 areas which could toggle window resizing,
		// returns which area the mouse is over in the form of an EWindowArea enumeration.
		// If the mouse cursor isn't over any of the areas, returns EWindowArea::mouseOverNone.
		// _computeResizableAreas is called from here
		EWindowArea _getMouseOverResizableArea(void);

		// Stores computed screen positions of each area as a rect and stores in _mrctResizeArea[9]
		void _computeResizableAreas(void);

		// Resizable areas of the window, computed in _computeResizableAreas()
		// Use EWindowArea to access correct area
		CRect _mrctResizeArea[9];

		// Debug rendering of computed areas in _mrctResizeArea
		void _debugRenderAreas(void);

		bool _mbShowCloseButton;	// Whether this window has a close button or not
		CUIButton* _mpButtonClose;	// The close button
	};
}