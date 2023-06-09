#pragma once
#include "PCH.h"
#include "precision.h"

namespace X
{

	// Vector class, using real number which is defined in the precision.h file to use either floats or doubles
	class CVector2r
	{
	public:
		// Constructor, sets all values to zero
		CVector2r();

		// Constructor, sets all values to the ones given
		CVector2r(real rX, real rY);

		// Addition operator which adds two vectors together
		// Vector addition is commutative. (v1 + v2 == v2 + v1) is always true.
		// Triangle law of vector addition states that when two vectors are represented as two sides of the
		// triangle with the order of magnitude and direction, then the third side of the triangle represents
		// the magnitude and direction of the resultant vector.
		// Example:
		// CVector2r vOne(1.0f, 1.0f);
		// CVector2r vTwo(2.0f, 2.0f);
		// CVector2r vThree = vOne + vTwo;
		CVector2r operator +(const CVector2r& vec) const;

		// Addition operator which adds two vectors together
		// Vector addition is commutative. (v1 + v2 == v2 + v1) is always true.
		// Triangle law of vector addition states that when two vectors are represented as two sides of the
		// triangle with the order of magnitude and direction, then the third side of the triangle represents
		// the magnitude and direction of the resultant vector.
		// Example:
		// CVector2r vOne(1.0f, 1.0f);
		// CVector2r vThree(2.0f, 2.0f);
		// vThree += vOne;
		CVector2r& operator +=(const CVector2r& vec);

		// Subtraction operator 
		// Vector subtraction is not commutative. (v1 - v2 == v2 - v1) is only true when v1 is same as v2.
		// Example:
		// CVector2r vOne(1.0f, 1.0f);
		// CVector2r vTwo(2.0f, 2.0f);
		// CVector2r vMinusOne = vOne - vTwo;
		CVector2r operator -(const CVector2r& vec) const;

		// Subtraction operator 
		// Vector subtraction is not commutative. (v1 - v2 == v2 - v1) is only true when v1 is same as v2.
		// Example:
		// CVector2r vMinusOne(1.0f, 1.0f);
		// CVector2r vTwo(2.0f, 2.0f);
		// vMinusOne -= vTwo;
		CVector2r& operator -=(const CVector2r& vec);

		// Multiplication by scalar
		// Adjusts the magnitude of a vector, keeping the direction the same (Unless the scalar is negative, in which case the direction is reversed)
		// Example:
		// CVector2r vTwo(2.0f, 2.0f);
		// CVector2r vOne = vTwo * 0.5f;
		const CVector2r operator *(const real r) const;

		// Multiplication by scalar
		// Adjusts the magnitude of a vector, keeping the direction the same (Unless the scalar is negative, in which case the direction is reversed) 
		// Example:
		// CVector2r vOne(2.0f, 2.0f);
		// vOne *= 0.5f; 
		void operator *=(const real r);

		// Check for equality
		bool operator ==(const CVector2r& vec) const;

		// Check for inequality
		bool operator !=(const CVector2r& vec) const;

		// Divides both elements by the given scalar value
		// If scalar given is less than 0.0000001, an exception occurs
		void divide(real rScaler);

		// Multiplies both elements by the given scalar value
		// Adjusts the magnitude of a vector, keeping the direction the same (Unless the scalar is negative, in which case the direction is reversed) 
		void multiply(real rScaler);

		// Sets each element to the values given
		void set(real rX, real rY);

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

		// Computes distance between this vector and the one given, treating each vector as a point in 3D space.
		real getDistance(const CVector2r& vec) const;

		// Computes distance squared between this vector and the one given, treating each vector as a point in 3D space.
		// This is faster than the getDistance() method, as no expensive square root is used.
		// This is useful if you have many vectors/points and need to find the closest out of them all.
		// After finding the closest, then you can squareroot the return value from this method or use getDistance() to obtain the true distance. 
		real getDistanceSquared(const CVector2r& vec) const;

		// Returns 1.0 if vOther is clockwise, else -1.0 if vOther is anticlockwise
		real getSign(const CVector2r& vOther) const;

		// Computes the dot product (also known as inner product) between this vector and the one given.
		// Dot product is commutative (v1.getDot(v2) == v2.getDot(v1)) == always true.
		// If returned dot product is...
		// > 0		vectors are pointing roughly in same direction (upto 90 degrees apart)
		// = 0		vectors are perpendicular to one another (90 degrees)
		// < 0		vectors are pointing in opposite directions (+90 degrees to 180 degrees)	
		real getDot(const CVector2r& vec) const;

		// Returns angle (in radians between this and parsed vector (unsigned result)
		real getAngleRadiansUnsigned(const CVector2r& vOther) const;

		// Returns signed angle (in radians) between this vector and one parsed
		// If given vector is clockwise of this vector, the angle (in radians) will be positive, else negative
		real getAngleRadiansSigned(const CVector2r& vOther) const;

		// Returns signed angle (in degrees) between this vector and one parsed
		// If given vector is clockwise of this vector, the angle (in degrees) will be positive, else negative
		real getAngleDegreesSigned(const CVector2r& vOther) const;

		// Returns angle in degrees that this vector is from north (CVector2r(0,1)) in the range of 0-360
		real getAngleDegrees360(void) const;

		// If this vector represents a position, rotate it clockwise by the given number of degrees
		void rotate(real rDegrees);

		// Limits the length of this vector so that is equal to or below the parsed value
		// If the length of this vector does not exceed the one given, it remains unchanged.
		void limitLength(real rMaxLength);

		// Returns a vector which is at right angle / 90 degrees / perpendicular to this one
		CVector2r getPerpendicular(void) const;

		// Determines a point between two specified points.
		// The parameter r determines where the new interpolated point is located relative to the two end points specified by parameters pt1 and pt2.
		// The closer the value of the parameter r is to 1.0, the closer the interpolated point is to the first point (parameter pt1).
		// The closer the value of the parameter r is to 0, the closer the interpolated point is to the second point (parameter pt2). 
		// Returns the new interpolated point
		CVector2r interpolate(const CVector2r& v1, const CVector2r& v2, real r) const;

		real x;
		real y;
	};
}