// edge_processor.h
#ifndef EDGE_PROCESSOR_H
#define EDGE_PROCESSOR_H

#include "image_processor.h"
#include <SDL.h>

/**
 * @class EdgeDetectionProcessor
 * @brief Sobel算子边缘检测处理器
 */
class EdgeDetectionProcessor : public ImageProcessor {
public:
    EdgeDetectionProcessor() = default;
    virtual ~EdgeDetectionProcessor() = default;

    /**
     * @brief 应用边缘检测效果
     * @param surface 输入/输出的SDL表面指针
     */
    void apply_effect(SDL_Surface*& surface) override;

private:
    // Sobel算子内核
    static constexpr int SOBEL_X[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };
    static constexpr int SOBEL_Y[3][3] = {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}
    };

    /**
     * @brief 计算像素梯度值
     * @param pixels 像素数组
     * @param fmt 像素格式
     * @param width 图像宽度
     * @param x,y 当前像素坐标
     * @return 包含RGB梯度值的结构体
     */
    struct Gradient {
        Uint8 r, g, b;
    };
    Gradient calculate_gradient(Uint32* pixels, SDL_PixelFormat* fmt,
        int width, int x, int y) const;
};

#endif // EDGE_PROCESSOR_H