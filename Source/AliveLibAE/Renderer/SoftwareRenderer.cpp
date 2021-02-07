#include "stdafx.h"
#include "SoftwareRenderer.hpp"
#include "PsxRender.hpp"
#include "Psx.hpp"

void SoftwareRenderer::Destroy()
{
    SDL_DestroyTexture(mBackBufferTexture);
}

bool SoftwareRenderer::Create(TWindowHandleType window)
{
    mRenderer = SDL_CreateRenderer(window, -1, 0);
    return mRenderer != nullptr;
}

void SoftwareRenderer::Clear(BYTE r, BYTE g, BYTE b)
{
    SDL_SetRenderDrawColor(mRenderer, r, g, b, 255);
    SDL_RenderClear(mRenderer);
}

void SoftwareRenderer::StartFrame(int xOff, int yOff)
{
    mFrame_xOff = xOff;
    mFrame_yOff = yOff;
    if (mFrameStarted)
    {
        ALIVE_FATAL("Start frame called before EndFrame");
    }
    mFrameStarted = true;
}

void SoftwareRenderer::EndFrame()
{
    SDL_RenderPresent(mRenderer);
    mFrameStarted = false;
}

void SoftwareRenderer::BltBackBuffer(const SDL_Rect* pCopyRect, const SDL_Rect* pDst)
{
    SDL_RenderCopy(mRenderer, mBackBufferTexture, pCopyRect, pDst);
}

void SoftwareRenderer::OutputSize(int* w, int* h)
{
    SDL_GetRendererOutputSize(mRenderer, w, h);
}

bool SoftwareRenderer::UpdateBackBuffer(const void* pPixels, int pitch)
{
    if (!mBackBufferTexture)
    {
        return false;
    }
    SDL_UpdateTexture(mBackBufferTexture, nullptr, pPixels, pitch);
    return true;
}

void SoftwareRenderer::CreateBackBuffer(bool filter, int format, int w, int h)
{
    if (!mBackBufferTexture || mLastW != w || mLastH != h)
    {
        if (filter)
        {
            SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
        }
        else
        {
            SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
        }

        SDL_DestroyTexture(mBackBufferTexture);
        mBackBufferTexture = SDL_CreateTexture(mRenderer, format, SDL_TextureAccess::SDL_TEXTUREACCESS_STREAMING, w, h);

        mLastH = w;
        mLastW = h;
    }
}

void SoftwareRenderer::SetTPage(short tPage)
{
    PSX_TPage_Change_4F6430(tPage);
}

void SoftwareRenderer::SetClip(Prim_PrimClipper& clipper)
{
    sPSX_EMU_DrawEnvState_C3D080.field_0_clip.x = clipper.field_C_x;
    sPSX_EMU_DrawEnvState_C3D080.field_0_clip.y = clipper.field_E_y;
    sPSX_EMU_DrawEnvState_C3D080.field_0_clip.w = clipper.mBase.header.mRect.w;
    sPSX_EMU_DrawEnvState_C3D080.field_0_clip.h = clipper.mBase.header.mRect.h;
    PSX_SetDrawEnv_Impl_4FE420(
        16 * clipper.field_C_x,
        16 * clipper.field_E_y,
        (16 * (clipper.field_C_x + clipper.mBase.header.mRect.w)) - 16,
        (16 * (clipper.field_E_y + clipper.mBase.header.mRect.h)) - 16,
        1000 / 2,
        nullptr);
}

void SoftwareRenderer::Draw(Prim_Sprt& sprt)
{
    PrimAny any;
    any.mSprt = &sprt;
    DrawOTag_Render_SPRT(any, static_cast<short>(mFrame_xOff), static_cast<short>(mFrame_yOff), any.mSprt->field_14_w, any.mSprt->field_16_h);
}

void SoftwareRenderer::Draw(Prim_GasEffect& gasEffect)
{
    PSX_RenderLaughingGasEffect_4F7B80(gasEffect.x, gasEffect.y, gasEffect.w, gasEffect.h, gasEffect.pData);
}

void SoftwareRenderer::Draw(Prim_Tile& tile)
{
    PrimAny any;
    any.mTile = &tile;
    DrawOTag_Render_TILE(any, static_cast<short>(mFrame_xOff) + any.mSprt->mBase.vert.x, static_cast<short>(mFrame_yOff) + any.mSprt->mBase.vert.y, any.mTile->field_14_w, any.mTile->field_16_h);
}

void SoftwareRenderer::Draw(Line_F2& line)
{
     PSX_Render_Line_Prim_4F7D90(&line, static_cast<short>(mFrame_xOff), static_cast<short>(mFrame_yOff));
}

void SoftwareRenderer::Draw(Line_G2& line)
{
    PSX_Render_Line_Prim_4F7D90(&line, static_cast<short>(mFrame_xOff), static_cast<short>(mFrame_yOff));
}

void SoftwareRenderer::Draw(Line_G4& line)
{
    PSX_Render_Line_Prim_4F7D90(&line, static_cast<short>(mFrame_xOff), static_cast<short>(mFrame_yOff));
}

void SoftwareRenderer::Draw(Poly_F3& poly)
{
    PrimAny any;
    any.mPolyF3 = &poly;
    DrawPoly(any);
}

void SoftwareRenderer::Draw(Poly_G3& poly)
{
    PrimAny any;
    any.mPolyG3 = &poly;
    DrawPoly(any);
}

void SoftwareRenderer::Draw(Poly_F4& poly)
{
    PrimAny any;
    any.mPolyF4 = &poly;
    DrawPoly(any);
}

void SoftwareRenderer::Draw(Poly_FT4& poly)
{
    PrimAny any;
    any.mPolyFT4 = &poly;
    DrawPoly(any);
}

void SoftwareRenderer::Draw(Poly_G4& poly)
{
    PrimAny any;
    any.mPolyG4 = &poly;
    DrawPoly(any);
}

void SoftwareRenderer::DrawPoly(PrimAny& any)
{
    // This works by func 1 populating some data structure and then func 2 does the actual rendering
    // for POLY_FT4 it may return nullptr as it short circuits this logic and renders the polygon itself in some cases.
    OT_Prim* pPolyBuffer = PSX_Render_Convert_Polys_To_Internal_Format_4F7110(any.mVoid, static_cast<short>(mFrame_xOff), static_cast<short>(mFrame_yOff));
    if (pPolyBuffer)
    {
        PSX_Render_Internal_Format_Polygon_4F7960(pPolyBuffer, static_cast<short>(mFrame_xOff), static_cast<short>(mFrame_yOff));
    }
}

void SoftwareRenderer::Upload(BitDepth bitDepth, const PSX_RECT& rect, const BYTE* pPixels)
{
    switch (bitDepth)
    {
        case BitDepth::e16Bit:
            PSX_LoadImage16_4F5E20(&rect, pPixels);
            break;

        case BitDepth::e8Bit:
        case BitDepth::e4Bit:
            PSX_LoadImage_4F5FB0(&rect, pPixels);
            break;

        default:
            ALIVE_FATAL("unknown bit depth");
            break;
    }
}
