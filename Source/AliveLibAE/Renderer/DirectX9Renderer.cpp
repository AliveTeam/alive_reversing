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
    BYTE data1[4];
    BYTE data2[4];
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
    {0, (sizeof(float) * 6) + sizeof(DWORD), D3DDECLTYPE_UBYTE4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 0},
    {0, (sizeof(float) * 6) + sizeof(DWORD) + 4, D3DDECLTYPE_UBYTE4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 1},
    D3DDECL_END()};

DirectX9Renderer::DirectX9Renderer()
    : mPaletteCache(256)
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

    const char* prog = R"(
    sampler texPalette : register(s1);
    sampler texGas;
    sampler texCamera : register(s0); // s0 = sampler register 0;
    sampler texFG1Masks[4];
    sampler texSpriteSheets[8] : register(s2);

    static const int BLEND_MODE_HALF_DST_ADD_HALF_SRC = 0;
    static const int BLEND_MODE_ONE_DST_ADD_ONE_SRC   = 1;
    static const int BLEND_MODE_ONE_DST_SUB_ONE_SRC   = 2;
    static const int BLEND_MODE_ONE_DST_ADD_QRT_SRC   = 3;

    static const int DRAW_FLAT        = 0;
    static const int DRAW_DEFAULT_FT4 = 1;
    static const int DRAW_CAM         = 2;
    static const int DRAW_FG1         = 3;
    static const int DRAW_GAS         = 4;

    float4 PixelToPalette(float v, int palIndex)
    {
        return tex2D(texPalette, float2(v, palIndex / 255.0f));
    }

    float3 handle_shading(float4 fsShadeColor, float3 texelT, bool isShaded)
    {
        float3 texelP = texelT;

        if (isShaded)
        {
            texelP.r = saturate((texelT.r * (fsShadeColor.r)) / 0.5f);
            texelP.g = saturate((texelT.g * (fsShadeColor.g)) / 0.5f);
            texelP.b = saturate((texelT.b * (fsShadeColor.b)) / 0.5f);
        }

        return texelP;
    }

    float4 handle_final_color(float4 fsShadeColor, float4 src, bool doShading, bool isShaded, int blendMode, bool isSemiTrans)
    {
        // Overwrite with the pal alpha as our src alpha is actually the pal index to use
        fsShadeColor.a = src.a;

        float4 ret = src;

        if (all(src == float4(0.0, 0.0, 0.0, 0.0)))
        {
            return float4(0.0, 0.0, 0.0, 1.0);
        }

        if (doShading)
        {
            ret.rgb = handle_shading(fsShadeColor, src.rgb, isShaded);
        }

        if (isSemiTrans && src.a == 1.0)
        {
            if (blendMode == BLEND_MODE_HALF_DST_ADD_HALF_SRC)
            {
                ret = float4(ret.rgb * 0.5, 0.5);
            }
            else if (blendMode == BLEND_MODE_ONE_DST_ADD_ONE_SRC || blendMode == BLEND_MODE_ONE_DST_SUB_ONE_SRC)
            {
                ret = float4(ret.rgb, 1.0);
            }
            else if (blendMode == BLEND_MODE_ONE_DST_ADD_QRT_SRC)
            {
                ret = float4(ret.rgb * 0.25, 1.0);
            }
        }
        else
        {
            ret = float4(ret.rgb, 0.0);
        }

        return ret;
    }

    float4 draw_default_ft4(float4 fsShadeColor, int textureUnit, int palIndex, float2 fsUV, bool isShaded, int blendMode, bool isSemiTrans)
    {
        float texelSprite = 0.0;

        if (textureUnit == 1)
        {
            texelSprite = tex2D(texSpriteSheets[0], fsUV);
        }

        float4 texelPal = PixelToPalette(texelSprite, palIndex);
        
        float4 finalCol = handle_final_color(fsShadeColor, texelPal, true, isShaded, blendMode, isSemiTrans);

        return finalCol;
    }

    float4 draw_cam(float2 fsUV)
    {
        return tex2D(texCamera, fsUV);
    }

    float4 draw_fg1(int palIndex, float2 fsUV)
    {
        float4 mask = float4(0.0, 0.0, 0.0, 0.0);

        if (palIndex == 0)
        {
            mask = tex2D(texFG1Masks[0], fsUV);
        }
        else if (palIndex == 1)
        {
            mask = tex2D(texFG1Masks[1], fsUV);
        }
        else if (palIndex == 2)
        {
            mask = tex2D(texFG1Masks[2], fsUV);
        }
        else if (palIndex == 3)
        {
            mask = tex2D(texFG1Masks[3], fsUV);
        }

        float4 outColor = float4(tex2D(texCamera, fsUV).rgb, 0.0);

        if (all(mask.rgb == float3(0.0, 0.0, 0.0)))
        {
            outColor = float4(0.0, 0.0, 0.0, 1.0);
        }

        return outColor;
    }

    float4 PS( float4 fsShadeColor : COLOR0, float2 fsUV : TEXCOORD0, int4 data1 : BLENDINDICES0, int4 data2: BLENDINDICES1) : COLOR
    {
        int drawType = data1[0];
        int palIndex = data1[1];
        int isShaded = data1[2];
        int blendMode = data1[3];

        int isSemiTrans = data2[0];
        int textureUnit = data2[1];


        if (drawType == DRAW_DEFAULT_FT4)
        {
            return draw_default_ft4(fsShadeColor, textureUnit, palIndex, fsUV, isShaded, blendMode, isSemiTrans);
        }
        else if (drawType == DRAW_FG1)
        {
            return draw_fg1(palIndex, fsUV);
        }

        // assume cam for now
        return draw_cam(fsUV);
    }
    )";

    LPD3DXBUFFER shader;
    LPD3DXBUFFER err;
    LPD3DXCONSTANTTABLE pConstantTable;
    DWORD dwShaderFlags = D3DXSHADER_SKIPOPTIMIZATION | D3DXSHADER_DEBUG;

    const HRESULT shaderHr = D3DXCompileShader(prog, strlen(prog), NULL, NULL, "PS", "ps_3_0", dwShaderFlags, &shader, &err, &pConstantTable);
    if (FAILED(shaderHr))
    {
        const DWORD errBufferSize = err->GetBufferSize();
        std::string errStr(reinterpret_cast<const char*>(err->GetBufferPointer()), errBufferSize);
        ALIVE_FATAL("D3DXCompileShader failed 0x%08X Compiler returned: %s", shaderHr, errStr.c_str());
    }

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


    DX_VERIFY(mDevice->CreateTexture(640, 240, 0, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &mCamTexture, nullptr));
   
    D3DLOCKED_RECT locked = {};
    DX_VERIFY(mDevice->CreateTexture(256, 256, 0, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &mPaletteTexture, nullptr));
    DX_VERIFY(mPaletteTexture->LockRect(0, &locked, nullptr, D3DLOCK_DISCARD));
    for (u32 y = 0; y < 256; y++)
    {
        u32* p = (u32*) locked.pBits;
        p = p + ((locked.Pitch / 4) * y);
        for (u32 x = 0; x < 256; x++)
        {
            *p = D3DCOLOR_ARGB(255, x, y, x + y);
            p++;
        }
    }

    DX_VERIFY(mPaletteTexture->UnlockRect(0));

    //mCamTexture->SetAutoGenFilterType(D3DTEXF_NONE);

    DX_VERIFY(mCamTexture->LockRect(0, &locked, nullptr, D3DLOCK_DISCARD));
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

    DX_VERIFY(mCamTexture->UnlockRect(0));


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
        HRESULT hr2 = mDevice->BeginScene();
        if (FAILED(hr2))
        {
            LOG_WARNING("Begin scene failed");
        }

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

