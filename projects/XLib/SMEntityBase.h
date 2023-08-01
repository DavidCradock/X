#pragma once
#include "PCH.h"
#include "vector3f.h"
#include "matrix.h"
#include "quaternion.h"

namespace X
{
	// This is an object which is inserted into the various scene managers
	// This is a base class which we derive other classes from, we don't use these directly.
	// This is because all entities have their position, rotation and scale, but differ by what they render (meshes, particles etc)
	class CSMEntityBase
	{
	public:
		CSMEntityBase();
		
		// Returns the world matrix and updates it if needed
		CMatrix getWorldMatrix(void);

		// Sets this entities position in world space
		void setWorldPosition(CVector3f vNewPosition);

		// Moves the entity by the specified amount in world space from it's current position
		void translateWorld(CVector3f vTranslation);

		// Moves the entity by the specified amount offseting it's position based upon it's current orientation
		void translateLocal(float fForward, float fUp, float fRight);

		// Moves the entity by the specified amount offseting it's position based upon it's current orientation
		void translateLocal(const CVector3f& vForwardUpRight);

		// Gets currently set position in world space
		CVector3f getWorldPosition(void) const;

		// Set scale of this entity
		void setScale(CVector3f vScale);

		// Returns entity's currently set scale
		CVector3f getScale(void) const;

		// Sets the rotation of this entity with the given axis and angle
		void setRotation(const CVector3f& vAxis, float fAngleRadians);

		// Sets the rotation of this entity to the given quaternion
		void setRotation(const CQuaternion& quaternion);

		// Rotates the existing rotation by the specified axis and angle
		void rotate(const CVector3f& vAxis, float fAngleRadians);

		// Rotates the existing rotation by the given pitch, yaw and roll, in relation to the entity's current rotation axis
		void rotateLocal(float fPitchRadians, float fYawRadians, float fRollRadians);

		// Rotates the existing rotation by the given pitch, yaw and roll, in relation to the entity's current rotation axis
		void rotateLocal(const CVector3f& vPitchYawRoll);

		// Using current rotation, computes current pitch, yaw and roll and stores values in given parameters.
		// Uses CQuaternion::getEuler()
		void getEuler(float& fPitchRadians, float& fYawRadians, float& fRollRadians) const;

		// Using current rotation, computes current pitch, yaw and roll and returns the result in a CVector3f
		// Uses CQuaternion::getEuler()
		CVector3f getEuler(void) const;

	private:
		// Updates the world matrix ragardless of _mbWorldNeedsUpdate value
		void _updateWorldMatrix(void);

		CMatrix matrixWorld;		// Holds the world transform of this entity
		bool _mbWorldNeedsUpdate;	// Whether the world matrix of this entity needs updating
		
		CVector3f _mvPosition;		// Position of this entity in world space
		CVector3f _mvScale;			// Scale of this entity
		CQuaternion _mqRotation;	// Rotation of this entity
	};
}