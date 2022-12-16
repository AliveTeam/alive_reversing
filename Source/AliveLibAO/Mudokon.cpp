#include "stdafx_ao.h"
#include "../relive_lib/Function.hpp"
#include "Mudokon.hpp"
#include "GameSpeak.hpp"
#include "../relive_lib/Shadow.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "../relive_lib/Collisions.hpp"
#include "Game.hpp"
#include "Abe.hpp"
#include "Midi.hpp"
#include "BirdPortal.hpp"
#include "LiftPoint.hpp"
#include "DDCheat.hpp"
#include "../relive_lib/Events.hpp"
#include "Gibs.hpp"
#include "Blood.hpp"
#include "Math.hpp"
#include "Bullet.hpp"
#include "Sfx.hpp"
#include "MusicTrigger.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "../relive_lib/Particle.hpp"
#include "CameraSwapper.hpp"
#include "Lever.hpp"
#include "ScreenShake.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "PossessionFlicker.hpp"
#include "AbilityRing.hpp"
#include "CheatController.hpp"
#include "Grid.hpp"
#include "../relive_lib/FatalError.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "Path.hpp"

namespace AO {

using TMudStateFunction = decltype(&Mudokon::Motion_0_Idle);

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

s16 sAlertedMudCount_507B90 = 0;
s16 sMudRunningToPortalCount_507B94 = 0;

Mudokon::Mudokon(relive::Path_TLV* pTlv, const Guid& tlvId)
    : BaseAliveGameObject()
{
    field_128 = -1;
    field_13E = -1;
    SetNextMotion(-1);
    field_13C = 0;
    field_1C0_timer = 0;
    field_1B8_brain_state = 0;
    field_1BA_brain_sub_state = 0;
    SetCanBeesChase(true);
    field_1BC = 0;
    field_124_voice_pitch = 0;
    field_148_res_array = {};

    LoadAnimations();
    Animation_Init(GetAnimRes(AnimId::Mudokon_Idle));

    GetAnimation().SetSemiTrans(true);

    SetCanSetOffExplosives(true);
    SetDoPurpleLightEffect(true);

    mPersist = false;
    mBit7_Unknown = false;
    mBit8_Unknown = false;
    mAlerted = false;
    mGiveRingWithoutPassword = false;

    field_1B6 = 0;
    field_198_abe_must_face_mud = 0;

    SetTint(sMudTints_4CD320, mCurrentLevel);

    relive::reliveScale scale = relive::reliveScale::eFull;
    switch (pTlv->mTlvType)
    {
        case ReliveTypes::eNone:
            ALIVE_FATAL("Mudokon ctor pTlv->mTlvType32.mType was None_m1. This shouldn't happen.");
            break;
        case ReliveTypes::eLiftMudokon:
        {
            SetType(ReliveTypes::eRingOrLiftMud);

            auto liftMudTlv = static_cast<relive::Path_LiftMudokon*>(pTlv);

            field_18C_how_far_to_walk = FP_FromInteger(liftMudTlv->mHowFarToWalk);
            field_110_lift_switch_id = liftMudTlv->mLiftSwitchId;

            mSnapToGrid = false;
            mDeaf = false;

            GetAnimation().SetFlipX(liftMudTlv->mFacing == relive::Path_LiftMudokon::Direction::eLeft);

            mGivePassword = liftMudTlv->mGivePassword;
            field_184 = 1;
            field_1B8_brain_state = 0;
            field_188 = 5;

            field_1A4_code_converted = Code_Convert(liftMudTlv->mCode1, liftMudTlv->mCode2);
            field_1A8_code_length = Code_Length(field_1A4_code_converted);

            scale = liftMudTlv->mScale;
        }
        break;

        case ReliveTypes::eRingMudokon:
        {
            SetType(ReliveTypes::eRingOrLiftMud);

            auto ringMudTlv = static_cast<relive::Path_RingMudokon*>(pTlv);

            GetAnimation().SetFlipX(ringMudTlv->mFacing == relive::reliveXDirection::eLeft); // TODO: Check
            field_184 = 0;
            mGivePassword = ringMudTlv->mGivePassword;
            field_198_abe_must_face_mud = ringMudTlv->mAbeMustFaceMud == relive::Path_RingMudokon::MustFaceMud::eYes;
            field_1B8_brain_state = 2;

            if (ringMudTlv->mAction == relive::reliveSwitchOp::eSetTrue)
            {
                // Pull switch
                field_188 = 6;
            }
            else
            {
                // Give ring
                field_188 = 7;
            }

            field_1AA_ring_timeout = ringMudTlv->mRingTimeout;
            field_1A4_code_converted = Code_Convert(ringMudTlv->mCode1, ringMudTlv->mCode2);
            field_1A8_code_length = Code_Length(field_1A4_code_converted);

            mGiveRingWithoutPassword = ringMudTlv->mGiveRingWithoutPassword == relive::reliveChoice::eYes;
            mSnapToGrid = false;
            mDeaf = false;


            scale = ringMudTlv->mScale;
        }
        break;

        case ReliveTypes::eMudokon:
        {
            SetType(ReliveTypes::eMudokon);

            auto mudTlv = static_cast<relive::Path_Mudokon*>(pTlv);

            if (mudTlv->mJob == relive::Path_Mudokon::MudJobs::eStandScrub)
            {
                field_1B8_brain_state = 8;
            }
            else if (mudTlv->mJob == relive::Path_Mudokon::MudJobs::eSitScrub)
            {
                field_1B8_brain_state = 9;
            }
            else if (mudTlv->mJob == relive::Path_Mudokon::MudJobs::eSitChant)
            {
                field_1B8_brain_state = 14;
            }

            field_124_voice_pitch = mudTlv->mVoicePitch;
            field_1B2_rescue_switch_id = mudTlv->mRescueSwitchId;

            GetAnimation().SetFlipX(mudTlv->mFacing == relive::reliveXDirection::eLeft);

            // TODO: Check these as well
            mDeaf = mudTlv->mDeaf == relive::reliveChoice::eYes;
            mPersist = mudTlv->mPersistAndResetOffscreen == relive::reliveChoice::eYes;
            mSnapToGrid = true;

            field_1B4_idle_time = 0;

            scale = mudTlv->mScale;
        }
        break;
        default:
            LOG_WARNING("Mudokon ctor pTlv->mTlvType32.mType was %d. This is unhandled.", static_cast<s32>(pTlv->mTlvType));
            break;
    }

    SetCurrentMotion(eMudMotions::Motion_0_Idle);

    if (scale == relive::reliveScale::eHalf)
    {
        SetSpriteScale(FP_FromDouble(0.5));
        GetAnimation().SetRenderLayer(Layer::eLayer_AbeMenu_Half_13);
        SetScale(Scale::Bg);
    }
    else
    {
        SetSpriteScale(FP_FromInteger(1));
        GetAnimation().SetRenderLayer(Layer::eLayer_AbeMenu_32);
        SetScale(Scale::Fg);
    }

    FP hitX = {};
    FP hitY = {};
    const s16 bHit = sCollisions->Raycast(
        FP_FromInteger(pTlv->mTopLeftX),
        FP_FromInteger(pTlv->mTopLeftY),
        FP_FromInteger(pTlv->mBottomRightX),
        FP_FromInteger(pTlv->mBottomRightY),
        &BaseAliveGameObjectCollisionLine,
        &hitX,
        &hitY,
        GetSpriteScale() != FP_FromDouble(0.5) ? kFgWallsOrFloor : kBgWallsOrFloor) ? 1 : 0;

    mXPos = FP_FromInteger((pTlv->mTopLeftX + pTlv->mBottomRightX) / 2);

    if (bHit)
    {
        mYPos = hitY;
        if (BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eDynamicCollision_32 || BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eBackgroundDynamicCollision_36)
        {
            const PSX_RECT bRect = VGetBoundingRect();
            OnCollisionWith(
                PSX_Point{bRect.x, static_cast<s16>(bRect.y + 5)},
                PSX_Point{bRect.w, static_cast<s16>(bRect.h + 5)},
                gPlatformsArray);
        }
    }

    BaseAliveGameObjectLastLineYPos = mYPos;

    if (field_1B8_brain_state == 9 || field_1B8_brain_state == 8)
    {
        mBit7_Unknown = true;
        MapFollowMe(mSnapToGrid);
    }
    else
    {
        mBit7_Unknown = false;
    }

    mBit2_Unknown = true;

    field_1BA_brain_sub_state = 0;
    field_1A0 = 0;
    mTlvId = tlvId;
    field_1C4_bDoPathTrans = false;

    if (field_188 == 6)
    {
        mXPos += FP_FromInteger(8);
    }

    field_1AC_pBirdPortal = nullptr;
    field_194_pLiftPoint = nullptr;

    CreateShadow();

    VUpdate();
}

Mudokon::~Mudokon()
{
    if (mHealth <= FP_FromInteger(0))
    {
        sKilledMudokons++;
    }

    RemoveAlerted();

    KillBirdPortal();
    KillLiftPoint_194();

    if (!mBit2_Unknown || mHealth <= FP_FromInteger(0) || GetElectrocuted())
    {
        Path::TLV_Reset(mTlvId, -1, 0, 1);
    }
    else
    {
        Path::TLV_Reset(mTlvId, -1, 0, 0);
    }

    /*
    for (auto& res : field_148_res_array.res)
    {
        if (res && res != mAnim.field_20_ppBlock)
        {
            ResourceManager::FreeResource_455550(res);
        }
    }
    */

    if (field_1B8_brain_state == 7)
    {
        if (field_1AA_ring_timeout)
        {
            sActiveHero->field_168_ring_pulse_timer = field_1AA_ring_timeout + sGnFrame;
        }
        else
        {
            sActiveHero->field_168_ring_pulse_timer = sGnFrame + 200000;
        }
        sActiveHero->field_16C_bHaveShrykull = false;
    }

    if (GetCurrentMotion() == eMudMotions::Motion_52_Chant || GetCurrentMotion() == eMudMotions::Motion_59_CrouchChant)
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
        SetDead(true);
        return;
    }

    if (field_1C4_bDoPathTrans)
    {
        DoPathTrans();
        field_1C4_bDoPathTrans = false;
    }

    const auto old_motion = GetCurrentMotion();
    const auto old_brain = field_1B8_brain_state;
    const auto old_brain_sub_state = field_1BA_brain_sub_state;
    field_1BA_brain_sub_state = InvokeMemberFunction(this, gMudBrains_4CD430, field_1B8_brain_state);

    if (old_brain != field_1B8_brain_state)
    {
        // Main brain state changed
        //LOG_INFO("Brain changed from " << old_brain << " to " << field_1B8_brain_state);
    }
    else if (old_brain_sub_state != field_1BA_brain_sub_state)
    {
        //LOG_INFO("Brain sub state changed from " << old_brain_sub_state << " to " << field_1BA_brain_sub_state);
    }

    const FP oldX = mXPos;
    const FP oldY = mYPos;
    InvokeMemberFunction(this, gMudMotions_4CD470, mCurrentMotion);

    if (oldX != mXPos || oldY != mYPos)
    {
        BaseAliveGameObjectPathTLV = gMap.TLV_Get_At(
            0,
            mXPos,
            mYPos,
            mXPos,
            mYPos);
        VOnTlvCollision(BaseAliveGameObjectPathTLV);
    }

    if (old_motion != GetCurrentMotion() || mbMotionChanged)
    {
        mbMotionChanged = false;
        VUpdateResBlock();

        if (old_motion == eMudMotions::Motion_10_Unused)
        {
            GetAnimation().SetFrame(mBaseAliveGameObjectLastAnimFrame);
        }
    }
    else if (field_1BC)
    {
        SetCurrentMotion(mPreviousMotion);
        VUpdateResBlock();
        GetAnimation().SetFrame(mBaseAliveGameObjectLastAnimFrame);
        field_1BC = 0;
    }

