#include "../stdafx.h"
#include "SoftwareRenderer.hpp"
#include "../PsxRender.hpp"
#include "../Psx.hpp"

#include "../relive_lib/Animation.hpp"
#include "../relive_lib/data_conversion/rgb_conversion.hpp"
#include "../Font.hpp"

#include "../AliveLibCommon/FatalError.hpp"

#if SDL_VERTEX_IS_SUPPORTED
void set_pixel(SDL_Surface* surface, int x, int y, u32 pixel)
{
    Uint8* target_pixel = (Uint8*) surface->pixels + y * surface->pitch + x * sizeof(u32);
    *(u32*) target_pixel = pixel;
}

void SoftwareRenderer::Destroy()
{
    SDL_DestroyTexture(mBackBufferTexture);
}

bool SoftwareRenderer::Create(TWindowHandleType window)
{
    mWindow = window;
    mRenderer = SDL_CreateRenderer(window, -1, 0);
    if (mRenderer)
    {
        SDL_RenderSetLogicalSize(mRenderer, 640, 480);
        //SDL_RenderSetScale(mRenderer, 1.0f, 2.0f);
    }
    return mRenderer != nullptr;
}

void SoftwareRenderer::Clear(u8 r, u8 g, u8 b)
{
    r = 127;
    g = 127;
    b = 127;
    SDL_SetRenderDrawColor(mRenderer, r, g, b, 255);
    
    //SDL_SetRenderDrawBlendMode(mRenderer, SDL_BLENDMODE_BLEND);
    
    /*
    s32 wW, wH;
    SDL_GetWindowSize(mWindow, &wW, &wH);

    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;
    rect.w = wW;
    rect.h = wH;
    SDL_RenderSetViewport(mRenderer, &rect);
*/
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

void SoftwareRenderer::EndFrame()
{
    SDL_RenderPresent(mRenderer);
    mFrameStarted = false;
    
    // this breaks the "Abes Exoddus" intro text, not sure if it breaks anything else yet
    // but stops all of the flicker by emulating how the vram was "sticky"
    //SDL_RenderClear(mRenderer);
}

void SoftwareRenderer::OutputSize(s32* w, s32* h)
{
    SDL_GetRendererOutputSize(mRenderer, w, h);
}

void SoftwareRenderer::SetTPage(u16 /*tPage*/)
{
    // PSX_TPage_Change_4F6430(tPage);
}

void SoftwareRenderer::SetClip(Prim_PrimClipper& clipper)
{
    SDL_Rect rect;
    rect.x = clipper.field_C_x;
    rect.y = clipper.field_E_y*2;
    rect.w = clipper.mBase.header.mRect.w;
    rect.h = clipper.mBase.header.mRect.h*2;

    // HACK: turn it off when the FMV change sets it to be this which makes no sense
    if (rect.x == 0 && rect.y == 0 && rect.w == 1 && rect.h == 2)
    {
        SDL_RenderSetClipRect(mRenderer, nullptr);
    }
    else
    {
        //LOG_INFO("Set clip " << rect.x << ", " << rect.y << " " << rect.w << " " << rect.h);
        SDL_RenderSetClipRect(mRenderer, &rect);
    }
}

void SoftwareRenderer::ToggleFilterScreen()
{
    // TODO: Implement this
}

void SoftwareRenderer::ToggleKeepAspectRatio()
{
    // TODO: Implement this
}

// ExplosionSet, ScreenShake, RollingBallShaker
void SoftwareRenderer::SetScreenOffset(Prim_ScreenOffset& /*offset*/)
{
}

// Blood, ZapLine, HintFly
void SoftwareRenderer::Draw(Prim_Sprt& sprt)
{
    /*
    PrimAny any;
    any.mSprt = &sprt;
    DrawOTag_Render_SPRT(any, static_cast<s16>(mFrame_xOff), static_cast<s16>(mFrame_yOff), any.mSprt->field_14_w, any.mSprt->field_16_h);
    */

    const u8 a = (sprt.mBase.header.rgb_code.code_or_pad & 2) ? 127 : 255;
    if (a == 127)
    {
        SDL_SetRenderDrawBlendMode(mRenderer, SDL_BLENDMODE_BLEND);
    }
    SDL_SetRenderDrawColor(mRenderer, R0(&sprt), G0(&sprt), B0(&sprt), a);

    SDL_Rect rect;
    rect.x = X0(&sprt);
    rect.y = Y0(&sprt) * 2;
    rect.w = sprt.field_14_w;
    rect.h = sprt.field_16_h * 2;
    SDL_RenderFillRect(mRenderer, &rect);
}


static SDL_Texture* MakeGasTexture(SDL_Renderer* pRender, const u16* pPixels, u32 w, u32 h)
{
    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0,
                                                          w,
                                                          h, 32, SDL_PIXELFORMAT_ABGR8888);

    SDL_LockSurface(surface);

    u32 i = 0;
    for (u32 y = 0; y < h; y++)
    {
        for (u32 x = 0; x < w; x++)
        {
            set_pixel(surface, x, y, RGBConversion::RGBA555ToRGBA888(pPixels[i++]));
        }
    }
    SDL_UnlockSurface(surface);

    SDL_Texture* pTexture = SDL_CreateTextureFromSurface(pRender, surface);
    if (!pTexture)
    {
        LOG_ERROR(SDL_GetError());
    }
    SDL_FreeSurface(surface);
    return pTexture;
}

