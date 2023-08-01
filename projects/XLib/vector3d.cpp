#include "PCH.h"
#include "vector3d.h"

namespace X
{
	CVector3d::CVector3d()
	{
		x = y = z = 0.0f;
	}

	CVector3d::CVector3d(double dX, double dY, double dZ)
	{
		x = dX;
		y = dY;
		z = dZ;
	}

	CVector3d CVector3d::operator +(const CVector3d& vec) const
	{
		return CVector3d(x + vec.x, y + vec.y, z + vec.z);
	}

	CVector3d& CVector3d::operator +=(const CVector3d& vec)
	{
		x += vec.x;
		y += vec.y;
		z += vec.z;
		return *this;
	}

	CVector3d CVector3d::operator -(const CVector3d& vec) const
	{
		return CVector3d(x - vec.x, y - vec.y, z - vec.z);
	}

	CVector3d& CVector3d::operator -=(const CVector3d& vec)
	{
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;
		return *this;
	}

	const CVector3d CVector3d::operator*(const double d) const
	{
		return CVector3d(x * d, y * d, z * d);
	}

	void CVector3d::operator*=(const double d)
	{
		x = x * d;
		y = y * d;
		z = z * d;
	}

	bool CVector3d::operator ==(const CVector3d& vec) const
	{
		return x == vec.x && y == vec.y && z == vec.z;
	}

	bool CVector3d::operator !=(const CVector3d& vec) const
	{
		return x != vec.x || y != vec.y || z != vec.z;
	}

	void CVector3d::set(double dX, double dY, double dZ)
	{
		x = dX;
		y = dY;
		z = dZ;
	}

	void CVector3d::setZero(void)
	{
		x = y = z = 0.0f;
	}

	bool CVector3d::isZero(void) const
	{
		return x == 0 && y == 0 && z == 0;
	}

	void CVector3d::negate(void)
	{
		x = -x;
		y = -y;
		z = -z;
	}

	double CVector3d::getMagnitude(void) const
	{
		double dMagnitude = x * x;
		dMagnitude += y * y;
		dMagnitude += z * z;
		dMagnitude = sqrt(dMagnitude);
		return dMagnitude;
	}

	void CVector3d::normalise(void)
	{
		// Compute magnitude aka length
		double dMagnitude = x * x;
		dMagnitude += y * y;
		dMagnitude += z * z;
		dMagnitude = sqrt(dMagnitude);
		if (dMagnitude == 0.0f)	// Prevent divide by zero
			return;
		double dReciprocal = 1.0 / dMagnitude;
		x *= dReciprocal;
		y *= dReciprocal;
		z *= dReciprocal;
	}

	double CVector3d::getDot(const CVector3d& vec) const
	{
		return x * vec.x + y * vec.y + z * vec.z;
	}

	CVector3d CVector3d::getCross(const CVector3d& vec) const
	{
		CVector3d vCross;
		vCross.x = y * vec.z - z * vec.y;
		vCross.y = z * vec.x - x * vec.z;
		vCross.z = x * vec.y - y * vec.x;
		return vCross;
	}

	double CVector3d::getAngle(const CVector3d& vec) const
	{
		return acos(getDot(vec));
	}

	double CVector3d::getDistance(const CVector3d& vec) const
	{
		double dx = x - vec.x;
		double dy = y - vec.y;
		double dz = z - vec.z;
		return sqrt(dx * dx + dy * dy + dz * dz);
	}

	double CVector3d::getDistanceSquared(const CVector3d& vec) const
	{
		double dx = x - vec.x;
		double dy = y - vec.y;
		double dz = z - vec.z;
		return dx * dx + dy * dy + dz * dz;
	}

	void CVector3d::getAsArray(double* pArray) const
	{
		pArray[0] = x;
		pArray[1] = y;
		pArray[2] = z;
	}
}