#ifndef _BLUR_PROCESSOR_H_
#define _BLER_PROCESSOR_H_

#include "image_processor.h"

class BlurProcessor :public ImageProcessor
{
public:
	void apply_effect(SDL_Surface*& surface)override ;
	void apply_effect(SDL_Surface*& surface, int radius);

private:
	template<typename T>         //±‡“Î∆˜√ª”–ºÊ»›µΩclamp
	T clamp(T val, T minVal, T maxVal)
	{
		return (val < minVal) ? minVal : (val > maxVal) ? maxVal : val;
	}
};

#endif // !_BLUR_PROCESSOR_H_
