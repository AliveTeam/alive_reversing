#include "stdafx.h"
#include "CircularFade.hpp"
#include "Function.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "Game.hpp"
#include "stdlib.hpp"

CircularFade::CircularFade(FP xpos, FP ypos, FP scale, s16 direction, s8 destroyOnDone)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    mBaseGameObjectFlags.Set(BaseGameObject::eUpdateDuringCamSwap_Bit10);

    if (direction)
    {
        field_1B8_fade_colour = 0;
    }
    else
    {
        field_1B8_fade_colour = 255;
    }

    VFadeIn(direction, destroyOnDone);

    const u8 fade_rgb = static_cast<u8>((field_1B8_fade_colour * 60) / 100);
    mRGB.SetRGB(fade_rgb, fade_rgb, fade_rgb);

    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::SpotLight));
    Animation_Init(GetAnimRes(AnimId::SpotLight));

    mVisualFlags.Clear(VisualFlags::eApplyShadowZoneColour);

    GetAnimation().mFlags.Clear(AnimFlags::eBlending);
    SetSpriteScale(scale * FP_FromInteger(2));
    GetAnimation().SetSpriteScale(scale * FP_FromInteger(2));

    mXPos = xpos;
    mYPos = ypos;
    GetAnimation().SetRenderMode(TPageAbr::eBlend_2);
    GetAnimation().SetRenderLayer(Layer::eLayer_FadeFlash_40);
    mRGB.SetRGB(field_1B8_fade_colour, field_1B8_fade_colour, field_1B8_fade_colour);

    Init_SetTPage(&field_198_tPages[0], 0, 0, PSX_getTPage(TPageAbr::eBlend_2));
    Init_SetTPage(&field_198_tPages[1], 0, 0, PSX_getTPage(TPageAbr::eBlend_2));
}

CircularFade::~CircularFade()
{
    if (!(field_F4_flags.Get(Flags::eBit2_Done)))
    {
        --gNumCamSwappers;
    }
}

