#include "../../stdafx.h"
#include "DirectX9Renderer.hpp"
#include "../../../relive_lib/FatalError.hpp"
#include "../../../relive_lib/ResourceManagerWrapper.hpp"
#include "../../../relive_lib/Animation.hpp"
#include "../../../AliveLibAE/Font.hpp"
#include "pixel_shader.h"
#include "cam_fg1_shader.h"
#include "flat_shader.h"
#include <SDL_syswm.h>

#ifdef _WIN32

    #undef DIRECT3D_VERSION
    #define DIRECT3D_VERSION 0x0900

#pragma comment(lib, "D3d9.lib")

#define DX_SPRITE_TEXTURE_LIFETIME 300

#define DX_DEBUG 1

#if DX_DEBUG > 0
    #define DX_VERIFY(x)    \
        {                   \
        HRESULT hr = x;     \
        if (FAILED(hr))     \
        {                   \
            ALIVE_FATAL(#x " failed HRESULT 0x%08X", hr); \
        }\
        }
#else
    #define DX_VERIFY(x) (x);
#endif

namespace DXTexture {
static void LoadSubImage(IDirect3DTexture9& texture, u32 xStart, u32 yStart, u32 width, u32 height, const RGBA32* pixels)
{
    D3DLOCKED_RECT locked = {};
    DX_VERIFY(texture.LockRect(0, &locked, nullptr, D3DLOCK_DISCARD));
    if (locked.pBits)
    {
        const RGBA32* pSrc = pixels;
        for (u32 y = yStart; y < yStart + height; y++)
        {
            u32* p = reinterpret_cast<u32*>(locked.pBits);
            p = p + ((locked.Pitch / sizeof(u32)) * y);
            for (u32 x = xStart; x < xStart + width; x++)
            {
                *p = (pSrc->a << 24) + (pSrc->r << 16) + (pSrc->g << 8) + (pSrc->b);
                p++;
                pSrc++;
            }
        }
    }
    DX_VERIFY(texture.UnlockRect(0));
}

static void FillTexture(IDirect3DTexture9& texture, u32 width, u32 height, const RGBA32& pixel)
{
    D3DLOCKED_RECT locked = {};
    DX_VERIFY(texture.LockRect(0, &locked, nullptr, D3DLOCK_DISCARD));
    if (locked.pBits)
    {
        for (u32 y = 0; y < height; y++)
        {
            u32* p = reinterpret_cast<u32*>(locked.pBits);
            p = p + ((locked.Pitch / sizeof(u32)) * y);
            for (u32 x = 0; x < width; x++)
            {
                *p = (pixel.a << 24) + (pixel.r << 16) + (pixel.g << 8) + (pixel.b);
                p++;
            }
        }
    }
    DX_VERIFY(texture.UnlockRect(0));
}

static void LoadSubImage(IDirect3DTexture9& texture, u32 xStart, u32 yStart, u32 width, u32 height, const u8* pixels)
{
    D3DLOCKED_RECT locked = {};
    DX_VERIFY(texture.LockRect(0, &locked, nullptr, D3DLOCK_DISCARD));
    if (locked.pBits)
    {
        const u8* pSrc = pixels;
        for (u32 y = yStart; y < yStart + height; y++)
        {
            u8* p = reinterpret_cast<u8*>(locked.pBits);
            p = p + ((locked.Pitch / sizeof(u8)) * y);
            for (u32 x = xStart; x < xStart + width; x++)
            {
                *p = *pSrc;
                p++;
                pSrc++;
            }
        }
    }
    DX_VERIFY(texture.UnlockRect(0));
}
}


// TODO: Copy pasted from GL renderer
inline u16 GetTPageBlendMode(u16 tpage)
{
    return (tpage >> 4) & 3;
}

class VertexInfo final
{
public:
    template<typename PsxPrimType>
    constexpr static VertexInfo GTriangle(u8 primType, u16 blendMode, const PsxPrimType& prim)
    {
        VertexInfo vi = {};
        vi.mPrimType = primType;

        vi.SetRGB3(prim);

        vi.mIsSemiTrans = GetPolyIsSemiTrans(&prim);
        vi.mBlendMode = blendMode;
        vi.mIsShaded = GetPolyIsShaded(&prim);

        vi.SetXY3(prim);

        return vi;
    }

    template <typename PsxPrimType>
    constexpr static VertexInfo QQuad(u8 primType, u16 blendMode, const PsxPrimType& prim)
    {
        VertexInfo vi = {};
        vi.mPrimType = primType;

        vi.SetRGB(prim);

        vi.mIsSemiTrans = GetPolyIsSemiTrans(&prim);
        vi.mBlendMode = blendMode;
        vi.mIsShaded = GetPolyIsShaded(&prim);

        vi.SetXY3(prim);

        vi.mX[3] = X3(&prim);
        vi.mY[3] = Y3(&prim);

        return vi;
    }

    template <typename PsxPrimType>
    constexpr static VertexInfo FQuad(u8 primType, u32 palIdx, const PsxPrimType& prim)
    {
        VertexInfo vi = {};
        vi.mPrimType = primType;
        vi.mPalIndex = palIdx;

        vi.SetRGB(prim);

        vi.mIsSemiTrans = GetPolyIsSemiTrans(&prim);
        vi.mBlendMode = GetTPageBlendMode(GetTPage(&prim));
        vi.mIsShaded = GetPolyIsShaded(&prim);

        vi.SetXY3(prim);

        vi.mX[3] = X3(&prim);
        vi.mY[3] = Y3(&prim);

        return vi;
    }


    // TODO: Strongly type
    u8 mPrimType;
    u8 mR[4];
    u8 mG[4];
    u8 mB[4];
    u32 mPalIndex;
    bool mIsSemiTrans;
    bool mIsShaded;
    // TODO: Strongly type
    u16 mBlendMode;

    f32 mX[4];
    f32 mY[4];

private:
    template <typename PsxPrimType>
    void SetXY3(const PsxPrimType& prim)
    {
        mX[0] = X0(&prim);
        mX[1] = X1(&prim);
        mX[2] = X2(&prim);

        mY[0] = Y0(&prim);
        mY[1] = Y1(&prim);
        mY[2] = Y2(&prim);
    }

    template <typename PsxPrimType>
    void SetRGB(const PsxPrimType& prim)
    {
        mR[0] = R0(&prim);
        mG[0] = G0(&prim);
        mB[0] = B0(&prim);

        for (u32 i = 1; i < 4; i++)
        {
            mR[i] = mR[0];
            mG[i] = mG[0];
            mB[i] = mB[0];
        }
    }

    template <typename PsxPrimType>
    void SetRGB3(const PsxPrimType& prim)
    {
        mR[0] = R0(&prim);
        mG[0] = G0(&prim);
        mB[0] = B0(&prim);

        mR[1] = R1(&prim);
        mG[1] = G1(&prim);
        mB[1] = B1(&prim);

        mR[2] = R2(&prim);
        mG[2] = G2(&prim);
        mB[2] = B2(&prim);
    }
};

struct CUSTOMVERTEX final
{
    FLOAT X, Y, Z, RHW;
    DWORD COLOR; // diffuse

    // TEXTURECOORD0
    float u;
    float v;

    // TEXTURECOORD1
    float isSemiTrans;
    float isShaded;

    // TEXTURECOORD2
    float palIndex;
    float blendMode;

    // TEXTURECOORD3
    float drawType;
    float textureUnit;
};

const D3DVERTEXELEMENT9 simple_decl[] =
{
    // D3DFVF_XYZRHW
    {0, FIELD_OFFSET(CUSTOMVERTEX, X), D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITIONT, 0},
    
    // D3DFVF_DIFFUSE
    {0, FIELD_OFFSET(CUSTOMVERTEX, COLOR), D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
    
    // D3DFVF_TEX1
    {0, FIELD_OFFSET(CUSTOMVERTEX, u), D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
    {0, FIELD_OFFSET(CUSTOMVERTEX, isSemiTrans), D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1},
    {0, FIELD_OFFSET(CUSTOMVERTEX, palIndex), D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2},
    {0, FIELD_OFFSET(CUSTOMVERTEX, drawType), D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3},

    D3DDECL_END()};

DirectX9Renderer::DirectX9Renderer(TWindowHandleType window)
  : IRenderer(window),
    mPaletteCache(256)
{
    mD3D9.Attach(Direct3DCreate9(D3D_SDK_VERSION));

    mPresentParams.Windowed = TRUE;
    mPresentParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
    
    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);
    SDL_GetWindowWMInfo(window, &wmInfo);
    HWND hwnd = wmInfo.info.win.window;

    mPresentParams.hDeviceWindow = hwnd;

    // TODO: Might make sense to enum the adapters here at some point
    mD3D9->CreateDevice(D3DADAPTER_DEFAULT,
                      D3DDEVTYPE_HAL,
                      hwnd,
                      D3DCREATE_HARDWARE_VERTEXPROCESSING,
                        &mPresentParams,
                      &mDevice.p);

    if (!mDevice)
    {
        ALIVE_FATAL("Couldnt get DirectX9 device %s", SDL_GetError());
    }

    // Verify pixel shader version is what we need
    D3DCAPS9 dxCaps = {};
    DX_VERIFY(mDevice->GetDeviceCaps(&dxCaps));
    if (dxCaps.PixelShaderVersion < D3DPS_VERSION(2, 0))
    {
        ALIVE_FATAL("Require pixel shader 2.0 or later but got %d.%d", D3DSHADER_VERSION_MAJOR(dxCaps.PixelShaderVersion), D3DSHADER_VERSION_MINOR(dxCaps.PixelShaderVersion));
    }

    LOG_INFO("PS 2.0 max instructions %d PS 3.0 max instructions %d", dxCaps.PS20Caps.NumInstructionSlots, dxCaps.MaxPixelShader30InstructionSlots);
    LOG_INFO("Max texture w %d max texture h %d", dxCaps.MaxTextureWidth, dxCaps.MaxTextureHeight);

    MakeVertexBuffer();

    // Use linear filtering on all samplers and texture clamping
    for (u32 i = 0; i < 16; i++)
    {
        DX_VERIFY(mDevice->SetSamplerState(i, D3DSAMP_MAGFILTER, D3DTEXF_POINT));
        DX_VERIFY(mDevice->SetSamplerState(i, D3DSAMP_MINFILTER, D3DTEXF_POINT));
        DX_VERIFY(mDevice->SetSamplerState(i, D3DSAMP_MIPFILTER, D3DTEXF_NONE));
        DX_VERIFY(mDevice->SetSamplerState(i, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP));
        DX_VERIFY(mDevice->SetSamplerState(i, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP));
        DX_VERIFY(mDevice->SetSamplerState(i, D3DSAMP_ADDRESSW, D3DTADDRESS_CLAMP));
    }

    // Turn off lighting/alpha blend
    DX_VERIFY(mDevice->SetRenderState(D3DRS_LIGHTING, FALSE));
    DX_VERIFY(mDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE));

    // Default blend mode
    DX_VERIFY(mDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA));
    DX_VERIFY(mDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA));
    DX_VERIFY(mDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD));
    DX_VERIFY(mDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE));

    // Vertex formats
    DX_VERIFY(mDevice->CreateVertexDeclaration(simple_decl, &mVertexDecl));
    DX_VERIFY(mDevice->SetVertexDeclaration(mVertexDecl));

    // Pixel shaders
    DX_VERIFY(mDevice->CreatePixelShader((DWORD*) pixel_shader, &mPixelShader));
    DX_VERIFY(mDevice->CreatePixelShader((DWORD*) cam_fg1_shader, &mCamFG1Shader));
    DX_VERIFY(mDevice->CreatePixelShader((DWORD*) flat_shader, &mFlatShader));

    // Render targets
    DX_VERIFY(mDevice->CreateRenderTarget(640, 240, D3DFMT_A8R8G8B8, D3DMULTISAMPLE_NONE, 0, FALSE, &mTextureRenderTarget, nullptr));
    DX_VERIFY(mDevice->GetRenderTarget(0, &mScreenRenderTarget));
    DX_VERIFY(mDevice->SetRenderTarget(0, mTextureRenderTarget));

    // Pal cache texture
    D3DLOCKED_RECT locked = {};
    mPaletteTexture = std::make_shared<ATL::CComPtr<IDirect3DTexture9>>();
    DX_VERIFY(mDevice->CreateTexture(256, 256, 0, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &*mPaletteTexture, nullptr));
    const RGBA32 kColourMagenta = {255, 0, 255, 255};
    DXTexture::FillTexture(**mPaletteTexture, 256, 256, kColourMagenta);

    DX_VERIFY(mDevice->SetTexture(mPalUnit, *mPaletteTexture));

    // select which vertex format we are using
    DX_VERIFY(mDevice->SetVertexDeclaration(mVertexDecl));

    // select the vertex buffer to display
    DX_VERIFY(mDevice->SetStreamSource(0, mCameraVBO, 0, sizeof(CUSTOMVERTEX)));
}

