#include "stdafx.h"
#include "AbilityRing.hpp"
#include "../relive_lib/Function.hpp"
#include "../relive_lib/GameObjects/ScreenManager.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "Sfx.hpp"
#include "Map.hpp"
#include "Abe.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "PossessionFlicker.hpp"
#include "stdlib.hpp"
#include <algorithm>
#include "QuikSave.hpp"

AbilityRing* AbilityRing::Factory(FP xpos, FP ypos, RingTypes type, FP scale)
{
    return relive_new AbilityRing(xpos, ypos, type, scale);
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

AbilityRing::AbilityRing(FP xpos, FP ypos, RingTypes ringType, FP scale)
    : BaseGameObject(true, 0),
    mRingXPos(xpos),
    mRingYPos(ypos)
{
    SetType(ReliveTypes::eAbilityRing);
    gObjListDrawables->Push_Back(this);
    SetDrawable(true);

    if (mRingPolyBuffer)
    {
        mRingScreenX = FP_GetExponent(gScreenManager->CamXPos());
        mRingScreenY = FP_GetExponent(gScreenManager->CamYPos());

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

        mRingType = ringType;

        switch (mRingType)
        {
            case RingTypes::eExplosive_Emit_1:
            case RingTypes::eHealing_Emit_12:
                for (PSX_RECT& r : mRingCollideRects)
                {
                    r = {};
                }
                [[fallthrough]];

            case RingTypes::eExplosive_Emit_Effect_2:
            case RingTypes::eInvisible_Pulse_Emit_9:
            case RingTypes::eHealing_Emit_Effect_11:
                mRingThickness = FP_FromInteger(8);
                mRingSpeed = FP_FromInteger(6);
                mRingRight = FP_FromInteger(6);
                mRingLeft = FP_FromInteger(0);

                if (mRingType == RingTypes::eInvisible_Pulse_Emit_9)
                {
                    mRingRed = 0;
                    mRingGreen = 255;
                    mRingBlue = 32;
                }
                else
                {
                    if (ringType != RingTypes::eHealing_Emit_Effect_11 && ringType != RingTypes::eHealing_Emit_12)
                    {
                        mRingRed = 80;
                        mRingGreen = 0;
                        mRingBlue = 0;
                    }
                    else
                    {
                        mRingRed = 255;
                        mRingGreen = 255;
                        mRingBlue = 32;
                    }
                }

                SfxPlayMono(relive::SoundEffects::IngameTransition, 0);
                break;

            case RingTypes::eExplosive_Give_3:
            case RingTypes::eInvisible_Pulse_Give_10:
            case RingTypes::eHealing_Give_13:
                mRingThickness = FP_FromInteger(8);
                mRingSpeed = FP_FromInteger(6);
                mRingRight = FP_FromInteger(350);
                mRingLeft = FP_FromInteger(342);
                if (ringType == RingTypes::eInvisible_Pulse_Give_10)
                {
                    mRingRed = 0;
                    mRingGreen = 255;
                    mRingBlue = 32;
                }
                else if (ringType == RingTypes::eHealing_Give_13)
                {
                    mRingBlue = 32;
                    mRingRed = 255;
                    mRingGreen = 255;
                }
                else
                {
                    mRingRed = 80;
                    mRingGreen = 0;
                    mRingBlue = 0;
                }
                break;

            case RingTypes::eExplosive_Pulse_0:
            case RingTypes::eShrykull_Pulse_Small_4:
            case RingTypes::eInvisible_Pulse_Small_7:
            case RingTypes::eHealing_Pulse_14:
                VSetTarget(sActiveHero);
                [[fallthrough]];

            case RingTypes::eShrykull_Pulse_Large_5:
            case RingTypes::eShrykull_Pulse_Orange_6:
            case RingTypes::eInvisible_Pulse_Large_8:
                mRingThickness = FP_FromInteger(5);
                mRingSpeed = FP_FromInteger(4);
                mRingRight = FP_FromInteger(4);
                mRingLeft = FP_FromInteger(0);
                mRingFadeoutDistance = 50;

                switch (mRingType)
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

                    case RingTypes::eInvisible_Pulse_Small_7:
                    case RingTypes::eInvisible_Pulse_Large_8:
                        mRingRed = 0;
                        mRingGreen = 255;
                        mRingBlue = 0;
                        break;

                    case RingTypes::eHealing_Pulse_14:
                        mRingBlue = 32;
                        mRingRed = 255;
                        mRingGreen = 255;
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

        if (mRingType == RingTypes::eShrykull_Pulse_Orange_6 && scale == FP_FromDouble(0.5))
        {
            mRingLayer = Layer::eLayer_RollingBallBombMineCar_Half_16;
        }

        mRingScaleX = FP_FromDouble(1.0999); // TODO: Matching ?? 0x11999
        mRingScaleY = FP_FromInteger(1);
        mRingThickness = (mRingThickness * scale);
        mRingSpeed = (mRingSpeed * scale);
        mRingLeft = (mRingLeft * scale);
        mRingRight = (mRingRight * scale);

        for (s32 y = 0; y < 2; y++)
        {
            for (s32 x = 0; x < 64; x++)
            {
                Poly_G4* pPoly = &mRingPolyBuffer[x].mPolys[y];
                pPoly->SetRGB0(mRingRed & 255, mRingGreen & 255, mRingBlue & 255);
                pPoly->SetRGB1(mRingRed & 255, mRingGreen & 255, mRingBlue & 255);
                pPoly->SetRGB2(mRingRed & 255, mRingGreen & 255, mRingBlue & 255);
                pPoly->SetRGB3(mRingRed & 255, mRingGreen & 255, mRingBlue & 255);
                pPoly->SetSemiTransparent(mRingSemiTrans);
                pPoly->SetBlendMode(relive::TBlendModes::eBlend_1);
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

void AbilityRing::VRender(BasePrimitive** ppOt)
{
    if (gMap.Is_Point_In_Current_Camera(
            mRingLevel,
            mRingPath,
            mRingXPos,
            mRingYPos,
            0)
        && !mRefreshTargetObjId)
    {
        s16 y3 = mRingScreenYPos;
        s16 y4 = mRingScreenYPos;

        s16 x3 = PsxToPCX(FP_GetExponent(FP_FromInteger(mRingScreenXPos) + (mRingLeft * mRingScaleX)), 11);
        s16 x4 = PsxToPCX(FP_GetExponent(FP_FromInteger(mRingScreenXPos) + (mRingRight * mRingScaleX)), 11);

        u8 angIncrement = 0;
        if (mRingRight <= FP_FromInteger(150))
        {
            if (mRingRight <= FP_FromInteger(50))
            {
                mRingCount = 16;
                angIncrement = 16;
            }
            else
            {
                mRingCount = 32;
                angIncrement = 8;
            }
        }
        else
        {
            mRingCount = 64;
            angIncrement = 4;
        }

        u8 ang = angIncrement;
        for (s32 i = 0; i < mRingCount; i++)
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
                pPoly->SetXY0(x1, y1);
                pPoly->SetXY1(x2, y2);
                pPoly->SetXY2(x3, y3);
                pPoly->SetXY3(x4, y4);

                OrderingTable_Add(OtLayer(ppOt, mRingLayer), pPoly);


                mRingCollideRects[i] = rect;
                mRingCollideRects[i].x = PCToPsxX(mRingCollideRects[i].x, 20);
                mRingCollideRects[i].w = PCToPsxX(mRingCollideRects[i].w, 20);
            }

            x3 = x1;
            y3 = y1;
            x4 = x2;
            y4 = y2;

            ang += angIncrement;
        }
    }
}

void AbilityRing::VUpdate()
{
    if (mRefreshTargetObjId)
    {
        mRefreshTargetObjId = false;
        mRingTargetObjId = RefreshId(mRingTargetObjId);
    }

    auto pTarget = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(mRingTargetObjId));
    if (pTarget)
    {
        if (pTarget->GetDead())
        {
            mRingTargetObjId = Guid{};
        }
        else
        {
            mRingScreenX = FP_GetExponent(gScreenManager->CamXPos());
            mRingScreenY = FP_GetExponent(gScreenManager->CamYPos());
            const PSX_RECT bRect = pTarget->VGetBoundingRect();
            mRingScreenXPos = (bRect.x + bRect.w) / 2 - mRingScreenX;
            mRingScreenYPos = (bRect.y + bRect.h) / 2 - mRingScreenY;
        }
    }

    switch (mRingType)
    {
        case RingTypes::eExplosive_Pulse_0:
        case RingTypes::eShrykull_Pulse_Small_4:
        case RingTypes::eShrykull_Pulse_Orange_6:
        case RingTypes::eInvisible_Pulse_Small_7:
        case RingTypes::eInvisible_Pulse_Large_8:
        case RingTypes::eHealing_Pulse_14:
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
        case RingTypes::eHealing_Emit_12:
            if (mRingType == RingTypes::eHealing_Emit_12)
            {
                CollideWithObjects(false);
            }
            else
            {
                CollideWithObjects(true);
            }
            [[fallthrough]];

        case RingTypes::eExplosive_Emit_Effect_2:
        case RingTypes::eInvisible_Pulse_Emit_9:
        case RingTypes::eHealing_Emit_Effect_11:
        case RingTypes::eShrykull_Pulse_Large_5:
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
        case RingTypes::eInvisible_Pulse_Give_10:
        case RingTypes::eHealing_Give_13:
            mRingRight -= mRingSpeed;
            mRingLeft = mRingRight - mRingThickness;
            if (mRingLeft < FP_FromInteger(0))
            {
                SetDead(true);
                mRingLeft = FP_FromInteger(0);
                SfxPlayMono(relive::SoundEffects::IngameTransition, 0);
                if (mRingType == RingTypes::eExplosive_Give_3)
                {
                    relive_new PossessionFlicker(sActiveHero, 8, 255, 128, 128);
                }
            }
            break;

        default:
            return;
    }
}

void AbilityRing::VGetSaveState(SerializedObjectData& pSaveBuffer)
{
    AbilityRingSaveState data = {};

    data.mType = ReliveTypes::eAbilityRing;
    data.mRingXPos = mRingXPos;
    data.mRingYPos = mRingYPos;
    data.mRingType = mRingType;

    if (mRingLayer == Layer::eLayer_Above_FG1_39)
    {
        data.mRingScale = FP_FromInteger(1);
    }
    else
    {
        data.mRingScale = FP_FromDouble(0.5);
    }

    data.mRingRight = mRingRight;
    data.mRingCount = mRingCount;

    data.mRingRed = mRingRed;
    data.mRingGreen = mRingGreen;
    data.mRingBlue = mRingBlue;

    data.mRingTlvInfo = Guid{};
    if (mRingTargetObjId == Guid{})
    {
        pSaveBuffer.Write(data);
        return;
    }

    BaseGameObject* pTargetObj = sObjectIds.Find_Impl(mRingTargetObjId);
    if (pTargetObj)
    {
        data.mRingTlvInfo = pTargetObj->mBaseGameObjectTlvInfo;
    }
    pSaveBuffer.Write(data);
}

void AbilityRing::CreateFromSaveState(SerializedObjectData& pBuffer)
{
    const auto pState = pBuffer.ReadTmpPtr<AbilityRingSaveState>();
    auto pRing = relive_new AbilityRing(pState->mRingXPos, pState->mRingYPos, pState->mRingType, pState->mRingScale);
    if (pRing)
    {
        pRing->mRingRed = pState->mRingRed;
        pRing->mRingGreen = pState->mRingGreen;
        pRing->mRingBlue = pState->mRingBlue;
        pRing->mRingRight = pState->mRingRight;
        pRing->mRingCount = pState->mRingCount;
        pRing->mRingTargetObjId = pState->mRingTlvInfo;
        pRing->mRefreshTargetObjId = true;
    }
}


void AbilityRing::CollideWithObjects(s16 bDealDamage)
{
    for (s32 i = 0; i < mRingCount; i++)
    {
        mRingCollideRects[i].x += mRingScreenX;
        mRingCollideRects[i].y += mRingScreenY;
        mRingCollideRects[i].w += mRingScreenX;
        mRingCollideRects[i].h += mRingScreenY;
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
            for (s32 j = 0; j < mRingCount; j++)
            {
                if (mRingCollideRects[j].x <= bRect.w && mRingCollideRects[j].w >= bRect.x && mRingCollideRects[j].h >= bRect.y && mRingCollideRects[j].y <= bRect.h)
                {
                    if (bDealDamage)
                    {
                        if (bDealDamage == 1)
                        {
                            pObj->VTakeDamage(this);
                        }
                    }
                    else if (pObj->Type() == ReliveTypes::eMudokon)
                    {
                        // is the mudokon sick?
                        if (pObj->GetCanBePossessed())
                        {
                            if (pObj->mHealth > FP_FromInteger(0))
                            {
                                // heal the sick mudokon
                                pObj->VPossessed();
                            }
                        }
                    }
                }
            }
        }
    }
}

void AbilityRing::VSetTarget(BaseGameObject* pTarget)
{
    mRingTargetObjId = pTarget->mBaseGameObjectId;
}

void AbilityRing::VScreenChanged()
{
    if (mRingType == RingTypes::eHealing_Emit_12)
    {
        for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
        {
            IBaseAliveGameObject* pObj = gBaseAliveGameObjects->ItemAt(i);
            if (!pObj)
            {
                break;
            }

            if (pObj->Type() == ReliveTypes::eMudokon)
            {
                if (pObj->GetCanBePossessed())
                {
                    // Only heal alive muds in the same screen
                    if (pObj->Is_In_Current_Camera() == CameraPos::eCamCurrent_0 && pObj->mHealth > FP_FromInteger(0))
                    {
                        pObj->VPossessed();
                    }
                }
            }
        }
    }
    SetDead(true);
}
