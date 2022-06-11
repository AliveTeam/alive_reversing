#include "stdafx_ao.h"
#include "Function.hpp"
#include "MovingBomb.hpp"
#include "ResourceManager.hpp"
#include "stdlib.hpp"
#include "Map.hpp"
#include "Game.hpp"
#include "Shadow.hpp"
#include "Collisions.hpp"
#include "PlatformBase.hpp"
#include "Midi.hpp"
#include "Gibs.hpp"
#include "Explosion.hpp"
#include "Sfx.hpp"
#include "SwitchStates.hpp"
#include "Events.hpp"
#include "Abe.hpp"
#include "Math.hpp"
#include "Grid.hpp"

namespace AO {

const TintEntry kMovingBombTints_4CD310[4] = {
    {EReliveLevelIds::eStockYards, 30u, 30u, 55u},
    {EReliveLevelIds::eStockYardsReturn, 30u, 30u, 55u},
    {EReliveLevelIds::eNone, 127u, 127u, 127u}};

ALIVE_VAR(1, 0x507B8C, MovingBomb*, gMovingBomb_507B8C, nullptr);

MovingBomb::MovingBomb(Path_MovingBomb* pTlv, s32 tlvInfo)
    : BaseAliveGameObject()
{
    mBaseGameObjectFlags.Set(Options::eCanExplode_Bit7);
    mBaseGameObjectTypeId = ReliveTypes::eTimedMine;
    const AnimRecord rec = AO::AnimRec(AnimId::MovingBomb);
    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init_417FD0(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1);

    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit15_bSemiTrans);
    mBaseAnimatedWithPhysicsGameObject_Anim.mRenderMode = TPageAbr::eBlend_0;
    field_10C_state = States::eTriggeredBySwitch_1;

    if (pTlv->field_1E_scale == Scale_short::eHalf_1)
    {
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromDouble(0.5);
        mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Bg;
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_BombRollingBall_Half_16;
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromInteger(1);
        mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Fg;
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_BombRollingBall_35;
    }

    mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(pTlv->field_10_top_left.field_0_x);
    mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(pTlv->field_10_top_left.field_2_y);


    field_118_speed = FP_FromRaw(pTlv->field_18_speed << 8);
    mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromRaw(pTlv->field_24_start_speed << 8);
    field_11C_switch_id = pTlv->field_1A_switch_id;
    field_114_timer = sGnFrame;
    mBaseAnimatedWithPhysicsGameObject_YOffset = 0;
    field_110_tlvInfo = tlvInfo;
    field_120_min = 0;
    field_11E_max = 0;
    field_12A_persist_offscreen = pTlv->field_26_persist_offscreen;
    field_124_sound_channels = 0;

    if (pTlv->field_1C_bTriggered_by_alarm == Choice_short::eYes_1)
    {
        field_10C_state = States::eTriggeredByAlarm_0;
        mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit3_Render);
    }

    SetTint_418750(kMovingBombTints_4CD310, gMap.mCurrentLevel);

    field_128_disabled_resources = pTlv->field_22_disabled_resources;

    if (!(field_128_disabled_resources & 1))
    {
        ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kAbeblowAOResID, 1, 0);
    }

    if (!(field_128_disabled_resources & 2))
    {
        ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kElmblowAOResID_217, 1, 0);
    }

    ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kExplo2AOResID, 1, 0);
    ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kMetalGibAOResID, 1, 0);
    ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Palt, AOResourceID::kAbeblowAOResID, 1, 0);
    ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Palt, AOResourceID::kSlogBlowAOResID, 1, 0);

    mLiftPoint = nullptr;

    FP hitX = {};
    FP hitY = {};
    if (sCollisions->RayCast(
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
    if (!(field_128_disabled_resources & 1))
    {
        ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kAbeblowAOResID, 0, 0));
    }

    if (!(field_128_disabled_resources & 2))
    {
        ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kElmblowAOResID_217, 0, 0));
    }

    ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kExplo2AOResID, 0, 0));
    ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kMetalGibAOResID, 0, 0));
    ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Palt, AOResourceID::kAbeblowAOResID, 0, 0));
    ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Palt, AOResourceID::kSlogBlowAOResID, 0, 0));

    if (mLiftPoint)
    {
        mLiftPoint->VRemove(this);
        mLiftPoint->mBaseGameObjectRefCount--;
        mLiftPoint = nullptr;
    }

    if (field_10C_state == States::eBlowingUp_6 || field_10C_state == States::eKillMovingBomb_7)
    {
        gMap.TLV_Reset(field_110_tlvInfo, -1, 0, 1);
    }
    else
    {
        gMap.TLV_Reset(field_110_tlvInfo, -1, 0, 0);
    }

    if (gMovingBomb_507B8C == this)
    {
        if (field_124_sound_channels)
        {
            SND_Stop_Channels_Mask_4774A0(field_124_sound_channels);
            field_124_sound_channels = 0;
        }
        gMovingBomb_507B8C = nullptr;
    }
}

