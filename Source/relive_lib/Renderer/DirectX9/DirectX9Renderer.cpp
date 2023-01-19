#include "../../stdafx.h"
#include "DirectX9Renderer.hpp"
#include "../../../relive_lib/FatalError.hpp"
#include "../../../relive_lib/ResourceManagerWrapper.hpp"
#include "../../../relive_lib/Animation.hpp"
#include "../../../AliveLibAE/Font.hpp"
#include "pixel_shader.h"
#include "cam_fg1_shader.h"
#include "flat_shader.h"
#include "gas_shader.h"
#include "scanlines_shader.h"
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

// TODO: Copy pasted from GL renderer
inline u16 GetTPageBlendMode(u16 tpage)
{
    return (tpage >> 4) & 3;
}

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


static void LoadSubImage(IDirect3DTexture9& texture, u32 xStart, u32 yStart, u32 width, u32 height, const u16* pixels)
{
    D3DLOCKED_RECT locked = {};
    DX_VERIFY(texture.LockRect(0, &locked, nullptr, D3DLOCK_DISCARD));
    if (locked.pBits)
    {
        const u16* pSrc = pixels;
        for (u32 y = yStart; y < yStart + height; y++)
        {
            u16* p = reinterpret_cast<u16*>(locked.pBits);
            p = p + ((locked.Pitch / sizeof(u16)) * y);
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
  : IRenderer(window), mPaletteCache(256)
    , mBatcher(UvMode::Normalized)
{
    mD3D9.Attach(Direct3DCreate9(D3D_SDK_VERSION));

    CreateDevice();
    CheckDeviceCaps();
    SetDeviceStates();
    CreateAllResources();
}

void DirectX9Renderer::SetDeviceStates()
{
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

    // TODO: triangulated lines won't render correctly without turning off culling
    DX_VERIFY(mDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE));
}

void DirectX9Renderer::CreateDevice()
{
    mPresentParams.Windowed = TRUE;
    mPresentParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
    // VSync off
    mPresentParams.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

    s32 w = 0;
    s32 h = 0;
    SDL_GetWindowSize(mWindow, &w, &h);
    mPresentParams.BackBufferWidth = w;
    mPresentParams.BackBufferHeight = h;

    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);
    SDL_GetWindowWMInfo(mWindow, &wmInfo);
    HWND hwnd = wmInfo.info.win.window;

    mPresentParams.hDeviceWindow = hwnd;

    // TODO: Might make sense to enum the adapters here at some point
    DX_VERIFY(mD3D9->CreateDevice(D3DADAPTER_DEFAULT,
                                  D3DDEVTYPE_HAL,
                                  hwnd,
                                  D3DCREATE_HARDWARE_VERTEXPROCESSING,
                                  &mPresentParams,
                                  &mDevice.p));

    if (!mDevice)
    {
        ALIVE_FATAL("Couldnt get DirectX9 device %s", SDL_GetError());
    }
}

void DirectX9Renderer::CheckDeviceCaps()
{
    // Verify pixel shader version is what we need
    D3DCAPS9 dxCaps = {};
    DX_VERIFY(mDevice->GetDeviceCaps(&dxCaps));
    if (dxCaps.PixelShaderVersion < D3DPS_VERSION(2, 0))
    {
        ALIVE_FATAL("Require pixel shader 2.0 or later but got %d.%d", D3DSHADER_VERSION_MAJOR(dxCaps.PixelShaderVersion), D3DSHADER_VERSION_MINOR(dxCaps.PixelShaderVersion));
    }

    LOG_INFO("PS 2.0 max instructions %d PS 3.0 max instructions %d", dxCaps.PS20Caps.NumInstructionSlots, dxCaps.MaxPixelShader30InstructionSlots);
    LOG_INFO("Max texture w %d max texture h %d", dxCaps.MaxTextureWidth, dxCaps.MaxTextureHeight);
}

void DirectX9Renderer::FreeAllResources()
{
    mVertexDecl = nullptr;
    mScreenRenderTarget = nullptr;
    mTextureRenderTarget = nullptr;
    mPixelShader = nullptr;
    mCamFG1Shader = nullptr;
    mFlatShader = nullptr;
    mGasShader = nullptr;
    mScanLinesShader = nullptr;
    mVBO = nullptr;
    mIndexBuffer = nullptr;
    mBatcher.StartFrame();
    mPaletteTexture = nullptr;
    mPaletteCache.Clear();
    mTextureCache.Clear();
    mGasTexture = nullptr;
}

void DirectX9Renderer::CreateAllResources()
{
    // Vertex formats
    DX_VERIFY(mDevice->CreateVertexDeclaration(simple_decl, &mVertexDecl));
    DX_VERIFY(mDevice->SetVertexDeclaration(mVertexDecl));

    // Pixel shaders
    DX_VERIFY(mDevice->CreatePixelShader((DWORD*) pixel_shader, &mPixelShader));
    DX_VERIFY(mDevice->CreatePixelShader((DWORD*) cam_fg1_shader, &mCamFG1Shader));
    DX_VERIFY(mDevice->CreatePixelShader((DWORD*) flat_shader, &mFlatShader));
    DX_VERIFY(mDevice->CreatePixelShader((DWORD*) gas_shader, &mGasShader));
    DX_VERIFY(mDevice->CreatePixelShader((DWORD*) scanlines_shader, &mScanLinesShader));

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

    // TODO: Preallocate VBO and IBOs to some large amount
}

void DirectX9Renderer::ReCreateDevice()
{
    FreeAllResources();
    DX_VERIFY(mDevice->Reset(&mPresentParams));
    CreateDevice();
    SetDeviceStates();
    CreateAllResources();
}

DirectX9Renderer::~DirectX9Renderer()
{
    TRACE_ENTRYEXIT;

    FreeAllResources();
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
        s32 w = 0;
        s32 h = 0;
        SDL_GetWindowSize(mWindow, &w, &h);
        if (w > 0 && h > 0 && w != static_cast<s32>(mPresentParams.BackBufferWidth) && h != static_cast<s32>(mPresentParams.BackBufferHeight))
        {
            ReCreateDevice();
        }

        mFrameStarted = true;

        // Set offsets for the screen (this is for the screen shake effect)
        mOffsetX = 0;
        mOffsetY = 0;

        mBatcher.StartFrame();

        DX_VERIFY(mDevice->BeginScene());

        // Draw everything to the texture
        mDevice->SetRenderTarget(0, mTextureRenderTarget);
    }
}

