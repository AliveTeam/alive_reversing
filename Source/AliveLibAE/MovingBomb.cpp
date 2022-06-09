#include "stdafx.h"
#include "MovingBomb.hpp"
#include "Function.hpp"
#include "Game.hpp"
#include "Map.hpp"
#include "Shadow.hpp"
#include "stdlib.hpp"
#include "Sound/Midi.hpp"
#include "ObjectIds.hpp"
#include "PlatformBase.hpp"
#include "Sfx.hpp"
#include "Abe.hpp"
#include "Explosion.hpp"
#include "Gibs.hpp"
#include "BaseAliveGameObject.hpp"
#include "Events.hpp"
#include "SwitchStates.hpp"

const TintEntry kMovingBombTints_55C734[4] = {
    {EReliveLevelIds::eBarracks, 97u, 97u, 97u},
    {EReliveLevelIds::eBrewery_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eNone, 127u, 127u, 127u}};

ALIVE_VAR(1, 0x5C300C, MovingBomb*, gMovingBomb_5C300C, nullptr);

MovingBomb::MovingBomb(Path_MovingBomb* pTlv, s32 tlvInfo)
    : BaseAliveGameObject(0)
{
    mBaseGameObjectFlags.Set(BaseGameObject::eCanExplode_Bit7);

    SetType(ReliveTypes::eTimedMine_or_MovingBomb);

    const AnimRecord& rec = AnimRec(AnimId::MovingBomb);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);

    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit15_bSemiTrans);
    mBaseAnimatedWithPhysicsGameObject_Anim.mRenderMode = TPageAbr::eBlend_0;
    field_118_state = States::eTriggeredBySwitch_1;

    if (pTlv->field_16_scale == Scale_short::eHalf_1)
    {
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromDouble(0.5);
        mBaseAnimatedWithPhysicsGameObject_Scale = 0;
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_BombMineCar_Half_16;
    }
    else if (pTlv->field_16_scale == Scale_short::eFull_0)
    {
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromInteger(1);
        mBaseAnimatedWithPhysicsGameObject_Scale = 1;
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_BombMineCar_35;
    }

    mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(pTlv->field_8_top_left.field_0_x);
    mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);
    field_124_speed = FP_FromRaw(pTlv->field_10_speed << 8);
    mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromRaw(pTlv->field_1C_start_speed << 8);
    field_128_start_moving_switch_id = pTlv->field_12_start_moving_switch_id;
    field_120_timer = sGnFrame;
    field_11C_tlvInfo = tlvInfo;
    field_12C_max = 0;
    field_12A_min = 0;
    field_136_persist_offscreen = pTlv->field_1E_persist_offscreen;
    field_130_sound_channels = 0;

    if (pTlv->field_14_bTriggered_by_alarm == Choice_short::eYes_1)
    {
        field_118_state = States::eTriggeredByAlarm_0;
        mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit3_Render);
    }

    SetTint(&kMovingBombTints_55C734[0], gMap.mCurrentLevel);

    field_134_disabled_resources = pTlv->field_1A_disabled_resources;
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
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos,
            mBaseAnimatedWithPhysicsGameObject_XPos + FP_FromInteger(24),
            mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(24),
            &BaseAliveGameObjectCollisionLine,
            &hitX,
            &hitY,
            0x100))
    {
        mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
        mBaseAnimatedWithPhysicsGameObject_XPos = hitX;
    }

    mShadow = relive_new Shadow();
}

MovingBomb::~MovingBomb()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObjectId));
    if (pPlatform)
    {
        pPlatform->VRemove(this);
        BaseAliveGameObjectId = -1;
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
    mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);
    field_120_timer = sGnFrame + 1;
    SFX_Play_Mono(SoundEffect::GreenTick_2, 100, mBaseAnimatedWithPhysicsGameObject_SpriteScale);
}

void MovingBomb::VRender(PrimHeader** ot)
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit3_Render))
    {
        BaseAnimatedWithPhysicsGameObject::VRender(ot);
    }
}

