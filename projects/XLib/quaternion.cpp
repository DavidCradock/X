#include "PCH.h"
#include "quaternion.h"
#include "matrix.h"

namespace X
{
	CQuaternion::CQuaternion()
	{
		setIdentity();
	}

	CQuaternion::CQuaternion(float fX, float fY, float fZ, float fW)
	{
		q[0] = fX;
		q[1] = fY;
		q[2] = fZ;
		q[3] = fW;
	}

	CQuaternion::CQuaternion(const CQuaternion& quaternion)
	{
		*this = quaternion;
	}

	float& CQuaternion::operator [](const int iIndex)
	{
		return q[iIndex];
	}

	const CQuaternion CQuaternion::operator *(const CQuaternion& quat) const
	{
		/* Does the same as below? But too many sqrtf
		CVector3 v1(mfQuat[0], mfQuat[1], mfQuat[2]);
		CVector3 v2(quat.mfQuat[0], quat.mfQuat[1], quat.mfQuat[2]);
		CVector3 vFinal = v1 * quat.mfQuat[3] + v2 * mfQuat[3] + v1.getCross(v2);
		float fScalar = mfQuat[3] * quat.mfQuat[3] - v1.getDot(v2);
		return CQuaternion(vFinal.x, vFinal.y, vFinal.z, fScalar);
		*/
		return CQuaternion(q[3] * quat.q[0] + q[0] * quat.q[3] +
			q[1] * quat.q[2] - q[2] * quat.q[1],

			q[3] * quat.q[1] + q[1] * quat.q[3] +
			q[2] * quat.q[0] - q[0] * quat.q[2],

			q[3] * quat.q[2] + q[2] * quat.q[3] +
			q[0] * quat.q[1] - q[1] * quat.q[0],

			q[3] * quat.q[3] - q[0] * quat.q[0] -
			q[1] * quat.q[1] - q[2] * quat.q[2]);
	}

	const void CQuaternion::operator *=(const CQuaternion& quat)
	{
		CQuaternion tmp(q[3] * quat.q[0] + q[0] * quat.q[3] +
			q[1] * quat.q[2] - q[2] * quat.q[1],

			q[3] * quat.q[1] + q[1] * quat.q[3] +
			q[2] * quat.q[0] - q[0] * quat.q[2],

			q[3] * quat.q[2] + q[2] * quat.q[3] +
			q[0] * quat.q[1] - q[1] * quat.q[0],

			q[3] * quat.q[3] - q[0] * quat.q[0] -
			q[1] * quat.q[1] - q[2] * quat.q[2]);
		*this = tmp;
	}

	const CQuaternion CQuaternion::operator +(const CQuaternion& quat) const
	{
		return CQuaternion(
			q[0] + quat.q[0],
			q[1] + quat.q[1],
			q[2] + quat.q[2],
			q[3] + quat.q[3]);
	}

	const CQuaternion CQuaternion::operator -(const CQuaternion& quat) const
	{
		return CQuaternion(
			q[0] - quat.q[0],
			q[1] - quat.q[1],
			q[2] - quat.q[2],
			q[3] - quat.q[3]);
	}

	const CQuaternion CQuaternion::operator *(const float fScalar) const
	{
		return CQuaternion(
			q[0] * fScalar,
			q[1] * fScalar,
			q[2] * fScalar,
			q[3] * fScalar);
	}

	const CQuaternion CQuaternion::operator /(const float fScalar) const
	{
		float fInvScl = 1 / fScalar;
		return CQuaternion(
			q[0] * fInvScl,
			q[1] * fInvScl,
			q[2] * fInvScl,
			q[3] * fInvScl);
	}

	CVector3f CQuaternion::operator *(const CVector3f& vec) const
	{
		// nVidia SDK implementation (Taken from OGRE)
		CVector3f v2, v3;
		CVector3f vQ(q[0], q[1], q[2]);
		v2 = vQ.getCross(vec);
		v3 = vQ.getCross(v2);
		v2 *= (2.0f * q[3]);
		v3 *= 2.0f;
		return vec + v2 + v3;
	}

	const void CQuaternion::operator =(const CQuaternion& quat)
	{
		memcpy(q, quat.q, sizeof(float[4]));
	}

	const bool CQuaternion::operator ==(const CQuaternion& quat) const
	{
		return (q[0] == quat.q[0] && q[1] == quat.q[1] && q[2] == quat.q[2] && q[3] == quat.q[3]);
	}

	const bool CQuaternion::operator != (const CQuaternion& quat) const
	{
		return !(*this == quat);
	}

	void CQuaternion::setIdentity(void)
	{
		q[0] = q[1] = q[2] = 0;
		q[3] = 1;
	}

	float CQuaternion::getMagnitude(void) const
	{
		return sqrtf(	(q[0] * q[0]) +
						(q[1] * q[1]) +
						(q[2] * q[2]) +
						(q[3] * q[3]));
	}

	void CQuaternion::normalise(void)
	{
		float fInvMag = 1.0f / getMagnitude();
		if (fInvMag != 1.0f)
		{
			q[0] *= fInvMag;
			q[1] *= fInvMag;
			q[2] *= fInvMag;
			q[3] *= fInvMag;
		}
	}

	CQuaternion CQuaternion::getConjugate(void) const
	{
		return CQuaternion(-q[0], -q[1], -q[2], q[3]);
	}

	CQuaternion CQuaternion::getNegative(void) const
	{
		return CQuaternion(-q[0], -q[1], -q[2], -q[3]);
	}

