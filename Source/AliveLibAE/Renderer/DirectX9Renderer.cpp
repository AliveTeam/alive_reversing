#include "../stdafx.h"
#include "DirectX9Renderer.hpp"
#include "../../AliveLibCommon/FatalError.hpp"
#include "../../relive_lib/ResourceManagerWrapper.hpp"
#include "../../relive_lib/Animation.hpp"
#include "../Font.hpp"
#include "pixel_shader.h"

#ifdef _WIN32

    #undef DIRECT3D_VERSION
    #define DIRECT3D_VERSION 0x0900
    #include <d3dx9.h>

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

void DirectX9TextureCache::DeleteTexture(ATL::CComPtr<IDirect3DTexture9> /*texture*/)
{

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
    : mPaletteCache(256)
{
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "direct3d");
    // WARNING: SDL2 bug here, if batching is turned off then the 1st call to BeginScene will
    // fail and resizing the window will then cause many other D3D device calls to fail.
    SDL_SetHint(SDL_HINT_RENDER_BATCHING, "1");

    mRenderer = std::make_unique<SDL_Renderer_RAII>(SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED));
    if (!mRenderer->mRenderer)
    {
        ALIVE_FATAL("Failed to create renderer %s", SDL_GetError());
    }

    SDL_RendererInfo info = {};
    if (SDL_GetRendererInfo(mRenderer->mRenderer, &info) < 0)
    {
        ALIVE_FATAL("Failed to get renderer info %s",  SDL_GetError());
    }

    if (info.name && strcmp(info.name, "direct3d") != 0)
    {
        ALIVE_FATAL("SDL picked driver %s but we expected direct3d", info.name ? info.name : "(null)");
    }

    mDevice = SDL_RenderGetD3D9Device(mRenderer->mRenderer);
    if (!mDevice)
    {
        ALIVE_FATAL("Couldnt get DirectX9 device %s", SDL_GetError());
    }

    D3DCAPS9 dxCaps = {};
    DX_VERIFY(mDevice->GetDeviceCaps(&dxCaps));
    if (dxCaps.PixelShaderVersion < D3DPS_VERSION(2, 0))
    {
        ALIVE_FATAL("Require pixel shader 2.0 or later but got %d.%d", D3DSHADER_VERSION_MAJOR(dxCaps.PixelShaderVersion), D3DSHADER_VERSION_MINOR(dxCaps.PixelShaderVersion));
    }

    LOG_INFO("PS 2.0 max instructions %d PS 3.0 max instructions %d", dxCaps.PS20Caps.NumInstructionSlots, dxCaps.MaxPixelShader30InstructionSlots);
    LOG_INFO("Max texture w %d max texture h %d", dxCaps.MaxTextureWidth, dxCaps.MaxTextureHeight);

    MakeVertexBuffer();

    for (u32 i = 0; i < 8; i++)
    {
        mDevice->SetSamplerState(i, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
        mDevice->SetSamplerState(i, D3DSAMP_MINFILTER, D3DTEXF_POINT);
        mDevice->SetSamplerState(i, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
        mDevice->SetSamplerState(i, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
        mDevice->SetSamplerState(i, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
        mDevice->SetSamplerState(i, D3DSAMP_ADDRESSW, D3DTADDRESS_CLAMP);
    }

    mDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
    mDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

    mDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    mDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    mDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

    mDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

    DX_VERIFY(mDevice->CreateVertexDeclaration(simple_decl, &mVertexDecl));
    DX_VERIFY(mDevice->SetVertexDeclaration(mVertexDecl));

    DX_VERIFY(mDevice->CreatePixelShader((DWORD*) pixel_shader, &mPixelShader));

    DX_VERIFY(mDevice->CreateRenderTarget(640, 240, D3DFMT_A8R8G8B8, D3DMULTISAMPLE_NONE, 0, FALSE, &mTextureRenderTarget, nullptr));

    DX_VERIFY(mDevice->GetRenderTarget(0, &mScreenRenderTarget));
    DX_VERIFY(mDevice->SetRenderTarget(0, mTextureRenderTarget));


    // DX_VERIFY(mDevice->CreateTexture(640, 240, 0, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &mCamTexture, nullptr));

    D3DLOCKED_RECT locked = {};
    DX_VERIFY(mDevice->CreateTexture(256, 256, 0, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &mPaletteTexture, nullptr));
    const RGBA32 kColourMagenta = {255, 0, 255, 255};
    DXTexture::FillTexture(*mPaletteTexture, 256, 256, kColourMagenta);

    // mCamTexture->SetAutoGenFilterType(D3DTEXF_NONE);

    // DXTexture::FillTexture(*mCamTexture, 640, 240, kColourMagenta);


    //  mDevice->SetRenderState(D3DRS_SPECULARENABLE, 0);
    //  mDevice->SetRenderState(D3DRS_LIGHTING, 0);


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

void DirectX9Renderer::StartFrame(s32 /*xOff*/, s32 /*yOff*/)
{
    if (!mFrameStarted)
    {
        mFrameStarted = true;

        // TODO: the 1st call fails :)
        HRESULT hr2 = mDevice->BeginScene();
        if (FAILED(hr2))
        {
            LOG_WARNING("Begin scene failed");
        }

        // Draw everything to the texture
        mDevice->SetRenderTarget(0, mTextureRenderTarget);
        DX_VERIFY(mDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0));

        mDevice->SetPixelShader(mPixelShader);
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
        RECT dstRect = {0, 0, 640, 240};
        mDevice->StretchRect(mTextureRenderTarget, NULL, mScreenRenderTarget, nullptr, D3DTEXF_POINT);

        DX_VERIFY(mDevice->Present(NULL, NULL, NULL, NULL));
        // SDL_RenderPresent(mRenderer);

        mFrameStarted = false;
    }

    // Always decrease resource lifetimes regardless of drawing to prevent
    // memory leaks
    DecreaseResourceLifetimes();
}

void DirectX9Renderer::OutputSize(s32* w, s32* h)
{
    SDL_GetRendererOutputSize(mRenderer->mRenderer, w, h);
}

void DirectX9Renderer::SetTPage(u16 /*tPage*/)
{
}

void DirectX9Renderer::SetClip(Prim_PrimClipper& clipper)
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

void DirectX9Renderer::SetScreenOffset(Prim_ScreenOffset& /*offset*/)
{
}

void DirectX9Renderer::ToggleFilterScreen()
{
}

void DirectX9Renderer::ToggleKeepAspectRatio()
{
}

void DirectX9Renderer::Draw(Prim_Sprt& /*sprt*/)
{
}

void DirectX9Renderer::Draw(Prim_GasEffect& /*gasEffect*/)
{
}

void DirectX9Renderer::Draw(Prim_Tile& /*tile*/)
{
}

void DirectX9Renderer::Draw(Line_F2& /*line*/)
{
}

void DirectX9Renderer::Draw(Line_G2& /*line*/)
{
}

void DirectX9Renderer::Draw(Line_G4& /*line*/)
{
}

void DirectX9Renderer::Draw(Poly_F3& /*poly*/)
{
}

void DirectX9Renderer::Draw(Poly_G3& /*poly*/)
{
}

void DirectX9Renderer::Draw(Poly_F4& /*poly*/)
{
}

// TODO: Copy pasted from GL renderer
inline u16 GetTPageBlendMode(u16 tpage)
{
    return (tpage >> 4) & 3;
}

void DirectX9Renderer::SetupBlendMode(u16 blendMode)
{

    if ((TPageAbr) blendMode == TPageAbr::eBlend_2)
    {
        mDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        mDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
        
        mDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
    }
    else
    {
        mDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
        mDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCALPHA);
        
        mDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
    }
}

u32 DirectX9Renderer::PreparePalette(AnimationPal& pCache)
{
    const PaletteCache::AddResult addRet = mPaletteCache.Add(pCache);

    if (addRet.mAllocated)
    {
        DXTexture::LoadSubImage(*mPaletteTexture, 0, addRet.mIndex, ALIVE_COUNTOF(pCache.mPal), 1, &pCache.mPal[0]);
        //mStats.mPalUploadCount++;
    }

    return addRet.mIndex;
}

void DirectX9Renderer::Draw(Poly_FT4& poly)
{
    // select which vertex format we are using
    DX_VERIFY(mDevice->SetVertexDeclaration(mVertexDecl));

    // select the vertex buffer to display
    DX_VERIFY(mDevice->SetStreamSource(0, mCameraVBO, 0, sizeof(CUSTOMVERTEX)));

    u8 r = poly.mBase.header.rgb_code.r;
    u8 g = poly.mBase.header.rgb_code.g;
    u8 b = poly.mBase.header.rgb_code.b;

    if (poly.mCam && !poly.mFg1)
    {
        /*
        u8 blendMode = static_cast<u8>(GetTPageBlendMode(GetTPage(&poly)));
        SetupBlendMode(blendMode);

        SetQuad(0.0f, 0.0f, 640.0f, 240.0f);

        auto pSrc = reinterpret_cast<const RGBA32*>(poly.mCam->mData.mPixels->data());
        DXTexture::LoadSubImage(*mCamTexture, 0, 0, 640, 240, pSrc);

        DX_VERIFY(mDevice->SetTexture(mCamUnit, mCamTexture));
        DX_VERIFY(mDevice->SetTexture(mPalUnit, mPaletteTexture));
        DX_VERIFY(mDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2));
        */
    }
    else if (poly.mCam && poly.mFg1)
    {
        /*
        IDirect3DTexture9* pTextureToUse = mTextureCache.GetCachedTextureId(poly.mFg1->mUniqueId.Id(), DX_SPRITE_TEXTURE_LIFETIME);
        if (!pTextureToUse)
        {
            DX_VERIFY(mDevice->CreateTexture(poly.mFg1->mImage.mWidth, poly.mFg1->mImage.mHeight, 0, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &pTextureToUse, nullptr));

            mTextureCache.Add(poly.mFg1->mUniqueId.Id(), DX_SPRITE_TEXTURE_LIFETIME, pTextureToUse);

            DXTexture::LoadSubImage(*pTextureToUse, 0, 0, poly.mFg1->mImage.mWidth, poly.mFg1->mImage.mHeight, poly.mFg1->mImage.mPixels->data());

            auto pSrc = reinterpret_cast<const RGBA32*>(poly.mFg1->mImage.mPixels->data());
            DXTexture::LoadSubImage(*pTextureToUse, 0, 0, 640, 240, pSrc);
            // mStats.mFg1UploadCount++;
        }

        u8 blendMode = static_cast<u8>(GetTPageBlendMode(GetTPage(&poly)));
        SetupBlendMode(blendMode);

        u8 textureUnit = 1;
        u8 palIdx = mFG1Units[0];
        SetQuad(3, false, false, blendMode, palIdx, textureUnit, 128, 128, 128, 0.0f, 0.0f, 1.0f, 1.0f, poly);


        DX_VERIFY(mDevice->SetTexture(mCamUnit, mCamTexture));
        DX_VERIFY(mDevice->SetTexture(mPalUnit, mPaletteTexture));
        DX_VERIFY(mDevice->SetTexture(mFG1Units[0], pTextureToUse));
        DX_VERIFY(mDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2));
        */
    }
    else if (poly.mAnim)
    {
        IDirect3DTexture9* pTextureToUse = PrepareTextureFromAnim(*poly.mAnim);
       

        bool isSemiTrans = GetPolyIsSemiTrans(&poly);
        bool isShaded = GetPolyIsShaded(&poly);
        u8 blendMode = static_cast<u8>(GetTPageBlendMode(GetTPage(&poly)));
        SetupBlendMode(blendMode);

        //

        const PerFrameInfo* pHeader = poly.mAnim->Get_FrameHeader(-1);
        std::shared_ptr<TgaData> pTga = poly.mAnim->mAnimRes.mTgaPtr;

        u8 palIndex = static_cast<u8>(PreparePalette(*poly.mAnim->mAnimRes.mCurPal));

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

        u8 textureUnit = 1;
        SetQuad(1, isSemiTrans, isShaded, blendMode, palIndex, textureUnit, r, g, b, u0, v0, u1, v1, poly);

        DX_VERIFY(mDevice->SetTexture(mSpriteUnit, pTextureToUse));
        DX_VERIFY(mDevice->SetTexture(mPalUnit, mPaletteTexture));
       // DX_VERIFY(mDevice->SetTexture(mCamUnit, mCamTexture));
        DX_VERIFY(mDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2));
    }
    else if (poly.mFont)
    {
        IDirect3DTexture9* pTextureToUse = mTextureCache.GetCachedTextureId(poly.mFont->field_C_resource_id.mUniqueId.Id(), DX_SPRITE_TEXTURE_LIFETIME);
        std::shared_ptr<TgaData> pTga = poly.mFont->field_C_resource_id.mTgaPtr;

        if (!pTextureToUse)
        {
            DX_VERIFY(mDevice->CreateTexture(pTga->mWidth, pTga->mHeight, 0, 0, D3DFMT_L8, D3DPOOL_MANAGED, &pTextureToUse, nullptr));

            mTextureCache.Add(poly.mFont->field_C_resource_id.mUniqueId.Id(), DX_SPRITE_TEXTURE_LIFETIME, pTextureToUse);

            DXTexture::LoadSubImage(*pTextureToUse, 0, 0, pTga->mWidth, pTga->mHeight, pTga->mPixels.data());

            // mStats.mFontUploadCount++;
        }

        FontResource& fontRes = poly.mFont->field_C_resource_id;

        auto pPal = fontRes.mCurPal;
        const u8 palIndex = static_cast<u8>(PreparePalette(*pPal));

        float u0 = U0(&poly) / (f32)pTga->mWidth;
        float v0 = V0(&poly) / (f32) pTga->mHeight;

        float u1 = U3(&poly) / (f32) pTga->mWidth;
        float v1 = V3(&poly) / (f32) pTga->mHeight;

        u8 textureUnit = 1;

        bool isSemiTrans = GetPolyIsSemiTrans(&poly);
        bool isShaded = GetPolyIsShaded(&poly);
        u8 blendMode = static_cast<u8>(GetTPageBlendMode(GetTPage(&poly)));
        SetupBlendMode(blendMode);
        SetQuad(1, isSemiTrans, isShaded, blendMode, palIndex, textureUnit, r, g, b, u0, v0, u1, v1, poly);

        DX_VERIFY(mDevice->SetTexture(mSpriteUnit, pTextureToUse));
        DX_VERIFY(mDevice->SetTexture(mPalUnit, mPaletteTexture));
        DX_VERIFY(mDevice->SetTexture(mCamUnit, mCamTexture));
        DX_VERIFY(mDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2));
    }
}

