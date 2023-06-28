#pragma once
#include "PCH.h"
#include "GUIBaseObject.h"
#include "timer.h"
#include "GUIColour.h"

namespace X
{
	// Data set used by GUILineGraph object.
	// It holds values used to render the line graph
	class GUILineGraphDataSet
	{
	public:
		std::vector<float> values;		// Each of the values stored in this data set
		GUIColour colour;				// The colour used to render this data set as a line on a graph

		// Add new value to the data set
		void addValue(float fValue);

		// Remove oldest entry value
		void removeValue(void);

		// Get number of values currently in the set
		unsigned int getNumValues(void);

		// Returns highest value in dataset
		float getHighestValue(void);

		// Returns loweset value in dataset
		float getLowestValue(void);

	private:
	};

	// A 2 axis line graph
	// A dataset has to be added to store all values (y axis) which are plotted against the x axis
	// Only one dataset is required and is represented by a line on the graph, however, multiple datasets can exist (multiple lines)
	class GUILineGraph : public GUIBaseObject
	{
	public:

		// Called from GUIContainer to render this object
		void render(void* pParentContainer, const std::string& strFramebufferToSampleFrom);

		// Called from GUIContainer to update this object
		void update(void* pParentContainer, bool bParentContainerAcceptingMouseClicks);

		// Add a new named data set to this graph
		// If the named set already exists, an exception occurs
		// Returns a pointer to the newly added data set
		GUILineGraphDataSet* addDataset(const std::string& strName, const GUIColour& cCol);

		// Returns a pointer to a previously added data set
		// If the data set couldn't be found, an exception occurs
		GUILineGraphDataSet* getDataset(const std::string& strName);

		// Removes a previously added data set from the graph
		// If the named data set doesn't exist, this silently fails
		void removeDataset(const std::string& strName);

		// Returns total number of data sets
		unsigned int getNumDatasets(void);

		// Returns the name of the data set at given index.
		// If given index is invalid, an exception occurs
		std::string getDatasetName(unsigned int iIndex);
	private:
		Timer _mTimer;
		std::map<std::string, GUILineGraphDataSet*> _mmapDataSets;	// Hashmap holding each data set
	};
}