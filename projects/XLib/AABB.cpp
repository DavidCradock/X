#include "PCH.h"
#include "AABB.h"

namespace X
{
	CAABB::CAABB()
	{
		_mvMin.set(-0.5f, -0.5f, -0.5f);
		_mvMax.set(0.5f, 0.5f, 0.5f);
	}

	CAABB::CAABB(const CVector3f& vMin, const CVector3f& vMax)
	{
		_mvMin = vMin;
		_mvMax = vMax;
	}

	void CAABB::setMinMax(const CVector3f& vMin, const CVector3f& vMax)
	{
		_mvMin = vMin;
		_mvMax = vMax;
	}

	void CAABB::setPosDims(const CVector3f& vPosition, const CVector3f& vDimensions)
	{
		CVector3f vHalfDims = vDimensions * 0.5f;
		_mvMin = vPosition - vHalfDims;
		_mvMax = vPosition + vHalfDims;
	}

	void CAABB::setPosition(const CVector3f& vNewPosition)
	{
		CVector3f vHalfDims = _mvMax - _mvMin;
		vHalfDims *= 0.5f;

		_mvMin = vNewPosition - vHalfDims;
		_mvMax = vNewPosition + vHalfDims;
	}

	void CAABB::setDimensions(const CVector3f& vNewDimensions)
	{
		CVector3f vOldHalfDims = _mvMax - _mvMin;
		vOldHalfDims *= 0.5f;
		CVector3f vPos = _mvMin + vOldHalfDims;

		CVector3f vHalfDims = vNewDimensions * 0.5f;
		_mvMin = vPos - vHalfDims;
		_mvMax = vPos + vHalfDims;
	}

	CVector3f CAABB::getDimensions(void) const
	{
		return _mvMax - _mvMin;
	}

	CVector3f CAABB::getHalfDimensions(void) const
	{
		CVector3f vHalfDims = _mvMax - _mvMin;
		vHalfDims *= 0.5f;
		return vHalfDims;
	}

	CVector3f CAABB::getPosition(void) const
	{
		CVector3f vHalfDims = _mvMax - _mvMin;
		vHalfDims *= 0.5f;

		CVector3f vPos = _mvMin + vHalfDims;
		return vPos;
	}

	bool CAABB::getPointIsInside(const CVector3f& vPoint) const
	{
		if (vPoint.x < _mvMin.x)
			return false;
		if (vPoint.x > _mvMax.x)
			return false;
		if (vPoint.y < _mvMin.y)
			return false;
		if (vPoint.y > _mvMax.y)
			return false;
		if (vPoint.z < _mvMin.z)
			return false;
		if (vPoint.z > _mvMax.z)
			return false;
		return true;
	}

	bool CAABB::getAABBintersects(const CAABB& aabb) const
	{
		if (_mvMin.x > aabb._mvMax.x)	// This is to right of other
			return false;
		if (_mvMin.y > aabb._mvMax.y)	// This is above other
			return false;
		if (_mvMin.z > aabb._mvMax.z)	// This is behind other
			return false;
		if (_mvMax.x < aabb._mvMin.x)	// This is to left of other
			return false;
		if (_mvMax.y < aabb._mvMin.y)	// This is below other
			return false;
		if (_mvMax.z < aabb._mvMin.z)	// This if infront of other
			return false;
		return true;
	}

	void CAABB::resizeArea(float fMultiplier)
	{
		// Get current dimensions of the AABB
		CVector3f vCurrentDims = _mvMax - _mvMin;

		// Compute new dimensions
		CVector3f vNewDims = vCurrentDims * fMultiplier;

		// Compute amount to offset _mvMin and _mvMax by
		CVector3f vOffset = vNewDims * 0.25f;

		// Offset _mvMin and _mvMax
		_mvMin -= vOffset;
		_mvMax += vOffset;
	}

	CVector3f CAABB::getMin(void) const
	{
		return _mvMin;
	}

	CVector3f CAABB::getMax(void) const
	{
		return _mvMax;
	}
}