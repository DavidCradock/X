#pragma once
#include "PCH.h"
#include "vector3f.h"

namespace X
{
	class CMatrix;

	class CQuaternion
	{
	public:
		// Default constructor, sets quaternion to identity (0, 0, 0, 1)
		CQuaternion();

		// Sets quaternion from given values
		CQuaternion(float fX, float fY, float fZ, float fW);

		// Sets quaternion from given quaternion
		CQuaternion(const CQuaternion& quaternion);

		// Access each element
		float& operator[] (const int iIndex);

		// Multiply this quaternion by another and return result
		const CQuaternion operator * (const CQuaternion& quaternion) const;

		// Multiply this quaternion by another
		const void operator *= (const CQuaternion& quaternion);

		// Add two quaternions, return result
		const CQuaternion operator+(const CQuaternion& quat) const;

		// Subtract two quaternions, return result
		const CQuaternion operator-(const CQuaternion& quat) const;

		// Multiply by scalar, return result
		const CQuaternion operator*(const float fScalar) const;

		// Divide by scalar, return result
		const CQuaternion operator/(const float fScalar) const;

		// Multiply a vector, return result
		CVector3f operator *(const CVector3f& vec) const;

		// Set this quaternion to the one on the right of the operator
		const void operator = (const CQuaternion& quat);
		
		// Comparison operator
		const bool operator == (const CQuaternion& quat) const;
		
		// Comparison operator
		const bool operator != (const CQuaternion& quat) const;

		// Sets quaternion to identity (0, 0, 0, 1)
		void setIdentity(void);

		// Gets magnitude of the quaternion
		float getMagnitude(void) const;

		// Normalizes the quaternion
		void normalise(void);

		// Returns a quaternion with this quaternion's xyz elements inverted.
		CQuaternion getConjugate(void) const;

		// Returns a quaternion with this quaternion's elements inverted.
		CQuaternion getNegative(void) const;

		// Sets this quaternion from an axis and an angle
		// This does not normalize the given axis
		void setFromAxisAngle(const CVector3f& vAxis, float fAngleRadians);

		// Returns this quaternion's axis
		CVector3f getAxis(void) const;

		// Returns this quaternion's angle
		float getAngle(void) const;

		// Sets this quaternion from Euler angles
		void setFromEuler(float fAngleRadiansX, float fAngleRadiansY, float fAngleRadiansZ);

		// Returns a vector containing the amount of rotation around each local axis
		CVector3f getEuler(void) const;

		// Sets quaternion from given CMatrix
		void setFromMatrix(const CMatrix& matrix);

		// Rotate this quaternion by another
		void rotate(const CQuaternion& quat);

		// Return a quaternion which is interpolated between this quat and the one given using smooth SLERP
		CQuaternion getSLERP(const CQuaternion& quaternion, float fInterval) const;

		// Return a quaternion which is interpolated between this quat and the one given using linear interpolation
		CQuaternion getLERP(const CQuaternion& quaternion, float fInterval) const;

		float q[4];	// x, y, z and w
	};
}