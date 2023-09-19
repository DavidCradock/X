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

		/************************************************************************************************************************************************************/
		/* Text */
		/************************************************************************************************************************************************************/

		// Add text to this tooltip and return a pointer to it
		// If the name already exists, an exception occurs
		CUIText* textAdd(const std::string& strName, float fPosX, float fPosY, float fWidth, float fHeight, const std::string& strText);

		// Add text to this tooltip and return a pointer to it
		// If the name already exists, an exception occurs
		CUIText* textAdd(const std::string& strName, int iPosX, int iPosY, int iWidth, int iHeight, const std::string& strText);

		// Returns a pointer to the named object
		// If the object doesn't exist, an exception occurs
		CUIText* textGet(const std::string& strName);

		// Removes the named object from this tooltip
		// If the named object doesn't exist, this silently fails
		void textRemove(const std::string& strName);

		// Removes all text from this tooltip
		void textRemoveAll(void);

	private:
		CVector2f _mvDimensions;			// Dimensions of the tooltip
		CVector2f _mvPosition;				// Top left corner position of the tooltip.
		bool _mbEnabled;					// Whether this tooltip is enabled or not.
		CUIContainer* _mpContainer;			// The container this tooltip belongs to. Set in constructor

		float _mfTooltipDelay;				// Used to prevent tooltip from fading in until SCSettings::getUITooltipDelaySeconds() has passed
		CColour _mColour;					// Current colour of the tooltip, used for fading in/out

		std::map<std::string, CUIText*> _mmapTexts;

		// Computes _mvDimensions based upon all the widgets added and their positions.
		void _computeDimensions(void);
	};
}