void MovingBomb::VScreenChanged()
{
    if (field_12A_persist_offscreen == Choice_short::eNo_0 || gMap.mCurrentLevel != gMap.mLevel || gMap.mCurrentPath != gMap.mPath)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

s16 MovingBomb::VTakeDamage(BaseGameObject* pFrom)
{
    if (mBaseGameObjectFlags.Get(BaseGameObject::eDead))
    {
        return 1;
    }

    if (mHealth <= FP_FromInteger(0))
    {
        return 1;
    }

    if (pFrom->mBaseGameObjectTypeId != ReliveTypes::eAbilityRing && pFrom->mBaseGameObjectTypeId != ReliveTypes::eExplosion && pFrom->mBaseGameObjectTypeId != ReliveTypes::eShrykull)
    {
        return 0;
    }

    mHealth = FP_FromInteger(0);

    relive_new Explosion(
        mBaseAnimatedWithPhysicsGameObject_XPos,
        mBaseAnimatedWithPhysicsGameObject_YPos,
        mBaseAnimatedWithPhysicsGameObject_SpriteScale);

    relive_new Gibs(
        GibType::Metal_5,
        mBaseAnimatedWithPhysicsGameObject_XPos,
        mBaseAnimatedWithPhysicsGameObject_YPos,
        FP_FromInteger(0),
        FP_FromInteger(5),
        mBaseAnimatedWithPhysicsGameObject_SpriteScale);

    field_10C_state = States::eKillMovingBomb_7;
    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit3_Render);
    field_114_timer = sGnFrame + 4;
    return 0;
}

void MovingBomb::VRender(PrimHeader** ppOt)
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit3_Render))
    {
        BaseAnimatedWithPhysicsGameObject::VRender(ppOt);
    }
}

void MovingBomb::VOnThrowableHit(BaseGameObject* /*pFrom*/)
{
    mBaseGameObjectFlags.Clear(Options::eCanExplode_Bit7);
    field_10C_state = States::eBlowingUp_6;
    mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);
    field_114_timer = sGnFrame + 1;
    SFX_Play_Mono(SoundEffect::GreenTick_3, 100, 0);
}

