#pragma once
#include "PCH.h"

namespace X
{
	// This window renders the profiling information from the global x->pProfiler object.
	class CUIDefConProfiling
	{
	public:
		CUIDefConProfiling();
	
		// Called from CUIDefaultContainers::initialise() to initialise everything
		void initialise(void);

		// Called from CUIDefaultContainers::update() to update everything
		void update(float fTimeDeltaSecs);

	private:
		float fUpdateDelaySeconds;		// Number of seconds between updating of text
		float fUpdateDelay;				// When reached zero, we update the text
		bool bPaused;
	};
}