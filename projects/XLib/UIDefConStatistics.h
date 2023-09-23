#pragma once
#include "PCH.h"
#include "timer.h"

namespace X
{
	// This shows basic information such as frames per second, averaged and displayed in a line graph.
	class CUIDefConStatistics
	{
	public:
		CUIDefConStatistics();

		// Called from CUIDefaultContainers::initialise() to initialise everything
		void initialise(void);

		// Called from CUIDefaultContainers::update() to update everything
		void update(float fTimeDeltaSecs);

	private:
		float fAddValueToLinegraphDataset;	// Time counter used to add a new value to the data set.
		CTimer timer;
		bool bvsyncEnabled;					// Used for comparison and only update text when value has changed
	};
}