#include "stdafx_ao.h"
#include "Scrab.hpp"
#include "Function.hpp"
#include "Map.hpp"
#include "../relive_lib/Events.hpp"
#include "../relive_lib/Collisions.hpp"
#include "../relive_lib/VRam.hpp"
#include "DDCheat.hpp"
#include "../relive_lib/Shadow.hpp"
#include "ResourceManager.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "MusicController.hpp"
#include "PlatformBase.hpp"
#include "Math.hpp"
#include "Sfx.hpp"
#include "CameraSwapper.hpp"
#include "LiftPoint.hpp"
#include "Game.hpp"
#include "FixedPoint.hpp"
#include "Gibs.hpp"
#include "../relive_lib/Particle.hpp"
#include "Midi.hpp"
#include "SwitchStates.hpp"
#include "Grid.hpp"

void Scrab_ForceLink()
{ }

namespace AO {

using TScrabMotionFunction = decltype(&Scrab::Motion_0_Empty_45E3D0);

const TScrabMotionFunction sScrabMotionTable_4CF690[] = {
    &Scrab::Motion_0_Empty_45E3D0,
    &Scrab::Motion_1_Stand_45E620,
    &Scrab::Motion_2_Walk_45E730,
    &Scrab::Motion_3_Run_45EAB0,
    &Scrab::Motion_4_Turn_45EF30,
    &Scrab::Motion_5_RunToStand_45ED90,
    &Scrab::Motion_6_HopBegin_45F3C0,
    &Scrab::Motion_7_HopMidair_45F1A0,
    &Scrab::Motion_8_HopLand_45F500,
    &Scrab::Motion_9_JumpToFall_45EFD0,
    &Scrab::Motion_10_StandToWalk_45E670,
    &Scrab::Motion_11_StandToRun_45E9F0,
    &Scrab::Motion_12_WalkToStand_45E930,
    &Scrab::Motion_13_RunJumpBegin_45F5D0,
    &Scrab::Motion_14_RunJumpEnd_45F850,
    &Scrab::Motion_15_ToFall_45F180,
    &Scrab::Motion_16_Stamp_45F920,
    &Scrab::Motion_17_Empty_45F9C0,
    &Scrab::Motion_18_GetEaten_45FF70,
    &Scrab::Motion_19_Unused_45F9D0,
    &Scrab::Motion_20_HowlBegin_45FA60,
    &Scrab::Motion_21_HowlEnd_45FAF0,
    &Scrab::Motion_22_Shriek_45FB00,
    &Scrab::Motion_23_ScrabBattleAnim_45FBA0,
    &Scrab::Motion_24_FeedToGulp_45FC30,
    &Scrab::Motion_25_ToFeed_45FCE0,
    &Scrab::Motion_26_Feed_45FDA0,
    &Scrab::Motion_27_AttackLunge_45FDF0,
    &Scrab::Motion_28_LegKick_45FF60,
    &Scrab::Motion_29_DeathBegin_45FFA0,
};

const AnimId sScrabMotionAnimIds[30] = {
    AnimId::Scrab_Idle,
    AnimId::Scrab_Idle,
    AnimId::Scrab_Walk,
    AnimId::Scrab_Run,
    AnimId::Scrab_Turn,
    AnimId::Scrab_RunToStand,
    AnimId::Scrab_HopBegin,
    AnimId::Scrab_Jump,
    AnimId::Scrab_Landing,
    AnimId::Scrab_Idle,
    AnimId::Scrab_StandToWalk,
    AnimId::Scrab_StandToRun,
    AnimId::Scrab_WalkToStand,
    AnimId::Scrab_Jump,
    AnimId::Scrab_Landing,
    AnimId::Scrab_AO_ToFall,
    AnimId::Scrab_Stamp,
    AnimId::Scrab_GetEaten,
    AnimId::Scrab_GetEaten,
    AnimId::Scrab_AO_M_19_Unused,
    AnimId::Scrab_HowlBegin,
    AnimId::Scrab_HowlEnd,
    AnimId::Scrab_Shriek,
    AnimId::Scrab_ScrabBattleAnim,
    AnimId::Scrab_FeedToGulp,
    AnimId::Scrab_AO_ToFeed,
    AnimId::Scrab_Feed,
    AnimId::Scrab_AttackLunge,
    AnimId::Scrab_LegKick,
    AnimId::Scrab_DeathBegin};

const static Scrab::TBrainType sScrabAITable[]{
    &Scrab::Brain_Fighting_45C370,
    &Scrab::Brain_BatDeath_45CA60,
    &Scrab::Brain_Death_45CB80,
    &Scrab::Brain_ChasingEnemy_45CC90,
    &Scrab::Brain_Patrol_460020,
    &Scrab::Brain_WalkAround_460D80,
};

Scrab::Scrab(relive::Path_Scrab* pTlv, const Guid& tlvId)
    : BaseAliveGameObject()
{
    mBaseGameObjectTypeId = ReliveTypes::eScrab;

    for (s32 i = 0; i < ALIVE_COUNTOF(field_150_resources); i++)
    {
        field_150_resources[i] = nullptr;
    }

    field_150_resources[0] = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kArsbasicAOResID, 1, 0);
    field_150_resources[11] = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kArschewAOResID, 1, 0);
    field_150_resources[6] = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kArsdanceAOResID, 1, 0);
    field_150_resources[8] = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kArsdeadAOResID, 1, 0);
    field_150_resources[1] = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kArseatAOResID, 1, 0);
    field_150_resources[10] = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kArsgrwlAOResID, 1, 0);
    field_150_resources[5] = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kArshowlAOResID, 1, 0);
    field_150_resources[2] = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kArsprceAOResID, 1, 0);
    field_150_resources[9] = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kArsroarAOResID, 1, 0);
    field_150_resources[3] = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kArsskwrAOResID, 1, 0);
    field_150_resources[4] = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kArswhirlAOResID, 1, 0);
    field_150_resources[13] = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kArscrshAOResID, 1, 0);

    Animation_Init(AnimId::Scrab_Idle, field_150_resources[0]);

    mBaseAliveGameObjectFlags.Set(Flags_10A::e10A_Bit4_SetOffExplosives);

    field_132_res_block_idx = 0;
    field_118_timer = 0;

    SetBrain(&Scrab::Brain_Patrol_460020);

    field_110_brain_sub_state = 0;
    mNextMotion = 0;
    mLiftPoint = nullptr;
    mCurrentMotion = 1;
    field_112 = 0;

    field_11C_pFight_target = nullptr;
    field_120_pTarget = nullptr;

    field_140_last_shriek_timer = 0;

    mXPos = FP_FromInteger(pTlv->mTopLeftX + 12);
    mYPos = FP_FromInteger(pTlv->mTopLeftY);

    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        mSpriteScale = FP_FromDouble(0.5);
        mAnim.mRenderLayer = Layer::eLayer_8;
        mScale = Scale::Bg;
    }
    else
    {
        mSpriteScale = FP_FromInteger(1);
        mAnim.mRenderLayer = Layer::eLayer_27;
        mScale = Scale::Fg;
    }

    field_114_attack_delay = pTlv->mAttackDelay;
    field_116_patrol_type = pTlv->mPatrolType;
    field_144_left_min_delay = pTlv->mPauseLeftMin;
    field_146_left_max_delay = pTlv->mPauseLeftMax;
    field_148_right_min_delay = pTlv->mPauseRightMin;
    field_14A_right_max_delay = pTlv->mPauseRightMax;

    field_138_spotting_abe_delay = pTlv->mSpottingAbeDelay;

    // TODO: flags
    field_188_flags = 32 * (pTlv->mRoarRandomly == relive::reliveChoice::eYes) | (((field_188_flags & ~0x11) | 4) & ~0x28);

    FP hitX = {};
    FP hitY = {};
    if (sCollisions->Raycast(
            mXPos,
            mYPos,
            mXPos,
            mYPos + FP_FromInteger(30),
            &BaseAliveGameObjectCollisionLine,
            &hitX,
            &hitY,
            mSpriteScale != FP_FromDouble(0.5) ? kFgFloor : kBgFloor)
        == 1)
    {
        mYPos = hitY;
        ToStand();

        if (BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eDynamicCollision_32 || BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eBackgroundDynamicCollision_36)
        {
            PlatformCollide();
        }

        field_188_flags |= 8u;
    }

    mBaseAliveGameObjectFlags.Set(Flags_10A::e10A_Bit6);

    mRGB.SetRGB(127, 127, 127);

    field_130_unused = 0;

    field_134_tlvInfo = tlvId;

    mShadow = relive_new Shadow();
}

Scrab::~Scrab()
{
    if (field_11C_pFight_target)
    {
        field_11C_pFight_target->mBaseGameObjectRefCount--;
    }

    if (field_120_pTarget)
    {
        field_120_pTarget->mBaseGameObjectRefCount--;
    }

    VOnTrapDoorOpen();

    for (s32 i = 0; i < ALIVE_COUNTOF(field_150_resources); i++)
    {
        if (mAnim.field_20_ppBlock != field_150_resources[i])
        {
            if (field_150_resources[i])
            {
                ResourceManager::FreeResource_455550(field_150_resources[i]);
            }
        }
    }

    if (mHealth <= FP_FromInteger(0))
    {
        Path::TLV_Reset(field_134_tlvInfo, -1, 0, 1);
    }
    else
    {
        Path::TLV_Reset(field_134_tlvInfo, -1, 0, 0);
    }

    MusicController::static_PlayMusic(MusicController::MusicTypes::eType0, this, 0, 0);
    MusicController::ClearObject(this);
}

void Scrab::VRender(PrimHeader** ppOt)
{
    if (mBaseGameObjectUpdateDelay == 0)
    {
        BaseAnimatedWithPhysicsGameObject::VRender(ppOt);
    }
}

void Scrab::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (!(field_188_flags & 8))
    {
        FP hitX = {};
        FP hitY = {};
        if (sCollisions->Raycast(
                mXPos,
                mYPos,
                mXPos,
                mYPos + FP_FromInteger(30),
                &BaseAliveGameObjectCollisionLine,
                &hitX,
                &hitY,
                mSpriteScale != FP_FromDouble(0.5) ? kFgFloor : kBgFloor)
            == 1)
        {
            mYPos = hitY;

            ToStand();

            if (BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eDynamicCollision_32 || BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eBackgroundDynamicCollision_36)
            {
                PlatformCollide();
            }

            field_188_flags |= 8u;
        }
    }

    const FP hero_xd = FP_Abs(mXPos - sActiveHero->mXPos);
    const FP hero_yd = FP_Abs(mYPos - sActiveHero->mYPos);

    if (hero_xd > FP_FromInteger(2048) || hero_yd > FP_FromInteger(960))
    {
        mAnim.mFlags.Clear(AnimFlags::eBit2_Animate);
        mAnim.mFlags.Clear(AnimFlags::eBit3_Render);
        if (mAnim.mVramRect.w)
        {
            Vram_free(
                PSX_Point{mAnim.mVramRect.x, mAnim.mVramRect.y},
                PSX_Point{mAnim.mVramRect.w, mAnim.mVramRect.h});
            mAnim.mVramRect.w = 0;
        }
    }
    else
    {
        if (mHealth > FP_FromInteger(0))
        {
            if (!mAnim.mVramRect.w)
            {
                Vram_alloc(168, 69, 8u, &mAnim.mVramRect);
            }
            mAnim.mFlags.Set(AnimFlags::eBit2_Animate);
            mAnim.mFlags.Set(AnimFlags::eBit3_Render);
        }

        const auto old_motion = mCurrentMotion;

        field_110_brain_sub_state = (this->*field_10C_fn)();

        if (showDebugCreatureInfo_5076E0)
        {
            DDCheat::DebugStr(
                "Scrab %d %d %d %d\n",
                field_110_brain_sub_state,
                field_118_timer,
                mCurrentMotion,
                mNextMotion);
        }


        const FP old_x = mXPos;
        const FP old_y = mYPos;

        (this->*sScrabMotionTable_4CF690[mCurrentMotion])();

        if (old_x != mXPos || old_y != mYPos)
        {
            BaseAliveGameObjectPathTLV = gMap.TLV_Get_At_446060(
                nullptr,
                mXPos,
                mYPos,
                mXPos,
                mYPos);
            VOnTlvCollision(BaseAliveGameObjectPathTLV);
        }

        if (old_motion != mCurrentMotion || field_188_flags & 0x10)
        {
            field_188_flags &= ~0x10u;
            vUpdateAnim();

            if (old_motion != eScrabMotions::Motion_0_Empty_45E3D0)
            {
                mAnim.SetFrame(mBaseAliveGameObjectLastAnimFrame);
            }
        }
        else if (field_112)
        {
            mCurrentMotion = mPreviousMotion;
            vUpdateAnim();
            mAnim.SetFrame(mBaseAliveGameObjectLastAnimFrame);
            field_112 = 0;
        }
    }
}

enum Brain_BatDeath
{
    eStartHowling_0 = 0,
    eWalkAway_1 = 1,
    eStartShrieking_2 = 2,
    eTurnAround_3 = 3,
    eDie_4 = 4
};

s16 Scrab::VTakeDamage(BaseGameObject* pFrom)
{
    if (mHealth > FP_FromInteger(0))
    {
        switch (pFrom->mBaseGameObjectTypeId)
        {
            case ReliveTypes::eBat:
                if (BrainIs(&Scrab::Brain_BatDeath_45CA60))
                {
                    return 1;
                }

                mHealth = FP_FromInteger(0);
                mNextMotion = eScrabMotions::Motion_1_Stand_45E620;
                SetBrain(&Scrab::Brain_BatDeath_45CA60);
                field_110_brain_sub_state = Brain_BatDeath::eStartHowling_0;
                return 1;

            case ReliveTypes::eBullet:
            case ReliveTypes::eRollingBall:
                mHealth = FP_FromInteger(0);
                SetBrain(&Scrab::Brain_Death_45CB80);
                field_130_unused = 2;
                field_118_timer = sGnFrame + 90;
                mCurrentMotion = eScrabMotions::Motion_29_DeathBegin_45FFA0;
                vUpdateAnim();
                break;

            case ReliveTypes::eBaseBomb:
            case ReliveTypes::eExplosion:
            {
                relive_new Gibs(
                    GibType::Slog_2,
                    mXPos,
                    mYPos,
                    mVelX,
                    mVelY,
                    mSpriteScale);

                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                return 1;
            }

            case ReliveTypes::eAbilityRing:
                return 0;

            default:
                SfxPlayMono(relive::SoundEffects::KillEffect, 127);
                SfxPlayMono(relive::SoundEffects::FallingItemHit, 90);
                mHealth = FP_FromInteger(0);
                SetBrain(&Scrab::Brain_Death_45CB80);
                field_130_unused = 2;
                field_118_timer = sGnFrame + 90;
                mCurrentMotion = eScrabMotions::Motion_29_DeathBegin_45FFA0;
                vUpdateAnim();
                break;
        }
    }
    return 1;
}

