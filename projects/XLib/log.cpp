#include "PCH.h"
#include "log.h"

namespace X
{
	void SCLog::clear(void)
	{
		std::fstream fs;
		fs.open("log.txt", std::ios::out | std::ios::trunc);
		fs.close();
		_mlogEntry.clear();
	}

	void SCLog::add(const std::string& string, bool bAddNewLine, bool bAddTime)
	{
		std::fstream fs;
		fs.open("log.txt", std::ios::out | std::ios::app);
		if (bAddTime)
		{
			_timer.update();
			float fClockSec;
			int iClockMin, iClockHours, iClockDays, iClockWeeks;
			_timer.getClock(fClockSec, iClockMin, iClockHours, iClockDays, iClockWeeks);
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

		_mlogEntry.push_back(string);
	}

	size_t SCLog::getNumEntries(void) const
	{
		return _mlogEntry.size();
	}

	std::string SCLog::getEntryText(size_t entryIndex) const
	{
		ThrowIfTrue(entryIndex >= _mlogEntry.size(), "SCLog::getEntryText() with given index of " + std::to_string(entryIndex) + " failed as given index is invalid.");
		return _mlogEntry[entryIndex];
	}
}