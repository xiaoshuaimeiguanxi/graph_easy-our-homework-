#include "blur_processor.h"
#include<vector>
#include<algorithm>
#include<cstring>


void BlurProcessor::apply_effect(SDL_Surface*& surface)
{
	apply_effect(surface, 3);
}
void BlurProcessor::apply_effect(SDL_Surface*& surface, int radius)
{
	if (!surface || radius<=0)return;

	SDL_LockSurface(surface);

	int w = surface->w;
	int h = surface->h;
	Uint32* pixels = (Uint32*)surface->pixels;

	SDL_PixelFormat* fmt = surface->format;

	std::vector<Uint8> gray_temp(w * h); //中间灰度缓存

	int size = 2 * radius + 1;

	//横向灰度模糊
	for (int y = 0; y < h; ++y)
	{
		int sum = 0;

		//初始窗口
		for (int i= -radius; i <= radius; ++i)
		{
			int x = clamp(i, 0, w - 1);
			Uint8 r, g, b;
			SDL_GetRGB(pixels[y*w+x],fmt, &r, &g, &b);
			sum += (r + g + b) / 3;
		}

		for (int x = 0; x < w; ++x)
		{
			gray_temp[y * w + x] = sum / size;

			int x_remove = clamp(x - radius, 0, w - 1);
			int x_add = clamp(x + radius, 0, w - 1);

			Uint8 r1, g1, b1, r2, g2, b2;
			SDL_GetRGB(pixels[y * w + x_remove], fmt, &r1, &g1, &b1);
			SDL_GetRGB(pixels[y * w + x_add], fmt, &r2, &g2, &b2);

			sum += ((r2 + g2 + b2) / 3) - ((r1 + g1 + b1) / 3);
		}
	}

	//纵向模糊+写入最终图像
	for (int x = 0; x < w; ++x)
	{
		int sum = 0;

		//初始窗口
		for (int i = -radius; i <= radius; ++i)
		{
			int y = clamp(i, 0, h - 1);
			sum += gray_temp[y * w + x];
		}

		for (int y = 0; y < h; ++y)
		{
			Uint8 gray = sum / size;
			pixels[y * w + x] = SDL_MapRGB(fmt, gray, gray, gray);
			
			int y_remove = clamp(y - radius - 1, 0, h - 1);
			int y_add = clamp(y + radius, 0, h - 1);
			
			sum += gray_temp[y_add * w + x] - gray_temp[y_remove * w + x];
		}
	}

	SDL_UnlockSurface(surface);
}