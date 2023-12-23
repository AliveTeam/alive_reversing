#include "stdafx_ao.h"
#include "../relive_lib/Function.hpp"
#include "Grenade.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "ThrowableArray.hpp"
#include "PlatformBase.hpp"
#include "Map.hpp"
#include "Math.hpp"
#include "Sfx.hpp"
#include "../relive_lib/GameObjects/AirExplosion.hpp"
#include "../relive_lib/GameObjects/Gibs.hpp"
#include "../relive_lib/Events.hpp"
#include "../relive_lib/Collisions.hpp"
#include "../relive_lib/Grid.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "../relive_lib/FixedPoint.hpp"

namespace AO {

bool gInfiniteGrenades = false;

Grenade::Grenade(FP xpos, FP ypos, s16 numGrenades)
    : BaseThrowable()
{
    mBaseThrowableDead = 0;
    SetType(ReliveTypes::eGrenade);

    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Grenade));
    Animation_Init(GetAnimRes(AnimId::Grenade));

    SetInteractive(false);
    GetAnimation().SetBlendMode(relive::TBlendModes::eBlend_0);

    GetAnimation().SetRender(false);
    GetAnimation().SetSemiTrans(true);

    mXPos = xpos;
    mYPos = ypos;

    mPreviousXPos = xpos;
    mPreviousYPos = ypos;

    mVelX = FP_FromInteger(0);
    mVelY = FP_FromInteger(0);
    mBaseThrowableCount = numGrenades;

    if (numGrenades > 0)
    {
        mState = States::eFallingToBeCollected_0;
    }
    else
    {
        mState = States::eCountingDown_3;
        mExplodeCountdown = 90;
    }

    mBounceCount = 0;
    mExplosionObj = 0;
}


void Grenade::AddToPlatform()
{
    BaseAddToPlatform();
}

void Grenade::VTimeToExplodeRandom()
{
    mExplodeCountdown -= Math_NextRandom() % 16;
}

void Grenade::VThrow(FP velX, FP velY)
{
    GetAnimation().SetRender(true);

    mVelX = velX;
    mVelY = velY;

    if (mBaseThrowableCount == 0)
    {
        mState = States::eFalling_4;
    }
}

void Grenade::VScreenChanged()
{
    if (gMap.LevelChanged() || gMap.PathChanged())
    {
        SetDead(true);
    }
}

