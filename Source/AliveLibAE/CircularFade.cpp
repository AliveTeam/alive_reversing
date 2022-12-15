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
    SetUpdateDuringCamSwap(true);

    if (direction)
    {
        mFadeColour = 0;
    }
    else
    {
        mFadeColour = 255;
    }

    VFadeIn(direction, destroyOnDone);

    const u8 fade_rgb = static_cast<u8>((mFadeColour * 60) / 100);
    mRGB.SetRGB(fade_rgb, fade_rgb, fade_rgb);

    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::SpotLight));
    Animation_Init(GetAnimRes(AnimId::SpotLight));

    SetApplyShadowZoneColour(false);

    GetAnimation().SetBlending(false);
    SetSpriteScale(scale * FP_FromInteger(2));
    GetAnimation().SetSpriteScale(scale * FP_FromInteger(2));

    mXPos = xpos;
    mYPos = ypos;
    GetAnimation().SetRenderMode(TPageAbr::eBlend_2);
    GetAnimation().SetRenderLayer(Layer::eLayer_FadeFlash_40);
    mRGB.SetRGB(mFadeColour, mFadeColour, mFadeColour);

    Init_SetTPage(&mTPages[0], PSX_getTPage(TPageAbr::eBlend_2));
    Init_SetTPage(&mTPages[1], PSX_getTPage(TPageAbr::eBlend_2));
}

CircularFade::~CircularFade()
{
    if (!mDone)
    {
        --gNumCamSwappers;
    }
}

void CircularFade::VRender(PrimHeader** ppOt)
{
    const u8 fade_rgb = static_cast<u8>((mFadeColour * 60) / 100);

    mRGB.SetRGB(fade_rgb, fade_rgb, fade_rgb);

    GetAnimation().SetRGB(fade_rgb, fade_rgb, fade_rgb);

    GetAnimation().VRender(
        FP_GetExponent(FP_FromInteger(mXOffset) + mXPos - gScreenManager->CamXPos()),
        FP_GetExponent(FP_FromInteger(mYOffset) + mYPos - gScreenManager->CamYPos()),
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

    const u8 fadeColour = static_cast<u8>(mFadeColour);

    Poly_G4* pTile1 = &mTile1[gPsxDisplay.mBufferIndex];
    PolyG4_Init(pTile1);
    SetRGB0(pTile1, fadeColour, fadeColour, fadeColour);
    SetRGB1(pTile1, fadeColour, fadeColour, fadeColour);
    SetRGB2(pTile1, fadeColour, fadeColour, fadeColour);
    SetRGB3(pTile1, fadeColour, fadeColour, fadeColour);
    SetXYWH(pTile1, 0, 0, gPsxDisplay.mWidth, frameRect.y);
    Poly_Set_SemiTrans(&pTile1->mBase.header, 1);
    OrderingTable_Add(OtLayer(ppOt, GetAnimation().GetRenderLayer()), &pTile1->mBase.header);

    Poly_G4* pTile2 = &mTile2[gPsxDisplay.mBufferIndex];
    PolyG4_Init(pTile2);
    SetRGB0(pTile2, fadeColour, fadeColour, fadeColour);
    SetRGB1(pTile2, fadeColour, fadeColour, fadeColour);
    SetRGB2(pTile2, fadeColour, fadeColour, fadeColour);
    SetRGB3(pTile2, fadeColour, fadeColour, fadeColour);
    SetXYWH(pTile2, 0, frameRect.y, GetAnimation().GetFlipX() ? frameRect.x + 1 : frameRect.x, frameRect.h - frameRect.y);

    Poly_Set_SemiTrans(&pTile2->mBase.header, 1);
    OrderingTable_Add(OtLayer(ppOt, GetAnimation().GetRenderLayer()), &pTile2->mBase.header);

    Poly_G4* pTile3 = &mTile3[gPsxDisplay.mBufferIndex];
    PolyG4_Init(pTile3);
    SetRGB0(pTile3, fadeColour, fadeColour, fadeColour);
    SetRGB1(pTile3, fadeColour, fadeColour, fadeColour);
    SetRGB2(pTile3, fadeColour, fadeColour, fadeColour);
    SetRGB3(pTile3, fadeColour, fadeColour, fadeColour);
    SetXYWH(pTile3, frameRect.w, frameRect.y, gPsxDisplay.mWidth - frameRect.w, frameRect.h - frameRect.y);

    Poly_Set_SemiTrans(&pTile3->mBase.header, 1);
    OrderingTable_Add(OtLayer(ppOt, GetAnimation().GetRenderLayer()), &pTile3->mBase.header);

    Poly_G4* pTile4 = &mTile4[gPsxDisplay.mBufferIndex];
    PolyG4_Init(pTile4);
    SetRGB0(pTile4, fadeColour, fadeColour, fadeColour);
    SetRGB1(pTile4, fadeColour, fadeColour, fadeColour);
    SetRGB2(pTile4, fadeColour, fadeColour, fadeColour);
    SetRGB3(pTile4, fadeColour, fadeColour, fadeColour);
    SetXYWH(pTile4, 0, frameRect.h, gPsxDisplay.mWidth, gPsxDisplay.mHeight - frameRect.h);

    Poly_Set_SemiTrans(&pTile4->mBase.header, 1);
    OrderingTable_Add(OtLayer(ppOt, GetAnimation().GetRenderLayer()), &pTile4->mBase.header);

    OrderingTable_Add(OtLayer(ppOt, GetAnimation().GetRenderLayer()), &mTPages[gPsxDisplay.mBufferIndex].mBase);

    if ((mFadeColour == 255 && mFadeIn) || (mFadeColour == 0 && !mFadeIn))
    {
        if (!mDone)
        {
            mDone = true;
            --gNumCamSwappers;
        }

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
        mFadeColour += mSpeed;
        if (mFadeIn)
        {
            if (mFadeColour > 255)
            {
                mFadeColour = 255;
            }
        }
        else if (mFadeColour < 0)
        {
            mFadeColour = 0;
        }
    }
}

void CircularFade::VFadeIn(s16 direction, s8 destroyOnDone)
{
    gNumCamSwappers++;

    mFadeIn = direction;

    mDone = false;

    mDestroyOnDone = destroyOnDone;

    if (mFadeIn)
    {
        mSpeed = 12;
    }
    else
    {
        mSpeed = -12;
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

CircularFade* Make_Circular_Fade(FP xpos, FP ypos, FP scale, s16 direction, s8 destroyOnDone, bool surviveDeathReset)
{
    auto pCircularFade = relive_new CircularFade(xpos, ypos, scale, direction, destroyOnDone);
    if (!pCircularFade)
    {
        return nullptr;
    }

    pCircularFade->SetSurviveDeathReset(surviveDeathReset);
    return pCircularFade;
}
