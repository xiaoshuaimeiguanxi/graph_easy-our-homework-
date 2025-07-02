#include <SDL.h>
#include "gray_processor.h"

void GrayProcessor::apply_effect(SDL_Surface*& surface, ProcessorType id)
{
    if (surface == nullptr) return; // 空Surface直接返回
    if (id != ProcessorType::Gray)return;//判断是否是灰度处理

    // 1. 锁定Surface，确保可修改像素
    if (SDL_LockSurface(surface) < 0) {
        // 锁定失败可打印错误（可选）
        SDL_Log("SDL_LockSurface failed: %s", SDL_GetError());
        return;
    }

    int width = surface->w;    // 图像宽度
    int height = surface->h;  // 图像高度
    int bpp = surface->format->BytesPerPixel; // 每个像素的字节数（如RGB24是3，RGBA32是4）

    for (int i = 0; i < height; i++)//遍历像素
        for (int j = 0; j < width; j++)
        {
            Uint8* pixel_ptr = (Uint8*)surface->pixels + i * surface->pitch + j * bpp;//surface->pitch 是 每行的实际字节数

            Uint32 pixel = 0;
            for (int k = 0; k < bpp; k++)
            {
                pixel |= (Uint32)pixel_ptr[k] << (k * 8);//每个字节8位，转32位按位或合并
            }

            Uint8 r, g, b, a;
            SDL_GetRGBA(pixel, surface->format, &r, &g, &b, &a);//从 pixel（32 位无符号整数）中，自动解析当前 Surface 格式的 R、G、B、Alpha 分量。

            Uint8 gray = static_cast<Uint8>(0.299 * r + 0.587 * g + 0.114 * b);
            Uint32 new_pixel = SDL_MapRGBA(surface->format, gray, gray, gray, a);

            for (int k = 0; k < bpp; k++)
            {
                pixel_ptr[k] = (Uint8)((new_pixel >> (k * 8)) & 0xFF);//按位与 0xFF，只保留低 8 位（即第 i 个字节的原始值）
            }
        }
    SDL_UnlockSurface(surface);
}