#include "stdafx.h"
#include "CircularFade.hpp"
#include "../relive_lib/Function.hpp"
#include "../relive_lib/GameObjects/ScreenManager.hpp"
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
    GetAnimation().SetBlendMode(relive::TBlendModes::eBlend_2);
    GetAnimation().SetRenderLayer(Layer::eLayer_FadeFlash_40);
    mRGB.SetRGB(mFadeColour, mFadeColour, mFadeColour);
}

CircularFade::~CircularFade()
{
    if (!mDone)
    {
        --gNumCamSwappers;
    }
}

void CircularFade::VRender(OrderingTable& ot)
{
    const u8 fade_rgb = static_cast<u8>((mFadeColour * 60) / 100);

    mRGB.SetRGB(fade_rgb, fade_rgb, fade_rgb);

    GetAnimation().SetRGB(fade_rgb, fade_rgb, fade_rgb);

    GetAnimation().VRender(
        FP_GetExponent(FP_FromInteger(mXOffset) + mXPos - gScreenManager->CamXPos()),
        FP_GetExponent(FP_FromInteger(mYOffset) + mYPos - gScreenManager->CamYPos()),
        ot,
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
    pTile1->SetRGB0(fadeColour, fadeColour, fadeColour);
    pTile1->SetRGB1(fadeColour, fadeColour, fadeColour);
    pTile1->SetRGB2(fadeColour, fadeColour, fadeColour);
    pTile1->SetRGB3(fadeColour, fadeColour, fadeColour);
    pTile1->SetXYWH(0, 0, gPsxDisplay.mWidth, frameRect.y);
    pTile1->SetSemiTransparent(true);
    pTile1->SetBlendMode(relive::TBlendModes::eBlend_2);
    ot.Add(GetAnimation().GetRenderLayer(), pTile1);

    Poly_G4* pTile2 = &mTile2[gPsxDisplay.mBufferIndex];
    pTile2->SetRGB0(fadeColour, fadeColour, fadeColour);
    pTile2->SetRGB1(fadeColour, fadeColour, fadeColour);
    pTile2->SetRGB2(fadeColour, fadeColour, fadeColour);
    pTile2->SetRGB3(fadeColour, fadeColour, fadeColour);
    pTile2->SetXYWH(0, frameRect.y, GetAnimation().GetFlipX() ? frameRect.x + 1 : frameRect.x, frameRect.h - frameRect.y);
    pTile2->SetSemiTransparent(true);
    pTile2->SetBlendMode(relive::TBlendModes::eBlend_2);
    ot.Add(GetAnimation().GetRenderLayer(), pTile2);

    Poly_G4* pTile3 = &mTile3[gPsxDisplay.mBufferIndex];
    pTile3->SetRGB0(fadeColour, fadeColour, fadeColour);
    pTile3->SetRGB1(fadeColour, fadeColour, fadeColour);
    pTile3->SetRGB2(fadeColour, fadeColour, fadeColour);
    pTile3->SetRGB3(fadeColour, fadeColour, fadeColour);
    pTile3->SetXYWH(frameRect.w, frameRect.y, gPsxDisplay.mWidth - frameRect.w, frameRect.h - frameRect.y);
    pTile3->SetSemiTransparent(true);
    pTile3->SetBlendMode(relive::TBlendModes::eBlend_2);
    ot.Add(GetAnimation().GetRenderLayer(), pTile3);

    Poly_G4* pTile4 = &mTile4[gPsxDisplay.mBufferIndex];
    pTile4->SetRGB0(fadeColour, fadeColour, fadeColour);
    pTile4->SetRGB1(fadeColour, fadeColour, fadeColour);
    pTile4->SetRGB2(fadeColour, fadeColour, fadeColour);
    pTile4->SetRGB3(fadeColour, fadeColour, fadeColour);
    pTile4->SetXYWH(0, frameRect.h, gPsxDisplay.mWidth, gPsxDisplay.mHeight - frameRect.h);
    pTile4->SetSemiTransparent(true);
    pTile4->SetBlendMode(relive::TBlendModes::eBlend_2);
    ot.Add(GetAnimation().GetRenderLayer(), pTile4);

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
