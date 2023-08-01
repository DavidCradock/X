#include "PCH.h"
#include "timer.h"

namespace X
{
    CTimer::CTimer()
    {
        reset();
    }

    void CTimer::pause(void)
    {
        mbPaused = true;
    }

    void CTimer::resume(void)
    {
        mbPaused = false;
    }

    float CTimer::getSecondsPast(void) const
    {
        if (mbPaused)
            return 0.0f;
        return (float)mdDeltaSec;
    }

    void CTimer::update(void)
    {
        mdTimePointNew = std::chrono::steady_clock::now();
        mdTimeDeltaSec = mdTimePointNew - mdTimePointOld;
        mdTimePointOld = mdTimePointNew;// std::chrono::steady_clock::now();

        mdDeltaSec = mdTimeDeltaSec.count();

        // Compute FPS
        ++muiNumFrames;
        mdFPSFrameTime += mdDeltaSec * 1000.0;
        if (mdFPSFrameTime > 0.0)
        {
            mdFPS = muiNumFrames * (1000.0 / mdFPSFrameTime);
            
            muiNumFrames = 0;
            mdFPSFrameTime = 0.0;
        }

        // Compute FPS smoothed
        mdFPSAveragedTimeCount += mdDeltaSec;
        ++miFPSAveragedNumCallsPerSec;
        mdFPSAveragedAccum += mdFPS;
        if (mdFPSAveragedTimeCount > mdFPSAveragedRate)
        {
            if (miFPSAveragedNumCallsPerSec < 1)
                miFPSAveragedNumCallsPerSec = 1;
            mdFPSAveraged = mdFPSAveragedAccum / miFPSAveragedNumCallsPerSec;
            mdFPSAveragedTimeCount = 0;
            miFPSAveragedNumCallsPerSec = 0;
            mdFPSAveragedAccum = 0;
        }

        // Runtime
        mdRuntimeInSeconds += mdDeltaSec;
    }

    void CTimer::sleep(unsigned int uiMilliseconds) const
    {
        Sleep(uiMilliseconds);
    }

    void CTimer::setAveragedFPSRate(float fSecondsBetweenUpdates)
    {
        mdFPSAveragedRate = (double)fSecondsBetweenUpdates;
        if (mdFPSAveragedRate <= 0)
            mdFPSAveragedRate = 0.001;
    }

    float CTimer::getAveragedFPSRate(void) const
    {
        return (float)mdFPSAveragedRate;
    }

    float CTimer::getFPS(void) const
    {
         return (float)mdFPS;
    }

    float CTimer::getFPSAveraged(void) const
    {
        return (float)mdFPSAveraged;
    }

    void CTimer::reset(void)
    {
        mbPaused = false;
        mdTimePointNew = std::chrono::steady_clock::now();
        mdTimePointOld = mdTimePointNew;// std::chrono::steady_clock::now();
        mdTimeDeltaSec = mdTimePointNew - mdTimePointOld;
        mdDeltaSec = mdTimeDeltaSec.count();

        // Stuff for FPS
        mdFPS = 1.0;                // Holds computed current frames per second value
        muiNumFrames = 0;           // Used to compute FPS stuff        
        mdFPSFrameTime = 0;         // Used to compute FPS stuff
        
        // Stuff for FPS averaged
        mdFPSAveraged = 1.0;                    // Holds computed averaged frames per second value
        mdFPSAveragedRate = 1.0;                // Number of times per second in which is the delay until updating the value returned by getFPSAveraged() method.
        mdFPSAveragedTimeCount = 0;             // Used to compute FPSAveraged stuff
        miFPSAveragedNumCallsPerSec = 0;        // Used to compute FPSAveraged stuff
        mdFPSAveragedAccum = 0;                 // Used to compute FPSAveraged stuff

        mdRuntimeInSeconds = 0;
    }

    float CTimer::getRuntimeSeconds(void) const
    { 
        return (float)mdRuntimeInSeconds;
    }

    void CTimer::getClock(float& fSeconds, int& iMinutes, int& iHours, int& iDays, int &iWeeks) const
    {
        fSeconds = 0.0f;
        iMinutes = 0;
        iHours = 0;
        iDays = 0;
        iWeeks = 0;

        // How many seconds are in a week?
        // 60seconds in a minute, 60 minutes in an hour, 24 hours in a day and 7 days in a week.
        // 60 * 60 = 3600 seconds per hour
        // 3600 * 24 = 86400 seconds in a day
        // 86400 * 7 = 604800 seconds in a week
        double seconds = mdRuntimeInSeconds;
        while (seconds >= 604800)   // Weeks
        {
            seconds -= 604800;
            iWeeks++;
        }
        while (seconds >= 86400)    // Days
        {
            seconds -= 86400;
            iDays++;
        }
        while (seconds >= 3600)     // Hours
        {
            seconds -= 3600;
            iHours++;
        }
        while (seconds >= 60)       // Minutes
        {
            seconds -= 60;
            iMinutes++;
        }
        fSeconds = (float)seconds;  // Seconds
    }

    std::string CTimer::getClock(void) const
    {
        float fSecs = 0.0f;
        int iMins, iHours, iDays, iWeeks = 0;
        getClock(fSecs, iMins, iHours, iDays, iWeeks);
        std::string strRuntime = std::to_string((int)fSecs) + "sec ";
        strRuntime += std::to_string(iMins) + "min ";
        strRuntime += std::to_string(iHours) + "hr ";
        strRuntime += std::to_string(iDays) + "days ";
        strRuntime += std::to_string(iWeeks) + "weeks.";
        return strRuntime;
    }
}