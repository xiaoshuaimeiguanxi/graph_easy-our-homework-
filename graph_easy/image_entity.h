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
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, u8"��������ʧ��", u8"����ͼ��ʧ��", window);
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

    bool load_from_file(const char* path);//�ӱ��ؼ���ͼƬ
    void add_processor(ImageProcessor* processor, ProcessorType id);//��Ӵ�����
    void process_all();//��������ִ�к���
    void update_texture();//OpenGL��Ҫ����������GPU�Դ��У������������ת���ʹ������

    SDL_Surface* get_surface() const { return surface; }
    GLuint get_texture_id() const { return gl_texture; }

private:
    SDL_Surface* surface;  // SDLͼ����棨CPU�ڴ��е��������ݣ�
    GLuint gl_texture;// OpenGL����ID��GPU�Դ��е�ͼ�����ݣ�
    std::vector<std::pair< ImageProcessor*, ProcessorType>> processors;//�������洢�ֿ�
};

#endif