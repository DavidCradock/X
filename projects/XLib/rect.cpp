#include "PCH.h"
#include "rect.h"

namespace X
{
	CRect::CRect()
	{
	}

	CRect::CRect(int iMinX, int iMinY, int iMaxX, int iMaxY)
	{
		miMinX = iMinX;
		miMinY = iMinY;
		miMaxX = iMaxX;
		miMaxY = iMaxY;
	}

	void CRect::set(int iMinX, int iMinY, int iMaxX, int iMaxY)
	{
		miMinX = iMinX;
		miMinY = iMinY;
		miMaxX = iMaxX;
		miMaxY = iMaxY;
	}

	bool CRect::intersects(const CRect& other) const
	{
		if (miMaxX < other.miMinX)
			return false;
		if (miMinX > other.miMaxX)
			return false;
		if (miMaxY < other.miMinY)
			return false;
		if (miMinY > other.miMaxY)
			return false;

		return true;
	}

	void CRect::resizeArea(int iMultiplier)
	{
		// Get current width and height of the rect
		int iCurrentWidth = miMaxX - miMinX;
		int iCurrentHeight = miMaxY - miMinY;

		// Compute new dimensions
		int iNewWidth = iCurrentWidth * iMultiplier;
		int iNewHeight = iCurrentHeight * iMultiplier;

		// Quarter the new dims, to get the amount to offset mvMin and mvMax by
		int iOffsetX = iNewWidth / 4;
		int iOffsetY = iNewHeight / 4;

		// Offset mvMin and mvMax
		miMinX -= iOffsetX;
		miMaxX += iOffsetX;
		miMinY -= iOffsetY;
		miMaxY += iOffsetY;
	}

	bool CRect::doesPositionFitWithin(int iPosX, int iPosY)
	{
		if (iPosX < miMinX)
			return false;
		if (iPosX > miMaxX)
			return false;
		if (iPosY < miMinY)
			return false;
		if (iPosY > miMaxY)
			return false;
		return true;
	}

	void CRect::getCentre(int& iCentreX, int& iCentreY)
	{
		// Compute dims
		iCentreX = miMaxX - miMinX;
		iCentreY = miMaxY - miMinY;

		// Compute centre
		iCentreX /= 2;
		iCentreY /= 2;
	}
}