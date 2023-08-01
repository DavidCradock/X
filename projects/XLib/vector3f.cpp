#include "PCH.h"
#include "vector3f.h"

namespace X
{
	CVector3f::CVector3f()
	{
		x = y = z = 0.0f;
	}

	CVector3f::CVector3f(float fX, float fY, float fZ)
	{
		x = fX;
		y = fY;
		z = fZ;
	}

	CVector3f::CVector3f(const CVector3f& vector)
	{
		x = vector.x;
		y = vector.y;
		z = vector.z;
	}

	CVector3f CVector3f::operator +(const CVector3f& vec) const
	{
		return CVector3f(x + vec.x, y + vec.y, z + vec.z);
	}

	CVector3f& CVector3f::operator +=(const CVector3f& vec)
	{
		x += vec.x;
		y += vec.y;
		z += vec.z;
		return *this;
	}

	CVector3f CVector3f::operator -(const CVector3f& vec) const
	{
		return CVector3f(x - vec.x, y - vec.y, z - vec.z);
	}

	CVector3f& CVector3f::operator -=(const CVector3f& vec)
	{
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;
		return *this;
	}

	const CVector3f CVector3f::operator*(const float f) const
	{
		return CVector3f(x * f, y * f, z * f);
	}

	void CVector3f::operator*=(const float f)
	{
		x = x * f;
		y = y * f;
		z = z * f;
	}

	bool CVector3f::operator ==(const CVector3f& vec) const
	{
		return x == vec.x && y == vec.y && z == vec.z;
	}

	bool CVector3f::operator !=(const CVector3f& vec) const
	{
		return x != vec.x || y != vec.y || z != vec.z;
	}

	void CVector3f::set(float fX, float fY, float fZ)
	{
		x = fX;
		y = fY;
		z = fZ;
	}

	void CVector3f::setZero(void)
	{
		x = y = z = 0.0f;
	}

	bool CVector3f::isZero(void) const
	{
		return x == 0 && y == 0 && z == 0;
	}

	void CVector3f::negate(void)
	{
		x = -x;
		y = -y;
		z = -z;
	}

	float CVector3f::getMagnitude(void) const
	{
		float fMagnitude = x * x;
		fMagnitude += y * y;
		fMagnitude += z * z;
		fMagnitude = sqrtf(fMagnitude);
		return fMagnitude;
	}

	void CVector3f::normalise(void)
	{
		// Compute magnitude aka length
		float fMagnitude = x * x;
		fMagnitude += y * y;
		fMagnitude += z * z;
		fMagnitude = sqrtf(fMagnitude);
		if (fMagnitude == 0.0f)	// Prevent divide by zero
			return;
		float fReciprocal = 1.0f / fMagnitude;
		x *= fReciprocal;
		y *= fReciprocal;
		z *= fReciprocal;
	}

	float CVector3f::getDot(const CVector3f& vec) const
	{
		return x * vec.x + y * vec.y + z * vec.z;
	}

	CVector3f CVector3f::getCross(const CVector3f& vec) const
	{
		CVector3f vCross;
		vCross.x = y * vec.z - z * vec.y;
		vCross.y = z * vec.x - x * vec.z;
		vCross.z = x * vec.y - y * vec.x;
		return vCross;
	}

	float CVector3f::getAngle(const CVector3f& vec) const
	{
		return acosf(getDot(vec));
	}

	float CVector3f::getDistance(const CVector3f& vec) const
	{
		float fx = x - vec.x;
		float fy = y - vec.y;
		float fz = z - vec.z;
		return sqrtf(fx * fx + fy * fy + fz * fz);
	}

	float CVector3f::getDistanceSquared(const CVector3f& vec) const
	{
		float fx = x - vec.x;
		float fy = y - vec.y;
		float fz = z - vec.z;
		return fx * fx + fy * fy + fz * fz;
	}

	void CVector3f::getAsArray(float* pArray) const
	{
		pArray[0] = x;
		pArray[1] = y;
		pArray[2] = z;
	}

	void CVector3f::multiply(float f)
	{
		x *= f;
		y *= f;
		z *= f;
	}
}