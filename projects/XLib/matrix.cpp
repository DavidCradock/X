#include "PCH.h"
#include "matrix.h"
#include "utilities.h"
#include "log.h"
#include "vector4f.h"

namespace X
{
	CMatrix::CMatrix()
	{
		setIdentity();
	}

	const void CMatrix::operator =(const CMatrix& matrix)
	{
		memcpy(m, matrix.m, sizeof(float[16]));
	}

	const CMatrix CMatrix::operator *(const CMatrix& n) const
	{
		CMatrix r;
		r.m[0] = m[0] * n.m[0] + m[4] * n.m[1] + m[8] * n.m[2] + m[12] * n.m[3];
		r.m[1] = m[1] * n.m[0] + m[5] * n.m[1] + m[9] * n.m[2] + m[13] * n.m[3];
		r.m[2] = m[2] * n.m[0] + m[6] * n.m[1] + m[10] * n.m[2] + m[14] * n.m[3];
		r.m[3] = m[3] * n.m[0] + m[7] * n.m[1] + m[11] * n.m[2] + m[15] * n.m[3];

		r.m[4] = m[0] * n.m[4] + m[4] * n.m[5] + m[8] * n.m[6] + m[12] * n.m[7];
		r.m[5] = m[1] * n.m[4] + m[5] * n.m[5] + m[9] * n.m[6] + m[13] * n.m[7];
		r.m[6] = m[2] * n.m[4] + m[6] * n.m[5] + m[10] * n.m[6] + m[14] * n.m[7];
		r.m[7] = m[3] * n.m[4] + m[7] * n.m[5] + m[11] * n.m[6] + m[15] * n.m[7];

		r.m[8] = m[0] * n.m[8] + m[4] * n.m[9] + m[8] * n.m[10] + m[12] * n.m[11];
		r.m[9] = m[1] * n.m[8] + m[5] * n.m[9] + m[9] * n.m[10] + m[13] * n.m[11];
		r.m[10] = m[2] * n.m[8] + m[6] * n.m[9] + m[10] * n.m[10] + m[14] * n.m[11];
		r.m[11] = m[3] * n.m[8] + m[7] * n.m[9] + m[11] * n.m[10] + m[15] * n.m[11];

		r.m[12] = m[0] * n.m[12] + m[4] * n.m[13] + m[8] * n.m[14] + m[12] * n.m[15];
		r.m[13] = m[1] * n.m[12] + m[5] * n.m[13] + m[9] * n.m[14] + m[13] * n.m[15];
		r.m[14] = m[2] * n.m[12] + m[6] * n.m[13] + m[10] * n.m[14] + m[14] * n.m[15];
		r.m[15] = m[3] * n.m[12] + m[7] * n.m[13] + m[11] * n.m[14] + m[15] * n.m[15];
		return r;
	}

	const void CMatrix::operator *= (const CMatrix& n)
	{
		*this = *this * n;
	}

	bool CMatrix::operator==(const CMatrix& n) const
	{
		return	(m[0] == n.m[0]) && (m[1] == n.m[1]) && (m[2] == n.m[2]) && (m[3] == n.m[3]) &&
			(m[4] == n.m[4]) && (m[5] == n.m[5]) && (m[6] == n.m[6]) && (m[7] == n.m[7]) &&
			(m[8] == n.m[8]) && (m[9] == n.m[9]) && (m[10] == n.m[10]) && (m[11] == n.m[11]) &&
			(m[12] == n.m[12]) && (m[13] == n.m[13]) && (m[14] == n.m[14]) && (m[15] == n.m[15]);
	}

	bool CMatrix::operator!=(const CMatrix& n) const
	{
		return	(m[0] != n.m[0]) || (m[1] != n.m[1]) || (m[2] != n.m[2]) || (m[3] != n.m[3]) ||
			(m[4] != n.m[4]) || (m[5] != n.m[5]) || (m[6] != n.m[6]) || (m[7] != n.m[7]) ||
			(m[8] != n.m[8]) || (m[9] != n.m[9]) || (m[10] != n.m[10]) || (m[11] != n.m[11]) ||
			(m[12] != n.m[12]) || (m[13] != n.m[13]) || (m[14] != n.m[14]) || (m[15] != n.m[15]);
	}