void Scrab::VOnTlvCollision(relive::Path_TLV* pTlv)
{
    while (pTlv)
    {
        if (pTlv->mTlvType == ReliveTypes::eDeathDrop)
        {
            Scrab_SFX(ScrabSounds::eYell_8, 127, -1000, 0);
            mBaseGameObjectFlags.Set(Options::eDead);
            mHealth = FP_FromInteger(0);
            field_130_unused = 2;
        }

        pTlv = gMap.TLV_Get_At_446060(
            pTlv,
            mXPos,
            mYPos,
            mXPos,
            mYPos);
    }
}

void Scrab::VScreenChanged()
{
    if (gMap.mCurrentLevel != gMap.mNextLevel
        || gMap.mCurrentPath != gMap.mNextPath
        || gMap.mOverlayId != gMap.GetOverlayId())
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
    else
    {
        if (field_120_pTarget)
        {
            if (field_120_pTarget->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
            {
                field_120_pTarget->mBaseGameObjectRefCount--;
                field_120_pTarget = nullptr;
                mNextMotion = eScrabMotions::Motion_1_Stand_45E620;
                SetBrain(&Scrab::Brain_WalkAround_460D80);
                field_110_brain_sub_state = 0;
            }
        }
    }
}

void Scrab::ToStand()
{
    field_128 = FP_FromInteger(0);
    mVelX = FP_FromInteger(0);
    mVelY = FP_FromInteger(0);
    mCurrentMotion = eScrabMotions::Motion_1_Stand_45E620;
    MapFollowMe_401D30(1);
}

void Scrab::vUpdateAnim()
{
    mAnim.Set_Animation_Data(
        sScrabMotionAnimIds[mCurrentMotion],
        ResBlockForMotion(mCurrentMotion));
}

u8** Scrab::ResBlockForMotion(s16 motion)
{
    if (motion < eScrabMotions::Motion_16_Stamp_45F920)
    {
        field_132_res_block_idx = 0;
    }
    else if (motion < eScrabMotions::Motion_17_Empty_45F9C0)
    {
        field_132_res_block_idx = 6;
    }
    else if (motion < eScrabMotions::Motion_19_Unused_45F9D0)
    {
        field_132_res_block_idx = 8;
    }
    else if (motion < eScrabMotions::Motion_20_HowlBegin_45FA60)
    {
        field_132_res_block_idx = 10;
    }
    else if (motion < eScrabMotions::Motion_22_Shriek_45FB00)
    {
        field_132_res_block_idx = 5;
    }
    else if (motion < eScrabMotions::Motion_23_ScrabBattleAnim_45FBA0)
    {
        field_132_res_block_idx = 9;
    }
    else if (motion < eScrabMotions::Motion_24_FeedToGulp_45FC30)
    {
        field_132_res_block_idx = 4;
    }
    else if (motion < eScrabMotions::Motion_25_ToFeed_45FCE0)
    {
        field_132_res_block_idx = 11;
    }
    else if (motion < eScrabMotions::Motion_27_AttackLunge_45FDF0)
    {
        field_132_res_block_idx = 1;
    }
    else if (motion < eScrabMotions::Motion_28_LegKick_45FF60)
    {
        field_132_res_block_idx = 2;
    }
    else if (motion >= eScrabMotions::Motion_29_DeathBegin_45FFA0)
    {
        field_132_res_block_idx = motion >= 30 ? 0 : 13;
    }
    else
    {
        field_132_res_block_idx = 3;
    }
    return field_150_resources[field_132_res_block_idx];
}

void Scrab::PlatformCollide()
{
    PSX_RECT bRect = VGetBoundingRect();

    bRect.y += 5;
    bRect.h += 5;

    VOnCollisionWith(
        {bRect.x, bRect.y},
        {bRect.w, bRect.h},
        ObjListPlatforms_50766C,
        (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection_401C10);
}

void Scrab::VOnTrapDoorOpen()
{
    if (mLiftPoint)
    {
        mLiftPoint->VRemove(this);
        mLiftPoint->mBaseGameObjectRefCount--;
        mLiftPoint = nullptr;

        mCurrentMotion = eScrabMotions::Motion_15_ToFall_45F180;

        field_188_flags |= 0x10u;
    }
}

s16 Scrab::ToNextMotion()
{
    MapFollowMe_401D30(1);

    switch (mNextMotion)
    {
        case eScrabMotions::Motion_4_Turn_45EF30:
        case eScrabMotions::Motion_16_Stamp_45F920:
        case eScrabMotions::Motion_19_Unused_45F9D0:
        case eScrabMotions::Motion_20_HowlBegin_45FA60:
        case eScrabMotions::Motion_22_Shriek_45FB00:
        case eScrabMotions::Motion_23_ScrabBattleAnim_45FBA0:
        case eScrabMotions::Motion_27_AttackLunge_45FDF0:
        case eScrabMotions::Motion_28_LegKick_45FF60:
        case eScrabMotions::Motion_25_ToFeed_45FCE0:
            mCurrentMotion = mNextMotion;
            mNextMotion = -1;
            return 1;

        case eScrabMotions::Motion_3_Run_45EAB0:
            if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
            {
                if (WallHit_401930(mSpriteScale * FP_FromInteger(30), -ScaleToGridSize(mSpriteScale)))
                {
                    return 0;
                }
                else
                {
                    mCurrentMotion = eScrabMotions::Motion_11_StandToRun_45E9F0;
                    mVelX = -(ScaleToGridSize(mSpriteScale) / FP_FromDouble(3.5));
                    mNextMotion = -1;
                    return 1;
                }
            }
            else
            {
                if (WallHit_401930(mSpriteScale * FP_FromInteger(30), ScaleToGridSize(mSpriteScale)))
                {
                    return 0;
                }
                else
                {
                    mVelX = (ScaleToGridSize(mSpriteScale) / FP_FromDouble(3.5));
                    mCurrentMotion = eScrabMotions::Motion_11_StandToRun_45E9F0;
                    mNextMotion = -1;
                    return 1;
                }
            }
            break;

        case eScrabMotions::Motion_2_Walk_45E730:
            if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
            {
                if (WallHit_401930(mSpriteScale * FP_FromInteger(30), -ScaleToGridSize(mSpriteScale)))
                {
                    return 0;
                }
                else
                {
                    mCurrentMotion = eScrabMotions::Motion_10_StandToWalk_45E670;
                    mVelX = -(ScaleToGridSize(mSpriteScale) / FP_FromInteger(7));
                    mNextMotion = -1;
                    return 1;
                }
            }
            else
            {
                if (WallHit_401930(mSpriteScale * FP_FromInteger(30), ScaleToGridSize(mSpriteScale)))
                {
                    return 0;
                }
                else
                {
                    mVelX = (ScaleToGridSize(mSpriteScale) / FP_FromInteger(7));
                    mCurrentMotion = eScrabMotions::Motion_10_StandToWalk_45E670;
                    mNextMotion = -1;
                    return 1;
                }
            }
            break;

        case eScrabMotions::Motion_1_Stand_45E620:
            ToStand();
            return 1;

        case eScrabMotions::Motion_7_HopMidair_45F1A0:
            if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
            {
                if (WallHit_401930(mSpriteScale * FP_FromInteger(30), -ScaleToGridSize(mSpriteScale)))
                {
                    return 0;
                }
            }
            else
            {
                if (WallHit_401930(mSpriteScale * FP_FromInteger(30), ScaleToGridSize(mSpriteScale)))
                {
                    return 0;
                }
            }
            mCurrentMotion = eScrabMotions::Motion_6_HopBegin_45F3C0;
            mNextMotion = -1;
            return 1;

        default:
            return 0;
    }
}

const relive::SfxDefinition sScrabSfx_4CF798[9] = {
    {0, 39, 60, 55, 0, 0, 0},
    {0, 39, 61, 70, 0, 0, 0},
    {0, 39, 62, 80, 0, 0, 0},
    {0, 39, 63, 80, 0, 0, 0},
    {0, 39, 64, 60, -127, 127, 0},
    {0, 39, 66, 90, 0, 0, 0},
    {0, 39, 67, 50, -511, 0, 0},
    {0, 39, 67, 50, 0, 511, 0},
    {0, 39, 68, 110, -1791, -1791, 0}};

s32 Scrab::Scrab_SFX(ScrabSounds soundId, s32 /*vol*/, s32 pitch, s16 applyDirection)
{
    s32 volumeLeft = 0;
    s32 volumeRight = 0;

    auto defaultSndIdxVol = sScrabSfx_4CF798[static_cast<s32>(soundId)].field_C_default_volume;
    if (mSpriteScale == FP_FromInteger(1))
    {
        volumeRight = defaultSndIdxVol;
    }
    else
    {
        volumeRight = defaultSndIdxVol / 2;
    }

    CameraPos direction = gMap.GetDirection(
        mCurrentLevel,
        mCurrentPath,
        mXPos,
        mYPos);
    PSX_RECT worldRect;
    gMap.Get_Camera_World_Rect(direction, &worldRect);
    volumeLeft = volumeRight;
    if (applyDirection)
    {
        switch (direction)
        {
            case CameraPos::eCamCurrent_0:
            {
                break;
            }
            case CameraPos::eCamTop_1:
            case CameraPos::eCamBottom_2:
            {
                volumeLeft = FP_GetExponent(FP_FromInteger(defaultSndIdxVol * 2) / FP_FromInteger(3));
                volumeRight = volumeLeft;
            }
            break;
            case CameraPos::eCamLeft_3:
            {
                FP percentHowFar = (FP_FromInteger(worldRect.w) - mXPos) / FP_FromInteger(368);
                volumeLeft = volumeRight - FP_GetExponent(percentHowFar * FP_FromInteger(volumeRight - (volumeRight / 3)));
                volumeRight -= FP_GetExponent(percentHowFar * FP_FromInteger(volumeRight));
                break;
            }
            case CameraPos::eCamRight_4:
            {
                FP percentHowFar = (mXPos - FP_FromInteger(worldRect.x)) / FP_FromInteger(368);
                volumeLeft = volumeRight - FP_GetExponent(percentHowFar * FP_FromInteger(volumeRight));
                volumeRight -= FP_GetExponent(percentHowFar * FP_FromInteger(volumeRight - (volumeRight / 3)));
                break;
            }
            default:
                return 0;
        }
    }
    return SFX_SfxDefinition_Play_477330(sScrabSfx_4CF798[static_cast<s32>(soundId)],
                                         static_cast<s16>(volumeLeft),
                                         static_cast<s16>(volumeRight),
                                         static_cast<s16>(pitch),
                                         static_cast<s16>(pitch));
}

void Scrab::ToJump()
{
    BaseAliveGameObjectLastLineYPos = mYPos;

    if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
    {
        mVelX = (mSpriteScale * FP_FromDouble(-5.7));
    }
    else
    {
        mVelX = (mSpriteScale * FP_FromDouble(5.7));
    }

    mVelY = (mSpriteScale * FP_FromDouble(-9.6));
    mYPos += mVelY;
    VOnTrapDoorOpen();
    mCurrentMotion = eScrabMotions::Motion_13_RunJumpBegin_45F5D0;
    BaseAliveGameObjectCollisionLine = nullptr;
}

void Scrab::MoveOnLine()
{
    const FP oldX = mXPos;
    if (BaseAliveGameObjectCollisionLine)
    {
        BaseAliveGameObjectCollisionLine = BaseAliveGameObjectCollisionLine->MoveOnLine(
            &mXPos,
            &mYPos,
            mVelX);

        if (BaseAliveGameObjectCollisionLine)
        {
            if (mLiftPoint)
            {
                if (BaseAliveGameObjectCollisionLine->mLineType != eLineTypes::eDynamicCollision_32 && BaseAliveGameObjectCollisionLine->mLineType != eLineTypes::eBackgroundDynamicCollision_36)
                {
                    const auto oldMotion = mCurrentMotion;
                    VOnTrapDoorOpen();
                    mCurrentMotion = oldMotion;
                }
            }
            else
            {
                if (BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eDynamicCollision_32 || BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eBackgroundDynamicCollision_36)
                {
                    PlatformCollide();
                }
            }
        }
        else
        {
            VOnTrapDoorOpen();
            BaseAliveGameObjectLastLineYPos = mYPos;
            mCurrentMotion = eScrabMotions::Motion_15_ToFall_45F180;
            mXPos = mVelX + oldX;
            field_128 = FP_FromInteger(1);
        }
    }
    else
    {
        mCurrentMotion = eScrabMotions::Motion_9_JumpToFall_45EFD0;
        BaseAliveGameObjectLastLineYPos = mYPos;
    }
}

s16 Scrab::VOnSameYLevel(BaseAnimatedWithPhysicsGameObject* pObj)
{
    const PSX_RECT ourRect = VGetBoundingRect();
    const PSX_RECT otherRect = pObj->VGetBoundingRect();

    const FP k10Scaled = (FP_FromInteger(10) * mSpriteScale);
    if (FP_FromInteger(ourRect.y) <= (FP_FromInteger(otherRect.h) - k10Scaled) && ourRect.y >= otherRect.y)
    {
        return TRUE;
    }

    if (ourRect.h <= otherRect.h)
    {
        if (FP_FromInteger(ourRect.h) >= (k10Scaled + FP_FromInteger(otherRect.y)))
        {
            return TRUE;
        }
    }

    if (ourRect.y >= otherRect.y)
    {
        if (ourRect.h <= otherRect.h)
        {
            return TRUE;
        }

        if (ourRect.y > otherRect.y)
        {
            return FALSE;
        }
    }

    if (ourRect.h < otherRect.h)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

Scrab* Scrab::FindScrabToFight()
{
    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->mBaseGameObjectTypeId == ReliveTypes::eScrab)
        {
            Scrab* pOther = static_cast<Scrab*>(pObj);
            if (pOther != this
                && !pOther->BrainIs(&Scrab::Brain_Death_45CB80)
                && VOnSameYLevel(pOther)
                && gMap.Is_Point_In_Current_Camera(
                    pOther->mCurrentLevel,
                    pOther->mCurrentPath,
                    pOther->mXPos,
                    pOther->mYPos,
                    0)
                && gMap.Is_Point_In_Current_Camera(
                    mCurrentLevel,
                    mCurrentPath,
                    mXPos,
                    mYPos,
                    0))
            {
                return pOther;
            }
        }
    }
    return nullptr;
}


s16 Scrab::FindAbeOrMud()
{
    if (CanSeeAbe(sActiveHero) && sActiveHero->mHealth > FP_FromInteger(0) && sActiveHero->mSpriteScale == mSpriteScale && !WallHit_401930(sActiveHero->mXPos - mXPos, mSpriteScale * FP_FromInteger(35)))
    {
        field_120_pTarget = sActiveHero;
        sActiveHero->mBaseGameObjectRefCount++;
        return 1;
    }

    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        BaseGameObject* pObjIter = gBaseGameObjects->ItemAt(i);
        if (!pObjIter)
        {
            break;
        }

        if (pObjIter->mBaseGameObjectFlags.Get(BaseGameObject::eIsBaseAliveGameObject_Bit6))
        {
            BaseAliveGameObject* pObj = static_cast<BaseAliveGameObject*>(pObjIter);

            if (pObj->mBaseGameObjectTypeId == ReliveTypes::eRingOrLiftMud || pObj->mBaseGameObjectTypeId == ReliveTypes::eMudokon || pObj->mBaseGameObjectTypeId == ReliveTypes::SlingMud)
            {
                if (CanSeeAbe(pObj) && pObj->mHealth > FP_FromInteger(0) && pObj->mSpriteScale == mSpriteScale && !WallHit_401930(pObj->mXPos - mXPos, mSpriteScale * FP_FromInteger(35)))
                {
                    field_120_pTarget = pObj;
                    field_120_pTarget->mBaseGameObjectRefCount++;
                    return 1;
                }
            }
        }
    }
    return 0;
}

