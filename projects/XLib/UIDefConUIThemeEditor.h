#pragma once
#include "PCH.h"

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

	private:
	};
}