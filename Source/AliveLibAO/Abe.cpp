#include "stdafx_ao.h"
#include "../relive_lib/Function.hpp"
#include "Abe.hpp"
#include "BellSong.hpp"
#include "../relive_lib/GameObjects/Blood.hpp"
#include "BoomMachine.hpp"
#include "Bullet.hpp"
#include "CheatController.hpp"
#include "../relive_lib/GameObjects/DeathBirdParticle.hpp"
#include "Door.hpp"
#include "../relive_lib/GameObjects/Dove.hpp"
#include "ThrowableArray.hpp"
#include "Elum.hpp"
#include "../relive_lib/GameObjects/Flash.hpp"
#include "LiftPoint.hpp"
#include "../relive_lib/Shadow.hpp"
#include "Game.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "Midi.hpp"
#include "Movie.hpp"
#include "../relive_lib/GameObjects/CircularFade.hpp"
#include "../relive_lib/GameObjects/Fade.hpp"
#include "Throwable.hpp"
#include "../relive_lib/GameObjects/OrbWhirlWind.hpp"
#include "../relive_lib/GameObjects/Particle.hpp"
#include "PossessionFlicker.hpp"
#include "PullRingRope.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "../relive_lib/GameObjects/ScreenManager.hpp"
#include "../relive_lib/GameObjects/ThrowableTotalIndicator.hpp"
#include "../relive_lib/Events.hpp"
#include "DDCheat.hpp"
#include "LiftPoint.hpp"
#include "Input.hpp"
#include "PullRingRope.hpp"
#include "../relive_lib/GameObjects/ScreenShake.hpp"
#include "Sfx.hpp"
#include "MusicController.hpp"
#include "AbilityRing.hpp"
#include "MusicTrigger.hpp"
#include "Math.hpp"
#include "Input.hpp"
#include "Grenade.hpp"
#include "../relive_lib/Collisions.hpp"
#include "BirdPortal.hpp"
#include "SaveGame.hpp"
#include "BeeSwarm.hpp"
#include "Shrykull.hpp"
#include "Lever.hpp"
#include "GameSpeak.hpp"
#include "ZBall.hpp"
#include "../relive_lib/GameObjects/Gibs.hpp"
#include "../relive_lib/Camera.hpp"
#include "AnimationCallBacks.hpp"
#include "../relive_lib/Grid.hpp"
#include "../relive_lib/Sound/Midi.hpp"
#include "../relive_lib/FatalError.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "../relive_lib/Psx.hpp"
#include "Path.hpp"
#include "../relive_lib/data_conversion/PathTlvsAO.hpp"

namespace AO {

s16 gAbeInvulnerableCheat = 0;
s16 gAbeInvisibleCheat = 0;

extern SaveData gSaveBuffer;

using TAbeMotionFunction = decltype(&Abe::Motion_0_Idle);

const TAbeMotionFunction sAbeMotionMachineTable_4C5F08[] = {
    &Abe::Motion_0_Idle,
    &Abe::Motion_1_WalkLoop,
    &Abe::Motion_2_StandingTurn,
    &Abe::Motion_3_Fall,
    &Abe::Motion_4_WalkToIdle,
    &Abe::Motion_5_MidWalkToIdle,
    &Abe::Motion_6_WalkBegin,
    &Abe::Motion_7_Speak,
    &Abe::Motion_8_Speak,
    &Abe::Motion_9_Speak,
    &Abe::Motion_10_Speak,
    &Abe::Motion_11_Speak,
    &Abe::Motion_12_Speak,
    &Abe::Motion_13_Speak,
    &Abe::Motion_14_Speak,
    &Abe::Motion_15_Null,
    &Abe::Motion_16_HoistBegin,
    &Abe::Motion_17_HoistIdle,
    &Abe::Motion_18_HoistLand,
    &Abe::Motion_19_CrouchIdle,
    &Abe::Motion_20_CrouchToStand,
    &Abe::Motion_21_StandToCrouch,
    &Abe::Motion_22_CrouchSpeak,
    &Abe::Motion_23_CrouchSpeak,
    &Abe::Motion_24_RollBegin,
    &Abe::Motion_25_RollLoop,
    &Abe::Motion_26_RollEnd,
    &Abe::Motion_27_RunSlideStop,
    &Abe::Motion_28_RunTurn,
    &Abe::Motion_29_HopBegin,
    &Abe::Motion_30_HopMid,
    &Abe::Motion_31_HopLand,
    &Abe::Motion_32_RunJumpBegin,
    &Abe::Motion_33_RunJumpMid,
    &Abe::Motion_34_RunJumpLand,
    &Abe::Motion_35_RunLoop,
    &Abe::Motion_36_DunnoBegin,
    &Abe::Motion_37_DunnoMid,
    &Abe::Motion_38_DunnoEnd,
    &Abe::Motion_39_CrouchTurn,
    &Abe::Motion_40_RunToRoll,
    &Abe::Motion_41_StandingToRun,
    &Abe::Motion_42_SneakLoop,
    &Abe::Motion_43_WalkToSneak,
    &Abe::Motion_44_SneakToWalk,
    &Abe::Motion_45_MidWalkToSneak,
    &Abe::Motion_46_MidSneakToWalk,
    &Abe::Motion_47_SneakBegin,
    &Abe::Motion_48_SneakToIdle,
    &Abe::Motion_49_MidSneakToIdle,
    &Abe::Motion_50_WalkToRun,
    &Abe::Motion_51_MidWalkToRun,
    &Abe::Motion_52_RunToWalk,
    &Abe::Motion_53_MidRunToWalk,
    &Abe::Motion_54_RunTurnToRun,
    &Abe::Motion_55_RunTurnToWalk,
    &Abe::Motion_56_RunJumpLandRun,
    &Abe::Motion_57_RunJumpLand_Walk,
    &Abe::Motion_58_ToSpeak,
    &Abe::Motion_59_DeathDropFall,
    &Abe::Motion_60_Dead,
    &Abe::Motion_61_Respawn,
    &Abe::Motion_62_LoadedSaveSpawn,
    &Abe::Motion_63_TurnToRun,
    &Abe::Motion_64_LedgeAscend,
    &Abe::Motion_65_LedgeDescend,
    &Abe::Motion_66_LedgeHang,
    &Abe::Motion_67_ToOffScreenHoist,
    &Abe::Motion_68_LedgeHangWobble,
    &Abe::Motion_69_RingRopePullHang,
    &Abe::Motion_70_Knockback,
    &Abe::Motion_71_KnockbackGetUp,
    &Abe::Motion_72_PushWall,
    &Abe::Motion_73_RollingKnockback,
    &Abe::Motion_74_JumpIntoWell,
    &Abe::Motion_75_ToInsideOfAWellLocal,
    &Abe::Motion_76_ToWellShotOut,
    &Abe::Motion_77_WellBegin,
    &Abe::Motion_78_InsideWellLocal,
    &Abe::Motion_79_WellShotOut,
    &Abe::Motion_80_ToWellBegin,
    &Abe::Motion_81_InsideWellExpress,
    &Abe::Motion_82_WellExpressShotOut,
    &Abe::Motion_83_ToWellBegin,
    &Abe::Motion_84_ToInsideWellLocal,
    &Abe::Motion_85_ToWellShotOut,
    &Abe::Motion_86_FallLandDie,
    &Abe::Motion_87_ToFall,
    &Abe::Motion_88_HandstoneBegin,
    &Abe::Motion_89_HandstoneEnd,
    &Abe::Motion_90_GrenadeMachineUse,
    &Abe::Motion_91_FallingFromGrab,
    &Abe::Motion_92_ForceDownFromHoist,
    &Abe::Motion_93_WalkOffEdge,
    &Abe::Motion_94_RunOffEdge,
    &Abe::Motion_95_SneakOffEdge,
    &Abe::Motion_96_HopToFall,
    &Abe::Motion_97_RunJumpToFall,
    &Abe::Motion_98_LandSoft,
    &Abe::Motion_99_HoistBeginLong,
    &Abe::Motion_100_RollOffLedge,
    &Abe::Motion_101_LeverUse,
    &Abe::Motion_102_ElumWalkLoop,
    &Abe::Motion_103_ElumIdle,
    &Abe::Motion_104_ElumRunSlideStop,
    &Abe::Motion_105_ElumRunTurn,
    &Abe::Motion_106_Null,
    &Abe::Motion_107_ElumHopBegin,
    &Abe::Motion_108_ElumHopMid,
    &Abe::Motion_109_ElumHopLand,
    &Abe::Motion_110_ElumRunJumpBegin,
    &Abe::Motion_111_ElumRunJumpMid,
    &Abe::Motion_112_ElumRunJumpLand,
    &Abe::Motion_113_ElumTurn,
    &Abe::Motion_114_ElumRunLoop,
    &Abe::Motion_115_ElumSpeak,
    &Abe::Motion_116_Null,
    &Abe::Motion_117_ElumWalkBegin,
    &Abe::Motion_118_ElumRunBegin,
    &Abe::Motion_119_Null,
    &Abe::Motion_120_ElumRunToWalk,
    &Abe::Motion_121_ElumMidRunToWalk,
    &Abe::Motion_122_ElumRunTurnToRun,
    &Abe::Motion_123_ElumRunTurnToWalk,
    &Abe::Motion_124_ElumWalkEnd,
    &Abe::Motion_125_ElumMidWalkEnd,
    &Abe::Motion_126_ElumBeesStruggling,
    &Abe::Motion_127_SlapBomb,
    &Abe::Motion_128_KnockForward,
    &Abe::Motion_129_RollingKnockForward,
    &Abe::Motion_130_KnockForwardGetUp,
    &Abe::Motion_131_LiftUseUp,
    &Abe::Motion_132_LiftUseDown,
    &Abe::Motion_133_LiftGrabBegin,
    &Abe::Motion_134_LiftGrabEnd,
    &Abe::Motion_135_LiftGrabIdle,
    &Abe::Motion_136_ElumMountEnd,
    &Abe::Motion_137_ElumUnmountBegin,
    &Abe::Motion_138_ElumUnmountEnd,
    &Abe::Motion_139_ElumMountBegin,
    &Abe::Motion_140_BeesStruggling,
    &Abe::Motion_141_BeesStrugglingOnLift,
    &Abe::Motion_142_RockThrowStandingHold,
    &Abe::Motion_143_RockThrowStandingThrow,
    &Abe::Motion_144_RockThrowStandingEnd,
    &Abe::Motion_145_RockThrowCrouchingHold,
    &Abe::Motion_146_RockThrowCrouchingThrow,
    &Abe::Motion_147_ShotRolling,
    &Abe::Motion_148_Shot,
    &Abe::Motion_149_PickupItem,
    &Abe::Motion_150_Chant,
    &Abe::Motion_151_ChantEnd,
    &Abe::Motion_152_ElumFallOffEdge,
    &Abe::Motion_153_ElumFall,
    &Abe::Motion_154_ElumLand,
    &Abe::Motion_155_ElumJumpToFall,
    &Abe::Motion_156_DoorEnter,
    &Abe::Motion_157_DoorExit,
    &Abe::Motion_158_ElumKnockback,
    &Abe::Motion_159_Idle_RubEyes,
    &Abe::Motion_160_Idle_Stretch_Arms,
    &Abe::Motion_161_Idle_Yawn,
    &Abe::Motion_162_ToShrykull,
    &Abe::Motion_163_ShrykullEnd,
    &Abe::Motion_164_PoisonGasDeath};


Abe* gAbe = nullptr;

const relive::SfxDefinition sSFXList_4C6638[49] = {
    {0, 3, 69, 60, -1, 1},
    {0, 3, 70, 60, -1, 1},
    {0, 3, 59, 67, -1, 1},
    {0, 3, 61, 67, -1, 1},
    {0, 3, 58, 52, -1, 1},
    {0, 3, 67, 45, -1, 1},
    {0, 3, 57, 60, -1, 1},
    {0, 3, 65, 50, -1, 1},
    {0, 3, 68, 60, -1, 1},
    {0, 3, 72, 70, -1, 1},
    {0, 4, 36, 50, -127, 0},
    {0, 4, 37, 50, -127, 0},
    {0, 4, 38, 50, -127, 0},
    {0, 4, 36, 55, 0, 127},
    {0, 4, 37, 55, 0, 127},
    {0, 4, 38, 55, 0, 127},
    {0, 4, 59, 50, -1, 1},
    {0, 4, 72, 127, -1, 1},
    {0, 4, 80, 127, -1, 1},
    {0, 11, 61, 60, -1, 0},
    {0, 11, 62, 60, -1, 1},
    {0, 11, 64, 60, -1, 1},
    {0, 11, 65, 50, -1, 1},
    {0, 11, 64, 50, -1, 1},
    {0, 11, 66, 50, -1, 1},
    {0, 23, 60, 90, 0, 0},
    {0, 23, 61, 90, 0, 0},
    {0, 23, 62, 90, 0, 0},
    {0, 23, 63, 90, 0, 0},
    {0, 23, 64, 90, 0, 0},
    {0, 0, 0, 0, 0, 0},
    {0, 58, 61, 97, 0, 0},
    {0, 58, 60, 97, 0, 0},
    {0, 59, 60, 127, 0, 0},
    {0, 59, 61, 127, 0, 0},
    {0, 59, 62, 127, 0, 0},
    {0, 59, 63, 110, 0, 0},
    {0, 60, 61, 127, 0, 0},
    {0, 60, 63, 127, 0, 0},
    {0, 61, 55, 127, 0, 0},
    {0, 61, 56, 127, 0, 0},
    {0, 61, 57, 127, 0, 0},
    {0, 61, 58, 127, 0, 0},
    {0, 61, 59, 127, 0, 0},
    {0, 61, 60, 127, 0, 0},
    {0, 61, 61, 127, 0, 0},
    {0, 0, 0, 0, 0, 0},
    {0, 65, 60, 127, 0, 0},
    {0, 65, 64, 80, 0, 0}};

bool IsAbe(BaseGameObject* pObj)
{
    if (gAbe)
    {
        return gAbe == pObj;
    }
    return false;
}

s32 Environment_SFX(EnvironmentSfx sfxId, s32 volume, s32 pitchMin, BaseAliveGameObject* pAliveObj)
{
    s16 sndIndex = 0;
    switch (sfxId)
    {
        case EnvironmentSfx::eSlideStop_0:
            sndIndex = 11;
            volume = 40;
            break;
        case EnvironmentSfx::eWalkingFootstep_1:
            sndIndex = Math_RandomRange(10, 12);
            if (volume == 0)
            {
                volume = Math_RandomRange(54, 58);
            }
            break;
        case EnvironmentSfx::eRunningFootstep_2:
            sndIndex = Math_RandomRange(13, 15);
            if (volume == 0)
            {
                volume = Math_RandomRange(66, 70);
            }
            break;
        case EnvironmentSfx::eSneakFootstep_3:
            sndIndex = 5;
            break;
        case EnvironmentSfx::eRunSlide_4:
            sndIndex = 4;
            break;
        case EnvironmentSfx::eLandingSoft_5:
        {
            s32 v4 = 0;
            s32 v5 = 0;
            if (volume > 0 || !pAliveObj || pAliveObj->GetSpriteScale() != FP_FromDouble(0.5))
            {
                v4 = SFX_SfxDefinition_Play_Mono(sSFXList_4C6638[2], volume, pitchMin, 0x7FFF);
                v5 = SFX_SfxDefinition_Play_Mono(sSFXList_4C6638[48], volume, pitchMin, 0x7FFF);
            }
            else
            {
                v4 = SFX_SfxDefinition_Play_Mono(
                    sSFXList_4C6638[2],
                    sSFXList_4C6638[2].mDefaultVolume / 2,
                    pitchMin,
                    0x7FFF);
                v5 = SFX_SfxDefinition_Play_Mono(
                    sSFXList_4C6638[48],
                    sSFXList_4C6638[18].mDefaultVolume / 2,
                    pitchMin,
                    0x7FFF);
            }
            return v5 | v4;
        }
        case EnvironmentSfx::eHitGroundSoft_6:
            sndIndex = 2;
            break;
        case EnvironmentSfx::eDeathNoise_7:
            sndIndex = 16;
            break;
        case EnvironmentSfx::eRollingNoise_8:
            sndIndex = 6;
            break;
        case EnvironmentSfx::eGenericMovement_9:
        case EnvironmentSfx::eRunJumpOrLedgeHoist_11:
        {
            s32 result = 0;
            if (pAliveObj && pAliveObj->GetSpriteScale() == FP_FromDouble(0.5))
            {
                result = SfxPlayMono(relive::SoundEffects::AbeGenericMovement, 20);
            }
            else
            {
                result = SfxPlayMono(relive::SoundEffects::AbeGenericMovement, 35);
            }
            return result;
        }
        case EnvironmentSfx::eExhaustingHoistNoise_10:
        case EnvironmentSfx::eExhaustingElumMount_16:
            sndIndex = 19;
            break;
        case EnvironmentSfx::eUnknown_12:
            sndIndex = 25;
            break;
        case EnvironmentSfx::eKnockback_13:
            if (gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarms
                || gMap.mCurrentLevel == EReliveLevelIds::eBoardRoom
                || gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarmsReturn)
            {
                sndIndex = 2;
            }
            else
            {
                sndIndex = 9;
            }
            break;
        case EnvironmentSfx::eElumHitWall_14:
            sndIndex = 17;
            break;
        case EnvironmentSfx::eFallingDeathScreamHitGround_15:
            return 0;
        case EnvironmentSfx::eMountElumSmackNoise_17:
            sndIndex = 20;
            break;
        case EnvironmentSfx::eElumGetMountedNoise_18:
            sndIndex = 21;
            break;
        case EnvironmentSfx::eAbeMountedElumNoise_19:
            sndIndex = 22;
            break;
        case EnvironmentSfx::eElumHowl_20:
            sndIndex = 23;
            break;
        case EnvironmentSfx::eElumOkay_21:
            sndIndex = 24;
            break;
        default:
            break;
    }
    if (volume == 0)
    {
        volume = sSFXList_4C6638[sndIndex].mDefaultVolume;
    }
    if (pAliveObj)
    {
        if (pAliveObj->GetSpriteScale() == FP_FromDouble(0.5))
        {
            volume *= 2;
            volume /= 3;
        }
    }
    return SFX_SfxDefinition_Play_Mono(sSFXList_4C6638[sndIndex], volume, pitchMin, 0x7FFF);
}

s32 Mudokon_SFX(MudSounds idx, s32 volume, s32 pitch, BaseAliveGameObject* pHero)
{
    if (idx == MudSounds::eLaugh1_8
        && pHero == gAbe
        && (gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarmsReturn || gMap.mCurrentLevel == EReliveLevelIds::eBoardRoom))
    {
        idx = MudSounds::eLaugh2_11;
    }
    if (idx == MudSounds::eOops_16)
    {
        if (pHero && pHero->GetSpriteScale() == FP_FromDouble(0.5))
        {
            SND_SEQ_Play(SeqId::eAbeOops_23, 1, 85, 85);
            return 0;
        }
        else
        {
            SND_SEQ_Play(SeqId::eAbeOops_23, 1, 110, 110);
            return 0;
        }
    }
    else
    {
        auto idxShort = static_cast<s16>(idx);
        if (volume == 0)
        {
            volume = sSFXList_4C6638[idxShort + 30].mDefaultVolume;
        }
        if (pHero)
        {
            if (pHero->GetSpriteScale() == FP_FromDouble(0.5))
            {
                volume *= 2;
                volume /= 3;
            }
        }
        return SFX_SfxDefinition_Play_Mono(sSFXList_4C6638[idxShort + 30], volume, pitch, pitch);
    }
}

BirdPortal* Abe::VIntoBirdPortal(s16 gridBlocks)
{
    auto pPortal = BaseAliveGameObject::VIntoBirdPortal(gridBlocks);
    if (pPortal && pPortal->mPortalType == relive::Path_BirdPortal::PortalType::eAbe)
    {
        return pPortal;
    }
    return nullptr;
}


void Abe::VOnTrapDoorOpen()
{
    PlatformBase* pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    if (pPlatform)
    {
        if (!mShrivel)
        {
            mCurrentMotion = eAbeMotions::Motion_93_WalkOffEdge;
            mbMotionChanged = true;
        }

        pPlatform->VRemove(this);
        BaseAliveGameObject_PlatformId = Guid{};
        BaseAliveGameObjectLastLineYPos = mYPos;
    }
}

const TintEntry sAbeTintTable[] = {
    {EReliveLevelIds::eStockYards, 25u, 25u, 25u},
    {EReliveLevelIds::eStockYardsReturn, 25u, 25u, 25u},
    {EReliveLevelIds::eDesert, 125u, 125u, 95u},
    {EReliveLevelIds::eDesertTemple, 120u, 120u, 90u},
    {EReliveLevelIds::eNone, 102u, 102u, 102u}};


void Abe::LoadAnimations()
{
    for (auto& animId : sAbeMotionAnimIds)
    {
        if (animId != AnimId::None)
        {
            mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(animId));
        }
    }
}

Abe::Abe()
{
    SetType(ReliveTypes::eAbe);
    SetSurviveDeathReset(true);
    Init_GameStates();

    LoadAnimations();
    Animation_Init(GetAnimRes(AnimId::Mudokon_Walk));

    GetAnimation().SetFnPtrArray(gAbe_Anim_Frame_Fns);

    PSX_Point pPoint = {};
    gMap.GetCurrentCamCoords(&pPoint);
    mXPos = FP_FromInteger(pPoint.x + XGrid_Index_To_XPos_AO(GetSpriteScale(), 4));
    mYPos = FP_FromInteger(pPoint.y + 240);
    BaseAliveGameObjectLastLineYPos = mYPos;
    mCurrentMotion = eAbeMotions::Motion_3_Fall;

    GetAnimation().SetRenderLayer(Layer::eLayer_AbeMenu_32);
    SetCanSetOffExplosives(true);

    mContinueSpriteScale = GetSpriteScale();
    mContinueLevel = gMap.mCurrentLevel;

    // Changes Abe's "default" colour depending on the level we are in
    SetTint(sAbeTintTable, gMap.mCurrentLevel);

    GetAnimation().SetSemiTrans(true);
    GetAnimation().SetBlendMode(relive::TBlendModes::eBlend_0);

    field_110_state.raw = 0;
    field_114_gnFrame = sGnFrame;
    SetCanBeesChase(true);

    // Set Abe to be the current player controlled object
    sControlledCharacter = this;

    CreateShadow();
}

Abe::~Abe()
{
    MusicController::ClearObject(this);

    SND_SEQ_Stop(SeqId::eMudokonChant1_11);

    auto pFade = sObjectIds.Find(mFadeId, ReliveTypes::eFade);
    if (pFade)
    {
        pFade->SetDead(true);
        mFadeId = Guid{};
    }

    if (field_15C_pThrowable)
    {
        field_15C_pThrowable->mBaseGameObjectRefCount--;
        field_15C_pThrowable->SetDead(true);
        field_15C_pThrowable = nullptr;
    }

    if (mPullRingRope)
    {
        mPullRingRope->mBaseGameObjectRefCount--;
        mPullRingRope->SetDead(true);
        mPullRingRope = nullptr;
    }

    auto pCircularFade = sObjectIds.Find_Impl(mCircularFadeId);
    if (pCircularFade)
    {
        pCircularFade->SetDead(true);
        mCircularFadeId = Guid{};
    }

    auto pOrbWhirlWind = sObjectIds.Find_Impl(mOrbWhirlWindId);
    if (pOrbWhirlWind)
    {
        pOrbWhirlWind->SetDead(true);
        mOrbWhirlWindId = Guid{};
    }

    if (field_18C_pObjToPossess)
    {
        field_18C_pObjToPossess->mBaseGameObjectRefCount--;
        field_18C_pObjToPossess->SetDead(true);
        field_18C_pObjToPossess = nullptr;
    }

    if (mThrowable)
    {
        mThrowable->mBaseGameObjectRefCount--;
        mThrowable->SetDead(true);
        mThrowable = nullptr;
    }

    gAbe = nullptr;

    // OG fix for going back to menu with DDCheat on and then it crashes reading a deleted pointer
    // ditto for the ending where MeatSaw/Invisible switch screen change/update funcs read it.
    sControlledCharacter = nullptr;
}

const u32 sAbe_xVel_table_4BB118[8] = {262144, 262144, 0, 4294705152, 4294705152, 4294705152, 0, 262144};
const u32 sAbe_yVel_table_4BB138[8] = {0, 4294705152, 4294705152, 4294705152, 0, 262144, 262144, 262144};

void Abe::VUpdate()
{
    if (gAbeInvulnerableCheat)
    {
        mHealth = FP_FromInteger(1);
    }

    if (!Input_IsChanting())
    {
        mBlockChanting = false;
    }

    if (gDDCheat_FlyingEnabled && sControlledCharacter == this)
    {
        VOnTrapDoorOpen();

        const FP old_y = mYPos;
        mShrivel = false;
        BaseAliveGameObjectLastLineYPos = old_y;
        mCurrentMotion = eAbeMotions::Motion_87_ToFall;
        BaseAliveGameObjectCollisionLine = nullptr;

        if (Input().IsAnyHeld(InputCommands::eUp | InputCommands::eDown | InputCommands::eLeft | InputCommands::eRight))
        {
            const s32 dir = Input().Dir();
            mVelX = FP_FromRaw(sAbe_xVel_table_4BB118[dir] * 2);
            mVelY = FP_FromRaw(sAbe_yVel_table_4BB138[dir]);

            if (Input().IsAnyHeld(InputCommands::eRun))
            {
                mVelX += FP_FromRaw(sAbe_xVel_table_4BB118[dir]);
                mVelY += FP_FromRaw(sAbe_yVel_table_4BB138[dir]);
            }

            mXPos += mVelX;
            mYPos += mVelY;

            PSX_Point mapSize = {};
            gMap.Get_map_size(&mapSize);

            if (mXPos < FP_FromInteger(0))
            {
                mXPos = FP_FromInteger(0);
            }

            if (mXPos >= FP_FromInteger(mapSize.x))
            {
                mXPos = FP_FromInteger(mapSize.x) - FP_FromInteger(1);
            }

            if (mYPos < FP_FromInteger(0))
            {
                mYPos = FP_FromInteger(0);
            }

            if (mYPos >= FP_FromInteger(mapSize.y))
            {
                mYPos = FP_FromInteger(mapSize.y) - FP_FromInteger(1);
                SetActiveCameraDelayedFromDir();
                return;
            }
        }
        else
        {
            mVelX = FP_FromInteger(0);
            mVelY = FP_FromInteger(0);
        }
        SetActiveCameraDelayedFromDir();
    }
    else
    {
        GetAnimation().SetAnimate(true);


        // Execute the current state
        eAbeMotions motion_idx = mCurrentMotion;
        const FP oldX = mXPos;
        const FP oldY = mYPos;

        InvokeMemberFunction(this, sAbeMotionMachineTable_4C5F08, motion_idx);

        if (BaseAliveGameObjectCollisionLine)
        {
            // Snap to a whole number so we are "on" the line
            mYPos = FP_NoFractional(mYPos);
        }

        if (!mShrivel)
        {
            // Did position change?
            if (oldX != mXPos || oldY != mYPos || sControlledCharacter == gElum)
            {
                // Get the TLV we are on
                BaseAliveGameObjectPathTLV = gMap.TLV_Get_At(
                    nullptr,
                    mXPos,
                    mYPos,
                    mXPos,
                    mYPos);

                VOnTlvCollision(BaseAliveGameObjectPathTLV);
            }

            if (mbGotShot)
            {
                motion_idx = field_112_prev_motion;
                ToKnockback(1, 0);
                mCurrentMotion = motion_idx;
                mNextMotion = eAbeMotions::Motion_0_Idle;
                field_112_prev_motion = eAbeMotions::Motion_0_Idle;
                mbMotionChanged = true;
                mbGotShot = false;
#if ORIGINAL_GAME_FIXES || ORIGINAL_GAME_FIX_DEATH_DELAY_AO
                mReturnToPreviousMotion = false; // OG Change - Fixes "Death Delay Glitch"
#endif
            }

            if (EventGet(kEventScreenShake))
            {
                if (IsStanding())
                {
                    ToKnockback(1, 0);
                }
            }

            if (motion_idx != mCurrentMotion || mbMotionChanged)
            {
                mbMotionChanged = false;

                if (mCurrentMotion != eAbeMotions::Motion_15_Null && !mShrivel)
                {
                    GetAnimation().Set_Animation_Data(GetAnimRes(sAbeMotionAnimIds[static_cast<u32>(mCurrentMotion)]));

                    field_12C_timer = sGnFrame;

                    if (motion_idx == eAbeMotions::Motion_15_Null || motion_idx == eAbeMotions::Motion_62_LoadedSaveSpawn)
                    {
                        GetAnimation().SetFrame(mBaseAliveGameObjectLastAnimFrame);
                    }
                }
            }
            else if (mReturnToPreviousMotion)
            {
                mCurrentMotion = mPreviousMotion;
                GetAnimation().Set_Animation_Data(GetAnimRes(sAbeMotionAnimIds[static_cast<u32>(mCurrentMotion)]));

                field_12C_timer = sGnFrame;
                GetAnimation().SetFrame(mBaseAliveGameObjectLastAnimFrame);
                mReturnToPreviousMotion = false;
            }

            if (field_11C_regen_health_timer <= static_cast<s32>(sGnFrame) && mHealth > FP_FromInteger(0))
            {
                mHealth = FP_FromInteger(1);
            }

            if (field_168_ring_pulse_timer)
            {
                if (GetAnimation().GetRender())
                {
                    if (static_cast<s32>(sGnFrame) <= field_168_ring_pulse_timer || field_16C_bHaveShrykull)
                    {
                        if (!(sGnFrame % 32))
                        {
                            const PSX_RECT bRect = VGetBoundingRect();
                            AbilityRing::Factory(
                                FP_FromInteger((bRect.w + bRect.x) / 2),
                                FP_FromInteger((bRect.h + bRect.y) / 2),
                                field_16C_bHaveShrykull != 0 ? RingTypes::eShrykull_Pulse_Small_4 : RingTypes::eExplosive_Pulse_0,
                                FP_FromInteger(1));
                            SFX_Play_Pitch(relive::SoundEffects::PossessEffect, 25, 2650);
                        }
                    }
                    else
                    {
                        field_168_ring_pulse_timer = 0;
                    }
                }
            }

            if (EventGet(kEventMudokonDead))
            {
                field_130_say = 16;
                field_134_auto_say_timer = MakeTimer(Math_RandomRange(22, 30));
                relive_new MusicTrigger(relive::Path_MusicTrigger::MusicTriggerMusicType::eDeathDrumShort, relive::Path_MusicTrigger::TriggeredBy::eTouching, 0, 90);
            }

            if (field_130_say >= 0 && static_cast<s32>(sGnFrame) >= field_134_auto_say_timer)
            {
                if (gMap.Is_Point_In_Current_Camera(
                        mCurrentLevel,
                        mCurrentPath,
                        mXPos,
                        mYPos,
                        0))
                {
                    if (mCurrentMotion != eAbeMotions::Motion_150_Chant
                        && mCurrentMotion != eAbeMotions::Motion_7_Speak
                        && mCurrentMotion != eAbeMotions::Motion_8_Speak
                        && mCurrentMotion != eAbeMotions::Motion_9_Speak
                        && mCurrentMotion != eAbeMotions::Motion_10_Speak
                        && mCurrentMotion != eAbeMotions::Motion_11_Speak
                        && mCurrentMotion != eAbeMotions::Motion_12_Speak
                        && mCurrentMotion != eAbeMotions::Motion_13_Speak
                        && mCurrentMotion != eAbeMotions::Motion_14_Speak)
                    {
                        if (mCurrentMotion == eAbeMotions::Motion_0_Idle || mCurrentMotion == eAbeMotions::Motion_15_Null)
                        {
                            mbMotionChanged = true;
                            if (field_130_say == 5 || field_130_say == 16)
                            {
                                mCurrentMotion = eAbeMotions::Motion_10_Speak;
                            }
                            else
                            {
                                mCurrentMotion = eAbeMotions::Motion_9_Speak;
                            }
                        }
                        if (field_130_say == 5)
                        {
                            EventBroadcast(kEventMudokonComfort, gAbe);
                        }
                        Mudokon_SFX(static_cast<MudSounds>(field_130_say), 0, 0, this);
                    }
                }
                field_130_say = -1;
            }

            if (mParamoniaDone && mScrabaniaDone)
            {
                if (!mGiveShrykullFromBigFace && gMap.mNextLevel == EReliveLevelIds::eLines)
                {
                    LOG_INFO("Set mGiveShrykullFromBigFace");
                    mGiveShrykullFromBigFace = true;
                }
            }

            if (mGiveShrykullFromBigFace)
            {
                static bool bLogged = false;
                if (!bLogged)
                {
                    LOG_INFO("Have mGiveShrykullFromBigFace %d", static_cast<s32>(gMap.mCurrentLevel));
                    bLogged = true;
                }

                if (!mGotShrykullFromBigFace && gMap.mCurrentLevel == EReliveLevelIds::eLines)
                {
                    LOG_INFO("Set mHaveShrykull true");
                    field_16C_bHaveShrykull = true;
                    field_168_ring_pulse_timer = MakeTimer(32000);
                    mGotShrykullFromBigFace = true;
                }
            }
        }
    }
}

void Abe::VRender(OrderingTable& ot)
{
    // When in death shrivel don't reset scale else can't shrivel into a black blob
    if (!mShrivel)
    {
        GetAnimation().SetSpriteScale(GetSpriteScale());
    }

    if (mCurrentMotion != eAbeMotions::Motion_15_Null && mCurrentMotion != eAbeMotions::Motion_78_InsideWellLocal && mCurrentMotion != eAbeMotions::Motion_81_InsideWellExpress && mCurrentMotion != eAbeMotions::Motion_84_ToInsideWellLocal && mCurrentMotion != eAbeMotions::Motion_75_ToInsideOfAWellLocal)
    {
        BaseAnimatedWithPhysicsGameObject::VRender(ot);
    }
}

void Abe::FreeElumRes()
{
    if (mCurrentMotion == eAbeMotions::Motion_136_ElumMountEnd)
    {
        mElumMountEnd = true;
    }
    else
    {
        mElumMountEnd = false;
    }

    if (mCurrentMotion == eAbeMotions::Motion_137_ElumUnmountBegin)
    {
        mElumUnmountBegin = true;
    }
    else
    {
        mElumUnmountBegin = false;
    }

    ElumFree();
}

void Abe::ToDeathDropFall()
{
    mShrivel = true;
    mCurrentMotion = eAbeMotions::Motion_59_DeathDropFall;
    field_114_gnFrame = 0;
    mHealth = FP_FromInteger(0);
    MusicController::static_PlayMusic(MusicController::MusicTypes::eType0, this, 1, 0);
}

