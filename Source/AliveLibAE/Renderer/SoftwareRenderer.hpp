#pragma once

#include "IRenderer.hpp"

class SoftwareRenderer : public IRenderer
{
public:
    void Destroy() override;
    bool Create(TWindowHandleType window) override;
    void Clear(BYTE r, BYTE g, BYTE b) override;
    void StartFrame(int xOff, int yOff) override;
    void EndFrame() override;
    void BltBackBuffer(const SDL_Rect* pCopyRect, const SDL_Rect* pDst) override;
    void OutputSize(int* w, int* h) override;
    bool UpdateBackBuffer(const void* pPixels, int pitch) override;
    void CreateBackBuffer(bool filter, int format, int w, int h) override;

    void SetTPage(short tPage) override;
    void SetClip(Prim_PrimClipper& clipper) override;
    void Draw(Prim_Sprt& sprt) override;
    void Draw(Prim_GasEffect& gasEffect) override;
    void Draw(Prim_Tile& tile) override;
    void Draw(Line_F2& line) override;
    void Draw(Line_G2& line) override;
    void Draw(Line_G4& line) override;
    void Draw(Poly_F3& poly) override;
    void Draw(Poly_G3& poly) override;
    void Draw(Poly_F4& poly) override;
    void Draw(Poly_FT4& poly) override;
    void Draw(Poly_G4& poly) override;
private:
    bool mFrameStarted = false;

    void DrawPoly(PrimAny& any);

    SDL_Texture* mBackBufferTexture = nullptr;
    SDL_Renderer* mRenderer = nullptr;

    int mLastH = 0;
    int mLastW = 0;

    int mFrame_xOff = 0;
    int mFrame_yOff = 0;
};
