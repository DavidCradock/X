#pragma once
#include "PCH.h"
#include "colour.h"
#include "UIImage.h"

namespace X
{
	// Contains functionality to be able to edit CUIThemes
	class CUIDefConUIThemeEditor
	{
	public:
		CUIDefConUIThemeEditor();

		// Called from CUIDefaultContainers::initialise() to initialise everything
		void initialise(void);

		// Called from CUIDefaultContainers::update() to update everything
		void update(float fTimeDeltaSecs);

		CColour* pCurrentlySelectedThemeColour;
		CUIImage* pCurrentlySelectedThemeColourImageWidget;
	private:
		void _initMainWindow(void);
		void _initHelpWindow(void);
		void _initWidgetPreviewWindow(void);

		void _colourImageAdd(const std::string& strName, int& iPositionY, CColour& colour, void (*function)(void));
	};
}