void DirectX9Renderer::SetupBlendMode(u16 blendMode)
{

    if ((TPageAbr) blendMode == TPageAbr::eBlend_2)
    {
        mDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        mDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
        
        mDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
        
       //GL_VERIFY(glBlendFunc(GL_SRC_ALPHA, GL_ONE));
        //GL_VERIFY(glBlendEquation(GL_FUNC_REVERSE_SUBTRACT));
    }
    else
    {
        mDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
        mDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCALPHA);
        
        mDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

       // GL_VERIFY(glBlendFunc(GL_ONE, GL_SRC_ALPHA));
       // GL_VERIFY(glBlendEquation(GL_FUNC_ADD));
    }
}

u32 DirectX9Renderer::PreparePalette(AnimationPal& pCache)
{
    const PaletteCache::AddResult addRet = mPaletteCache.Add(pCache);

    if (addRet.mAllocated)
    {
        D3DLOCKED_RECT locked = {};
        //const RECT toLock = {0, static_cast<LONG>(addRet.mIndex), 256, static_cast<LONG>(addRet.mIndex) + 1};
        DX_VERIFY(mPaletteTexture->LockRect(0, &locked, nullptr, 0));

        RGBA32* pSrc = &pCache.mPal[0];
        for (u32 y = addRet.mIndex; y < addRet.mIndex + 1; y++)
        {
            u32* p = (u32*) locked.pBits;
            p = p + ((locked.Pitch / 4) * addRet.mIndex);
            for (u32 x = 0; x < ALIVE_COUNTOF(pCache.mPal); x++)
            {
                *p = (pSrc->a << 24) + (pSrc->r << 16) + (pSrc->g << 8) + (pSrc->b);
                p++;
                pSrc++;
            }
        }

        DX_VERIFY(mPaletteTexture->UnlockRect(0));

        // TODO: Write palette data
        //mPaletteTexture->LoadSubImage(0, addRet.mIndex, GL_PALETTE_DEPTH, 1, pCache.mPal);

        //mStats.mPalUploadCount++;
    }

    return addRet.mIndex;
}

