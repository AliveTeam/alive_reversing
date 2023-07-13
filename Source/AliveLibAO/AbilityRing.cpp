#include "stdafx_ao.h"
#include "AbilityRing.hpp"
#include "../relive_lib/Function.hpp"
#include "../relive_lib/GameObjects/ScreenManager.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "Sfx.hpp"
#include "Map.hpp"
#include "Abe.hpp"
#include "BaseAliveGameObject.hpp"
#include "PossessionFlicker.hpp"
#include "Math.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "../relive_lib/ObjectIds.hpp"

#undef min
#undef max

#include <algorithm>

namespace AO {

AbilityRing* AbilityRing::Factory(FP xpos, FP ypos, RingTypes ring_type)
{
    return relive_new AbilityRing(xpos, ypos, ring_type);
}

static s32 MinDistance(s32 screenX, s32 screenY, s32 width1, s32 height1, s32 width2, s32 height2)
{
    const s32 d1 = Math_Distance(screenX, screenY, width1, height1);
    const s32 d2 = Math_Distance(screenX, screenY, width2, height2);
    if (d2 <= d1)
    {
        return d1;
    }
    else
    {
        return d2;
    }
}

AbilityRing::AbilityRing(FP xpos, FP ypos, RingTypes ring_type)
    : BaseGameObject(true, 0),
    mRingXPos(xpos),
    mRingYPos(ypos)
{
    SetType(ReliveTypes::eAbilityRing);
    mRingTargetObjId = Guid{};
    gObjListDrawables->Push_Back(this);
    SetDrawable(true);

    if (mRingPolyBuffer)
    {
        mRingScreenX = FP_GetExponent(gScreenManager->mCamPos->x - FP_FromInteger(gScreenManager->mCamXOff));
        mRingScreenY = FP_GetExponent(gScreenManager->mCamPos->y - FP_FromInteger(gScreenManager->mCamYOff));

        mRingScreenXPos = FP_GetExponent(xpos) - mRingScreenX;
        mRingScreenYPos = FP_GetExponent(ypos) - mRingScreenY;

        const s32 d1 = MinDistance(mRingScreenXPos, mRingScreenYPos, gPsxDisplay.mWidth, 0, 0, 0);
        const s32 d2 = MinDistance(mRingScreenXPos, mRingScreenYPos, gPsxDisplay.mWidth, gPsxDisplay.mHeight, 0, gPsxDisplay.mHeight);

        if (d1 <= d2)
        {
            mRingFadeoutDistance = static_cast<s16>(MinDistance(mRingScreenXPos, mRingScreenYPos, gPsxDisplay.mWidth, gPsxDisplay.mHeight, 0, gPsxDisplay.mHeight));
        }
        else
        {
            mRingFadeoutDistance = static_cast<s16>(MinDistance(mRingScreenXPos, mRingScreenYPos, gPsxDisplay.mWidth, 0, 0, 0));
        }

        mRingType = ring_type;

        switch (mRingType)
        {
            case RingTypes::eExplosive_Emit_1:
                for (PSX_RECT& r : mRingCollideRects)
                {
                    r = {};
                }
                [[fallthrough]];

            case RingTypes::eExplosive_Emit_Effect_2:
                mRingThickness = FP_FromInteger(8);
                mRingSpeed = FP_FromInteger(6);
                mRingRight = FP_FromInteger(6);
                mRingLeft = FP_FromInteger(0);
                mRingRed = 80;
                mRingGreen = 0;
                mRingBlue = 0;
                SfxPlayMono(relive::SoundEffects::IngameTransition, 0);
                break;

            case RingTypes::eExplosive_Give_3:
                mRingThickness = FP_FromInteger(8);
                mRingSpeed = FP_FromInteger(6);
                mRingRight = FP_FromInteger(350);
                mRingLeft = FP_FromInteger(342);
                mRingRed = 80;
                mRingGreen = 0;
                mRingBlue = 0;
                break;

            case RingTypes::eExplosive_Pulse_0:
            case RingTypes::eShrykull_Pulse_Small_4:
                SetTarget(sActiveHero);
                [[fallthrough]];

            case RingTypes::eShrykull_Pulse_Large_5:
            case RingTypes::eShrykull_Pulse_Orange_6:
                mRingThickness = FP_FromInteger(5);
                mRingSpeed = FP_FromInteger(4);
                mRingRight = FP_FromInteger(4);
                mRingLeft = FP_FromInteger(0);
                mRingFadeoutDistance = 50;
                switch (ring_type)
                {
                    case RingTypes::eExplosive_Pulse_0:
                        mRingRed = 255;
                        mRingGreen = 0;
                        mRingBlue = 0;
                        break;
                    case RingTypes::eShrykull_Pulse_Small_4:
                        mRingRed = 0;
                        mRingGreen = 0;
                        mRingBlue = 255;
                        break;
                    case RingTypes::eShrykull_Pulse_Large_5:
                        mRingRed = 0;
                        mRingGreen = 0;
                        mRingBlue = 80;
                        break;
                    case RingTypes::eShrykull_Pulse_Orange_6:
                        mRingRed = 255;
                        mRingGreen = 128;
                        mRingBlue = 64;
                        break;
                    default:
                        break;
                }
                break;

            default:
                break;
        }

        mRingPath = gMap.mCurrentPath;
        mRingLayer = Layer::eLayer_Above_FG1_39;
        mRingLevel = gMap.mCurrentLevel;

        mRingScaleX = FP_FromDouble(1.0999); // TODO: Matching ?? 0x11999
        mRingScaleY = FP_FromInteger(1);

        for (s32 y = 0; y < 2; y++)
        {
            for (s32 x = 0; x < 64; x++)
            {
                Poly_G4* pPoly = &mRingPolyBuffer[x].mPolys[y];
                pPoly->SetRGB0(mRingRed & 255, mRingGreen & 255, mRingBlue & 255);
                pPoly->SetRGB1(mRingRed & 255, mRingGreen & 255, mRingBlue & 255);
                pPoly->SetRGB2(mRingRed & 255, mRingGreen & 255, mRingBlue & 255);
                pPoly->SetRGB3(mRingRed & 255, mRingGreen & 255, mRingBlue & 255);
                pPoly->SetBlendMode(relive::TBlendModes::eBlend_1);
                pPoly->SetSemiTransparent(mRingSemiTrans);
            }
        }
    }
    else
    {
        SetDead(true);
    }
}

AbilityRing::~AbilityRing()
{
    relive_delete[] mRingPolyBuffer;
    gObjListDrawables->Remove_Item(this);
}

void AbilityRing::VRender(OrderingTable& ot)
{
    if (gMap.Is_Point_In_Current_Camera(
            mRingLevel,
            mRingPath,
            mRingXPos,
            mRingYPos,
            0)) //&& !mRefreshTargetObjId) Missing part of the check from AE
    {
        s16 y3 = mRingScreenYPos;
        s16 y4 = mRingScreenYPos;

        s16 x3 = PsxToPCX(FP_GetExponent(FP_FromInteger(mRingScreenXPos) + (mRingLeft * mRingScaleX)), 11);
        s16 x4 = PsxToPCX(FP_GetExponent(FP_FromInteger(mRingScreenXPos) + (mRingRight * mRingScaleX)), 11);

        //Not hardcoded in Exoddus
        u8 ang = 4;
        auto count = 64;

        for (s32 i = 0; i < count; i++)
        {
            const s16 x1 = (s16) PsxToPCX(mRingScreenXPos + FP_GetExponent(mRingLeft * Math_Sine(ang) * mRingScaleX), 11);
            const s16 x2 = (s16) PsxToPCX(mRingScreenXPos + FP_GetExponent(mRingRight * Math_Sine(ang) * mRingScaleX), 11);

            const s16 y1 = mRingScreenYPos + FP_GetExponent(mRingLeft * Math_Cosine(ang) * mRingScaleY);
            const s16 y2 = mRingScreenYPos + FP_GetExponent(mRingRight * Math_Cosine(ang) * mRingScaleY);

            const s16 x = std::min(std::min(x1, x3), std::min(x2, x4));
            const s16 y = std::min(std::min(y1, y3), std::min(y2, y4));
            const s16 w = std::max(std::max(x1, x3), std::max(x2, x4));
            const s16 h = std::max(std::max(y1, y3), std::max(y2, y4));

            const PSX_RECT rect = {x, y, w, h};
            if (rect.w < 0 || rect.x > 640 || rect.y > 240 || rect.h < 0)
            {
                //TODO untie from Render() into Update()
                mRingCollideRects[i].x = 0;
                mRingCollideRects[i].w = 0;
                mRingCollideRects[i].y = 0;
                mRingCollideRects[i].h = 0;
            }
            else
            {
                Poly_G4* pPoly = &mRingPolyBuffer[i].mPolys[gPsxDisplay.mBufferIndex];
                pPoly->SetXY0( x1, y1);
                pPoly->SetXY1( x2, y2);
                pPoly->SetXY2( x3, y3);
                pPoly->SetXY3( x4, y4);

                ot.Add(mRingLayer, pPoly);

                mRingCollideRects[i] = rect;
                mRingCollideRects[i].x = PCToPsxX(mRingCollideRects[i].x, 20);
                mRingCollideRects[i].w = PCToPsxX(mRingCollideRects[i].w, 20);
            }

            x3 = x1;
            y3 = y1;
            x4 = x2;
            y4 = y2;

            ang += 4;
        }
    }
}

void AbilityRing::VUpdate()
{
    auto pTarget = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(mRingTargetObjId));
    if (pTarget)
    {
        if (pTarget->GetDead())
        {
            mRingTargetObjId = Guid{};
        }
        else
        {
            mRingScreenX = FP_GetExponent(gScreenManager->mCamPos->x - FP_FromInteger(gScreenManager->mCamXOff));
            mRingScreenY = FP_GetExponent(gScreenManager->mCamPos->y - FP_FromInteger(gScreenManager->mCamYOff));

            const PSX_RECT bRect = pTarget->VGetBoundingRect();

            mRingScreenXPos = (bRect.w + bRect.x) / 2 - mRingScreenX;
            mRingScreenYPos = (bRect.h + bRect.y) / 2 - mRingScreenY;
        }
    }

