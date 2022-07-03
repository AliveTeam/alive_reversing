#include "stdafx_ao.h"
#include "Function.hpp"
#include "Mudokon.hpp"
#include "GameSpeak.hpp"
#include "ResourceManager.hpp"
#include "Shadow.hpp"
#include "stdlib.hpp"
#include "Collisions.hpp"
#include "Game.hpp"
#include "Abe.hpp"
#include "Midi.hpp"
#include "BirdPortal.hpp"
#include "LiftPoint.hpp"
#include "DDCheat.hpp"
#include "Events.hpp"
#include "Gibs.hpp"
#include "Blood.hpp"
#include "Math.hpp"
#include "Bullet.hpp"
#include "Sfx.hpp"
#include "MusicTrigger.hpp"
#include "SwitchStates.hpp"
#include "Particle.hpp"
#include "CameraSwapper.hpp"
#include "Lever.hpp"
#include "ScreenShake.hpp"
#include "PsxDisplay.hpp"
#include "PossessionFlicker.hpp"
#include "AbilityRing.hpp"
#include "CheatController.hpp"
#include "Sys_common.hpp"
#include "magic_enum/include/magic_enum.hpp"

void Mud_ForceLink()
{ }

namespace AO {

using TMudStateFunction = decltype(&Mudokon::Motion_0_Idle);

#define MAKE_STRINGS(VAR) #VAR,
const char_type* const sMudStateNames[] = {
    MUD_STATES_ENUM(MAKE_STRINGS)};

const TMudStateFunction gMudMotions_4CD470[] = {
    &Mudokon::Motion_0_Idle,
    &Mudokon::Motion_1_WalkLoop,
    &Mudokon::Motion_2_StandingTurn,

    // All the same func
    &Mudokon::Motion_3_Speak,
    &Mudokon::Motion_4_Speak,
    &Mudokon::Motion_5_Speak,
    &Mudokon::Motion_6_Speak,

    &Mudokon::Motion_7_WalkBegin,
    &Mudokon::Motion_8_WalkToIdle,
    &Mudokon::Motion_9_MidWalkToIdle,
    &Mudokon::Motion_10_Unused,
    &Mudokon::Motion_11_Null,
    &Mudokon::Motion_12_LiftUse,
    &Mudokon::Motion_13_LiftGrabBegin,
    &Mudokon::Motion_14_LiftGrabEnd,
    &Mudokon::Motion_15_LeverUse,
    &Mudokon::Motion_16_StandScrubLoop,
    &Mudokon::Motion_17_StandScrubLoopToPause,
    &Mudokon::Motion_18_StandScrubPauseToLoop,
    &Mudokon::Motion_19_StandScrubPause,
    &Mudokon::Motion_20_IdleToStandScrub,
    &Mudokon::Motion_21_StandScrubToIdle,
    &Mudokon::Motion_22_CrouchScrub,
    &Mudokon::Motion_23_CrouchIdle,
    &Mudokon::Motion_24_CrouchTurn,
    &Mudokon::Motion_25_StandToCrouch,
    &Mudokon::Motion_26_CrouchToStand,
    &Mudokon::Motion_27_WalkToRun,
    &Mudokon::Motion_28_MidWalkToRun,
    &Mudokon::Motion_29_RunLoop,
    &Mudokon::Motion_30_RunToWalk,
    &Mudokon::Motion_31_MidRunToWalk,
    &Mudokon::Motion_32_RunSlideStop,
    &Mudokon::Motion_33_RunSlideTurn,
    &Mudokon::Motion_34_RunTurnToRun,
    &Mudokon::Motion_35_SneakLoop,
    &Mudokon::Motion_36_MidWalkToSneak,
    &Mudokon::Motion_37_SneakToWalk,
    &Mudokon::Motion_38_WalkToSneak,
    &Mudokon::Motion_39_MidSneakToWalk,
    &Mudokon::Motion_40_SneakBegin,
    &Mudokon::Motion_41_SneakToIdle,
    &Mudokon::Motion_42_MidSneakToIdle,
    &Mudokon::Motion_43_RunJumpBegin,
    &Mudokon::Motion_44_RunJumpMid,
    &Mudokon::Motion_45_StandToRun,
    &Mudokon::Motion_46_FallLandDie,
    &Mudokon::Motion_47_Knockback,
    &Mudokon::Motion_48_KnockbackGetUp,
    &Mudokon::Motion_49_WalkOffEdge,
    &Mudokon::Motion_50_LandSoft,
    &Mudokon::Motion_51_Fall,
    &Mudokon::Motion_52_Chant,
    &Mudokon::Motion_53_ChantEnd,
    &Mudokon::Motion_54_ToDuck,
    &Mudokon::Motion_55_Duck,
    &Mudokon::Motion_56_DuckToCrouch,
    &Mudokon::Motion_57_Struggle,
    &Mudokon::Motion_58_StruggleToCrouchChant,
    &Mudokon::Motion_59_CrouchChant,
    &Mudokon::Motion_60_CrouchChantToStruggle,
    &Mudokon::Motion_61_DuckKnockback,
    &Mudokon::Motion_62_PoisonGasDeath,
};

using TMudBrain = decltype(&Mudokon::Brain_0_ComingIn);

const TMudBrain gMudBrains_4CD430[] = {
    &Mudokon::Brain_0_ComingIn,
    &Mudokon::Brain_1_ComingOut,
    &Mudokon::Brain_2_SingSequenceIdle,
    &Mudokon::Brain_3_SingSequenceSing,
    &Mudokon::Brain_4_SingSequencePassword,
    &Mudokon::Brain_5_LiftUse,
    &Mudokon::Brain_6_LeverUse,
    &Mudokon::Brain_7_GiveRings,
    &Mudokon::Brain_8_StandScrub,
    &Mudokon::Brain_9_CrouchScrub,
    &Mudokon::Brain_10_ListeningToAbe,
    &Mudokon::Brain_11_ShrivelDeath,
    &Mudokon::Brain_12_Escape,
    &Mudokon::Brain_13_FallAndSmackDeath,
    &Mudokon::Brain_14_Chant,
    &Mudokon::Brain_15_Choke,
};


static const TintEntry sMudTints_4CD320[] = {
    {EReliveLevelIds::eStockYards, 25u, 25u, 25u},
    {EReliveLevelIds::eStockYardsReturn, 25u, 25u, 25u},
    {EReliveLevelIds::eNone, 87u, 103u, 67u}};

ALIVE_VAR(1, 0x507B90, s16, sAlertedMudCount_507B90, 0);
ALIVE_VAR(1, 0x507B94, s16, sMudRunningToPortalCount_507B94, 0);

Mudokon::Mudokon(Path_TLV* pTlv, s32 tlvInfo)
    : BaseAliveGameObject()
{
    field_128 = -1;
    field_13E = -1;
    mNextMotion = -1;
    field_13C = 0;
    field_1C0_timer = 0;
    field_1B8_brain_state = 0;
    field_1BA_brain_sub_state = 0;
    field_EC = 3;
    field_1BC = 0;
    field_124_voice_pitch = 0;
    field_148_res_array = {};

    const AnimRecord& mudRec = AO::AnimRec(AnimId::Mudokon_Idle);
    field_148_res_array.res[0] = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, mudRec.mResourceId, 1, 0);
    Animation_Init(AnimId::Mudokon_Idle, field_148_res_array.res[0]);

    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit15_bSemiTrans);

    mBaseAliveGameObjectFlags.Set(Flags_10A::e10A_Bit4_SetOffExplosives);
    mBaseAliveGameObjectFlags.Set(Flags_10A::e10A_Bit6);

    field_144_flags.Clear(Flags_144::e144_Bit6_bPersist);
    field_144_flags.Clear(Flags_144::e144_Bit7);
    field_144_flags.Clear(Flags_144::e144_Bit8);
    field_144_flags.Clear(Flags_144::e144_Bit9);
    field_144_flags.Clear(Flags_144::e144_Bit10_give_ring_without_password);

    field_1B6 = 0;
    field_198_abe_must_face_mud = 0;

    SetTint(sMudTints_4CD320, mBaseAnimatedWithPhysicsGameObject_LvlNumber);

    Scale_short scale = Scale_short::eFull_0;
    switch (pTlv->mTlvType32.mType)
    {
        case TlvTypes::None_m1:
            ALIVE_FATAL("Mudokon ctor pTlv->mTlvType32.mType was None_m1. This shouldn't happen.");
            break;
        case TlvTypes::LiftMudokon_32:
        {
            mBaseGameObjectTypeId = ReliveTypes::eRingOrLiftMud;

            auto liftMudTlv = static_cast<Path_LiftMudokon*>(pTlv);

            field_148_res_array.res[1] = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kAbeliftAOResID, 1, 0);


            field_18C_how_far_to_walk = FP_FromInteger(liftMudTlv->field_18_how_far_to_walk);
            field_110_lift_switch_id = liftMudTlv->field_1A_lift_switch_id;

            field_144_flags.Set(Flags_144::e144_Bit5_unused, liftMudTlv->field_1C_direction == Path_LiftMudokon::Direction::eLeft_1);
            field_144_flags.Clear(Flags_144::e144_Bit4_bSnapToGrid);
            field_144_flags.Clear(Flags_144::e144_Bit11_bDeaf);

            mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit5_FlipX, liftMudTlv->field_1C_direction == Path_LiftMudokon::Direction::eLeft_1);

            field_186_give_password = liftMudTlv->field_1E_give_password;
            field_184 = 1;
            field_1B8_brain_state = 0;
            field_188 = 5;

            field_1A4_code_converted = Code_Convert(liftMudTlv->field_22_code1, liftMudTlv->field_24_code2);
            field_1A8_code_length = Code_Length(field_1A4_code_converted);

            scale = liftMudTlv->field_20_scale;
        }
        break;

        case TlvTypes::RingMudokon_50:
        {
            mBaseGameObjectTypeId = ReliveTypes::eRingOrLiftMud;

            auto ringMudTlv = static_cast<Path_RingMudokon*>(pTlv);

            mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit5_FlipX, ringMudTlv->field_18_direction == XDirection_short::eLeft_0); // TODO: Check
            field_184 = 0;
            field_186_give_password = ringMudTlv->field_1E_give_password;
            field_198_abe_must_face_mud = ringMudTlv->field_1A_abe_must_face_mud == Path_RingMudokon::MustFaceMud::eYes_0;
            field_1B8_brain_state = 2;

            if (ringMudTlv->field_24_action == SwitchOp::eSetTrue_0)
            {
                // Pull switch
                field_148_res_array.res[2] = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kAbepullAOResID, 1, 0);
                field_188 = 6;
            }
            else
            {
                // Give ring
                field_148_res_array.res[9] = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kAbeommAOResID, 1, 0);
                field_188 = 7;
            }

            field_1AA_ring_timeout = ringMudTlv->field_26_ring_timeout;
            field_1A4_code_converted = Code_Convert(ringMudTlv->field_20_code1, ringMudTlv->field_22_code2);
            field_1A8_code_length = Code_Length(field_1A4_code_converted);

            field_144_flags.Set(Flags_144::e144_Bit10_give_ring_without_password, ringMudTlv->field_28_give_ring_without_password == Choice_short::eYes_1);
            field_144_flags.Clear(Flags_144::e144_Bit4_bSnapToGrid);
            field_144_flags.Clear(Flags_144::e144_Bit11_bDeaf);


            scale = ringMudTlv->field_1C_scale;
        }
        break;

        case TlvTypes::Mudokon_82:
        {
            mBaseGameObjectTypeId = ReliveTypes::eMudokon;

            auto mudTlv = static_cast<Path_Mudokon*>(pTlv);

            if (mudTlv->field_1A_job == Path_Mudokon::MudJobs::eStandScrub_0)
            {
                field_1B8_brain_state = 8;
                field_148_res_array.res[3] = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kMudchslAOResID, 1, 0);
            }
            else if (mudTlv->field_1A_job == Path_Mudokon::MudJobs::eSitScrub_1)
            {
                field_1B8_brain_state = 9;
            }
            else if (mudTlv->field_1A_job == Path_Mudokon::MudJobs::eSitChant_2)
            {
                field_1B8_brain_state = 14;
                field_148_res_array.res[12] = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kMudltusAOResID, 1, 0);
            }

            field_148_res_array.res[10] = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kMudoduckAOResID, 1, 0);
            field_148_res_array.res[13] = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kMudbtlnkAOResID, 1, 0);
            field_148_res_array.res[4] = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kMudscrubAOResID, 1, 0);
            field_148_res_array.res[5] = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kAbebasicAOResID, 1, 0);
            field_148_res_array.res[6] = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kAbeknfdAOResID, 1, 0);
            field_148_res_array.res[7] = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kAbeknbkAOResID, 1, 0);
            field_148_res_array.res[8] = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kAbeedgeAOResID, 1, 0);
            field_148_res_array.res[14] = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kAbegasAOResID, 1, 0);


            field_124_voice_pitch = mudTlv->field_1E_voice_pitch;
            field_1B2_rescue_switch_id = mudTlv->field_20_rescue_switch_id;

            mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit5_FlipX, mudTlv->field_1C_direction == XDirection_short::eLeft_0);

            // TODO: Check these as well
            field_144_flags.Set(Flags_144::e144_Bit11_bDeaf, mudTlv->field_22_deaf == Choice_short::eYes_1);
            field_144_flags.Set(Flags_144::e144_Bit6_bPersist, mudTlv->field_26_persist & 1);
            field_144_flags.Set(Flags_144::e144_Bit4_bSnapToGrid);

            field_1B4_idle_time = 0;

            scale = mudTlv->field_18_scale;
        }
        break;
        default:
            LOG_WARNING("Mudokon ctor pTlv->mTlvType32.mType was " << magic_enum::enum_name(pTlv->mTlvType32.mType) << ". This is unhandled.");
            break;
    }

    mCurrentMotion = eMudMotions::Motion_0_Idle;

    if (scale == Scale_short::eHalf_1)
    {
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromDouble(0.5);
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_AbeMenu_Half_13;
        mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Bg;
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromInteger(1);
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_AbeMenu_32;
        mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Fg;
    }

    FP hitX = {};
    FP hitY = {};
    const s16 bHit = sCollisions->Raycast(
        FP_FromInteger(pTlv->mTopLeft.x),
        FP_FromInteger(pTlv->mTopLeft.y),
        FP_FromInteger(pTlv->mBottomRight.x),
        FP_FromInteger(pTlv->mBottomRight.y),
        &BaseAliveGameObjectCollisionLine,
        &hitX,
        &hitY,
        mBaseAnimatedWithPhysicsGameObject_SpriteScale != FP_FromDouble(0.5) ? kFgWallsOrFloor : kBgWallsOrFloor) ? 1 : 0;

    mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger((pTlv->mTopLeft.x + pTlv->mBottomRight.x) / 2);

    if (bHit)
    {
        mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
        if (BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eDynamicCollision_32 || BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eBackgroundDynamicCollision_36)
        {
            const PSX_RECT bRect = VGetBoundingRect();
            VOnCollisionWith(
                PSX_Point{bRect.x, static_cast<s16>(bRect.y + 5)},
                PSX_Point{bRect.w, static_cast<s16>(bRect.h + 5)},
                ObjListPlatforms_50766C,
                1,
                (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection_401C10);
        }
    }

    BaseAliveGameObjectLastLineYPos = mBaseAnimatedWithPhysicsGameObject_YPos;

    if (field_1B8_brain_state == 9 || field_1B8_brain_state == 8)
    {
        field_144_flags.Set(Flags_144::e144_Bit7);
        MapFollowMe_401D30(field_144_flags.Get(Flags_144::e144_Bit4_bSnapToGrid));
    }
    else
    {
        field_144_flags.Clear(Flags_144::e144_Bit7);
    }

    field_144_flags.Set(Flags_144::e144_Bit2);

    field_19A = 99;
    field_19C = 99;
    field_1BA_brain_sub_state = 0;
    field_1A0 = 0;
    field_10C = tlvInfo;
    field_1C4_bDoPathTrans = FALSE;

    if (field_188 == 6)
    {
        mBaseAnimatedWithPhysicsGameObject_XPos += FP_FromInteger(8);
    }

    field_1AC_pBirdPortal = nullptr;
    field_194_pLiftPoint = nullptr;

    mShadow = relive_new Shadow();

    VUpdate();
}

