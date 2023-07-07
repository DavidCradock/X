#pragma once
#include "PCH.h"
#include "precision.h"

namespace X
{

	// Vector class, using doubles
	class CVector2d
	{
	public:
		// Constructor, sets all values to zero
		CVector2d();

		// Constructor, sets all values to the ones given
		CVector2d(double dX, real dY);

		// Addition operator which adds two vectors together
		// Vector addition is commutative. (v1 + v2 == v2 + v1) is always true.
		// Triangle law of vector addition states that when two vectors are represented as two sides of the
		// triangle with the order of magnitude and direction, then the third side of the triangle represents
		// the magnitude and direction of the resultant vector.
		// Example:
		// CVector2d vOne(1.0, 1.0);
		// CVector2d vTwo(2.0, 2.0);
		// CVector2d vThree = vOne + vTwo;
		CVector2d operator +(const CVector2d& vec) const;

		// Addition operator which adds two vectors together
		// Vector addition is commutative. (v1 + v2 == v2 + v1) is always true.
		// Triangle law of vector addition states that when two vectors are represented as two sides of the
		// triangle with the order of magnitude and direction, then the third side of the triangle represents
		// the magnitude and direction of the resultant vector.
		// Example:
		// CVector2d vOne(1.0, 1.0);
		// CVector2d vThree(2.0, 2.0);
		// vThree += vOne;
		CVector2d& operator +=(const CVector2d& vec);

		// Subtraction operator 
		// Vector subtraction is not commutative. (v1 - v2 == v2 - v1) is only true when v1 is same as v2.
		// Example:
		// CVector2d vOne(1.0, 1.0);
		// CVector2d vTwo(2.0, 2.0);
		// CVector2d vMinusOne = vOne - vTwo;
		CVector2d operator -(const CVector2d& vec) const;

		// Subtraction operator 
		// Vector subtraction is not commutative. (v1 - v2 == v2 - v1) is only true when v1 is same as v2.
		// Example:
		// CVector2d vMinusOne(1.0, 1.0);
		// CVector2d vTwo(2.0, 2.0);
		// vMinusOne -= vTwo;
		CVector2d& operator -=(const CVector2d& vec);

		// Multiplication by scalar
		// Adjusts the magnitude of a vector, keeping the direction the same (Unless the scalar is negative, in which case the direction is reversed)
		// Example:
		// CVector2d vTwo(2.0, 2.0);
		// CVector2d vOne = vTwo * 0.5;
		const CVector2d operator *(const double d) const;

		// Multiplication by scalar
		// Adjusts the magnitude of a vector, keeping the direction the same (Unless the scalar is negative, in which case the direction is reversed) 
		// Example:
		// CVector2d vOne(2.0, 2.0);
		// vOne *= 0.5f; 
		void operator *=(const double d);

		// Check for equality
		bool operator ==(const CVector2d& vec) const;

		// Check for inequality
		bool operator !=(const CVector2d& vec) const;

		// Sets each element to the values given
		void set(double dX, double dY);

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
		// Uses sqrt() so it's quite expensive.
		double getMagnitude(void) const;

		// Normalises this vector so that it becomes a unit vector (Has a magnitude of 1)
		// Uses sqrt() so it's quite expensive.
		// If the vector has zero length(magnitude), then it is not modified.
		void normalise(void);

		// Computes distance between this vector and the one given, treating each vector as a point in 3D space.
		double getDistance(const CVector2d& vec) const;

		// Computes distance squared between this vector and the one given, treating each vector as a point in 3D space.
		// This is faster than the getDistance() method, as no expensive square root is used.
		// This is useful if you have many vectors/points and need to find the closest out of them all.
		// After finding the closest, then you can squareroot the return value from this method or use getDistance() to obtain the true distance. 
		double getDistanceSquared(const CVector2d& vec) const;

		double x;
		double y;
	};
}