#include "image_entity.h"

#include <SDL.h>
#include <SDL_image.h>

extern SDL_Window* window;

bool ImageEntity::load_from_file(const char* path)
{
	surface = IMG_Load(path);
	if (!surface)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, u8"错误", u8"加载图像失败", window);
	}
	update_texture();
	return true;
}

void ImageEntity::add_processor(ImageProcessor* processor)
{
	processors.emplace_back(processor);
}

void ImageEntity::process_all()
{
	for (ImageProcessor* p : processors)
	{
		p->apply_effect(surface);
	}
	update_texture();
}

void ImageEntity::update_texture()
{
    SDL_PixelFormat* format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32);
    SDL_Surface* converted = SDL_ConvertSurface(surface, format, 0);
    SDL_FreeFormat(format);

    if (!converted) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, u8"错误", u8"转换文件格式失败", window);
    }

    glGenTextures(1, &gl_texture);
    glBindTexture(GL_TEXTURE_2D, gl_texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
        converted->w, converted->h, 0,
        GL_RGBA, GL_UNSIGNED_BYTE,
        converted->pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


    SDL_FreeSurface(converted);
    glBindTexture(GL_TEXTURE_2D, 0);
}