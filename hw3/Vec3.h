#ifndef VEC3__H_
#define VEC3__H_

class Point;

// class Vec3
// simple 3d vector class
// supports standard vector operations (add, subtract, scale, cross, dot)

typedef class Vec3
{
public:
	Vec3():x(0), y(0), z(0) {}
	Vec3(const Point &rhs);
	Vec3(const float x, const float y, const float z):x(x), y(y), z(z) {}

	Vec3 &operator+=(const Vec3 &rhs); // vector addition/subtraction
	Vec3 operator+(const Vec3 &rhs) const;
	Vec3 &operator-=(const Vec3 &rhs);
	Vec3 operator-(const Vec3 &rhs) const;

	Vec3 &operator*=(const float &rhs); // float scale operators
	Vec3 operator*(const float &rhs) const;
	Vec3 &operator/=(const float &rhs);
	Vec3 operator/(const float &rhs) const;

	Vec3 normal() const; // normalization and length
	float length() const;
	
	friend float dot(const Vec3 &rhs, const Vec3 &lhs); // vector operations
	friend Vec3 cross(const Vec3 &rhs, const Vec3 &lhs);

	float x, y, z;
} Vec3;

#endif