	void CMatrix::setIdentity(void)
	{
		m[0] = m[5] = m[10] = m[15] = 1.0f;
		m[1] = m[2] = m[3] = m[4] = m[6] = m[7] = m[8] = m[9] = m[11] = m[12] = m[13] = m[14] = 0.0f;
	}

	void CMatrix::setZero(void)
	{
		m[0] = m[1] = m[2] = m[3] = m[4] = m[5] = m[6] = m[7] = m[8] = m[9] = m[10] = m[11] = m[12] = m[13] = m[14] = m[15] = 0.0f;
	}

	void CMatrix::set(const float src[16])
	{
		m[0] = src[0];  m[1] = src[1];  m[2] = src[2];  m[3] = src[3];
		m[4] = src[4];  m[5] = src[5];  m[6] = src[6];  m[7] = src[7];
		m[8] = src[8];  m[9] = src[9];  m[10] = src[10]; m[11] = src[11];
		m[12] = src[12]; m[13] = src[13]; m[14] = src[14]; m[15] = src[15];
	}

	void CMatrix::set(const CMatrix& src)
	{
		m[0] = src.m[0];	m[1] = src.m[1];	m[2] = src.m[2];	m[3] = src.m[3];
		m[4] = src.m[4];	m[5] = src.m[5];	m[6] = src.m[6];	m[7] = src.m[7];
		m[8] = src.m[8];	m[9] = src.m[9];	m[10] = src.m[10];	m[11] = src.m[11];
		m[12] = src.m[12];	m[13] = src.m[13];	m[14] = src.m[14];	m[15] = src.m[15];
	}

	void CMatrix::set(const glm::mat4& matrix)
	{
		m[0] = matrix[0][0];
		m[1] = matrix[0][1];
		m[2] = matrix[0][2];
		m[3] = matrix[0][3];
		m[4] = matrix[1][0];
		m[5] = matrix[1][1];
		m[6] = matrix[1][2];
		m[7] = matrix[1][3];
		m[8] = matrix[2][0];
		m[9] = matrix[2][1];
		m[10] = matrix[2][2];
		m[11] = matrix[2][3];
		m[12] = matrix[3][0];
		m[13] = matrix[3][1];
		m[14] = matrix[3][2];
		m[15] = matrix[3][3];
	}

	CMatrix CMatrix::get(void) const
	{
		CMatrix mt;
		mt.m[0] = m[0];		mt.m[1] = m[1];		mt.m[2] = m[2];		mt.m[3] = m[3];
		mt.m[4] = m[4];		mt.m[5] = m[5];		mt.m[6] = m[6];		mt.m[7] = m[7];
		mt.m[8] = m[8];		mt.m[9] = m[9];		mt.m[10] = m[10];	mt.m[11] = m[11];
		mt.m[12] = m[12];	mt.m[13] = m[13];	mt.m[14] = m[14];	mt.m[15] = m[15];
		return mt;
	}

	glm::mat4 CMatrix::getGLM(void) const
	{
		glm::mat4 glm;
		glm[0][0] = m[0];
		glm[0][1] = m[1];
		glm[0][2] = m[2];
		glm[0][3] = m[3];
		glm[1][0] = m[4];
		glm[1][1] = m[5];
		glm[1][2] = m[6];
		glm[1][3] = m[7];
		glm[2][0] = m[8];
		glm[2][1] = m[9];
		glm[2][2] = m[10];
		glm[2][3] = m[11];
		glm[3][0] = m[12];
		glm[3][1] = m[13];
		glm[3][2] = m[14];
		glm[3][3] = m[15];
		return glm;
	}

	void CMatrix::setTranslation(float fX, float fY, float fZ)
	{
		m[12] = fX;
		m[13] = fY;
		m[14] = fZ;
	}

