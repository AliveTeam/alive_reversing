#include "stdafx.h"
#include "Grenade.hpp"
#include "Function.hpp"
#include "ThrowableArray.hpp"
#include "stdlib.hpp"
#include "PlatformBase.hpp"
#include "Sfx.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "../relive_lib/Events.hpp"
#include "Gibs.hpp"
#include "AirExplosion.hpp"
#include "Grid.hpp"
#include "Map.hpp"
#include "../relive_lib/Collisions.hpp"
#include "Path.hpp"
#include "FixedPoint.hpp"

s32 Grenade::CreateFromSaveState(const u8* pBuffer)
{
    auto pState = reinterpret_cast<const GrenadeSaveState*>(pBuffer);
    auto pGrenade = relive_new Grenade(pState->field_8_xpos, pState->field_C_ypos, pState->field_2A_savedcount, 0, nullptr);

    pGrenade->mBaseGameObjectTlvInfo = pState->field_4_obj_id;

    pGrenade->mXPos = pState->field_8_xpos;
    pGrenade->mYPos = pState->field_C_ypos;

    pGrenade->mCollectionRect.x = pGrenade->mXPos - (ScaleToGridSize(pGrenade->GetSpriteScale()) / FP_FromInteger(2));
    pGrenade->mCollectionRect.w = pGrenade->mXPos + (ScaleToGridSize(pGrenade->GetSpriteScale()) / FP_FromInteger(2));
    pGrenade->mCollectionRect.h = pGrenade->mYPos;
    pGrenade->mCollectionRect.y = pGrenade->mYPos - ScaleToGridSize(pGrenade->GetSpriteScale());

    pGrenade->mVelX = pState->field_10_velx;
    pGrenade->mVelY = pState->field_14_vely;
    pGrenade->mCurrentPath = pState->field_1C_path_number;
    pGrenade->mCurrentLevel = pState->field_1E_lvl_number;
    pGrenade->SetSpriteScale(pState->field_18_sprite_scale);

    pGrenade->GetAnimation().SetLoop(pState->mLoop);
    pGrenade->GetAnimation().SetRender(pState->mRender);

    pGrenade->SetDrawable(pState->mDrawable);
    pGrenade->SetInteractive(pState->mInteractive);

    pGrenade->mBaseAliveGameObjectFlags.Set(AliveObjectFlags::eRestoredFromQuickSave);
    pGrenade->BaseAliveGameObjectCollisionLineType = pState->field_28_line_type;
    pGrenade->mBaseThrowableCount = pState->field_2A_savedcount;
    pGrenade->mState = pState->field_2C_state;
    pGrenade->field_124 = pState->field_2E;
    pGrenade->mPreviousXPos = pState->field_34_xpos;
    pGrenade->mPreviousYPos = pState->field_38_ypos;

    pGrenade->mExplodeNow = pState->mExplodeNow;
    pGrenade->mBlowUpOnCollision = pState->mBlowUpOnCollision;

    pGrenade->mExplodeCountdown = pState->field_30_explode_timer;

    return sizeof(GrenadeSaveState);
}

s32 Grenade::VGetSaveState(u8* pSaveBuffer)
{
    auto pState = reinterpret_cast<GrenadeSaveState*>(pSaveBuffer);

    pState->field_0_type = ReliveTypes::eGrenade;

    pState->field_4_obj_id = mBaseGameObjectTlvInfo;

    pState->field_8_xpos = mXPos;
    pState->field_C_ypos = mYPos;
    pState->field_10_velx = mVelX;
    pState->field_14_vely = mVelY;

    pState->field_1C_path_number = mCurrentPath;
    pState->field_1E_lvl_number = mCurrentLevel;
    pState->field_18_sprite_scale = GetSpriteScale();

    pState->mLoop = GetAnimation().GetLoop();
    pState->mDrawable = GetDrawable();
    pState->mRender = GetAnimation().GetRender();
    pState->mInteractive = GetInteractive();

    if (BaseAliveGameObjectCollisionLine)
    {
        pState->field_28_line_type = BaseAliveGameObjectCollisionLine->mLineType;
    }
    else
    {
        pState->field_28_line_type = -1;
    }

    pState->field_24_base_id = BaseAliveGameObject_PlatformId;
    pState->field_2A_savedcount = mBaseThrowableCount;
    pState->field_2C_state = mState;
    pState->field_2E = field_124;
    pState->field_34_xpos = mPreviousXPos;
    pState->field_38_ypos = mPreviousYPos;

    pState->mExplodeNow = mExplodeNow;
    pState->mBlowUpOnCollision = mBlowUpOnCollision;

    pState->field_30_explode_timer = mExplodeCountdown;

    return sizeof(GrenadeSaveState);
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

    GetAnimation().SetRenderMode(TPageAbr::eBlend_0);

    mExplosionId = Guid{};
    mXPos = xpos;
    mYPos = ypos;

    mPreviousXPos = xpos;
    mPreviousYPos = ypos;

    mVelX = FP_FromInteger(0);
    mVelY = FP_FromInteger(0);
    mBaseThrowableCount = 0;
    field_124 = 0;
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
    if (gMap.mCurrentLevel != gMap.mNextLevel || gMap.mCurrentPath != gMap.mNextPath)
    {
        SetDead(true);
    }
    mExplosionId = Guid{};
}

