#pragma once
#include "PCH.h"
#include "timerMinimal.h"

namespace X
{
	// Used by CProfiler class to return a vector of this struct containing the profiler's results
	struct SProfilerResults
	{
		// The section's name
		std::string strSectionName;

		// The total accumulated time in seconds
		double dAccumulatedTimeSeconds;

		// The percentage of the "main" section that this section takes up.
		float fPercentageOfMain;
	};

	// A class to deal with time profiling.
	// It works on the concept of code sections.
	// We create an object of this class (Or use the global one stored in x->pProfiler) and then call
	// begin("nameOfSection") before any code we wish to time.
	// Then after all the section code, we call
	// end("nameOfSection")
	// If elsewhere in the code, we wish to time a section and add that time to the named section we've already used,
	// we add another begin()/end() pair and the time taken between those will be added to that code sections total time.
	// For everything to be computed properly, we MUST have a section called "main" which encapsulates all other sections.
	// This will allow the class to compute percentages each section takes.
	// Example:
	// CProfiler prof;
	// void mainLoop(void)
	// {
	//   prof.begin("main");
	//   prof.begin("audio");
	//	 // Audio code goes here
	//	 prof.end("audio");
	//   prof.begin("gfx");
	//	 // Rendering code goes here
	//	 prof.end("gfx");
	//   prof.end("main");
	// }
	class CProfiler
	{
	public:
		CProfiler();

		// Begin timing a named section of code
		// There must be at least one section called "main" for things to work correctly.
		void begin(const std::string& strSectionName = "main");

		// End timing a named section of code
		void end(const std::string& strSectionName=  "main");

		// Resets the profiler, removing all previously used sections, except for "main"
		void reset(void);

		// Returns the number of accumulated seconds for the named section.
		// If the section doesn't exist, an exception occurs.
		double getSectionTime(const std::string& strSectionName = "main");

		// Returns the total number of sections including "main"
		size_t getSectionNumber(void);

		// Returns a vector of SProfilerResults, sorted by accumulated time in seconds.
		// Unfortunately, this only returns valid results after a call to end() with the "main" section specified has been called,
		// otherwise we don't have all the needed information we need to compute the percentages of main.
		// However, passing true to bReturnCachedResults will return the results from results which have been obtained during the
		// last call to end() with "main" being given as the section. These results are one frame old, but are complete.
		std::vector<SProfilerResults> getResults(bool bReturnCachedResults = true);

		// Renders the results of the profiler to the screen using x->pResource->defaultRes.font_default
		// It's crude and dirty but will suffice until the user interface is implemented and then we can
		// create a CUIWindow which displays the results in a much better way. TODO
		void printResults(void);

		// Members needed by each code section.
		struct SSection
		{
			// Minimal timer object used to time this section
			CTimerMinimal timer;

			// Total accumulated time since call or calls between begin and end
			double dAccumulatedTimeInSeconds;

			// Number of times begin() has been called for this named section since
			// begin() was called on "main" section.
			int iBeginCalledCount;
		};
	private:
		// Holds each named section including "main"
		std::map<std::string, SSection> _mmapSections;
		
		// Count until update _mstrResultsText.
		float _mfUpdateResultsText;

		// Holds results text to be printed to the screen.
		std::vector<std::string> _mvecstrResultsText;

		CTimerMinimal _mTimerMinimal;

		// This holds the cached results which are created upon a call to end() with "main" being the named section
		std::vector<SProfilerResults> _mvResultsCached;
	};
}