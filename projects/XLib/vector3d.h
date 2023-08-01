#pragma once
#include "PCH.h"

namespace X
{
	// Vector class, using doubles
	class CVector3d
	{
	public:
		// Constructor, sets all values to zero
		CVector3d();

		// Constructor, sets all values to the ones given
		CVector3d(double dX, double dY, double dZ);

		// Addition operator which adds two vectors together
		// Vector addition is commutative. (v1 + v2 == v2 + v1) is always true.
		// Triangle law of vector addition states that when two vectors are represented as two sides of the
		// triangle with the order of magnitude and direction, then the third side of the triangle represents
		// the magnitude and direction of the resultant vector.
		// Example:
		// CVector3d vOne(1.0, 1.0, 1.0);
		// CVector3d vTwo(2.0, 2.0, 2.0);
		// CVector3d vThree = vOne + vTwo;
		CVector3d operator +(const CVector3d& vec) const;

		// Addition operator which adds two vectors together
		// Vector addition is commutative. (v1 + v2 == v2 + v1) is always true.
		// Triangle law of vector addition states that when two vectors are represented as two sides of the
		// triangle with the order of magnitude and direction, then the third side of the triangle represents
		// the magnitude and direction of the resultant vector.
		// Example:
		// CVector3d vOne(1.0, 1.0, 1.0);
		// CVector3d vThree(2.0, 2.0, 2.0);
		// vThree += vOne;
		CVector3d& operator +=(const CVector3d& vec);

		// Subtraction operator 
		// Vector subtraction is not commutative. (v1 - v2 == v2 - v1) is only true when v1 is same as v2.
		// Example:
		// CVector3d vOne(1.0, 1.0, 1.0);
		// CVector3d vTwo(2.0, 2.0, 2.0);
		// CVector3d vMinusOne = vOne - vTwo;
		CVector3d operator -(const CVector3d& vec) const;

		// Subtraction operator 
		// Vector subtraction is not commutative. (v1 - v2 == v2 - v1) is only true when v1 is same as v2.
		// Example:
		// CVector3d vMinusOne(1.0, 1.0, 1.0);
		// CVector3d vTwo(2.0, 2.0, 2.0);
		// vMinusOne -= vTwo;
		CVector3d& operator -=(const CVector3d& vec);

		// Multiplication by scalar
		// Adjusts the magnitude of a vector, keeping the direction the same (Unless the scalar is negative, in which case the direction is reversed)
		// Example:
		// CVector3d vTwo(2.0, 2.0, 2.0);
		// CVector3d vOne = vTwo * 0.5;
		const CVector3d operator *(const double d) const;

		// Multiplication by scalar
		// Adjusts the magnitude of a vector, keeping the direction the same (Unless the scalar is negative, in which case the direction is reversed) 
		// Example:
		// CVector3d vOne(2.0, 2.0, 2.0);
		// vOne *= 0.5; 
		void operator *=(const double d);

		// Check for equality
		bool operator ==(const CVector3d& vec) const;

		// Check for inequality
		bool operator !=(const CVector3d& vec) const;

		// Sets each element to the values given
		void set(double dX, double dY, double dZ);

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

		// Computes the dot product (also known as inner product) between this vector and the one given.
		// Dot product is commutative (v1.getDot(v2) == v2.getDot(v1)) == always true.
		// If returned dot product is...
		// > 0		vectors are pointing roughly in same direction (upto 90 degrees apart)
		// = 0		vectors are perpendicular to one another (90 degrees)
		// < 0		vectors are pointing in opposite directions (+90 degrees to 180 degrees)	
		double getDot(const CVector3d& vec) const;

		// Computes the cross product between this vector and the one given.
		// The cross product is a vector which is perpendicular to the two vectors it was calculated from. 
		CVector3d getCross(const CVector3d& vec) const;

		// Computes the angle (in radians) between two UNIT VECTORS (Length of 1)
		// No checking of vectors are unit length here. 
		double getAngle(const CVector3d& vec) const;

		// Computes distance between this vector and the one given, treating each vector as a point in 3D space.
		double getDistance(const CVector3d& vec) const;

		// Computes distance squared between this vector and the one given, treating each vector as a point in 3D space.
		// This is faster than the getDistance() method, as no expensive square root is used.
		// This is useful if you have many vectors/points and need to find the closest out of them all.
		// After finding the closest, then you can squareroot the return value from this method or use getDistance() to obtain the true distance. 
		double getDistanceSquared(const CVector3d& vec) const;

		// Returns x,y and z as an array of doubles
		// Example:
		// double array[3];
		// vector.getAsArray(array);
		void getAsArray(double* pArray) const;

		double x;
		double y;
		double z;
	};
}