	void CMatrix::setTranslation(const CVector3f& vVec)
	{
		m[12] = vVec.x;
		m[13] = vVec.y;
		m[14] = vVec.z;
	}

	void CMatrix::setScale(float fX, float fY, float fZ)
	{
		m[0] = fX;
		m[5] = fY;
		m[10] = fZ;
	}

	void CMatrix::setScale(const CVector3f& vVec)
	{
		m[0] = vVec.x;
		m[5] = vVec.y;
		m[10] = vVec.z;
	}

	void CMatrix::setFromAxisAngleDegrees(const CVector3f& vAxis, float fAngleDegrees)
	{
		float fAngleRadians = deg2rad(fAngleDegrees);
		float fCos = cosf(fAngleRadians);
		float fSin = sinf(fAngleRadians);
		float fOMC = 1.0f - fCos;

		m[0] = fCos + (vAxis.x * vAxis.x) * fOMC;
		m[5] = fCos + (vAxis.y * vAxis.y) * fOMC;
		m[10] = fCos + (vAxis.z * vAxis.z) * fOMC;
		m[15] = 1.0f;
		m[4] = vAxis.x * vAxis.y * fOMC + vAxis.z * fSin;
		m[1] = vAxis.x * vAxis.y * fOMC - vAxis.z * fSin;
		m[8] = vAxis.x * vAxis.z * fOMC + vAxis.y * fSin;
		m[2] = vAxis.x * vAxis.z * fOMC - vAxis.y * fSin;
		m[9] = vAxis.y * vAxis.z * fOMC + vAxis.x * fSin;
		m[6] = vAxis.y * vAxis.z * fOMC - vAxis.x * fSin;
	}

	void CMatrix::setFromAxisAngleRadians(const CVector3f& vAxis, float fAngleRadians)
	{
		float fCos = cosf(fAngleRadians);
		float fSin = sinf(fAngleRadians);
		float fOMC = 1.0f - fCos;

		m[0] = fCos + (vAxis.x * vAxis.x) * fOMC;
		m[5] = fCos + (vAxis.y * vAxis.y) * fOMC;
		m[10] = fCos + (vAxis.z * vAxis.z) * fOMC;
		m[15] = 1.0f;
		m[4] = vAxis.x * vAxis.y * fOMC + vAxis.z * fSin;
		m[1] = vAxis.x * vAxis.y * fOMC - vAxis.z * fSin;
		m[8] = vAxis.x * vAxis.z * fOMC + vAxis.y * fSin;
		m[2] = vAxis.x * vAxis.z * fOMC - vAxis.y * fSin;
		m[9] = vAxis.y * vAxis.z * fOMC + vAxis.x * fSin;
		m[6] = vAxis.y * vAxis.z * fOMC - vAxis.x * fSin;
	}

	void CMatrix::setFromQuaternion(const CQuaternion& q)
	{
		m[0] = 1.0f - 2.0f * (q.q[1] * q.q[1] + q.q[2] * q.q[2]);
		m[1] = 2.0f * (q.q[0] * q.q[1] - q.q[2] * q.q[3]);
		m[2] = 2.0f * (q.q[0] * q.q[2] + q.q[1] * q.q[3]);

		m[4] = 2.0f * (q.q[0] * q.q[1] + q.q[2] * q.q[3]);
		m[5] = 1.0f - 2.0f * (q.q[0] * q.q[0] + q.q[2] * q.q[2]);
		m[6] = 2.0f * (q.q[1] * q.q[2] - q.q[0] * q.q[3]);

		m[8] = 2.0f * (q.q[0] * q.q[2] - q.q[1] * q.q[3]);
		m[9] = 2.0f * (q.q[1] * q.q[2] + q.q[0] * q.q[3]);
		m[10] = 1.0f - 2.0f * (q.q[0] * q.q[0] + q.q[1] * q.q[1]);

		m[15] = 1.0f;
	}

	void CMatrix::getRightVector(CVector3f& vVec) const
	{
		vVec.x = m[0];
		vVec.y = m[1];
		vVec.z = m[2];
	}

