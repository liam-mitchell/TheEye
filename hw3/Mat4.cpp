#include "Mat4.h"
#include "Mat3.h"
#include "Vec3.h"
#include "Vec4.h"
#include <iostream>
#include <math.h>

Mat4::Mat4(const float a00, const float a10, const float a20, const float a30, const float a01, const float a11, const float a21, const float a31, const float a02, const float a12, const float a22, const float a32, const float a03, const float a13, const float a23, const float a33)
{
	m_data[0][0] = a00;
	m_data[0][1] = a01;
	m_data[0][2] = a02;
	m_data[0][3] = a03;

	m_data[1][0] = a10;
	m_data[1][1] = a11;
	m_data[1][2] = a12;
	m_data[1][3] = a13;

	m_data[2][0] = a20;
	m_data[2][1] = a21;
	m_data[2][2] = a22;
	m_data[2][3] = a23;

	m_data[3][0] = a30;
	m_data[3][1] = a31;
	m_data[3][2] = a32;
	m_data[3][3] = a33;
}

Mat4::Mat4(const Vec4 &c1, const Vec4 &c2, const Vec4 &c3, const Vec4 &c4)
{
	m_data[0][0] = c1.x;
	m_data[0][1] = c1.y;
	m_data[0][2] = c1.z;
	m_data[0][3] = c1.w;

	m_data[1][0] = c2.x;
	m_data[1][1] = c2.y;
	m_data[1][2] = c2.z;
	m_data[1][3] = c2.w;

	m_data[2][0] = c3.x;
	m_data[2][1] = c3.y;
	m_data[2][2] = c3.z;
	m_data[2][3] = c3.w;

	m_data[3][0] = c4.x;
	m_data[3][1] = c4.y;
	m_data[3][2] = c4.z;
	m_data[3][3] = c4.w;
}

Mat4::Mat4()
{
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			m_data[i][j] = 0;		
		}
	}
}

Mat4::Mat4(const float a)
{
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			m_data[i][j] = 0;		
		}

		m_data[i][i] = a;
	}
}

Mat4::Mat4(const Mat3 &src)
{
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			m_data[i][j] = src.m_data[i][j];
		}

		m_data[i][3] = 0;
	}

	for (int i = 0; i < 3; ++i)
	{
		m_data[3][i] = 0;
	}

	m_data[3][3] = 1;
}

Mat4 &Mat4::operator*=(const Mat4 &rhs) //right-multiplies rhs onto this
{
	Vec4 r0(m_data[0][0], m_data[1][0], m_data[2][0], m_data[3][0]); //create row vectors for first matrix
	Vec4 r1(m_data[0][1], m_data[1][1], m_data[2][1], m_data[3][1]);
	Vec4 r2(m_data[0][2], m_data[1][2], m_data[2][2], m_data[3][2]);
	Vec4 r3(m_data[0][3], m_data[1][3], m_data[2][3], m_data[3][3]);

	Vec4 c0(rhs.m_data[0][0], rhs.m_data[0][1], rhs.m_data[0][2], rhs.m_data[0][3]); //and column vectors for second
	Vec4 c1(rhs.m_data[1][0], rhs.m_data[1][1], rhs.m_data[1][2], rhs.m_data[1][3]);
	Vec4 c2(rhs.m_data[2][0], rhs.m_data[2][1], rhs.m_data[2][2], rhs.m_data[2][3]);
	Vec4 c3(rhs.m_data[3][0], rhs.m_data[3][1], rhs.m_data[3][2], rhs.m_data[3][3]);

	m_data[0][0] = dot(r0, c0); //dot into appropriate cells of result
	m_data[1][0] = dot(r0, c1);
	m_data[2][0] = dot(r0, c2);
	m_data[3][0] = dot(r0, c3);

	m_data[0][1] = dot(r1, c0);
	m_data[1][1] = dot(r1, c1);
	m_data[2][1] = dot(r1, c2);
	m_data[3][1] = dot(r1, c3);

	m_data[0][2] = dot(r2, c0);
	m_data[1][2] = dot(r2, c1);
	m_data[2][2] = dot(r2, c2);
	m_data[3][2] = dot(r2, c3);

	m_data[0][3] = dot(r3, c0);
	m_data[1][3] = dot(r3, c1);
	m_data[2][3] = dot(r3, c2);
	m_data[3][3] = dot(r3, c3);

	return *this;
}

Mat4 Mat4::operator*(const Mat4 &rhs) const
{
	Mat4 temp = *this;
	temp *= rhs;
	return temp;
}

Vec3 Mat4::operator*(const Vec3 &rhs) const
{
	Vec4 rhs4(rhs.x, rhs.y, rhs.z, 1.0f);

	Vec4 r0(m_data[0][0], m_data[1][0], m_data[2][0], m_data[3][0]);
	Vec4 r1(m_data[0][1], m_data[1][1], m_data[2][1], m_data[3][1]);
	Vec4 r2(m_data[0][2], m_data[1][2], m_data[2][2], m_data[3][2]);

	Vec3 temp;
	temp.x = dot(r0, rhs4);
	temp.y = dot(r1, rhs4);
	temp.z = dot(r2, rhs4);

	return temp;
}

Mat4 Mat4::transpose() const
{
	Mat4 temp;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			temp.m_data[i][j] = m_data[j][i];
		}
	}

	return temp;
}

Mat4 Mat4::rotation(const float deg, const Vec3 &axis)
{
	Mat3 i(1.0f);
	
	Mat3 atdota(axis.x * axis.x, axis.x * axis.y, axis.x * axis.z, 
				axis.x * axis.y, axis.y * axis.y, axis.y * axis.z,
				axis.x * axis.z, axis.y * axis.z, axis.z * axis.z);

	Mat3 across(0, -axis.z, axis.y,
				axis.z, 0, -axis.x,
				-axis.y, axis.x, 0);

	Mat3 rot = i * cos(radians(deg)) + atdota * (1 - cos(radians(deg))) + across * sin(radians(deg));
	return Mat4(rot);
}

Mat4 Mat4::translation(const float x, const float y, const float z)
{
	Vec4 c1(1, 0, 0, 0), c2(0, 1, 0, 0), c3(0, 0, 1, 0), c4(x, y, z, 1);
	return Mat4 (c1, c2, c3, c4);
}

Mat4 Mat4::scale(const float x, const float y, const float z)
{
	Vec4 c1(x, 0, 0, 0), c2(0, y, 0, 0), c3(0, 0, z, 0), c4(0, 0, 0, 1);
	return Mat4(c1, c2, c3, c4);
}

void Mat4::print() const
{
	std::cout << "Matrix at " << this << " has values:\n";

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			std::cout.precision(3);
			std::cout.width(5);
			std::cout.fill('0');
			std::cout << m_data[j][i] << "  ";
		}

		std::cout << std::endl;
	}
}