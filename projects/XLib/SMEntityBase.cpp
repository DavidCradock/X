#include "PCH.h"
#include "SMEntityBase.h"

namespace X
{
	CSMEntityBase::CSMEntityBase()
	{
		_mbWorldNeedsUpdate = false;
		_mvPosition.set(0, 0, 0);
		_mvScale.set(1.0f, 1.0f, 1.0f);
		_mqRotation.setIdentity();
	}

	CMatrix CSMEntityBase::getWorldMatrix(void)
	{
		if (_mbWorldNeedsUpdate)
			_updateWorldMatrix();
		return matrixWorld;
	}

	void CSMEntityBase::setWorldPosition(CVector3f vNewPosition)
	{
		_mvPosition = vNewPosition;
		_mbWorldNeedsUpdate = true;
	}

	void CSMEntityBase::translateWorld(CVector3f vTranslation)
	{
		_mvPosition.x += vTranslation.x;
		_mvPosition.y += vTranslation.y;
		_mvPosition.z += vTranslation.z;
		_mbWorldNeedsUpdate = true;
	}

	void CSMEntityBase::translateLocal(float fForward, float fUp, float fRight)
	{
		CMatrix matrix;
		matrix.setFromQuaternion(_mqRotation);
		CVector3f vForward, vRight, vUp;
		matrix.getForwardVector(vForward);
		matrix.getRightVector(vRight);
		matrix.getUpVector(vUp);
		vForward *= fForward;
		vRight *= fRight;
		vUp *= fUp;
		_mvPosition += vForward;
		_mvPosition += vRight;
		_mvPosition += vUp;
		_mbWorldNeedsUpdate = true;
	}

	void CSMEntityBase::translateLocal(const CVector3f& vForwardUpRight)
	{
		translateLocal(vForwardUpRight.x, vForwardUpRight.y, vForwardUpRight.z);
	}

	CVector3f CSMEntityBase::getWorldPosition(void) const
	{
		return _mvPosition;
	}

	void CSMEntityBase::setScale(CVector3f vScale)
	{
		_mvScale = vScale;
		_mbWorldNeedsUpdate = true;
	}

	CVector3f CSMEntityBase::getScale(void) const
	{
		return _mvScale;
	}

	void CSMEntityBase::setRotation(const CVector3f& vAxis, float fAngleRadians)
	{
		_mqRotation.setFromAxisAngle(vAxis, fAngleRadians);
		_mbWorldNeedsUpdate = true;
	}

	void CSMEntityBase::setRotation(const CQuaternion& quaternion)
	{
		_mqRotation = quaternion;
		_mbWorldNeedsUpdate = true;
	}

	void CSMEntityBase::rotate(const CVector3f& vAxis, float fAngleRadians)
	{
		CQuaternion qNewRot;
		qNewRot.setFromAxisAngle(vAxis, fAngleRadians);
		_mqRotation *= qNewRot;
		_mbWorldNeedsUpdate = true;
	}

	void CSMEntityBase::rotateLocal(float fPitchRadians, float fYawRadians, float fRollRadians)
	{
		// Create a rotation matrix from current rotation stored in quaternion
		CMatrix matrix;
		matrix.setFromQuaternion(_mqRotation);

		// These will hold, forward, right and up vectors from the matrix
		CVector3f vForward, vRight, vUp;

		// This will holds new rotation quaternion to apply to current rotation
		CQuaternion qNewRot;

		// Apply pitch
		matrix.getRightVector(vRight);
		qNewRot.setFromAxisAngle(vRight, fPitchRadians);
		_mqRotation *= qNewRot;

		// Apply yaw
		matrix.setFromQuaternion(_mqRotation);
		matrix.getUpVector(vUp);
		qNewRot.setFromAxisAngle(vUp, fYawRadians);
		_mqRotation *= qNewRot;

		// Apply roll
		matrix.setFromQuaternion(_mqRotation);
		matrix.getForwardVector(vForward);
		qNewRot.setFromAxisAngle(vForward, fRollRadians);
		_mqRotation *= qNewRot;

		_mbWorldNeedsUpdate = true;
	}

	void CSMEntityBase::rotateLocal(const CVector3f& vPitchYawRoll)
	{
		rotateLocal(vPitchYawRoll.x, vPitchYawRoll.y, vPitchYawRoll.z);
	}

	void CSMEntityBase::_updateWorldMatrix(void)
	{
		CMatrix trans;
		trans.setTranslation(_mvPosition);
		
		CMatrix scale;
		scale.setScale(_mvScale);

		CMatrix rotation;
		rotation.setFromQuaternion(_mqRotation);

		matrixWorld = trans * scale * rotation;
		_mbWorldNeedsUpdate = false;
	}

	void CSMEntityBase::getEuler(float& fPitchRadians, float& fYawRadians, float& fRollRadians) const
	{
		CVector3f vEuler = _mqRotation.getEuler();
		fPitchRadians = vEuler.x;
		fYawRadians = vEuler.y;
		fRollRadians = vEuler.z;
	}

	CVector3f CSMEntityBase::getEuler(void) const
	{
		return _mqRotation.getEuler();
	}
}