DirectX9Renderer::~DirectX9Renderer()
{
    TRACE_ENTRYEXIT;

    // TODO: Fix me, dtor can't call clear else pure call boom
    mTextureCache.Clear();
}

void DirectX9Renderer::Clear(u8 /*r*/, u8 /*g*/, u8 /*b*/)
{
    //mDevice->Clear(0, nullptr, D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET | D3DCLEAR_STENCIL, D3DCOLOR_XRGB(r, g, b), 1.0f, 0);
}

void DirectX9Renderer::StartFrame()
{
    IRenderer::StartFrame();

    if (!mFrameStarted)
    {
        mFrameStarted = true;

        // Set offsets for the screen (this is for the screen shake effect)
        mOffsetX = 0;
        mOffsetY = 0;

        DX_VERIFY(mDevice->BeginScene());

        // Draw everything to the texture
        mDevice->SetRenderTarget(0, mTextureRenderTarget);
    }
}

void DirectX9Renderer::EndFrame()
{
    if (mFrameStarted)
    {
        DX_VERIFY(mDevice->EndScene());

        // Render to the screen instead of the texture
        DX_VERIFY(mDevice->SetRenderTarget(0, mScreenRenderTarget));

        DX_VERIFY(mDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0));

        // Copy the rendered to texture to the entire screen
        //const SDL_Rect dstRectSDL = GetTargetDrawRect();
        //const RECT dstRect = {dstRectSDL.x, dstRectSDL.y, dstRectSDL.w, dstRectSDL.h};
        mDevice->StretchRect(mTextureRenderTarget, nullptr, mScreenRenderTarget, nullptr, D3DTEXF_POINT);

        // 0x8876086C

        const HRESULT presentHR = mDevice->Present(NULL, NULL, NULL, NULL);
        if (presentHR == D3DERR_DEVICELOST)
        {
            // TODO: Handle device lost properly
            ALIVE_FATAL("TODO D3DERR_DEVICELOST");
        }
        else
        {
            if (FAILED(presentHR))
            {
                ALIVE_FATAL("mDevice->Present failed HRESULT 0x%08X", presentHR);
            }
        }

        mFrameStarted = false;
    }

    // Always decrease resource lifetimes regardless of drawing to prevent
    // memory leaks
    DecreaseResourceLifetimes();
}

