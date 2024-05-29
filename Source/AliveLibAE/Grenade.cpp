#include "stdafx.h"
#include "Grenade.hpp"
#include "../relive_lib/Function.hpp"
#include "ThrowableArray.hpp"
#include "stdlib.hpp"
#include "../relive_lib/GameObjects/PlatformBase.hpp"
#include "Sfx.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "../relive_lib/Events.hpp"
#include "../relive_lib/GameObjects/Gibs.hpp"
#include "../relive_lib/GameObjects/AirExplosion.hpp"
#include "../relive_lib/Grid.hpp"
#include "Map.hpp"
#include "../relive_lib/Collisions.hpp"
#include "Path.hpp"
#include "../relive_lib/FixedPoint.hpp"
#include "QuikSave.hpp"

void Grenade::CreateFromSaveState(SerializedObjectData& pBuffer)
{
    const auto pState = pBuffer.ReadTmpPtr<GrenadeSaveState>();
    auto pGrenade = relive_new Grenade(pState->mXPos, pState->mYPos, pState->mThrowableCount, 0, nullptr);

    pGrenade->mBaseGameObjectTlvInfo = pState->mTlvInfo;

    pGrenade->mXPos = pState->mXPos;
    pGrenade->mYPos = pState->mYPos;

    pGrenade->mCollectionRect.x = pGrenade->mXPos - (ScaleToGridSize(pGrenade->GetSpriteScale()) / FP_FromInteger(2));
    pGrenade->mCollectionRect.w = pGrenade->mXPos + (ScaleToGridSize(pGrenade->GetSpriteScale()) / FP_FromInteger(2));
    pGrenade->mCollectionRect.h = pGrenade->mYPos;
    pGrenade->mCollectionRect.y = pGrenade->mYPos - ScaleToGridSize(pGrenade->GetSpriteScale());

    pGrenade->mVelX = pState->mVelX;
    pGrenade->mVelY = pState->mVelY;
    pGrenade->mCurrentPath = pState->mCurrentPath;
    pGrenade->mCurrentLevel = pState->mCurrentLevel;
    pGrenade->SetSpriteScale(pState->mSpriteScale);

    pGrenade->GetAnimation().SetLoop(pState->mLoop);
    pGrenade->GetAnimation().SetRender(pState->mRender);

    pGrenade->SetDrawable(pState->mDrawable);
    pGrenade->SetInteractive(pState->mInteractive);

    pGrenade->SetRestoredFromQuickSave(true);
    pGrenade->BaseAliveGameObjectCollisionLineType = pState->mCollisionLineType;
    pGrenade->mBaseThrowableCount = pState->mThrowableCount;
    pGrenade->mState = pState->mState;
    pGrenade->mBounceCount = pState->mBounceCount;
    pGrenade->mPreviousXPos = pState->mPreviousXPos;
    pGrenade->mPreviousYPos = pState->mPreviousYPos;

    pGrenade->mExplodeNow = pState->mExplodeNow;
    pGrenade->mBlowUpOnCollision = pState->mBlowUpOnCollision;

    pGrenade->mExplodeCountdown = pState->mExplodeCountdown;
}

void Grenade::VGetSaveState(SerializedObjectData& pSaveBuffer)
{
    GrenadeSaveState data = {};

    data.mType = ReliveTypes::eGrenade;

    data.mTlvInfo = mBaseGameObjectTlvInfo;

    data.mXPos = mXPos;
    data.mYPos = mYPos;
    data.mVelX = mVelX;
    data.mVelY = mVelY;

    data.mCurrentPath = mCurrentPath;
    data.mCurrentLevel = mCurrentLevel;
    data.mSpriteScale = GetSpriteScale();

    data.mLoop = GetAnimation().GetLoop();
    data.mDrawable = GetDrawable();
    data.mRender = GetAnimation().GetRender();
    data.mInteractive = GetInteractive();

    if (BaseAliveGameObjectCollisionLine)
    {
        data.mCollisionLineType = BaseAliveGameObjectCollisionLine->mLineType;
    }
    else
    {
        data.mCollisionLineType = eLineTypes::eNone_m1;
    }

    data.mPlatformId = BaseAliveGameObject_PlatformId;
    data.mThrowableCount = mBaseThrowableCount;
    data.mState = mState;
    data.mBounceCount = mBounceCount;
    data.mPreviousXPos = mPreviousXPos;
    data.mPreviousYPos = mPreviousYPos;

    data.mExplodeNow = mExplodeNow;
    data.mBlowUpOnCollision = mBlowUpOnCollision;

    data.mExplodeCountdown = mExplodeCountdown;

    pSaveBuffer.Write(data);
}

