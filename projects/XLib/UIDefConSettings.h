#pragma once
#include "PCH.h"

namespace X
{
	// This allows modification of all the settings contained within the global x->pSettings object
	class CUIDefConSettings
	{
	public:
		CUIDefConSettings();

		// Called from CUIDefaultContainers::initialise() to initialise everything
		void initialise(void);

		// Called from CUIDefaultContainers::update() to update everything
		void update(float fTimeDeltaSecs);

	private:
	};
}