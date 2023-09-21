#pragma once
#include "PCH.h"
#include "vector2f.h"
#include "colour.h"
#include "UIText.h"

namespace X
{
	class CUIContainer;

	// Hovering over widgets may show a tooltip.
	// A tooltip is typically used to show additional information about the widget which is being hovered over.
	// A tooltip can hold many different types of widgets such as text, images etc.
	// Each widget has a tooltip and by default, it is disabled and will not be shown.
	// To enable a tooltip for a widget, access that widget object's tooltip and call setEnabled(true);
	class CUITooltip
	{
	public:
		CUITooltip(CUIContainer* pContainer);
		~CUITooltip();

		// Called from CUIContainer::renderTooltips() to render this object
		void render(void);

		// Called from each widget's update method to update this object
		void update(const CVector2f& vWidgetPosition, const CVector2f& vWidgetDims, float fTimeDeltaSec);

		// Enable or disable this tooltip from being rendered
		void setEnabled(bool bEnabled);

		// Returns whether this tooltip is enabled or not
		bool getEnabled(void) const;

		// Resets tooltip. Usually called when a container is set as non-visible
		void reset(void);

		// Sets the text to be displayed in the tooltip
		void setText(const std::string& strText, bool bEnable = true, int iMaxWidth = 300);
	private:
		CVector2f _mvDimensions;			// Dimensions of the tooltip
		CVector2f _mvPosition;				// Top left corner position of the tooltip.
		bool _mbEnabled;					// Whether this tooltip is enabled or not.
		CUIContainer* _mpContainer;			// The container this tooltip belongs to. Set in constructor

		float _mfTooltipDelay;				// Used to prevent tooltip from fading in until SCSettings::getUITooltipDelaySeconds() has passed
		CColour _mColour;					// Current colour of the tooltip, used for fading in/out

		std::string _mstrText;				// Text string used to render the font
		int _miTextMaxWidth;				// Maximum width of the rendered text. Set with setText()

		// Computes _mvDimensions based upon all the widgets added and their positions.
		void _computeDimensions(void);
	};
}