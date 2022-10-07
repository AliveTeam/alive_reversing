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
#include "Explosion.hpp"
#include "Grid.hpp"
#include "Map.hpp"

Grenade::Grenade(FP xpos, FP ypos, s32 numGrenades, bool bBlowUpOnCollision, s32 unused, BaseGameObject* pOwner)
    : BaseThrowable(0)
{
    mBaseThrowableDead = 0;

    Init(xpos, ypos);

    mBaseThrowableCount = static_cast<s16>(numGrenades);
    field_132_bBlowUpOnCollision = bBlowUpOnCollision;

    if (bBlowUpOnCollision)
    {
        field_120_state = GrenadeStates::eDoesNothing_8;
        mBaseThrowableDead = 1;
    }
    else if (numGrenades)
    {
        field_120_state = GrenadeStates::eFallingToBeCollected_0;
    }
    else
    {
        field_120_state = GrenadeStates::eCountingDown_3;
        field_122_explode_timer = 90;
    }

    field_138_pOwner = pOwner;
    field_130_unused = static_cast<s16>(unused);
}

s32 Grenade::CreateFromSaveState(const u8* pBuffer)
{
    auto pState = reinterpret_cast<const Grenade_SaveState*>(pBuffer);
    auto pGrenade = relive_new Grenade(pState->field_8_xpos, pState->field_C_ypos, pState->field_2A_savedcount, 0, 0, nullptr);

    pGrenade->mBaseGameObjectTlvInfo = pState->field_4_obj_id;

    pGrenade->mXPos = pState->field_8_xpos;
    pGrenade->mYPos = pState->field_C_ypos;

    pGrenade->mCollectionRect.x = pGrenade->mXPos - (ScaleToGridSize(pGrenade->mSpriteScale) / FP_FromInteger(2));
    pGrenade->mCollectionRect.w = pGrenade->mXPos + (ScaleToGridSize(pGrenade->mSpriteScale) / FP_FromInteger(2));
    pGrenade->mCollectionRect.h = pGrenade->mYPos;
    pGrenade->mCollectionRect.y = pGrenade->mYPos - ScaleToGridSize(pGrenade->mSpriteScale);

    pGrenade->mVelX = pState->field_10_velx;
    pGrenade->mVelY = pState->field_14_vely;
    pGrenade->mCurrentPath = pState->field_1C_path_number;
    pGrenade->mCurrentLevel = MapWrapper::FromAESaveData(pState->field_1E_lvl_number);
    pGrenade->mSpriteScale = pState->field_18_sprite_scale;

    pGrenade->mAnim.mFlags.Set(AnimFlags::eBit8_Loop, pState->field_20_flags.Get(Grenade_SaveState::eBit3_bLoop));
    pGrenade->mAnim.mFlags.Set(AnimFlags::eBit3_Render, pState->field_20_flags.Get(Grenade_SaveState::eBit1_bRender));

    pGrenade->mBaseGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4, pState->field_20_flags.Get(Grenade_SaveState::eBit2_bDrawable));
    pGrenade->mBaseGameObjectFlags.Set(BaseGameObject::eInteractive_Bit8, pState->field_20_flags.Get(Grenade_SaveState::eBit4_bInteractive));

    pGrenade->mBaseAliveGameObjectFlags.Set(Flags_114::e114_Bit9_RestoredFromQuickSave);
    pGrenade->BaseAliveGameObjectCollisionLineType = pState->field_28_line_type;
    pGrenade->mBaseThrowableCount = pState->field_2A_savedcount;
    pGrenade->field_120_state = pState->field_2C_state;
    pGrenade->field_124 = pState->field_2E;
    pGrenade->field_128_xpos = pState->field_34_xpos;
    pGrenade->field_12C_ypos = pState->field_38_ypos;

    pGrenade->field_130_unused = pState->field_20_flags.Get(Grenade_SaveState::eBit5_unused);
    pGrenade->field_134_bExplodeNow = pState->field_20_flags.Get(Grenade_SaveState::eBit6_bExplodeNow);
    pGrenade->field_132_bBlowUpOnCollision = pState->field_20_flags.Get(Grenade_SaveState::eBit7_bBlowUpOnCollision);

    pGrenade->field_122_explode_timer = pState->field_30_explode_timer;

    return sizeof(Grenade_SaveState);
}

