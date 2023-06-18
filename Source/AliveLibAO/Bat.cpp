#include "stdafx_ao.h"
#include "Bat.hpp"
#include "../relive_lib/Collisions.hpp"
#include "Math.hpp"
#include "../relive_lib/Events.hpp"
#include "Game.hpp"
#include "Sfx.hpp"
#include "Abe.hpp"
#include "CameraSwapper.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "Midi.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"
#include "Path.hpp"
#include "../relive_lib/FixedPoint.hpp"

namespace AO {

void Bat::LoadAnimations()
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Bat));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Bat_Flying));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Bat_Unknown));
}

Bat::Bat(relive::Path_Bat* pTlv, const Guid& tlvId)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eBat);

    LoadAnimations();
    Animation_Init(GetAnimRes(AnimId::Bat));

    GetAnimation().SetRenderLayer(Layer::eLayer_SligGreeterFartsBats_33);

    FP hitX = {};
    FP hitY = {};
    // OG Bug fix, if bat isn't spawned on a line then we crash
    // so mBatLine is checked here and in VUpdate
    gCollisions->Raycast(
        FP_FromInteger(pTlv->mTopLeftX),
        FP_FromInteger(pTlv->mTopLeftY),
        FP_FromInteger(pTlv->mBottomRightX),
        FP_FromInteger(pTlv->mBottomRightY),
        &mBatLine,
        &hitX,
        &hitY,
        CollisionMask(eTrackLine_8));

    mTlvInfo = tlvId;

    if (mBatLine)
    {
        mXPos = FP_FromInteger(mBatLine->mRect.x);
        mYPos = FP_FromInteger(mBatLine->mRect.y);
    }

    mTimeBeforeMoving = pTlv->mTimeBeforeMoving;
    mBatSpeed = FP_FromRaw(pTlv->mSpeed << 8);

    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        SetSpriteScale(FP_FromDouble(0.5));
        SetScale(Scale::Bg);
        GetAnimation().SetRenderLayer(Layer::eLayer_BeforeShadow_Half_6);
    }
    else
    {
        SetSpriteScale(FP_FromInteger(1));
        SetScale(Scale::Fg);
        GetAnimation().SetRenderLayer(Layer::eLayer_BeforeShadow_25);
    }

    mBatState = BatStates::eSetTimer_0;
    mAttackTarget = nullptr;
    mAttackDuration = pTlv->mAttackDuration;
}

Bat::~Bat()
{
    if (mAttackTarget)
    {
        mAttackTarget->mBaseGameObjectRefCount--;
    }
    Path::TLV_Reset(mTlvInfo, -1, 0, 0);
}

void Bat::VScreenChanged()
{
    SetDead(true);
}

void Bat::FlyTo(FP xpos, FP ypos, FP* xSpeed, FP* ySpeed)
{
    const FP xd = FP_Abs(xpos - mEnemyXPos);
    if (xd > FP_FromInteger(350))
    {
        mXPos += *xSpeed;
    }

    const FP yd = FP_Abs(ypos - mEnemyYPos);
    if (yd > FP_FromInteger(200))
    {
        mYPos += *ySpeed;
    }

    mEnemyXPos = xpos;
    mEnemyYPos = ypos;

    *xSpeed = xpos + FP_FromInteger((Math_NextRandom() & 63) - 32);
    *xSpeed = *xSpeed - mXPos;

    *ySpeed = ypos + FP_FromInteger((Math_NextRandom() & 31) - 8);
    *ySpeed = *ySpeed - mYPos;

    const s32 ySpeedi = FP_GetExponent(*ySpeed);
    const s32 xSpeedi = FP_GetExponent(*xSpeed);
    FP x_final = FP_FromInteger(Math_SquareRoot_Int((ySpeedi * ySpeedi) + (xSpeedi * xSpeedi)));
    if (FP_GetExponent(x_final) == 0)
    {
        LOG_WARNING("Fixing divide by zero crash");
        x_final += FP_FromInteger(1);
    }

    mVelX = (FP_FromInteger(8) * *xSpeed) / x_final;
    mVelY = (FP_FromInteger(8) * *ySpeed) / x_final;

    mXPos += mVelX;
    mYPos += mVelY;
}