	void CMatrix::getUpVector(CVector3f& vVec) const
	{
		vVec.x = m[4];
		vVec.y = m[5];
		vVec.z = m[6];
	}

	void CMatrix::getForwardVector(CVector3f& vVec) const
	{
		vVec.x = m[8];
		vVec.y = m[9];
		vVec.z = m[10];
	}

	CMatrix CMatrix::transpose(void)
	{
		CMatrix mt;
		mt.m[0] = m[0];		mt.m[1] = m[4];		mt.m[2] = m[8];		mt.m[3] = m[12];
		mt.m[4] = m[1];		mt.m[5] = m[5];		mt.m[6] = m[9];		mt.m[7] = m[13];
		mt.m[8] = m[2];		mt.m[9] = m[6];		mt.m[10] = m[10];	mt.m[11] = m[14];
		mt.m[12] = m[3];	mt.m[13] = m[7];	mt.m[14] = m[11];	mt.m[15] = m[15];
		return mt;
	}

	CMatrix CMatrix::multiply(const CMatrix& n)
	{
		CMatrix r;
		r.m[0] = m[0] * n.m[0] + m[4] * n.m[1] + m[8] * n.m[2] + m[12] * n.m[3];
		r.m[1] = m[1] * n.m[0] + m[5] * n.m[1] + m[9] * n.m[2] + m[13] * n.m[3];
		r.m[2] = m[2] * n.m[0] + m[6] * n.m[1] + m[10] * n.m[2] + m[14] * n.m[3];
		r.m[3] = m[3] * n.m[0] + m[7] * n.m[1] + m[11] * n.m[2] + m[15] * n.m[3];

		r.m[4] = m[0] * n.m[4] + m[4] * n.m[5] + m[8] * n.m[6] + m[12] * n.m[7];
		r.m[5] = m[1] * n.m[4] + m[5] * n.m[5] + m[9] * n.m[6] + m[13] * n.m[7];
		r.m[6] = m[2] * n.m[4] + m[6] * n.m[5] + m[10] * n.m[6] + m[14] * n.m[7];
		r.m[7] = m[3] * n.m[4] + m[7] * n.m[5] + m[11] * n.m[6] + m[15] * n.m[7];

		r.m[8] = m[0] * n.m[8] + m[4] * n.m[9] + m[8] * n.m[10] + m[12] * n.m[11];
		r.m[9] = m[1] * n.m[8] + m[5] * n.m[9] + m[9] * n.m[10] + m[13] * n.m[11];
		r.m[10] = m[2] * n.m[8] + m[6] * n.m[9] + m[10] * n.m[10] + m[14] * n.m[11];
		r.m[11] = m[3] * n.m[8] + m[7] * n.m[9] + m[11] * n.m[10] + m[15] * n.m[11];

		r.m[12] = m[0] * n.m[12] + m[4] * n.m[13] + m[8] * n.m[14] + m[12] * n.m[15];
		r.m[13] = m[1] * n.m[12] + m[5] * n.m[13] + m[9] * n.m[14] + m[13] * n.m[15];
		r.m[14] = m[2] * n.m[12] + m[6] * n.m[13] + m[10] * n.m[14] + m[14] * n.m[15];
		r.m[15] = m[3] * n.m[12] + m[7] * n.m[13] + m[11] * n.m[14] + m[15] * n.m[15];
		return r;
	}

	CVector3f CMatrix::multiply(const CVector3f& v)
	{
		CVector3f r;
		r.x = m[0] * v.x + m[4] * v.y + m[8] * v.z + m[12];
		r.y = m[1] * v.x + m[5] * v.y + m[9] * v.z + m[13];
		r.z = m[2] * v.x + m[6] * v.y + m[10] * v.z + m[14];
		return r;
	}

