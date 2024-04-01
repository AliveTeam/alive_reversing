#include "Sdl2Texture.hpp"
#include "FatalError.hpp"

Sdl2Texture::Sdl2Texture(Sdl2Context& context, u32 width, u32 height, SDL_PixelFormatEnum format, SDL_TextureAccess access)
    : mContext(context), mFormat(format), mHeight(height), mWidth(width)
{
    mTexture = SDL_CreateTexture(mContext.GetRenderer(), mFormat, access, mWidth, mHeight);
}

Sdl2Texture::~Sdl2Texture()
{
    SDL_DestroyTexture(mTexture);
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

    SDL_SetTextureBlendMode(srcTex->GetTexture(), SDL_BLENDMODE_NONE);
    SDL_RenderCopy(context.GetRenderer(), srcTex->GetTexture(), NULL, NULL);

    SDL_SetTextureBlendMode(maskTex, blendMode);
    SDL_RenderCopy(context.GetRenderer(), maskTex, NULL, NULL);

    context.RestoreFramebuffer();

    SDL_SetTextureBlendMode(resultTex->GetTexture(), SDL_BLENDMODE_BLEND);

    return resultTex;
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