// LaughingGas
void SoftwareRenderer::Draw(Prim_GasEffect& gasEffect)
{
    const s32 gasWidth = (gasEffect.w - gasEffect.x);
    const s32 gasHeight = (gasEffect.h - gasEffect.y);

    // TODO: Wrong using RGB555 instead of 565
    SDL_Texture* pTexture = MakeGasTexture(mRenderer, gasEffect.pData, gasWidth/4, gasHeight/2);

    // TODO: Wrong
    SDL_SetTextureBlendMode(pTexture, SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(pTexture, 40);

    SDL_Rect dst;
    dst.x = gasEffect.x;
    dst.y = gasEffect.y * 2;
    dst.w = gasEffect.w;
    dst.h = gasEffect.h * 2;
    SDL_RenderCopy(mRenderer, pTexture, nullptr, &dst);
    SDL_DestroyTexture(pTexture);


    // PSX_RenderLaughingGasEffect_4F7B80(gasEffect.x, gasEffect.y, gasEffect.w, gasEffect.h, gasEffect.pData);
}

// CircularFade, EffectBase
void SoftwareRenderer::Draw(Prim_Tile& tile)
{
    /*
    PrimAny any;
    any.mTile = &tile;
    DrawOTag_Render_TILE(any, static_cast<s16>(mFrame_xOff) + any.mSprt->mBase.vert.x, static_cast<s16>(mFrame_yOff) + any.mSprt->mBase.vert.y, any.mTile->field_14_w, any.mTile->field_16_h);
    */
    const u8 a = (tile.mBase.header.rgb_code.code_or_pad & 2) ? 127 : 255;
    if (a == 127)
    {
        SDL_SetRenderDrawBlendMode(mRenderer, SDL_BLENDMODE_BLEND);
    }

    SDL_SetRenderDrawColor(mRenderer, R0(&tile), G0(&tile), B0(&tile), a);

    SDL_Rect rect;
    rect.x = X0(&tile);
    rect.y = Y0(&tile)*2;
    rect.w = tile.field_14_w;
    rect.h = tile.field_16_h * 2;
    SDL_RenderFillRect(mRenderer, &rect);
}

// ThrowableTotalIndicator
void SoftwareRenderer::Draw(Line_F2& line)
{
    /*
    PSX_Render_Line_Prim_4F7D90(&line, static_cast<s16>(mFrame_xOff), static_cast<s16>(mFrame_yOff));
    */
    const u8 a = (line.mBase.header.rgb_code.code_or_pad & 2) ? 127 : 255;
    if (a == 127)
    {
        SDL_SetRenderDrawBlendMode(mRenderer, SDL_BLENDMODE_BLEND);
    }

    SDL_SetRenderDrawColor(mRenderer, R0(&line), G0(&line), B0(&line), a);

    // TODO: This probably looks nothing like it should do
    SDL_RenderDrawLine(mRenderer, X0(&line), Y0(&line) * 2, X1(&line), Y1(&line) * 2);
}

// SnoozeParticle, Spark, ThrowableTotalIndicator
void SoftwareRenderer::Draw(Line_G2& /*line*/)
{
    // TODO
    // PSX_Render_Line_Prim_4F7D90(&line, static_cast<s16>(mFrame_xOff), static_cast<s16>(mFrame_yOff));
}

// Only used by SnoozeParticle
void SoftwareRenderer::Draw(Line_G4& /*line*/)
{
    // TODO
    // PSX_Render_Line_Prim_4F7D90(&line, static_cast<s16>(mFrame_xOff), static_cast<s16>(mFrame_yOff));
}

void SoftwareRenderer::Draw(Poly_F3& poly)
{
    SDL_Vertex vert[3];

    const u8 r = R0(&poly);
    const u8 g = G0(&poly);
    const u8 b = B0(&poly);

    // center
    vert[0].position.x = X0(&poly);
    vert[0].position.y = Y0(&poly) * 2.0f;
    vert[0].color.r = r;
    vert[0].color.g = g;
    vert[0].color.b = b;
    vert[0].color.a = 255;

    // left
    vert[1].position.x = X1(&poly);
    vert[1].position.y = Y1(&poly) * 2.0f;
    vert[1].color.r = r;
    vert[1].color.g = g;
    vert[1].color.b = b;
    vert[1].color.a = 255;

    // right
    vert[2].position.x = X2(&poly);
    vert[2].position.y = Y2(&poly) * 2.0f;
    vert[2].color.r = r;
    vert[2].color.g = g;
    vert[2].color.b = b;
    vert[2].color.a = 255;

    SDL_RenderGeometry(mRenderer, nullptr, vert, 3, nullptr, 0);
}

void SoftwareRenderer::Draw(Poly_G3& poly)
{
    SDL_Vertex vert[3];

    // center
    vert[0].position.x = X0(&poly);
    vert[0].position.y = Y0(&poly) * 2.0f;
    vert[0].color.r = R0(&poly);
    vert[0].color.g = G0(&poly);
    vert[0].color.b = B0(&poly);
    vert[0].color.a = 255;

    // left
    vert[1].position.x = X1(&poly);
    vert[1].position.y = Y1(&poly) * 2.0f;
    vert[1].color.r = R1(&poly);
    vert[1].color.g = G1(&poly);
    vert[1].color.b = B1(&poly);
    vert[1].color.a = 255;

    // right
    vert[2].position.x = X2(&poly);
    vert[2].position.y = Y2(&poly) * 2.0f;
    vert[2].color.r = R2(&poly);
    vert[2].color.g = G2(&poly);
    vert[2].color.b = B2(&poly);
    vert[2].color.a = 255;

    SDL_RenderGeometry(mRenderer, nullptr, vert, 3, nullptr, 0);
}

void SoftwareRenderer::Draw(Poly_F4& poly)
{
    SDL_Vertex vert[4];

    // TODO: Why isn't this semi transparent when a= 127 for the pause menu ??
    // need 1x1 white pixel texture
    const u8 a = (poly.mBase.header.rgb_code.code_or_pad & 2) ? 127 : 255;
    if (a == 127)
    {
        SDL_SetRenderDrawBlendMode(mRenderer, SDL_BLENDMODE_BLEND);
    }
    /*
    u32 tPageAbr = ((u32) tPage >> 5) & 3;
    switch (tPageAbr)
    {

    }*/

    // center
    vert[0].position.x = X0(&poly);
    vert[0].position.y = Y0(&poly) * 2.0f;
    vert[0].color.r = R0(&poly);
    vert[0].color.g = G0(&poly);
    vert[0].color.b = B0(&poly);
    vert[0].color.a = a;

    // left
    vert[1].position.x = X1(&poly);
    vert[1].position.y = Y1(&poly) * 2.0f;
    vert[1].color.r = R0(&poly);
    vert[1].color.g = G0(&poly);
    vert[1].color.b = B0(&poly);
    vert[1].color.a = a;

    // right
    vert[2].position.x = X2(&poly);
    vert[2].position.y = Y2(&poly) * 2.0f;
    vert[2].color.r = R0(&poly);
    vert[2].color.g = G0(&poly);
    vert[2].color.b = B0(&poly);
    vert[2].color.a = a;

    vert[3].position.x = X3(&poly);
    vert[3].position.y = Y3(&poly) * 2.0f;
    vert[3].color.r = R0(&poly);
    vert[3].color.g = G0(&poly);
    vert[3].color.b = B0(&poly);
    vert[3].color.a = a;

    s32 indexList[6] = {0, 1, 2, 2, 1, 3};
    SDL_RenderGeometry(mRenderer, nullptr, vert, 4, indexList, 6);
}

static SDL_Texture* MakeTexture(SDL_Renderer* pRender, const AnimationPal& pPal, const u8* pPixels, u32 w, u32 h)
{
    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0,
                                                          w,
                                                          h, 32, SDL_PIXELFORMAT_ABGR8888);

    SDL_LockSurface(surface);


    u32 i = 0;
    for (u32 y = 0; y < h; y++)
    {
        for (u32 x = 0; x < w; x++)
        {
            set_pixel(surface, x, y, pPal.mPal[pPixels[i++]].ToU32());
        }
    }
    SDL_UnlockSurface(surface);

    SDL_Texture* pTexture = SDL_CreateTextureFromSurface(pRender, surface);
    if (!pTexture)
    {
        LOG_ERROR(SDL_GetError());
    }
    SDL_FreeSurface(surface);
    return pTexture;
}