void Grenade::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }

    switch (mState)
    {
        case States::eFallingToBeCollected_0:
            if (!InTheAir())
            {
                mCollectionRect.x = mXPos - (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(2));
                mCollectionRect.y = mYPos - ScaleToGridSize(GetSpriteScale());
                mCollectionRect.w = mXPos + (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(2));
                mCollectionRect.h = mYPos;

	            SetInteractive(true);
                mState = States::eWaitToBeCollected_1;
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

                const auto oldLine = BaseAliveGameObjectCollisionLine;
                BaseAliveGameObjectCollisionLine = BaseAliveGameObjectCollisionLine->MoveOnLine(&mXPos, &mYPos, mVelX);
                if (BaseAliveGameObject_PlatformId != Guid{} && BaseAliveGameObjectCollisionLine != oldLine)
                {
                    auto pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
                    pPlatform->VRemove(this);
                    BaseAliveGameObject_PlatformId = Guid{};
                }

                if (!BaseAliveGameObjectCollisionLine)
                {
                    mState = States::eFallingToBeCollected_0;
                }
            }
            else if (abs(SnapToXGrid_AO(GetSpriteScale(), FP_GetExponent(mXPos)) - FP_GetExponent(mXPos)) > 1)
            {
                const auto oldLine = BaseAliveGameObjectCollisionLine;
                BaseAliveGameObjectCollisionLine = BaseAliveGameObjectCollisionLine->MoveOnLine(&mXPos, &mYPos, mVelX);
                if (BaseAliveGameObject_PlatformId != Guid{} && BaseAliveGameObjectCollisionLine != oldLine)
                {
                    if (BaseAliveGameObjectCollisionLine != oldLine)
                    {
                       auto pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
                       pPlatform->VRemove(this);
                       BaseAliveGameObject_PlatformId = Guid{};
                    }
                }

                if (!BaseAliveGameObjectCollisionLine)
                {
                    mState = States::eFalling_4;
                }
            }
            else
            {
                mVelX = FP_FromInteger(0);

                mCollectionRect.x = mXPos - (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(2));
                mCollectionRect.y = mYPos - ScaleToGridSize(GetSpriteScale());
                mCollectionRect.w = mXPos + (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(2));
                mCollectionRect.h = mYPos;

                SetInteractive(true);
                mState = States::eDoesNothing_2;
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

                    OnCollisionWith(xy, wh, gBaseGameObjects);
                }
            }
            else
            {
                mState = States::eHitGround_5;
            }
            break;

        case States::eHitGround_5:
        {
            mVelX = FP_FromRaw(mVelX.fpValue / 2);

            const auto oldLine = BaseAliveGameObjectCollisionLine;
            BaseAliveGameObjectCollisionLine = BaseAliveGameObjectCollisionLine->MoveOnLine(&mXPos, &mYPos, mVelX);
            auto pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
            if (pPlatform)
            {
                if (BaseAliveGameObjectCollisionLine != oldLine)
                {
                    pPlatform->VRemove(this);
                    BaseAliveGameObject_PlatformId = Guid{};
                }
            }

            if (!BaseAliveGameObjectCollisionLine)
            {
                mState = States::eFalling_4;
            }

            BlowUpAfterCountdown();
            break;
        }

        case States::eWaitForExplodeEnd_6:
            if (mExplosionObj->GetDead())
            {
                mState = States::eExploded_7;
                mExplosionObj->mBaseGameObjectRefCount--;
                mExplosionObj = nullptr;
            }
            break;

        case States::eExploded_7:
            SetDead(true);
            break;

        default:
            return;
    }
}


