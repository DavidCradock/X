#pragma once
#include "PCH.h"
#include "resourceTexture2DAtlas.h"
#include "resourceVertexBufferCPT2.h"
#include "UITheme.h"

namespace X
{
	class CUIContainer;

	class CUIScrollbar
	{
	public:
		CUIScrollbar();
		~CUIScrollbar();

		// Sets the dimensions of the widget.
		void setDimensions(const CVector2f& vDimensions);

		// Returns the dimensions of the widget.
		CVector2f getDimensions(void) const;

		// Sets the position of this widget in relation to it's container.
		void setPosition(const CVector2f& vPosition);

		// Returns the position of the widget in relation to it's container.
		CVector2f getPosition(void) const;

		// Sets whether this widget is visible or not.
		void setVisible(bool bVisible);

		// Returns whether this widget is visible or not.
		bool getVisible(void);

		// Render this object's non-font stuff
		void render(CUIContainer* pContainer, bool bContainerIsWindow, CUITheme* pTheme, CResourceVertexBufferCPT2* pVB);

		// Update this object
		void update(float fTimeDeltaSec, CUIContainer* pContainer, bool bContainerIsWindow, CUITheme* pTheme);

		// Sets tab position, clamped to 0-1 range
		void setTabPos(float fPos);

		// Gets tab position 0-1 range
		float getTabPos(void) const;

		// Set tab ratio.
		// This is multiplied by the scrollbar dimensions(depending upon orientation) to obtain tab dimensions
		void setTabRatio(float fRatio);

		// Returns currently set tab ratio
		// This is multiplied by the scrollbar dimensions(depending upon orientation) to obtain tab dimensions
		float getTabRatio(void) const;

		// Returns scrollbar's orientation (Which is based upon it's dimensions)
		bool getOrientationHorizontal(void) const;

		// Return computed tab dimensions
		CVector2f getTabDims(void) const;

		
	private:
		CVector2f _mvDimensions;			// Dimensions of the widget
		CVector2f _mvPosition;				// Position of the widget in relation to it's container.
		bool _mbVisible;					// Whether this widget is visible or not.

		float _mfScrollbarPosition;			// 0-1 position of scrollbar
		float _mfTabRatio;					// Multiplied by scrollbar dims(depending upon orientation) to obtain tab dimensions
		bool _mbTabBeingMoved;				// Whether the tab is being moved or not
		bool _mbOrientationIsHorizontal;	// Computed in update() and used in render(). Scrollbar orientation based on dimensions
		float _mfTabPos[2];					// Computed in update() and used in render(). Tab position
		float _mfTabDims[2];				// Computed in update() and used in render(). Tab dimensions
		CColour _mTabColour;				// For fading between over/not over colours of tab
	};
}