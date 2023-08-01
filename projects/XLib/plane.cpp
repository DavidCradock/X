#include "PCH.h"
#include "plane.h"

namespace X
{
	CPlane::CPlane()
	{
		_mvNormal.set(0.0f, 1.0f, 0.0f);
		_mfDistanceToOrigin = 0.0f;
	}

	CPlane::CPlane(const CVector3f& vNormal, float fDistanceToOrigin)
	{
		_mvNormal = vNormal;
		_mfDistanceToOrigin = fDistanceToOrigin;
	}

	void CPlane::set(const CVector3f& vNormal, float fDistanceToOrigin)
	{
		_mvNormal = vNormal;
		_mfDistanceToOrigin = fDistanceToOrigin;
	}

	CVector3f CPlane::getNormal(void) const
	{
		return _mvNormal;
	}

	float CPlane::getDistanceToOrigin(void) const
	{
		return _mfDistanceToOrigin;
	}

	float CPlane::getDistanceFromPlane(const CVector3f& vPoint) const
	{
		// If returned dot product is...
		// > 0		vectors are pointing roughly in same direction (upto 90 degrees apart)
		// = 0		vectors are perpendicular to one another (90 degrees)
		// < 0		vectors are pointing in opposite directions (+90 degrees to 180 degrees)
		return _mvNormal.getDot(vPoint) - _mfDistanceToOrigin;
	}
}