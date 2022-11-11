#pragma once

#ifdef _WIN32

    #include "IRenderer.hpp"
    #include "TextureCache.hpp"
    #include "PaletteCache.hpp"

    #undef DIRECT3D_VERSION
    #define DIRECT3D_VERSION 0x0900
    #include <d3d9.h>

class DirectX9TextureCache final : public TextureCache<IDirect3DTexture9*>
{
public:
    void DeleteTexture(IDirect3DTexture9* texture) override;
};

class DirectX9Renderer final : public IRenderer
{
public:
    DirectX9Renderer();
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
    u32 PreparePalette(AnimationPal& pCache);

    void DecreaseResourceLifetimes();

    void MakeVertexBuffer();
    void SetQuad(f32 x, f32 y, f32 w, f32 h);
    void SetQuad(Poly_FT4& poly);


    IDirect3DTexture9* PrepareTextureFromAnim(Animation& anim);

    bool mFrameStarted = false;

    SDL_Renderer* mRenderer = nullptr;
    IDirect3DDevice9* mDevice = nullptr;

    IDirect3DVertexDeclaration9* mVertexDecl = nullptr;

    IDirect3DSurface9* mScreenRenderTarget = nullptr;
    IDirect3DSurface9* mTextureRenderTarget = nullptr;
    IDirect3DTexture9* mTexture = nullptr;

    IDirect3DPixelShader9* mPixelShader = nullptr;

    LPDIRECT3DVERTEXBUFFER9 v_buffer = NULL;

    IDirect3DTexture9* mPaletteTexture = nullptr;
    PaletteCache mPaletteCache;
    DirectX9TextureCache mTextureCache;
};

#endif
