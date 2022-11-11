#include "../stdafx.h"
#include "DirectX9Renderer.hpp"
#include "../../AliveLibCommon/FatalError.hpp"
#include "../../relive_lib/ResourceManagerWrapper.hpp"
#include "../../relive_lib/Animation.hpp"

#ifdef _WIN32

    #undef DIRECT3D_VERSION
    #define DIRECT3D_VERSION 0x0900
    #include <d3dx9.h>
    #include <d3dx9shader.h>

struct CUSTOMVERTEX
{
    FLOAT X, Y, Z, RHW;
    DWORD COLOR; // diffuse
    float u;
    float v;
};

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

void DirectX9TextureCache::DeleteTexture(IDirect3DTexture9* texture)
{
    texture->Release();
}

D3DVERTEXELEMENT9 simple_decl[] = {
    // D3DFVF_XYZRHW
    {0, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITIONT, 0},
    // D3DFVF_DIFFUSE
    {0, sizeof(float) * 4, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
    // D3DFVF_TEX1
    {0, (sizeof(float) * 4) + sizeof(DWORD), D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
    D3DDECL_END()};

DirectX9Renderer::DirectX9Renderer()
    : mPaletteCache(512)
{

}

void DirectX9Renderer::Destroy()
{
    if (mRenderer)
    {
        SDL_DestroyRenderer(mRenderer);
        mRenderer = nullptr;
    }

    if (v_buffer)
    {
        v_buffer->Release();
        v_buffer = nullptr;
    }
}


bool DirectX9Renderer::Create(TWindowHandleType window)
{
    // Find the directx9 driver
    const s32 numDrivers = SDL_GetNumRenderDrivers();
    if (numDrivers < 0)
    {
        LOG_ERROR("Failed to get driver count %s", SDL_GetError());
    }

    LOG_INFO("Got %d drivers", numDrivers);

    s32 index = -1;
    for (s32 i = 0; i < numDrivers; i++)
    {
        SDL_RendererInfo info = {};
        if (SDL_GetRenderDriverInfo(i, &info) < 0)
        {
            LOG_WARNING("Failed to get render %d info %s", i, SDL_GetError());
        }
        else
        {
            LOG_INFO("%d name %s", i, info.name ? info.name : "(null)");
            if (info.name && strstr(info.name, "direct3d"))
            {
                index = i;
                break;
            }
        }
    }

    if (index == -1)
    {
        LOG_WARNING("DirectX9 SDL2 driver not found");
        return false;
    }

    mRenderer = SDL_CreateRenderer(window, index, SDL_RENDERER_ACCELERATED);
    if (!mRenderer)
    {
        LOG_ERROR("Failed to create renderer %s", SDL_GetError());
        return false;
    }

    mDevice = SDL_RenderGetD3D9Device(mRenderer);
    if (!mDevice)
    {
        Destroy();

        LOG_ERROR("Couldnt get DirectX9 device %s", SDL_GetError());
        return false;
    }

    MakeVertexBuffer();
    
    mDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
    mDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    mDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    mDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    mDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

    DX_VERIFY(mDevice->CreateVertexDeclaration(simple_decl, &mVertexDecl));
    DX_VERIFY(mDevice->SetVertexDeclaration(mVertexDecl));

    const char* prog = R"(
    sampler camTex : register(s0); // s0 = sampler register 0

    float4 PS( float4 Col : COLOR0, float2 tex : TEXCOORD0 ) : COLOR
    {
        return tex2D (camTex, tex) * Col;
    }
    )";

    LPD3DXBUFFER shader;
    LPD3DXBUFFER err;
    LPD3DXCONSTANTTABLE pConstantTable;
    DWORD dwShaderFlags = D3DXSHADER_SKIPOPTIMIZATION | D3DXSHADER_DEBUG;
    DX_VERIFY(D3DXCompileShader(prog, strlen(prog), NULL, NULL, "PS", "ps_3_0", dwShaderFlags, &shader, &err, &pConstantTable));
    DX_VERIFY(mDevice->CreatePixelShader((DWORD*)shader->GetBufferPointer(), &mPixelShader));


    D3DCAPS9 hal_caps = {};
    DX_VERIFY(mDevice->GetDeviceCaps(&hal_caps));
    if (hal_caps.PixelShaderVersion < D3DPS_VERSION(3, 0))
    {
        ALIVE_FATAL("Require pixel shader 3.0 or later but got %d.%d", D3DSHADER_VERSION_MAJOR(hal_caps.PixelShaderVersion), D3DSHADER_VERSION_MINOR(hal_caps.PixelShaderVersion));
    }

    DX_VERIFY(mDevice->CreateRenderTarget(640, 240, D3DFMT_A8R8G8B8, D3DMULTISAMPLE_NONE, 0, FALSE, &mTextureRenderTarget, nullptr));

    DX_VERIFY(mDevice->GetRenderTarget(0, &mScreenRenderTarget));
    DX_VERIFY(mDevice->SetRenderTarget(0, mTextureRenderTarget));

    DX_VERIFY(mDevice->CreateTexture(640, 240, 0, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &mTexture, nullptr));

    D3DLOCKED_RECT locked = {};
    DX_VERIFY(mTexture->LockRect(0, &locked, nullptr, D3DLOCK_DISCARD));

    for (u32 y = 0; y < 240; y++)
    {
        u32* p = (u32*) locked.pBits;
        p = p + ((locked.Pitch / 4) * y);
        for (u32 x = 0; x < 640; x++)
        {
            *p = D3DCOLOR_ARGB(255, x, y, x+y);
            p++;
        }
    }

    DX_VERIFY(mTexture->UnlockRect(0));


  //  mDevice->SetRenderState(D3DRS_SPECULARENABLE, 0);
  //  mDevice->SetRenderState(D3DRS_LIGHTING, 0);


    return true;
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
        mDevice->BeginScene();

        // Draw everything to the texture
        mDevice->SetRenderTarget(0, mTextureRenderTarget);

        mDevice->SetPixelShader(mPixelShader);

        DX_VERIFY(mDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(128, 0, 0), 1.0f, 0));
    }
}