void DirectX9Renderer::Draw(Poly_G4& /*poly*/)
{

}

static float FromBool(const bool v)
{
    return v ? 1.0f : 0.0f;
}

static float FromInt(const u32 v)
{
    return static_cast<float>(v);
}

void DirectX9Renderer::SetQuad(f32 x, f32 y, f32 w, f32 h)
{
    bool isSemiTrans = false;
    bool isShaded = false;
    u8 type = 2;
    u8 blendMode = 0;
    u8 palIndex = 0;
    u8 textureUnit =0;

    float fudge = 0.5f;
    // create the vertices using the CUSTOMVERTEX struct
    CUSTOMVERTEX vertices[] = {
        {
            x - fudge,
            y - fudge,
            0.5f,
            1.0f,
            D3DCOLOR_XRGB(128, 128, 128), // TL
            0.0f,
            0.0f,
            FromBool(isSemiTrans),
            FromBool(isShaded),
            FromInt(palIndex),
            FromInt(blendMode),
            FromInt(type),
            FromInt(textureUnit)
        },
        {
            w - fudge,
            y - fudge,
            0.4f,
            1.0f,
            D3DCOLOR_XRGB(128, 128, 128),
            1.0f,
            0.0f,
            FromBool(isSemiTrans),
            FromBool(isShaded),
            FromInt(palIndex),
            FromInt(blendMode),
            FromInt(type),
            FromInt(textureUnit)
        },
        {
            x - fudge,
            h - fudge,
            0.5f,
            1.0f,
            D3DCOLOR_XRGB(128, 128, 128),
            0.0f,
            1.0f,
            FromBool(isSemiTrans),
            FromBool(isShaded),
            FromInt(palIndex),
            FromInt(blendMode),
            FromInt(type),
            FromInt(textureUnit)
        },

        {
            w - fudge,
            y - fudge,
            0.5f,
            1.0f,
            D3DCOLOR_XRGB(128, 128, 128), // TL
            1.0f,
            0.0f,
            FromBool(isSemiTrans),
            FromBool(isShaded),
            FromInt(palIndex),
            FromInt(blendMode),
            FromInt(type),
            FromInt(textureUnit)
        },
        {
            w - fudge,
            h - fudge,
            0.5f,
            1.0f,
            D3DCOLOR_XRGB(128, 128, 128),
            1.0f,
            1.0f,
            FromBool(isSemiTrans),
            FromBool(isShaded),
            FromInt(palIndex),
            FromInt(blendMode),
            FromInt(type),
            FromInt(textureUnit)
        },
        {
            x - fudge,
            h - fudge,
            0.5f,
            1.0f,
            D3DCOLOR_XRGB(128, 128, 128),
            0.0f,
            1.0f,
            FromBool(isSemiTrans),
            FromBool(isShaded),
            FromInt(palIndex),
            FromInt(blendMode),
            FromInt(type),
            FromInt(textureUnit)
        },
    };

    VOID* pVoid = nullptr;

    // lock mCameraVBO and load the vertices into it
    DX_VERIFY(mCameraVBO->Lock(0, 0, &pVoid, 0));
    memcpy(pVoid, vertices, sizeof(vertices));
    DX_VERIFY(mCameraVBO->Unlock());
}

