#pragma once

#include "IRenderer.hpp"

#define SDL_VERTEX_IS_SUPPORTED SDL_MAJOR_VERSION > 2 || (SDL_MAJOR_VERSION == 2 && (SDL_MINOR_VERSION > 0 || SDL_PATCHLEVEL >= 18))

#if SDL_VERTEX_IS_SUPPORTED
class SoftwareRenderer : public IRenderer
{
public:
    SoftwareRenderer(TWindowHandleType window);
    ~SoftwareRenderer() override;

    void Clear(u8 r, u8 g, u8 b) override;
    void StartFrame() override;
    void EndFrame() override;
    void SetTPage(u16 tPage) override;
    void SetClip(Prim_PrimClipper& clipper) override;
    void ToggleFilterScreen() override;
    void Draw(Prim_GasEffect& gasEffect) override;
    void Draw(Line_G2& line) override;
    void Draw(Line_G4& line) override;
    void Draw(Poly_G3& poly) override;
    void Draw(Poly_FT4& poly) override;
    void Draw(Poly_G4& poly) override;

private:
    bool mFrameStarted = false;

    SDL_Texture* mBackBufferTexture = nullptr;
    SDL_Renderer* mRenderer = nullptr;
    TWindowHandleType mWindow = nullptr;

    s32 mLastH = 0;
    s32 mLastW = 0;
};
#endif