void Grenade::VUpdate()
{
    auto pExplosion = sObjectIds.Find_Impl(mExplosionId);


    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }

    if (mBaseAliveGameObjectFlags.Get(AliveObjectFlags::eRestoredFromQuickSave))
    {
        mBaseAliveGameObjectFlags.Clear(AliveObjectFlags::eRestoredFromQuickSave);
        if (BaseAliveGameObjectCollisionLineType == -1)
        {
            BaseAliveGameObjectCollisionLine = nullptr;
        }
        else
        {
            sCollisions->Raycast(
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
            else if (abs(SnapToXGrid(GetSpriteScale(), FP_GetExponent(mXPos)) - FP_GetExponent(mXPos)) > 1)
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

s16 Grenade::InTheAir(s16 blowUpOnFloorTouch)
{
    sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId);

    mPreviousXPos = mXPos;
    mPreviousYPos = mYPos;

    mVelY += FP_FromInteger(1);

    mXPos += mVelX;
    mYPos += mVelY;

    // Kill the nade if it hits a death drop
    BaseAliveGameObjectPathTLV = sPathInfo->TlvGetAt(
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

        BaseAliveGameObjectPathTLV = sPathInfo->TlvGetAt(
            BaseAliveGameObjectPathTLV,
            mXPos,
            mYPos,
            mXPos,
            mYPos);
    }

    FP hitX = {};
    FP hitY = {};
    if (sCollisions->Raycast(
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
            s16 vol = 75 - 20 * field_124;
            if (vol < 40)
            {
                vol = 40;
            }
            SfxPlayMono(relive::SoundEffects::GrenadeBounce, vol);
            EventBroadcast(kEventNoise, this);
            EventBroadcast(kEventSuspiciousNoise, this);
            EventBroadcast(kEventSpeaking, this);
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

            if (field_124 <= 4)
            {
                s16 vol = 75 - 20 * field_124;
                if (vol < 40)
                {
                    vol = 40;
                }

                SfxPlayMono(relive::SoundEffects::GrenadeBounce, vol);
                field_124++;

                EventBroadcast(kEventNoise, this);
                EventBroadcast(kEventSuspiciousNoise, this);
                EventBroadcast(kEventSpeaking, this);
            }
        }
    }

    if (sCollisions->Raycast(
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
                    s16 vol = 75 - 20 * field_124;
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
                    s16 vol = 75 - 20 * field_124;
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
    if (!pHit->GetCanExplode())
    {
        return 1;
    }

    auto pHit2 = static_cast<BaseAliveGameObject*>(pHit);
    if (pHit2->GetSpriteScale() != GetSpriteScale())
    {
        return 1;
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
    return 0;
}

s16 Grenade::TimeToBlowUp()
{
    mExplodeCountdown--;
    const s16 timer = mExplodeCountdown;
    if (!(timer % 16))
    {
        SfxPlayMono(relive::SoundEffects::GreenTick, 0);
    }

    if (timer)
    {
        return 0;
    }

    BlowUp(0);
    return 1;
}

void Grenade::BlowUp(s16 bSmallExplosion)
{
    auto pExplosion = relive_new AirExplosion(
        mXPos,
        mYPos - (GetSpriteScale() * FP_FromInteger(5)),
        GetSpriteScale(),
        bSmallExplosion);
    if (pExplosion)
    {
        mExplosionId = pExplosion->mBaseGameObjectId;
    }

    GetAnimation().SetRender(false);

    mState = GrenadeStates::eWaitForExplodeEnd_6;

    relive_new Gibs(GibType::Metal_5, mXPos, mYPos, FP_FromInteger(0), FP_FromInteger(5), GetSpriteScale(), bSmallExplosion);
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
        if (gpThrowableArray)
        {
            gpThrowableArray->Remove(mBaseThrowableCount >= 1 ? mBaseThrowableCount : 1);
        }
    }
}

s16 Grenade::OnCollision_InstantExplode(BaseGameObject* pHit)
{
    if (pHit == mGrenadeOwner)
    {
        // Don't do anything if hit the person who threw it
        return 1;
    }

    if (pHit->GetCanExplode() && static_cast<BaseAliveGameObject*>(pHit)->GetSpriteScale() == GetSpriteScale())
    {
        mExplodeNow = true;
        return 0;
    }

    return 1;
}
