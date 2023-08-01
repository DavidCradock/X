#include "PCH.h"
#include "vector4f.h"

namespace X
{
	CVector4f::CVector4f()
	{
		x = y = z = w = 0.0f;
	}

	CVector4f::CVector4f(float fX, float fY, float fZ, float fW)
	{
		x = fX;
		y = fY;
		z = fZ;
		w = fW;
	}

	CVector4f CVector4f::operator +(const CVector4f& vec) const
	{
		return CVector4f(x + vec.x, y + vec.y, z + vec.z, w + vec.w);
	}

	CVector4f& CVector4f::operator +=(const CVector4f& vec)
	{
		x += vec.x;
		y += vec.y;
		z += vec.z;
		w += vec.w;
		return *this;
	}

	CVector4f CVector4f::operator -(const CVector4f& vec) const
	{
		return CVector4f(x - vec.x, y - vec.y, z - vec.z, w - vec.w);
	}

	CVector4f& CVector4f::operator -=(const CVector4f& vec)
	{
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;
		w -= vec.w;
		return *this;
	}

	const CVector4f CVector4f::operator*(const float f) const
	{
		return CVector4f(x * f, y * f, z * f, w * f);
	}

	void CVector4f::operator*=(const float f)
	{
		x = x * f;
		y = y * f;
		z = z * f;
		w = w * f;
	}

	const CVector4f CVector4f::operator *(const CVector4f& v) const
	{
		CVector4f result;
		result.x = x * v.x;
		result.y = y * v.y;
		result.z = z * v.z;
		result.w = w * v.w;
		return result;
	}

	bool CVector4f::operator ==(const CVector4f& vec) const
	{
		return x == vec.x && y == vec.y && z == vec.z && w == vec.w;
	}

	bool CVector4f::operator !=(const CVector4f& vec) const
	{
		return x != vec.x || y != vec.y || z != vec.z || w != vec.w;
	}

	void CVector4f::set(float fX, float fY, float fZ, float fW)
	{
		x = fX;
		y = fY;
		z = fZ;
		w = fW;
	}

	void CVector4f::setZero(void)
	{
		x = y = z = w = 0.0f;
	}

	bool CVector4f::isZero(void) const
	{
		return x == 0 && y == 0 && z == 0 && w == 0;
	}

	void CVector4f::negate(void)
	{
		x = -x;
		y = -y;
		z = -z;
		w = -w;
	}

	float CVector4f::getMagnitude(void) const
	{
		float fMagnitude = x * x;
		fMagnitude += y * y;
		fMagnitude += z * z;
		fMagnitude += w * w;
		fMagnitude = sqrtf(fMagnitude);
		return fMagnitude;
	}

	void CVector4f::normalise(void)
	{
		// Compute magnitude aka length
		float fMagnitude = x * x;
		fMagnitude += y * y;
		fMagnitude += z * z;
		fMagnitude += w * w;
		fMagnitude = sqrtf(fMagnitude);
		if (fMagnitude == 0.0f)	// Prevent divide by zero
			return;
		float fReciprocal = 1.0f / fMagnitude;
		x *= fReciprocal;
		y *= fReciprocal;
		z *= fReciprocal;
		w *= fReciprocal;
	}

	void CVector4f::getAsArray(float* pArray) const
	{
		pArray[0] = x;
		pArray[1] = y;
		pArray[2] = z;
		pArray[3] = w;
	}

	void CVector4f::multiply(float f)
	{
		x *= f;
		y *= f;
		z *= f;
		w *= f;
	}
}