Mudokon::~Mudokon()
{
    if (mHealth <= FP_FromInteger(0))
    {
        sKilledMudokons_5076BC++;
    }

    RemoveAlerted();

    KillBirdPortal();
    KillLiftPoint_194();

    if (!field_144_flags.Get(Flags_144::e144_Bit2) || mHealth <= FP_FromInteger(0) || mBaseAliveGameObjectFlags.Get(Flags_10A::e10A_Bit5_Electrocuted))
    {
        gMap.TLV_Reset(field_10C, -1, 0, 1);
    }
    else
    {
        gMap.TLV_Reset(field_10C, -1, 0, 0);
    }

    for (auto& res : field_148_res_array.res)
    {
        if (res && res != mBaseAnimatedWithPhysicsGameObject_Anim.field_20_ppBlock)
        {
            ResourceManager::FreeResource_455550(res);
        }
    }

    if (field_1B8_brain_state == 7)
    {
        if (field_1AA_ring_timeout)
        {
            sActiveHero_507678->field_168_ring_pulse_timer = field_1AA_ring_timeout + sGnFrame;
        }
        else
        {
            sActiveHero_507678->field_168_ring_pulse_timer = sGnFrame + 200000;
        }
        sActiveHero_507678->field_16C_bHaveShrykull = FALSE;
    }

    if (mCurrentMotion == eMudMotions::Motion_52_Chant || mCurrentMotion == eMudMotions::Motion_59_CrouchChant)
    {
        SND_Seq_Stop_477A60(SeqId::eMudokonChant_12);
    }
}

void Mudokon::KillLiftPoint_194()
{
    if (field_194_pLiftPoint)
    {
        field_194_pLiftPoint->mBaseGameObjectRefCount--;
        field_194_pLiftPoint = nullptr;
    }
}

void Mudokon::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        return;
    }

    if (field_1C4_bDoPathTrans)
    {
        DoPathTrans();
        field_1C4_bDoPathTrans = FALSE;
    }

    const auto old_motion = mCurrentMotion;
    const auto old_brain = field_1B8_brain_state;
    const auto old_brain_sub_state = field_1BA_brain_sub_state;
    field_1BA_brain_sub_state = (this->*gMudBrains_4CD430[field_1B8_brain_state])();

    if (old_brain != field_1B8_brain_state)
    {
        // Main brain state changed
        //LOG_INFO("Brain changed from " << old_brain << " to " << field_1B8_brain_state);
    }
    else if (old_brain_sub_state != field_1BA_brain_sub_state)
    {
        //LOG_INFO("Brain sub state changed from " << old_brain_sub_state << " to " << field_1BA_brain_sub_state);
    }

    const FP oldX = mBaseAnimatedWithPhysicsGameObject_XPos;
    const FP oldY = mBaseAnimatedWithPhysicsGameObject_YPos;
    (this->*gMudMotions_4CD470[mCurrentMotion])();

    if (oldX != mBaseAnimatedWithPhysicsGameObject_XPos || oldY != mBaseAnimatedWithPhysicsGameObject_YPos)
    {
        BaseAliveGameObjectPathTLV = gMap.TLV_Get_At_446060(
            0,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos);
        VOnTlvCollision(BaseAliveGameObjectPathTLV);
    }

    if (old_motion != mCurrentMotion || field_108_bMotionChanged)
    {
        field_108_bMotionChanged = FALSE;
        VUpdateResBlock();

        if (old_motion == eMudMotions::Motion_10_Unused)
        {
            mBaseAnimatedWithPhysicsGameObject_Anim.SetFrame(mBaseAliveGameObjectLastAnimFrame);
        }
    }
    else if (field_1BC)
    {
        mCurrentMotion = mPreviousMotion;
        VUpdateResBlock();
        mBaseAnimatedWithPhysicsGameObject_Anim.SetFrame(mBaseAliveGameObjectLastAnimFrame);
        field_1BC = 0;
    }

    if (mBaseAliveGameObjectFlags.Get(Flags_10A::e10A_Bit5_Electrocuted))
    {
        EventBroadcast(kEventMudokonDead, sActiveHero_507678);
    }
}

const AnimId sMudMotionAnimIds[64] = {
    AnimId::Mudokon_Idle,
    AnimId::Mudokon_Walk,
    AnimId::Mudokon_StandingTurn,
    AnimId::Mudokon_Speak1,
    AnimId::Mudokon_Speak2,
    AnimId::Mudokon_Speak3,
    AnimId::Mudokon_SpeakFart,
    AnimId::Mudokon_WalkBegin,
    AnimId::Mudokon_WalkToIdle,
    AnimId::Mudokon_MidWalkToIdle,
    AnimId::Mudokon_Idle,
    AnimId::Mudokon_AO_Null,
    AnimId::Mudokon_AO_LiftUse,
    AnimId::Mudokon_LiftGrabBegin,
    AnimId::Mudokon_LiftGrabEnd,
    AnimId::Mudokon_LeverUse,
    AnimId::Mudokon_StandScrubLoop,
    AnimId::Mudokon_StandScrubLoopToPause,
    AnimId::Mudokon_StandScrubPauseToLoop,
    AnimId::Mudokon_StandScrubPause,
    AnimId::Mudokon_IdleToStandScrub,
    AnimId::Mudokon_StandScrubToIdle,
    AnimId::Mudokon_CrouchScrub,
    AnimId::Mudokon_CrouchIdle,
    AnimId::Mudokon_CrouchTurn,
    AnimId::Mudokon_StandToCrouch,
    AnimId::Mudokon_CrouchToStand,
    AnimId::Mudokon_WalkToRun,
    AnimId::Mudokon_MidWalkToRun,
    AnimId::Mudokon_Run,
    AnimId::Mudokon_RunToWalk,
    AnimId::Mudokon_MidRunToWalk,
    AnimId::Mudokon_RunSlideStop,
    AnimId::Mudokon_RunSlideTurn,
    AnimId::Mudokon_RunTurnToRun,
    AnimId::Mudokon_Sneak,
    AnimId::Mudokon_WalkToSneak,
    AnimId::Mudokon_SneakToWalk,
    AnimId::Mudokon_MidWalkToSneak,
    AnimId::Mudokon_MidSneakToWalk,
    AnimId::Mudokon_SneakBegin,
    AnimId::Mudokon_SneakToIdle,
    AnimId::Mudokon_MidSneakToIdle,
    AnimId::Mudokon_RunJumpBegin,
    AnimId::Mudokon_RunJumpMid,
    AnimId::Mudokon_StandToRun,
    AnimId::Mudokon_KnockForward,
    AnimId::Mudokon_Knockback,
    AnimId::Mudokon_KnockbackGetUp,
    AnimId::Mudokon_WalkOffEdge,
    AnimId::Mudokon_AO_LandSoft_Long,
    AnimId::Mudokon_FallingFromGrab, // check/rename
    AnimId::Mudokon_Chant,
    AnimId::Mudokon_ChantEnd,
    AnimId::Mudokon_ToDuck,
    AnimId::Mudokon_Duck,
    AnimId::Mudokon_DuckToCrouch,
    AnimId::Mudokon_Struggle,
    AnimId::Mudokon_StruggleToCrouchChant,
    AnimId::Mudokon_CrouchChant,
    AnimId::Mudokon_CrouchChantToStruggle,
    AnimId::Mudokon_DuckKnockback,
    AnimId::Mudokon_PoisonGasDeath,
    AnimId::None};

void Mudokon::VUpdateResBlock()
{
    mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(
        sMudMotionAnimIds[mCurrentMotion],
        GetResBlockForMotion(mCurrentMotion));
}

void Mudokon::VScreenChanged()
{
    // Map/overlay changed or mud shouldn't persist
    if (gMap.mCurrentLevel != gMap.mLevel || gMap.mOverlayId != gMap.GetOverlayId() || !field_144_flags.Get(Flags_144::e144_Bit6_bPersist))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        KillBirdPortal();
        KillLiftPoint_194();
        return;
    }

    // Path changed?
    if (gMap.mCurrentPath != gMap.mPath)
    {
        // See if we need to go to the next path
        auto pTlv = gMap.TLV_Get_At_446060(nullptr, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos);
        while (pTlv)
        {
            if (pTlv->mTlvType32 == TlvTypes::MudokonPathTrans_89)
            {
                // Gonna go to the next path
                field_1C4_bDoPathTrans = TRUE;
                return;
            }
            pTlv = gMap.TLV_Get_At_446060(pTlv, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos);
        }

        // Wasn't a path trans and path changed, die
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        KillBirdPortal();
        KillLiftPoint_194();
    }
}

s16 Mudokon::VTakeDamage(BaseGameObject* pFrom)
{
    switch (pFrom->mBaseGameObjectTypeId)
    {
        case ReliveTypes::eGasClock:
            if (mHealth > FP_FromInteger(0))
            {
                field_1B8_brain_state = 15;
                field_1BA_brain_sub_state = 0;
            }
            return 1;

        case ReliveTypes::eElectricWall:
            Mudokon_SFX(MudSounds::eDeathDropScream_17, 0, 0, this);
            return 1;

        case ReliveTypes::eBaseBomb:
        case ReliveTypes::eMeatSaw:
        case ReliveTypes::eExplosion:
            if (mHealth > FP_FromInteger(0))
            {
                mHealth = FP_FromInteger(0);

                relive_new Gibs(
                    GibType::Mud_4,
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    mBaseAnimatedWithPhysicsGameObject_YPos,
                    FP_FromInteger(0),
                    FP_FromInteger(0),
                    mBaseAnimatedWithPhysicsGameObject_SpriteScale);

                relive_new Gibs(
                    GibType::Mud_4,
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    mBaseAnimatedWithPhysicsGameObject_YPos,
                    FP_FromInteger(0),
                    FP_FromInteger(0),
                    mBaseAnimatedWithPhysicsGameObject_SpriteScale);

                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                EventBroadcast(kEventMudokonDead, sActiveHero_507678);
            }
            return 1;

        case ReliveTypes::eAbilityRing:
            return 0;

        case ReliveTypes::eBullet:
            field_106_shot = TRUE;
            if (mHealth > FP_FromInteger(0))
            {
                auto pBullet = static_cast<Bullet*>(pFrom);
                FP bloodXOff = {};
                if (pBullet->field_20_x_distance <= FP_FromInteger(0))
                {
                    bloodXOff = FP_FromInteger(-24);
                }
                else
                {
                    bloodXOff = FP_FromInteger(24);
                }

                relive_new Blood(
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    pBullet->field_1C_ypos,
                    bloodXOff,
                    FP_FromInteger(0),
                    mBaseAnimatedWithPhysicsGameObject_SpriteScale,
                    50);

                EventBroadcast(kEventMudokonDead, sActiveHero_507678);
                EventBroadcast(kEventMudokonDead, sActiveHero_507678);
                return DoSmashDamage();
            }
            return 1;

        case ReliveTypes::eRockSpawner:
            EventBroadcast(kEventMudokonDead, sActiveHero_507678);
            return DoSmashDamage();

        case ReliveTypes::eScrab:
            return DoSmashDamage();

        case ReliveTypes::eShrykull:
        case ReliveTypes::eElectrocute:
            if (mHealth > FP_FromInteger(0))
            {
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                mHealth = FP_FromInteger(0);
            }
            return 1;

        case ReliveTypes::eSlog:
            if (mHealth > FP_FromInteger(0))
            {
                field_144_flags.Clear(Flags_144::e144_Bit6_bPersist);
                mHealth = FP_FromInteger(0);
                field_1B8_brain_state = 11;
                field_1C0_timer = sGnFrame + 90;
                mCurrentMotion = eMudMotions::Motion_46_FallLandDie;
                VUpdateResBlock();
            }
            return 1;

        case ReliveTypes::eBeeSwarm:
            if (mHealth > FP_FromInteger(0))
            {
                mHealth -= FP_FromDouble(0.15);
                if (mHealth > FP_FromInteger(0))
                {
                    Mudokon_SFX(Math_RandomRange(0, 127) >= 64 ? MudSounds::eBeesStruggle_18 : MudSounds::eKnockbackOuch_10, 0, 2 * Math_NextRandom() - 256, 0);
                    field_1B6 = 1;
                }
                else
                {
                    Mudokon_SFX(MudSounds::eKnockbackOuch_10, 0, 1000, this);
                    Environment_SFX_42A220(EnvironmentSfx::eDeathNoise_7, 0, 0x7FFF, this);
                    field_144_flags.Clear(Flags_144::e144_Bit6_bPersist);
                    mHealth = FP_FromInteger(0);
                    field_1B8_brain_state = 11;
                    field_1C0_timer = sGnFrame + 90;
                    mCurrentMotion = eMudMotions::Motion_46_FallLandDie;
                    mNextMotion = -1;
                    VUpdateResBlock();
                }
            }
            return 1;

        default:
            if (mHealth > FP_FromInteger(0))
            {
                Mudokon_SFX(Math_RandomRange(0, 127) >= 64 ? MudSounds::eBeesStruggle_18 : MudSounds::eKnockbackOuch_10, 127, 2 * Math_NextRandom() - 256, 0);
                mHealth -= FP_FromDouble(0.06);
                mCurrentMotion = eMudMotions::Motion_46_FallLandDie;
                mNextMotion = -1;
                field_1C0_timer = sGnFrame + 30;
                VUpdateResBlock();
                field_1B8_brain_state = 9;
                field_1BA_brain_sub_state = 6;

                RemoveAlerted();

                if (mHealth <= FP_FromInteger(0))
                {
                    field_144_flags.Clear(Flags_144::e144_Bit6_bPersist);
                    field_1B8_brain_state = 11;
                    field_1C0_timer = sGnFrame + 90;
                }
            }
            return 1;
    }
}

s16 Mudokon::DoSmashDamage()
{
    if (mHealth > FP_FromInteger(0))
    {
        field_144_flags.Clear(Flags_144::e144_Bit6_bPersist);
        field_1C0_timer = sGnFrame + 90;
        mHealth = FP_FromInteger(0);
        field_1B8_brain_state = 11;
        mCurrentMotion = eMudMotions::Motion_46_FallLandDie;
        mNextMotion = -1;
        VUpdateResBlock();
        Mudokon_SFX(MudSounds::eKnockbackOuch_10, 0, Math_RandomRange(-127, 127), this);
        return 1;
    }
    return 0;
}

void Mudokon::KillBirdPortal()
{
    if (field_1AC_pBirdPortal)
    {
        sMudRunningToPortalCount_507B94--;
        if (sMudRunningToPortalCount_507B94 == 0)
        {
            field_1AC_pBirdPortal->VKillPortalClipper();
            field_1AC_pBirdPortal->VGiveShrykull(1);
        }

        field_1AC_pBirdPortal->mBaseGameObjectRefCount--;
        field_1AC_pBirdPortal = nullptr;
    }
}

u8** Mudokon::GetResBlockForMotion(s16 motion)
{
    if (motion < eMudMotions::Motion_11_Null)
    {
        return field_148_res_array.res[0];
    }
    if (motion < eMudMotions::Motion_15_LeverUse)
    {
        return field_148_res_array.res[1];
    }
    if (motion < eMudMotions::Motion_16_StandScrubLoop)
    {
        return field_148_res_array.res[2];
    }
    if (motion < eMudMotions::Motion_22_CrouchScrub)
    {
        return field_148_res_array.res[3];
    }
    if (motion < eMudMotions::Motion_23_CrouchIdle)
    {
        return field_148_res_array.res[4];
    }
    if (motion < eMudMotions::Motion_46_FallLandDie)
    {
        return field_148_res_array.res[5];
    }
    if (motion < eMudMotions::Motion_47_Knockback)
    {
        return field_148_res_array.res[6];
    }
    if (motion < eMudMotions::Motion_49_WalkOffEdge)
    {
        return field_148_res_array.res[7];
    }
    if (motion < eMudMotions::Motion_52_Chant)
    {
        return field_148_res_array.res[8];
    }
    if (motion < eMudMotions::Motion_54_ToDuck)
    {
        return field_148_res_array.res[9];
    }
    if (motion < eMudMotions::Motion_57_Struggle)
    {
        return field_148_res_array.res[10];
    }
    if (motion < eMudMotions::Motion_58_StruggleToCrouchChant)
    {
        return field_148_res_array.res[11];
    }
    if (motion < eMudMotions::Motion_61_DuckKnockback)
    {
        return field_148_res_array.res[12];
    }
    if (motion < eMudMotions::Motion_62_PoisonGasDeath)
    {
        return field_148_res_array.res[13];
    }

    if (motion >= 63)
    {
        return nullptr;
    }

    return field_148_res_array.res[14];
}

