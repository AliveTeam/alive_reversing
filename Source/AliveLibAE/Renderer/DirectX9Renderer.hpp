#pragma once

#ifdef _WIN32

#include "IRenderer.hpp"

class DirectX9Renderer : public IRenderer
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
    void PalFree(const PalRecord& record) override;
    bool PalAlloc(PalRecord& record) override;
    void PalSetData(const PalRecord& record, const BYTE* pPixels) override;
    void SetTPage(short tPage) override;
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

    void Upload(BitDepth bitDepth, const PSX_RECT& rect, const BYTE* pPixels) override;
private:
    SDL_Renderer* mRenderer = nullptr;
    IDirect3DDevice9* mDevice = nullptr;
};

#endif