s16 Scrab::CanSeeAbe(BaseAliveGameObject* pObj)
{
    if (pObj->mSpriteScale != mSpriteScale)
    {
        return 0;
    }

    if (pObj == sActiveHero)
    {
        if (sActiveHero->mCurrentMotion == eAbeMotions::Motion_66_LedgeHang_428D90
            || sActiveHero->mCurrentMotion == eAbeMotions::Motion_68_LedgeHangWobble_428E50)
        {
            return VOnSameYLevel(pObj);
        }
    }
    return pObj->mYPos > (mYPos - mSpriteScale * FP_FromInteger(35));
}

void Scrab::Motion_0_Empty_45E3D0()
{
    if (sNumCamSwappers_507668 <= 0)
    {
        if (sControlledCharacter == this)
        {
            mCurrentMotion = mPreviousMotion;
            if (mLiftPoint)
            {
                // TODO: Is type of mLiftPoint too low?
                static_cast<LiftPoint*>(mLiftPoint)->field_12C_bMoving |= 1u;
            }
        }
        else
        {
            mCurrentMotion = mPreviousMotion;
            if (mLiftPoint)
            {
                mXPos = FP_FromInteger((BaseAliveGameObjectCollisionLine->mRect.x + BaseAliveGameObjectCollisionLine->mRect.w) / 2);
                mYPos = FP_FromInteger(BaseAliveGameObjectCollisionLine->mRect.y);
            }
        }
    }
}

void Scrab::Motion_1_Stand_45E620()
{
    ToNextMotion();

    if (gMap.GetDirection(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos)
        >= CameraPos::eCamCurrent_0)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eType11, this, 0, 0);
    }
}

const FP sWalkVelTable_4BC788[22] = {
    FP_FromDouble(1.570587158203125),
    FP_FromDouble(1.5780487060546875),
    FP_FromDouble(1.5256805419921875),
    FP_FromDouble(1.5651397705078125),
    FP_FromDouble(1.5966339111328125),
    FP_FromDouble(3.694122314453125),
    FP_FromDouble(3.9574737548828125),
    FP_FromDouble(3.9715423583984375),
    FP_FromDouble(3.3073272705078125),
    FP_FromDouble(2.629669189453125),
    FP_FromDouble(2.1679229736328125),
    FP_FromDouble(2.0220794677734375),
    FP_FromDouble(1.804046630859375),
    FP_FromDouble(1.6065216064453125),
    FP_FromDouble(2.6418609619140625),
    FP_FromDouble(2.685821533203125),
    FP_FromDouble(2.673095703125),
    FP_FromDouble(2.691070556640625),
    FP_FromDouble(1.3966827392578125),
    FP_FromDouble(1.376983642578125),
    FP_FromDouble(1.3767242431640625),
    FP_FromDouble(2.1646728515625)};

void Scrab::Motion_2_Walk_45E730()
{
    switch (mAnim.mCurrentFrame)
    {
        case 3:
        case 8:
        case 13:
        case 18:
            Scrab_SFX(ScrabSounds::eWalk1_6, Math_RandomRange(40, 50), 0x7FFF, 1);
            break;

        default:
            break;
    }

    FP vel = {};
    if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
    {
        vel = -sWalkVelTable_4BC788[mAnim.mCurrentFrame];
    }
    else
    {
        vel = sWalkVelTable_4BC788[mAnim.mCurrentFrame];
    }

    mVelX = (mSpriteScale * vel);

    const FP xOff = mVelX + FP_FromRaw(mVelX.fpValue / 2);
    if (WallHit_401930(mSpriteScale * FP_FromInteger(30), xOff))
    {
        ToStand();
        return;
    }

    MoveOnLine();

    if (mCurrentMotion == eScrabMotions::Motion_2_Walk_45E730)
    {
        if (mAnim.mCurrentFrame == 5 || mAnim.mCurrentFrame == 15)
        {
            if (mNextMotion == eScrabMotions::Motion_1_Stand_45E620
                || mNextMotion == eScrabMotions::Motion_4_Turn_45EF30
                || mNextMotion == eScrabMotions::Motion_16_Stamp_45F920
                || mNextMotion == eScrabMotions::Motion_19_Unused_45F9D0
                || mNextMotion == eScrabMotions::Motion_22_Shriek_45FB00
                || mNextMotion == eScrabMotions::Motion_20_HowlBegin_45FA60
                || mNextMotion == eScrabMotions::Motion_7_HopMidair_45F1A0
                || mNextMotion == eScrabMotions::Motion_27_AttackLunge_45FDF0
                || mNextMotion == eScrabMotions::Motion_28_LegKick_45FF60)
            {
                mCurrentMotion = eScrabMotions::Motion_12_WalkToStand_45E930;
            }
        }
        else if (mAnim.mCurrentFrame == 7 || mAnim.mCurrentFrame == 18)
        {
            if (mNextMotion == eScrabMotions::Motion_3_Run_45EAB0)
            {
                mCurrentMotion = eScrabMotions::Motion_3_Run_45EAB0;
                mNextMotion = -1;
            }

            if (!(field_188_flags & 2))
            {
                MapFollowMe_401D30(1);
                field_188_flags |= 2u;
            }
        }
        else
        {
            field_188_flags &= ~2u;
        }

        if (gMap.GetDirection(
                mCurrentLevel,
                mCurrentPath,
                mXPos,
                mYPos)
            >= CameraPos::eCamCurrent_0)
        {
            MusicController::static_PlayMusic(MusicController::MusicTypes::eType11, this, 0, 0);
        }
    }
}

const FP sRunVelTable_4BC800[14] = {
    FP_FromDouble(9.498733520507812),
    FP_FromDouble(9.733749389648438),
    FP_FromDouble(9.584152221679688),
    FP_FromDouble(9.141021728515625),
    FP_FromDouble(9.099105834960938),
    FP_FromDouble(9.576034545898438),
    FP_FromDouble(9.327011108398438),
    FP_FromDouble(8.654891967773438),
    FP_FromDouble(5.2911224365234375),
    FP_FromDouble(4.2801666259765625),
    FP_FromDouble(3.446380615234375),
    FP_FromDouble(3.3974456787109375),
    FP_FromDouble(3.8137969970703125),
    FP_FromDouble(5.15692138671875)};


void Scrab::Motion_3_Run_45EAB0()
{
    switch (mAnim.mCurrentFrame)
    {
        case 0:
        case 5:
        case 7:
        case 12:
            Scrab_SFX(ScrabSounds::eWalk1_6, Math_RandomRange(40, 50), 0x7FFF, 1);
            break;
        default:
            break;
    }

    FP vel = {};
    if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
    {
        vel = -sRunVelTable_4BC800[mAnim.mCurrentFrame];
    }
    else
    {
        vel = sRunVelTable_4BC800[mAnim.mCurrentFrame];
    }
    mVelX = (mSpriteScale * vel);

    if (WallHit_401930(mSpriteScale * FP_FromInteger(30), mVelX))
    {
        ToStand();
    }
    else
    {
        MoveOnLine();

        if (mCurrentMotion == eScrabMotions::Motion_3_Run_45EAB0)
        {
            if (field_120_pTarget == sActiveHero)
            {
                const PSX_RECT objRect = field_120_pTarget->VGetBoundingRect();
                const PSX_RECT bRect = VGetBoundingRect();

                if (bRect.x <= objRect.w
                    && bRect.w >= objRect.x
                    && bRect.h >= objRect.y
                    && bRect.y <= objRect.h
                    && field_120_pTarget->mSpriteScale == mSpriteScale)
                {
                    if (VIsFacingMe(field_120_pTarget))
                    {
                        if (field_120_pTarget->VTakeDamage(this))
                        {
                            SfxPlayMono(relive::SoundEffects::KillEffect, 0);
                            Mudokon_SFX(MudSounds::eKnockbackOuch_10, 0, 0, field_120_pTarget);
                        }
                    }
                }
            }

            if (mAnim.mCurrentFrame != 3 && mAnim.mCurrentFrame != 10)
            {
                field_188_flags &= ~2u;
            }
            else
            {
                if (mNextMotion == eScrabMotions::Motion_2_Walk_45E730)
                {
                    mCurrentMotion = eScrabMotions::Motion_2_Walk_45E730;
                    mNextMotion = -1;
                }

                if (mNextMotion == eScrabMotions::Motion_1_Stand_45E620
                    || mNextMotion == eScrabMotions::Motion_4_Turn_45EF30
                    || mNextMotion == eScrabMotions::Motion_7_HopMidair_45F1A0
                    || mNextMotion == eScrabMotions::Motion_27_AttackLunge_45FDF0
                    || mNextMotion == eScrabMotions::Motion_19_Unused_45F9D0
                    || mNextMotion == eScrabMotions::Motion_20_HowlBegin_45FA60)
                {
                    mCurrentMotion = eScrabMotions::Motion_5_RunToStand_45ED90;
                }

                if (mNextMotion == eScrabMotions::Motion_23_ScrabBattleAnim_45FBA0)
                {
                    ToStand();
                    mCurrentMotion = eScrabMotions::Motion_23_ScrabBattleAnim_45FBA0;
                    mNextMotion = -1;
                }

                if (!(field_188_flags & 2))
                {
                    MapFollowMe_401D30(1);
                    field_188_flags |= 2;
                }
            }

            if (gMap.GetDirection(
                    mCurrentLevel,
                    mCurrentPath,
                    mXPos,
                    mYPos)
                >= CameraPos::eCamCurrent_0)
            {
                MusicController::static_PlayMusic(MusicController::MusicTypes::eType12, this, 0, 0);
            }
        }
    }
}

void Scrab::Motion_4_Turn_45EF30()
{
    if (mAnim.mCurrentFrame == 5
        || mAnim.mCurrentFrame == 9
        || mAnim.mCurrentFrame == 11)
    {
        Scrab_SFX(ScrabSounds::eWalk1_6, Math_RandomRange(40, 50), 0x7FFF, 1);
    }

    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mAnim.mFlags.Toggle(AnimFlags::eBit5_FlipX);

        if (ToNextMotion())
        {
            mAnim.Set_Animation_Data(mAnim.mFrameTableOffset, nullptr);
        }
        else
        {
            ToStand();
        }
    }
}

const FP sRunToStandVelTable_4BC838[10] = {
    FP_FromDouble(7.49066162109375),
    FP_FromDouble(6.9139404296875),
    FP_FromDouble(3.889495849609375),
    FP_FromDouble(2.368621826171875),
    FP_FromDouble(0.7609710693359375),
    FP_FromDouble(0.64093017578125),
    FP_FromDouble(0.7079010009765625),
    FP_FromDouble(0.7738494873046875),
    FP_FromDouble(0.7254180908203125),
    };

void Scrab::Motion_5_RunToStand_45ED90()
{
    if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
    {
        mVelX = (mSpriteScale * -sRunToStandVelTable_4BC838[mAnim.mCurrentFrame]);
    }
    else
    {
        mVelX = (mSpriteScale * sRunToStandVelTable_4BC838[mAnim.mCurrentFrame]);
    }

    if (WallHit_401930(mSpriteScale * FP_FromInteger(30), mVelX))
    {
        ToStand();
        return;
    }

    MoveOnLine();

    if (mCurrentMotion == eScrabMotions::Motion_5_RunToStand_45ED90)
    {
        if (field_120_pTarget)
        {
            const PSX_RECT bObjRect = field_120_pTarget->VGetBoundingRect();
            const PSX_RECT bRect = VGetBoundingRect();

            if (bObjRect.x <= bRect.w
                && bObjRect.w >= bRect.x
                && bObjRect.h >= bRect.y
                && bObjRect.y <= bRect.h
                && field_120_pTarget->mSpriteScale == mSpriteScale)
            {
                if (VIsFacingMe(field_120_pTarget))
                {
                    if (field_120_pTarget->VTakeDamage(this))
                    {
                        SfxPlayMono(relive::SoundEffects::KillEffect, 0);
                        Mudokon_SFX(MudSounds::eKnockbackOuch_10, 0, 0, field_120_pTarget);
                    }
                }
            }
        }

        if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            MapFollowMe_401D30(1);

            if (!ToNextMotion())
            {
                ToStand();
            }
        }
    }
}

const FP sHopBeginVelTable_4BC860[4] = {
    FP_FromDouble(9.889144897460938),
    FP_FromDouble(10.10455322265625),
    FP_FromDouble(10.10455322265625),
    FP_FromDouble(9.889144897460938) };

void Scrab::Motion_6_HopBegin_45F3C0()
{
    EventBroadcast(kEventNoise, this);

    if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
    {
        mVelX = (mSpriteScale * -sHopBeginVelTable_4BC860[mAnim.mCurrentFrame]);
    }
    else
    {
        mVelX = (mSpriteScale * sHopBeginVelTable_4BC860[mAnim.mCurrentFrame]);
    }

    if (!WallHit_401930(mSpriteScale * FP_FromInteger(30), mVelX))
    {
        mXPos += mVelX;

        if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            SFX_Play_Pitch(relive::SoundEffects::PickupItem, 50, -800);

            BaseAliveGameObjectLastLineYPos = mYPos;

            if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
            {
                mVelX = (mSpriteScale * -FP_FromDouble(5.7));
            }
            else
            {
                mVelX = (mSpriteScale * FP_FromDouble(5.7));
            }

            mVelY = (mSpriteScale * -FP_FromDouble(9.6));
            mYPos += mVelY;
            VOnTrapDoorOpen();
            mCurrentMotion = eScrabMotions::Motion_7_HopMidair_45F1A0;
            BaseAliveGameObjectCollisionLine = nullptr;
        }
    }
}

const FP sHopMidAirVelTable_4BC870[8] = {
    FP_FromDouble(9.458358764648438),
    FP_FromDouble(8.812179565429688),
    FP_FromDouble(8.11224365234375),
    FP_FromDouble(7.560302734375),
    FP_FromDouble(7.0954742431640625),
    FP_FromDouble(6.7178192138671875),
    FP_FromDouble(6.4272918701171875),
    FP_FromDouble(6.2239532470703125)};