void DirectX9Renderer::Draw(Poly_FT4& poly)
{
    // select which vertex format we are using
    DX_VERIFY(mDevice->SetVertexDeclaration(mVertexDecl));

    // select the vertex buffer to display
    DX_VERIFY(mDevice->SetStreamSource(0, v_buffer, 0, sizeof(CUSTOMVERTEX)));

    if (poly.mCam && !poly.mFg1)
    {
        mDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        mDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
        mDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

        SetQuad(0.0f, 0.0f, 640.0f, 240.0f);

        D3DLOCKED_RECT locked = {};
        DX_VERIFY(mCamTexture->LockRect(0, &locked, nullptr, D3DLOCK_DISCARD));

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
        DX_VERIFY(mCamTexture->UnlockRect(0));

        DX_VERIFY(mDevice->SetTexture(0, mCamTexture));
        DX_VERIFY(mDevice->SetTexture(1, mPaletteTexture));

        // copy the vertex buffer to the back buffer
        DX_VERIFY(mDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2));
    }
    else if (poly.mCam && poly.mFg1)
    {
        // EL todo
        /*
        mDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        mDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
        mDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

        SetQuad(0.0f, 0.0f, 640.0f, 240.0f);

        D3DLOCKED_RECT locked = {};
        DX_VERIFY(mCamTexture->LockRect(0, &locked, nullptr, D3DLOCK_DISCARD));

        RGBA32* pSrc = (RGBA32*) poly.mFg1->mImage.mPixels->data();

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
        DX_VERIFY(mCamTexture->UnlockRect(0));

        DX_VERIFY(mDevice->SetTexture(0, mCamTexture));
        DX_VERIFY(mDevice->SetTexture(1, mPaletteTexture));
        DX_VERIFY(mDevice->SetTexture(2, mPaletteTexture));

        // copy the vertex buffer to the back buffer
        DX_VERIFY(mDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2));
        */
    }
    else if (poly.mAnim)
    {
        IDirect3DTexture9* pTextureToUse = PrepareTextureFromAnim(*poly.mAnim);
       
        u8 r = poly.mBase.header.rgb_code.r;
        u8 g = poly.mBase.header.rgb_code.g;
        u8 b = poly.mBase.header.rgb_code.b;

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

        //   {poly.mBase.vert.x, poly.mBase.vert.y, r, g, b, u0, v0, pTga->mWidth, pTga->mHeight, GL_PSX_DRAW_MODE_DEFAULT_FT4, isSemiTrans, isShaded, blendMode, palIndex, 0},
        
        u8 textureUnit = 1;
        SetQuad(1, isSemiTrans, isShaded, blendMode, palIndex, textureUnit, r, g, b, u0, v0, u1, v1, poly);

        DX_VERIFY(mDevice->SetTexture(2, pTextureToUse));
        DX_VERIFY(mDevice->SetTexture(1, mPaletteTexture));
        DX_VERIFY(mDevice->SetTexture(0, mCamTexture));

        // copy the vertex buffer to the back buffer
        DX_VERIFY(mDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2));
    }

}

