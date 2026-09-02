#pragma once

#include <SDL3/SDL.h>

class Sdl3Context final
{
public:
    Sdl3Context(SDL_Window* window);
    ~Sdl3Context();

    SDL_Renderer* GetRenderer();
    bool IsRenderTargetSupported();
    void Present();
    void RestoreFramebuffer();
    void SaveFramebuffer();
    void UseScreenFramebuffer();
    void UseTextureFramebuffer(SDL_Texture* texture);

private:
    SDL_Renderer* mRenderer;
    SDL_Rect mLastClipRect;
    SDL_Texture* mLastFramebuffer;
    SDL_Window* mWindow;
};