void DirectX9Renderer::SetTPage(u16 tPage)
{
    mGlobalTPage = tPage;
}

void DirectX9Renderer::SetClip(const Prim_PrimClipper& clipper)
{
    RECT rect = {};
    rect.left = clipper.field_C_x;
    rect.right = clipper.field_C_x + clipper.mBase.header.mRect.w;
    rect.top = clipper.field_E_y;
    rect.bottom = clipper.field_E_y + clipper.mBase.header.mRect.h;

    if (rect.left == 0 && rect.right == 0 && rect.bottom == 1 && rect.top == 1)
    {
        DX_VERIFY(mDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE));
        return;
    }

    DX_VERIFY(mDevice->SetScissorRect(&rect));
    DX_VERIFY(mDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE));
}

void DirectX9Renderer::ToggleFilterScreen()
{
    // TODO
}

void DirectX9Renderer::Draw(const Prim_GasEffect& /*gasEffect*/)
{
    // TODO
}

void DirectX9Renderer::Draw(const Line_G2& /*line*/)
{
    // TODO
}

void DirectX9Renderer::Draw(const Line_G4& /*line*/)
{
    // TODO
}


void DirectX9Renderer::Draw(const Poly_G3& poly)
{
    mDevice->SetPixelShader(mFlatShader);

    auto vi = VertexInfo::GTriangle(0, GetTPageBlendMode(mGlobalTPage), poly);
    DrawTris(nullptr, 0, vi, 0.0f, 0.0f, 0.0f, 0.0f, 1);
}

