#include "../stdafx.h"
#include "SoftwareRenderer.hpp"
#include "../PsxRender.hpp"
#include "../Psx.hpp"
#include "../relive_lib/VRam.hpp"

void SoftwareRenderer::Destroy()
{
    SDL_DestroyTexture(mBackBufferTexture);
}

bool SoftwareRenderer::Create(TWindowHandleType window)
{
    mRenderer = SDL_CreateRenderer(window, -1, 0);
    return mRenderer != nullptr;
}

void SoftwareRenderer::Clear(u8 r, u8 g, u8 b)
{
    r = 127;
    g = 127;
    b = 127;
    SDL_SetRenderDrawColor(mRenderer, r, g, b, 255);
}

void SoftwareRenderer::StartFrame(s32 xOff, s32 yOff)
{
    if (!mFrameStarted)
    {
        mFrame_xOff = xOff;
        mFrame_yOff = yOff;
        mFrameStarted = true;
    }
    else
    {
        LOG_WARNING("Start frame called before EndFrame");
    }
}

void SoftwareRenderer::PalFree(const IRenderer::PalRecord& record)
{
    Pal_free(PSX_Point{record.x, record.y}, record.depth);
}

bool SoftwareRenderer::PalAlloc(IRenderer::PalRecord& record)
{
    PSX_RECT rect = {};
    const bool ret = Pal_Allocate(&rect, record.depth);
    record.x = rect.x;
    record.y = rect.y;
    return ret;
}

void SoftwareRenderer::PalSetData(const IRenderer::PalRecord& record, const u8* pPixels)
{
    PSX_RECT rect = {};
    rect.x = record.x;
    rect.y = record.y;
    rect.w = record.depth;
    rect.h = 1;
    PSX_LoadImage16_4F5E20(&rect, pPixels);
}

void SoftwareRenderer::EndFrame()
{
    SDL_RenderPresent(mRenderer);
    mFrameStarted = false;
    SDL_RenderClear(mRenderer);
}

void SoftwareRenderer::BltBackBuffer(const SDL_Rect* /*pCopyRect*/, const SDL_Rect* /*pDst*/)
{
    // SDL_RenderCopy(mRenderer, mBackBufferTexture, pCopyRect, pDst);
}

void SoftwareRenderer::OutputSize(s32* w, s32* h)
{
    SDL_GetRendererOutputSize(mRenderer, w, h);
}

bool SoftwareRenderer::UpdateBackBuffer(const void* pPixels, s32 pitch)
{
    if (!mBackBufferTexture)
    {
        return false;
    }
    SDL_UpdateTexture(mBackBufferTexture, nullptr, pPixels, pitch);
    return true;
}

void SoftwareRenderer::CreateBackBuffer(bool filter, s32 format, s32 w, s32 h)
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

        if (mBackBufferTexture)
        {
            SDL_DestroyTexture(mBackBufferTexture);
        }
        mBackBufferTexture = SDL_CreateTexture(mRenderer, format, SDL_TextureAccess::SDL_TEXTUREACCESS_STREAMING, w, h);

        mLastH = w;
        mLastW = h;
    }
}

void SoftwareRenderer::SetTPage(s16 /*tPage*/)
{
    // PSX_TPage_Change_4F6430(tPage);
}

void SoftwareRenderer::SetClip(Prim_PrimClipper& /*clipper*/)
{
    /*
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
    */
}

void SoftwareRenderer::SetScreenOffset(Prim_ScreenOffset& /*offset*/)
{
}

void SoftwareRenderer::Draw(Prim_Sprt& /*sprt*/)
{
    /*
    PrimAny any;
    any.mSprt = &sprt;
    DrawOTag_Render_SPRT(any, static_cast<s16>(mFrame_xOff), static_cast<s16>(mFrame_yOff), any.mSprt->field_14_w, any.mSprt->field_16_h);
    */
}

void SoftwareRenderer::Draw(Prim_GasEffect& /*gasEffect*/)
{
    // PSX_RenderLaughingGasEffect_4F7B80(gasEffect.x, gasEffect.y, gasEffect.w, gasEffect.h, gasEffect.pData);
}

