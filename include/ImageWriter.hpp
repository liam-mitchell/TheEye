// ==========================================================================
// Image Writing using STB Library
//  - requires stb_image_write.h
//
//  How to use:
//     ImageWriter image_writer(1000,1000);
//     image_writer.set(x,y,r,g,b);
//     image_writer.save("file.png");
//
//
// Author: Kamyar Allahverdi
//         University of Calgary
// Date:   November 2016
// ==========================================================================
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <cstring>
#include <vector>

class ImageWriter {
  using uchar=unsigned char;
  using Image=std::vector<uchar>;
  Image image;
  void init() {
    image = Image(height*width*4);
  }
public:
  int width;
  int height;
  ImageWriter():width(200),height(200) {
    init();
  }
  ImageWriter(int w,int h):width(w),height(h) {
    init();
  }
  void set(int x, int y, int r, int g, int b) {
    image[(y*width+x)*4+0]=r;
    image[(y*width+x)*4+1]=g;
    image[(y*width+x)*4+2]=b;
    image[(y*width+x)*4+3]=255;
  }
  uchar *get_data() {
    return (uchar*)image.data();
  }
  void save(const char *filename) {
    stbi_write_png(filename, width, height, 4, (uchar*)get_data(), width*4*sizeof(uchar));
  }
  // OpenGL reads pixel data bottom-to-top.
  // stb saves top-to-bottom.
  // Flip the pixels along the Y axis, then save.
  void save_opengl(const char *filename) {
    Image flipped(width * height * 4);

    for (int y = 0; y < height; ++y) {
      int srcy = height - 1 - y;

      std::memcpy(flipped.data() + (y * width * 4), image.data() + (srcy * width * 4), width * 4);
    }

    stbi_write_png(filename, width, height, 4, flipped.data(), width*4*sizeof(uchar));
  }
};
