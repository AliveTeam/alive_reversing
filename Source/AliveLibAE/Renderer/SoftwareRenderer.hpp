#pragma once

#include "IRenderer.hpp"

class SoftwareRenderer : public IRenderer
{
public:
    void Destroy() override;
    bool Create(TWindowHandleType window) override;
    void Clear(u8 r, u8 g, u8 b) override;
    void StartFrame(s32 xOff, s32 yOff) override;
    void EndFrame() override;
    void Present() override;
    void BltBackBuffer(const SDL_Rect* pCopyRect, const SDL_Rect* pDst) override;
    void OutputSize(s32* w, s32* h) override;
    bool UpdateBackBuffer(const void* pPixels, s32 pitch) override;
    void CreateBackBuffer(bool filter, s32 format, s32 w, s32 h) override;
    void PalFree(const PalRecord& record) override;
    bool PalAlloc(PalRecord& record) override;
    void PalSetData(const PalRecord& record, const u8* pPixels) override;
    void SetTPage(s16 tPage) override;
    void SetClip(Prim_PrimClipper& clipper) override;
    void SetScreenOffset(Prim_ScreenOffset& offset) override;
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

    void Upload(BitDepth bitDepth, const PSX_RECT& rect, const u8* pPixels) override;

    void LoadExternalCam(const char* path, const unsigned char* key, int keyLength) override;

private:
    bool mFrameStarted = false;

    void DrawPoly(PrimAny& any);

    SDL_Texture* mBackBufferTexture = nullptr;
    SDL_Renderer* mRenderer = nullptr;

    s32 mLastH = 0;
    s32 mLastW = 0;

    s32 mFrame_xOff = 0;
    s32 mFrame_yOff = 0;
};
