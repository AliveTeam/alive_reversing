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
    DWORD COLOR;
    float u;
    float v;
};
#define CUSTOMFVF (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

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

// TODO: Temp test code
IDirect3DTexture9* DirectX9Renderer::MakeTexture(const char* fileName)
{
    IDirect3DTexture9* d3dTexture;
    D3DXIMAGE_INFO SrcInfo; // Optional

    // Use a magenta colourkey
    D3DCOLOR colorkey = 0xFFFF00FF;

    // Load image from file
    HRESULT hr = D3DXCreateTextureFromFileEx(mDevice, fileName, 140, 140, 0, 0,
                                             D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT,
                                             colorkey, &SrcInfo, NULL, &d3dTexture);
    if (FAILED(hr))
    {
        return NULL;
    }

    // Return the newly made texture
    return d3dTexture;
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

    /*
    const char* prog = "yo";
    LPD3DXBUFFER shader;
    LPD3DXBUFFER err;
    DX_VERIFY(D3DXAssembleShader(prog, strlen(prog), NULL, NULL, 0, &shader, &err));
    */

    //mDevice->CreateVertexShader();
    //mDevice->CreatePixelShader();

    D3DCAPS9 hal_caps = {};
    DX_VERIFY(mDevice->GetDeviceCaps(&hal_caps));
    if (hal_caps.PixelShaderVersion < D3DPS_VERSION(3, 0))
    {
        ALIVE_FATAL("Require pixel shader 3.0 or later but got %d.%d", D3DSHADER_VERSION_MAJOR(hal_caps.PixelShaderVersion), D3DSHADER_VERSION_MINOR(hal_caps.PixelShaderVersion));
    }

    DX_VERIFY(mDevice->CreateRenderTarget(640, 240, D3DFMT_A8R8G8B8, D3DMULTISAMPLE_NONE, 0, FALSE, &mRenderTarget, nullptr));

    //mDevice->CreatePixelShader()
    //mDevice->SetPixelShader();

    //mTexture = MakeTexture("C:\\Data\\Poggings.bmp");
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

    return true;
}

void DirectX9Renderer::Clear(u8 /*r*/, u8 /*g*/, u8 /*b*/)
{
    //mDevice->Clear(0, nullptr, D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET | D3DCLEAR_STENCIL, D3DCOLOR_XRGB(r, g, b), 1.0f, 0);

}

void DirectX9Renderer::StartFrame(s32 /*xOff*/, s32 /*yOff*/)
{
    DX_VERIFY(mDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(128, 0, 0), 1.0f, 0));
    if (!mFrameStarted)
    {
        mFrameStarted = true;

        // TODO: the 1st call fails :)
        mDevice->BeginScene();

       // mDevice->SetRenderTarget(0, mRenderTarget);

    }
}

void DirectX9Renderer::EndFrame()
{
    if (mFrameStarted)
    {

        DX_VERIFY(mDevice->EndScene());

        //mDevice->SetRenderTarget(0, nullptr);


        DX_VERIFY(mDevice->Present(NULL, NULL, NULL, NULL));
        // SDL_RenderPresent(mRenderer);

        mFrameStarted = false;
    }
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

void DirectX9Renderer::Draw(Poly_FT4& poly)
{
    if (poly.mCam)
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
        AnimResource& r = poly.mAnim->mAnimRes;
        if (r.mTgaPtr)
        {
            SetQuad(poly);
//            SetQuad(X0(&poly), Y0(&poly), static_cast<f32>(r.mTgaPtr->mWidth), static_cast<f32>(r.mTgaPtr->mHeight));
        }
    }

    if (poly.mAnim)
    {
      
        // select which vertex format we are using
        DX_VERIFY(mDevice->SetFVF(CUSTOMFVF));

        // select the vertex buffer to display
        DX_VERIFY(mDevice->SetStreamSource(0, v_buffer, 0, sizeof(CUSTOMVERTEX)));

        DX_VERIFY(mDevice->SetTexture(0, mTexture));

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
            0.5f,
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
            (f32) Y0(&poly),
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

void DirectX9Renderer::MakeVertexBuffer()
{
    // create a vertex buffer interface called v_buffer
    DX_VERIFY(mDevice->CreateVertexBuffer(6 * sizeof(CUSTOMVERTEX),
                               0,
                               CUSTOMFVF,
                               D3DPOOL_MANAGED,
                               &v_buffer,
                               NULL));

    SetQuad(0.0f, 0.0f, 640.0f, 240.0f);
}

#endif
