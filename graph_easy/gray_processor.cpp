#include<SDL.h>
#include"gray_processor.h"

void GrayProcessor::apply_effect(SDL_Surface*& surface)
{
	if (!surface)return;

	//锁定表面以访问像素数据
	SDL_LockSurface(surface);

	Uint8 r, g, b;
	Uint32* pixels = static_cast<Uint32*>(surface->pixels);
	const int pixel_count = surface->w * surface->h;

	//获取表面像素格式信息
	SDL_PixelFormat* fmt = surface->format;
	const Uint32 alpha_mask = fmt->Amask;

	//处理每个像素
	for (int i = 0; i < pixel_count; i++) {
		Uint32 pixel = pixels[i];

		//提取RGB分量
		SDL_GetRGB(pixel, fmt, &r, &g, &b);

		//计算灰度值（使用加权平均值）
		Uint8 gray = static_cast<Uint8>(0.299 * r + 0.587 * g + 0.144 * b);

		//保留原始alpha通道
		Uint32 alpha = pixel & alpha_mask;

		//创建新的灰度像素
		pixels[i] = SDL_MapRGBA(fmt, gray, gray, gray, alpha >> fmt->Ashift);
	}

	//解锁表面
	SDL_UnlockSurface(surface);
}