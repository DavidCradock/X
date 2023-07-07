#include "PCH.h"
#include "vector2f.h"

namespace X
{
	CVector2f::CVector2f()
	{
		x = y = 0.0f;
	}

	CVector2f::CVector2f(float fX, float fY)
	{
		x = fX;
		y = fY;
	}

	CVector2f CVector2f::operator +(const CVector2f& vec) const
	{
		return CVector2f(x + vec.x, y + vec.y);
	}

	CVector2f& CVector2f::operator +=(const CVector2f& vec)
	{
		x += vec.x;
		y += vec.y;
		return *this;
	}

	CVector2f CVector2f::operator -(const CVector2f& vec) const
	{
		return CVector2f(x - vec.x, y - vec.y);
	}

	CVector2f& CVector2f::operator -=(const CVector2f& vec)
	{
		x -= vec.x;
		y -= vec.y;
		return *this;
	}

	const CVector2f CVector2f::operator*(const float f) const
	{
		return CVector2f(x * f, y * f);
	}

	void CVector2f::operator*=(const float f)
	{
		x = x * f;
		y = y * f;
	}

	bool CVector2f::operator ==(const CVector2f& vec) const
	{
		return x == vec.x && y == vec.y;
	}

	bool CVector2f::operator !=(const CVector2f& vec) const
	{
		return x != vec.x || y != vec.y;
	}

	void CVector2f::set(float fX, float fY)
	{
		x = fX;
		y = fY;
	}

	void CVector2f::setZero(void)
	{
		x = y = 0.0f;
	}

	bool CVector2f::isZero(void) const
	{
		return x == 0 && y == 0;
	}

	void CVector2f::negate(void)
	{
		x = -x;
		y = -y;
	}

	float CVector2f::getMagnitude(void) const
	{
		float fMagnitude = x * x;
		fMagnitude += y * y;
		fMagnitude = sqrtf(fMagnitude);
		return fMagnitude;
	}

	void CVector2f::normalise(void)
	{
		// Compute magnitude aka length
		float fMagnitude = x * x;
		fMagnitude += y * y;
		fMagnitude = sqrtf(fMagnitude);
		if (fMagnitude == 0.0f)	// Prevent divide by zero
			return;
		float fReciprocal = float(1.0f / fMagnitude);
		x *= fReciprocal;
		y *= fReciprocal;
	}

	float CVector2f::getDistance(const CVector2f& vec) const
	{
		float fx = x - vec.x;
		float fy = y - vec.y;
		return sqrtf(fx * fx + fy * fy);
	}

	float CVector2f::getDistanceSquared(const CVector2f& vec) const
	{
		float fx = x - vec.x;
		float fy = y - vec.y;
		return fx * fx + fy * fy;
	}
}