	void CQuaternion::setFromAxisAngle(const CVector3f& vAxis, float fAngleRadians)
	{
		float sa = sinf(fAngleRadians * 0.5f);
		float ca = cosf(fAngleRadians * 0.5f);
		q[0] = vAxis.x * sa;
		q[1] = vAxis.y * sa;
		q[2] = vAxis.z * sa;
		q[3] = ca;
	}

	CVector3f CQuaternion::getAxis(void) const
	{
		float sa = 1 / sqrtf(1 - (q[4] * q[4]));
		return CVector3f(q[0] * sa, q[1] * sa, q[2] * sa);
	}

	float CQuaternion::getAngle(void) const
	{
		return (float)2 * acosf(q[3]);
	}

	void CQuaternion::setFromEuler(float fAngleRadiansX, float fAngleRadiansY, float fAngleRadiansZ)
	{
		double dSY = sin(fAngleRadiansZ * 0.5f);
		double dSP = sin(fAngleRadiansY * 0.5f);
		double dSR = sin(fAngleRadiansX * 0.5f);
		double dCY = cos(fAngleRadiansZ * 0.5f);
		double dCP = cos(fAngleRadiansY * 0.5f);
		double dCR = cos(fAngleRadiansX * 0.5f);

		q[0] = float(dSR * dCP * dCY - dCR * dSP * dSY);
		q[1] = float(dCR * dSP * dCY + dSR * dCP * dSY);
		q[2] = float(dCR * dCP * dSY - dSR * dSP * dCY);
		q[3] = float(dCR * dCP * dCY + dSR * dSP * dSY);
		normalise();
	}

	CVector3f CQuaternion::getEuler(void) const
	{
		float fSq[4];
		fSq[0] = q[0] * q[0];
		fSq[1] = q[1] * q[1];
		fSq[2] = q[2] * q[2];
		fSq[3] = q[3] * q[3];

		double m11 = fSq[3] + fSq[0] - fSq[1] - fSq[2];
		double m21 = 2 * (q[0] * q[1] + q[2] * q[3]);
		double m31 = 2 * (q[2] * q[0] - q[1] * q[3]);
		double m32 = 2 * (q[2] * q[1] + q[0] * q[3]);
		double m33 = fSq[3] - fSq[0] - fSq[1] + fSq[2];
		return CVector3f(float(atan2(m32, m33)), asinf(float(-m31)), float(atan2(m21, m11)));
	}

	void CQuaternion::setFromMatrix(const CMatrix& m)
	{
		float fTr = m.m[0] + m.m[5] + m.m[9] + 1.0f;
		// If the trace of the matrix is greater than 0, perform an instant calculation
		if (fTr > 0)
		{
			float fS = 0.5f / sqrtf(fTr);

			q[3] = 0.25f / fS;
			q[0] = (m.m[9] - m.m[6]) * fS;
			q[1] = (m.m[2] - m.m[8]) * fS;
			q[2] = (m.m[4] - m.m[1]) * fS;
		}
		// if the first diagonal element is the largest
		else if (m.m[0] > m.m[5] && m.m[0] > m.m[10])
		{
			float fS = float(1 / (sqrt(m.m[0] - m.m[5] - m.m[10] + 1.0f) * 2));
			q[0] = 0.5f * fS;
			q[1] = (m.m[1] + m.m[4]) * fS;
			q[2] = (m.m[2] + m.m[8]) * fS;
			q[3] = (m.m[6] + m.m[9]) * fS;
		}
		// if the second.....
		else if (m.m[5] > m.m[10])
		{
			float fS = float(1 / (sqrt(m.m[5] - m.m[0] - m.m[10] + 1.0f) * 2));
			q[0] = (m.m[1] + m.m[4]) * fS;
			q[1] = 0.5f * fS;
			q[2] = (m.m[6] + m.m[9]) * fS;
			q[3] = (m.m[2] + m.m[8]) * fS;
		}
		// if the last element is the largest
		else
		{
			float fS = float(1 / (sqrt(m.m[10] - m.m[0] - m.m[5] + 1.0f) * 2));
			q[0] = (m.m[2] + m.m[8]) * fS;
			q[1] = (m.m[6] + m.m[9]) * fS;
			q[2] = 0.5f * fS;
			q[3] = (m.m[1] + m.m[4]) * fS;
		}
	}

	void CQuaternion::rotate(const CQuaternion& quat)
	{
		CQuaternion tmp(quat * (*this) * quat.getConjugate());
		*this = tmp;
	}

	CQuaternion CQuaternion::getSLERP(const CQuaternion& quat, float fInterval) const
	{
		CQuaternion q(quat);

		float fDot = q[0] * q.q[0] + q[1] * q.q[1] + q[2] * q.q[2] + q[3] * q.q[3];
		if (fDot < 0.0f)
		{
			q = q.getNegative();
			fDot = -fDot;
		}

		// Inaccurate, use lerp instead
		if (fDot < 1.00001f && fDot > 0.99999f)
		{
			return getLERP(q, fInterval);
		}

		// Calculate the angle between the quaternions 
		float fTheta = acosf(fDot);

		return ((*this * sinf(fTheta * (1 - fInterval)) + q * sinf(fTheta * fInterval)) / sinf(fTheta));
	}

	CQuaternion CQuaternion::getLERP(const CQuaternion& quat, float fInterval) const
	{
		CQuaternion ret(((quat - *this) * fInterval) + *this);
		ret.normalise();
		return ret;
	}
}