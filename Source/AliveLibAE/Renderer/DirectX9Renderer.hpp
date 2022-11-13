#pragma once

#ifdef _WIN32

    #include "IRenderer.hpp"
    #include "TextureCache.hpp"
    #include "PaletteCache.hpp"

    #undef DIRECT3D_VERSION
    #define DIRECT3D_VERSION 0x0900
    #include <d3d9.h>
    #include <atlbase.h>

class SDL_Renderer_RAII final
{
public:
    SDL_Renderer_RAII(const SDL_Renderer_RAII&) = delete;

    explicit SDL_Renderer_RAII(SDL_Renderer* renderer) 
        : mRenderer(renderer)
    {

    }

    ~SDL_Renderer_RAII()
    {
        if (mRenderer)
        {
            SDL_DestroyRenderer(mRenderer);
        }
    }

    SDL_Renderer* mRenderer = nullptr;
};

class DirectX9TextureCache final : public TextureCache<ATL::CComPtr<IDirect3DTexture9>>
{
public:
    void DeleteTexture(ATL::CComPtr<IDirect3DTexture9> texture) override;
};

class DirectX9Renderer final : public IRenderer
{
public:
    DirectX9Renderer();
    ~DirectX9Renderer();
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
    void SetupBlendMode(u16 blendMode);

    u32 PreparePalette(AnimationPal& pCache);

    void DecreaseResourceLifetimes();

    void MakeVertexBuffer();
    void SetQuad(f32 x, f32 y, f32 w, f32 h);
    void SetQuad(u8 type, bool isSemiTrans, bool isShaded, u8 blendMode, u8 palIndex, u8 textureUnit, u8 r, u8 g, u8 b, float u0, float v0, float u1, float v1, Poly_FT4& poly);


    IDirect3DTexture9* PrepareTextureFromAnim(Animation& anim);

    bool mFrameStarted = false;

    // TODO: Remove heap alloc when using a normal ctor
    std::unique_ptr<SDL_Renderer_RAII> mRenderer;
    ATL::CComPtr<IDirect3DDevice9> mDevice;

    ATL::CComPtr<IDirect3DVertexDeclaration9> mVertexDecl;

    ATL::CComPtr<IDirect3DSurface9> mScreenRenderTarget;
    ATL::CComPtr<IDirect3DSurface9> mTextureRenderTarget;
    ATL::CComPtr<IDirect3DTexture9> mCamTexture;

    ATL::CComPtr<IDirect3DPixelShader9> mPixelShader;

    ATL::CComPtr<IDirect3DVertexBuffer9> v_buffer;

    ATL::CComPtr<IDirect3DTexture9> mPaletteTexture;
    PaletteCache mPaletteCache;
    DirectX9TextureCache mTextureCache;
};

#endif