    if (GetElectrocuted())
    {
        EventBroadcast(kEventMudokonDead, sActiveHero);
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
    AnimId::Mudokon_LiftUseUp,
    AnimId::Mudokon_LiftUseDown,
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

void Mudokon::LoadAnimations()
{
    for (auto& animId : sMudMotionAnimIds)
    {
        if (animId != AnimId::None)
        {
            mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(animId));
        }
    }
}

void Mudokon::VUpdateResBlock()
{
    GetAnimation().Set_Animation_Data(GetAnimRes(sMudMotionAnimIds[mCurrentMotion]));
}

void Mudokon::VScreenChanged()
{
    // Map/overlay changed or mud shouldn't persist
    if (gMap.LevelChanged() || !mPersist)
    {
        SetDead(true);
        KillBirdPortal();
        KillLiftPoint_194();
        return;
    }

    if (gMap.PathChanged())
    {
        // See if we need to go to the next path
        auto pTlv = gMap.TLV_Get_At(nullptr, mXPos, mYPos, mXPos, mYPos);
        while (pTlv)
        {
            if (pTlv->mTlvType == ReliveTypes::eMudokonPathTrans)
            {
                // Gonna go to the next path
                field_1C4_bDoPathTrans = true;
                return;
            }
            pTlv = gMap.TLV_Get_At(pTlv, mXPos, mYPos, mXPos, mYPos);
        }

        // Wasn't a path trans and path changed, die
        SetDead(true);
        KillBirdPortal();
        KillLiftPoint_194();
    }
}

bool Mudokon::VTakeDamage(BaseGameObject* pFrom)
{
    switch (pFrom->Type())
    {
        case ReliveTypes::eGasCountDown:
            if (mHealth > FP_FromInteger(0))
            {
                field_1B8_brain_state = 15;
                field_1BA_brain_sub_state = 0;
            }
            return true;

        case ReliveTypes::eElectricWall:
            Mudokon_SFX(MudSounds::eDeathDropScream_17, 0, 0, this);
            return true;

        case ReliveTypes::eGroundExplosion:
        case ReliveTypes::eMeatSaw:
        case ReliveTypes::eAirExplosion:
            if (mHealth > FP_FromInteger(0))
            {
                mHealth = FP_FromInteger(0);

                relive_new Gibs(
                    GibType::Mud_4,
                    mXPos,
                    mYPos,
                    FP_FromInteger(0),
                    FP_FromInteger(0),
                    GetSpriteScale());

                relive_new Gibs(
                    GibType::Mud_4,
                    mXPos,
                    mYPos,
                    FP_FromInteger(0),
                    FP_FromInteger(0),
                    GetSpriteScale());

                SetDead(true);
                EventBroadcast(kEventMudokonDead, sActiveHero);
            }
            return true;

        case ReliveTypes::eAbilityRing:
            return false;

        case ReliveTypes::eBullet:
            mbGotShot = true;
            if (mHealth > FP_FromInteger(0))
            {
                auto pBullet = static_cast<Bullet*>(pFrom);
                FP bloodXOff = {};
                if (pBullet->mXDistance <= FP_FromInteger(0))
                {
                    bloodXOff = FP_FromInteger(-24);
                }
                else
                {
                    bloodXOff = FP_FromInteger(24);
                }

                relive_new Blood(
                    mXPos,
                    pBullet->mYPos,
                    bloodXOff,
                    FP_FromInteger(0),
                    GetSpriteScale(),
                    50);

                EventBroadcast(kEventMudokonDead, sActiveHero);
                EventBroadcast(kEventMudokonDead, sActiveHero);
                return DoSmashDamage();
            }
            return true;

        case ReliveTypes::eRockSpawner:
            EventBroadcast(kEventMudokonDead, sActiveHero);
            return DoSmashDamage();

        case ReliveTypes::eScrab:
            return DoSmashDamage();

        case ReliveTypes::eShrykull:
        case ReliveTypes::eElectrocute:
            if (mHealth > FP_FromInteger(0))
            {
                SetDead(true);
                mHealth = FP_FromInteger(0);
            }
            return true;

        case ReliveTypes::eSlog:
            if (mHealth > FP_FromInteger(0))
            {
                mPersist = false;
                mHealth = FP_FromInteger(0);
                field_1B8_brain_state = 11;
                field_1C0_timer = sGnFrame + 90;
                SetCurrentMotion(eMudMotions::Motion_46_FallLandDie);
                VUpdateResBlock();
            }
            return true;

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
                    Environment_SFX(EnvironmentSfx::eDeathNoise_7, 0, 0x7FFF, this);
                    mPersist = false;
                    mHealth = FP_FromInteger(0);
                    field_1B8_brain_state = 11;
                    field_1C0_timer = sGnFrame + 90;
                    SetCurrentMotion(eMudMotions::Motion_46_FallLandDie);
                    SetNextMotion(-1);
                    VUpdateResBlock();
                }
            }
            return true;

        default:
            if (mHealth > FP_FromInteger(0))
            {
                Mudokon_SFX(Math_RandomRange(0, 127) >= 64 ? MudSounds::eBeesStruggle_18 : MudSounds::eKnockbackOuch_10, 127, 2 * Math_NextRandom() - 256, 0);
                mHealth -= FP_FromDouble(0.06);
                SetCurrentMotion(eMudMotions::Motion_46_FallLandDie);
                SetNextMotion(-1);
                field_1C0_timer = sGnFrame + 30;
                VUpdateResBlock();
                field_1B8_brain_state = 9;
                field_1BA_brain_sub_state = 6;

                RemoveAlerted();

                if (mHealth <= FP_FromInteger(0))
                {
                    mPersist = false;
                    field_1B8_brain_state = 11;
                    field_1C0_timer = sGnFrame + 90;
                }
            }
            return true;
    }
}

bool Mudokon::DoSmashDamage()
{
    if (mHealth > FP_FromInteger(0))
    {
        mPersist = false;
        field_1C0_timer = sGnFrame + 90;
        mHealth = FP_FromInteger(0);
        field_1B8_brain_state = 11;
        SetCurrentMotion(eMudMotions::Motion_46_FallLandDie);
        SetNextMotion(-1);
        VUpdateResBlock();
        Mudokon_SFX(MudSounds::eKnockbackOuch_10, 0, Math_RandomRange(-127, 127), this);
        return true;
    }
    return false;
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
    const auto mudMotion = static_cast<eMudMotions>(motion);
    if (mudMotion < eMudMotions::Motion_11_Null)
    {
        return field_148_res_array.res[0];
    }
    if (mudMotion < eMudMotions::Motion_15_LeverUse)
    {
        return field_148_res_array.res[1];
    }
    if (mudMotion < eMudMotions::Motion_16_StandScrubLoop)
    {
        return field_148_res_array.res[2];
    }
    if (mudMotion < eMudMotions::Motion_22_CrouchScrub)
    {
        return field_148_res_array.res[3];
    }
    if (mudMotion < eMudMotions::Motion_23_CrouchIdle)
    {
        return field_148_res_array.res[4];
    }
    if (mudMotion < eMudMotions::Motion_46_FallLandDie)
    {
        return field_148_res_array.res[5];
    }
    if (mudMotion < eMudMotions::Motion_47_Knockback)
    {
        return field_148_res_array.res[6];
    }
    if (mudMotion < eMudMotions::Motion_49_WalkOffEdge)
    {
        return field_148_res_array.res[7];
    }
    if (mudMotion < eMudMotions::Motion_52_Chant)
    {
        return field_148_res_array.res[8];
    }
    if (mudMotion < eMudMotions::Motion_54_ToDuck)
    {
        return field_148_res_array.res[9];
    }
    if (mudMotion < eMudMotions::Motion_57_Struggle)
    {
        return field_148_res_array.res[10];
    }
    if (mudMotion < eMudMotions::Motion_58_StruggleToCrouchChant)
    {
        return field_148_res_array.res[11];
    }
    if (mudMotion < eMudMotions::Motion_61_DuckKnockback)
    {
        return field_148_res_array.res[12];
    }
    if (mudMotion < eMudMotions::Motion_62_PoisonGasDeath)
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

    if (sActiveHero->GetAnimation().GetFlipX())
    {
        mXPos = FP_FromInteger((camCoords.x + XGrid_Index_To_XPos(GetSpriteScale(), MaxGridBlocks(GetSpriteScale())))) + ScaleToGridSize(GetSpriteScale());
    }
    else
    {
        mXPos = (FP_FromInteger(camCoords.x + XGrid_Index_To_XPos(GetSpriteScale(), 0))) - ScaleToGridSize(GetSpriteScale());
    }

    if (sActiveHero->BaseAliveGameObjectCollisionLine)
    {
        mYPos = sActiveHero->mYPos;
    }
    else
    {
        mYPos = FP_FromInteger(FP_GetExponent(mYPos) % 480 + camCoords.y);
    }

    if (BaseAliveGameObjectCollisionLine)
    {
        if (BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eDynamicCollision_32)
        {
            auto pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
            if (pPlatform)
            {
                pPlatform->VRemove(this);
                BaseAliveGameObject_PlatformId = Guid{};
            }
        }

        PathLine* pLine = nullptr;
        FP hitY = {};
        FP hitX = {};
        if (sCollisions->Raycast(
                mXPos,
                mYPos - FP_FromInteger(40),
                mXPos,
                mYPos + FP_FromInteger(40),
                &pLine,
                &hitX,
                &hitY,
                GetSpriteScale() != FP_FromDouble(0.5) ? kFgWallsOrFloor : kBgWallsOrFloor))
        {
            mYPos = hitY;
            BaseAliveGameObjectCollisionLine = pLine;
        }
        else
        {
            BaseAliveGameObjectCollisionLine = nullptr;
        }
    }
    SetUpdateDelay(20);
    mCurrentLevel = gMap.mCurrentLevel;
    mCurrentPath = gMap.mCurrentPath;
}

void Mudokon::ToStand()
{
    GetAnimation().SetFlipY(false);
    GetAnimation().SetSwapXY(false);
    field_11C = FP_FromInteger(0);
    mVelX = FP_FromInteger(0);
    mVelY = FP_FromInteger(0);
    SetCurrentMotion(eMudMotions::Motion_0_Idle);
}

void Mudokon::CheckFloorGone()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    if (pPlatform)
    {
        if (pPlatform->GetDead())
        {
            VOnTrapDoorOpen();
            mBit8_Unknown = true;
        }
    }
    else
    {
        if (mBit8_Unknown)
        {
            Motion_51_Fall();
        }
    }
}

s16 Mudokon::CanAbeSneak(Mudokon* /*pMud*/)
{
    return sActiveHero->mCurrentMotion == eAbeMotions::Motion_42_SneakLoop
        || sActiveHero->mCurrentMotion == eAbeMotions::Motion_47_SneakBegin
        || sActiveHero->mCurrentMotion == eAbeMotions::Motion_43_WalkToSneak
        || sActiveHero->mCurrentMotion == eAbeMotions::Motion_45_MidWalkToSneak
        || sActiveHero->mCurrentMotion == eAbeMotions::Motion_44_SneakToWalk
        || sActiveHero->mCurrentMotion == eAbeMotions::Motion_46_MidSneakToWalk;
}

void Mudokon::ToKnockback()
{
    Environment_SFX(EnvironmentSfx::eKnockback_13, 0, 0x7FFF, this);

    if (mVelX > FP_FromInteger(0))
    {
        if (GetAnimation().GetFlipX())
        {
            mXPos += mVelX + (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(2));
        }
        else
        {
            mXPos += mVelX - (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(2));
        }
    }
    MapFollowMe(mSnapToGrid);

    mVelX = FP_FromInteger(0);

    if (mVelY < FP_FromInteger(0))
    {
        mVelY = FP_FromInteger(0);
    }

    Environment_SFX(EnvironmentSfx::eKnockback_13, 0, 0x7FFF, this);
    SetCurrentMotion(eMudMotions::Motion_47_Knockback);
}

void Mudokon::ReduceXVelocityBy(FP amount)
{
    if (mVelX > FP_FromInteger(0))
    {
        mVelX -= (GetSpriteScale() * amount);
        if (mVelX < FP_FromInteger(0))
        {
            mVelX = FP_FromInteger(0);
        }
    }
    else if (mVelX < FP_FromInteger(0))
    {
        mVelX += (GetSpriteScale() * amount);
        if (mVelX > FP_FromInteger(0))
        {
            mVelX = FP_FromInteger(0);
        }
    }

    if (FP_GetDouble(mVelX))
    {
        MoveOnLine();
    }
}