void Scrab::Motion_7_HopMidair_45F1A0()
{
    EventBroadcast(kEventNoise, this);

    if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
    {
        mVelX = (mSpriteScale * -sHopMidAirVelTable_4BC870[mAnim.mCurrentFrame]);
    }
    else
    {
        mVelX = (mSpriteScale * sHopMidAirVelTable_4BC870[mAnim.mCurrentFrame]);
    }

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    if (WallHit_401930(mSpriteScale * FP_FromInteger(30), mVelX))
    {
        ToStand();
        mCurrentMotion = eScrabMotions::Motion_9_JumpToFall_45EFD0;
    }
    else
    {
        if (InAirCollision_4019C0(&pLine, &hitX, &hitY, FP_FromDouble(1.8)))
        {
            switch (pLine->mLineType)
            {
                case eLineTypes::eFloor_0:
                case eLineTypes::eBackgroundFloor_4:
                case eLineTypes::eDynamicCollision_32:
                case eLineTypes::eBackgroundDynamicCollision_36:
                {
                    ToStand();
                    BaseAliveGameObjectCollisionLine = pLine;
                    mCurrentMotion = eScrabMotions::Motion_8_HopLand_45F500;

                    PSX_RECT bRect = VGetBoundingRect();
                    bRect.y += 5;
                    bRect.h += 5;

                    VOnCollisionWith(
                        {bRect.x, bRect.y},
                        {bRect.w, bRect.h},
                        ObjListPlatforms_50766C,
                        (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection_401C10);

                    mXPos = hitX;
                    mYPos = hitY;
                    return;
                }

                case eLineTypes::eWallLeft_1:
                case eLineTypes::eWallRight_2:
                    mVelX = (-mVelX / FP_FromInteger(2));
                    return;

                default:
                    break;
            }
        }

        if (mYPos - BaseAliveGameObjectLastLineYPos > FP_FromInteger(5))
        {
            field_128 = FP_FromDouble(1.25);
            mCurrentMotion = eScrabMotions::Motion_9_JumpToFall_45EFD0;
        }
    }
}

const FP sLandVelXTable_4BC890[4] = {
    FP_FromDouble(6.1077423095703125),
    FP_FromDouble(6.078704833984375),
    FP_FromDouble(6.1367950439453125),
    FP_FromDouble(6.2820892333984375)};

void Scrab::Motion_8_HopLand_45F500()
{
    if (mAnim.mCurrentFrame == 0)
    {
        Scrab_SFX(ScrabSounds::eHitCollision_4, 0, 0x7FFF, 1);
    }

    EventBroadcast(kEventNoise, this);

    if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
    {
        mVelX = (mSpriteScale * -sLandVelXTable_4BC890[mAnim.mCurrentFrame]);
    }
    else
    {
        mVelX = (mSpriteScale * sLandVelXTable_4BC890[mAnim.mCurrentFrame]);
    }

    if (WallHit_401930(mSpriteScale * FP_FromInteger(30), mVelX))
    {
        ToStand();
        return;
    }

    MoveOnLine();

    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToStand();
    }
}

void Scrab::Motion_9_JumpToFall_45EFD0()
{
    if (mVelX > FP_FromInteger(0))
    {
        mVelX -= (mSpriteScale * field_128);
        if (mVelX < FP_FromInteger(0))
        {
            mVelX = FP_FromInteger(0);
        }
    }
    else if (mVelX < FP_FromInteger(0))
    {
        mVelX += (mSpriteScale * field_128);
        if (mVelX > FP_FromInteger(0))
        {
            mVelX = FP_FromInteger(0);
        }
    }

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    if (InAirCollision_4019C0(&pLine, &hitX, &hitY, FP_FromDouble(1.8)))
    {
        switch (pLine->mLineType)
        {
            case eLineTypes::eFloor_0:
            case eLineTypes::eBackgroundFloor_4:
            case eLineTypes::eDynamicCollision_32:
            case eLineTypes::eBackgroundDynamicCollision_36:
            {
                ToStand();

                BaseAliveGameObjectCollisionLine = pLine;

                PSX_RECT bRect = VGetBoundingRect();
                bRect.y += 5;
                bRect.h += 5;

                VOnCollisionWith(
                    {bRect.x, bRect.y},
                    {bRect.y, bRect.w},
                    ObjListPlatforms_50766C,
                    (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection_401C10);

                mXPos = hitX;
                mYPos = hitY;
                MapFollowMe_401D30(1);
                break;
            }

            case eLineTypes::eWallLeft_1:
            case eLineTypes::eWallRight_2:
                mVelX = (-mVelX / FP_FromInteger(2));
                break;

            default:
                return;
        }
    }
}

const FP sStandToWalkVelTable_4BC778[3] = {
    FP_FromDouble(1.1195068359375),
    FP_FromDouble(2.3691558837890625),
    FP_FromDouble(3.2076568603515625)};

void Scrab::Motion_10_StandToWalk_45E670()
{
    FP vel = sStandToWalkVelTable_4BC778[mAnim.mCurrentFrame];

    if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
    {
        mVelX = (mSpriteScale * -vel);
    }
    else
    {
        mVelX = (mSpriteScale * vel);
    }

    if (WallHit_401930(mSpriteScale * FP_FromInteger(30), mVelX))
    {
        ToStand();
    }
    else
    {
        MoveOnLine();

        if (mCurrentMotion == eScrabMotions::Motion_10_StandToWalk_45E670)
        {
            if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                mCurrentMotion = eScrabMotions::Motion_2_Walk_45E730;
            }
        }
    }
}

const FP sStandToRunVel_4BC7F0[3] = {
    FP_FromDouble(2.29766845703125),
    FP_FromDouble(2.6963653564453125),
    FP_FromDouble(3.8794708251953125)};

void Scrab::Motion_11_StandToRun_45E9F0()
{
    if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
    {
        mVelX = (mSpriteScale * -sStandToRunVel_4BC7F0[mAnim.mCurrentFrame]);
    }
    else
    {
        mVelX = (mSpriteScale * sStandToRunVel_4BC7F0[mAnim.mCurrentFrame]);
    }

    if (WallHit_401930(mSpriteScale * FP_FromInteger(30), mVelX))
    {
        ToStand();
    }
    else
    {
        MoveOnLine();

        if (mCurrentMotion == eScrabMotions::Motion_11_StandToRun_45E9F0)
        {
            if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                mCurrentMotion = eScrabMotions::Motion_3_Run_45EAB0;
            }
        }
    }
}

const FP sWalkToStandVel_4BC7E0[3] = {
    FP_FromDouble(1.041473388671875),
    FP_FromDouble(3.2936248779296875),
    FP_FromDouble(2.8589019775390625)};

void Scrab::Motion_12_WalkToStand_45E930()
{
    if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
    {
        mVelX = (mSpriteScale * -sWalkToStandVel_4BC7E0[mAnim.mCurrentFrame]);
    }
    else
    {
        mVelX = (mSpriteScale * sWalkToStandVel_4BC7E0[mAnim.mCurrentFrame]);
    }

    if (WallHit_401930(mSpriteScale * FP_FromInteger(30), mVelX))
    {
        ToStand();
    }

    MoveOnLine();

    if (mCurrentMotion == eScrabMotions::Motion_12_WalkToStand_45E930 && mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame) && !ToNextMotion())
    {
        ToStand();
    }
}

const FP sRunJumpBeginVelTable_4BC8A0[14] = {
    FP_FromDouble(13.124908447265625),
    FP_FromDouble(12.364852905273438),
    FP_FromDouble(11.541610717773438),
    FP_FromDouble(10.89239501953125),
    FP_FromDouble(9.345687866210938),
    FP_FromDouble(8.901473999023438),
    FP_FromDouble(8.55975341796875),
    FP_FromDouble(8.320602416992188),
    FP_FromInteger(0),
    FP_FromInteger(0),
    FP_FromInteger(0),
    FP_FromInteger(0),
    FP_FromInteger(0),
    FP_FromInteger(0)};

void Scrab::Motion_13_RunJumpBegin_45F5D0()
{
    if (mAnim.mCurrentFrame == 1)
    {
        SFX_Play_Pitch(relive::SoundEffects::PickupItem, 50, -800);
    }

    EventBroadcast(kEventNoise, this);

    if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
    {
        mVelX = (mSpriteScale * -sRunJumpBeginVelTable_4BC8A0[mAnim.mCurrentFrame]);
    }
    else
    {
        mVelX = (mSpriteScale * sRunJumpBeginVelTable_4BC8A0[mAnim.mCurrentFrame]);
    }

    if (WallHit_401930(mSpriteScale * FP_FromInteger(30), mVelX))
    {
        ToStand();
        mCurrentMotion = eScrabMotions::Motion_9_JumpToFall_45EFD0;
    }
    else
    {
        PathLine* pLine = nullptr;
        FP hitX = {};
        FP hitY = {};
        if (InAirCollision_4019C0(&pLine, &hitX, &hitY, FP_FromDouble(1.8)))
        {
            switch (pLine->mLineType)
            {
                case eLineTypes::eFloor_0:
                case eLineTypes::eBackgroundFloor_4:
                case eLineTypes::eDynamicCollision_32:
                case eLineTypes::eBackgroundDynamicCollision_36:
                {
                    ToStand();
                    BaseAliveGameObjectCollisionLine = pLine;
                    mCurrentMotion = eScrabMotions::Motion_14_RunJumpEnd_45F850;

                    PSX_RECT bRect = VGetBoundingRect();
                    bRect.y += 5;
                    bRect.h += 5;

                    VOnCollisionWith(
                        {bRect.x, bRect.y},
                        {bRect.w, bRect.h},
                        ObjListPlatforms_50766C,
                        (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection_401C10);

                    mXPos = hitX;
                    mYPos = hitY;
                    return;
                }

                case eLineTypes::eWallLeft_1:
                case eLineTypes::eWallRight_2:
                    mVelX = (-mVelX / FP_FromInteger(2));
                    return;

                default:
                    break;
            }
        }

        if (mYPos - BaseAliveGameObjectLastLineYPos > FP_FromInteger(5))
        {
            field_128 = FP_FromDouble(1.25);
            mCurrentMotion = eScrabMotions::Motion_9_JumpToFall_45EFD0;
        }

        if (gMap.GetDirection(
                mCurrentLevel,
                mCurrentPath,
                mXPos,
                mYPos)
            >= CameraPos::eCamCurrent_0)
        {
            MusicController::static_PlayMusic(MusicController::MusicTypes::eType11, this, 0, 0);
        }
    }
}

const FP sRunJumpEndVelTable_4BC8C0[4] = {
    FP_FromDouble(4.1839141845703125),
    FP_FromDouble(4.1497650146484375),
    FP_FromDouble(4.21807861328125),
    FP_FromDouble(4.3889617919921875)};

void Scrab::Motion_14_RunJumpEnd_45F850()
{
    EventBroadcast(kEventNoise, this);

    if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
    {
        mVelX = (mSpriteScale * -sRunJumpEndVelTable_4BC8C0[mAnim.mCurrentFrame]);
    }
    else
    {
        mVelX = (mSpriteScale * sRunJumpEndVelTable_4BC8C0[mAnim.mCurrentFrame]);
    }

    if (WallHit_401930(mSpriteScale * FP_FromInteger(30), mVelX))
    {
        ToStand();
        return;
    }

    MoveOnLine();

    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        Scrab_SFX(ScrabSounds::eHitCollision_4, 0, 0x7FFF, 1);
        if (!ToNextMotion())
        {
            ToStand();
        }
    }
}

void Scrab::Motion_15_ToFall_45F180()
{
    Motion_9_JumpToFall_45EFD0();

    if (mCurrentMotion == eScrabMotions::Motion_1_Stand_45E620)
    {
        mCurrentMotion = eScrabMotions::Motion_8_HopLand_45F500;
    }
}

void Scrab::Motion_16_Stamp_45F920()
{
    if (mAnim.mCurrentFrame == 9)
    {
        Scrab_SFX(ScrabSounds::eHitCollision_4, 0, 0x7FFF, 1);
        SFX_Play_Pitch(relive::SoundEffects::KillEffect, 60, Math_RandomRange(-255, 255));
    }

    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        Scrab_SFX(ScrabSounds::eHowl_0, 60, 511, 1);
        ToNextMotion();
    }

    if (gMap.GetDirection(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos)
        >= CameraPos::eCamCurrent_0)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eType12, this, 0, 0);
    }
}

void Scrab::Motion_17_Empty_45F9C0()
{
    // Empty
}

void Scrab::Motion_18_GetEaten_45FF70()
{
    if (!BaseAliveGameObjectCollisionLine)
    {
        Motion_9_JumpToFall_45EFD0();

        if (mCurrentMotion != eScrabMotions::Motion_18_GetEaten_45FF70)
        {
            mCurrentMotion = eScrabMotions::Motion_18_GetEaten_45FF70;
        }
    }
}

void Scrab::Motion_19_Unused_45F9D0()
{
    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (!ToNextMotion())
        {
            ToStand();
        }
    }

    if (gMap.GetDirection(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos)
        >= CameraPos::eCamCurrent_0)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eType11, this, 0, 0);
    }
}

void Scrab::Motion_20_HowlBegin_45FA60()
{
    if (mAnim.mCurrentFrame == 2)
    {
        Scrab_SFX(ScrabSounds::eYell_8, 0, Math_RandomRange(-1600, -900), 1);
    }

    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (mNextMotion != -1)
        {
            mCurrentMotion = eScrabMotions::Motion_21_HowlEnd_45FAF0;
        }
    }

    if (gMap.GetDirection(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos)
        >= CameraPos::eCamCurrent_0)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eType12, this, 0, 0);
    }
}

void Scrab::Motion_21_HowlEnd_45FAF0()
{
    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToNextMotion();
    }
}

void Scrab::Motion_22_Shriek_45FB00()
{
    if (mAnim.mCurrentFrame == 4)
    {
        Scrab_SFX(ScrabSounds::eHowl_0, 0, 0x7FFF, 1);
    }

    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (!ToNextMotion())
        {
            ToStand();
        }
    }

    if (gMap.GetDirection(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos)
        >= CameraPos::eCamCurrent_0)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eType11, this, 0, 0);
    }
}

void Scrab::Motion_23_ScrabBattleAnim_45FBA0()
{
    if (mAnim.mCurrentFrame == 0)
    {
        field_14C = Scrab_SFX(ScrabSounds::eShredding_5, 100, Math_RandomRange(-600, 200), 1);
    }

    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToNextMotion();
    }

    if (gMap.GetDirection(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos)
        >= CameraPos::eCamCurrent_0)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eType12, this, 0, 0);
    }
}

void Scrab::Motion_24_FeedToGulp_45FC30()
{
    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (mNextMotion == eScrabMotions::Motion_25_ToFeed_45FCE0)
        {
            field_188_flags &= ~4u;
            mCurrentMotion = eScrabMotions::Motion_25_ToFeed_45FCE0;
            mNextMotion = -1;
        }
        else
        {
            ToStand();
        }
    }

    if (gMap.GetDirection(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos)
        >= CameraPos::eCamCurrent_0)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eType11, this, 0, 0);
    }
}

const u32 sFeedVelTable_4BC8D0[12] = {
    380127,
    526666,
    388557,
    295550,
    135669,
    8971,
    4294918160,
    4294924422,
    4294964851,
    0,
    4294966370,
    4294966453};


void Scrab::Motion_25_ToFeed_45FCE0()
{
    if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
    {
        mVelX = (mSpriteScale * -FP_FromRaw(sFeedVelTable_4BC8D0[mAnim.mCurrentFrame]));
    }
    else
    {
        mVelX = (mSpriteScale * FP_FromRaw(sFeedVelTable_4BC8D0[mAnim.mCurrentFrame]));
    }

    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eScrabMotions::Motion_26_Feed_45FDA0;
    }

    if ((field_188_flags >> 2) & 1)
    {
        if (WallHit_401930(mSpriteScale * FP_FromInteger(30), mVelX))
        {
            ToStand();
        }
        else
        {
            MoveOnLine();
        }
    }
}

