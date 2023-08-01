#pragma once
#include "PCH.h"
#include "precision.h"

namespace X
{

	// Vector class, using real number which is defined in the precision.h file to use either floats or doubles
	class CVector3r
	{
	public:
		// Constructor, sets all values to zero
		CVector3r();

		// Constructor, sets all values to the ones given
		CVector3r(real rX, real rY, real rZ);

		// Addition operator which adds two vectors together
		// Vector addition is commutative. (v1 + v2 == v2 + v1) is always true.
		// Triangle law of vector addition states that when two vectors are represented as two sides of the
		// triangle with the order of magnitude and direction, then the third side of the triangle represents
		// the magnitude and direction of the resultant vector.
		// Example:
		// CVector3r vOne(1.0f, 1.0f, 1.0f);
		// CVector3r vTwo(2.0f, 2.0f, 2.0f);
		// CVector3r vThree = vOne + vTwo;
		CVector3r operator +(const CVector3r& vec) const;

		// Addition operator which adds two vectors together
		// Vector addition is commutative. (v1 + v2 == v2 + v1) is always true.
		// Triangle law of vector addition states that when two vectors are represented as two sides of the
		// triangle with the order of magnitude and direction, then the third side of the triangle represents
		// the magnitude and direction of the resultant vector.
		// Example:
		// CVector3r vOne(1.0f, 1.0f, 1.0f);
		// CVector3r vThree(2.0f, 2.0f, 2.0f);
		// vThree += vOne;
		CVector3r& operator +=(const CVector3r& vec);

		// Subtraction operator 
		// Vector subtraction is not commutative. (v1 - v2 == v2 - v1) is only true when v1 is same as v2.
		// Example:
		// CVector3r vOne(1.0f, 1.0f, 1.0f);
		// CVector3r vTwo(2.0f, 2.0f, 2.0f);
		// CVector3r vMinusOne = vOne - vTwo;
		CVector3r operator -(const CVector3r& vec) const;

		// Subtraction operator 
		// Vector subtraction is not commutative. (v1 - v2 == v2 - v1) is only true when v1 is same as v2.
		// Example:
		// CVector3r vMinusOne(1.0f, 1.0f, 1.0f);
		// CVector3r vTwo(2.0f, 2.0f, 2.0f);
		// vMinusOne -= vTwo;
		CVector3r& operator -=(const CVector3r& vec);

		// Multiplication by scalar
		// Adjusts the magnitude of a vector, keeping the direction the same (Unless the scalar is negative, in which case the direction is reversed)
		// Example:
		// CVector3r vTwo(2.0f, 2.0f, 2.0f);
		// CVector3r vOne = vTwo * 0.5f;
		const CVector3r operator *(const real r) const;

		// Multiplication by scalar
		// Adjusts the magnitude of a vector, keeping the direction the same (Unless the scalar is negative, in which case the direction is reversed) 
		// Example:
		// CVector3r vOne(2.0f, 2.0f, 2.0f);
		// vOne *= 0.5f; 
		void operator *=(const real r);

		// Check for equality
		bool operator ==(const CVector3r& vec) const;

		// Check for inequality
		bool operator !=(const CVector3r& vec) const;

		// Sets each element to the values given
		void set(real rX, real rY, real rZ);

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
		real getMagnitude(void) const;

		// Normalises this vector so that it becomes a unit vector (Has a magnitude of 1)
		// Uses sqrtf() so it's quite expensive.
		// If the vector has zero length(magnitude), then it is not modified.
		void normalise(void);

		// Computes the dot product (also known as inner product) between this vector and the one given.
		// Dot product is commutative (v1.getDot(v2) == v2.getDot(v1)) == always true.
		// If returned dot product is...
		// > 0		vectors are pointing roughly in same direction (upto 90 degrees apart)
		// = 0		vectors are perpendicular to one another (90 degrees)
		// < 0		vectors are pointing in opposite directions (+90 degrees to 180 degrees)	
		real getDot(const CVector3r& vec) const;

		// Computes the cross product between this vector and the one given.
		// The cross product is a vector which is perpendicular to the two vectors it was calculated from. 
		CVector3r getCross(const CVector3r& vec) const;

		// Computes the angle (in radians) between two UNIT VECTORS (Length of 1)
		// No checking of vectors are unit length here. 
		real getAngle(const CVector3r& vec) const;

		// Computes distance between this vector and the one given, treating each vector as a point in 3D space.
		real getDistance(const CVector3r& vec) const;

		// Computes distance squared between this vector and the one given, treating each vector as a point in 3D space.
		// This is faster than the getDistance() method, as no expensive square root is used.
		// This is useful if you have many vectors/points and need to find the closest out of them all.
		// After finding the closest, then you can squareroot the return value from this method or use getDistance() to obtain the true distance. 
		real getDistanceSquared(const CVector3r& vec) const;

		// Returns x,y and z as an array of reals
		// Example:
		// real array[3];
		// vector.getAsArray(array);
		void getAsArray(real* pArray) const;

		real x;
		real y;
		real z;
	};
}