void DirectX9Renderer::EndFrame()
{
    if (mFrameStarted)
    {
        mBatcher.NewBatch();

        SDL_Rect viewPortRect = GetTargetDrawRect();

        Poly_FT4 fullScreenPoly;
        PolyFT4_Init(&fullScreenPoly);
        SetRGB0(&fullScreenPoly, 255, 255, 255);
        SetXYWH(&fullScreenPoly, (s16) viewPortRect.x, (s16) viewPortRect.y, (s16) viewPortRect.w, (s16) viewPortRect.h);

        // NOTE: This texture in the last batch is always used as the FB source
        std::shared_ptr<ATL::CComPtr<IDirect3DTexture9>> nullTex;
        mBatcher.PushCAM(fullScreenPoly, nullTex);

        // Add the in flight batch
        mBatcher.EndFrame();

        DrawBatches();

        HRESULT presentHR = mDevice->Present(NULL, NULL, NULL, NULL);
        if (presentHR == D3DERR_DEVICELOST)
        {
            presentHR = mDevice->TestCooperativeLevel();
            switch (presentHR)
            {
                case D3DERR_DEVICELOST:
                    LOG_WARNING("Waiting for D3DERR_DEVICELOST state to be over");
                    break;

                case D3DERR_DEVICENOTRESET:
                    LOG_INFO("Begin reset for D3DERR_DEVICELOST");
                    ReCreateDevice();
                    break;
                default:
                    ALIVE_FATAL("TestCooperativeLevel failed HRESULT 0x%08X", presentHR);
                    break;
            }
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
    SDL_Rect rect ;
    rect.x = clipper.field_C_x;
    rect.y = clipper.field_E_y;
    rect.w = clipper.mBase.header.mRect.w;
    rect.h = clipper.mBase.header.mRect.h;

    if (rect.x == 0 && rect.y == 0 && rect.w == 1 && rect.h == 1)
    {
        rect = {};
    }

    mBatcher.SetScissor(rect);
}

void DirectX9Renderer::Draw(const Prim_GasEffect& gasEffect)
{
    const u32 gasWidth = static_cast<u32>(std::floor((gasEffect.w - gasEffect.x) / 4));
    const u32 gasHeight = static_cast<u32>(std::floor((gasEffect.h - gasEffect.y) / 2));
    
    // TODO: Square texture
    mGasTexture = MakeTexture16(gasEffect.pData, gasWidth, gasHeight, gasWidth, gasHeight);

    // TODO: If there is more than 1 gas in a frame break the batch ?
    mBatcher.PushGas(gasEffect);
}

void DirectX9Renderer::Draw(const Line_G2& line)
{
    mBatcher.PushLine(line, GetTPageBlendMode(mGlobalTPage));
}

void DirectX9Renderer::Draw(const Line_G4& line)
{
    mBatcher.PushLine(line, GetTPageBlendMode(mGlobalTPage));
}

void DirectX9Renderer::Draw(const Poly_G3& poly)
{
    mBatcher.PushPolyG3(poly, GetTPageBlendMode(mGlobalTPage));
}

void DirectX9Renderer::SetupBlendMode(u16 blendMode)
{
    if (static_cast<TPageAbr>(blendMode) == TPageAbr::eBlend_2)
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

void DirectX9Renderer::Draw(const Poly_FT4& poly)
{
    if (poly.mCam && !poly.mFg1)
    {
        // TODO: use square textures
        // TDX9Texture pTextureToUse = MakeCachedTexture(poly.mCam->mUniqueId.Id(), *poly.mCam->mData.mPixels, 1024, 1024, poly.mCam->mData.mWidth, poly.mCam->mData.mHeight);
        TDX9Texture pTextureToUse = MakeCachedTexture(poly.mCam->mUniqueId.Id(), *poly.mCam->mData.mPixels, poly.mCam->mData.mWidth, poly.mCam->mData.mHeight, poly.mCam->mData.mWidth, poly.mCam->mData.mHeight);
        mBatcher.PushCAM(poly, pTextureToUse);
    }
    else if (poly.mCam && poly.mFg1)
    {
        // TODO: use square textures
        //TDX9Texture pTextureToUse = MakeCachedTexture(poly.mFg1->mUniqueId.Id(), *poly.mFg1->mImage.mPixels, 1024, 1024, poly.mFg1->mImage.mWidth, poly.mFg1->mImage.mHeight);
        TDX9Texture pTextureToUse = MakeCachedTexture(poly.mFg1->mUniqueId.Id(), *poly.mFg1->mImage.mPixels, poly.mFg1->mImage.mWidth, poly.mFg1->mImage.mHeight, poly.mFg1->mImage.mWidth, poly.mFg1->mImage.mHeight);
        mBatcher.PushFG1(poly, pTextureToUse);
    }
    else if (poly.mAnim)
    {
        std::shared_ptr<TgaData> pTga = poly.mAnim->mAnimRes.mTgaPtr;
        TDX9Texture pTextureToUse = MakeCachedIndexedTexture(poly.mAnim->mAnimRes.mUniqueId.Id(), pTga->mPixels, pTga->mWidth, pTga->mHeight, pTga->mWidth, pTga->mHeight);
        const u32 palIdx = PreparePalette(*poly.mAnim->mAnimRes.mCurPal);
        mBatcher.PushAnim(poly, palIdx, pTextureToUse);
    }
    else if (poly.mFont)
    {
        std::shared_ptr<TgaData> pTga = poly.mFont->mFntResource.mTgaPtr;
        FontResource& fontRes = poly.mFont->mFntResource;
        TDX9Texture pTextureToUse = MakeCachedIndexedTexture(fontRes.mUniqueId.Id(), pTga->mPixels, pTga->mWidth, pTga->mHeight, pTga->mWidth, pTga->mHeight);
        mBatcher.PushFont(poly, PreparePalette(*fontRes.mCurPal), pTextureToUse);
    }
}

void DirectX9Renderer::Draw(const Poly_G4& poly)
{
    mBatcher.PushPolyG4(poly, GetTPageBlendMode(mGlobalTPage));
}

void DirectX9Renderer::DecreaseResourceLifetimes()
{
    mTextureCache.DecreaseResourceLifetimes();

    mPaletteCache.ResetUseFlags();
}

TDX9Texture DirectX9Renderer::MakeTexture16(const u16* pixels, u32 textureW, u32 textureH, u32 actualW, u32 actualH)
{
    auto textureId = std::make_shared<ATL::CComPtr<IDirect3DTexture9>>();
    DX_VERIFY(mDevice->CreateTexture(textureW, textureH, 0, 0, D3DFMT_R5G6B5, D3DPOOL_MANAGED, &*textureId, nullptr));
    DXTexture::LoadSubImage(**textureId, 0, 0, actualW, actualH, pixels);
    return textureId;
}

TDX9Texture DirectX9Renderer::MakeTexture32(const std::vector<u8>& pixels, u32 textureW, u32 textureH, u32 actualW, u32 actualH)
{
    auto textureId = std::make_shared<ATL::CComPtr<IDirect3DTexture9>>();
    DX_VERIFY(mDevice->CreateTexture(textureW, textureH, 0, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &*textureId, nullptr));
    auto pData = reinterpret_cast<const RGBA32*>(pixels.data());
    DXTexture::LoadSubImage(**textureId, 0, 0, actualW, actualH, pData);
    return textureId;
}

TDX9Texture DirectX9Renderer::MakeTexture8(const std::vector<u8>& pixels, u32 textureW, u32 textureH, u32 actualW, u32 actualH)
{
    auto textureId = std::make_shared<ATL::CComPtr<IDirect3DTexture9>>();
    DX_VERIFY(mDevice->CreateTexture(textureW, textureH, 0, 0, D3DFMT_L8, D3DPOOL_MANAGED, &*textureId, nullptr));
    DXTexture::LoadSubImage(**textureId, 0, 0, actualW, actualH, pixels.data());
    return textureId;
}

TDX9Texture DirectX9Renderer::MakeCachedIndexedTexture(u32 uniqueId, const std::vector<u8>& pixels, u32 textureW, u32 textureH, u32 actualW, u32 actualH)
{
    TDX9Texture textureId = mTextureCache.GetCachedTexture(uniqueId, DX_SPRITE_TEXTURE_LIFETIME);
    if (!textureId)
    {
        textureId = MakeTexture8(pixels, textureW, textureH, actualW, actualH);
        mTextureCache.Add(uniqueId, DX_SPRITE_TEXTURE_LIFETIME, textureId);
    }
    return textureId;
}

TDX9Texture DirectX9Renderer::MakeCachedTexture(u32 uniqueId, const std::vector<u8>& pixels, u32 textureW, u32 textureH, u32 actualW, u32 actualH)
{
    TDX9Texture textureId = mTextureCache.GetCachedTexture(uniqueId, DX_SPRITE_TEXTURE_LIFETIME);
    if (!textureId)
    {
        textureId = MakeTexture32(pixels, textureW, textureH, actualW, actualH);
        mTextureCache.Add(uniqueId, DX_SPRITE_TEXTURE_LIFETIME, textureId);
    }
    return textureId;
}

void DirectX9Renderer::DrawBatches()
{
    if (mBatcher.mBatches.empty() || mBatcher.mVertices.empty())
    {
        return;
    }

    DX_VERIFY(mDevice->SetVertexDeclaration(mVertexDecl));

    // Alloc VBO space
    const u32 requiredVboSize = static_cast<u32>(mBatcher.mVertices.size()) * sizeof(CUSTOMVERTEX);
    if (mVboSize < requiredVboSize || !mVBO)
    {
        LOG_INFO("Allocate VBO bytes %d", requiredVboSize);

        mVBO = nullptr;

        mVboSize = requiredVboSize;
        DX_VERIFY(mDevice->CreateVertexBuffer(mVboSize,
                                              0,
                                              0,
                                              D3DPOOL_MANAGED,
                                              &mVBO,
                                              NULL));
    }

    VOID* pVoid = nullptr;
    DX_VERIFY(mVBO->Lock(0, requiredVboSize, &pVoid, 0));

    // Convert and write vertices into the VBO
    CUSTOMVERTEX* pDstIter = reinterpret_cast<CUSTOMVERTEX*>(pVoid);
    constexpr float fudge = 0.5f;
    for (const IRenderer::PsxVertexData& vertex : mBatcher.mVertices)
    {
        pDstIter->X = vertex.x - fudge;
        pDstIter->Y = vertex.y - fudge;
        pDstIter->Z = 0.5f;
        pDstIter->RHW = 1.0f;
        pDstIter->COLOR = D3DCOLOR_XRGB(static_cast<u8>(vertex.r), static_cast<u8>(vertex.g), static_cast<u8>(vertex.b));

        pDstIter->u = vertex.u;
        pDstIter->v = vertex.v;

        pDstIter->isSemiTrans = static_cast<f32>(vertex.isSemiTrans);
        pDstIter->isShaded = static_cast<f32>(vertex.isShaded);

        pDstIter->palIndex = static_cast<f32>(vertex.paletteIndex);
        pDstIter->blendMode = static_cast<f32>(vertex.blendMode);

        pDstIter->drawType = static_cast<f32>(vertex.drawMode);
        pDstIter->textureUnit = static_cast<f32>(vertex.textureUnitIndex);

        pDstIter++;
    }
    DX_VERIFY(mVBO->Unlock());

    // Alloc index buffer space
    const u32 requiredIndexBufferSize = static_cast<u32>(mBatcher.mIndices.size()) * sizeof(u32);
    if (mIndexBufferSize < requiredIndexBufferSize || !mIndexBuffer)
    {
        LOG_INFO("Allocate index buffer bytes %d", requiredIndexBufferSize);

        mIndexBuffer = nullptr;
        mIndexBufferSize = requiredIndexBufferSize;
        DX_VERIFY(mDevice->CreateIndexBuffer(mIndexBufferSize,
                                             0,
                                             D3DFMT_INDEX32,
                                             D3DPOOL_MANAGED,
                                             &mIndexBuffer,
                                             NULL));
    }
    pVoid = nullptr;
    DX_VERIFY(mIndexBuffer->Lock(0, requiredIndexBufferSize, &pVoid, 0));
    memcpy(pVoid, mBatcher.mIndices.data(), requiredIndexBufferSize);
    DX_VERIFY(mIndexBuffer->Unlock());

    DX_VERIFY(mDevice->SetIndices(mIndexBuffer));
    DX_VERIFY(mDevice->SetStreamSource(0, mVBO, 0, sizeof(CUSTOMVERTEX)));

    u32 idxOffset = 0;
    u32 baseTextureIdx = 0;
    for (std::size_t i = 0; i < mBatcher.mBatches.size() - 1; i++)
    {
        Batcher<ATL::CComPtr<IDirect3DTexture9>, BatchData, kSpriteTextureUnitCount>::RenderBatch& batch = mBatcher.mBatches[i];

        if (batch.mScissor.x == 0 && batch.mScissor.y == 0 && batch.mScissor.w == 0 && batch.mScissor.h == 0)
        {
            // Disable scissor
            DX_VERIFY(mDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE));
        }
        else
        {
            RECT rect = {};
            rect.left = batch.mScissor.x;
            rect.right = batch.mScissor.x + batch.mScissor.w;
            rect.top = batch.mScissor.y;
            rect.bottom = batch.mScissor.y + batch.mScissor.h;

            DX_VERIFY(mDevice->SetScissorRect(&rect));
            DX_VERIFY(mDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE));
        }

        // TODO: check why we get batches of 0 tris in DX
        if (batch.mNumTrisToDraw > 0)
        {
            u32 textureUnit = 0;
            switch (mBatcher.mVertices[mBatcher.mIndices[idxOffset]].drawMode)
            {
                case IRenderer::PsxDrawMode::DefaultFT4:
                    textureUnit = mSpriteUnit;
                    DX_VERIFY(mDevice->SetPixelShader(mPixelShader));
                    break;

                case IRenderer::PsxDrawMode::FG1:
                    textureUnit = mFG1Units[0];
                    DX_VERIFY(mDevice->SetPixelShader(mCamFG1Shader));
                    break;

                case IRenderer::PsxDrawMode::Camera:
                    textureUnit = mCamUnit;
                    DX_VERIFY(mDevice->SetPixelShader(mCamFG1Shader));
                    break;

                case IRenderer::PsxDrawMode::Flat:
                    // No texture bind required for untextured prims
                    DX_VERIFY(mDevice->SetPixelShader(mFlatShader));
                    break;

                case IRenderer::PsxDrawMode::Gas:
                    DX_VERIFY(mDevice->SetPixelShader(mGasShader));
                    DX_VERIFY(mDevice->SetTexture(mGasUnit, *mGasTexture));
                    break;
            }

            // NOTE: We expect this to only ever be 1 due to shader instruction space limitations
            for (u32 j = 0; j < batch.mTexturesInBatch; j++)
            {
                const u32 textureId = batch.mTextureIds[j];
                const u32 batchTextureIdx = batch.TextureIdxForId(textureId);
                auto pTex = mBatcher.mBatchTextures[baseTextureIdx + batchTextureIdx];

                DX_VERIFY(mDevice->SetTexture(textureUnit, *pTex));
            }

            SetupBlendMode(static_cast<u16>(batch.mBlendMode));

            DX_VERIFY(mDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, batch.mNumTrisToDraw * 3, idxOffset, batch.mNumTrisToDraw));

            idxOffset += (batch.mNumTrisToDraw) * 3;
        }

        baseTextureIdx += batch.mTexturesInBatch;
    }

    // TODO: Should be the size of the back buffer ??
    auto tmpTexture = std::make_shared<ATL::CComPtr<IDirect3DTexture9>>();
    DX_VERIFY(mDevice->CreateTexture(640, 240, 0, D3DUSAGE_RENDERTARGET, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &*tmpTexture, nullptr));

    LPDIRECT3DSURFACE9 pTexSurface;
    (*tmpTexture)->GetSurfaceLevel(0, &pTexSurface);
    
    // Copy the frame buffer to tmpTexture
    DX_VERIFY(mDevice->StretchRect(mTextureRenderTarget, NULL, pTexSurface, NULL, D3DTEXF_NONE));

    // Render to the screen instead of the texture
    DX_VERIFY(mDevice->SetRenderTarget(0, mScreenRenderTarget));

    DX_VERIFY(mDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0));

    SetupBlendMode(0);

    for (std::size_t i = 0; i < mBatcher.mBatches.size(); i++)
    {
        if (i == mBatcher.mBatches.size() - 1)
        {
            if (mBatcher.mBatches[i].mNumTrisToDraw > 0)
            {
                Batcher<ATL::CComPtr<IDirect3DTexture9>, BatchData, kSpriteTextureUnitCount>::RenderBatch& batch = mBatcher.mBatches[i];

                // Draw tmpTexture as a full screen quad
                DX_VERIFY(mDevice->SetTexture(mCamUnit, *tmpTexture));

                if (mFramebufferFilter)
                {
                    DX_VERIFY(mDevice->SetPixelShader(mScanLinesShader));
                }
                else
                {
                    DX_VERIFY(mDevice->SetPixelShader(mCamFG1Shader));
                }
                DX_VERIFY(mDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE));
                DX_VERIFY(mDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, batch.mNumTrisToDraw * 3, idxOffset, batch.mNumTrisToDraw));
            }
        }
    }

    pTexSurface->Release();

    DX_VERIFY(mDevice->EndScene());
}

#endif
