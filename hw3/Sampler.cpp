#include "Sampler.h"

// sample()
// params: image width and height, x and y coordinates
// return: normalized x and y coordinates from [-1 ... 1] on image plane
// notes:

Sample Sampler::sample(const int w, const int h, const float x, const float y)
{
	Sample temp;
	temp.x = (x - w / 2.0f) / (w / 2.0f);
	temp.y = (h / 2.0f - y) / (h / 2.0f);
	return temp;
}