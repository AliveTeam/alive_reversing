#include "stdafx_ao.h"
#include "../relive_lib/Function.hpp"
#include "Grenade.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "ThrowableArray.hpp"
#include "../relive_lib/GameObjects/PlatformBase.hpp"
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
#include "../relive_lib/GameType.hpp"

namespace AO {

bool gInfiniteGrenades = false;

Grenade::Grenade(FP xpos, FP ypos, s16 numGrenades, bool bBlowUpOnCollision, BaseGameObject* pOwner)
{
    mBaseThrowableDead = 0;
    
    Init(xpos, ypos);

    mBaseThrowableCount = numGrenades;
    mBlowUpOnCollision = bBlowUpOnCollision;

    if (bBlowUpOnCollision)
    {
        mState = GrenadeStates::eDoesNothing_8;
        mBaseThrowableDead = 1;
    }
    else if (numGrenades > 0)
    {
        mState = GrenadeStates::eFallingToBeCollected_0;
    }
    else
    {
        mState = GrenadeStates::eCountingDown_3;
        mExplodeCountdown = 90;
    }

    mGrenadeOwner = pOwner;
}

void Grenade::Init(FP xpos, FP ypos)
{
    SetType(ReliveTypes::eGrenade);

    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Grenade));
    Animation_Init(GetAnimRes(AnimId::Grenade));

    SetInteractive(false);

    GetAnimation().SetRender(false);
    GetAnimation().SetSemiTrans(true);

    GetAnimation().SetBlendMode(relive::TBlendModes::eBlend_0);

    mXPos = xpos;
    mYPos = ypos;

    mPreviousXPos = xpos;
    mPreviousYPos = ypos;

    mVelX = FP_FromInteger(0);
    mVelY = FP_FromInteger(0);
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

    if (!mBlowUpOnCollision)
    {
        if (mBaseThrowableCount == 0)
        {
            mState = GrenadeStates::eFalling_4;
        }
    }
    else
    {
        mState = GrenadeStates::eFallingBlowUpOnGround_9;
    }
}

void Grenade::VScreenChanged()
{
    if (gMap.LevelChanged() || gMap.PathChanged())
    {
        SetDead(true);
    }
        mExplosionId = Guid{};
}

void Grenade::VUpdate()
{
    if (EventGet(Event::kEventDeathReset))
    {
        SetDead(true);
    }

    if (GetRestoredFromQuickSave())
    {
        SetRestoredFromQuickSave(false);
        if (BaseAliveGameObjectCollisionLineType == -1)
        {
            BaseAliveGameObjectCollisionLine = nullptr;
        }
        else
        {
            gCollisions->Raycast(
                mXPos,
                mYPos - FP_FromInteger(10),
                mXPos,
                mYPos + FP_FromInteger(10),
                &BaseAliveGameObjectCollisionLine,
                &mXPos,
                &mYPos,
                CollisionMask(static_cast<eLineTypes>(BaseAliveGameObjectCollisionLineType)));
        }
        BaseAliveGameObjectCollisionLineType = 0;
    }

    switch (mState)
    {
        case GrenadeStates::eFallingToBeCollected_0:
            if (!InTheAir(false))
            {
                mCollectionRect.x = mXPos - (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(2));
                mCollectionRect.y = mYPos - ScaleToGridSize(GetSpriteScale());
                mCollectionRect.w = mXPos + (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(2));
                mCollectionRect.h = mYPos;

	            SetInteractive(true);
                mState = GrenadeStates::eWaitToBeCollected_1;
            }
            break;

        case GrenadeStates::eWaitToBeCollected_1:
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
                    mState = GrenadeStates::eFallingToBeCollected_0;
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
                    mState = GrenadeStates::eFalling_4;
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
                mState = GrenadeStates::eDoesNothing_2;
            }
            break;

        case GrenadeStates::eDoesNothing_2:
            break;

        case GrenadeStates::eCountingDown_3:
            TimeToBlowUp();
            break;

        case GrenadeStates::eFalling_4:
            if (InTheAir(false))
            {
                if (!TimeToBlowUp())
                {
                    const PSX_RECT bRect = VGetBoundingRect();

                    const PSX_Point xy = {bRect.x, static_cast<s16>(bRect.y + 5)};
                    const PSX_Point wh = {bRect.w, static_cast<s16>(bRect.h + 5)};
                    mDoBounceOff = true;
                    OnCollisionWith(xy, wh, gBaseGameObjects);
                }
            }
            else
            {
                mState = GrenadeStates::eHitGround_5;
            }
            break;

        case GrenadeStates::eHitGround_5:
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
                mState = GrenadeStates::eFalling_4;
            }

            TimeToBlowUp();
            break;
        }

        case GrenadeStates::eWaitForExplodeEnd_6:
        {
            auto pExplosion = sObjectIds.Find_Impl(mExplosionId);
            if (!pExplosion || pExplosion->GetDead())
            {
                mState = GrenadeStates::eExploded_7;
                mExplosionId = Guid{};
            }
            break;
        }

        case GrenadeStates::eExploded_7:
            SetDead(true);
            break;

        case GrenadeStates::eDoesNothing_8:
            break;

        case GrenadeStates::eFallingBlowUpOnGround_9:
        {
            InTheAir(true);

            const PSX_RECT bRect = VGetBoundingRect();

            const PSX_Point xy = {bRect.x, static_cast<s16>(bRect.y + 5)};
            const PSX_Point wh = {bRect.w, static_cast<s16>(bRect.h + 5)};
            mDoBounceOff = false;
            OnCollisionWith(xy, wh, gBaseGameObjects);

            if (mExplodeNow)
            {
                BlowUp(true);
            }
        }
        break;

        default:
            return;
    }
}

