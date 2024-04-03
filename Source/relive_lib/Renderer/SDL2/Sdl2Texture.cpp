#include "Sdl2Texture.hpp"
#include "FatalError.hpp"
#include "Renderer/PaletteCache.hpp"
#include "data_conversion/AnimationConverter.hpp"

Sdl2Texture::Sdl2Texture(Sdl2Context& context, u32 width, u32 height, SDL_PixelFormatEnum format, SDL_TextureAccess access)
    : mContext(context), mFormat(format), mHeight(height), mWidth(width)
{
    // SDL2 does not support palette textures, if we want to store indexed
    // colour we have to handle that internally (using mIndexedPixels)
    if (mFormat == SDL_PIXELFORMAT_INDEX8)
    {
        mIndexedPixels = (u8*) malloc(mWidth * mHeight);
    }
    else
    {
        mTexture = SDL_CreateTexture(mContext.GetRenderer(), mFormat, access, mWidth, mHeight);

        if (!mTexture)
        {
            ALIVE_FATAL("%s", SDL_GetError());
        }
    }
}

Sdl2Texture::~Sdl2Texture()
{
    if (mTexture)
    {
        SDL_DestroyTexture(mTexture);
    }
}

std::shared_ptr<Sdl2Texture> Sdl2Texture::FromMask(Sdl2Context& context, std::shared_ptr<Sdl2Texture> srcTex, const u8* maskPixels)
{
    auto resultTex = std::make_shared<Sdl2Texture>(context, srcTex->mWidth, srcTex->mHeight, srcTex->mFormat, SDL_TEXTUREACCESS_TARGET);

    // Create mask texture
    u8* texPixels;
    s32 pitch;
    SDL_Texture* maskTex = SDL_CreateTexture(context.GetRenderer(), SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, srcTex->mWidth, srcTex->mHeight);

    SDL_LockTexture(maskTex, NULL, (void**) &texPixels, &pitch);

    for (s32 i = 0; i < pitch * static_cast<s32>(srcTex->mHeight); i += 4)
    {
        s32 r = i;
        s32 g = i + 1;
        s32 b = i + 2;
        s32 a = i + 3;

        // Mask out black pixels
        if (maskPixels[r] == 0 && maskPixels[g] == 0 && maskPixels[b] == 0)
        {
            texPixels[a] = 0;
        }
        else
        {
            texPixels[a] = 255;
        }
    }

    SDL_UnlockTexture(maskTex);

    // Paint result texture
    SDL_BlendMode blendMode =
        SDL_ComposeCustomBlendMode(
            SDL_BLENDFACTOR_ZERO,
            SDL_BLENDFACTOR_DST_COLOR,
            SDL_BLENDOPERATION_ADD,
            SDL_BLENDFACTOR_SRC_ALPHA,
            SDL_BLENDFACTOR_ZERO,
            SDL_BLENDOPERATION_ADD
        );

    context.SaveFramebuffer();
    context.UseTextureFramebuffer(resultTex->GetTexture());

    SDL_RenderCopy(context.GetRenderer(), srcTex->GetTexture(), NULL, NULL);

    SDL_SetTextureBlendMode(maskTex, blendMode);
    SDL_RenderCopy(context.GetRenderer(), maskTex, NULL, NULL);

    SDL_SetTextureBlendMode(resultTex->GetTexture(), SDL_BLENDMODE_BLEND);

    // Cleanup
    context.RestoreFramebuffer();
    SDL_DestroyTexture(maskTex);

    return resultTex;
}

SDL_Texture* Sdl2Texture::GetTexture()
{
    if (mFormat == SDL_PIXELFORMAT_INDEX8)
    {
        ALIVE_FATAL("%s", "SDL2 use GetTextureUsePalette with INDEX8 tex");
    }

    return mTexture;
}

SDL_Texture* Sdl2Texture::GetTextureUsePalette(const std::shared_ptr<AnimationPal>& palette)
{
    if (mFormat != SDL_PIXELFORMAT_INDEX8)
    {
        ALIVE_FATAL("%s", "SDL2 attempt to use palette on non-indexed tex");
    }

    // (Re)create temp tex if necessary
    if (mTexture)
    {
        if (PaletteCache::HashPalette(palette.get()) == mLastPaletteHash)
        {
            LOG("%s", "SDL2 palette tex cache hit");
            return mTexture;
        }

        SDL_DestroyTexture(mTexture);
    }

    LOG("%s", "SDL2 palette tex cache miss");
    mTexture = SDL_CreateTexture(mContext.GetRenderer(), SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, mWidth, mHeight);

    // Lock both textures - write indexed colours to temp texture
    u8* pixelsTarget = NULL;
    s32 pitchTarget = 0;

    SDL_LockTexture(mTexture, NULL, (void**) &pixelsTarget, &pitchTarget);

    for (s32 i = 0; i < static_cast<s32>(mWidth * mHeight); i++)
    {
        s32 r = (i * 4);
        s32 g = (i * 4) + 1;
        s32 b = (i * 4) + 2;
        s32 a = (i * 4) + 3;

        RGBA32 colour = palette->mPal[mIndexedPixels[i]];

        pixelsTarget[r] = colour.r;
        pixelsTarget[g] = colour.g;
        pixelsTarget[b] = colour.b;
        pixelsTarget[a] = colour.a;
    }

    SDL_UnlockTexture(mTexture);

    mLastPaletteHash = PaletteCache::HashPalette(palette.get());

    return mTexture;
}

void Sdl2Texture::Update(const SDL_Rect* rect, const void* pixels)
{
    if (mFormat == SDL_PIXELFORMAT_INDEX8)
    {
        memcpy(mIndexedPixels, pixels, mWidth * mHeight);
    }
    else
    {
        u32 pitch = 0;

        switch (mFormat)
        {
            case SDL_PIXELFORMAT_RGBA32:
                pitch = mWidth * 4;
                break;

            default:
                ALIVE_FATAL("SDL2 - Unsupported texture format %d", mFormat);
                break;
        }

        SDL_UpdateTexture(mTexture, rect, pixels, pitch);
    }
}
