#include "stdafx_ao.h"
#include "Function.hpp"
#include "Grenade.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "ThrowableArray.hpp"
#include "PlatformBase.hpp"
#include "Map.hpp"
#include "Math.hpp"
#include "Sfx.hpp"
#include "Explosion.hpp"
#include "Gibs.hpp"
#include "../relive_lib/Events.hpp"
#include "../relive_lib/Collisions.hpp"
#include "Grid.hpp"

namespace AO {

s16 gInfiniteGrenades_5076EC = 0;

Grenade::Grenade(FP xpos, FP ypos, s16 numGrenades)
    : BaseThrowable()
{
    field_10E_bDead = 0;
    SetType(ReliveTypes::eGrenade);

    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Grenade));
    Animation_Init(GetAnimRes(AnimId::Grenade));

    mBaseGameObjectFlags.Clear(Options::eInteractive_Bit8);
    mAnim.mRenderMode = TPageAbr::eBlend_0;

    mAnim.mFlags.Clear(AnimFlags::eRender);
    mAnim.mFlags.Set(AnimFlags::eSemiTrans);

    mXPos = xpos;
    field_120_xpos = xpos;

    mYPos = ypos;
    field_124_ypos = ypos;

    mVelX = FP_FromInteger(0);
    mVelY = FP_FromInteger(0);
    field_10C_count = numGrenades;

    if (numGrenades > 0)
    {
        field_110_state = States::eFallingToBeCollected_0;
    }
    else
    {
        field_110_state = States::eCountingDown_3;
        field_112_explode_timer = 90;
    }

    field_118 = 0;
    field_11C = 0;
}

Grenade::~Grenade()
{
    if (field_11C)
    {
        field_11C->mBaseGameObjectRefCount--;
    }

    if (!gInfiniteGrenades_5076EC && !field_10E_bDead)
    {
        if (mLiftPoint)
        {
            mLiftPoint->VRemove(this);
            mLiftPoint->mBaseGameObjectRefCount--;
            mLiftPoint = nullptr;
        }

        if (gpThrowableArray_50E26C)
        {
            s16 count = 0;
            if (field_10C_count >= 1u)
            {
                count = field_10C_count;
            }
            else
            {
                count = 1;
            }
            gpThrowableArray_50E26C->Remove(count);
        }
    }
}

void Grenade::AddToPlatform()
{
    BaseAddToPlatform();
}

void Grenade::VTimeToExplodeRandom()
{
    field_112_explode_timer -= Math_NextRandom() % 16;
}

void Grenade::VThrow(FP velX, FP velY)
{
    mAnim.mFlags.Set(AnimFlags::eRender);

    mVelX = velX;
    mVelY = velY;

    if (field_10C_count == 0)
    {
        field_110_state = States::eFalling_4;
    }
}

