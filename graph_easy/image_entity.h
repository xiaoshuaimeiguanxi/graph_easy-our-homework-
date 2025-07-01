#ifndef _IMAGE_ENTITY_H_
#define _IMAGE_ENTITY_H_

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <vector>
#include <string>

#include "image_processor.h"

class ImageEntity
{
public:
    ImageEntity() :surface(nullptr), gl_texture(0) {}
    ~ImageEntity()
    {
        for (ImageProcessor* p : processors)
        {
            delete p;
        }
        if (surface) SDL_FreeSurface(surface);
        if (gl_texture) glDeleteTextures(1, &gl_texture);
    }

    bool load_from_file(const char* path);
    void add_processor(ImageProcessor* processor);
    void process_all();
    void update_texture();

    SDL_Surface* get_surface() const { return surface; }
    GLuint get_texture_id() const { return gl_texture; }

private:
    SDL_Surface* surface;
    GLuint gl_texture;
    std::vector<ImageProcessor*> processors;
};

#endif