s32 Grenade::VGetSaveState(u8* pSaveBuffer)
{
    auto pState = reinterpret_cast<Grenade_SaveState*>(pSaveBuffer);

    pState->field_0_type = AETypes::eGrenade_65;

    pState->field_4_obj_id = mBaseGameObjectTlvInfo;

    pState->field_8_xpos = mXPos;
    pState->field_C_ypos = mYPos;
    pState->field_10_velx = mVelX;
    pState->field_14_vely = mVelY;

    pState->field_1C_path_number = mCurrentPath;
    pState->field_1E_lvl_number = MapWrapper::ToAE(mCurrentLevel);
    pState->field_18_sprite_scale = mSpriteScale;

    pState->field_20_flags.Set(Grenade_SaveState::eBit3_bLoop, mAnim.mFlags.Get(AnimFlags::eBit8_Loop));
    pState->field_20_flags.Set(Grenade_SaveState::eBit2_bDrawable, mBaseGameObjectFlags.Get(BaseGameObject::eDrawable_Bit4));
    pState->field_20_flags.Set(Grenade_SaveState::eBit1_bRender, mAnim.mFlags.Get(AnimFlags::eBit3_Render));
    pState->field_20_flags.Set(Grenade_SaveState::eBit4_bInteractive, mBaseGameObjectFlags.Get(BaseGameObject::eInteractive_Bit8));

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
    pState->field_2C_state = field_120_state;
    pState->field_2E = field_124;
    pState->field_34_xpos = field_128_xpos;
    pState->field_38_ypos = field_12C_ypos;

    pState->field_20_flags.Set(Grenade_SaveState::eBit5_unused, field_130_unused & 1);
    pState->field_20_flags.Set(Grenade_SaveState::eBit6_bExplodeNow, field_134_bExplodeNow & 1);
    pState->field_20_flags.Set(Grenade_SaveState::eBit7_bBlowUpOnCollision, field_132_bBlowUpOnCollision & 1);

    pState->field_30_explode_timer = field_122_explode_timer;

    return sizeof(Grenade_SaveState);
}

void Grenade::VScreenChanged()
{
    if (gMap.mCurrentLevel != gMap.mNextLevel || gMap.mCurrentPath != gMap.mNextPath)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
    field_11C_explosion_id = Guid{};
}

void Grenade::Init(FP xpos, FP ypos)
{
    SetType(ReliveTypes::eGrenade);

    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Grenade));
    Animation_Init(GetAnimRes(AnimId::Grenade));
    mBaseGameObjectFlags.Clear(BaseGameObject::eInteractive_Bit8);

    mAnim.mFlags.Clear(AnimFlags::eBit3_Render);
    mAnim.mFlags.Clear(AnimFlags::eBit15_bSemiTrans);

    mAnim.mRenderMode = TPageAbr::eBlend_0;

    field_11C_explosion_id = Guid{};
    mXPos = xpos;
    mYPos = ypos;
    field_128_xpos = xpos;
    field_12C_ypos = ypos;
    mVelX = FP_FromInteger(0);
    mVelY = FP_FromInteger(0);
    mBaseThrowableCount = 0;
    field_124 = 0;
    field_134_bExplodeNow = FALSE;
}

void Grenade::VOnTrapDoorOpen()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    if (pPlatform)
    {
        pPlatform->VRemove(this);
        BaseAliveGameObject_PlatformId = Guid{};
        if (field_120_state == GrenadeStates::eWaitToBeCollected_1 || field_120_state == GrenadeStates::eDoesNothing_2)
        {
            field_120_state = GrenadeStates::eFallingToBeCollected_0;
        }
        else if (field_120_state != GrenadeStates::eWaitForExplodeEnd_6)
        {
            field_120_state = GrenadeStates::eFalling_4;
        }
    }
}

void Grenade::VThrow(FP velX, FP velY)
{
    mAnim.mFlags.Set(AnimFlags::eBit3_Render);

    mVelX = velX;
    mVelY = velY;

    if (field_132_bBlowUpOnCollision == 0)
    {
        if (mBaseThrowableCount == 0)
        {
            field_120_state = GrenadeStates::eFalling_4;
        }
    }
    else
    {
        field_120_state = GrenadeStates::eFallingBlowUpOnGround_9;
    }
}

bool Grenade::VCanThrow()
{
    return FALSE;
}

bool Grenade::VIsFalling()
{
    return FALSE;
}

void Grenade::VTimeToExplodeRandom()
{
    field_122_explode_timer -= Math_NextRandom() % 16;
}