void Mudokon::MoveOnLine()
{
    CheckFloorGone();

    const FP old_xpos = mXPos;
    if (BaseAliveGameObjectCollisionLine)
    {
        BaseAliveGameObjectCollisionLine = BaseAliveGameObjectCollisionLine->MoveOnLine(
            &mXPos,
            &mYPos,
            mVelX);
    }

    if (BaseAliveGameObjectCollisionLine)
    {
        auto pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
        if (pPlatform)
        {
            if (BaseAliveGameObjectCollisionLine->mLineType != eLineTypes::eDynamicCollision_32 && BaseAliveGameObjectCollisionLine->mLineType != eLineTypes::eBackgroundDynamicCollision_36)
            {
                pPlatform->VRemove(this);
                BaseAliveGameObject_PlatformId = Guid{};

            }
        }
        else
        {
            if (BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eDynamicCollision_32 ||
                BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eBackgroundDynamicCollision_36)
            {
                const PSX_RECT bRect = VGetBoundingRect();
                OnCollisionWith(
                    PSX_Point{bRect.x, static_cast<s16>(bRect.y + 5)},
                    PSX_Point{bRect.w, static_cast<s16>(bRect.h + 5)},
                    gPlatformsArray);
            }
        }
    }
    else
    {
        field_11C = FP_FromInteger(0); // TODO: is 0 wrong? AE sets 0.3 - check IDA/OG bug in AO?
        BaseAliveGameObjectLastLineYPos = mYPos;
        SetCurrentMotion(eMudMotions::Motion_49_WalkOffEdge);
        mXPos = old_xpos + mVelX;
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

    if (FP_Abs(field_1AC_pBirdPortal->mXPos - mXPos) < FP_FromInteger(gPsxDisplay.mWidth) && FP_Abs(field_1AC_pBirdPortal->mHitY - mYPos) < FP_FromInteger(10))
    {
        if (field_1AC_pBirdPortal->mPortalType == relive::Path_BirdPortal::PortalType::eWorker || field_1AC_pBirdPortal->mPortalType == relive::Path_BirdPortal::PortalType::eShrykull)
        {
            sActiveHero->ChangeChantState(1);
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
    if (pTarget->mXPos == mXPos)
    {
        return true;
    }
    else if (pTarget->mXPos > mXPos && !GetAnimation().GetFlipX())
    {
        return true;
    }
    else if (pTarget->mXPos < mXPos && GetAnimation().GetFlipX())
    {
        return true;
    }
    return false;
}

GameSpeakEvents Mudokon::LastGameSpeak()
{
    if (!gMap.Is_Point_In_Current_Camera(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos,
            1)
        || mDeaf)
    {
        return GameSpeakEvents::eNone_m1;
    }

    if (field_128 == gEventSystem->mLastEventIndex)
    {
        if (gEventSystem->mLastEvent == GameSpeakEvents::eNone_m1)
        {
            return GameSpeakEvents::eNone_m1;
        }
        else
        {
            return GameSpeakEvents::eSameAsLast_m2;
        }
    }

    field_128 = gEventSystem->mLastEventIndex;

    return gEventSystem->mLastEvent;
}

void Mudokon::AddAlerted()
{
    if (!mAlerted)
    {
        sAlertedMudCount_507B90++;
        mAlerted = true;
    }
}


void Mudokon::RemoveAlerted()
{
    if (mAlerted)
    {
        sAlertedMudCount_507B90--;
        mAlerted = false;
    }
}

s16 Mudokon::IAmNearestToAbe()
{
    for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
    {
        IBaseAliveGameObject* pObjIter = gBaseAliveGameObjects->ItemAt(i);
        if (!pObjIter)
        {
            break;
        }

        if (pObjIter != this)
        {
            if (pObjIter->Type() == ReliveTypes::eRingOrLiftMud || pObjIter->Type() == ReliveTypes::eMudokon) // mud or password mud?
            {
                if (Math_Distance(
                        FP_GetExponent(sActiveHero->mXPos),
                        FP_GetExponent(sActiveHero->mYPos),
                        FP_GetExponent(mXPos),
                        FP_GetExponent(mYPos))
                    > Math_Distance(
                        FP_GetExponent(sActiveHero->mXPos),
                        FP_GetExponent(sActiveHero->mYPos),
                        FP_GetExponent(pObjIter->mXPos),
                        FP_GetExponent(pObjIter->mYPos)))
                {
                    // Our distance is to abe is larger than the mud we just found so we can't be nearest
                    return false;
                }
            }
        }
    }
    return true;
}

void Mudokon::VOnTrapDoorOpen()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    if (pPlatform)
    {
        if (mbGotShot)
        {
            mBit8_Unknown = true;
        }
        else
        {
            const auto motion = static_cast<s16>(eMudMotions::Motion_49_WalkOffEdge);
            VSetMotion(motion);
        }

        pPlatform->VRemove(this);
        BaseAliveGameObject_PlatformId = Guid{};
    }
}

void Mudokon::VOnTlvCollision(relive::Path_TLV* pTlv)
{
    if (pTlv)
    {
        while (pTlv)
        {
            if (pTlv->mTlvType == ReliveTypes::eDeathDrop)
            {
                if (mHealth > FP_FromInteger(0))
                {
                    field_1BA_brain_sub_state = 0;
                    mHealth = FP_FromInteger(0);
                    field_1B8_brain_state = 13;
                    EventBroadcast(kEventMudokonDead, sActiveHero);
                }
            }
            pTlv = gMap.TLV_Get_At(pTlv, mXPos, mYPos, mXPos, mYPos);
        }
    }
}

void Mudokon::Motion_0_Idle()
{
    CheckFloorGone();

    BaseAliveGameObjectLastLineYPos = mYPos;

    switch (GetNextMotion())
    {
        case eMudMotions::Motion_1_WalkLoop:
            if (GetAnimation().GetFlipX())
            {
                mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(9));
            }
            else
            {
                mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(9));
            }
            SetCurrentMotion(eMudMotions::Motion_7_WalkBegin);
            SetNextMotion(-1);
            break;

        case eMudMotions::Motion_35_SneakLoop:
            if (GetAnimation().GetFlipX())
            {
                mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(10));
            }
            else
            {
                mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(10));
            }
            SetCurrentMotion(eMudMotions::Motion_40_SneakBegin);
            SetNextMotion(-1);
            break;

        case eMudMotions::Motion_29_RunLoop:
            if (GetAnimation().GetFlipX())
            {
                mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
            }
            else
            {
                mVelX = ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4);
            }
            SetCurrentMotion(eMudMotions::Motion_45_StandToRun);
            SetNextMotion(-1);
            break;

        case eMudMotions::Motion_52_Chant:
            SND_SEQ_PlaySeq_4775A0(SeqId::eMudokonChant_12, 0, 1);
            SetCurrentMotion(eMudMotions::Motion_52_Chant);
            SetNextMotion(-1);
            break;

        case eMudMotions::Motion_23_CrouchIdle:
            SetCurrentMotion(eMudMotions::Motion_25_StandToCrouch);
            SetNextMotion(-1);
            break;

        case eMudMotions::Motion_59_CrouchChant:
            SetCurrentMotion(eMudMotions::Motion_58_StruggleToCrouchChant);
            SetNextMotion(-1);
            break;

        case eMudMotions::Motion_16_StandScrubLoop:
            SetCurrentMotion(eMudMotions::Motion_20_IdleToStandScrub);
            SetNextMotion(-1);
            break;

        default:
            if (mNextMotion != -1)
            {
                SetCurrentMotion(mNextMotion);
                SetNextMotion(-1);
            }
            break;
    }
}

void Mudokon::Motion_1_WalkLoop()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (WallHit(
            GetSpriteScale() * FP_FromInteger(50),
            mVelX))
    {
        ToStand();
    }
    else
    {
        MoveOnLine();
        if (GetCurrentMotion() == eMudMotions::Motion_1_WalkLoop)
        {
            switch (GetAnimation().GetCurrentFrame())
            {
                case 2:
                    if (GetNextMotion() == eMudMotions::Motion_0_Idle)
                    {
                        SetCurrentMotion(eMudMotions::Motion_9_MidWalkToIdle);
                        SetNextMotion(-1);
                    }
                    break;

                case 5:
                    Environment_SFX(EnvironmentSfx::eWalkingFootstep_1, 0, 0x7FFF, this);
                    if (!mBit7_Unknown)
                    {
                        mBit7_Unknown = true;
                        MapFollowMe(mSnapToGrid);
                    }

                    if (GetNextMotion() == eMudMotions::Motion_29_RunLoop)
                    {
                        SetCurrentMotion(eMudMotions::Motion_28_MidWalkToRun);
                        SetNextMotion(-1);
                    }
                    else if (GetNextMotion() == eMudMotions::Motion_35_SneakLoop)
                    {
                        SetCurrentMotion(eMudMotions::Motion_36_MidWalkToSneak);
                        SetNextMotion(-1);
                    }
                    break;

                case 11:
                    if (GetNextMotion() == eMudMotions::Motion_0_Idle)
                    {
                        SetCurrentMotion(eMudMotions::Motion_8_WalkToIdle);
                        SetNextMotion(-1);
                    }
                    break;

                case 14:
                    Environment_SFX(EnvironmentSfx::eWalkingFootstep_1, 0, 0x7FFF, this);
                    if (!mBit7_Unknown)
                    {
                        mBit7_Unknown = true;
                        MapFollowMe(mSnapToGrid);
                    }

                    if (GetNextMotion() == eMudMotions::Motion_29_RunLoop)
                    {
                        SetCurrentMotion(eMudMotions::Motion_27_WalkToRun);
                        SetNextMotion(-1);
                    }
                    else if (GetNextMotion() == eMudMotions::Motion_35_SneakLoop)
                    {
                        SetCurrentMotion(eMudMotions::Motion_38_WalkToSneak);
                        SetNextMotion(-1);
                    }
                    break;

                default:
                    mBit7_Unknown = false;
                    break;
            }
        }
    }
}

void Mudokon::Motion_2_StandingTurn()
{
    CheckFloorGone();

    if (GetAnimation().GetCurrentFrame() == 0)
    {
        Environment_SFX(EnvironmentSfx::eGenericMovement_9, 0, 0x7FFF, this);
    }

    if (GetAnimation().GetIsLastFrame())
    {
        SetCurrentMotion(eMudMotions::Motion_0_Idle);
        GetAnimation().ToggleFlipX();
    }
}