bool Grenade::InTheAir()
{
    mPreviousXPos = mXPos;
    mPreviousYPos = mYPos;

    mVelY += FP_FromInteger(1);

    mXPos += mVelX;
    mYPos += mVelY;

    u16 result = 0;
    mXPos = CamX_VoidSkipper(mXPos, mVelX, 8, &result);
    mYPos = CamY_VoidSkipper(mYPos, mVelY, 8, &result);

    FP hitX = {};
    FP hitY = {};
    if (gCollisions->Raycast(
        mPreviousXPos,
        mPreviousYPos,
        mXPos,
        mYPos,
        &BaseAliveGameObjectCollisionLine,
        &hitX,
        &hitY,
        GetSpriteScale() != FP_FromDouble(0.5) ? kFgFloor : kBgFloor) == 1)
    {
        if (mVelY > FP_FromInteger(0))
        {
            if (mVelY < FP_FromInteger(1))
            {
                if (BaseAliveGameObject_PlatformId == Guid{})
                {
                    AddToPlatform();
                }
                return false;
            }

            mXPos = hitX;
            mYPos = hitY;
            mVelY = (-mVelY / FP_FromInteger(2));
            mVelX = (mVelX / FP_FromInteger(2));
            if (mBounceCount <= 4)
            {
                s16 vol = 75 - 20 * mBounceCount;
                if (vol < 40)
                {
                    vol = 40;
                }

                SfxPlayMono(relive::SoundEffects::GrenadeBounce, vol);
                mBounceCount++;

                EventBroadcast(kEventNoise, this);
                EventBroadcast(kEventSuspiciousNoise, this);
                EventBroadcast(kEventSpeaking, this);
            }
        }
    }

    if (gCollisions->Raycast(
        mPreviousXPos,
        mPreviousYPos,
        mXPos,
        mYPos,
        &BaseAliveGameObjectCollisionLine,
        &hitX,
        &hitY,
        GetSpriteScale() != FP_FromDouble(0.5) ? kFgWalls : kBgWalls) == 1)
    {
        switch (BaseAliveGameObjectCollisionLine->mLineType)
        {
            case eLineTypes::eWallLeft_1:
            case eLineTypes::eBackgroundWallLeft_5:
                if (mVelX < FP_FromInteger(0))
                {
                    mYPos = hitY;
                    mXPos = hitX;
                    mVelX = (-mVelX / FP_FromInteger(2));
                    s16 vol = 75 - 20 * mBounceCount;
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

            case eLineTypes::eWallRight_2:
            case eLineTypes::eBackgroundWallRight_6:
                if (mVelX > FP_FromInteger(0))
                {
                    mXPos = hitX;
                    mYPos = hitY;
                    mVelX = (-mVelX / FP_FromInteger(2));
                    s16 vol = 75 - 20 * mBounceCount;
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
                return true;
        }
    }

    return true;
}

bool Grenade::OnCollision_BounceOff(BaseGameObject* pHit)
{
    if (!pHit->GetCanExplode())
    {
        return true;
    }

    auto pHit2 = static_cast<BaseAliveGameObject*>(pHit);

    const PSX_RECT bRect = pHit2->VGetBoundingRect();

    if (mXPos < FP_FromInteger(bRect.x + 12) || mXPos > FP_FromInteger(bRect.w - 12))
    {
        mXPos = mPreviousXPos;
        mVelX = (-mVelX / FP_FromInteger(2));
    }
    else
    {
        mYPos = mPreviousYPos;
        mVelY = (-mVelY / FP_FromInteger(2));
    }

    pHit2->VOnThrowableHit(this);

    SfxPlayMono(relive::SoundEffects::RockBounceOnMine, 0);
    return false;
}

bool Grenade::BlowUpAfterCountdown()
{
    mExplodeCountdown--;
    const s16 timer = mExplodeCountdown;
    if (!(timer % 16))
    {
        SfxPlayMono(relive::SoundEffects::GreenTick, 0);
    }

    if (timer)
    {
        return false;
    }

    auto pExplosion = relive_new AirExplosion(
        mXPos,
        mYPos - (GetSpriteScale() * FP_FromInteger(5)),
        GetSpriteScale(),
        false);
    if (pExplosion)
    {
        GetAnimation().SetRender(false);
        mExplosionObj = pExplosion;
        pExplosion->mBaseGameObjectRefCount++;
        mState = States::eWaitForExplodeEnd_6;
    }

    relive_new Gibs(GibType::eMetal, mXPos, mYPos, FP_FromInteger(0), FP_FromInteger(5), GetSpriteScale(), false);
    return true;
}

bool Grenade::VCanThrow()
{
    return false;
}

bool Grenade::VIsFalling()
{
    return false;
}

void Grenade::VOnTrapDoorOpen()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    if (pPlatform)
    {
        pPlatform->VRemove(this);
        BaseAliveGameObject_PlatformId = Guid{};

        if (mState == States::eWaitToBeCollected_1 || mState == States::eDoesNothing_2)
        {
            mState = States::eFallingToBeCollected_0;
        }
        else if (mState != States::eWaitForExplodeEnd_6)
        {
            mState = States::eFalling_4;
        }
    }
}

Grenade::~Grenade()
{
    if (mExplosionObj)
    {
        mExplosionObj->mBaseGameObjectRefCount--;
    }

    if (!gInfiniteGrenades && !mBaseThrowableDead)
    {
        auto pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
        if (pPlatform)
        {
            pPlatform->VRemove(this);
            BaseAliveGameObject_PlatformId = Guid{};
        }

        if (gThrowableArray)
        {
            gThrowableArray->Remove(mBaseThrowableCount >= 1 ? mBaseThrowableCount : 1);
        }
    }
}

} // namespace AO
