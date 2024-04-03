#pragma once

#include <SDL.h>
#include "Sdl2Context.hpp"

class Sdl2Texture final
{
public:
    Sdl2Texture(Sdl2Context& context, u32 width, u32 height, SDL_PixelFormatEnum format, SDL_TextureAccess access);
    ~Sdl2Texture();

    static std::shared_ptr<Sdl2Texture> FromMask(Sdl2Context& context, std::shared_ptr<Sdl2Texture> srcTex, const u8* maskPixels);

    SDL_Texture* GetTexture();
    SDL_Texture* GetTextureUsePalette(const std::shared_ptr<AnimationPal>& palette);
    void Update(const SDL_Rect* rect, const void* pixels);

private:
    Sdl2Context& mContext;
    SDL_PixelFormatEnum mFormat;
    u8* mIndexedPixels = 0;
    SDL_Texture* mTexture = 0;
    u32 mHeight = 0;
    u32 mWidth = 0;
};