Grenade::Grenade(FP xpos, FP ypos, s32 numGrenades, bool bBlowUpOnCollision, BaseGameObject* pOwner)
    : BaseThrowable(0)
{
    mBaseThrowableDead = 0;

    Init(xpos, ypos);

    mBaseThrowableCount = static_cast<s16>(numGrenades);
    mBlowUpOnCollision = bBlowUpOnCollision;

    if (bBlowUpOnCollision)
    {
        mState = GrenadeStates::eDoesNothing_8;
        mBaseThrowableDead = 1;
    }
    else if (numGrenades)
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
    GetAnimation().SetSemiTrans(false);

    GetAnimation().SetBlendMode(relive::TBlendModes::eBlend_0);

    mExplosionId = Guid{};
    mXPos = xpos;
    mYPos = ypos;

    mPreviousXPos = xpos;
    mPreviousYPos = ypos;

    mVelX = FP_FromInteger(0);
    mVelY = FP_FromInteger(0);
    mBaseThrowableCount = 0;
    mBounceCount = 0;
    mExplodeNow = false;
}


void Grenade::AddToPlatform()
{
    BaseAddToPlatform([](ReliveTypes type)
                      { return type == ReliveTypes::eLiftPoint || type == ReliveTypes::eTrapDoor; });
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
    auto pExplosion = sObjectIds.Find_Impl(mExplosionId);


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

                auto oldLine = BaseAliveGameObjectCollisionLine;
                BaseAliveGameObjectCollisionLine = BaseAliveGameObjectCollisionLine->MoveOnLine(&mXPos, &mYPos, mVelX);
                if (BaseAliveGameObject_PlatformId != Guid{} && BaseAliveGameObjectCollisionLine != oldLine)
                {
                    VOnTrapDoorOpen();
                }

                if (!BaseAliveGameObjectCollisionLine)
                {
                    GetAnimation().SetLoop(true);
                    mState = GrenadeStates::eFallingToBeCollected_0;
                }
            }
            else if (abs(SnapToXGrid_AE(GetSpriteScale(), FP_GetExponent(mXPos)) - FP_GetExponent(mXPos)) > 1)
            {
                auto oldLine = BaseAliveGameObjectCollisionLine;
                BaseAliveGameObjectCollisionLine = BaseAliveGameObjectCollisionLine->MoveOnLine(&mXPos, &mYPos, mVelX);
                if (BaseAliveGameObject_PlatformId != Guid{} && BaseAliveGameObjectCollisionLine != oldLine)
                {
                    VOnTrapDoorOpen();
                }

                if (!BaseAliveGameObjectCollisionLine)
                {
                    GetAnimation().SetLoop(true);
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
            mVelX = FP_FromRaw(mVelX.fpValue / 2);

            BaseAliveGameObjectCollisionLine = BaseAliveGameObjectCollisionLine->MoveOnLine(&mXPos, &mYPos, mVelX);
            if (!BaseAliveGameObjectCollisionLine)
            {
                GetAnimation().SetLoop(true);
                mState = GrenadeStates::eFalling_4;
            }

            TimeToBlowUp();
            break;

        case GrenadeStates::eWaitForExplodeEnd_6:
            if (!pExplosion || pExplosion->GetDead())
            {
                mState = GrenadeStates::eExploded_7;
                mExplosionId = Guid{};
            }
            break;

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
    }
}

bool Grenade::InTheAir(s16 blowUpOnFloorTouch)
{
    sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId);

    mPreviousXPos = mXPos;
    mPreviousYPos = mYPos;

    mVelY += FP_FromInteger(1);

    mXPos += mVelX;
    mYPos += mVelY;

    // Kill the nade if it hits a death drop
    BaseAliveGameObjectPathTLV = gPathInfo->TLV_Get_At(
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
            return 1;
        }

        BaseAliveGameObjectPathTLV = gPathInfo->TLV_Get_At(
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
            GetScale() == Scale::Fg ? kFgFloor : kBgFloor) == 1)
    {
        if (mVelY <= FP_FromInteger(0))
        {
            mXPos = hitX;
            mYPos = hitY;
            mVelY = (-mVelY / FP_FromInteger(2));
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
        else
        {
            if (mVelY < FP_FromInteger(1))
            {
                if (BaseAliveGameObject_PlatformId == Guid{})
                {
                    AddToPlatform();
                }
                return 0;
            }

            mXPos = hitX;
            mYPos = hitY;
            mVelY = (-mVelY / FP_FromInteger(2));
            mVelX = (mVelX / FP_FromInteger(2));

            if (blowUpOnFloorTouch)
            {
                mExplodeNow = true;
                return 1;
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
            GetScale() == Scale::Fg ? kFgWalls : kBgWalls) == 1)
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
                return 1;
        }
    }

    return 1;
}

bool Grenade::OnCollision_BounceOff(BaseGameObject* pHit)
{
    if (!pHit->GetCanExplode())
    {
        return true;
    }

    auto pHit2 = static_cast<BaseAliveGameObject*>(pHit);
    if (pHit2->GetSpriteScale() != GetSpriteScale())
    {
        return true;
    }

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
    if (!gInfiniteThrowables && !mBaseThrowableDead)
    {
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
