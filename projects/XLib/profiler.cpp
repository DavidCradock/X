#include "PCH.h"
#include "profiler.h"
#include "logging.h"
#include "singletons.h"		// For printResults()
#include "stringUtils.h"	// For printResults()

namespace X
{
	CProfiler::CProfiler()
	{
		reset();
	}

	void CProfiler::reset(void)
	{
		// Remove all sections
		_mmapSections.clear();

		_mfUpdateResultsText = 0.0f;
	}

	void CProfiler::begin(const std::string& strSectionName)
	{
		// Add named section if it doesn't exist.
		auto it = _mmapSections.find(strSectionName);
		if (_mmapSections.end() == it)
		{
			SSection newSection;
			newSection.dAccumulatedTimeInSeconds = 0;
			newSection.iBeginCalledCount = 0;
			_mmapSections[strSectionName] = newSection;
			it = _mmapSections.find(strSectionName);
		}

		// If this section is "main", then reset all sections
		if ("main" == strSectionName)
		{
			auto it2 = _mmapSections.begin();
			while (it2 != _mmapSections.end())
			{
				it2->second.dAccumulatedTimeInSeconds = 0;
				it2->second.iBeginCalledCount = 0;
				it2++;
			}
		}

		it->second.timer.update();
		it->second.iBeginCalledCount++;
		if (it->second.iBeginCalledCount == 1)
			it->second.dAccumulatedTimeInSeconds = 0;
	}

	void CProfiler::end(const std::string& strSectionName)
	{
		// If named section doesn't exist, throw an exception.
		auto it = _mmapSections.find(strSectionName);
		ThrowIfTrue(_mmapSections.end() == it, "CProfiler::end(\"" + strSectionName + "\") failed. The named section doesn't exist.");

		// Update this section's accumulated time
		it->second.timer.update();
		it->second.dAccumulatedTimeInSeconds += it->second.timer.getSecondsPast();

		// If this section is "main", update the cached results.
		if ("main" == strSectionName)
		{
			_mvResultsCached.clear();
			_mvResultsCached = getResults(false);
		}
	}

	double CProfiler::getSectionTime(const std::string& strSectionName)
	{
		// If named section doesn't exist, throw an exception.
		auto it = _mmapSections.find(strSectionName);
		ThrowIfTrue(_mmapSections.end() == it, "CProfiler::getSectionTime(\"" + strSectionName + "\") failed. The named section doesn't exist.");
		return it->second.dAccumulatedTimeInSeconds;
	}

	size_t CProfiler::getSectionNumber(void)
	{
		return _mmapSections.size();
	}

	bool sortByVal(const std::pair<std::string, CProfiler::SSection>& a, const std::pair<std::string, CProfiler::SSection>& b)
	{
		return (a.second.dAccumulatedTimeInSeconds > b.second.dAccumulatedTimeInSeconds);
	}

	std::vector<SProfilerResults> CProfiler::getResults(bool bReturnCachedResults)
	{
		if (bReturnCachedResults)
		{
			return _mvResultsCached;
		}

		// Will hold the final results
		std::vector<SProfilerResults> vResults;

		// Create a vector of pairs
		std::vector<std::pair<std::string, CProfiler::SSection>> vec;

		// Copy key-value pairs from the map to the vector
		std::map<std::string, CProfiler::SSection> ::iterator it;
		for (it = _mmapSections.begin(); it != _mmapSections.end(); it++)
		{
			vec.push_back(make_pair(it->first, it->second));
		}

		// Sort the vector by increasing order of its pair's second value's dAccumulatedTimeInSeconds value.
		sort(vec.begin(), vec.end(), sortByVal);

		// Find main and get it's accumulated time
		auto itMain = _mmapSections.find("main");
		ThrowIfTrue(_mmapSections.end() == itMain, "CProfiler::getResults() failed. The \"main\" section doesn't exist.");
		double dMainTime = itMain->second.dAccumulatedTimeInSeconds;
		
		// Prevent divide by zero error below.
		if (dMainTime <= 0)
			dMainTime = 0.00001;

		// Now copy vector to vector and return
		for (size_t i = 0; i < vec.size(); i++)
		{
			SProfilerResults results;
			results.dAccumulatedTimeSeconds = vec[i].second.dAccumulatedTimeInSeconds;
			results.fPercentageOfMain = float(results.dAccumulatedTimeSeconds / dMainTime);
			results.fPercentageOfMain *= 100;
			results.strSectionName = vec[i].first;
			vResults.push_back(results);
		}
		return vResults;
	}

	void CProfiler::printResults(void)
	{
		CResourceFont* pFont = x->pResource->getFont(x->pResource->defaultRes.font_default);
		float fTextHeight = pFont->getTextHeight() + 2;

		// Update text string vector with results every so often
		_mTimerMinimal.update();
		_mfUpdateResultsText -= (float)_mTimerMinimal.getSecondsPast();
		if (_mfUpdateResultsText <= 0.0f)
		{
			_mfUpdateResultsText = 1.0f;
			_mvecstrResultsText.clear();

			// Get sorted vector of results
			std::vector<SProfilerResults> vResults = getResults();
			_mvecstrResultsText.push_back("CProfiler::printResults()");
			for (size_t i = 0; i < vResults.size(); i++)
			{
				std::string strTxt;
				StringUtils::appendFloat(strTxt, vResults[i].fPercentageOfMain, 1);
				strTxt += "% of main. (";
				StringUtils::appendDouble(strTxt, 1000 * vResults[i].dAccumulatedTimeSeconds, 4);
				strTxt += ") ms. \"";
				strTxt += vResults[i].strSectionName;
				strTxt += "\"";
				_mvecstrResultsText.push_back(strTxt);
			}
		}

		CVector2f vTextPos(5, 5);
		for (size_t i = 0; i < _mvecstrResultsText.size(); i++)
		{
			pFont->print(_mvecstrResultsText[i], int(vTextPos.x), int(vTextPos.y), x->pWindow->getWidth(), x->pWindow->getHeight());
			vTextPos.y += fTextHeight;
		}
	}
}