void Scrab::Motion_26_Feed_45FDA0()
{
    if (mAnim.mCurrentFrame == 0)
    {
        if (Math_RandomRange(0, 100) >= 50)
        {
            SfxPlayMono(relive::SoundEffects::Eating2, 0);
        }
        else
        {
            SfxPlayMono(relive::SoundEffects::Eating1, 0);
        }
    }

    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eScrabMotions::Motion_24_FeedToGulp_45FC30;
    }
}

void Scrab::Motion_27_AttackLunge_45FDF0()
{
    if (field_120_pTarget)
    {
        const PSX_RECT objRect = field_120_pTarget->VGetBoundingRect();
        const PSX_RECT bRect = VGetBoundingRect();

        if (objRect.x <= bRect.w
            && objRect.w >= bRect.x
            && objRect.h >= bRect.y
            && objRect.y <= bRect.h
            && field_120_pTarget->mSpriteScale == mSpriteScale)
        {
            if (VIsFacingMe(field_120_pTarget))
            {
                if (field_120_pTarget->VTakeDamage(this))
                {
                    SfxPlayMono(relive::SoundEffects::KillEffect, 0);
                    Mudokon_SFX(MudSounds::eKnockbackOuch_10, 0, 0, field_120_pTarget);
                }

                field_120_pTarget->VTakeDamage(this);
            }
        }

        if (mAnim.mCurrentFrame == 4)
        {
            Scrab_SFX(ScrabSounds::eHowl_0, 0, 0x7FFF, 1);
        }
    }

    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToStand();
    }

    if (gMap.GetDirection(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos)
        >= CameraPos::eCamCurrent_0)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eType12, this, 0, 0);
    }
}

void Scrab::Motion_28_LegKick_45FF60()
{
    Motion_27_AttackLunge_45FDF0();
}


void Scrab::Motion_29_DeathBegin_45FFA0()
{
    if (!BaseAliveGameObjectCollisionLine)
    {
        Motion_9_JumpToFall_45EFD0();

        if (mCurrentMotion != eScrabMotions::Motion_29_DeathBegin_45FFA0)
        {
            mCurrentMotion = eScrabMotions::Motion_29_DeathBegin_45FFA0;
        }
    }

    s32 tableVal = 0;
    if (mAnim.mCurrentFrame == -1)
    {
        tableVal = 0;
    }
    else
    {
        tableVal = sFeedVelTable_4BC8D0[mAnim.mCurrentFrame];
    }

    if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
    {
        mVelX = (mSpriteScale * -FP_FromRaw(tableVal));
    }
    else
    {
        mVelX = (mSpriteScale * FP_FromRaw(tableVal));
    }

    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eScrabMotions::Motion_18_GetEaten_45FF70;
    }
}

s16 Scrab::Brain_Fighting_45C370()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(Options::eDead);
    }

    Scrab* pFighter = field_11C_pFight_target;
    if (pFighter && (pFighter->mBaseGameObjectFlags.Get(BaseGameObject::eDead) || !VOnSameYLevel(field_11C_pFight_target)))
    {
        field_11C_pFight_target->mBaseGameObjectRefCount--;
        field_188_flags &= ~1u;
        field_11C_pFight_target = nullptr;
        mNextMotion = eScrabMotions::Motion_1_Stand_45E620;
        SetBrain(&Scrab::Brain_WalkAround_460D80); // patrol ??
        return 0;
    }

    switch (field_110_brain_sub_state)
    {
        case 0:
            if (sNumCamSwappers_507668 > 0)
            {
                return field_110_brain_sub_state;
            }

            if (VIsFacingMe(field_11C_pFight_target))
            {
                if (!VIsObjNearby(ScaleToGridSize(mSpriteScale) * FP_FromInteger(8), field_11C_pFight_target))
                {
                    if (field_11C_pFight_target->mCurrentMotion == eScrabMotions::Motion_20_HowlBegin_45FA60)
                    {
                        return field_110_brain_sub_state;
                    }
                    mNextMotion = eScrabMotions::Motion_20_HowlBegin_45FA60;
                    return 7;
                }
                else
                {
                    mNextMotion = eScrabMotions::Motion_4_Turn_45EF30;
                    return 2;
                }
            }
            else
            {
                mNextMotion = eScrabMotions::Motion_4_Turn_45EF30;
                return 1;
            }
            break;

        case 1:
            if (mCurrentMotion != eScrabMotions::Motion_4_Turn_45EF30)
            {
                if (mNextMotion == eScrabMotions::Motion_4_Turn_45EF30 || mHealth < FP_FromInteger(0))
                {
                    return field_110_brain_sub_state;
                }
                mNextMotion = eScrabMotions::Motion_4_Turn_45EF30;
                return field_110_brain_sub_state;
            }

            if (!mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                return field_110_brain_sub_state;
            }

            if (field_11C_pFight_target->mXPos != mXPos ||
                (field_11C_pFight_target->mAnim.mFlags.Get(AnimFlags::eBit5_FlipX) != mAnim.mFlags.Get(AnimFlags::eBit5_FlipX)) ||
                field_11C_pFight_target->mCurrentMotion != eScrabMotions::Motion_4_Turn_45EF30)
            {
                mNextMotion = eScrabMotions::Motion_1_Stand_45E620;
                return 0;
            }
            else
            {
                mNextMotion = eScrabMotions::Motion_2_Walk_45E730;
                return 3;
            }
            break;

        case 2:
            if (mCurrentMotion != eScrabMotions::Motion_4_Turn_45EF30)
            {
                if (mNextMotion == eScrabMotions::Motion_4_Turn_45EF30 || mHealth < FP_FromInteger(0))
                {
                    return field_110_brain_sub_state;
                }
                mNextMotion = eScrabMotions::Motion_4_Turn_45EF30;
                return field_110_brain_sub_state;
            }

            if (!mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                return field_110_brain_sub_state;
            }

            mNextMotion = eScrabMotions::Motion_2_Walk_45E730;
            return 3;

        case 3:
        {
            FP xpos = {};
            if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
            {
                if (WallHit_401930(mSpriteScale * FP_FromInteger(30), -(ScaleToGridSize(mSpriteScale) * FP_FromInteger(2))))
                {
                    mNextMotion = eScrabMotions::Motion_4_Turn_45EF30;
                    return 4;
                }

                if (Check_IsOnEndOfLine_4021A0(1, 2))
                {
                    mNextMotion = eScrabMotions::Motion_4_Turn_45EF30;
                    return 4;
                }

                if (!VIsObjNearby(ScaleToGridSize(mSpriteScale) * FP_FromInteger(10), field_11C_pFight_target))
                {
                    mNextMotion = eScrabMotions::Motion_4_Turn_45EF30;
                    return 4;
                }
                xpos = mXPos - FP_FromInteger(50);
            }
            else
            {
                if (WallHit_401930(mSpriteScale * FP_FromInteger(30), ScaleToGridSize(mSpriteScale) * FP_FromInteger(2)))
                {
                    mNextMotion = eScrabMotions::Motion_4_Turn_45EF30;
                    return 4;
                }

                if (Check_IsOnEndOfLine_4021A0(0, 2))
                {
                    mNextMotion = eScrabMotions::Motion_4_Turn_45EF30;
                    return 4;
                }

                if (!VIsObjNearby(ScaleToGridSize(mSpriteScale) * FP_FromInteger(10), field_11C_pFight_target))
                {
                    mNextMotion = eScrabMotions::Motion_4_Turn_45EF30;
                    return 4;
                }
                xpos = mXPos + FP_FromInteger(50);
            }

            if (gMap.Is_Point_In_Current_Camera(
                    mCurrentLevel,
                    mCurrentPath,
                    xpos,
                    mYPos,
                    0))
            {
                return field_110_brain_sub_state;
            }

            mNextMotion = eScrabMotions::Motion_4_Turn_45EF30;
            return 4;
        }

        case 4:
            if (mCurrentMotion == eScrabMotions::Motion_4_Turn_45EF30)
            {
                if (!mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
                {
                    return field_110_brain_sub_state;
                }
                mNextMotion = eScrabMotions::Motion_20_HowlBegin_45FA60;
                return 7;
            }
            else
            {
                if (mNextMotion == eScrabMotions::Motion_4_Turn_45EF30 || mHealth < FP_FromInteger(0))
                {
                    return field_110_brain_sub_state;
                }
                mNextMotion = eScrabMotions::Motion_4_Turn_45EF30;
                return field_110_brain_sub_state;
            }
            break;

        case 5:
            if (mCurrentMotion != eScrabMotions::Motion_19_Unused_45F9D0
                || !mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                return field_110_brain_sub_state;
            }
            mNextMotion = eScrabMotions::Motion_1_Stand_45E620;
            field_118_timer = sGnFrame + 30;
            return 6;

        case 6:
            if (field_118_timer > static_cast<s32>(sGnFrame))
            {
                return field_110_brain_sub_state;
            }
            mNextMotion = eScrabMotions::Motion_20_HowlBegin_45FA60;
            return 7;

        case 7:
            if (mCurrentMotion != eScrabMotions::Motion_20_HowlBegin_45FA60
                || !mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                return field_110_brain_sub_state;
            }
            mNextMotion = eScrabMotions::Motion_1_Stand_45E620;
            field_118_timer = sGnFrame + 20;
            return 8;

        case 8:
            if (field_118_timer > static_cast<s32>(sGnFrame))
            {
                return field_110_brain_sub_state;
            }
            mNextMotion = eScrabMotions::Motion_22_Shriek_45FB00;
            return 9;

        case 9:
            if (mCurrentMotion != eScrabMotions::Motion_22_Shriek_45FB00
                || !mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                return field_110_brain_sub_state;
            }
            field_188_flags |= 1u;
            mNextMotion = 1;
            return 10;

        case 10:
            if (!(field_11C_pFight_target->field_188_flags & 1))
            {
                return field_110_brain_sub_state;
            }
            mNextMotion = eScrabMotions::Motion_3_Run_45EAB0;
            return 11;

        case 11:
            if (!VIsObjNearby(ScaleToGridSize(mSpriteScale), field_11C_pFight_target)
                || !BaseAliveGameObjectCollisionLine
                || !field_11C_pFight_target->BaseAliveGameObjectCollisionLine)
            {
                return field_110_brain_sub_state;
            }

            MapFollowMe_401D30(TRUE);

            if (field_11C_pFight_target->mAnim.mFlags.Get(AnimFlags::eBit3_Render))
            {
                mAnim.mFlags.Clear(AnimFlags::eBit3_Render);
                mHealth = FP_FromInteger(0);
            }
            mCurrentMotion = eScrabMotions::Motion_23_ScrabBattleAnim_45FBA0;
            field_188_flags &= ~1u;
            field_118_timer = sGnFrame + 40;
            return 12;

        case 12:
            if (field_118_timer > static_cast<s32>(sGnFrame))
            {
                return field_110_brain_sub_state;
            }

            SND_Stop_Channels_Mask(field_14C);

            field_14C = 0;

            Scrab_SFX(ScrabSounds::eDeathHowl_1, 0, -1571, 1);
            Scrab_SFX(ScrabSounds::eYell_8, 0, -1571, 1);
            Environment_SFX_42A220(EnvironmentSfx::eHitGroundSoft_6, 0, -383, 0);
            field_11C_pFight_target->mBaseGameObjectRefCount--;
            if (mAnim.mFlags.Get(AnimFlags::eBit3_Render))
            {
                field_11C_pFight_target = nullptr;
                mCurrentMotion = eScrabMotions::Motion_1_Stand_45E620;
                field_118_timer = sGnFrame + 20;
                return 13;
            }
            else
            {
                mAnim.mFlags.Set(AnimFlags::eBit3_Render);
                mXPos = field_11C_pFight_target->mXPos;
                field_11C_pFight_target = nullptr;
                SetBrain(&Scrab::Brain_Death_45CB80);
                mCurrentMotion = eScrabMotions::Motion_29_DeathBegin_45FFA0;
                field_130_unused = 2;
                field_118_timer = sGnFrame + 90;
                return 0;
            }
            break;

        case 13:
            if (field_118_timer > static_cast<s32>(sGnFrame))
            {
                return field_110_brain_sub_state;
            }
            mNextMotion = eScrabMotions::Motion_16_Stamp_45F920;
            field_118_timer = sGnFrame + 75;
            return 14;

        case 14:
            if (field_118_timer > static_cast<s32>(sGnFrame))
            {
                return field_110_brain_sub_state;
            }
            mNextMotion = eScrabMotions::Motion_20_HowlBegin_45FA60;
            field_118_timer = sGnFrame + 45;
            return 15;

        case 15:
            if (field_118_timer <= static_cast<s32>(sGnFrame))
            {
                mNextMotion = 1;
                SetBrain(&Scrab::Brain_WalkAround_460D80); // patrol ??
                return 0;
            }
            return field_110_brain_sub_state;

        default:
            return field_110_brain_sub_state;
    }
}

s16 Scrab::Brain_BatDeath_45CA60()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    switch (field_110_brain_sub_state)
    {
        case Brain_BatDeath::eStartHowling_0:
            if (mCurrentMotion == eScrabMotions::Motion_1_Stand_45E620)
            {
                mNextMotion = eScrabMotions::Motion_20_HowlBegin_45FA60;
                field_118_timer = sGnFrame + 45;
                return Brain_BatDeath::eWalkAway_1;
            }
            break;

        case Brain_BatDeath::eWalkAway_1:
            if (field_118_timer <= static_cast<s32>(sGnFrame))
            {
                mNextMotion = eScrabMotions::Motion_2_Walk_45E730;
                return Brain_BatDeath::eStartShrieking_2;
            }
            break;

        case Brain_BatDeath::eStartShrieking_2:
            if (mCurrentMotion == eScrabMotions::Motion_2_Walk_45E730)
            {
                mNextMotion = eScrabMotions::Motion_22_Shriek_45FB00;
                return Brain_BatDeath::eTurnAround_3;
            }
            break;

        case Brain_BatDeath::eTurnAround_3:
            if (mCurrentMotion == eScrabMotions::Motion_22_Shriek_45FB00
                && mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                mNextMotion = eScrabMotions::Motion_4_Turn_45EF30;
                return Brain_BatDeath::eDie_4;
            }
            break;

        case Brain_BatDeath::eDie_4:
            if (mCurrentMotion == eScrabMotions::Motion_4_Turn_45EF30)
            {
                if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
                {
                    SetBrain(&Scrab::Brain_Death_45CB80);
                    field_130_unused = 2;
                    field_118_timer = sGnFrame + 90;
                    mCurrentMotion = eScrabMotions::Motion_29_DeathBegin_45FFA0;
                }
            }
            break;

        default:
            break;
    }

    return field_110_brain_sub_state;
}

s16 Scrab::Brain_Death_45CB80()
{
    if (field_118_timer > static_cast<s32>(sGnFrame) && field_118_timer < static_cast<s32>(sGnFrame) + 80)
    {
        mSpriteScale -= FP_FromDouble(0.008);
        mRGB.r -= 2;
        mRGB.g -= 2;
        mRGB.b -= 2;
    }

    if (static_cast<s32>(sGnFrame) < field_118_timer - 24)
    {
        DeathSmokeEffect(true);
    }

    if (field_118_timer < static_cast<s32>(sGnFrame))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    return 100;
}

