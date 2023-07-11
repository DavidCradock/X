#include "PCH.h"
#include "log.h"

namespace X
{
	void CLog::clear(void)
	{
		std::fstream fs;
		fs.open("log.txt", std::ios::out | std::ios::trunc);
		fs.close();
		mlogEntry.clear();
	}

	void CLog::add(const std::string& string, bool bAddNewLine, bool bAddTime)
	{
		std::fstream fs;
		fs.open("log.txt", std::ios::out | std::ios::app);
		if (bAddTime)
		{
			timer.update();
			float fClockSec;
			int iClockMin, iClockHours, iClockDays, iClockWeeks;
			timer.getClock(fClockSec, iClockMin, iClockHours, iClockDays, iClockWeeks);
			std::string strClock;
			strClock += std::to_string(iClockWeeks) + "w:";
			strClock += std::to_string(iClockDays) + "d:";
			strClock += std::to_string(iClockHours) + "h:";
			strClock += std::to_string(iClockMin) + "m:";
			if (fClockSec < 10.0f)
				strClock += "0";
			strClock += std::to_string(int(fClockSec)) + "s : ";
			fs << strClock.c_str();
		}
		fs << string.c_str();
		if (bAddNewLine)
			fs << "\n";
		fs.close();

		mlogEntry.push_back(string);
	}

	size_t CLog::getNumEntries(void) const
	{
		return mlogEntry.size();
	}

	std::string CLog::getEntryText(size_t entryIndex) const
	{
		ThrowIfTrue(entryIndex >= mlogEntry.size(), "CLog::getEntryText() with given index of " + std::to_string(entryIndex) + " failed as given index is invalid.");
		return mlogEntry[entryIndex];
	}
}