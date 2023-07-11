#include "PCH.h"
#include "matrix.h"

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

	CMatrix CMatrix::get(void) const
	{
		CMatrix mt;
		mt.m[0] = m[0];		mt.m[1] = m[1];		mt.m[2] = m[2];		mt.m[3] = m[3];
		mt.m[4] = m[4];		mt.m[5] = m[5];		mt.m[6] = m[6];		mt.m[7] = m[7];
		mt.m[8] = m[8];		mt.m[9] = m[9];		mt.m[10] = m[10];	mt.m[11] = m[11];
		mt.m[12] = m[12];	mt.m[13] = m[13];	mt.m[14] = m[14];	mt.m[15] = m[15];
		return mt;
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

	void CMatrix::setFromAxisAngle(const CVector3f& vAxis, float fAngle)
	{
		float fCos = cosf(fAngle);
		float fSin = sinf(fAngle);
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
}