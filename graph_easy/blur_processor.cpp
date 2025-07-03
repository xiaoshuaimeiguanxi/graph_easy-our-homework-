#include "blur_processor.h"
#include<vector>
#include<algorithm>
#include<cstring>


void BlurProcessor::apply_effect(SDL_Surface*& surface)
{
	if (!surface) return;

	// 强制复制为 RGBA32 格式（安全、匹配 OpenGL）
	SDL_Surface* converted = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);
	if (!converted) return;

	SDL_FreeSurface(surface);  // 释放旧的
	surface = converted;       // 替换为新 surface


	apply_effect(surface, 10);
}
void BlurProcessor::apply_effect(SDL_Surface*& surface, int radius)
{
	if (!surface || radius<=0)return;

	SDL_LockSurface(surface);

	int w = surface->w;
	int h = surface->h;
	Uint32* pixels = (Uint32*)surface->pixels;

	SDL_PixelFormat* fmt = surface->format;

	std::vector<Uint32> temp(w * h); //中间灰度缓存

	int size = 2 * radius + 1;

	//横向模糊
	for (int y = 0; y < h; ++y)
	{
		int rsum = 0, gsum = 0, bsum = 0, asum = 0;

		//初始窗口
		for (int i= -radius; i <= radius; ++i)
		{
			int x = clamp(i, 0, w - 1);
			Uint8 r, g, b, a;
			SDL_GetRGBA(pixels[y * w + x], fmt, &r, &g, &b, &a);
			rsum += r; gsum += g; bsum += b, asum += a;
		}

		for (int x = 0; x < w; ++x)
		{
			Uint8 r_avg = rsum / size;
			Uint8 g_avg = gsum / size;
			Uint8 b_avg = bsum / size;
			Uint8 a_avg = asum / size;
			temp[y * w + x] = SDL_MapRGBA(fmt, r_avg, g_avg, b_avg, a_avg);

			//滑动窗口更新
			int x_remove = clamp(x - radius, 0, w - 1);
			int x_add = clamp(x + radius, 0, w - 1);

			Uint8 r1, g1, b1, a1, r2, g2, b2, a2;
			SDL_GetRGBA(pixels[y * w + x_remove], fmt, &r1, &g1, &b1,&a1);
			SDL_GetRGBA(pixels[y * w + x_add], fmt, &r2, &g2, &b2, &a2);

			rsum += r2 - r1;
			gsum += g2 - g1;
			bsum += b2 - b1;
			asum += a2 - a1;
		}
	}

	//纵向模糊+写入最终图像
	for (int x = 0; x < w; ++x)
	{
		int rsum = 0, gsum = 0, bsum = 0, asum = 0;

		//初始窗口
		for (int i = -radius; i <= radius; ++i)
		{
			int y = clamp(i, 0, h - 1);
			Uint8 r, g, b, a;
			SDL_GetRGBA(temp[y * w + x], fmt, &r, &g, &b, &a);
			rsum += r; gsum += g; bsum += b; asum += a;
		}

		for (int y = 0; y < h; ++y)
		{
			Uint8 r_avg = rsum / size;
			Uint8 g_avg = gsum / size;
			Uint8 b_avg = bsum / size;
			Uint8 a_avg = asum / size;
			pixels[y * w + x] = SDL_MapRGBA(fmt, r_avg, g_avg, b_avg, a_avg);
			
			//滑动窗口更新
			int y_remove = clamp(y - radius - 1, 0, h - 1);
			int y_add = clamp(y + radius, 0, h - 1);
			
			Uint8 r1, g1, b1, a1, r2, g2, b2, a2;
			SDL_GetRGBA(temp[y_remove * w + x], fmt, &r1, &g1, &b1, &a1);
			SDL_GetRGBA(temp[y_add * w + x], fmt, &r2, &g2, &b2, &a2);

			rsum += r2 - r1;
			gsum += g2 - g1;
			bsum += b2 - b1;
			asum += a2 - a1;
		}
	}

	SDL_UnlockSurface(surface);
}