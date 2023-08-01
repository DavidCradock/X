#pragma once
#include "PCH.h"

namespace X
{
	// Vector class, using floats
	class CVector4f
	{
	public:
		// Constructor, sets all values to zero
		CVector4f();

		// Constructor, sets all values to the ones given
		CVector4f(float fX, float fY, float fZ, float fW);

		// Addition operator which adds two vectors together
		// Vector addition is commutative. (v1 + v2 == v2 + v1) is always true.
		// Triangle law of vector addition states that when two vectors are represented as two sides of the
		// triangle with the order of magnitude and direction, then the third side of the triangle represents
		// the magnitude and direction of the resultant vector.
		// Example:
		// CVector4f vOne(1.0f, 1.0f, 1.0f, 1.0f);
		// CVector4f vTwo(2.0f, 2.0f, 2.0f, 2.0f);
		// CVector4f vThree = vOne + vTwo;
		CVector4f operator +(const CVector4f& vec) const;

		// Addition operator which adds two vectors together
		// Vector addition is commutative. (v1 + v2 == v2 + v1) is always true.
		// Triangle law of vector addition states that when two vectors are represented as two sides of the
		// triangle with the order of magnitude and direction, then the third side of the triangle represents
		// the magnitude and direction of the resultant vector.
		// Example:
		// CVector4f vOne(1.0f, 1.0f, 1.0f, 1.0f);
		// CVector4f vThree(2.0f, 2.0f, 2.0f, 2.0f);
		// vThree += vOne;
		CVector4f& operator +=(const CVector4f& vec);

		// Subtraction operator 
		// Vector subtraction is not commutative. (v1 - v2 == v2 - v1) is only true when v1 is same as v2.
		// Example:
		// CVector4f vOne(1.0f, 1.0f, 1.0f, 1.0f);
		// CVector4f vTwo(2.0f, 2.0f, 2.0f, 2.0f);
		// CVector4f vMinusOne = vOne - vTwo;
		CVector4f operator -(const CVector4f& vec) const;

		// Subtraction operator 
		// Vector subtraction is not commutative. (v1 - v2 == v2 - v1) is only true when v1 is same as v2.
		// Example:
		// CVector4f vMinusOne(1.0f, 1.0f, 1.0f, 1.0f);
		// CVector4f vTwo(2.0f, 2.0f, 2.0f, 2.0f);
		// vMinusOne -= vTwo;
		CVector4f& operator -=(const CVector4f& vec);

		// Multiplication by scalar
		// Adjusts the magnitude of a vector, keeping the direction the same (Unless the scalar is negative, in which case the direction is reversed)
		// Example:
		// CVector4f vTwo(2.0f, 2.0f, 2.0f, 2.0f);
		// CVector4f vOne = vTwo * 0.5f;
		const CVector4f operator *(const float f) const;

		// Multiplication by scalar
		// Adjusts the magnitude of a vector, keeping the direction the same (Unless the scalar is negative, in which case the direction is reversed) 
		// Example:
		// CVector4f vOne(2.0f, 2.0f, 2.0f, 2.0f);
		// vOne *= 0.5f; 
		void operator *=(const float f);

		// Multiplication by another vector
		// Example:
		// CVector4f vOne(3.0f, 3.0f, 3.0f, 3.0f);
		// CVector4f vTwo(2.0f, 2.0f, 2.0f, 2.0f);
		// CVector4f vResult = vOne * vTwo;
		const CVector4f operator *(const CVector4f& v) const;

		// Check for equality
		bool operator ==(const CVector4f& vec) const;

		// Check for inequality
		bool operator !=(const CVector4f& vec) const;

		// Sets each element to the values given
		void set(float fX, float fY, float fZ, float fW);

		// Sets each element within the vector's set to zero.
		// A zero vector is a special case. It does not have a direction, and it's magnitude is zero. 
		void setZero(void);

		// Returns true if the vector is a zero vector.
		bool isZero(void) const;

		// Negates the vector.
		// A negated vector has the same magnitude as before, however direction is reversed. 
		void negate(void);

		// Computes and returns the vector's magnitude
		// A vector's magnitude is also know as "the length" or norm of the vector.
		// Uses sqrtf() so it's quite expensive.
		float getMagnitude(void) const;

		// Normalises this vector so that it becomes a unit vector (Has a magnitude of 1)
		// Uses sqrtf() so it's quite expensive.
		// If the vector has zero length(magnitude), then it is not modified.
		void normalise(void);

		// Returns x, y, z and w as an array of floats
		// Example:
		// float array[4];
		// vector.getAsArray(array);
		void getAsArray(float* pArray) const;

		// Multiplies this vector by the given scalar
		void multiply(float f);

		float x;
		float y;
		float z;
		float w;
	};
}