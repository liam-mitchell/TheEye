#include "Color.h"
#include "Vec3.h"

Color::Color(const Vec3 &rhs)
{
	r = rhs.x;
	g = rhs.y;
	b = rhs.z;
}

Color &Color::operator+=(const Color &rhs)
{
	r += rhs.r;
	g += rhs.g;
	b += rhs.b;

	return *this;
}

Color Color::operator+(const Color &rhs) const
{
	Color temp = *this;
	temp += rhs;
	return temp;
}

Color &Color::operator-=(const Color &rhs)
{
	r -= rhs.r;
	g -= rhs.g;
	b -= rhs.b;

	return *this;
}

Color Color::operator-(const Color &rhs) const 
{
	Color temp = *this;
	temp -= rhs;
	return temp;
}

Color &Color::operator*=(const Color &rhs)
{
	r *= rhs.r;
	g *= rhs.g;
	b *= rhs.b;

	return *this;
}

Color Color::operator*(const Color &rhs) const
{
	Color temp = *this;
	temp *= rhs;
	return temp;
}

Color &Color::operator*=(const Vec3 &rhs)
{
	r *= rhs.x;
	g *= rhs.y;
	b *= rhs.z;

	return *this;
}

Color Color::operator*(const Vec3 &rhs) const 
{
	Color temp = *this;
	temp *= rhs;
	return temp;
}

Color &Color::operator*=(const float &rhs)
{
	r *= rhs;
	g *= rhs;
	b *= rhs;

	return *this;
}

Color Color::operator*(const float &rhs) const 
{
	Color temp = *this;
	temp *= rhs;
	return temp;
}

Color &Color::operator/=(const float &rhs)
{
	if (rhs != 0)
	{
		r /= rhs;
		g /= rhs;
		b /= rhs;
	}
	return *this;
}

Color Color::operator/(const float &rhs) const 
{
	Color temp = *this;
	temp /= rhs;
	return temp;
}

// clamp()
// params: none 
// return: color with rgb clamped in the range [0...1]
// notes:

Color Color::clamp()
{
	Color temp = *this;
	if (temp.r > 1.0f) temp.r = 1.0f;
	if (temp.g > 1.0f) temp.g = 1.0f;
	if (temp.b > 1.0f) temp.b = 1.0f;
	return temp;
}

// toHex()
// params: none 
// return: rgb color from [0...1] -> [0...255]
// notes: for writing colors to file

BYTE *Color::toHex()
{
	BYTE *colors = new BYTE[3];
	colors[0] = (int)(b * 255);
	colors[1] = (int)(g * 255);
	colors[2] = (int)(r * 255);
	return colors;
}