#include "Sdl3Context.hpp"

Sdl3Context::Sdl3Context(SDL_Window* window)
    : mWindow(window)
{
    mRenderer = SDL_CreateRenderer(window, NULL);
    if (!mRenderer)
    {
        ALIVE_FATAL("Couldnt create SDL3 renderer: %s", SDL_GetError());
    }

    LOG_INFO("SDL3 renderer name: %s", SDL_GetRendererName(mRenderer));
}

Sdl3Context::~Sdl3Context()
{
}

SDL_Renderer* Sdl3Context::GetRenderer()
{
    return mRenderer;
}

bool Sdl3Context::IsRenderTargetSupported()
{
    SDL_Texture* texture = SDL_CreateTexture(mRenderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, 1, 1);
    const bool isTargetSupported = texture != NULL;

    if (texture)
    {
        SDL_DestroyTexture(texture);
    }

    return isTargetSupported;
}

void Sdl3Context::Present()
{
    SDL_RenderPresent(mRenderer);
}

void Sdl3Context::RestoreFramebuffer()
{
    SDL_SetRenderTarget(mRenderer, mLastFramebuffer);
    mLastFramebuffer = nullptr;

    if (mLastClipRect.x != 0 || mLastClipRect.y != 0 || mLastClipRect.w != 0 || mLastClipRect.h != 0)
    {
        SDL_SetRenderClipRect(mRenderer, &mLastClipRect);
    }
}

void Sdl3Context::SaveFramebuffer()
{
    SDL_GetRenderClipRect(mRenderer, &mLastClipRect);
    mLastFramebuffer = SDL_GetRenderTarget(mRenderer);
    SDL_SetRenderTarget(mRenderer, nullptr);
}

void Sdl3Context::UseScreenFramebuffer()
{
    SDL_SetRenderTarget(mRenderer, nullptr);
}

void Sdl3Context::UseTextureFramebuffer(SDL_Texture* texture)
{
    SDL_SetRenderTarget(mRenderer, texture);
}