void Mudokon::Motion_3_Speak()
{
    CheckFloorGone();

    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (GetAnimation().GetIsLastFrame())
    {
        SetCurrentMotion(eMudMotions::Motion_0_Idle);
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

    if (WallHit(
            GetSpriteScale() * FP_FromInteger(50),
            mVelX))
    {
        ToStand();
    }
    else
    {
        if (GetAnimation().GetIsLastFrame())
        {
            SetCurrentMotion(eMudMotions::Motion_1_WalkLoop);
        }
        MoveOnLine();
    }
}

void Mudokon::Motion_8_WalkToIdle()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (WallHit(
            GetSpriteScale() * FP_FromInteger(50),
            mVelX))
    {
        ToStand();
    }
    else
    {
        MoveOnLine();

        if (GetAnimation().GetCurrentFrame() == 0)
        {
            Environment_SFX(EnvironmentSfx::eWalkingFootstep_1, 0, 0x7FFF, this);
            return;
        }

        if (GetAnimation().GetIsLastFrame())
        {
            MapFollowMe(mSnapToGrid);
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
    if (gNumCamSwappers <= 0)
    {
        SetCurrentMotion(mPreviousMotion);
        if (BaseAliveGameObject_PlatformId != Guid{})
        {
            mXPos = FP_FromInteger((BaseAliveGameObjectCollisionLine->mRect.x + BaseAliveGameObjectCollisionLine->mRect.w) / 2);
            mYPos = FP_FromInteger(BaseAliveGameObjectCollisionLine->mRect.y);
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
    if (!pLiftPoint->OnAnyFloor() || pLiftPoint->mIgnoreLiftMover)
    {
        pLiftPoint->Move(FP_FromInteger(0), FP_FromInteger(3), 0);
    }
    else
    {
        pLiftPoint->Move(FP_FromInteger(0), FP_FromInteger(0), 0);
        SetCurrentMotion(eMudMotions::Motion_14_LiftGrabEnd);
        SetNextMotion(eMudMotions::Motion_0_Idle);
    }
}

void Mudokon::Motion_13_LiftGrabBegin()
{
    if (GetAnimation().GetIsLastFrame())
    {
        SetCurrentMotion(mNextMotion);
        field_194_pLiftPoint->mKeepOnMiddleFloor = true;
    }
}

void Mudokon::Motion_14_LiftGrabEnd()
{
    if (GetAnimation().GetIsLastFrame())
    {
        SetCurrentMotion(mNextMotion);
    }
}

void Mudokon::Motion_15_LeverUse()
{
    if (GetAnimation().GetIsLastFrame())
    {
        SetCurrentMotion(eMudMotions::Motion_0_Idle);
    }
}

void Mudokon::Motion_16_StandScrubLoop()
{
    if (gMap.Is_Point_In_Current_Camera(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos,
            0))
    {
        if (GetAnimation().GetCurrentFrame())
        {
            if (GetAnimation().GetCurrentFrame() == 5)
            {
                SfxPlayMono(relive::SoundEffects::Clean2, 0);
            }
        }
        else
        {
            SfxPlayMono(relive::SoundEffects::Clean1, 0);
        }
    }

    if (mNextMotion != -1)
    {
        if (GetAnimation().GetIsLastFrame())
        {
            if (GetNextMotion() != eMudMotions::Motion_0_Idle)
            {
                SetCurrentMotion(mNextMotion);
            }
            else
            {
                SetCurrentMotion(eMudMotions::Motion_21_StandScrubToIdle);
            }
            SetNextMotion(-1);
        }
    }
}

void Mudokon::Motion_17_StandScrubLoopToPause()
{
    if (GetAnimation().GetIsLastFrame())
    {
        SetCurrentMotion(eMudMotions::Motion_19_StandScrubPause);
    }
}

void Mudokon::Motion_18_StandScrubPauseToLoop()
{
    if (GetAnimation().GetIsLastFrame())
    {
        SetCurrentMotion(eMudMotions::Motion_16_StandScrubLoop);
    }
}

void Mudokon::Motion_19_StandScrubPause()
{
    if (mNextMotion != -1)
    {
        if (GetAnimation().GetIsLastFrame())
        {
            SetCurrentMotion(eMudMotions::Motion_18_StandScrubPauseToLoop);
            SetNextMotion(-1);
        }
    }
}

void Mudokon::Motion_20_IdleToStandScrub()
{
    if (GetAnimation().GetIsLastFrame())
    {
        SetCurrentMotion(eMudMotions::Motion_16_StandScrubLoop);
    }
}

void Mudokon::Motion_21_StandScrubToIdle()
{
    if (GetAnimation().GetIsLastFrame())
    {
        SetCurrentMotion(eMudMotions::Motion_0_Idle);
    }
}

void Mudokon::Motion_22_CrouchScrub()
{
    if (gMap.Is_Point_In_Current_Camera(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos,
            0))
    {
        if (GetAnimation().GetCurrentFrame() == 2)
        {
            SfxPlayMono(relive::SoundEffects::Clean1, 0);
        }
        else if (GetAnimation().GetCurrentFrame() == 6)
        {
            SfxPlayMono(relive::SoundEffects::Clean2, 0);
        }
    }

    if (GetAnimation().GetIsLastFrame())
    {
        SetCurrentMotion(eMudMotions::Motion_23_CrouchIdle);
    }
}

void Mudokon::Motion_23_CrouchIdle()
{
    CheckFloorGone();

    if (GetNextMotion() == eMudMotions::Motion_55_Duck)
    {
        SetCurrentMotion(eMudMotions::Motion_54_ToDuck);
        SetNextMotion(-1);
    }
    else if (GetNextMotion() == eMudMotions::Motion_0_Idle)
    {
        SetCurrentMotion(eMudMotions::Motion_26_CrouchToStand);
        SetNextMotion(-1);
    }
    else if (mNextMotion != -1)
    {
        SetCurrentMotion(mNextMotion);
        SetNextMotion(-1);
    }
}

void Mudokon::Motion_24_CrouchTurn()
{
    CheckFloorGone();

    if (GetAnimation().GetIsLastFrame())
    {
        SetCurrentMotion(eMudMotions::Motion_23_CrouchIdle);
        GetAnimation().ToggleFlipX();
    }
}

void Mudokon::Motion_25_StandToCrouch()
{
    CheckFloorGone();

    if (GetAnimation().GetIsLastFrame())
    {
        SetCurrentMotion(eMudMotions::Motion_23_CrouchIdle);
    }
}

void Mudokon::Motion_26_CrouchToStand()
{
    CheckFloorGone();

    if (GetAnimation().GetIsLastFrame())
    {
        SetCurrentMotion(eMudMotions::Motion_0_Idle);
    }
}

void Mudokon::Motion_27_WalkToRun()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (GetAnimation().GetFlipX())
    {
        mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
    }
    else
    {
        mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
    }

    if (GetAnimation().GetIsLastFrame())
    {
        SetCurrentMotion(eMudMotions::Motion_29_RunLoop);
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
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

    if (GetAnimation().GetFlipX())
    {
        mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
    }
    else
    {
        mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
    }

    if (GetAnimation().GetIsLastFrame())
    {
        SetPreviousMotion(eMudMotions::Motion_29_RunLoop);
        mBaseAliveGameObjectLastAnimFrame = 8;
        field_1BC = 1;
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
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

    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
    {
        ToKnockback();
        return;
    }

    MoveOnLine();

    if (GetCurrentMotion() == eMudMotions::Motion_29_RunLoop)
    {
        if (GetAnimation().GetCurrentFrame() == 0 || GetAnimation().GetCurrentFrame() == 8)
        {
            if (!mBit7_Unknown)
            {
                mBit7_Unknown = true;
                MapFollowMe(mSnapToGrid);
            }

            if (GetNextMotion() == eMudMotions::Motion_44_RunJumpMid)
            {
                SetCurrentMotion(eMudMotions::Motion_43_RunJumpBegin);
                SetNextMotion(-1);
            }
        }
        else if (GetAnimation().GetCurrentFrame() == 4 || GetAnimation().GetCurrentFrame() == 12)
        {
            Environment_SFX(EnvironmentSfx::eRunningFootstep_2, 0, 0x7FFF, this);
            if (!mBit7_Unknown)
            {
                mBit7_Unknown = true;
                MapFollowMe(mSnapToGrid);
            }

            switch (GetNextMotion())
            {
                case eMudMotions::Motion_1_WalkLoop:
                    if (GetAnimation().GetCurrentFrame() == 4)
                    {
                        SetCurrentMotion(eMudMotions::Motion_30_RunToWalk);
                        SetNextMotion(-1);
                    }
                    return;

                    // TODO: OG bug - MidRunToWalk will always be set on frame 12. AE doesn't do this.
                    // couldn't see any breaking changes after commenting this out.

                    /*case eMudMotions::Motion_29_RunLoop_43DB10:
                if (mAnim.mCurrentFrame == 12)
                {
                    SetCurrentMotion(eMudMotions::Motion_28_MidWalkToRun_43DA40);
                    field_FE_next_state = -1;
                }
                return;*/

                case eMudMotions::Motion_0_Idle:
                    SetCurrentMotion(eMudMotions::Motion_32_RunSlideStop);
                    SetNextMotion(-1);
                    Environment_SFX(EnvironmentSfx::eRunSlide_4, 0, 0x7FFF, this);
                    return;

                case eMudMotions::Motion_33_RunSlideTurn:
                    SetCurrentMotion(eMudMotions::Motion_33_RunSlideTurn);
                    SetNextMotion(-1);
                    Environment_SFX(EnvironmentSfx::eRunSlide_4, 0, 0x7FFF, this);
                    return;

                case eMudMotions::Motion_44_RunJumpMid:
                    SetCurrentMotion(eMudMotions::Motion_43_RunJumpBegin);
                    SetNextMotion(-1);
                    return;
            }
        }
        else
        {
            mBit7_Unknown = false;
        }
    }
}

void Mudokon::Motion_30_RunToWalk()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (GetAnimation().GetFlipX())
    {
        mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(9));
    }
    else
    {
        mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(9));
    }

    if (GetAnimation().GetIsLastFrame())
    {
        SetCurrentMotion(eMudMotions::Motion_1_WalkLoop);
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
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

    if (GetAnimation().GetFlipX())
    {
        mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(9));
    }
    else
    {
        mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(9));
    }

    if (GetAnimation().GetIsLastFrame())
    {
        SetPreviousMotion(eMudMotions::Motion_1_WalkLoop);
        mBaseAliveGameObjectLastAnimFrame = 9;
        field_1BC = 1;
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
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

    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
    {
        ToKnockback();
    }
    else
    {
        ReduceXVelocityBy(FP_FromDouble(0.375));

        if (GetCurrentMotion() == eMudMotions::Motion_32_RunSlideStop)
        {
            if (GetAnimation().GetIsLastFrame())
            {
                Environment_SFX(EnvironmentSfx::eWalkingFootstep_1, 0, 0x7FFF, this);
                MapFollowMe(mSnapToGrid);
                ToStand();
            }
        }
    }
}

void Mudokon::Motion_33_RunSlideTurn()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
    {
        ToKnockback();
    }
    else
    {
        ReduceXVelocityBy(FP_FromDouble(0.375));

        if (GetCurrentMotion() == eMudMotions::Motion_33_RunSlideTurn)
        {
            if (GetAnimation().GetIsLastFrame())
            {
                MapFollowMe(mSnapToGrid);

                if (GetAnimation().GetFlipX())
                {
                    mVelX = ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4);
                    SetCurrentMotion(eMudMotions::Motion_34_RunTurnToRun);
                }
                else
                {
                    mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
                    SetCurrentMotion(eMudMotions::Motion_34_RunTurnToRun);
                }
            }
        }
    }
}

void Mudokon::Motion_34_RunTurnToRun()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
    {
        ToStand();
    }
    else
    {
        MoveOnLine();
        if (GetAnimation().GetIsLastFrame())
        {
            SetCurrentMotion(eMudMotions::Motion_29_RunLoop);
            GetAnimation().ToggleFlipX();
        }
    }
}

void Mudokon::Motion_35_SneakLoop()
{
    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
    {
        ToStand();
    }
    else
    {
        MoveOnLine();
        if (GetCurrentMotion() == eMudMotions::Motion_35_SneakLoop)
        {
            switch (GetAnimation().GetCurrentFrame())
            {
                case 3:
                    if (!mNextMotion)
                    {
                        SetCurrentMotion(eMudMotions::Motion_41_SneakToIdle);
                        SetNextMotion(-1);
                    }
                    break;

                case 6:
                    Environment_SFX(EnvironmentSfx::eSneakFootstep_3, 0, 0x7FFF, this);
                    MapFollowMe(mSnapToGrid);

                    if (GetNextMotion() == eMudMotions::Motion_1_WalkLoop || GetNextMotion() == eMudMotions::Motion_29_RunLoop)
                    {
                        SetCurrentMotion(eMudMotions::Motion_37_SneakToWalk);
                        SetNextMotion(-1);
                    }
                    break;

                case 13:
                    if (!mNextMotion)
                    {
                        SetCurrentMotion(eMudMotions::Motion_42_MidSneakToIdle);
                        SetNextMotion(-1);
                    }
                    break;

                case 16:
                    Environment_SFX(EnvironmentSfx::eSneakFootstep_3, 0, 0x7FFF, this);
                    MapFollowMe(mSnapToGrid);

                    if (GetNextMotion() == eMudMotions::Motion_1_WalkLoop || GetNextMotion() == eMudMotions::Motion_29_RunLoop)
                    {
                        SetCurrentMotion(eMudMotions::Motion_39_MidSneakToWalk);
                        SetNextMotion(-1);
                    }
                    break;
            }
        }
    }
}

void Mudokon::Motion_36_MidWalkToSneak()
{
    if (GetAnimation().GetFlipX())
    {
        mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(10));
    }
    else
    {
        mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(10));
    }

    if (GetAnimation().GetIsLastFrame())
    {
        SetCurrentMotion(eMudMotions::Motion_35_SneakLoop);
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
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
    if (GetAnimation().GetFlipX())
    {
        mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(9));
    }
    else
    {
        mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(9));
    }

    if (GetAnimation().GetIsLastFrame())
    {
        SetCurrentMotion(eMudMotions::Motion_1_WalkLoop);
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
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
    if (GetAnimation().GetFlipX())
    {
        mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(10));
    }
    else
    {
        mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(10));
    }

    if (GetAnimation().GetIsLastFrame())
    {
        SetPreviousMotion(eMudMotions::Motion_35_SneakLoop);
        mBaseAliveGameObjectLastAnimFrame = 10;
        field_1BC = 1;
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
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
    if (GetAnimation().GetFlipX())
    {
        mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(9));
    }
    else
    {
        mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(9));
    }

    if (GetAnimation().GetIsLastFrame())
    {
        SetPreviousMotion(eMudMotions::Motion_1_WalkLoop);
        mBaseAliveGameObjectLastAnimFrame = 9;
        field_1BC = 1;
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
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
    if (GetAnimation().GetIsLastFrame())
    {
        SetCurrentMotion(eMudMotions::Motion_35_SneakLoop);
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
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

    if (GetAnimation().GetIsLastFrame())
    {
        MapFollowMe(mSnapToGrid);
        ToStand();
    }
}

void Mudokon::Motion_42_MidSneakToIdle()
{
    MoveOnLine();

    if (GetAnimation().GetIsLastFrame())
    {
        MapFollowMe(mSnapToGrid);
        ToStand();
    }
}

void Mudokon::Motion_43_RunJumpBegin()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    CheckFloorGone();

    mXPos += mVelX;

    if (GetAnimation().GetCurrentFrame() == 0)
    {
        Environment_SFX(EnvironmentSfx::eRunJumpOrLedgeHoist_11, 0, 0x7FFF, this);
    }

    if (GetAnimation().GetIsLastFrame())
    {
        BaseAliveGameObjectLastLineYPos = mYPos;

        if (GetAnimation().GetFlipX())
        {
            mVelX = GetSpriteScale() * FP_FromDouble(-7.6);
        }
        else
        {
            mVelX = GetSpriteScale() * FP_FromDouble(7.6);
        }

        mVelY = (GetSpriteScale() * FP_FromDouble(-9.6));
        mYPos += mVelY;
        VOnTrapDoorOpen();
        SetCurrentMotion(eMudMotions::Motion_44_RunJumpMid);
        BaseAliveGameObjectCollisionLine = nullptr;
    }
}

