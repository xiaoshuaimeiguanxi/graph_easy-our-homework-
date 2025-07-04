// edge_processor.h
#ifndef EDGE_PROCESSOR_H
#define EDGE_PROCESSOR_H

#include "image_processor.h"
#include <SDL.h>

/**
 * @class EdgeDetectionProcessor
 * @brief Sobel���ӱ�Ե��⴦����
 */
class EdgeDetectionProcessor : public ImageProcessor {
public:
    EdgeDetectionProcessor() = default;
    virtual ~EdgeDetectionProcessor() = default;

    /**
     * @brief Ӧ�ñ�Ե���Ч��
     * @param surface ����/�����SDL����ָ��
     */
    void apply_effect(SDL_Surface*& surface) override;

private:
    // Sobel�����ں�
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
     * @brief ���������ݶ�ֵ
     * @param pixels ��������
     * @param fmt ���ظ�ʽ
     * @param width ͼ����
     * @param x,y ��ǰ��������
     * @return ����RGB�ݶ�ֵ�Ľṹ��
     */
    struct Gradient {
        Uint8 r, g, b;
    };
    Gradient calculate_gradient(Uint32* pixels, SDL_PixelFormat* fmt,
        int width, int x, int y) const;
};

#endif // EDGE_PROCESSOR_H