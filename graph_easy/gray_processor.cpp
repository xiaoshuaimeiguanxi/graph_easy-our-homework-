#include <SDL.h>

#include "gray_processor.h"

void GrayProcessor::apply_effect(SDL_Surface*& surface)
{
	if (!surface)return;
	
	//锁定表面（如果需要）
	if (SDL_MUSTLOCK(surface))SDL_LockSurface(surface);

	int width = surface->w;
	int height = surface->h;
	int pitch = surface->pitch; //一行的字节数

	Uint8 r, g, b;
	Uint32* pixels = (Uint32*)surface->pixels;

	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			Uint32* pixel = (Uint32*)((Uint8*)surface->pixels + y * pitch + x * sizeof(Uint32));
			SDL_GetRGB(*pixel, surface->format, &r, &g, &b);

			//加权平均法计算灰度值
			Uint8 gray = static_cast<Uint8>(0.299 * r + 0.587 * g + 0.144 * b);

			*pixel = SDL_MapRGB(surface->format, gray, gray, gray);
		}
	}

	if (SDL_MUSTLOCK(surface))SDL_UnlockSurface(surface);
}
