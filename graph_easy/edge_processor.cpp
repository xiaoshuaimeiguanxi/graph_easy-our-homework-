// edge_processor.cpp
#include "edge_processor.h"
#include <algorithm>
#include <cmath>

void EdgeDetectionProcessor::apply_effect(SDL_Surface*& surface) {
    if (!surface) return;

    // 创建临时表面
    SDL_Surface* temp_surface = SDL_CreateRGBSurfaceWithFormat(
        0,
        surface->w,
        surface->h,
        surface->format->BitsPerPixel,
        surface->format->format
    );
    if (!temp_surface) return;

    SDL_LockSurface(surface);
    SDL_LockSurface(temp_surface);

    Uint32* src_pixels = static_cast<Uint32*>(surface->pixels);
    Uint32* dst_pixels = static_cast<Uint32*>(temp_surface->pixels);
    const int width = surface->w;
    const int height = surface->h;

    // 处理每个像素（边缘像素除外）
    for (int y = 1; y < height - 1; ++y) {
        for (int x = 1; x < width - 1; ++x) {
            Gradient grad = calculate_gradient(src_pixels, surface->format, width, x, y);

            // 保留原始alpha通道
            Uint32 original_pixel = src_pixels[y * width + x];
            Uint8 alpha = (original_pixel & surface->format->Amask) >> surface->format->Ashift;

            dst_pixels[y * width + x] = SDL_MapRGBA(
                surface->format,
                grad.r, grad.g, grad.b, alpha
            );
        }
    }

    SDL_UnlockSurface(temp_surface);
    SDL_UnlockSurface(surface);

    // 用处理后的表面替换原表面
    SDL_BlitSurface(temp_surface, nullptr, surface, nullptr);
    SDL_FreeSurface(temp_surface);
}

EdgeDetectionProcessor::Gradient
EdgeDetectionProcessor::calculate_gradient(Uint32* pixels, SDL_PixelFormat* fmt,
    int width, int x, int y) const {
    int gx_r = 0, gx_g = 0, gx_b = 0;
    int gy_r = 0, gy_g = 0, gy_b = 0;

    for (int ky = -1; ky <= 1; ++ky) {
        for (int kx = -1; kx <= 1; ++kx) {
            Uint32 pixel = pixels[(y + ky) * width + (x + kx)];
            Uint8 r, g, b;
            SDL_GetRGB(pixel, fmt, &r, &g, &b);

            const int kernel_x = SOBEL_X[ky + 1][kx + 1];
            const int kernel_y = SOBEL_Y[ky + 1][kx + 1];

            gx_r += r * kernel_x;
            gx_g += g * kernel_x;
            gx_b += b * kernel_x;

            gy_r += r * kernel_y;
            gy_g += g * kernel_y;
            gy_b += b * kernel_y;
        }
    }

    // 计算梯度幅值并限制范围
    auto clamp = [](int value) {
        return static_cast<Uint8>(std::min(255, std::abs(value)));
        };

    return Gradient{
        clamp(gx_r + gy_r),
        clamp(gx_g + gy_g),
        clamp(gx_b + gy_b)
    };
}