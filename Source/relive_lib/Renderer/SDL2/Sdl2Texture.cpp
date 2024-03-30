#include "Sdl2Texture.hpp"
#include "FatalError.hpp"

Sdl2Texture::Sdl2Texture(SDL_Renderer* renderer, u32 width, u32 height, SDL_PixelFormatEnum format, SDL_TextureAccess access)
    : mFormat(format), mRenderer(renderer), mHeight(height), mWidth(width)
{
    mTexture = SDL_CreateTexture(mRenderer, mFormat, access, mWidth, mHeight);
}

Sdl2Texture::~Sdl2Texture()
{
    SDL_DestroyTexture(mTexture);
}

SDL_Texture* Sdl2Texture::GetTexture()
{
    return mTexture;
}

void Sdl2Texture::Update(const SDL_Rect* rect, const void* pixels)
{
    u32 pitch = mWidth;

    switch (mFormat)
    {
        case SDL_PIXELFORMAT_RGBA32:
            pitch *= 4;
            break;

        default:
            ALIVE_FATAL("SDL2 - Unsupported texture format %d", mFormat);
            break;
    }

    SDL_UpdateTexture(mTexture, rect, pixels, pitch);
}
