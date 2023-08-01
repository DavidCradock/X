#include "PCH.h"
#include "vector3r.h"

namespace X
{
	CVector3r::CVector3r()
	{
		x = y = z = 0.0;
	}

	CVector3r::CVector3r(real rX, real rY, real rZ)
	{
		x = rX;
		y = rY;
		z = rZ;
	}

	CVector3r CVector3r::operator +(const CVector3r& vec) const
	{
		return CVector3r(x + vec.x, y + vec.y, z + vec.z);
	}

	CVector3r& CVector3r::operator +=(const CVector3r& vec)
	{
		x += vec.x;
		y += vec.y;
		z += vec.z;
		return *this;
	}

	CVector3r CVector3r::operator -(const CVector3r& vec) const
	{
		return CVector3r(x - vec.x, y - vec.y, z - vec.z);
	}

	CVector3r& CVector3r::operator -=(const CVector3r& vec)
	{
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;
		return *this;
	}

	const CVector3r CVector3r::operator*(const real r) const
	{
		return CVector3r(x * r, y * r, z * r);
	}

	void CVector3r::operator*=(const real r)
	{
		x = x * r;
		y = y * r;
		z = z * r;
	}

	bool CVector3r::operator ==(const CVector3r& vec) const
	{
		return x == vec.x && y == vec.y && z == vec.z;
	}

	bool CVector3r::operator !=(const CVector3r& vec) const
	{
		return x != vec.x || y != vec.y || z != vec.z;
	}

	void CVector3r::set(real rX, real rY, real rZ)
	{
		x = rX;
		y = rY;
		z = rZ;
	}

	void CVector3r::setZero(void)
	{
		x = y = z = 0.0;
	}

	bool CVector3r::isZero(void) const
	{
		return x == 0 && y == 0 && z == 0;
	}

	void CVector3r::negate(void)
	{
		x = -x;
		y = -y;
		z = -z;
	}

	real CVector3r::getMagnitude(void) const
	{
		real rMagnitude = x * x;
		rMagnitude += y * y;
		rMagnitude += z * z;
		rMagnitude = real_sqrt(rMagnitude);
		return rMagnitude;
	}

	void CVector3r::normalise(void)
	{
		// Compute magnitude aka length
		real rMagnitude = x * x;
		rMagnitude += y * y;
		rMagnitude += z * z;
		rMagnitude = real_sqrt(rMagnitude);
		if (rMagnitude == 0.0)	// Prevent divide by zero
			return;
		real rReciprocal = real(1.0 / rMagnitude);
		x *= rReciprocal;
		y *= rReciprocal;
		z *= rReciprocal;
	}

	real CVector3r::getDot(const CVector3r& vec) const
	{
		return x * vec.x + y * vec.y + z * vec.z;
	}

	CVector3r CVector3r::getCross(const CVector3r& vec) const
	{
		CVector3r vCross;
		vCross.x = y * vec.z - z * vec.y;
		vCross.y = z * vec.x - x * vec.z;
		vCross.z = x * vec.y - y * vec.x;
		return vCross;
	}

	real CVector3r::getAngle(const CVector3r& vec) const
	{
		return acos(getDot(vec));
	}

	real CVector3r::getDistance(const CVector3r& vec) const
	{
		real rx = x - vec.x;
		real ry = y - vec.y;
		real rz = z - vec.z;
		return real_sqrt(rx * rx + ry * ry + rz * rz);
	}

	real CVector3r::getDistanceSquared(const CVector3r& vec) const
	{
		real rx = x - vec.x;
		real ry = y - vec.y;
		real rz = z - vec.z;
		return rx * rx + ry * ry + rz * rz;
	}

	void CVector3r::getAsArray(real* pArray) const
	{
		pArray[0] = x;
		pArray[1] = y;
		pArray[2] = z;
	}
}