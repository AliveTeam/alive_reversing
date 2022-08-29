#include "stdafx.h"
#include "MovingBomb.hpp"
#include "Function.hpp"
#include "Game.hpp"
#include "Map.hpp"
#include "../relive_lib/Shadow.hpp"
#include "stdlib.hpp"
#include "Sound/Midi.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "PlatformBase.hpp"
#include "Sfx.hpp"
#include "Abe.hpp"
#include "Explosion.hpp"
#include "Gibs.hpp"
#include "BaseAliveGameObject.hpp"
#include "../relive_lib/Events.hpp"
#include "SwitchStates.hpp"

const TintEntry kMovingBombTints_55C734[4] = {
    {EReliveLevelIds::eBarracks, 97u, 97u, 97u},
    {EReliveLevelIds::eBrewery_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eNone, 127u, 127u, 127u}};

ALIVE_VAR(1, 0x5C300C, MovingBomb*, gMovingBomb_5C300C, nullptr);

MovingBomb::MovingBomb(relive::Path_MovingBomb* pTlv, const Guid& tlvId)
    : BaseAliveGameObject(0)
{
    mBaseGameObjectFlags.Set(BaseGameObject::eCanExplode_Bit7);

    SetType(ReliveTypes::eTimedMine_or_MovingBomb);

    const AnimRecord& rec = AnimRec(AnimId::MovingBomb);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(AnimId::MovingBomb, ppRes);

    mAnim.mFlags.Set(AnimFlags::eBit15_bSemiTrans);
    mAnim.mRenderMode = TPageAbr::eBlend_0;
    field_118_state = States::eTriggeredBySwitch_1;

    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        mSpriteScale = FP_FromDouble(0.5);
        mScale = Scale::Bg;
        mAnim.mRenderLayer = Layer::eLayer_RollingBallBombMineCar_Half_16;
    }
    else if (pTlv->mScale == relive::reliveScale::eFull)
    {
        mSpriteScale = FP_FromInteger(1);
        mScale = Scale::Fg;
        mAnim.mRenderLayer = Layer::eLayer_RollingBallBombMineCar_35;
    }

    mXPos = FP_FromInteger(pTlv->mTopLeftX);
    mYPos = FP_FromInteger(pTlv->mTopLeftY);
    field_124_speed = FP_FromRaw(pTlv->mSpeed << 8);
    mVelX = FP_FromRaw(pTlv->mStartSpeed << 8);
    field_128_start_moving_switch_id = pTlv->mStartMovingSwitchId;
    field_120_timer = sGnFrame;
    field_11C_tlvInfo = tlvId;
    field_12C_max = 0;
    field_12A_min = 0;
    field_136_persist_offscreen = pTlv->mPersistOffscreen;
    field_130_sound_channels = 0;

    if (pTlv->mTriggeredByAlarm == relive::reliveChoice::eYes)
    {
        field_118_state = States::eTriggeredByAlarm_0;
        mAnim.mFlags.Clear(AnimFlags::eBit3_Render);
    }

    SetTint(&kMovingBombTints_55C734[0], gMap.mCurrentLevel);

    field_134_disabled_resources = pTlv->mDisabledResources;
    if (!(field_134_disabled_resources & 1))
    {
        Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID);
    }

    Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kExplo2ResID);
    Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kMetalGibResID);
    Add_Resource(ResourceManager::Resource_Palt, AEResourceID::kAbeblowResID);
    Add_Resource(ResourceManager::Resource_Palt, AEResourceID::kSlogBlowResID);

    FP hitX = {};
    FP hitY = {};
    if (sCollisions->Raycast(
            mXPos,
            mYPos,
            mXPos + FP_FromInteger(24),
            mYPos + FP_FromInteger(24),
            &BaseAliveGameObjectCollisionLine,
            &hitX,
            &hitY,
            CollisionMask(eTrackLine_8)))
    {
        mYPos = hitY;
        mXPos = hitX;
    }

    mShadow = relive_new Shadow();
}

MovingBomb::~MovingBomb()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    if (pPlatform)
    {
        pPlatform->VRemove(this);
        BaseAliveGameObject_PlatformId = Guid{};
    }

    if (field_118_state >= States::eBlowingUp_6)
    {
        Path::TLV_Reset(field_11C_tlvInfo, -1, 0, 1);
    }
    else
    {
        Path::TLV_Reset(field_11C_tlvInfo, -1, 0, 0);
    }

    if (gMovingBomb_5C300C == this)
    {
        if (field_130_sound_channels)
        {
            SND_Stop_Channels_Mask(field_130_sound_channels);
            field_130_sound_channels = 0;
        }
        gMovingBomb_5C300C = 0;
    }
}

