#include "Sdl2Context.hpp"

Sdl2Context::Sdl2Context(SDL_Window* window)
    : mWindow(window)
{
    mRenderer = SDL_CreateRenderer(window, -1, 0);
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
    return SDL_RenderTargetSupported(mRenderer);
}

void Sdl2Context::Present()
{
    SDL_RenderPresent(mRenderer);
}

void Sdl2Context::UseScreenFramebuffer()
{
    SDL_SetRenderTarget(mRenderer, NULL);
}

void Sdl2Context::UseTextureFramebuffer(SDL_Texture* texture)
{
    SDL_SetRenderTarget(mRenderer, texture);
}