    switch (mRingType)
    {
        case RingTypes::eExplosive_Pulse_0:
        case RingTypes::eShrykull_Pulse_Small_4:
        case RingTypes::eShrykull_Pulse_Orange_6:
            mRingRight += mRingSpeed;
            mRingLeft = mRingRight - mRingThickness;

            if (mRingLeft < FP_FromInteger(0))
            {
                mRingLeft = FP_FromInteger(0);
            }

            if (FP_GetExponent(mRingLeft) <= mRingFadeoutDistance)
            {
                mRingRed = (mRingRed >> 1) + (mRingRed >> 2);
                mRingGreen = (mRingGreen >> 1) + (mRingGreen >> 2);
                mRingBlue = (mRingBlue >> 1) + (mRingBlue >> 2);

                for (s32 i = 0; i < 2; i++)
                {
                    for (s32 j = 0; j < 64; j++)
                    {
                        mRingPolyBuffer[j].mPolys[i].SetRGB0(mRingRed & 255, mRingGreen & 255, mRingBlue & 255);
                        mRingPolyBuffer[j].mPolys[i].SetRGB1(mRingRed & 255, mRingGreen & 255, mRingBlue & 255);
                        mRingPolyBuffer[j].mPolys[i].SetRGB2(mRingRed & 255, mRingGreen & 255, mRingBlue & 255);
                        mRingPolyBuffer[j].mPolys[i].SetRGB3(mRingRed & 255, mRingGreen & 255, mRingBlue & 255);
                    }
                }
            }
            else
            {
                SetDead(true);
            }
            return;

        case RingTypes::eExplosive_Emit_1:
            CollideWithObjects();
            [[fallthrough]];

        case RingTypes::eExplosive_Emit_Effect_2:
            mRingRight += mRingSpeed;
            mRingLeft = mRingRight - mRingThickness;

            if (mRingLeft < FP_FromInteger(0))
            {
                mRingLeft = FP_FromInteger(0);
            }

            if (FP_GetExponent(mRingLeft) > mRingFadeoutDistance)
            {
                SetDead(true);
            }
            break;

        case RingTypes::eExplosive_Give_3:
            mRingRight -= mRingSpeed;
            mRingLeft = mRingRight - mRingThickness;
            if (mRingLeft < FP_FromInteger(0))
            {
                SetDead(true);
                mRingLeft = FP_FromInteger(0);
                SfxPlayMono(relive::SoundEffects::IngameTransition, 0);
                relive_new PossessionFlicker(sActiveHero, 8, 255, 128, 128);
            }
            break;

        case RingTypes::eShrykull_Pulse_Large_5:
            mRingRight += mRingSpeed;
            mRingLeft = mRingRight - mRingThickness;
            if (mRingLeft >= FP_FromInteger(0))
            {
                if (FP_GetExponent(mRingLeft) > mRingFadeoutDistance)
                {
                    SetDead(true);
                }
            }
            else
            {
                mRingLeft = FP_FromInteger(0);
                if (mRingFadeoutDistance < 0)
                {
                    SetDead(true);
                }
            }
            break;
        default:
            return;
    }
}

void AbilityRing::VScreenChanged()
{
    SetDead(true);
}

void AbilityRing::CollideWithObjects()
{
    for (auto& rect : mRingCollideRects)
    {
        rect.x += mRingScreenX;
        rect.y += mRingScreenY;
        rect.w += mRingScreenX;
        rect.h += mRingScreenY;
    }

    for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
    {
        IBaseAliveGameObject* pObj = gBaseAliveGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        const PSX_RECT bRect = pObj->VGetBoundingRect();

        if (!(pObj->GetDead()))
        {
            for (auto& rect : mRingCollideRects)
            {
                if (rect.x <= bRect.w && rect.w >= bRect.x && rect.h >= bRect.y && rect.y <= bRect.h)
                {
                    pObj->VTakeDamage(this);
                }
            }
        }
    }
}

void AbilityRing::SetTarget(BaseAliveGameObject* pTarget)
{
    mRingTargetObjId = pTarget->mBaseGameObjectId;
}

} // namespace AO