	CMatrix CMatrix::multiply(const float fScalar)
	{
		CMatrix r = *this;
		r.m[0] *= fScalar;	r.m[1] *= fScalar;	r.m[2] *= fScalar;	r.m[3] *= fScalar;
		r.m[4] *= fScalar;	r.m[5] *= fScalar;	r.m[6] *= fScalar;	r.m[7] *= fScalar;
		r.m[8] *= fScalar;	r.m[9] *= fScalar;	r.m[10] *= fScalar;	r.m[11] *= fScalar;
		r.m[12] *= fScalar;	r.m[13] *= fScalar;	r.m[14] *= fScalar;	r.m[15] *= fScalar;
		return r;
	}

	void CMatrix::setProjectionPerspective(
		float fFieldOfViewInDegrees,
		float fNearClippingPlaneDistance,
		float fFarClippingPlaneDistance,
		float fAspectRatio)
	{
		float fFOVRadians = deg2rad(fFieldOfViewInDegrees);
		float fTanHalfFOV = tan(fFOVRadians * 0.5f);
		float fNearMinusFar = fNearClippingPlaneDistance - fFarClippingPlaneDistance;
		ThrowIfTrue(fNearMinusFar == 0.0f, "CMatrix::setProjectionPerspective() failed. Given near and far clipping plane distances invalid.");

		// First column
		m[0] = 1.0f / (fAspectRatio * fTanHalfFOV);
		m[1] = 0.0f;
		m[2] = 0.0f;
		m[3] = 0.0f;

		// Second column
		m[4] = 0.0f;
		m[5] = 1.0f / fTanHalfFOV;
		m[6] = 0.0f;
		m[7] = 0.0f;

		// Third column
		m[8] = 0.0f;
		m[9] = 0.0f;
		m[10] = fFarClippingPlaneDistance / (fNearMinusFar);
		m[11] = -1.0f;

		// Fourth column
		m[12] = 0.0f;
		m[13] = 0.0f;
		m[14] = -(fFarClippingPlaneDistance * fNearClippingPlaneDistance) / (fFarClippingPlaneDistance - fNearClippingPlaneDistance);
		m[15] = 1.0f;	// 0.0f?
	}

	void CMatrix::setProjectionOrthographic(
		float fLeft,
		float fRight,
		float fTop,
		float fBottom,
		float fNear,
		float fFar)
	{
		// First column
		m[0] = 2.0f / (fRight - fLeft);
		m[1] = 0.0f;
		m[2] = 0.0f;
		m[3] = 0.0f;

		// Second column
		m[4] = 0.0f;
		m[5] = 2.0f / (fTop - fBottom);
		m[6] = 0.0f;
		m[7] = 0.0f;

		// Third column
		m[8] = 0.0f;
		m[9] = 0.0f;
		m[10] = -1.0f / (fFar - fNear);
		m[11] = 0.0f;

		// Fourth column
		m[12] = -(fRight + fLeft) / (fRight - fLeft);
		m[13] = -(fTop + fBottom) / (fTop - fBottom);
		m[14] = -fNear / (fFar - fNear);
		m[15] = 1.0f;
	}

	void CMatrix::setViewLookat(const CVector3f& vEyePos, const CVector3f& vTarget, const CVector3f &vUp)
	{
		CVector3f vf = vTarget - vEyePos;		vf.normalise();
		CVector3f vs = vf.getCross(vUp);		vs.normalise();
		CVector3f vu = vs.getCross(vf);			vu.normalise(); // No need to normalise as other two vectors already are.

		// First column
		m[0] = vs.x;
		m[1] = vu.x;
		m[2] = -vf.x;
		m[3] = 0.0f;

		// Second column
		m[4] = vs.y;
		m[5] = vu.y;
		m[6] = -vf.y;
		m[7] = 0.0f;

		// Third column
		m[8] = vs.z;
		m[9] = vu.z;
		m[10] = -vf.z;
		m[11] = 0.0f;

		// Fourth column
		m[12] = -vs.getDot(vEyePos);
		m[13] = -vu.getDot(vEyePos);
		m[14] = vf.getDot(vEyePos);
		m[15] = 1.0f;
	}