void Mudokon::DoPathTrans()
{
    PSX_Point camCoords = {};
    gMap.GetCurrentCamCoords(&camCoords);

    if (sActiveHero_507678->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger((camCoords.x + XGrid_Index_To_XPos(mBaseAnimatedWithPhysicsGameObject_SpriteScale, MaxGridBlocks_41FA10(mBaseAnimatedWithPhysicsGameObject_SpriteScale)))) + ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_XPos = (FP_FromInteger(camCoords.x + XGrid_Index_To_XPos(mBaseAnimatedWithPhysicsGameObject_SpriteScale, 0))) - ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
    }

    if (sActiveHero_507678->BaseAliveGameObjectCollisionLine)
    {
        mBaseAnimatedWithPhysicsGameObject_YPos = sActiveHero_507678->mBaseAnimatedWithPhysicsGameObject_YPos;
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos) % 480 + camCoords.y);
    }

    if (BaseAliveGameObjectCollisionLine)
    {
        if (BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eDynamicCollision_32)
        {
            mLiftPoint->VRemove(this);
            mLiftPoint->mBaseGameObjectRefCount--;
            mLiftPoint = nullptr;
        }

        PathLine* pLine = nullptr;
        FP hitY = {};
        FP hitX = {};
        if (sCollisions->Raycast(
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(40),
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(40),
                &pLine,
                &hitX,
                &hitY,
                mBaseAnimatedWithPhysicsGameObject_SpriteScale != FP_FromDouble(0.5) ? kFgWallsOrFloor : kBgWallsOrFloor))
        {
            mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
            BaseAliveGameObjectCollisionLine = pLine;
        }
        else
        {
            BaseAliveGameObjectCollisionLine = nullptr;
        }
    }
    mBaseGameObjectUpdateDelay = 20;
    mBaseAnimatedWithPhysicsGameObject_LvlNumber = gMap.mCurrentLevel;
    mBaseAnimatedWithPhysicsGameObject_PathNumber = gMap.mCurrentPath;
}

void Mudokon::ToStand()
{
    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit6_FlipY);
    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit7_SwapXY);
    field_11C = FP_FromInteger(0);
    field_120_unused = 0;
    mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
    mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);
    mCurrentMotion = eMudMotions::Motion_0_Idle;
}

void Mudokon::CheckFloorGone()
{
    if (mLiftPoint)
    {
        if (mLiftPoint->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
        {
            VOnTrapDoorOpen();
            field_144_flags.Set(Flags_144::e144_Bit8);
        }
    }
    else
    {
        if (field_144_flags.Get(Flags_144::e144_Bit8))
        {
            Motion_51_Fall();
        }
    }
}

s16 Mudokon::CanAbeSneak(Mudokon* /*pMud*/)
{
    return sActiveHero_507678->mCurrentMotion == eAbeMotions::Motion_42_SneakLoop_424BB0
        || sActiveHero_507678->mCurrentMotion == eAbeMotions::Motion_47_SneakBegin_424ED0
        || sActiveHero_507678->mCurrentMotion == eAbeMotions::Motion_43_WalkToSneak_424790
        || sActiveHero_507678->mCurrentMotion == eAbeMotions::Motion_45_MidWalkToSneak_424890
        || sActiveHero_507678->mCurrentMotion == eAbeMotions::Motion_44_SneakToWalk_4249A0
        || sActiveHero_507678->mCurrentMotion == eAbeMotions::Motion_46_MidSneakToWalk_424AA0;
}

void Mudokon::ToKnockback()
{
    Environment_SFX_42A220(EnvironmentSfx::eKnockback_13, 0, 0x7FFF, this);

    if (mBaseAnimatedWithPhysicsGameObject_VelX > FP_FromInteger(0))
    {
        if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
        {
            mBaseAnimatedWithPhysicsGameObject_XPos += mBaseAnimatedWithPhysicsGameObject_VelX + (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(2));
        }
        else
        {
            mBaseAnimatedWithPhysicsGameObject_XPos += mBaseAnimatedWithPhysicsGameObject_VelX - (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(2));
        }
    }
    MapFollowMe_401D30(field_144_flags.Get(Flags_144::e144_Bit4_bSnapToGrid));

    mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);

    if (mBaseAnimatedWithPhysicsGameObject_VelY < FP_FromInteger(0))
    {
        mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);
    }

    Environment_SFX_42A220(EnvironmentSfx::eKnockback_13, 0, 0x7FFF, this);
    mCurrentMotion = eMudMotions::Motion_47_Knockback;
}

void Mudokon::ReduceXVelocityBy(FP amount)
{
    if (mBaseAnimatedWithPhysicsGameObject_VelX > FP_FromInteger(0))
    {
        mBaseAnimatedWithPhysicsGameObject_VelX -= (mBaseAnimatedWithPhysicsGameObject_SpriteScale * amount);
        if (mBaseAnimatedWithPhysicsGameObject_VelX < FP_FromInteger(0))
        {
            mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
        }
    }
    else if (mBaseAnimatedWithPhysicsGameObject_VelX < FP_FromInteger(0))
    {
        mBaseAnimatedWithPhysicsGameObject_VelX += (mBaseAnimatedWithPhysicsGameObject_SpriteScale * amount);
        if (mBaseAnimatedWithPhysicsGameObject_VelX > FP_FromInteger(0))
        {
            mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
        }
    }

    if (FP_GetDouble(mBaseAnimatedWithPhysicsGameObject_VelX))
    {
        MoveOnLine();
    }
}

void Mudokon::MoveOnLine()
{
    CheckFloorGone();

    const FP old_xpos = mBaseAnimatedWithPhysicsGameObject_XPos;
    if (BaseAliveGameObjectCollisionLine)
    {
        BaseAliveGameObjectCollisionLine = BaseAliveGameObjectCollisionLine->MoveOnLine(
            &mBaseAnimatedWithPhysicsGameObject_XPos,
            &mBaseAnimatedWithPhysicsGameObject_YPos,
            mBaseAnimatedWithPhysicsGameObject_VelX);
    }

    if (BaseAliveGameObjectCollisionLine)
    {
        if (mLiftPoint)
        {
            if (BaseAliveGameObjectCollisionLine->mLineType != eLineTypes::eDynamicCollision_32 &&
                BaseAliveGameObjectCollisionLine->mLineType != eLineTypes::eBackgroundDynamicCollision_36)
            {
                mLiftPoint->VRemove(this);
                mLiftPoint->mBaseGameObjectRefCount--;
                mLiftPoint = nullptr;
            }
        }
        else
        {
            if (BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eDynamicCollision_32 ||
                BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eBackgroundDynamicCollision_36)
            {
                const PSX_RECT bRect = VGetBoundingRect();
                VOnCollisionWith(
                    PSX_Point{bRect.x, static_cast<s16>(bRect.y + 5)},
                    PSX_Point{bRect.w, static_cast<s16>(bRect.h + 5)},
                    ObjListPlatforms_50766C,
                    1,
                    (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection_401C10);
            }
        }
    }
    else
    {
        field_11C = FP_FromInteger(0); // TODO: is 0 wrong? check AE
        BaseAliveGameObjectLastLineYPos = mBaseAnimatedWithPhysicsGameObject_YPos;
        mCurrentMotion = eMudMotions::Motion_49_WalkOffEdge;
        mBaseAnimatedWithPhysicsGameObject_XPos = old_xpos + mBaseAnimatedWithPhysicsGameObject_VelX;
    }
}

s16 Mudokon::FindBirdPortal()
{
    if (field_1AC_pBirdPortal)
    {
        return 0;
    }

    field_1AC_pBirdPortal = static_cast<BirdPortal*>(EventGet(kEventPortalOpen));
    if (!field_1AC_pBirdPortal)
    {
        return 0;
    }

    if (FP_Abs(field_1AC_pBirdPortal->field_18_xpos - mBaseAnimatedWithPhysicsGameObject_XPos) < FP_FromInteger(gPsxDisplay.mWidth) && FP_Abs(field_1AC_pBirdPortal->field_28_ypos - mBaseAnimatedWithPhysicsGameObject_YPos) < FP_FromInteger(10))
    {
        if (field_1AC_pBirdPortal->field_10_portal_type == PortalType::eWorker_1 || field_1AC_pBirdPortal->field_10_portal_type == PortalType::eShrykull_2)
        {
            sActiveHero_507678->ChangeChantState_430510(1);
            field_1AC_pBirdPortal->mBaseGameObjectRefCount++;
            sMudRunningToPortalCount_507B94++;
            return 1;
        }
    }
    field_1AC_pBirdPortal = nullptr;
    return 0;
}

s16 Mudokon::FacingBirdPortal(BirdPortal* pTarget)
{
    if (pTarget->field_18_xpos == mBaseAnimatedWithPhysicsGameObject_XPos)
    {
        return TRUE;
    }
    else if (pTarget->field_18_xpos > mBaseAnimatedWithPhysicsGameObject_XPos && !mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        return TRUE;
    }
    else if (pTarget->field_18_xpos < mBaseAnimatedWithPhysicsGameObject_XPos && mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        return TRUE;
    }
    return FALSE;
}

GameSpeakEvents Mudokon::LastGameSpeak()
{
    if (!gMap.Is_Point_In_Current_Camera(
            mBaseAnimatedWithPhysicsGameObject_LvlNumber,
            mBaseAnimatedWithPhysicsGameObject_PathNumber,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos,
            1)
        || field_144_flags.Get(Flags_144::e144_Bit11_bDeaf))
    {
        return GameSpeakEvents::eNone_m1;
    }

    if (field_128 == pEventSystem_4FF954->field_18_last_event_index)
    {
        if (pEventSystem_4FF954->field_10_last_event == GameSpeakEvents::eNone_m1)
        {
            return GameSpeakEvents::eNone_m1;
        }
        else
        {
            return GameSpeakEvents::eSameAsLast_m2;
        }
    }

    field_128 = pEventSystem_4FF954->field_18_last_event_index;

    return pEventSystem_4FF954->field_10_last_event;
}

void Mudokon::AddAlerted()
{
    if (!field_144_flags.Get(Flags_144::e144_Bit9))
    {
        sAlertedMudCount_507B90++;
        field_144_flags.Set(Flags_144::e144_Bit9);
    }
}


void Mudokon::RemoveAlerted()
{
    if (field_144_flags.Get(Flags_144::e144_Bit9))
    {
        sAlertedMudCount_507B90--;
        field_144_flags.Clear(Flags_144::e144_Bit9);
    }
}

s16 Mudokon::IAmNearestToAbe()
{
    for (s32 i = 0; i < gBaseAliveGameObjects_4FC8A0->Size(); i++)
    {
        BaseAliveGameObject* pObjIter = gBaseAliveGameObjects_4FC8A0->ItemAt(i);
        if (!pObjIter)
        {
            break;
        }

        if (pObjIter != this)
        {
            if (pObjIter->mBaseGameObjectTypeId == ReliveTypes::eRingOrLiftMud || pObjIter->mBaseGameObjectTypeId == ReliveTypes::eMudokon) // mud or password mud?
            {
                if (Math_Distance(
                        FP_GetExponent(sActiveHero_507678->mBaseAnimatedWithPhysicsGameObject_XPos),
                        FP_GetExponent(sActiveHero_507678->mBaseAnimatedWithPhysicsGameObject_YPos),
                        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
                        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos))
                    > Math_Distance(
                        FP_GetExponent(sActiveHero_507678->mBaseAnimatedWithPhysicsGameObject_XPos),
                        FP_GetExponent(sActiveHero_507678->mBaseAnimatedWithPhysicsGameObject_YPos),
                        FP_GetExponent(pObjIter->mBaseAnimatedWithPhysicsGameObject_XPos),
                        FP_GetExponent(pObjIter->mBaseAnimatedWithPhysicsGameObject_YPos)))
                {
                    // Our distance is to abe is larger than the mud we just found so we can't be nearest
                    return FALSE;
                }
            }
        }
    }
    return TRUE;
}

void Mudokon::VOnTrapDoorOpen()
{
    if (mLiftPoint)
    {
        if (field_106_shot)
        {
            field_144_flags.Set(Flags_144::e144_Bit8);
        }
        else
        {
            VSetMotion(eMudMotions::Motion_49_WalkOffEdge);
        }

        mLiftPoint->VRemove(this);
        mLiftPoint->mBaseGameObjectRefCount--;
        mLiftPoint = nullptr;
    }
}

void Mudokon::VOnTlvCollision(Path_TLV* pTlv)
{
    if (pTlv)
    {
        while (pTlv)
        {
            if (pTlv->mTlvType32 == TlvTypes::DeathDrop_5)
            {
                if (mHealth > FP_FromInteger(0))
                {
                    field_1BA_brain_sub_state = 0;
                    mHealth = FP_FromInteger(0);
                    field_1B8_brain_state = 13;
                    EventBroadcast(kEventMudokonDead, sActiveHero_507678);
                }
            }
            pTlv = gMap.TLV_Get_At_446060(pTlv, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos);
        }
    }
}

void Mudokon::Motion_0_Idle()
{
    CheckFloorGone();

    BaseAliveGameObjectLastLineYPos = mBaseAnimatedWithPhysicsGameObject_YPos;

    switch (mNextMotion)
    {
        case eMudMotions::Motion_1_WalkLoop:
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
            {
                mBaseAnimatedWithPhysicsGameObject_VelX = -(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(9));
            }
            else
            {
                mBaseAnimatedWithPhysicsGameObject_VelX = (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(9));
            }
            mCurrentMotion = eMudMotions::Motion_7_WalkBegin;
            mNextMotion = -1;
            break;

        case eMudMotions::Motion_35_SneakLoop:
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
            {
                mBaseAnimatedWithPhysicsGameObject_VelX = -(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(10));
            }
            else
            {
                mBaseAnimatedWithPhysicsGameObject_VelX = (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(10));
            }
            mCurrentMotion = eMudMotions::Motion_40_SneakBegin;
            mNextMotion = -1;
            break;

        case eMudMotions::Motion_29_RunLoop:
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
            {
                mBaseAnimatedWithPhysicsGameObject_VelX = -(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(4));
            }
            else
            {
                mBaseAnimatedWithPhysicsGameObject_VelX = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(4);
            }
            mCurrentMotion = eMudMotions::Motion_45_StandToRun;
            mNextMotion = -1;
            break;

        case eMudMotions::Motion_52_Chant:
            SND_SEQ_PlaySeq_4775A0(SeqId::eMudokonChant_12, 0, 1);
            mCurrentMotion = eMudMotions::Motion_52_Chant;
            mNextMotion = -1;
            break;

        case eMudMotions::Motion_23_CrouchIdle:
            mCurrentMotion = eMudMotions::Motion_25_StandToCrouch;
            mNextMotion = -1;
            break;

        case eMudMotions::Motion_59_CrouchChant:
            mCurrentMotion = eMudMotions::Motion_58_StruggleToCrouchChant;
            mNextMotion = -1;
            break;

        case eMudMotions::Motion_16_StandScrubLoop:
            mCurrentMotion = eMudMotions::Motion_20_IdleToStandScrub;
            mNextMotion = -1;
            break;

        default:
            if (mNextMotion != -1)
            {
                mCurrentMotion = mNextMotion;
                mNextMotion = -1;
            }
            break;
    }
}

void Mudokon::Motion_1_WalkLoop()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (WallHit_401930(
            mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(50),
            mBaseAnimatedWithPhysicsGameObject_VelX))
    {
        ToStand();
    }
    else
    {
        MoveOnLine();
        if (mCurrentMotion == eMudMotions::Motion_1_WalkLoop)
        {
            switch (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame)
            {
                case 2:
                    if (mNextMotion == eMudMotions::Motion_0_Idle)
                    {
                        mCurrentMotion = eMudMotions::Motion_9_MidWalkToIdle;
                        mNextMotion = -1;
                    }
                    break;

                case 5:
                    Environment_SFX_42A220(EnvironmentSfx::eWalkingFootstep_1, 0, 0x7FFF, this);
                    if (!field_144_flags.Get(Flags_144::e144_Bit7))
                    {
                        field_144_flags.Set(Flags_144::e144_Bit7);
                        MapFollowMe_401D30(field_144_flags.Get(Flags_144::e144_Bit4_bSnapToGrid));
                    }

                    if (mNextMotion == eMudMotions::Motion_29_RunLoop)
                    {
                        mCurrentMotion = eMudMotions::Motion_28_MidWalkToRun;
                        mNextMotion = -1;
                    }
                    else if (mNextMotion == eMudMotions::Motion_35_SneakLoop)
                    {
                        mCurrentMotion = eMudMotions::Motion_36_MidWalkToSneak;
                        mNextMotion = -1;
                    }
                    break;

                case 11:
                    if (mNextMotion == eMudMotions::Motion_0_Idle)
                    {
                        mCurrentMotion = eMudMotions::Motion_8_WalkToIdle;
                        mNextMotion = -1;
                    }
                    break;

                case 14:
                    Environment_SFX_42A220(EnvironmentSfx::eWalkingFootstep_1, 0, 0x7FFF, this);
                    if (!field_144_flags.Get(Flags_144::e144_Bit7))
                    {
                        field_144_flags.Set(Flags_144::e144_Bit7);
                        MapFollowMe_401D30(field_144_flags.Get(Flags_144::e144_Bit4_bSnapToGrid));
                    }

                    if (mNextMotion == eMudMotions::Motion_29_RunLoop)
                    {
                        mCurrentMotion = eMudMotions::Motion_27_WalkToRun;
                        mNextMotion = -1;
                    }
                    else if (mNextMotion == eMudMotions::Motion_35_SneakLoop)
                    {
                        mCurrentMotion = eMudMotions::Motion_38_WalkToSneak;
                        mNextMotion = -1;
                    }
                    break;

                default:
                    field_144_flags.Clear(Flags_144::e144_Bit7);
                    break;
            }
        }
    }
}

