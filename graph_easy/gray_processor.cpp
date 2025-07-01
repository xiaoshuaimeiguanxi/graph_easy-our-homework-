#include <SDL.h>

#include "gray_processor.h"

void GrayProcessor::apply_effect(SDL_Surface*& surface)
{
    if (!surface)
        return;

    SDL_LockSurface(surface);

    Uint32* pixels = (Uint32*)surface->pixels;
    int width = surface->w;
    int height = surface->h;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            // 获取原始像素
            Uint32 pixel = pixels[y * width + x];
            Uint8 r, g, b, a;
            SDL_GetRGBA(pixel, surface->format, &r, &g, &b, &a);

            // 计算灰度值（保留原始alpha通道）
            Uint8 gray = static_cast<Uint8>(0.299 * r + 0.587 * g + 0.114 * b);
            Uint32 new_pixel = SDL_MapRGBA(surface->format, gray, gray, gray, a);

            // 原地修改像素
            pixels[y * width + x] = new_pixel;
        }
    }

    SDL_UnlockSurface(surface);
}