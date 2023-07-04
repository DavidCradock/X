#pragma once
#include "PCH.h"
#include "plane.h"

namespace X
{
	// A frustum is defined by six planes.
	// It can be used to determine whether various geometric shapes and points are inside of it
	class CFrustum
	{
	public:
		CFrustum();

		// Sets this frustum's planes from the given values used to set a camera's projection matrix
		void computeFromCameraProjection(float fFOVdegrees, float fWidth, float fHeight, float fZNear, float fZFar);

		CPlane mPlaneNear;
		CPlane mPlaneFar;
		CPlane mPlaneLeft;
		CPlane mPlaneRight;
		CPlane mPlaneTop;
		CPlane mPlaneBottom;
	};
}