void DirectX9Renderer::SetupBlendMode(u16 blendMode)
{
    if ((TPageAbr) blendMode == TPageAbr::eBlend_2)
    {
        DX_VERIFY(mDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA));
        DX_VERIFY(mDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE));
        DX_VERIFY(mDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT));
    }
    else
    {
        DX_VERIFY(mDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE));
        DX_VERIFY(mDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCALPHA));
        DX_VERIFY(mDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD));
    }
}

u32 DirectX9Renderer::PreparePalette(AnimationPal& pCache)
{
    const PaletteCache::AddResult addRet = mPaletteCache.Add(pCache);

    if (addRet.mAllocated)
    {
        DXTexture::LoadSubImage(**mPaletteTexture, 0, addRet.mIndex, ALIVE_COUNTOF(pCache.mPal), 1, &pCache.mPal[0]);
        //mStats.mPalUploadCount++;
    }

    return addRet.mIndex;
}

void DirectX9Renderer::DrawTris(TDX9Texture pTexture, u32 textureUnit, const VertexInfo& vi, float u0, float v0, float u1, float v1, u32 numTris)
{
    DX_VERIFY(mDevice->SetTexture(textureUnit, pTexture ? *pTexture : nullptr));

    SetupBlendMode(vi.mBlendMode);
    SetQuad(vi, u0, v0, u1, v1);

    DX_VERIFY(mDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, numTris));
}