void SoftwareRenderer::Draw(Prim_Tile& /*tile*/)
{
    /*
    PrimAny any;
    any.mTile = &tile;
    DrawOTag_Render_TILE(any, static_cast<s16>(mFrame_xOff) + any.mSprt->mBase.vert.x, static_cast<s16>(mFrame_yOff) + any.mSprt->mBase.vert.y, any.mTile->field_14_w, any.mTile->field_16_h);
    */
}

void SoftwareRenderer::Draw(Line_F2& /*line*/)
{
    /*
    PSX_Render_Line_Prim_4F7D90(&line, static_cast<s16>(mFrame_xOff), static_cast<s16>(mFrame_yOff));
    */
}

void SoftwareRenderer::Draw(Line_G2& /*line*/)
{
    // PSX_Render_Line_Prim_4F7D90(&line, static_cast<s16>(mFrame_xOff), static_cast<s16>(mFrame_yOff));
}

void SoftwareRenderer::Draw(Line_G4& /*line*/)
{
    // PSX_Render_Line_Prim_4F7D90(&line, static_cast<s16>(mFrame_xOff), static_cast<s16>(mFrame_yOff));
}

void SoftwareRenderer::Draw(Poly_F3& /*poly*/)
{
    /*
    PrimAny any;
    any.mPolyF3 = &poly;
    DrawPoly(any);
    */
    __debugbreak();
}

void SoftwareRenderer::Draw(Poly_G3& poly)
{
    SDL_Vertex vert[3];

    // center
    vert[0].position.x = X0(&poly);
    vert[0].position.y = Y0(&poly);
    vert[0].color.r = R0(&poly);
    vert[0].color.g = G0(&poly);
    vert[0].color.b = B0(&poly);
    vert[0].color.a = 255;

    // left
    vert[1].position.x = X1(&poly);
    vert[1].position.y = Y1(&poly);
    vert[1].color.r = R1(&poly);
    vert[1].color.g = G1(&poly);
    vert[1].color.b = B1(&poly);
    vert[1].color.a = 255;

    // right
    vert[2].position.x = X2(&poly);
    vert[2].position.y = Y2(&poly);
    vert[2].color.r = R2(&poly);
    vert[2].color.g = G2(&poly);
    vert[2].color.b = B2(&poly);
    vert[2].color.a = 255;

    SDL_RenderGeometry(mRenderer, nullptr, vert, 3, nullptr, 0);
}

void SoftwareRenderer::Draw(Poly_F4& poly)
{
    /*
    PrimAny any;
    any.mPolyF4 = &poly;
    DrawPoly(any);
    */

    SDL_Vertex vert[4];

    // center
    vert[0].position.x = X0(&poly);
    vert[0].position.y = Y0(&poly);
    vert[0].color.r = R0(&poly);
    vert[0].color.g = G0(&poly);
    vert[0].color.b = B0(&poly);
    vert[0].color.a = 255;

    // left
    vert[1].position.x = X1(&poly);
    vert[1].position.y = Y1(&poly);
    vert[1].color.r = R0(&poly);
    vert[1].color.g = G0(&poly);
    vert[1].color.b = B0(&poly);
    vert[1].color.a = 255;

    // right
    vert[2].position.x = X2(&poly);
    vert[2].position.y = Y2(&poly);
    vert[2].color.r = R0(&poly);
    vert[2].color.g = G0(&poly);
    vert[2].color.b = B0(&poly);
    vert[2].color.a = 255;

    vert[3].position.x = X3(&poly);
    vert[3].position.y = Y3(&poly);
    vert[3].color.r = R0(&poly);
    vert[3].color.g = G0(&poly);
    vert[3].color.b = B0(&poly);
    vert[3].color.a = 255;

    s32 indexList[6] = {0, 1, 2, 2, 1, 3};
    SDL_RenderGeometry(mRenderer, nullptr, vert, 4, indexList, 6);

    // TODO
    //__debugbreak();
}

