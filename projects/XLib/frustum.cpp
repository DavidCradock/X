#include "PCH.h"
#include "frustum.h"
#include "resourceManager.h"
#include "utilities.h"

namespace X
{
	CFrustum::CFrustum()
	{

	}

	void CFrustum::computeFromViewProjection(const CMatrix& cameraViewMatrix, const CMatrix& cameraProjectionMatrix)
	{
		/* Taken from Bard
		// Get the view and projection matrices.
		glm::mat4 viewMatrix = glm::lookAt(eye, center, up);
		glm::mat4 projectionMatrix = glm::perspective(fov, aspect, near, far);

		// Compute the frustum planes.
		std::vector<glm::Plane> frustumPlanes;
		frustumPlanes.push_back(glm::Plane(viewMatrix * projectionMatrix * glm::vec4(1, 0, 0, 0), -1));
		frustumPlanes.push_back(glm::Plane(viewMatrix * projectionMatrix * glm::vec4(-1, 0, 0, 0), 1));
		frustumPlanes.push_back(glm::Plane(viewMatrix * projectionMatrix * glm::vec4(0, 1, 0, 0), -1));
		frustumPlanes.push_back(glm::Plane(viewMatrix * projectionMatrix * glm::vec4(0, -1, 0, 0), 1));
		frustumPlanes.push_back(glm::Plane(viewMatrix * projectionMatrix * glm::vec4(0, 0, 1, 0), -1));
		frustumPlanes.push_back(glm::Plane(viewMatrix * projectionMatrix * glm::vec4(0, 0, -1, 0), 1));

		// Normalize the frustum planes.
		for (int i = 0; i < 6; i++) {
		  frustumPlanes[i].normalize();
		}
*/

		// Combine the view and projection matrices
		// Then create vectors with offsets for the six planes
		// Then multiply them by the combined matrices
		// and finally set the distance to origins accordingly
		CMatrix matrixViewProj = cameraViewMatrix * cameraProjectionMatrix;
		CVector3f v;
		float fMag;

		v.set(1.0f, 0.0f, 0.0f);
		v = matrixViewProj.multiply(v);
		fMag = v.getMagnitude();
		v.normalise();
		mPlaneRight.set(v, -fMag);

		v.set(-1.0f,  0.0f,  0.0f);
		v = matrixViewProj.multiply(v);
		fMag = v.getMagnitude();
		v.normalise();
		mPlaneLeft.set(v, fMag);

		v.set( 0.0f,  1.0f,  0.0f);
		v = matrixViewProj.multiply(v);
		fMag = v.getMagnitude();
		v.normalise();
		mPlaneTop.set(v, -fMag);

		v.set( 0.0f, -1.0f,  0.0f);
		v = matrixViewProj.multiply(v);
		fMag = v.getMagnitude();
		v.normalise();
		mPlaneBottom.set(v, fMag);

		v.set( 0.0f,  0.0f,  1.0f);
		v = matrixViewProj.multiply(v);
		fMag = v.getMagnitude();
		v.normalise();
		mPlaneNear.set(v, -fMag);

		v.set( 0.0f,  0.0f, -1.0f);
		v = matrixViewProj.multiply(v);
		fMag = v.getMagnitude();
		v.normalise();
		mPlaneFar.set(v, fMag);
	}