void DirectX9Renderer::SetQuad(u8 type, bool isSemiTrans, bool isShaded, u8 blendMode, u8 palIndex, u8 textureUnit, u8 r, u8 g, u8 b, float u0, float v0, float u1, float v1, Poly_FT4& poly)
{
    float fudge = 0.5f;
    // create the vertices using the CUSTOMVERTEX struct
    CUSTOMVERTEX vertices[] = {
        {
            (f32) X0(&poly) - fudge,
            (f32) Y0(&poly) - fudge,
            0.5f,
            1.0f,
            D3DCOLOR_XRGB(r, g, b),
            u0, // 0
            v0, // 0
            FromBool(isSemiTrans),
            FromBool(isShaded),
            FromInt(palIndex),
            FromInt(blendMode),
            FromInt(type),
            FromInt(textureUnit)
        },
        {
            (f32) X1(&poly) - fudge,
            (f32) Y1(&poly) - fudge,
            0.5f,
            1.0f,
            D3DCOLOR_XRGB(r, g, b),
            u1, // 1
            v0, // 0
            FromBool(isSemiTrans),
            FromBool(isShaded),
            FromInt(palIndex),
            FromInt(blendMode),
            FromInt(type),
            FromInt(textureUnit)
        },
        {
            (f32) X2(&poly) - fudge,
            (f32) Y3(&poly) - fudge,
            0.5f,
            1.0f,
            D3DCOLOR_XRGB(r, g, b),
            u0, // 0
            v1, // 1
            FromBool(isSemiTrans),
            FromBool(isShaded),
            FromInt(palIndex),
            FromInt(blendMode),
            FromInt(type),
            FromInt(textureUnit)
        },

        {
            (f32) X1(&poly) - fudge,
            (f32) Y1(&poly) - fudge,
            0.5f,
            1.0f,
            D3DCOLOR_XRGB(r, g, b),
            u1, // 1
            v0, // 0
            FromBool(isSemiTrans),
            FromBool(isShaded),
            FromInt(palIndex),
            FromInt(blendMode),
            FromInt(type),
            FromInt(textureUnit)
        },
        {
            (f32) X3(&poly) - fudge,
            (f32) Y3(&poly) - fudge,
            0.5f,
            1.0f,
            D3DCOLOR_XRGB(r, g, b),
            u1,   // 1
            v1,   // 1
            FromBool(isSemiTrans),
            FromBool(isShaded),
            FromInt(palIndex),
            FromInt(blendMode),
            FromInt(type),
            FromInt(textureUnit)
        },
        {
            (f32) X2(&poly) - fudge,
            (f32) Y2(&poly) - fudge,
            0.5f,
            1.0f,
            D3DCOLOR_XRGB(r, g, b),
            u0, // 0
            v1, // 1
            FromBool(isSemiTrans),
            FromBool(isShaded),
            FromInt(palIndex),
            FromInt(blendMode),
            FromInt(type),
            FromInt(textureUnit)
        },
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

    SetQuad(0.0f, 0.0f, 640.0f, 240.0f);
}


IDirect3DTexture9* DirectX9Renderer::PrepareTextureFromAnim(Animation& anim)
{
    const AnimResource& r = anim.mAnimRes;

    IDirect3DTexture9* textureId = mTextureCache.GetCachedTextureId(r.mUniqueId.Id(), DX_SPRITE_TEXTURE_LIFETIME);

    if (!textureId)
    {
        DX_VERIFY(mDevice->CreateTexture(r.mTgaPtr->mWidth, r.mTgaPtr->mHeight, 0, 0, D3DFMT_L8, D3DPOOL_MANAGED, &textureId, nullptr));

        mTextureCache.Add(r.mUniqueId.Id(), DX_SPRITE_TEXTURE_LIFETIME, textureId);

        DXTexture::LoadSubImage(*textureId, 0, 0, r.mTgaPtr->mWidth, r.mTgaPtr->mHeight, r.mTgaPtr->mPixels.data());
        //mStats.mAnimUploadCount++;
    }

    return textureId;
}

#endif
