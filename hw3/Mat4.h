#ifndef MAT4__H_
#define MAT4__H_

// class Mat4
// supports construction from list of floats (in row-major order), diagonal matrix from single float, or from column vectors, as well as adding homogeneous coords to a mat3
// also provides multiplication by mat4 or vec3
// and creation of some transformation matrices

class Vec3;
class Vec4;
class Mat3;

typedef class Mat4
{
public:
	Mat4();
	Mat4(const float a00, const float a10, const float a20, const float a30, const float a01, const float a11, const float a21, const float a31, const float a02, const float a12, const float a22, const float a32, const float a03, const float a13, const float a23, const float a33);
	Mat4(const float a);
	Mat4(const Vec4 &c1, const Vec4 &c2, const Vec4 &c3, const Vec4 &c4);
	explicit Mat4(const Mat3 &src);

	Mat4 &operator*=(const Mat4 &rhs);
	Mat4 operator*(const Mat4 &rhs) const;
	Vec3 operator*(const Vec3 &rhs) const; //transforms a vec3 by this transformation matrix

	Mat4 transpose() const; // simple matrix helper and debugging operations
	void print() const;

	static Mat4 rotation(const float deg, const Vec3 &axis); // creation of transformation matrices (uses axis-angle formula for rotation)
	static Mat4 translation(const float x, const float y, const float z);
	static Mat4 scale(const float x, const float y, const float z);
public:
	float m_data[4][4];
} Mat4;

float radians(const float deg); //converts degrees to radians

#endif