void Mudokon::Motion_2_StandingTurn()
{
    CheckFloorGone();

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 0)
    {
        Environment_SFX_42A220(EnvironmentSfx::eGenericMovement_9, 0, 0x7FFF, this);
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eMudMotions::Motion_0_Idle;
        mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Toggle(AnimFlags::eBit5_FlipX);
    }
}

void Mudokon::Motion_3_Speak()
{
    CheckFloorGone();

    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eMudMotions::Motion_0_Idle;
    }
}

void Mudokon::Motion_4_Speak()
{
    Motion_3_Speak();
}

void Mudokon::Motion_5_Speak()
{
    Motion_3_Speak();
}

void Mudokon::Motion_6_Speak()
{
    Motion_3_Speak();
}

void Mudokon::Motion_7_WalkBegin()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (WallHit_401930(
            mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(50),
            mBaseAnimatedWithPhysicsGameObject_VelX))
    {
        ToStand();
    }
    else
    {
        if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            mCurrentMotion = eMudMotions::Motion_1_WalkLoop;
        }
        MoveOnLine();
    }
}

void Mudokon::Motion_8_WalkToIdle()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (WallHit_401930(
            mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(50),
            mBaseAnimatedWithPhysicsGameObject_VelX))
    {
        ToStand();
    }
    else
    {
        MoveOnLine();

        if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 0)
        {
            Environment_SFX_42A220(EnvironmentSfx::eWalkingFootstep_1, 0, 0x7FFF, this);
            return;
        }

        if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            MapFollowMe_401D30(field_144_flags.Get(Flags_144::e144_Bit4_bSnapToGrid));
            ToStand();
        }
    }
}

void Mudokon::Motion_9_MidWalkToIdle()
{
    Motion_8_WalkToIdle();
}

void Mudokon::Motion_10_Unused()
{
    if (sNumCamSwappers_507668 <= 0)
    {
        mCurrentMotion = mPreviousMotion;
        if (mLiftPoint)
        {
            mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger((BaseAliveGameObjectCollisionLine->mRect.x + BaseAliveGameObjectCollisionLine->mRect.w) / 2);
            mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(BaseAliveGameObjectCollisionLine->mRect.y);
        }
    }
}

void Mudokon::Motion_11_Null()
{
    // Empty
}

void Mudokon::Motion_12_LiftUse()
{
    auto pLiftPoint = static_cast<LiftPoint*>(field_194_pLiftPoint);
    if (!pLiftPoint->OnAnyFloor() || pLiftPoint->field_27A_flags.Get(LiftPoint::Flags::eBit7_bIgnoreLiftMover))
    {
        pLiftPoint->Move(FP_FromInteger(0), FP_FromInteger(3), 0);
    }
    else
    {
        pLiftPoint->Move(FP_FromInteger(0), FP_FromInteger(0), 0);
        mCurrentMotion = eMudMotions::Motion_14_LiftGrabEnd;
        mNextMotion = eMudMotions::Motion_0_Idle;
    }
}

void Mudokon::Motion_13_LiftGrabBegin()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = mNextMotion;
        field_194_pLiftPoint->field_27A_flags.Set(LiftPoint::Flags::eBit8_KeepOnMiddleFloor);
    }
}

void Mudokon::Motion_14_LiftGrabEnd()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = mNextMotion;
    }
}

void Mudokon::Motion_15_LeverUse()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eMudMotions::Motion_0_Idle;
    }
}

void Mudokon::Motion_16_StandScrubLoop()
{
    if (gMap.Is_Point_In_Current_Camera(
            mBaseAnimatedWithPhysicsGameObject_LvlNumber,
            mBaseAnimatedWithPhysicsGameObject_PathNumber,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos,
            0))
    {
        if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame)
        {
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 5)
            {
                SfxPlayMono(SoundEffect::Clean2_86, 0, 0);
            }
        }
        else
        {
            SfxPlayMono(SoundEffect::Clean1_85, 0, 0);
        }
    }

    if (mNextMotion != -1)
    {
        if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            if (mNextMotion != eMudMotions::Motion_0_Idle)
            {
                mCurrentMotion = mNextMotion;
            }
            else
            {
                mCurrentMotion = eMudMotions::Motion_21_StandScrubToIdle;
            }
            mNextMotion = -1;
        }
    }
}

void Mudokon::Motion_17_StandScrubLoopToPause()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eMudMotions::Motion_19_StandScrubPause;
    }
}

void Mudokon::Motion_18_StandScrubPauseToLoop()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eMudMotions::Motion_16_StandScrubLoop;
    }
}

void Mudokon::Motion_19_StandScrubPause()
{
    if (mNextMotion != -1)
    {
        if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            mCurrentMotion = eMudMotions::Motion_18_StandScrubPauseToLoop;
            mNextMotion = -1;
        }
    }
}

void Mudokon::Motion_20_IdleToStandScrub()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eMudMotions::Motion_16_StandScrubLoop;
    }
}

void Mudokon::Motion_21_StandScrubToIdle()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eMudMotions::Motion_0_Idle;
    }
}

void Mudokon::Motion_22_CrouchScrub()
{
    if (gMap.Is_Point_In_Current_Camera(
            mBaseAnimatedWithPhysicsGameObject_LvlNumber,
            mBaseAnimatedWithPhysicsGameObject_PathNumber,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos,
            0))
    {
        if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 2)
        {
            SfxPlayMono(SoundEffect::Clean1_85, 0, 0);
        }
        else if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 6)
        {
            SfxPlayMono(SoundEffect::Clean2_86, 0, 0);
        }
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eMudMotions::Motion_23_CrouchIdle;
    }
}

void Mudokon::Motion_23_CrouchIdle()
{
    CheckFloorGone();

    if (mNextMotion == eMudMotions::Motion_55_Duck)
    {
        mCurrentMotion = eMudMotions::Motion_54_ToDuck;
        mNextMotion = -1;
    }
    else if (mNextMotion == eMudMotions::Motion_0_Idle)
    {
        mCurrentMotion = eMudMotions::Motion_26_CrouchToStand;
        mNextMotion = -1;
    }
    else if (mNextMotion != -1)
    {
        mCurrentMotion = mNextMotion;
        mNextMotion = -1;
    }
}

void Mudokon::Motion_24_CrouchTurn()
{
    CheckFloorGone();

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eMudMotions::Motion_23_CrouchIdle;
        mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Toggle(AnimFlags::eBit5_FlipX);
    }
}

void Mudokon::Motion_25_StandToCrouch()
{
    CheckFloorGone();

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eMudMotions::Motion_23_CrouchIdle;
    }
}

void Mudokon::Motion_26_CrouchToStand()
{
    CheckFloorGone();

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eMudMotions::Motion_0_Idle;
    }
}

void Mudokon::Motion_27_WalkToRun()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        mBaseAnimatedWithPhysicsGameObject_VelX = -(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(4));
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_VelX = (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(4));
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eMudMotions::Motion_29_RunLoop;
    }

    if (WallHit_401930(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(50), mBaseAnimatedWithPhysicsGameObject_VelX))
    {
        ToStand();
    }
    else
    {
        MoveOnLine();
    }
}

void Mudokon::Motion_28_MidWalkToRun()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        mBaseAnimatedWithPhysicsGameObject_VelX = -(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(4));
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_VelX = (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(4));
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mPreviousMotion = eMudMotions::Motion_29_RunLoop;
        mBaseAliveGameObjectLastAnimFrame = eMudMotions::Motion_8_WalkToIdle;
        field_1BC = 1;
    }

    if (WallHit_401930(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(50), mBaseAnimatedWithPhysicsGameObject_VelX))
    {
        ToStand();
    }
    else
    {
        MoveOnLine();
    }
}

void Mudokon::Motion_29_RunLoop()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (WallHit_401930(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(50), mBaseAnimatedWithPhysicsGameObject_VelX))
    {
        ToKnockback();
        return;
    }

    MoveOnLine();

    if (mCurrentMotion == eMudMotions::Motion_29_RunLoop)
    {
        if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 0 || mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 8)
        {
            if (!field_144_flags.Get(Flags_144::e144_Bit7))
            {
                field_144_flags.Set(Flags_144::e144_Bit7);
                MapFollowMe_401D30(field_144_flags.Get(Flags_144::e144_Bit4_bSnapToGrid));
            }

            if (mNextMotion == eMudMotions::Motion_44_RunJumpMid)
            {
                mCurrentMotion = eMudMotions::Motion_43_RunJumpBegin;
                mNextMotion = -1;
            }
        }
        else if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 4 || mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 12)
        {
            Environment_SFX_42A220(EnvironmentSfx::eRunningFootstep_2, 0, 0x7FFF, this);
            if (!field_144_flags.Get(Flags_144::e144_Bit7))
            {
                field_144_flags.Set(Flags_144::e144_Bit7);
                MapFollowMe_401D30(field_144_flags.Get(Flags_144::e144_Bit4_bSnapToGrid));
            }

            switch (mNextMotion)
            {
                case eMudMotions::Motion_1_WalkLoop:
                    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 4)
                    {
                        mCurrentMotion = eMudMotions::Motion_30_RunToWalk;
                        mNextMotion = -1;
                    }
                    return;

                    // TODO: OG bug - MidRunToWalk will always be set on frame 12. AE doesn't do this.
                    // couldn't see any breaking changes after commenting this out.

                    /*case eMudMotions::Motion_29_RunLoop_43DB10:
                if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 12)
                {
                    mCurrentMotion = eMudMotions::Motion_28_MidWalkToRun_43DA40;
                    field_FE_next_state = -1;
                }
                return;*/

                case eMudMotions::Motion_0_Idle:
                    mCurrentMotion = eMudMotions::Motion_32_RunSlideStop;
                    mNextMotion = -1;
                    Environment_SFX_42A220(EnvironmentSfx::eRunSlide_4, 0, 0x7FFF, this);
                    return;

                case eMudMotions::Motion_33_RunSlideTurn:
                    mCurrentMotion = eMudMotions::Motion_33_RunSlideTurn;
                    mNextMotion = -1;
                    Environment_SFX_42A220(EnvironmentSfx::eRunSlide_4, 0, 0x7FFF, this);
                    return;

                case eMudMotions::Motion_44_RunJumpMid:
                    mCurrentMotion = eMudMotions::Motion_43_RunJumpBegin;
                    mNextMotion = -1;
                    return;
            }
        }
        else
        {
            field_144_flags.Clear(Flags_144::e144_Bit7);
        }
    }
}

void Mudokon::Motion_30_RunToWalk()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        mBaseAnimatedWithPhysicsGameObject_VelX = -(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(9));
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_VelX = (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(9));
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eMudMotions::Motion_1_WalkLoop;
    }

    if (WallHit_401930(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(50), mBaseAnimatedWithPhysicsGameObject_VelX))
    {
        ToStand();
    }
    else
    {
        MoveOnLine();
    }
}

void Mudokon::Motion_31_MidRunToWalk()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        mBaseAnimatedWithPhysicsGameObject_VelX = -(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(9));
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_VelX = (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(9));
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mPreviousMotion = eMudMotions::Motion_1_WalkLoop;
        mBaseAliveGameObjectLastAnimFrame = 9;
        field_1BC = 1;
    }

    if (WallHit_401930(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(50), mBaseAnimatedWithPhysicsGameObject_VelX))
    {
        ToStand();
    }
    else
    {
        MoveOnLine();
    }
}

void Mudokon::Motion_32_RunSlideStop()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (WallHit_401930(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(50), mBaseAnimatedWithPhysicsGameObject_VelX))
    {
        ToKnockback();
    }
    else
    {
        ReduceXVelocityBy(FP_FromDouble(0.375));

        if (mCurrentMotion == eMudMotions::Motion_32_RunSlideStop)
        {
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                Environment_SFX_42A220(EnvironmentSfx::eWalkingFootstep_1, 0, 0x7FFF, this);
                MapFollowMe_401D30(field_144_flags.Get(Flags_144::e144_Bit4_bSnapToGrid));
                ToStand();
            }
        }
    }
}

void Mudokon::Motion_33_RunSlideTurn()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (WallHit_401930(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(50), mBaseAnimatedWithPhysicsGameObject_VelX))
    {
        ToKnockback();
    }
    else
    {
        ReduceXVelocityBy(FP_FromDouble(0.375));

        if (mCurrentMotion == eMudMotions::Motion_33_RunSlideTurn)
        {
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                MapFollowMe_401D30(field_144_flags.Get(Flags_144::e144_Bit4_bSnapToGrid));

                if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
                {
                    mBaseAnimatedWithPhysicsGameObject_VelX = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(4);
                    mCurrentMotion = eMudMotions::Motion_34_RunTurnToRun;
                }
                else
                {
                    mBaseAnimatedWithPhysicsGameObject_VelX = -(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(4));
                    mCurrentMotion = eMudMotions::Motion_34_RunTurnToRun;
                }
            }
        }
    }
}

void Mudokon::Motion_34_RunTurnToRun()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (WallHit_401930(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(50), mBaseAnimatedWithPhysicsGameObject_VelX))
    {
        ToStand();
    }
    else
    {
        MoveOnLine();
        if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            mCurrentMotion = eMudMotions::Motion_29_RunLoop;
            mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Toggle(AnimFlags::eBit5_FlipX);
        }
    }
}

void Mudokon::Motion_35_SneakLoop()
{
    if (WallHit_401930(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(50), mBaseAnimatedWithPhysicsGameObject_VelX))
    {
        ToStand();
    }
    else
    {
        MoveOnLine();
        if (mCurrentMotion == eMudMotions::Motion_35_SneakLoop)
        {
            switch (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame)
            {
                case 3:
                    if (!mNextMotion)
                    {
                        mCurrentMotion = eMudMotions::Motion_41_SneakToIdle;
                        mNextMotion = -1;
                    }
                    break;

                case 6:
                    Environment_SFX_42A220(EnvironmentSfx::eSneakFootstep_3, 0, 0x7FFF, this);
                    MapFollowMe_401D30(field_144_flags.Get(Flags_144::e144_Bit4_bSnapToGrid));

                    if (mNextMotion == eMudMotions::Motion_1_WalkLoop || mNextMotion == eMudMotions::Motion_29_RunLoop)
                    {
                        mCurrentMotion = eMudMotions::Motion_37_SneakToWalk;
                        mNextMotion = -1;
                    }
                    break;

                case 13:
                    if (!mNextMotion)
                    {
                        mCurrentMotion = eMudMotions::Motion_42_MidSneakToIdle;
                        mNextMotion = -1;
                    }
                    break;

                case 16:
                    Environment_SFX_42A220(EnvironmentSfx::eSneakFootstep_3, 0, 0x7FFF, this);
                    MapFollowMe_401D30(field_144_flags.Get(Flags_144::e144_Bit4_bSnapToGrid));

                    if (mNextMotion == eMudMotions::Motion_1_WalkLoop || mNextMotion == eMudMotions::Motion_29_RunLoop)
                    {
                        mCurrentMotion = eMudMotions::Motion_39_MidSneakToWalk;
                        mNextMotion = -1;
                    }
                    break;
            }
        }
    }
}

void Mudokon::Motion_36_MidWalkToSneak()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        mBaseAnimatedWithPhysicsGameObject_VelX = -(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(10));
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_VelX = (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(10));
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eMudMotions::Motion_35_SneakLoop;
    }

    if (WallHit_401930(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(50), mBaseAnimatedWithPhysicsGameObject_VelX))
    {
        ToStand();
    }
    else
    {
        MoveOnLine();
    }
}

void Mudokon::Motion_37_SneakToWalk()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        mBaseAnimatedWithPhysicsGameObject_VelX = -(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(9));
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_VelX = (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(9));
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eMudMotions::Motion_1_WalkLoop;
    }

    if (WallHit_401930(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(50), mBaseAnimatedWithPhysicsGameObject_VelX))
    {
        ToStand();
    }
    else
    {
        MoveOnLine();
    }
}

void Mudokon::Motion_38_WalkToSneak()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        mBaseAnimatedWithPhysicsGameObject_VelX = -(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(10));
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_VelX = (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(10));
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mPreviousMotion = eMudMotions::Motion_35_SneakLoop;
        mBaseAliveGameObjectLastAnimFrame = 10;
        field_1BC = 1;
    }

    if (WallHit_401930(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(50), mBaseAnimatedWithPhysicsGameObject_VelX))
    {
        ToStand();
    }
    else
    {
        MoveOnLine();
    }
}

void Mudokon::Motion_39_MidSneakToWalk()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        mBaseAnimatedWithPhysicsGameObject_VelX = -(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(9));
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_VelX = (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(9));
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mPreviousMotion = eMudMotions::Motion_1_WalkLoop;
        mBaseAliveGameObjectLastAnimFrame = 9;
        field_1BC = 1;
    }

    if (WallHit_401930(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(50), mBaseAnimatedWithPhysicsGameObject_VelX))
    {
        ToStand();
    }
    else
    {
        MoveOnLine();
    }
}

