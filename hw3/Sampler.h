#ifndef SAMPLER__H_
#define SAMPLER__H_

#include "Image.h"

// class Sample
// small class responsible for converting pixel coords into normalized image plane coords in range [-1 ... 1]
// and helper struct Sample to hold said coords

typedef struct Sample
{
	float x, y;
} Sample;

typedef class Sampler
{
public:
	static Sample sample(const int w, const int h, const float x, const float y);
private:

} Sampler;

#endif