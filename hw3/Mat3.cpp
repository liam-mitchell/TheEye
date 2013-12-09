#include "Mat3.h"
#include "Vec3.h"

Mat3::Mat3(const float a)
{
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			m_data[i][j] = 0;		
		}

		m_data[i][i] = a;
	}
}

Mat3::Mat3(const float a00, const float a10, const float a20, const float a01, const float a11, const float a21, const float a02, const float a12, const float a22)
{
	m_data[0][0] = a00;
	m_data[0][1] = a01;
	m_data[0][2] = a02;

	m_data[1][0] = a10;
	m_data[1][1] = a11;
	m_data[1][2] = a12;

	m_data[2][0] = a20;
	m_data[2][1] = a21;
	m_data[2][2] = a22;
}

Mat3::Mat3(const Vec3 &c1, const Vec3 &c2, const Vec3 &c3)
{
	m_data[0][0] = c1.x;
	m_data[0][1] = c1.y;
	m_data[0][2] = c1.z;

	m_data[1][0] = c2.x;
	m_data[1][1] = c2.y;
	m_data[1][2] = c2.z;

	m_data[2][0] = c3.x;
	m_data[2][1] = c3.y;
	m_data[2][2] = c3.z;
}

Mat3 &Mat3::operator*=(const Mat3 &rhs) //right-multiplies rhs onto this
{
	Vec3 r0(m_data[0][0], m_data[1][0], m_data[2][0]); //create row vectors for first matrix
	Vec3 r1(m_data[0][1], m_data[1][1], m_data[2][1]);
	Vec3 r2(m_data[0][2], m_data[1][2], m_data[2][2]);

	Vec3 c0(rhs.m_data[0][0], rhs.m_data[0][1], rhs.m_data[0][2]); //and column vectors for second matrix
	Vec3 c1(rhs.m_data[1][0], rhs.m_data[1][1], rhs.m_data[1][2]);
	Vec3 c2(rhs.m_data[2][0], rhs.m_data[2][1], rhs.m_data[2][2]);

	m_data[0][0] = dot(r0, c0); //dot appropriate rows and columns into result
	m_data[1][0] = dot(r0, c1);
	m_data[2][0] = dot(r0, c2);

	m_data[0][1] = dot(r1, c0);
	m_data[1][1] = dot(r1, c1);
	m_data[2][1] = dot(r1, c2);

	m_data[0][2] = dot(r2, c0);
	m_data[1][2] = dot(r2, c1);
	m_data[2][2] = dot(r2, c2);

	return *this;
}

Mat3 Mat3::operator*(const Mat3 &rhs) const
{
	Mat3 temp = *this;
	temp *= rhs;
	return temp;
}

Mat3 &Mat3::operator*=(const float rhs)
{
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			m_data[i][j] *= rhs;
		}
	}

	return *this;
}

Mat3 Mat3::operator*(const float rhs) const
{
	Mat3 temp = *this;
	temp *= rhs;
	return temp;
}

Mat3 &Mat3::operator+=(const Mat3 &rhs)
{
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			m_data[i][j] += rhs.m_data[i][j];
		}
	}

	return *this;
}

Mat3 Mat3::operator+(const Mat3 &rhs) const
{
	Mat3 temp = *this;
	temp += rhs;
	return temp;
}