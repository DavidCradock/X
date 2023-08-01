#pragma once
#include "PCH.h"
#include "plane.h"
#include "matrix.h"
#include "SMCamera.h"
#include "AABB.h"

namespace X
{
	// A frustum is defined by six planes.
	// It can be used to determine whether various geometric shapes and points are inside of it
	class CFrustum
	{
	public:
		// Default constructor, does nothing
		CFrustum();

		// Sets this frustum's planes from the given matrices which are from a camera's view and projection matrices.
		// TODO Check this
		void computeFromViewProjection(const CMatrix& cameraViewMatrix, const CMatrix& cameraProjectionMatrix);

		// Returns true if the given position is inside the frustum, else false.
		// TODO Check this
		bool isPointInside(const CVector3f& vPosition) const;

		// Returns true if the given Axis Aligned Bounding Box is intersecting this frustum, else false.
		// TODO Check this
		bool isAABBIntersecting(const CAABB& aabb) const;

		CPlane mPlaneNear;
		CPlane mPlaneFar;
		CPlane mPlaneLeft;
		CPlane mPlaneRight;
		CPlane mPlaneTop;
		CPlane mPlaneBottom;
	};
}