s16 Scrab::Brain_ChasingEnemy_Real_45CC90()
{
    
    return 0;
}

s16 Scrab::Brain_ChasingEnemy_45CC90()
{
    // 0 to 17
    if (field_110_brain_sub_state == 8)
    {
        //return Brain_ChasingEnemy_Real_45CC90();
    }

    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(Options::eDead);
    }

    field_11C_pFight_target = FindScrabToFight();
    if (field_11C_pFight_target)
    {
        field_120_pTarget->mBaseGameObjectRefCount--;
        field_120_pTarget = nullptr;
        field_11C_pFight_target->mBaseGameObjectRefCount++;
        SetBrain(&Scrab::Brain_Fighting_45C370);
        mNextMotion = eScrabMotions::Motion_1_Stand_45E620;
        return 0;
    }

    if (field_120_pTarget->mBaseGameObjectFlags.Get(BaseGameObject::eDead)
        || (field_13C_spotting_timer <= static_cast<s32>(sGnFrame)
            && !CanSeeAbe(field_120_pTarget)
            && field_120_pTarget->mHealth > FP_FromInteger(0)
            && gMap.Is_Point_In_Current_Camera(
                mCurrentLevel,
                mCurrentPath,
                mXPos,
                mYPos,
                0)))
    {
        bool bCloseToEdge = false;
        if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
        {
            if (!Check_IsOnEndOfLine_4021A0(1, 2))
            {
                bCloseToEdge = true;
            }
        }
        else
        {
            if (!Check_IsOnEndOfLine_4021A0(0, 2))
            {
                bCloseToEdge = true;
            }
        }

        if (bCloseToEdge)
        {
            field_120_pTarget->mBaseGameObjectRefCount--;
            field_120_pTarget = nullptr;
            mNextMotion = eScrabMotions::Motion_1_Stand_45E620;
            SetBrain(&Scrab::Brain_WalkAround_460D80);
            return 0;
        }
    }

    if (EventGet(kEventAbeOhm))
    {
        mNextMotion = eScrabMotions::Motion_20_HowlBegin_45FA60;
        return 18;
    }

    const FP kGridSize = ScaleToGridSize(mSpriteScale);

    switch (field_110_brain_sub_state)
    {
        case 0:
            if (sNumCamSwappers_507668 > 0)
            {
                return field_110_brain_sub_state;
            }
            field_13C_spotting_timer = sGnFrame + field_138_spotting_abe_delay;
            return 1;

        case 1:
        {
            if ((!CanSeeAbe(field_120_pTarget)
                 && gMap.Is_Point_In_Current_Camera(
                     mCurrentLevel,
                     mCurrentPath,
                     mXPos,
                     mYPos,
                     0))
                || WallHit_401930(mSpriteScale * FP_FromInteger(30), field_120_pTarget->mXPos - mXPos)
                || field_120_pTarget->mHealth <= FP_FromInteger(0))
            {
                if (mCurrentMotion == eScrabMotions::Motion_1_Stand_45E620 && Math_NextRandom() < 26u && (field_188_flags & 0x20) && (sGnFrame - field_140_last_shriek_timer) > 60)
                {
                    field_140_last_shriek_timer = sGnFrame;
                    mNextMotion = eScrabMotions::Motion_22_Shriek_45FB00;
                    return 17;
                }

                return 1;
            }

            if (!VIsFacingMe(field_120_pTarget))
            {
                if (gMap.Is_Point_In_Current_Camera(
                        mCurrentLevel,
                        mCurrentPath,
                        mXPos,
                        mYPos,
                        0))
                {
                    mNextMotion = eScrabMotions::Motion_4_Turn_45EF30;
                }
                else
                {
                    mCurrentMotion = eScrabMotions::Motion_4_Turn_45EF30;
                    mNextMotion = -1;
                    MapFollowMe_401D30(TRUE);
                }
                return 2;
            }

            if (mLiftPoint)
            {
                if (mLiftPoint->mBaseGameObjectTypeId == ReliveTypes::eLiftPoint)
                {
                    auto pLiftPoint = static_cast<LiftPoint*>(mLiftPoint);
                    if (!pLiftPoint->OnAnyFloor())
                    {
                        return 4;
                    }
                }
            }

            if (VIsObjNearby(kGridSize, field_120_pTarget) && VOnSameYLevel(field_120_pTarget))
            {
                mNextMotion = eScrabMotions::Motion_28_LegKick_45FF60;
                return 10;
            }

            relive::Path_TLV* pTlv = nullptr;
            if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
            {
                pTlv = gMap.TLV_Get_At_446260(
                    FP_GetExponent(mXPos - kGridSize),
                    FP_GetExponent(mYPos),
                    FP_GetExponent(mXPos - kGridSize),
                    FP_GetExponent(mYPos),
                    ReliveTypes::eEnemyStopper);
            }
            else
            {
                pTlv = gMap.TLV_Get_At_446260(
                    FP_GetExponent(mXPos + kGridSize),
                    FP_GetExponent(mYPos),
                    FP_GetExponent(mXPos + kGridSize),
                    FP_GetExponent(mYPos),
                    ReliveTypes::eEnemyStopper);
            }

            if (!pTlv)
            {
                pTlv = gMap.TLV_Get_At_446260(
                    FP_GetExponent(mXPos),
                    FP_GetExponent(mYPos),
                    FP_GetExponent(mXPos),
                    FP_GetExponent(mYPos),
                    ReliveTypes::eEnemyStopper);
            }

            auto pStopper = static_cast<relive::Path_EnemyStopper*>(pTlv);
            BaseAliveGameObjectPathTLV = pTlv;
            if (pStopper)
            {
                const bool bLeft = pStopper->mStopDirection == relive::Path_EnemyStopper::StopDirection::Left && field_120_pTarget->mXPos < mXPos;
                const bool bRight = pStopper->mStopDirection == relive::Path_EnemyStopper::StopDirection::Right && field_120_pTarget->mXPos > mXPos;
                const bool bBoth = pStopper->mStopDirection == relive::Path_EnemyStopper::StopDirection::Both;
                const bool bSwitchOn = SwitchStates_Get(pStopper->mSwitchId) ? true : false;
                if ((bLeft || bRight || bBoth) && !bSwitchOn)
                {
                    if (mCurrentMotion == eScrabMotions::Motion_1_Stand_45E620 && Math_NextRandom() < 26u && (field_188_flags & 0x20) && (sGnFrame - field_140_last_shriek_timer) > 60)
                    {
                        field_140_last_shriek_timer = sGnFrame;
                        mNextMotion = eScrabMotions::Motion_22_Shriek_45FB00;
                        return 17;
                    }
                    return 1;
                }
            }

            if (VIsObjNearby(kGridSize * FP_FromDouble(1.5), field_120_pTarget) && VOnSameYLevel(field_120_pTarget))
            {
                mNextMotion = eScrabMotions::Motion_27_AttackLunge_45FDF0;
                return 10;
            }

            field_118_timer = sGnFrame + field_114_attack_delay;
            return 8;
        }

        case 2:
            if (mCurrentMotion != eScrabMotions::Motion_4_Turn_45EF30)
            {
                if (mNextMotion == eScrabMotions::Motion_4_Turn_45EF30 || mHealth < FP_FromInteger(0))
                {
                    return field_110_brain_sub_state;
                }
                mNextMotion = eScrabMotions::Motion_4_Turn_45EF30;
                return field_110_brain_sub_state;
            }

            if (!mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                return field_110_brain_sub_state;
            }

            mNextMotion = eScrabMotions::Motion_1_Stand_45E620;
            return 1;

        case 3:
        {
            if (mVelX > FP_FromInteger(0))
            {
                const s16 x_exp = FP_GetExponent(mXPos);
                const s32 xSnapped = (x_exp & 0xFC00) + SnapToXGrid(mSpriteScale, x_exp & 0x3FF);
                if (abs(xSnapped - x_exp) < 6 && Check_IsOnEndOfLine_4021A0(0, 1))
                {
                    if (field_120_pTarget->mYPos - mYPos < FP_FromInteger(5)
                        || gMap.TLV_Get_At_446260(
                            FP_GetExponent(mXPos + kGridSize),
                            FP_GetExponent(mYPos + FP_FromInteger(10)),
                            FP_GetExponent(mXPos + kGridSize),
                            FP_GetExponent(mYPos + FP_FromInteger(10)),
                            ReliveTypes::eScrabNoFall))
                    {
                        if (!Check_IsOnEndOfLine_4021A0(0, 4))
                        {
                            ToJump();
                            mNextMotion = -1;
                            return 7;
                        }
                    }
                }

                BaseAliveGameObjectPathTLV = gMap.TLV_Get_At_446260(
                    FP_GetExponent(mXPos + (kGridSize * FP_FromInteger(2))),
                    FP_GetExponent(mYPos),
                    FP_GetExponent(mXPos + (kGridSize * FP_FromInteger(2))),
                    FP_GetExponent(mYPos),
                    ReliveTypes::eEnemyStopper);

                auto pStopper = static_cast<relive::Path_EnemyStopper*>(BaseAliveGameObjectPathTLV);
                if (pStopper)
                {
                    if (pStopper->mStopDirection == relive::Path_EnemyStopper::StopDirection::Right || pStopper->mStopDirection == relive::Path_EnemyStopper::StopDirection::Both)
                    {
                        if (!SwitchStates_Get(pStopper->mSwitchId))
                        {
                            mNextMotion = eScrabMotions::Motion_1_Stand_45E620;
                            return 1;
                        }
                    }
                }
            }
            else if (mVelX < FP_FromInteger(0))
            {
                const s16 x_exp = FP_GetExponent(mXPos);
                const s32 xSnapped = (x_exp & 0xFC00) + SnapToXGrid(mSpriteScale, x_exp & 0x3FF);
                if (abs(xSnapped - x_exp) < 6 && Check_IsOnEndOfLine_4021A0(1, 1))
                {
                    if ((field_120_pTarget->mYPos - mYPos < FP_FromInteger(5))
                        || gMap.TLV_Get_At_446260(
                            FP_GetExponent(mXPos - kGridSize),
                            FP_GetExponent(mYPos + FP_FromInteger(10)),
                            FP_GetExponent(mXPos - kGridSize),
                            FP_GetExponent(mYPos + FP_FromInteger(10)),
                            ReliveTypes::eScrabNoFall))
                    {
                        if (!Check_IsOnEndOfLine_4021A0(1, 4))
                        {
                            ToJump();
                            mNextMotion = -1;
                            return 7;
                        }
                    }
                }

                BaseAliveGameObjectPathTLV = gMap.TLV_Get_At_446260(
                    FP_GetExponent(mXPos - (kGridSize * FP_FromInteger(2))),
                    FP_GetExponent(mYPos),
                    FP_GetExponent(mXPos - (kGridSize * FP_FromInteger(2))),
                    FP_GetExponent(mYPos),
                    ReliveTypes::eEnemyStopper);

                auto pStopper = static_cast<relive::Path_EnemyStopper*>(BaseAliveGameObjectPathTLV);
                if (pStopper)
                {
                    if (pStopper->mStopDirection == relive::Path_EnemyStopper::StopDirection::Left || pStopper->mStopDirection == relive::Path_EnemyStopper::StopDirection::Both)
                    {
                        if (!SwitchStates_Get(pStopper->mSwitchId))
                        {
                            mNextMotion = eScrabMotions::Motion_1_Stand_45E620;
                            return 1;
                        }
                    }
                }
            }

            if (!VIsFacingMe(field_120_pTarget))
            {
                if (gMap.Is_Point_In_Current_Camera(
                        mCurrentLevel,
                        mCurrentPath,
                        mXPos,
                        mYPos,
                        0))
                {
                    mNextMotion = eScrabMotions::Motion_4_Turn_45EF30;
                }
                else
                {
                    mCurrentMotion = eScrabMotions::Motion_4_Turn_45EF30;
                    mNextMotion = -1;
                    MapFollowMe_401D30(TRUE);
                }
                return 2;
            }

            if (VIsObjNearby(kGridSize * FP_FromInteger(3), field_120_pTarget)
                && mCurrentMotion == eScrabMotions::Motion_3_Run_45EAB0
                && VOnSameYLevel(field_120_pTarget))
            {
                if (WallHit_401930(mSpriteScale * FP_FromInteger(30), field_120_pTarget->mXPos - mXPos))
                {
                    mNextMotion = eScrabMotions::Motion_1_Stand_45E620;
                    return 1;
                }
                mNextMotion = eScrabMotions::Motion_27_AttackLunge_45FDF0;
                return 10;
            }

            auto pLiftPoint = static_cast<LiftPoint*>(mLiftPoint);
            if (pLiftPoint && mLiftPoint->mBaseGameObjectTypeId == ReliveTypes::eLiftPoint && !pLiftPoint->OnAnyFloor())
            {
                mNextMotion = eScrabMotions::Motion_1_Stand_45E620;
                return 4;
            }

            if (BaseAliveGameObjectCollisionLine)
            {
                return field_110_brain_sub_state;
            }
            return 5;
        }

        case 4:
            if (VIsObjNearby(kGridSize, field_120_pTarget) && VOnSameYLevel(field_120_pTarget))
            {
                mNextMotion = eScrabMotions::Motion_28_LegKick_45FF60;
                return 10;
            }

            if (mLiftPoint)
            {
                auto pLiftPoint = static_cast<LiftPoint*>(mLiftPoint);
                if (!pLiftPoint->OnAnyFloor())
                {
                    return field_110_brain_sub_state;
                }
            }
            return 11;

        case 5:
        case 6:
            if (mCurrentMotion != eScrabMotions::Motion_1_Stand_45E620)
            {
                return field_110_brain_sub_state;
            }
            mNextMotion = -1;
            return 8;

        case 7:
            if (mCurrentMotion != eScrabMotions::Motion_14_RunJumpEnd_45F850)
            {
                return field_110_brain_sub_state;
            }
            return 11;

        case 8:
        {
            if (!CanSeeAbe(field_120_pTarget)
                && gMap.Is_Point_In_Current_Camera(
                    mCurrentLevel,
                    mCurrentPath,
                    mXPos,
                    mYPos,
                    0))
            {
                mNextMotion = eScrabMotions::Motion_1_Stand_45E620;
                return 1;
            }

            if (!VIsFacingMe(field_120_pTarget))
            {
                mNextMotion = eScrabMotions::Motion_4_Turn_45EF30;
                return 9;
            }

            if (VIsObjNearby(kGridSize * FP_FromDouble(1.5), field_120_pTarget) && VOnSameYLevel(field_120_pTarget))
            {
                if (WallHit_401930(mSpriteScale * FP_FromInteger(30), field_120_pTarget->mXPos - mXPos))
                {
                    return 1;
                }

                if (!VIsObjNearby(kGridSize, field_120_pTarget))
                {
                    mNextMotion = eScrabMotions::Motion_27_AttackLunge_45FDF0;
                    return 10;
                }

                mNextMotion = eScrabMotions::Motion_28_LegKick_45FF60;
                return 10;
            }

            if (field_118_timer > static_cast<s32>(sGnFrame))
            {
                if (mCurrentMotion != eScrabMotions::Motion_1_Stand_45E620 || !mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
                {
                    return field_110_brain_sub_state;
                }
                mNextMotion = eScrabMotions::Motion_22_Shriek_45FB00;
                return field_110_brain_sub_state;
            }

            BaseAliveGameObjectPathTLV = gMap.TLV_Get_At_446260(
                FP_GetExponent(mXPos),
                FP_GetExponent(mYPos),
                FP_GetExponent(mXPos),
                FP_GetExponent(mYPos),
                ReliveTypes::eEnemyStopper);

            auto pStopper = static_cast<relive::Path_EnemyStopper*>(BaseAliveGameObjectPathTLV);

            if (pStopper)
            {
                const bool bLeft = pStopper->mStopDirection == relive::Path_EnemyStopper::StopDirection::Left && field_120_pTarget->mXPos < mXPos;
                const bool bRight = pStopper->mStopDirection == relive::Path_EnemyStopper::StopDirection::Right && field_120_pTarget->mXPos > mXPos;
                const bool bBoth = pStopper->mStopDirection == relive::Path_EnemyStopper::StopDirection::Both;
                const bool bSwitchOn = SwitchStates_Get(pStopper->mSwitchId) ? true : false;
                if ((bLeft || bRight || bBoth) && !bSwitchOn)
                {
                    return 1;
                }
            }

            if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
            {
                if (Check_IsOnEndOfLine_4021A0(1, 1))
                {
                    mNextMotion = eScrabMotions::Motion_7_HopMidair_45F1A0;
                    return 6;
                }
            }
            else
            {
                if (Check_IsOnEndOfLine_4021A0(0, 1))
                {
                    mNextMotion = eScrabMotions::Motion_7_HopMidair_45F1A0;
                    return 6;
                }
            }

            mNextMotion = eScrabMotions::Motion_3_Run_45EAB0;
            return 3;
        }

        case 9:
            if (mCurrentMotion != eScrabMotions::Motion_4_Turn_45EF30)
            {
                if (mNextMotion == eScrabMotions::Motion_4_Turn_45EF30 || mHealth < FP_FromInteger(0))
                {
                    return field_110_brain_sub_state;
                }
                mNextMotion = eScrabMotions::Motion_4_Turn_45EF30;
                return field_110_brain_sub_state;
            }

            if (!mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                return field_110_brain_sub_state;
            }

            mNextMotion = eScrabMotions::Motion_1_Stand_45E620;
            return 8;

        case 10:
            if ((mCurrentMotion == eScrabMotions::Motion_27_AttackLunge_45FDF0 || mCurrentMotion == eScrabMotions::Motion_28_LegKick_45FF60) && mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                if (field_120_pTarget->mHealth <= FP_FromInteger(0))
                {
                    if (!VIsFacingMe(field_120_pTarget))
                    {
                        mNextMotion = eScrabMotions::Motion_4_Turn_45EF30;
                        return 14;
                    }

                    if (!VIsObjNearby(kGridSize, field_120_pTarget))
                    {
                        mNextMotion = eScrabMotions::Motion_2_Walk_45E730;
                        return 13;
                    }

                    field_188_flags &= ~4u;
                    mNextMotion = eScrabMotions::Motion_16_Stamp_45F920;
                    field_118_timer = sGnFrame + 30;
                    return 15;
                }

                if (!VIsFacingMe(field_120_pTarget))
                {
                    mNextMotion = eScrabMotions::Motion_4_Turn_45EF30;
                    return 12;
                }
                return HandleRunning();
            }
            return field_110_brain_sub_state;

        case 11:
            if (!CanSeeAbe(field_120_pTarget)
                && gMap.Is_Point_In_Current_Camera(
                    mCurrentLevel,
                    mCurrentPath,
                    mXPos,
                    mYPos,
                    0))
            {
                mNextMotion = eScrabMotions::Motion_1_Stand_45E620;
                SetBrain(&Scrab::Brain_WalkAround_460D80);
                return 0;
            }

            if (field_120_pTarget->mHealth <= FP_FromInteger(0))
            {
                mNextMotion = eScrabMotions::Motion_1_Stand_45E620;
                return 1;
            }

            if (!VIsFacingMe(field_120_pTarget))
            {
                mNextMotion = eScrabMotions::Motion_4_Turn_45EF30;
                return 12;
            }

            if (VIsObjNearby(kGridSize * FP_FromDouble(1.5), field_120_pTarget))
            {
                if (WallHit_401930(mSpriteScale * FP_FromInteger(30), field_120_pTarget->mXPos - mXPos))
                {
                    return 1;
                }

                if (VIsObjNearby(kGridSize, field_120_pTarget))
                {
                    mNextMotion = eScrabMotions::Motion_28_LegKick_45FF60;
                    return 10;
                }

                mNextMotion = eScrabMotions::Motion_27_AttackLunge_45FDF0;
                return 10;
            }

            return HandleRunning();


        case 12:
            if (mCurrentMotion != eScrabMotions::Motion_4_Turn_45EF30)
            {
                if (mNextMotion == eScrabMotions::Motion_4_Turn_45EF30 || mHealth < FP_FromInteger(0))
                {
                    return field_110_brain_sub_state;
                }
                mNextMotion = eScrabMotions::Motion_4_Turn_45EF30;
                return field_110_brain_sub_state;
            }

            if (!mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                return field_110_brain_sub_state;
            }

            mNextMotion = eScrabMotions::Motion_1_Stand_45E620;
            return 11;

        case 13:
            if (VIsObjNearby(kGridSize, field_120_pTarget))
            {
                field_188_flags &= ~4u;
                mNextMotion = eScrabMotions::Motion_16_Stamp_45F920;
                field_118_timer = sGnFrame + 30;
                return 15;
            }
            return field_110_brain_sub_state;

        case 14:
            if (mCurrentMotion != eScrabMotions::Motion_4_Turn_45EF30)
            {
                if (mNextMotion == eScrabMotions::Motion_4_Turn_45EF30 || mHealth < FP_FromInteger(0))
                {
                    return field_110_brain_sub_state;
                }
                mNextMotion = eScrabMotions::Motion_4_Turn_45EF30;
                return field_110_brain_sub_state;
            }

            if (!mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                return field_110_brain_sub_state;
            }

            if (!VIsObjNearby(kGridSize, field_120_pTarget))
            {
                mNextMotion = eScrabMotions::Motion_2_Walk_45E730;
                return 13;
            }
            field_188_flags &= ~4u;
            mNextMotion = eScrabMotions::Motion_16_Stamp_45F920;
            field_118_timer = sGnFrame + 30;
            return 15;

        case 15:
            if (field_118_timer <= static_cast<s32>(sGnFrame))
            {
                if (FP_Abs(field_120_pTarget->mYPos - mYPos) >= FP_FromInteger(5))
                {
                    mNextMotion = eScrabMotions::Motion_22_Shriek_45FB00;
                    return 17;
                }

                mNextMotion = eScrabMotions::Motion_25_ToFeed_45FCE0;
                return 16;
            }
            return field_110_brain_sub_state;

        case 16:
            if (mCurrentMotion != eScrabMotions::Motion_24_FeedToGulp_45FC30 || !mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                return field_110_brain_sub_state;
            }
            mNextMotion = eScrabMotions::Motion_25_ToFeed_45FCE0;
            return field_110_brain_sub_state;

        case 17:
        {
            if (mCurrentMotion != eScrabMotions::Motion_22_Shriek_45FB00 || !mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                return field_110_brain_sub_state;
            }

            mNextMotion = eScrabMotions::Motion_1_Stand_45E620;

            if (mLiftPoint && mLiftPoint->mBaseGameObjectTypeId == ReliveTypes::eLiftPoint)
            {
                auto pLiftPoint = static_cast<LiftPoint*>(mLiftPoint);
                if (!pLiftPoint->OnAnyFloor())
                {
                    return 4;
                }
            }
            return 1;
        }

        case 18:
            if (EventGet(kEventAbeOhm))
            {
                return field_110_brain_sub_state;
            }
            mNextMotion = eScrabMotions::Motion_1_Stand_45E620;
            return 1;

        default:
            return field_110_brain_sub_state;
    }
}