void Mudokon::Motion_44_RunJumpMid()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (GetAnimation().GetCurrentFrame() == 5)
    {
        SFX_Play_Pitch(relive::SoundEffects::PossessEffect, 40, 2400);
    }

    const PSX_RECT bRect = VGetBoundingRect();

    if ((mVelX > FP_FromInteger(0) && (FP_FromInteger(bRect.x) > field_1AC_pBirdPortal->mXPos)) || ((mVelX < FP_FromInteger(0) && FP_FromInteger(bRect.w) < field_1AC_pBirdPortal->mXPos)))
    {
        mBit2_Unknown = false;
        mPersist = false;

        SetDead(true);
        mVelY = FP_FromInteger(0);
        mVelX = FP_FromInteger(0);

        SND_SEQ_Play_477760(SeqId::eSaveTriggerMusic_45, 1, 127, 127);


        relive_new MusicTrigger(relive::Path_MusicTrigger::MusicTriggerMusicType::eChime, relive::Path_MusicTrigger::TriggeredBy::eTimer, 0, 300);

        sRescuedMudokons++;

        if (field_1AC_pBirdPortal)
        {
            field_1AC_pBirdPortal->VMudSaved();
        }

        if (field_1B2_rescue_switch_id)
        {
            SwitchStates_Set(field_1B2_rescue_switch_id, 1);
        }
    }

    mVelY += (GetSpriteScale() * FP_FromDouble(1.8));
    mXPos += mVelX;
    mYPos += mVelY;
}

void Mudokon::Motion_45_StandToRun()
{
    if (GetAnimation().GetIsLastFrame())
    {
        SetCurrentMotion(eMudMotions::Motion_29_RunLoop);
    }

    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
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
    if (GetAnimation().GetCurrentFrame() == 4)
    {
        Environment_SFX(EnvironmentSfx::eKnockback_13, 0, 0x7FFF, this);
    }

    if ((gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarms
         || gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarmsReturn
         || gMap.mCurrentLevel == EReliveLevelIds::eBoardRoom)
        && GetAnimation().GetCurrentFrame() == 7)
    {
        Environment_SFX(EnvironmentSfx::eHitGroundSoft_6, 80, -200, this);
    }

    CheckFloorGone();

    if (mNextMotion != -1)
    {
        SetCurrentMotion(mNextMotion);
        SetNextMotion(-1);
    }
}

void Mudokon::Motion_47_Knockback()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if ((gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarms
         || gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarmsReturn
         || gMap.mCurrentLevel == EReliveLevelIds::eBoardRoom)
        && GetAnimation().GetCurrentFrame() == 7)
    {
        Environment_SFX(EnvironmentSfx::eHitGroundSoft_6, 80, -200, this);
    }

    if (!BaseAliveGameObjectCollisionLine)
    {
        Motion_51_Fall();

        if (GetCurrentMotion() == eMudMotions::Motion_46_FallLandDie || GetCurrentMotion() == eMudMotions::Motion_50_LandSoft)
        {
            SetCurrentMotion(eMudMotions::Motion_47_Knockback);
        }
    }

    if (GetAnimation().GetIsLastFrame())
    {
        if (BaseAliveGameObjectCollisionLine)
        {
            SetCurrentMotion(eMudMotions::Motion_48_KnockbackGetUp);
        }
    }
}

void Mudokon::Motion_48_KnockbackGetUp()
{
    CheckFloorGone();

    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (GetAnimation().GetIsLastFrame())
    {
        ToStand();
    }
}

void Mudokon::Motion_49_WalkOffEdge()
{
    if (GetAnimation().GetIsLastFrame())
    {
        SetCurrentMotion(eMudMotions::Motion_51_Fall);
    }
    Motion_51_Fall();
}

void Mudokon::Motion_50_LandSoft()
{
    CheckFloorGone();

    if (GetAnimation().GetCurrentFrame() == 2)
    {
        if (GetPreviousMotion() == eMudMotions::Motion_51_Fall)
        {
            Environment_SFX(EnvironmentSfx::eLandingSoft_5, 0, 0x7FFF, this);
        }
        else
        {
            Environment_SFX(EnvironmentSfx::eHitGroundSoft_6, 0, 0x7FFF, this);
        }
    }

    if (GetAnimation().GetIsLastFrame())
    {
        ToStand();
    }
}

