#pragma once
#include "PCH.h"

namespace X
{
	// This is a text based console for logging and entering commands
	class CUIDefConConsole
	{
	public:
		CUIDefConConsole();

		// Called from CUIDefaultContainers::initialise() to initialise everything
		void initialise(void);

		// Called from CUIDefaultContainers::update() to update everything
		void update(float fTimeDeltaSecs);

	private:
	};
}