void Grenade::VScreenChanged()
{
    if (gMap.mCurrentLevel != gMap.mNextLevel || gMap.mCurrentPath != gMap.mNextPath)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void Grenade::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(Options::eDead);
    }

    switch (field_110_state)
    {
        case States::eFallingToBeCollected_0:
            if (!InTheAir())
            {
                mCollectionRect.x = mXPos - (ScaleToGridSize(mSpriteScale) / FP_FromInteger(2));
                mCollectionRect.y = mYPos - ScaleToGridSize(mSpriteScale);
                mCollectionRect.w = mXPos + (ScaleToGridSize(mSpriteScale) / FP_FromInteger(2));
                mCollectionRect.h = mYPos;

                mBaseGameObjectFlags.Set(Options::eInteractive_Bit8);
                field_110_state = States::eWaitToBeCollected_1;
            }
            break;

        case States::eWaitToBeCollected_1:
            if (mVelX < FP_FromInteger(0))
            {
                mVelX = FP_FromInteger(0);
            }

            if (FP_Abs(mVelX) >= FP_FromInteger(1))
            {
                if (mVelX <= FP_FromInteger(0))
                {
                    mVelX += FP_FromDouble(0.01);
                }
                else
                {
                    mVelX -= FP_FromDouble(0.01);
                }

                const auto oldLine = field_114_pCollisionLine;
                field_114_pCollisionLine = field_114_pCollisionLine->MoveOnLine(&mXPos, &mYPos, mVelX);
                if (mLiftPoint)
                {
                    if (field_114_pCollisionLine != oldLine)
                    {
                        mLiftPoint->VRemove(this);
                        mLiftPoint->mBaseGameObjectRefCount--;
                        mLiftPoint = nullptr;
                    }
                }

                if (!field_114_pCollisionLine)
                {
                    field_110_state = States::eFallingToBeCollected_0;
                }
            }
            else if (abs(SnapToXGrid(mSpriteScale, FP_GetExponent(mXPos)) - FP_GetExponent(mXPos)) > 1)
            {
                const auto oldLine = field_114_pCollisionLine;
                field_114_pCollisionLine = field_114_pCollisionLine->MoveOnLine(&mXPos, &mYPos, mVelX);
                if (mLiftPoint)
                {
                    if (field_114_pCollisionLine != oldLine)
                    {
                        mLiftPoint->VRemove(this);
                        mLiftPoint->mBaseGameObjectRefCount--;
                        mLiftPoint = nullptr;
                    }
                }

                if (!field_114_pCollisionLine)
                {
                    field_110_state = States::eFalling_4;
                }
            }
            else
            {
                mVelX = FP_FromInteger(0);

                mCollectionRect.x = mXPos - (ScaleToGridSize(mSpriteScale) / FP_FromInteger(2));
                mCollectionRect.y = mYPos - ScaleToGridSize(mSpriteScale);
                mCollectionRect.w = mXPos + (ScaleToGridSize(mSpriteScale) / FP_FromInteger(2));
                mCollectionRect.h = mYPos;

                mBaseGameObjectFlags.Set(Options::eInteractive_Bit8);
                field_110_state = States::eDoesNothing_2;
            }
            break;

        case States::eDoesNothing_2:
            break;

        case States::eCountingDown_3:
            BlowUpAfterCountdown();
            break;

        case States::eFalling_4:
            if (InTheAir())
            {
                if (!BlowUpAfterCountdown())
                {
                    const PSX_RECT bRect = VGetBoundingRect();

                    const PSX_Point xy = {bRect.x, static_cast<s16>(bRect.y + 5)};
                    const PSX_Point wh = {bRect.w, static_cast<s16>(bRect.h + 5)};

                    VOnCollisionWith(xy, wh, gBaseGameObjects, (TCollisionCallBack) &Grenade::OnCollision_BounceOff);
                }
            }
            else
            {
                field_110_state = States::eHitGround_5;
            }
            break;

        case States::eHitGround_5:
        {
            mVelX = FP_FromRaw(mVelX.fpValue / 2);

            const auto oldLine = field_114_pCollisionLine;
            field_114_pCollisionLine = field_114_pCollisionLine->MoveOnLine(&mXPos, &mYPos, mVelX);
            if (mLiftPoint)
            {
                if (field_114_pCollisionLine != oldLine)
                {
                    mLiftPoint->VRemove(this);
                    mLiftPoint->mBaseGameObjectRefCount--;
                    mLiftPoint = nullptr;
                }
            }

            if (!field_114_pCollisionLine)
            {
                field_110_state = States::eFalling_4;
            }

            BlowUpAfterCountdown();
            break;
        }

        case States::eWaitForExplodeEnd_6:
            if (field_11C->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
            {
                field_110_state = States::eExploded_7;
                field_11C->mBaseGameObjectRefCount--;
                field_11C = nullptr;
            }
            break;

        case States::eExploded_7:
            mBaseGameObjectFlags.Set(Options::eDead);
            break;

        default:
            return;
    }
}

void Grenade::VOnTrapDoorOpen()
{
    if (mLiftPoint)
    {
        mLiftPoint->VRemove(this);
        mLiftPoint->mBaseGameObjectRefCount--;
        mLiftPoint = nullptr;

        if (field_110_state == States::eWaitToBeCollected_1 || field_110_state == States::eDoesNothing_2)
        {
            field_110_state = States::eFallingToBeCollected_0;
        }
        else if (field_110_state != States::eWaitForExplodeEnd_6)
        {
            field_110_state = States::eFalling_4;
        }
    }
}