void MovingBomb::BlowUp()
{
    mBaseGameObjectFlags.Clear(BaseGameObject::eCanExplode_Bit7);
    field_118_state = States::eBlowingUp_6;
    mVelY = FP_FromInteger(0);
    field_120_timer = sGnFrame + 1;
    SfxPlayMono(SoundEffect::GreenTick_2, 100, mSpriteScale);
}

void MovingBomb::VRender(PrimHeader** ot)
{
    if (mAnim.mFlags.Get(AnimFlags::eBit3_Render))
    {
        BaseAnimatedWithPhysicsGameObject::VRender(ot);
    }
}

void MovingBomb::VScreenChanged()
{
    BaseGameObject::VScreenChanged();

    if (field_136_persist_offscreen == relive::reliveChoice::eNo)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        return;
    }

    const FP xDelta = FP_Abs(sControlledCharacter->mXPos - mXPos);
    if (xDelta > FP_FromInteger(750))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        return;
    }

    const FP yDelta = FP_Abs(sControlledCharacter->mYPos - mYPos);
    if (yDelta > FP_FromInteger(520))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        return;
    }
}

void MovingBomb::FollowLine()
{
    if (BaseAliveGameObjectCollisionLine)
    {
        BaseAliveGameObjectCollisionLine = BaseAliveGameObjectCollisionLine->MoveOnLine(&mXPos, &mYPos, mVelX);
    }
}

s16 MovingBomb::VTakeDamage(BaseGameObject* pFrom)
{
    if (mBaseGameObjectFlags.Get(BaseGameObject::eDead) || mHealth <= FP_FromInteger(0))
    {
        return 1;
    }

    switch (pFrom->Type())
    {
        case ReliveTypes::eAbilityRing:
        case ReliveTypes::eExplosion:
        case ReliveTypes::eShrykull:
        {
            mHealth = FP_FromInteger(0);
            relive_new Explosion(mXPos, mYPos, mSpriteScale, 0);

            relive_new Gibs(GibType::Metal_5, mXPos, mYPos, FP_FromInteger(0), FP_FromInteger(5), mSpriteScale, 0);

            field_118_state = States::eKillMovingBomb_7;

            mAnim.mFlags.Clear(AnimFlags::eBit3_Render);
            field_120_timer = sGnFrame + 4;
        }
            return 0;

        case ReliveTypes::eElectrocute:
            field_118_state = States::eKillMovingBomb_7;
            return 0;

        default:
            return 1;
    }
}

void MovingBomb::VOnThrowableHit(BaseGameObject* /*pObj*/)
{
    if (!mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit7_Electrocuted))
    {
        BlowUp();
    }
}

