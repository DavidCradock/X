#pragma once
#include "PCH.h"
#include "singleton.h"
#include "utilities.h"
#include "timer.h"
#include "openGLExtensions.h"

namespace X
{
	// Gets thrown via the ThrowIfFalse/True macros below
	class CException
	{
	public:
		CException(std::string strExceptionText, std::string strFilename, int iLineNumber)
		{
			mstrException = "File: " + strFilename + "\n";
			mstrException += "Line: " + std::to_string(iLineNumber) + "\n";
			mstrException += "Desc: " + strExceptionText + "\n";
			mstrException += "Please click the OK button to close.";
		}
		std::string mstrException;
	};
	// Macro to check a bool and if false, throw an exception which adds filename, line number and the given text.
	#ifndef ThrowIfFalse
	#define ThrowIfFalse(x, y)							\
	{                                                   \
		if(!x){											\
			__debugbreak();								\
			throw CException(y, __FILE__, __LINE__);}	\
	}
	#endif
	// Macro to check a bool and is true, throw an exception which adds filename, line number and the given text.
	#ifndef ThrowIfTrue
	#define ThrowIfTrue(x, y)                           \
	{                                                   \
		if(x){											\
			__debugbreak();								\
			throw CException(y, __FILE__, __LINE__);}	\
	}
	#endif

	// Logging of information to "log.txt" file and storage of each added log entry into memory
	// Call clear() on application startup, otherwise "log.txt" will be appended to, holding previous run's logging.
	class CLog : public CSingleton<CLog>
	{
	public:
		// Clear the log file and all log entries in memory
		void clear(void);

		// Add text to the log file and entries in memory
		void add(const std::string& string, bool bAddNewLine = true, bool bAddTime = true);

		// Return number of entries added via add()
		size_t getNumEntries(void) const;

		// Returns a string for an entry.
		// If entryIndex is out of bounds, an exception occurs
		std::string getEntryText(size_t entryIndex) const;
	private:
		std::vector<std::string> mlogEntry;	// A vector of strings, to hold each entry added by add() method
		CTimer timer;						// For adding time to the beginning of each entry
	};
}