bool Abe::IsStanding()
{
    return mCurrentMotion == eAbeMotions::Motion_0_Idle
        || mCurrentMotion == eAbeMotions::Motion_1_WalkLoop
        || mCurrentMotion == eAbeMotions::Motion_27_RunSlideStop
        || mCurrentMotion == eAbeMotions::Motion_28_RunTurn
        || mCurrentMotion == eAbeMotions::Motion_35_RunLoop
        || mCurrentMotion == eAbeMotions::Motion_2_StandingTurn
        || mCurrentMotion == eAbeMotions::Motion_36_DunnoBegin
        || mCurrentMotion == eAbeMotions::Motion_37_DunnoMid
        || mCurrentMotion == eAbeMotions::Motion_38_DunnoEnd
        || mCurrentMotion == eAbeMotions::Motion_4_WalkToIdle
        || mCurrentMotion == eAbeMotions::Motion_5_MidWalkToIdle
        || mCurrentMotion == eAbeMotions::Motion_6_WalkBegin
        || mCurrentMotion == eAbeMotions::Motion_41_StandingToRun
        || mCurrentMotion == eAbeMotions::Motion_42_SneakLoop
        || mCurrentMotion == eAbeMotions::Motion_43_WalkToSneak
        || mCurrentMotion == eAbeMotions::Motion_44_SneakToWalk
        || mCurrentMotion == eAbeMotions::Motion_45_MidWalkToSneak
        || mCurrentMotion == eAbeMotions::Motion_46_MidSneakToWalk
        || mCurrentMotion == eAbeMotions::Motion_47_SneakBegin
        || mCurrentMotion == eAbeMotions::Motion_48_SneakToIdle
        || mCurrentMotion == eAbeMotions::Motion_49_MidSneakToIdle
        || mCurrentMotion == eAbeMotions::Motion_50_WalkToRun
        || mCurrentMotion == eAbeMotions::Motion_51_MidWalkToRun
        || mCurrentMotion == eAbeMotions::Motion_52_RunToWalk
        || mCurrentMotion == eAbeMotions::Motion_53_MidRunToWalk
        || mCurrentMotion == eAbeMotions::Motion_54_RunTurnToRun
        || mCurrentMotion == eAbeMotions::Motion_55_RunTurnToWalk
        || mCurrentMotion == eAbeMotions::Motion_58_ToSpeak
        || mCurrentMotion == eAbeMotions::Motion_7_Speak
        || mCurrentMotion == eAbeMotions::Motion_8_Speak
        || mCurrentMotion == eAbeMotions::Motion_9_Speak
        || mCurrentMotion == eAbeMotions::Motion_10_Speak
        || mCurrentMotion == eAbeMotions::Motion_11_Speak
        || mCurrentMotion == eAbeMotions::Motion_12_Speak
        || mCurrentMotion == eAbeMotions::Motion_13_Speak
        || mCurrentMotion == eAbeMotions::Motion_14_Speak
        || mCurrentMotion == eAbeMotions::Motion_101_LeverUse
        || mCurrentMotion == eAbeMotions::Motion_140_BeesStruggling
        || mCurrentMotion == eAbeMotions::Motion_143_RockThrowStandingThrow
        || mCurrentMotion == eAbeMotions::Motion_142_RockThrowStandingHold
        || mCurrentMotion == eAbeMotions::Motion_144_RockThrowStandingEnd
        || mCurrentMotion == eAbeMotions::Motion_150_Chant
        || mCurrentMotion == eAbeMotions::Motion_151_ChantEnd
        || mCurrentMotion == eAbeMotions::Motion_159_Idle_RubEyes
        || mCurrentMotion == eAbeMotions::Motion_160_Idle_Stretch_Arms
        || mCurrentMotion == eAbeMotions::Motion_161_Idle_Yawn
        || mCurrentMotion == eAbeMotions::Motion_164_PoisonGasDeath;
}

void Abe::FollowLift()
{
    PlatformBase* pLift = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    if (pLift)
    {
        mVelY = pLift->mVelY;
        if (pLift->GetDead())
        {
            pLift->VOnAbeInteraction();
        }
        SetActiveCameraDelayedFromDir();
    }
}

void Abe::ExitShrykull(s16 bResetRingTimer)
{
    GetAnimation().SetAnimate(true);
    GetAnimation().SetRender(true);

    mCurrentMotion = eAbeMotions::Motion_163_ShrykullEnd;

    mbMotionChanged = true;
    field_114_gnFrame = 1;

    if (bResetRingTimer)
    {
        field_168_ring_pulse_timer = 0;
    }
}

s16 Abe::RunTryEnterWell()
{
    if (!Input().IsAnyHeld(InputCommands::eUp) || GetAnimation().GetCurrentFrame() < 4)
    {
        return 0;
    }

    auto pWellLocal = static_cast<relive::Path_WellLocal*>(gMap.VTLV_Get_At_Of_Type(
        FP_GetExponent(mXPos),
        FP_GetExponent(mYPos),
        FP_GetExponent(mXPos),
        FP_GetExponent(mYPos),
        ReliveTypes::eWellLocal));
    if (pWellLocal)
    {
        if ((pWellLocal->mScale == relive::reliveScale::eFull && GetSpriteScale() == FP_FromInteger(1)) ||
            (pWellLocal->mScale == relive::reliveScale::eHalf && GetSpriteScale() == FP_FromDouble(0.5)))
        {
            mSnapAbe = false;
            BaseAliveGameObjectPathTLV = pWellLocal;
            mCurrentMotion = eAbeMotions::Motion_77_WellBegin;
            return 1;
        }
    }

    auto pWellExpress = static_cast<relive::Path_WellExpress*>(gMap.VTLV_Get_At_Of_Type(
        FP_GetExponent(mXPos),
        FP_GetExponent(mYPos),
        FP_GetExponent(mXPos),
        FP_GetExponent(mYPos),
        ReliveTypes::eWellExpress));
    if (pWellExpress)
    {
        if ((pWellExpress->mScale == relive::reliveScale::eFull && GetSpriteScale() == FP_FromInteger(1)) ||
            (pWellExpress->mScale == relive::reliveScale::eHalf && GetSpriteScale() == FP_FromDouble(0.5)))
        {
            mSnapAbe = false;
            BaseAliveGameObjectPathTLV = pWellExpress;
            mCurrentMotion = eAbeMotions::Motion_80_430EF0;
        }
    }

    return 0;
}

void Abe::ChangeChantState(bool bKeepChanting)
{
    if (bKeepChanting)
    {
        field_110_state.chant = ChantStates::eChantingForBirdPortal_6;
    }
    else
    {
        field_110_state.chant = ChantStates::eIdleChanting_0;
    }
}

IBaseAliveGameObject* Abe::FindObjectToPossess()
{
    for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
    {
        IBaseAliveGameObject* pObj = gBaseAliveGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->GetIsBaseAliveGameObject())
        {
            if (pObj->GetCanBePossessed())
            {
                if (pObj->Type() != ReliveTypes::eSlig || (pObj->Is_In_Current_Camera() == CameraPos::eCamCurrent_0 && pObj->mHealth > FP_FromInteger(0)))
                {
                    return pObj;
                }
            }
        }
    }
    return nullptr;
}

void Abe::ToDieFinal()
{
    mShrivel = true;
    mCurrentMotion = eAbeMotions::Motion_60_Dead;
    field_114_gnFrame = 0;
    mHealth = FP_FromInteger(0);
    MusicController::static_PlayMusic(MusicController::MusicTypes::eDeathLong_14, this, 1, 0);
}

void Abe::ToKnockback(s16 bKnockbackSound, s16 bDelayedAnger)
{
    if (sControlledCharacter->Type() != ReliveTypes::eSlig || mHealth <= FP_FromInteger(0))
    {
        SND_SEQ_Stop(SeqId::eMudokonChant1_11);
        mElumMountEnd = false;
        mElumUnmountBegin = false;

        auto pOrbWhirlWind = static_cast<OrbWhirlWind*>(sObjectIds.Find_Impl(mOrbWhirlWindId));
        if (pOrbWhirlWind)
        {
            pOrbWhirlWind->ToStop();
            mOrbWhirlWindId = Guid{};
        }

        if (mVelX != FP_FromInteger(0))
        {
            mXPos -= mVelX;
        }

        MapFollowMe(1);

        mVelX = FP_FromInteger(0);
        if (mVelY < FP_FromInteger(0))
        {
            mVelY = FP_FromInteger(0);
        }

        field_114_gnFrame = MakeTimer(10);

        if (bKnockbackSound)
        {
            Mudokon_SFX(MudSounds::eKnockbackOuch_10, 0, Math_RandomRange(-127, 127), this);
            Environment_SFX(EnvironmentSfx::eKnockback_13, 0, 0x7FFF, this);
        }

        if (mRidingElum)
        {
            sControlledCharacter = this;
            BaseAliveGameObjectLastLineYPos = mYPos;
            BaseAliveGameObjectCollisionLine = nullptr;
            mYPos -= (GetSpriteScale() * FP_FromInteger(20));
            VOnTrapDoorOpen();
            FreeElumRes();
        }

        mCurrentMotion = eAbeMotions::Motion_70_Knockback;

        if (bDelayedAnger)
        {
            field_130_say = 5;
            field_134_auto_say_timer = MakeTimer(27);
        }

        if (mThrowable)
        {
            mThrowable->VToDead();
            mThrowable = nullptr;
            if (!gInfiniteGrenades)
            {
                field_19C_throwable_count++;
            }
        }
    }
}

void Abe::ToIdle()
{
    field_120_x_vel_slow_by = FP_FromInteger(0);
    mVelX = FP_FromInteger(0);
    mVelY = FP_FromInteger(0);
    field_114_gnFrame = sGnFrame;
    mCurrentMotion = eAbeMotions::Motion_0_Idle;
    field_10C_prev_held = 0;
    field_10E_released_buttons = 0;
}

void Abe::MoveForward()
{
    FollowLift();

    const FP old_x = mXPos;

    if (BaseAliveGameObjectCollisionLine)
    {
        BaseAliveGameObjectCollisionLine = BaseAliveGameObjectCollisionLine->MoveOnLine(
            &mXPos,
            &mYPos,
            mVelX);
    }

    auto pTrapdoor = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    const CollisionMask mask = GetSpriteScale() != FP_FromDouble(0.5) ? kFgFloor : kBgFloor;
    if (BaseAliveGameObjectCollisionLine && (mask.Mask() == CollisionMask(BaseAliveGameObjectCollisionLine->mLineType).Mask()))
    {
        if (pTrapdoor)
        {
            if (BaseAliveGameObjectCollisionLine->mLineType != eLineTypes ::eDynamicCollision_32 &&
                BaseAliveGameObjectCollisionLine->mLineType != eLineTypes::eBackgroundDynamicCollision_36)
            {
                pTrapdoor->VRemove(this);
                BaseAliveGameObject_PlatformId = Guid{};
            }
        }
        else if (BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eDynamicCollision_32 ||
                BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eBackgroundDynamicCollision_36)
        {
            PSX_RECT bRect = VGetBoundingRect();
            bRect.y += 5;
            bRect.h += 5;
            bRect.w += 5; // TODO: Seems wrong - same in AE

            OnCollisionWith(
                {bRect.x, bRect.y},
                {bRect.w, bRect.h},
                gPlatformsArray);
        }
    }
    else
    {
        BaseAliveGameObjectCollisionLine = nullptr;

        if (pTrapdoor)
        {
            pTrapdoor->VRemove(this);
            BaseAliveGameObject_PlatformId = Guid{};
        }

        field_10C_prev_held = 0;
        switch (mCurrentMotion)
        {
            case eAbeMotions::Motion_1_WalkLoop:
            case eAbeMotions::Motion_6_WalkBegin:
            case eAbeMotions::Motion_4_WalkToIdle:
            case eAbeMotions::Motion_5_MidWalkToIdle:
                mCurrentMotion = eAbeMotions::Motion_93_WalkOffEdge;
                break;

            case eAbeMotions::Motion_35_RunLoop:
            case eAbeMotions::Motion_41_StandingToRun:
                mCurrentMotion = eAbeMotions::Motion_94_RunOffEdge;
                break;

            case eAbeMotions::Motion_40_RunToRoll:
            case eAbeMotions::Motion_24_RollBegin:
            case eAbeMotions::Motion_25_RollLoop:
            case eAbeMotions::Motion_26_RollEnd:
                mCurrentMotion = eAbeMotions::Motion_100_RollOffLedge;
                break;

            default:
                mCurrentMotion = eAbeMotions::Motion_95_SneakOffEdge;
                break;
        }

        mXPos = old_x + mVelX;
        BaseAliveGameObjectLastLineYPos = mYPos;

        // TODO: OG bug, dead code due to switch default case ?
        if (mCurrentMotion == eAbeMotions::Motion_70_Knockback || mCurrentMotion == eAbeMotions::Motion_128_KnockForward)
        {
            field_120_x_vel_slow_by = FP_FromDouble(0.67); // TODO: Check
        }
    }
}

void Abe::ElumFree()
{
    if (mElumMountEnd)
    {
        if (gElum->mCurrentMotion != eElumMotions::Motion_1_Idle)
        {
            gElum->Vsub_416120();
        }
    }

    if (mElumUnmountBegin)
    {
        if (gElum->mCurrentMotion != eElumMotions::Motion_1_Idle)
        {
            gElum->Vsub_416120();
        }
    }
}

eAbeMotions Abe::DoGameSpeak(u16 input)
{
    if (Input_IsChanting())
    {
        field_114_gnFrame = MakeTimer(90);
        SND_SEQ_PlaySeq(SeqId::eMudokonChant1_11, 0, 1);
        field_110_state.chant = ChantStates::eIdleChanting_0;
        return eAbeMotions::Motion_150_Chant;
    }

#if ORIGINAL_PS1_BEHAVIOR // OG Change - Faster check for GameSpeak
    const bool leftGameSpeakPressed = (input & eLeftGameSpeak);
    const bool rightGameSpeakPressed = (input & eRightGameSpeak);
#else
    const bool leftGameSpeakPressed = Input().IsAnyPressed(InputCommands::eLeftGameSpeak);
    const bool rightGameSpeakPressed = Input().IsAnyPressed(InputCommands::eRightGameSpeak);
#endif

    if (leftGameSpeakPressed)
    {
        if (input & InputCommands::eGameSpeak2)
        {
            gEventSystem->PushEvent(GameSpeakEvents::eAbe_FollowMe);
            Mudokon_SFX(MudSounds::eFollowMe_4, 0, 0, this);
            if (mCurrentMotion == eAbeMotions::Motion_14_Speak)
            {
                mbMotionChanged = true;
            }
            return eAbeMotions::Motion_14_Speak;
        }
        if (input & InputCommands::eGameSpeak4)
        {
            gEventSystem->PushEvent(GameSpeakEvents::eAbe_Wait);
            Mudokon_SFX(MudSounds::eWait_6, 0, 0, this);
            if (mCurrentMotion == eAbeMotions::Motion_14_Speak)
            {
                mbMotionChanged = true;
            }
            return eAbeMotions::Motion_13_Speak;
        }
        if (input & InputCommands::eGameSpeak1)
        {
            gEventSystem->PushEvent(GameSpeakEvents::eAbe_Hello);
            Mudokon_SFX(MudSounds::eHello_3, 0, 0, this);
            if (mCurrentMotion == eAbeMotions::Motion_9_Speak)
            {
                mbMotionChanged = true;
            }
            return eAbeMotions::Motion_9_Speak;
        }
        if (input & InputCommands::eGameSpeak3)
        {
            gEventSystem->PushEvent(GameSpeakEvents::eAbe_Anger);
            Mudokon_SFX(MudSounds::eAngry_5, 0, 0, this);
            if (mCurrentMotion == eAbeMotions::Motion_10_Speak)
            {
                mbMotionChanged = true;
            }
            return eAbeMotions::Motion_10_Speak;
        }
    }

    if (rightGameSpeakPressed)
    {
        if (input & InputCommands::eGameSpeak6)
        {
            gEventSystem->PushEvent(GameSpeakEvents::eAbe_WhistleHigh);
            Mudokon_SFX(MudSounds::eWhistleHigh_1, 0, 0, this);
            if (mCurrentMotion == eAbeMotions::Motion_9_Speak)
            {
                mbMotionChanged = true;
            }
            return eAbeMotions::Motion_9_Speak;
        }
        if (input & InputCommands::eGameSpeak5)
        {
            gEventSystem->PushEvent(GameSpeakEvents::eAbe_WhistleLow);
            Mudokon_SFX(MudSounds::eWhistleLow_2, 0, 0, this);
            if (mCurrentMotion == eAbeMotions::Motion_8_Speak)
            {
                mbMotionChanged = true;
            }
            return eAbeMotions::Motion_8_Speak;
        }
        if (input & InputCommands::eGameSpeak8)
        {
            gEventSystem->PushEvent(GameSpeakEvents::eAbe_Laugh);
            Mudokon_SFX(MudSounds::eLaugh1_8, 0, 0, this);
            if (mCurrentMotion == eAbeMotions::Motion_12_Speak)
            {
                mbMotionChanged = true;
            }
            return eAbeMotions::Motion_12_Speak;
        }
        if (input & InputCommands::eGameSpeak7)
        {
            gEventSystem->PushEvent(GameSpeakEvents::eAbe_Fart);
            Mudokon_SFX(MudSounds::eFart_7, 0, 0, this);
            if (gEnableFartGasCheat)
            {
                FP xPos = mXPos;
                if (GetAnimation().GetFlipX())
                {
                    xPos += (FP_FromInteger(12) * GetSpriteScale());
                }
                else
                {
                    xPos -= (FP_FromInteger(12) * GetSpriteScale());
                }
                New_Smoke_Particles(
                    xPos,
                    mYPos - (FP_FromInteger(24) * GetSpriteScale()),
                    FP_FromDouble(0.5) * GetSpriteScale(),
                    3,
                    RGB16{ 32, 128, 32 });
            }
            field_130_say = 8;
            field_134_auto_say_timer = MakeTimer(15);
            if (mCurrentMotion == eAbeMotions::Motion_10_Speak)
            {
                mbMotionChanged = true;
            }
            return eAbeMotions::Motion_10_Speak;
        }
    }
    return eAbeMotions::None_m1;
}

void Abe::SyncToElum(s16 elumMotion)
{
    if (mCurrentMotion != eAbeMotions::Motion_137_ElumUnmountBegin && mCurrentMotion != eAbeMotions::Motion_138_ElumUnmountEnd)
    {
        switch (static_cast<eElumMotions>(elumMotion))
        {
            case eElumMotions::Motion_1_Idle:
                mCurrentMotion = eAbeMotions::Motion_103_ElumIdle;
                ToNewElumSyncMotion(gElum->GetAnimation().GetCurrentFrame());
                BaseAliveGameObjectCollisionLine = gElum->BaseAliveGameObjectCollisionLine;
                break;

            case eElumMotions::Motion_3_WalkLoop:
                mCurrentMotion = eAbeMotions::Motion_102_ElumWalkLoop;
                ToNewElumSyncMotion(gElum->GetAnimation().GetCurrentFrame());
                BaseAliveGameObjectCollisionLine = gElum->BaseAliveGameObjectCollisionLine;
                break;

            case eElumMotions::Motion_4_Turn:
                mCurrentMotion = eAbeMotions::Motion_113_ElumTurn;
                ToNewElumSyncMotion(gElum->GetAnimation().GetCurrentFrame());
                break;

            case eElumMotions::Motion_5_WalkToIdle:
                mCurrentMotion = eAbeMotions::Motion_124_ElumWalkEnd;
                ToNewElumSyncMotion(gElum->GetAnimation().GetCurrentFrame());
                break;

            case eElumMotions::Motion_6_MidWalkToIdle:
                mCurrentMotion = eAbeMotions::Motion_125_ElumMidWalkEnd;
                ToNewElumSyncMotion(gElum->GetAnimation().GetCurrentFrame());
                break;

            case eElumMotions::Motion_7_IdleToWalk1:
                mCurrentMotion = eAbeMotions::Motion_116_Null;
                ToNewElumSyncMotion(gElum->GetAnimation().GetCurrentFrame());
                break;

            case eElumMotions::Motion_8_IdleToWalk2:
                mCurrentMotion = eAbeMotions::Motion_117_ElumWalkBegin;
                ToNewElumSyncMotion(gElum->GetAnimation().GetCurrentFrame());
                break;

            case eElumMotions::Motion_12_RunTurn:
                mCurrentMotion = eAbeMotions::Motion_105_ElumRunTurn;
                ToNewElumSyncMotion(gElum->GetAnimation().GetCurrentFrame());
                break;

            case eElumMotions::Motion_13_RunTurnToWalk:
                mCurrentMotion = eAbeMotions::Motion_123_ElumRunTurnToWalk;
                ToNewElumSyncMotion(gElum->GetAnimation().GetCurrentFrame());
                break;

            case eElumMotions::Motion_21_Land:
                mCurrentMotion = eAbeMotions::Motion_153_ElumFall;
                ToNewElumSyncMotion(gElum->GetAnimation().GetCurrentFrame());
                BaseAliveGameObjectCollisionLine = nullptr;
                break;

            case eElumMotions::Motion_22_RunOffEdge:
                mCurrentMotion = eAbeMotions::Motion_154_ElumLand;
                ToNewElumSyncMotion(gElum->GetAnimation().GetCurrentFrame());
                BaseAliveGameObjectCollisionLine = gElum->BaseAliveGameObjectCollisionLine;
                break;

            case eElumMotions::Motion_23_WalkOffEdge:
                mCurrentMotion = eAbeMotions::Motion_152_ElumFallOffEdge;
                ToNewElumSyncMotion(gElum->GetAnimation().GetCurrentFrame());
                BaseAliveGameObjectCollisionLine = nullptr;
                break;

            case eElumMotions::Motion_24_JumpToFall:
                mCurrentMotion = eAbeMotions::Motion_155_ElumJumpToFall;
                ToNewElumSyncMotion(gElum->GetAnimation().GetCurrentFrame());
                BaseAliveGameObjectCollisionLine = nullptr;
                break;

            case eElumMotions::Motion_30_HopBegin:
                mCurrentMotion = eAbeMotions::Motion_107_ElumHopBegin;
                ToNewElumSyncMotion(gElum->GetAnimation().GetCurrentFrame());
                BaseAliveGameObjectCollisionLine = nullptr;
                break;

            case eElumMotions::Motion_31_HopMid:
                mCurrentMotion = eAbeMotions::Motion_108_ElumHopMid;
                ToNewElumSyncMotion(gElum->GetAnimation().GetCurrentFrame());
                break;

            case eElumMotions::Motion_32_HopLand:
                mCurrentMotion = eAbeMotions::Motion_109_ElumHopLand;
                ToNewElumSyncMotion(gElum->GetAnimation().GetCurrentFrame());
                BaseAliveGameObjectCollisionLine = gElum->BaseAliveGameObjectCollisionLine;
                break;

            case eElumMotions::Motion_33_RunJumpBegin:
                mCurrentMotion = eAbeMotions::Motion_110_ElumRunJumpBegin;
                ToNewElumSyncMotion(gElum->GetAnimation().GetCurrentFrame());
                BaseAliveGameObjectCollisionLine = nullptr;
                break;

            case eElumMotions::Motion_34_RunJumpMid:
                mCurrentMotion = eAbeMotions::Motion_111_ElumRunJumpMid;
                ToNewElumSyncMotion(gElum->GetAnimation().GetCurrentFrame());
                break;

            case eElumMotions::Motion_35_RunJumpLand:
                mCurrentMotion = eAbeMotions::Motion_112_ElumRunJumpLand;
                ToNewElumSyncMotion(gElum->GetAnimation().GetCurrentFrame());
                BaseAliveGameObjectCollisionLine = gElum->BaseAliveGameObjectCollisionLine;
                break;

            case eElumMotions::Motion_36_RunLoop:
                mCurrentMotion = eAbeMotions::Motion_114_ElumRunLoop;
                ToNewElumSyncMotion(gElum->GetAnimation().GetCurrentFrame());
                BaseAliveGameObjectCollisionLine = gElum->BaseAliveGameObjectCollisionLine;
                break;

            case eElumMotions::Motion_37_RunSlideStop:
                mCurrentMotion = eAbeMotions::Motion_104_ElumRunSlideStop;
                ToNewElumSyncMotion(gElum->GetAnimation().GetCurrentFrame());
                break;

            case eElumMotions::Motion_38_RunTurnToRun:
                mCurrentMotion = eAbeMotions::Motion_122_ElumRunTurnToRun;
                ToNewElumSyncMotion(gElum->GetAnimation().GetCurrentFrame());
                break;

            case eElumMotions::Motion_39_IdleToRun:
                mCurrentMotion = eAbeMotions::Motion_118_ElumRunBegin;
                ToNewElumSyncMotion(gElum->GetAnimation().GetCurrentFrame());
                break;

            case eElumMotions::Motion_40_WalkToRun:
                mCurrentMotion = eAbeMotions::Motion_119_Null;
                ToNewElumSyncMotion(gElum->GetAnimation().GetCurrentFrame());
                break;

            case eElumMotions::Motion_41_MidWalkToRun:
                mCurrentMotion = eAbeMotions::Motion_120_ElumRunToWalk;
                ToNewElumSyncMotion(gElum->GetAnimation().GetCurrentFrame());
                break;

            case eElumMotions::Motion_43_MidRunToWalk:
                mCurrentMotion = eAbeMotions::Motion_121_ElumMidRunToWalk;
                ToNewElumSyncMotion(gElum->GetAnimation().GetCurrentFrame());
                break;

            case eElumMotions::Motion_50_Knockback:
                mCurrentMotion = eAbeMotions::Motion_158_ElumKnockback;
                ToNewElumSyncMotion(gElum->GetAnimation().GetCurrentFrame());
                break;

            default:
                break;
        }
    }
}

void Abe::PickUpThrowabe_Or_PressBomb(FP fpX, s32 fpY, s16 bStandToCrouch)
{
    BaseAliveGameObject* pSlapableOrCollectable = nullptr;
    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->GetInteractive())
        {
            BaseAliveGameObject* pAliveObj = static_cast<BaseAliveGameObject*>(pObj);
            if (fpX >= pAliveObj->mCollectionRect.x && fpX <= pAliveObj->mCollectionRect.w)
            {
                const FP yPos = FP_FromInteger(fpY);
                if (yPos >= pAliveObj->mCollectionRect.y && yPos <= pAliveObj->mCollectionRect.h)
                {
                    pSlapableOrCollectable = pAliveObj;
                    pSlapableOrCollectable->mBaseGameObjectRefCount++;
                    field_15C_pThrowable = pSlapableOrCollectable;
                    break;
                }
            }
        }
    }


    if (pSlapableOrCollectable)
    {
        bool tryToSlapOrCollect = false;

        switch (pSlapableOrCollectable->Type())
        {
            case ReliveTypes::eTimedMine:
            case ReliveTypes::eUXB:
                mCurrentMotion = eAbeMotions::Motion_127_SlapBomb;
                if (bStandToCrouch)
                {
                    field_15C_pThrowable->mBaseGameObjectRefCount--;
                    field_15C_pThrowable = nullptr;
                }
                tryToSlapOrCollect = true;
                break;

            case ReliveTypes::eGrenade:
            case ReliveTypes::eMeat:
            case ReliveTypes::eRock:
                mCurrentMotion = eAbeMotions::Motion_149_PickupItem;
                field_19C_throwable_count += static_cast<s8>(static_cast<BaseThrowable*>(field_15C_pThrowable)->VGetCount());

                if (!gThrowableIndicatorExists)
                {
                    const FP v16 = (GetSpriteScale() * FP_FromInteger(-30)) + mYPos;
                    relive_new ThrowableTotalIndicator(
                                                                          (GetSpriteScale() * FP_FromInteger(0)) + mXPos,
                                                                               v16,
                                                                               GetAnimation().GetRenderLayer(),
                                                                               GetAnimation().GetSpriteScale(),
                                                                               field_19C_throwable_count,
                                                                               1);
                }
                tryToSlapOrCollect = true;
                break;

            case ReliveTypes::eMine:
                field_15C_pThrowable->mBaseGameObjectRefCount--;
                field_15C_pThrowable = nullptr;
                tryToSlapOrCollect = true;
                break;

            default:
                return;
        }

        if (tryToSlapOrCollect)
        {
            if (mCurrentMotion == eAbeMotions::Motion_149_PickupItem)
            {
                if (bStandToCrouch)
                {
                    SfxPlayMono(relive::SoundEffects::PickupItem, 0, GetSpriteScale());
                    field_15C_pThrowable->mBaseGameObjectRefCount--;
                    field_15C_pThrowable->VOnAbeInteraction();
                    field_15C_pThrowable = nullptr;
                    mCurrentMotion = eAbeMotions::Motion_19_CrouchIdle;
                }
            }
        }
    }
}

void Abe::CrouchingGameSpeak()
{
    field_10C_prev_held |= Input().GetPressed();

    if (Input().IsAnyHeld(InputCommands::eLeftGameSpeak))
    {
        if (field_10C_prev_held & InputCommands::eGameSpeak2)
        {
            gEventSystem->PushEvent(GameSpeakEvents::eAbe_FollowMe);
            Mudokon_SFX(MudSounds::eFollowMe_4, 0, 0, this);
            mCurrentMotion = eAbeMotions::Motion_22_CrouchSpeak;
        }
        else if (field_10C_prev_held & InputCommands::eGameSpeak4)
        {
            gEventSystem->PushEvent(GameSpeakEvents::eAbe_Wait);
            Mudokon_SFX(MudSounds::eWait_6, 0, 0, this);
            mCurrentMotion = eAbeMotions::Motion_23_CrouchSpeak;
        }
        else if (field_10C_prev_held & InputCommands::eGameSpeak1)
        {
            gEventSystem->PushEvent(GameSpeakEvents::eAbe_Hello);
            Mudokon_SFX(MudSounds::eHello_3, 0, 0, this);
            mCurrentMotion = eAbeMotions::Motion_22_CrouchSpeak;
        }
        else if (field_10C_prev_held & InputCommands::eGameSpeak3)
        {
            gEventSystem->PushEvent(GameSpeakEvents::eAbe_Anger);
            Mudokon_SFX(MudSounds::eAngry_5, 0, 0, this);
            mCurrentMotion = eAbeMotions::Motion_23_CrouchSpeak;
        }
    }
    else if (Input().IsAnyHeld(InputCommands::eRightGameSpeak))
    {
        if (field_10C_prev_held & InputCommands::eGameSpeak6)
        {
            gEventSystem->PushEvent(GameSpeakEvents::eAbe_WhistleHigh);
            Mudokon_SFX(MudSounds::eWhistleHigh_1, 0, 0, this);
            mCurrentMotion = eAbeMotions::Motion_23_CrouchSpeak;
        }
        else if (field_10C_prev_held & InputCommands::eGameSpeak5)
        {
            gEventSystem->PushEvent(GameSpeakEvents::eAbe_WhistleLow);
            Mudokon_SFX(MudSounds::eWhistleLow_2, 0, 0, this);
            mCurrentMotion = eAbeMotions::Motion_22_CrouchSpeak;
        }
        else if (field_10C_prev_held & InputCommands::eGameSpeak8)
        {
            gEventSystem->PushEvent(GameSpeakEvents::eAbe_Laugh);
            Mudokon_SFX(MudSounds::eLaugh1_8, 0, 0, this);
            mCurrentMotion = eAbeMotions::Motion_23_CrouchSpeak;
        }
        else if (field_10C_prev_held & InputCommands::eGameSpeak7)
        {
            gEventSystem->PushEvent(GameSpeakEvents::eAbe_Fart);
            Mudokon_SFX(MudSounds::eFart_7, 0, 0, this);
            if (gEnableFartGasCheat)
            {
                FP xPos = {};
                if (GetAnimation().GetFlipX())
                {
                    xPos = mXPos + FP_FromDouble(10) * GetSpriteScale();
                }
                else
                {
                    xPos = mXPos - FP_FromDouble(10) * GetSpriteScale();
                }
                New_Smoke_Particles(
                    xPos,
                    mYPos - FP_FromDouble(6) * GetSpriteScale(),
                    GetSpriteScale() * FP_FromDouble(0.5),
                    3,
                    RGB16{ 32, 128, 32 });
            }
            field_130_say = 8;
            mCurrentMotion = eAbeMotions::Motion_22_CrouchSpeak;
            field_134_auto_say_timer = MakeTimer(15);
        }
    }
}

void Abe::FallOnBombs()
{
    const PSX_RECT bOurRect = VGetBoundingRect();
    for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
    {
        IBaseAliveGameObject* pObjIter = gBaseAliveGameObjects->ItemAt(i);
        if (!pObjIter)
        {
            break;
        }

        if (pObjIter->Type() == ReliveTypes::eMine || pObjIter->Type() == ReliveTypes::eUXB)
        {
            const PSX_RECT objRect = pObjIter->VGetBoundingRect();
            if (bOurRect.x <= objRect.w
                && bOurRect.w >= objRect.x
                && bOurRect.h >= objRect.y
                && bOurRect.y <= objRect.h)
            {
                pObjIter->VTakeDamage(this);
            }
        }
    }
}

s16 Abe::ToLeftRightMovement()
{
    mVelY = FP_FromInteger(0);
    if (sControlledCharacter != this)
    {
        return 0;
    }

    const FP gridSize = ScaleToGridSize(GetSpriteScale());
    const bool flipX = GetAnimation().GetFlipX();

    if ((flipX && Input().IsAnyHeld(InputCommands::eRight)) || (!flipX && Input().IsAnyHeld(InputCommands::eLeft)))
    {
        mCurrentMotion = eAbeMotions::Motion_2_StandingTurn;
        return 1;
    }

    if (Input().IsAnyHeld(InputCommands::eRight) || Input().IsAnyHeld(InputCommands::eLeft))
    {
        const FP directionX = FP_FromInteger(Input().IsAnyHeld(InputCommands::eRight) ? 1 : -1);

        if (Input().IsAnyHeld(InputCommands::eRun))
        {
            mVelX = directionX * (gridSize / FP_FromInteger(4));
            mCurrentMotion = eAbeMotions::Motion_41_StandingToRun;
        }
        else if (Input().IsAnyHeld(InputCommands::eSneak))
        {
            mVelX = directionX * (gridSize / FP_FromInteger(10));
            mCurrentMotion = eAbeMotions::Motion_47_SneakBegin;
        }
        else
        {
            mVelX = directionX * (gridSize / FP_FromInteger(9));
            mCurrentMotion = eAbeMotions::Motion_6_WalkBegin;
        }

        if (!WallHit(GetSpriteScale() * FP_FromInteger(50), directionX * gridSize))
        {
            if (!WallHit(GetSpriteScale() * FP_FromInteger(20), directionX * gridSize))
            {
                return 1;
            }
        }

        if (WallHit(GetSpriteScale() * FP_FromInteger(20), directionX * gridSize))
        {
            mVelX = FP_FromInteger(0);
            mCurrentMotion = eAbeMotions::Motion_72_PushWall;
            Environment_SFX(EnvironmentSfx::eGenericMovement_9, 0, 0x7FFF, this);
            return 0;
        }

        mCurrentMotion = eAbeMotions::Motion_21_StandToCrouch;
        return 1;
    }

    return 0;
}


void Abe::MoveWithVelocity(FP speed)
{
    if (mVelX > FP_FromInteger(0))
    {
        mVelX = mVelX - (GetSpriteScale() * speed);
        if (mVelX < FP_FromInteger(0))
        {
            mVelX = FP_FromInteger(0);
        }
    }
    else if (mVelX < FP_FromInteger(0))
    {
        mVelX = (GetSpriteScale() * speed) + mVelX;
        if (mVelX > FP_FromInteger(0))
        {
            mVelX = FP_FromInteger(0);
        }
    }

    if (FP_GetExponent(mVelX))
    {
        MoveForward();
    }
}

