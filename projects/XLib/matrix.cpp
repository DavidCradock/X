#include "PCH.h"
#include "matrix.h"

namespace X
{
	CMatrix::CMatrix()
	{

	}

	CMatrix::CMatrix(
		float m00, float m01, float m02, float m03,
		float m04, float m05, float m06, float m07,
		float m08, float m09, float m10, float m11,
		float m12, float m13, float m14, float m15)
	{
		m[0] = m00;		m[1] = m01;		m[2] = m02;		m[3] = m03;
		m[4] = m04;		m[5] = m05;		m[6] = m06;		m[7] = m07;
		m[8] = m08;		m[9] = m09;		m[10] = m10;	m[11] = m11;
		m[12] = m12;	m[13] = m13;	m[14] = m14;	m[15] = m15;
	}

	bool CMatrix::operator==(const CMatrix& other) const
	{
		return	(m[0] == other.m[0])	&& (m[1] == other.m[1])		&&	(m[2] == other.m[2])	&&	(m[3] == other.m[3]) &&
				(m[4] == other.m[4])	&& (m[5] == other.m[5])		&& (m[6] == other.m[6])		&& (m[7] == other.m[7]) &&
				(m[8] == other.m[8])	&& (m[9] == other.m[9])		&& (m[10] == other.m[10])	&& (m[11] == other.m[11]) &&
				(m[12] == other.m[12])	&& (m[13] == other.m[13])	&& (m[14] == other.m[14])	&& (m[15] == other.m[15]);
	}

	bool CMatrix::operator!=(const CMatrix& other) const
	{
		return	(m[0] != other.m[0])	|| (m[1] != other.m[1])		|| (m[2] != other.m[2])		|| (m[3] != other.m[3]) ||
				(m[4] != other.m[4])	|| (m[5] != other.m[5])		|| (m[6] != other.m[6])		|| (m[7] != other.m[7]) ||
				(m[8] != other.m[8])	|| (m[9] != other.m[9])		|| (m[10] != other.m[10])	|| (m[11] != other.m[11]) ||
				(m[12] != other.m[12])	|| (m[13] != other.m[13])	|| (m[14] != other.m[14])	|| (m[15] != other.m[15]);
	}

	void CMatrix::setIdentity(void)
	{
		m[0] = m[5] = m[10] = m[15] = 1.0f;
		m[1] = m[2] = m[3] = m[4] = m[6] = m[7] = m[8] = m[9] = m[11] = m[12] = m[13] = m[14] = 0.0f;
	}

	void CMatrix::setZero(void)
	{
		m[0] = m[1] = m[2] = m[3] =	m[4] = m[5] = m[6] = m[7] = m[8] = m[9] = m[10] = m[11] = m[12] = m[13] = m[14] = m[15] = 0.0f;
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

	CMatrix CMatrix::get(void)
	{
		CMatrix mt;
		mt.m[0] = m[0];		mt.m[1] = m[1];		mt.m[2] = m[2];		mt.m[3] = m[3];
		mt.m[4] = m[4];		mt.m[5] = m[5];		mt.m[6] = m[6];		mt.m[7] = m[7];
		mt.m[8] = m[8];		mt.m[9] = m[9];		mt.m[10] = m[10];	mt.m[11] = m[11];
		mt.m[12] = m[12];	mt.m[13] = m[13];	mt.m[14] = m[14];	mt.m[15] = m[15];
		return mt;
	}

	void CMatrix::getRightVector(CVector3f& vVec)
	{
		vVec.x = m[0];
		vVec.y = m[1];
		vVec.z = m[2];
	}

	void CMatrix::getUpVector(CVector3f& vVec)
	{
		vVec.x = m[4];
		vVec.y = m[5];
		vVec.z = m[6];
	}

	void CMatrix::getForwardVector(CVector3f& vVec)
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

	CMatrix CMatrix::multiply(const CMatrix& other)
	{
		CMatrix r;
		r.m[0] = m[0] * other.m[0] + m[4] * other.m[1] + m[8] * other.m[2] + m[12] * other.m[3];
		r.m[1] = m[1] * other.m[0] + m[5] * other.m[1] + m[9] * other.m[2] + m[13] * other.m[3];
		r.m[2] = m[2] * other.m[0] + m[6] * other.m[1] + m[10] * other.m[2] + m[14] * other.m[3];
		r.m[3] = m[3] * other.m[0] + m[7] * other.m[1] + m[11] * other.m[2] + m[15] * other.m[3];

		r.m[4] = m[0] * other.m[4] + m[4] * other.m[5] + m[8] * other.m[6] + m[12] * other.m[7];
		r.m[5] = m[1] * other.m[4] + m[5] * other.m[5] + m[9] * other.m[6] + m[13] * other.m[7];
		r.m[6] = m[2] * other.m[4] + m[6] * other.m[5] + m[10] * other.m[6] + m[14] * other.m[7];
		r.m[7] = m[3] * other.m[4] + m[7] * other.m[5] + m[11] * other.m[6] + m[15] * other.m[7];

		r.m[8] = m[0] * other.m[8] + m[4] * other.m[9] + m[8] * other.m[10] + m[12] * other.m[11];
		r.m[9] = m[1] * other.m[8] + m[5] * other.m[9] + m[9] * other.m[10] + m[13] * other.m[11];
		r.m[10] = m[2] * other.m[8] + m[6] * other.m[9] + m[10] * other.m[10] + m[14] * other.m[11];
		r.m[11] = m[3] * other.m[8] + m[7] * other.m[9] + m[11] * other.m[10] + m[15] * other.m[11];

		r.m[12] = m[0] * other.m[12] + m[4] * other.m[13] + m[8] * other.m[14] + m[12] * other.m[15];
		r.m[13] = m[1] * other.m[12] + m[5] * other.m[13] + m[9] * other.m[14] + m[13] * other.m[15];
		r.m[14] = m[2] * other.m[12] + m[6] * other.m[13] + m[10] * other.m[14] + m[14] * other.m[15];
		r.m[15] = m[3] * other.m[12] + m[7] * other.m[13] + m[11] * other.m[14] + m[15] * other.m[15];
		return r;
	}
}