void DirectX9Renderer::Draw(Poly_G4& /*poly*/)
{

}

void DirectX9Renderer::SetQuad(f32 x, f32 y, f32 w, f32 h)
{
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
            {0, 0, 0, 0}, {0, 0, 0, 0},
        },
        {
            w - fudge,
            y - fudge,
            0.4f,
            1.0f,
            D3DCOLOR_XRGB(128, 128, 128),
            1.0f,
            0.0f,
            {0, 0, 0, 0}, { 0, 0, 0, 0},
        },
        {
            x - fudge,
            h - fudge,
            0.5f,
            1.0f,
            D3DCOLOR_XRGB(128, 128, 128),
            0.0f,
            1.0f,
            {0, 0, 0, 0}, { 0, 0, 0, 0},
        },

        {
            w - fudge,
            y - fudge,
            0.5f,
            1.0f,
            D3DCOLOR_XRGB(128, 128, 128), // TL
            1.0f,
            0.0f,
            {0, 0, 0, 0}, { 0, 0, 0, 0},
        },
        {
            w - fudge,
            h - fudge,
            0.5f,
            1.0f,
            D3DCOLOR_XRGB(128, 128, 128),
            1.0f,
            1.0f,
            {0, 0, 0, 0}, { 0, 0, 0, 0},
        },
        {
            x - fudge,
            h - fudge,
            0.5f,
            1.0f,
            D3DCOLOR_XRGB(128, 128, 128),
            0.0f,
            1.0f,
            {0, 0, 0, 0}, { 0, 0, 0, 0},
        },
    };

    VOID* pVoid = nullptr;

    // lock v_buffer and load the vertices into it
    DX_VERIFY(v_buffer->Lock(0, 0, &pVoid, 0));
    memcpy(pVoid, vertices, sizeof(vertices));
    DX_VERIFY(v_buffer->Unlock());
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
            {type, palIndex, isShaded, blendMode}, {isSemiTrans, textureUnit, 0, 0},
        },
        {
            (f32) X1(&poly) - fudge,
            (f32) Y1(&poly) - fudge,
            0.5f,
            1.0f,
            D3DCOLOR_XRGB(r, g, b),
            u1, // 1
            v0, // 0
            {type, palIndex, isShaded, blendMode}, {isSemiTrans, textureUnit, 0, 0},
        },
        {
            (f32) X2(&poly) - fudge,
            (f32) Y3(&poly) - fudge,
            0.5f,
            1.0f,
            D3DCOLOR_XRGB(r, g, b),
            u0, // 0
            v1, // 1
            {type, palIndex, isShaded, blendMode}, { isSemiTrans, textureUnit, 0, 0},
        },

        {
            (f32) X1(&poly) - fudge,
            (f32) Y1(&poly) - fudge,
            0.5f,
            1.0f,
            D3DCOLOR_XRGB(r, g, b),
            u1, // 1
            v0, // 0
            {type, palIndex, isShaded, blendMode}, { isSemiTrans, textureUnit, 0, 0},
        },
        {
            (f32) X3(&poly) - fudge,
            (f32) Y3(&poly) - fudge,
            0.5f,
            1.0f,
            D3DCOLOR_XRGB(r, g, b),
            u1,   // 1
            v1,   // 1
            {type, palIndex, isShaded, blendMode}, { isSemiTrans, textureUnit, 0, 0},
        },
        {
            (f32) X2(&poly) - fudge,
            (f32) Y2(&poly) - fudge,
            0.5f,
            1.0f,
            D3DCOLOR_XRGB(r, g, b),
            u0, // 0
            v1, // 1
            {type, palIndex, isShaded, blendMode}, { isSemiTrans, textureUnit, 0, 0},
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
        DX_VERIFY(mDevice->CreateTexture(r.mTgaPtr->mWidth, r.mTgaPtr->mHeight, 0, 0, D3DFMT_L8, D3DPOOL_MANAGED, &newTexture, nullptr));

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
