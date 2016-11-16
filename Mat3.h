#ifndef MAT3__H_
#define MAT3__H_

// class Mat3
// basic matrix class, supports construction from list of floats (in row-major order), construction of diagonal matrix from float and construction from column vectors
// as well as matrix multiplication and memberwise addition

class Vec3;

typedef class Mat3
{
public:
	Mat3(const float a00, const float a10, const float a20, const float a01, const float a11, const float a21, const float a02, const float a12, const float a22);
	Mat3(const float a);
	Mat3(const Vec3 &c1, const Vec3 &c2, const Vec3 &c3);
	Mat3 &operator*=(const Mat3 &rhs);
	Mat3 operator*(const Mat3 &rhs) const;
	Mat3 &operator*=(const float rhs);
	Mat3 operator*(const float rhs) const;
	Mat3 &operator+=(const Mat3 &rhs);
	Mat3 operator+(const Mat3 &rhs) const;
public:
	float m_data[3][3];
} Mat3;

#endif //MAT3__H_