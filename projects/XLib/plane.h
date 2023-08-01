#pragma once
#include "PCH.h"
#include "vector3f.h"

namespace X
{
	// A representation of a plane in 3D space.
	// A plane is defined by two things.
	// 1. Distance to the origin
	// 2. A normal vector
	class CPlane
	{
	public:
		CPlane();
		CPlane(const CVector3f& vNormal, float fDistanceToOrigin);

		// Sets plane to given values
		void set(const CVector3f& vNormal, float fDistanceToOrigin);

		// Returns normal of plane
		CVector3f getNormal(void) const;

		// Returns distance from origin
		float getDistanceToOrigin(void) const;

		// Uses to dot product to compute the distance of a point from the plane.
		// If the given point is on the side of the plane it's normal is pointing,
		// a positive value is returned, else negative.
		float getDistanceFromPlane(const CVector3f& vPoint) const;

	private:
		CVector3f _mvNormal;
		float _mfDistanceToOrigin;
	};
}