#pragma once
#include "PCH.h"
#include "resourceTexture2DAtlas.h"
#include "resourceVertexBufferCPT2.h"
#include "UITheme.h"

namespace X
{
	class CUIContainer;

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
		void setDimensions(const CVector2f& vDimensions);

		// Returns the dimensions of the widget.
		CVector2f getDimensions(void) const;

		// Sets the position of this widget in relation to it's container.
		// Both axis should be at least zero. I wish there was an unsigned float, but oh well :)
		void setPosition(float fX, float fY);

		// Sets the position of this widget in relation to it's container.
		// Both axis should be at least zero. I wish there was an unsigned float, but oh well :)
		void setPosition(const CVector2f& vPosition);

		// Returns the position of the widget in relation to it's container.
		CVector2f getPosition(void) const;

		// Sets whether this widget is visible or not.
		void setVisible(bool bVisible);

		// Returns whether this widget is visible or not.
		bool getVisible(void) const;

		// Render this widget
		void render(CResourceVertexBufferCPT2* pVB);

		/******************************************************************* Widget specific *******************************************************************/

		// Sets progress 0-1 range
		void setProgress(float fProgress);

		// Gets progress 0-1 range
		float getProgress(void) const;
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