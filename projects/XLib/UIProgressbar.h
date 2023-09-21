#pragma once
#include "PCH.h"
#include "resourceTexture2DAtlas.h"
#include "resourceVertexBufferCPT2.h"
#include "UITheme.h"

namespace X
{
	class CUIContainer;
	class CUITooltip;

	// A progress bar
	class CUIProgressbar
	{
		friend class CUIContainer;
	public:
		CUIProgressbar(CUIContainer* pContainer);
		~CUIProgressbar();

		// Sets the dimensions of the widget.
		void setDimensions(float fX, float fY);

		// Sets the dimensions of the widget.
		void setDimensions(int iX, int iY);

		// Sets the dimensions of the widget.
		void setDimensions(const CVector2f& vDimensions);

		// Returns the dimensions of the widget.
		CVector2f getDimensions(void) const;

		// Returns the minimum dimensions of this widget based upon the container's currently set theme.
		CVector2f getDimensionsMinimum(void) const;

		// Sets the position of this widget in relation to it's container.
		// Both axis should be at least zero.
		void setPosition(float fX, float fY);

		// Sets the position of this widget in relation to it's container.
		// Both axis should be at least zero.
		void setPosition(int iX, int iY);

		// Sets the position of this widget in relation to it's container.
		// Both axis should be at least zero.
		void setPosition(const CVector2f& vPosition);

		// Returns the position of the widget in relation to it's container.
		CVector2f getPosition(void) const;

		// Sets whether this widget is visible or not.
		void setVisible(bool bVisible);

		// Returns whether this widget is visible or not.
		bool getVisible(void) const;

		// Render this widget
		void renderBG(CResourceVertexBufferCPT2* pVB);

		// Render this widget's tooltip
		void renderTooltip(void);

		// Update this widget and it's tooltip
		void update(float fTimeDeltaSec);

		// Resets all colours and time based values for the widget, including tooltip
		void reset(void);

		// Sets progress 0-1 range
		void setProgress(float fProgress);

		// Gets progress 0-1 range
		float getProgress(void) const;

		// Tooltip object for this widget.
		// Access and use this object to setup and enable the tooltip.
		CUITooltip* pTooltip;
	private:
		// Common amoung widgets
		CVector2f _mvDimensions;			// Dimensions of the widget
		CVector2f _mvPosition;				// Position of the widget in relation to it's container.
		bool _mbVisible;					// Whether this widget is visible or not.
		CUIContainer* _mpContainer;			// The container this widget belongs to. Set in constructor

		// Widget specific
		float _mfProgress;	// 0-1 position of progress bar
	};
}