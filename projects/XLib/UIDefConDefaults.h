#pragma once
#include "PCH.h"

namespace X
{
	// This container holds buttons to toggle visibility of all the other default containers/windows.
	class CUIDefConDefaults
	{
	public:
		CUIDefConDefaults();

		// Called from CUIDefaultContainers::initialise() to initialise everything
		void initialise(void);

		// Called from CUIDefaultContainers::update() to update everything
		void update(float fTimeDeltaSecs);

	private:
	};
}