void Abe::ToNewElumSyncMotion(s32 elum_frame)
{
    GetAnimation().Set_Animation_Data(GetAnimRes(sAbeMotionAnimIds[static_cast<u32>(mCurrentMotion)]));

    field_12C_timer = sGnFrame;
    GetAnimation().SetFrame(elum_frame + 1);
    GetAnimation().SetFlipX(gElum->GetAnimation().GetFlipX());
}

void Abe::SetActiveControlledCharacter()
{
    mBlockChanting = true;
    sControlledCharacter = this;
}

PullRingRope* Abe::GetPullRope()
{
    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->Type() == ReliveTypes::ePullRingRope)
        {
            PullRingRope* pRope = static_cast<PullRingRope*>(pObj);

            const PSX_RECT bRect = pRope->VGetBoundingRect();
            if ((mYPos - pRope->mYPos - (GetSpriteScale() * FP_FromInteger(80))) <= FP_FromInteger(0))
            {
                if (mXPos > FP_FromInteger(bRect.x) && mXPos < FP_FromInteger(bRect.w))
                {
                    return pRope;
                }
            }
        }
    }
    return nullptr;
}

void Abe::ElumKnockForward()
{
    ToKnockback(1, 1);
    mCurrentMotion = eAbeMotions::Motion_128_KnockForward;
    mNextMotion = eAbeMotions::Motion_0_Idle;
    mbMotionChanged = true;
    GetAnimation().Set_Animation_Data(GetAnimRes(sAbeMotionAnimIds[static_cast<u32>(mCurrentMotion)]));

    sControlledCharacter = gAbe;
    gElum->field_154_bAbeForcedDownFromElum = 1;
}

eAbeMotions Abe::TryMountElum()
{
    if (gElum)
    {
        if (FP_Abs(mXPos - gElum->mXPos) <= FP_FromInteger(2) && FP_Abs(mYPos - gElum->mYPos) <= FP_FromInteger(2))
        {
            if (gElum->GetAnimation().GetFlipX() != GetAnimation().GetFlipX())
            {
                mNextMotion = eAbeMotions::Motion_139_ElumMountBegin;
                return eAbeMotions::Motion_2_StandingTurn;
            }

            if (gElum->mCurrentMotion != eElumMotions::Motion_1_Idle
                || gElum->mBrainIdx == 1
                || gElum->mStrugglingWithBees)
            {
                return eAbeMotions::Motion_0_Idle;
            }

            mRidingElum = true;
            return eAbeMotions::Motion_139_ElumMountBegin;
        }
    }
    return eAbeMotions::Motion_0_Idle;
}

void Abe::BulletDamage(Bullet* pBullet)
{
    const PSX_RECT rect = VGetBoundingRect();

    enum class ShootKind
    {
        eEverythingElse_0 = 0,
        eHanging_1 = 1,
        eRolling_2 = 2,
    };

    if (mCurrentMotion == eAbeMotions::Motion_137_ElumUnmountBegin
        || mCurrentMotion == eAbeMotions::Motion_136_ElumMountEnd
        || mCurrentMotion == eAbeMotions::Motion_138_ElumUnmountEnd
        || mCurrentMotion == eAbeMotions::Motion_139_ElumMountBegin
        || !gMap.Is_Point_In_Current_Camera(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos,
            1)
        || mHealth <= FP_FromInteger(0))
    {
        return;
    }

    ShootKind shootKind = ShootKind::eEverythingElse_0;
    if (mCurrentMotion == eAbeMotions::Motion_24_RollBegin
        || mCurrentMotion == eAbeMotions::Motion_25_RollLoop
        || mCurrentMotion == eAbeMotions::Motion_26_RollEnd
        || mCurrentMotion == eAbeMotions::Motion_39_CrouchTurn
        || mCurrentMotion == eAbeMotions::Motion_19_CrouchIdle)
    {
        shootKind = ShootKind::eRolling_2;
    }
    else if (mCurrentMotion == eAbeMotions::Motion_64_LedgeAscend
             || mCurrentMotion == eAbeMotions::Motion_66_LedgeHang
             || mCurrentMotion == eAbeMotions::Motion_68_LedgeHangWobble
             || mCurrentMotion == eAbeMotions::Motion_65_LedgeDescend
             || mCurrentMotion == eAbeMotions::Motion_67_ToOffScreenHoist)
    {
        shootKind = ShootKind::eHanging_1;
    }

    mHealth = FP_FromInteger(0);
    mbGotShot = true;

    switch (pBullet->mBulletType)
    {
        case BulletType::ePossessedSlig_0:
        case BulletType::eNormalBullet_1:
        {
            FP bloodXOff = {};
            if (pBullet->mXDistance > FP_FromInteger(0))
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

            switch (shootKind)
            {
                case ShootKind::eEverythingElse_0:
                {
                    if (!mRidingElum)
                    {
                        ToKnockback(1, 1);
                    }
                    else
                    {
                        ElumKnockForward();
                    }

                    if (GetAnimation().GetFlipX() != (pBullet->mXDistance > FP_FromInteger(0)))
                    {
                        mCurrentMotion = eAbeMotions::Motion_128_KnockForward;
                    }
                    mbMotionChanged = true;
                    mbGotShot = false;

                    if (pBullet->mXDistance >= FP_FromInteger(0))
                    {
                        mVelX = GetSpriteScale() * FP_FromDouble(7.8);
                    }
                    else
                    {
                        mVelX = -GetSpriteScale() * FP_FromDouble(7.8);
                    }
                    break;
                }
                case ShootKind::eHanging_1:
                {
                    mCurrentMotion = eAbeMotions::Motion_92_ForceDownFromHoist;
                    mbMotionChanged = true;
                    mbGotShot = false;
                    field_114_gnFrame = 0;
                    break;
                }
                case ShootKind::eRolling_2:
                {
                    if (GetAnimation().GetFlipX() == (pBullet->mXDistance > FP_FromInteger(0)))
                    {
                        mNextMotion = eAbeMotions::Motion_73_RollingKnockback;
                    }
                    else
                    {
                        mNextMotion = eAbeMotions::Motion_129_RollingKnockForward;
                    }
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case BulletType::eZBullet_2:
        {
            if (GetSpriteScale() == FP_FromDouble(0.5))
            {
                mbGotShot = false;
                mHealth = FP_FromInteger(1);
                return;
            }
            if (Bullet::InZBulletCover(FP_FromInteger(rect.x), FP_FromInteger(rect.y), rect))
            {
                mbGotShot = false;
                mHealth = FP_FromInteger(1);
                return;
            }
            if (shootKind != ShootKind::eEverythingElse_0)
            {
                if (shootKind == ShootKind::eHanging_1)
                {
                    mCurrentMotion = eAbeMotions::Motion_92_ForceDownFromHoist;
                    mbMotionChanged = true;
                    mbGotShot = false;
                    field_114_gnFrame = 0;
                }
                else if (shootKind == ShootKind::eRolling_2)
                {
                    mNextMotion = eAbeMotions::Motion_147_ShotRolling;
                }
            }
            if (mCurrentMotion != eAbeMotions::Motion_114_ElumRunLoop)
            {
                if (!mRidingElum)
                {
                    mNextMotion = eAbeMotions::Motion_148_Shot;
                }
                else
                {
                    ElumKnockForward();
                    mCurrentMotion = eAbeMotions::Motion_148_Shot;
                    mbMotionChanged = true;
                    mbGotShot = false;
                }
            }
            else
            {
                mbGotShot = false;
                mHealth = FP_FromInteger(1);
                return;
            }

                relive_new Blood(
                    mXPos,
                    mYPos - FP_FromInteger(45),
                    FP_FromInteger(0),
                    FP_FromInteger(0),
                    FP_FromInteger(1),
                    50);

            break;
        }
        default:
            break;
    }

    if (mbGotShot)
    {
        field_112_prev_motion = mNextMotion;
    }

    Environment_SFX(EnvironmentSfx::eElumHitWall_14, 0, 0x7FFF, this);
    Mudokon_SFX(MudSounds::eKnockbackOuch_10, 127, 0, this);
    Environment_SFX(EnvironmentSfx::eDeathNoise_7, 0, 0x7FFF, this);
    SFX_Play_Pitch(relive::SoundEffects::Eating1, 0, -500, GetSpriteScale());
    SfxPlayMono(relive::SoundEffects::KillEffect, 0, GetSpriteScale());
}

bool Abe::NearDoorIsOpen()
{
    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->Type() == ReliveTypes::eDoor)
        {
            auto pDoor = static_cast<Door*>(pObj);
            PSX_RECT Rect = VGetBoundingRect();
            PSX_RECT Rect2 = pDoor->VGetBoundingRect();

            if (Rect.x <= Rect2.w && Rect.w >= Rect2.x && Rect.h >= Rect2.y && Rect.y <= Rect2.h)
            {
                return pDoor->vIsOpen_40E800();
            }
        }
    }

    // We didn't find a door - so for some reason that makes no sense return that it is open...
    return true;
}

s16 Abe::RunTryEnterDoor()
{
    if (!Input().IsAnyHeld(InputCommands::eUp))
    {
        return 0;
    }
    if (GetElectrocuted())
    {
        return 0;
    }
    if (GetAnimation().GetCurrentFrame() < 4)
    {
        return 0;
    }

    // Are we actually on a door?
    relive::Path_TLV* pDoorTlv = gMap.VTLV_Get_At_Of_Type(
        FP_GetExponent(mXPos),
        FP_GetExponent(mYPos),
        FP_GetExponent(mXPos),
        FP_GetExponent(mYPos),
        ReliveTypes::eDoor);

    if (!pDoorTlv)
    {
        return 0;
    }

    if (!NearDoorIsOpen())
    {
        return 0;
    }

    BaseAliveGameObjectPathTLV = pDoorTlv;
    field_110_state.raw = 0;
    mCurrentMotion = eAbeMotions::Motion_156_DoorEnter;
    mXPos = FP_FromInteger((pDoorTlv->mBottomRightX + pDoorTlv->mTopLeftX) / 2);
    MapFollowMe(true);
    return 1;
}

eAbeMotions Abe::MoveLiftUpOrDown(FP yVelocity)
{
    LiftPoint* pLiftPoint = static_cast<LiftPoint*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    if (!pLiftPoint)
    {
        return eAbeMotions::Motion_135_LiftGrabIdle;
    }

    pLiftPoint->Move(FP_FromInteger(0), yVelocity);
    FollowLift();

    if (gBeeInstanceCount && gBeesNearAbe)
    {
        return eAbeMotions::Motion_141_BeesStrugglingOnLift;
    }
    if (sControlledCharacter == this && !GetAnimation().GetIsLastFrame() && GetAnimation().GetCurrentFrame() != 5)
    {
        return mCurrentMotion;
    }
    if (yVelocity >= FP_FromInteger(0))
    {
        if (yVelocity > FP_FromInteger(0))
        {
            if (pLiftPoint->OnBottomFloor())
            {
                return eAbeMotions::Motion_135_LiftGrabIdle;
            }
            if (Input().IsAnyHeld(InputCommands::eDown))
            {
                return eAbeMotions::Motion_132_LiftUseDown;
            }
            if (Input().IsAnyHeld(InputCommands::eUp))
            {
                return eAbeMotions::Motion_131_LiftUseUp;
            }
        }
    }
    else
    {
        if (pLiftPoint->OnTopFloor())
        {
            return eAbeMotions::Motion_135_LiftGrabIdle;
        }
        if (Input().IsAnyHeld(InputCommands::eUp))
        {
            return eAbeMotions::Motion_131_LiftUseUp;
        }
        if (Input().IsAnyHeld(InputCommands::eDown))
        {
            return eAbeMotions::Motion_132_LiftUseDown;
        }
    }

    if (Input().GetHeld() && pLiftPoint->OnAnyFloor() && !pLiftPoint->mMoving)
    {
        return eAbeMotions::Motion_134_LiftGrabEnd;
    }

    pLiftPoint->Move(FP_FromInteger(0), FP_FromInteger(0));
    return eAbeMotions::Motion_135_LiftGrabIdle;
}

void Abe::VScreenChanged()
{
    if (sControlledCharacter == this || sControlledCharacter == gElum)
    {
        mCurrentLevel = gMap.mNextLevel;
        mCurrentPath = gMap.mNextPath;
    }

    // Level has changed?
    if (gMap.LevelChanged())
    {
        // Set the correct tint for this map
        SetTint(sAbeTintTable, gMap.mNextLevel);

        if (gMap.mCurrentLevel != EReliveLevelIds::eMenu)
        {
            if (field_19C_throwable_count > 0)
            {
                if (gThrowableArray)
                {
                    gThrowableArray->Remove(field_19C_throwable_count);
                }
            }

            field_19C_throwable_count = 0;
            field_168_ring_pulse_timer = 0;
        }

        if (gMap.mNextLevel == EReliveLevelIds::eCredits || gMap.mNextLevel == EReliveLevelIds::eMenu)
        {
            // Remove Abe for menu/credits levels?
            SetDead(true);
        }
    }
}

void Abe::VOnTlvCollision(relive::Path_TLV* pTlv)
{
    while (pTlv)
    {
        if (pTlv->mTlvType == ReliveTypes::eContinuePoint)
        {
            relive::Path_ContinuePoint* pContinuePointTlv = static_cast<relive::Path_ContinuePoint*>(pTlv);

            if ((pContinuePointTlv->mZoneNumber != mContinueZoneNumber || mContinueLevel != gMap.mCurrentLevel) && !GetElectrocuted() && mCurrentMotion != eAbeMotions::Motion_156_DoorEnter)
            {
                mContinueZoneNumber = pContinuePointTlv->mZoneNumber;
                mContinueClearFromId = pContinuePointTlv->mClearFromId;
                mContinueClearToId = pContinuePointTlv->mClearToId;

                mContinueTopLeft.x = pContinuePointTlv->mTopLeftX;
                mContinueTopLeft.y = pContinuePointTlv->mTopLeftY;
                mContinueBottomRight.x = pContinuePointTlv->mBottomRightX;
                mContinueBottomRight.y = pContinuePointTlv->mBottomRightY;
                mContinueSpriteScale = GetSpriteScale();

                mAbeRespawnFlipX = pContinuePointTlv->mAbeSpawnDir == relive::Path_ContinuePoint::spawnDirection::eLeft;

                const s32 bHaveShry = field_168_ring_pulse_timer - sGnFrame;
                field_150_saved_ring_timer = bHaveShry < 0 ? 0 : bHaveShry;
                field_154_bSavedHaveShrykull = field_16C_bHaveShrykull;

                mContinueLevel = gMap.mCurrentLevel;
                mContinuePath = gMap.mCurrentPath;
                mContinueCamera = gMap.mCurrentCamera;

                if (gRestartRuptureFarmsSavedMuds == 0 && gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarmsReturn && gMap.mCurrentPath == 19 && gMap.mCurrentCamera == 3)
                {
                    gRestartRuptureFarmsKilledMuds = sKilledMudokons;
                    gRestartRuptureFarmsSavedMuds = sRescuedMudokons;
                }

                SaveGame::SaveToMemory(&gSaveBuffer);

                const FP camXPos = FP_NoFractional(gScreenManager->CamXPos());

                FP indicator_xpos = {};
                if (mXPos - camXPos >= FP_FromInteger(384 / 2)) // mid screen x
                {
                    indicator_xpos = mXPos - ScaleToGridSize(GetSpriteScale());
                }
                else
                {
                    indicator_xpos = ScaleToGridSize(GetSpriteScale()) + mXPos;
                }
                const FP indicator_ypos = mYPos + (GetSpriteScale() * FP_FromInteger(-50));

                relive_new ThrowableTotalIndicator(indicator_xpos, indicator_ypos, GetAnimation().GetRenderLayer(),
                                                                            GetAnimation().GetSpriteScale(), 11, 1);
            }
        }
        else if (pTlv->mTlvType == ReliveTypes::eDeathDrop)
        {
            Mudokon_SFX(MudSounds::eDeathDropScream_17, 0, 0, this);

            EventBroadcast(kEventNoise, this);
            EventBroadcast(kEventSuspiciousNoise, this);
            EventBroadcast(kEventLoudNoise, this);
            EventBroadcast(kEventSpeaking, this);

            if (mRidingElum && sControlledCharacter != this)
            {
                sControlledCharacter = gAbe;
                FreeElumRes();
            }
            ToDeathDropFall();
        }

        // To next TLV
        pTlv = gMap.TLV_Get_At(pTlv, mXPos, mYPos, mXPos, mYPos);
    }
}

eAbeMotions Abe::HandleDoAction()
{
    eAbeMotions mountMotion = TryMountElum();
    if (mountMotion != eAbeMotions::Motion_0_Idle)
    {
        return mountMotion;
    }

    relive::Path_TLV* pTlv = gMap.TLV_Get_At(
        nullptr,
        mXPos,
        mYPos,
        mXPos,
        mYPos);

    while (pTlv)
    {
        switch (pTlv->mTlvType)
        {
            case ReliveTypes::eWellLocal:
                BaseAliveGameObjectPathTLV = pTlv;
                return eAbeMotions::Motion_77_WellBegin;

            case ReliveTypes::eLever:
                if (FP_FromInteger(FP_GetExponent(mXPos) - pTlv->mTopLeftX) < ScaleToGridSize(GetSpriteScale()))
                {
                    // Wrong dir
                    if (GetAnimation().GetFlipX())
                    {
                        return eAbeMotions::Motion_36_DunnoBegin;
                    }

                    // Get switch
                    auto pSwitch = static_cast<Lever*>(FindObjectOfType(
                        ReliveTypes::eLever,
                        mXPos + ScaleToGridSize(GetSpriteScale()),
                        mYPos - FP_FromInteger(5)));

                    // Pull it
                    if (pSwitch)
                    {
                        pSwitch->VPull(mXPos < pSwitch->mXPos);
                        return eAbeMotions::Motion_101_LeverUse;
                    }
                }
                else if (FP_FromInteger(pTlv->mBottomRightX - FP_GetExponent(mXPos)) < ScaleToGridSize(GetSpriteScale()))
                {
                    // Wrong dir
                    if (!GetAnimation().GetFlipX())
                    {
                        return eAbeMotions::Motion_36_DunnoBegin;
                    }

                    // Get switch
                    auto pSwitch = static_cast<Lever*>(FindObjectOfType(
                        ReliveTypes::eLever,
                        mXPos - ScaleToGridSize(GetSpriteScale()),
                        mYPos - FP_FromInteger(5)));

                    // Pull it
                    if (pSwitch)
                    {
                        pSwitch->VPull(mXPos < pSwitch->mXPos);
                        return eAbeMotions::Motion_101_LeverUse;
                    }
                }
                break;

            case ReliveTypes::eWellExpress:
                BaseAliveGameObjectPathTLV = pTlv;
                return eAbeMotions::Motion_80_430EF0;

            case ReliveTypes::eBoomMachine:
            {
                auto pBoomMachine = static_cast<BoomMachine*>(FindObjectOfType(
                    ReliveTypes::eBoomMachine,
                    mXPos,
                    mYPos - (GetSpriteScale() * FP_FromInteger(25))));

                if (pBoomMachine && pBoomMachine->VIsButtonOn())
                {
                    pBoomMachine->VHandleButton();
                    return eAbeMotions::Motion_90_GrenadeMachineUse;
                }
                return eAbeMotions::Motion_36_DunnoBegin;
            }

            default:
                break;
        }

        pTlv = gMap.TLV_Get_At(
            pTlv,
            mXPos,
            mYPos,
            mXPos,
            mYPos);
    }
    return eAbeMotions::Motion_36_DunnoBegin;
}

bool Abe::VTakeDamage(BaseGameObject* pFrom)
{
    SND_SEQ_Stop(SeqId::eMudokonChant1_11);

    const auto old_say = field_130_say;
    field_130_say = -1;

    auto pOrbWhirlWind = static_cast<OrbWhirlWind*>(sObjectIds.Find_Impl(mOrbWhirlWindId));
    if (pOrbWhirlWind)
    {
        pOrbWhirlWind->ToStop();
        mOrbWhirlWindId = Guid{};
    }

    if (field_18C_pObjToPossess)
    {
        field_18C_pObjToPossess->mBaseGameObjectRefCount--;
        field_18C_pObjToPossess = nullptr;
    }

    // CantBeDamaged_44BAB0() helper func in AE
    switch (mCurrentMotion)
    {
        case eAbeMotions::Motion_74_JumpIntoWell:
        case eAbeMotions::Motion_75_ToInsideOfAWellLocal:
        case eAbeMotions::Motion_76_ToWellShotOut:
        case eAbeMotions::Motion_77_WellBegin:
        case eAbeMotions::Motion_78_InsideWellLocal:
        case eAbeMotions::Motion_79_WellShotOut:
        case eAbeMotions::Motion_80_430EF0:
        case eAbeMotions::Motion_81_InsideWellExpress:
        case eAbeMotions::Motion_82_WellExpressShotOut:
        case eAbeMotions::Motion_83_430F00:
        case eAbeMotions::Motion_84_ToInsideWellLocal:
        case eAbeMotions::Motion_85_ToWellShotOut:
        case eAbeMotions::Motion_156_DoorEnter:
        case eAbeMotions::Motion_157_DoorExit:
        case eAbeMotions::Motion_162_ToShrykull:
        case eAbeMotions::Motion_163_ShrykullEnd:
            return false;
    }

    if (gAbeInvulnerableCheat || mShrivel || !GetAnimation().GetRender())
    {
        return false;
    }

    field_11C_regen_health_timer = MakeTimer(900);

    auto oldHp = mHealth;

    switch (pFrom->Type())
    {
        case ReliveTypes::eBat:
            if (mHealth > FP_FromInteger(0))
            {
                mHealth -= FP_FromInteger(1);
                if (mHealth > FP_FromInteger(0))
                {
                    Mudokon_SFX(MudSounds::eKnockbackOuch_10, 0, 0, this);
                }
                else
                {
                    Mudokon_SFX(MudSounds::eKnockbackOuch_10, 0, 0, this);
                    Environment_SFX(EnvironmentSfx::eDeathNoise_7, 0, 0x7FFF, this);
                    mHealth = FP_FromInteger(0);
                    mbGotShot = true;
                    field_112_prev_motion = eAbeMotions::Motion_128_KnockForward;
                    mNextMotion = eAbeMotions::Motion_128_KnockForward;
                    if (mCurrentMotion != eAbeMotions::Motion_64_LedgeAscend
                        && mCurrentMotion != eAbeMotions::Motion_66_LedgeHang
                        && mCurrentMotion != eAbeMotions::Motion_68_LedgeHangWobble
                        && mCurrentMotion != eAbeMotions::Motion_65_LedgeDescend
                        && mCurrentMotion != eAbeMotions::Motion_67_ToOffScreenHoist)
                    {
                        break;
                    }
                    else
                    {
                        mCurrentMotion = eAbeMotions::Motion_92_ForceDownFromHoist;
                        mbMotionChanged = true;
                        mbGotShot = false;
                        field_114_gnFrame = 0;
                        return true;
                    }
                }
            }
            break;

        case ReliveTypes::eGasCountDown:
            if (mHealth > FP_FromInteger(0))
            {
                if (mCurrentMotion == eAbeMotions::Motion_64_LedgeAscend
                    || mCurrentMotion == eAbeMotions::Motion_66_LedgeHang
                    || mCurrentMotion == eAbeMotions::Motion_68_LedgeHangWobble
                    || mCurrentMotion == eAbeMotions::Motion_65_LedgeDescend
                    || mCurrentMotion == eAbeMotions::Motion_67_ToOffScreenHoist)
                {
                    mCurrentMotion = eAbeMotions::Motion_92_ForceDownFromHoist;
                    mbGotShot = false;
                    field_114_gnFrame = 0;
                    mbMotionChanged = true;
                    return true;
                }

                if (IsStanding())
                {
                    mHealth = FP_FromInteger(0);
                    mCurrentMotion = eAbeMotions::Motion_164_PoisonGasDeath;
                    mbMotionChanged = true;
                    field_114_gnFrame = 1;
                }
                else
                {
                    if (mCurrentMotion != eAbeMotions::Motion_70_Knockback && mCurrentMotion != eAbeMotions::Motion_71_KnockbackGetUp)
                    {
                        ToKnockback(1, 1);
                        mbMotionChanged = true;
                    }
                }
            }
            break;

        case ReliveTypes::eElectricWall:
            Mudokon_SFX(MudSounds::eOops_16, 0, 0, this);
            break;

        case ReliveTypes::eGroundExplosion:
        case ReliveTypes::eAirExplosion:
            if (GetAnimation().GetRender())
            {
                if (mHealth > FP_FromInteger(0))
                {
                    mbGotShot = true;
                    field_112_prev_motion = eAbeMotions::Motion_70_Knockback;
                    mNextMotion = eAbeMotions::Motion_70_Knockback;
                }

                mHealth = FP_FromInteger(0);
                mRGB.SetRGB(30, 30, 30);

                relive_new Gibs(
                    GibType::eAbe,
                    mXPos,
                    mYPos,
                    FP_FromInteger(0),
                    FP_FromInteger(0),
                    GetSpriteScale(),
                    false);

                GetAnimation().SetRender(false);
                GetShadow()->mEnabled = false;
            }
            break;

        case ReliveTypes::eSecurityClaw:
        case ReliveTypes::eSecurityOrb:
            field_130_say = 5;
            field_134_auto_say_timer = MakeTimer(27);
            // The zap makes Abe drop his stuff everywhere
            for (s32 i = 0; i < field_19C_throwable_count; i++)
            {
                mThrowable = Make_Throwable(mXPos, mYPos - FP_FromInteger(30), 0);

                const FP rand1 = FP_FromRaw((Math_NextRandom() - 127) << 11); // TODO: Wat?
                const FP rand2 = (FP_FromDouble(0.03125) * FP_FromRaw(Math_NextRandom())) - FP_FromInteger(2);
                mThrowable->VThrow(rand1, rand2);
                mThrowable->SetSpriteScale(GetSpriteScale());
                mThrowable->VTimeToExplodeRandom();
                mThrowable = nullptr;
            }
            field_19C_throwable_count = 0;
            break;

        case ReliveTypes::eRockSpawner:
            if (mHealth > FP_FromInteger(0))
            {
                mbMotionChanged = true;
                mHealth = FP_FromInteger(0);

                if (mCurrentMotion == eAbeMotions::Motion_64_LedgeAscend
                    || mCurrentMotion == eAbeMotions::Motion_66_LedgeHang
                    || mCurrentMotion == eAbeMotions::Motion_68_LedgeHangWobble
                    || mCurrentMotion == eAbeMotions::Motion_65_LedgeDescend
                    || mCurrentMotion == eAbeMotions::Motion_67_ToOffScreenHoist)
                {
                    mCurrentMotion = eAbeMotions::Motion_92_ForceDownFromHoist;
                    mbMotionChanged = true;
                    mbGotShot = false;
                    field_114_gnFrame = 0;
                    return true;
                }
                ToKnockback(1, 1);
                SfxPlayMono(relive::SoundEffects::KillEffect, 127);
            }
            break;

        case ReliveTypes::eMeatSaw:
            if (mHealth > FP_FromInteger(0))
            {
                mbMotionChanged = true;
                mHealth = FP_FromInteger(0);
                ToKnockback(1, 1);
                mRGB.SetRGB(30, 30, 30);

                relive_new Gibs(
                    GibType::eAbe,
                    mXPos,
                    mYPos,
                    FP_FromInteger(0),
                    FP_FromInteger(0),
                    GetSpriteScale(),
                    false);

                relive_new Gibs(
                    GibType::eAbe,
                    mXPos,
                    mYPos,
                    FP_FromInteger(0),
                    FP_FromInteger(0),
                    GetSpriteScale(),
                    false);

                GetAnimation().SetRender(false);
            }
            break;

        case ReliveTypes::eParamite:
        case ReliveTypes::eScrab:
        case ReliveTypes::eSlog:
            if (mHealth > FP_FromInteger(0))
            {
                auto pAliveObj = static_cast<BaseAliveGameObject*>(pFrom);
                mHealth = FP_FromInteger(0);
                const PSX_RECT abeRect = VGetBoundingRect();
                if (pAliveObj->mVelX <= FP_FromInteger(0))
                {
                    relive_new Blood(
                        mXPos,
                        FP_FromInteger(abeRect.h + abeRect.y) / FP_FromInteger(2),
                        FP_FromInteger(-24),
                        FP_FromInteger(0),
                        GetSpriteScale(),
                        50);
                }
                else
                {
                    relive_new Blood(
                        mXPos,
                        FP_FromInteger(abeRect.h + abeRect.y) / FP_FromInteger(2),
                        FP_FromInteger(24),
                        FP_FromInteger(0),
                        GetSpriteScale(),
                        50);
                }

                if (mCurrentMotion == eAbeMotions::Motion_64_LedgeAscend
                    || mCurrentMotion == eAbeMotions::Motion_66_LedgeHang
                    || mCurrentMotion == eAbeMotions::Motion_68_LedgeHangWobble
                    || mCurrentMotion == eAbeMotions::Motion_65_LedgeDescend
                    || mCurrentMotion == eAbeMotions::Motion_67_ToOffScreenHoist)
                {
                    mCurrentMotion = eAbeMotions::Motion_92_ForceDownFromHoist;
                    mbMotionChanged = true;
                    mbGotShot = false;
                    field_114_gnFrame = 0;
                    return true;
                }

                if (mCurrentMotion >= eAbeMotions::Motion_102_ElumWalkLoop
                    && mCurrentMotion <= eAbeMotions::Motion_125_ElumMidWalkEnd)
                {
                    mNextMotion = eAbeMotions::Motion_148_Shot;
                    field_112_prev_motion = eAbeMotions::Motion_148_Shot;
                    mbGotShot = true;
                    return true;
                }

                ToKnockback(1, 1);
                mbMotionChanged = true;

                if (pAliveObj->mXPos < mXPos)
                {
                    if (!(GetAnimation().GetFlipX()))
                    {
                        mCurrentMotion = eAbeMotions::Motion_128_KnockForward;
                    }
                }
                else if (pAliveObj->mXPos > mXPos)
                {
                    if (GetAnimation().GetFlipX())
                    {
                        mCurrentMotion = eAbeMotions::Motion_128_KnockForward;
                    }
                }

                if (pAliveObj->GetAnimation().GetFlipX())
                {
                    mVelX = (GetSpriteScale() * FP_FromDouble(-7.8));
                }
                else
                {
                    mVelX = (GetSpriteScale() * FP_FromDouble(7.8));
                }

                SfxPlayMono(relive::SoundEffects::KillEffect, 127);
                if (pAliveObj->Type() != ReliveTypes::eParamite)
                {
                    SfxPlayMono(relive::SoundEffects::FallingItemHit, 90);
                }
            }
            break;

        case ReliveTypes::eAbilityRing:
            return false;

        case ReliveTypes::eRollingBall:
            if (mHealth > FP_FromInteger(0))
            {
                mbMotionChanged = true;
                mHealth = FP_FromInteger(0);
                if (mCurrentMotion == eAbeMotions::Motion_64_LedgeAscend
                    || mCurrentMotion == eAbeMotions::Motion_66_LedgeHang
                    || mCurrentMotion == eAbeMotions::Motion_68_LedgeHangWobble
                    || mCurrentMotion == eAbeMotions::Motion_65_LedgeDescend
                    || mCurrentMotion == eAbeMotions::Motion_67_ToOffScreenHoist)
                {
                    mCurrentMotion = eAbeMotions::Motion_92_ForceDownFromHoist;
                    mbMotionChanged = true;
                    mbGotShot = false;
                    field_114_gnFrame = 0;
                    return true;
                }

                auto pAliveObj = static_cast<BaseAliveGameObject*>(pFrom);

                ToKnockback(1, 1);

                if (pAliveObj->mXPos < mXPos)
                {
                    if (!GetAnimation().GetFlipX())
                    {
                        mCurrentMotion = eAbeMotions::Motion_128_KnockForward;
                    }
                }
                else if (pAliveObj->mXPos > mXPos)
                {
                    if (GetAnimation().GetFlipX())
                    {
                        mCurrentMotion = eAbeMotions::Motion_128_KnockForward;
                    }
                }

                if (pAliveObj->mVelX >= FP_FromInteger(0))
                {
                    mVelX = (GetSpriteScale() * FP_FromDouble(7.8));
                }
                else
                {
                    mVelX = (GetSpriteScale() * FP_FromDouble(-7.8));
                }

                SfxPlayMono(relive::SoundEffects::KillEffect, 127);
            }
            break;

        case ReliveTypes::eUnknown:
            break;

        case ReliveTypes::eZBall:
            if (mHealth > FP_FromInteger(0))
            {
                mbGotShot = true;
                mHealth = FP_FromInteger(0);
                if (static_cast<ZBall*>(pFrom)->mFrameAbove12 == 1)
                {
                    field_112_prev_motion = eAbeMotions::Motion_148_Shot;
                    mNextMotion = eAbeMotions::Motion_148_Shot;
                }
                else
                {
                    field_112_prev_motion = eAbeMotions::Motion_147_ShotRolling;
                    mNextMotion = eAbeMotions::Motion_147_ShotRolling;
                }
            }
            break;

        case ReliveTypes::eBeeSwarm:
            if (mHealth > FP_FromInteger(0))
            {
                mHealth -= FP_FromDouble(0.15);
                if (mHealth > FP_FromInteger(0))
                {
                    const auto rnd_sfx = Math_RandomRange(0, 127) >= 64 ? MudSounds::eBeesStruggle_18 : MudSounds::eKnockbackOuch_10;
                    const FP v16 = (FP_FromInteger(1) - gAbe->mHealth) / FP_FromDouble(0.15);
                    const s16 calc_pitch = Math_RandomRange(200 * FP_GetExponent(v16), 200 * FP_GetExponent(v16) + 1);
                    Mudokon_SFX(rnd_sfx, 0, calc_pitch, this);
                }
                else
                {
                    Mudokon_SFX(MudSounds::eKnockbackOuch_10, 0, 1000, this);
                    Environment_SFX(EnvironmentSfx::eDeathNoise_7, 0, 0x7FFF, this);
                    mHealth = FP_FromInteger(0);
                    mbGotShot = true;
                    field_112_prev_motion = eAbeMotions::Motion_128_KnockForward;
                    mNextMotion = eAbeMotions::Motion_128_KnockForward;
                    if (mCurrentMotion == eAbeMotions::Motion_64_LedgeAscend
                        || mCurrentMotion == eAbeMotions::Motion_66_LedgeHang
                        || mCurrentMotion == eAbeMotions::Motion_68_LedgeHangWobble
                        || mCurrentMotion == eAbeMotions::Motion_65_LedgeDescend
                        || mCurrentMotion == eAbeMotions::Motion_67_ToOffScreenHoist)
                    {
                        mCurrentMotion = eAbeMotions::Motion_92_ForceDownFromHoist;
                        mbMotionChanged = true;
                        mbGotShot = false;
                        field_114_gnFrame = 0;
                        return true;
                    }

                    if (mRidingElum)
                    {
                        ElumKnockForward();
                        mbGotShot = false;
                    }
                }

                if (mCurrentMotion == eAbeMotions::Motion_103_ElumIdle)
                {
                    mCurrentMotion = eAbeMotions::Motion_126_ElumBeesStruggling;
                    mbMotionChanged = true;
                }
            }
            break;

        case ReliveTypes::eElectrocute:
            GetAnimation().SetRender(false);
            ToDieFinal();
            break;

        case ReliveTypes::eBullet:
            // NOTE: This was in the default case! The type may not be bullet in there which would corrupt memory or crash
            BulletDamage(static_cast<Bullet*>(pFrom));
            if (!mbGotShot)
            {
                field_130_say = old_say;
                oldHp = FP_FromInteger(0);
            }
            break;

        default:
            break;
    }

    if (sControlledCharacter->GetPossessed())
    {
        if (mHealth == FP_FromInteger(0))
        {
            sControlledCharacter->VUnPosses();
            mBlockChanting = true;
            sControlledCharacter = this;
        }
    }

    return oldHp > FP_FromInteger(0);
}

static bool IsSameScaleAsHoist(relive::Path_Hoist* pHoist, BaseAliveGameObject* pObj)
{
    auto width = pHoist->Width();

    //The width is used to determine the layer as there is no layer property in the TLV
    if (pObj->GetSpriteScale() == FP_FromInteger(1))
    {
        return width > 18;
    }
    else
    {
        return width <= 18;
    }
}

static bool IsFacingSameDirectionAsHoist(relive::Path_Hoist* pHoist, BaseAliveGameObject* pObj)
{
    if (pHoist->mGrabDirection == relive::Path_Hoist::GrabDirection::eFacingLeft && !pObj->GetAnimation().GetFlipX())
    {
        return false;
    }
    else if (pHoist->mGrabDirection == relive::Path_Hoist::GrabDirection::eFacingRight && pObj->GetAnimation().GetFlipX())
    {
        return false;
    }
    return true;
}

void Abe::TryHoist()
{
    mCurrentMotion = eAbeMotions::Motion_16_HoistBegin;

    auto pHoist = static_cast<relive::Path_Hoist*>(gMap.VTLV_Get_At_Of_Type(
        FP_GetExponent(mXPos),
        FP_GetExponent(mYPos),
        FP_GetExponent(mXPos),
        FP_GetExponent(mYPos),
        ReliveTypes::eHoist));

    if (pHoist && IsSameScaleAsHoist(pHoist, this))
    {
        if (FP_FromInteger(pHoist->Height()) <= GetSpriteScale() * FP_FromInteger(95))
        {
            mCurrentMotion = eAbeMotions::Motion_16_HoistBegin;
        }
        else
        {
            mCurrentMotion = eAbeMotions::Motion_99_HoistBeginLong;
        }
        if (!IsFacingSameDirectionAsHoist(pHoist, this))
        {
            mNextMotion = mCurrentMotion;
            mCurrentMotion = eAbeMotions::Motion_2_StandingTurn;
        }
        BaseAliveGameObjectPathTLV = pHoist;
    }
}

void Abe::Motion_0_Idle()
{
    FollowLift();
    if (Input_IsChanting() && !mBlockChanting)
    {
        if (field_168_ring_pulse_timer && field_16C_bHaveShrykull)
        {
            mCurrentMotion = eAbeMotions::Motion_162_ToShrykull;
        }
        else
        {
            field_114_gnFrame = MakeTimer(90);
            mCurrentMotion = eAbeMotions::Motion_150_Chant;
            SND_SEQ_PlaySeq(SeqId::eMudokonChant1_11, 0, 1);
        }
        field_110_state.raw = 0;
        return;
    }
    if (Input().IsAnyHeld(InputCommands::eLeftGameSpeak | InputCommands::eRightGameSpeak))
    {
        const auto pressed = Input().GetPressed();
        if (pressed & (InputCommands::eHop | InputCommands::eThrowItem | InputCommands::eCrouchOrRoll | InputCommands::eDoAction))
        {
            field_10C_prev_held = pressed;
            mCurrentMotion = eAbeMotions::Motion_58_ToSpeak;
            return;
        }
    }
    if (Input().IsAnyHeld(InputCommands::eHop))
    {
        if (Input().IsAnyHeld(InputCommands::eUp))
        {
            TryHoist();
        }
        else if (!Input().IsAnyHeld(InputCommands::eLeftGameSpeak | InputCommands::eRightGameSpeak))
        {
            mCurrentMotion = eAbeMotions::Motion_29_HopBegin;
            field_1A0_portal = VIntoBirdPortal(2);
            if (field_1A0_portal)
            {
                field_19E_portal_sub_state = PortalSubStates::eJumpingInsidePortal_0;
            }
        }
        return;
    }

    if (ToLeftRightMovement())
    {
        return;
    }

    if (Input().IsAnyHeld(InputCommands::eDown))
    {
        // Check for a lift rope (going down)
        BaseGameObject* pLiftPoint = sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId);
        if (pLiftPoint)
        {
            const FP liftPlatformXMidPoint = FP_FromInteger((BaseAliveGameObjectCollisionLine->mRect.x + BaseAliveGameObjectCollisionLine->mRect.w) / 2);
            const FP halfGrid = ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(2);

            if (pLiftPoint->Type() == ReliveTypes::eLiftPoint && FP_Abs(mXPos - liftPlatformXMidPoint) < halfGrid)
            {
                // AO exclusive - Abe only uses lift facing one side
                if (GetAnimation().GetFlipX())
                {
                    mCurrentMotion = eAbeMotions::Motion_2_StandingTurn;
                    mNextMotion = eAbeMotions::Motion_133_LiftGrabBegin;
                }
                else
                {
                    mCurrentMotion = eAbeMotions::Motion_133_LiftGrabBegin;
                }
                return;
            }
        }


        const auto pHoist = static_cast<relive::Path_Hoist*>(gMap.VTLV_Get_At_Of_Type(
            FP_GetExponent(mXPos),
            FP_GetExponent(mYPos + FP_FromInteger(16)),
            FP_GetExponent(mXPos),
            FP_GetExponent(mYPos + FP_FromInteger(16)),
            ReliveTypes::eHoist));

        if (pHoist)
        {
            if (!IsSameScaleAsHoist(pHoist, this))
            {
                return;
            }

            if (IsFacingSameDirectionAsHoist(pHoist, this))
            {
                mCurrentMotion = eAbeMotions::Motion_65_LedgeDescend;
            }
            else
            {
                mNextMotion = eAbeMotions::Motion_65_LedgeDescend;
                mCurrentMotion = eAbeMotions::Motion_2_StandingTurn;
            }
        }
        else
        {
            mCurrentMotion = eAbeMotions::Motion_21_StandToCrouch;
        }
        return;
    }
    if (Input().IsAnyHeld(InputCommands::eCrouchOrRoll))
    {
        mCurrentMotion = eAbeMotions::Motion_21_StandToCrouch;
    }

    bool handleDoActionOrThrow = false;
    if (Input().IsAnyHeld(InputCommands::eUp))
    {
        BaseGameObject* pLiftPoint = sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId);
        if (pLiftPoint)
        {
            if (pLiftPoint->Type() == ReliveTypes::eLiftPoint)
            {
                const FP halfGrid = ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(2);
                const FP liftPlatformXMidPoint = FP_FromInteger((BaseAliveGameObjectCollisionLine->mRect.x + BaseAliveGameObjectCollisionLine->mRect.w) / 2);
                const FP xPosToMidLiftPlatformDistance = FP_Abs(mXPos - liftPlatformXMidPoint);
                if (xPosToMidLiftPlatformDistance < halfGrid)
                {
                    // AO exclusive - Abe only uses lift facing one side
                    if (GetAnimation().GetFlipX())
                    {
                        mCurrentMotion = eAbeMotions::Motion_2_StandingTurn;
                        mNextMotion = eAbeMotions::Motion_133_LiftGrabBegin;
                    }
                    else
                    {
                        mCurrentMotion = eAbeMotions::Motion_133_LiftGrabBegin;
                    }
                    return;
                }
            }
        }

        mCurrentMotion = TryMountElum();
        if (mCurrentMotion != eAbeMotions::Motion_0_Idle)
        {
            return;
        }
        auto pTlv = gMap.TLV_Get_At(
            nullptr,
            mXPos,
            mYPos,
            mXPos,
            mYPos);

        while (pTlv)
        {
            switch (pTlv->mTlvType)
            {
                case ReliveTypes::eDoor:
                {
                    if (NearDoorIsOpen() && !GetElectrocuted())
                    {
                        BaseAliveGameObjectPathTLV = pTlv;
                        field_110_state.raw = 0;
                        mCurrentMotion = eAbeMotions::Motion_156_DoorEnter;
                    }
                    else
                    {
                        if (Input().IsAnyPressed(InputCommands::eUp))
                        {
                            mCurrentMotion = eAbeMotions::Motion_36_DunnoBegin;
                        }
                    }
                    return;
                }
                case ReliveTypes::eWellLocal:
                {
                    auto well = static_cast<relive::Path_WellLocal*>(pTlv);
                    if ((well->mScale != relive::reliveScale::eFull || GetSpriteScale() != FP_FromInteger(1)) &&
                        (well->mScale != relive::reliveScale::eHalf || GetSpriteScale() != FP_FromDouble(0.5)))
                    {
                        break;
                    }
                    mSnapAbe = false;
                    BaseAliveGameObjectPathTLV = pTlv;
                    mCurrentMotion = eAbeMotions::Motion_77_WellBegin;
                    return;
                }
                case ReliveTypes::eWellExpress:
                {
                    auto well = static_cast<relive::Path_WellExpress*>(pTlv);
                    if ((well->mScale != relive::reliveScale::eFull || GetSpriteScale() != FP_FromInteger(1)) &&
                        (well->mScale != relive::reliveScale::eHalf || GetSpriteScale() != FP_FromDouble(0.5)))
                    {
                        break;
                    }
                    mSnapAbe = false;
                    BaseAliveGameObjectPathTLV = pTlv;
                    mCurrentMotion = eAbeMotions::Motion_80_430EF0;
                    return;
                }
                case ReliveTypes::eMovieHandStone:
                case ReliveTypes::eBellSongStone:
                case ReliveTypes::eDemoPlaybackStone:
                case ReliveTypes::eHandStone:
                {
                    BaseAliveGameObjectPathTLV = pTlv;
                    mCurrentMotion = eAbeMotions::Motion_88_HandstoneBegin;
                    field_110_state.stone = StoneStates::eHandstoneStart_0;
                    return;
                }
                case ReliveTypes::eBoomMachine:
                {
                    auto pMachineButton = static_cast<BoomMachine*>(FindObjectOfType(
                        ReliveTypes::eBoomMachine,
                        mXPos,
                        mYPos - GetSpriteScale() * FP_FromInteger(25)));
                    if (pMachineButton)
                    {
                        pMachineButton->VHandleButton();
                        mCurrentMotion = eAbeMotions::Motion_90_GrenadeMachineUse;
                    }
                    else
                    {
                        mCurrentMotion = eAbeMotions::Motion_36_DunnoBegin;
                    }
                    break;
                }
                default:
                    break;
            }

            pTlv = gMap.TLV_Get_At(
                pTlv,
                mXPos,
                mYPos,
                mXPos,
                mYPos);
        }
        if (mCurrentMotion == eAbeMotions::Motion_0_Idle)
        {
            TryHoist();
        }
        handleDoActionOrThrow = true;
    }

    if (!Input().IsAnyHeld(InputCommands::eUp) || handleDoActionOrThrow)
    {
        if (Input().IsAnyPressed(InputCommands::eThrowItem) && mCurrentMotion == eAbeMotions::Motion_0_Idle)
        {
            if (field_19C_throwable_count > 0 || gInfiniteGrenades)
            {
                mThrowable = Make_Throwable(
                    mXPos,
                    mYPos - FP_FromInteger(40),
                    0);

                if (gThrowableIndicatorExists == 0)
                {
                    const FP xOffSet = GetAnimation().GetFlipX() ? FP_FromInteger(15) : FP_FromInteger(-15) * GetSpriteScale();

                    relive_new ThrowableTotalIndicator(mXPos + xOffSet,
                                                                                    mYPos + GetSpriteScale() * FP_FromInteger(-50),
                                                                                    GetAnimation().GetRenderLayer(),
                                                                                    GetAnimation().GetSpriteScale(),
                                                                                    field_19C_throwable_count,
                                                                                    true);
                }
                mCurrentMotion = eAbeMotions::Motion_142_RockThrowStandingHold;

                if (!gInfiniteGrenades)
                {
                    field_19C_throwable_count--;
                }
            }
            else
            {
                mCurrentMotion = eAbeMotions::Motion_36_DunnoBegin;
            }
        }
        else
        {
            if (Input().IsAnyPressed(InputCommands::eDoAction))
            {
                mCurrentMotion = HandleDoAction();
            }
            else if (gBeeInstanceCount && gBeesNearAbe)
            {
                mCurrentMotion = eAbeMotions::Motion_140_BeesStruggling;
            }
            else if (sGnFrame - field_114_gnFrame > 150)
            {
                field_114_gnFrame = sGnFrame;
                const s16 rnd = Math_RandomRange(0, 2);
                u16 loaded = 0;

                //loaded |= 1;
                //loaded |= 2;
                //loaded |= 4;
                if ((1 << rnd) & loaded)
                {
                    switch (rnd)
                    {
                        case 0:
                        {
                            mCurrentMotion = eAbeMotions::Motion_161_Idle_Yawn;
                            break;
                        }
                        case 1:
                        {
                            mCurrentMotion = eAbeMotions::Motion_159_Idle_RubEyes;
                            break;
                        }
                        case 2:
                        {
                            mCurrentMotion = eAbeMotions::Motion_160_Idle_Stretch_Arms;
                            break;
                        }
                        default:
                            break;
                    }
                }
            }
        }
    }
}

