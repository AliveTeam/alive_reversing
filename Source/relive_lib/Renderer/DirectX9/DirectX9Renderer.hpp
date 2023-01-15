#pragma once

#ifdef _WIN32

    #include "../IRenderer.hpp"
    #include "../TextureCache.hpp"
    #include "../PaletteCache.hpp"

    #include "../Vulkan/Batcher.hpp"

    #undef DIRECT3D_VERSION
    #define DIRECT3D_VERSION 0x0900
    #include <d3d9.h>
    #include <atlbase.h>

class VertexInfo;

using TDX9Texture = std::shared_ptr<ATL::CComPtr<IDirect3DTexture9>>;

class DirectX9Renderer final : public IRenderer
{
public:
    explicit DirectX9Renderer(TWindowHandleType window);
    ~DirectX9Renderer();
    void Clear(u8 r, u8 g, u8 b) override;
    void StartFrame() override;
    void EndFrame() override;
    void SetTPage(u16 tPage) override;
    void SetClip(const Prim_PrimClipper& clipper) override;
    void ToggleFilterScreen() override;
    void Draw(const Prim_GasEffect& gasEffect) override;
    void Draw(const Line_G2& line) override;
    void Draw(const Line_G4& line) override;
    void Draw(const Poly_G3& poly) override;
    void Draw(const Poly_FT4& poly) override;
    void Draw(const Poly_G4& poly) override;

private:
    void SetupBlendMode(u16 blendMode);

    u32 PreparePalette(AnimationPal& pCache);

    void DecreaseResourceLifetimes();

    void DrawBatches();

    TDX9Texture MakeCachedIndexedTexture(u32 uniqueId, const std::vector<u8>& pixels, u32 textureW, u32 textureH, u32 actualW, u32 actualH);
    TDX9Texture MakeCachedTexture(u32 uniqueId, const std::vector<u8>& pixels, u32 textureW, u32 textureH, u32 actualW, u32 actualH);

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

    u32 mVboSize = 0;
    ATL::CComPtr<IDirect3DVertexBuffer9> mVBO;

    u32 mIndexBufferSize = 0;
    ATL::CComPtr<IDirect3DIndexBuffer9> mIndexBuffer;

    static constexpr u32 kSpriteTextureUnitCount = 1;
    struct BatchData
    {
    };
    Batcher<ATL::CComPtr<IDirect3DTexture9>, BatchData, kSpriteTextureUnitCount> mBatcher;

    TDX9Texture mPaletteTexture;
    PaletteCache mPaletteCache;
    TextureCache<TDX9Texture> mTextureCache;

    const u8 mCamUnit = 4;
    const u8 mPalUnit = 5;
    const u8 mGasUnit = 6;
    const u8 mFG1Units[4] = {0, 1, 2, 3};
    const u8 mSpriteUnit = 7; // TODO: Do we have a total of 8 or 16 units ?
    // sprites = 7+
};

#endif
