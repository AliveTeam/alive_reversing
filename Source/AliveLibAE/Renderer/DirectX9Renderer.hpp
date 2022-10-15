#pragma once

#ifdef _WIN32

    #include "IRenderer.hpp"

class DirectX9Renderer final : public IRenderer
{
public:
    void Destroy() override;
    bool Create(TWindowHandleType window) override;
    void Clear(u8 r, u8 g, u8 b) override;
    void StartFrame(s32 xOff, s32 yOff) override;
    void EndFrame() override;
    void OutputSize(s32* w, s32* h) override;
    void SetTPage(u16 tPage) override;
    void SetClip(Prim_PrimClipper& clipper) override;
    void SetScreenOffset(Prim_ScreenOffset& offset) override;
    void ToggleFilterScreen() override;
    void ToggleKeepAspectRatio() override;
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
    SDL_Renderer* mRenderer = nullptr;
    IDirect3DDevice9* mDevice = nullptr;
};

#endif