void DirectX9Renderer::Draw(const Poly_FT4& poly)
{
    if (poly.mCam && !poly.mFg1)
    {
        mDevice->SetPixelShader(mCamFG1Shader);

        TDX9Texture pTextureToUse = MakeCachedTexture(poly.mCam->mUniqueId.Id(), *poly.mCam->mData.mPixels, 1024, 1024, poly.mCam->mData.mWidth, poly.mCam->mData.mHeight);
        auto vi = VertexInfo::FQuad(2, 0, poly);
        DrawTris(pTextureToUse, mCamUnit, vi, 0.0f, 0.0f, 640.0f / 1024.0f, 240.0f / 1024.0f);
    }
    else if (poly.mCam && poly.mFg1)
    {
        mDevice->SetPixelShader(mCamFG1Shader);

        TDX9Texture pTextureToUse = MakeCachedTexture(poly.mFg1->mUniqueId.Id(), *poly.mFg1->mImage.mPixels, 1024, 1024, poly.mFg1->mImage.mWidth, poly.mFg1->mImage.mHeight);
        auto vi = VertexInfo::FQuad(1, 0, poly);
        DrawTris(pTextureToUse, mFG1Units[0], vi, 0.0f, 0.0f, 640.0f / 1024.0f, 240.0f / 1024.0f);
    }
    else if (poly.mAnim)
    {
        mDevice->SetPixelShader(mPixelShader);

        std::shared_ptr<TgaData> pTga = poly.mAnim->mAnimRes.mTgaPtr;

        const PerFrameInfo* pHeader = poly.mAnim->Get_FrameHeader(-1);

        float u0 = (static_cast<float>(pHeader->mSpriteSheetX) / pTga->mWidth);
        float v0 = (static_cast<float>(pHeader->mSpriteSheetY) / pTga->mHeight);

        float u1 = u0 + ((float) pHeader->mWidth / (float) pTga->mWidth);
        float v1 = v0 + ((float) pHeader->mHeight / (float) pTga->mHeight);

        if (poly.mFlipX)
        {
            std::swap(u0, u1);
        }

        if (poly.mFlipY)
        {
            std::swap(v1, v0);
        }

        TDX9Texture pTextureToUse = MakeCachedIndexedTexture(poly.mAnim->mAnimRes.mUniqueId.Id(), pTga->mPixels, pTga->mWidth, pTga->mHeight, pTga->mWidth, pTga->mHeight);
        auto vi = VertexInfo::FQuad(1, PreparePalette(*poly.mAnim->mAnimRes.mCurPal), poly);
        DrawTris(pTextureToUse, mSpriteUnit, vi, u0, v0, u1, v1);
    }
    else if (poly.mFont)
    {
        mDevice->SetPixelShader(mPixelShader);

        std::shared_ptr<TgaData> pTga = poly.mFont->mFntResource.mTgaPtr;

        float u0 = U0(&poly) / (f32)pTga->mWidth;
        float v0 = V0(&poly) / (f32) pTga->mHeight;

        float u1 = U3(&poly) / (f32) pTga->mWidth;
        float v1 = V3(&poly) / (f32) pTga->mHeight;

        FontResource& fontRes = poly.mFont->mFntResource;
        TDX9Texture pTextureToUse = MakeCachedIndexedTexture(fontRes.mUniqueId.Id(), pTga->mPixels, pTga->mWidth, pTga->mHeight, pTga->mWidth, pTga->mHeight);
        auto vi = VertexInfo::FQuad(1, PreparePalette(*fontRes.mCurPal), poly);
        DrawTris(pTextureToUse, mSpriteUnit, vi, u0, v0, u1, v1);
    }
}