void Grenade::BlowUp(s16 bSmallExplosion)
{
    auto pExplosion = relive_new Explosion(
        mXPos,
        mYPos - (mSpriteScale * FP_FromInteger(5)),
        mSpriteScale,
        bSmallExplosion);
    if (pExplosion)
    {
        field_11C_explosion_id = pExplosion->mBaseGameObjectId;
    }

    mAnim.mFlags.Clear(AnimFlags::eBit3_Render);

    field_120_state = GrenadeStates::eWaitForExplodeEnd_6;

    relive_new Gibs(GibType::Metal_5, mXPos, mYPos, FP_FromInteger(0), FP_FromInteger(5), mSpriteScale, bSmallExplosion);
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

s16 Grenade::TimeToBlowUp()
{
    field_122_explode_timer--;
    const s16 timer = field_122_explode_timer;
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

void Grenade::VUpdate()
{
    auto pExplosion = sObjectIds.Find_Impl(field_11C_explosion_id);


    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit9_RestoredFromQuickSave))
    {
        mBaseAliveGameObjectFlags.Clear(Flags_114::e114_Bit9_RestoredFromQuickSave);
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

    switch (field_120_state)
    {
        case GrenadeStates::eFallingToBeCollected_0:
            if (!InTheAir(FALSE))
            {
                mCollectionRect.x = mXPos - (ScaleToGridSize(mSpriteScale) / FP_FromInteger(2));
                mCollectionRect.y = mYPos - ScaleToGridSize(mSpriteScale);
                mCollectionRect.w = mXPos + (ScaleToGridSize(mSpriteScale) / FP_FromInteger(2));
                mCollectionRect.h = mYPos;

                mBaseGameObjectFlags.Set(BaseGameObject::eInteractive_Bit8);
                field_120_state = GrenadeStates::eWaitToBeCollected_1;
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
                    mAnim.mFlags.Set(AnimFlags::eBit8_Loop);
                    field_120_state = GrenadeStates::eFallingToBeCollected_0;
                }
            }
            else if (abs(SnapToXGrid(mSpriteScale, FP_GetExponent(mXPos)) - FP_GetExponent(mXPos)) > 1)
            {
                auto oldLine = BaseAliveGameObjectCollisionLine;
                BaseAliveGameObjectCollisionLine = BaseAliveGameObjectCollisionLine->MoveOnLine(&mXPos, &mYPos, mVelX);
                if (BaseAliveGameObject_PlatformId != Guid{} && BaseAliveGameObjectCollisionLine != oldLine)
                {
                    VOnTrapDoorOpen();
                }

                if (!BaseAliveGameObjectCollisionLine)
                {
                    mAnim.mFlags.Set(AnimFlags::eBit8_Loop);
                    field_120_state = GrenadeStates::eFalling_4;
                }
            }
            else
            {
                mVelX = FP_FromInteger(0);

                mCollectionRect.x = mXPos - (ScaleToGridSize(mSpriteScale) / FP_FromInteger(2));
                mCollectionRect.y = mYPos - ScaleToGridSize(mSpriteScale);
                mCollectionRect.w = mXPos + (ScaleToGridSize(mSpriteScale) / FP_FromInteger(2));
                mCollectionRect.h = mYPos;

                mBaseGameObjectFlags.Set(BaseGameObject::eInteractive_Bit8);
                field_120_state = GrenadeStates::eDoesNothing_2;
            }
            break;

        case GrenadeStates::eDoesNothing_2:
            break;

        case GrenadeStates::eCountingDown_3:
            TimeToBlowUp();
            break;

        case GrenadeStates::eFalling_4:
            if (InTheAir(FALSE))
            {
                if (!TimeToBlowUp())
                {
                    const PSX_RECT bRect = VGetBoundingRect();

                    const PSX_Point xy = {bRect.x, static_cast<s16>(bRect.y + 5)};
                    const PSX_Point wh = {bRect.w, static_cast<s16>(bRect.h + 5)};
                    VOnCollisionWith(xy, wh, gBaseGameObjects, (TCollisionCallBack) &Grenade::OnCollision_BounceOff);
                }
            }
            else
            {
                field_120_state = GrenadeStates::eHitGround_5;
            }
            break;

        case GrenadeStates::eHitGround_5:
            mVelX = FP_FromRaw(mVelX.fpValue / 2);

            BaseAliveGameObjectCollisionLine = BaseAliveGameObjectCollisionLine->MoveOnLine(&mXPos, &mYPos, mVelX);
            if (!BaseAliveGameObjectCollisionLine)
            {
                mAnim.mFlags.Set(AnimFlags::eBit8_Loop);
                field_120_state = GrenadeStates::eFalling_4;
            }

            TimeToBlowUp();
            break;

        case GrenadeStates::eWaitForExplodeEnd_6:
            if (!pExplosion || pExplosion->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
            {
                field_120_state = GrenadeStates::eExploded_7;
                field_11C_explosion_id = Guid{};
            }
            break;

        case GrenadeStates::eExploded_7:
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            break;

        case GrenadeStates::eDoesNothing_8:
            break;

        case GrenadeStates::eFallingBlowUpOnGround_9:
        {
            InTheAir(TRUE);

            const PSX_RECT bRect = VGetBoundingRect();

            const PSX_Point xy = {bRect.x, static_cast<s16>(bRect.y + 5)};
            const PSX_Point wh = {bRect.w, static_cast<s16>(bRect.h + 5)};
            VOnCollisionWith(xy, wh, gBaseGameObjects, (TCollisionCallBack) &Grenade::OnCollision_InstantExplode);

            if (field_134_bExplodeNow)
            {
                BlowUp(TRUE);
            }
        }
        break;
    }
}

s16 Grenade::InTheAir(s16 blowUpOnFloorTouch)
{
    sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId);

    field_128_xpos = mXPos;
    field_12C_ypos = mYPos;

    mVelY += FP_FromInteger(1);
    mXPos += mVelX;
    mYPos = mVelY + mYPos;

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
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
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
            field_128_xpos,
            field_12C_ypos,
            mXPos,
            mYPos,
            &BaseAliveGameObjectCollisionLine,
            &hitX,
            &hitY,
            mScale == Scale::Fg ? kFgFloor : kBgFloor)
        == 1)
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
                field_134_bExplodeNow = TRUE;
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
                ++field_124;

                EventBroadcast(kEventNoise, this);
                EventBroadcast(kEventSuspiciousNoise, this);
                EventBroadcast(kEventSpeaking, this);
            }
        }
    }

    if (sCollisions->Raycast(
            field_128_xpos,
            field_12C_ypos,
            mXPos,
            mYPos,
            &BaseAliveGameObjectCollisionLine,
            &hitX,
            &hitY,
            mScale == Scale::Fg ? kFgWalls : kBgWalls)
        == 1)
    {
        switch (BaseAliveGameObjectCollisionLine->mLineType)
        {
            case 1u:
            case 5u:
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

            case 2u:
            case 6u:
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

void Grenade::AddToPlatform()
{
    BaseAddToPlatform([](ReliveTypes type)
                      { return type == ReliveTypes::eLiftPoint || type == ReliveTypes::eTrapDoor; });
}

s16 Grenade::OnCollision_BounceOff(BaseGameObject* pHit)
{
    if (!pHit->mBaseGameObjectFlags.Get(BaseGameObject::eCanExplode_Bit7))
    {
        return 1;
    }

    auto pHit2 = static_cast<BaseAliveGameObject*>(pHit);
    if (pHit2->mSpriteScale != mSpriteScale)
    {
        return 1;
    }

    const PSX_RECT bRect = pHit2->VGetBoundingRect();

    if (field_128_xpos < FP_FromInteger(bRect.x + 12) || field_128_xpos > FP_FromInteger(bRect.w - 12))
    {
        mXPos = field_128_xpos;
        mVelX = (-mVelX / FP_FromInteger(2));
    }
    else
    {
        mYPos = field_12C_ypos;
        mVelY = (-mVelY / FP_FromInteger(2));
    }

    pHit2->VOnThrowableHit(this);

    SfxPlayMono(relive::SoundEffects::RockBounceOnMine, 0);
    return 0;
}

s16 Grenade::OnCollision_InstantExplode(BaseGameObject* pHit)
{
    if (pHit == field_138_pOwner)
    {
        // Don't do anything if hit the person who threw it
        return 1;
    }

    if (pHit->mBaseGameObjectFlags.Get(BaseGameObject::eCanExplode_Bit7) && static_cast<BaseAliveGameObject*>(pHit)->mSpriteScale == mSpriteScale)
    {
        field_134_bExplodeNow = 1;
        return 0;
    }

    return 1;
}
