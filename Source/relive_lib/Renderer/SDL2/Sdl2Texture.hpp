#pragma once

#include <SDL.h>

class Sdl2Texture final
{
public:
    Sdl2Texture(SDL_Renderer* renderer, u32 width, u32 height, SDL_PixelFormatEnum format, SDL_TextureAccess access);
    ~Sdl2Texture();

    SDL_Texture* GetTexture();
    void Update(const SDL_Rect* rect, const void* pixels);

private:
    SDL_PixelFormatEnum mFormat;
    SDL_Renderer* mRenderer = 0;
    SDL_Texture* mTexture = 0;
    u32 mHeight = 0;
    u32 mWidth = 0;
};
