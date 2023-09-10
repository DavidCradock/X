#pragma once
#include "PCH.h"
//#include "resourceTexture2DAtlas.h"
//#include "resourceVertexBufferCPT2.h"
#include "UITheme.h"

namespace X
{
	class CUIContainer;

	// A 2 axis line graph
	// A dataset has to be added to store all values (y axis) which are plotted against the x axis
	// Only one dataset is required and is represented by a line on the graph, however, multiple datasets can exist (multiple lines)
	class CUILineGraph
	{
		friend class CUIContainer;
	public:
		CUILineGraph(CUIContainer* pContainer);
		~CUILineGraph();

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
		void render(void);

	
		// Update this widget
		void update(float fTimeDeltaSec);

		/******************************************************************* Widget specific *******************************************************************/

	private:
		// Common amoung widgets
		CVector2f _mvDimensions;			// Dimensions of the widget
		CVector2f _mvPosition;				// Position of the widget in relation to it's container.
		bool _mbVisible;					// Whether this widget is visible or not.
		CUIContainer* _mpContainer;			// The container this widget belongs to. Set in constructor

		// Widget specific

	};
}