void Abe::Motion_1_WalkLoop()
{
    field_10C_prev_held |= Input().GetHeld();

    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    MoveForward();

    if (mCurrentMotion == eAbeMotions::Motion_1_WalkLoop)
    {
        switch (GetAnimation().GetCurrentFrame())
        {
            case 2:
            {
                if ((mVelX > FP_FromInteger(0) && Input().IsAnyHeld(InputCommands::eLeft)) || (mVelX < FP_FromInteger(0) && Input().IsAnyHeld(InputCommands::eRight)))
                {
                    mCurrentMotion = eAbeMotions::Motion_5_MidWalkToIdle;
                    field_10C_prev_held = 0;
                    return;
                }

                if (field_10C_prev_held & InputCommands::eHop)
                {
                    mNextMotion = eAbeMotions::Motion_30_HopMid;
                    mCurrentMotion = eAbeMotions::Motion_5_MidWalkToIdle;
                    field_10C_prev_held = 0;
                    return;
                }

                if (!Input().IsAnyHeld(InputCommands::eRight | InputCommands::eLeft))
                {
                    mCurrentMotion = eAbeMotions::Motion_5_MidWalkToIdle;
                    field_10C_prev_held = 0;
                }
                else
                {
                    FP directedScale = {};
                    if (GetAnimation().GetFlipX())
                    {
                        directedScale = -ScaleToGridSize(GetSpriteScale());
                    }
                    else
                    {
                        directedScale = ScaleToGridSize(GetSpriteScale());
                    }

                    if (WallHit(GetSpriteScale() * FP_FromInteger(50), directedScale * FP_FromDouble(1.5)))
                    {
                        mCurrentMotion = eAbeMotions::Motion_5_MidWalkToIdle;
                        field_10C_prev_held = 0;
                        return;
                    }
                    else
                    {
                        field_10C_prev_held = 0;
                    }
                }
                break;
            }

            case 11:
                if ((mVelX <= FP_FromInteger(0) || !(Input().IsAnyHeld(InputCommands::eLeft))) && (mVelX >= FP_FromInteger(0) || !(Input().IsAnyHeld(InputCommands::eRight))))
                {
                    if (field_10C_prev_held & InputCommands::eHop)
                    {
                        mNextMotion = eAbeMotions::Motion_30_HopMid;
                    }
                    else if (Input().IsAnyHeld(InputCommands::eRight | InputCommands::eLeft))
                    {
                        FP directedScale = {};
                        if (GetAnimation().GetFlipX())
                        {
                            directedScale = -ScaleToGridSize(GetSpriteScale());
                        }
                        else
                        {
                            directedScale = ScaleToGridSize(GetSpriteScale());
                        }

                        if (!WallHit(GetSpriteScale() * FP_FromInteger(50), directedScale * FP_FromDouble(1.5)))
                        {
                            field_10C_prev_held = 0;
                            return;
                        }
                    }
                }
                mCurrentMotion = eAbeMotions::Motion_4_WalkToIdle;
                field_10C_prev_held = 0;
                return;

            case 5:
                Environment_SFX(EnvironmentSfx::eWalkingFootstep_1, 0, 0x7FFF, this);

                if (!mWalkToRun)
                {
                    mWalkToRun = true;
                    MapFollowMe(1);
                }

                if (Input().IsAnyHeld(InputCommands::eRun))
                {
                    mCurrentMotion = eAbeMotions::Motion_51_MidWalkToRun;
                }
                else if (Input().IsAnyHeld(InputCommands::eSneak))
                {
                    mCurrentMotion = eAbeMotions::Motion_43_WalkToSneak;
                }
                break;

            case 14:
                Environment_SFX(EnvironmentSfx::eWalkingFootstep_1, 0, 0x7FFF, this);

                if (!mWalkToRun)
                {
                    mWalkToRun = true;
                    MapFollowMe(1);
                }

                if (Input().IsAnyHeld(InputCommands::eRun))
                {
                    mCurrentMotion = eAbeMotions::Motion_50_WalkToRun;
                }
                else if (Input().IsAnyHeld(InputCommands::eSneak))
                {
                    mCurrentMotion = eAbeMotions::Motion_45_MidWalkToSneak;
                }
                break;

            default:
                mWalkToRun = false;
                break;
        }
    }
}

void Abe::Motion_2_StandingTurn()
{
    FollowLift();

    if (GetAnimation().GetCurrentFrame() == 4)
    {
        if (Input().IsAnyHeld(InputCommands::eRun) && Input().IsAnyHeld(InputCommands::eRight | InputCommands::eLeft))
        {
#if ORIGINAL_GAME_FIXES || ORIGINAL_GAME_FIX_AUTO_TURN
            mNextMotion = eAbeMotions::Motion_0_Idle; // OG Change - Fixes "Auto-Turn" bug
#endif

            mCurrentMotion = eAbeMotions::Motion_63_TurnToRun;
            GetAnimation().ToggleFlipX();

            if (GetAnimation().GetFlipX())
            {
                mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
            }
            else
            {
                mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
            }
            return;
        }
    }

    if (!GetAnimation().GetCurrentFrame())
    {
        Environment_SFX(EnvironmentSfx::eGenericMovement_9, 0, 0x7FFF, this);
    }

    if (GetAnimation().GetIsLastFrame())
    {
        GetAnimation().ToggleFlipX();

        if (mNextMotion != eAbeMotions::Motion_0_Idle)
        {
            // OG bug: this local variable allows you to "store" your next state if you face the opposite
            // ledge direction and press down/up and then interrupt it by running away before you hoist.
            const eAbeMotions kNext_state = mNextMotion;
            if (mNextMotion != eAbeMotions::Motion_139_ElumMountBegin)
            {
                if (mNextMotion == eAbeMotions::Motion_101_LeverUse)
                {
                    Lever* pSwitch;
                    if (GetAnimation().GetFlipX())
                    {
                        pSwitch = static_cast<Lever*>(FindObjectOfType(ReliveTypes::eLever, mXPos - ScaleToGridSize(GetSpriteScale()), mYPos - FP_FromInteger(5)));
                    }
                    else
                    {
                        pSwitch = static_cast<Lever*>(FindObjectOfType(ReliveTypes::eLever, mXPos + ScaleToGridSize(GetSpriteScale()), mYPos - FP_FromInteger(5)));
                    }

                    if (pSwitch)
                    {
                        pSwitch->VPull(mXPos < pSwitch->mXPos);
                    }
                }
                mNextMotion = eAbeMotions::Motion_0_Idle;
                mCurrentMotion = kNext_state;
                return;
            }

            if (gElum)
            {
                if (gElum->mCurrentMotion == eElumMotions::Motion_1_Idle && !gElum->mStrugglingWithBees)
                {
                    mRidingElum = true;
                    mNextMotion = eAbeMotions::Motion_0_Idle;
                    mCurrentMotion = kNext_state;
                    return;
                }
            }
            mNextMotion = eAbeMotions::Motion_0_Idle;
        }
        else
        {
            if (ToLeftRightMovement())
            {
                GetAnimation().Set_Animation_Data(GetAnimation().mAnimRes);
                return;
            }
        }
        ToIdle();
    }
}

static bool isEdgeGrabbable(relive::Path_Edge* pEdge, BaseAliveGameObject* pObj)
{
    if (pEdge->mGrabDirection == relive::Path_Edge::GrabDirection::eFacingLeft && pObj->GetAnimation().GetFlipX())
    {
        return true;
    }
    else if (pEdge->mGrabDirection == relive::Path_Edge::GrabDirection::eFacingRight && !pObj->GetAnimation().GetFlipX())
    {
        return true;
    }
    else if (pEdge->mGrabDirection == relive::Path_Edge::GrabDirection::eFacingAnyDirection)
    {
        return true;
    }

    return false;
}

void Abe::Motion_3_Fall()
{
    if (mVelX > FP_FromInteger(0))
    {
        mVelX -= (GetSpriteScale() * field_120_x_vel_slow_by);
        if (mVelX < FP_FromInteger(0))
        {
            mVelX = FP_FromInteger(0);
        }
    }
    else if (mVelX < FP_FromInteger(0))
    {
        mVelX += (GetSpriteScale() * field_120_x_vel_slow_by);
        if (mVelX > FP_FromInteger(0))
        {
            mVelX = FP_FromInteger(0);
        }
    }

    FP hitX = {};
    FP hitY = {};
    PathLine* pPathLine = nullptr;
    const s32 bCollision = InAirCollision(&pPathLine, &hitX, &hitY, FP_FromDouble(1.8));
    SetActiveCameraDelayedFromDir();

    BaseAliveGameObjectPathTLV = gMap.TLV_Get_At(
        nullptr,
        mXPos,
        mYPos,
        mXPos,
        mYPos);

    if (BaseAliveGameObjectPathTLV)
    {
        if (mHealth > FP_FromInteger(0))
        {
            if (BaseAliveGameObjectPathTLV->mTlvType == ReliveTypes::eWellLocal || BaseAliveGameObjectPathTLV->mTlvType == ReliveTypes::eWellExpress)
            {
                // The well must be on the same scale/layer
                relive::Path_WellBase* pWellBase = static_cast<relive::Path_WellBase*>(BaseAliveGameObjectPathTLV);
                if ((pWellBase->mScale == relive::reliveScale::eFull && GetSpriteScale() == FP_FromInteger(1)) ||
                    (pWellBase->mScale == relive::reliveScale::eHalf && GetSpriteScale() == FP_FromDouble(0.5)))
                {
                    mSnapAbe = true;
                    mCurrentMotion = eAbeMotions::Motion_74_JumpIntoWell;
                    return;
                }
            }
        }
    }

    if (bCollision)
    {
        switch (pPathLine->mLineType)
        {
            case eLineTypes::eFloor_0:
            case eLineTypes::eBackgroundFloor_4:
            case eLineTypes::eDynamicCollision_32:
            case eLineTypes::eBackgroundDynamicCollision_36:
            {
                mXPos = hitX;
                mYPos = hitY;
                BaseAliveGameObjectCollisionLine = pPathLine;
                MapFollowMe(1);
                field_114_gnFrame = MakeTimer(30);
                if (mYPos - BaseAliveGameObjectLastLineYPos > FP_FromInteger(240))
                {
                    BaseAliveGameObjectLastLineYPos += FP_FromInteger(240);
                }

                relive::Path_SoftLanding* pSoftLanding = static_cast<relive::Path_SoftLanding*>(gMap.VTLV_Get_At_Of_Type(
                    FP_GetExponent(mXPos),
                    FP_GetExponent(mYPos),
                    FP_GetExponent(mXPos),
                    FP_GetExponent(mYPos),
                    ReliveTypes::eSoftLanding));

                if (mLandSoft
                    || (pSoftLanding && mHealth > FP_FromInteger(0))
                    || ((mYPos - BaseAliveGameObjectLastLineYPos) < (GetSpriteScale() * FP_FromInteger(180))
                        && (mHealth > FP_FromInteger(0) || gAbeInvulnerableCheat)))
                {
                    mCurrentMotion = eAbeMotions::Motion_98_LandSoft;
                }
                else
                {
                    mHealth = FP_FromInteger(0);
                    mCurrentMotion = eAbeMotions::Motion_86_FallLandDie;
                    field_11C_regen_health_timer = MakeTimer(900);
                }
                mPreviousMotion = eAbeMotions::Motion_3_Fall;

                PSX_Point xy{FP_GetExponent(mXPos - FP_FromInteger(25)), FP_GetExponent(mYPos - FP_FromInteger(25))};
                PSX_Point wh{FP_GetExponent(mXPos + FP_FromInteger(25)), FP_GetExponent(mYPos + FP_FromInteger(25))};
                OnCollisionWith(
                    xy,
                    wh,
                    gPlatformsArray);
                break;
            }
            case eLineTypes::eWallLeft_1:
            case eLineTypes::eWallRight_2:
            case eLineTypes::eBackgroundWallLeft_5:
            case eLineTypes::eBackgroundWallRight_6:
                mXPos = hitX;
                mYPos = hitY;
                ToKnockback(1, 1);
                break;
            default:
                return;
        }
        return;
    }

    if (mHealth <= FP_FromInteger(0))
    {
        return;
    }

    bool tryToHang = false;
    relive::Path_Edge* pEdge = static_cast<relive::Path_Edge*>(gMap.VTLV_Get_At_Of_Type(
        FP_GetExponent(mXPos),
        FP_GetExponent(mYPos - GetSpriteScale() * FP_FromInteger(80)),
        FP_GetExponent(mXPos),
        FP_GetExponent(mYPos),
        ReliveTypes::eEdge));

    if (pEdge)
    {
        if (pEdge->mCanGrab == relive::reliveChoice::eYes && isEdgeGrabbable(pEdge, this))
        {
            tryToHang = true;
        }
        BaseAliveGameObjectPathTLV = pEdge;
    }
    else
    {
        relive::Path_Hoist* pHoist = static_cast<relive::Path_Hoist*>(gMap.VTLV_Get_At_Of_Type(
            FP_GetExponent(mXPos),
            FP_GetExponent(mYPos - GetSpriteScale() * FP_FromInteger(20)),
            FP_GetExponent(mXPos),
            FP_GetExponent(mYPos - GetSpriteScale() * FP_FromInteger(20)),
            ReliveTypes::eHoist));

        if (pHoist)
        {
            if (IsFacingSameDirectionAsHoist(pHoist, this))
            {
                tryToHang = true;
            }
        }

        BaseAliveGameObjectPathTLV = pHoist;
    }

    if (tryToHang)
    {
        if (mVelX == FP_FromInteger(0))
        {
            return;
        }

        mXPos = FP_FromInteger((BaseAliveGameObjectPathTLV->mTopLeftX + BaseAliveGameObjectPathTLV->mBottomRightX) / 2);

        MapFollowMe(true);

        if (gCollisions->Raycast(
                mXPos,
                mYPos - GetSpriteScale() * FP_FromInteger(20) - GetSpriteScale() * FP_FromInteger(80),
                mXPos,
                mYPos,
                &pPathLine,
                &hitX,
                &hitY,
                GetSpriteScale() != FP_FromDouble(0.5) ? kFgFloor : kBgFloor))
        {
            mVelX = FP_FromInteger(0);
            mVelY = FP_FromInteger(0);
            mYPos = hitY;
            BaseAliveGameObjectCollisionLine = pPathLine;
            mCurrentMotion = eAbeMotions::Motion_68_LedgeHangWobble;
            GetShadow()->mShadowAtBottom = true;
        }
    }
}

void Abe::Motion_4_WalkToIdle()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    MoveForward();

    if (GetAnimation().GetCurrentFrame() != 0)
    {
        if (GetAnimation().GetIsLastFrame())
        {
            MapFollowMe(1);

            if (mNextMotion == eAbeMotions::Motion_30_HopMid)
            {
                mNextMotion = eAbeMotions::Motion_0_Idle;
                mCurrentMotion = eAbeMotions::Motion_29_HopBegin;

                field_1A0_portal = VIntoBirdPortal(2);
                if (field_1A0_portal)
                {
                    field_19E_portal_sub_state = PortalSubStates::eJumpingInsidePortal_0;
                }
            }
            else
            {
                ToIdle();
            }
        }
    }
    else
    {
        Environment_SFX(EnvironmentSfx::eWalkingFootstep_1, 0, 0x7FFF, this);
    }
}

void Abe::Motion_5_MidWalkToIdle()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    MoveForward();

    if (GetAnimation().GetCurrentFrame())
    {
        if (GetAnimation().GetIsLastFrame())
        {
            MapFollowMe(1);
            if (mNextMotion == eAbeMotions::Motion_30_HopMid)
            {
                mNextMotion = eAbeMotions::Motion_0_Idle;
                mCurrentMotion = eAbeMotions::Motion_29_HopBegin;
                field_1A0_portal = VIntoBirdPortal(2);
                if (field_1A0_portal)
                {
                    field_19E_portal_sub_state = PortalSubStates::eJumpingInsidePortal_0;
                }
            }
            else
            {
                ToIdle();
            }
        }
    }
    else
    {
        Environment_SFX(EnvironmentSfx::eWalkingFootstep_1, 0, 0x7FFF, this);
    }
}

void Abe::Motion_6_WalkBegin()
{
    field_10C_prev_held |= Input().GetHeld();

    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eAbeMotions::Motion_1_WalkLoop;
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
    {
        ToIdle();
    }
    else
    {
        MoveForward();
    }
}

void Abe::Motion_7_Speak()
{
    Motion_58_ToSpeak();
}

void Abe::Motion_8_Speak()
{
    Motion_58_ToSpeak();
}

void Abe::Motion_9_Speak()
{
    Motion_58_ToSpeak();
}

void Abe::Motion_10_Speak()
{
    Motion_58_ToSpeak();
}

void Abe::Motion_11_Speak()
{
    Motion_58_ToSpeak();
}

void Abe::Motion_12_Speak()
{
    Motion_58_ToSpeak();
}

void Abe::Motion_13_Speak()
{
    Motion_58_ToSpeak();
}

void Abe::Motion_14_Speak()
{
    Motion_58_ToSpeak();
}

void Abe::Motion_15_Null()
{
    // Empty
}

void Abe::Motion_16_HoistBegin()
{
    FollowLift();

    if (GetAnimation().GetIsLastFrame())
    {
        BaseAliveGameObjectLastLineYPos = mYPos;
        const FP velY = GetSpriteScale() * FP_FromInteger(-8);
        mVelY = velY;
        mYPos += velY;
        VOnTrapDoorOpen();
        mCurrentMotion = eAbeMotions::Motion_17_HoistIdle;
        BaseAliveGameObjectCollisionLine = nullptr;
    }
}

void Abe::Motion_17_HoistIdle()
{
    BaseGameObject* pPlatform = sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId);

    PathLine* pPathLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    const auto bCollision = InAirCollision(&pPathLine, &hitX, &hitY, FP_FromDouble(1.8));
    SetActiveCameraDelayedFromDir();
    if (bCollision)
    {
        switch (pPathLine->mLineType)
        {
            case eLineTypes::eFloor_0:
            case eLineTypes::eBackgroundFloor_4:
            case eLineTypes::eDynamicCollision_32:
            case eLineTypes::eBackgroundDynamicCollision_36:
            {
                mXPos = hitX;
                mYPos = hitY;

                MapFollowMe(1);

                BaseAliveGameObjectCollisionLine = pPathLine;

                mCurrentMotion = eAbeMotions::Motion_18_HoistLand;
                mPreviousMotion = eAbeMotions::Motion_17_HoistIdle;

                PSX_RECT rect = VGetBoundingRect();
                rect.y += 5;
                rect.h += 5;

                OnCollisionWith(
                    {rect.x, rect.y},
                    {rect.w, rect.h},
                    gPlatformsArray);
                break;
            }
        }
        return;
    }

    mPullRingRope = GetPullRope();
    if (mPullRingRope)
    {
        if (mPullRingRope->Pull(this))
        {
            mCurrentMotion = eAbeMotions::Motion_69_RingRopePullHang;
            mNextMotion = eAbeMotions::Motion_0_Idle;
            mPullRingRope->mBaseGameObjectRefCount++;
            return;
        }
        mPullRingRope = nullptr;
    }

    if (mVelY >= FP_FromInteger(0))
    {
        auto pHoist = static_cast<relive::Path_Hoist*>(gMap.VTLV_Get_At_Of_Type(
            FP_GetExponent(mXPos),
            FP_GetExponent(mYPos),
            FP_GetExponent(mXPos),
            FP_GetExponent(mYPos),
            ReliveTypes::eHoist));
        BaseAliveGameObjectPathTLV = pHoist;
        if (pHoist && IsSameScaleAsHoist(pHoist, this))
        {
            if (IsFacingSameDirectionAsHoist(pHoist, this))
            {
                if (pHoist->mHoistType == relive::Path_Hoist::Type::eOffScreen)
                {
                    if (gMap.SetActiveCameraDelayed(MapDirections::eMapTop_2, this, -1))
                    {
                        PSX_Prevent_Rendering();
                        mCurrentMotion = eAbeMotions::Motion_67_ToOffScreenHoist;
                        return;
                    }
                    mYPos -= GetSpriteScale() * FP_FromInteger(80);
                    GetShadow()->mShadowAtBottom = true;
                }

                mCurrentMotion = eAbeMotions::Motion_66_LedgeHang;
                Environment_SFX(EnvironmentSfx::eWalkingFootstep_1, 0, 127, this);


                if (gCollisions->Raycast(
                        mXPos,
                        mYPos - GetSpriteScale() * FP_FromInteger(80),
                        mXPos,
                        mYPos,
                        &pPathLine,
                        &hitX,
                        &hitY,
                        GetSpriteScale() != FP_FromDouble(0.5) ? kFgFloor : kBgFloor))
                {
                    mYPos = hitY;
                    BaseAliveGameObjectCollisionLine = pPathLine;
                    mVelY = FP_FromInteger(0);
                    if (!pPlatform)
                    {
                        if (pPathLine->mLineType == eLineTypes::eDynamicCollision_32 ||
                            pPathLine->mLineType == eLineTypes::eBackgroundDynamicCollision_36)
                        {
                            PSX_RECT rect = VGetBoundingRect();
                            rect.y += 5;
                            rect.h += 5;

                            OnCollisionWith(
                                {rect.x, rect.y},
                                {rect.w, rect.h},
                                gPlatformsArray);
                        }
                    }
                    GetShadow()->mShadowAtBottom = true;
                }
                else
                {
                    mCurrentMotion = eAbeMotions::Motion_17_HoistIdle;
                }
            }
        }
        else
        {
            BaseAliveGameObjectPathTLV = gMap.TLV_Get_At(
                nullptr,
                mXPos,
                mYPos,
                mXPos,
                mYPos);
        }
    }
}