static void SetSemiTransBlendMode(SDL_Texture* pTexture, s16 tPage)
{
    u32 tPageAbr = ((u32) tPage >> 5) & 3;
    switch (tPageAbr)
    {
        case 0: // 0.5xB + 0.5xF
        {
            SDL_SetTextureBlendMode(pTexture, SDL_BLENDMODE_BLEND);
            break;
        }

        case 1: // 1.0xB + 1.0xF
            SDL_SetTextureBlendMode(pTexture, SDL_BLENDMODE_ADD);
            break;

        case 2: // 1.0xB - 1.0xF
            SDL_SetTextureBlendMode(pTexture, SDL_BLENDMODE_MOD);
            break;

        case 3: // 1.0xB + 0.25xF
            LOG_WARNING("Blend mode 3");
            break;
    }
}

void SoftwareRenderer::Draw(Poly_FT4& poly)
{
    SDL_Texture* pTexture = nullptr;
    f32 u0 = 0.0f;
    f32 v0 = 0.0f;
    f32 u1 = 1.0f;
    f32 v1 = 1.0f;

    if (poly.mAnim)
    {
        std::shared_ptr<TgaData> pTga = poly.mAnim->mAnimRes.mTgaPtr;
        pTexture = MakeTexture(mRenderer, *pTga->mPal, pTga->mPixels.data(), pTga->mWidth, pTga->mHeight);

        if (poly.mBase.header.rgb_code.code_or_pad & 2)
        {
            const s16 tPage = GetTPage(&poly);
            SetSemiTransBlendMode(pTexture, tPage);
        }

        if (poly.mBase.header.rgb_code.code_or_pad & 1)
        {
            const s16 tPage = GetTPage(&poly);
            // TODO: Wrong
            SetSemiTransBlendMode(pTexture, tPage);
        }


        const PerFrameInfo* pHeader = poly.mAnim->Get_FrameHeader(-1);


        u0 = static_cast<f32>(pHeader->mSpriteSheetX) / static_cast<f32>(pTga->mWidth);
        v0 = static_cast<f32>(pHeader->mSpriteSheetY) / static_cast<f32>(pTga->mHeight);

        u1 = u0 + static_cast<f32>(pHeader->mWidth) / static_cast<f32>(pTga->mWidth);
        v1 = v0 + static_cast<f32>(pHeader->mHeight) / static_cast<f32>(pTga->mHeight);

        if (poly.mFlipX)
        {
            std::swap(u0, u1);
        }

        if (poly.mFlipY)
        {
            std::swap(v1, v0);
        }

        /*
        SDL_Rect src = {};
        src.x = pHeader->mSpriteSheetX;
        src.y = pHeader->mSpriteSheetY;
        src.w = pHeader->mWidth;
        src.h = pHeader->mHeight;

        SDL_SetTextureColorMod(pTexture, 127, 127, 127);

        SDL_Rect dst;
        dst.x = X0(&poly);
        dst.y = Y0(&poly) * 2;
        dst.w = X3(&poly) - X0(&poly);
        dst.h = (Y3(&poly) * 2) - (Y0(&poly) * 2);
        SDL_RenderCopy(mRenderer, pTexture, &src, &dst);
        */
    }
    else if (poly.mFg1)
    {
        pTexture = SDL_CreateTexture(mRenderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, poly.mFg1->mImage.mWidth, poly.mFg1->mImage.mHeight);
        void* pixels = nullptr;
        s32 pitch = 0;
        SDL_LockTexture(pTexture, nullptr, &pixels, &pitch);

        const u32* pFg1Src = reinterpret_cast<const u32*>(poly.mFg1->mImage.mPixels->data());
        const u32* pCamSrc = reinterpret_cast<const u32*>(poly.mCam->mData.mPixels->data());
        
        for (u32 y = 0; y < poly.mFg1->mImage.mHeight; y++)
        {
            for (u32 x = 0; x < poly.mFg1->mImage.mWidth; x++)
            {
                Uint8* target_pixel = (Uint8*) pixels + y * pitch + x * sizeof(u32);
                if (*pFg1Src != 0xFF000000)
                {
                    *(u32*) target_pixel = (*pCamSrc) | 0xFF000000;
                }
                else
                {
                    *(u32*) target_pixel = 0;
                }
                pFg1Src++;
                pCamSrc++;
            }
        }

        SDL_UnlockTexture(pTexture);
        SDL_SetTextureBlendMode(pTexture, SDL_BLENDMODE_BLEND);
    }
    else if (poly.mCam && poly.mCam->mData.mPixels)
    {
        pTexture = SDL_CreateTexture(mRenderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, poly.mCam->mData.mWidth, poly.mCam->mData.mHeight);

        void* pixels = nullptr;
        s32 pitch = 0;
        SDL_LockTexture(pTexture, nullptr, &pixels, &pitch);

        const u32* pSrc = reinterpret_cast<const u32*>(poly.mCam->mData.mPixels->data());
        for (u32 y = 0; y < poly.mCam->mData.mHeight; y++)
        {
            for (u32 x = 0; x < poly.mCam->mData.mWidth; x++)
            {
                Uint8* target_pixel = (Uint8*) pixels + y * pitch + x * sizeof(u32);
                *(u32*) target_pixel = *pSrc;
                pSrc++;
            }
        }

        SDL_UnlockTexture(pTexture);

        /*
        SDL_SetTextureColorMod(pTexture, 255, 255, 255);
    
        SDL_Rect dst;
        dst.x = X0(&poly);
        dst.y = Y0(&poly) * 2;
        dst.w = X3(&poly);
        dst.h = Y3(&poly) * 2;
        SDL_RenderCopy(mRenderer, pTexture, nullptr, &dst);
        */
    }
    else if (poly.mFont)
    {
        std::shared_ptr<TgaData> pTga = poly.mFont->field_C_resource_id.mTgaPtr;
        pTexture = MakeTexture(mRenderer, *pTga->mPal, pTga->mPixels.data(), pTga->mWidth, pTga->mHeight);

        u0 = U0(&poly);
        v0 = V0(&poly);

        u1 = U3(&poly);
        v1 = V3(&poly);

        u0 = u0 / static_cast<f32>(pTga->mWidth);
        v0 = v0 / static_cast<f32>(pTga->mHeight);

        u1 = u1 / static_cast<f32>(pTga->mWidth);
        v1 = v1 / static_cast<f32>(pTga->mHeight);
    }


    SDL_Vertex vert[4];

    u16 polyR = R0(&poly);
    u16 polyG = G0(&poly);
    u16 polyB = B0(&poly);

    u8 r = static_cast<u8>(std::min(255, polyR * 2));
    u8 g = static_cast<u8>(std::min(255, polyG * 2));
    u8 b = static_cast<u8>(std::min(255, polyB * 2));

    // center
    vert[0].position.x = X0(&poly);
    vert[0].position.y = Y0(&poly) * 2.0f;
    vert[0].color.r = r;
    vert[0].color.g = g;
    vert[0].color.b = b;
    vert[0].color.a = 255;
    vert[0].tex_coord.x = u0;
    vert[0].tex_coord.y = v0;

    // left
    vert[1].position.x = X1(&poly);
    vert[1].position.y = Y1(&poly) * 2.0f;
    vert[1].color.r = r;
    vert[1].color.g = g;
    vert[1].color.b = b;
    vert[1].color.a = 255;
    vert[1].tex_coord.x = u1;
    vert[1].tex_coord.y = v0;

    // right
    vert[2].position.x = X2(&poly);
    vert[2].position.y = Y2(&poly) * 2.0f;
    vert[2].color.r = r;
    vert[2].color.g = g;
    vert[2].color.b = b;
    vert[2].color.a = 255;
    vert[2].tex_coord.x = u0;
    vert[2].tex_coord.y = v1;

    vert[3].position.x = X3(&poly);
    vert[3].position.y = Y3(&poly) * 2.0f;
    vert[3].color.r = r;
    vert[3].color.g = g;
    vert[3].color.b = b;
    vert[3].color.a = 255;
    vert[3].tex_coord.x = u1;
    vert[3].tex_coord.y = v1;

    s32 indexList[6] = {0, 1, 2, 2, 1, 3};
    SDL_RenderGeometry(mRenderer, pTexture, vert, 4, indexList, 6);

    if (pTexture)
    {
        SDL_DestroyTexture(pTexture);
    }
}