s16 MovingBomb::HitObject()
{
    const PSX_RECT ourRect = VGetBoundingRect();
    for (s32 i = 0; i < gBaseAliveGameObjects_4FC8A0->Size(); i++)
    {
        BaseAliveGameObject* pObjIter = gBaseAliveGameObjects_4FC8A0->ItemAt(i);
        if (!pObjIter)
        {
            break;
        }

        if (pObjIter != this)
        {
            if (pObjIter->mBaseAliveGameObjectFlags.Get(Flags_10A::e10A_Bit4_SetOffExplosives))
            {
                if (pObjIter->mHealth > FP_FromInteger(0))
                {
                    const PSX_RECT objRect = pObjIter->VGetBoundingRect();
                    if (RectsOverlap(ourRect, objRect) && pObjIter->mBaseAnimatedWithPhysicsGameObject_SpriteScale == mBaseAnimatedWithPhysicsGameObject_SpriteScale)
                    {
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

void MovingBomb::FollowLine()
{
    if (BaseAliveGameObjectCollisionLine)
    {
        const FP oldX = mBaseAnimatedWithPhysicsGameObject_XPos;
        const FP oldY = mBaseAnimatedWithPhysicsGameObject_YPos;

        BaseAliveGameObjectCollisionLine = BaseAliveGameObjectCollisionLine->MoveOnLine(&mBaseAnimatedWithPhysicsGameObject_XPos, &mBaseAnimatedWithPhysicsGameObject_YPos, mBaseAnimatedWithPhysicsGameObject_VelX);
        if (BaseAliveGameObjectCollisionLine)
        {
            u16 a4 = 0;
            const FP screen_x = CamX_VoidSkipper(oldX, mBaseAnimatedWithPhysicsGameObject_XPos - oldX, 12, &a4);
            if (a4)
            {
                FP hitX = {};
                FP hitY = {};
                mBaseAnimatedWithPhysicsGameObject_XPos = screen_x;
                if (sCollisions->RayCast(
                        mBaseAnimatedWithPhysicsGameObject_XPos,
                        mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(20),
                        mBaseAnimatedWithPhysicsGameObject_XPos,
                        mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(20),
                        &BaseAliveGameObjectCollisionLine,
                        &hitX,
                        &hitY,
                        0x100))
                {
                    mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
                }
            }

            // OG bug? Why y = oldx, surely y-oldy ?
            const FP screen_y = CamY_VoidSkipper(oldY, mBaseAnimatedWithPhysicsGameObject_YPos - oldX, 12, &a4);
            if (a4)
            {
                FP hitX = {};
                FP hitY = {};
                mBaseAnimatedWithPhysicsGameObject_YPos = screen_y;
                if (sCollisions->RayCast(
                        mBaseAnimatedWithPhysicsGameObject_XPos - FP_FromInteger(20),
                        mBaseAnimatedWithPhysicsGameObject_YPos,
                        mBaseAnimatedWithPhysicsGameObject_XPos + FP_FromInteger(20),
                        mBaseAnimatedWithPhysicsGameObject_YPos,
                        &BaseAliveGameObjectCollisionLine,
                        &hitX,
                        &hitY,
                        0x100))
                {
                    mBaseAnimatedWithPhysicsGameObject_XPos = hitX;
                }
            }
        }
    }
}

void MovingBomb::VUpdate()
{
    if (Event_Get(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(Options::eDead);
    }

    if (field_10C_state == States::eTriggeredByAlarm_0 || field_10C_state == States::eTriggeredBySwitch_1 || field_10C_state == States::eMoving_2 || field_10C_state == States::eStopMoving_3 || field_10C_state == States::eWaitABit_4 || field_10C_state == States::eToMoving_5)
    {
        if (HitObject())
        {
            mBaseGameObjectFlags.Clear(Options::eCanExplode_Bit7);
            field_10C_state = States::eBlowingUp_6;
            mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);
            field_114_timer = sGnFrame + 1;
            SFX_Play_Mono(SoundEffect::GreenTick_3, 100);
        }
    }

    if (!gMovingBomb_507B8C || gMovingBomb_507B8C == this)
    {
        if (mBaseAnimatedWithPhysicsGameObject_Anim.field_92_current_frame != 0 && mBaseAnimatedWithPhysicsGameObject_Anim.field_92_current_frame != 7)
        {
            gMovingBomb_507B8C = this;
        }
        else
        {
            if (field_124_sound_channels)
            {
                SND_Stop_Channels_Mask_4774A0(field_124_sound_channels);
            }

            if (VIsObjNearby(FP_FromInteger(700), sActiveHero_507678))
            {
                if (FP_Abs(sActiveHero_507678->mBaseAnimatedWithPhysicsGameObject_YPos - mBaseAnimatedWithPhysicsGameObject_YPos) <= FP_FromInteger(700))
                {
                    if (field_10C_state == States::eWaitABit_4)
                    {
                        field_124_sound_channels = SFX_Play_Mono(SoundEffect::SecurityOrb_56, 55);
                    }
                    else
                    {
                        field_124_sound_channels = SFX_Play_Mono(SoundEffect::SecurityOrb_56, 80);
                    }
                }
            }
            else
            {
                if (field_10C_state == States::eWaitABit_4)
                {
                    field_124_sound_channels = 0;
                    gMovingBomb_507B8C = this;
                }
                else
                {
                    field_124_sound_channels = SFX_Play_Mono(SoundEffect::SecurityOrb_56, 12, 0);
                    gMovingBomb_507B8C = this;
                }
            }
        }
    }

    switch (field_10C_state)
    {
        case States::eTriggeredByAlarm_0:
            if (Event_Get(kEventAlarm))
            {
                mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit3_Render);
                field_10C_state = States::eMoving_2;
            }
            break;

        case States::eTriggeredBySwitch_1:
            if (SwitchStates_Get(field_11C_switch_id))
            {
                field_10C_state = States::eMoving_2;
            }
            break;

        case States::eMoving_2:
            if (mBaseAnimatedWithPhysicsGameObject_VelX < field_118_speed)
            {
                mBaseAnimatedWithPhysicsGameObject_VelX += (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromDouble(0.5));
            }

            FollowLine();

            BaseAliveGameObjectPathTLV = gMap.TLV_Get_At_446260(
                FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
                FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
                FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
                FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
                TlvTypes::MovingBombStopper_87);

            if (BaseAliveGameObjectPathTLV)
            {
                auto pStopper = static_cast<Path_MovingBombStopper*>(BaseAliveGameObjectPathTLV);
                field_11E_max = pStopper->field_18_min_delay;
                field_120_min = pStopper->field_1A_max_delay;
                field_10C_state = States::eStopMoving_3;
            }
            break;

        case States::eStopMoving_3:
            mBaseAnimatedWithPhysicsGameObject_VelX -= (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromDouble(0.5));
            if (mBaseAnimatedWithPhysicsGameObject_VelX < FP_FromInteger(0))
            {
                field_10C_state = States::eWaitABit_4;
                field_114_timer = sGnFrame + Math_RandomRange_450F20(field_11E_max, field_120_min);
            }

            FollowLine();
            break;

        case States::eWaitABit_4:
            if (field_114_timer <= static_cast<s32>(sGnFrame))
            {
                field_10C_state = States::eToMoving_5;
            }
            break;

        case States::eToMoving_5:
            if (mBaseAnimatedWithPhysicsGameObject_VelX < field_118_speed)
            {
                mBaseAnimatedWithPhysicsGameObject_VelX += (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromDouble(0.5));
            }

            FollowLine();

            BaseAliveGameObjectPathTLV = gMap.TLV_Get_At_446260(
                FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
                FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
                FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
                FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
                TlvTypes::MovingBombStopper_87);
            if (!BaseAliveGameObjectPathTLV)
            {
                field_10C_state = States::eMoving_2;
            }
            break;

        case States::eBlowingUp_6:
            if (field_114_timer <= static_cast<s32>(sGnFrame))
            {
                SFX_Play_Mono(SoundEffect::GreenTick_3, 100, 0);

                mHealth = FP_FromInteger(0);

                relive_new Explosion(
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    mBaseAnimatedWithPhysicsGameObject_YPos,
                    mBaseAnimatedWithPhysicsGameObject_SpriteScale);

                relive_new Gibs(
                    GibType::Metal_5,
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    mBaseAnimatedWithPhysicsGameObject_YPos,
                    FP_FromInteger(0),
                    FP_FromInteger(5),
                    mBaseAnimatedWithPhysicsGameObject_SpriteScale);

                field_10C_state = States::eKillMovingBomb_7;
                mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit3_Render);
                field_114_timer = sGnFrame + 4;
            }
            break;

        case States::eKillMovingBomb_7:
            if (field_114_timer <= static_cast<s32>(sGnFrame))
            {
                mBaseGameObjectFlags.Set(Options::eDead);
            }
            break;

        default:
            break;
    }
}

} // namespace AO