	const float* CMatrix::getFloat(void) const
	{
		return &m[0];
	}

	CMatrix CMatrix::inverse(void)
	{
		float coef00 = m[10] * m[15] - m[14] * m[11];
		float coef02 = m[6] * m[15] - m[14] * m[7];
		float coef03 = m[6] * m[11] - m[10] * m[7];
		float coef04 = m[9] * m[15] - m[13] * m[11];
		float coef06 = m[5] * m[15] - m[13] * m[7];
		float coef07 = m[5] * m[11] - m[9] * m[7];
		float coef08 = m[9] * m[14] - m[13] * m[10];
		float coef10 = m[5] * m[14] - m[13] * m[6];
		float coef11 = m[5] * m[10] - m[9] * m[6];
		float coef12 = m[8] * m[15] - m[12] * m[11];
		float coef14 = m[4] * m[15] - m[12] * m[7];
		float coef15 = m[4] * m[11] - m[8] * m[7];
		float coef16 = m[8] * m[14] - m[12] * m[10];
		float coef18 = m[4] * m[14] - m[12] * m[6];
		float coef19 = m[4] * m[10] - m[8] * m[6];
		float coef20 = m[8] * m[13] - m[12] * m[9];
		float coef22 = m[4] * m[13] - m[12] * m[5];
		float coef23 = m[4] * m[9] - m[8] * m[5];

		CVector4f fac0(coef00, coef00, coef02, coef03);
		CVector4f fac1(coef04, coef04, coef06, coef07);
		CVector4f fac2(coef08, coef08, coef10, coef11);
		CVector4f fac3(coef12, coef12, coef14, coef15);
		CVector4f fac4(coef16, coef16, coef18, coef19);
		CVector4f fac5(coef20, coef20, coef22, coef23);

		CVector4f vec0(m[4], m[0], m[0], m[0]);
		CVector4f vec1(m[5], m[1], m[1], m[1]);
		CVector4f vec2(m[6], m[2], m[2], m[2]);
		CVector4f vec3(m[7], m[3], m[3], m[3]);
		
		CVector4f inv0 = vec1 * fac0 - vec2 * fac1 + vec3 * fac2;
		CVector4f inv1 = vec0 * fac0 - vec2 * fac3 + vec3 * fac4;
		CVector4f inv2 = vec0 * fac1 - vec1 * fac3 + vec3 * fac5;
		CVector4f inv3 = vec0 * fac2 - vec1 * fac4 + vec2 * fac5;
		
		CVector4f signA(+1, -1, +1, -1);
		CVector4f signB(-1, +1, -1, +1);

		CMatrix matinv;
		CVector4f vResult0 = inv0 * signA;
		CVector4f vResult1 = inv1 * signB;
		CVector4f vResult2 = inv2 * signA;
		CVector4f vResult3 = inv3 * signB;
		matinv.m[0] = vResult0.x;	matinv.m[1] = vResult0.y;	matinv.m[2] = vResult0.z;	matinv.m[3] = vResult0.w;
		matinv.m[4] = vResult1.x;	matinv.m[5] = vResult1.y;	matinv.m[6] = vResult1.z;	matinv.m[7] = vResult1.w;
		matinv.m[8] = vResult2.x;	matinv.m[9] = vResult2.y;	matinv.m[10] = vResult2.z;	matinv.m[11] = vResult2.w;
		matinv.m[12] = vResult3.x;	matinv.m[13] = vResult3.y;	matinv.m[14] = vResult3.z;	matinv.m[15] = vResult3.w;

		CVector4f row0(matinv.m[0], matinv.m[4], matinv.m[8], matinv.m[12]);
		CVector4f dot0 = row0 * m[0];
		float fDot1 = (dot0.x + dot0.y) + (dot0.z + dot0.w);
		float fOneOverDeterminant = 1.0f / fDot1;
		return matinv.multiply(fOneOverDeterminant);
	}

	CVector3f CMatrix::getTranslation(void) const
	{
		return CVector3f(m[12], m[13], m[14]);
	}
}