void Mudokon::Motion_40_SneakBegin()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eMudMotions::Motion_35_SneakLoop;
    }

    if (WallHit_401930(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(50), mBaseAnimatedWithPhysicsGameObject_VelX))
    {
        ToStand();
    }
    else
    {
        MoveOnLine();
    }
}

void Mudokon::Motion_41_SneakToIdle()
{
    MoveOnLine();

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        MapFollowMe_401D30(field_144_flags.Get(Flags_144::e144_Bit4_bSnapToGrid));
        ToStand();
    }
}

void Mudokon::Motion_42_MidSneakToIdle()
{
    MoveOnLine();

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        MapFollowMe_401D30(field_144_flags.Get(Flags_144::e144_Bit4_bSnapToGrid));
        ToStand();
    }
}

void Mudokon::Motion_43_RunJumpBegin()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    CheckFloorGone();

    mBaseAnimatedWithPhysicsGameObject_XPos += mBaseAnimatedWithPhysicsGameObject_VelX;

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 0)
    {
        Environment_SFX_42A220(EnvironmentSfx::eRunJumpOrLedgeHoist_11, 0, 0x7FFF, this);
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        BaseAliveGameObjectLastLineYPos = mBaseAnimatedWithPhysicsGameObject_YPos;

        if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
        {
            mBaseAnimatedWithPhysicsGameObject_VelX = mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromDouble(-7.6);
        }
        else
        {
            mBaseAnimatedWithPhysicsGameObject_VelX = mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromDouble(7.6);
        }

        mBaseAnimatedWithPhysicsGameObject_VelY = (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromDouble(-9.6));
        mBaseAnimatedWithPhysicsGameObject_YPos += mBaseAnimatedWithPhysicsGameObject_VelY;
        VOnTrapDoorOpen();
        mCurrentMotion = eMudMotions::Motion_44_RunJumpMid;
        BaseAliveGameObjectCollisionLine = nullptr;
    }
}

void Mudokon::Motion_44_RunJumpMid()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 5)
    {
        SFX_Play_Pitch(SoundEffect::PossessEffect_21, 40, 2400, 0);
    }

    const PSX_RECT bRect = VGetBoundingRect();

    if ((mBaseAnimatedWithPhysicsGameObject_VelX > FP_FromInteger(0) && (FP_FromInteger(bRect.x) > field_1AC_pBirdPortal->field_18_xpos)) || ((mBaseAnimatedWithPhysicsGameObject_VelX < FP_FromInteger(0) && FP_FromInteger(bRect.w) < field_1AC_pBirdPortal->field_18_xpos)))
    {
        field_144_flags.Clear(Flags_144::e144_Bit2);
        field_144_flags.Clear(Flags_144::e144_Bit6_bPersist);

        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);
        mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);

        SND_SEQ_Play_477760(SeqId::eSaveTriggerMusic_45, 1, 127, 127);


        relive_new MusicTrigger(MusicTriggerMusicType::eChime_5, TriggeredBy::eTimer_0, 0, 300);

        sRescuedMudokons_5076C0++;

        if (field_1AC_pBirdPortal)
        {
            field_1AC_pBirdPortal->VMudSaved();
        }

        if (field_1B2_rescue_switch_id)
        {
            SwitchStates_Set(field_1B2_rescue_switch_id, 1);
        }
    }

    mBaseAnimatedWithPhysicsGameObject_VelY += (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromDouble(1.8));
    mBaseAnimatedWithPhysicsGameObject_XPos += mBaseAnimatedWithPhysicsGameObject_VelX;
    mBaseAnimatedWithPhysicsGameObject_YPos += mBaseAnimatedWithPhysicsGameObject_VelY;
}

void Mudokon::Motion_45_StandToRun()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eMudMotions::Motion_29_RunLoop;
    }

    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (WallHit_401930(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(50), mBaseAnimatedWithPhysicsGameObject_VelX))
    {
        ToStand();
    }
    else
    {
        MoveOnLine();
    }
}

void Mudokon::Motion_46_FallLandDie()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 4)
    {
        Environment_SFX_42A220(EnvironmentSfx::eKnockback_13, 0, 0x7FFF, this);
    }

    if ((gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarms
         || gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarmsReturn
         || gMap.mCurrentLevel == EReliveLevelIds::eBoardRoom)
        && mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 7)
    {
        Environment_SFX_42A220(EnvironmentSfx::eHitGroundSoft_6, 80, -200, this);
    }

    CheckFloorGone();

    if (mNextMotion != -1)
    {
        mCurrentMotion = mNextMotion;
        mNextMotion = -1;
    }
}

void Mudokon::Motion_47_Knockback()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if ((gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarms
         || gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarmsReturn
         || gMap.mCurrentLevel == EReliveLevelIds::eBoardRoom)
        && mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 7)
    {
        Environment_SFX_42A220(EnvironmentSfx::eHitGroundSoft_6, 80, -200, this);
    }

    if (!BaseAliveGameObjectCollisionLine)
    {
        Motion_51_Fall();

        if (mCurrentMotion == eMudMotions::Motion_46_FallLandDie || mCurrentMotion == eMudMotions::Motion_50_LandSoft)
        {
            mCurrentMotion = eMudMotions::Motion_47_Knockback;
        }
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (BaseAliveGameObjectCollisionLine)
        {
            mCurrentMotion = eMudMotions::Motion_48_KnockbackGetUp;
        }
    }
}

void Mudokon::Motion_48_KnockbackGetUp()
{
    CheckFloorGone();

    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToStand();
    }
}

void Mudokon::Motion_49_WalkOffEdge()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eMudMotions::Motion_51_Fall;
    }
    Motion_51_Fall();
}

void Mudokon::Motion_50_LandSoft()
{
    CheckFloorGone();

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 2)
    {
        if (mPreviousMotion == eMudMotions::Motion_51_Fall)
        {
            Environment_SFX_42A220(EnvironmentSfx::eLandingSoft_5, 0, 0x7FFF, this);
        }
        else
        {
            Environment_SFX_42A220(EnvironmentSfx::eHitGroundSoft_6, 0, 0x7FFF, this);
        }
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToStand();
    }
}

void Mudokon::Motion_51_Fall()
{
    if (mBaseAnimatedWithPhysicsGameObject_VelX > FP_FromInteger(0))
    {
        mBaseAnimatedWithPhysicsGameObject_VelX -= mBaseAnimatedWithPhysicsGameObject_SpriteScale * field_11C;
        if (mBaseAnimatedWithPhysicsGameObject_VelX < FP_FromInteger(0))
        {
            mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
        }
    }
    else if (mBaseAnimatedWithPhysicsGameObject_VelX < FP_FromInteger(0))
    {
        mBaseAnimatedWithPhysicsGameObject_VelX += mBaseAnimatedWithPhysicsGameObject_SpriteScale * field_11C;
        if (mBaseAnimatedWithPhysicsGameObject_VelX > FP_FromInteger(0))
        {
            mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
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
                BaseAliveGameObjectCollisionLine = pLine;
                mBaseAnimatedWithPhysicsGameObject_XPos = hitX;
                mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
                MapFollowMe_401D30(field_144_flags.Get(Flags_144::e144_Bit4_bSnapToGrid));
                if (mBaseAnimatedWithPhysicsGameObject_YPos - BaseAliveGameObjectLastLineYPos > FP_FromInteger(240))
                {
                    BaseAliveGameObjectLastLineYPos += FP_FromInteger(240);
                }

                if (mBaseAnimatedWithPhysicsGameObject_YPos - BaseAliveGameObjectLastLineYPos < (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(180)) && (mHealth > FP_FromInteger(0) || gAbeInvulnerableCheat_5076E4))
                {
                    mCurrentMotion = eMudMotions::Motion_50_LandSoft;
                }
                else
                {
                    mHealth = FP_FromInteger(0);
                    ToKnockback();
                    field_144_flags.Clear(Flags_144::e144_Bit6_bPersist);
                    field_1B8_brain_state = 11;
                    field_1C0_timer = sGnFrame + 90;
                    mCurrentMotion = eMudMotions::Motion_46_FallLandDie;
                }

                mPreviousMotion = eMudMotions::Motion_51_Fall;

                PSX_Point xy;
                xy.x = FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos - FP_FromInteger(10));
                xy.y = FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(10));

                PSX_Point wh;
                wh.x = FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos + FP_FromInteger(10));
                wh.y = FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(10));

                VOnCollisionWith(
                    xy,
                    wh,
                    ObjListPlatforms_50766C,
                    1,
                    (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection_401C10);
                break;

            case eLineTypes::eWallLeft_1:
            case eLineTypes::eWallRight_2:
            case eLineTypes::eBackgroundWallLeft_5:
            case eLineTypes::eBackgroundWallRight_6:
                mBaseAnimatedWithPhysicsGameObject_XPos = hitX;
                mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
                ToKnockback();
                break;

            default:
                return;
        }
    }
}

void Mudokon::Motion_52_Chant()
{
    if (!(sGnFrame % 4))
    {
        const FP rndX = FP_FromInteger(40 * Math_NextRandom() / 256 - 20);
        const FP rndY = FP_FromInteger(30 * Math_NextRandom() / 256 + 30);

        New_Chant_Particle_4198E0(
            mBaseAnimatedWithPhysicsGameObject_XPos + (mBaseAnimatedWithPhysicsGameObject_SpriteScale * rndX),
            mBaseAnimatedWithPhysicsGameObject_YPos - (mBaseAnimatedWithPhysicsGameObject_SpriteScale * rndY),
            mBaseAnimatedWithPhysicsGameObject_SpriteScale,
            Layer::eLayer_0);
    }

    if (!SND_SsIsEos_DeInlined_477930(SeqId::eMudokonChant_12))
    {
        SND_SEQ_Play_477760(SeqId::eMudokonChant_12, 1, 50, 50);
    }

    if (mNextMotion == eMudMotions::Motion_0_Idle)
    {
        if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            SND_Seq_Stop_477A60(SeqId::eMudokonChant_12);
            mCurrentMotion = eMudMotions::Motion_53_ChantEnd;
            mNextMotion = -1;
        }
    }
}

void Mudokon::Motion_53_ChantEnd()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eMudMotions::Motion_0_Idle;
    }
}

void Mudokon::Motion_54_ToDuck()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eMudMotions::Motion_55_Duck;
    }
}

void Mudokon::Motion_55_Duck()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (mNextMotion == eMudMotions::Motion_23_CrouchIdle)
        {
            mNextMotion = -1;
            mCurrentMotion = eMudMotions::Motion_56_DuckToCrouch;
        }
    }
}

void Mudokon::Motion_56_DuckToCrouch()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eMudMotions::Motion_23_CrouchIdle;
    }
}

void Mudokon::Motion_57_Struggle()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (mNextMotion == eMudMotions::Motion_0_Idle)
        {
            mCurrentMotion = eMudMotions::Motion_0_Idle;
        }
    }
}

void Mudokon::Motion_58_StruggleToCrouchChant()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eMudMotions::Motion_59_CrouchChant;
    }
}

void Mudokon::Motion_59_CrouchChant()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (mNextMotion != -1)
        {
            mNextMotion = -1;
            mCurrentMotion = eMudMotions::Motion_60_CrouchChantToStruggle;
        }
    }

    if ((sGnFrame % 8) == 0)
    {
        const FP rndX = FP_FromInteger(30 * Math_NextRandom() / 256 - 10);
        const FP rndY = FP_FromInteger(20 * Math_NextRandom() / 256 + 10);

        New_Chant_Particle_4198E0(
            mBaseAnimatedWithPhysicsGameObject_XPos + (mBaseAnimatedWithPhysicsGameObject_SpriteScale * rndX),
            mBaseAnimatedWithPhysicsGameObject_YPos - (mBaseAnimatedWithPhysicsGameObject_SpriteScale * rndY),
            mBaseAnimatedWithPhysicsGameObject_SpriteScale,
            Layer::eLayer_0);
    }

    if (!SND_SsIsEos_DeInlined_477930(SeqId::eMudokonChant_12))
    {
        if (mBaseAnimatedWithPhysicsGameObject_SpriteScale == FP_FromDouble(0.5))
        {
            SND_SEQ_Play_477760(SeqId::eMudokonChant_12, 1, 30, 30);
        }
        else
        {
            SND_SEQ_Play_477760(SeqId::eMudokonChant_12, 1, 50, 50);
        }
    }
}

void Mudokon::Motion_60_CrouchChantToStruggle()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eMudMotions::Motion_0_Idle;
    }
}

void Mudokon::Motion_61_DuckKnockback()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 1)
    {
        Environment_SFX_42A220(EnvironmentSfx::eGenericMovement_9, 0, 0x7FFF, this);
    }

    CheckFloorGone();

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eMudMotions::Motion_55_Duck;
        mNextMotion = -1;
    }
}

void Mudokon::Motion_62_PoisonGasDeath()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_144_flags.Clear(Flags_144::e144_Bit6_bPersist);
        mHealth = FP_FromInteger(0);
        field_1B8_brain_state = 11;
        field_1C0_timer = sGnFrame + 90;
        mNextMotion = -1;
    }
}

s16 Mudokon::Brain_0_ComingIn()
{
    if (field_1BA_brain_sub_state == 0)
    {
        field_1C0_timer = sGnFrame + 120;
        return 1;
    }

    else if (field_1BA_brain_sub_state == 1)
    {
        if (static_cast<s32>(sGnFrame) > field_1C0_timer)
        {
            mNextMotion = eMudMotions::Motion_1_WalkLoop;
            field_190 = field_18C_how_far_to_walk;
            return 2;
        }
    }
    else if (field_1BA_brain_sub_state == 2)
    {
        if (mBaseAnimatedWithPhysicsGameObject_VelX < FP_FromInteger(0))
        {
            field_190 += mBaseAnimatedWithPhysicsGameObject_VelX;
        }
        else
        {
            field_190 -= mBaseAnimatedWithPhysicsGameObject_VelX;
        }

        if (field_190 < FP_FromInteger(0))
        {
            mCurrentMotion = eMudMotions::Motion_0_Idle;
            field_1B8_brain_state = 2;
            return 0;
        }
    }
    return field_1BA_brain_sub_state;
}

s16 Mudokon::Brain_1_ComingOut()
{
    if (field_1BA_brain_sub_state == 0)
    {
        if (mCurrentMotion == eMudMotions::Motion_0_Idle)
        {
            mCurrentMotion = eMudMotions::Motion_2_StandingTurn;
            mNextMotion = eMudMotions::Motion_1_WalkLoop;
            return 1;
        }
    }
    else if (field_1BA_brain_sub_state == 1)
    {
        if (mCurrentMotion == eMudMotions::Motion_1_WalkLoop)
        {
            field_144_flags.Clear(Flags_144::e144_Bit2);
            field_190 = field_18C_how_far_to_walk;
            return 2;
        }
    }
    else if (field_1BA_brain_sub_state == 2)
    {
        if (mBaseAnimatedWithPhysicsGameObject_VelX < FP_FromInteger(0))
        {
            field_190 += mBaseAnimatedWithPhysicsGameObject_VelX;
        }
        else
        {
            field_190 -= mBaseAnimatedWithPhysicsGameObject_VelX;
        }

        if (field_190 < FP_FromInteger(0))
        {
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            mCurrentMotion = eMudMotions::Motion_0_Idle;
            return 3;
        }
    }
    return field_1BA_brain_sub_state;
}

s16 Mudokon::Brain_2_SingSequenceIdle()
{
    switch (field_1BA_brain_sub_state)
    {
        case 0:
            if (field_144_flags.Get(Flags_144::e144_Bit10_give_ring_without_password))
            {
                field_1B8_brain_state = field_188;
                return 0;
            }
            else
            {
                field_1C0_timer = sGnFrame + 60;
                return 1;
            }
            break;

        case 1:
            if (static_cast<s32>(sGnFrame) > field_1C0_timer)
            {
                Mudokon_SFX(MudSounds::eHello_3, 0, 300, this);
                mNextMotion = eMudMotions::Motion_4_Speak;
                return 2;
            }
            break;

        case 2:
            if (!field_198_abe_must_face_mud
                || sActiveHero_507678->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX) != mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
            {
                GameSpeakEvents last_speak = {};
                if (field_128 == pEventSystem_4FF954->field_18_last_event_index)
                {
                    if (pEventSystem_4FF954->field_10_last_event == GameSpeakEvents::eNone_m1)
                    {
                        last_speak = GameSpeakEvents::eNone_m1;
                    }
                    else
                    {
                        last_speak = GameSpeakEvents::eSameAsLast_m2;
                    }
                }
                else
                {
                    last_speak = pEventSystem_4FF954->field_10_last_event;
                    field_128 = pEventSystem_4FF954->field_18_last_event_index;
                }

                if (last_speak == GameSpeakEvents::eHello_9)
                {
                    field_1C0_timer = sGnFrame + 20;
                    return 3;
                }
            }
            break;

        case 3:
            if (field_1C0_timer <= static_cast<s32>(sGnFrame))
            {
                if (field_186_give_password == Choice_short::eYes_1)
                {
                    field_1B8_brain_state = 4;
                }
                else
                {
                    field_1B8_brain_state = 3;
                }
                return 0;
            }
            break;

        default:
            return field_1BA_brain_sub_state;
    }
    return field_1BA_brain_sub_state;
}