void DirectX9Renderer::Draw(const Poly_G4& poly)
{
    mDevice->SetPixelShader(mFlatShader);

    auto vi = VertexInfo::QQuad(0, GetTPageBlendMode(mGlobalTPage), poly);
    DrawTris(nullptr, 0, vi, 0.0f, 0.0f, 0.0f, 0.0f, 2);
}

static float FromBool(const bool v)
{
    return v ? 1.0f : 0.0f;
}

static float FromInt(const u32 v)
{
    return static_cast<float>(v);
}

void DirectX9Renderer::SetQuad(const VertexInfo& vi, float u0, float v0, float u1, float v1)
{
    float fudge = 0.5f;
    // create the vertices using the CUSTOMVERTEX struct
    CUSTOMVERTEX vertices[] = {
        // Tri 1
        {vi.mX[0] - fudge,
         vi.mY[0] - fudge,
         0.5f,
         1.0f,
         D3DCOLOR_XRGB(vi.mR[0], vi.mG[0], vi.mB[0]),
         u0, // 0
         v0, // 0
         FromBool(vi.mIsSemiTrans),
         FromBool(vi.mIsShaded),
         FromInt(vi.mPalIndex),
         FromInt(vi.mBlendMode),
         FromInt(vi.mPrimType),
         0.0f},

        {vi.mX[1] - fudge,
         vi.mY[1] - fudge,
         0.5f,
         1.0f,
         D3DCOLOR_XRGB(vi.mR[1], vi.mG[1], vi.mB[1]),
         u1, // 1
         v0, // 0
         FromBool(vi.mIsSemiTrans),
         FromBool(vi.mIsShaded),
         FromInt(vi.mPalIndex),
         FromInt(vi.mBlendMode),
         FromInt(vi.mPrimType),
         0.0f},

        {vi.mX[2] - fudge,
         vi.mY[2] - fudge,
         0.5f,
         1.0f,
         D3DCOLOR_XRGB(vi.mR[2], vi.mG[2], vi.mB[2]),
         u0, // 0
         v1, // 1
         FromBool(vi.mIsSemiTrans),
         FromBool(vi.mIsShaded),
         FromInt(vi.mPalIndex),
         FromInt(vi.mBlendMode),
         FromInt(vi.mPrimType),
         0.0f},

         // Tri 2
        {vi.mX[1] - fudge,
         vi.mY[1] - fudge,
         0.5f,
         1.0f,
         D3DCOLOR_XRGB(vi.mR[1], vi.mG[1], vi.mB[1]),
         u1, // 1
         v0, // 0
         FromBool(vi.mIsSemiTrans),
         FromBool(vi.mIsShaded),
         FromInt(vi.mPalIndex),
         FromInt(vi.mBlendMode),
         FromInt(vi.mPrimType),
         0.0f},

        {vi.mX[3] - fudge,
         vi.mY[3] - fudge,
         0.5f,
         1.0f,
         D3DCOLOR_XRGB(vi.mR[3], vi.mG[3], vi.mB[3]),
         u1, // 1
         v1, // 1
         FromBool(vi.mIsSemiTrans),
         FromBool(vi.mIsShaded),
         FromInt(vi.mPalIndex),
         FromInt(vi.mBlendMode),
         FromInt(vi.mPrimType),
         0.0f},

        {vi.mX[2] - fudge,
         vi.mY[2] - fudge,
         0.5f,
         1.0f,
         D3DCOLOR_XRGB(vi.mR[2], vi.mG[2], vi.mB[2]),
         u0, // 0
         v1, // 1
         FromBool(vi.mIsSemiTrans),
         FromBool(vi.mIsShaded),
         FromInt(vi.mPalIndex),
         FromInt(vi.mBlendMode),
         FromInt(vi.mPrimType),
         0.0f}
    };

    VOID* pVoid = nullptr;

    // lock mCameraVBO and load the vertices into it
    DX_VERIFY(mCameraVBO->Lock(0, 0, &pVoid, 0));
    memcpy(pVoid, vertices, sizeof(vertices));
    DX_VERIFY(mCameraVBO->Unlock());
}

