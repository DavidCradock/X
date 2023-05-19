#include "PCH.h"
#include "log.h"

namespace X
{
	void Log::clear(void)
	{
		std::fstream fs;
		fs.open("log.txt", std::ios::out | std::ios::trunc);
		fs.close();
		mlogEntry.clear();
	}

	void Log::add(const std::string& string, bool bAddNewLine)
	{
		std::fstream fs;
		fs.open("log.txt", std::ios::out | std::ios::app);
		fs << string.c_str();
		if (bAddNewLine)
			fs << "\n";
		fs.close();

		mlogEntry.push_back(string);
	}

	size_t Log::getNumEntries(void)
	{
		return mlogEntry.size();
	}

	std::string Log::getEntryText(size_t entryIndex)
	{
		ThrowIfTrue(entryIndex >= mlogEntry.size(), "Log::getEntryText() with given index of " + std::to_string(entryIndex) + " failed as given index is invalid.");
		return mlogEntry[entryIndex];
	}
}