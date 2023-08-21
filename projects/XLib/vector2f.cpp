#include "PCH.h"
#include "vector2f.h"
#include "logging.h"

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

	void CVector2f::divide(float fScaler)
	{
		ThrowIfTrue(fScaler < 0.0000001f, "CVector2f::divide() given scalar of zero which creates a divide by zero.");
		float f1Over = 1.0f / fScaler;
		x *= f1Over;
		y *= f1Over;
	}

	void CVector2f::multiply(float fScaler)
	{
		x *= fScaler;
		y *= fScaler;
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

	float CVector2f::getSign(const CVector2f& vOther) const
	{
		if (y * vOther.x > x * vOther.y)
		{
			return 1.0f;
		}
		else
		{
			return -1.0f;
		}
	}

	float CVector2f::getDot(const CVector2f& vec) const
	{
		return x * vec.x + y * vec.y;
	}

	float CVector2f::getAngleRadiansUnsigned(const CVector2f& vOther) const
	{
		float fDot = getDot(vOther);
		if (fDot < -1.0f)
			fDot = 1.0f;
		else if (fDot > 1.0f)
			fDot = 1.0f;
		return acosf(fDot);
	}

	float CVector2f::getAngleRadiansSigned(const CVector2f& vOther) const
	{
		CVector2f vOtherNorm = vOther;
		vOtherNorm.normalise();

		float fDot = getDot(vOtherNorm);
		// Make sure fDot is valid
		if (fDot < -1.0f)
			fDot = -1.0f;
		else if (fDot > 1.0f)
			fDot = 1.0f;
		float fAngleRadians = acosf(fDot);
		fAngleRadians *= getSign(vOtherNorm);
		return fAngleRadians;
	}

	float CVector2f::getAngleDegreesSigned(const CVector2f& vOther) const
	{
		// Normalise this vector
		CVector2f vThisNorm(x, y);
		vThisNorm.normalise();

		// Normalise other vector
		CVector2f vOtherNorm(vOther.x, vOther.y);
		vOtherNorm.normalise();

		// Compute dot
		float fTmp = vThisNorm.x * vOtherNorm.x + vThisNorm.y * vOtherNorm.y;

		// Convert to unsigned radians
		// make sure fTmp is valid
		if (fTmp < -1.0f)
			fTmp = -1.0f;
		else if (fTmp > 1.0f)
			fTmp = 1.0f;
		fTmp = (float)acosf(fTmp);

		// Convert to signed radians
		if (vThisNorm.y * vOtherNorm.x < vThisNorm.x * vOtherNorm.y)
			fTmp *= -1.0f;

		// Convert to degrees
		// NOTE: 180.0f / Pi is used to convert radians to degrees
		// and it is a value of : 57.2957795
		return fTmp * 57.2957795f;
	}

	float CVector2f::getAngleDegrees360(void) const
	{
		// Normalise this vector
		CVector2f vThisNorm(x, y);
		vThisNorm.normalise();

		CVector2f vNorth(0.0f, 1.0f);

		// Compute dot
		float fTmp = vThisNorm.x * vNorth.x + vThisNorm.y * vNorth.y;

		// Convert to unsigned radians
		// First, make sure fTmp is valid
		if (fTmp < -1.0f)
			fTmp = -1.0f;
		else if (fTmp > 1.0f)
			fTmp = 1.0f;
		fTmp = (float)acosf(fTmp);

		// Convert to degrees
		// NOTE: 180.0f / Pi is used to convert radians to degrees
		// and it is a value of : 57.2957795
		fTmp *= 57.2957795f;

		if (vThisNorm.y * vNorth.x < vThisNorm.x * vNorth.y)
		{
			return fTmp;
		}
		return 180.0f + (180.0f - fTmp);
	}

	void CVector2f::rotate(float fDegrees)
	{
		float fRad = fDegrees * -0.01745329251994329576923690768489f;
		float fOldX = x;
		float fOldY = y;
		float fSin, fCos;
		fSin = sinf(fRad);
		fCos = cosf(fRad);
		x = fOldX * fCos - fOldY * fSin;
		y = fOldX * fSin + fOldY * fCos;
	}

	void CVector2f::limitLength(float fMaxLength)
	{
		float fLen = getMagnitude();
		if (fLen <= fMaxLength)
			return;
		divide(fLen);	// Normalise
		multiply(fMaxLength);
	}

	CVector2f CVector2f::getPerpendicular(void) const
	{
		//CVector2f vPerpendicular(y, -x);
		CVector2f vPerpendicular(-y, x);
		return vPerpendicular;
	}

	CVector2f CVector2f::interpolate(const CVector2f& v1, const CVector2f& v2, float f) const
	{
		if (f < 0.0f)
			return v2;
		else if (f > 1.0f)
			return v1;
		CVector2f vA = v1;
		vA.multiply(f);
		CVector2f vB = v2;
		vB.multiply(1.0f - f);
		return vA + vB;
	}

	void CVector2f::getAsArray(float* pArray) const
	{
		pArray[0] = x;
		pArray[1] = y;
	}

}