#ifndef COLOR__H_
#define COLOR__H_

typedef unsigned char BYTE;

// class Color
// stores rgb values normalized between [0...1]

class Vec3;

typedef class Color
{
public:
	Color():r(0), g(0), b(0) {}
	Color(const float r, const float g, const float b):r(r), g(g), b(b) {}
	Color(const Vec3 &rhs);

	Color &operator+=(const Color &rhs); //memberwise color addition, subtraction, multiplication
	Color operator+(const Color &rhs) const;
	Color &operator-=(const Color &rhs);
	Color operator-(const Color &rhs) const;
	Color &operator*=(const Color &rhs);
	Color operator*(const Color &rhs) const;

	Color &operator*=(const Vec3 &rhs); //memberwise multiplication by vector
	Color operator*(const Vec3 &rhs) const;

	Color &operator*=(const float &rhs); //scalar multiplication/division
	Color operator*(const float &rhs) const;
	Color &operator/=(const float &rhs);
	Color operator/(const float &rhs) const;

	BYTE *toHex(); //translate from normalized -> rgb ([0...255]) colors
	Color clamp(); //normalize colors
private:
	float r, g, b;
} Color;

#endif