s16 Scrab::Brain_Patrol_460020()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(Options::eDead);
    }

    field_11C_pFight_target = FindScrabToFight();
    if (field_11C_pFight_target)
    {
        field_11C_pFight_target->mBaseGameObjectRefCount++;
        mNextMotion = eScrabMotions::Motion_1_Stand_45E620;
        SetBrain(&Scrab::Brain_Fighting_45C370);
        return 0;
    }

    if (FindAbeOrMud())
    {
        if (CanSeeAbe(field_120_pTarget))
        {
            mNextMotion = eScrabMotions::Motion_1_Stand_45E620;
            SetBrain(&Scrab::Brain_ChasingEnemy_45CC90);
            return 0;
        }
        field_120_pTarget->mBaseGameObjectRefCount--;
        field_120_pTarget = nullptr;
    }

    if (EventGet(kEventAbeOhm))
    {
        mNextMotion = eScrabMotions::Motion_20_HowlBegin_45FA60;
        return 9;
    }

    switch (field_110_brain_sub_state)
    {
        case 0:
        {
            if (sNumCamSwappers_507668 > 0)
            {
                return field_110_brain_sub_state;
            }

            auto pLiftPoint = static_cast<LiftPoint*>(mLiftPoint);
            if (pLiftPoint)
            {
                if (pLiftPoint->mBaseGameObjectTypeId == ReliveTypes::eLiftPoint)
                {
                    if (!pLiftPoint->OnAnyFloor())
                    {
                        SetBrain(&Scrab::Brain_WalkAround_460D80);
                        return 0;
                    }
                }
            }

            auto pStopper = static_cast<relive::Path_EnemyStopper*>(gMap.TLV_Get_At_446260(
                FP_GetExponent(mXPos),
                FP_GetExponent(mYPos),
                FP_GetExponent(mXPos),
                FP_GetExponent(mYPos),
                ReliveTypes::eEnemyStopper));
            BaseAliveGameObjectPathTLV = pStopper;

            if (pStopper)
            {
                if (!SwitchStates_Get(pStopper->mSwitchId))
                {
                    if (pStopper->mStopDirection == relive::Path_EnemyStopper::StopDirection::Right && !mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
                    {
                        return 0;
                    }

                    if (pStopper->mStopDirection == relive::Path_EnemyStopper::StopDirection::Left && mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
                    {
                        return 0;
                    }

                    if (pStopper->mStopDirection == relive::Path_EnemyStopper::StopDirection::Both)
                    {
                        return 0;
                    }
                }
            }

            mNextMotion = GetMotionForPatrolType(field_116_patrol_type);

            if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
            {
                BaseAliveGameObjectPathTLV = gMap.TLV_Get_At_446260(
                    FP_GetExponent(mXPos),
                    FP_GetExponent(mYPos),
                    FP_GetExponent(mXPos),
                    FP_GetExponent(mYPos),
                    ReliveTypes::eScrabLeftBound);
                if (BaseAliveGameObjectPathTLV)
                {
                    mNextMotion = eScrabMotions::Motion_4_Turn_45EF30;
                    return 2;
                }
                return 1;
            }
            else
            {
                BaseAliveGameObjectPathTLV = gMap.TLV_Get_At_446260(
                    FP_GetExponent(mXPos),
                    FP_GetExponent(mYPos),
                    FP_GetExponent(mXPos),
                    FP_GetExponent(mYPos),
                    ReliveTypes::eScrabRightBound);
                if (BaseAliveGameObjectPathTLV)
                {
                    mNextMotion = eScrabMotions::Motion_4_Turn_45EF30;
                    return 5;
                }
                return 4;
            }
            break;
        }

        case 1:
            if (mLiftPoint)
            {
                auto pLiftPoint = static_cast<LiftPoint*>(mLiftPoint);
                if (pLiftPoint->mBaseGameObjectTypeId == ReliveTypes::eLiftPoint && !pLiftPoint->OnAnyFloor())
                {
                    mNextMotion = eScrabMotions::Motion_1_Stand_45E620;
                    return 8;
                }
            }

            if (gMap.TLV_Get_At_446260(
                    FP_GetExponent(mXPos - ScaleToGridSize(mSpriteScale)),
                    FP_GetExponent(mYPos),
                    FP_GetExponent(mXPos - ScaleToGridSize(mSpriteScale)),
                    FP_GetExponent(mYPos),
                    ReliveTypes::eScrabLeftBound))
            {
                mNextMotion = eScrabMotions::Motion_4_Turn_45EF30;
                return 2;
            }
            return field_110_brain_sub_state;

        case 2:
            if (mLiftPoint)
            {
                auto pLiftPoint = static_cast<LiftPoint*>(mLiftPoint);
                if (pLiftPoint->mBaseGameObjectTypeId == ReliveTypes::eLiftPoint && !pLiftPoint->OnAnyFloor())
                {
                    mNextMotion = eScrabMotions::Motion_1_Stand_45E620;
                    return 8;
                }
            }

            if (mCurrentMotion == eScrabMotions::Motion_4_Turn_45EF30 && mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                mNextMotion = eScrabMotions::Motion_1_Stand_45E620;
                field_118_timer = sGnFrame + Math_RandomRange(field_144_left_min_delay, field_146_left_max_delay);
                return 3;
            }
            return field_110_brain_sub_state;

        case 3:
            if (mLiftPoint)
            {
                auto pLiftPoint = static_cast<LiftPoint*>(mLiftPoint);
                if (pLiftPoint->mBaseGameObjectTypeId == ReliveTypes::eLiftPoint && !pLiftPoint->OnAnyFloor())
                {
                    mNextMotion = eScrabMotions::Motion_1_Stand_45E620;
                    return 8;
                }
            }

            if (field_118_timer > static_cast<s32>(sGnFrame))
            {
                return field_110_brain_sub_state;
            }

            if (Math_NextRandom() < 30u)
            {
                mNextMotion = eScrabMotions::Motion_20_HowlBegin_45FA60;
                field_118_timer = sGnFrame + 30;
                return 7;
            }

            mNextMotion = GetMotionForPatrolType(field_116_patrol_type);
            return 4;

        case 4:
            if (mLiftPoint)
            {
                auto pLiftPoint = static_cast<LiftPoint*>(mLiftPoint);
                if (pLiftPoint->mBaseGameObjectTypeId == ReliveTypes::eLiftPoint && !pLiftPoint->OnAnyFloor())
                {
                    mNextMotion = eScrabMotions::Motion_1_Stand_45E620;
                    return 8;
                }
            }

            if (gMap.TLV_Get_At_446260(
                    FP_GetExponent(ScaleToGridSize(mSpriteScale) + mXPos),
                    FP_GetExponent(mYPos),
                    FP_GetExponent(ScaleToGridSize(mSpriteScale) + mXPos),
                    FP_GetExponent(mYPos),
                    ReliveTypes::eScrabRightBound))
            {
                mNextMotion = eScrabMotions::Motion_4_Turn_45EF30;
                return 5;
            }
            return field_110_brain_sub_state;

        case 5:
            if (mLiftPoint)
            {
                auto pLiftPoint = static_cast<LiftPoint*>(mLiftPoint);
                if (pLiftPoint->mBaseGameObjectTypeId == ReliveTypes::eLiftPoint && !pLiftPoint->OnAnyFloor())
                {
                    mNextMotion = eScrabMotions::Motion_1_Stand_45E620;
                    return 8;
                }
            }

            if (mCurrentMotion == eScrabMotions::Motion_4_Turn_45EF30 && mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                mNextMotion = eScrabMotions::Motion_1_Stand_45E620;
                field_118_timer = sGnFrame + Math_RandomRange(field_148_right_min_delay, field_14A_right_max_delay);
                return 6;
            }
            return field_110_brain_sub_state;

        case 6:
            if (mLiftPoint)
            {
                auto pLiftPoint = static_cast<LiftPoint*>(mLiftPoint);
                if (pLiftPoint->mBaseGameObjectTypeId == ReliveTypes::eLiftPoint && !pLiftPoint->OnAnyFloor())
                {
                    mNextMotion = eScrabMotions::Motion_1_Stand_45E620;
                    return 8;
                }
            }

            if (field_118_timer > static_cast<s32>(sGnFrame))
            {
                return field_110_brain_sub_state;
            }

            if (Math_NextRandom() < 30u)
            {
                mNextMotion = eScrabMotions::Motion_20_HowlBegin_45FA60;
                field_118_timer = sGnFrame + 30;
                return 7;
            }

            mNextMotion = GetMotionForPatrolType(field_116_patrol_type);
            return 1;

        case 7:
            if (field_118_timer > static_cast<s32>(sGnFrame))
            {
                return field_110_brain_sub_state;
            }

            mNextMotion = GetMotionForPatrolType(field_116_patrol_type);

            if (mLiftPoint)
            {
                auto pLiftPoint = static_cast<LiftPoint*>(mLiftPoint);
                if (pLiftPoint->mBaseGameObjectTypeId == ReliveTypes::eLiftPoint && !pLiftPoint->OnAnyFloor())
                {
                    mNextMotion = eScrabMotions::Motion_1_Stand_45E620;
                    return 8;
                }
            }

            if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
            {
                return 1;
            }
            return 4;

        case 8:
            if (mLiftPoint)
            {
                auto pLift = static_cast<LiftPoint*>(mLiftPoint);
                if (!pLift->OnAnyFloor())
                {
                    return field_110_brain_sub_state;
                }
            }
            return 0;

        case 9:
            if (!EventGet(kEventAbeOhm))
            {
                mNextMotion = eScrabMotions::Motion_1_Stand_45E620;
                return 0;
            }
            return field_110_brain_sub_state;

        default:
            return field_110_brain_sub_state;
    }
}

s16 Scrab::Brain_WalkAround_460D80()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(Options::eDead);
    }

    field_11C_pFight_target = FindScrabToFight();
    if (field_11C_pFight_target)
    {
        field_11C_pFight_target->mBaseGameObjectRefCount++;
        SetBrain(&Scrab::Brain_Fighting_45C370);
        mNextMotion = eScrabMotions::Motion_1_Stand_45E620;
        return 0;
    }

    if (FindAbeOrMud())
    {
        if (CanSeeAbe(field_120_pTarget))
        {
            mNextMotion = eScrabMotions::Motion_1_Stand_45E620;
            SetBrain(&Scrab::Brain_ChasingEnemy_45CC90);
            return 0;
        }
        field_120_pTarget->mBaseGameObjectRefCount--;
        field_120_pTarget = nullptr;
    }

    if (EventGet(kEventAbeOhm))
    {
        mNextMotion = eScrabMotions::Motion_20_HowlBegin_45FA60;
        return 6;
    }

    switch (field_110_brain_sub_state)
    {
        case 0:
            if (sNumCamSwappers_507668 > 0)
            {
                return field_110_brain_sub_state;
            }
            field_12C = mXPos;
            return 1;

        case 1:
            if (mLiftPoint)
            {
                auto pLiftPoint = static_cast<LiftPoint*>(mLiftPoint);
                if (pLiftPoint->mBaseGameObjectTypeId == ReliveTypes::eLiftPoint)
                {
                    if (!pLiftPoint->OnAnyFloor())
                    {
                        return 1;
                    }
                }
            }

            if (field_118_timer > static_cast<s32>(sGnFrame))
            {
                if (Math_NextRandom() >= 8u || !(field_188_flags & 0x20) || sGnFrame - field_140_last_shriek_timer <= 90)
                {
                    return field_110_brain_sub_state;
                }
                mNextMotion = eScrabMotions::Motion_22_Shriek_45FB00;
                field_140_last_shriek_timer = sGnFrame;
                return 5;
            }

            if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
            {
                auto pStopper = static_cast<relive::Path_EnemyStopper*>(gMap.TLV_Get_At_446260(
                    FP_GetExponent(mXPos),
                    FP_GetExponent(mYPos),
                    FP_GetExponent(mXPos),
                    FP_GetExponent(mYPos),
                    ReliveTypes::eEnemyStopper));

                BaseAliveGameObjectPathTLV = pStopper;

                if (pStopper)
                {
                    if ((pStopper->mStopDirection == relive::Path_EnemyStopper::StopDirection::Left || pStopper->mStopDirection == relive::Path_EnemyStopper::StopDirection::Both) && !SwitchStates_Get(pStopper->mSwitchId))
                    {
                        mNextMotion = eScrabMotions::Motion_4_Turn_45EF30;
                        return 4;
                    }
                }

                if (WallHit_401930(mSpriteScale * FP_FromInteger(30), -ScaleToGridSize(mSpriteScale) * FP_FromDouble(1.5)) || Check_IsOnEndOfLine_4021A0(1, 2))
                {
                    mNextMotion = eScrabMotions::Motion_4_Turn_45EF30;
                    return 4;
                }
                mNextMotion = eScrabMotions::Motion_2_Walk_45E730;
                return 2;
            }
            else
            {
                auto pStopper = static_cast<relive::Path_EnemyStopper*>(gMap.TLV_Get_At_446260(
                    FP_GetExponent(mXPos),
                    FP_GetExponent(mYPos),
                    FP_GetExponent(mXPos),
                    FP_GetExponent(mYPos),
                    ReliveTypes::eEnemyStopper));

                BaseAliveGameObjectPathTLV = pStopper;

                if (pStopper)
                {
                    if ((pStopper->mStopDirection == relive::Path_EnemyStopper::StopDirection::Right || pStopper->mStopDirection == relive::Path_EnemyStopper::StopDirection::Both) && !SwitchStates_Get(pStopper->mSwitchId))
                    {
                        mNextMotion = eScrabMotions::Motion_4_Turn_45EF30;
                        return 4;
                    }
                }

                if (WallHit_401930(mSpriteScale * FP_FromInteger(30), ScaleToGridSize(mSpriteScale) * FP_FromDouble(1.5))
                    || Check_IsOnEndOfLine_4021A0(0, 2))
                {
                    mNextMotion = eScrabMotions::Motion_4_Turn_45EF30;
                    return 4;
                }
                mNextMotion = eScrabMotions::Motion_2_Walk_45E730;
                return 3;
            }
            break;

        case 2:
        {
            auto pStopper = static_cast<relive::Path_EnemyStopper*>(gMap.TLV_Get_At_446260(
                FP_GetExponent(mXPos),
                FP_GetExponent(mYPos),
                FP_GetExponent(mXPos),
                FP_GetExponent(mYPos),
                ReliveTypes::eEnemyStopper));

            BaseAliveGameObjectPathTLV = pStopper;

            if (pStopper)
            {
                if ((pStopper->mStopDirection == relive::Path_EnemyStopper::StopDirection::Left || pStopper->mStopDirection == relive::Path_EnemyStopper::StopDirection::Both) && !SwitchStates_Get(pStopper->mSwitchId))
                {
                    mNextMotion = eScrabMotions::Motion_4_Turn_45EF30;
                    return 4;
                }
            }

            if (field_12C - mXPos > FP_FromInteger(368))
            {
                mNextMotion = eScrabMotions::Motion_4_Turn_45EF30;
                return 4;
            }

            if (WallHit_401930(mSpriteScale * FP_FromInteger(30), -ScaleToGridSize(mSpriteScale) * FP_FromDouble(1.5))
                || Check_IsOnEndOfLine_4021A0(1, 2))
            {
                mNextMotion = eScrabMotions::Motion_4_Turn_45EF30;
                return 4;
            }

            if (Math_NextRandom() >= 8u || !(field_188_flags & 0x20) || sGnFrame - field_140_last_shriek_timer <= 90)
            {
                if (mLiftPoint)
                {
                    auto pLiftPoint = static_cast<LiftPoint*>(mLiftPoint);
                    if (pLiftPoint->mBaseGameObjectTypeId == ReliveTypes::eLiftPoint)
                    {
                        if (!pLiftPoint->OnAnyFloor())
                        {
                            mNextMotion = eScrabMotions::Motion_1_Stand_45E620;
                            return 1;
                        }
                    }
                }
                return field_110_brain_sub_state;
            }

            field_140_last_shriek_timer = sGnFrame;
            mNextMotion = eScrabMotions::Motion_22_Shriek_45FB00;
            return 5;
        }

        case 3:
        {
            auto pStopper = static_cast<relive::Path_EnemyStopper*>(gMap.TLV_Get_At_446260(
                FP_GetExponent(mXPos),
                FP_GetExponent(mYPos),
                FP_GetExponent(mXPos),
                FP_GetExponent(mYPos),
                ReliveTypes::eEnemyStopper));

            BaseAliveGameObjectPathTLV = pStopper;

            if (pStopper)
            {
                if ((pStopper->mStopDirection == relive::Path_EnemyStopper::StopDirection::Right || pStopper->mStopDirection == relive::Path_EnemyStopper::StopDirection::Both) && !SwitchStates_Get(pStopper->mSwitchId))
                {
                    mNextMotion = eScrabMotions::Motion_4_Turn_45EF30;
                    return 4;
                }
            }

            if (mXPos - field_12C > FP_FromInteger(368)
                || WallHit_401930(mSpriteScale * FP_FromInteger(30), ScaleToGridSize(mSpriteScale) * FP_FromDouble(1.5))
                || Check_IsOnEndOfLine_4021A0(0, 2))
            {
                mNextMotion = eScrabMotions::Motion_4_Turn_45EF30;
                return 4;
            }

            if (Math_NextRandom() >= 8u || !(field_188_flags & 0x20) || sGnFrame - field_140_last_shriek_timer <= 90)
            {
                if (mLiftPoint)
                {
                    auto pLiftPoint = static_cast<LiftPoint*>(mLiftPoint);
                    if (pLiftPoint->mBaseGameObjectTypeId == ReliveTypes::eLiftPoint)
                    {
                        if (!pLiftPoint->OnAnyFloor())
                        {
                            mNextMotion = eScrabMotions::Motion_1_Stand_45E620;
                            return 1;
                        }
                    }
                }
                return field_110_brain_sub_state;
            }

            field_140_last_shriek_timer = sGnFrame;
            mNextMotion = eScrabMotions::Motion_22_Shriek_45FB00;
            return 5;
        }

        case 4:
            if (mCurrentMotion != 4 || !mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                return field_110_brain_sub_state;
            }

            if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
            {
                field_118_timer = sGnFrame + Math_RandomRange(field_144_left_min_delay, field_146_left_max_delay);
            }
            else
            {
                field_118_timer = sGnFrame + Math_RandomRange(field_148_right_min_delay, field_14A_right_max_delay);
            }

            return 1;

        case 5:
            if (mLiftPoint)
            {
                auto pLiftPoint = static_cast<LiftPoint*>(mLiftPoint);
                if (pLiftPoint->mBaseGameObjectTypeId == ReliveTypes::eLiftPoint && !pLiftPoint->OnAnyFloor())
                {
                    mNextMotion = eScrabMotions::Motion_1_Stand_45E620;
                    return 1;
                }
            }

            if (mCurrentMotion != eScrabMotions::Motion_22_Shriek_45FB00 || !mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                return field_110_brain_sub_state;
            }
            mNextMotion = eScrabMotions::Motion_1_Stand_45E620;
            return 1;

        case 6:
            if (EventGet(kEventAbeOhm))
            {
                return field_110_brain_sub_state;
            }
            mNextMotion = eScrabMotions::Motion_1_Stand_45E620;
            return 1;

        default:
            return field_110_brain_sub_state;
    }
}

