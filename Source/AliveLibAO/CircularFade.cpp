#include "stdafx_ao.h"
#include "CircularFade.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "../relive_lib/Primitives.hpp"

namespace AO {

CircularFade::CircularFade(FP xpos, FP ypos, FP scale, s16 direction, s8 destroyOnDone)
    : BaseAnimatedWithPhysicsGameObject(0)
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

    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Circular_Fade));
    Animation_Init(GetAnimRes(AnimId::Circular_Fade));

    SetApplyShadowZoneColour(false);

    GetAnimation().SetBlending(false);
    SetSpriteScale(scale * FP_FromInteger(2));
    GetAnimation().SetSpriteScale(scale * FP_FromInteger(2));

    mXPos = xpos;
    mYPos = ypos;
    GetAnimation().SetRenderMode(TPageAbr::eBlend_2);
    GetAnimation().SetRenderLayer(Layer::eLayer_FadeFlash_40);
    mRGB.SetRGB(field_1A8_fade_colour, field_1A8_fade_colour, field_1A8_fade_colour);

    Init_SetTPage(&field_188_tPage[0], 0, 0, PSX_getTPage(TPageAbr::eBlend_2));
    Init_SetTPage(&field_188_tPage[1], 0, 0, PSX_getTPage(TPageAbr::eBlend_2));
}

void CircularFade::VRender(PrimHeader** ppOt)
{
    const u8 fade_rgb = static_cast<u8>((field_1A8_fade_colour * 60) / 100);

    mRGB.SetRGB(fade_rgb, fade_rgb, fade_rgb);

    GetAnimation().SetRGB(fade_rgb, fade_rgb, fade_rgb);

    GetAnimation().VRender(
        FP_GetExponent(mXPos + (FP_FromInteger(pScreenManager->mCamXOff + mXOffset)) - pScreenManager->mCamPos->x),
        FP_GetExponent(mYPos + (FP_FromInteger(pScreenManager->mCamYOff + mYOffset)) - pScreenManager->mCamPos->y),
        ppOt,
        0,
        0);

    PSX_RECT frameRect = {};
    GetAnimation().Get_Frame_Rect(&frameRect);

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

    Poly_G4* pTile = &field_E8[gPsxDisplay.mBufferIndex];
    PolyG4_Init(pTile);
    SetRGB0(pTile, fadeColour, fadeColour, fadeColour);
    SetRGB1(pTile, fadeColour, fadeColour, fadeColour);
    SetRGB2(pTile, fadeColour, fadeColour, fadeColour);
    SetRGB3(pTile, fadeColour, fadeColour, fadeColour);
    SetXYWH(pTile, 0, 0, gPsxDisplay.mWidth, frameRect.y);

    Poly_Set_SemiTrans(&pTile->mBase.header, 1);
    OrderingTable_Add(OtLayer(ppOt, GetAnimation().GetRenderLayer()), &pTile->mBase.header);

    Poly_G4* pTile2_1 = &field_110[gPsxDisplay.mBufferIndex];
    PolyG4_Init(pTile2_1);
    SetRGB0(pTile2_1, fadeColour, fadeColour, fadeColour);
    SetRGB1(pTile2_1, fadeColour, fadeColour, fadeColour);
    SetRGB2(pTile2_1, fadeColour, fadeColour, fadeColour);
    SetRGB3(pTile2_1, fadeColour, fadeColour, fadeColour);
    SetXYWH(pTile2_1, 0, frameRect.y, GetAnimation().GetFlipX() ? frameRect.x + 1 : frameRect.x, frameRect.h - frameRect.y);

    Poly_Set_SemiTrans(&pTile2_1->mBase.header, 1);
    OrderingTable_Add(OtLayer(ppOt, GetAnimation().GetRenderLayer()), &pTile2_1->mBase.header);

    Poly_G4* pTile2 = &field_138[gPsxDisplay.mBufferIndex];
    PolyG4_Init(pTile2);
    SetRGB0(pTile2, fadeColour, fadeColour, fadeColour);
    SetRGB1(pTile2, fadeColour, fadeColour, fadeColour);
    SetRGB2(pTile2, fadeColour, fadeColour, fadeColour);
    SetRGB3(pTile2, fadeColour, fadeColour, fadeColour);
    SetXYWH(pTile2, frameRect.w + 1, frameRect.y, gPsxDisplay.mWidth - frameRect.w, frameRect.h - frameRect.y);

    Poly_Set_SemiTrans(&pTile2->mBase.header, 1);
    OrderingTable_Add(OtLayer(ppOt, GetAnimation().GetRenderLayer()), &pTile2->mBase.header);

    Poly_G4* pTile3 = &field_160[gPsxDisplay.mBufferIndex];
    PolyG4_Init(pTile3);
    SetRGB0(pTile3, fadeColour, fadeColour, fadeColour);
    SetRGB1(pTile3, fadeColour, fadeColour, fadeColour);
    SetRGB2(pTile3, fadeColour, fadeColour, fadeColour);
    SetRGB3(pTile3, fadeColour, fadeColour, fadeColour);
    SetXYWH(pTile3, 0, frameRect.h, gPsxDisplay.mWidth, gPsxDisplay.mHeight - frameRect.h);

    Poly_Set_SemiTrans(&pTile3->mBase.header, 1);
    OrderingTable_Add(OtLayer(ppOt, GetAnimation().GetRenderLayer()), &pTile3->mBase.header);
    OrderingTable_Add(OtLayer(ppOt, GetAnimation().GetRenderLayer()), &field_188_tPage[gPsxDisplay.mBufferIndex].mBase);

    if ((field_1A8_fade_colour == 255 && mFadeIn) || (field_1A8_fade_colour == 0 && !mFadeIn))
    {
        mDone = true;

        if (mDestroyOnDone)
        {
            SetDead(true);
        }
    }
}

void CircularFade::VUpdate()
{
    if (!mDone)
    {
        field_1A8_fade_colour += field_1AA_speed;
        if (mFadeIn)
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

void CircularFade::VFadeIn(u8 direction, s8 destroyOnDone)
{
    mFadeIn = direction;

    mDone = false;

    mDestroyOnDone = destroyOnDone;

    if (mFadeIn)
    {
        field_1AA_speed = 12;
    }
    else
    {
        field_1AA_speed = -12;
    }
}

void CircularFade::VScreenChanged()
{
    // Empty
}

s32 CircularFade::VDone()
{
    return mDone;
}

} // namespace AO
