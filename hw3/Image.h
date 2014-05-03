#ifndef IMAGE__H_
#define IMAGE__H_

#include <string>
#include "Color.h"

// class Image
// manages a dynamic array of colors
// and outputs them to a png file (using FreeImage library)

typedef class Image 
{
public:
	Image::Image():m_width(0), m_height(0), m_colors(0) {}
	Image(const int width, const int height);
	Image(const Image &rhs);
	Image &operator=(Image rhs);
	Image(Image &&rhs);
	~Image();

	void setPixel(const int i, const int j, const Color &col);
	void writeImage(const std::string &filename);
private:
	friend void swap(Image &rhs, Image &lhs);
	int m_width, m_height;
	Color *m_colors;
} Image;

#endif