void Mudokon::Motion_51_Fall()
{
    if (mVelX > FP_FromInteger(0))
    {
        mVelX -= GetSpriteScale() * field_11C;
        if (mVelX < FP_FromInteger(0))
        {
            mVelX = FP_FromInteger(0);
        }
    }
    else if (mVelX < FP_FromInteger(0))
    {
        mVelX += GetSpriteScale() * field_11C;
        if (mVelX > FP_FromInteger(0))
        {
            mVelX = FP_FromInteger(0);
        }
    }

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    if (InAirCollision(&pLine, &hitX, &hitY, FP_FromDouble(1.8)))
    {
        switch (pLine->mLineType)
        {
            case eLineTypes::eFloor_0:
            case eLineTypes::eBackgroundFloor_4:
            case eLineTypes::eDynamicCollision_32:
            case eLineTypes::eBackgroundDynamicCollision_36:
                BaseAliveGameObjectCollisionLine = pLine;
                mXPos = hitX;
                mYPos = hitY;
                MapFollowMe(mSnapToGrid);
                if (mYPos - BaseAliveGameObjectLastLineYPos > FP_FromInteger(240))
                {
                    BaseAliveGameObjectLastLineYPos += FP_FromInteger(240);
                }

                if (mYPos - BaseAliveGameObjectLastLineYPos < (GetSpriteScale() * FP_FromInteger(180)) && (mHealth > FP_FromInteger(0) || gAbeInvulnerableCheat))
                {
                    SetCurrentMotion(eMudMotions::Motion_50_LandSoft);
                }
                else
                {
                    mHealth = FP_FromInteger(0);
                    ToKnockback();
                    mPersist = false;
                    field_1B8_brain_state = 11;
                    field_1C0_timer = sGnFrame + 90;
                    SetCurrentMotion(eMudMotions::Motion_46_FallLandDie);
                }

                SetPreviousMotion(eMudMotions::Motion_51_Fall);

                PSX_Point xy;
                xy.x = FP_GetExponent(mXPos - FP_FromInteger(10));
                xy.y = FP_GetExponent(mYPos - FP_FromInteger(10));

                PSX_Point wh;
                wh.x = FP_GetExponent(mXPos + FP_FromInteger(10));
                wh.y = FP_GetExponent(mYPos + FP_FromInteger(10));

                OnCollisionWith(
                    xy,
                    wh,
                    gPlatformsArray);
                break;

            case eLineTypes::eWallLeft_1:
            case eLineTypes::eWallRight_2:
            case eLineTypes::eBackgroundWallLeft_5:
            case eLineTypes::eBackgroundWallRight_6:
                mXPos = hitX;
                mYPos = hitY;
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

        New_TintChant_Particle(
            mXPos + (GetSpriteScale() * rndX),
            mYPos - (GetSpriteScale() * rndY),
            GetSpriteScale(),
            Layer::eLayer_0);
    }

    if (!SND_SsIsEos_DeInlined_477930(SeqId::eMudokonChant_12))
    {
        SND_SEQ_Play_477760(SeqId::eMudokonChant_12, 1, 50, 50);
    }

    if (GetNextMotion() == eMudMotions::Motion_0_Idle)
    {
        if (GetAnimation().GetIsLastFrame())
        {
            SND_Seq_Stop_477A60(SeqId::eMudokonChant_12);
            SetCurrentMotion(eMudMotions::Motion_53_ChantEnd);
            SetNextMotion(-1);
        }
    }
}

void Mudokon::Motion_53_ChantEnd()
{
    if (GetAnimation().GetIsLastFrame())
    {
        SetCurrentMotion(eMudMotions::Motion_0_Idle);
    }
}

void Mudokon::Motion_54_ToDuck()
{
    if (GetAnimation().GetIsLastFrame())
    {
        SetCurrentMotion(eMudMotions::Motion_55_Duck);
    }
}

void Mudokon::Motion_55_Duck()
{
    if (GetAnimation().GetIsLastFrame())
    {
        if (GetNextMotion() == eMudMotions::Motion_23_CrouchIdle)
        {
            SetNextMotion(-1);
            SetCurrentMotion(eMudMotions::Motion_56_DuckToCrouch);
        }
    }
}

void Mudokon::Motion_56_DuckToCrouch()
{
    if (GetAnimation().GetIsLastFrame())
    {
        SetCurrentMotion(eMudMotions::Motion_23_CrouchIdle);
    }
}

void Mudokon::Motion_57_Struggle()
{
    if (GetAnimation().GetIsLastFrame())
    {
        if (GetNextMotion() == eMudMotions::Motion_0_Idle)
        {
            SetCurrentMotion(eMudMotions::Motion_0_Idle);
        }
    }
}

void Mudokon::Motion_58_StruggleToCrouchChant()
{
    if (GetAnimation().GetIsLastFrame())
    {
        SetCurrentMotion(eMudMotions::Motion_59_CrouchChant);
    }
}

void Mudokon::Motion_59_CrouchChant()
{
    if (GetAnimation().GetIsLastFrame())
    {
        if (mNextMotion != -1)
        {
            SetNextMotion(-1);
            SetCurrentMotion(eMudMotions::Motion_60_CrouchChantToStruggle);
        }
    }

    if ((sGnFrame % 8) == 0)
    {
        const FP rndX = FP_FromInteger(30 * Math_NextRandom() / 256 - 10);
        const FP rndY = FP_FromInteger(20 * Math_NextRandom() / 256 + 10);

        New_TintChant_Particle(
            mXPos + (GetSpriteScale() * rndX),
            mYPos - (GetSpriteScale() * rndY),
            GetSpriteScale(),
            Layer::eLayer_0);
    }

    if (!SND_SsIsEos_DeInlined_477930(SeqId::eMudokonChant_12))
    {
        if (GetSpriteScale() == FP_FromDouble(0.5))
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
    if (GetAnimation().GetIsLastFrame())
    {
        SetCurrentMotion(eMudMotions::Motion_0_Idle);
    }
}

void Mudokon::Motion_61_DuckKnockback()
{
    if (GetAnimation().GetCurrentFrame() == 1)
    {
        Environment_SFX(EnvironmentSfx::eGenericMovement_9, 0, 0x7FFF, this);
    }

    CheckFloorGone();

    if (GetAnimation().GetIsLastFrame())
    {
        SetCurrentMotion(eMudMotions::Motion_55_Duck);
        SetNextMotion(-1);
    }
}

void Mudokon::Motion_62_PoisonGasDeath()
{
    if (GetAnimation().GetIsLastFrame())
    {
        mPersist = false;
        mHealth = FP_FromInteger(0);
        field_1B8_brain_state = 11;
        field_1C0_timer = sGnFrame + 90;
        SetNextMotion(-1);
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
            SetNextMotion(eMudMotions::Motion_1_WalkLoop);
            field_190 = field_18C_how_far_to_walk;
            return 2;
        }
    }
    else if (field_1BA_brain_sub_state == 2)
    {
        if (mVelX < FP_FromInteger(0))
        {
            field_190 += mVelX;
        }
        else
        {
            field_190 -= mVelX;
        }

        if (field_190 < FP_FromInteger(0))
        {
            SetCurrentMotion(eMudMotions::Motion_0_Idle);
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
        if (GetCurrentMotion() == eMudMotions::Motion_0_Idle)
        {
            SetCurrentMotion(eMudMotions::Motion_2_StandingTurn);
            SetNextMotion(eMudMotions::Motion_1_WalkLoop);
            return 1;
        }
    }
    else if (field_1BA_brain_sub_state == 1)
    {
        if (GetCurrentMotion() == eMudMotions::Motion_1_WalkLoop)
        {
            mBit2_Unknown = false;
            field_190 = field_18C_how_far_to_walk;
            return 2;
        }
    }
    else if (field_1BA_brain_sub_state == 2)
    {
        if (mVelX < FP_FromInteger(0))
        {
            field_190 += mVelX;
        }
        else
        {
            field_190 -= mVelX;
        }

        if (field_190 < FP_FromInteger(0))
        {
            SetDead(true);
            SetCurrentMotion(eMudMotions::Motion_0_Idle);
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
            if (mGiveRingWithoutPassword)
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
                SetNextMotion(eMudMotions::Motion_4_Speak);
                return 2;
            }
            break;

        case 2:
            if (!field_198_abe_must_face_mud
                || sActiveHero->GetAnimation().GetFlipX() != GetAnimation().GetFlipX())
            {
                GameSpeakEvents last_speak = {};
                if (field_128 == gEventSystem->mLastEventIndex)
                {
                    if (gEventSystem->mLastEvent == GameSpeakEvents::eNone_m1)
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
                    last_speak = gEventSystem->mLastEvent;
                    field_128 = gEventSystem->mLastEventIndex;
                }

                if (last_speak == GameSpeakEvents::eAbe_Hello_9)
                {
                    field_1C0_timer = sGnFrame + 20;
                    return 3;
                }
            }
            break;

        case 3:
            if (field_1C0_timer <= static_cast<s32>(sGnFrame))
            {
                if (mGivePassword == relive::reliveChoice::eYes)
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
                case GameSpeakEvents::eAbe_WhistleHigh_1:
                    SetNextMotion(eMudMotions::Motion_4_Speak);
                    Mudokon_SFX(MudSounds::eWhistleHigh_1, 0, 0, this);
                    break;

                case GameSpeakEvents::eAbe_WhistleLow_2:
                    SetNextMotion(eMudMotions::Motion_5_Speak);
                    Mudokon_SFX(MudSounds::eWhistleLow_2, 0, 0, this);
                    break;

                case GameSpeakEvents::eAbe_Fart_3:
                    SetNextMotion(eMudMotions::Motion_6_Speak);
                    Mudokon_SFX(MudSounds::eFart_7, 0, 300, this);
                    break;

                case GameSpeakEvents::eAbe_Laugh_4:
                    SetNextMotion(eMudMotions::Motion_3_Speak);
                    Mudokon_SFX(MudSounds::eLaugh1_8, 0, 300, this);
                    break;

                default:
                    SetNextMotion(eMudMotions::Motion_3_Speak);
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
            if (field_128 == gEventSystem->mLastEventIndex)
            {
                if (gEventSystem->mLastEvent == GameSpeakEvents::eNone_m1)
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
                last_speak = gEventSystem->mLastEvent;
                field_128 = gEventSystem->mLastEventIndex;
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
            if (field_128 == gEventSystem->mLastEventIndex)
            {
                if (gEventSystem->mLastEvent == GameSpeakEvents::eNone_m1)
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
                field_128 = gEventSystem->mLastEventIndex;
                last_speak = gEventSystem->mLastEvent;
            }

            if (last_speak != GameSpeakEvents::eNone_m1 && last_speak != GameSpeakEvents::eSameAsLast_m2)
            {
                field_13E = static_cast<s16>(gEventSystem->mLastEventIndex);
                field_118 = sGnFrame + 60;
                return 4;
            }
            break;
        }

        case 4:
        {
            GameSpeakEvents last_speak = {};
            if (field_128 == gEventSystem->mLastEventIndex)
            {
                if (gEventSystem->mLastEvent == GameSpeakEvents::eNone_m1)
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
                field_128 = gEventSystem->mLastEventIndex;
                last_speak = gEventSystem->mLastEvent;
            }


            if (last_speak != GameSpeakEvents::eNone_m1 && last_speak != GameSpeakEvents::eSameAsLast_m2)
            {
                field_118 = sGnFrame + 60;
            }

            if (static_cast<s32>(sGnFrame) <= field_118)
            {
                if (gEventSystem->MatchBuffer(field_12C, field_13C, field_13E) != GameSpeakMatch::eFullMatch_1)
                {
                    if (gEventSystem->MatchBuffer(
                            field_12C,
                            field_13C,
                            field_13E)
                        != GameSpeakMatch::eNoMatch_0) // Full or partial
                    {
                        return field_1BA_brain_sub_state;
                    }
                }
            }

            field_1A0 = gEventSystem->MatchBuffer(field_12C, field_13C, field_13E) == GameSpeakMatch::eFullMatch_1 || gVoiceCheat;
            field_1C0_timer = sGnFrame + 30;
            return 5;
        }

        case 5:
            if (static_cast<s32>(sGnFrame) > field_1C0_timer)
            {
                if (field_1A0)
                {
                    Mudokon_SFX(MudSounds::eOkay_13, 0, 300, this);
                    SetNextMotion(3);
                }
                else
                {
                    Mudokon_SFX(MudSounds::eRefuse_14, 0, 300, this);
                    SetNextMotion(6);
                }
                return 6;
            }
            break;

        case 6:
            if (GetCurrentMotion() == eMudMotions::Motion_0_Idle)
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
            SetNextMotion(eMudMotions::Motion_3_Speak);
            Mudokon_SFX(MudSounds::ePassword_9, 0, 300, this);
            field_13C = GameSpeak::sub_40FA60(field_1A4_code_converted, field_12C);
            return 1;

        case 1:
            field_1C0_timer = sGnFrame;
            return 2;

        case 2:
        {
            GameSpeakEvents last_speak = {};
            if (field_128 == gEventSystem->mLastEventIndex)
            {
                if (gEventSystem->mLastEvent == GameSpeakEvents::eNone_m1)
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
                field_128 = gEventSystem->mLastEventIndex;
                last_speak = gEventSystem->mLastEvent;
            }

            if (last_speak != GameSpeakEvents::eNone_m1 && last_speak != GameSpeakEvents::eSameAsLast_m2)
            {
                field_13E = static_cast<s16>(gEventSystem->mLastEventIndex);
                field_118 = sGnFrame + 60;
                return 3;
            }
            break;
        }

        case 3:
        {
            GameSpeakEvents last_speak = {};
            if (field_128 == gEventSystem->mLastEventIndex)
            {
                if (gEventSystem->mLastEvent == GameSpeakEvents::eNone_m1)
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
                field_128 = gEventSystem->mLastEventIndex;
                last_speak = gEventSystem->mLastEvent;
            }

            if (last_speak != GameSpeakEvents::eNone_m1 && last_speak != GameSpeakEvents::eSameAsLast_m2)
            {
                field_118 = sGnFrame + 60;
            }

            if (static_cast<s32>(sGnFrame) <= field_118)
            {
                if (gEventSystem->MatchBuffer(field_12C, field_13C, field_13E) != GameSpeakMatch::eFullMatch_1)
                {
                    if (gEventSystem->MatchBuffer(
                            field_12C,
                            field_13C,
                            field_13E)
                        != GameSpeakMatch::eNoMatch_0) // Full or partial
                    {
                        return field_1BA_brain_sub_state;
                    }
                }
            }

            field_1A0 = gEventSystem->MatchBuffer(
                            field_12C,
                            field_13C,
                            field_13E)
                         == GameSpeakMatch::eFullMatch_1
                     || gVoiceCheat;

            field_1C0_timer = sGnFrame + 30;
            return 4;
        }

        case 4:
            if (static_cast<s32>(sGnFrame) > field_1C0_timer)
            {
                if (field_1A0)
                {
                    Mudokon_SFX(MudSounds::eOkay_13, 0, 300, this);
                    SetNextMotion(eMudMotions::Motion_3_Speak);
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
                    SetNextMotion(eMudMotions::Motion_6_Speak);
                }
                return 5;
            }
            break;

        case 5:
            if (GetCurrentMotion() == eMudMotions::Motion_0_Idle)
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
    else if (GetCurrentMotion() == eMudMotions::Motion_0_Idle)
    {
        SetCurrentMotion(eMudMotions::Motion_13_LiftGrabBegin);
        SetNextMotion(eMudMotions::Motion_12_LiftUse);
        field_194_pLiftPoint = nullptr;
        for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
        {
            IBaseAliveGameObject* pObj = gBaseAliveGameObjects->ItemAt(i);
            if (!pObj)
            {
                break;
            }

            if (pObj->Type() == ReliveTypes::eLiftPoint)
            {
                auto pLiftPoint = static_cast<LiftPoint*>(pObj);
                if (field_110_lift_switch_id == pLiftPoint->mLiftPointId)
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

    SetNextMotion(eMudMotions::Motion_15_LeverUse);

    FP directedGridSize = {};
    if (GetAnimation().GetFlipX())
    {
        directedGridSize = -ScaleToGridSize(GetSpriteScale());
    }
    else
    {
        directedGridSize = ScaleToGridSize(GetSpriteScale());
    }

    auto pSwitch = static_cast<Lever*>(FindObjectOfType(ReliveTypes::eLever, mXPos + directedGridSize, mYPos - ScaleToGridSize(GetSpriteScale())));
    if (pSwitch)
    {
        pSwitch->VPull(mXPos < pSwitch->mXPos);
    }

    return 1;
}

s16 Mudokon::Brain_7_GiveRings()
{
    switch (field_1BA_brain_sub_state)
    {
        case 0:
            if (sActiveHero->field_168_ring_pulse_timer <= 0)
            {
                SetNextMotion(eMudMotions::Motion_52_Chant);
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

                const PSX_RECT heroRect = sActiveHero->VGetBoundingRect();

                auto pAbeRing = relive_new AbilityRing(
                    FP_FromInteger((heroRect.w + heroRect.x) / 2),
                    FP_FromInteger((heroRect.h + heroRect.y) / 2),
                    RingTypes::eExplosive_Give_3);
                if (pAbeRing)
                {
                    pAbeRing->SetTarget(sActiveHero);
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
                    sActiveHero->field_168_ring_pulse_timer = field_1AA_ring_timeout + sGnFrame;
                }
                else
                {
                    sActiveHero->field_168_ring_pulse_timer = sGnFrame + 200000;
                }

                SetNextMotion(eMudMotions::Motion_0_Idle);
                sActiveHero->field_16C_bHaveShrykull = 0;
                return 4;
            }
            break;

        case 4:
            if (sActiveHero->field_168_ring_pulse_timer <= 0)
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
            SetCurrentMotion(eMudMotions::Motion_16_StandScrubLoop);
            SetNextMotion(-1);
            field_1C0_timer = (Math_NextRandom() % 64) + sGnFrame + 35;
            return 1;

        case 1:
            if (last_speak == GameSpeakEvents::eAbe_Hello_9 && sAlertedMudCount_507B90 == 0 && IAmNearestToAbe())
            {
                AddAlerted();
                SetNextMotion(eMudMotions::Motion_21_StandScrubToIdle);
                field_1C0_timer = sGnFrame + 15;
                return 3;
            }

            if (static_cast<s32>(sGnFrame) > field_1C0_timer
                && GetCurrentMotion() == eMudMotions::Motion_16_StandScrubLoop)
            {
                SetNextMotion(eMudMotions::Motion_17_StandScrubLoopToPause);
            }

            if (GetCurrentMotion() == eMudMotions::Motion_19_StandScrubPause)
            {
                field_1C0_timer = (Math_NextRandom() % 64) + sGnFrame + 35;
                return 2;
            }
            return field_1BA_brain_sub_state;

        case 2:
            if (last_speak == GameSpeakEvents::eAbe_Hello_9 && sAlertedMudCount_507B90 == 0 && IAmNearestToAbe())
            {
                AddAlerted();
                SetNextMotion(eMudMotions::Motion_21_StandScrubToIdle);
                field_1C0_timer = sGnFrame + 15;
                return 3;
            }

            if (static_cast<s32>(sGnFrame) > field_1C0_timer)
            {
                SetNextMotion(eMudMotions::Motion_18_StandScrubPauseToLoop);
                field_1C0_timer = (Math_NextRandom() % 64) + sGnFrame + 35;
                return 1;
            }
            return field_1BA_brain_sub_state;

        case 3:
            if (static_cast<s32>(sGnFrame) <= field_1C0_timer || GetCurrentMotion() != eMudMotions::Motion_0_Idle)
            {
                if (mNextMotion != -1 || GetCurrentMotion() != eMudMotions::Motion_16_StandScrubLoop)
                {
                    return field_1BA_brain_sub_state;
                }
                SetNextMotion(eMudMotions::Motion_21_StandScrubToIdle);
                return field_1BA_brain_sub_state;
            }
            field_1B8_brain_state = 10;
            return 0;

        case 4:
            switch (GetCurrentMotion())
            {
                case eMudMotions::Motion_0_Idle:
                    SetNextMotion(eMudMotions::Motion_23_CrouchIdle);
                    return field_1BA_brain_sub_state;

                case eMudMotions::Motion_16_StandScrubLoop:
                case eMudMotions::Motion_19_StandScrubPause:
                    SetNextMotion(eMudMotions::Motion_0_Idle);
                    return field_1BA_brain_sub_state;

                case eMudMotions::Motion_23_CrouchIdle:
                    SetNextMotion(eMudMotions::Motion_55_Duck);
                    field_1C0_timer = sGnFrame + 60;
                    return 5;

                default:
                    return field_1BA_brain_sub_state;
            }
            break;

        case 5:
            if (static_cast<s32>(sGnFrame) > field_1C0_timer)
            {
                SetNextMotion(eMudMotions::Motion_23_CrouchIdle);
                return 6;
            }
            return field_1BA_brain_sub_state;

        case 6:
            if (GetCurrentMotion() == eMudMotions::Motion_0_Idle)
            {
                SetNextMotion(eMudMotions::Motion_16_StandScrubLoop);
            }
            else
            {
                if (GetCurrentMotion() == eMudMotions::Motion_16_StandScrubLoop)
                {
                    return 0;
                }

                if (GetCurrentMotion() == eMudMotions::Motion_23_CrouchIdle)
                {
                    SetNextMotion(eMudMotions::Motion_0_Idle);
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
        SetNextMotion(eMudMotions::Motion_55_Duck);
        field_1C0_timer = sGnFrame + 60;
        return 5;
    }

    switch (field_1BA_brain_sub_state)
    {
        case 0:
            SetCurrentMotion(eMudMotions::Motion_22_CrouchScrub);
            SetNextMotion(-1);
            field_1C0_timer = (Math_NextRandom() % 64) + 15;
            field_114 = (Math_NextRandom() % 64) + 240;
            return 2;

        case 1:
            if (last_speak == GameSpeakEvents::eAbe_Hello_9 && sAlertedMudCount_507B90 == 0 && IAmNearestToAbe())
            {
                AddAlerted();
                field_1C0_timer = sGnFrame + 15;
                return 3;
            }

            if (GetCurrentMotion() == eMudMotions::Motion_23_CrouchIdle && static_cast<s32>(sGnFrame) > field_114)
            {
                SetNextMotion(eMudMotions::Motion_24_CrouchTurn);
                field_114 = (Math_NextRandom() % 64) + sGnFrame + 240;
            }

            if (GetCurrentMotion() == eMudMotions::Motion_23_CrouchIdle && static_cast<s32>(sGnFrame) > field_1C0_timer)
            {
                SetNextMotion(eMudMotions::Motion_22_CrouchScrub);
                field_1C0_timer = (Math_NextRandom() % 64) + sGnFrame + 35;
            }

            if (GetCurrentMotion() != eMudMotions::Motion_22_CrouchScrub)
            {
                return field_1BA_brain_sub_state;
            }
            return 2;

        case 2:
            if (last_speak != GameSpeakEvents::eAbe_Hello_9 || sAlertedMudCount_507B90 || !IAmNearestToAbe())
            {
                if (GetCurrentMotion() == eMudMotions::Motion_23_CrouchIdle)
                {
                    if (static_cast<s32>(sGnFrame) > field_1C0_timer)
                    {
                        field_1C0_timer = (Math_NextRandom() % 64) + sGnFrame + 15;
                        return 1;
                    }
                    SetCurrentMotion(eMudMotions::Motion_22_CrouchScrub);
                }

                if (IsEventInRange(
                        kEventSpeaking,
                        mXPos,
                        mYPos,
                        GetSpriteScale()))
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
                if (GetCurrentMotion() == eMudMotions::Motion_23_CrouchIdle)
                {
                    SetNextMotion(eMudMotions::Motion_26_CrouchToStand);
                }

                if (GetCurrentMotion() == eMudMotions::Motion_0_Idle)
                {
                    field_1BA_brain_sub_state = 0;
                    field_1B8_brain_state = 10;
                    return 0;
                }

                return field_1BA_brain_sub_state;
            }
            break;

        case 4:
            if (last_speak != GameSpeakEvents::eAbe_Hello_9 || sAlertedMudCount_507B90 || !IAmNearestToAbe())
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
                SetNextMotion(eMudMotions::Motion_23_CrouchIdle);

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
                SetNextMotion(eMudMotions::Motion_61_DuckKnockback);
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
        SetNextMotion(eMudMotions::Motion_0_Idle);
        return 0;
    }

    switch (field_1BA_brain_sub_state)
    {
        case 0:
            mPersist = true;
            SetNextMotion(-1);
            SetCurrentMotion(eMudMotions::Motion_0_Idle);
            field_1B0 = -1;
            return 1;

        case 1:
            if (GetCurrentMotion() != eMudMotions::Motion_0_Idle)
            {
                return field_1BA_brain_sub_state;
            }

            if (!VIsFacingMe(sActiveHero))
            {
                SetNextMotion(eMudMotions::Motion_2_StandingTurn);
                return field_1BA_brain_sub_state;
            }
            Mudokon_SFX(MudSounds::eHello_3, 0, field_124_voice_pitch, this);
            SetNextMotion(eMudMotions::Motion_4_Speak);
            return 6;

        case 2:
            if (GetCurrentMotion() == eMudMotions::Motion_0_Idle)
            {
                SetNextMotion(eMudMotions::Motion_1_WalkLoop);
                return field_1BA_brain_sub_state;
            }

            if (GetCurrentMotion() == eMudMotions::Motion_1_WalkLoop)
            {
                SetNextMotion(eMudMotions::Motion_0_Idle);
                return 3;
            }
            return field_1BA_brain_sub_state;

        case 3:
        {
            FP gridSizeDirected = {};
            if (GetAnimation().GetFlipX())
            {
                gridSizeDirected = -ScaleToGridSize(GetSpriteScale());
            }
            else
            {
                gridSizeDirected = ScaleToGridSize(GetSpriteScale());
            }
            const s16 bHitWall = WallHit(GetSpriteScale() * FP_FromInteger(50), gridSizeDirected);
            const s16 bEndOfLine = Check_IsOnEndOfLine(GetAnimation().GetFlipX(), 1);

            if (GetCurrentMotion() != eMudMotions::Motion_0_Idle)
            {
                const s16 oldState = field_1BA_brain_sub_state;
                SetNextMotion(eMudMotions::Motion_0_Idle);
                field_1B4_idle_time = 0;
                return oldState;
            }

            field_1B4_idle_time++;

            if (field_1B4_idle_time <= 150 || BetweenCameras_418500() != BetweenCamPos::None_0)
            {
                if (!VIsFacingMe(sActiveHero))
                {
                    SetNextMotion(eMudMotions::Motion_2_StandingTurn);
                    return field_1BA_brain_sub_state;
                }

                if (VIsObjNearby(ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(2), sActiveHero)
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
                        if (field_128 == gEventSystem->mLastEventIndex)
                        {
                            if (gEventSystem->mLastEvent == GameSpeakEvents::eNone_m1)
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
                            last_speak = gEventSystem->mLastEvent;
                            field_128 = gEventSystem->mLastEventIndex;
                        }
                    }

                    if (EventGet(kEventMudokonComfort)
                        && gMap.Is_Point_In_Current_Camera(
                            mCurrentLevel,
                            mCurrentPath,
                            mXPos,
                            mYPos,
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
                        case GameSpeakEvents::eAbe_WhistleHigh_1:
                            return 17;

                        case GameSpeakEvents::eAbe_WhistleLow_2:
                            return 18;

                        case GameSpeakEvents::eAbe_Fart_3:
                            field_1C0_timer = sGnFrame + 30;
                            return 13;

                        case GameSpeakEvents::eAbe_Laugh_4:
                            return 16;

                        case GameSpeakEvents::eAbe_Hello_9:
                            return 11;

                        case GameSpeakEvents::eAbe_FollowMe_10:
                            if (bHitWall)
                            {
                                return 9;
                            }
                            field_1B0 = 2;
                            return 10;

                        case GameSpeakEvents::eAbe_Anger_11:
                            return 14;

                        case GameSpeakEvents::eAbe_Wait_12:
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
                        SetNextMotion(eMudMotions::Motion_35_SneakLoop);
                    }
                    else
                    {
                        SetNextMotion(eMudMotions::Motion_1_WalkLoop);
                    }
                    return 4;
                }
            }
            else
            {
                if (!field_148_res_array.res[12])
                {
                    SetNextMotion(eMudMotions::Motion_23_CrouchIdle);
                }
                else
                {
                    SetNextMotion(eMudMotions::Motion_59_CrouchChant);
                }
                return 19;
            }
            break;
        }

        case 4:
        {
            if (GetCurrentMotion() == eMudMotions::Motion_0_Idle)
            {
                return 3;
            }

            if (GetCurrentMotion() != eMudMotions::Motion_1_WalkLoop && GetCurrentMotion() != eMudMotions::Motion_35_SneakLoop)
            {
                return field_1BA_brain_sub_state;
            }

            FP gridSizeDirected = {};
            if (GetAnimation().GetFlipX())
            {
                gridSizeDirected = -ScaleToGridSize(GetSpriteScale());
            }
            else
            {
                gridSizeDirected = ScaleToGridSize(GetSpriteScale());
            }

            const s16 bHitWall = WallHit(GetSpriteScale() * FP_FromInteger(50), gridSizeDirected);

            if (Check_IsOnEndOfLine(GetAnimation().GetFlipX(), 1) || bHitWall)
            {
                SetNextMotion(eMudMotions::Motion_0_Idle);
                return 3;
            }

            if (!VIsFacingMe(sActiveHero))
            {
                SetNextMotion(eMudMotions::Motion_0_Idle);
                return 3;
            }

            if (VIsObjNearby(ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(2), sActiveHero))
            {
                SetNextMotion(eMudMotions::Motion_0_Idle);
                return 3;
            }

            GameSpeakEvents last_speak = {};
            if (field_128 == gEventSystem->mLastEventIndex)
            {
                if (gEventSystem->mLastEvent == GameSpeakEvents::eNone_m1)
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
                last_speak = gEventSystem->mLastEvent;
                field_128 = gEventSystem->mLastEventIndex;
            }

            if (last_speak == GameSpeakEvents::eAbe_Wait_12
                && BetweenCameras_418500() == BetweenCamPos::None_0)
            {
                SetNextMotion(eMudMotions::Motion_0_Idle);
                return 8;
            }

            if (sActiveHero->mCurrentMotion == eAbeMotions::Motion_35_RunLoop
                || sActiveHero->mCurrentMotion == eAbeMotions::Motion_25_RollLoop)
            {
                SetNextMotion(eMudMotions::Motion_29_RunLoop);
                return 5;
            }

            if (sActiveHero->mCurrentMotion == eAbeMotions::Motion_42_SneakLoop
                && GetCurrentMotion() == eMudMotions::Motion_1_WalkLoop)
            {
                SetNextMotion(eMudMotions::Motion_35_SneakLoop);
            }

            if (sActiveHero->mCurrentMotion == eAbeMotions::Motion_1_WalkLoop && GetCurrentMotion() == eMudMotions::Motion_35_SneakLoop)
            {
                SetNextMotion(eMudMotions::Motion_1_WalkLoop);
            }

            return field_1BA_brain_sub_state;
        }

        case 5:
        {
            if (GetCurrentMotion() == eMudMotions::Motion_0_Idle)
            {
                return 3;
            }

            SetNextMotion(eMudMotions::Motion_29_RunLoop);
            if (GetCurrentMotion() != eMudMotions::Motion_29_RunLoop)
            {
                return field_1BA_brain_sub_state;
            }

            FP gridSizeDirected = {};
            if (GetAnimation().GetFlipX())
            {
                gridSizeDirected = -ScaleToGridSize(GetSpriteScale());
            }
            else
            {
                gridSizeDirected = ScaleToGridSize(GetSpriteScale());
            }

            const s16 bHitWall = WallHit(GetSpriteScale() * FP_FromInteger(50), gridSizeDirected * FP_FromInteger(3));
            if (Check_IsOnEndOfLine(GetAnimation().GetFlipX() ? 1 : 0, 4)
                || bHitWall)
            {
                SetNextMotion(eMudMotions::Motion_0_Idle);
                return 3;
            }

            switch (sActiveHero->mCurrentMotion)
            {
                case eAbeMotions::Motion_1_WalkLoop:
                    SetNextMotion(eMudMotions::Motion_1_WalkLoop);
                    return 4;

                case eAbeMotions::Motion_70_Knockback:
                case eAbeMotions::Motion_27_RunSlideStop:
                case eAbeMotions::Motion_0_Idle:
                    //[[fallthrough]];
                case eAbeMotions::Motion_19_CrouchIdle:
                    if (sActiveHero->mCurrentMotion == eAbeMotions::Motion_70_Knockback)
                    {
                        Mudokon_SFX(MudSounds::eLaugh1_8, 0, field_124_voice_pitch, this);
                    }

                    if (VIsObjNearby(ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(2), sActiveHero))
                    {
                        SetNextMotion(eMudMotions::Motion_0_Idle);
                        return 3;
                    }
                    else
                    {
                        SetNextMotion(eMudMotions::Motion_1_WalkLoop);
                        return 4;
                    }
                    break;

                default:
                    if (!VIsFacingMe(sActiveHero))
                    {
                        SetNextMotion(eMudMotions::Motion_33_RunSlideTurn);
                        return 7;
                    }
                    return field_1BA_brain_sub_state;
            }
            break;
        }

        case 6:
            if (GetCurrentMotion() != eMudMotions::Motion_0_Idle)
            {
                field_1B4_idle_time = 0;
                return field_1BA_brain_sub_state;
            }

            field_1B4_idle_time++;

            if (field_1B4_idle_time <= 150
                && gMap.Is_Point_In_Current_Camera(
                    mCurrentLevel,
                    mCurrentPath,
                    mXPos,
                    mYPos,
                    1)
                && sAlertedMudCount_507B90 <= 1)
            {
                if (!VIsFacingMe(sActiveHero))
                {
                    SetNextMotion(eMudMotions::Motion_2_StandingTurn);
                    return field_1BA_brain_sub_state;
                }

                if (EventGet(kEventMudokonComfort)
                    && gMap.Is_Point_In_Current_Camera(
                        mCurrentLevel,
                        mCurrentPath,
                        mXPos,
                        mYPos,
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
                    case GameSpeakEvents::eAbe_WhistleHigh_1:
                        return 17;

                    case GameSpeakEvents::eAbe_WhistleLow_2:
                        return 18;

                    case GameSpeakEvents::eAbe_Fart_3:
                        field_1C0_timer = sGnFrame + 30;
                        return 13;

                    case GameSpeakEvents::eAbe_Laugh_4:
                        return 16;

                    case GameSpeakEvents::eAbe_Hello_9:
                        return 11;

                    case GameSpeakEvents::eAbe_FollowMe_10:
                        field_1B0 = 3;
                        return 10;

                    case GameSpeakEvents::eAbe_Anger_11:
                        return 14;

                    case GameSpeakEvents::eAbe_Wait_12:
                        return 15;

                    default:
                        return field_1BA_brain_sub_state;
                }
            }
            else
            {
                if (field_148_res_array.res[12])
                {
                    SetNextMotion(eMudMotions::Motion_59_CrouchChant);
                }
                else
                {
                    SetNextMotion(eMudMotions::Motion_23_CrouchIdle);
                }
            }
            return 19;

        case 7:
            if (GetCurrentMotion() == eMudMotions::Motion_29_RunLoop)
            {
                return 5;
            }
            return field_1BA_brain_sub_state;

        case 8:
            if (GetCurrentMotion() == eMudMotions::Motion_0_Idle)
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
                SetNextMotion(eMudMotions::Motion_6_Speak);
                return 3;
            }
            return field_1BA_brain_sub_state;

        case 10:
            if (static_cast<s32>(sGnFrame) > field_1C0_timer)
            {
                Mudokon_SFX(MudSounds::eOkay_13, 0, field_124_voice_pitch, this);
                SetNextMotion(eMudMotions::Motion_3_Speak);
                return field_1B0;
            }
            return field_1BA_brain_sub_state;

        case 11:
            if (static_cast<s32>(sGnFrame) > field_1C0_timer)
            {
                Mudokon_SFX(MudSounds::eHello_3, 0, field_124_voice_pitch, this);
                SetNextMotion(eMudMotions::Motion_4_Speak);
                return field_1B0;
            }
            return field_1BA_brain_sub_state;

        case 12:
            if (static_cast<s32>(sGnFrame) > field_1C0_timer)
            {
                Mudokon_SFX(MudSounds::eLaugh1_8, 0, field_124_voice_pitch, this);
                SetNextMotion(eMudMotions::Motion_4_Speak);
                return field_1B0;
            }
            return field_1BA_brain_sub_state;

        case 13:
            if (static_cast<s32>(sGnFrame) > field_1C0_timer)
            {
                Mudokon_SFX(MudSounds::eLaugh2_11, 0, field_124_voice_pitch, this);
                SetNextMotion(eMudMotions::Motion_6_Speak);
                return field_1B0;
            }
            return field_1BA_brain_sub_state;


        case 14:
            if (static_cast<s32>(sGnFrame) > field_1C0_timer)
            {
                Mudokon_SFX(MudSounds::eDunno_15, 0, field_124_voice_pitch, this);
                SetNextMotion(eMudMotions::Motion_6_Speak);
                return field_1B0;
            }
            return field_1BA_brain_sub_state;

        case 15:
            if (static_cast<s32>(sGnFrame) > field_1C0_timer)
            {
                Mudokon_SFX(MudSounds::eAngry_5, 0, field_124_voice_pitch, this);
                SetNextMotion(eMudMotions::Motion_6_Speak);
                return field_1B0;
            }
            return field_1BA_brain_sub_state;

        case 16:
            if (static_cast<s32>(sGnFrame) > field_1C0_timer)
            {
                Mudokon_SFX(MudSounds::eFart_7, 0, field_124_voice_pitch, this);
                SetNextMotion(eMudMotions::Motion_3_Speak);
                if (!gEnableFartGasCheat)
                {
                    return field_1B0;
                }

                if (GetAnimation().GetFlipX())
                {
                    const FP fart_scale = (FP_FromDouble(0.5) * GetSpriteScale());
                    const FP fart_y = mYPos - (FP_FromInteger(24) * GetSpriteScale());
                    const FP fart_x = (FP_FromInteger(12) * GetSpriteScale()) + mXPos;
                    New_Smoke_Particles(fart_x, fart_y, fart_scale, 3, RGB16{32, 128, 32});
                }
                else
                {
                    const FP fart_scale = (FP_FromDouble(0.5) * GetSpriteScale());
                    const FP fart_y = mYPos - (FP_FromInteger(24) * GetSpriteScale());
                    const FP fart_x = mXPos - (FP_FromInteger(12) * GetSpriteScale());
                    New_Smoke_Particles(fart_x, fart_y, fart_scale, 3, RGB16{32, 128, 32});
                }
                return field_1B0;
            }
            break;

        case 17:
            if (static_cast<s32>(sGnFrame) > field_1C0_timer)
            {
                Mudokon_SFX(MudSounds::eWhistleHigh_1, 0, 0, this);
                SetNextMotion(eMudMotions::Motion_4_Speak);
                return field_1B0;
            }
            break;

        case 18:
            if (static_cast<s32>(sGnFrame) > field_1C0_timer)
            {
                Mudokon_SFX(MudSounds::eWhistleLow_2, 0, 0, this);
                SetNextMotion(eMudMotions::Motion_5_Speak);
                return field_1B0;
            }
            break;

        case 19:
            if (GetCurrentMotion() != eMudMotions::Motion_23_CrouchIdle && GetCurrentMotion() != eMudMotions::Motion_59_CrouchChant)
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
        mRGB.r -= 2;
        mRGB.g -= 2;
        mRGB.b -= 2;
        SetSpriteScale(GetSpriteScale() - FP_FromDouble(0.008));
    }

    if (static_cast<s32>(sGnFrame) < field_1C0_timer - 24)
    {
        DeathSmokeEffect(true);
    }

    if (GetSpriteScale() < FP_FromInteger(0))
    {
        SetDead(true);
    }

    return 100;
}

s16 Mudokon::Brain_12_Escape()
{
    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
        return field_1BA_brain_sub_state;
    }

    BirdPortal* pPortal = field_1AC_pBirdPortal;
    if (!pPortal || pPortal->GetDead())
    {
        sMudRunningToPortalCount_507B94--;
        if (pPortal)
        {
            pPortal->mBaseGameObjectRefCount--;
        }

        mPersist = true;
        field_1AC_pBirdPortal = nullptr;
        SetNextMotion(eMudMotions::Motion_0_Idle);
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
                if (FP_Abs(pPortal->mXPos - mXPos) >= ScaleToGridSize(GetSpriteScale()))
                {
                    return 2;
                }
                return 4;
            }
            break;

        case 2:
            if (GetCurrentMotion() == eMudMotions::Motion_23_CrouchIdle)
            {
                SetNextMotion(eMudMotions::Motion_26_CrouchToStand);
            }

            if (GetCurrentMotion() == eMudMotions::Motion_16_StandScrubLoop)
            {
                SetNextMotion(eMudMotions::Motion_21_StandScrubToIdle);
            }

            if (GetCurrentMotion() == eMudMotions::Motion_19_StandScrubPause)
            {
                SetNextMotion(eMudMotions::Motion_18_StandScrubPauseToLoop);
            }

            if (GetCurrentMotion() == eMudMotions::Motion_0_Idle || GetCurrentMotion() == eMudMotions::Motion_1_WalkLoop)
            {
                if (FacingBirdPortal(pPortal))
                {
                    SetNextMotion(eMudMotions::Motion_29_RunLoop);
                }
                else
                {
                    SetNextMotion(eMudMotions::Motion_2_StandingTurn);
                }
            }

            if (GetCurrentMotion() == eMudMotions::Motion_29_RunLoop)
            {
                if (!FacingBirdPortal(field_1AC_pBirdPortal))
                {
                    SetNextMotion(eMudMotions::Motion_33_RunSlideTurn);
                }
                else
                {
                    if (VIntoBirdPortal(3))
                    {
                        field_1BA_brain_sub_state = 3;
                        SetNextMotion(eMudMotions::Motion_44_RunJumpMid);
                    }
                }
            }
            break;

        case 4:
            if (GetCurrentMotion() == eMudMotions::Motion_23_CrouchIdle)
            {
                SetNextMotion(eMudMotions::Motion_26_CrouchToStand);
            }

            if (GetCurrentMotion() == eMudMotions::Motion_16_StandScrubLoop)
            {
                SetNextMotion(eMudMotions::Motion_21_StandScrubToIdle);
            }

            if (GetCurrentMotion() == eMudMotions::Motion_19_StandScrubPause)
            {
                SetNextMotion(eMudMotions::Motion_18_StandScrubPauseToLoop);
            }

            if (GetCurrentMotion() == eMudMotions::Motion_0_Idle || GetCurrentMotion() == eMudMotions::Motion_1_WalkLoop)
            {
                if ((pPortal->mEnterSide == relive::Path_BirdPortal::PortalSide::eRight) == GetAnimation().GetFlipX())
                {
                    SetNextMotion(eMudMotions::Motion_2_StandingTurn);
                }
                else
                {
                    SetNextMotion(eMudMotions::Motion_29_RunLoop);
                }
            }

            if (GetCurrentMotion() == eMudMotions::Motion_29_RunLoop)
            {
                if (FP_Abs(pPortal->mXPos - mXPos) <= ScaleToGridSize(GetSpriteScale()))
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
                Environment_SFX(EnvironmentSfx::eFallingDeathScreamHitGround_15, 0, 0x7FFF, this);
                relive_new ScreenShake(0);
                SetDead(true);
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
        SetNextMotion(eMudMotions::Motion_55_Duck);
        field_1C0_timer = sGnFrame + 60;
        return 5;
    }

    switch (field_1BA_brain_sub_state)
    {
        case 0:
            SetCurrentMotion(eMudMotions::Motion_59_CrouchChant);
            SetNextMotion(-1);
            return 1;

        case 1:
            if (field_1B6)
            {
                SetNextMotion(eMudMotions::Motion_0_Idle);
                return 3;
            }

            if (lastSpeak == GameSpeakEvents::eAbe_Hello_9 && sAlertedMudCount_507B90 == 0 && IAmNearestToAbe())
            {
                AddAlerted();
                field_1C0_timer = sGnFrame + 15;
                return 2;
            }
            break;

        case 2:
            if (static_cast<s32>(sGnFrame) > field_1C0_timer && GetCurrentMotion() == eMudMotions::Motion_0_Idle)
            {
                field_1BA_brain_sub_state = 0;
                field_1B8_brain_state = 10;
                return field_1BA_brain_sub_state;
            }
            break;

        case 3:
            if (GetCurrentMotion() == eMudMotions::Motion_57_Struggle)
            {
                field_1B6 = 0;
                field_1C0_timer = sGnFrame + 32;
                return 4;
            }
            SetNextMotion(eMudMotions::Motion_57_Struggle);
            break;

        case 4:
            if (static_cast<s32>(sGnFrame) > field_1C0_timer)
            {
                if (field_1B6 == 0)
                {
                    SetNextMotion(eMudMotions::Motion_0_Idle);
                    return 5;
                }
                field_1B6 = 0;
                field_1C0_timer = sGnFrame + 32;
            }
            break;

        case 5:
            if (GetCurrentMotion() == eMudMotions::Motion_0_Idle)
            {
                SetNextMotion(eMudMotions::Motion_59_CrouchChant);
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
    if (GetCurrentMotion() != eMudMotions::Motion_62_PoisonGasDeath)
    {
        SetNextMotion(eMudMotions::Motion_62_PoisonGasDeath);
    }
    return 0;
}

} // namespace AO