void SoftwareRenderer::Draw(Poly_FT4& poly)
{
    /*
    PrimAny any;
    any.mPolyFT4 = &poly;
    DrawPoly(any);
    */

    // TODO: texture test
    SDL_Vertex vert[4];

    // center
    vert[0].position.x = X0(&poly);
    vert[0].position.y = Y0(&poly);
    vert[0].color.r = R0(&poly);
    vert[0].color.g = G0(&poly);
    vert[0].color.b = B0(&poly);
    vert[0].color.a = 255;

    // left
    vert[1].position.x = X1(&poly);
    vert[1].position.y = Y1(&poly);
    vert[1].color.r = R0(&poly);
    vert[1].color.g = G0(&poly);
    vert[1].color.b = B0(&poly);
    vert[1].color.a = 255;

    // right
    vert[2].position.x = X2(&poly);
    vert[2].position.y = Y2(&poly);
    vert[2].color.r = R0(&poly);
    vert[2].color.g = G0(&poly);
    vert[2].color.b = B0(&poly);
    vert[2].color.a = 255;

    vert[3].position.x = X3(&poly);
    vert[3].position.y = Y3(&poly);
    vert[3].color.r = R0(&poly);
    vert[3].color.g = G0(&poly);
    vert[3].color.b = B0(&poly);
    vert[3].color.a = 255;

    s32 indexList[6] = {0, 1, 2, 2, 1, 3};
    SDL_RenderGeometry(mRenderer, nullptr, vert, 4, indexList, 6);
}

void SoftwareRenderer::Draw(Poly_G4& poly)
{
    /*
    PrimAny any;
    any.mPolyG4 = &poly;
    DrawPoly(any);
    */

    SDL_Vertex vert[4];

    // center
    vert[0].position.x = X0(&poly);
    vert[0].position.y = Y0(&poly);
    vert[0].color.r = R0(&poly);
    vert[0].color.g = G0(&poly);
    vert[0].color.b = B0(&poly);
    vert[0].color.a = 255;

    // left
    vert[1].position.x = X1(&poly);
    vert[1].position.y = Y1(&poly);
    vert[1].color.r = R1(&poly);
    vert[1].color.g = G1(&poly);
    vert[1].color.b = B1(&poly);
    vert[1].color.a = 255;

    // right
    vert[2].position.x = X2(&poly);
    vert[2].position.y = Y2(&poly);
    vert[2].color.r = R2(&poly);
    vert[2].color.g = G2(&poly);
    vert[2].color.b = B2(&poly);
    vert[2].color.a = 255;

    vert[3].position.x = X3(&poly);
    vert[3].position.y = Y3(&poly);
    vert[3].color.r = R3(&poly);
    vert[3].color.g = G3(&poly);
    vert[3].color.b = B3(&poly);
    vert[3].color.a = 255;

    s32 indexList[6] = {0, 1, 2, 2, 1, 3};
    SDL_RenderGeometry(mRenderer, nullptr, vert, 4, indexList, 6);
}

void SoftwareRenderer::DrawPoly(PrimAny& /*any*/)
{
    /*
    // This works by func 1 populating some data structure and then func 2 does the actual rendering
    // for POLY_FT4 it may return nullptr as it s16 circuits this logic and renders the polygon itself in some cases.
    OT_Prim* pPolyBuffer = PSX_Render_Convert_Polys_To_Internal_Format_4F7110(any.mVoid, static_cast<s16>(mFrame_xOff), static_cast<s16>(mFrame_yOff));
    if (pPolyBuffer)
    {
        PSX_Render_Internal_Format_Polygon_4F7960(pPolyBuffer, static_cast<s16>(mFrame_xOff), static_cast<s16>(mFrame_yOff));
    }
    */
}

void SoftwareRenderer::Upload(BitDepth bitDepth, const PSX_RECT& /*rect*/, const u8* /*pPixels*/)
{
    switch (bitDepth)
    {
        case BitDepth::e16Bit:
            // PSX_LoadImage16_4F5E20(&rect, pPixels);
            break;

        case BitDepth::e8Bit:
        case BitDepth::e4Bit:
            // PSX_LoadImage_4F5FB0(&rect, pPixels);
            break;

        default:
            ALIVE_FATAL("unknown bit depth");
            break;
    }
}