s16 MovingBomb::HitObject()
{
    const PSX_RECT bRect = VGetBoundingRect();
    for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
    {
        auto pObj = gBaseAliveGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj != this)
        {
            if (pObj->mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit6_SetOffExplosives))
            {
                if (pObj->mHealth > FP_FromInteger(0))
                {
                    const PSX_RECT objRect = pObj->VGetBoundingRect();
                    if (bRect.x <= objRect.w && bRect.w >= objRect.x && bRect.h >= objRect.y && bRect.y <= objRect.h && pObj->mSpriteScale == mSpriteScale && pObj->mCurrentPath == mCurrentPath)
                    {
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

void MovingBomb::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (field_118_state < States::eBlowingUp_6)
    {
        if (HitObject())
        {
            if (!mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit7_Electrocuted))
            {
                BlowUp();
            }
        }
    }

    if (gMovingBomb_5C300C == nullptr || gMovingBomb_5C300C == this)
    {
        if (mAnim.mCurrentFrame != 0 && mAnim.mCurrentFrame != 7)
        {
            gMovingBomb_5C300C = this;
        }
        else
        {
            if (field_130_sound_channels)
            {
                SND_Stop_Channels_Mask(field_130_sound_channels);
            }

            if (VIsObjNearby(FP_FromInteger(700), sActiveHero))
            {
                const FP yDelta = FP_Abs(sActiveHero->mYPos - mYPos);
                if (yDelta <= FP_FromInteger(700))
                {
                    if (field_118_state == States::eWaitABit_4)
                    {
                        field_130_sound_channels = SfxPlayMono(SoundEffect::SecurityOrb_48, 55, mSpriteScale);
                    }
                    else
                    {
                        field_130_sound_channels = SfxPlayMono(SoundEffect::SecurityOrb_48, 80, mSpriteScale);
                    }
                    gMovingBomb_5C300C = this;
                }
                else
                {
                    if (field_118_state == States::eWaitABit_4)
                    {
                        field_130_sound_channels = 0;
                        gMovingBomb_5C300C = this;
                    }
                    else
                    {
                        field_130_sound_channels = SfxPlayMono(SoundEffect::SecurityOrb_48, 12, mSpriteScale);
                        gMovingBomb_5C300C = this;
                    }
                }
            }
        }
    }

    switch (field_118_state)
    {
        case States::eTriggeredByAlarm_0:
            if (EventGet(kEventAlarm))
            {
                mAnim.mFlags.Set(AnimFlags::eBit3_Render);
                field_118_state = States::eMoving_2;
            }
            break;

        case States::eTriggeredBySwitch_1:
            if (SwitchStates_Get(field_128_start_moving_switch_id))
            {
                field_118_state = States::eMoving_2;
            }
            break;

        case States::eMoving_2:
            if (mVelX < field_124_speed)
            {
                mVelX += (mSpriteScale * FP_FromDouble(0.5));
            }

            FollowLine();

            BaseAliveGameObjectPathTLV = sPathInfo->TLV_Get_At_4DB4B0(
                FP_GetExponent(mXPos),
                FP_GetExponent(mYPos),
                FP_GetExponent(mXPos),
                FP_GetExponent(mYPos),
                ReliveTypes::eMovingBombStopper);

            if (BaseAliveGameObjectPathTLV)
            {
                auto pStopper = static_cast<relive::Path_MovingBombStopper*>(BaseAliveGameObjectPathTLV);
                field_12A_min = pStopper->mMinDelay;
                field_12C_max = pStopper->mMaxDelay;
                field_118_state = States::eStopMoving_3;
            }
            break;

        case States::eStopMoving_3:
            mVelX -= (mSpriteScale * FP_FromDouble(0.5));
            if (mVelX < FP_FromInteger(0))
            {
                field_118_state = States::eWaitABit_4;
                field_120_timer = sGnFrame + Math_RandomRange(field_12A_min, field_12C_max);
            }

            FollowLine();
            break;

        case States::eWaitABit_4:
            if (field_120_timer <= static_cast<s32>(sGnFrame))
            {
                field_118_state = States::eToMoving_5;
            }
            break;

        case States::eToMoving_5:
            if (mVelX < field_124_speed)
            {
                mVelX += (mSpriteScale * FP_FromDouble(0.5));
            }

            FollowLine();

            BaseAliveGameObjectPathTLV = sPathInfo->TLV_Get_At_4DB4B0(
                FP_GetExponent(mXPos),
                FP_GetExponent(mYPos),
                FP_GetExponent(mXPos),
                FP_GetExponent(mYPos),
                ReliveTypes::eMovingBombStopper);

            if (!BaseAliveGameObjectPathTLV)
            {
                field_118_state = States::eMoving_2;
            }
            break;

        case States::eBlowingUp_6:
            if (field_120_timer <= static_cast<s32>(sGnFrame))
            {
                SfxPlayMono(SoundEffect::GreenTick_2, 100, mSpriteScale);

                mHealth = FP_FromInteger(0);

                relive_new Explosion(
                    mXPos,
                    mYPos,
                    mSpriteScale,
                    0);

                relive_new Gibs(
                    GibType::Metal_5,
                    mXPos,
                    mYPos,
                    FP_FromInteger(0),
                    FP_FromInteger(5),
                    mSpriteScale,
                    0);

                field_118_state = States::eKillMovingBomb_7;
                mAnim.mFlags.Clear(AnimFlags::eBit3_Render);
                field_120_timer = sGnFrame + 4;
            }
            break;

        case States::eKillMovingBomb_7:
            if (field_120_timer <= static_cast<s32>(sGnFrame))
            {
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            }
            break;

        default:
            return;
    }
}
