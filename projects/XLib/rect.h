#pragma once
#include "PCH.h"
#include "vector2f.h"

namespace X
{
	// Represents a non-uniform cube aka a rectangle
	// Used to compute whether two rects intersect one another
	class CRect
	{
	public:
		// Default constructor, sets everything to zero
		CRect();

		// Construct the rectangle, setting to passed values
		CRect(int iMinX, int iMinY, int iMaxX, int iMaxY);

		// Sets this rect using given values
		void set(int iMinX, int iMinY, int iMaxX, int iMaxY);

		// Returns true if the passed CRect intersects with this one
		bool intersects(const CRect& other) const;

		// Resizes the rect so that it's dimensions are multiplied by the given int.
		// Example:
		// If mvMin = (0,0) and mvMax = (10,10) (total dims of (10,10)) and the given int is 2 then the new values would be
		// mvMin = (-5,-5) and mvMax = (15,15) (total dims of (20, 20))
		void resizeArea(int iMultiplier);

		// Given a position, returns whether it fits within this rect or not
		bool doesPositionFitWithin(int iPosX, int iPosY);

		// Returns the centre position of the rect
		void getCentre(int& iCentreX, int& iCentreY);

		int miMinX;		// Left
		int miMinY;		// Bottom
		int miMaxX;		// Right
		int miMaxY;		// Top
	};
}