void Scrab::SetBrain(TBrainType fn)
{
    field_10C_fn = fn;
}

bool Scrab::BrainIs(TBrainType fn)
{
    return field_10C_fn == fn;
}

s16 Scrab::HandleRunning()
{
    BaseAliveGameObjectPathTLV = gMap.TLV_Get_At_446260(
        FP_GetExponent(mXPos),
        FP_GetExponent(mYPos),
        FP_GetExponent(mXPos),
        FP_GetExponent(mYPos),
        ReliveTypes::eEnemyStopper);
    auto pStopper = static_cast<relive::Path_EnemyStopper*>(BaseAliveGameObjectPathTLV);

    if (pStopper)
    {
        if ((pStopper->mStopDirection == relive::Path_EnemyStopper::StopDirection::Left && field_120_pTarget->mXPos < mXPos) || (pStopper->mStopDirection == relive::Path_EnemyStopper::StopDirection::Right && field_120_pTarget->mXPos > mXPos) || pStopper->mStopDirection == relive::Path_EnemyStopper::StopDirection::Both)
        {
            if (!SwitchStates_Get(pStopper->mSwitchId))
            {
                return 1;
            }
        }
    }

    if (!mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
    {
        if (Check_IsOnEndOfLine_4021A0(0, 1))
        {
            mNextMotion = eScrabMotions::Motion_7_HopMidair_45F1A0;
            return 6;
        }
    }
    else
    {
        if (Check_IsOnEndOfLine_4021A0(1, 1))
        {
            mNextMotion = eScrabMotions::Motion_7_HopMidair_45F1A0;
            return 6;
        }
    }

    mNextMotion = eScrabMotions::Motion_3_Run_45EAB0;
    return 3;
}

s16 Scrab::GetMotionForPatrolType(relive::Path_Scrab::ScrabPatrolType ScrabPatrolType)
{
    switch (ScrabPatrolType)
    {
        case relive::Path_Scrab::ScrabPatrolType::eRunOrWalk192:
            if (Math_NextRandom() < 192u)
            {
                return eScrabMotions::Motion_2_Walk_45E730;
            }
            else
            {
                return eScrabMotions::Motion_3_Run_45EAB0;
            }
            break;

        case relive::Path_Scrab::ScrabPatrolType::eRunOrWalk128:
            if (Math_NextRandom() < 128u)
            {
                return eScrabMotions::Motion_2_Walk_45E730;
            }
            else
            {
                return eScrabMotions::Motion_3_Run_45EAB0;
            }
            break;

        case relive::Path_Scrab::ScrabPatrolType::eRunOrWalk64:
            if (Math_NextRandom() < 64u)
            {
                return eScrabMotions::Motion_2_Walk_45E730;
            }
            else
            {
                return eScrabMotions::Motion_3_Run_45EAB0;
            }
            break;

        case relive::Path_Scrab::ScrabPatrolType::eRun:
            return eScrabMotions::Motion_3_Run_45EAB0;

        default:
            return eScrabMotions::Motion_2_Walk_45E730;
    }
}

} // namespace AO
