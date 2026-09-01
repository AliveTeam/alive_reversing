#include "Sdl2Context.hpp"

Sdl2Context::Sdl2Context(SDL_Window* window)
    : mWindow(window)
{
    mRenderer = SDL_CreateRenderer(window, NULL);
    if (!mRenderer)
    {
        ALIVE_FATAL("Couldnt create SDL3 renderer: %d", SDL_GetError());
    }

    LOG_INFO("SDL3 renderer name: %s", SDL_GetRendererName(mRenderer));
}

Sdl2Context::~Sdl2Context()
{
}

SDL_Renderer* Sdl2Context::GetRenderer()
{
    return mRenderer;
}

bool Sdl2Context::IsRenderTargetSupported()
{
    SDL_Texture* texture = SDL_CreateTexture(mRenderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, 1, 1);
    const bool isTargetSupported = texture != NULL;

    if (texture)
    {
        SDL_DestroyTexture(texture);
    }

    return isTargetSupported;
}

void Sdl2Context::Present()
{
    SDL_RenderPresent(mRenderer);
}

void Sdl2Context::RestoreFramebuffer()
{
    SDL_SetRenderTarget(mRenderer, mLastFramebuffer);
    mLastFramebuffer = nullptr;

    if (mLastClipRect.x != 0 || mLastClipRect.y != 0 || mLastClipRect.w != 0 || mLastClipRect.h != 0)
    {
        SDL_SetRenderClipRect(mRenderer, &mLastClipRect);
    }
}

void Sdl2Context::SaveFramebuffer()
{
    SDL_GetRenderClipRect(mRenderer, &mLastClipRect);
    mLastFramebuffer = SDL_GetRenderTarget(mRenderer);
    SDL_SetRenderTarget(mRenderer, nullptr);
}

void Sdl2Context::UseScreenFramebuffer()
{
    SDL_SetRenderTarget(mRenderer, nullptr);
}

void Sdl2Context::UseTextureFramebuffer(SDL_Texture* texture)
{
    SDL_SetRenderTarget(mRenderer, texture);
}
