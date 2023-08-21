#include "PCH.h"
#include "vector2r.h"
#include "logging.h"

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

	void CVector2r::divide(real rScaler)
	{
		ThrowIfTrue(rScaler < 0.0000001, "CVector2r::divide() given scalar of zero which creates a divide by zero.");
		real r1Over = 1.0 / rScaler;
		x *= r1Over;
		y *= r1Over;
	}

	void CVector2r::multiply(real rScaler)
	{
		x *= rScaler;
		y *= rScaler;
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

	real CVector2r::getSign(const CVector2r& vOther) const
	{
		if (y * vOther.x > x * vOther.y)
		{
			return 1.0;
		}
		else
		{
			return -1.0;
		}
	}

	real CVector2r::getDot(const CVector2r& vec) const
	{
		return x * vec.x + y * vec.y;
	}

	real CVector2r::getAngleRadiansUnsigned(const CVector2r& vOther) const
	{
		real rDot = getDot(vOther);
		if (rDot < -1.0f)
			rDot = 1.0f;
		else if (rDot > 1.0f)
			rDot = 1.0f;
		return acos(rDot);
	}

	real CVector2r::getAngleRadiansSigned(const CVector2r& vOther) const
	{
		CVector2r vOtherNorm = vOther;
		vOtherNorm.normalise();

		real rDot = getDot(vOtherNorm);
		// Make sure rDot is valid
		if (rDot < -1.0)
			rDot = -1.0;
		else if (rDot > 1.0)
			rDot = 1.0;
		real rAngleRadians = acos(rDot);
		rAngleRadians *= getSign(vOtherNorm);
		return rAngleRadians;
	}

	real CVector2r::getAngleDegreesSigned(const CVector2r& vOther) const
	{
		// Normalise this vector
		CVector2r vThisNorm(x, y);
		vThisNorm.normalise();

		// Normalise other vector
		CVector2r vOtherNorm(vOther.x, vOther.y);
		vOtherNorm.normalise();

		// Compute dot
		real rTmp = vThisNorm.x * vOtherNorm.x + vThisNorm.y * vOtherNorm.y;

		// Convert to unsigned radians
		// make sure rTmp is valid
		if (rTmp < -1.0)
			rTmp = -1.0;
		else if (rTmp > 1.0)
			rTmp = 1.0;
		rTmp = (real)acos(rTmp);

		// Convert to signed radians
		if (vThisNorm.y * vOtherNorm.x < vThisNorm.x * vOtherNorm.y)
			rTmp *= -1.0;

		// Convert to degrees
		// NOTE: 180.0 / Pi is used to convert radians to degrees
		// and it is a value of : 57.29577951308232
		return rTmp * 57.29577951308232;
	}

	real CVector2r::getAngleDegrees360(void) const
	{
		// Normalise this vector
		CVector2r vThisNorm(x, y);
		vThisNorm.normalise();

		CVector2r vNorth(0.0, 1.0);

		// Compute dot
		real rTmp = vThisNorm.x * vNorth.x + vThisNorm.y * vNorth.y;

		// Convert to unsigned radians
		// First, make sure rTmp is valid
		if (rTmp < -1.0)
			rTmp = -1.0;
		else if (rTmp > 1.0)
			rTmp = 1.0;
		rTmp = (real)acos(rTmp);

		// Convert to degrees
		// NOTE: 180.0 / Pi is used to convert radians to degrees
		// and it is a value of : 57.29577951308232
		rTmp *= 57.29577951308232;

		if (vThisNorm.y * vNorth.x < vThisNorm.x * vNorth.y)
		{
			return rTmp;
		}
		return 180.0 + (180.0 - rTmp);
	}

	void CVector2r::rotate(real rDegrees)
	{
		real rRad = rDegrees * -0.01745329251994329576923690768489;
		real rOldX = x;
		real rOldY = y;
		real rSin, rCos;
		rSin = real_sin(rRad);
		rCos = real_cos(rRad);
		x = rOldX * rCos - rOldY * rSin;
		y = rOldX * rSin + rOldY * rCos;
	}

	void CVector2r::limitLength(real rMaxLength)
	{
		real rLen = getMagnitude();
		if (rLen <= rMaxLength)
			return;
		divide(rLen);	// Normalise
		multiply(rMaxLength);
	}

	CVector2r CVector2r::getPerpendicular(void) const
	{
		//CVector2r vPerpendicular(y, -x);
		CVector2r vPerpendicular(-y, x);
		return vPerpendicular;
	}

	CVector2r CVector2r::interpolate(const CVector2r& v1, const CVector2r& v2, real r) const
	{
		if (r < 0.0)
			return v2;
		else if (r > 1.0)
			return v1;
		CVector2r vA = v1;
		vA.multiply(r);
		CVector2r vB = v2;
		vB.multiply(1.0 - r);
		return vA + vB;
	}

	void CVector2r::getAsArray(real* pArray) const
	{
		pArray[0] = x;
		pArray[1] = y;
	}

}