void Bat::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }

    if (gNumCamSwappers != 0)
    {
        return;
    }

    FP xSpeed = {};
    FP ySpeed = {};

    switch (mBatState)
    {
        case BatStates::eSetTimer_0:
            mBatState = BatStates::eInit_1;
            mTimer = MakeTimer(mTimeBeforeMoving);
            break;

        case BatStates::eInit_1:
            if (static_cast<s32>(sGnFrame) > mTimer)
            {
                mBatState = BatStates::eStartMoving_2;
                mBatVelX = FP_FromInteger(0);
                GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Bat_Unknown));
            }
            break;

        case BatStates::eStartMoving_2:
            if (mBatVelX < mBatSpeed)
            {
                mBatVelX += FP_FromDouble(1.8);
                if (mBatVelX > mBatSpeed)
                {
                    mBatVelX = mBatSpeed;
                }
            }

            if (mBatLine)
            {
                mBatLine = mBatLine->MoveOnLine(&mXPos, &mYPos, mBatVelX);
            }

            if (GetAnimation().GetIsLastFrame())
            {
                mBatState = BatStates::eFlying_3;
                GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Bat_Flying));
                mTimer = MakeTimer(Math_RandomRange(0, 90));
            }
            break;

        case BatStates::eFlying_3:
            if (mBatVelX < mBatSpeed)
            {
                mBatVelX += FP_FromDouble(1.8);
                if (mBatVelX > mBatSpeed)
                {
                    mBatVelX = mBatSpeed;
                }
            }

            if (!(GetAnimation().GetCurrentFrame() % 3))
            {
                SfxPlayMono(relive::RandomSfx(relive::SoundEffects::Bat1, relive::SoundEffects::Bat2), Math_RandomRange(20, 26));
            }

            if (static_cast<s32>(sGnFrame) > mTimer)
            {
                SND_SEQ_PlaySeq(SeqId::eBatSqueaking_18, 1, 1);
                mTimer = MakeTimer(Math_RandomRange(120, 240));
            }

            if (mBatLine)
            {
                mBatLine = mBatLine->MoveOnLine(&mXPos, &mYPos, mBatVelX);
            }

            if (!mBatLine)
            {
                SetDead(true);
            }

            if (!sActiveHero->mShrivel)
            {
                for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
                {
                    IBaseAliveGameObject* pObjIter = gBaseAliveGameObjects->ItemAt(i);
                    if (!pObjIter)
                    {
                        break;
                    }

                    if (pObjIter->Type() != ReliveTypes::eSecurityOrb && pObjIter->Type() != ReliveTypes::eSlig && pObjIter->Type() != ReliveTypes::eSlog)
                    {
                        const PSX_RECT bObjRect = pObjIter->VGetBoundingRect();
                        if (FP_GetExponent(mXPos) >= bObjRect.x && FP_GetExponent(mXPos) <= bObjRect.w && FP_GetExponent(mYPos) >= bObjRect.y && FP_GetExponent(mYPos) <= bObjRect.h && pObjIter->GetSpriteScale() == GetSpriteScale())
                        {
                            for (s32 j = 0; j < gBaseGameObjects->Size(); j++)
                            {
                                BaseGameObject* pMaybeBat = gBaseGameObjects->ItemAt(j);
                                if (!pMaybeBat)
                                {
                                    break;
                                }

                                if (pMaybeBat->Type() == ReliveTypes::eBat)
                                {
                                    auto pBat = static_cast<Bat*>(pMaybeBat);

                                    pBat->mAttackTarget = pObjIter;
                                    pBat->mAttackTarget->mBaseGameObjectRefCount++;

                                    pBat->mBatState = BatStates::eAttackTarget_4;
                                    pBat->GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Bat_Flying));

                                    pBat->mTimer = 0;
                                    pBat->mAttackDurationTimer = sGnFrame + pBat->mAttackDuration;

                                    pBat->mEnemyXPos = pBat->mAttackTarget->mXPos;
                                    pBat->mEnemyYPos = pBat->mAttackTarget->mYPos;
                                }
                            }
                        }
                    }
                }
            }
            break;

        case BatStates::eAttackTarget_4:
        {
            if (mAttackTarget->GetDead() || EventGet(kEventDeathReset))
            {
                SetDead(true);
                return;
            }

            const PSX_RECT bRect = mAttackTarget->VGetBoundingRect();
            FlyTo(
                FP_FromInteger((bRect.w + bRect.x) / 2),
                FP_FromInteger((bRect.h + bRect.y) / 2),
                &xSpeed,
                &ySpeed);

            if (FP_Abs(xSpeed) < FP_FromInteger(10))
            {
                if (FP_Abs(ySpeed) < FP_FromInteger(20) && static_cast<s32>(sGnFrame) > mTimer)
                {
                    mAttackTarget->VTakeDamage(this);
                    mTimer = MakeTimer(30);
                    SND_SEQ_PlaySeq(SeqId::eBatSqueaking_18, 1, 1);
                }
            }

            if (mAttackDurationTimer <= static_cast<s32>(sGnFrame))
            {
                mAttackTarget->mBaseGameObjectRefCount--;
                mAttackTarget = nullptr;
                mBatState = BatStates::eFlyAwayAndDie_5;
            }
        }
        break;

        case BatStates::eFlyAwayAndDie_5:
        {
            FlyTo(mXPos, mYPos - FP_FromInteger(40), &xSpeed, &ySpeed);
            if (EventGet(kEventDeathReset))
            {
                SetDead(true);
            }
        }
        break;

        default:
            return;
    }
}

} // namespace AO