void SoftwareRenderer::Draw(Poly_G4& poly)
{
    SDL_Vertex vert[4];

    // center
    vert[0].position.x = X0(&poly);
    vert[0].position.y = Y0(&poly) * 2.0f;
    vert[0].color.r = R0(&poly);
    vert[0].color.g = G0(&poly);
    vert[0].color.b = B0(&poly);
    vert[0].color.a = 255;

    vert[1].position.x = X1(&poly);
    vert[1].position.y = Y1(&poly) * 2.0f;
    vert[1].color.r = R1(&poly);
    vert[1].color.g = G1(&poly);
    vert[1].color.b = B1(&poly);
    vert[1].color.a = 255;

    vert[2].position.x = X2(&poly);
    vert[2].position.y = Y2(&poly) * 2.0f;
    vert[2].color.r = R2(&poly);
    vert[2].color.g = G2(&poly);
    vert[2].color.b = B2(&poly);
    vert[2].color.a = 255;

    vert[3].position.x = X3(&poly);
    vert[3].position.y = Y3(&poly) * 2.0f;
    vert[3].color.r = R3(&poly);
    vert[3].color.g = G3(&poly);
    vert[3].color.b = B3(&poly);
    vert[3].color.a = 255;

    s32 indexList[6] = {0, 1, 2, 2, 1, 3};
    SDL_RenderGeometry(mRenderer, nullptr, vert, 4, indexList, 6);
}

#endif