s16 Mudokon::Brain_3_SingSequenceSing()
{
    switch (field_1BA_brain_sub_state)
    {
        case 0:
            field_19E_code_idx = 0;
            field_13C = GameSpeak::sub_40FA60(field_1A4_code_converted, field_12C);
            return 1;

        case 1:
            switch (Code_LookUp(field_1A4_code_converted, field_19E_code_idx, field_1A8_code_length))
            {
                case GameSpeakEvents::eWhistleHigh_1:
                    mNextMotion = eMudMotions::Motion_4_Speak;
                    Mudokon_SFX(MudSounds::eWhistleHigh_1, 0, 0, this);
                    break;

                case GameSpeakEvents::eWhistleLow_2:
                    mNextMotion = eMudMotions::Motion_5_Speak;
                    Mudokon_SFX(MudSounds::eWhistleLow_2, 0, 0, this);
                    break;

                case GameSpeakEvents::eFart_3:
                    mNextMotion = eMudMotions::Motion_6_Speak;
                    Mudokon_SFX(MudSounds::eFart_7, 0, 300, this);
                    break;

                case GameSpeakEvents::eLaugh_4:
                    mNextMotion = eMudMotions::Motion_3_Speak;
                    Mudokon_SFX(MudSounds::eLaugh1_8, 0, 300, this);
                    break;

                default:
                    mNextMotion = eMudMotions::Motion_3_Speak;
                    break;
            }

            field_19E_code_idx++;

            if (field_19E_code_idx >= field_1A8_code_length)
            {
                return 3;
            }
            else
            {
                field_1C0_timer = sGnFrame + 30;
                return 2;
            }
            break;

        case 2:
        {
            GameSpeakEvents last_speak = {};
            if (field_128 == pEventSystem_4FF954->field_18_last_event_index)
            {
                if (pEventSystem_4FF954->field_10_last_event == GameSpeakEvents::eNone_m1)
                {
                    last_speak = GameSpeakEvents::eNone_m1;
                }
                else
                {
                    last_speak = GameSpeakEvents::eSameAsLast_m2;
                }
            }
            else
            {
                last_speak = pEventSystem_4FF954->field_10_last_event;
                field_128 = pEventSystem_4FF954->field_18_last_event_index;
            }

            if (last_speak == GameSpeakEvents::eNone_m1 || last_speak == GameSpeakEvents::eSameAsLast_m2)
            {
                if (static_cast<s32>(sGnFrame) > field_1C0_timer)
                {
                    return 1;
                }
            }
            else
            {
                field_1A0 = 0;
                field_1C0_timer = sGnFrame + 30;
                return 5;
            }
            break;
        }

        case 3:
        {
            GameSpeakEvents last_speak = {};
            if (field_128 == pEventSystem_4FF954->field_18_last_event_index)
            {
                if (pEventSystem_4FF954->field_10_last_event == GameSpeakEvents::eNone_m1)
                {
                    last_speak = GameSpeakEvents::eNone_m1;
                }
                else
                {
                    last_speak = GameSpeakEvents::eSameAsLast_m2;
                }
            }
            else
            {
                field_128 = pEventSystem_4FF954->field_18_last_event_index;
                last_speak = pEventSystem_4FF954->field_10_last_event;
            }

            if (last_speak != GameSpeakEvents::eNone_m1 && last_speak != GameSpeakEvents::eSameAsLast_m2)
            {
                field_13E = static_cast<s16>(pEventSystem_4FF954->field_18_last_event_index);
                field_118 = sGnFrame + 60;
                return 4;
            }
            break;
        }

        case 4:
        {
            GameSpeakEvents last_speak = {};
            if (field_128 == pEventSystem_4FF954->field_18_last_event_index)
            {
                if (pEventSystem_4FF954->field_10_last_event == GameSpeakEvents::eNone_m1)
                {
                    last_speak = GameSpeakEvents::eNone_m1;
                }
                else
                {
                    last_speak = GameSpeakEvents::eSameAsLast_m2;
                }
            }
            else
            {
                field_128 = pEventSystem_4FF954->field_18_last_event_index;
                last_speak = pEventSystem_4FF954->field_10_last_event;
            }


            if (last_speak != GameSpeakEvents::eNone_m1 && last_speak != GameSpeakEvents::eSameAsLast_m2)
            {
                field_118 = sGnFrame + 60;
            }

            if (static_cast<s32>(sGnFrame) <= field_118)
            {
                if (pEventSystem_4FF954->MatchBuffer(field_12C, field_13C, field_13E) != GameSpeakMatch::eFullMatch_1)
                {
                    if (pEventSystem_4FF954->MatchBuffer(
                            field_12C,
                            field_13C,
                            field_13E)
                        != GameSpeakMatch::eNoMatch_0) // Full or partial
                    {
                        return field_1BA_brain_sub_state;
                    }
                }
            }

            field_1A0 = pEventSystem_4FF954->MatchBuffer(field_12C, field_13C, field_13E) == GameSpeakMatch::eFullMatch_1 || sVoiceCheat_507708;
            field_1C0_timer = sGnFrame + 30;
            return 5;
        }

        case 5:
            if (static_cast<s32>(sGnFrame) > field_1C0_timer)
            {
                if (field_1A0)
                {
                    Mudokon_SFX(MudSounds::eOkay_13, 0, 300, this);
                    mNextMotion = 3;
                }
                else
                {
                    Mudokon_SFX(MudSounds::eRefuse_14, 0, 300, this);
                    mNextMotion = 6;
                }
                return 6;
            }
            break;

        case 6:
            if (mCurrentMotion == eMudMotions::Motion_0_Idle)
            {
                if (field_1A0)
                {
                    field_1B8_brain_state = field_188;
                }
                else
                {
                    field_1B8_brain_state = 2;
                }
                return 0;
            }
            break;

        case 8:
            if (static_cast<s32>(sGnFrame) > field_1C0_timer)
            {
                return 0;
            }
            break;

        default:
            return field_1BA_brain_sub_state;
    }
    return field_1BA_brain_sub_state;
}

s16 Mudokon::Brain_4_SingSequencePassword()
{
    switch (field_1BA_brain_sub_state)
    {
        case 0:
            mNextMotion = eMudMotions::Motion_3_Speak;
            Mudokon_SFX(MudSounds::ePassword_9, 0, 300, this);
            field_13C = GameSpeak::sub_40FA60(field_1A4_code_converted, field_12C);
            return 1;

        case 1:
            field_1C0_timer = sGnFrame;
            return 2;

        case 2:
        {
            GameSpeakEvents last_speak = {};
            if (field_128 == pEventSystem_4FF954->field_18_last_event_index)
            {
                if (pEventSystem_4FF954->field_10_last_event == GameSpeakEvents::eNone_m1)
                {
                    last_speak = GameSpeakEvents::eNone_m1;
                }
                else
                {
                    last_speak = GameSpeakEvents::eSameAsLast_m2;
                }
            }
            else
            {
                field_128 = pEventSystem_4FF954->field_18_last_event_index;
                last_speak = pEventSystem_4FF954->field_10_last_event;
            }

            if (last_speak != GameSpeakEvents::eNone_m1 && last_speak != GameSpeakEvents::eSameAsLast_m2)
            {
                field_13E = static_cast<s16>(pEventSystem_4FF954->field_18_last_event_index);
                field_118 = sGnFrame + 60;
                return 3;
            }
            break;
        }

        case 3:
        {
            GameSpeakEvents last_speak = {};
            if (field_128 == pEventSystem_4FF954->field_18_last_event_index)
            {
                if (pEventSystem_4FF954->field_10_last_event == GameSpeakEvents::eNone_m1)
                {
                    last_speak = GameSpeakEvents::eNone_m1;
                }
                else
                {
                    last_speak = GameSpeakEvents::eSameAsLast_m2;
                }
            }
            else
            {
                field_128 = pEventSystem_4FF954->field_18_last_event_index;
                last_speak = pEventSystem_4FF954->field_10_last_event;
            }

            if (last_speak != GameSpeakEvents::eNone_m1 && last_speak != GameSpeakEvents::eSameAsLast_m2)
            {
                field_118 = sGnFrame + 60;
            }

            if (static_cast<s32>(sGnFrame) <= field_118)
            {
                if (pEventSystem_4FF954->MatchBuffer(field_12C, field_13C, field_13E) != GameSpeakMatch::eFullMatch_1)
                {
                    if (pEventSystem_4FF954->MatchBuffer(
                            field_12C,
                            field_13C,
                            field_13E)
                        != GameSpeakMatch::eNoMatch_0) // Full or partial
                    {
                        return field_1BA_brain_sub_state;
                    }
                }
            }

            field_1A0 = pEventSystem_4FF954->MatchBuffer(
                            field_12C,
                            field_13C,
                            field_13E)
                         == GameSpeakMatch::eFullMatch_1
                     || sVoiceCheat_507708;

            field_1C0_timer = sGnFrame + 30;
            return 4;
        }

        case 4:
            if (static_cast<s32>(sGnFrame) > field_1C0_timer)
            {
                if (field_1A0)
                {
                    Mudokon_SFX(MudSounds::eOkay_13, 0, 300, this);
                    mNextMotion = eMudMotions::Motion_3_Speak;
                }
                else
                {
                    if (Math_NextRandom() >= 128u)
                    {
                        Mudokon_SFX(MudSounds::eLaugh1_8, 0, 300, this);
                    }
                    else
                    {
                        Mudokon_SFX(MudSounds::eRefuse_14, 0, 300, this);
                    }
                    mNextMotion = eMudMotions::Motion_6_Speak;
                }
                return 5;
            }
            break;

        case 5:
            if (mCurrentMotion == eMudMotions::Motion_0_Idle)
            {
                if (field_1A0)
                {
                    field_1B8_brain_state = field_188;
                }
                else
                {
                    field_1B8_brain_state = 2;
                }
                return 0;
            }
            break;

        case 6:
            if (static_cast<s32>(sGnFrame) > field_1C0_timer)
            {
                return 0;
            }
            break;

        default:
            return field_1BA_brain_sub_state;
    }

    return field_1BA_brain_sub_state;
}

s16 Mudokon::Brain_5_LiftUse()
{
    if (field_1BA_brain_sub_state)
    {
        if (field_1BA_brain_sub_state == 1 && !mCurrentMotion)
        {
            if (field_184)
            {
                field_1B8_brain_state = 1;
                return 0;
            }
            else
            {
                return 2;
            }
        }
    }
    else if (mCurrentMotion == eMudMotions::Motion_0_Idle)
    {
        mCurrentMotion = eMudMotions::Motion_13_LiftGrabBegin;
        mNextMotion = eMudMotions::Motion_12_LiftUse;
        field_194_pLiftPoint = nullptr;
        for (s32 i = 0; i < gBaseAliveGameObjects_4FC8A0->Size(); i++)
        {
            BaseAliveGameObject* pObj = gBaseAliveGameObjects_4FC8A0->ItemAt(i);
            if (!pObj)
            {
                break;
            }

            if (pObj->mBaseGameObjectTypeId == ReliveTypes::eLiftPoint)
            {
                auto pLiftPoint = static_cast<LiftPoint*>(pObj);
                if (field_110_lift_switch_id == pLiftPoint->field_278_lift_point_id)
                {
                    field_194_pLiftPoint = pLiftPoint;
                    pObj->mBaseGameObjectRefCount++;
                    break;
                }
            }
        }
        return 1;
    }
    return field_1BA_brain_sub_state;
}

s16 Mudokon::Brain_6_LeverUse()
{
    if (field_1BA_brain_sub_state || mCurrentMotion)
    {
        return field_1BA_brain_sub_state;
    }

    mNextMotion = eMudMotions::Motion_15_LeverUse;

    FP directedGridSize = {};
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        directedGridSize = -ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
    }
    else
    {
        directedGridSize = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
    }

    auto pSwitch = static_cast<Lever*>(FindObjectOfType_418280(ReliveTypes::eLever, mBaseAnimatedWithPhysicsGameObject_XPos + directedGridSize, mBaseAnimatedWithPhysicsGameObject_YPos - ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale)));
    if (pSwitch)
    {
        pSwitch->VPull(mBaseAnimatedWithPhysicsGameObject_XPos < pSwitch->mBaseAnimatedWithPhysicsGameObject_XPos);
    }

    return 1;
}

s16 Mudokon::Brain_7_GiveRings()
{
    switch (field_1BA_brain_sub_state)
    {
        case 0:
            if (sActiveHero_507678->field_168_ring_pulse_timer <= 0)
            {
                mNextMotion = eMudMotions::Motion_52_Chant;
                field_1C0_timer = sGnFrame + 30;
                return 1;
            }
            return 4;

        case 1:
            if (static_cast<s32>(sGnFrame) > field_1C0_timer)
            {
                relive_new PossessionFlicker(this, 10, 255, 128, 128);
                field_1C0_timer = sGnFrame + 15;
                return 2;
            }
            break;

        case 2:
            if (static_cast<s32>(sGnFrame) > field_1C0_timer)
            {
                const PSX_RECT ourRect = VGetBoundingRect();

                    relive_new AbilityRing(
                        FP_FromInteger((ourRect.w + ourRect.x) / 2),
                        FP_FromInteger((ourRect.h + ourRect.y) / 2),
                        RingTypes::eExplosive_Emit_Effect_2);

                const PSX_RECT heroRect = sActiveHero_507678->VGetBoundingRect();

                auto pAbeRing = relive_new AbilityRing(
                    FP_FromInteger((heroRect.w + heroRect.x) / 2),
                    FP_FromInteger((heroRect.h + heroRect.y) / 2),
                    RingTypes::eExplosive_Give_3);
                if (pAbeRing)
                {
                    pAbeRing->SetTarget(sActiveHero_507678);
                }
                field_1C0_timer = sGnFrame + 30;
                return 3;
            }
            break;

        case 3:
            if (static_cast<s32>(sGnFrame) > field_1C0_timer)
            {
                if (field_1AA_ring_timeout)
                {
                    sActiveHero_507678->field_168_ring_pulse_timer = field_1AA_ring_timeout + sGnFrame;
                }
                else
                {
                    sActiveHero_507678->field_168_ring_pulse_timer = sGnFrame + 200000;
                }

                mNextMotion = eMudMotions::Motion_0_Idle;
                sActiveHero_507678->field_16C_bHaveShrykull = 0;
                return 4;
            }
            break;

        case 4:
            if (sActiveHero_507678->field_168_ring_pulse_timer <= 0)
            {
                field_1B8_brain_state = 2;
                return 0;
            }
            break;

        default:
            return field_1BA_brain_sub_state;
    }
    return field_1BA_brain_sub_state;
}