bool Grenade::InTheAir(bool blowUpOnFloorTouch)
{
    mPreviousXPos = mXPos;
    mPreviousYPos = mYPos;

    mVelY += FP_FromInteger(1);

    mXPos += mVelX;
    mYPos += mVelY;

    if (GetGameType() == GameType::eAo)
    {
        u16 result = 0;
        mXPos = CamX_VoidSkipper(mXPos, mVelX, 8, &result);
        mYPos = CamY_VoidSkipper(mYPos, mVelY, 8, &result);
    }

    // Kill the nade if it hits a death drop
    BaseAliveGameObjectPathTLV = GetMap().TLV_Get_At(
        nullptr,
        mXPos,
        mYPos,
        mXPos,
        mYPos);

    while (BaseAliveGameObjectPathTLV)
    {
        if (BaseAliveGameObjectPathTLV->mTlvType == ReliveTypes::eDeathDrop)
        {
            SetDead(true);
            return true;
        }

        BaseAliveGameObjectPathTLV = GetMap().TLV_Get_At(
            BaseAliveGameObjectPathTLV,
            mXPos,
            mYPos,
            mXPos,
            mYPos);
    }

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
        PerGameScale() == Scale::Fg ? kFgFloor : kBgFloor) == 1)
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

            if (blowUpOnFloorTouch)
            {
                mExplodeNow = true;
                return true;
            }

            if (mBounceCount <= 4)
            {
                s16 vol = 75 - 20 * mBounceCount;
                if (vol < 40)
                {
                    vol = 40;
                }

                SfxPlayMono(relive::SoundEffects::GrenadeBounce, vol);
                mBounceCount++;

                EventBroadcast(Event::kEventNoise, this);
                EventBroadcast(Event::kEventSuspiciousNoise, this);
                EventBroadcast(Event::kEventSpeaking, this);
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
        PerGameScale() == Scale::Fg ? kFgWalls : kBgWalls) == 1)
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
                    EventBroadcast(Event::kEventNoise, this);
                    EventBroadcast(Event::kEventSuspiciousNoise, this);
                    EventBroadcast(Event::kEventSpeaking, this);
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
                    EventBroadcast(Event::kEventNoise, this);
                    EventBroadcast(Event::kEventSuspiciousNoise, this);
                    EventBroadcast(Event::kEventSpeaking, this);
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

    if (mPreviousXPos < FP_FromInteger(bRect.x + 12) || mPreviousXPos > FP_FromInteger(bRect.w - 12))
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

bool Grenade::TimeToBlowUp()
{
    mExplodeCountdown--;
    const s16 timer = mExplodeCountdown;
    if (!(timer % 16))
    {
        SfxPlayMono(relive::SoundEffects::GreenTick, 0);
    }

    if (timer == 0)
    {
        BlowUp(false);
        return true;
    }

    return false;
}

void Grenade::BlowUp(bool bSmallExplosion)
{
    auto pExplosion = relive_new AirExplosion(
        mXPos,
        mYPos - (GetSpriteScale() * FP_FromInteger(5)),
        GetSpriteScale(),
        bSmallExplosion);
    if (pExplosion)
    {
        mExplosionId = pExplosion->mBaseGameObjectId;
        GetAnimation().SetRender(false);
        mState = GrenadeStates::eWaitForExplodeEnd_6;
    }

    relive_new Gibs(GibType::eMetal, mXPos, mYPos, FP_FromInteger(0), FP_FromInteger(5), GetSpriteScale(), bSmallExplosion);
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

        if (mState == GrenadeStates::eWaitToBeCollected_1 || mState == GrenadeStates::eDoesNothing_2)
        {
            mState = GrenadeStates::eFallingToBeCollected_0;
        }
        else if (mState != GrenadeStates::eWaitForExplodeEnd_6)
        {
            mState = GrenadeStates::eFalling_4;
        }
    }
}

Grenade::~Grenade()
{
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

bool Grenade::OnCollision_InstantExplode(BaseGameObject* pHit)
{
    if (pHit == mGrenadeOwner)
    {
        // Don't do anything if hit the person who threw it
        return true;
    }

    if (pHit->GetCanExplode() && static_cast<BaseAliveGameObject*>(pHit)->GetSpriteScale() == GetSpriteScale())
    {
        mExplodeNow = true;
        return false;
    }

    return true;
}

} // namespace AO
