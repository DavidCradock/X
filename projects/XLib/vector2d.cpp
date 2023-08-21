#include "PCH.h"
#include "vector2d.h"
#include "logging.h"

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

	void CVector2d::divide(double dScaler)
	{
		ThrowIfTrue(dScaler < 0.0000001, "CVector2d::divide() given scalar of zero which creates a divide by zero.");
		double d1Over = 1.0 / dScaler;
		x *= d1Over;
		y *= d1Over;
	}

	void CVector2d::multiply(double dScaler)
	{
		x *= dScaler;
		y *= dScaler;
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

	double CVector2d::getSign(const CVector2d& vOther) const
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

	double CVector2d::getDot(const CVector2d& vec) const
	{
		return x * vec.x + y * vec.y;
	}

	double CVector2d::getAngleRadiansUnsigned(const CVector2d& vOther) const
	{
		double dDot = getDot(vOther);
		if (dDot < -1.0)
			dDot = 1.0;
		else if (dDot > 1.0)
			dDot = 1.0;
		return acos(dDot);
	}

	double CVector2d::getAngleRadiansSigned(const CVector2d& vOther) const
	{
		CVector2d vOtherNorm = vOther;
		vOtherNorm.normalise();

		double dDot = getDot(vOtherNorm);
		// Make sure dDot is valid
		if (dDot < -1.0)
			dDot = -1.0;
		else if (dDot > 1.0)
			dDot = 1.0;
		double dAngleRadians = acos(dDot);
		dAngleRadians *= getSign(vOtherNorm);
		return dAngleRadians;
	}

	double CVector2d::getAngleDegreesSigned(const CVector2d& vOther) const
	{
		// Normalise this vector
		CVector2d vThisNorm(x, y);
		vThisNorm.normalise();

		// Normalise other vector
		CVector2d vOtherNorm(vOther.x, vOther.y);
		vOtherNorm.normalise();

		// Compute dot
		double dTmp = vThisNorm.x * vOtherNorm.x + vThisNorm.y * vOtherNorm.y;

		// Convert to unsigned radians
		// make sure dTmp is valid
		if (dTmp < -1.0)
			dTmp = -1.0;
		else if (dTmp > 1.0)
			dTmp = 1.0;
		dTmp = (double)acos(dTmp);

		// Convert to signed radians
		if (vThisNorm.y * vOtherNorm.x < vThisNorm.x * vOtherNorm.y)
			dTmp *= -1.0;

		// Convert to degrees
		// NOTE: 180.0 / Pi is used to convert radians to degrees
		// and it is a value of : 57.29577951308232
		return dTmp * 57.29577951308232;
	}

	double CVector2d::getAngleDegrees360(void) const
	{
		// Normalise this vector
		CVector2d vThisNorm(x, y);
		vThisNorm.normalise();

		CVector2d vNorth(0.0, 1.0);

		// Compute dot
		double dTmp = vThisNorm.x * vNorth.x + vThisNorm.y * vNorth.y;

		// Convert to unsigned radians
		// First, make sure dTmp is valid
		if (dTmp < -1.0)
			dTmp = -1.0;
		else if (dTmp > 1.0)
			dTmp = 1.0;
		dTmp = (double)acos(dTmp);

		// Convert to degrees
		// NOTE: 180.0 / Pi is used to convert radians to degrees
		// and it is a value of : 57.29577951308232
		dTmp *= 57.29577951308232;

		if (vThisNorm.y * vNorth.x < vThisNorm.x * vNorth.y)
		{
			return dTmp;
		}
		return 180.0 + (180.0 - dTmp);
	}

	void CVector2d::rotate(double dDegrees)
	{
		double dRad = dDegrees * -0.01745329251994329576923690768489;
		double dOldX = x;
		double dOldY = y;
		double dSin, dCos;
		dSin = sin(dRad);
		dCos = cos(dRad);
		x = dOldX * dCos - dOldY * dSin;
		y = dOldX * dSin + dOldY * dCos;
	}

	void CVector2d::limitLength(double dMaxLength)
	{
		double dLen = getMagnitude();
		if (dLen <= dMaxLength)
			return;
		divide(dLen);	// Normalise
		multiply(dMaxLength);
	}

	CVector2d CVector2d::getPerpendicular(void) const
	{
		//CVector2d vPerpendicular(y, -x);
		CVector2d vPerpendicular(-y, x);
		return vPerpendicular;
	}

	CVector2d CVector2d::interpolate(const CVector2d& v1, const CVector2d& v2, double d) const
	{
		if (d < 0.0)
			return v2;
		else if (d > 1.0)
			return v1;
		CVector2d vA = v1;
		vA.multiply(d);
		CVector2d vB = v2;
		vB.multiply(1.0 - d);
		return vA + vB;
	}

	void CVector2d::getAsArray(double* pArray) const
	{
		pArray[0] = x;
		pArray[1] = y;
	}
}