	bool CFrustum::isPointInside(const CVector3f& vPosition) const
	{
		/* Taken from Bard
		bool isPointInsideFrustum(glm::vec3 point, std::vector<glm::Plane> frustumPlanes)
		{
			for (int i = 0; i < 6; i++)
			{
				if (frustumPlanes[i].dot(point) < 0)
				{
					return false;
				}
			}
			return true;
		}
		This code takes a point and a vector of frustum planes as input.
		It then iterates through the frustum planes and checks if the point is on the "wrong" side of any of the planes.
		If the point is on the wrong side of any of the planes, then it is not inside the frustum. Otherwise, the point is inside the frustum.
		
		The dot() function is used to calculate the dot product of the point and the plane normal.
		The dot product is a measure of the angle between two vectors.
		If the dot product is negative, then the point is on the "wrong" side of the plane.
		*/

		// Compute dot product between position and each plane's normal...
		// If returned dot product is...
		// > 0		vectors are pointing roughly in same direction (upto 90 degrees apart)
		// = 0		vectors are perpendicular to one another (90 degrees)
		// < 0		vectors are pointing in opposite directions (+90 degrees to 180 degrees)

		CVector3f v;
		v = mPlaneNear.getNormal() * abs(mPlaneNear.getDistanceToOrigin());
		if (v.getDot(vPosition) < 0)
			return false;
		v = mPlaneFar.getNormal() * abs(mPlaneFar.getDistanceToOrigin());
		if (v.getDot(vPosition) < 0)
			return false;
		v = mPlaneLeft.getNormal() * abs(mPlaneLeft.getDistanceToOrigin());
		if (v.getDot(vPosition) < 0)
			return false;
		v = mPlaneRight.getNormal() * abs(mPlaneRight.getDistanceToOrigin());
		if (v.getDot(vPosition) < 0)
			return false;
		v = mPlaneTop.getNormal() * abs(mPlaneTop.getDistanceToOrigin());
		if (v.getDot(vPosition) < 0)
			return false;
		v = mPlaneBottom.getNormal() * abs(mPlaneBottom.getDistanceToOrigin());
		if (v.getDot(vPosition) < 0)
			return false;
		return true;
	}

	bool CFrustum::isAABBIntersecting(const CAABB& aabb) const
	{
		/* Taken from Bard
		bool isAABBIntersectingFrustum(glm::vec3 min, glm::vec3 max, std::vector<glm::Plane> frustumPlanes)
		{
			for (int i = 0; i < 6; i++)
			{
				// Check if any of the AABB's vertices are on the wrong side of the plane.
				if (frustumPlanes[i].dot(min) < 0 || frustumPlanes[i].dot(max) > 0)
				{
					return false;
				}
			}
			return true;
		}
		This code takes an AABB and a vector of frustum planes as input.
		It then iterates through the frustum planes and checks if any of the AABB's vertices are on the "wrong" side of any of the planes.
		If any of the AABB's vertices are on the wrong side of any of the planes, then the AABB is not intersecting the frustum.
		Otherwise, the AABB is intersecting the frustum.

		The dot() function is used to calculate the dot product of the AABB's vertices and the plane normals.
		The dot product is a measure of the angle between two vectors. If the dot product is negative, then the vertex is on the "wrong" side of the plane.
		*/

		CVector3f v;
		CVector3f vMin = aabb.getMin();
		CVector3f vMax = aabb.getMax();

		v = mPlaneNear.getNormal() * abs(mPlaneNear.getDistanceToOrigin());
		if (v.getDot(vMin) < 0 || v.getDot(vMax) > 0)
			return false;
		v = mPlaneFar.getNormal() * abs(mPlaneFar.getDistanceToOrigin());
		if (v.getDot(vMin) < 0 || v.getDot(vMax) > 0)
			return false;
		v = mPlaneLeft.getNormal() * abs(mPlaneLeft.getDistanceToOrigin());
		if (v.getDot(vMin) < 0 || v.getDot(vMax) > 0)
			return false;
		v = mPlaneRight.getNormal() * abs(mPlaneRight.getDistanceToOrigin());
		if (v.getDot(vMin) < 0 || v.getDot(vMax) > 0)
			return false;
		v = mPlaneTop.getNormal() * abs(mPlaneTop.getDistanceToOrigin());
		if (v.getDot(vMin) < 0 || v.getDot(vMax) > 0)
			return false;
		v = mPlaneBottom.getNormal() * abs(mPlaneBottom.getDistanceToOrigin());
		if (v.getDot(vMin) < 0 || v.getDot(vMax) > 0)
			return false;
		return true;
	}
}