void DirectX9Renderer::DecreaseResourceLifetimes()
{
    mTextureCache.DecreaseResourceLifetimes();

    mPaletteCache.ResetUseFlags();
}

void DirectX9Renderer::MakeVertexBuffer()
{
    // create a vertex buffer interface called mCameraVBO
    DX_VERIFY(mDevice->SetVertexDeclaration(mVertexDecl));

    DX_VERIFY(mDevice->CreateVertexBuffer(6 * sizeof(CUSTOMVERTEX),
                               0,
                               0,
                               D3DPOOL_MANAGED,
                               &mCameraVBO,
                               NULL));
}

TDX9Texture DirectX9Renderer::MakeCachedIndexedTexture(u32 uniqueId, const std::vector<u8>& pixels, u32 textureW, u32 textureH, u32 actualW, u32 actualH)
{
    TDX9Texture textureId = mTextureCache.GetCachedTexture(uniqueId, DX_SPRITE_TEXTURE_LIFETIME);
    if (!textureId)
    {
        textureId = std::make_shared<ATL::CComPtr<IDirect3DTexture9>>();
        DX_VERIFY(mDevice->CreateTexture(textureW, textureH, 0, 0, D3DFMT_L8, D3DPOOL_MANAGED, &*textureId, nullptr));

        mTextureCache.Add(uniqueId, DX_SPRITE_TEXTURE_LIFETIME, textureId);
        DXTexture::LoadSubImage(**textureId, 0, 0, actualW, actualH, pixels.data());
    }
    return textureId;
}

TDX9Texture DirectX9Renderer::MakeCachedTexture(u32 uniqueId, const std::vector<u8>& pixels, u32 textureW, u32 textureH, u32 actualW, u32 actualH)
{
    TDX9Texture textureId = mTextureCache.GetCachedTexture(uniqueId, DX_SPRITE_TEXTURE_LIFETIME);
    if (!textureId)
    {
        textureId = std::make_shared<ATL::CComPtr<IDirect3DTexture9>>();
        DX_VERIFY(mDevice->CreateTexture(textureW, textureH, 0, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &*textureId, nullptr));

        mTextureCache.Add(uniqueId, DX_SPRITE_TEXTURE_LIFETIME, textureId);
        auto pData = reinterpret_cast<const RGBA32*>(pixels.data());
        DXTexture::LoadSubImage(**textureId, 0, 0, actualW, actualH, pData);
    }
    return textureId;
}

#endif