void Abe::Motion_18_HoistLand()
{
    FollowLift();

    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (GetAnimation().GetCurrentFrame() == 2)
    {
        if (mPreviousMotion == eAbeMotions::Motion_3_Fall)
        {
            Environment_SFX(EnvironmentSfx::eLandingSoft_5, 0, 0x7FFF, this);
        }
        else
        {
            Environment_SFX(EnvironmentSfx::eHitGroundSoft_6, 0, 0x7FFF, this);
        }

        if (Input().IsAnyHeld(InputCommands::eHop))
        {
            if (Input().IsAnyHeld(InputCommands::eUp))
            {
                mCurrentMotion = eAbeMotions::Motion_16_HoistBegin;
            }
            else
            {
                mCurrentMotion = eAbeMotions::Motion_29_HopBegin;
                field_1A0_portal = VIntoBirdPortal(2);
                if (field_1A0_portal)
                {
                    field_19E_portal_sub_state = PortalSubStates::eJumpingInsidePortal_0;
                }
            }
        }
    }

    if (GetAnimation().GetIsLastFrame())
    {
        ToIdle();
    }
}

void Abe::Motion_19_CrouchIdle()
{
    if (!BaseAliveGameObjectCollisionLine)
    {
        mCurrentMotion = eAbeMotions::Motion_100_RollOffLedge;
        return;
    }

    FollowLift();

    if (Input().IsAnyPressed(InputCommands::eLeftGameSpeak) && Input().IsAnyPressed(InputCommands::eRightGameSpeak))
    {
        Mudokon_SFX(MudSounds::eDunno_15, 0, 0, this);
        mCurrentMotion = eAbeMotions::Motion_23_CrouchSpeak;
        return;
    }

    // Crouching game speak
    CrouchingGameSpeak();

    field_10C_prev_held = 0;
    field_10E_released_buttons = 0;

    if (mCurrentMotion == eAbeMotions::Motion_22_CrouchSpeak || mCurrentMotion == eAbeMotions::Motion_23_CrouchSpeak)
    {
        EventBroadcast(kEventSpeaking, this);
        return;
    }


    // Hit bombs/pick up items ?
    if (Input().IsAnyPressed(InputCommands::eDoAction))
    {
        if (!Input().IsAnyHeld(InputCommands::eRight | InputCommands::eLeft))
        {
            FP gridSize = {};
            if (GetAnimation().GetFlipX())
            {
                gridSize = -ScaleToGridSize(GetSpriteScale());
            }
            else
            {
                gridSize = ScaleToGridSize(GetSpriteScale());
            }

            PickUpThrowabe_Or_PressBomb(
                gridSize + mXPos,
                FP_GetExponent(mYPos - FP_FromInteger(5)),
                0);
        }
    }


    // Crouching throw stuff
    if (Input().IsAnyPressed(InputCommands::eThrowItem))
    {
        if (mCurrentMotion == eAbeMotions::Motion_19_CrouchIdle)
        {
            if (field_19C_throwable_count > 0 || gInfiniteGrenades)
            {
                mThrowable = Make_Throwable(
                    mXPos,
                    mYPos - FP_FromInteger(40),
                    0);

                if (!gThrowableIndicatorExists)
                {
                    const FP yOff = mYPos + (GetSpriteScale() * FP_FromInteger(-30));
                    const FP xOff = GetSpriteScale() * (GetAnimation().GetFlipX() ? FP_FromInteger(-10) : FP_FromInteger(10));
                    relive_new ThrowableTotalIndicator(mXPos + xOff,
                                                                             yOff,
                                                                             GetAnimation().GetRenderLayer(),
                                                                             GetAnimation().GetSpriteScale(),
                                                                             field_19C_throwable_count,
                                                                             1);
                }

                mCurrentMotion = eAbeMotions::Motion_145_RockThrowCrouchingHold;

                if (!gInfiniteGrenades)
                {
                    field_19C_throwable_count--;
                }

                return;
            }
        }

        if (!field_19C_throwable_count && !gInfiniteGrenades)
        {
            Mudokon_SFX(MudSounds::eDunno_15, 0, 0, this);
            mCurrentMotion = eAbeMotions::Motion_23_CrouchSpeak;
            return;
        }
    }

    // Try to stand up
    if (Input().IsAnyPressed(InputCommands::eUp) || Input().IsAnyPressed(InputCommands::eCrouchOrRoll))
    {
        PathLine* pLine = nullptr;
        FP hitX = {};
        FP hitY = {};
        if (!gCollisions->Raycast(
                mXPos,
                mYPos,
                mXPos,
                mYPos - (GetSpriteScale() * FP_FromInteger(60)),
                &pLine,
                &hitX,
                &hitY,
                GetSpriteScale() != FP_FromDouble(0.5) ? kFgCeiling : kBgCeiling))
        {
            mCurrentMotion = eAbeMotions::Motion_20_CrouchToStand;
            return;
        }
    }

    // Crouching turns
    if (Input().IsAnyHeld(InputCommands::eRight))
    {
        if (GetAnimation().GetFlipX())
        {
            mCurrentMotion = eAbeMotions::Motion_39_CrouchTurn;
        }
        else
        {
            mCurrentMotion = eAbeMotions::Motion_24_RollBegin;
            field_10E_released_buttons = 0;
        }
    }

    if (Input().IsAnyHeld(InputCommands::eLeft))
    {
        if (GetAnimation().GetFlipX())
        {
            mCurrentMotion = eAbeMotions::Motion_24_RollBegin;
            field_10E_released_buttons = 0;
        }
        else
        {
            mCurrentMotion = eAbeMotions::Motion_39_CrouchTurn;
        }
    }
}

void Abe::Motion_20_CrouchToStand()
{
    FollowLift();

    if (GetAnimation().GetCurrentFrame() == 3)
    {
        if (Input().IsAnyHeld(InputCommands::eLeft | InputCommands::eRight))
        {
            ToLeftRightMovement();
        }
    }

    if (GetAnimation().GetIsLastFrame())
    {
        ToIdle();
    }
}

void Abe::Motion_21_StandToCrouch()
{
    FollowLift();

    if (GetAnimation().GetIsLastFrame())
    {
        PickUpThrowabe_Or_PressBomb(
            mXPos,
            FP_GetExponent(mYPos - FP_FromInteger(5)),
            1);
        mCurrentMotion = eAbeMotions::Motion_19_CrouchIdle;
    }
}

void Abe::Motion_22_CrouchSpeak()
{
    Motion_23_CrouchSpeak();
}

void Abe::Motion_23_CrouchSpeak()
{
    field_10C_prev_held |= Input().GetPressed();

    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eAbeMotions::Motion_19_CrouchIdle;

        CrouchingGameSpeak();

        if (mCurrentMotion == eAbeMotions::Motion_22_CrouchSpeak || mCurrentMotion == eAbeMotions::Motion_23_CrouchSpeak)
        {
            EventBroadcast(kEventSpeaking, this);
        }

        field_10C_prev_held = 0;
    }
}

void Abe::Motion_24_RollBegin()
{
    if (GetAnimation().GetFlipX())
    {
        mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
    }
    else
    {
        mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(20), mVelX))
    {
        ToKnockback(1, 1);

        mCurrentMotion = eAbeMotions::Motion_73_RollingKnockback;
    }
    else
    {
        MoveForward();

        if (GetAnimation().GetIsLastFrame())
        {
            if (mCurrentMotion == eAbeMotions::Motion_24_RollBegin)
            {
                mCurrentMotion = eAbeMotions::Motion_25_RollLoop;
            }
        }
    }
}

bool Abe::Is_Celling_Above()
{
    FP hitY = {};
    FP hitX = {};
    PathLine* pLine = nullptr;
    return gCollisions->Raycast(
               mXPos,
               mYPos,
               mXPos,
               mYPos - (GetSpriteScale() * FP_FromInteger(60)),
               &pLine,
               &hitX,
               &hitY,
               GetSpriteScale() != FP_FromDouble(0.5) ? kFgCeiling : kBgCeiling)
        != 0;
}

void Abe::Motion_25_RollLoop()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    field_10E_released_buttons |= Input().GetReleased();

    if (WallHit(GetSpriteScale() * FP_FromInteger(20), mVelX))
    {
        ToKnockback(1, 1);
        mCurrentMotion = eAbeMotions::Motion_73_RollingKnockback;
    }
    else
    {
        MoveForward();

        if (mCurrentMotion == eAbeMotions::Motion_25_RollLoop)
        {
            if (GetAnimation().GetCurrentFrame() == 1 || GetAnimation().GetCurrentFrame() == 5 || GetAnimation().GetCurrentFrame() == 9)
            {
                if (!Input().IsAnyHeld(InputCommands::eRun)
                    || Input().IsAnyHeld(InputCommands::eCrouchOrRoll)
                    || Is_Celling_Above()
                    || field_12C_timer + 9 >= static_cast<s32>(sGnFrame))
                {
                    // TODO: Investigate this logic as it seems wrong - but it is what OG is doing
                    // seems like it should be checking for crouch being released to come out of roll
                    // rather than any key, the always true part of the check is removed by the compiler in
                    // AE
                    if (field_10E_released_buttons && InputCommands::eCrouchOrRoll)
                    {
                        if (!Is_Celling_Above() && field_12C_timer + 9 < static_cast<s32>(sGnFrame))
                        {
                            ToLeftRightMovement();
                            field_10E_released_buttons = 0;
                        }
                    }
                }
                else
                {
                    ToLeftRightMovement();
                    field_10C_prev_held = 0;
                }
            }
            else if (GetAnimation().GetCurrentFrame() == 0 || GetAnimation().GetCurrentFrame() == 4 || GetAnimation().GetCurrentFrame() == 8)
            {
                MapFollowMe(true);

                if ((mVelX > FP_FromInteger(0) && !Input().IsAnyHeld(InputCommands::eRight)) || (mVelX < FP_FromInteger(0) && !Input().IsAnyHeld(InputCommands::eLeft)))
                {
                    mCurrentMotion = eAbeMotions::Motion_19_CrouchIdle;
                    mVelX = FP_FromInteger(0);
                }
            }

            if (GetAnimation().GetCurrentFrame() == 0 || GetAnimation().GetCurrentFrame() == 6)
            {
                Environment_SFX(EnvironmentSfx::eRollingNoise_8, 0, 0x7FFF, this);
            }
        }
    }
}

void Abe::Motion_26_RollEnd()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (WallHit(GetSpriteScale() * FP_FromInteger(20), mVelX))
    {
        ToKnockback(1, 1);
        mCurrentMotion = eAbeMotions::Motion_73_RollingKnockback;
    }
    else
    {
        MoveForward();

        if (mCurrentMotion == eAbeMotions::Motion_26_RollEnd)
        {
            if (GetAnimation().GetIsLastFrame())
            {
                MapFollowMe(1);
                mCurrentMotion = eAbeMotions::Motion_19_CrouchIdle;
                mVelX = FP_FromInteger(0);
            }
        }
    }
}

void Abe::Motion_27_RunSlideStop()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
    {
        ToKnockback(1, 1);
    }
    else
    {
        MoveWithVelocity(FP_FromDouble(0.375));
        if (mCurrentMotion == eAbeMotions::Motion_27_RunSlideStop
            && !RunTryEnterWell()
            && !RunTryEnterDoor())
        {
            if (GetAnimation().GetCurrentFrame() >= 9)
            {
                if (GetAnimation().GetCurrentFrame() == 15)
                {
                    Environment_SFX(EnvironmentSfx::eSlideStop_0, 0, 0x7FFF, this);
                    MapFollowMe(1);

                    if (!ToLeftRightMovement())
                    {
                        ToIdle();
                    }
                }
            }
            else if ((GetAnimation().GetFlipX() && Input().IsAnyHeld(InputCommands::eRight)) || (!GetAnimation().GetFlipX() && Input().IsAnyHeld(InputCommands::eLeft)))
            {
                mReturnToPreviousMotion = true;
                mPreviousMotion = eAbeMotions::Motion_28_RunTurn;
                mBaseAliveGameObjectLastAnimFrame = GetAnimation().GetCurrentFrame();
            }
        }
    }
}

void Abe::Motion_28_RunTurn()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
    {
        ToKnockback(1, 1);
    }
    else
    {
        MoveWithVelocity(FP_FromDouble(0.375));
        if (mCurrentMotion == eAbeMotions::Motion_28_RunTurn
            && !RunTryEnterWell()
            && !RunTryEnterDoor())
        {
            if (GetAnimation().GetIsLastFrame())
            {
                MapFollowMe(true);

                const FP gridSize = ScaleToGridSize(GetSpriteScale());
                if (GetAnimation().GetFlipX())
                {
                    if (Input().IsAnyHeld(InputCommands::eRun))
                    {
                        mVelX = gridSize / FP_FromInteger(4);
                        mCurrentMotion = eAbeMotions::Motion_54_RunTurnToRun;
                    }
                    else
                    {
                        mVelX = gridSize / FP_FromInteger(9);
                        mCurrentMotion = eAbeMotions::Motion_55_RunTurnToWalk;
                    }
                }
                else
                {
                    FP velX = {};
                    if (Input().IsAnyHeld(InputCommands::eRun))
                    {
                        velX = gridSize / FP_FromInteger(4);
                        mCurrentMotion = eAbeMotions::Motion_54_RunTurnToRun;
                    }
                    else
                    {
                        velX = gridSize / FP_FromInteger(9);
                        mCurrentMotion = eAbeMotions::Motion_55_RunTurnToWalk;
                    }
                    mVelX = -velX;
                }
            }
        }
    }
}

void Abe::Motion_29_HopBegin()
{
    FollowLift();

    if (GetAnimation().GetCurrentFrame() == 9)
    {
        const FP velX = GetSpriteScale() * (GetAnimation().GetFlipX() ? FP_FromInteger(-17) : FP_FromInteger(17));
        mVelX = velX;

        if (!field_1A0_portal)
        {
            if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
            {
                EventBroadcast(kEventNoise, this);
                EventBroadcast(kEventSuspiciousNoise, this);
                mVelX = FP_FromInteger(0);
                ToKnockback(1, 1);
                return;
            }
        }
        mXPos += mVelX;
    }

    if (GetAnimation().GetIsLastFrame())
    {
        BaseAliveGameObjectLastLineYPos = mYPos;
        const FP velX = GetSpriteScale() * (GetAnimation().GetFlipX() ? FP_FromDouble(-13.57) : FP_FromDouble(13.57));
        mVelX = velX;
        mXPos += velX;

        const FP velY = GetSpriteScale() * FP_FromDouble(-2.7);
        mVelY = velY;
        mYPos += velY;

        VOnTrapDoorOpen();

        mCurrentMotion = eAbeMotions::Motion_30_HopMid;
        BaseAliveGameObjectCollisionLine = nullptr;

        if (!field_1A0_portal)
        {
            field_1A0_portal = VIntoBirdPortal(2);
            if (field_1A0_portal)
            {
                field_19E_portal_sub_state = PortalSubStates::eJumpingInsidePortal_0;
            }
        }
    }
}

void Abe::IntoPortalStates()
{
    switch (field_19E_portal_sub_state)
    {
        case PortalSubStates::eJumpingInsidePortal_0:
        {
            PSX_RECT bRect = VGetBoundingRect();
            if ((mVelX > FP_FromInteger(0) && FP_FromInteger(bRect.x) > field_1A0_portal->mXPos) || (mVelX < FP_FromInteger(0) && FP_FromInteger(bRect.w) < field_1A0_portal->mXPos))
            {
                GetAnimation().SetRender(false);
                mVelY = FP_FromInteger(0);
                mVelX = FP_FromInteger(0);
                field_1A0_portal->VKillPortalClipper();
                field_1A0_portal->VGiveShrykull(true);
                field_19E_portal_sub_state = PortalSubStates::eSetNewActiveCamera_1;
            }
            mVelY += GetSpriteScale() * FP_FromDouble(1.8);
            mXPos += mVelX;
            mYPos += mVelY;
            return;
        }
        case PortalSubStates::eSetNewActiveCamera_1:
        {
            if (field_1A0_portal->VAbeInsidePortal())
            {
                EReliveLevelIds level = {};
                u16 path = 0;
                u16 camera = 0;
                CameraSwapEffects screenChangeEffect = {};
                u16 movieId = 0;
                field_1A0_portal->VGetMapChange(&level, &path, &camera, &screenChangeEffect, &movieId);
                gMap.SetActiveCam(level, path, camera, screenChangeEffect, movieId, false);
                field_19E_portal_sub_state = PortalSubStates::eSetNewAbePosition_4;
            }
            break;
        }
        case PortalSubStates::eHopOutOfPortal_2:
        {
            if (field_1A0_portal->VPortalExit_AbeExitting())
            {
                field_1A0_portal->VPortalClipper(0);
                GetAnimation().SetRender(true);
                mCurrentMotion = eAbeMotions::Motion_29_HopBegin;
                field_1A0_portal->VIncreaseTimerAndKillPortalClipper();
                field_1A0_portal = 0;
            }
            break;
        }
        case PortalSubStates::eSetNewAbePosition_4:
        {
            field_1A0_portal->VExitPortal();
            field_19E_portal_sub_state = PortalSubStates::eHopOutOfPortal_2;
            GetAnimation().SetFlipX(field_1A0_portal->mEnterSide == relive::Path_BirdPortal::PortalSide::eLeft);

            if (GetAnimation().GetFlipX())
            {
                mXPos = ScaleToGridSize(GetSpriteScale()) + field_1A0_portal->mExitX;
            }
            else
            {
                mXPos = field_1A0_portal->mExitX - ScaleToGridSize(GetSpriteScale());
            }

            mYPos = field_1A0_portal->mExitY;
            mVelY = FP_FromInteger(0);
            field_120_x_vel_slow_by = FP_FromInteger(0);
            break;
        }
        default:
            return;
    }
}

void Abe::Motion_30_HopMid()
{
    if (field_1A0_portal)
    {
        IntoPortalStates();
    }
    else
    {
        if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
        {
            EventBroadcast(kEventNoise, this);
            EventBroadcast(kEventSuspiciousNoise, this);
            if (gMap.VTLV_Get_At_Of_Type(
                    FP_GetExponent(mVelX + mXPos),
                    FP_GetExponent(mYPos - GetSpriteScale() * FP_FromInteger(50)),
                    FP_GetExponent(mVelX + mXPos),
                    FP_GetExponent(mYPos - GetSpriteScale() * FP_FromInteger(50)),
                    ReliveTypes::eElumStart))
            {
                SfxPlayMono(relive::SoundEffects::RingBellHammer, 0);
            }
            mNextMotion = eAbeMotions::Motion_0_Idle;
            ToKnockback(1, 1);
        }
        else
        {
            PathLine* pLine = nullptr;
            FP hitX = {};
            FP hitY = {};

            // this has to be called before SetActiveCameraDelayedFromDir,
            // due to both of them modifying the same private fields in a fixed order
            bool hasCollidedWithAir = InAirCollision(&pLine, &hitX, &hitY, FP_FromDouble(1.80));

            SetActiveCameraDelayedFromDir();

            if (hasCollidedWithAir)
            {
                EventBroadcast(kEventNoise, this);
                EventBroadcast(kEventSuspiciousNoise, this);
                switch (pLine->mLineType)
                {
                    case eLineTypes::eFloor_0:
                    case eLineTypes::eBackgroundFloor_4:
                    case eLineTypes::eDynamicCollision_32:
                    case eLineTypes::eBackgroundDynamicCollision_36:
                    {
                        Environment_SFX(EnvironmentSfx::eHitGroundSoft_6, 0, 0x7FFF, this);
                        BaseAliveGameObjectCollisionLine = pLine;
                        mVelY = FP_FromInteger(0);
                        mCurrentMotion = eAbeMotions::Motion_31_HopLand;

                        PSX_RECT rect = VGetBoundingRect();
                        rect.y += 5;
                        rect.h += 5;

                        OnCollisionWith(
                            {rect.x, rect.y},
                            {rect.w, rect.h},
                            gPlatformsArray);
                        mXPos = hitX;
                        mYPos = hitY;
                        mVelX = FP_FromInteger(0);
                        MapFollowMe(1);
                        mNextMotion = eAbeMotions::Motion_0_Idle;
                        return;
                    }
                    default:
                        break;
                }
            }
            if (GetAnimation().GetIsLastFrame())
            {
                if (mCurrentMotion == eAbeMotions::Motion_30_HopMid)
                {
                    mVelX = FP_FromRaw(mVelX.fpValue / 2);
                    if (GetAnimation().GetFlipX())
                    {
                        mXPos += GetSpriteScale() * FP_FromInteger(5);
                    }
                    else
                    {
                        mXPos -= GetSpriteScale() * FP_FromInteger(5);
                    }
                    field_120_x_vel_slow_by = FP_FromDouble(0.55);
                    mYPos += GetSpriteScale() * FP_FromInteger(2);
                    mCurrentMotion = eAbeMotions::Motion_96_HopToFall;
                    mNextMotion = eAbeMotions::Motion_0_Idle;
                }
            }
        }
    }
}

void Abe::Motion_31_HopLand()
{
    FollowLift();

    if (GetAnimation().GetCurrentFrame() == 2 && Input().IsAnyHeld(InputCommands::eHop))
    {
        mReturnToPreviousMotion = true;
        mPreviousMotion = eAbeMotions::Motion_29_HopBegin;
        mBaseAliveGameObjectLastAnimFrame = 5;
    }
    else
    {
        if (GetAnimation().GetIsLastFrame())
        {
            if (!ToLeftRightMovement())
            {
                ToIdle();
            }
        }
    }
}

void Abe::Motion_32_RunJumpBegin()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
    {
        ToKnockback(1, 1);
    }
    else
    {
        FollowLift();

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
                mVelX = (GetSpriteScale() * FP_FromDouble(-7.6));
            }
            else
            {
                mVelX = (GetSpriteScale() * FP_FromDouble(7.6));
            }

            mVelY = (GetSpriteScale() * FP_FromDouble(-9.6));
            mYPos += mVelY;
            VOnTrapDoorOpen();
            mCurrentMotion = eAbeMotions::Motion_33_RunJumpMid;
            BaseAliveGameObjectCollisionLine = nullptr;
        }
    }
}

void Abe::Motion_33_RunJumpMid()
{
    BaseGameObject* pLiftPoint = sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId);

    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (field_1A0_portal)
    {
        IntoPortalStates();
        return;
    }
    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
    {
        if (gMap.VTLV_Get_At_Of_Type(
                FP_GetExponent(mVelX + mXPos),
                FP_GetExponent(mYPos - GetSpriteScale() * FP_FromInteger(50)),
                FP_GetExponent(mVelX + mXPos),
                FP_GetExponent(mYPos - GetSpriteScale() * FP_FromInteger(50)),
                ReliveTypes::eElumStart))
        {
            SfxPlayMono(relive::SoundEffects::RingBellHammer, 0);
        }
        mNextMotion = eAbeMotions::Motion_0_Idle;
        ToKnockback(1, 1);
        return;
    }

    FP hitX = {};
    FP hitY = {};
    PathLine* pLine = nullptr;
    auto bCollision = InAirCollision(&pLine, &hitX, &hitY, FP_FromDouble(1.8));

    SetActiveCameraDelayedFromDir();

    if (bCollision)
    {
        switch (pLine->mLineType)
        {
            case eLineTypes::eFloor_0:
            case eLineTypes::eBackgroundFloor_4:
            case eLineTypes::eDynamicCollision_32:
            case eLineTypes::eBackgroundDynamicCollision_36:
            {
                BaseAliveGameObjectCollisionLine = pLine;
                mCurrentMotion = eAbeMotions::Motion_34_RunJumpLand;
                mXPos = hitX;
                mYPos = hitY;

                PSX_RECT rect = VGetBoundingRect();
                rect.y += 5;
                rect.h += 5;

                if (pLine->mLineType == eLineTypes::eDynamicCollision_32)
                {
                    OnCollisionWith(
                        {rect.x, rect.y},
                        {rect.w, rect.h},
                        gPlatformsArray);
                }
                mNextMotion = eAbeMotions::Motion_0_Idle;
                return;
            }
        }
    }
    else
    {
        auto pHoist = static_cast<relive::Path_Hoist*>(gMap.VTLV_Get_At_Of_Type(
            FP_GetExponent(mXPos - mVelX),
            FP_GetExponent(mYPos),
            FP_GetExponent(mXPos - mVelX),
            FP_GetExponent(mYPos),
            ReliveTypes::eHoist));

        bool checkCollision = false;
        if (pHoist)
        {
            BaseAliveGameObjectPathTLV = pHoist;

            if (IsSameScaleAsHoist(pHoist, this) && (IsFacingSameDirectionAsHoist(pHoist, this)) && pHoist->mHoistType != relive::Path_Hoist::Type::eOffScreen)
            {
                checkCollision = true;
            }
        }
        else
        {
            auto pEdgeTlv = static_cast<relive::Path_Edge*>(gMap.VTLV_Get_At_Of_Type(
                FP_GetExponent(mXPos - mVelX),
                FP_GetExponent(mYPos),
                FP_GetExponent(mXPos - mVelX),
                FP_GetExponent(mYPos),
                ReliveTypes::eEdge));
            BaseAliveGameObjectPathTLV = pEdgeTlv;
            if (pEdgeTlv && pEdgeTlv->mCanGrab == relive::reliveChoice::eYes)
            {
                if (isEdgeGrabbable(pEdgeTlv, this))
                {
                    checkCollision = true;
                }
            }
        }
        if (checkCollision)
        {
            if (gCollisions->Raycast(
                    mXPos,
                    mYPos - GetSpriteScale() * FP_FromInteger(100),
                    mXPos,
                    mYPos,
                    &pLine,
                    &hitX,
                    &hitY,
                    GetSpriteScale() != FP_FromDouble(0.5) ? kFgFloor : kBgFloor))
            {
                mXPos = FP_FromInteger(
                    (BaseAliveGameObjectPathTLV->mBottomRightX + BaseAliveGameObjectPathTLV->mTopLeftX) / 2);
                MapFollowMe(true);
                mYPos = hitY;
                mCurrentMotion = eAbeMotions::Motion_68_LedgeHangWobble;
                GetShadow()->mShadowAtBottom = true;
                BaseAliveGameObjectCollisionLine = pLine;
                mVelX = FP_FromInteger(0);
                mVelY = FP_FromInteger(0);
                mNextMotion = eAbeMotions::Motion_0_Idle;
                if (!pLiftPoint)
                {
                    if (pLine->mLineType == eLineTypes ::eDynamicCollision_32 ||
                        pLine->mLineType == eLineTypes::eBackgroundDynamicCollision_36)
                    {
                        PSX_RECT rect = VGetBoundingRect();
                        rect.y += 5;
                        rect.h += 5;

                        OnCollisionWith(
                            {rect.x, rect.y},
                            {rect.w, rect.h},
                            gPlatformsArray);
                    }
                }
            }
        }
        else
        {
            BaseAliveGameObjectPathTLV = gMap.TLV_Get_At(
                nullptr,
                mXPos,
                mYPos,
                mXPos,
                mYPos);
        }
    }

    if (GetAnimation().GetIsLastFrame())
    {
        if (mCurrentMotion == eAbeMotions::Motion_33_RunJumpMid)
        {
            field_120_x_vel_slow_by = FP_FromDouble(0.75);
            mCurrentMotion = eAbeMotions::Motion_97_RunJumpToFall;
            mNextMotion = eAbeMotions::Motion_0_Idle;
        }
    }
}

void Abe::Motion_34_RunJumpLand()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (GetAnimation().GetIsLastFrame())
    {
        Environment_SFX(EnvironmentSfx::eHitGroundSoft_6, 0, 0x7FFF, this);
        MapFollowMe(1);

        if (Input().IsAnyHeld(InputCommands::eLeft))
        {
            if (!(InputCommands::eHop & field_10C_prev_held))
            {
                if (GetAnimation().GetFlipX())
                {
                    if (Input().IsAnyHeld(InputCommands::eRun))
                    {
                        mCurrentMotion = eAbeMotions::Motion_56_RunJumpLandRun;
                        mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
                    }
                    else
                    {
                        if (WallHit(GetSpriteScale() * FP_FromInteger(50), -ScaleToGridSize(GetSpriteScale())))
                        {
                            mCurrentMotion = eAbeMotions::Motion_27_RunSlideStop;
                            mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
                        }
                        else
                        {
                            mCurrentMotion = eAbeMotions::Motion_57_RunJumpLand_Walk;
                            mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(9));
                        }
                    }
                    return;
                }

                mCurrentMotion = eAbeMotions::Motion_28_RunTurn;
                mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
                Environment_SFX(EnvironmentSfx::eRunSlide_4, 0, 0x7FFF, this);
                return;
            }

            field_1A0_portal = VIntoBirdPortal(3);
            if (field_1A0_portal)
            {
                field_19E_portal_sub_state = PortalSubStates::eJumpingInsidePortal_0;
            }

            mCurrentMotion = eAbeMotions::Motion_32_RunJumpBegin;
            field_10C_prev_held = 0;
            return;
        }

        if (Input().IsAnyHeld(InputCommands::eRight))
        {
            if (InputCommands::eHop & field_10C_prev_held)
            {
                field_1A0_portal = VIntoBirdPortal(3);
                if (field_1A0_portal)
                {
                    field_19E_portal_sub_state = PortalSubStates::eJumpingInsidePortal_0;
                }

                mCurrentMotion = eAbeMotions::Motion_32_RunJumpBegin;
                field_10C_prev_held = 0;
                return;
            }

            if (GetAnimation().GetFlipX())
            {
                mCurrentMotion = eAbeMotions::Motion_28_RunTurn;
                Environment_SFX(EnvironmentSfx::eRunSlide_4, 0, 0x7FFF, this);
                return;
            }

            if (Input().IsAnyHeld(InputCommands::eRun))
            {
                mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
                mCurrentMotion = eAbeMotions::Motion_56_RunJumpLandRun;
            }
            else
            {
                if (WallHit(GetSpriteScale() * FP_FromInteger(50), ScaleToGridSize(GetSpriteScale())))
                {
                    mCurrentMotion = eAbeMotions::Motion_27_RunSlideStop;
                    mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
                }
                else
                {
                    mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(9));
                    mCurrentMotion = eAbeMotions::Motion_57_RunJumpLand_Walk;
                }
            }
        }
        else
        {
            if (!(InputCommands::eHop & field_10C_prev_held))
            {
                mCurrentMotion = eAbeMotions::Motion_27_RunSlideStop;
                if (GetAnimation().GetFlipX())
                {
                    mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
                    Environment_SFX(EnvironmentSfx::eRunSlide_4, 0, 0x7FFF, this);
                }
                else
                {
                    mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
                    Environment_SFX(EnvironmentSfx::eRunSlide_4, 0, 0x7FFF, this);
                }
                return;
            }

            field_1A0_portal = VIntoBirdPortal(3);
            if (field_1A0_portal)
            {
                field_19E_portal_sub_state = PortalSubStates::eJumpingInsidePortal_0;
            }

            mCurrentMotion = eAbeMotions::Motion_29_HopBegin;
            field_10C_prev_held = 0;
        }
    }
}

bool Abe::CheckForPortalAndRunJump()
{
    if (field_10C_prev_held & InputCommands::eHop)
    {
        field_1A0_portal = VIntoBirdPortal(3);
        if (field_1A0_portal)
        {
            field_19E_portal_sub_state = PortalSubStates::eJumpingInsidePortal_0;
        }

        mCurrentMotion = eAbeMotions::Motion_32_RunJumpBegin;
        field_10C_prev_held = 0;
        return true;
    }
    return false;
}

void Abe::Motion_35_RunLoop()
{
    field_10C_prev_held |= Input().GetHeld();

    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
    {
        ToKnockback(1, 1);
        return;
    }

    MoveForward();

    if (mCurrentMotion != eAbeMotions::Motion_35_RunLoop)
    {
        return;
    }

    if (GetAnimation().GetCurrentFrame() == 0 || GetAnimation().GetCurrentFrame() == 8)
    {
        if (!mWalkToRun)
        {
            mWalkToRun = true;
            MapFollowMe(true);
        }

        CheckForPortalAndRunJump();
    }
    else if (GetAnimation().GetCurrentFrame() == 4 || GetAnimation().GetCurrentFrame() == 12)
    {
        Environment_SFX(EnvironmentSfx::eRunningFootstep_2, 0, 0x7FFF, this);

        // Snap
        if (!mWalkToRun)
        {
            mWalkToRun = true;
            MapFollowMe(true);
        }

        // Check turning in middle of run (pressing reverse direction of movement)
        if ((mVelX > FP_FromInteger(0) && Input().IsAnyHeld(InputCommands::eLeft)) || (mVelX < FP_FromInteger(0) && Input().IsAnyHeld(InputCommands::eRight)))
        {
            mCurrentMotion = eAbeMotions::Motion_28_RunTurn;
            Environment_SFX(EnvironmentSfx::eRunSlide_4, 0, 0x7FFF, this);
            field_10C_prev_held = 0;
            return;
        }

        // Check jumping into a portal
        if (CheckForPortalAndRunJump())
        {
            return;
        }

        // Running to roll
        if (field_10C_prev_held & InputCommands::eCrouchOrRoll)
        {
            mCurrentMotion = eAbeMotions::Motion_40_RunToRoll;

            // For some reason dont clear released in the frame 12 case
            if (GetAnimation().GetCurrentFrame() == 4)
            {
                field_10E_released_buttons = 0;
            }
            field_10C_prev_held = 0;
            return;
        }

        // No longer running
        if (!Input().IsAnyHeld(InputCommands::eRight) && !Input().IsAnyHeld(InputCommands::eLeft))
        {
            mCurrentMotion = eAbeMotions::Motion_27_RunSlideStop;
            Environment_SFX(EnvironmentSfx::eRunSlide_4, 0, 0x7FFF, this);
            field_10C_prev_held = 0;
            return;
        }

        // Continue running
        if (Input().IsAnyHeld(InputCommands::eRun))
        {
            field_10C_prev_held = 0;
            return;
        }

        // Stop running
        FP gridSize = {};
        if (mVelX >= FP_FromInteger(0))
        {
            gridSize = ScaleToGridSize(GetSpriteScale());
        }
        else
        {
            gridSize = -ScaleToGridSize(GetSpriteScale());
        }

        if (WallHit(GetSpriteScale() * FP_FromInteger(50), gridSize))
        {
            ToKnockback(1, 1);
        }
        else
        {
            if (GetAnimation().GetCurrentFrame() == 4)
            {
                mCurrentMotion = eAbeMotions::Motion_52_RunToWalk;
            }
            else
            {
                mCurrentMotion = eAbeMotions::Motion_53_MidRunToWalk;
            }
        }

        field_10C_prev_held = 0;
    }
    else
    {
        mWalkToRun = false;
    }
}

void Abe::Motion_36_DunnoBegin()
{
    FollowLift();

    if (GetAnimation().GetIsLastFrame())
    {
        Mudokon_SFX(MudSounds::eDunno_15, 0, 0, this);

        if (Input().IsAnyHeld(InputCommands::eDoAction | InputCommands::eThrowItem))
        {
            mCurrentMotion = eAbeMotions::Motion_37_DunnoMid;
        }
        else
        {
            mCurrentMotion = eAbeMotions::Motion_38_DunnoEnd;
        }
    }
}

void Abe::Motion_37_DunnoMid()
{
    FollowLift();

    if (!Input().IsAnyHeld(InputCommands::eDoAction | InputCommands::eThrowItem) || GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eAbeMotions::Motion_38_DunnoEnd;
    }
}

