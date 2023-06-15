#pragma once
#include "PCH.h"
#include "plane.h"

namespace X
{
	// A frustum is defined by six planes.
	// It can be used to determine whether various geometric shapes and points are inside of it
	class Frustum
	{
	public:
		Frustum();

		// Sets this frustum's planes from the given values used to set a camera's projection matrix
		void computeFromCameraProjection(float fFOVdegrees, float fWidth, float fHeight, float fZNear, float fZFar);

		Plane mPlaneNear;
		Plane mPlaneFar;
		Plane mPlaneLeft;
		Plane mPlaneRight;
		Plane mPlaneTop;
		Plane mPlaneBottom;
	};
}