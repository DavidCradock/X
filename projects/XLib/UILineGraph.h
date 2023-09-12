#pragma once
#include "PCH.h"
//#include "resourceTexture2DAtlas.h"
//#include "resourceVertexBufferCPT2.h"
#include "UITheme.h"

namespace X
{
	class CUIContainer;

	// Data set used by CUILineGraph object.
	class CUILineGraphDataSet
	{
	public:
		std::vector<float> values;		// Each of the values stored in this data set
		CColour colour;					// The colour used to render this data set as a line on a graph

		// Add new value to the data set
		void addValue(float fValue);

		// Remove oldest entry value
		void removeValue(void);

		// Get number of values currently in the set
		unsigned int getNumValues(void) const;

		// Returns highest value in dataset
		float getHighestValue(void) const;

		// Returns loweset value in dataset
		float getLowestValue(void) const;

	private:
	};

	// A 2 axis line graph
	// A dataset has to be added to store all values (y axis) which are plotted against the x axis
	// Only one dataset is required and is represented by a line on the graph, however, multiple datasets can exist (multiple lines)
	class CUILineGraph
	{
		friend class CUIContainer;
	public:
		CUILineGraph(CUIContainer* pContainer);
		~CUILineGraph();

		// Sets the dimensions of the widget.
		void setDimensions(float fX, float fY);

		// Sets the dimensions of the widget.
		void setDimensions(int iX, int iY);

		// Sets the dimensions of the widget.
		void setDimensions(const CVector2f& vDimensions);

		// Returns the dimensions of the widget.
		CVector2f getDimensions(void) const;

		// Returns the minimum dimensions of this widget based upon the container's currently set theme.
		CVector2f getDimensionsMinimum(void) const;

		// Sets the position of this widget in relation to it's container.
		// Both axis should be at least zero.
		void setPosition(float fX, float fY);

		// Sets the position of this widget in relation to it's container.
		// Both axis should be at least zero.
		void setPosition(int iX, int iY);

		// Sets the position of this widget in relation to it's container.
		// Both axis should be at least zero.
		void setPosition(const CVector2f& vPosition);

		// Returns the position of the widget in relation to it's container.
		CVector2f getPosition(void) const;

		// Sets whether this widget is visible or not.
		void setVisible(bool bVisible);

		// Returns whether this widget is visible or not.
		bool getVisible(void) const;

		// Render this widget's BG
		void renderBG(CResourceVertexBufferCPT2* pVB);

		// Render this widget's non-background items
		void renderNonBG(void);

		// Update this widget
		void update(float fTimeDeltaSec);

		/******************************************************************* Widget specific *******************************************************************/

		// Add a new named data set to this graph
		// If the named set already exists, an exception occurs
		// Returns a pointer to the newly added data set
		CUILineGraphDataSet* addDataset(const std::string& strName, const CColour& cCol);

		// Returns a pointer to a previously added data set
		// If the data set couldn't be found, an exception occurs
		CUILineGraphDataSet* getDataset(const std::string& strName);

		// Removes a previously added data set from the graph
		// If the named data set doesn't exist, this silently fails
		void removeDataset(const std::string& strName);

		// Returns total number of data sets
		unsigned int getNumDatasets(void);

		// Returns the name of the data set at given index.
		// If given index is invalid, an exception occurs
		std::string getDatasetName(unsigned int iIndex);
	private:
		// Common amoung widgets
		CVector2f _mvDimensions;			// Dimensions of the widget
		CVector2f _mvPosition;				// Position of the widget in relation to it's container.
		bool _mbVisible;					// Whether this widget is visible or not.
		CUIContainer* _mpContainer;			// The container this widget belongs to. Set in constructor

		// Widget specific
		std::map<std::string, CUILineGraphDataSet*> _mmapDataSets;	// Hashmap holding each data set
	};
}