void Abe::Motion_38_DunnoEnd()
{
    FollowLift();

    if (GetAnimation().GetIsLastFrame())
    {
        ToIdle();
    }
}

void Abe::Motion_39_CrouchTurn()
{
    FollowLift();

    if (GetAnimation().GetCurrentFrame())
    {
        if (GetAnimation().GetIsLastFrame())
        {
            GetAnimation().ToggleFlipX();

            if (mNextMotion == eAbeMotions::Motion_0_Idle)
            {
                mCurrentMotion = eAbeMotions::Motion_19_CrouchIdle;
            }
            else
            {
                mCurrentMotion = mNextMotion;
                mNextMotion = eAbeMotions::Motion_0_Idle;
            }
        }
    }
    else
    {
        Environment_SFX(EnvironmentSfx::eGenericMovement_9, 0, 0x7FFF, this);
    }
}

void Abe::Motion_40_RunToRoll()
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

    if (WallHit(GetSpriteScale() * FP_FromInteger(20), mVelX))
    {
        ToKnockback(1, 1);
        mCurrentMotion = eAbeMotions::Motion_73_RollingKnockback;
    }
    else
    {
        MoveForward();

        if (GetAnimation().GetIsLastFrame())
        {
            if (mCurrentMotion == eAbeMotions::Motion_40_RunToRoll)
            {
                mCurrentMotion = eAbeMotions::Motion_25_RollLoop;
            }
        }
    }
}

void Abe::Motion_41_StandingToRun()
{
    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eAbeMotions::Motion_35_RunLoop;
    }

    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    field_10C_prev_held |= Input().GetHeld();

    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
    {
        ToIdle();
    }
    else
    {
        MoveForward();
    }
}

void Abe::Motion_42_SneakLoop()
{
    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
    {
        ToIdle();
        MapFollowMe(1);
        return;
    }

    MoveForward();

    if (mCurrentMotion == eAbeMotions::Motion_42_SneakLoop)
    {
        if (GetAnimation().GetCurrentFrame() == 3)
        {
            FP directedScale = {};
            if (GetAnimation().GetFlipX())
            {
                directedScale = -ScaleToGridSize(GetSpriteScale());
            }
            else
            {
                directedScale = ScaleToGridSize(GetSpriteScale());
            }

            if (WallHit(GetSpriteScale() * FP_FromInteger(50), directedScale) || (mVelX > FP_FromInteger(0) && Input().IsAnyHeld(InputCommands::eLeft)) || (mVelX < FP_FromInteger(0) && Input().IsAnyHeld(InputCommands::eRight)) || !Input().IsAnyHeld(InputCommands::eRight | InputCommands::eLeft))
            {
                mCurrentMotion = eAbeMotions::Motion_48_SneakToIdle;
            }
            return;
        }

        if (GetAnimation().GetCurrentFrame() == 6)
        {
            Environment_SFX(EnvironmentSfx::eSneakFootstep_3, 0, 0x7FFF, this);
            MapFollowMe(1);

            if (Input().IsAnyHeld(InputCommands::eRight | InputCommands::eLeft) && !Input().IsAnyHeld(InputCommands::eSneak))
            {
                mCurrentMotion = eAbeMotions::Motion_44_SneakToWalk;
                field_10C_prev_held = 0;
                return;
            }

            field_10C_prev_held = 0;
            return;
        }

        if (GetAnimation().GetCurrentFrame() != 13)
        {
            if (GetAnimation().GetCurrentFrame() != 16)
            {
                return;
            }

            Environment_SFX(EnvironmentSfx::eSneakFootstep_3, 0, 0x7FFF, this);
            MapFollowMe(1);

            if (Input().IsAnyHeld(InputCommands::eRight | InputCommands::eLeft))
            {
                if (!Input().IsAnyHeld(InputCommands::eSneak))
                {
                    mCurrentMotion = eAbeMotions::Motion_46_MidSneakToWalk;
                }
            }
            field_10C_prev_held = 0;
            return;
        }

        FP directedScale = {};
        if (GetAnimation().GetFlipX())
        {
            directedScale = -ScaleToGridSize(GetSpriteScale());
        }
        else
        {
            directedScale = ScaleToGridSize(GetSpriteScale());
        }

        if (WallHit(GetSpriteScale() * FP_FromInteger(50), directedScale) || (mVelX > FP_FromInteger(0) && Input().IsAnyHeld(InputCommands::eLeft)) || (mVelX < FP_FromInteger(0) && Input().IsAnyHeld(InputCommands::eRight)) || !Input().IsAnyHeld(InputCommands::eRight | InputCommands::eLeft))
        {
            mCurrentMotion = eAbeMotions::Motion_49_MidSneakToIdle;
        }
    }
}

void Abe::Motion_43_WalkToSneak()
{
    field_10C_prev_held |= Input().GetHeld();

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
        mCurrentMotion = eAbeMotions::Motion_42_SneakLoop;
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
    {
        ToIdle();
        MapFollowMe(1);
    }
    else
    {
        MoveForward();
    }
}

void Abe::Motion_44_SneakToWalk()
{
    field_10C_prev_held |= Input().GetHeld();

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
        mCurrentMotion = eAbeMotions::Motion_1_WalkLoop;
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
    {
        ToIdle();
        MapFollowMe(1);
    }
    else
    {
        MoveForward();
    }
}

void Abe::Motion_45_MidWalkToSneak()
{
    field_10C_prev_held |= Input().GetHeld();

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
        mReturnToPreviousMotion = true;
        mPreviousMotion = eAbeMotions::Motion_42_SneakLoop;
        mBaseAliveGameObjectLastAnimFrame = 10;
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
    {
        ToIdle();
        MapFollowMe(1);
    }
    else
    {
        MoveForward();
    }
}

void Abe::Motion_46_MidSneakToWalk()
{
    field_10C_prev_held |= Input().GetHeld();

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
        mReturnToPreviousMotion = true;
        mPreviousMotion = eAbeMotions::Motion_1_WalkLoop;
        mBaseAliveGameObjectLastAnimFrame = 9;
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
    {
        ToIdle();
        MapFollowMe(1);
    }
    else
    {
        MoveForward();
    }
}

void Abe::Motion_47_SneakBegin()
{
    field_10C_prev_held |= Input().GetHeld();

    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eAbeMotions::Motion_42_SneakLoop;
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
    {
        ToIdle();
        MapFollowMe(1);
    }
    else
    {
        MoveForward();
    }
}

void Abe::Motion_48_SneakToIdle()
{
    if (GetAnimation().GetCurrentFrame() == 0)
    {
        Environment_SFX(EnvironmentSfx::eSneakFootstep_3, 0, 0x7FFF, this);
    }

    MoveForward();

    if (GetAnimation().GetIsLastFrame())
    {
        MapFollowMe(1);
        ToIdle();
    }
}

void Abe::Motion_49_MidSneakToIdle()
{
    if (!GetAnimation().GetCurrentFrame())
    {
        Environment_SFX(EnvironmentSfx::eSneakFootstep_3, 0, 0x7FFF, this);
    }

    MoveForward();

    if (GetAnimation().GetIsLastFrame())
    {
        MapFollowMe(1);

        ToIdle();
    }
}

void Abe::Motion_50_WalkToRun()
{
    field_10C_prev_held |= Input().GetHeld();

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
        mCurrentMotion = eAbeMotions::Motion_35_RunLoop;
        field_10C_prev_held = 0;
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
    {
        ToIdle();
        MapFollowMe(1);
    }
    else
    {
        MoveForward();
    }
}

void Abe::Motion_51_MidWalkToRun()
{
    field_10C_prev_held |= Input().GetHeld();

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
        mReturnToPreviousMotion = true;
        mPreviousMotion = eAbeMotions::Motion_35_RunLoop;
        mBaseAliveGameObjectLastAnimFrame = 8;
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
    {
        ToIdle();
        MapFollowMe(1);
    }
    else
    {
        MoveForward();
    }
}

void Abe::Motion_52_RunToWalk()
{
    field_10C_prev_held |= Input().GetHeld();

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
        mCurrentMotion = eAbeMotions::Motion_1_WalkLoop;
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
    {
        ToIdle();
    }
    else
    {
        MoveForward();
    }
}

void Abe::Motion_53_MidRunToWalk()
{
    field_10C_prev_held |= Input().GetHeld();

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
        mReturnToPreviousMotion = true;
        mPreviousMotion = eAbeMotions::Motion_1_WalkLoop;
        mBaseAliveGameObjectLastAnimFrame = 9;
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
    {
        ToIdle();
    }
    else
    {
        MoveForward();
    }
}

void Abe::Motion_54_RunTurnToRun()
{
    field_10C_prev_held |= Input().GetHeld();

    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
    {
        ToIdle();
    }
    else
    {
        MoveForward();

        if (GetAnimation().GetIsLastFrame())
        {
            mCurrentMotion = eAbeMotions::Motion_35_RunLoop;
            GetAnimation().ToggleFlipX();
        }
    }
}

void Abe::Motion_55_RunTurnToWalk()
{
    field_10C_prev_held |= Input().GetHeld();

    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);


    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
    {
        ToIdle();
    }
    else
    {
        MoveForward();

        if (GetAnimation().GetIsLastFrame())
        {
            mCurrentMotion = eAbeMotions::Motion_1_WalkLoop;
            GetAnimation().ToggleFlipX();
        }
    }
}

void Abe::Motion_56_RunJumpLandRun()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
    {
        ToIdle();
    }
    else
    {
        MoveForward();

        if (GetAnimation().GetIsLastFrame())
        {
            mCurrentMotion = eAbeMotions::Motion_35_RunLoop;
        }
    }
}

void Abe::Motion_57_RunJumpLand_Walk()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
    {
        ToIdle();
    }
    else
    {
        MoveForward();

        if (GetAnimation().GetIsLastFrame())
        {
            mCurrentMotion = eAbeMotions::Motion_1_WalkLoop;
        }
    }
}

void Abe::Motion_58_ToSpeak()
{
    FollowLift();

    field_10C_prev_held |= Input().GetPressed();

    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = DoGameSpeak(field_10C_prev_held);
        if (mCurrentMotion == eAbeMotions::None_m1)
        {
            ToIdle();
        }
        else
        {
            EventBroadcast(kEventSpeaking, this);
        }
        field_10C_prev_held = 0;
    }
}

void Abe::Motion_59_DeathDropFall()
{
    GetAnimation().SetAnimate(false);

    FollowLift();

    if (field_114_gnFrame == 0)
    {
        field_120_x_vel_slow_by = FP_FromInteger(0);
        mVelX = FP_FromInteger(0);
        mVelY = FP_FromInteger(0);
        field_118_timer = MakeTimer(90);
        field_114_gnFrame = 1;
    }
    else if (field_114_gnFrame == 1)
    {
        if (static_cast<s32>(sGnFrame) == field_118_timer - 30)
        {
            SND_SEQ_Play(SeqId::eHitBottomOfDeathPit_10, 1, 65, 65);
        }
        else if (static_cast<s32>(sGnFrame) == field_118_timer - 24)
        {
            Environment_SFX(EnvironmentSfx::eFallingDeathScreamHitGround_15, 0, 0x7FFF, this);

            relive_new ScreenShake(true, false);
        }
        else if (static_cast<s32>(sGnFrame) >= field_118_timer)
        {
            mShrivel = true;
            mCurrentMotion = eAbeMotions::Motion_60_Dead;
            field_114_gnFrame = 0;
            mHealth = FP_FromInteger(0);
            MusicController::static_PlayMusic(MusicController::MusicTypes::eDeathLong_14, this, 1, 0);
        }
    }
}

void Abe::Motion_60_Dead()
{
    GetAnimation().SetAnimate(false);
    FollowLift();

    switch (field_114_gnFrame)
    {
        case 0:
        {
            EventBroadcast(kEventHeroDying, this);
            field_118_timer = MakeTimer(30);
            field_120_x_vel_slow_by = FP_FromInteger(0);
            mVelX = FP_FromInteger(0);
            mVelY = FP_FromInteger(0);
            field_10C_prev_held = 0;
            field_114_gnFrame++;

            auto aux = 0;
            if (BaseAliveGameObjectPathTLV && BaseAliveGameObjectPathTLV->mTlvType == ReliveTypes::eDeathDrop)
            {
                aux = 60;
            }
            else
            {
                aux = 15;
            }
            const FP ypos = FP_FromInteger(Math_NextRandom() % 10) + mYPos + FP_FromInteger(15);
            const FP xpos = FP_FromInteger(((Math_NextRandom() % 64) - 32)) + mXPos;
            relive_new DeathBirdParticle(
                xpos,
                ypos,
                (Math_NextRandom() % 8) + field_118_timer + aux,
                true,
                GetSpriteScale());

            return;
        }
        case 1:
        {
            EventBroadcast(kEventHeroDying, this);
            if (!(sGnFrame % 4))
            {
                auto aux = 0;
                if (BaseAliveGameObjectPathTLV && BaseAliveGameObjectPathTLV->mTlvType == ReliveTypes::eDeathDrop)
                {
                    aux = 60;
                }
                else
                {
                    aux = 15;
                }
                const FP ypos = FP_FromInteger(Math_NextRandom() % 10) + mYPos + FP_FromInteger(15);
                const FP xpos = FP_FromInteger(((Math_NextRandom() % 64) - 32)) + mXPos;
                relive_new DeathBirdParticle(
                    xpos,
                    ypos,
                    (Math_NextRandom() % 8) + field_118_timer + aux,
                    false,
                    GetSpriteScale());
            }
            SetSpriteScale(GetSpriteScale() - FP_FromDouble(0.008));

            mRGB.r -= 2;
            mRGB.g -= 2;
            mRGB.b -= 2;
            if (static_cast<s32>(sGnFrame) > field_118_timer)
            {
                field_118_timer = MakeTimer(60);
                if (BaseAliveGameObjectPathTLV)
                {
                    if (BaseAliveGameObjectPathTLV->mTlvType == ReliveTypes::eDeathDrop)
                    {
                        field_118_timer = (sGnFrame + 60) + 45;
                    }
                }
                field_114_gnFrame++;
                MusicController::static_PlayMusic(MusicController::MusicTypes::eDeathLong_14, this, 1, 0);
            }
            return;
        }
        case 2:
        {
            EventBroadcast(kEventHeroDying, this);
            if (static_cast<s32>(sGnFrame) > field_118_timer)
            {
                field_114_gnFrame++;
            }
            return;
        }
        case 3:
        {
            EventBroadcast(kEventHeroDying, this);
            auto pFade = sObjectIds.Find(mFadeId, ReliveTypes::eFade);
            if (pFade)
            {
                pFade->SetDead(true);
                mFadeId = Guid{};
            }

            pFade = relive_new Fade(Layer::eLayer_FadeFlash_40, FadeOptions::eFadeIn, 0, 8, relive::TBlendModes::eBlend_2);
            if (pFade)
            {
                mFadeId = pFade->mBaseGameObjectId;
            }

            auto pCircularFade = sObjectIds.Find_Impl(mCircularFadeId);
            if (pCircularFade)
            {
                pCircularFade->SetDead(true);
            }
            field_114_gnFrame++;
            return;
        }
        case 4:
        {
            EventBroadcast(kEventHeroDying, this);
            auto pFade = static_cast<Fade*>(sObjectIds.Find(mFadeId, ReliveTypes::eFade));
            if (pFade && pFade->mDone)
            {
                VOnTrapDoorOpen();
                BaseAliveGameObjectCollisionLine = nullptr;
                if (gElum)
                {
                    if (!gElum->mRespawnOnDead)
                    {
                        gElum->SetDead(true);
                    }
                }

                if (mRidingElum)
                {
                    mRidingElum = false;
                }

                field_118_timer = MakeTimer(8);
                field_114_gnFrame++;
            }
            return;
        }
        case 5:
        {
            EventBroadcast(kEventDeathReset, this);
            if (static_cast<s32>(sGnFrame) > field_118_timer)
            {
                mCurrentMotion = eAbeMotions::Motion_61_Respawn;
                field_118_timer = MakeTimer(2);
                field_114_gnFrame = 0;
                MusicController::static_PlayMusic(MusicController::MusicTypes::eType0, this, 1, 0);
                if (field_168_ring_pulse_timer && field_16C_bHaveShrykull)
                {

                }
                else if (mContinueLevel != EReliveLevelIds::eRuptureFarmsReturn || mContinuePath != 6)
                {
                    field_168_ring_pulse_timer = 0;
                }
                field_130_say = -1;
                GetShadow()->mEnabled = false;
            }
            return;
        }
        default:
            return;
    }
}

void Abe::Motion_61_Respawn()
{
    // This will crash the game if abe didnt touch a checkpoint yet

    if (gAttract)
    {
        // Depending on the randomn seed value abe can die in a demo, if so the checkpoint save will load
        // depending on the saved camera number on returning the main menu this can crash. Hack fix it
        // by killing abe so the bad save cant get loaded before we return to the menu.
        LOG_WARNING("Destroying abe to prevent game crash, he isnt supposed to die in a demo!");
        SetDead(true);
        gAbe = nullptr;
        sControlledCharacter = nullptr;
        return;
    }

    auto pLiftPoint = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));

    GetAnimation().SetAnimate(false);
    FollowLift();
    EventBroadcast(kEventResetting, this);

    switch (field_114_gnFrame)
    {
        case 0:
        {
            mHealth = FP_FromInteger(1);
            if (field_19C_throwable_count != 0)
            {
                if (gThrowableArray)
                {
                    gThrowableArray->Remove(field_19C_throwable_count);
                }
            }
            field_19C_throwable_count = 0;

            if (static_cast<s32>(sGnFrame) > field_118_timer)
            {
                if (pLiftPoint)
                {
                    if (pLiftPoint->GetDrawable())
                    {
                        pLiftPoint->VRemove(this);
                        BaseAliveGameObject_PlatformId = Guid{};
                    }
                }
                if (mContinueCamera > 300u)
                {
                    mContinueCamera = gMap.mCurrentCamera;
                    mContinuePath = gMap.mCurrentPath;
                    mContinueLevel = gMap.mCurrentLevel;

                    PSX_Point camPos = {};
                    gMap.GetCurrentCamCoords(&camPos);
                    mContinueTopLeft.x = camPos.x + 512;
                    mContinueTopLeft.y = camPos.y + 240;
                }
                SaveGame::LoadFromMemory(&gSaveBuffer, 0);
                if (field_19C_throwable_count)
                {
                    LoadRockTypes(gSaveBuffer.mCurrentLevel, gSaveBuffer.mCurrentPath);
                    if (!gThrowableArray)
                    {
                        gThrowableArray = relive_new ThrowableArray();
                    }
                    gThrowableArray->Add(field_19C_throwable_count);
                }
                field_114_gnFrame = 1;
                mCurrentMotion = eAbeMotions::Motion_61_Respawn;
            }
            break;
        }
        case 1:
        {
            mCurrentLevel = mContinueLevel;
            mCurrentPath = mContinuePath;
            mXPos = FP_FromInteger(mContinueTopLeft.x);
            mYPos = FP_FromInteger(mContinueTopLeft.y) + GetSpriteScale() * FP_FromInteger(30);

            BaseAliveGameObjectLastLineYPos = mYPos;

            GetAnimation().SetFlipX(mAbeRespawnFlipX);
            MapFollowMe(true);
            SetTint(sAbeTintTable, gMap.mCurrentLevel);
            if (gElum)
            {
                gElum->SetSpriteScale(GetSpriteScale());
            }
            if (GetSpriteScale() == FP_FromDouble(0.5))
            {
                GetAnimation().SetRenderLayer(Layer::eLayer_AbeMenu_Half_13);
                SetScale(Scale::Bg);
                if (gElum)
                {
                    gElum->GetAnimation().SetRenderLayer(Layer::eLayer_ZapLinesMudsElum_Half_9);
                    gElum->SetScale(Scale::Bg);
                }
            }
            else
            {
                GetAnimation().SetRenderLayer(Layer::eLayer_AbeMenu_32);
                SetScale(Scale::Fg);
                if (gElum)
                {
                    gElum->GetAnimation().SetRenderLayer(Layer::eLayer_ZapLinesElumMuds_28);
                    gElum->SetScale(Scale::Fg);
                }
            }
            GetAnimation().SetRender(false);
            EventBroadcast(kEventDeathResetEnd, this);

            //TODO not used?
            //if (dword_507720)
            //{
            //    *(_BYTE *) (dword_507720 + 6) |= 4u;
            //}
            SetElectrocuted(false);

            auto pFade = static_cast<Fade*>(sObjectIds.Find(mFadeId, ReliveTypes::eFade));
            pFade->Init(Layer::eLayer_FadeFlash_40, FadeOptions::eFadeOut, 1, 8);
            mNextMotion = eAbeMotions::Motion_0_Idle;
            field_118_timer = MakeTimer(60);
            field_114_gnFrame = 2;
            break;
        }
        case 2:
        {
            if (static_cast<s32>(sGnFrame) > field_118_timer)
            {
                auto xDiff = gScreenManager->CamXPos();
                auto yDiff = gScreenManager->CamYPos();

                for (s32 i = 0; i < 8; i++)
                {
                    auto pDove = relive_new Dove(
                        AnimId::Dove_Flying,
                        xDiff + FP_FromInteger(Math_NextRandom() * 2),
                        yDiff - FP_FromInteger(Math_NextRandom() % 32),
                        GetSpriteScale());
                    if (pDove)
                    {
                        pDove->AsJoin(
                            mXPos + FP_FromInteger((Math_NextRandom() % 16) - 8),
                            mYPos + FP_FromInteger((Math_NextRandom() % 16) - 20));
                        pDove->SetSpriteScale(GetSpriteScale());
                    }
                }
                SfxPlayMono(relive::SoundEffects::RespawnDove, 0, GetSpriteScale());
                field_118_timer = MakeTimer(45);
                field_114_gnFrame = 3;
            }
            break;
        }
        case 3:
        {
            if (field_118_timer - sGnFrame == 10)
            {
                SfxPlayMono(relive::SoundEffects::Respawn, 90);
            }
            if (static_cast<s32>(sGnFrame) > field_118_timer)
            {
                New_DestroyOrCreateObject_Particle(
                    mXPos,
                    GetSpriteScale() * FP_FromInteger(25) + mYPos,
                    GetSpriteScale());
                field_114_gnFrame = 4;
                field_118_timer = MakeTimer(2);
            }
            break;
        }
        case 4:
        {
            if (static_cast<s32>(sGnFrame) > field_118_timer)
            {
                GetShadow()->mEnabled = true;
                GetShadow()->mShadowAtBottom = false;
                MusicController::static_PlayMusic(MusicController::MusicTypes::eType0, this, 0, 0);
                mLandSoft = true;
                GetAnimation().SetRender(true);
                SetDrawable(true);
                mCurrentMotion = eAbeMotions::Motion_3_Fall;

                relive_new Flash(Layer::eLayer_Above_FG1_39, 255u, 0, 255u);
                mbGotShot = false;
                mShrivel = false;
                field_114_gnFrame = sGnFrame;
            }
            break;
        }
        default:
            return;
    }
}

void Abe::Motion_62_LoadedSaveSpawn()
{
    EventBroadcast(kEventResetting, this);

    if (field_114_gnFrame)
    {
        auto pSaveData = field_2AC_pSaveData;
        mYPos = FP_FromInteger(pSaveData->mActiveHero_YPos);
        mXPos = FP_FromInteger(pSaveData->mActiveHero_XPos);

        PathLine* pLine2 = nullptr;
        FP hitX2 = {};
        FP hitY2 = {};
        if (gCollisions->Raycast(
                gAbe->mXPos,
                gAbe->mYPos - FP_FromInteger(60),
                gAbe->mXPos,
                gAbe->mYPos + FP_FromInteger(60),
                &pLine2,
                &hitX2,
                &hitY2,
                CollisionMask(static_cast<eLineTypes>(pSaveData->field_23A_mode_mask))))
        {
            gAbe->BaseAliveGameObjectCollisionLine = pLine2;
            gAbe->mYPos = hitY2;
            gAbe->mCurrentMotion = eAbeMotions::Motion_0_Idle;
        }
        else
        {
            gAbe->mCurrentMotion = eAbeMotions::Motion_3_Fall;
        }
        gAbe->mLandSoft = false;
        gAbe->BaseAliveGameObjectLastLineYPos = gAbe->mYPos;
        gAbe->field_110_state.raw = static_cast<s16>(pSaveData->field_244_stone_state);
        gAbe->field_114_gnFrame = pSaveData->field_248_gnFrame;
        gAbe->mBaseAliveGameObjectLastAnimFrame = pSaveData->mActiveHero_CurrentFrame;
        gAbe->GetAnimation().SetFlipX(pSaveData->mActiveHero_FlipX & 1);
        gAbe->MapFollowMe(true);
        gAbe->GetAnimation().SetRender(true);
        if (gAbe->field_19C_throwable_count)
        {
            if (!gThrowableArray)
            {
                LoadRockTypes(gSaveBuffer.mCurrentLevel, gSaveBuffer.mCurrentPath);

                gThrowableArray = relive_new ThrowableArray();
            }
            gThrowableArray->Add(gAbe->field_19C_throwable_count);
        }
        if (pSaveData->mInfiniteGrenades == -1)
        {
            LoadRockTypes(EReliveLevelIds::eRuptureFarmsReturn, 19);
            if (!gThrowableArray)
            {
                gThrowableArray = relive_new ThrowableArray();
            }
            gThrowableArray->Add(1);
            gInfiniteGrenades = true;
        }
        else
        {
            gInfiniteGrenades = false;
        }
        if (pSaveData->mElumExists)
        {
            if (!gElum)
            {
                Elum::Spawn(Guid{});
            }

            if (gElum)
            {
                gElum->SetUpdatable(false);
                gElum->GetAnimation().SetRender(false);
                gElum->mContinueRect = pSaveData->mElum_ContinueRect;
                gElum->mPreviousContinueZoneNumber = pSaveData->mElum_PreviousContinueZoneNumber;
                gElum->mAbeZoneNumber = pSaveData->mElum_AbeZoneNumber;
                gElum->mContinuePath = pSaveData->mElum_ContinuePath;
                gElum->mContinueLevel = pSaveData->mElum_ContinueLevel;
                gElum->mContinueSpriteScale = pSaveData->mElum_ContinueSpriteScale;
                gElum->mRespawnOnDead = pSaveData->mElum_RespawnOnDead;
                gElum->mCurrentLevel = pSaveData->mElum_CurrentLevel;
                gElum->mCurrentPath = pSaveData->mElum_CurrentPath;
                gElum->mXPos = FP_FromInteger(pSaveData->mElum_XPos);
                gElum->mYPos = FP_FromInteger(pSaveData->mElum_YPos);
                gElum->GetAnimation().SetFlipX(pSaveData->mElum_FlipX & 1);
                gElum->mBaseAliveGameObjectLastAnimFrame = 0;
                gElum->field_120_bUnknown = 1;
                gElum->mDontFollowAbe = pSaveData->mElum_DontFollowAbe;
                gElum->mBrainIdx = pSaveData->mElum_BrainIdx;
                gElum->mBrainSubState = pSaveData->mElum_BrainSubState;
                gElum->mHoneyXPos = static_cast<s16>(pSaveData->mElum_HoneyXPos);
                gElum->mHoneyCamera = pSaveData->mElum_HoneyCamera;

                gElum->mFoundHoney = pSaveData->mElum_FoundHoney & 1;
                gElum->mFalling = pSaveData->mElum_Falling & 1;
                gElum->mStungByBees = pSaveData->mElum_StungByBees & 1;
                if (gElum->mCurrentPath == gAbe->mCurrentPath)
                {
                    if (pSaveData->mElum_LineType != -1)
                    {
                        PathLine* pLine = nullptr;
                        FP hitX = {};
                        FP hitY = {};
                        if (gCollisions->Raycast(
                            gElum->mXPos,
                            gElum->mYPos - FP_FromInteger(60),
                            gElum->mXPos,
                            gElum->mYPos + FP_FromInteger(60),
                            &pLine,
                            &hitX,
                            &hitY,
                            CollisionMask(static_cast<eLineTypes>(pSaveData->mElum_LineType))))
                        {
                            gElum->BaseAliveGameObjectCollisionLine = pLine;
                            gElum->mCurrentMotion = eElumMotions::Motion_1_Idle;
                            gElum->mPreviousMotion = eElumMotions::Motion_1_Idle;
                        }
                        else
                        {
                            gElum->mCurrentMotion = eElumMotions::Motion_21_Land;
                            gElum->mPreviousMotion = eElumMotions::Motion_21_Land;
                        }
                    }
                }
                else
                {
                    //TODO fix this madness
                    gElum->BaseAliveGameObjectCollisionLine = reinterpret_cast<PathLine*>(-2);
                }

                if (gElum->mFoundHoney)
                {
                    gElum->mCurrentMotion = eElumMotions::Motion_25_LickingHoney;
                    gElum->mPreviousMotion = eElumMotions::Motion_25_LickingHoney;
                }
                gElum->MapFollowMe(true);
                gElum->SetUpdatable(true);
                gElum->GetAnimation().SetRender(true);
            }
        }
    }
    else
    {
        field_114_gnFrame = 1;
    }
}

void Abe::Motion_63_TurnToRun()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
    {
        ToKnockback(1, 1);
    }
    else
    {
        MoveForward();

        if (mCurrentMotion == eAbeMotions::Motion_63_TurnToRun)
        {
            if (GetAnimation().GetIsLastFrame())
            {
                mCurrentMotion = eAbeMotions::Motion_35_RunLoop;
            }
        }
    }
}

void Abe::Motion_64_LedgeAscend()
{
    FollowLift();

    if (GetAnimation().GetCurrentFrame() == 0)
    {
        Environment_SFX(EnvironmentSfx::eExhaustingHoistNoise_10, 0, 0x7FFF, this);
    }
    if (GetAnimation().GetCurrentFrame() == 4)
    {
        Environment_SFX(EnvironmentSfx::eRunJumpOrLedgeHoist_11, 0, 0x7FFF, this);
        GetShadow()->mShadowAtBottom = false;
    }
    else if (GetAnimation().GetIsLastFrame())
    {
        MapFollowMe(1);
        ToIdle();
    }
}

void Abe::Motion_65_LedgeDescend()
{
    FollowLift();

    if (GetAnimation().GetCurrentFrame() == 2)
    {
        Environment_SFX(EnvironmentSfx::eRunJumpOrLedgeHoist_11, 0, 0x7FFF, this);
        GetShadow()->mShadowAtBottom = !GetShadow()->mShadowAtBottom;
    }
    else if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eAbeMotions::Motion_66_LedgeHang;
    }
}

void Abe::Motion_66_LedgeHang()
{
    FollowLift();

    GetShadow()->mShadowAtBottom = true;

    if (Input().IsAnyHeld(InputCommands::eUp))
    {
        mCurrentMotion = eAbeMotions::Motion_64_LedgeAscend;
    }
    else if (Input().IsAnyHeld(InputCommands::eDown))
    {
        VOnTrapDoorOpen();

        BaseAliveGameObjectCollisionLine = nullptr;
        mCurrentMotion = eAbeMotions::Motion_91_FallingFromGrab;
        mYPos += (GetSpriteScale() * FP_FromInteger(80)) + FP_FromInteger(3);
        BaseAliveGameObjectLastLineYPos = mYPos;
        GetShadow()->mShadowAtBottom = false;
    }
}

void Abe::Motion_67_ToOffScreenHoist()
{
    BaseGameObject* pLiftPoint = sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId);

    mYPos -= (GetSpriteScale() * FP_FromInteger(80));

    GetShadow()->mShadowAtBottom = !GetShadow()->mShadowAtBottom;

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    if (gCollisions->Raycast(
            mXPos,
            mYPos - (GetSpriteScale() * FP_FromInteger(80)),
            mXPos,
            mYPos,
            &pLine,
            &hitX,
            &hitY,
            GetSpriteScale() != FP_FromDouble(0.5) ? kFgFloor : kBgFloor))
    {
        mYPos = hitY;
        BaseAliveGameObjectCollisionLine = pLine;
        mVelY = FP_FromInteger(0);
        if (!pLiftPoint)
        {
            if (BaseAliveGameObjectCollisionLine->mLineType == eLineTypes ::eDynamicCollision_32 ||
                BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eBackgroundDynamicCollision_36)
            {
                PSX_RECT bRect = VGetBoundingRect();
                bRect.y += 5;
                bRect.h += 5;

                OnCollisionWith(
                    {bRect.x, bRect.y},
                    {bRect.w, bRect.h},
                    gPlatformsArray);
            }
        }
        mCurrentMotion = eAbeMotions::Motion_66_LedgeHang;
    }
    else
    {
        mCurrentMotion = eAbeMotions::Motion_17_HoistIdle;
    }
}

void Abe::Motion_68_LedgeHangWobble()
{
    if (GetAnimation().GetCurrentFrame() == 0)
    {
        if (!mSfxPlaying)
        {
            mSfxPlaying = true;
            Environment_SFX(EnvironmentSfx::eWalkingFootstep_1, 0, 127, this);
        }
    }
    else if (GetAnimation().GetCurrentFrame() == 2)
    {
        if (!mSfxPlaying)
        {
            mSfxPlaying = true;
            Mudokon_SFX(MudSounds::eBeesStruggle_18, 45, -200, this);
        }
    }
    else
    {
        mSfxPlaying = false;
    }

    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    FollowLift();

    if (Input().IsAnyHeld(InputCommands::eUp))
    {
        mSfxPlaying = false;
        mCurrentMotion = eAbeMotions::Motion_64_LedgeAscend;
    }
    else
    {
        if (Input().IsAnyHeld(InputCommands::eDown))
        {
            mSfxPlaying = false;

            VOnTrapDoorOpen();
            BaseAliveGameObjectCollisionLine = nullptr;

            mCurrentMotion = eAbeMotions::Motion_91_FallingFromGrab;
            mYPos += (GetSpriteScale() * FP_FromInteger(80)) + FP_FromInteger(3);
            GetShadow()->mShadowAtBottom = false;
            BaseAliveGameObjectLastLineYPos = mYPos;
        }
        else if (GetAnimation().GetIsLastFrame())
        {
            mSfxPlaying = false;
            mCurrentMotion = eAbeMotions::Motion_66_LedgeHang;
        }
    }
}

void Abe::Motion_69_RingRopePullHang()
{
    if (mPullRingRope->vIsNotBeingPulled())
    {
        mCurrentMotion = eAbeMotions::Motion_91_FallingFromGrab;
        mPullRingRope->mBaseGameObjectRefCount--;
        mPullRingRope = nullptr;
        mVelY = FP_FromInteger(0);
    }
}

