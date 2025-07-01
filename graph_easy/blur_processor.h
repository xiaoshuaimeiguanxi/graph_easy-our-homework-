#ifndef _BLUR_PROCESSOR_H_
#define _BLER_PROCESSOR_H_

#include "image_processor.h"

class BlurProcessor :public ImageProcessor
{
public:
	void apply_effect(SDL_Surface*& surface) override;
};

#endif // !_BLUR_PROCESSOR_H_
