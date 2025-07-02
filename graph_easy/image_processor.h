#ifndef _IMAGE_PROCESSOR_H_
#define _IMAGE_PROCESSOR_H_

#include <SDL.h>

enum class ProcessorType
{
	None,
	Gray,
	Blur
};

class ImageProcessor
{
public:
	ImageProcessor() = default;
	virtual ~ImageProcessor() {}
	virtual void apply_effect(SDL_Surface*& surface, ProcessorType id) = 0;
};

#endif // !_IMAGE_PROCESSOR_H_
