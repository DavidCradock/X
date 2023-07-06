#include "PCH.h"
#include "vector2r.h"

namespace X
{
	CVector2r::CVector2r()
	{
		x = y = 0.0;
	}

	CVector2r::CVector2r(real rX, real rY)
	{
		x = rX;
		y = rY;
	}

	CVector2r CVector2r::operator +(const CVector2r& vec) const
	{
		return CVector2r(x + vec.x, y + vec.y);
	}

	CVector2r& CVector2r::operator +=(const CVector2r& vec)
	{
		x += vec.x;
		y += vec.y;
		return *this;
	}

	CVector2r CVector2r::operator -(const CVector2r& vec) const
	{
		return CVector2r(x - vec.x, y - vec.y);
	}

	CVector2r& CVector2r::operator -=(const CVector2r& vec)
	{
		x -= vec.x;
		y -= vec.y;
		return *this;
	}

	const CVector2r CVector2r::operator*(const real r) const
	{
		return CVector2r(x * r, y * r);
	}

	void CVector2r::operator*=(const real r)
	{
		x = x * r;
		y = y * r;
	}

	bool CVector2r::operator ==(const CVector2r& vec) const
	{
		return x == vec.x && y == vec.y;
	}

	bool CVector2r::operator !=(const CVector2r& vec) const
	{
		return x != vec.x || y != vec.y;
	}

	void CVector2r::set(real rX, real rY)
	{
		x = rX;
		y = rY;
	}

	void CVector2r::setZero(void)
	{
		x = y = 0.0;
	}

	bool CVector2r::isZero(void) const
	{
		return x == 0 && y == 0;
	}

	void CVector2r::negate(void)
	{
		x = -x;
		y = -y;
	}

	real CVector2r::getMagnitude(void) const
	{
		real rMagnitude = x * x;
		rMagnitude += y * y;
		rMagnitude = real_sqrt(rMagnitude);
		return rMagnitude;
	}

	void CVector2r::normalise(void)
	{
		// Compute magnitude aka length
		real rMagnitude = x * x;
		rMagnitude += y * y;
		rMagnitude = real_sqrt(rMagnitude);
		if (rMagnitude == 0.0)	// Prevent divide by zero
			return;
		real rReciprocal = real(1.0 / rMagnitude);
		x *= rReciprocal;
		y *= rReciprocal;
	}

	real CVector2r::getDot(const CVector2r& vec) const
	{
		return x * vec.x + y * vec.y;
	}

	real CVector2r::getAngle(const CVector2r& vec) const
	{
		return acos(getDot(vec));
	}

	real CVector2r::getDistance(const CVector2r& vec) const
	{
		real rx = x - vec.x;
		real ry = y - vec.y;
		return real_sqrt(rx * rx + ry * ry);
	}

	real CVector2r::getDistanceSquared(const CVector2r& vec) const
	{
		real rx = x - vec.x;
		real ry = y - vec.y;
		return rx * rx + ry * ry;
	}
}