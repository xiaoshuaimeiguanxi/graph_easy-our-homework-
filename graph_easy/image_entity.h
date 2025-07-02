#ifndef _IMAGE_ENTITY_H_
#define _IMAGE_ENTITY_H_

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <vector>
#include <string>

#include "image_processor.h"

extern SDL_Window* window;

class ImageEntity
{
public:
    ImageEntity() :surface(nullptr), gl_texture(0) {}
    ImageEntity(const char* path) 
    {
        surface = IMG_Load(path);
        if (!surface)
        {
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, u8"程序启动失败", u8"加载图像失败", window);
        }
        update_texture();
    }
    ~ImageEntity()
    {
        for (auto& p : processors)
        {
            delete p.first;
        }
        if (surface) SDL_FreeSurface(surface);
        if (gl_texture) glDeleteTextures(1, &gl_texture);
    }

    bool load_from_file(const char* path);//从本地加载图片
    void add_processor(ImageProcessor* processor, ProcessorType id);//添加处理器
    void process_all();//处理器的执行函数
    void update_texture();//OpenGL需要纹理数据在GPU显存中，负责这个数据转换和传输过程

    SDL_Surface* get_surface() const { return surface; }
    GLuint get_texture_id() const { return gl_texture; }

private:
    SDL_Surface* surface;  // SDL图像表面（CPU内存中的像素数据）
    GLuint gl_texture;// OpenGL纹理ID（GPU显存中的图像数据）
    std::vector<std::pair< ImageProcessor*, ProcessorType>> processors;//处理器存储仓库
};

#endif