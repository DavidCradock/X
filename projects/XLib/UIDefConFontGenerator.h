#pragma once
#include "PCH.h"

namespace X
{
	// This container is used to create font files for use with the font resource CResourceFont
	class CUIDefConFontGenerator
	{
	public:
		CUIDefConFontGenerator();

		// Called from CUIDefaultContainers::initialise() to initialise everything
		void initialise(void);

		// Called from CUIDefaultContainers::update() to update everything
		void update(float fTimeDeltaSecs);

	private:
	};
}