void DirectX9Renderer::EndFrame()
{
    if (mFrameStarted)
    {
        DX_VERIFY(mDevice->EndScene());

        // Render to the screen instead of the texture
        DX_VERIFY(mDevice->SetRenderTarget(0, mScreenRenderTarget));

        // Copy the rendered to texture to the entire screen
        mDevice->StretchRect(mTextureRenderTarget, NULL, mScreenRenderTarget, NULL, D3DTEXF_NONE);

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
    SDL_GetRendererOutputSize(mRenderer, w, h);
}

void DirectX9Renderer::SetTPage(u16 /*tPage*/)
{
}

void DirectX9Renderer::SetClip(Prim_PrimClipper& /*clipper*/)
{
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

u32 DirectX9Renderer::PreparePalette(AnimationPal& pCache)
{
    const PaletteCache::AddResult addRet = mPaletteCache.Add(pCache);

    if (addRet.mAllocated)
    {
        // TODO: Write palette data
        //mPaletteTexture->LoadSubImage(0, addRet.mIndex, GL_PALETTE_DEPTH, 1, pCache.mPal);

        //mStats.mPalUploadCount++;
    }

    return addRet.mIndex;
}

void DirectX9Renderer::Draw(Poly_FT4& poly)
{
    IDirect3DTexture9* pTextureToUse = mTexture;

    if (poly.mCam && !poly.mFg1)
    {
        SetQuad(0.0f, 0.0f, 640.0f, 240.0f);

        D3DLOCKED_RECT locked = {};
        DX_VERIFY(mTexture->LockRect(0, &locked, nullptr, D3DLOCK_DISCARD));

        RGBA32* pSrc = (RGBA32*) poly.mCam->mData.mPixels->data();

        for (u32 y = 0; y < 240; y++)
        {
            u32* p = (u32*) locked.pBits;
            p = p + ((locked.Pitch / 4) * y);
            for (u32 x = 0; x < 640; x++)
            {
                *p = (pSrc->a << 24) + (pSrc->r << 16) + (pSrc->g << 8) + (pSrc->b);
                p++;
                pSrc++;
            }
        }
        DX_VERIFY(mTexture->UnlockRect(0));

    }
    else if (poly.mAnim)
    {
        pTextureToUse = PrepareTextureFromAnim(*poly.mAnim);
        /*
        u32 r = poly.mBase.header.rgb_code.r;
        u32 g = poly.mBase.header.rgb_code.g;
        u32 b = poly.mBase.header.rgb_code.b;

        bool isSemiTrans = GetPolyIsSemiTrans(&poly);
        bool isShaded = GetPolyIsShaded(&poly);
        u32 blendMode = GetTPageBlendMode(GetTPage(&poly));

        //

        const PerFrameInfo* pHeader = poly.mAnim->Get_FrameHeader(-1);
        std::shared_ptr<TgaData> pTga = poly.mAnim->mAnimRes.mTgaPtr;

        const u32 palIndex = PreparePalette(*poly.mAnim->mAnimRes.mCurPal);

        u32 u0 = pHeader->mSpriteSheetX;
        u32 v0 = pHeader->mSpriteSheetY;

        u32 u1 = u0 + pHeader->mWidth;
        u32 v1 = v0 + pHeader->mHeight;

        if (poly.mFlipX)
        {
            std::swap(u0, u1);
        }

        if (poly.mFlipY)
        {
            std::swap(v1, v0);
        }

        //   {poly.mBase.vert.x, poly.mBase.vert.y, r, g, b, u0, v0, pTga->mWidth, pTga->mHeight, GL_PSX_DRAW_MODE_DEFAULT_FT4, isSemiTrans, isShaded, blendMode, palIndex, 0},
        */
        SetQuad(poly);

        //DX_VERIFY(mDevice->SetPixelShaderConstantI());

    }

    if ((poly.mCam && !poly.mFg1) || poly.mAnim)
    {
      
        // select which vertex format we are using
        DX_VERIFY(mDevice->SetVertexDeclaration(mVertexDecl));

        // select the vertex buffer to display
        DX_VERIFY(mDevice->SetStreamSource(0, v_buffer, 0, sizeof(CUSTOMVERTEX)));

        DX_VERIFY(mDevice->SetTexture(0, pTextureToUse));

        // copy the vertex buffer to the back buffer
        DX_VERIFY(mDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2));
    }
}

void DirectX9Renderer::Draw(Poly_G4& /*poly*/)
{

}

void DirectX9Renderer::SetQuad(f32 x, f32 y, f32 w, f32 h)
{
    // create the vertices using the CUSTOMVERTEX struct
    CUSTOMVERTEX vertices[] = {
        {
            x,
            y,
            0.5f,
            1.0f,
            D3DCOLOR_XRGB(128, 128, 128), // TL
            0.0f,
            0.0f,
        },
        {
            w,
            y,
            0.4f,
            1.0f,
            D3DCOLOR_XRGB(128, 128, 128),
            1.0f,
            0.0f,
        },
        {
            x,
            h,
            0.5f,
            1.0f,
            D3DCOLOR_XRGB(128, 128, 128),
            0.0f,
            1.0f,
        },

        {
            w,
            y,
            0.5f,
            1.0f,
            D3DCOLOR_XRGB(128, 128, 128), // TL
            1.0f,
            0.0f,
        },
        {
            w,
            h,
            0.5f,
            1.0f,
            D3DCOLOR_XRGB(128, 128, 128),
            1.0f,
            1.0f,
        },
        {
            x,
            h,
            0.5f,
            1.0f,
            D3DCOLOR_XRGB(128, 128, 128),
            0.0f,
            1.0f,
        },
    };

    VOID* pVoid = nullptr;

    // lock v_buffer and load the vertices into it
    DX_VERIFY(v_buffer->Lock(0, 0, &pVoid, 0));
    memcpy(pVoid, vertices, sizeof(vertices));
    DX_VERIFY(v_buffer->Unlock());
}

void DirectX9Renderer::SetQuad(Poly_FT4& poly)
{
    // create the vertices using the CUSTOMVERTEX struct
    CUSTOMVERTEX vertices[] = {
        {
            (f32)X0(&poly),
            (f32)Y0(&poly),
            0.5f,
            1.0f,
            D3DCOLOR_XRGB(128, 128, 128), // TL
            0.0f,
            0.0f,
        },
        {
            (f32) X1(&poly),
            (f32) Y1(&poly),
            0.5f,
            1.0f,
            D3DCOLOR_XRGB(128, 128, 128),
            1.0f,
            0.0f,
        },
        {
            (f32) X2(&poly),
            (f32) Y3(&poly),
            0.5f,
            1.0f,
            D3DCOLOR_XRGB(128, 128, 128),
            0.0f,
            1.0f,
        },

        {
            (f32) X1(&poly),
            (f32) Y1(&poly),
            0.5f,
            1.0f,
            D3DCOLOR_XRGB(128, 128, 128), // TL
            1.0f,
            0.0f,
        },
        {
            (f32) X3(&poly),
            (f32) Y3(&poly),
            0.5f,
            1.0f,
            D3DCOLOR_XRGB(128, 128, 128),
            1.0f,
            1.0f,
        },
        {
            (f32) X2(&poly),
            (f32) Y2(&poly),
            0.5f,
            1.0f,
            D3DCOLOR_XRGB(128, 128, 128),
            0.0f,
            1.0f,
        },
    };

    VOID* pVoid = nullptr;

    // lock v_buffer and load the vertices into it
    DX_VERIFY(v_buffer->Lock(0, 0, &pVoid, 0));
    memcpy(pVoid, vertices, sizeof(vertices));
    DX_VERIFY(v_buffer->Unlock());
}

void DirectX9Renderer::DecreaseResourceLifetimes()
{
    mTextureCache.DecreaseResourceLifetimes();

    mPaletteCache.ResetUseFlags();
}

void DirectX9Renderer::MakeVertexBuffer()
{
    // create a vertex buffer interface called v_buffer
    DX_VERIFY(mDevice->SetVertexDeclaration(mVertexDecl));

    DX_VERIFY(mDevice->CreateVertexBuffer(6 * sizeof(CUSTOMVERTEX),
                               0,
                               0,
                               D3DPOOL_MANAGED,
                               &v_buffer,
                               NULL));

    SetQuad(0.0f, 0.0f, 640.0f, 240.0f);
}


IDirect3DTexture9* DirectX9Renderer::PrepareTextureFromAnim(Animation& anim)
{
    const AnimResource& r = anim.mAnimRes;

    IDirect3DTexture9* textureId = mTextureCache.GetCachedTextureId(r.mUniqueId.Id(), DX_SPRITE_TEXTURE_LIFETIME);

    if (!textureId)
    {
        IDirect3DTexture9* newTexture = nullptr;
        DX_VERIFY(mDevice->CreateTexture(r.mTgaPtr->mWidth, r.mTgaPtr->mHeight, 0, 0, D3DFMT_A8, D3DPOOL_MANAGED, &newTexture, nullptr));

        mTextureCache.Add(r.mUniqueId.Id(), DX_SPRITE_TEXTURE_LIFETIME, newTexture);

        D3DLOCKED_RECT locked = {};
        DX_VERIFY(newTexture->LockRect(0, &locked, nullptr, D3DLOCK_DISCARD));

        u32 off = 0;
        for (u32 y = 0; y < r.mTgaPtr->mHeight; y++)
        {
            u8* p = (u8*) locked.pBits;
            p = p + ((locked.Pitch) * y);
            for (u32 x = 0; x < r.mTgaPtr->mWidth; x++)
            {
                *p = r.mTgaPtr->mPixels[off];
                p++;
                off++;
            }
        }

        DX_VERIFY(newTexture->UnlockRect(0));

        //mStats.mAnimUploadCount++;
    }

    return textureId;
}

#endif