s16 Mudokon::Brain_8_StandScrub()
{
    if (FindBirdPortal())
    {
        AddAlerted();
        field_1B8_brain_state = 12;
        return 0;
    }

    const GameSpeakEvents last_speak = LastGameSpeak();
    if (last_speak == GameSpeakEvents::Slig_LookOut_6)
    {
        if (field_1BA_brain_sub_state != 4 && field_1BA_brain_sub_state != 5)
        {
            field_1BA_brain_sub_state = 4;
        }
    }

    switch (field_1BA_brain_sub_state)
    {
        case 0:
            mCurrentMotion = eMudMotions::Motion_16_StandScrubLoop;
            mNextMotion = -1;
            field_1C0_timer = (Math_NextRandom() % 64) + sGnFrame + 35;
            return 1;

        case 1:
            if (last_speak == GameSpeakEvents::eHello_9 && sAlertedMudCount_507B90 == 0 && IAmNearestToAbe())
            {
                AddAlerted();
                mNextMotion = eMudMotions::Motion_21_StandScrubToIdle;
                field_1C0_timer = sGnFrame + 15;
                return 3;
            }

            if (static_cast<s32>(sGnFrame) > field_1C0_timer
                && mCurrentMotion == eMudMotions::Motion_16_StandScrubLoop)
            {
                mNextMotion = eMudMotions::Motion_17_StandScrubLoopToPause;
            }

            if (mCurrentMotion == eMudMotions::Motion_19_StandScrubPause)
            {
                field_1C0_timer = (Math_NextRandom() % 64) + sGnFrame + 35;
                return 2;
            }
            return field_1BA_brain_sub_state;

        case 2:
            if (last_speak == GameSpeakEvents::eHello_9 && sAlertedMudCount_507B90 == 0 && IAmNearestToAbe())
            {
                AddAlerted();
                mNextMotion = eMudMotions::Motion_21_StandScrubToIdle;
                field_1C0_timer = sGnFrame + 15;
                return 3;
            }

            if (static_cast<s32>(sGnFrame) > field_1C0_timer)
            {
                mNextMotion = eMudMotions::Motion_18_StandScrubPauseToLoop;
                field_1C0_timer = (Math_NextRandom() % 64) + sGnFrame + 35;
                return 1;
            }
            return field_1BA_brain_sub_state;

        case 3:
            if (static_cast<s32>(sGnFrame) <= field_1C0_timer || mCurrentMotion != eMudMotions::Motion_0_Idle)
            {
                if (mNextMotion != -1 || mCurrentMotion != eMudMotions::Motion_16_StandScrubLoop)
                {
                    return field_1BA_brain_sub_state;
                }
                mNextMotion = eMudMotions::Motion_21_StandScrubToIdle;
                return field_1BA_brain_sub_state;
            }
            field_1B8_brain_state = 10;
            return 0;

        case 4:
            switch (mCurrentMotion)
            {
                case eMudMotions::Motion_0_Idle:
                    mNextMotion = eMudMotions::Motion_23_CrouchIdle;
                    return field_1BA_brain_sub_state;

                case eMudMotions::Motion_16_StandScrubLoop:
                case eMudMotions::Motion_19_StandScrubPause:
                    mNextMotion = eMudMotions::Motion_0_Idle;
                    return field_1BA_brain_sub_state;

                case eMudMotions::Motion_23_CrouchIdle:
                    mNextMotion = eMudMotions::Motion_55_Duck;
                    field_1C0_timer = sGnFrame + 60;
                    return 5;

                default:
                    return field_1BA_brain_sub_state;
            }
            break;

        case 5:
            if (static_cast<s32>(sGnFrame) > field_1C0_timer)
            {
                mNextMotion = eMudMotions::Motion_23_CrouchIdle;
                return 6;
            }
            return field_1BA_brain_sub_state;

        case 6:
            if (mCurrentMotion == eMudMotions::Motion_0_Idle)
            {
                mNextMotion = eMudMotions::Motion_16_StandScrubLoop;
            }
            else
            {
                if (mCurrentMotion == eMudMotions::Motion_16_StandScrubLoop)
                {
                    return 0;
                }

                if (mCurrentMotion == eMudMotions::Motion_23_CrouchIdle)
                {
                    mNextMotion = eMudMotions::Motion_0_Idle;
                }
            }
            return field_1BA_brain_sub_state;

        default:
            return field_1BA_brain_sub_state;
    }
}

s16 Mudokon::Brain_9_CrouchScrub()
{
    if (FindBirdPortal())
    {
        AddAlerted();
        field_1B8_brain_state = 12;
        return 0;
    }

    const GameSpeakEvents last_speak = LastGameSpeak();
    if (last_speak == GameSpeakEvents::Slig_LookOut_6)
    {
        mNextMotion = eMudMotions::Motion_55_Duck;
        field_1C0_timer = sGnFrame + 60;
        return 5;
    }

    switch (field_1BA_brain_sub_state)
    {
        case 0:
            mCurrentMotion = eMudMotions::Motion_22_CrouchScrub;
            mNextMotion = -1;
            field_1C0_timer = (Math_NextRandom() % 64) + 15;
            field_114 = (Math_NextRandom() % 64) + 240;
            return 2;

        case 1:
            if (last_speak == GameSpeakEvents::eHello_9 && sAlertedMudCount_507B90 == 0 && IAmNearestToAbe())
            {
                AddAlerted();
                field_1C0_timer = sGnFrame + 15;
                return 3;
            }

            if (mCurrentMotion == eMudMotions::Motion_23_CrouchIdle && static_cast<s32>(sGnFrame) > field_114)
            {
                mNextMotion = eMudMotions::Motion_24_CrouchTurn;
                field_114 = (Math_NextRandom() % 64) + sGnFrame + 240;
            }

            if (mCurrentMotion == eMudMotions::Motion_23_CrouchIdle && static_cast<s32>(sGnFrame) > field_1C0_timer)
            {
                mNextMotion = eMudMotions::Motion_22_CrouchScrub;
                field_1C0_timer = (Math_NextRandom() % 64) + sGnFrame + 35;
            }

            if (mCurrentMotion != eMudMotions::Motion_22_CrouchScrub)
            {
                return field_1BA_brain_sub_state;
            }
            return 2;

        case 2:
            if (last_speak != GameSpeakEvents::eHello_9 || sAlertedMudCount_507B90 || !IAmNearestToAbe())
            {
                if (mCurrentMotion == eMudMotions::Motion_23_CrouchIdle)
                {
                    if (static_cast<s32>(sGnFrame) > field_1C0_timer)
                    {
                        field_1C0_timer = (Math_NextRandom() % 64) + sGnFrame + 15;
                        return 1;
                    }
                    mCurrentMotion = eMudMotions::Motion_22_CrouchScrub;
                }

                if (IsEventInRange(
                        kEventSpeaking,
                        mBaseAnimatedWithPhysicsGameObject_XPos,
                        mBaseAnimatedWithPhysicsGameObject_YPos,
                        mBaseAnimatedWithPhysicsGameObject_SpriteScale))
                {
                    field_1C0_timer = (Math_NextRandom() % 64) + sGnFrame + 15;
                    return 4;
                }
            }
            else
            {
                AddAlerted();
                field_1C0_timer = sGnFrame + 15;
                return 3;
            }
            break;

        case 3:
            if (static_cast<s32>(sGnFrame) > field_1C0_timer)
            {
                if (mCurrentMotion == eMudMotions::Motion_23_CrouchIdle)
                {
                    mNextMotion = eMudMotions::Motion_26_CrouchToStand;
                }

                if (mCurrentMotion == eMudMotions::Motion_0_Idle)
                {
                    field_1BA_brain_sub_state = 0;
                    field_1B8_brain_state = 10;
                    return 0;
                }

                return field_1BA_brain_sub_state;
            }
            break;

        case 4:
            if (last_speak != GameSpeakEvents::eHello_9 || sAlertedMudCount_507B90 || !IAmNearestToAbe())
            {
                if (static_cast<s32>(sGnFrame) > field_1C0_timer)
                {
                    return 1;
                }
            }
            else
            {
                AddAlerted();
                field_1C0_timer = sGnFrame + 15;
                return 3;
            }
            break;

        case 5:
            if (static_cast<s32>(sGnFrame) > field_1C0_timer)
            {
                mNextMotion = eMudMotions::Motion_23_CrouchIdle;

                field_1C0_timer = (Math_NextRandom() % 64) + sGnFrame + 15;
                field_114 = (Math_NextRandom() % 64) + sGnFrame + 240;

                if (mHealth > FP_FromInteger(0))
                {
                    mHealth = FP_FromInteger(1);
                }
                return 2;
            }
            break;

        case 6:
            if (static_cast<s32>(sGnFrame) > field_1C0_timer)
            {
                mNextMotion = eMudMotions::Motion_61_DuckKnockback;
                field_1C0_timer = sGnFrame + 60;
                return 5;
            }
            break;

        default:
            break;
    }

    return field_1BA_brain_sub_state;
}

s16 Mudokon::Brain_10_ListeningToAbe()
{
    if (FindBirdPortal())
    {
        field_1B8_brain_state = 12;
        mNextMotion = eMudMotions::Motion_0_Idle;
        return 0;
    }

    switch (field_1BA_brain_sub_state)
    {
        case 0:
            field_144_flags.Set(Flags_144::e144_Bit6_bPersist);
            mNextMotion = -1;
            mCurrentMotion = eMudMotions::Motion_0_Idle;
            field_1B0 = -1;
            return 1;

        case 1:
            if (mCurrentMotion != eMudMotions::Motion_0_Idle)
            {
                return field_1BA_brain_sub_state;
            }

            if (!VIsFacingMe(sActiveHero_507678))
            {
                mNextMotion = eMudMotions::Motion_2_StandingTurn;
                return field_1BA_brain_sub_state;
            }
            Mudokon_SFX(MudSounds::eHello_3, 0, field_124_voice_pitch, this);
            mNextMotion = eMudMotions::Motion_4_Speak;
            return 6;

        case 2:
            if (mCurrentMotion == eMudMotions::Motion_0_Idle)
            {
                mNextMotion = eMudMotions::Motion_1_WalkLoop;
                return field_1BA_brain_sub_state;
            }

            if (mCurrentMotion == eMudMotions::Motion_1_WalkLoop)
            {
                mNextMotion = eMudMotions::Motion_0_Idle;
                return 3;
            }
            return field_1BA_brain_sub_state;

        case 3:
        {
            FP gridSizeDirected = {};
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
            {
                gridSizeDirected = -ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
            }
            else
            {
                gridSizeDirected = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
            }
            const s16 bHitWall = WallHit_401930(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(50), gridSizeDirected);
            const s16 bEndOfLine = Check_IsOnEndOfLine_4021A0(mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX), 1);

            if (mCurrentMotion != eMudMotions::Motion_0_Idle)
            {
                const s16 oldState = field_1BA_brain_sub_state;
                mNextMotion = eMudMotions::Motion_0_Idle;
                field_1B4_idle_time = 0;
                return oldState;
            }

            field_1B4_idle_time++;

            if (field_1B4_idle_time <= 150 || BetweenCameras_418500() != BetweenCamPos::None_0)
            {
                if (!VIsFacingMe(sActiveHero_507678))
                {
                    mNextMotion = eMudMotions::Motion_2_StandingTurn;
                    return field_1BA_brain_sub_state;
                }

                if (VIsObjNearby(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(2), sActiveHero_507678)
                    || bHitWall
                    || bEndOfLine)
                {
                    GameSpeakEvents last_speak = {};
                    if (bHitWall || bEndOfLine)
                    {
                        last_speak = LastGameSpeak();
                    }
                    else
                    {
                        if (field_128 == pEventSystem_4FF954->field_18_last_event_index)
                        {
                            if (pEventSystem_4FF954->field_10_last_event == GameSpeakEvents::eNone_m1)
                            {
                                last_speak = GameSpeakEvents::eNone_m1;
                            }
                            else
                            {
                                last_speak = GameSpeakEvents::eSameAsLast_m2;
                            }
                        }
                        else
                        {
                            last_speak = pEventSystem_4FF954->field_10_last_event;
                            field_128 = pEventSystem_4FF954->field_18_last_event_index;
                        }
                    }

                    if (EventGet(kEventMudokonComfort)
                        && gMap.Is_Point_In_Current_Camera(
                            mBaseAnimatedWithPhysicsGameObject_LvlNumber,
                            mBaseAnimatedWithPhysicsGameObject_PathNumber,
                            mBaseAnimatedWithPhysicsGameObject_XPos,
                            mBaseAnimatedWithPhysicsGameObject_YPos,
                            1))
                    {
                        field_1B0 = field_1BA_brain_sub_state;
                        field_1C0_timer = sGnFrame + Math_RandomRange(22, 30);
                        return 13;
                    }

                    if (last_speak == GameSpeakEvents::eNone_m1)
                    {
                        return field_1BA_brain_sub_state;
                    }

                    field_1B0 = field_1BA_brain_sub_state;
                    field_1C0_timer = sGnFrame + 20;

                    switch (last_speak)
                    {
                        case GameSpeakEvents::eWhistleHigh_1:
                            return 17;

                        case GameSpeakEvents::eWhistleLow_2:
                            return 18;

                        case GameSpeakEvents::eFart_3:
                            field_1C0_timer = sGnFrame + 30;
                            return 13;

                        case GameSpeakEvents::eLaugh_4:
                            return 16;

                        case GameSpeakEvents::eHello_9:
                            return 11;

                        case GameSpeakEvents::eFollowMe_10:
                            if (bHitWall)
                            {
                                return 9;
                            }
                            field_1B0 = 2;
                            return 10;

                        case GameSpeakEvents::eAnger_11:
                            return 14;

                        case GameSpeakEvents::eWait_12:
                            if (BetweenCameras_418500() == BetweenCamPos::None_0)
                            {
                                field_1B0 = 6;
                                return 10;
                            }
                            return 9;

                        default:
                            return field_1BA_brain_sub_state;
                    }
                }
                else
                {
                    if (CanAbeSneak(this))
                    {
                        mNextMotion = eMudMotions::Motion_35_SneakLoop;
                    }
                    else
                    {
                        mNextMotion = eMudMotions::Motion_1_WalkLoop;
                    }
                    return 4;
                }
            }
            else
            {
                if (!field_148_res_array.res[12])
                {
                    mNextMotion = eMudMotions::Motion_23_CrouchIdle;
                }
                else
                {
                    mNextMotion = eMudMotions::Motion_59_CrouchChant;
                }
                return 19;
            }
            break;
        }

        case 4:
        {
            if (mCurrentMotion == eMudMotions::Motion_0_Idle)
            {
                return 3;
            }

            if (mCurrentMotion != eMudMotions::Motion_1_WalkLoop && mCurrentMotion != eMudMotions::Motion_35_SneakLoop)
            {
                return field_1BA_brain_sub_state;
            }

            FP gridSizeDirected = {};
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
            {
                gridSizeDirected = -ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
            }
            else
            {
                gridSizeDirected = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
            }

            const s16 bHitWall = WallHit_401930(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(50), gridSizeDirected);

            if (Check_IsOnEndOfLine_4021A0(mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX), 1) || bHitWall)
            {
                mNextMotion = eMudMotions::Motion_0_Idle;
                return 3;
            }

            if (!VIsFacingMe(sActiveHero_507678))
            {
                mNextMotion = eMudMotions::Motion_0_Idle;
                return 3;
            }

            if (VIsObjNearby(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(2), sActiveHero_507678))
            {
                mNextMotion = eMudMotions::Motion_0_Idle;
                return 3;
            }

            GameSpeakEvents last_speak = {};
            if (field_128 == pEventSystem_4FF954->field_18_last_event_index)
            {
                if (pEventSystem_4FF954->field_10_last_event == GameSpeakEvents::eNone_m1)
                {
                    last_speak = GameSpeakEvents::eNone_m1;
                }
                else
                {
                    last_speak = GameSpeakEvents::eSameAsLast_m2;
                }
            }
            else
            {
                last_speak = pEventSystem_4FF954->field_10_last_event;
                field_128 = pEventSystem_4FF954->field_18_last_event_index;
            }

            if (last_speak == GameSpeakEvents::eWait_12
                && BetweenCameras_418500() == BetweenCamPos::None_0)
            {
                mNextMotion = eMudMotions::Motion_0_Idle;
                return 8;
            }

            if (sActiveHero_507678->mCurrentMotion == eAbeMotions::Motion_35_RunLoop_425060
                || sActiveHero_507678->mCurrentMotion == eAbeMotions::Motion_25_RollLoop_427BB0)
            {
                mNextMotion = eMudMotions::Motion_29_RunLoop;
                return 5;
            }

            if (sActiveHero_507678->mCurrentMotion == eAbeMotions::Motion_42_SneakLoop_424BB0
                && mCurrentMotion == eMudMotions::Motion_1_WalkLoop)
            {
                mNextMotion = eMudMotions::Motion_35_SneakLoop;
            }

            if (sActiveHero_507678->mCurrentMotion == eAbeMotions::Motion_1_WalkLoop_423F90 && mCurrentMotion == eMudMotions::Motion_35_SneakLoop)
            {
                mNextMotion = eMudMotions::Motion_1_WalkLoop;
            }

            return field_1BA_brain_sub_state;
        }

        case 5:
        {
            if (mCurrentMotion == eMudMotions::Motion_0_Idle)
            {
                return 3;
            }

            mNextMotion = eMudMotions::Motion_29_RunLoop;
            if (mCurrentMotion != eMudMotions::Motion_29_RunLoop)
            {
                return field_1BA_brain_sub_state;
            }

            FP gridSizeDirected = {};
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
            {
                gridSizeDirected = -ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
            }
            else
            {
                gridSizeDirected = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
            }

            const s16 bHitWall = WallHit_401930(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(50), gridSizeDirected * FP_FromInteger(3));
            if (Check_IsOnEndOfLine_4021A0(mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX) ? 1 : 0, 4)
                || bHitWall)
            {
                mNextMotion = eMudMotions::Motion_0_Idle;
                return 3;
            }

            switch (sActiveHero_507678->mCurrentMotion)
            {
                case eAbeMotions::Motion_1_WalkLoop_423F90:
                    mNextMotion = eMudMotions::Motion_1_WalkLoop;
                    return 4;

                case eAbeMotions::Motion_70_Knockback_428FB0:
                case eAbeMotions::Motion_27_RunSlideStop_425B60:
                case eAbeMotions::Motion_0_Idle_423520:
                    //[[fallthrough]];
                case eAbeMotions::Motion_19_CrouchIdle_4284C0:
                    if (sActiveHero_507678->mCurrentMotion == eAbeMotions::Motion_70_Knockback_428FB0)
                    {
                        Mudokon_SFX(MudSounds::eLaugh1_8, 0, field_124_voice_pitch, this);
                    }

                    if (VIsObjNearby(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(2), sActiveHero_507678))
                    {
                        mNextMotion = eMudMotions::Motion_0_Idle;
                        return 3;
                    }
                    else
                    {
                        mNextMotion = eMudMotions::Motion_1_WalkLoop;
                        return 4;
                    }
                    break;

                default:
                    if (!VIsFacingMe(sActiveHero_507678))
                    {
                        mNextMotion = eMudMotions::Motion_33_RunSlideTurn;
                        return 7;
                    }
                    return field_1BA_brain_sub_state;
            }
            break;
        }

        case 6:
            if (mCurrentMotion != eMudMotions::Motion_0_Idle)
            {
                field_1B4_idle_time = 0;
                return field_1BA_brain_sub_state;
            }

            field_1B4_idle_time++;

            if (field_1B4_idle_time <= 150
                && gMap.Is_Point_In_Current_Camera(
                    mBaseAnimatedWithPhysicsGameObject_LvlNumber,
                    mBaseAnimatedWithPhysicsGameObject_PathNumber,
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    mBaseAnimatedWithPhysicsGameObject_YPos,
                    1)
                && sAlertedMudCount_507B90 <= 1)
            {
                if (!VIsFacingMe(sActiveHero_507678))
                {
                    mNextMotion = eMudMotions::Motion_2_StandingTurn;
                    return field_1BA_brain_sub_state;
                }

                if (EventGet(kEventMudokonComfort)
                    && gMap.Is_Point_In_Current_Camera(
                        mBaseAnimatedWithPhysicsGameObject_LvlNumber,
                        mBaseAnimatedWithPhysicsGameObject_PathNumber,
                        mBaseAnimatedWithPhysicsGameObject_XPos,
                        mBaseAnimatedWithPhysicsGameObject_YPos,
                        1))
                {
                    field_1B0 = field_1BA_brain_sub_state;
                    field_1C0_timer = sGnFrame + Math_RandomRange(22, 30);
                    return 13;
                }
                const GameSpeakEvents speak = LastGameSpeak();
                if (speak == GameSpeakEvents::eNone_m1)
                {
                    return field_1BA_brain_sub_state;
                }

                field_1C0_timer = sGnFrame + 20;
                field_1B0 = field_1BA_brain_sub_state;

                switch (speak)
                {
                    case GameSpeakEvents::eWhistleHigh_1:
                        return 17;

                    case GameSpeakEvents::eWhistleLow_2:
                        return 18;

                    case GameSpeakEvents::eFart_3:
                        field_1C0_timer = sGnFrame + 30;
                        return 13;

                    case GameSpeakEvents::eLaugh_4:
                        return 16;

                    case GameSpeakEvents::eHello_9:
                        return 11;

                    case GameSpeakEvents::eFollowMe_10:
                        field_1B0 = 3;
                        return 10;

                    case GameSpeakEvents::eAnger_11:
                        return 14;

                    case GameSpeakEvents::eWait_12:
                        return 15;

                    default:
                        return field_1BA_brain_sub_state;
                }
            }
            else
            {
                if (field_148_res_array.res[12])
                {
                    mNextMotion = eMudMotions::Motion_59_CrouchChant;
                }
                else
                {
                    mNextMotion = eMudMotions::Motion_23_CrouchIdle;
                }
            }
            return 19;

        case 7:
            if (mCurrentMotion == eMudMotions::Motion_29_RunLoop)
            {
                return 5;
            }
            return field_1BA_brain_sub_state;

        case 8:
            if (mCurrentMotion == eMudMotions::Motion_0_Idle)
            {
                field_1C0_timer = sGnFrame + 20;
                field_1B0 = 6;
                return 10;
            }
            return field_1BA_brain_sub_state;

        case 9:
            if (static_cast<s32>(sGnFrame) > field_1C0_timer)
            {
                Mudokon_SFX(MudSounds::eRefuse_14, 0, field_124_voice_pitch, this);
                mNextMotion = eMudMotions::Motion_6_Speak;
                return 3;
            }
            return field_1BA_brain_sub_state;

        case 10:
            if (static_cast<s32>(sGnFrame) > field_1C0_timer)
            {
                Mudokon_SFX(MudSounds::eOkay_13, 0, field_124_voice_pitch, this);
                mNextMotion = eMudMotions::Motion_3_Speak;
                return field_1B0;
            }
            return field_1BA_brain_sub_state;

        case 11:
            if (static_cast<s32>(sGnFrame) > field_1C0_timer)
            {
                Mudokon_SFX(MudSounds::eHello_3, 0, field_124_voice_pitch, this);
                mNextMotion = eMudMotions::Motion_4_Speak;
                return field_1B0;
            }
            return field_1BA_brain_sub_state;

        case 12:
            if (static_cast<s32>(sGnFrame) > field_1C0_timer)
            {
                Mudokon_SFX(MudSounds::eLaugh1_8, 0, field_124_voice_pitch, this);
                mNextMotion = eMudMotions::Motion_4_Speak;
                return field_1B0;
            }
            return field_1BA_brain_sub_state;

        case 13:
            if (static_cast<s32>(sGnFrame) > field_1C0_timer)
            {
                Mudokon_SFX(MudSounds::eLaugh2_11, 0, field_124_voice_pitch, this);
                mNextMotion = eMudMotions::Motion_6_Speak;
                return field_1B0;
            }
            return field_1BA_brain_sub_state;


        case 14:
            if (static_cast<s32>(sGnFrame) > field_1C0_timer)
            {
                Mudokon_SFX(MudSounds::eDunno_15, 0, field_124_voice_pitch, this);
                mNextMotion = eMudMotions::Motion_6_Speak;
                return field_1B0;
            }
            return field_1BA_brain_sub_state;

        case 15:
            if (static_cast<s32>(sGnFrame) > field_1C0_timer)
            {
                Mudokon_SFX(MudSounds::eAngry_5, 0, field_124_voice_pitch, this);
                mNextMotion = eMudMotions::Motion_6_Speak;
                return field_1B0;
            }
            return field_1BA_brain_sub_state;

        case 16:
            if (static_cast<s32>(sGnFrame) > field_1C0_timer)
            {
                Mudokon_SFX(MudSounds::eFart_7, 0, field_124_voice_pitch, this);
                mNextMotion = eMudMotions::Motion_3_Speak;
                if (sEnableFartGasCheat_507704 == 0)
                {
                    return field_1B0;
                }

                if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
                {
                    const FP fart_scale = (FP_FromDouble(0.5) * mBaseAnimatedWithPhysicsGameObject_SpriteScale);
                    const FP fart_y = mBaseAnimatedWithPhysicsGameObject_YPos - (FP_FromInteger(24) * mBaseAnimatedWithPhysicsGameObject_SpriteScale);
                    const FP fart_x = (FP_FromInteger(12) * mBaseAnimatedWithPhysicsGameObject_SpriteScale) + mBaseAnimatedWithPhysicsGameObject_XPos;
                    New_Smoke_Particles_419A80(fart_x, fart_y, fart_scale, 3, 1);
                }
                else
                {
                    const FP fart_scale = (FP_FromDouble(0.5) * mBaseAnimatedWithPhysicsGameObject_SpriteScale);
                    const FP fart_y = mBaseAnimatedWithPhysicsGameObject_YPos - (FP_FromInteger(24) * mBaseAnimatedWithPhysicsGameObject_SpriteScale);
                    const FP fart_x = mBaseAnimatedWithPhysicsGameObject_XPos - (FP_FromInteger(12) * mBaseAnimatedWithPhysicsGameObject_SpriteScale);
                    New_Smoke_Particles_419A80(fart_x, fart_y, fart_scale, 3, 1);
                }

                return field_1B0;
            }
            break;

        case 17:
            if (static_cast<s32>(sGnFrame) > field_1C0_timer)
            {
                Mudokon_SFX(MudSounds::eWhistleHigh_1, 0, 0, this);
                mNextMotion = eMudMotions::Motion_4_Speak;
                return field_1B0;
            }
            break;

        case 18:
            if (static_cast<s32>(sGnFrame) > field_1C0_timer)
            {
                Mudokon_SFX(MudSounds::eWhistleLow_2, 0, 0, this);
                mNextMotion = eMudMotions::Motion_5_Speak;
                return field_1B0;
            }
            break;

        case 19:
            if (mCurrentMotion != eMudMotions::Motion_23_CrouchIdle && mCurrentMotion != eMudMotions::Motion_59_CrouchChant)
            {
                return field_1BA_brain_sub_state;
            }

            RemoveAlerted();

            if (field_148_res_array.res[12])
            {
                field_1B8_brain_state = 14;
            }
            else
            {
                field_1B8_brain_state = 9;
            }
            return 0;

        default:
            break;
    }
    return field_1BA_brain_sub_state;
}