void MovingBomb::VScreenChanged()
{
    BaseGameObject::VScreenChanged();

    if (field_136_persist_offscreen == Choice_short::eNo_0)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        return;
    }

    const FP xDelta = FP_Abs(sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_XPos - mBaseAnimatedWithPhysicsGameObject_XPos);
    if (xDelta > FP_FromInteger(750))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        return;
    }

    const FP yDelta = FP_Abs(sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_YPos - mBaseAnimatedWithPhysicsGameObject_YPos);
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
        BaseAliveGameObjectCollisionLine = BaseAliveGameObjectCollisionLine->MoveOnLine(&mBaseAnimatedWithPhysicsGameObject_XPos, &mBaseAnimatedWithPhysicsGameObject_YPos, mBaseAnimatedWithPhysicsGameObject_VelX);
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
            relive_new Explosion(mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, mBaseAnimatedWithPhysicsGameObject_SpriteScale, 0);

            relive_new Gibs(GibType::Metal_5, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, FP_FromInteger(0), FP_FromInteger(5), mBaseAnimatedWithPhysicsGameObject_SpriteScale, 0);

            field_118_state = States::eKillMovingBomb_7;

            mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit3_Render);
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
    PSX_RECT bRect = {};
    VGetBoundingRect(&bRect, 1);
    for (s32 i = 0; i < gBaseAliveGameObjects_5C1B7C->Size(); i++)
    {
        auto pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(i);
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
                    PSX_RECT objRect = {};
                    pObj->VGetBoundingRect(&objRect, 1);
                    if (bRect.x <= objRect.w && bRect.w >= objRect.x && bRect.h >= objRect.y && bRect.y <= objRect.h && pObj->mBaseAnimatedWithPhysicsGameObject_SpriteScale == mBaseAnimatedWithPhysicsGameObject_SpriteScale && pObj->mBaseAnimatedWithPhysicsGameObject_PathNumber == mBaseAnimatedWithPhysicsGameObject_PathNumber)
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
    if (Event_Get(kEventDeathReset))
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
        if (mBaseAnimatedWithPhysicsGameObject_Anim.field_92_current_frame != 0 && mBaseAnimatedWithPhysicsGameObject_Anim.field_92_current_frame != 7)
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
                const FP yDelta = FP_Abs(sActiveHero->mBaseAnimatedWithPhysicsGameObject_YPos - mBaseAnimatedWithPhysicsGameObject_YPos);
                if (yDelta <= FP_FromInteger(700))
                {
                    if (field_118_state == States::eWaitABit_4)
                    {
                        field_130_sound_channels = SFX_Play_Mono(SoundEffect::SecurityOrb_48, 55, mBaseAnimatedWithPhysicsGameObject_SpriteScale);
                    }
                    else
                    {
                        field_130_sound_channels = SFX_Play_Mono(SoundEffect::SecurityOrb_48, 80, mBaseAnimatedWithPhysicsGameObject_SpriteScale);
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
                        field_130_sound_channels = SFX_Play_Mono(SoundEffect::SecurityOrb_48, 12, mBaseAnimatedWithPhysicsGameObject_SpriteScale);
                        gMovingBomb_5C300C = this;
                    }
                }
            }
        }
    }

    switch (field_118_state)
    {
        case States::eTriggeredByAlarm_0:
            if (Event_Get(kEventAlarm))
            {
                mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit3_Render);
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
            if (mBaseAnimatedWithPhysicsGameObject_VelX < field_124_speed)
            {
                mBaseAnimatedWithPhysicsGameObject_VelX += (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromDouble(0.5));
            }

            FollowLine();

            BaseAliveGameObjectPathTLV = sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
                FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
                FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
                FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
                FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
                TlvTypes::MovingBombStopper_53);

            if (BaseAliveGameObjectPathTLV)
            {
                auto pStopper = static_cast<Path_MovingBombStopper*>(BaseAliveGameObjectPathTLV);
                field_12A_min = pStopper->field_10_min;
                field_12C_max = pStopper->field_12_max;
                field_118_state = States::eStopMoving_3;
            }
            break;

        case States::eStopMoving_3:
            mBaseAnimatedWithPhysicsGameObject_VelX -= (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromDouble(0.5));
            if (mBaseAnimatedWithPhysicsGameObject_VelX < FP_FromInteger(0))
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
            if (mBaseAnimatedWithPhysicsGameObject_VelX < field_124_speed)
            {
                mBaseAnimatedWithPhysicsGameObject_VelX += (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromDouble(0.5));
            }

            FollowLine();

            BaseAliveGameObjectPathTLV = sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
                FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
                FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
                FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
                FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
                TlvTypes::MovingBombStopper_53);

            if (!BaseAliveGameObjectPathTLV)
            {
                field_118_state = States::eMoving_2;
            }
            break;

        case States::eBlowingUp_6:
            if (field_120_timer <= static_cast<s32>(sGnFrame))
            {
                SFX_Play_Mono(SoundEffect::GreenTick_2, 100, mBaseAnimatedWithPhysicsGameObject_SpriteScale);

                mHealth = FP_FromInteger(0);

                relive_new Explosion(
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    mBaseAnimatedWithPhysicsGameObject_YPos,
                    mBaseAnimatedWithPhysicsGameObject_SpriteScale,
                    0);

                relive_new Gibs(
                    GibType::Metal_5,
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    mBaseAnimatedWithPhysicsGameObject_YPos,
                    FP_FromInteger(0),
                    FP_FromInteger(5),
                    mBaseAnimatedWithPhysicsGameObject_SpriteScale,
                    0);

                field_118_state = States::eKillMovingBomb_7;
                mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit3_Render);
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
