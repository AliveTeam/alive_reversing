#pragma once

#include <SDL3/SDL.h>
#include "Sdl3Context.hpp"
#include "data_conversion/rgb_conversion.hpp"

class Sdl3Texture final
{
public:
    Sdl3Texture(Sdl3Context& context, u32 width, u32 height, SDL_PixelFormat format, SDL_TextureAccess access);
    ~Sdl3Texture();

    static std::shared_ptr<Sdl3Texture> FromMask(Sdl3Context& context, std::shared_ptr<Sdl3Texture> srcTex, const u8* maskPixels);

    u32 GetHeight();
    u32 GetWidth();
    SDL_Texture* GetTexture();
    SDL_Texture* GetTextureUsePalette(const std::shared_ptr<AnimationPal>& palette, const RGBA32& shading, bool isSemiTrans, relive::TBlendModes blendMode);
    void Resize(u32 width, u32 height);
    void SetTextureBlendMode(SDL_BlendMode blendMode);
    void Update(const SDL_Rect* rect, const void* pixels);

private:
    u8 HandleShading(const u8 src, const u8 shade);

private:
    Sdl3Context& mContext;
    SDL_PixelFormat mFormat;
    SDL_Texture* mTexture = 0;
    SDL_TextureAccess mTextureAccess;
    u32 mHeight = 0;
    u32 mWidth = 0;

    // Palette tex related
    u8* mIndexedPixels = 0;
    relive::TBlendModes mLastBlendMode = relive::TBlendModes::eBlend_0;
    u32 mLastPaletteHash = 0;
    bool mLastSemiTransparent = false;
    RGBA32 mLastShadeColor;
};