void Abe::Motion_70_Knockback()
{
    ElumFree();

    if (field_1A0_portal)
    {
        field_1A0_portal->VKillPortalClipper();
        field_1A0_portal = nullptr;
    }

    if (GetAnimation().GetCurrentFrame() == 12)
    {
        FallOnBombs();
    }

    if (GetAnimation().GetRender())
    {
        if (BaseAliveGameObjectCollisionLine)
        {
            if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
            {
                mVelX = FP_FromInteger(0);
            }

            MoveWithVelocity(FP_FromDouble(0.7));

            if ((gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarms
                 || gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarmsReturn
                 || gMap.mCurrentLevel == EReliveLevelIds::eBoardRoom)
                && GetAnimation().GetCurrentFrame() == 7)
            {
                Environment_SFX(EnvironmentSfx::eHitGroundSoft_6, 80, -200, this);
                EventBroadcast(kEventNoise, this);
                EventBroadcast(kEventSuspiciousNoise, this);
            }
        }
        else
        {
            Motion_3_Fall();

            if (mCurrentMotion == eAbeMotions::Motion_86_FallLandDie || mCurrentMotion == eAbeMotions::Motion_98_LandSoft)
            {
                field_114_gnFrame = MakeTimer(10);
                mCurrentMotion = eAbeMotions::Motion_70_Knockback;
                Environment_SFX(EnvironmentSfx::eHitGroundSoft_6, 80, -200, this);
            }
        }
    }

    if (GetAnimation().GetForwardLoopCompleted())
    {
        if ((BaseAliveGameObjectCollisionLine || !GetAnimation().GetRender()))
        {
            if (mRidingElum)
            {
                mRidingElum = false;
            }

            if (field_114_gnFrame <= static_cast<s32>(sGnFrame))
            {
                if (mHealth > FP_FromInteger(0) || gAbeInvulnerableCheat)
                {
                    mCurrentMotion = eAbeMotions::Motion_71_KnockbackGetUp;
                }
                else
                {
                    ToDieFinal();
                }
            }
        }
    }
}

void Abe::Motion_71_KnockbackGetUp()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (GetAnimation().GetIsLastFrame())
    {
        ToIdle();
    }
}

void Abe::Motion_72_PushWall()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (GetAnimation().GetCurrentFrame() == 10)
    {
        if (Math_NextRandom() <= 127u)
        {
            Environment_SFX(EnvironmentSfx::eExhaustingHoistNoise_10, 0, 0x7FFF, this);
        }
    }

    if (GetAnimation().GetIsLastFrame())
    {
        ToIdle();
    }
}

void Abe::Motion_73_RollingKnockback()
{
    if (GetAnimation().GetCurrentFrame() == 12)
    {
        FallOnBombs();
    }

    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (GetAnimation().GetIsLastFrame())
    {
        if (field_114_gnFrame <= static_cast<s32>(sGnFrame))
        {
            if (mHealth > FP_FromInteger(0) || gAbeInvulnerableCheat)
            {
                mCurrentMotion = eAbeMotions::Motion_71_KnockbackGetUp;
            }
            else
            {
                ToDieFinal();
            }
        }
    }
}

void Abe::Motion_74_JumpIntoWell()
{
    GetShadow()->mEnabled = false;

    if (GetSpriteScale() == FP_FromDouble(0.5))
    {
        GetAnimation().SetRenderLayer(Layer::eLayer_BeforeWell_Half_3);
    }
    else
    {
        GetAnimation().SetRenderLayer(Layer::eLayer_BeforeWell_22);
    }

    Motion_77_WellBegin();
}

void Abe::Motion_75_ToInsideOfAWellLocal()
{
    Motion_78_InsideWellLocal();
}

void Abe::Motion_76_ToWellShotOut()
{
    Motion_79_WellShotOut();
}

void Abe::Motion_77_WellBegin()
{
    EventBroadcast(kEventNoise, this);
    if (GetAnimation().GetCurrentFrame() > 10)
    {
        GetShadow()->mEnabled = false;

        BaseAliveGameObjectPathTLV = gMap.VTLV_Get_At_Of_Type(
            FP_GetExponent(mXPos),
            FP_GetExponent(mYPos),
            FP_GetExponent(mXPos),
            FP_GetExponent(mYPos),
            ReliveTypes::eWellLocal);
        if (!BaseAliveGameObjectPathTLV)
        {
            BaseAliveGameObjectPathTLV = gMap.VTLV_Get_At_Of_Type(
                FP_GetExponent(mXPos),
                FP_GetExponent(mYPos),
                FP_GetExponent(mXPos),
                FP_GetExponent(mYPos),
                ReliveTypes::eWellExpress);
        }

        const s16 xpos = FP_GetExponent(mXPos);
        const auto tlv_mid_x = (BaseAliveGameObjectPathTLV->mBottomRightX + BaseAliveGameObjectPathTLV->mTopLeftX) / 2;
        if (xpos > tlv_mid_x)
        {
            mXPos -= GetSpriteScale();
        }
        else if (xpos < tlv_mid_x)
        {
            mXPos += GetSpriteScale();
        }
    }
    if (GetAnimation().GetCurrentFrame() == 11)
    {
        if (GetSpriteScale() == FP_FromDouble(0.5))
        {
            GetAnimation().SetRenderLayer(Layer::eLayer_BeforeWell_Half_3);
        }
        else
        {
            GetAnimation().SetRenderLayer(Layer::eLayer_BeforeWell_22);
        }
    }
    if (GetAnimation().GetIsLastFrame())
    {
        field_114_gnFrame = 15;
        SfxPlayMono(relive::SoundEffects::WellEnter, 0, GetSpriteScale());

        switch (mCurrentMotion)
        {
            case eAbeMotions::Motion_77_WellBegin:
            {
                mCurrentMotion = eAbeMotions::Motion_78_InsideWellLocal;
                break;
            }
            case eAbeMotions::Motion_80_430EF0:
            {
                mCurrentMotion = eAbeMotions::Motion_81_InsideWellExpress;
                break;
            }
            case eAbeMotions::Motion_83_430F00:
            {
                mCurrentMotion = eAbeMotions::Motion_84_ToInsideWellLocal;
                break;
            }
            case eAbeMotions::Motion_74_JumpIntoWell:
            {
                mCurrentMotion = eAbeMotions::Motion_75_ToInsideOfAWellLocal;
                break;
            }
            default:
            {
                ALIVE_FATAL("Unrecognized state in Abe::Motion_77_WellBegin_430F10 called! %d", mCurrentMotion);
                break;
            }
        }
    }
}

FP gPointlessWellScale_4C73CC = FP_FromDouble(1.8);

void Abe::Motion_78_InsideWellLocal()
{
    const s32 old_gnFrame = field_114_gnFrame;
    field_114_gnFrame--;

    if (old_gnFrame == 0)
    {
        BaseAliveGameObjectPathTLV = gMap.VTLV_Get_At_Of_Type(
            FP_GetExponent(mXPos),
            FP_GetExponent(mYPos),
            FP_GetExponent(mXPos),
            FP_GetExponent(mYPos),
            ReliveTypes::eWellLocal);

        if (!BaseAliveGameObjectPathTLV)
        {
            BaseAliveGameObjectPathTLV = gMap.VTLV_Get_At_Of_Type(
                FP_GetExponent(mXPos),
                FP_GetExponent(mYPos),
                FP_GetExponent(mXPos),
                FP_GetExponent(mYPos),
                ReliveTypes::eWellExpress);
        }

        auto pWellBase = static_cast<relive::Path_WellBase*>(BaseAliveGameObjectPathTLV);

        field_120_x_vel_slow_by = FP_FromInteger(0);

        if (mSnapAbe)
        {
            // Snap to exit point
            mVelX = (GetSpriteScale() * (FP_FromInteger(pWellBase->mExitX) / FP_FromInteger(100)));
            mVelY = (GetSpriteScale() * (FP_FromInteger(pWellBase->mExitY) / FP_FromInteger(100)));
            mSnapAbe = false;
        }
        else
        {
            if (SwitchStates_Get(pWellBase->mSwitchId))
            {
                auto pLocalWell = static_cast<relive::Path_WellLocal*>(BaseAliveGameObjectPathTLV);
                mVelX = (GetSpriteScale() * FP_FromInteger(pLocalWell->mOnDestX) / FP_FromInteger(100));
                mVelY = (GetSpriteScale() * FP_FromInteger(pLocalWell->mOnDestY) / FP_FromInteger(100));
            }
            else
            {
                mVelX = (GetSpriteScale() * FP_FromInteger(pWellBase->mOffDestX) / FP_FromInteger(100));
                mVelY = (GetSpriteScale() * FP_FromInteger(pWellBase->mOffDestY) / FP_FromInteger(100));
            }
        }

        MapFollowMe(1);

        mYPos += mVelY;
        gPointlessWellScale_4C73CC = FP_FromDouble(1.8);

        if (mVelX >= FP_FromInteger(0))
        {
            GetAnimation().SetFlipX(false);
        }
        else
        {
            GetAnimation().SetFlipX(true);
        }

        SfxPlayMono(relive::SoundEffects::WellExit, 0, GetSpriteScale());

        // TODO: handle this better
        s16 nextMotion = static_cast<s16>(mCurrentMotion);
        nextMotion++;
        mCurrentMotion = static_cast<eAbeMotions>(nextMotion); // can be motion 76 and 79 maybe more?

        if (GetSpriteScale() == FP_FromDouble(0.5))
        {
            GetAnimation().SetRenderLayer(Layer::eLayer_BeforeWell_Half_3);
        }
        else
        {
            GetAnimation().SetRenderLayer(Layer::eLayer_BeforeWell_22);
        }

        BaseAliveGameObjectCollisionLine = nullptr;
    }
}

void Abe::Motion_79_WellShotOut()
{
    if (mVelY >= FP_FromInteger(0))
    {
        Motion_3_Fall();
    }
    else
    {
        mVelY += (GetSpriteScale() * gPointlessWellScale_4C73CC);

        mXPos += mVelX;
        mYPos += mVelY;

        SetActiveCameraDelayedFromDir();
        BaseAliveGameObjectPathTLV = gMap.TLV_Get_At(
            nullptr,
            mXPos,
            mYPos,
            mXPos,
            mYPos);
    }

    if (GetAnimation().GetForwardLoopCompleted()
        || (mCurrentMotion != eAbeMotions::Motion_79_WellShotOut
            && mCurrentMotion != eAbeMotions::Motion_85_ToWellShotOut
            && mCurrentMotion != eAbeMotions::Motion_76_ToWellShotOut))
    {
        if (GetSpriteScale() == FP_FromDouble(0.5))
        {
            GetAnimation().SetRenderLayer(Layer::eLayer_AbeMenu_Half_13);
        }
        else
        {
            GetAnimation().SetRenderLayer(Layer::eLayer_AbeMenu_32);
        }

        GetShadow()->mEnabled = true;
    }

    if (mCurrentMotion == eAbeMotions::Motion_86_FallLandDie)
    {
        mHealth = FP_FromInteger(1);
        mCurrentMotion = eAbeMotions::Motion_98_LandSoft;
    }
}

void Abe::Motion_80_ToWellBegin()
{
    Motion_77_WellBegin();
}

void Abe::Motion_81_InsideWellExpress()
{
    BaseAliveGameObjectPathTLV = gMap.VTLV_Get_At_Of_Type(
        FP_GetExponent(mXPos),
        FP_GetExponent(mYPos),
        FP_GetExponent(mXPos),
        FP_GetExponent(mYPos),
        ReliveTypes::eWellLocal);

    if (!BaseAliveGameObjectPathTLV)
    {
        BaseAliveGameObjectPathTLV = gMap.VTLV_Get_At_Of_Type(
            FP_GetExponent(mXPos),
            FP_GetExponent(mYPos),
            FP_GetExponent(mXPos),
            FP_GetExponent(mYPos),
            ReliveTypes::eWellExpress);
    }


    if (mSnapAbe)
    {
        mCurrentMotion = eAbeMotions::Motion_78_InsideWellLocal;
        return;
    }

    relive::Path_WellExpress* pExpressWell = static_cast<relive::Path_WellExpress*>(BaseAliveGameObjectPathTLV);
    if (SwitchStates_Get(pExpressWell->mSwitchId))
    {
        field_190_level = pExpressWell->mOnDestLevel;
        field_192_path = pExpressWell->mOnDestPath;
        field_194_camera = pExpressWell->mOnDestCamera;
        field_196_door_id = pExpressWell->mOnOtherWellId;
    }
    else
    {
        field_190_level = pExpressWell->mOffDestLevel;
        field_192_path = pExpressWell->mOffDestPath;
        field_194_camera = pExpressWell->mOffDestCamera;
        field_196_door_id = pExpressWell->mOffOtherWellId;
    }

    field_120_x_vel_slow_by = FP_FromInteger(0);

    if (gMap.mCurrentLevel == EReliveLevelIds::eLines)
    {
        if (field_190_level == EReliveLevelIds::eForest)
        {
            if (mParamoniaDone)
            {
                mWalkToRun = true;
                field_114_gnFrame = 0;
                mCurrentMotion = eAbeMotions::Motion_78_InsideWellLocal;
                Motion_78_InsideWellLocal();
                return;
            }
            LOG_INFO("set bParamoniaDone");
            mParamoniaDone = true;
        }

        if (field_190_level == EReliveLevelIds::eDesert)
        {
            if (mScrabaniaDone)
            {
                mWalkToRun = true;
                field_114_gnFrame = 0;
                mCurrentMotion = eAbeMotions::Motion_78_InsideWellLocal;
                Motion_78_InsideWellLocal();
                return;
            }
            LOG_INFO("set bScrabinaDone");
            mScrabaniaDone = true;
        }
    }
    if (field_190_level != gMap.mCurrentLevel
        || field_192_path != gMap.mCurrentPath
        || field_194_camera != gMap.mCurrentCamera)
    {
        field_114_gnFrame = 1;
        if (pExpressWell->mMovieId)
        {
            gMap.SetActiveCam(field_190_level, field_192_path, field_194_camera, CameraSwapEffects::ePlay1FMV_5, pExpressWell->mMovieId, 0);
        }
        else
        {
            gMap.SetActiveCam(field_190_level, field_192_path, field_194_camera, CameraSwapEffects::eInstantChange_0, 0, 0);
        }
        mCurrentMotion = eAbeMotions::Motion_82_WellExpressShotOut;
    }
    else
    {
        Motion_82_WellExpressShotOut();
        mVelX = FP_FromInteger(0);
        mVelY = FP_FromInteger(0);
        mYPos = mYPos - mVelY;
        mWalkToRun = true;
        mCurrentMotion = eAbeMotions::Motion_78_InsideWellLocal;
    }
}

void Abe::Motion_82_WellExpressShotOut()
{
    PSX_Point camCoords = {};
    gMap.GetCurrentCamCoords(&camCoords);
    relive::Path_WellBase* pWell = nullptr;
    relive::Path_TLV* pTlvIter = nullptr;
    do
    {
        pTlvIter = gMap.TLV_Get_At(
            pTlvIter,
            FP_FromInteger(camCoords.x + 256),
            FP_FromInteger(camCoords.y + 120),
            FP_FromInteger(camCoords.x + 624),
            FP_FromInteger(camCoords.y + 360));
        if (!pTlvIter)
        {
            break;
        }

        if ((pTlvIter->mTlvType == ReliveTypes::eWellLocal || pTlvIter->mTlvType == ReliveTypes::eWellExpress) && static_cast<relive::Path_WellBase*>(pTlvIter)->mOtherWellId == field_196_door_id)
        {
            pWell = static_cast<relive::Path_WellBase*>(pTlvIter);
            break;
        }
    }
    while (pTlvIter);


    mCurrentLevel = gMap.mCurrentLevel;
    mCurrentPath = gMap.mCurrentPath;

    if (pWell)
    {
        if (pWell->mScale == relive::reliveScale::eHalf)
        {
            SetSpriteScale(FP_FromDouble(0.5));
            SetScale(Scale::Bg);
        }
        else
        {
            SetSpriteScale(FP_FromInteger(1));
            SetScale(Scale::Fg);
        }

        mCurrentMotion = eAbeMotions::Motion_78_InsideWellLocal;

        BaseAliveGameObjectPathTLV = pWell;
        mXPos = FP_FromInteger((pWell->mTopLeftX + pWell->mBottomRightX) / 2);
        mYPos = FP_FromInteger(pWell->mBottomRightY);
        mSnapAbe = true;
    }
    else
    {
        mXPos = FP_FromInteger(camCoords.x + 450);
        mYPos = FP_FromInteger(camCoords.y + 320);
        mVelX = (GetSpriteScale() * -FP_FromDouble(2.7));
        mVelY = (GetSpriteScale() * -FP_FromInteger(15));
    }
}

void Abe::Motion_83_ToWellBegin()
{
    Motion_77_WellBegin();
}

void Abe::Motion_84_ToInsideWellLocal()
{
    Motion_78_InsideWellLocal();
}

void Abe::Motion_85_ToWellShotOut()
{
    Motion_79_WellShotOut();
}

void Abe::Motion_86_FallLandDie()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (GetAnimation().GetCurrentFrame() == 0)
    {
        SfxPlayMono(relive::SoundEffects::KillEffect, 85);
        SND_SEQ_Play(SeqId::eHitBottomOfDeathPit_10, 1, 95, 95);
        relive_new ScreenShake(true, false);
    }

    if (GetAnimation().GetIsLastFrame())
    {
        if (static_cast<s32>(sGnFrame) >= field_114_gnFrame)
        {
            ToDieFinal();
        }
    }
}

void Abe::Motion_87_ToFall()
{
    Motion_3_Fall();
}

s32 gCounter_507728 = 0;
u8** gpDemoPlaybackRes_50772C = nullptr;
s32 sAbeSound_507730 = 0;
BellSong* sBellSong = nullptr;

void Abe::Motion_88_HandstoneBegin()
{
    switch (field_110_state.stone)
    {
        case StoneStates::eHandstoneStart_0:
        {
            if (GetAnimation().GetForwardLoopCompleted())
            {
                auto pCircularFade = relive_new CircularFade(
                    mXPos,
                    mYPos,
                    GetSpriteScale(),
                    1,
                    0);

                mCircularFadeId = pCircularFade->mBaseGameObjectId;
                pCircularFade->GetAnimation().SetFlipX(GetAnimation().GetFlipX());

                field_110_state.stone = StoneStates::eGetHandstoneType_1;
                SfxPlayMono(relive::SoundEffects::IngameTransition, 90);
                BaseAliveGameObjectPathTLV = gMap.VTLV_Get_At_Of_Type(
                    FP_GetExponent(mXPos),
                    FP_GetExponent(mYPos),
                    FP_GetExponent(mXPos),
                    FP_GetExponent(mYPos),
                    ReliveTypes::eDemoPlaybackStone);
                if (!BaseAliveGameObjectPathTLV)
                    BaseAliveGameObjectPathTLV = gMap.VTLV_Get_At_Of_Type(
                        FP_GetExponent(mXPos),
                        FP_GetExponent(mYPos),
                        FP_GetExponent(mXPos),
                        FP_GetExponent(mYPos),
                        ReliveTypes::eBellSongStone);
                if (!BaseAliveGameObjectPathTLV)
                {
                    BaseAliveGameObjectPathTLV = gMap.VTLV_Get_At_Of_Type(
                        FP_GetExponent(mXPos),
                        FP_GetExponent(mYPos),
                        FP_GetExponent(mXPos),
                        FP_GetExponent(mYPos),
                        ReliveTypes::eMovieHandStone);
                    sAbeSound_507730 = SFX_Play_Pitch(relive::SoundEffects::HandstoneTransition, 127, -300);
                    if (!BaseAliveGameObjectPathTLV)
                        BaseAliveGameObjectPathTLV = gMap.VTLV_Get_At_Of_Type(
                            FP_GetExponent(mXPos),
                            FP_GetExponent(mYPos),
                            FP_GetExponent(mXPos),
                            FP_GetExponent(mYPos),
                            ReliveTypes::eHandStone);
                }

                if (BaseAliveGameObjectPathTLV)
                {
                    mHandStoneType = BaseAliveGameObjectPathTLV->mTlvType;
                    switch (mHandStoneType)
                    {
                        case ReliveTypes::eMovieHandStone:
                        {
                            mMovieStone = static_cast<relive::Path_MovieStone*>(BaseAliveGameObjectPathTLV);
                            break;
                        }
                        case ReliveTypes::eBellSongStone:
                        {
                            mBellsongStone = static_cast<relive::Path_BellsongStone*>(BaseAliveGameObjectPathTLV);
                            break;
                        }
                        case ReliveTypes::eDemoPlaybackStone:
                        {
                            ALIVE_FATAL("never expected eDemoPlaybackStone to be used");
                            //field_174_pathStone.demoId = ptlv->field_18_data.demoId;
                            break;
                        }
                        case ReliveTypes::eHandStone:
                        {
                            mHandStone = static_cast<relive::Path_HandStone*>(BaseAliveGameObjectPathTLV);
                            break;
                        }
                        default:
                            return;
                    }
                }
                else
                {
                    mCurrentMotion = eAbeMotions::Motion_89_HandstoneEnd;
                }
            }
            break;
        }
        case StoneStates::eGetHandstoneType_1:
        {
            auto pCircularFade = static_cast<CircularFade*>(sObjectIds.Find_Impl(mCircularFadeId));
            if (pCircularFade->VDone())
            {
                switch (mHandStoneType)
                {
                    case ReliveTypes::eMovieHandStone:
                    {
                        auto pFmvInfo = Path_Get_FMV_Record(
                            gMap.mCurrentLevel,
                            mMovieStone->mMovieId);

                        relive_new Movie(pFmvInfo->field_0_pName);

                        field_110_state.stone = StoneStates::eHandstoneMovieDone_2;
                        break;
                    }
                    case ReliveTypes::eBellSongStone:
                    {
                        sBellSong = relive_new BellSong(
                            mBellsongStone->mType,
                            Code_Convert(mBellsongStone->mCode1, mBellsongStone->mCode2));

                        SwitchStates_Do_Operation(mBellsongStone->mSwitchId, relive::reliveSwitchOp::eSetTrue);
                        field_110_state.stone = StoneStates::eBellSongDone_4;
                        break;
                    }
                    case ReliveTypes::eDemoPlaybackStone:
                        ALIVE_FATAL("never expected eDemoPlaybackStone to be used");
                        /*field_164_pCircularFade->SetDead(true);
                        field_164_pCircularFade = nullptr;
                        mState.stone = StoneStates::eFreeDemoPlaybackResources_3;
                        gCounter_507728 = 2;
                        gpDemoPlaybackRes_50772C = ResourceManager::GetLoadedResource(ResourceManager::Resource_Plbk, field_174_pathStone.demoId, 1, 0);
                        if (gpDemoPlaybackRes_50772C)
                        {
                            relive_new DemoPlayback(gpDemoPlaybackRes_50772C, 1);
                        }
                        break;*/
                    case ReliveTypes::eHandStone:
                    {
                        GetAnimation().SetRender(false);
                        field_110_state.stone = StoneStates::eWaitForInput_6;
                        field_16E_cameraIdx = 1;
                        pCircularFade->SetDead(true);
                        mCircularFadeId = Guid{};
                        auto pFade = relive_new Fade(Layer::eLayer_FadeFlash_40, FadeOptions::eFadeOut, 0, 8, relive::TBlendModes::eBlend_2);
                        if (pFade)
                        {
                            mFadeId = pFade->mBaseGameObjectId;
                        }
                        field_190_level = gMap.mCurrentLevel;
                        field_192_path = gMap.mCurrentPath;
                        field_194_camera = gMap.mCurrentCamera;
                        gMap.SetActiveCam(
                            mHandStone->mLevel1,
                            mHandStone->mPath1,
                            mHandStone->mCameraId1,
                            CameraSwapEffects::eInstantChange_0, 0, 0);
                        break;
                    }
                    default:
                        return;
                }
            }
            break;
        }
        case StoneStates::eHandstoneMovieDone_2:
        {
            if (gMovieRefCount == 0)
            {
                gPsxDisplay.PutCurrentDispEnv();
                gScreenManager->DecompressCameraToVRam(gMap.field_2C_camera_array[0]->mCamRes);
                gScreenManager->EnableRendering();
                auto pCircularFade = static_cast<CircularFade*>(sObjectIds.Find_Impl(mCircularFadeId));
                pCircularFade->VFadeIn(0, 0);
                field_110_state.stone = StoneStates::eHandstoneEnd_5;
            }
            break;
        }
        case StoneStates::eFreeDemoPlaybackResources_3:
        {
            gCounter_507728--;
            if (gCounter_507728 == 0)
            {
                field_110_state.stone = StoneStates::eHandstoneEnd_5;
                auto pCircularFade = relive_new CircularFade(
                    mXPos,
                    mYPos,
                    GetSpriteScale(),
                    0,
                    0);
                mCircularFadeId = pCircularFade->mBaseGameObjectId;
                pCircularFade->GetAnimation().SetFlipX(GetAnimation().GetFlipX());
            }
            break;
        }
        case StoneStates::eBellSongDone_4:
        {
            if (sBellSong->mDone)
            {
                sBellSong->SetDead(true);
                auto pCircularFade = static_cast<CircularFade*>(sObjectIds.Find_Impl(mCircularFadeId));
                pCircularFade->VFadeIn(0, 0);
                field_110_state.stone = StoneStates::eHandstoneEnd_5;
            }
            break;
        }
        case StoneStates::eHandstoneEnd_5:
        {
            auto pCircularFade = static_cast<CircularFade*>(sObjectIds.Find_Impl(mCircularFadeId));
            if (pCircularFade && pCircularFade->VDone())
            {
                pCircularFade->SetDead(true);
                mCurrentMotion = eAbeMotions::Motion_89_HandstoneEnd;
                if (sAbeSound_507730)
                {
                    SND_Stop_Channels_Mask(sAbeSound_507730);
                    sAbeSound_507730 = 0;
                }
            }
            break;
        }
        case StoneStates::eWaitForInput_6:
        {
            auto pFade = static_cast<Fade*>(sObjectIds.Find(mFadeId, ReliveTypes::eFade));
            if (pFade && pFade->mDone)
            {
                if (Input().IsAnyPressed(InputCommands::eHop | InputCommands::eThrowItem | InputCommands::eCrouchOrRoll | InputCommands::eDoAction))
                {
                    pFade->Init(Layer::eLayer_FadeFlash_40, FadeOptions::eFadeIn, 0, 8);
                    field_110_state.stone = StoneStates::eSetActiveCamToAbeOrWaitForInput_7;
                    SfxPlayMono(relive::SoundEffects::IngameTransition, 90);
                }
            }
            break;
        }
        case StoneStates::eSetActiveCamToAbeOrWaitForInput_7:
        {
            auto pFade = static_cast<Fade*>(sObjectIds.Find(mFadeId, ReliveTypes::eFade));
            if (pFade && pFade->mDone)
            {
                Path_Stone_camera camera = {};
                switch (field_16E_cameraIdx)
                {
                    case 0:
                        camera.level = MapWrapper::ToAO(mHandStone->mLevel1);
                        camera.path = mHandStone->mPath1;
                        camera.camera = mHandStone->mCameraId1;
                        break;

                    case 1:
                        camera.level = MapWrapper::ToAO(mHandStone->mLevel2);
                        camera.path = mHandStone->mPath2;
                        camera.camera = mHandStone->mCameraId2;
                        break;

                    case 2:
                        camera.level = MapWrapper::ToAO(mHandStone->mLevel3);
                        camera.path = mHandStone->mPath3;
                        camera.camera = mHandStone->mCameraId3;
                        break;

                    default:
                        LOG_ERROR("CameraIdx out of bounds %d", field_16E_cameraIdx);
                }
                if (field_16E_cameraIdx > 2 || (camera.level == LevelIds::eForestChase_14 || camera.level == LevelIds::eDesertEscape_15))
                {
                    field_110_state.stone = StoneStates::eSetActiveCamToAbe_12;
                }
                else
                {
                    pFade->SetDead(true);
                    field_110_state.stone = StoneStates::eWaitForInput_6;
                    field_16E_cameraIdx++;
                    pFade = relive_new Fade(Layer::eLayer_FadeFlash_40, FadeOptions::eFadeOut, 0, 8, relive::TBlendModes::eBlend_2);
                    mFadeId = pFade->mBaseGameObjectId;
                    gMap.SetActiveCam(MapWrapper::FromAO(camera.level), camera.path, camera.camera, CameraSwapEffects::eInstantChange_0, 0, 0);
                }
            }
            break;
        }
        case StoneStates::eSetActiveCamToAbe_12:
        {
            auto pFade = static_cast<Fade*>(sObjectIds.Find(mFadeId, ReliveTypes::eFade));
            if (pFade && pFade->mDone)
            {
                GetAnimation().SetRender(true);
                field_110_state.stone = StoneStates::eCircularFadeExit_13;
                gMap.SetActiveCam(field_190_level, field_192_path, field_194_camera, CameraSwapEffects::eInstantChange_0, 0, 0);
            }
            break;
        }
        case StoneStates::eCircularFadeExit_13:
        {
            auto pFade = static_cast<Fade*>(sObjectIds.Find(mFadeId, ReliveTypes::eFade));
            pFade->SetDead(true);

            auto pCircularFade = relive_new CircularFade(mXPos, mYPos, GetSpriteScale(), 0, 0);
            mCircularFadeId = pCircularFade->mBaseGameObjectId;
            field_110_state.stone = StoneStates::eHandstoneEnd_5;
            pCircularFade->GetAnimation().SetFlipX(GetAnimation().GetFlipX());

            if (sAbeSound_507730)
            {
                SND_Stop_Channels_Mask(sAbeSound_507730);
                sAbeSound_507730 = 0;
            }
            break;
        }
        default:
            return;
    }
}

void Abe::Motion_89_HandstoneEnd()
{
    if (GetAnimation().GetForwardLoopCompleted())
    {
        mCurrentMotion = eAbeMotions::Motion_0_Idle;
    }
}

void Abe::Motion_90_GrenadeMachineUse()
{
    if (GetAnimation().GetForwardLoopCompleted())
    {
        mCurrentMotion = eAbeMotions::Motion_0_Idle;
    }
}

void Abe::Motion_91_FallingFromGrab()
{
    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eAbeMotions::Motion_87_ToFall;
    }

    Motion_3_Fall();

    if (mCurrentMotion == eAbeMotions::Motion_86_FallLandDie)
    {
        mCurrentMotion = eAbeMotions::Motion_98_LandSoft;
    }

    mPreviousMotion = eAbeMotions::Motion_91_FallingFromGrab;
}

void Abe::Motion_92_ForceDownFromHoist()
{
    if (field_114_gnFrame == 0)
    {
        mYPos += (GetSpriteScale() * FP_FromInteger(80));
        GetShadow()->mShadowAtBottom = false;
        VOnTrapDoorOpen();
        BaseAliveGameObjectCollisionLine = nullptr;
        BaseAliveGameObjectLastLineYPos = mYPos;
        field_114_gnFrame++;
    }

    Motion_3_Fall();
}

void Abe::Motion_93_WalkOffEdge()
{
    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eAbeMotions::Motion_87_ToFall;
    }
    Motion_3_Fall();
}

void Abe::Motion_94_RunOffEdge()
{
    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eAbeMotions::Motion_87_ToFall;
    }
    Motion_3_Fall();
}

void Abe::Motion_95_SneakOffEdge()
{
    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eAbeMotions::Motion_87_ToFall;
    }
    Motion_3_Fall();
}

void Abe::Motion_96_HopToFall()
{
    if (GetAnimation().GetFlipX())
    {
        mXPos += (GetSpriteScale() * FP_FromInteger(5));
    }
    else
    {
        mXPos -= (GetSpriteScale() * FP_FromInteger(5));
    }

    mVelY += (GetSpriteScale() * FP_FromInteger(4));

    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eAbeMotions::Motion_87_ToFall;
    }

    Motion_3_Fall();
}

void Abe::Motion_97_RunJumpToFall()
{
    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eAbeMotions::Motion_87_ToFall;
    }
    Motion_3_Fall();
}

void Abe::Motion_98_LandSoft()
{
    FollowLift();

    if (GetAnimation().GetCurrentFrame() == 2)
    {
        if (!mLandSoft)
        {
            EventBroadcast(kEventNoise, this);
            EventBroadcast(kEventSuspiciousNoise, this);
        }

        mLandSoft = false;

        if (mPreviousMotion == eAbeMotions::Motion_3_Fall)
        {
            Environment_SFX(EnvironmentSfx::eLandingSoft_5, 0, 0x7FFF, this);
        }
        else
        {
            Environment_SFX(EnvironmentSfx::eHitGroundSoft_6, 0, 0x7FFF, this);
        }

        if (Input().IsAnyHeld(InputCommands::eLeft | InputCommands::eRight))
        {
            ToLeftRightMovement();
        }
    }

    if (GetAnimation().GetIsLastFrame())
    {
        ToIdle();
    }
}

void Abe::Motion_99_HoistBeginLong()
{
    FollowLift();

    if (GetAnimation().GetIsLastFrame())
    {
        BaseAliveGameObjectLastLineYPos = mYPos;
        mVelY = (GetSpriteScale() * FP_FromInteger(-8)) * FP_FromDouble(1.3);
        mYPos += mVelY;
        VOnTrapDoorOpen();
        mCurrentMotion = eAbeMotions::Motion_17_HoistIdle;
        BaseAliveGameObjectCollisionLine = nullptr;
    }
}

void Abe::Motion_100_RollOffLedge()
{
    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eAbeMotions::Motion_87_ToFall;
    }
    Motion_3_Fall();
}

void Abe::Motion_101_LeverUse()
{
    if (GetAnimation().GetIsLastFrame())
    {
        ToIdle();
    }
}

void Abe::Motion_102_ElumWalkLoop()
{
    // Empty
}

void Abe::Motion_103_ElumIdle()
{
    if (!Input().IsAnyHeld(InputCommands::eLeftGameSpeak | InputCommands::eRightGameSpeak) ||
        !Input().IsAnyPressed(InputCommands::eHop | InputCommands::eThrowItem | InputCommands::eCrouchOrRoll | InputCommands::eDoAction))
    {
        if (Input().GetPressed() == InputCommands::eDown && !gDDCheat_FlyingEnabled)
        {
            mCurrentMotion = eAbeMotions::Motion_137_ElumUnmountBegin;
        }
    }
    else
    {
        mCurrentMotion = eAbeMotions::Motion_115_ElumSpeak;
        if (Input().IsAnyHeld(InputCommands::eLeftGameSpeak))
        {
            if (Input().IsAnyPressed(InputCommands::eGameSpeak2))
            {
                gEventSystem->PushEvent(GameSpeakEvents::eAbe_FollowMe);
                Mudokon_SFX(MudSounds::eFollowMe_4, 0, 0, this);
            }
            else if (Input().IsAnyPressed(InputCommands::eGameSpeak4))
            {
                gEventSystem->PushEvent(GameSpeakEvents::eAbe_Wait);
                Mudokon_SFX(MudSounds::eWait_6, 0, 0, this);
            }
            else if (Input().IsAnyPressed(InputCommands::eGameSpeak1))
            {
                gEventSystem->PushEvent(GameSpeakEvents::eAbe_Hello);
                Mudokon_SFX(MudSounds::eHello_3, 0, 0, this);
            }
            else if (Input().IsAnyPressed(InputCommands::eGameSpeak3))
            {
                gEventSystem->PushEvent(GameSpeakEvents::eAbe_Anger);
                Mudokon_SFX(MudSounds::eAngry_5, 0, 0, this);
            }
        }

        if (Input().IsAnyHeld(InputCommands::eRightGameSpeak))
        {
            if (Input().IsAnyPressed(InputCommands::eGameSpeak6))
            {
                gEventSystem->PushEvent(GameSpeakEvents::eAbe_WhistleHigh);
                Mudokon_SFX(MudSounds::eWhistleHigh_1, 0, 0, this);
            }
            else if (Input().IsAnyPressed(InputCommands::eGameSpeak5))
            {
                gEventSystem->PushEvent(GameSpeakEvents::eAbe_WhistleLow);
                Mudokon_SFX(MudSounds::eWhistleLow_2, 0, 0, this);
            }
            else if (Input().IsAnyPressed(InputCommands::eGameSpeak8))
            {
                gEventSystem->PushEvent(GameSpeakEvents::eAbe_Laugh);
                Mudokon_SFX(MudSounds::eLaugh1_8, 0, 0, this);
            }
            else if (Input().IsAnyPressed(InputCommands::eGameSpeak7))
            {
                gEventSystem->PushEvent(GameSpeakEvents::eAbe_Fart);
                Mudokon_SFX(MudSounds::eFart_7, 0, 0, this);
            }
        }
    }
}

