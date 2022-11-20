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

class VertexInfo;

class DirectX9Renderer final : public IRenderer
{
public:
    explicit DirectX9Renderer(TWindowHandleType window);
    ~DirectX9Renderer();
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
    void Draw(Poly_G3& poly) override;
    void Draw(Poly_F4& poly) override;
    void Draw(Poly_FT4& poly) override;
    void Draw(Poly_G4& poly) override;

private:
    void SetupBlendMode(u16 blendMode);

    u32 PreparePalette(AnimationPal& pCache);

    void DecreaseResourceLifetimes();

    void MakeVertexBuffer();
    void SetQuad(const VertexInfo& vi, float u0, float v0, float u1, float v1);

    IDirect3DTexture9* MakeCachedIndexedTexture(u32 uniqueId, const std::vector<u8>& pixels, u32 textureW, u32 textureH, u32 actualW, u32 actualH);
    IDirect3DTexture9* MakeCachedTexture(u32 uniqueId, const std::vector<u8>& pixels, u32 textureW, u32 textureH, u32 actualW, u32 actualH);
    void DrawTris(IDirect3DTexture9* pTexture, u32 textureUnit, const VertexInfo& vi, float u0, float v0, float u1, float v1, u32 numTris = 2);

    bool mFrameStarted = false;
    
    // TODO: Simply down the prim types so we don't need this
    u16 mGlobalTPage = 0;

    // TODO: Remove heap alloc when using a normal ctor
    std::unique_ptr<SDL_Renderer_RAII> mRenderer;
    ATL::CComPtr<IDirect3DDevice9> mDevice;

    ATL::CComPtr<IDirect3DVertexDeclaration9> mVertexDecl;

    ATL::CComPtr<IDirect3DSurface9> mScreenRenderTarget;
    ATL::CComPtr<IDirect3DSurface9> mTextureRenderTarget;

    ATL::CComPtr<IDirect3DPixelShader9> mPixelShader;
    ATL::CComPtr<IDirect3DPixelShader9> mCamFG1Shader;
    ATL::CComPtr<IDirect3DPixelShader9> mFlatShader;

    ATL::CComPtr<IDirect3DVertexBuffer9> mCameraVBO;

    ATL::CComPtr<IDirect3DTexture9> mPaletteTexture;
    PaletteCache mPaletteCache;
    DirectX9TextureCache mTextureCache;

    const u8 mCamUnit = 4;
    const u8 mPalUnit = 5;
    const u8 mGasUnit = 6;
    const u8 mFG1Units[4] = {0, 1, 2, 3};
    const u8 mSpriteUnit = 7; // TODO: Do we have a total of 8 or 16 units ?
    // sprites = 7+
};

#endif