void CircularFade::VRender(PrimHeader** ppOt)
{
    const u8 fade_rgb = static_cast<u8>((field_1B8_fade_colour * 60) / 100);

    mRGB.SetRGB(fade_rgb, fade_rgb, fade_rgb);

    GetAnimation().SetRGB(fade_rgb, fade_rgb, fade_rgb);

    GetAnimation().VRender(
        FP_GetExponent(FP_FromInteger(mXOffset) + mXPos - pScreenManager->CamXPos()),
        FP_GetExponent(FP_FromInteger(mYOffset) + mYPos - pScreenManager->CamYPos()),
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

    const u8 fadeColour = static_cast<u8>(field_1B8_fade_colour);

    Poly_G4* pTile1 = &field_F8_tile1[gPsxDisplay.mBufferIndex];
    PolyG4_Init(pTile1);
    SetRGB0(pTile1, fadeColour, fadeColour, fadeColour);
    SetRGB1(pTile1, fadeColour, fadeColour, fadeColour);
    SetRGB2(pTile1, fadeColour, fadeColour, fadeColour);
    SetRGB3(pTile1, fadeColour, fadeColour, fadeColour);
    SetXYWH(pTile1, 0, 0, gPsxDisplay.mWidth, frameRect.y);
    Poly_Set_SemiTrans(&pTile1->mBase.header, 1);
    OrderingTable_Add(OtLayer(ppOt, GetAnimation().GetRenderLayer()), &pTile1->mBase.header);

    Poly_G4* pTile2 = &field_120_tile2[gPsxDisplay.mBufferIndex];
    PolyG4_Init(pTile2);
    SetRGB0(pTile2, fadeColour, fadeColour, fadeColour);
    SetRGB1(pTile2, fadeColour, fadeColour, fadeColour);
    SetRGB2(pTile2, fadeColour, fadeColour, fadeColour);
    SetRGB3(pTile2, fadeColour, fadeColour, fadeColour);
    SetXYWH(pTile2, 0, frameRect.y, GetAnimation().mFlags.Get(AnimFlags::eFlipX) ? frameRect.x + 1 : frameRect.x, frameRect.h - frameRect.y);

    Poly_Set_SemiTrans(&pTile2->mBase.header, 1);
    OrderingTable_Add(OtLayer(ppOt, GetAnimation().GetRenderLayer()), &pTile2->mBase.header);

    Poly_G4* pTile3 = &field_148_tile3[gPsxDisplay.mBufferIndex];
    PolyG4_Init(pTile3);
    SetRGB0(pTile3, fadeColour, fadeColour, fadeColour);
    SetRGB1(pTile3, fadeColour, fadeColour, fadeColour);
    SetRGB2(pTile3, fadeColour, fadeColour, fadeColour);
    SetRGB3(pTile3, fadeColour, fadeColour, fadeColour);
    SetXYWH(pTile3, frameRect.w, frameRect.y, gPsxDisplay.mWidth - frameRect.w, frameRect.h - frameRect.y);

    Poly_Set_SemiTrans(&pTile3->mBase.header, 1);
    OrderingTable_Add(OtLayer(ppOt, GetAnimation().GetRenderLayer()), &pTile3->mBase.header);

    Poly_G4* pTile4 = &field_170_tile4[gPsxDisplay.mBufferIndex];
    PolyG4_Init(pTile4);
    SetRGB0(pTile4, fadeColour, fadeColour, fadeColour);
    SetRGB1(pTile4, fadeColour, fadeColour, fadeColour);
    SetRGB2(pTile4, fadeColour, fadeColour, fadeColour);
    SetRGB3(pTile4, fadeColour, fadeColour, fadeColour);
    SetXYWH(pTile4, 0, frameRect.h, gPsxDisplay.mWidth, gPsxDisplay.mHeight - frameRect.h);

    Poly_Set_SemiTrans(&pTile4->mBase.header, 1);
    OrderingTable_Add(OtLayer(ppOt, GetAnimation().GetRenderLayer()), &pTile4->mBase.header);

    OrderingTable_Add(OtLayer(ppOt, GetAnimation().GetRenderLayer()), &field_198_tPages[gPsxDisplay.mBufferIndex].mBase);

    if ((field_1B8_fade_colour == 255 && field_F4_flags.Get(Flags::eBit1_FadeIn)) || (field_1B8_fade_colour == 0 && !(field_F4_flags.Get(Flags::eBit1_FadeIn))))
    {
        if (!(field_F4_flags.Get(Flags::eBit2_Done)))
        {
            field_F4_flags.Set(Flags::eBit2_Done);
            --gNumCamSwappers;
        }

        if (field_F4_flags.Get(Flags::eBit3_DestroyOnDone))
        {
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        }
    }
}

void CircularFade::VUpdate()
{
    if ((!field_F4_flags.Get(Flags::eBit4_NeverSet) && !field_F4_flags.Get(Flags::eBit2_Done)))
    {
        field_1B8_fade_colour += field_1BA_speed;
        if (field_F4_flags.Get(Flags::eBit1_FadeIn))
        {
            if (field_1B8_fade_colour > 255)
            {
                field_1B8_fade_colour = 255;
            }
        }
        else if (field_1B8_fade_colour < 0)
        {
            field_1B8_fade_colour = 0;
        }
    }
}

s32 CircularFade::VFadeIn(s16 direction, s8 destroyOnDone) // TODO: Likely no return
{
    gNumCamSwappers++;

    field_F4_flags.Set(Flags::eBit1_FadeIn, direction);

    field_F4_flags.Clear(Flags::eBit2_Done);
    field_F4_flags.Clear(Flags::eBit4_NeverSet);

    field_F4_flags.Set(Flags::eBit3_DestroyOnDone, destroyOnDone);

    if (field_F4_flags.Get(Flags::eBit1_FadeIn))
    {
        field_1BA_speed = 12;
    }
    else
    {
        field_1BA_speed = -12;
    }
    return field_F4_flags.Raw().all;
}

void CircularFade::VScreenChanged()
{
    // Empty
}

s32 CircularFade::VDone()
{
    return field_F4_flags.Get(Flags::eBit2_Done);
}

CircularFade* Make_Circular_Fade_4CE8C0(FP xpos, FP ypos, FP scale, s16 direction, s8 destroyOnDone, s8 surviveDeathReset)
{
    auto pCircularFade = relive_new CircularFade(xpos, ypos, scale, direction, destroyOnDone);
    if (!pCircularFade)
    {
        return nullptr;
    }

    pCircularFade->mBaseGameObjectFlags.Set(BaseGameObject::eSurviveDeathReset_Bit9, surviveDeathReset);
    return pCircularFade;
}