void Abe::Motion_104_ElumRunSlideStop()
{
    // Empty
}

void Abe::Motion_105_ElumRunTurn()
{
    // Empty
}

void Abe::Motion_106_Null()
{
    // Empty
}

void Abe::Motion_107_ElumHopBegin()
{
    // Empty
}

void Abe::Motion_108_ElumHopMid()
{
    // Empty
}

void Abe::Motion_109_ElumHopLand()
{
    // Empty
}

void Abe::Motion_110_ElumRunJumpBegin()
{
    // Empty
}

void Abe::Motion_111_ElumRunJumpMid()
{
    // Empty
}

void Abe::Motion_112_ElumRunJumpLand()
{
    // Empty
}

void Abe::Motion_113_ElumTurn()
{
    // Empty
}

void Abe::Motion_114_ElumRunLoop()
{
    // Empty
}

void Abe::Motion_115_ElumSpeak()
{
    EventBroadcast(kEventSpeaking, this);

    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eAbeMotions::Motion_103_ElumIdle;
    }
}

void Abe::Motion_116_Null()
{
    // Empty
}

void Abe::Motion_117_ElumWalkBegin()
{
    // Empty
}

void Abe::Motion_118_ElumRunBegin()
{
    // Empty
}

void Abe::Motion_119_Null()
{
    // Empty
}

void Abe::Motion_120_ElumRunToWalk()
{
    // Empty
}

void Abe::Motion_121_ElumMidRunToWalk()
{
    // Empty
}

void Abe::Motion_122_ElumRunTurnToRun()
{
    // Empty
}

void Abe::Motion_123_ElumRunTurnToWalk()
{
    // Empty
}

void Abe::Motion_124_ElumWalkEnd()
{
    // Empty
}

void Abe::Motion_125_ElumMidWalkEnd()
{
    // Empty
}

void Abe::Motion_126_ElumBeesStruggling()
{
    // Empty
}

void Abe::Motion_127_SlapBomb()
{
    if (gAbe->GetAnimation().GetCurrentFrame() >= 6)
    {
        if (field_15C_pThrowable)
        {
            field_15C_pThrowable->mBaseGameObjectRefCount--;
            field_15C_pThrowable->VOnAbeInteraction();
            field_15C_pThrowable = nullptr;
        }
    }

    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eAbeMotions::Motion_19_CrouchIdle;
    }
}

void Abe::Motion_128_KnockForward()
{
    ElumFree();

    if (GetAnimation().GetCurrentFrame() == 12)
    {
        FallOnBombs();
    }

    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    FollowLift();

    if (GetAnimation().GetRender())
    {
        if (BaseAliveGameObjectCollisionLine)
        {
            if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
            {
                mVelX = FP_FromInteger(0);
            }
            MoveWithVelocity(FP_FromDouble(0.7));
        }
        else
        {
            Motion_3_Fall();

            if (mCurrentMotion == eAbeMotions::Motion_86_FallLandDie || mCurrentMotion == eAbeMotions::Motion_98_LandSoft)
            {
                mCurrentMotion = eAbeMotions::Motion_128_KnockForward;
                field_114_gnFrame = MakeTimer(10);
            }
        }
    }

    if (GetAnimation().GetForwardLoopCompleted())
    {
        if ((BaseAliveGameObjectCollisionLine || !GetAnimation().GetRender()) /*&& !field_104_pending_resource_count*/)
        {
            if (mRidingElum)
            {
                mRidingElum = false;
            }

            if (field_114_gnFrame <= static_cast<s32>(sGnFrame))
            {
                if (mHealth > FP_FromInteger(0) || gAbeInvulnerableCheat)
                {
                    mCurrentMotion = eAbeMotions::Motion_130_KnockForwardGetUp;
                }
                else
                {
                    ToDieFinal();
                }
            }
        }
    }
}

void Abe::Motion_129_RollingKnockForward()
{
    if (GetAnimation().GetCurrentFrame() == 12)
    {
        FallOnBombs();
    }

    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (GetAnimation().GetForwardLoopCompleted())
    {
        if (field_114_gnFrame <= static_cast<s32>(sGnFrame))
        {
            if (mHealth > FP_FromInteger(0) || gAbeInvulnerableCheat)
            {
                mCurrentMotion = eAbeMotions::Motion_130_KnockForwardGetUp;
            }
            else
            {
                ToDieFinal();
            }
        }
    }
}

void Abe::Motion_130_KnockForwardGetUp()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (GetAnimation().GetIsLastFrame())
    {
        ToIdle();
    }
}

void Abe::Motion_131_LiftUseUp()
{
    mCurrentMotion = MoveLiftUpOrDown(FP_FromInteger(-4));
}

void Abe::Motion_132_LiftUseDown()
{
    mCurrentMotion = MoveLiftUpOrDown(FP_FromInteger(4));
}

void Abe::Motion_133_LiftGrabBegin()
{
    auto pLiftPoint = static_cast<LiftPoint*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    pLiftPoint->Move(FP_FromInteger(0), FP_FromInteger(0));

    mVelY = FP_FromInteger(0);
    if (GetAnimation().GetIsLastFrame())
    {
        if (Input().IsAnyHeld(InputCommands::eUp))
        {
            if (!pLiftPoint->OnTopFloor())
            {
                mCurrentMotion = eAbeMotions::Motion_131_LiftUseUp;
                return;
            }
        }
        else if (Input().IsAnyHeld(InputCommands::eDown))
        {
            if (!pLiftPoint->OnBottomFloor())
            {
                mCurrentMotion = eAbeMotions::Motion_132_LiftUseDown;
                return;
            }
        }
        mCurrentMotion = eAbeMotions::Motion_135_LiftGrabIdle;
    }
}

void Abe::Motion_134_LiftGrabEnd()
{
    mVelY = FP_FromInteger(0);
    if (GetAnimation().GetIsLastFrame())
    {
        ToIdle();
    }
}

void Abe::Motion_135_LiftGrabIdle()
{
    LiftPoint* pLiftPoint = static_cast<LiftPoint*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));

    FollowLift();

    pLiftPoint->Move(FP_FromInteger(0), FP_FromInteger(0));

    mVelY = FP_FromInteger(0);

    if (gBeeInstanceCount > 0 && gBeesNearAbe)
    {
        mCurrentMotion = eAbeMotions::Motion_141_BeesStrugglingOnLift;
    }
    else
    {
        if (Input().IsAnyHeld(InputCommands::eUp))
        {
            if (!pLiftPoint->OnTopFloor())
            {
                mCurrentMotion = eAbeMotions::Motion_131_LiftUseUp;
            }
        }
        else if (Input().IsAnyHeld(InputCommands::eDown))
        {
            if (!pLiftPoint->OnBottomFloor())
            {
                mCurrentMotion = eAbeMotions::Motion_132_LiftUseDown;
            }
        }
        else
        {
            if (pLiftPoint->OnAnyFloor())
            {
                mCurrentMotion = eAbeMotions::Motion_134_LiftGrabEnd;
            }
        }
    }
}

void Abe::Motion_136_ElumMountEnd()
{
    switch (GetAnimation().GetCurrentFrame())
    {
        case 0:
            if (!mSfxPlaying)
            {
                mSfxPlaying = true;
                SfxPlayMono(relive::SoundEffects::AbeGenericMovement, 0, GetSpriteScale());
            }
            break;

        case 5:
            if (!mSfxPlaying)
            {
                mSfxPlaying = true;
                Environment_SFX(EnvironmentSfx::eExhaustingElumMount_16, 0, 0x7FFF, this);
            }
            break;

        case 12:
            if (!mSfxPlaying)
            {
                mSfxPlaying = true;
                Environment_SFX(EnvironmentSfx::eMountElumSmackNoise_17, 0, 0x7FFF, this);
                SfxPlayMono(relive::SoundEffects::MountingElum, 0, GetSpriteScale());
            }
            break;

        case 15:
            if (!mSfxPlaying)
            {
                mSfxPlaying = true;
                Environment_SFX(EnvironmentSfx::eElumGetMountedNoise_18, 0, 0x7FFF, this);
            }
            break;

        default:
            mSfxPlaying = false;
            break;
    }

    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eAbeMotions::Motion_103_ElumIdle;
        sControlledCharacter = gElum;
        MusicController::static_PlayMusic(MusicController::MusicTypes::eAbeOnElum_1, nullptr, 0, 0);
        gAbe->GetShadow()->mEnabled = false;
        Environment_SFX(EnvironmentSfx::eAbeMountedElumNoise_19, 0, 0x7FFF, this);
    }
}

void Abe::Motion_137_ElumUnmountBegin()
{
    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eAbeMotions::Motion_138_ElumUnmountEnd;
    }
}

void Abe::Motion_138_ElumUnmountEnd()
{
    switch (GetAnimation().GetCurrentFrame())
    {
        case 4:
            if (!mSfxPlaying)
            {
                mSfxPlaying = true;
                SfxPlayMono(relive::SoundEffects::AbeGenericMovement, 17, GetSpriteScale());
            }
            break;

        case 12:
            if (!mSfxPlaying)
            {
                mSfxPlaying = true;
                Environment_SFX(EnvironmentSfx::eExhaustingElumMount_16, 0, 0x7FFF, this);
            }
            break;

        case 19:
            if (!mSfxPlaying)
            {
                mSfxPlaying = true;
                Environment_SFX(EnvironmentSfx::eElumGetMountedNoise_18, 0, 0x7FFF, this);
            }
            break;

        case 20:
            if (!mSfxPlaying)
            {
                mSfxPlaying = true;
                SfxPlayMono(relive::SoundEffects::MountingElum, 0, GetSpriteScale());
            }
            break;

        default:
            mSfxPlaying = false;
            break;
    }

    if (GetAnimation().GetCurrentFrame() >= 28)
    {
        mRidingElum = false;
        BaseAliveGameObjectCollisionLine = gElum->BaseAliveGameObjectCollisionLine;

        LiftPoint* pLiftPoint = static_cast<LiftPoint*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
        if (pLiftPoint)
        {
            if (BaseAliveGameObjectCollisionLine->mLineType != eLineTypes ::eDynamicCollision_32 &&
                BaseAliveGameObjectCollisionLine->mLineType != eLineTypes::eBackgroundDynamicCollision_36)
            {
                mPreviousMotion = mCurrentMotion;
                VOnTrapDoorOpen();
                mCurrentMotion = mPreviousMotion;
            }
        }
        else
        {
            if (BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eDynamicCollision_32 ||
                BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eBackgroundDynamicCollision_36)
            {
                PSX_RECT bRect = VGetBoundingRect();
                bRect.y += 5;
                bRect.h += 5;
                OnCollisionWith(
                    {bRect.x, bRect.y},
                    {bRect.w, bRect.h},
                    gPlatformsArray);
            }
        }
        sControlledCharacter = gAbe;
        MusicController::static_PlayMusic(MusicController::MusicTypes::eType0, this, 0, 0);
        gAbe->GetShadow()->mEnabled = false;
        ToIdle();
    }
}

void Abe::Motion_139_ElumMountBegin()
{
    if (/*!field_104_pending_resource_count &&*/ GetAnimation().GetIsLastFrame())
    {
        /*
        while (!ResourceManager::FreeResource_455550(field_1A4_resources.res[61]))
        {
            // Empty
        }

        field_1A4_resources.res[61] = {};
        field_104_pending_resource_count = 1;

        ResourceManager::LoadResourceFile("ABEWELM.BND", BaseAliveGameObject::OnResourceLoaded, this);
        */
        VOnTrapDoorOpen();
        mCurrentMotion = eAbeMotions::Motion_136_ElumMountEnd;
    }
}

void Abe::Motion_140_BeesStruggling()
{
    Motion_0_Idle();
    if ((!gBeeInstanceCount || !gBeesNearAbe) && mCurrentMotion == eAbeMotions::Motion_140_BeesStruggling)
    {
        ToIdle();
    }
}

void Abe::Motion_141_BeesStrugglingOnLift()
{
    LiftPoint* pLiftPoint = static_cast<LiftPoint*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));

    pLiftPoint->Move(FP_FromInteger(0), FP_FromInteger(12));

    mVelY = pLiftPoint->mVelY;
    if (pLiftPoint->GetDead())
    {
        VOnTrapDoorOpen();
    }
    SetActiveCameraDelayedFromDir();

    mVelY = pLiftPoint->mVelY;

    if (pLiftPoint->OnBottomFloor())
    {
        ToIdle();
    }
}

void Abe::Motion_142_RockThrowStandingHold()
{
    if (GetAnimation().GetCurrentFrame() >= 4)
    {
        if (Input().IsAnyHeld(InputCommands::eRight | InputCommands::eLeft | InputCommands::eUp | InputCommands::eDown))
        {
            if (Input().IsAnyHeld(InputCommands::eRight))
            {
                if (GetAnimation().GetFlipX())
                {
                    mThrowDirection = 0;
                }
                else
                {
                    mThrowDirection = 2;
                }
            }
            else if (Input().IsAnyHeld(InputCommands::eLeft))
            {
                if (GetAnimation().GetFlipX())
                {
                    mThrowDirection = 2;
                }
                else
                {
                    mThrowDirection = 0;
                }
            }
            else if (Input().IsAnyHeld(InputCommands::eUp))
            {
                mThrowDirection = 1;
            }
            else
            {
                mThrowDirection = 3;
            }
            mCurrentMotion = eAbeMotions::Motion_143_RockThrowStandingThrow;
        }
    }

    if (Input().IsAnyReleased(InputCommands::eThrowItem))
    {
        mThrowable->VToDead();
        mThrowable = nullptr;
        mCurrentMotion = eAbeMotions::Motion_144_RockThrowStandingEnd;
        if (!gInfiniteGrenades)
        {
            field_19C_throwable_count++;
        }
    }
}

void Abe::Motion_143_RockThrowStandingThrow()
{
    if (GetAnimation().GetCurrentFrame() == 0)
    {
        SfxPlayMono(relive::SoundEffects::AirStream, 0, GetSpriteScale());
    }

    if (GetAnimation().GetForwardLoopCompleted())
    {
        ToIdle();
    }
}

void Abe::Motion_144_RockThrowStandingEnd()
{
    if (GetAnimation().GetForwardLoopCompleted())
    {
        ToIdle();
    }
}

void Abe::Motion_145_RockThrowCrouchingHold()
{
    if (GetAnimation().GetCurrentFrame() >= 4)
    {
        if (Input().IsAnyHeld(InputCommands::eRight | InputCommands::eLeft | InputCommands::eUp | InputCommands::eDown))
        {
            mThrowDirection = 4;
            mCurrentMotion = eAbeMotions::Motion_146_RockThrowCrouchingThrow;
            if (mThrowable->Type() == ReliveTypes::eMeat)
            {
                mThrowDirection = 5;
            }
        }
    }

    if (Input().IsAnyReleased(InputCommands::eThrowItem))
    {
        mThrowable->VToDead();
        mThrowable = nullptr;
        mCurrentMotion = eAbeMotions::Motion_19_CrouchIdle;
        if (!gInfiniteGrenades)
        {
            field_19C_throwable_count++;
        }
    }
}

void Abe::Motion_146_RockThrowCrouchingThrow()
{
    FollowLift();

    if (!GetAnimation().GetCurrentFrame())
    {
        SfxPlayMono(relive::SoundEffects::AirStream, 0, GetSpriteScale());
    }

    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eAbeMotions::Motion_19_CrouchIdle;
    }
}

void Abe::Motion_147_ShotRolling()
{
    ElumFree();

    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    Motion_3_Fall();

    if (mCurrentMotion != eAbeMotions::Motion_147_ShotRolling && !gAbeInvulnerableCheat)
    {
        mCurrentMotion = eAbeMotions::Motion_147_ShotRolling;
        BaseAliveGameObjectCollisionLine = nullptr;
        mYPos += (GetSpriteScale() * FP_FromInteger(4));
    }

    if (!gMap.Is_Point_In_Current_Camera(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos,
            0))
    {
        if (GetAnimation().GetForwardLoopCompleted())
        {
            mYPos += FP_FromInteger(240);
            Mudokon_SFX(MudSounds::eDeathDropScream_17, 0, 0, this);
            ToDeathDropFall();
        }
    }
}

void Abe::Motion_148_Shot()
{
    ElumFree();

    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    Motion_3_Fall();

    if (mCurrentMotion != eAbeMotions::Motion_148_Shot && !gAbeInvulnerableCheat)
    {
        mCurrentMotion = eAbeMotions::Motion_148_Shot;
        BaseAliveGameObjectCollisionLine = nullptr;
        mYPos += (GetSpriteScale() * FP_FromInteger(4));
    }

    if (!gMap.Is_Point_In_Current_Camera(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos,
            0))
    {
        if (GetAnimation().GetForwardLoopCompleted())
        {
            mYPos += FP_FromInteger(240);
            Mudokon_SFX(MudSounds::eDeathDropScream_17, 0, 0, this);
            ToDeathDropFall();
        }
    }
}

void Abe::Motion_149_PickupItem()
{
    if (GetAnimation().GetCurrentFrame() == 7)
    {
        SfxPlayMono(relive::SoundEffects::PickupItem, 0, GetSpriteScale());
    }

    if (GetAnimation().GetForwardLoopCompleted())
    {
        mCurrentMotion = eAbeMotions::Motion_19_CrouchIdle;
    }

    if (gAbe->GetAnimation().GetCurrentFrame() >= 5)
    {
        if (field_15C_pThrowable)
        {
            field_15C_pThrowable->mBaseGameObjectRefCount--;
            field_15C_pThrowable->VOnAbeInteraction();
            field_15C_pThrowable = nullptr;
        }
    }
}

void Abe::Motion_150_Chant()
{
    FollowLift();
    if (field_110_state.chant != ChantStates::eWaitForUnpossessing_3 && field_110_state.chant != ChantStates::eUnpossessing_4)
    {
        SND_SEQ_PlaySeq(SeqId::eMudokonChant1_11, 0, 0);
    }

    auto pOrbWhirlWind = static_cast<OrbWhirlWind*>(sObjectIds.Find_Impl(mOrbWhirlWindId));
    switch (field_110_state.chant)
    {
        case ChantStates::eIdleChanting_0:
        {
            EventBroadcast(kEventSpeaking, this);
            EventBroadcast(kEventAbeOhm, this);
            auto pObjToPossess = FindObjectToPossess();
            if (field_168_ring_pulse_timer)
            {
                if (!field_16C_bHaveShrykull)
                {
                    const PSX_RECT rect = VGetBoundingRect();
                    AbilityRing::Factory(
                        FP_FromInteger((rect.w + rect.x) / 2),
                        FP_FromInteger((rect.h + rect.y) / 2),
                        RingTypes::eExplosive_Emit_1,
                        FP_FromInteger(1));
                    field_168_ring_pulse_timer = 0;
                }
            }

            if ((GetAnimation().GetIsLastFrame()
                 || GetAnimation().GetCurrentFrame() == 3)
                && !Input_IsChanting())
            {
                mCurrentMotion = eAbeMotions::Motion_151_ChantEnd;
                if (pOrbWhirlWind)
                {
                    pOrbWhirlWind->ToStop();
                    mOrbWhirlWindId = Guid{};
                }
                return;
            }
            if (!(sGnFrame % 4))
            {
                New_RandomizedChant_Particle(this);
            }
            if (static_cast<s32>(sGnFrame) >= field_114_gnFrame - 70)
            {
                if (pObjToPossess)
                {
                    if (!pOrbWhirlWind)
                    {
                        FP xPos = {};
                        if (GetAnimation().GetFlipX())
                        {
                            xPos = mXPos + FP_FromInteger(-4);
                        }
                        else
                        {
                            xPos = mXPos + FP_FromInteger(4);
                        }
                        pOrbWhirlWind = relive_new OrbWhirlWind(
                            xPos,
                            mYPos - GetSpriteScale() * FP_FromInteger(38),
                            GetSpriteScale(),
                            false);

                        mOrbWhirlWindId = pOrbWhirlWind->mBaseGameObjectId;
                    }
                }
                else
                {
                    field_114_gnFrame = MakeTimer(70);
                    if (pOrbWhirlWind)
                    {
                        pOrbWhirlWind->ToStop();
                        mOrbWhirlWindId = Guid{};
                    }
                }
            }

            if (static_cast<s32>(sGnFrame) > field_114_gnFrame)
            {
                field_18C_pObjToPossess = pObjToPossess;
                if (field_18C_pObjToPossess)
                {
                    field_18C_pObjToPossess->mBaseGameObjectRefCount++;
                    SFX_Play_Pitch(relive::SoundEffects::PossessEffect, 0, -600);
                    field_114_gnFrame = MakeTimer(30);
                    field_110_state.chant = ChantStates::ePossessVictim_1;

                    const PSX_RECT rect = field_18C_pObjToPossess->VGetBoundingRect();
                    pOrbWhirlWind->ToSpin(
                        FP_FromInteger((rect.w + rect.x) / 2),
                        FP_FromInteger((rect.h + rect.y) / 2),
                        pObjToPossess->GetSpriteScale(),
                        pObjToPossess);

                    relive_new PossessionFlicker(gAbe, 30, 128, 255, 255);
                }
            }
            break;
        }
        case ChantStates::ePossessVictim_1:
        {
            if (static_cast<s32>(sGnFrame) > field_114_gnFrame)
            {
                field_110_state.chant = ChantStates::ePossessedVictim_2;
                return;
            }
            if (field_18C_pObjToPossess)
            {
                if (field_18C_pObjToPossess->GetDead())
                {
                    field_18C_pObjToPossess->mBaseGameObjectRefCount--;
                    field_18C_pObjToPossess = nullptr;
                }
            }
            if (field_18C_pObjToPossess)
            {
                if (field_18C_pObjToPossess->Is_In_Current_Camera() == CameraPos::eCamCurrent_0)
                {
                    return;
                }
                mCurrentMotion = eAbeMotions::Motion_151_ChantEnd;
                field_18C_pObjToPossess->mBaseGameObjectRefCount--;
                field_18C_pObjToPossess = nullptr;
            }
            else
            {
                mCurrentMotion = eAbeMotions::Motion_151_ChantEnd;
            }
            if (pOrbWhirlWind)
            {
                pOrbWhirlWind->ToStop();
                mOrbWhirlWindId = Guid{};
            }
            break;
        }
        case ChantStates::ePossessedVictim_2:
        {
            EventBroadcast(kEventSpeaking, this);
            EventBroadcast(kEventAbeOhm, this);
            mOrbWhirlWindId = Guid{};
            if (field_18C_pObjToPossess)
            {
                if (field_18C_pObjToPossess->GetDead())
                {
                    field_18C_pObjToPossess->mBaseGameObjectRefCount--;
                    field_18C_pObjToPossess = nullptr;
                }
            }
            if (field_18C_pObjToPossess)
            {
                sControlledCharacter = field_18C_pObjToPossess;
                field_18C_pObjToPossess->VPossessed();
                field_18C_pObjToPossess->mBaseGameObjectRefCount--;
                field_18C_pObjToPossess = nullptr;
                if (sControlledCharacter->Type() == ReliveTypes::eSlig)
                {
                    mLaughAtChantEnd = true;
                }

                relive_new PossessionFlicker(sControlledCharacter, 60, 128, 255, 255);

                SND_SEQ_Stop(SeqId::eMudokonChant1_11);
                SFX_Play_Pitch(relive::SoundEffects::PossessEffect, 70, 400);
                field_110_state.chant = ChantStates::eWaitForUnpossessing_3;
            }
            else if (GetAnimation().GetIsLastFrame())
            {
                mCurrentMotion = eAbeMotions::Motion_151_ChantEnd;
            }
            break;
        }
        case ChantStates::eWaitForUnpossessing_3:
        {
            if (sControlledCharacter == this)
            {
                relive_new PossessionFlicker(sControlledCharacter, 15, 128, 255, 255);

                field_110_state.chant = ChantStates::eUnpossessing_4;
                field_114_gnFrame = MakeTimer(15);
            }
            break;
        }
        case ChantStates::eUnpossessing_4:
        {
            if (!(sGnFrame % 4))
            {
                New_RandomizedChant_Particle(this);
            }
            if (static_cast<s32>(sGnFrame) > field_114_gnFrame)
            {
                if (GetAnimation().GetIsLastFrame())
                {
                    mCurrentMotion = eAbeMotions::Motion_151_ChantEnd;
                }
            }
            break;
        }
        case ChantStates::eChantingForBirdPortal_6:
        {
            EventBroadcast(kEventSpeaking, this);
            EventBroadcast(kEventAbeOhm, this);
            if (!(sGnFrame % 4))
            {
                New_RandomizedChant_Particle(this);
            }

            break;
        }
        default:
            return;
    }
}

void Abe::Motion_151_ChantEnd()
{
    SND_SEQ_Stop(SeqId::eMudokonChant1_11);

    FollowLift();

    if (GetAnimation().GetIsLastFrame())
    {
        if (mLaughAtChantEnd)
        {
            mCurrentMotion = eAbeMotions::Motion_12_Speak;
            Mudokon_SFX(MudSounds::eLaugh1_8, 0, 0, this);
            mLaughAtChantEnd = false;
        }
        else
        {
            ToIdle();
        }
    }
}

void Abe::Motion_152_ElumFallOffEdge()
{
    // Empty
}

void Abe::Motion_153_ElumFall()
{
    // Empty
}

void Abe::Motion_154_ElumLand()
{
    // Empty
}

void Abe::Motion_155_ElumJumpToFall()
{
    // Empty
}

void Abe::Motion_156_DoorEnter()
{
    switch (field_110_state.door)
    {
        case AbeDoorStates::eAbeComesIn_0:
        {
            if (GetAnimation().GetIsLastFrame())
            {
                field_110_state.door = AbeDoorStates::eWaitABit_2;
                GetAnimation().SetRender(false);
                field_118_timer = MakeTimer(3);
            }
            return;
        }
        case AbeDoorStates::eUnused_1:
        {
            auto pFade = static_cast<Fade*>(sObjectIds.Find(mFadeId, ReliveTypes::eFade));
            if (pFade && pFade->mDone)
            {
                field_110_state.door = AbeDoorStates::eWaitABit_2;
                field_118_timer = MakeTimer(5);
            }
            ALIVE_FATAL("never expected AbeDoorStates::eUnused_1 to be called");
            return;
        }
        case AbeDoorStates::eWaitABit_2:
        {
            if (field_118_timer <= static_cast<s32>(sGnFrame))
            {
                field_110_state.door = AbeDoorStates::eClearTlvIds_3;
                field_118_timer = MakeTimer(3);
            }
            return;
        }
        case AbeDoorStates::eClearTlvIds_3:
        {
            if (field_118_timer <= static_cast<s32>(sGnFrame))
            {
                field_110_state.door = AbeDoorStates::eSetNewActiveCamera_4;
                auto pTlv = static_cast<relive::Path_ResetPath*>(gMap.VTLV_Get_At_Of_Type(
                    FP_GetExponent(mXPos),
                    FP_GetExponent(mYPos),
                    FP_GetExponent(mXPos),
                    FP_GetExponent(mYPos),
                    ReliveTypes::eResetPath));
                BaseAliveGameObjectPathTLV = pTlv;
                if (pTlv)
                {
                    if (pTlv->mClearIds == relive::reliveChoice::eYes)
                    {
                        for (s16 i = pTlv->mFrom; i <= pTlv->mTo; i++)
                        {
                            if (i != pTlv->mExclude && i > 1)
                            {
                                SwitchStates_Set(i, 0);
                            }
                        }
                    }
                    if (pTlv->mClearObjects == relive::reliveChoice::eYes)
                    {
                        gMap.ResetPathObjects(pTlv->mPath);
                    }
                }
            }
            return;
        }
        case AbeDoorStates::eSetNewActiveCamera_4:
        {
            auto pDoorTlv = static_cast<relive::Path_Door*>(gMap.VTLV_Get_At_Of_Type(
                FP_GetExponent(mXPos),
                FP_GetExponent(mYPos),
                FP_GetExponent(mXPos),
                FP_GetExponent(mYPos),
                ReliveTypes::eDoor));
            BaseAliveGameObjectPathTLV = pDoorTlv;
            gMap.field_1E_door = 1;
            const auto changeEffect = kPathChangeEffectToInternalScreenChangeEffect[pDoorTlv->mWipeEffect];
            s16 flag = 0;
            if (changeEffect == CameraSwapEffects::ePlay1FMV_5 || changeEffect == CameraSwapEffects::eUnknown_11)
            {
                flag = 1;
            }
            gMap.SetActiveCam(
                pDoorTlv->mNextLevel,
                pDoorTlv->mNextPath,
                pDoorTlv->mNextCamera,
                changeEffect,
                pDoorTlv->mMovieId,
                flag);
            field_110_state.door = AbeDoorStates::eSetNewAbePosition_5;
            field_196_door_id = pDoorTlv->mTargetDoorId;
            return;
        }
        case AbeDoorStates::eSetNewAbePosition_5:
        {
            mCurrentLevel = gMap.mCurrentLevel;
            mCurrentPath = gMap.mCurrentPath;
            gMap.field_1E_door = 0;
            auto pPathDoor = static_cast<relive::Path_Door*>(gMap.TLV_First_Of_Type_In_Camera(ReliveTypes::eDoor, 0));
            BaseAliveGameObjectPathTLV = pPathDoor;

            while (pPathDoor->mDoorId != field_196_door_id)
            {
                pPathDoor = static_cast<relive::Path_Door*>(Path_TLV::TLV_Next_Of_Type_446500(BaseAliveGameObjectPathTLV, ReliveTypes::eDoor));
                BaseAliveGameObjectPathTLV = pPathDoor;
                
                if (!BaseAliveGameObjectPathTLV)
                {
                    ALIVE_FATAL("Couldn't find target door. If this is a custom level, check if the level, path and camera is correct.");
                }
            }

            if (pPathDoor->mDoorType == relive::Path_Door::DoorTypes::eBasicDoor)
            {
                if (pPathDoor->mScale == relive::reliveScale::eHalf)
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
            }
            else if (pPathDoor->mDoorType == relive::Path_Door::DoorTypes::eTrialDoor || pPathDoor->mDoorType == relive::Path_Door::DoorTypes::eHubDoor)
            {
                if (gMap.mCurrentLevel != EReliveLevelIds::eRuptureFarmsReturn)
                {
                    SetSpriteScale(FP_FromDouble(0.5));
                    SetScale(Scale::Bg);
                }
                else
                {
                    SetSpriteScale(FP_FromInteger(1));
                    SetScale(Scale::Fg);
                }
            }
            GetAnimation().SetFlipX(pPathDoor->mExitDirection == relive::reliveXDirection::eRight);
            mXPos = FP_FromInteger((BaseAliveGameObjectPathTLV->Width()) / 2)
                          + FP_FromInteger(BaseAliveGameObjectPathTLV->mTopLeftX);
            MapFollowMe(true);

            FP hitX = {};
            FP hitY = {};
            if (gCollisions->Raycast(
                    mXPos,
                    FP_FromInteger(BaseAliveGameObjectPathTLV->mTopLeftY),
                    mXPos,
                    FP_FromInteger(BaseAliveGameObjectPathTLV->mBottomRightY),
                    &BaseAliveGameObjectCollisionLine,
                    &hitX,
                    &hitY,
                    GetSpriteScale() != FP_FromDouble(0.5) ? kFgWallsOrFloor : kBgWallsOrFloor))
            {
                mYPos = hitY;
            }

            field_110_state.door = AbeDoorStates::eAbeComesOut_6;
            field_118_timer = MakeTimer(30);
            return;
        }
        case AbeDoorStates::eAbeComesOut_6:
        {
            if (field_118_timer <= static_cast<s32>(sGnFrame))
            {
                GetAnimation().SetRender(true);
                field_110_state.door = AbeDoorStates::eAbeComesIn_0;
                mCurrentMotion = eAbeMotions::Motion_157_DoorExit;
            }
            return;
        }
        default:
            return;
    }
}

void Abe::Motion_157_DoorExit()
{
    if (GetAnimation().GetIsLastFrame())
    {
        BaseAliveGameObjectPathTLV = gMap.TLV_Get_At(
            nullptr,
            mXPos,
            mYPos,
            mXPos,
            mYPos);

        VOnTlvCollision(BaseAliveGameObjectPathTLV);

        ToIdle();
    }
}

void Abe::Motion_158_ElumKnockback()
{
    // Empty
}

void Abe::Motion_159_Idle_RubEyes()
{
    if (GetAnimation().GetIsLastFrame())
    {
        ToIdle();
    }
}

void Abe::Motion_160_Idle_Stretch_Arms()
{
    Motion_159_Idle_RubEyes();
}

void Abe::Motion_161_Idle_Yawn()
{
    Motion_159_Idle_RubEyes();
}

void Abe::Motion_162_ToShrykull()
{
    if (field_110_state.raw == 0)
    {
        if (GetAnimation().GetIsLastFrame())
        {
            GetAnimation().SetAnimate(false);
            GetAnimation().SetRender(false);

            field_110_state.raw = 1;

            relive_new Shrykull();
        }
    }
}

void Abe::Motion_163_ShrykullEnd()
{
    if (field_114_gnFrame)
    {
        field_114_gnFrame = field_114_gnFrame - 1;
    }
    else if (GetAnimation().GetForwardLoopCompleted())
    {
        mCurrentMotion = eAbeMotions::Motion_9_Speak;
        Mudokon_SFX(MudSounds::eLaugh1_8, 0, 0, this);
    }
}

void Abe::Motion_164_PoisonGasDeath()
{
    switch (GetAnimation().GetCurrentFrame())
    {
        case 0:
            SFX_Play_Pitch(relive::SoundEffects::Choke, 127, 128);
            break;
        case 9:
            SFX_Play_Pitch(relive::SoundEffects::Choke, 127, 384);
            break;
        case 28:
            SFX_Play_Pitch(relive::SoundEffects::Choke, 127, 640);
            break;
        case 32:
            Environment_SFX(EnvironmentSfx::eHitGroundSoft_6, 80, 0, this);
            break;
        case 50:
            Environment_SFX(EnvironmentSfx::eHitGroundSoft_6, 100, -200, this);
            break;
        case 53:
            Environment_SFX(EnvironmentSfx::eHitGroundSoft_6, 50, -200, this);
            break;
        default:
            break;
    }

    if (GetAnimation().GetIsLastFrame())
    {
        const s32 v2 = field_114_gnFrame;
        field_114_gnFrame--;
        if (v2 == 0)
        {
            ToDieFinal();
        }
    }
}

} // namespace AO