s16 Grenade::InTheAir()
{
    field_120_xpos = mXPos;
    field_124_ypos = mYPos;

    mVelY += FP_FromInteger(1);

    mXPos += mVelX;
    mYPos += mVelY;

    u16 result = 0;
    mXPos = CamX_VoidSkipper(mXPos, mVelX, 8, &result);
    mYPos = CamY_VoidSkipper(mYPos, mVelY, 8, &result);

    FP hitX = {};
    FP hitY = {};
    const auto bHit = sCollisions->Raycast(
        field_120_xpos,
        field_124_ypos,
        mXPos,
        mYPos,
        &field_114_pCollisionLine,
        &hitX,
        &hitY,
        mSpriteScale != FP_FromDouble(0.5) ? kFgFloor : kBgFloor);

    result = bHit ? 1 : 0;

    if (bHit == 1)
    {
        if (mVelY > FP_FromInteger(0))
        {
            if (mVelY < FP_FromInteger(1))
            {
                if (!mLiftPoint)
                {
                    AddToPlatform();
                }
                return 0;
            }

            mXPos = hitX;
            mYPos = hitY;
            mVelY = (-mVelY / FP_FromInteger(2));
            mVelX = (mVelX / FP_FromInteger(2));
            if (field_118 <= 4)
            {
                s16 vol = 75 - 20 * field_118;
                if (vol < 40)
                {
                    vol = 40;
                }
                SfxPlayMono(relive::SoundEffects::GrenadeBounce, vol);
                field_118++;
                EventBroadcast(kEventNoise, this);
                EventBroadcast(kEventSuspiciousNoise, this);
                EventBroadcast(kEventSpeaking, this);
            }
        }
    }

    const auto v20 = sCollisions->Raycast(
        field_120_xpos,
        field_124_ypos,
        mXPos,
        mYPos,
        &field_114_pCollisionLine,
        &hitX,
        &hitY,
        mSpriteScale != FP_FromDouble(0.5) ? kFgWalls : kBgWalls);

    if (v20 == 1)
    {
        switch (field_114_pCollisionLine->mLineType)
        {
            case 1:
            case 5:
                if (mVelX < FP_FromInteger(0))
                {
                    mYPos = hitY;
                    mXPos = hitX;
                    mVelX = (-mVelX / FP_FromInteger(2));
                    s16 vol = 75 - 20 * field_118;
                    if (vol < 40)
                    {
                        vol = 40;
                    }
                    SfxPlayMono(relive::SoundEffects::GrenadeBounce, vol);
                    EventBroadcast(kEventNoise, this);
                    EventBroadcast(kEventSuspiciousNoise, this);
                    EventBroadcast(kEventSpeaking, this);
                }
                break;

            case 2:
            case 6:
                if (mVelX > FP_FromInteger(0))
                {
                    mXPos = hitX;
                    mYPos = hitY;
                    mVelX = (-mVelX / FP_FromInteger(2));
                    s16 vol = 75 - 20 * field_118;
                    if (vol < 40)
                    {
                        vol = 40;
                    }

                    SfxPlayMono(relive::SoundEffects::GrenadeBounce, vol);
                    EventBroadcast(kEventNoise, this);
                    EventBroadcast(kEventSuspiciousNoise, this);
                    EventBroadcast(kEventSpeaking, this);
                }
                break;

            default:
                return 1;
        }
    }
    return 1;
}

s16 Grenade::OnCollision_BounceOff(BaseGameObject* pHit)
{
    if (!pHit->mBaseGameObjectFlags.Get(BaseGameObject::eCanExplode_Bit7))
    {
        return 1;
    }

    auto pHit2 = static_cast<BaseAliveGameObject*>(pHit);

    const PSX_RECT bRect = pHit2->VGetBoundingRect();

    if (mXPos < FP_FromInteger(bRect.x + 12) || mXPos > FP_FromInteger(bRect.w - 12))
    {
        mXPos = field_120_xpos;
        mVelX = (-mVelX / FP_FromInteger(2));
    }
    else
    {
        mYPos = field_124_ypos;
        mVelY = (-mVelY / FP_FromInteger(2));
    }

    pHit2->VOnThrowableHit(this);

    SfxPlayMono(relive::SoundEffects::RockBounceOnMine, 0);
    return 0;
}

s16 Grenade::BlowUpAfterCountdown()
{
    field_112_explode_timer--;
    const s16 timer = field_112_explode_timer;
    if (!(timer % 16))
    {
        SfxPlayMono(relive::SoundEffects::GreenTick, 0);
    }

    if (timer)
    {
        return 0;
    }

    auto pExplosion = relive_new Explosion(
        mXPos,
        mYPos - (mSpriteScale * FP_FromInteger(5)),
        mSpriteScale);
    if (pExplosion)
    {
        mAnim.mFlags.Clear(AnimFlags::eRender);
        field_11C = pExplosion;
        pExplosion->mBaseGameObjectRefCount++;
        field_110_state = States::eWaitForExplodeEnd_6;
    }

    relive_new Gibs(GibType::Metal_5, mXPos, mYPos, FP_FromInteger(0), FP_FromInteger(5), mSpriteScale);
    return 1;
}

s16 Grenade::VCanThrow()
{
    return FALSE;
}

s16 Grenade::VIsFalling()
{
    return FALSE;
}

} // namespace AO
