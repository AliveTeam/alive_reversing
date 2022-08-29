#include "stdafx_ao.h"
#include "CircularFade.hpp"
#include "Function.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include "ResourceManager.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "../relive_lib/Primitives.hpp"

namespace AO {

CircularFade::CircularFade(FP xpos, FP ypos, FP scale, s16 direction, s8 destroyOnDone)
{
    if (direction)
    {
        field_1A8_fade_colour = 0;
    }
    else
    {
        field_1A8_fade_colour = 255;
    }

    // NOTE: Inlined
    VFadeIn(static_cast<s8>(direction), destroyOnDone);

    const u8 fade_rgb = static_cast<u8>((field_1A8_fade_colour * 60) / 100);
    mRGB.SetRGB(fade_rgb, fade_rgb, fade_rgb);

    const AnimRecord rec = AO::AnimRec(AnimId::Circular_Fade);
    u8** ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init(AnimId::Circular_Fade, ppRes);

    mVisualFlags.Clear(VisualFlags::eApplyShadowZoneColour);

    mAnim.mFlags.Clear(AnimFlags::eBit16_bBlending);
    mSpriteScale.fpValue = scale.fpValue * 2;
    mAnim.field_14_scale.fpValue = scale.fpValue * 2;

    mXPos = xpos;
    mYPos = ypos;
    mAnim.mRenderMode = TPageAbr::eBlend_2;
    mAnim.mRenderLayer = Layer::eLayer_FadeFlash_40;
    mRGB.SetRGB(field_1A8_fade_colour, field_1A8_fade_colour, field_1A8_fade_colour);

    Init_SetTPage(&field_188_tPage[0], 0, 0, PSX_getTPage(TPageMode::e16Bit_2, TPageAbr::eBlend_2, 0, 0));
    Init_SetTPage(&field_188_tPage[1], 0, 0, PSX_getTPage(TPageMode::e16Bit_2, TPageAbr::eBlend_2, 0, 0));
}

void CircularFade::VRender(PrimHeader** ppOt)
{
    const u8 fade_rgb = static_cast<u8>((field_1A8_fade_colour * 60) / 100);

    mRGB.SetRGB(fade_rgb, fade_rgb, fade_rgb);

    mAnim.mRed = fade_rgb;
    mAnim.mGreen = fade_rgb;
    mAnim.mBlue = fade_rgb;

    mAnim.VRender(
        FP_GetExponent(mXPos + (FP_FromInteger(pScreenManager->mCamXOff + mXOffset)) - pScreenManager->mCamPos->x),
        FP_GetExponent(mYPos + (FP_FromInteger(pScreenManager->mCamYOff + mYOffset)) - pScreenManager->mCamPos->y),
        ppOt,
        0,
        0);
    PSX_RECT frameRect = {};
    mAnim.Get_Frame_Rect(&frameRect);
    pScreenManager->InvalidateRectCurrentIdx(
        frameRect.x,
        frameRect.y,
        frameRect.w,
        frameRect.h);

    frameRect.h--;
    frameRect.w--;

    if (frameRect.y < 0)
    {
        frameRect.y = 0;
    }

    if (frameRect.x < 0)
    {
        frameRect.x = 0;
    }

    if (frameRect.w >= 640)
    {
        frameRect.w = 639;
    }

    if (frameRect.h >= 240)
    {
        frameRect.h = 240;
    }

    const u8 fadeColour = static_cast<u8>(field_1A8_fade_colour);


    Prim_Tile* pTile = &field_E8[gPsxDisplay.mBufferIndex];
    Init_Tile(pTile);
    SetRGB0(pTile, fadeColour, fadeColour, fadeColour);
    SetXY0(pTile, 0, 0);
    pTile->field_14_w = gPsxDisplay.mWidth;
    pTile->field_16_h = frameRect.y;
    Poly_Set_SemiTrans(&pTile->mBase.header, 1);
    OrderingTable_Add(OtLayer(ppOt, mAnim.mRenderLayer), &pTile->mBase.header);

    Prim_Tile* pTile2_1 = &field_110[gPsxDisplay.mBufferIndex];
    Init_Tile(pTile2_1);
    SetRGB0(pTile2_1, fadeColour, fadeColour, fadeColour);

    s16 w = 0;
    if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
    {
        w = frameRect.x + 1;
    }
    else
    {
        w = frameRect.x;
    }
    SetXY0(pTile2_1, 0, frameRect.y);
    pTile2_1->field_14_w = w;
    pTile2_1->field_16_h = frameRect.h - frameRect.y;
    Poly_Set_SemiTrans(&pTile2_1->mBase.header, 1);
    OrderingTable_Add(OtLayer(ppOt, mAnim.mRenderLayer), &pTile2_1->mBase.header);

    Prim_Tile* pTile2 = &field_138[gPsxDisplay.mBufferIndex];
    Init_Tile(pTile2);
    SetRGB0(pTile2, fadeColour, fadeColour, fadeColour);
    SetXY0(pTile2, frameRect.w + 1, frameRect.y);
    pTile2->field_14_w = gPsxDisplay.mWidth - frameRect.w;
    pTile2->field_16_h = frameRect.h - frameRect.y;
    Poly_Set_SemiTrans(&pTile2->mBase.header, 1);
    OrderingTable_Add(OtLayer(ppOt, mAnim.mRenderLayer), &pTile2->mBase.header);

    Prim_Tile* pTile3 = &field_160[gPsxDisplay.mBufferIndex];
    Init_Tile(pTile3);
    SetRGB0(pTile3, fadeColour, fadeColour, fadeColour);
    SetXY0(pTile3, 0, frameRect.h);
    pTile3->field_14_w = gPsxDisplay.mWidth;
    pTile3->field_16_h = gPsxDisplay.mHeight - frameRect.h;
    Poly_Set_SemiTrans(&pTile3->mBase.header, 1);
    OrderingTable_Add(OtLayer(ppOt, mAnim.mRenderLayer), &pTile3->mBase.header);
    OrderingTable_Add(OtLayer(ppOt, mAnim.mRenderLayer), &field_188_tPage[gPsxDisplay.mBufferIndex].mBase);

    if (field_1A8_fade_colour < 255)
    {
        pScreenManager->InvalidateRectCurrentIdx(
            0,
            0,
            gPsxDisplay.mWidth,
            gPsxDisplay.mHeight);
    }

    if ((field_1A8_fade_colour == 255 && field_E4_flags.Get(CircularFade::eBit1_FadeIn)) || (field_1A8_fade_colour == 0 && !field_E4_flags.Get(CircularFade::eBit1_FadeIn)))
    {
        field_E4_flags.Set(CircularFade::eBit2_Done);

        if (field_E4_flags.Get(CircularFade::eBit3_DestroyOnDone))
        {
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        }
    }
}

void CircularFade::VUpdate()
{
    if ((!field_E4_flags.Get(Flags::eBit4_NeverSet) && !field_E4_flags.Get(Flags::eBit2_Done)))
    {
        field_1A8_fade_colour += field_1AA_speed;
        if (field_E4_flags.Get(Flags::eBit1_FadeIn))
        {
            if (field_1A8_fade_colour > 255)
            {
                field_1A8_fade_colour = 255;
            }
        }
        else if (field_1A8_fade_colour < 0)
        {
            field_1A8_fade_colour = 0;
        }
    }
}

s8 CircularFade::VFadeIn(u8 direction, s8 destroyOnDone)
{
    field_E4_flags.Set(Flags::eBit1_FadeIn, direction);

    field_E4_flags.Clear(Flags::eBit2_Done);
    field_E4_flags.Clear(Flags::eBit4_NeverSet);

    field_E4_flags.Set(Flags::eBit3_DestroyOnDone, destroyOnDone);


    if (field_E4_flags.Get(Flags::eBit1_FadeIn))
    {
        field_1AA_speed = 12;
    }
    else
    {
        field_1AA_speed = -12;
    }
    return static_cast<s8>(field_E4_flags.Raw().all);
}

void CircularFade::VScreenChanged()
{
    // Empty
}

s32 CircularFade::VDone()
{
    return field_E4_flags.Get(Flags::eBit2_Done);
}

CircularFade* Make_Circular_Fade(FP xpos, FP ypos, FP scale, s16 direction, s8 destroyOnDone)
{
    return relive_new CircularFade(xpos, ypos, scale, direction, destroyOnDone);
}

} // namespace AO
