#include "PCH.h"
#include "vector2d.h"

namespace X
{
	CVector2d::CVector2d()
	{
		x = y = 0.0;
	}

	CVector2d::CVector2d(double dX, real dY)
	{
		x = dX;
		y = dY;
	}

	CVector2d CVector2d::operator +(const CVector2d& vec) const
	{
		return CVector2d(x + vec.x, y + vec.y);
	}

	CVector2d& CVector2d::operator +=(const CVector2d& vec)
	{
		x += vec.x;
		y += vec.y;
		return *this;
	}

	CVector2d CVector2d::operator -(const CVector2d& vec) const
	{
		return CVector2d(x - vec.x, y - vec.y);
	}

	CVector2d& CVector2d::operator -=(const CVector2d& vec)
	{
		x -= vec.x;
		y -= vec.y;
		return *this;
	}

	const CVector2d CVector2d::operator*(const double d) const
	{
		return CVector2d(x * d, y * d);
	}

	void CVector2d::operator*=(const double d)
	{
		x = x * d;
		y = y * d;
	}

	bool CVector2d::operator ==(const CVector2d& vec) const
	{
		return x == vec.x && y == vec.y;
	}

	bool CVector2d::operator !=(const CVector2d& vec) const
	{
		return x != vec.x || y != vec.y;
	}

	void CVector2d::set(double dX, double dY)
	{
		x = dX;
		y = dY;
	}

	void CVector2d::setZero(void)
	{
		x = y = 0.0;
	}

	bool CVector2d::isZero(void) const
	{
		return x == 0 && y == 0;
	}

	void CVector2d::negate(void)
	{
		x = -x;
		y = -y;
	}

	double CVector2d::getMagnitude(void) const
	{
		double dMagnitude = x * x;
		dMagnitude += y * y;
		dMagnitude = sqrt(dMagnitude);
		return dMagnitude;
	}

	void CVector2d::normalise(void)
	{
		// Compute magnitude aka length
		double dMagnitude = x * x;
		dMagnitude += y * y;
		dMagnitude = sqrt(dMagnitude);
		if (dMagnitude == 0.0)	// Prevent divide by zero
			return;
		double dReciprocal = double(1.0 / dMagnitude);
		x *= dReciprocal;
		y *= dReciprocal;
	}

	double CVector2d::getDistance(const CVector2d& vec) const
	{
		double dx = x - vec.x;
		double dy = y - vec.y;
		return real_sqrt(dx * dx + dy * dy);
	}

	double CVector2d::getDistanceSquared(const CVector2d& vec) const
	{
		double dx = x - vec.x;
		double dy = y - vec.y;
		return dx * dx + dy * dy;
	}
}