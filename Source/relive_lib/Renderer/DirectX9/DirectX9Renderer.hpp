#pragma once

#ifdef _WIN32

    #include "../IRenderer.hpp"
    #include "../TextureCache.hpp"
    #include "../PaletteCache.hpp"

    #undef DIRECT3D_VERSION
    #define DIRECT3D_VERSION 0x0900
    #include <d3d9.h>
    #include <atlbase.h>

class DirectX9TextureCache final : public TextureCache<ATL::CComPtr<IDirect3DTexture9>>
{
public:

};

class VertexInfo;

class DirectX9Renderer final : public IRenderer
{
public:
    explicit DirectX9Renderer(TWindowHandleType window);
    ~DirectX9Renderer();
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

    D3DPRESENT_PARAMETERS mPresentParams = {};

    ATL::CComPtr<IDirect3D9> mD3D9;
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
