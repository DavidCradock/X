#pragma once
#include "PCH.h"
#include "vector3f.h"

namespace X
{
	// An Axis Aligned Bounding Box
	// An AABB is simply a box with it's axis aligned to the world.
	// It is similar to the 2D CRect class, but in 3D and is used for culling with the COctTree class
	// 
	class CAABB
	{
	public:
		// Default constructor, sets to a 1x1x1 cube centered at origin
		CAABB();

		// Sets box with given values
		CAABB(const CVector3f& vMin, const CVector3f& vMax);

		// Sets box position and dims with given values
		void setMinMax(const CVector3f& vMin, const CVector3f& vMax);

		// Sets box position and dimes with given values
		void setPosDims(const CVector3f& vPosition, const CVector3f& vDimensions);

		// Sets position of box, leaving dimensions alone
		void setPosition(const CVector3f& vNewPosition);

		// Sets dimensions of box, leaving position alone
		void setDimensions(const CVector3f& vNewDimensions);

		// Returns dimensions of the box
		CVector3f getDimensions(void) const;

		// Returns half dimensions of the box
		CVector3f getHalfDimensions(void) const;

		// Returns position of the box
		CVector3f getPosition(void) const;

		// Returns true if the given point is inside of this box
		bool getPointIsInside(const CVector3f& vPoint) const;

		// Returns true if the given AABB intersects with this one
		// TODO test this
		bool getAABBintersects(const CAABB& aabb) const;

		// Resizes the area so that it's dimensions are multiplied by the given float
		// Example:
		// If mvMin = (0,0) and mvMax = (10,10) (total dims of (10,10)) and the given float is 2 then the new values would be
		// mvMin = (-5,-5) and mvMax = (15,15) (total dims of (20, 20))
		// TODO Test this
		void resizeArea(float fMultiplier);

		// Returns _mvMin
		CVector3f getMin(void) const;

		// Returns _mvMax
		CVector3f getMax(void) const;
	private:
		CVector3f _mvMin;	// Minimum positions of the corners of the box
		CVector3f _mvMax;	// Maximum positions of the corners of the box
	};
}