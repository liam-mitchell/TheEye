#include "Image.h"
#include "FreeImage.h"
#include <iostream>

Image::Image(const int width, const int height):m_width(width), m_height(height)
{
	m_colors = new Color[width * height];
}

Image::Image(const Image &rhs):m_width(rhs.m_width), m_height(rhs.m_height)
{
	if (m_colors) {
		delete [] m_colors;
		m_colors = nullptr;
	}
	if (rhs.m_colors) m_colors = new Color[m_width * m_height];

	for (int j = 0; j < m_height; ++j)
	{
		for (int i = 0; i < m_width; ++i)
		{
			m_colors[i + j * m_width] = rhs.m_colors[i + j * m_width];
		}
	}
}

// move-constructor and assignment-operator
// params: 
// return: 
// notes: both use the (nothrow) swap friend function - exception-safe


Image::Image(Image &&rhs):m_colors(0), m_width(0), m_height(0)
{
	swap(*this, rhs);
}

Image &Image::operator=(Image rhs)
{
	swap(*this, rhs);
	return *this;
}

// setPixel()
// params: coords of pixel on image, color to set
// return: none
// notes:

void Image::setPixel(const int i, const int j, const Color &col)
{
	if (i > m_width || j > m_height)
	{
		std::cerr << "Sample outside image bounds!\n";
	} else
	{
		m_colors[i + m_width * j] = col;
	}

}

// writeImage()
// params: filename to output
// return: none
// notes: uses the FreeImage library to save images - docs available online

void Image::writeImage(const std::string &filename)
{
	BYTE *pixels = new BYTE[3 * m_width * m_height]; //allocate space for rgb * number of pixels
	for (int j = 0; j < m_height; ++j)
	{
		for (int i = 0; i < m_width; ++i)
		{
			BYTE *col = m_colors[i + j * m_width].toHex(); 
			for (int k = 0; k < 3; ++k)
			{
				pixels[3 * (i + j * m_width) + k] = col[k]; //store rgb in pixel array sequentially
			}
			delete [] col; //delete the byte array returned by toHex()
		}
	}

	FIBITMAP *img = FreeImage_ConvertFromRawBits(pixels, m_width, m_height, m_width * 3, 24, 0xFF0000, 0x00FF00, 0x0000FF, false);
	std::cout << "Saving screenshot! \n" << filename << std::endl;
	FreeImage_Save(FIF_PNG, img, filename.c_str(), 0);
	delete [] pixels;
}

// swap()
// params: images to swap
// return: none
// notes: nothrow guarantee - used in move-cons and assignment

void swap(Image &rhs, Image &lhs)
{
	using std::swap;

	swap(rhs.m_width, lhs.m_width);
	swap(rhs.m_height, lhs.m_height);
	swap(rhs.m_colors, lhs.m_colors);
}

Image::~Image()
{
	if (m_colors) delete [] m_colors;
}
