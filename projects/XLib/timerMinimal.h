#pragma once
#include "PCH.h"

namespace X
{
    // Timing
    // Create an object of this class, keep it around and call update() between the code which you wish to measure the time difference.
    // Then call getSecondsPast() to obtain the time passed since the two calls to update()
	// This class is similar to CTimer, but with less features and is slightly faster to compute.
	// It also returns the time delta as a double instead of float for more precision.
    class CTimerMinimal
    {
    public:
        // Constructor
		CTimerMinimal();

        // Call this to compute time difference between last call to this method.
        void update(void);

        // Returns time passed in seconds since last call to update() method.
        double getSecondsPast(void) const;

        // Resets all values to sane values, including the member set by setAveragedFPSRate()
        // Also, if this timer has been paused, it is unpaused.
        void reset(void);

    private:
        std::chrono::duration<double> _mdTimeDeltaSec;
        std::chrono::time_point<std::chrono::steady_clock> _mdTimePointOld, _mdTimePointNew;
        double _mdDeltaSec;             // Holds time delta since last call to update() method in seconds.
    };
}