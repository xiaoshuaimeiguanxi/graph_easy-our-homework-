#ifndef _GRAY_PROCESSOR_H_
#define _GRAY_PROCESSOR_H_

#include "image_processor.h"

class GrayProcessor:public ImageProcessor
{
public:
	void apply_effect(SDL_Surface*& surface) override;
};

#endif // !_GRAY_PROCESSOR_H_