s16 Mudokon::Brain_11_ShrivelDeath()
{
    if (field_1C0_timer < static_cast<s32>(sGnFrame) + 80)
    {
        mBaseAnimatedWithPhysicsGameObject_RGB.r -= 2;
        mBaseAnimatedWithPhysicsGameObject_RGB.g -= 2;
        mBaseAnimatedWithPhysicsGameObject_RGB.b -= 2;
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = mBaseAnimatedWithPhysicsGameObject_SpriteScale - FP_FromDouble(0.008);
    }

    if (static_cast<s32>(sGnFrame) < field_1C0_timer - 24)
    {
        DeathSmokeEffect(true);
    }

    if (mBaseAnimatedWithPhysicsGameObject_SpriteScale < FP_FromInteger(0))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    return 100;
}

s16 Mudokon::Brain_12_Escape()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(Options::eDead);
        return field_1BA_brain_sub_state;
    }

    BirdPortal* pPortal = field_1AC_pBirdPortal;
    if (!pPortal || pPortal->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
    {
        sMudRunningToPortalCount_507B94--;
        if (pPortal)
        {
            pPortal->mBaseGameObjectRefCount--;
        }

        field_144_flags.Set(Flags_144::e144_Bit6_bPersist);
        field_1AC_pBirdPortal = nullptr;
        mNextMotion = eMudMotions::Motion_0_Idle;
        field_1B8_brain_state = 10;
        return 6;
    }

    switch (field_1BA_brain_sub_state)
    {
        case 0:
            if (pPortal->VActivePortal())
            {
                field_1C0_timer = sGnFrame + (Math_NextRandom() % 8);
                return 1;
            }
            break;

        case 1:
            if (static_cast<s32>(sGnFrame) > field_1C0_timer)
            {
                if (FP_Abs(pPortal->field_18_xpos - mBaseAnimatedWithPhysicsGameObject_XPos) >= ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale))
                {
                    return 2;
                }
                return 4;
            }
            break;

        case 2:
            if (mCurrentMotion == eMudMotions::Motion_23_CrouchIdle)
            {
                mNextMotion = eMudMotions::Motion_26_CrouchToStand;
            }

            if (mCurrentMotion == eMudMotions::Motion_16_StandScrubLoop)
            {
                mNextMotion = eMudMotions::Motion_21_StandScrubToIdle;
            }

            if (mCurrentMotion == eMudMotions::Motion_19_StandScrubPause)
            {
                mNextMotion = eMudMotions::Motion_18_StandScrubPauseToLoop;
            }

            if (mCurrentMotion == eMudMotions::Motion_0_Idle || mCurrentMotion == eMudMotions::Motion_1_WalkLoop)
            {
                if (FacingBirdPortal(pPortal))
                {
                    mNextMotion = eMudMotions::Motion_29_RunLoop;
                }
                else
                {
                    mNextMotion = eMudMotions::Motion_2_StandingTurn;
                }
            }

            if (mCurrentMotion == eMudMotions::Motion_29_RunLoop)
            {
                if (!FacingBirdPortal(field_1AC_pBirdPortal))
                {
                    mNextMotion = eMudMotions::Motion_33_RunSlideTurn;
                }
                else
                {
                    if (IntoBirdPortal_402350(3))
                    {
                        field_1BA_brain_sub_state = 3;
                        mNextMotion = eMudMotions::Motion_44_RunJumpMid;
                    }
                }
            }
            break;

        case 4:
            if (mCurrentMotion == eMudMotions::Motion_23_CrouchIdle)
            {
                mNextMotion = eMudMotions::Motion_26_CrouchToStand;
            }

            if (mCurrentMotion == eMudMotions::Motion_16_StandScrubLoop)
            {
                mNextMotion = eMudMotions::Motion_21_StandScrubToIdle;
            }

            if (mCurrentMotion == eMudMotions::Motion_19_StandScrubPause)
            {
                mNextMotion = eMudMotions::Motion_18_StandScrubPauseToLoop;
            }

            if (mCurrentMotion == eMudMotions::Motion_0_Idle || mCurrentMotion == eMudMotions::Motion_1_WalkLoop)
            {
                if ((pPortal->field_12_side == PortalSide::eRight_0) == mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
                {
                    mNextMotion = eMudMotions::Motion_2_StandingTurn;
                }
                else
                {
                    mNextMotion = eMudMotions::Motion_29_RunLoop;
                }
            }

            if (mCurrentMotion == eMudMotions::Motion_29_RunLoop)
            {
                if (FP_Abs(pPortal->field_18_xpos - mBaseAnimatedWithPhysicsGameObject_XPos) <= ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale))
                {
                    return field_1BA_brain_sub_state;
                }
                return 2;
            }
            break;

        default:
            return field_1BA_brain_sub_state;
    }

    return field_1BA_brain_sub_state;
}

s16 Mudokon::Brain_13_FallAndSmackDeath()
{
    if (field_1BA_brain_sub_state)
    {
        if (field_1BA_brain_sub_state == 1)
        {
            if (static_cast<s32>(sGnFrame) == field_1C0_timer - 6)
            {
                SND_SEQ_Play_477760(SeqId::eHitBottomOfDeathPit_10, 1, 65, 65);
            }

            if (static_cast<s32>(sGnFrame) > field_1C0_timer)
            {
                Environment_SFX_42A220(EnvironmentSfx::eFallingDeathScreamHitGround_15, 0, 0x7FFF, this);
                relive_new ScreenShake(0);
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            }
        }
        return field_1BA_brain_sub_state;
    }
    else
    {
        Mudokon_SFX(MudSounds::eDeathDropScream_17, 0, 0, this);
        field_1C0_timer = sGnFrame + 60;
        return 1;
    }
}

s16 Mudokon::Brain_14_Chant()
{
    if (FindBirdPortal())
    {
        AddAlerted();
        field_1B8_brain_state = 12;
        return 0;
    }

    const GameSpeakEvents lastSpeak = LastGameSpeak();
    if (lastSpeak == GameSpeakEvents::Slig_LookOut_6)
    {
        mNextMotion = eMudMotions::Motion_55_Duck;
        field_1C0_timer = sGnFrame + 60;
        return 5;
    }

    switch (field_1BA_brain_sub_state)
    {
        case 0:
            mCurrentMotion = eMudMotions::Motion_59_CrouchChant;
            mNextMotion = -1;
            return 1;

        case 1:
            if (field_1B6)
            {
                mNextMotion = eMudMotions::Motion_0_Idle;
                return 3;
            }

            if (lastSpeak == GameSpeakEvents::eHello_9 && sAlertedMudCount_507B90 == 0 && IAmNearestToAbe())
            {
                AddAlerted();
                field_1C0_timer = sGnFrame + 15;
                return 2;
            }
            break;

        case 2:
            if (static_cast<s32>(sGnFrame) > field_1C0_timer && mCurrentMotion == eMudMotions::Motion_0_Idle)
            {
                field_1BA_brain_sub_state = 0;
                field_1B8_brain_state = 10;
                return field_1BA_brain_sub_state;
            }
            break;

        case 3:
            if (!field_148_res_array.res[11])
            {
                field_148_res_array.res[11] = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kAbewaspAOResID, 1, 0);
            }

            if (mCurrentMotion == eMudMotions::Motion_57_Struggle)
            {
                field_1B6 = 0;
                field_1C0_timer = sGnFrame + 32;
                return 4;
            }
            mNextMotion = eMudMotions::Motion_57_Struggle;
            break;

        case 4:
            if (static_cast<s32>(sGnFrame) > field_1C0_timer)
            {
                if (field_1B6 == 0)
                {
                    mNextMotion = eMudMotions::Motion_0_Idle;
                    return 5;
                }
                field_1B6 = 0;
                field_1C0_timer = sGnFrame + 32;
            }
            break;

        case 5:
            if (mCurrentMotion == eMudMotions::Motion_0_Idle)
            {
                mNextMotion = eMudMotions::Motion_59_CrouchChant;
                return 1;
            }
            break;

        default:
            return field_1BA_brain_sub_state;
    }
    return field_1BA_brain_sub_state;
}

s16 Mudokon::Brain_15_Choke()
{
    if (mCurrentMotion != eMudMotions::Motion_62_PoisonGasDeath)
    {
        mNextMotion = eMudMotions::Motion_62_PoisonGasDeath;
    }
    return 0;
}

} // namespace AO
