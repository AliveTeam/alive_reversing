#include "stdafx.h"
#include "Slig.hpp"
#include "Function.hpp"
#include "Collisions.hpp"
#include "Shadow.hpp"
#include "ShadowZone.hpp"
#include "stdlib.hpp"
#include "Map.hpp"
#include "PathData.hpp"
#include "Abe.hpp"
#include "MusicController.hpp"
#include "DDCheat.hpp"
#include "BaseGameObject.hpp"
#include "CrawlingSlig.hpp" // TODO: SFX playing only
#include "SnoozeParticle.hpp"
#include "Events.hpp"
#include "Sfx.hpp"
#include "Blood.hpp"
#include "Gibs.hpp"
#include "Particle.hpp"
#include "Sound/Midi.hpp"
#include "ObjectIds.hpp"
#include "PlatformBase.hpp"
#include "ScreenShake.hpp"
#include "ScreenManager.hpp"
#include "LiftPoint.hpp"
#include "Lever.hpp"
#include "SwitchStates.hpp"
#include "Bullet.hpp"
#include "Dove.hpp"
#include "BulletShell.hpp"
#include "GameSpeak.hpp"
#include "AmbientSound.hpp"
#include "VRam.hpp"
#include "Electrocute.hpp"
#include "Grid.hpp"

const SfxDefinition kSfxInfoTable_5607E0[17] = {
    {0u, 1u, 58u, 40u, -256, -256},
    {0u, 1u, 58u, 35u, 0, 0},
    {0u, 1u, 59u, 55u, 0, 0},
    {0u, 1u, 59u, 55u, 127, 127},
    {0u, 1u, 60u, 60u, 0, 0},
    {0u, 1u, 61u, 60u, 0, 0},
    {0u, 1u, 54u, 60u, 0, 0},
    {0u, 1u, 55u, 60u, 0, 0},
    {0u, 39u, 60u, 110u, -1000, -950},
    {0u, 1u, 48u, 90u, 0, 0},
    {0u, 1u, 49u, 90u, 0, 0},
    {0u, 1u, 50u, 90u, 0, 0},
    {0u, 0u, 66u, 50u, -512, -384},
    {0u, 0u, 67u, 50u, -512, -384},
    {0u, 3u, 36u, 60u, 0, 0},
    {0u, 3u, 37u, 60u, 0, 0},
    {0u, 3u, 38u, 60u, 0, 0}};

void Slig_SoundEffect_4BFFE0(SligSfx effect, BaseAliveGameObject* pObj)
{
    const SfxDefinition* pEffect = &kSfxInfoTable_5607E0[static_cast<s32>(effect)];
    s16 vLeft = 0;
    s16 vRight = 0;
    if (Calc_Slig_Sound_Direction_4C01B0(pObj, 0, pEffect, &vLeft, &vRight))
    {
        s16 pitch = 0;
        if (effect == SligSfx::ePropeller1_9 || effect == SligSfx::ePropeller2_10 || effect == SligSfx::ePropeller3_11)
        {
            FP sndDistance = FP_FromInteger(abs(Math_Distance(0, 0, FP_GetExponent(pObj->field_C4_velx), FP_GetExponent(pObj->field_C8_vely))));
            if (sndDistance > FP_FromInteger(8))
            {
                sndDistance = FP_FromInteger(8);
            }

            pitch = Math_RandomRange(-127, 127) + FP_GetExponent((sndDistance / FP_FromInteger(8)) * FP_FromInteger(768)) + 512;
        }
        else
        {
            pitch = Math_RandomRange(pEffect->field_4_pitch_min, pEffect->field_6_pitch_max);
        }
        SFX_SfxDefinition_Play_Stereo(pEffect, vLeft, vRight, pitch, pitch);
    }
}

s32 Animation_OnFrame_Slig_4C0600(void* pObj, s16* pData)
{
    auto pSlig = reinterpret_cast<Slig*>(pObj);
    auto pPoints = reinterpret_cast<PSX_Point*>(pData);

    if (pSlig->UpdateDelay())
    {
        return 1;
    }

    BulletType bulletType = BulletType::eSligPossessedOrUnderGlukkonCommand_0;
    if (pSlig->mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit4_bPossesed) || pSlig->vUnderGlukkonCommand_4B1760())
    {
        bulletType = BulletType::eSligPossessedOrUnderGlukkonCommand_0;
    }
    else
    {
        bulletType = BulletType::eNormalBullet_2;
    }

    const FP xOff = (pSlig->field_CC_sprite_scale * FP_FromInteger(pPoints->field_0_x));
    const FP yOff = (pSlig->field_CC_sprite_scale * FP_FromInteger(pPoints->field_2_y));

    Bullet* pBullet = nullptr;

    FP bullet_xDist = FP_FromInteger(0);
    s8 fireDirection = 0;
    FP fireXpos = FP_FromInteger(0);
    s16 shellDirection = 0;
    if (pSlig->field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        bullet_xDist = FP_FromInteger(-640);
        fireDirection = 1;
        fireXpos = pSlig->mBaseAnimatedWithPhysicsGameObject_XPos - xOff;
        shellDirection = 0;
    }
    else
    {
        bullet_xDist = FP_FromInteger(640);
        fireDirection = 0;
        fireXpos = xOff + pSlig->mBaseAnimatedWithPhysicsGameObject_XPos;
        shellDirection = 1;
    }

    pBullet = ae_new<Bullet>(pSlig, bulletType, pSlig->mBaseAnimatedWithPhysicsGameObject_XPos, yOff + pSlig->mBaseAnimatedWithPhysicsGameObject_YPos, bullet_xDist, 0, pSlig->field_CC_sprite_scale, 0);
    if (pBullet)
    {
        pBullet->SetUpdateDelay(1);
    }

    New_ShootingFire_Particle(fireXpos, yOff + pSlig->mBaseAnimatedWithPhysicsGameObject_YPos, fireDirection, pSlig->field_CC_sprite_scale);

    if (pSlig->field_CC_sprite_scale == FP_FromDouble(0.5))
    {
        SFX_Play_Mono(SoundEffect::SligShoot_5, 85);
    }
    else
    {
        ae_new<BulletShell>(pSlig->mBaseAnimatedWithPhysicsGameObject_XPos, yOff + pSlig->mBaseAnimatedWithPhysicsGameObject_YPos, shellDirection, pSlig->field_CC_sprite_scale);
        SFX_Play_Mono(SoundEffect::SligShoot_5, 0);
    }

    Event_Broadcast(kEventShooting, pSlig);
    Event_Broadcast(kEventLoudNoise, pSlig);

    Dove::All_FlyAway(0);
    return 1;
}

TintEntry sSligTint_560570[15] = {
    {LevelIds_s8::eMines_1, 102u, 127u, 118u},
    {LevelIds_s8::eNecrum_2, 102u, 127u, 118u},
    {LevelIds_s8::eMudomoVault_3, 102u, 127u, 118u},
    {LevelIds_s8::eMudancheeVault_4, 102u, 127u, 118u},
    {LevelIds_s8::eFeeCoDepot_5, 102u, 127u, 118u},
    {LevelIds_s8::eBarracks_6, 102u, 127u, 118u},
    {LevelIds_s8::eMudancheeVault_Ender_7, 102u, 127u, 118u},
    {LevelIds_s8::eBonewerkz_8, 102u, 127u, 118u},
    {LevelIds_s8::eBrewery_9, 102u, 127u, 118u},
    {LevelIds_s8::eBrewery_Ender_10, 102u, 127u, 118u},
    {LevelIds_s8::eMudomoVault_Ender_11, 102u, 127u, 118u},
    {LevelIds_s8::eFeeCoDepot_Ender_12, 102u, 127u, 118u},
    {LevelIds_s8::eBarracks_Ender_13, 102u, 127u, 118u},
    {LevelIds_s8::eBonewerkz_Ender_14, 102u, 127u, 118u},
    {LevelIds_s8::eNone, 102u, 127u, 118u}};

ALIVE_VAR(1, 0xBAF7E4, s32, unused_BAF7E4, 0);
ALIVE_VAR(1, 0xBAF7E8, s16, sSligsUnderControlCount_BAF7E8, 0);

const AnimId sSligFrameTables_547318[52] = {
    AnimId::Slig_Idle,
    AnimId::Slig_StandToWalk,
    AnimId::Slig_Walking,
    AnimId::Slig_StandToRun,
    AnimId::Slig_Running,
    AnimId::Slig_TurnAroundStanding,
    AnimId::Slig_Shoot,
    AnimId::Slig_Idle,
    AnimId::Slig_SlidingToStand,
    AnimId::Slig_SlidingTurn,
    AnimId::Slig_SlidingTurnToWalk,
    AnimId::Slig_SlidingTurnToRun,
    AnimId::Slig_ReloadGun,
    AnimId::Slig_ShootToStand,
    AnimId::Slig_SteppingToStand,
    AnimId::Slig_StandingToStep,
    AnimId::Slig_Idle,
    AnimId::Slig_Gamespeak,
    AnimId::Slig_WalkToStand,
    AnimId::Slig_Recoil,
    AnimId::Slig_Speak1,
    AnimId::Slig_Speak2,
    AnimId::Slig_Speak3,
    AnimId::Slig_Speak4,
    AnimId::Slig_Speak1,
    AnimId::Slig_Speak2,
    AnimId::Slig_Speak3,
    AnimId::Slig_Speak4,
    AnimId::Slig_Speak1,
    AnimId::Slig_Speak2,
    AnimId::Slig_Speak3,
    AnimId::Slig_Speak4,
    AnimId::Slig_Sleeping,
    AnimId::Slig_SleepingToStand,
    AnimId::Slig_Knockback,
    AnimId::Slig_KnockbackToStand,
    AnimId::Slig_PossessShake,
    AnimId::Slig_PossessShake,
    AnimId::Slig_OutToFall,
    AnimId::Slig_FallingInitiate,
    AnimId::Slig_LandingSoft,
    AnimId::Slig_LandingFatal,
    AnimId::Slig_ShootZ,
    AnimId::Slig_ShootZtoStand,
    AnimId::Slig_Smash,
    AnimId::Slig_PullLever,
    AnimId::Slig_LiftGrip,
    AnimId::Slig_LiftUngrip,
    AnimId::Slig_LiftGripping,
    AnimId::Slig_LiftUp,
    AnimId::Slig_LiftDown,
    AnimId::Slig_Beat};

const TSligMotionFn sSlig_motion_table_5604A0[52] = {
    &Slig::M_StandIdle_0_4B4EC0,
    &Slig::M_StandToWalk_1_4B5F70,
    &Slig::M_Walking_2_4B5BC0,
    &Slig::M_StandToRun_3_4B62F0,
    &Slig::M_Running_4_4B6000,
    &Slig::M_TurnAroundStanding_5_4B6390,
    &Slig::M_Shoot_6_4B55A0,
    &Slig::M_Falling_7_4B42D0,
    &Slig::M_SlidingToStand_8_4B6520,
    &Slig::M_SlidingTurn_9_4B6680,
    &Slig::M_SlidingTurnToWalk_10_4B6800,
    &Slig::M_SlidingTurnToRun_11_4B6850,
    &Slig::M_ReloadGun_12_4B5530,
    &Slig::M_ShootToStand_13_4B5580,
    &Slig::M_SteppingToStand_14_4B8480,
    &Slig::M_StandingToStep_15_4B83B0,
    &Slig::M_DepossessingAbort_16_4B8250,
    &Slig::M_GameSpeak_17_4B5290,
    &Slig::M_WalkToStand_18_4B5FC0,
    &Slig::M_Recoil_19_4B8270,
    &Slig::M_SpeakHereBoy_20_4B5330,
    &Slig::M_SpeakHi_21_4B53D0,
    &Slig::M_SpeakFreeze_22_4B53F0,
    &Slig::M_SpeakGetHim_23_4B5410,
    &Slig::M_SpeakLaugh_24_4B5430,
    &Slig::M_SpeakBullshit1_25_4B5450,
    &Slig::M_SpeakLookOut_26_4B5470,
    &Slig::M_SpeakBullshit2_27_4B5490,
    &Slig::M_SpeakPanic_28_4B54B0,
    &Slig::M_SpeakWhat_29_4B54D0,
    &Slig::M_SpeakAIFreeze_30_4B54F0,
    &Slig::M_Blurgh_31_4B5510,
    &Slig::M_Sleeping_32_4B89A0,
    &Slig::M_SleepingToStand_33_4B8C50,
    &Slig::M_Knockback_34_4B68A0,
    &Slig::M_KnockbackToStand_35_4B6A30,
    &Slig::M_Depossessing_36_4B7F30,
    &Slig::M_Possess_37_4B72C0,
    &Slig::M_OutToFall_38_4B4570,
    &Slig::M_FallingInitiate_39_4B4640,
    &Slig::M_LandingSoft_40_4B4530,
    &Slig::M_LandingFatal_41_4B4680,
    &Slig::M_ShootZ_42_4B7560,
    &Slig::M_ShootZtoStand_43_4B77E0,
    &Slig::M_Smash_44_4B6B90,
    &Slig::M_PullLever_45_4B8950,
    &Slig::M_LiftGrip_46_4B3700,
    &Slig::M_LiftUngrip_47_4B3820,
    &Slig::M_LiftGripping_48_4B3850,
    &Slig::M_LiftUp_49_4B3930,
    &Slig::M_LiftDown_50_4B3960,
    &Slig::M_Beat_51_4B6C00};

const TSligBrainFn sSlig_brain_table_5605AC[36] = {
    &Slig::Brain_Death_0_4BBFB0,
    &Slig::Brain_ReturnControlToAbeAndDie_1_4BC410,
    &Slig::Brain_Possessed_2_4BBCF0,
    &Slig::Brain_DeathDropDeath_3_4BC1E0,
    &Slig::Brain_ListeningToGlukkon_4_4B9D20,
    &Slig::Brain_Empty_5_4B3220,
    &Slig::Brain_Empty_6_4B3420,
    &Slig::Brain_SpottedEnemy_7_4B3240,
    &Slig::Brain_Empty_8_4B3120,
    &Slig::Brain_Empty_9_4B3440,
    &Slig::Brain_EnemyDead_10_4B3460,
    &Slig::Brain_KilledEnemy_10_4B35C0,
    &Slig::Brain_PanicTurning_12_4BC490,
    &Slig::Brain_PanicRunning_13_4BC780,
    &Slig::Brain_PanicYelling_14_4BCA70,
    &Slig::Brain_Idle_15_4BD800,
    &Slig::Brain_StopChasing_16_4BCE30,
    &Slig::Brain_Chasing_17_4BCBD0,
    &Slig::Brain_StartChasing_18_4BCEB0,
    &Slig::Brain_Turning_19_4BDDD0,
    &Slig::Brain_StoppingNextToMudokon_20_4BF1E0,
    &Slig::Brain_Walking_21_4BE0C0,
    &Slig::Brain_GetAlertedTurn_22_4BE990,
    &Slig::Brain_GetAlerted_23_4BEC40,
    &Slig::Brain_BeatingUp_24_4BF2B0,
    &Slig::Brain_DiscussionWhat_25_4BF380,
    &Slig::Brain_Empty_26_4BF620,
    &Slig::Brain_Empty_27_4BF600,
    &Slig::Brain_ZShooting_28_4BFA70,
    &Slig::Brain_Shooting_29_4BF750,
    &Slig::Brain_ZSpottedEnemy_30_4BFA30,
    &Slig::Brain_WakingUp_31_4B9390,
    &Slig::Brain_Inactive_32_4B9430,
    &Slig::Brain_Paused_33_4B8DD0,
    &Slig::Brain_Sleeping_34_4B9170,
    &Slig::Brain_ChaseAndDisappear_35_4BF640};

static BrainFunctionData<TSligBrainFn> sSligBrainTable[36] = {
    {
        &Slig::Brain_Death_0_4BBFB0,
        0x40128F,
        "Brain_Death_0",
    },
    {
        &Slig::Brain_ReturnControlToAbeAndDie_1_4BC410,
        0x40371F,
        "Brain_ReturnControlToAbeAndDie_1",
    },
    {
        &Slig::Brain_Possessed_2_4BBCF0,
        0x40458E,
        "Brain_Possessed_2",
    },
    {
        &Slig::Brain_DeathDropDeath_3_4BC1E0,
        0x401EC9,
        "Brain_DeathDropDeath_3",
    },
    {
        &Slig::Brain_ListeningToGlukkon_4_4B9D20,
        0x4015A0,
        "Brain_ListeningToGlukkon_4",
    },
    {
        &Slig::Brain_Empty_5_4B3220,
        0x4B3220,
        "Brain_Empty_5",
    },
    {
        &Slig::Brain_Empty_6_4B3420,
        0x4B3420,
        "Brain_Empty_6",
    },
    {
        &Slig::Brain_SpottedEnemy_7_4B3240,
        0x4032AB,
        "Brain_SpottedEnemy_7",
    },
    {
        &Slig::Brain_Empty_8_4B3120,
        0x4B3120,
        "Brain_Empty_8",
    },
    {
        &Slig::Brain_Empty_9_4B3440,
        0x4B3440,
        "Brain_Empty_9",
    },
    {
        &Slig::Brain_EnemyDead_10_4B3460,
        0x401E60,
        "Brain_EnemyDead_10",
    },
    {
        &Slig::Brain_KilledEnemy_10_4B35C0,
        0x40120D,
        "Brain_KilledEnemy_10",
    },
    {
        &Slig::Brain_PanicTurning_12_4BC490,
        0x40479B,
        "Brain_PanicTurning_12",
    },
    {
        &Slig::Brain_PanicRunning_13_4BC780,
        0x40144C,
        "Brain_PanicRunning_13",
    },
    {
        &Slig::Brain_PanicYelling_14_4BCA70,
        0x403364,
        "Brain_PanicYelling_14",
    },
    {
        &Slig::Brain_Idle_15_4BD800,
        0x403F85,
        "Brain_Idle_15",
    },
    {
        &Slig::Brain_StopChasing_16_4BCE30,
        0x4021B2,
        "Brain_StopChasing_16",
    },
    {
        &Slig::Brain_Chasing_17_4BCBD0,
        0x403BCA,
        "Brain_Chasing2_17",
    },
    {
        &Slig::Brain_StartChasing_18_4BCEB0,
        0x403E36,
        "Brain_Chasing1_18",
    },
    {
        &Slig::Brain_Turning_19_4BDDD0,
        0x403ABC,
        "Brain_Turning_19",
    },
    {
        &Slig::Brain_StoppingNextToMudokon_20_4BF1E0,
        0x4020C2,
        "Brain_StoppingNextToMudokon_20",
    },
    {
        &Slig::Brain_Walking_21_4BE0C0,
        0x402A59,
        "Brain_Walking_21",
    },
    {
        &Slig::Brain_GetAlertedTurn_22_4BE990,
        0x40266C,
        "Brain_GetAlertedTurn_22",
    },
    {
        &Slig::Brain_GetAlerted_23_4BEC40,
        0x4020D1,
        "Brain_GetAlerted_23",
    },
    {
        &Slig::Brain_BeatingUp_24_4BF2B0,
        0x403B25,
        "Brain_BeatingUp_24",
    },
    {
        &Slig::Brain_DiscussionWhat_25_4BF380,
        0x403CE2,
        "Brain_DiscussionWhat_25",
    },
    {
        &Slig::Brain_Empty_26_4BF620,
        0x4BF620,
        "Brain_Empty_26",
    },
    {
        &Slig::Brain_Empty_27_4BF600,
        0x4BF600,
        "Brain_Empty_27",
    },
    {
        &Slig::Brain_ZShooting_28_4BFA70,
        0x4025B3,
        "Brain_ShootingFromBackground_28",
    },
    {
        &Slig::Brain_Shooting_29_4BF750,
        0x40308F,
        "Brain_Shooting_29",
    },
    {
        &Slig::Brain_ZSpottedEnemy_30_4BFA30,
        0x403215,
        "Brain_SpottedEnemyFromBackground_30",
    },
    {
        &Slig::Brain_WakingUp_31_4B9390,
        0x403E0E,
        "Brain_WakingUp_31",
    },
    {
        &Slig::Brain_Inactive_32_4B9430,
        0x4038CD,
        "Brain_Inactive_32",
    },
    {
        &Slig::Brain_Paused_33_4B8DD0,
        0x40346D,
        "Brain_Paused_33",
    },
    {
        &Slig::Brain_Sleeping_34_4B9170,
        0x402B17,
        "Brain_Sleeping_34",
    },
    {&Slig::Brain_ChaseAndDisappear_35_4BF640, 0x4022B1, "Brain_GameEnder_35"},
};

void Slig::SetBrain(TSligBrainFn fn)
{
    ::SetBrain(fn, field_154_brain_state, sSligBrainTable);
}

bool Slig::BrainIs(TSligBrainFn fn)
{
    return ::BrainIs(fn, field_154_brain_state, sSligBrainTable);
}

Slig::Slig(Path_Slig* pTlv, s32 tlvInfo)
    : BaseAliveGameObject(17)
{
    field_160_last_event_index = -1;
    field_176_unused = -1;
    field_174_unused = 0;

    if (tlvInfo != 0xFFFF)
    {
        mBaseGameObjectTlvInfo = tlvInfo;
    }

    field_10_resources_array.SetAt(0, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kSlgbasicResID, 1, 0));

    const AnimRecord& rec = AnimRec(AnimId::Slig_Idle);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);

    SetType(AETypes::eSlig_125);

    mBaseAliveGameObjectFlags.Clear(Flags_114::e114_Bit4_bPossesed);
    mBaseAliveGameObjectFlags.Clear(Flags_114::e114_Bit9_RestoredFromQuickSave);
    mBaseAliveGameObjectFlags.Set(Flags_114::e114_Bit3_Can_Be_Possessed);
    mBaseAliveGameObjectFlags.Set(Flags_114::e114_Bit6_SetOffExplosives);

    field_292_prevent_depossession &= ~1u;

    unused_BAF7E4 = 0;

    field_216_flags.Clear(Flags_216::eBit1_FollowGlukkon);
    field_216_flags.Clear(Flags_216::eBit3_GlukkonCalledAllOYa);
    field_216_flags.Clear(Flags_216::eBit4_HeardGlukkon);


    field_120_timer = 0;
    field_11E_pitch_min = 45 * (Math_NextRandom() % 5 - 2);
    mNextMotion = eSligMotions::M_StandIdle_0_4B4EC0;
    field_124_return_to_previous_motion = 0;
    field_150_explode_timer = 0;
    field_14C_death_by_being_shot_timer = 0;
    BaseAliveGameObjectPathTLV = pTlv;

    field_218_tlv_data = *pTlv;

    BaseAliveGameObjectId = -1;

    mCurrentMotion = eSligMotions::M_Falling_7_4B42D0;

    field_124_return_to_previous_motion = 0;
    mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(pTlv->field_8_top_left.field_0_x);
    mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);
    field_130_falling_velx_scale_factor = FP_FromInteger(0);
    field_118_tlvInfo = tlvInfo;
    field_128_input = 0;
    field_158_num_times_to_shoot = 0;
    field_15A_unused = 0;
    field_15C_force_alive_state = 0;
    field_136_shot_motion = -1;
    field_208_glukkon_obj_id = 0;
    field_20C_state_after_speak = -1;
    field_20E_attention_timeout = 0;
    field_210_unused = 0;
    field_134_res_idx = 0;

    field_20_animation.field_1C_fn_ptr_array = kSlig_Anim_Frame_Fns_55EFAC;

    if (pTlv->field_10_scale != Scale_short::eFull_0)
    {
        if (pTlv->field_10_scale == Scale_short::eHalf_1)
        {
            field_CC_sprite_scale = FP_FromDouble(0.5);
            field_20_animation.mRenderLayer = Layer::eLayer_SligGreeterFarts_Half_14;
            field_D6_scale = 0;
        }
    }
    else
    {
        field_CC_sprite_scale = FP_FromInteger(1);
        field_20_animation.mRenderLayer = Layer::eLayer_SligGreeterFarts_33;
        field_D6_scale = 1;
    }

    SetBaseAnimPaletteTint(&sSligTint_560570[0], gMap.mCurrentLevel, 412);

    FP hitX = {};
    FP hitY = {};
    if (sCollisions_DArray_5C1128->Raycast(
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(24),
            &BaseAliveGameObjectCollisionLine,
            &hitX,
            &hitY,
            field_D6_scale != 0 ? 1 : 16)
        == 1)
    {
        mCurrentMotion = eSligMotions::M_StandIdle_0_4B4EC0;
        mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
    }

    MapFollowMe(TRUE);

    Init();

    VStackOnObjectsOfType(AETypes::eSlig_125);

    if (gMap.mCurrentLevel == LevelIds::eBonewerkz_8 && gMap.mCurrentPath == 2 && gMap.mCurrentCamera == 5)
    {
        field_DA_xOffset = 0;
    }

    mApplyShadows |= 2u;
    field_142_unused = 0;
    field_140_unused = 0;

    mShadow = ae_new<Shadow>();
}

void renderWithGlowingEyes(PrimHeader** ot, BaseAliveGameObject* actor, s16* pPalAlloc, s16 palSize, PSX_RECT* palRect,
                           s16& r, s16& g, s16& b,
                           const s16* eyeColourIndices, s16 eyeColourIndicesSize)
{
    if (actor->field_20_animation.mAnimFlags.Get(AnimFlags::eBit3_Render))
    {
        if (gMap.mCurrentPath == actor->field_C0_path_number && gMap.mCurrentLevel == actor->field_C2_lvl_number && actor->Is_In_Current_Camera() == CameraPos::eCamCurrent_0)
        {
            actor->field_20_animation.field_14_scale = actor->field_CC_sprite_scale;

            PSX_RECT boundingRect = {};
            actor->VGetBoundingRect(&boundingRect, 1);
            s16 rMod = actor->field_D0_r;
            s16 gMod = actor->field_D2_g;
            s16 bMod = actor->field_D4_b;
            ShadowZone::ShadowZones_Calculate_Colour(
                FP_GetExponent(actor->mBaseAnimatedWithPhysicsGameObject_XPos),
                (boundingRect.h + boundingRect.y) / 2,
                actor->field_D6_scale,
                &rMod,
                &gMod,
                &bMod);
            if (!actor->mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit7_Electrocuted))
            {
                if (rMod != r || gMod != g || bMod != b)
                {
                    r = rMod;
                    g = gMod;
                    b = bMod;

                    const FrameInfoHeader* pFrameInfoHeader = actor->field_20_animation.Get_FrameHeader(0);
                    const u8* pAnimData = *actor->field_20_animation.field_20_ppBlock;
                    const u32 clut_offset = *reinterpret_cast<const u32*>(&(pAnimData)[pFrameInfoHeader->field_0_frame_header_offset]);
                    const u16* pAnimDataWithOffset = reinterpret_cast<const u16*>(&pAnimData[clut_offset + 4]);
                    for (s32 i = 0; i < palSize; i++)
                    {
                        s32 auxPalValue = pAnimDataWithOffset[i] & 0x1F;
                        u16 resultR = static_cast<s16>(auxPalValue * r) >> 7;
                        if (resultR > 31)
                        {
                            resultR = 31;
                        }

                        auxPalValue = (pAnimDataWithOffset[i] >> 5) & 0x1F;
                        u16 resultG = static_cast<s16>(auxPalValue * g) >> 7;
                        if (resultG > 31)
                        {
                            resultG = 31;
                        }

                        auxPalValue = (pAnimDataWithOffset[i] >> 10) & 0x1F;
                        u16 resultB = static_cast<s16>(auxPalValue * b) >> 7;
                        if (resultB > 31)
                        {
                            resultB = 31;
                        }

                        s32 resultMixed = (pAnimDataWithOffset[i] & 0x8000) | ((resultR & 31) + 32 * (resultG & 31) + 32 * 32 * (resultB & 31));
                        if (resultMixed <= 0 && pAnimDataWithOffset[i])
                        {
                            resultMixed = 1;
                        }
                        pPalAlloc[i] = static_cast<u16>(resultMixed);
                    }
                    for (s32 i = 0; i < eyeColourIndicesSize; i++)
                    {
                        pPalAlloc[eyeColourIndices[i]] = pAnimDataWithOffset[eyeColourIndices[i]];
                    }
                    Pal_Set_483510(
                        actor->field_20_animation.field_8C_pal_vram_xy,
                        actor->field_20_animation.field_90_pal_depth,
                        reinterpret_cast<const u8*>(pPalAlloc),
                        palRect);
                }
                actor->field_20_animation.mRed = 127;
                actor->field_20_animation.mGreen = 127;
                actor->field_20_animation.mBlue = 127;
            }

            actor->field_20_animation.VRender(
                FP_GetExponent(FP_FromInteger(actor->field_DA_xOffset) + actor->mBaseAnimatedWithPhysicsGameObject_XPos - pScreenManager->field_20_pCamPos->field_0_x),
                FP_GetExponent(FP_FromInteger(actor->field_D8_yOffset) + actor->mBaseAnimatedWithPhysicsGameObject_YPos - pScreenManager->field_20_pCamPos->field_4_y),
                ot,
                0,
                0);

            PSX_RECT rectToInvalidate = {};
            actor->field_20_animation.Get_Frame_Rect(&rectToInvalidate);
            pScreenManager->InvalidateRect_40EC90(
                rectToInvalidate.x,
                rectToInvalidate.y,
                rectToInvalidate.w,
                rectToInvalidate.h, pScreenManager->field_3A_idx);

            if (actor->mShadow)
            {
                actor->mShadow->Calculate_Position(actor->mBaseAnimatedWithPhysicsGameObject_XPos, actor->mBaseAnimatedWithPhysicsGameObject_YPos, &rectToInvalidate, actor->field_CC_sprite_scale, actor->field_D6_scale);
                actor->mShadow->Render(ot);
            }
        }
    }
}

void Slig::VRender(PrimHeader** ot)
{
    const s16 eyeIndices[] = {61, 62};
    renderWithGlowingEyes(ot, this, &field_178_pPalAlloc[0], ALIVE_COUNTOF(field_178_pPalAlloc),
                          &field_1F8_pal_rect, field_200_red, field_202_green, field_204_blue, &eyeIndices[0], ALIVE_COUNTOF(eyeIndices));
}

s16 Slig::VIsFacingMe(BaseAnimatedWithPhysicsGameObject* pOther)
{
    return vIsFacingMe_4B23D0(pOther);
}

s16 Slig::VOnSameYLevel(BaseAnimatedWithPhysicsGameObject* pOther)
{
    return vOnSameYLevel_4BB6C0(pOther);
}

s32 Slig::VGetSaveState(u8* pSaveBuffer)
{
    if (mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit7_Electrocuted))
    {
        return 0;
    }

    auto pState = reinterpret_cast<Slig_State*>(pSaveBuffer);

    pState->field_0_type = AETypes::eSlig_125;

    pState->field_4_xpos = mBaseAnimatedWithPhysicsGameObject_XPos;
    pState->field_8_ypos = mBaseAnimatedWithPhysicsGameObject_YPos;

    pState->field_C_velx = field_C4_velx;
    pState->field_10_vely = field_C8_vely;

    pState->field_58_falling_velx_scale_factor = field_130_falling_velx_scale_factor;

    pState->field_14_path_number = field_C0_path_number;
    pState->field_16_lvl_number = field_C2_lvl_number;
    pState->field_18_sprite_scale = field_CC_sprite_scale;

    pState->field_1C_scale = field_D6_scale;

    pState->field_1E_r = field_D0_r;
    pState->field_20_g = field_D2_g;
    pState->field_22_b = field_D4_b;

    if (mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit11_Electrocuting))
    {
        for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
        {
            BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
            if (!pObj)
            {
                break;
            }

            if (pObj->Type() == AETypes::eElectrocute_150)
            {
                auto pElectrocute = static_cast<Electrocute*>(pObj);
                if (pElectrocute->field_20_target_obj_id == field_8_object_id)
                {
                    pState->field_1E_r = pElectrocute->field_24_r;
                    pState->field_20_g = pElectrocute->field_26_g;
                    pState->field_22_b = pElectrocute->field_28_b;
                    break;
                }
            }
        }
    }
    pState->field_24_bFlipX = field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX);
    pState->field_26_current_motion = mCurrentMotion;
    pState->field_28_current_frame = field_20_animation.field_92_current_frame;
    pState->field_2A_frame_change_counter = field_20_animation.mFrameChangeCounter;
    pState->field_2D_bDrawable = mBaseGameObjectFlags.Get(BaseGameObject::eDrawable_Bit4);
    pState->field_2C_bRender = field_20_animation.mAnimFlags.Get(AnimFlags::eBit3_Render);
    pState->field_30_health = mHealth;
    pState->field_34_current_motion = mCurrentMotion;
    pState->field_36_next_motion = mNextMotion;
    pState->field_38_last_line_ypos = FP_GetExponent(BaseAliveGameObjectLastLineYPos);
    pState->field_3A_collision_line_type = -1;

    if (BaseAliveGameObjectCollisionLine)
    {
        pState->field_3A_collision_line_type = BaseAliveGameObjectCollisionLine->field_8_type;
    }

    pState->field_40_bActiveChar = this == sControlledCharacter_5C1B8C;
    pState->field_42_brain_sub_state = field_11C_brain_sub_state;
    pState->field_44_pitch_min = field_11E_pitch_min;
    pState->field_48_timer = field_120_timer;
    pState->field_4C_return_to_previous_motion = field_124_return_to_previous_motion;
    pState->field_4E_checked_if_off_screen = field_126_checked_if_off_screen;
    pState->field_50_input = InputObject::KeyboardInputToPsxButtons_45EF70(field_128_input);
    pState->field_54_timer = field_12C_timer;
    pState->field_58_falling_velx_scale_factor = field_130_falling_velx_scale_factor;
    pState->field_5C_tlvInfo = field_118_tlvInfo;
    pState->field_60_res_idx = field_134_res_idx;
    pState->field_62_shot_motion = field_136_shot_motion;
    pState->field_64_zone_rect = field_138_zone_rect;
    pState->field_6C_unused = field_140_unused;
    pState->field_6E_unused = field_142_unused;
    pState->field_70_unused = field_144_unused;
    pState->field_72_level = field_146_level;
    pState->field_74_path = field_148_path;
    pState->field_76_camera = field_14A_camera;
    pState->field_78_death_by_being_shot_timer = field_14C_death_by_being_shot_timer;
    pState->field_7C_explode_timer = field_150_explode_timer;
    pState->field_80_brain_state_idx = 0;

    s32 idx = 0;
    for (const auto& fn : sSlig_brain_table_5605AC)
    {
        if (BrainIs(fn))
        {
            pState->field_80_brain_state_idx = idx;
        }
        idx++;
    }

    pState->field_88_unused = unused_BAF7E4;
    pState->field_8C_num_times_to_shoot = field_158_num_times_to_shoot;
    pState->field_90_force_alive_state = field_15C_force_alive_state;
    pState->field_92_spotted_possessed_slig = field_15E_spotted_possessed_slig;
    pState->field_94_glukkon_id = -1;

    if (field_208_glukkon_obj_id != -1)
    {
        BaseGameObject* pGlukkon = sObjectIds.Find_Impl(field_208_glukkon_obj_id);
        if (pGlukkon)
        {
            pState->field_94_glukkon_id = pGlukkon->mBaseGameObjectTlvInfo;
        }
    }
    pState->field_98_state_after_speak = field_20C_state_after_speak;
    pState->field_9A_attention_timeout = field_20E_attention_timeout;

    pState->field_9C_unused = field_210_unused;

    pState->field_9E_next_command_arg1 = field_212_next_command_arg1;
    pState->field_A0_cmd_idx = field_214_cmd_idx;

    pState->field_A2_flags.Set(Slig_State::eBit2_FollowGlukkon, field_216_flags.Get(Flags_216::eBit1_FollowGlukkon));
    pState->field_A2_flags.Set(Slig_State::eBit3_StoppedForLeverOrLift, field_216_flags.Get(Flags_216::eBit2_StoppedForLeverOrLift));
    pState->field_A2_flags.Set(Slig_State::eBit4_GlukkonCalledAllOYa, field_216_flags.Get(Flags_216::eBit3_GlukkonCalledAllOYa));
    pState->field_A2_flags.Set(Slig_State::eBit5_HeardGlukkon, field_216_flags.Get(Flags_216::eBit4_HeardGlukkon));

    return sizeof(Slig_State);
}

s32 Slig::CreateFromSaveState(const u8* pBuffer)
{
    auto pState = reinterpret_cast<const Slig_State*>(pBuffer);
    auto pTlv = static_cast<Path_Slig*>(sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam(pState->field_5C_tlvInfo));

    const s16 disabledResources = pTlv->field_48_disable_resources;

    if (!(disabledResources & 1) && !ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kSlgleverResID, 0, 0))
    {
        ResourceManager::LoadResourceFile_49C170("SLGLEVER.BAN", nullptr);
    }

    if (!(disabledResources & 2) && !ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kSlgliftResID, 0, 0))
    {
        ResourceManager::LoadResourceFile_49C170("SLGLIFT.BAN", nullptr);
    }

    if (!(disabledResources & 0x40) && !ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kSlgsleepResID, 0, 0))
    {
        ResourceManager::LoadResourceFile_49C170("SLGSLEEP.BAN", nullptr);
    }

    if ((disabledResources & 0x80u) == 0 && !ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kSlgknfdResID, 0, 0))
    {
        ResourceManager::LoadResourceFile_49C170("SLGKNFD.BAN", nullptr);
    }

    if (!(disabledResources & 0x100) && !ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kSlgedgeResID, 0, 0))
    {
        ResourceManager::LoadResourceFile_49C170("SLGEDGE.BAN", nullptr);
    }

    if (!(disabledResources & 0x200) && !ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kSlgsmashResID, 0, 0))
    {
        ResourceManager::LoadResourceFile_49C170("SLGSMASH.BAN", nullptr);
    }

    if (!(disabledResources & 0x400) && !ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kSlgbeatResID, 0, 0))
    {
        ResourceManager::LoadResourceFile_49C170("SLGBEAT.BAN", nullptr);
    }

    if (!(disabledResources & 4) && !ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kSlgzshotResID, 0, 0))
    {
        ResourceManager::LoadResourceFile_49C170("SLIGZ.BND", nullptr);
    }

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kSlgbasicResID, 0, 0))
    {
        ResourceManager::LoadResourceFile_49C170("SLIG.BND", nullptr);
    }

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kSligBlowResID, 0, 0))
    {
        ResourceManager::LoadResourceFile_49C170("SLGBLOW.BAN", nullptr);
    }

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kObjectShadowResID, 0, 0))
    {
        ResourceManager::LoadResourceFile_49C170("SHADOW.BAN", nullptr);
    }

    auto pSlig = ae_new<Slig>(pTlv, pState->field_5C_tlvInfo);
    if (pSlig)
    {
        if (pState->field_40_bActiveChar)
        {
            sControlledCharacter_5C1B8C = pSlig;
            pSlig->mBaseAliveGameObjectFlags.Set(Flags_114::e114_Bit4_bPossesed);
        }

        pSlig->BaseAliveGameObjectPathTLV = nullptr;
        pSlig->BaseAliveGameObjectCollisionLine = nullptr;
        pSlig->mBaseAnimatedWithPhysicsGameObject_XPos = pState->field_4_xpos;
        pSlig->mBaseAnimatedWithPhysicsGameObject_YPos = pState->field_8_ypos;
        pSlig->field_C4_velx = pState->field_C_velx;
        pSlig->field_C8_vely = pState->field_10_vely;
        pSlig->field_C0_path_number = pState->field_14_path_number;
        pSlig->field_C2_lvl_number = pState->field_16_lvl_number;
        pSlig->field_CC_sprite_scale = pState->field_18_sprite_scale;

        if (pSlig->field_CC_sprite_scale == FP_FromInteger(1))
        {
            pSlig->field_20_animation.mRenderLayer = Layer::eLayer_SligGreeterFarts_33;
        }
        else
        {
            pSlig->field_20_animation.mRenderLayer = Layer::eLayer_SligGreeterFarts_Half_14;
        }

        pSlig->field_D6_scale = pState->field_1C_scale;

        pSlig->field_200_red = pState->field_1E_r;
        pSlig->field_D0_r = pState->field_1E_r;
        pSlig->field_202_green = pState->field_20_g;
        pSlig->field_D2_g = pState->field_20_g;
        pSlig->field_204_blue = pState->field_22_b;
        pSlig->field_D4_b = pState->field_22_b;

        pSlig->mCurrentMotion = pState->field_26_current_motion;
        const AnimRecord& animRec = AnimRec(sSligFrameTables_547318[pState->field_26_current_motion]);
        u8** ppRes = pSlig->ResForMotion_4B1E90(pSlig->mCurrentMotion);
        pSlig->field_20_animation.Set_Animation_Data(animRec.mFrameTableOffset, ppRes);


        pSlig->field_20_animation.field_92_current_frame = pState->field_28_current_frame;
        pSlig->field_20_animation.mFrameChangeCounter = pState->field_2A_frame_change_counter;

        pSlig->mBaseGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4, pState->field_2D_bDrawable & 1);

        pSlig->field_20_animation.mAnimFlags.Set(AnimFlags::eBit5_FlipX, pState->field_24_bFlipX & 1);
        pSlig->field_20_animation.mAnimFlags.Set(AnimFlags::eBit3_Render, pState->field_2C_bRender & 1);

        if (IsLastFrame(&pSlig->field_20_animation))
        {
            pSlig->field_20_animation.mAnimFlags.Set(AnimFlags::eBit18_IsLastFrame);
        }

        pSlig->mHealth = pState->field_30_health;
        pSlig->mCurrentMotion = pState->field_34_current_motion;
        pSlig->mNextMotion = pState->field_36_next_motion;
        pSlig->BaseAliveGameObjectLastLineYPos = FP_FromInteger(pState->field_38_last_line_ypos);

        pSlig->mBaseAliveGameObjectFlags.Set(Flags_114::e114_Bit9_RestoredFromQuickSave);

        pSlig->BaseAliveGameObjectCollisionLineType = pState->field_3A_collision_line_type;
        pSlig->field_11C_brain_sub_state = pState->field_42_brain_sub_state;
        pSlig->field_11E_pitch_min = pState->field_44_pitch_min;
        pSlig->field_120_timer = pState->field_48_timer;
        pSlig->field_124_return_to_previous_motion = pState->field_4C_return_to_previous_motion;
        pSlig->field_126_checked_if_off_screen = pState->field_4E_checked_if_off_screen;

        pSlig->field_128_input = InputObject::PsxButtonsToKeyboardInput_45EE40(pState->field_50_input);

        pSlig->field_12C_timer = pState->field_54_timer;
        pSlig->field_130_falling_velx_scale_factor = pState->field_58_falling_velx_scale_factor;
        pSlig->field_118_tlvInfo = pState->field_5C_tlvInfo;
        pSlig->field_134_res_idx = pState->field_60_res_idx;
        pSlig->field_136_shot_motion = pState->field_62_shot_motion;

        pSlig->field_138_zone_rect = pState->field_64_zone_rect;

        pSlig->field_140_unused = pState->field_6C_unused;
        pSlig->field_142_unused = pState->field_6E_unused;
        pSlig->field_144_unused = pState->field_70_unused;

        pSlig->field_146_level = pState->field_72_level;
        pSlig->field_148_path = pState->field_74_path;
        pSlig->field_14A_camera = pState->field_76_camera;

        pSlig->field_14C_death_by_being_shot_timer = pState->field_78_death_by_being_shot_timer;
        pSlig->field_150_explode_timer = pState->field_7C_explode_timer;

        pSlig->SetBrain(sSlig_brain_table_5605AC[pState->field_80_brain_state_idx]);

        unused_BAF7E4 = pState->field_88_unused;

        pSlig->field_158_num_times_to_shoot = static_cast<s16>(pState->field_8C_num_times_to_shoot); // TODO: Wrong type ??
        pSlig->field_15C_force_alive_state = pState->field_90_force_alive_state;
        pSlig->field_15E_spotted_possessed_slig = pState->field_92_spotted_possessed_slig;
        pSlig->field_208_glukkon_obj_id = pState->field_94_glukkon_id;
        pSlig->field_20C_state_after_speak = pState->field_98_state_after_speak;
        pSlig->field_20E_attention_timeout = pState->field_9A_attention_timeout;

        pSlig->field_210_unused = pState->field_9C_unused;
        pSlig->field_212_next_command_arg1 = pState->field_9E_next_command_arg1;

        pSlig->field_214_cmd_idx = pState->field_A0_cmd_idx;

        pSlig->field_216_flags.Set(Flags_216::eBit1_FollowGlukkon, pState->field_A2_flags.Get(Slig_State::eBit2_FollowGlukkon));
        pSlig->field_216_flags.Set(Flags_216::eBit2_StoppedForLeverOrLift, pState->field_A2_flags.Get(Slig_State::eBit3_StoppedForLeverOrLift));
        pSlig->field_216_flags.Set(Flags_216::eBit3_GlukkonCalledAllOYa, pState->field_A2_flags.Get(Slig_State::eBit4_GlukkonCalledAllOYa));
        pSlig->field_216_flags.Set(Flags_216::eBit4_HeardGlukkon, pState->field_A2_flags.Get(Slig_State::eBit5_HeardGlukkon));

        sSligsUnderControlCount_BAF7E8 = 0;
    }

    return sizeof(Slig_State);
}

void Slig::M_StandIdle_0_4B4EC0()
{
    FP xOff = {};
    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        xOff = FP_FromInteger(10) * field_CC_sprite_scale;
    }
    else
    {
        xOff = FP_FromInteger(-10) * field_CC_sprite_scale;
    }

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    if (!sCollisions_DArray_5C1128->Raycast(
            xOff + mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(10),
            xOff + mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(10),
            &pLine,
            &hitX,
            &hitY,
            field_D6_scale != 0 ? 1 : 16))
    {
        VOnTrapDoorOpen();
        mCurrentMotion = eSligMotions::M_FallingInitiate_39_4B4640;
        BaseAliveGameObjectLastLineYPos = mBaseAnimatedWithPhysicsGameObject_YPos;
        field_130_falling_velx_scale_factor = FP_FromDouble(0.3);
        return;
    }

    if (!MainMovement_4B4720())
    {
        if (sControlledCharacter_5C1B8C == this && mHealth > FP_FromInteger(0))
        {
            if (Input_IsChanting_45F260())
            {
                if (!(field_292_prevent_depossession & 1))
                {
                    mCurrentMotion = eSligMotions::M_Depossessing_36_4B7F30;
                    field_12C_timer = sGnFrame + 30;
                    SFX_Play_Mono(SoundEffect::PossessEffect_17, 0);
                    return;
                }
            }
            else
            {
                const auto inputHeld = sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_C_held;
                const auto gameSpeakCmds = InputCommands::Enum::eChant | InputCommands::Enum::eGameSpeak8 | InputCommands::Enum::eGameSpeak7 | InputCommands::Enum::eGameSpeak6 | InputCommands::Enum::eGameSpeak5 | InputCommands::Enum::eGameSpeak4 | InputCommands::Enum::eGameSpeak3 | InputCommands::Enum::eGameSpeak2 | InputCommands::Enum::eGameSpeak1;

                if (inputHeld & gameSpeakCmds)
                {
                    field_128_input = inputHeld;
                    mCurrentMotion = eSligMotions::M_GameSpeak_17_4B5290;
                    return;
                }
            }
        }

        if (static_cast<s32>(sGnFrame) >= field_12C_timer)
        {
            mCurrentMotion = eSligMotions::M_ReloadGun_12_4B5530;
        }
    }
}

void Slig::M_StandToWalk_1_4B5F70()
{
    field_128_input |= sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_C_held;
    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eSligMotions::M_Walking_2_4B5BC0;
    }
    MoveOnLine_4B4C40();
}

void Slig::M_Walking_2_4B5BC0()
{
    if (gMap.GetDirection_4811A0(
            field_C2_lvl_number,
            field_C0_path_number,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos)
            >= CameraPos::eCamCurrent_0
        && MusicController::static_GetMusicType(0, 0, 0) <= MusicController::MusicTypes::eTension_4)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eTension_4, this, 0, 0);
    }

    field_128_input |= sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_C_held;

    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        field_C4_velx = -(ScaleToGridSize(field_CC_sprite_scale) / FP_FromInteger(9));
    }
    else
    {
        field_C4_velx = (ScaleToGridSize(field_CC_sprite_scale) / FP_FromInteger(9));
    }

    if (WallHit(field_CC_sprite_scale * FP_FromInteger(45), field_C4_velx * FP_FromInteger(2)))
    {
        ToStand_4B4A20();
        return;
    }

    MoveOnLine_4B4C40();

    if (mCurrentMotion == eSligMotions::M_Walking_2_4B5BC0)
    {
        if (field_20_animation.field_92_current_frame == 5 || field_20_animation.field_92_current_frame == 14)
        {
            Slig_SoundEffect_4BFFE0(SligSfx::eWalkingStep_2, this);

            if (!field_126_checked_if_off_screen)
            {
                field_126_checked_if_off_screen = 1;
                MapFollowMe(TRUE);
            }

            if (sControlledCharacter_5C1B8C == this && mHealth > FP_FromInteger(0))
            {
                if (sInputKey_Run_5550E8 & sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed)
                {
                    field_124_return_to_previous_motion = 1;
                    mPreviousMotion = eSligMotions::M_Running_4_4B6000;
                    mBaseAliveGameObjectLastAnimFrame = (field_20_animation.field_92_current_frame - 5) != 0 ? 13 : 5;
                    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
                    {
                        field_128_input = 0;
                        field_C4_velx = -(ScaleToGridSize(field_CC_sprite_scale) / FP_FromInteger(4));
                        return;
                    }
                    field_C4_velx = ScaleToGridSize(field_CC_sprite_scale) / FP_FromInteger(4);
                }
                field_128_input = 0;
            }
        }
        else if (field_20_animation.field_92_current_frame == 11)
        {
            FP gridSize = {};
            if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
            {
                gridSize = -ScaleToGridSize(field_CC_sprite_scale);
            }
            else
            {
                gridSize = ScaleToGridSize(field_CC_sprite_scale);
            }

            if (WallHit(field_CC_sprite_scale * FP_FromInteger(45), gridSize * FP_FromDouble(2.5)))
            {
                mCurrentMotion = eSligMotions::M_WalkToStand_18_4B5FC0;
            }
            else if (sControlledCharacter_5C1B8C != this || mHealth <= FP_FromInteger(0))
            {
                if (mNextMotion == eSligMotions::M_Shoot_6_4B55A0)
                {
                    mCurrentMotion = eSligMotions::M_WalkToStand_18_4B5FC0;
                    field_12C_timer = sGnFrame + 60;
                }
                else if (mNextMotion != -1)
                {
                    mCurrentMotion = eSligMotions::M_WalkToStand_18_4B5FC0;
                }
            }
            else
            {
                PlayerControlStopWalkingIfRequired_4B8540();
            }
        }
        else
        {
            field_126_checked_if_off_screen = 0;
        }
    }
}

void Slig::M_StandToRun_3_4B62F0()
{
    field_128_input |= sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_C_held;
    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eSligMotions::M_Running_4_4B6000;
    }

    Event_Broadcast(kEventNoise, this);

    if (WallHit(field_CC_sprite_scale * FP_FromInteger(45), field_C4_velx))
    {
        FallKnockBackOrSmash_4B4A90();
    }
    else
    {
        MoveOnLine_4B4C40();
    }
}

void Slig::M_Running_4_4B6000()
{
    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        field_C4_velx = -(ScaleToGridSize(field_CC_sprite_scale) / FP_FromInteger(4));
    }
    else
    {
        field_C4_velx = (ScaleToGridSize(field_CC_sprite_scale) / FP_FromInteger(4));
    }

    if (gMap.GetDirection_4811A0(field_C2_lvl_number, field_C0_path_number, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos) >= CameraPos::eCamCurrent_0 && MusicController::static_GetMusicType(0, 0, 0) != MusicController::MusicTypes::ePossessed_9)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eSoftChase_8, this, 0, 0);
    }

    field_128_input |= sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_C_held;

    Event_Broadcast(kEventNoise, this);

    if (WallHit(field_CC_sprite_scale * FP_FromInteger(45), field_C4_velx))
    {
        FallKnockBackOrSmash_4B4A90();
    }
    else
    {
        MoveOnLine_4B4C40();

        if (mCurrentMotion == eSligMotions::M_Running_4_4B6000)
        {
            if (field_20_animation.field_92_current_frame == 4 || field_20_animation.field_92_current_frame == 12)
            {
                Slig_SoundEffect_4BFFE0(SligSfx::eRunningStep_3, this);

                if (field_126_checked_if_off_screen == 0)
                {
                    field_126_checked_if_off_screen = 1;
                    MapFollowMe(TRUE);
                }

                if (sControlledCharacter_5C1B8C == this && mHealth > FP_FromInteger(0))
                {
                    if (field_20_animation.field_92_current_frame == 4)
                    {
                        PlayerControlRunningSlideStopOrTurnFrame4_4B85D0();
                    }
                    else
                    {
                        PlayerControlRunningSlideStopOrTurnFrame12_4B8790();
                    }
                }
                else
                {
                    if (mNextMotion == eSligMotions::M_Walking_2_4B5BC0)
                    {
                        mCurrentMotion = eSligMotions::M_Walking_2_4B5BC0;
                        mNextMotion = -1;
                        if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
                        {
                            field_C4_velx = -(ScaleToGridSize(field_CC_sprite_scale) / FP_FromInteger(9));
                        }
                        else
                        {
                            field_C4_velx = (ScaleToGridSize(field_CC_sprite_scale) / FP_FromInteger(9));
                        }
                    }
                    else if (mNextMotion == eSligMotions::M_Shoot_6_4B55A0)
                    {
                        mCurrentMotion = eSligMotions::M_Shoot_6_4B55A0;
                        mNextMotion = -1;
                        field_12C_timer = sGnFrame + 60;
                    }
                    else if (mNextMotion != -1)
                    {
                        if (field_C4_velx >= FP_FromInteger(0))
                        {
                            field_C4_velx = (field_CC_sprite_scale * FP_FromDouble(13.2));
                        }
                        else
                        {
                            field_C4_velx = (field_CC_sprite_scale * FP_FromDouble(-13.2));
                        }

                        mCurrentMotion = eSligMotions::M_SlidingToStand_8_4B6520;
                    }
                }
            }
            else
            {
                field_126_checked_if_off_screen = 0;
            }
        }
    }
}

void Slig::M_TurnAroundStanding_5_4B6390()
{
    if (gMap.GetDirection_4811A0(
            field_C2_lvl_number,
            field_C0_path_number,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos)
            >= CameraPos::eCamCurrent_0
        && MusicController::static_GetMusicType(0, 0, 0) <= MusicController::MusicTypes::eTension_4)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eTension_4, this, 0, 0);
    }

    Slig_SoundEffect_4BFFE0(SligSfx::eStandingTurn_1, this);

    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_20_animation.mAnimFlags.Toggle(AnimFlags::eBit5_FlipX);
        ToStand_4B4A20();
    }
}

void Slig::M_Shoot_6_4B55A0()
{
    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (sControlledCharacter_5C1B8C == this && mHealth > FP_FromInteger(0))
        {
            if (sInputObject_5BD4E0.isPressed(sInputKey_ThrowItem_5550F4))
            {
                const FP k45Scaled = field_CC_sprite_scale * FP_FromInteger(45);
                const FP kGridSize = ScaleToGridSize(field_CC_sprite_scale);
                const FP k8 = FP_FromInteger(8);

                // Recoil right
                if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX) && sInputObject_5BD4E0.isPressed(sInputKey_Right_5550D0))
                {
                    PathLine* pLine = nullptr;
                    FP hitX = {};
                    FP hitY = {};
                    if (sCollisions_DArray_5C1128->Raycast(
                            mBaseAnimatedWithPhysicsGameObject_XPos,
                            mBaseAnimatedWithPhysicsGameObject_YPos,
                            mBaseAnimatedWithPhysicsGameObject_XPos + (k8 * (kGridSize / k8)),
                            mBaseAnimatedWithPhysicsGameObject_YPos,
                            &pLine,
                            &hitX,
                            &hitY,
                            field_D6_scale != 0 ? 6 : 96)
                        || sCollisions_DArray_5C1128->Raycast(
                            mBaseAnimatedWithPhysicsGameObject_XPos,
                            mBaseAnimatedWithPhysicsGameObject_YPos - k45Scaled,
                            mBaseAnimatedWithPhysicsGameObject_XPos + (k8 * (kGridSize / k8)),
                            mBaseAnimatedWithPhysicsGameObject_YPos - k45Scaled,
                            &pLine,
                            &hitX,
                            &hitY,
                            field_D6_scale != 0 ? 6 : 96))
                    {
                        return;
                    }
                    field_C4_velx = (kGridSize / k8);
                    mCurrentMotion = eSligMotions::M_Recoil_19_4B8270;
                    return;
                }

                // Recoil left
                if (!field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX) && sInputObject_5BD4E0.isPressed(sInputKey_Left_5550D4))
                {
                    PathLine* pLine = nullptr;
                    FP hitX = {};
                    FP hitY = {};
                    if (sCollisions_DArray_5C1128->Raycast(
                            mBaseAnimatedWithPhysicsGameObject_XPos,
                            mBaseAnimatedWithPhysicsGameObject_YPos,
                            mBaseAnimatedWithPhysicsGameObject_XPos - (k8 * (kGridSize / k8)),
                            mBaseAnimatedWithPhysicsGameObject_YPos,
                            &pLine,
                            &hitX,
                            &hitY,
                            field_D6_scale != 0 ? 6 : 0x60)
                        || sCollisions_DArray_5C1128->Raycast(
                            mBaseAnimatedWithPhysicsGameObject_XPos,
                            mBaseAnimatedWithPhysicsGameObject_YPos - k45Scaled,
                            mBaseAnimatedWithPhysicsGameObject_XPos - (k8 * (kGridSize / k8)),
                            mBaseAnimatedWithPhysicsGameObject_YPos - k45Scaled,
                            &pLine,
                            &hitX,
                            &hitY,
                            field_D6_scale != 0 ? 6 : 96))
                    {
                        return;
                    }
                    field_C4_velx = -(kGridSize / k8);
                    mCurrentMotion = eSligMotions::M_Recoil_19_4B8270;
                    return;
                }

                // General recoil
                if (!sInputObject_5BD4E0.isPressed(sInputKey_Down_5550DC) || field_CC_sprite_scale != FP_FromDouble(0.5))
                {
                    if (field_12C_timer > static_cast<s32>(sGnFrame))
                    {
                        return;
                    }

                    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
                    {
                        field_C4_velx = (kGridSize / k8);
                    }
                    else
                    {
                        field_C4_velx = -(kGridSize / k8);
                    }

                    PathLine* pLine = nullptr;
                    FP hitX = {};
                    FP hitY = {};
                    if (sCollisions_DArray_5C1128->Raycast(
                            mBaseAnimatedWithPhysicsGameObject_XPos,
                            mBaseAnimatedWithPhysicsGameObject_YPos,
                            mBaseAnimatedWithPhysicsGameObject_XPos + (k8 * field_C4_velx),
                            mBaseAnimatedWithPhysicsGameObject_YPos,
                            &pLine,
                            &hitX,
                            &hitY,
                            field_D6_scale != 0 ? 6 : 96)
                        || sCollisions_DArray_5C1128->Raycast(
                            mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos - k45Scaled,
                            mBaseAnimatedWithPhysicsGameObject_XPos + (k8 * field_C4_velx),
                            mBaseAnimatedWithPhysicsGameObject_YPos - k45Scaled,
                            &pLine,
                            &hitX,
                            &hitY,
                            field_D6_scale != 0 ? 6 : 96))
                    {
                        field_C4_velx = FP_FromInteger(0);
                    }
                    else
                    {
                        mCurrentMotion = eSligMotions::M_Recoil_19_4B8270;
                    }
                    return;
                }
            }
            else
            {
                SND_SEQ_PlaySeq(SeqId::HitBottomOfDeathPit_9, 1, 1);
                mCurrentMotion = eSligMotions::M_ShootToStand_13_4B5580;
                return;
            }
        }

        if (mNextMotion == eSligMotions::M_StandIdle_0_4B4EC0)
        {
            SND_SEQ_PlaySeq(SeqId::HitBottomOfDeathPit_9, 1, 1);
            mCurrentMotion = eSligMotions::M_ShootToStand_13_4B5580;
            mNextMotion = -1;
            return;
        }

        if (mNextMotion != -1 && mNextMotion != eSligMotions::M_Shoot_6_4B55A0)
        {
            SND_SEQ_PlaySeq(SeqId::HitBottomOfDeathPit_9, 1, 1);
            mCurrentMotion = eSligMotions::M_ShootToStand_13_4B5580;
            return;
        }
    }
}

void Slig::M_Falling_7_4B42D0()
{
    if (field_C4_velx <= FP_FromInteger(0))
    {
        if (field_C4_velx < FP_FromInteger(0))
        {
            field_C4_velx += (field_CC_sprite_scale * field_130_falling_velx_scale_factor);
            if (field_C4_velx < (ScaleToGridSize(field_CC_sprite_scale) / FP_FromInteger(4)))
            {
                field_C4_velx = -(ScaleToGridSize(field_CC_sprite_scale) / FP_FromInteger(4));
            }

            if (field_C4_velx > FP_FromInteger(0))
            {
                field_C4_velx = FP_FromInteger(0);
            }
        }
    }
    else
    {
        field_C4_velx -= (field_CC_sprite_scale * field_130_falling_velx_scale_factor);
        if (field_C4_velx > (ScaleToGridSize(field_CC_sprite_scale) / FP_FromInteger(4)))
        {
            field_C4_velx = (ScaleToGridSize(field_CC_sprite_scale) / FP_FromInteger(4));
        }

        if (field_C4_velx < FP_FromInteger(0))
        {
            field_C4_velx = FP_FromInteger(0);
        }
    }

    PathLine* ppPathLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    const auto bCollision = InAirCollision(&ppPathLine, &hitX, &hitY, FP_FromDouble(1.8));
    SetActiveCameraDelayedFromDir();
    if (bCollision)
    {
        switch (ppPathLine->field_8_type)
        {
            case eLineTypes::eFloor_0:
            case eLineTypes::eBackgroundFloor_4:
            case 32u:
            case 36u:
                BaseAliveGameObjectCollisionLine = ppPathLine;
                mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
                mBaseAnimatedWithPhysicsGameObject_XPos = hitX;
                MapFollowMe(TRUE);
                ToStand_4B4A20();
                PlatformCollide_4B4E00();
                mCurrentMotion = eSligMotions::M_LandingSoft_40_4B4530;
                break;
            case eLineTypes::eWallLeft_1:
            case eLineTypes::eWallRight_2:
            case eLineTypes::eBackgroundWallLeft_5:
            case eLineTypes::eBackgroundWallRight_6:
                mBaseAnimatedWithPhysicsGameObject_XPos = hitX;
                mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
                FallKnockBackOrSmash_4B4A90();
                break;
            default:
                return;
        }
    }
}

void Slig::M_SlidingToStand_8_4B6520()
{
    Event_Broadcast(kEventNoise, this);
    if (WallHit(field_CC_sprite_scale * FP_FromInteger(45), field_C4_velx))
    {
        FallKnockBackOrSmash_4B4A90();
    }
    else
    {
        SlowDown_4B6450(FP_FromDouble(2.125));
        if (mCurrentMotion == eSligMotions::M_SlidingToStand_8_4B6520)
        {
            if (field_20_animation.field_92_current_frame < 6 && sControlledCharacter_5C1B8C == this && mHealth > FP_FromInteger(0))
            {
                if ((field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX) && sInputObject_5BD4E0.isPressed(sInputKey_Right_5550D0)) || ((!(field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX)) && sInputObject_5BD4E0.isPressed(sInputKey_Left_5550D4))))
                {
                    mBaseAliveGameObjectLastAnimFrame = field_20_animation.field_92_current_frame;
                    mPreviousMotion = eSligMotions::M_SlidingTurn_9_4B6680;
                    field_124_return_to_previous_motion = 1;
                }
            }
            else if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                Environment_SFX_457A40(EnvironmentSfx::eSlideStop_0, 0, 0x7FFF, this);
                MapFollowMe(TRUE);
                MainMovement_4B4720();
            }
        }
    }
}

void Slig::M_SlidingTurn_9_4B6680()
{
    Event_Broadcast(kEventNoise, this);

    if (WallHit(field_CC_sprite_scale * FP_FromInteger(45), field_C4_velx))
    {
        FallKnockBackOrSmash_4B4A90();
    }
    else
    {
        SlowDown_4B6450(FP_FromDouble(2.125));

        if (mCurrentMotion == eSligMotions::M_SlidingTurn_9_4B6680)
        {
            if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                MapFollowMe(TRUE);

                if (sControlledCharacter_5C1B8C == this && mHealth > FP_FromInteger(0))
                {
                    if (sInputObject_5BD4E0.isPressed(sInputKey_Run_5550E8))
                    {
                        mNextMotion = eSligMotions::M_Running_4_4B6000;
                    }
                    else
                    {
                        mNextMotion = eSligMotions::M_Walking_2_4B5BC0;
                    }
                }

                if (mNextMotion == eSligMotions::M_Running_4_4B6000)
                {
                    field_C4_velx = -(ScaleToGridSize(field_CC_sprite_scale) / FP_FromInteger(4));
                    mCurrentMotion = eSligMotions::M_SlidingTurnToRun_11_4B6850;
                }
                else
                {
                    field_C4_velx = -(ScaleToGridSize(field_CC_sprite_scale) / FP_FromInteger(9));
                    mCurrentMotion = eSligMotions::M_SlidingTurnToWalk_10_4B6800;
                }

                if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
                {
                    field_C4_velx = -field_C4_velx;
                }
            }
        }
    }
}

void Slig::M_SlidingTurnToWalk_10_4B6800()
{
    Event_Broadcast(kEventNoise, this);
    MoveOnLine_4B4C40();

    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_20_animation.mAnimFlags.Toggle(AnimFlags::eBit5_FlipX);
        mCurrentMotion = eSligMotions::M_Walking_2_4B5BC0;
    }
}

void Slig::M_SlidingTurnToRun_11_4B6850()
{
    Event_Broadcast(kEventNoise, this);
    MoveOnLine_4B4C40();

    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_20_animation.mAnimFlags.Toggle(AnimFlags::eBit5_FlipX);
        mCurrentMotion = eSligMotions::M_Running_4_4B6000;
    }
}

void Slig::M_ReloadGun_12_4B5530()
{
    if (field_20_animation.field_92_current_frame == 1)
    {
        Slig_SoundEffect_4BFFE0(SligSfx::eReload1_6, this);
    }
    else if (field_20_animation.field_92_current_frame == 4)
    {
        Slig_SoundEffect_4BFFE0(SligSfx::eReload2_7, this);
    }

    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToStand_4B4A20();
    }
}

void Slig::M_ShootToStand_13_4B5580()
{
    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        MainMovement_4B4720();
    }
}

void Slig::M_SteppingToStand_14_4B8480()
{
    if (field_20_animation.field_92_current_frame == 0)
    {
        Slig_SoundEffect_4BFFE0(SligSfx::eWalkingStep_2, this);
    }

    if (WallHit(field_CC_sprite_scale * FP_FromInteger(45), field_C4_velx))
    {
        FallKnockBackOrSmash_4B4A90();
    }
    else
    {
        MoveOnLine_4B4C40();
        if (mCurrentMotion == eSligMotions::M_SteppingToStand_14_4B8480)
        {
            if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                MapFollowMe(TRUE);
                ToStand_4B4A20();
            }
        }
    }
}

void Slig::M_StandingToStep_15_4B83B0()
{
    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (sInputObject_5BD4E0.isPressed(sInputKey_Left_5550D4 | sInputKey_Right_5550D0))
        {
            mCurrentMotion = eSligMotions::M_StandToWalk_1_4B5F70;
        }
        else if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
        {
            mCurrentMotion = eSligMotions::M_SteppingToStand_14_4B8480;
            field_C4_velx = -(ScaleToGridSize(field_CC_sprite_scale) / FP_FromInteger(6));
        }
        else
        {
            field_C4_velx = (ScaleToGridSize(field_CC_sprite_scale) / FP_FromInteger(6));
            mCurrentMotion = eSligMotions::M_SteppingToStand_14_4B8480;
        }
    }
}

void Slig::M_DepossessingAbort_16_4B8250()
{
    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToStand_4B4A20();
    }
}

void Slig::M_GameSpeak_17_4B5290()
{
    field_128_input |= sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_C_held;
    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = GetNextMotionIncGameSpeak_4B5080(field_128_input);
        if (mCurrentMotion == -1)
        {
            ToStand_4B4A20();
            field_128_input = 0;
        }
        else
        {
            if (!BrainIs(&Slig::Brain_ListeningToGlukkon_4_4B9D20))
            {
                Event_Broadcast(kEventSpeaking, this);
            }
            field_128_input = 0;
        }
    }
}

void Slig::M_WalkToStand_18_4B5FC0()
{
    MoveOnLine_4B4C40();

    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        MapFollowMe(TRUE);
        if (!Slig::MainMovement_4B4720())
        {
            ToStand_4B4A20();
        }
    }
}

void Slig::M_Recoil_19_4B8270()
{
    MoveOnLine_4B4C40();

    if (mCurrentMotion == eSligMotions::M_Recoil_19_4B8270)
    {
        if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            MapFollowMe(TRUE);
            if (sControlledCharacter_5C1B8C == this && mHealth > FP_FromInteger(0))
            {
                if (sInputObject_5BD4E0.isPressed(sInputKey_ThrowItem_5550F4))
                {
                    field_C4_velx = FP_FromInteger(0);
                    mPreviousMotion = eSligMotions::M_Shoot_6_4B55A0;
                    field_12C_timer = sGnFrame + 60;
                    mBaseAliveGameObjectLastAnimFrame = 2;
                    field_124_return_to_previous_motion = 1;
                }
                else
                {
                    SND_SEQ_PlaySeq(SeqId::HitBottomOfDeathPit_9, 1, 1);
                    mCurrentMotion = eSligMotions::M_ShootToStand_13_4B5580;
                }
            }
            else if (mNextMotion != -1)
            {
                SND_SEQ_PlaySeq(SeqId::HitBottomOfDeathPit_9, 1, 1);
                mCurrentMotion = eSligMotions::M_ShootToStand_13_4B5580;
                mNextMotion = -1;
            }
        }
    }
    else if (field_CC_sprite_scale == FP_FromDouble(0.5))
    {
        SFX_Play_Mono(SoundEffect::SligShoot_5, 85);
    }
    else
    {
        SFX_Play_Mono(SoundEffect::SligShoot_5, 0);
    }
}

void Slig::M_SpeakHereBoy_20_4B5330()
{
    field_128_input |= sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_C_held;
    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = GetNextMotionIncGameSpeak_4B5080(field_128_input);
        if (mCurrentMotion == -1)
        {
            ToStand_4B4A20();
            field_128_input = 0;
        }
        else
        {
            if (!BrainIs(&Slig::Brain_ListeningToGlukkon_4_4B9D20))
            {
                Event_Broadcast(kEventSpeaking, this);
            }
            field_128_input = 0;
        }
    }
}

void Slig::M_SpeakHi_21_4B53D0()
{
    M_SpeakHereBoy_20_4B5330();
}

void Slig::M_SpeakFreeze_22_4B53F0()
{
    M_SpeakHereBoy_20_4B5330();
}

void Slig::M_SpeakGetHim_23_4B5410()
{
    M_SpeakHereBoy_20_4B5330();
}

void Slig::M_SpeakLaugh_24_4B5430()
{
    M_SpeakHereBoy_20_4B5330();
}

void Slig::M_SpeakBullshit1_25_4B5450()
{
    M_SpeakHereBoy_20_4B5330();
}

void Slig::M_SpeakLookOut_26_4B5470()
{
    M_SpeakHereBoy_20_4B5330();
}

void Slig::M_SpeakBullshit2_27_4B5490()
{
    M_SpeakHereBoy_20_4B5330();
}

void Slig::M_SpeakPanic_28_4B54B0()
{
    M_SpeakHereBoy_20_4B5330();
}

void Slig::M_SpeakWhat_29_4B54D0()
{
    M_SpeakHereBoy_20_4B5330();
}

void Slig::M_SpeakAIFreeze_30_4B54F0()
{
    M_SpeakHereBoy_20_4B5330();
}

void Slig::M_Blurgh_31_4B5510()
{
    M_SpeakHereBoy_20_4B5330();
}

void Slig::M_Sleeping_32_4B89A0()
{
    if (mNextMotion == eSligMotions::M_SleepingToStand_33_4B8C50)
    {
        // Out of sleep
        mCurrentMotion = eSligMotions::M_SleepingToStand_33_4B8C50;
        mNextMotion = -1;
    }
    else if (static_cast<s32>(sGnFrame % 60))
    {
        if (!(static_cast<s32>(sGnFrame - 20) % 60))
        {
            Slig_SoundEffect_4BFFE0(SligSfx::eSnooze1_5, this);
            if (gMap.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0))
            {
                FP xOff = {};
                if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
                {
                    xOff = -(field_CC_sprite_scale * FP_FromInteger(-20));
                }
                else
                {
                    xOff = (field_CC_sprite_scale * FP_FromInteger(-20));
                }
                ae_new<SnoozeParticle>(
                    xOff + mBaseAnimatedWithPhysicsGameObject_XPos,
                    mBaseAnimatedWithPhysicsGameObject_YPos + (field_CC_sprite_scale * FP_FromInteger(-10)),
                    field_20_animation.mRenderLayer,
                    field_20_animation.field_14_scale);
            }
        }
    }
    else
    {
        Slig_SoundEffect_4BFFE0(SligSfx::eSnooze2_4, this);

        if (gMap.Is_Point_In_Current_Camera_4810D0(
                field_C2_lvl_number,
                field_C0_path_number,
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos,
                0))
        {
            FP xOff = {};
            if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
            {
                xOff = -(field_CC_sprite_scale * FP_FromInteger(-20));
            }
            else
            {
                xOff = (field_CC_sprite_scale * FP_FromInteger(-20));
            }

            ae_new<SnoozeParticle>(
                xOff + mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos + (field_CC_sprite_scale * FP_FromInteger(-10)),
                field_20_animation.mRenderLayer,
                field_20_animation.field_14_scale);
        }
    }
}

void Slig::M_SleepingToStand_33_4B8C50()
{
    // OWI hack - kill all particles, even if they're not ours!
    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        auto pObj = gBaseGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->Type() == AETypes::eSnoozeParticle_124)
        {
            static_cast<SnoozeParticle*>(pObj)->field_1E4_state = SnoozeParticle::SnoozeParticleState::eBlowingUp_2;
        }
    }

    if (field_20_animation.field_92_current_frame >= 2 && field_20_animation.field_92_current_frame <= 10)
    {
        Slig_SoundEffect_4BFFE0(SligSfx::eToStand_0, this);
    }

    if (field_20_animation.field_92_current_frame == 9)
    {
        Slig_SoundEffect_4BFFE0(SligSfx::eWalkingStep_2, this);
    }

    switch (field_20_animation.field_92_current_frame)
    {
        case 9:
            if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
            {
                mBaseAnimatedWithPhysicsGameObject_XPos -= (field_CC_sprite_scale * FP_FromInteger(7));
                return;
            }
            else
            {
                mBaseAnimatedWithPhysicsGameObject_XPos += (field_CC_sprite_scale * FP_FromInteger(7));
                return;
            }
            break;

        case 10:
            if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
            {
                mBaseAnimatedWithPhysicsGameObject_XPos -= (field_CC_sprite_scale * FP_FromInteger(12));
                return;
            }
            else
            {
                mBaseAnimatedWithPhysicsGameObject_XPos += (field_CC_sprite_scale * FP_FromInteger(12));
                return;
            }
            break;

        case 11:
            if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
            {
                mBaseAnimatedWithPhysicsGameObject_XPos -= (field_CC_sprite_scale * FP_FromInteger(6));
                return;
            }
            else
            {
                mBaseAnimatedWithPhysicsGameObject_XPos += (field_CC_sprite_scale * FP_FromInteger(6));
                return;
            }
            break;

        default:
            if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                MusicController::static_PlayMusic(MusicController::MusicTypes::eTension_4, this, 0, 0);
                MainMovement_4B4720();
            }
            break;
    }
}

void Slig::M_Knockback_34_4B68A0()
{
    if (BaseAliveGameObjectCollisionLine)
    {
        SlowDown_4B6450(FP_FromDouble(0.39));
    }
    else
    {
        M_Falling_7_4B42D0();

        if (mCurrentMotion == eSligMotions::M_LandingSoft_40_4B4530)
        {
            mCurrentMotion = eSligMotions::M_Knockback_34_4B68A0;
            field_12C_timer = sGnFrame + 10;
            if (mBaseAnimatedWithPhysicsGameObject_YPos - BaseAliveGameObjectLastLineYPos > FP_FromInteger(180)
                && !sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
                    FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
                    FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
                    FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
                    FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
                    TlvTypes::SoftLanding_75))
            {
                mCurrentMotion = eSligMotions::M_LandingFatal_41_4B4680;
                field_12C_timer = sGnFrame + 30;
                mHealth = FP_FromInteger(0);
                Event_Broadcast(kEventMudokonComfort, this);
            }
        }
    }

    Event_Broadcast(kEventNoise, this);
    if (field_20_animation.field_92_current_frame == 4)
    {
        Environment_SFX_457A40(EnvironmentSfx::eHitGroundSoft_6, 80, -200, this);
    }

    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
    {
        if (BaseAliveGameObjectCollisionLine)
        {
            field_C4_velx = FP_FromInteger(0);

            if (mHealth > FP_FromInteger(0) && field_12C_timer <= static_cast<s32>(sGnFrame) && sActiveHero->mHealth > FP_FromInteger(0))
            {
                mCurrentMotion = eSligMotions::M_KnockbackToStand_35_4B6A30;
            }
        }
    }
}

void Slig::M_KnockbackToStand_35_4B6A30()
{
    Event_Broadcast(kEventNoise, this);

    if (field_20_animation.field_92_current_frame >= 2 && field_20_animation.field_92_current_frame <= 10)
    {
        Slig_SoundEffect_4BFFE0(SligSfx::eToStand_0, this);
    }

    if (field_20_animation.field_92_current_frame == 9)
    {
        Slig_SoundEffect_4BFFE0(SligSfx::eWalkingStep_2, this);
    }

    FP gridSize = {};
    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        gridSize = -ScaleToGridSize(field_CC_sprite_scale);
    }
    else
    {
        gridSize = ScaleToGridSize(field_CC_sprite_scale);
    }

    if (!WallHit(field_CC_sprite_scale * FP_FromInteger(45), gridSize))
    {
        s32 xOff = 0;
        switch (field_20_animation.field_92_current_frame)
        {
            case 9:
                xOff = 3;
                break;
            case 10:
                xOff = 13;
                break;
            case 11:
                xOff = 9;
                break;
        }

        if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
        {
            xOff = -xOff;
        }

        mBaseAnimatedWithPhysicsGameObject_XPos += (field_CC_sprite_scale * field_CC_sprite_scale) * FP_FromInteger(xOff);
    }

    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToStand_4B4A20();
    }
}

void Slig::M_Depossessing_36_4B7F30()
{
    if (sControlledCharacter_5C1B8C == this)
    {
        if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            if (!Input_IsChanting_45F260())
            {
                mCurrentMotion = eSligMotions::M_DepossessingAbort_16_4B8250;
            }
        }

        if (!(static_cast<s32>(sGnFrame) % 4))
        {
            const FP xRand = FP_FromInteger(Math_RandomRange(-20, 20));
            const FP yRand = FP_FromInteger(Math_RandomRange(20, 50));
            New_TintChant_Particle(
                (field_CC_sprite_scale * xRand) + mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos - (field_CC_sprite_scale * yRand),
                field_CC_sprite_scale,
                Layer::eLayer_0);
        }

        if (static_cast<s32>(sGnFrame) > field_12C_timer)
        {
            BlowToGibs_4B8020();
        }
    }
}

void Slig::M_Possess_37_4B72C0()
{
    if (!BaseAliveGameObjectCollisionLine)
    {
        M_Falling_7_4B42D0();
        if (mCurrentMotion != eSligMotions::M_Possess_37_4B72C0)
        {
            mCurrentMotion = eSligMotions::M_Possess_37_4B72C0;
        }
    }

    if (static_cast<s32>(sGnFrame) >= field_150_explode_timer)
    {
        if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit3_Render))
        {
            FP xOff = (FP_FromInteger(20) * field_CC_sprite_scale);
            if (field_C4_velx < FP_FromInteger(0))
            {
                xOff = -xOff;
            }

            ae_new<Gibs>(
                GibType::Slig_1,
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos,
                xOff,
                FP_FromInteger(0),
                field_CC_sprite_scale,
                0);

            New_Smoke_Particles(
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos - (FP_FromInteger(30) * field_CC_sprite_scale),
                field_CC_sprite_scale,
                3,
                0x80u,
                0x80u,
                0x80u);

            if (field_CC_sprite_scale == FP_FromDouble(0.5))
            {
                SFX_Play_Mono(SoundEffect::KillEffect_64, 80);
                SFX_Play_Mono(SoundEffect::FallingItemHit_47, 60);
            }
            else
            {
                SFX_Play_Mono(SoundEffect::KillEffect_64, 127);
                SFX_Play_Mono(SoundEffect::FallingItemHit_47, 90);
            }

            field_20_animation.mAnimFlags.Clear(AnimFlags::eBit3_Render);
            mCurrentMotion = eSligMotions::M_StandIdle_0_4B4EC0;
            field_C8_vely = FP_FromInteger(0);
            field_C4_velx = FP_FromInteger(0);
            mHealth = FP_FromInteger(0);
            MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, this, 0, 0);
            SetUpdateDelay(40);
            field_120_timer = sGnFrame + 60;
            SetBrain(&Slig::Brain_Death_0_4BBFB0);
        }
    }

    if (static_cast<s32>(sGnFrame) > field_14C_death_by_being_shot_timer)
    {
        if (field_C4_velx >= FP_FromInteger(0))
        {
            if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
            {
                mCurrentMotion = eSligMotions::M_Knockback_34_4B68A0;
            }
        }
        else if (!field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
        {
            mCurrentMotion = eSligMotions::M_Knockback_34_4B68A0;
        }
        mCurrentMotion = eSligMotions::M_Smash_44_4B6B90;
    }
}

// When slig is "air walking" in a death fall
void Slig::M_OutToFall_38_4B4570()
{
    M_Falling_7_4B42D0();

    const FP fallDepth = mBaseAnimatedWithPhysicsGameObject_YPos - BaseAliveGameObjectLastLineYPos;
    if (fallDepth > FP_FromInteger(16 * 640))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (mCurrentMotion == eSligMotions::M_LandingSoft_40_4B4530 && fallDepth > FP_FromInteger(180) && !sPath_dword_BB47C0->TLV_Get_At_4DB4B0(FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos), FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos), FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos), FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos), TlvTypes::SoftLanding_75))
    {
        mCurrentMotion = eSligMotions::M_LandingFatal_41_4B4680;
        field_12C_timer = sGnFrame + 30;
        mHealth = FP_FromInteger(0);
        Event_Broadcast(kEventMudokonComfort, this);
    }
}

void Slig::M_FallingInitiate_39_4B4640()
{
    Event_Broadcast(kEventNoise, this);

    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eSligMotions::M_OutToFall_38_4B4570;
    }

    M_Falling_7_4B42D0();
}

void Slig::M_LandingSoft_40_4B4530()
{
    if (field_20_animation.field_92_current_frame == 0)
    {
        Environment_SFX_457A40(EnvironmentSfx::eHitGroundSoft_6, 0, 0x7FFF, 0);
    }

    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToStand_4B4A20();
    }
}

void Slig::M_LandingFatal_41_4B4680()
{
    if (field_20_animation.field_92_current_frame == 0)
    {
        SND_SEQ_Play(SeqId::HitBottomOfDeathPit_9, 1, 65, 65);
        SFX_Play_Mono(SoundEffect::KillEffect_64, 80);
    }

    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (static_cast<s32>(sGnFrame) >= field_12C_timer && !BrainIs(&Slig::Brain_Death_0_4BBFB0))
        {
            mHealth = FP_FromInteger(0);
            SetBrain(&Slig::Brain_Death_0_4BBFB0);
            field_120_timer = sGnFrame + 60;
        }
    }
}

void Slig::M_ShootZ_42_4B7560()
{
    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        // Controlled by player and no longer pressing down or shoot
        if (sControlledCharacter_5C1B8C == this && (!sInputObject_5BD4E0.isPressed(sInputKey_ThrowItem_5550F4) || !sInputObject_5BD4E0.isPressed(sInputKey_Down_5550DC)))
        {
            SND_SEQ_PlaySeq(SeqId::HitBottomOfDeathPit_9, 1, 1);
            mCurrentMotion = eSligMotions::M_ShootZtoStand_43_4B77E0;
        }
        // Controlled by AI and need to leave Z-shoot motion
        else if (mNextMotion != -1)
        {
            SND_SEQ_PlaySeq(SeqId::HitBottomOfDeathPit_9, 1, 1);
            mNextMotion = -1;
            mCurrentMotion = eSligMotions::M_ShootZtoStand_43_4B77E0;
        }
    }
    else if (field_20_animation.field_92_current_frame == 7)
    {
        BulletType bulletType = BulletType::eSligPossessedOrUnderGlukkonCommand_0;
        if (sControlledCharacter_5C1B8C == this)
        {
            bulletType = BulletType::ePossessedSligZBullet_1;
        }
        else
        {
            bulletType = BulletType::eZBullet_3;
        }

        ae_new<Bullet>(
            this,
            bulletType,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(12),
            FP_FromInteger(640),
            0,
            field_CC_sprite_scale,
            field_218_tlv_data.field_22_num_times_to_shoot - field_158_num_times_to_shoot - 1);

        New_ShootingZFire_Particle(mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(12), field_CC_sprite_scale);

        if (field_CC_sprite_scale == FP_FromDouble(0.5))
        {
            SFX_Play_Mono(SoundEffect::SligShoot_5, 85);
        }
        else
        {
            SFX_Play_Mono(SoundEffect::SligShoot_5, 0);
        }

        // The doves don't like bullets
        Dove::All_FlyAway(0);

        Event_Broadcast(kEventShooting, this);
        Event_Broadcast(kEventLoudNoise, this);
    }
}

void Slig::M_ShootZtoStand_43_4B77E0()
{
    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToStand_4B4A20();
    }
}

void Slig::M_Smash_44_4B6B90()
{
    if (BaseAliveGameObjectCollisionLine)
    {
        if (field_20_animation.field_92_current_frame == 4)
        {
            Environment_SFX_457A40(EnvironmentSfx::eHitGroundSoft_6, 80, -200, this);
        }
    }
    else
    {
        M_Falling_7_4B42D0();

        if (mCurrentMotion != eSligMotions::M_Smash_44_4B6B90)
        {
            mCurrentMotion = eSligMotions::M_Smash_44_4B6B90;
        }
    }
}

void Slig::M_PullLever_45_4B8950()
{
    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit10_Teleporting))
        {
            Slig_GameSpeak_SFX_4C04F0(SligSpeak::eBlurgh_11, 0, field_11E_pitch_min, this);
            mCurrentMotion = eSligMotions::M_Blurgh_31_4B5510;
        }
        else
        {
            ToStand_4B4A20();
        }
    }
}

void Slig::M_LiftGrip_46_4B3700()
{
    auto pLiftPoint = static_cast<LiftPoint*>(sObjectIds.Find_Impl(BaseAliveGameObjectId));
    if (pLiftPoint)
    {
        pLiftPoint->vMove_4626A0(FP_FromInteger(0), FP_FromInteger(0), 0);
        field_C8_vely = FP_FromInteger(0);
        if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            if (sControlledCharacter_5C1B8C == this)
            {
                if (sInputObject_5BD4E0.isPressed(sInputKey_Up_5550D8))
                {
                    if (pLiftPoint->vOnTopFloor())
                    {
                        mCurrentMotion = eSligMotions::M_LiftGripping_48_4B3850;
                    }
                    else
                    {
                        mCurrentMotion = eSligMotions::M_LiftUp_49_4B3930;
                    }
                    return;
                }

                if (sInputObject_5BD4E0.isPressed(sInputKey_Down_5550DC))
                {
                    if (pLiftPoint->vOnBottomFloor())
                    {
                        mCurrentMotion = eSligMotions::M_LiftGripping_48_4B3850;
                    }
                    else
                    {
                        mCurrentMotion = eSligMotions::M_LiftDown_50_4B3960;
                    }
                    return;
                }

                mCurrentMotion = eSligMotions::M_LiftGripping_48_4B3850;
            }
            else
            {
                if (pLiftPoint->vOnTopFloor())
                {
                    mCurrentMotion = eSligMotions::M_LiftDown_50_4B3960;
                }

                if (pLiftPoint->vOnBottomFloor())
                {
                    mCurrentMotion = eSligMotions::M_LiftUp_49_4B3930;
                }
            }
        }
    }
}

void Slig::M_LiftUngrip_47_4B3820()
{
    field_C8_vely = FP_FromInteger(0);

    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToStand_4B4A20();
    }
}

void Slig::M_LiftGripping_48_4B3850()
{
    auto pLiftPoint = static_cast<LiftPoint*>(sObjectIds.Find_Impl(BaseAliveGameObjectId));
    if (pLiftPoint)
    {
        CheckPlatformVanished_4B3640();

        pLiftPoint->vMove_4626A0(FP_FromInteger(0), FP_FromInteger(0), 0);
        field_C8_vely = FP_FromInteger(0);

        if (sInputObject_5BD4E0.isPressed(sInputKey_Up_5550D8))
        {
            if (!pLiftPoint->vOnTopFloor())
            {
                mCurrentMotion = eSligMotions::M_LiftUp_49_4B3930;
            }
        }
        else if (sInputObject_5BD4E0.isPressed(sInputKey_Down_5550DC))
        {
            if (!pLiftPoint->vOnBottomFloor())
            {
                mCurrentMotion = eSligMotions::M_LiftDown_50_4B3960;
            }
        }
        else if (pLiftPoint->vOnAnyFloor())
        {
            mCurrentMotion = eSligMotions::M_LiftUngrip_47_4B3820;
        }
    }
}

void Slig::M_LiftUp_49_4B3930()
{
    mCurrentMotion = MoveLift_4B3990(FP_FromInteger(-4));
}

void Slig::M_LiftDown_50_4B3960()
{
    mCurrentMotion = MoveLift_4B3990(FP_FromInteger(4));
}


inline PSX_RECT MakeRectFromFP(FP x, FP y, FP w, FP h)
{
    PSX_RECT r = {};
    r.x = FP_GetExponent(x);
    r.w = FP_GetExponent(w);
    r.y = FP_GetExponent(y);
    r.h = FP_GetExponent(h);
    return r;
}

inline PSX_RECT MakeMinMaxRect(FP x, FP y, FP w, FP h, bool flipToMaxMin = false)
{
    if (flipToMaxMin)
    {
        return MakeRectFromFP(
            std::max(x, w),
            std::max(y, h),
            std::min(x, w),
            std::min(y, h));
    }
    else
    {
        return MakeRectFromFP(
            std::min(x, w),
            std::min(y, h),
            std::max(x, w),
            std::max(y, h));
    }
}

void Slig::M_Beat_51_4B6C00()
{
    if (field_20_animation.field_92_current_frame == 5)
    {
        SFX_Play_Pitch(SoundEffect::AirStream_23, 90, -300);
    }

    if (field_20_animation.field_92_current_frame == 8)
    {
        const FP kGridSize = ScaleToGridSize(field_CC_sprite_scale);
        const FP k2Scaled = FP_FromInteger(2) * kGridSize;

        PSX_RECT hitRect = {};
        hitRect = MakeMinMaxRect(
            field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX) ? mBaseAnimatedWithPhysicsGameObject_XPos - kGridSize : mBaseAnimatedWithPhysicsGameObject_XPos + kGridSize,
            mBaseAnimatedWithPhysicsGameObject_YPos,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos - k2Scaled);

        for (s32 i = 0; i < gBaseAliveGameObjects_5C1B7C->Size(); i++)
        {
            BaseAliveGameObject* pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(i);
            if (!pObj)
            {
                break;
            }

            if (pObj != this)
            {
                if (pObj->Type() == AETypes::eMudokon_110 || pObj->Type() == AETypes::eCrawlingSlig_26)
                {
                    PSX_RECT bRect = {};
                    pObj->VGetBoundingRect(&bRect, 1);

                    if (pObj->mHealth > FP_FromInteger(0) && pObj->field_D6_scale == field_D6_scale && PSX_Rects_overlap_no_adjustment(&hitRect, &bRect))
                    {
                        pObj->VTakeDamage(this);
                        Event_Broadcast(kEventNoise, this);
                        SFX_Play_Mono(SoundEffect::FallingItemHit_47, 60);
                        return;
                    }
                }
            }
        }
    }

    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eSligMotions::M_StandIdle_0_4B4EC0;
    }
}

enum Brain_2_Possessed
{
    eBrain2_StartPossession_0 = 0,
    eBrain2_PossessionShaking_1 = 1,
    eBrain2_PossessionInactive_2 = 2,
    eBrain2_PossessionSpeak_3 = 3,
    eBrain2_ControlledByPlayer_4 = 4
};

enum Brain_35_ChaseAndDisappear
{
    eBrain35_Summoned_0 = 0,
    eBrain35_Running_1 = 1,
    eBrain35_ReachedDestination_2 = 2
};

s16 Slig::Brain_Death_0_4BBFB0()
{
    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit3_Render) && field_120_timer < (static_cast<s32>(sGnFrame) + 30))
    {
        field_CC_sprite_scale -= FP_FromDouble(0.01);
        field_D0_r -= 2;
        field_D2_g -= 2;
        field_D4_b -= 2;

        if (field_CC_sprite_scale >= FP_FromDouble(0.3))
        {
            DeathSmokeEffect(true);
        }
    }
    else if (!field_20_animation.mAnimFlags.Get(AnimFlags::eBit3_Render))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (sControlledCharacter_5C1B8C == this)
    {
        if (field_120_timer < static_cast<s32>(sGnFrame))
        {
            sControlledCharacter_5C1B8C = sActiveHero;
            MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, this, 0, 0);
            gMap.SetActiveCam(field_146_level, field_148_path, field_14A_camera, CameraSwapEffects::eInstantChange_0, 0, 0);
        }
    }

    if (sControlledCharacter_5C1B8C != this)
    {
        if (!gMap.Is_Point_In_Current_Camera_4810D0(
                field_C2_lvl_number,
                field_C0_path_number,
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos,
                0))
        {
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        }
    }

    if (field_CC_sprite_scale < FP_FromInteger(0))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    return 116;
}

s16 Slig::Brain_ReturnControlToAbeAndDie_1_4BC410()
{
    if (sControlledCharacter_5C1B8C == this)
    {
        sControlledCharacter_5C1B8C = sActiveHero;
        MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, this, 0, 0);
        gMap.SetActiveCam(field_146_level, field_148_path, field_14A_camera, CameraSwapEffects::eInstantChange_0, 0, 0);
    }

    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    return 117;
}

s16 Slig::Brain_Possessed_2_4BBCF0()
{
    switch (field_11C_brain_sub_state)
    {
        case Brain_2_Possessed::eBrain2_StartPossession_0:
            Slig_GameSpeak_SFX_4C04F0(SligSpeak::eHelp_10, 0, field_11E_pitch_min, this);
            field_11C_brain_sub_state = 1;
            mHealth = FP_FromInteger(0);
            mCurrentMotion = eSligMotions::M_Possess_37_4B72C0;
            field_120_timer = sGnFrame + 30;
            field_150_explode_timer = sGnFrame + 1000;
            if (field_14C_death_by_being_shot_timer == 0)
            {
                field_14C_death_by_being_shot_timer = sGnFrame + 1000;
            }
            return field_11C_brain_sub_state;

        case Brain_2_Possessed::eBrain2_PossessionShaking_1:
            if (static_cast<s32>(sGnFrame) >= field_120_timer)
            {
                field_11C_brain_sub_state = Brain_2_Possessed::eBrain2_PossessionInactive_2;
                field_120_timer = sGnFrame + 20;
                mCurrentMotion = eSligMotions::M_StandIdle_0_4B4EC0;
                return field_11C_brain_sub_state;
            }
            break;

        case Brain_2_Possessed::eBrain2_PossessionInactive_2:
            if (static_cast<s32>(sGnFrame) >= field_120_timer)
            {
                if (Math_NextRandom() & 1)
                {
                    field_120_timer = sGnFrame + 20;
                    mCurrentMotion = eSligMotions::M_SpeakHi_21_4B53D0;
                    Slig_GameSpeak_SFX_4C04F0(SligSpeak::eHi_0, 0, field_11E_pitch_min, this);
                }
                else
                {
                    field_120_timer = sGnFrame + 20;
                    mCurrentMotion = eSligMotions::M_SpeakLaugh_24_4B5430;
                    Slig_GameSpeak_SFX_4C04F0(SligSpeak::eLaugh_3, 0, field_11E_pitch_min, this);
                }
                field_11C_brain_sub_state = Brain_2_Possessed::eBrain2_PossessionSpeak_3;
                return field_11C_brain_sub_state;
            }
            break;

        case Brain_2_Possessed::eBrain2_PossessionSpeak_3:
            if (Event_Get(kEventDeathReset))
            {
                if (sControlledCharacter_5C1B8C != this)
                {
                    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                }
            }

            if (static_cast<s32>(sGnFrame) >= field_120_timer)
            {
                field_11C_brain_sub_state = Brain_2_Possessed::eBrain2_ControlledByPlayer_4;
                mHealth = FP_FromInteger(1);
                return field_11C_brain_sub_state;
            }
            break;

        case Brain_2_Possessed::eBrain2_ControlledByPlayer_4:
            if (Event_Get(kEventDeathReset))
            {
                if (sControlledCharacter_5C1B8C != this)
                {
                    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                }
            }
            if (mHealth <= FP_FromInteger(0))
            {
                SetBrain(&Slig::Brain_Death_0_4BBFB0);
                field_120_timer = sGnFrame + 60;
                return field_11C_brain_sub_state;
            }
            break;

        default:
            return field_11C_brain_sub_state;
    }

    if (static_cast<s32>(sGnFrame) < field_120_timer)
    {
        if (mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit1_bShot))
        {
            BlowToGibs_4B8020();
        }
    }

    return field_11C_brain_sub_state;
}

enum Brain_3_DeathDropDeath
{
    eBrain3_SayHelpOnce_0 = 0,
    eBrain3_SayHelpAndDie_1 = 1,
    eBrain3_SwitchCamToAbe_2 = 2
};

s16 Slig::Brain_DeathDropDeath_3_4BC1E0()
{
    switch (field_11C_brain_sub_state)
    {
        case Brain_3_DeathDropDeath::eBrain3_SayHelpOnce_0:
            Slig_GameSpeak_SFX_4C04F0(SligSpeak::eHelp_10, 0, field_11E_pitch_min, this);
            field_120_timer = sGnFrame + 60;
            return Brain_3_DeathDropDeath::eBrain3_SayHelpAndDie_1;

        case Brain_3_DeathDropDeath::eBrain3_SayHelpAndDie_1:
        {
            if (static_cast<s32>(sGnFrame) < field_120_timer)
            {
                if (!((field_120_timer - sGnFrame) % 15))
                {
                    Slig_GameSpeak_SFX_4C04F0(
                        SligSpeak::eHelp_10,
                        static_cast<s16>(2 * ((field_120_timer & 0xFFFF) - sGnFrame)),
                        field_11E_pitch_min,
                        this);
                }

                if (static_cast<s32>(sGnFrame) == field_120_timer - 6)
                {
                    SND_SEQ_Play(SeqId::HitBottomOfDeathPit_9, 1, 65, 65);
                }

                return field_11C_brain_sub_state;
            }

            Environment_SFX_457A40(EnvironmentSfx::eFallingDeathScreamHitGround_15, 0, 0x7FFF, this);

            ae_new<ScreenShake>(0, 0);

            field_120_timer = sGnFrame + 30;
            return Brain_3_DeathDropDeath::eBrain3_SwitchCamToAbe_2;
        }

        case Brain_3_DeathDropDeath::eBrain3_SwitchCamToAbe_2:
            if (static_cast<s32>(sGnFrame) > field_120_timer)
            {
                if (sControlledCharacter_5C1B8C == this)
                {
                    MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, this, 0, 0);
                    sControlledCharacter_5C1B8C = sActiveHero;
                    gMap.SetActiveCam(field_146_level, field_148_path, field_14A_camera, CameraSwapEffects::eInstantChange_0, 0, 0);
                }
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            }
            return field_11C_brain_sub_state;

        default:
            return field_11C_brain_sub_state;
    }
}

const s16 sGlukkonResponseTable_560768[8][6] = {
    {eSligMotions::M_StandIdle_0_4B4EC0, eSligMotions::M_SpeakHi_21_4B53D0, -1, 0, 0, 0},
    {eSligMotions::M_StandIdle_0_4B4EC0, eSligMotions::M_SpeakWhat_29_4B54D0, -1, 0, 0, 0},
    {eSligMotions::M_StandIdle_0_4B4EC0, eSligMotions::M_SpeakBullshit1_25_4B5450, -1, 0, 0, 0},
    {eSligMotions::M_StandIdle_0_4B4EC0, eSligMotions::M_SpeakBullshit2_27_4B5490, -1, 0, 0, 0},
    {eSligMotions::M_Walking_2_4B5BC0, -1, 0, 0, 0, 0},
    {eSligMotions::M_Running_4_4B6000, -1, 0, 0, 0, 0},
    {eSligMotions::M_StandIdle_0_4B4EC0, eSligMotions::M_SpeakLaugh_24_4B5430, -1, 0, 0, 0},
    {eSligMotions::M_StandIdle_0_4B4EC0, eSligMotions::M_ReloadGun_12_4B5530, eSligMotions::M_ReloadGun_12_4B5530, eSligMotions::M_ReloadGun_12_4B5530, eSligMotions::M_ReloadGun_12_4B5530, -1}};


enum Brain_ListeningToGlukkon_States
{
    GettingAttention_0 = 0,
    IdleListening_1 = 1,
    Moving_2 = 2,
    NextToLeverThroughComeHere_3 = 3,
    Speaking_4 = 4,
    StoppingNextToLever_5 = 5,
    PullingLever_6 = 6,
    StoppingOnLift_7 = 7,
    Shooting_8 = 8,
    LostAttention_9 = 9,
};

enum Brain_ListeningToGlukkon_GlukkonCommands
{
    HeyOrAllYa_0 = 0,
    What_1 = 1,
    Stay_2 = 2,
    Bs_3 = 3,
    Walk_4 = 4,
    Run_5 = 5,
    AfterShootOrLaugh_6 = 6,
    DoitGunReload_7 = 7,
};

s16 Slig::Brain_ListeningToGlukkon_4_4B9D20()
{
    auto pGlukkonObj = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(field_208_glukkon_obj_id));
    auto pPlatformObj = static_cast<LiftPoint*>(sObjectIds.Find_Impl(BaseAliveGameObjectId));

    field_216_flags.Clear(Flags_216::eBit4_HeardGlukkon);

    if (!pGlukkonObj || !pGlukkonObj->mBaseGameObjectFlags.Get(BaseGameObject::eDrawable_Bit4))
    {
        PauseALittle_4BDD00();
        mNextMotion = eSligMotions::M_SpeakLaugh_24_4B5430;
        field_216_flags.Clear(Flags_216::eBit1_FollowGlukkon);
        --sSligsUnderControlCount_BAF7E8;
        return Brain_ListeningToGlukkon_States::GettingAttention_0;
    }

    switch (field_11C_brain_sub_state)
    {
        case Brain_ListeningToGlukkon_States::GettingAttention_0:
            return Brain_ListenToGlukkon_GettingAttention(pGlukkonObj);
        case Brain_ListeningToGlukkon_States::IdleListening_1:
            return Brain_ListenToGlukkon_IdleListen(pGlukkonObj, pPlatformObj);
        case Brain_ListeningToGlukkon_States::Moving_2:
            return Brain_ListenToGlukkon_Moving(pGlukkonObj);
        case Brain_ListeningToGlukkon_States::NextToLeverThroughComeHere_3:
            return Brain_ListenToGlukkon_NextLever(pGlukkonObj);
        case Brain_ListeningToGlukkon_States::Speaking_4:
            return Brain_ListenToGlukkon_Speaking();
        case Brain_ListeningToGlukkon_States::StoppingNextToLever_5:
            return Brain_ListenToGlukkon_StoppingNextToLever();
        case Brain_ListeningToGlukkon_States::PullingLever_6:
            return Brain_ListeningToGlukkon_PullingLever();
        case Brain_ListeningToGlukkon_States::StoppingOnLift_7:
            return Brain_ListeningToGlukkon_StoppingOnLift(pPlatformObj);
        case Brain_ListeningToGlukkon_States::Shooting_8:
            return Brain_ListeningToGlukkon_Shooting();
        case Brain_ListeningToGlukkon_States::LostAttention_9:
            return Brain_ListeningToGlukkon_LostAttention();
        default:
            field_11C_brain_sub_state = Brain_ListeningToGlukkon_States::GettingAttention_0;
            return field_11C_brain_sub_state;
    }
}

s16 Slig::Brain_ListeningToGlukkon_LostAttention()
{
    SetBrain(&Slig::Brain_Paused_33_4B8DD0);
    return field_11C_brain_sub_state;
}

s16 Slig::Brain_ListeningToGlukkon_Shooting()
{
    LastGlukkonSpeak_4B3090();
    if (static_cast<s32>(sGnFrame) == field_120_timer - 15)
    {
        mNextMotion = eSligMotions::M_Shoot_6_4B55A0;
    }

    if (static_cast<s32>(sGnFrame) <= field_120_timer)
    {
        return field_11C_brain_sub_state;
    }
    NextCommand_4B9A00(Brain_ListeningToGlukkon_GlukkonCommands::AfterShootOrLaugh_6, Brain_ListeningToGlukkon_States::IdleListening_1);
    return Brain_ListeningToGlukkon_States::Speaking_4;
}

s16 Slig::Brain_ListeningToGlukkon_StoppingOnLift(LiftPoint* pPlatformObj)
{
    const FP gridSize = ScaleToGridSize(field_CC_sprite_scale);

    if (mCurrentMotion == eSligMotions::M_Knockback_34_4B68A0)
    {
        mNextMotion = eSligMotions::M_StandIdle_0_4B4EC0;
        return Brain_ListeningToGlukkon_States::IdleListening_1;
    }

    if (mCurrentMotion != eSligMotions::M_StandIdle_0_4B4EC0)
    {
        return field_11C_brain_sub_state;
    }

    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        if (!pPlatformObj)
        {
            return Brain_ListeningToGlukkon_States::IdleListening_1;
        }

        if (pPlatformObj->Type() != AETypes::eLiftPoint_78)
        {
            return Brain_ListeningToGlukkon_States::IdleListening_1;
        }

        const FP gridSizeHalved = (gridSize / FP_FromInteger(2));
        const FP v84 = mBaseAnimatedWithPhysicsGameObject_XPos - gridSize - FP_FromInteger((BaseAliveGameObjectCollisionLine->field_0_rect.x + BaseAliveGameObjectCollisionLine->field_0_rect.w) / 2) >= FP_FromInteger(0) ? mBaseAnimatedWithPhysicsGameObject_XPos - gridSize - FP_FromInteger((BaseAliveGameObjectCollisionLine->field_0_rect.x + BaseAliveGameObjectCollisionLine->field_0_rect.w) / 2) : FP_FromInteger((BaseAliveGameObjectCollisionLine->field_0_rect.x + BaseAliveGameObjectCollisionLine->field_0_rect.w) / 2) - (mBaseAnimatedWithPhysicsGameObject_XPos - gridSize);
        if (v84 >= gridSizeHalved)
        {
            return Brain_ListeningToGlukkon_States::IdleListening_1;
        }
        mNextMotion = eSligMotions::M_SteppingToStand_14_4B8480;
        field_C4_velx = -(gridSize / FP_FromInteger(6));
        return field_11C_brain_sub_state;
    }

    if (!pPlatformObj)
    {
        return Brain_ListeningToGlukkon_States::IdleListening_1;
    }

    if (pPlatformObj->Type() != AETypes::eLiftPoint_78)
    {
        return Brain_ListeningToGlukkon_States::IdleListening_1;
    }

    const FP v90 = mBaseAnimatedWithPhysicsGameObject_XPos + gridSize - FP_FromInteger((BaseAliveGameObjectCollisionLine->field_0_rect.x + BaseAliveGameObjectCollisionLine->field_0_rect.w) / 2) >= FP_FromInteger(0) ? mBaseAnimatedWithPhysicsGameObject_XPos + gridSize
                                                                                                                                                                                   - FP_FromInteger((BaseAliveGameObjectCollisionLine->field_0_rect.x + BaseAliveGameObjectCollisionLine->field_0_rect.w) / 2)
                                                                                                                                                                             : FP_FromInteger((BaseAliveGameObjectCollisionLine->field_0_rect.x + BaseAliveGameObjectCollisionLine->field_0_rect.w) / 2)
                                                                                                                                                                                   - (mBaseAnimatedWithPhysicsGameObject_XPos + gridSize);
    if (v90 >= gridSize / FP_FromInteger(2))
    {
        return Brain_ListeningToGlukkon_States::IdleListening_1;
    }
    field_C4_velx = (gridSize / FP_FromInteger(6));
    mNextMotion = eSligMotions::M_SteppingToStand_14_4B8480;
    return field_11C_brain_sub_state;
}

s16 Slig::Brain_ListeningToGlukkon_PullingLever()
{
    const FP gridSize = ScaleToGridSize(field_CC_sprite_scale);

    Lever* pLever = nullptr;
    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        pLever = static_cast<Lever*>(FindObjectOfType(AETypes::eLever_139, mBaseAnimatedWithPhysicsGameObject_XPos - gridSize, mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5)));
    }
    else
    {
        pLever = static_cast<Lever*>(FindObjectOfType(AETypes::eLever_139, mBaseAnimatedWithPhysicsGameObject_XPos + gridSize, mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5)));
    }

    if (pLever)
    {
        if (mCurrentMotion == eSligMotions::M_PullLever_45_4B8950)
        {
            pLever->VPull(mBaseAnimatedWithPhysicsGameObject_XPos < pLever->mBaseAnimatedWithPhysicsGameObject_XPos);
            mNextMotion = eSligMotions::M_StandIdle_0_4B4EC0;
            return Brain_ListeningToGlukkon_States::IdleListening_1;
        }
        else
        {
            mNextMotion = eSligMotions::M_PullLever_45_4B8950;
            return field_11C_brain_sub_state;
        }
    }
    else
    {
        mNextMotion = eSligMotions::M_TurnAroundStanding_5_4B6390;
        return field_11C_brain_sub_state;
    }
}

s16 Slig::Brain_ListenToGlukkon_StoppingNextToLever()
{
    const FP gridSize = ScaleToGridSize(field_CC_sprite_scale);

    if (mCurrentMotion == eSligMotions::M_Knockback_34_4B68A0)
    {
        mNextMotion = eSligMotions::M_StandIdle_0_4B4EC0;
        return Brain_ListeningToGlukkon_States::IdleListening_1;
    }

    if (mCurrentMotion != eSligMotions::M_StandIdle_0_4B4EC0)
    {
        return field_11C_brain_sub_state;
    }

    if (!FindObjectOfType(AETypes::eLever_139, gridSize + mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5)))
    {
        if (!FindObjectOfType(AETypes::eLever_139, mBaseAnimatedWithPhysicsGameObject_XPos - gridSize, mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5)))
        {
            if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
            {
                mNextMotion = eSligMotions::M_SteppingToStand_14_4B8480;
                field_C4_velx = -(ScaleToGridSize(field_CC_sprite_scale) / FP_FromInteger(6));
                return field_11C_brain_sub_state;
            }
            field_C4_velx = (gridSize / FP_FromInteger(6));
            mNextMotion = eSligMotions::M_SteppingToStand_14_4B8480;
            return field_11C_brain_sub_state;
        }
    }

    if ((!FindObjectOfType(AETypes::eLever_139, gridSize + mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5)) || !(field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))) && (!FindObjectOfType(AETypes::eLever_139, mBaseAnimatedWithPhysicsGameObject_XPos - gridSize, mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5)) || field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX)))
    {
        return Brain_ListeningToGlukkon_States::IdleListening_1;
    }

    mNextMotion = eSligMotions::M_TurnAroundStanding_5_4B6390;
    return field_11C_brain_sub_state;
}

s16 Slig::Brain_ListenToGlukkon_Speaking()
{
    if (static_cast<s32>(sGnFrame) <= field_120_timer)
    {
        return field_11C_brain_sub_state;
    }

    if (mNextMotion == eSligMotions::M_TurnAroundStanding_5_4B6390)
    {
        return field_11C_brain_sub_state;
    }

    field_120_timer = sGnFrame + 8;

    mNextMotion = sGlukkonResponseTable_560768[field_212_next_command_arg1][field_214_cmd_idx];

    // keeps going till it hits -1
    field_214_cmd_idx++;

    if (sGlukkonResponseTable_560768[field_212_next_command_arg1][field_214_cmd_idx] != -1)
    {
        return field_11C_brain_sub_state;
    }

    return field_20C_state_after_speak;
}

s16 Slig::Brain_ListenToGlukkon_NextLever(BaseAliveGameObject* pGlukkonObj)
{
    if (!VIsFacingMe(pGlukkonObj))
    {
        mNextMotion = eSligMotions::M_TurnAroundStanding_5_4B6390;
        return field_11C_brain_sub_state;
    }

    if (mCurrentMotion != eSligMotions::M_StandIdle_0_4B4EC0)
    {
        return field_11C_brain_sub_state;
    }

    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        field_C4_velx = -(ScaleToGridSize(field_CC_sprite_scale) / FP_FromInteger(6));
    }
    else
    {
        field_C4_velx = (ScaleToGridSize(field_CC_sprite_scale) / FP_FromInteger(6));
    }

    mNextMotion = eSligMotions::M_SteppingToStand_14_4B8480;
    return Brain_ListeningToGlukkon_States::IdleListening_1;
}

s16 Slig::Brain_ListenToGlukkon_Moving(BaseAliveGameObject* pGlukkonObj)
{
    FP xOffset = (ScaleToGridSize(field_CC_sprite_scale) * FP_FromInteger(mCurrentMotion != eSligMotions::M_Running_4_4B6000 ? 1 : 3));
    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        xOffset = -xOffset;
    }

    if (WallHit(field_CC_sprite_scale * FP_FromInteger(45), xOffset))
    {
        mNextMotion = eSligMotions::M_StandIdle_0_4B4EC0;
        return Brain_ListeningToGlukkon_States::IdleListening_1;
    }

    if (mCurrentMotion == eSligMotions::M_StandIdle_0_4B4EC0)
    {
        if (mNextMotion != eSligMotions::M_Walking_2_4B5BC0 && mNextMotion != eSligMotions::M_Running_4_4B6000)
        {
            return Brain_ListeningToGlukkon_States::IdleListening_1;
        }
    }

    if (mCurrentMotion == eSligMotions::M_Knockback_34_4B68A0)
    {
        mNextMotion = eSligMotions::M_StandIdle_0_4B4EC0;
        return Brain_ListeningToGlukkon_States::IdleListening_1;
    }

    if (mCurrentMotion != eSligMotions::M_Walking_2_4B5BC0 && mCurrentMotion != eSligMotions::M_Running_4_4B6000)
    {
        return field_11C_brain_sub_state;
    }

    if (mCurrentMotion != eSligMotions::M_Running_4_4B6000 && FindSwitch_4B9A50())
    {
        mNextMotion = eSligMotions::M_StandIdle_0_4B4EC0;
        field_216_flags.Clear(Flags_216::eBit1_FollowGlukkon);
        field_216_flags.Set(Flags_216::eBit2_StoppedForLeverOrLift);
        return Brain_ListeningToGlukkon_States::StoppingNextToLever_5;
    }

    if (mCurrentMotion != eSligMotions::M_Running_4_4B6000 && FindLiftPoint_4B9B40())
    {
        mNextMotion = eSligMotions::M_StandIdle_0_4B4EC0;
        field_216_flags.Clear(Flags_216::eBit1_FollowGlukkon);
        field_216_flags.Set(Flags_216::eBit2_StoppedForLeverOrLift);
        return Brain_ListeningToGlukkon_States::StoppingOnLift_7;
    }

    if (!VIsFacingMe(pGlukkonObj))
    {
        mNextMotion = eSligMotions::M_StandIdle_0_4B4EC0;
        return Brain_ListeningToGlukkon_States::IdleListening_1;
    }

    if (VIsObjNearby(ScaleToGridSize(field_CC_sprite_scale), pGlukkonObj))
    {
        mNextMotion = eSligMotions::M_StandIdle_0_4B4EC0;
        return Brain_ListeningToGlukkon_States::IdleListening_1;
    }

    if (field_160_last_event_index != pEventSystem_5BC11C->field_28_last_event_index)
    {
        field_160_last_event_index = pEventSystem_5BC11C->field_28_last_event_index;
        if (pEventSystem_5BC11C->field_20_last_event == GameSpeakEvents::Glukkon_StayHere_38 && gMap.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0))
        {
            field_216_flags.Clear(Flags_216::eBit1_FollowGlukkon);
            NextCommand_4B9A00(Brain_ListeningToGlukkon_GlukkonCommands::Stay_2, Brain_ListeningToGlukkon_States::IdleListening_1);
            return Brain_ListeningToGlukkon_States::Speaking_4;
        }
    }

    if (!VIsObjNearby(ScaleToGridSize(field_CC_sprite_scale) * FP_FromInteger(5), pGlukkonObj))
    {
        if (mCurrentMotion == eSligMotions::M_Walking_2_4B5BC0)
        {
            mNextMotion = eSligMotions::M_Running_4_4B6000;
            return Brain_ListeningToGlukkon_States::Moving_2;
        }
    }

    if (!VIsObjNearby(ScaleToGridSize(field_CC_sprite_scale) * FP_FromInteger(5), pGlukkonObj) || mCurrentMotion != eSligMotions::M_Running_4_4B6000)
    {
        return field_11C_brain_sub_state;
    }

    mNextMotion = eSligMotions::M_Walking_2_4B5BC0;
    return Brain_ListeningToGlukkon_States::Moving_2;
}

s16 Slig::Brain_ListenToGlukkon_IdleListen(BaseAliveGameObject* pGlukkonObj, LiftPoint* pPlatformObj)
{
    if (mCurrentMotion != eSligMotions::M_StandIdle_0_4B4EC0)
    {
        field_20E_attention_timeout = 0;
        return field_11C_brain_sub_state;
    }

    if (field_216_flags.Get(Flags_216::eBit1_FollowGlukkon))
    {
        field_20E_attention_timeout++;
    }

    field_216_flags.Clear(Flags_216::eBit2_StoppedForLeverOrLift);

    FP directedGridSize = {};
    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        directedGridSize = -ScaleToGridSize(field_CC_sprite_scale);
    }
    else
    {
        directedGridSize = ScaleToGridSize(field_CC_sprite_scale);
    }

    const FP gridSize = ScaleToGridSize(field_CC_sprite_scale);

    const s32 bWallHit = WallHit(field_CC_sprite_scale * FP_FromInteger(45), directedGridSize);
    if (field_20E_attention_timeout <= 450 && (gMap.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 1) || field_216_flags.Get(Flags_216::eBit1_FollowGlukkon)))
    {
        if (field_216_flags.Get(Flags_216::eBit1_FollowGlukkon))
        {
            if (!VIsObjNearby(gridSize, pGlukkonObj))
            {
                if (!VIsFacingMe(pGlukkonObj))
                {
                    mNextMotion = eSligMotions::M_TurnAroundStanding_5_4B6390;
                    return field_11C_brain_sub_state;
                }

                if (VIsObjNearby(gridSize * FP_FromInteger(5), pGlukkonObj) && pGlukkonObj->mCurrentMotion != eSligMotions::M_Running_4_4B6000)
                {
                    if (!bWallHit)
                    {
                        if (FindLiftPoint_4B9B40())
                        {
                            field_216_flags.Clear(Flags_216::eBit1_FollowGlukkon);
                            return Brain_ListeningToGlukkon_States::StoppingOnLift_7;
                        }
                        else
                        {
                            NextCommand_4B9A00(Brain_ListeningToGlukkon_GlukkonCommands::Walk_4, Brain_ListeningToGlukkon_States::Moving_2);
                            return Brain_ListeningToGlukkon_States::Speaking_4;
                        }
                    }
                }
                else
                {
                    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
                    {
                        directedGridSize = -ScaleToGridSize(field_CC_sprite_scale);
                    }
                    else
                    {
                        directedGridSize = ScaleToGridSize(field_CC_sprite_scale);
                    }

                    if (!WallHit(field_CC_sprite_scale * FP_FromInteger(45), directedGridSize * FP_FromInteger(3)))
                    {
                        NextCommand_4B9A00(Brain_ListeningToGlukkon_GlukkonCommands::Run_5, Brain_ListeningToGlukkon_States::Moving_2);
                        return Brain_ListeningToGlukkon_States::Speaking_4;
                    }
                }
            }

            if (sSligsUnderControlCount_BAF7E8 > 1)
            {
                auto pOtherSlig = static_cast<Slig*>(FindObjectOfType(AETypes::eSlig_125, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5)));
                if (pOtherSlig)
                {
                    if (pOtherSlig != this && !VIsFacingMe(pOtherSlig) && pOtherSlig->mCurrentMotion != eSligMotions::M_TurnAroundStanding_5_4B6390)
                    {
                        mNextMotion = eSligMotions::M_TurnAroundStanding_5_4B6390;
                        return field_11C_brain_sub_state;
                    }
                }
            }
        }

        const GameSpeakEvents glukkonSpeak = LastGlukkonSpeak_4B3090();
        if (glukkonSpeak == GameSpeakEvents::eNone_m1 || glukkonSpeak == GameSpeakEvents::eSameAsLast_m2)
        {
            return field_11C_brain_sub_state;
        }

        if (glukkonSpeak == GameSpeakEvents::Glukkon_AllOYa_40)
        {
            NextCommand_4B9A00(Brain_ListeningToGlukkon_GlukkonCommands::HeyOrAllYa_0, Brain_ListeningToGlukkon_States::IdleListening_1);
        }
        else
        {
            if (glukkonSpeak == GameSpeakEvents::Glukkon_Laugh_43)
            {
                NextCommand_4B9A00(Brain_ListeningToGlukkon_GlukkonCommands::AfterShootOrLaugh_6, Brain_ListeningToGlukkon_States::IdleListening_1);
            }
            else
            {
                if (!field_216_flags.Get(Flags_216::eBit3_GlukkonCalledAllOYa) || glukkonSpeak == GameSpeakEvents::Glukkon_Hey_36)
                {
                    if (!HeardGlukkonToListenTo_4B9690(glukkonSpeak))
                    {
                        field_216_flags.Set(Flags_216::eBit3_GlukkonCalledAllOYa, glukkonSpeak == GameSpeakEvents::Glukkon_AllOYa_40);
                        return field_11C_brain_sub_state;
                    }
                }

                switch (glukkonSpeak)
                {
                    case GameSpeakEvents::Glukkon_Hey_36:
                        if (!VIsFacingMe(pGlukkonObj))
                        {
                            mNextMotion = eSligMotions::M_TurnAroundStanding_5_4B6390;
                        }
                        NextCommand_4B9A00(Brain_ListeningToGlukkon_GlukkonCommands::HeyOrAllYa_0, Brain_ListeningToGlukkon_States::IdleListening_1);
                        break;

                    case GameSpeakEvents::Glukkon_DoIt_37:
                        if (pPlatformObj && pPlatformObj->Type() == AETypes::eLiftPoint_78)
                        {
                            const FP scaled_2 = (ScaleToGridSize(field_CC_sprite_scale) / FP_FromInteger(2));
                            const FP lineMidX = FP_FromInteger((BaseAliveGameObjectCollisionLine->field_0_rect.x + BaseAliveGameObjectCollisionLine->field_0_rect.w) / 2);
                            const FP v33 = mBaseAnimatedWithPhysicsGameObject_XPos - lineMidX >= FP_FromInteger(0) ? mBaseAnimatedWithPhysicsGameObject_XPos - lineMidX : lineMidX - mBaseAnimatedWithPhysicsGameObject_XPos;
                            if (v33 < scaled_2 && (pPlatformObj->vOnTopFloor() || pPlatformObj->vOnBottomFloor()))
                            {
                                mNextMotion = eSligMotions::M_LiftGrip_46_4B3700;

                                for (s32 i = 0; i < gBaseAliveGameObjects_5C1B7C->Size(); i++)
                                {
                                    BaseAliveGameObject* pFoundSlig = gBaseAliveGameObjects_5C1B7C->ItemAt(i);
                                    if (!pFoundSlig)
                                    {
                                        break;
                                    }

                                    if (pFoundSlig->Type() == AETypes::eSlig_125 && pFoundSlig != this && pFoundSlig->mNextMotion == eSligMotions::M_LiftGrip_46_4B3700)
                                    {
                                        mNextMotion = eSligMotions::M_StandIdle_0_4B4EC0;
                                    }
                                }

                                field_11C_brain_sub_state = Brain_ListeningToGlukkon_States::IdleListening_1;
                                field_216_flags.Set(Flags_216::eBit3_GlukkonCalledAllOYa, glukkonSpeak == GameSpeakEvents::Glukkon_AllOYa_40);
                                return field_11C_brain_sub_state;
                            }
                        }

                        if (FindObjectOfType(AETypes::eLever_139, ScaleToGridSize(field_CC_sprite_scale) + mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5)) || FindObjectOfType(AETypes::eLever_139, mBaseAnimatedWithPhysicsGameObject_XPos - ScaleToGridSize(field_CC_sprite_scale), mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5)))
                        {
                            field_11C_brain_sub_state = Brain_ListeningToGlukkon_States::PullingLever_6;
                            field_216_flags.Set(Flags_216::eBit3_GlukkonCalledAllOYa, glukkonSpeak == GameSpeakEvents::Glukkon_AllOYa_40);
                            return field_11C_brain_sub_state;
                        }

                        if (!FindObjectOfType(AETypes::eMudokon_110, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5)))
                        {
                            const FP scaled = field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX) ? -ScaleToGridSize(field_CC_sprite_scale) : ScaleToGridSize(field_CC_sprite_scale);

                            if (!FindObjectOfType(AETypes::eMudokon_110, scaled + mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5)))
                            {
                                NextCommand_4B9A00(Brain_ListeningToGlukkon_GlukkonCommands::DoitGunReload_7, Brain_ListeningToGlukkon_States::LostAttention_9);
                                field_11C_brain_sub_state = Brain_ListeningToGlukkon_States::Speaking_4;
                                --sSligsUnderControlCount_BAF7E8;
                                field_216_flags.Set(Flags_216::eBit3_GlukkonCalledAllOYa, glukkonSpeak == GameSpeakEvents::Glukkon_AllOYa_40);
                                return field_11C_brain_sub_state;
                            }
                        }

                        mNextMotion = eSligMotions::M_Beat_51_4B6C00;

                        field_11C_brain_sub_state = Brain_ListeningToGlukkon_States::IdleListening_1;
                        field_216_flags.Set(Flags_216::eBit3_GlukkonCalledAllOYa, glukkonSpeak == GameSpeakEvents::Glukkon_AllOYa_40);
                        return field_11C_brain_sub_state;

                    case GameSpeakEvents::Glukkon_StayHere_38:
                        field_216_flags.Clear(Flags_216::eBit1_FollowGlukkon);
                        NextCommand_4B9A00(Brain_ListeningToGlukkon_GlukkonCommands::Stay_2, Brain_ListeningToGlukkon_States::IdleListening_1);
                        break;

                    case GameSpeakEvents::Glukkon_Commere_39:
                        field_216_flags.Set(Flags_216::eBit1_FollowGlukkon);
                        if (VIsObjNearby(gridSize, pGlukkonObj))
                        {
                            if (!VIsObjNearby(gridSize * FP_FromDouble(0.5), pGlukkonObj))
                            {
                                field_11C_brain_sub_state = Brain_ListeningToGlukkon_States::NextToLeverThroughComeHere_3;
                                field_216_flags.Set(Flags_216::eBit3_GlukkonCalledAllOYa, glukkonSpeak == GameSpeakEvents::Glukkon_AllOYa_40);
                                return field_11C_brain_sub_state;
                            }
                        }

                        if (FindLiftPoint_4B9B40())
                        {
                            field_216_flags.Clear(Flags_216::eBit1_FollowGlukkon);
                            field_11C_brain_sub_state = Brain_ListeningToGlukkon_States::StoppingOnLift_7;
                            field_216_flags.Set(Flags_216::eBit3_GlukkonCalledAllOYa, glukkonSpeak == GameSpeakEvents::Glukkon_AllOYa_40);
                            return field_11C_brain_sub_state;
                        }

                        if (bWallHit)
                        {
                            NextCommand_4B9A00(Brain_ListeningToGlukkon_GlukkonCommands::Bs_3, Brain_ListeningToGlukkon_States::IdleListening_1);
                        }
                        else
                        {
                            if (!VIsFacingMe(pGlukkonObj))
                            {
                                mNextMotion = eSligMotions::M_TurnAroundStanding_5_4B6390;
                            }
                            NextCommand_4B9A00(Brain_ListeningToGlukkon_GlukkonCommands::Stay_2, Brain_ListeningToGlukkon_States::IdleListening_1);
                        }
                        break;

                    case GameSpeakEvents::Glukkon_KillEm_44:
                        field_11C_brain_sub_state = Brain_ListeningToGlukkon_States::Shooting_8;
                        field_120_timer = sGnFrame + 25;
                        field_216_flags.Set(Flags_216::eBit3_GlukkonCalledAllOYa, glukkonSpeak == GameSpeakEvents::Glukkon_AllOYa_40);
                        return field_11C_brain_sub_state;

                    default:
                        field_216_flags.Set(Flags_216::eBit3_GlukkonCalledAllOYa, glukkonSpeak == GameSpeakEvents::Glukkon_AllOYa_40);
                        return field_11C_brain_sub_state;
                }
            }
        }
        field_11C_brain_sub_state = Brain_ListeningToGlukkon_States::Speaking_4;
        field_216_flags.Set(Flags_216::eBit3_GlukkonCalledAllOYa, glukkonSpeak == GameSpeakEvents::Glukkon_AllOYa_40);
        return field_11C_brain_sub_state;
    }

    PauseALittle_4BDD00();
    field_216_flags.Clear(Flags_216::eBit1_FollowGlukkon);
    --sSligsUnderControlCount_BAF7E8;
    return Brain_ListeningToGlukkon_States::GettingAttention_0;
}

s16 Slig::Brain_ListenToGlukkon_GettingAttention(BaseAliveGameObject* pGlukkonObj)
{
    mNextMotion = eSligMotions::M_StandIdle_0_4B4EC0;
    field_20E_attention_timeout = 0;
    field_216_flags.Clear(Flags_216::eBit1_FollowGlukkon);
    field_216_flags.Clear(Flags_216::eBit2_StoppedForLeverOrLift);
    if (!VIsFacingMe(pGlukkonObj))
    {
        mNextMotion = eSligMotions::M_TurnAroundStanding_5_4B6390;
    }
    NextCommand_4B9A00(Brain_ListeningToGlukkon_GlukkonCommands::HeyOrAllYa_0, Brain_ListeningToGlukkon_States::IdleListening_1);
    return Brain_ListeningToGlukkon_States::Speaking_4;
}

s16 Slig::Brain_Empty_5_4B3220()
{
    
    return 0;
}

s16 Slig::Brain_Empty_6_4B3420()
{
    
    return 0;
}

s16 Slig::Brain_SpottedEnemy_7_4B3240()
{
    if (gMap.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0) || field_218_tlv_data.field_2A_chase_abe_when_spotted == Choice_short::eNo_0)
    {
        if (VOnSameYLevel(sControlledCharacter_5C1B8C) && VIsObj_GettingNear(sControlledCharacter_5C1B8C) && VIsObjNearby(ScaleToGridSize(field_CC_sprite_scale) * FP_FromInteger(3), sControlledCharacter_5C1B8C) && !Event_Get(kEventResetting) && !sControlledCharacter_5C1B8C->mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit8_bInvisible))
        {
            ToShoot_4BF9A0();
        }
        else if (
            VOnSameYLevel(sControlledCharacter_5C1B8C) && Event_Get(kEventAbeOhm) && VIsFacingMe(sControlledCharacter_5C1B8C))
        {
            ToShoot_4BF9A0();
        }
        else if (field_120_timer > static_cast<s32>(sGnFrame))
        {
            if (sActiveHero->mHealth <= FP_FromInteger(0))
            {
                ToAbeDead_4B3580();
            }
            else
            {
                ShouldStillBeAlive_4BBC00();
            }
        }
        else if (VIsFacingMe(sControlledCharacter_5C1B8C) && !sControlledCharacter_5C1B8C->mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit8_bInvisible))
        {
            ToShoot_4BF9A0();
        }
        else
        {
            ToTurn_4BE090();
        }
    }
    else
    {
        ToChase_4BCFF0();
    }

    return 302;
}

s16 Slig::Brain_Empty_8_4B3120()
{
    
    return 0;
}

s16 Slig::Brain_Empty_9_4B3440()
{
    
    return 0;
}

s16 Slig::Brain_EnemyDead_10_4B3460()
{
    if (Event_Get(kEventDeathReset) && !gMap.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        return 113;
    }

    if (field_120_timer >= static_cast<s32>(sGnFrame))
    {
        ShouldStillBeAlive_4BBC00();
    }
    else
    {
        const auto rnd = Math_NextRandom();

        // Say this often
        if (rnd < 25)
        {
            mNextMotion = eSligMotions::M_SpeakBullshit1_25_4B5450;
            field_120_timer = sGnFrame + 15;
            return 113;
        }

        // Say this less often
        if (rnd < 50)
        {
            mNextMotion = eSligMotions::M_SpeakBullshit2_27_4B5490;
            field_120_timer = sGnFrame + 15;
            return 113;
        }

        // And turn even less often
        if (sActiveHero->mHealth > FP_FromInteger(0))
        {
            TurnOrWalk_4BD6A0(0);
        }
    }
    return 113;
}

s16 Slig::Brain_KilledEnemy_10_4B35C0()
{
    if (field_120_timer > static_cast<s32>(sGnFrame))
    {
        ShouldStillBeAlive_4BBC00();
    }
    else
    {
        ToAbeDead_4B3580();
    }
    return 114;
}

s16 Slig::Brain_PanicTurning_12_4BC490()
{
    if (Event_Get(kEventDeathReset) && !gMap.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        return 107;
    }

    if (ListenToGlukkonCommands_4B95D0())
    {
        ToUnderGlukkonCommand_4B9660();
        return 107;
    }

    bool checkTurn = false;
    if (mCurrentMotion != eSligMotions::M_TurnAroundStanding_5_4B6390)
    {
        if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            mNextMotion = eSligMotions::M_TurnAroundStanding_5_4B6390;
            return 107;
        }
        checkTurn = true;
    }
    else if (mCurrentMotion == eSligMotions::M_TurnAroundStanding_5_4B6390)
    {
        if (!field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            checkTurn = true;
        }
    }

    if (checkTurn)
    {
        if (field_20_animation.field_92_current_frame == 4)
        {
            if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
            {
                if (sControlledCharacter_5C1B8C->field_C4_velx >= FP_FromInteger(0) && (sControlledCharacter_5C1B8C->field_C4_velx != FP_FromInteger(0) || sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_XPos >= mBaseAnimatedWithPhysicsGameObject_XPos))
                {
                    ShouldStillBeAlive_4BBC00();
                    return 107;
                }
            }
            else
            {
                if (sControlledCharacter_5C1B8C->field_C4_velx <= FP_FromInteger(0) && (sControlledCharacter_5C1B8C->field_C4_velx != FP_FromInteger(0) || sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_XPos <= mBaseAnimatedWithPhysicsGameObject_XPos))
                {
                    ShouldStillBeAlive_4BBC00();
                    return 107;
                }
            }

            PSX_RECT bRect = {};
            VGetBoundingRect(&bRect, 1);

            PSX_RECT charRect = {};
            sControlledCharacter_5C1B8C->VGetBoundingRect(&charRect, 1);

            if (sControlledCharacter_5C1B8C->Type() != AETypes::eGlukkon_67 && sControlledCharacter_5C1B8C->Type() != AETypes::eSlig_125 && !IsInInvisibleZone(sControlledCharacter_5C1B8C) && !sControlledCharacter_5C1B8C->mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit8_bInvisible))
            {
                if (charRect.x <= bRect.w && charRect.w >= bRect.x && charRect.h >= bRect.y && charRect.y <= bRect.h)
                {
                    field_20_animation.mAnimFlags.Toggle(AnimFlags::eBit5_FlipX);
                    return 107;
                }
            }
        }
        ShouldStillBeAlive_4BBC00();
        return 107;
    }

    if (field_120_timer > static_cast<s32>(sGnFrame))
    {
        ToPanicRunning_4BCA30();
    }
    else
    {
        WaitOrWalk_4BE870();
    }
    return 107;
}

s16 Slig::Brain_PanicRunning_13_4BC780()
{
    if (field_C4_velx > FP_FromInteger(4) && ((ScaleToGridSize(field_CC_sprite_scale) * FP_FromInteger(4)) + mBaseAnimatedWithPhysicsGameObject_XPos) > FP_FromInteger(field_138_zone_rect.w))
    {
        ToPanicTurn_4BC750();
    }
    else if (field_C4_velx < FP_FromInteger(-4) && (mBaseAnimatedWithPhysicsGameObject_XPos - (ScaleToGridSize(field_CC_sprite_scale) * FP_FromInteger(4))) < FP_FromInteger(field_138_zone_rect.x))
    {
        ToPanicTurn_4BC750();
    }
    else if (HandleEnemyStopper_4BBA00(4))
    {
        ToPanicTurn_4BC750();
    }
    else if (ListenToGlukkonCommands_4B95D0())
    {
        ToUnderGlukkonCommand_4B9660();
    }
    else if (!mCurrentMotion && mNextMotion == -1)
    {
        ToPanicRunning_4BCA30();
    }
    else if (VOnSameYLevel(sControlledCharacter_5C1B8C) && sControlledCharacter_5C1B8C->Type() != AETypes::eGlukkon_67 && VIsFacingMe(sControlledCharacter_5C1B8C) && !IsInInvisibleZone(sControlledCharacter_5C1B8C) && !sControlledCharacter_5C1B8C->mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit8_bInvisible) && !IsWallBetween_4BB8B0(this, sControlledCharacter_5C1B8C) && gMap.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0) && gMap.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0) && !Event_Get(kEventResetting))
    {
        ToShoot_4BF9A0();
    }
    else if (sActiveHero->mHealth <= FP_FromInteger(0))
    {
        ToAbeDead_4B3580();
    }
    else
    {
        if (field_120_timer <= static_cast<s32>(sGnFrame))
        {
            WaitOrWalk_4BE870();
        }
        else
        {
            ShouldStillBeAlive_4BBC00();
        }
    }
    return 109;
}

s16 Slig::Brain_PanicYelling_14_4BCA70()
{
    if (ListenToGlukkonCommands_4B95D0())
    {
        ToUnderGlukkonCommand_4B9660();
    }

    if (mCurrentMotion != eSligMotions::M_SpeakPanic_28_4B54B0)
    {
        if (mNextMotion != eSligMotions::M_SpeakPanic_28_4B54B0)
        {
            mNextMotion = eSligMotions::M_SpeakPanic_28_4B54B0;
        }

        if (mCurrentMotion != eSligMotions::M_SpeakPanic_28_4B54B0)
        {
            ShouldStillBeAlive_4BBC00();
            return 115;
        }
    }

    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        Event_Broadcast(kEventAlarm, this);

        const bool kFlipX = field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX);
        field_120_timer = sGnFrame + field_218_tlv_data.field_2E_panic_timeout;
        if ((!kFlipX || mBaseAnimatedWithPhysicsGameObject_XPos >= FP_FromInteger((field_138_zone_rect.x + field_138_zone_rect.w) / 2)) && (kFlipX || mBaseAnimatedWithPhysicsGameObject_XPos <= FP_FromInteger((field_138_zone_rect.x + field_138_zone_rect.w) / 2)))
        {
            ToPanicRunning_4BCA30();
        }
        else
        {
            ToPanicTurn_4BC750();
        }
    }
    else
    {
        ShouldStillBeAlive_4BBC00();
    }
    return 115;
}

s16 Slig::Brain_Idle_15_4BD800()
{
    if ((Event_Get(kEventAbeOhm) || Event_Get(kEventAlarm)) && !Event_Get(kEventResetting) && field_218_tlv_data.field_2E_panic_timeout)
    {
        ToPanicYelling_4BCBA0();
        return 104;
    }

    if (ListenToGlukkonCommands_4B95D0())
    {
        ToUnderGlukkonCommand_4B9660();
        return 104;
    }

    if (VOnSameYLevel(sControlledCharacter_5C1B8C) && 
        VIsFacingMe(sControlledCharacter_5C1B8C) && 
        !IsInInvisibleZone(sControlledCharacter_5C1B8C) &&
        !sControlledCharacter_5C1B8C->mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit8_bInvisible) &&
        !IsWallBetween_4BB8B0(this, sControlledCharacter_5C1B8C) && (!field_15E_spotted_possessed_slig || sControlledCharacter_5C1B8C->Type() != AETypes::eSlig_125) &&
        !IsAbeEnteringDoor_4BB990(sControlledCharacter_5C1B8C) &&
        !Event_Get(kEventResetting) &&
        gMap.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0) && sControlledCharacter_5C1B8C->Type() != AETypes::eGlukkon_67)
    {
        RespondToEnemyOrPatrol_4B3140();
        return 104;
    }

    if (sActiveHero->mHealth <= FP_FromInteger(0))
    {
        ToAbeDead_4B3580();
        return 104;
    }

    auto pShooter = static_cast<BaseAliveGameObject*>(Event_Get(kEventShooting));
    if (pShooter && pShooter->field_CC_sprite_scale == field_CC_sprite_scale && gMap.Is_Point_In_Current_Camera_4810D0(pShooter->field_C2_lvl_number, pShooter->field_C0_path_number, pShooter->mBaseAnimatedWithPhysicsGameObject_XPos, pShooter->mBaseAnimatedWithPhysicsGameObject_YPos, 0) && pShooter == sControlledCharacter_5C1B8C && gMap.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0) && !Event_Get(kEventResetting))
    {
        if (VIsFacingMe(pShooter))
        {
            RespondToEnemyOrPatrol_4B3140();
            return 104;
        }
        else
        {
            ToTurn_4BE090();
            return 104;
        }
    }

    auto pNoiseOrSpeaking = static_cast<BaseAliveGameObject*>(Event_Get(kEventSuspiciousNoise));
    if (!pNoiseOrSpeaking)
    {
        pNoiseOrSpeaking = static_cast<BaseAliveGameObject*>(Event_Get(kEventSpeaking));
    }

    if (pNoiseOrSpeaking && pNoiseOrSpeaking->field_CC_sprite_scale == field_CC_sprite_scale && gMap.Is_Point_In_Current_Camera_4810D0(pNoiseOrSpeaking->field_C2_lvl_number, pNoiseOrSpeaking->field_C0_path_number, pNoiseOrSpeaking->mBaseAnimatedWithPhysicsGameObject_XPos, pNoiseOrSpeaking->mBaseAnimatedWithPhysicsGameObject_YPos, 0) && pNoiseOrSpeaking != this && gMap.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0) && !Event_Get(kEventResetting))
    {
        if (pNoiseOrSpeaking != sControlledCharacter_5C1B8C)
        {
            if (Math_NextRandom() >= 192u)
            {
                return 104;
            }
        }

        if (VIsFacingMe(sControlledCharacter_5C1B8C))
        {
            GoAlertedOrSayWhat_4BF140();
        }
        else
        {
            TurnOrSayWhat_4BEBC0();
        }
        return 104;
    }
    else
    {
        if (sControlledCharacter_5C1B8C->field_CC_sprite_scale > field_CC_sprite_scale && VIsFacingMe(sControlledCharacter_5C1B8C) && !IsInInvisibleZone(sControlledCharacter_5C1B8C) && !sControlledCharacter_5C1B8C->mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit8_bInvisible) && gMap.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0) && !InZCover_4BB7C0(sControlledCharacter_5C1B8C) && !InZCover_4BB7C0(this) && !Event_Get(kEventResetting))
        {
            ToZShoot_4BF9E0();
            return 104;
        }

        if (mCurrentMotion == eSligMotions::M_StandIdle_0_4B4EC0 && field_120_timer <= static_cast<s32>(sGnFrame))
        {
            ToTurn_4BE090();
            return 104;
        }
        else
        {
            ShouldStillBeAlive_4BBC00();
            return 104;
        }
    }
}

s16 Slig::Brain_StopChasing_16_4BCE30()
{
    if (field_120_timer > static_cast<s32>(sGnFrame))
    {
        if (HandleEnemyStopper_4BBA00(4))
        {
            mNextMotion = eSligMotions::M_StandIdle_0_4B4EC0;
            SetBrain(&Slig::Brain_Idle_15_4BD800);
            field_120_timer = sGnFrame + 1;
        }
    }
    else
    {
        field_15C_force_alive_state = 0;
        ToTurn_4BE090();
    }
    return 119;
}

s16 Slig::Brain_Chasing_17_4BCBD0()
{
    if (gMap.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0) && VOnSameYLevel(sControlledCharacter_5C1B8C) && VIsFacingMe(sControlledCharacter_5C1B8C) && !IsInInvisibleZone(sControlledCharacter_5C1B8C) && !sControlledCharacter_5C1B8C->mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit8_bInvisible) && !IsWallBetween_4BB8B0(this, sControlledCharacter_5C1B8C) && !RenderLayerIs_4BBBC0(sControlledCharacter_5C1B8C) && !Event_Get(kEventResetting) && sControlledCharacter_5C1B8C->Type() != AETypes::eGlukkon_67)
    {
        field_15C_force_alive_state = 0;
        RespondToEnemyOrPatrol_4B3140();
        return 118;
    }

    if (HandleEnemyStopper_4BBA00(4))
    {
        mNextMotion = eSligMotions::M_StandIdle_0_4B4EC0;
        SetBrain(&Slig::Brain_Idle_15_4BD800);
        field_120_timer = sGnFrame + 1;
        return 118;
    }

    if (field_C0_path_number != gMap.mCurrentPath || field_C2_lvl_number != gMap.mCurrentLevel || (Event_Get(kEventDeathReset) && !gMap.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0)))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
    else if (gMap.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0))
    {
        SetBrain(&Slig::Brain_StopChasing_16_4BCE30);
        field_120_timer = sGnFrame + field_218_tlv_data.field_34_stop_chase_delay;
        return 118;
    }
    return 118;
}

s16 Slig::Brain_StartChasing_18_4BCEB0()
{
    if (field_120_timer > static_cast<s32>(sGnFrame))
    {
        if (gMap.Is_Point_In_Current_Camera_4810D0(
                field_C2_lvl_number,
                field_C0_path_number,
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos,
                0)
            && !sControlledCharacter_5C1B8C->mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit8_bInvisible) && sControlledCharacter_5C1B8C->Type() != AETypes::eGlukkon_67)
        {
            field_15C_force_alive_state = 0;
            ToShoot_4BF9A0();
        }
    }
    else
    {
        if (field_C0_path_number != gMap.mCurrentPath || field_C2_lvl_number != gMap.mCurrentLevel)
        {
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        }

        if (!VIsFacingMe(sControlledCharacter_5C1B8C))
        {
            field_20_animation.mAnimFlags.Toggle(AnimFlags::eBit5_FlipX);
        }

        field_15C_force_alive_state = 1;
        mNextMotion = eSligMotions::M_Running_4_4B6000;
        SetBrain(&Slig::Brain_Chasing_17_4BCBD0);
        field_120_timer = field_218_tlv_data.field_14_pause_time;
    }
    return 122;
}

s16 Slig::Brain_Turning_19_4BDDD0()
{
    if (Event_Get(kEventDeathReset) && !gMap.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        return 106;
    }

    if (ListenToGlukkonCommands_4B95D0())
    {
        ToUnderGlukkonCommand_4B9660();
        return 106;
    }

    if ((mCurrentMotion == eSligMotions::M_TurnAroundStanding_5_4B6390 && field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame)) || (mCurrentMotion == eSligMotions::M_StandIdle_0_4B4EC0 && mNextMotion == -1))
    {
        WaitOrWalk_4BE870();
        return 106;
    }

    if (field_20_animation.field_92_current_frame != 4)
    {
        ShouldStillBeAlive_4BBC00();
        return 106;
    }

    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        if (sControlledCharacter_5C1B8C->field_C4_velx >= FP_FromInteger(0) && (sControlledCharacter_5C1B8C->field_C4_velx > FP_FromInteger(0) || sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_XPos >= mBaseAnimatedWithPhysicsGameObject_XPos))
        {
            ShouldStillBeAlive_4BBC00();
            return 106;
        }
    }
    else
    {
        if (sControlledCharacter_5C1B8C->field_C4_velx <= FP_FromInteger(0) && (sControlledCharacter_5C1B8C->field_C4_velx != FP_FromInteger(0) || sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_XPos <= mBaseAnimatedWithPhysicsGameObject_XPos))
        {
            ShouldStillBeAlive_4BBC00();
            return 106;
        }
    }

    PSX_RECT bRect = {};
    VGetBoundingRect(&bRect, 1);

    PSX_RECT charRect = {};
    sControlledCharacter_5C1B8C->VGetBoundingRect(&charRect, 1);


    if (sControlledCharacter_5C1B8C->Type() != AETypes::eGlukkon_67 && sControlledCharacter_5C1B8C->field_D6_scale == field_D6_scale && !IsInInvisibleZone(sControlledCharacter_5C1B8C) && !sControlledCharacter_5C1B8C->mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit8_bInvisible) && !IsWallBetween_4BB8B0(this, sControlledCharacter_5C1B8C) && PSX_Rects_overlap_no_adjustment(&charRect, &bRect) && sControlledCharacter_5C1B8C->Type() != AETypes::eSlig_125)
    {
        field_20_animation.mAnimFlags.Toggle(AnimFlags::eBit5_FlipX);
        return 106;
    }

    ShouldStillBeAlive_4BBC00();
    return 106;
}

s16 Slig::Brain_StoppingNextToMudokon_20_4BF1E0()
{
    if (mCurrentMotion != eSligMotions::M_StandIdle_0_4B4EC0)
    {
        return 128;
    }

    BaseAliveGameObject* pBeatTarget = FindBeatTarget_4BD070(static_cast<s32>(AETypes::eMudokon2_81), 1);
    if (!pBeatTarget || pBeatTarget->mHealth <= FP_FromInteger(0))
    {
        WaitOrWalk_4BE870();
        return 128;
    }

    if (Math_NextRandom() >= 100)
    {
        if (Math_NextRandom() & 1)
        {
            mNextMotion = eSligMotions::M_SpeakBullshit1_25_4B5450;
        }
        else
        {
            mNextMotion = eSligMotions::M_SpeakBullshit2_27_4B5490;
        }
    }
    else
    {
        mNextMotion = eSligMotions::M_Beat_51_4B6C00;
        SetBrain(&Slig::Brain_BeatingUp_24_4BF2B0);
    }

    return 128;
}

s16 Slig::Brain_Walking_21_4BE0C0()
{
    if (unused_BAF7E4) // NOTE: This is never set to true, so this branch is never executed.
    {
        ToPanicRunning_4BCA30();
        return 108;
    }

    if (Event_Get(kEventAlarm) && field_218_tlv_data.field_2E_panic_timeout)
    {
        ToPanicYelling_4BCBA0();
        return 108;
    }

    if (field_C4_velx > FP_FromInteger(0))
    {
        if ((ScaleToGridSize(field_CC_sprite_scale) * FP_FromInteger(2) + mBaseAnimatedWithPhysicsGameObject_XPos) >= FP_FromInteger(field_138_zone_rect.w))
        {
            PauseALittle_4BDD00();
            return 108;
        }
    }

    if (field_C4_velx < FP_FromInteger(0))
    {
        if ((mBaseAnimatedWithPhysicsGameObject_XPos - (ScaleToGridSize(field_CC_sprite_scale) * FP_FromInteger(2))) <= FP_FromInteger(field_138_zone_rect.x))
        {
            PauseALittle_4BDD00();
            return 108;
        }
    }

    if (field_C4_velx == FP_FromInteger(0) && mCurrentMotion == eSligMotions::M_StandIdle_0_4B4EC0 && mNextMotion != eSligMotions::M_Walking_2_4B5BC0)
    {
        PauseALittle_4BDD00();
    }
    else if (HandleEnemyStopper_4BBA00(2))
    {
        PauseALittle_4BDD00();
    }
    else if (ListenToGlukkonCommands_4B95D0())
    {
        ToUnderGlukkonCommand_4B9660();
    }
    else if (VOnSameYLevel(sControlledCharacter_5C1B8C) &&
        VIsFacingMe(sControlledCharacter_5C1B8C) &&
        !IsInInvisibleZone(sControlledCharacter_5C1B8C) &&
        !sControlledCharacter_5C1B8C->mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit8_bInvisible) &&
        !IsWallBetween_4BB8B0(this, sControlledCharacter_5C1B8C) &&
        gMap.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0) &&
        gMap.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0) && (!field_15E_spotted_possessed_slig || sControlledCharacter_5C1B8C->Type() != AETypes::eSlig_125) &&
        !IsAbeEnteringDoor_4BB990(sControlledCharacter_5C1B8C) && !Event_Get(kEventResetting) && sControlledCharacter_5C1B8C->Type() != AETypes::eGlukkon_67)
    {
        RespondToEnemyOrPatrol_4B3140();
    }
    else
    {
        auto pShooter = static_cast<BaseAliveGameObject*>(Event_Get(kEventShooting));
        if (pShooter && pShooter->field_CC_sprite_scale == field_CC_sprite_scale && gMap.Is_Point_In_Current_Camera_4810D0(pShooter->field_C2_lvl_number, pShooter->field_C0_path_number, pShooter->mBaseAnimatedWithPhysicsGameObject_XPos, pShooter->mBaseAnimatedWithPhysicsGameObject_YPos, 0) && pShooter == sControlledCharacter_5C1B8C && gMap.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0) && !Event_Get(kEventResetting))
        {
            if (VIsFacingMe(pShooter))
            {
                RespondToEnemyOrPatrol_4B3140();
            }
            else
            {
                ToTurn_4BE090();
            }
        }
        else if (VOnSameYLevel(sControlledCharacter_5C1B8C) && VIsFacingMe(sControlledCharacter_5C1B8C) && !IsWallBetween_4BB8B0(this, sControlledCharacter_5C1B8C) && Event_Get(kEventAbeOhm) && gMap.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0) && gMap.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0) && !Event_Get(kEventResetting))
        {
            ToShoot_4BF9A0();
        }
        else if (Event_Get(kEventAbeOhm) && !Event_Get(kEventResetting) && field_218_tlv_data.field_2E_panic_timeout)
        {
            ToPanicYelling_4BCBA0();
        }
        else if (sActiveHero->mHealth <= FP_FromInteger(0))
        {
            ToAbeDead_4B3580();
        }
        else
        {
            auto pNoiseOrSpeaker = static_cast<BaseAliveGameObject*>(Event_Get(kEventSuspiciousNoise));
            if (!pNoiseOrSpeaker)
            {
                pNoiseOrSpeaker = static_cast<BaseAliveGameObject*>(Event_Get(kEventSpeaking));
            }

            if (pNoiseOrSpeaker && pNoiseOrSpeaker->field_CC_sprite_scale == field_CC_sprite_scale && gMap.Is_Point_In_Current_Camera_4810D0(pNoiseOrSpeaker->field_C2_lvl_number, pNoiseOrSpeaker->field_C0_path_number, pNoiseOrSpeaker->mBaseAnimatedWithPhysicsGameObject_XPos, pNoiseOrSpeaker->mBaseAnimatedWithPhysicsGameObject_YPos, 0) && pNoiseOrSpeaker != this && gMap.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0) && !Event_Get(kEventResetting))
            {
                if (VIsFacingMe(sControlledCharacter_5C1B8C))
                {
                    GoAlertedOrSayWhat_4BF140();
                }
                else
                {
                    TurnOrSayWhat_4BEBC0();
                }
            }
            else if (sControlledCharacter_5C1B8C->field_CC_sprite_scale > field_CC_sprite_scale && VIsFacingMe(sControlledCharacter_5C1B8C) && !IsInInvisibleZone(sControlledCharacter_5C1B8C) && !sControlledCharacter_5C1B8C->mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit8_bInvisible) && gMap.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0) && !InZCover_4BB7C0(sControlledCharacter_5C1B8C) && !InZCover_4BB7C0(this) && !Event_Get(kEventResetting))
            {
                ToZShoot_4BF9E0();
            }
            else
            {
                if (Math_NextRandom() < field_218_tlv_data.field_3E_percent_beat_mud && FindBeatTarget_4BD070(81, 2) && mCurrentMotion)
                {
                    mNextMotion = eSligMotions::M_StandIdle_0_4B4EC0;
                    SetBrain(&Slig::Brain_StoppingNextToMudokon_20_4BF1E0);
                    field_120_timer = sGnFrame + 30;
                }
                else
                {
                    ShouldStillBeAlive_4BBC00();
                }
            }
        }
    }
    return 108;
}

s16 Slig::Brain_GetAlertedTurn_22_4BE990()
{
    if (mCurrentMotion == eSligMotions::M_TurnAroundStanding_5_4B6390 && field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        GoAlertedOrSayWhat_4BF140();
        return 123;
    }

    if (ListenToGlukkonCommands_4B95D0())
    {
        ToUnderGlukkonCommand_4B9660();
        return 123;
    }

    if (field_20_animation.field_92_current_frame == 4)
    {
        bool tryTurningToPlayer = true;
        if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
        {
            if (sControlledCharacter_5C1B8C->field_C4_velx >= FP_FromInteger(0) && (sControlledCharacter_5C1B8C->field_C4_velx > FP_FromInteger(0) || sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_XPos >= mBaseAnimatedWithPhysicsGameObject_XPos))
            {
                tryTurningToPlayer = false;
            }
        }
        else if (sControlledCharacter_5C1B8C->field_C4_velx <= FP_FromInteger(0) && (sControlledCharacter_5C1B8C->field_C4_velx > FP_FromInteger(0) || sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_XPos <= mBaseAnimatedWithPhysicsGameObject_XPos))
        {
            tryTurningToPlayer = false;
        }

        if (tryTurningToPlayer)
        {
            if (sControlledCharacter_5C1B8C->Type() != AETypes::eGlukkon_67 && !IsInInvisibleZone(sControlledCharacter_5C1B8C) && !sControlledCharacter_5C1B8C->mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit8_bInvisible))
            {
                PSX_RECT bRect = {};
                VGetBoundingRect(&bRect, 1);

                PSX_RECT bRectChar = {};
                sControlledCharacter_5C1B8C->VGetBoundingRect(&bRectChar, 1);

                if (PSX_Rects_overlap_no_adjustment(&bRectChar, &bRect))
                {
                    field_20_animation.mAnimFlags.Toggle(AnimFlags::eBit5_FlipX);
                    return 123;
                }
            }
        }
    }

    if (mCurrentMotion != eSligMotions::M_StandIdle_0_4B4EC0)
    {
        ShouldStillBeAlive_4BBC00();
    }
    else
    {
        mNextMotion = eSligMotions::M_TurnAroundStanding_5_4B6390;
    }
    return 123;
}

s16 Slig::Brain_GetAlerted_23_4BEC40()
{
    if (field_120_timer != (field_218_tlv_data.field_3A_alerted_listen_time + static_cast<s32>(sGnFrame) - 2) || Math_RandomRange(0, 100) >= field_218_tlv_data.field_3C_percent_say_what)
    {
        if (ListenToGlukkonCommands_4B95D0())
        {
            ToUnderGlukkonCommand_4B9660();
        }
    }
    else
    {
        mNextMotion = eSligMotions::M_SpeakWhat_29_4B54D0;
    }

    // Check if we should be killing Abe
    if (VOnSameYLevel(sControlledCharacter_5C1B8C) &&
        VIsFacingMe(sControlledCharacter_5C1B8C) &&
        !IsInInvisibleZone(sControlledCharacter_5C1B8C) &&
        !sControlledCharacter_5C1B8C->mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit8_bInvisible) &&
        !IsWallBetween_4BB8B0(this, sControlledCharacter_5C1B8C) &&
        gMap.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0) && (sControlledCharacter_5C1B8C->Type() != AETypes::eSlig_125 && !field_15E_spotted_possessed_slig) &&
        !IsAbeEnteringDoor_4BB990(sControlledCharacter_5C1B8C) && !Event_Get(kEventResetting) && sControlledCharacter_5C1B8C->Type() != AETypes::eGlukkon_67)
    {
        RespondToEnemyOrPatrol_4B3140();
    }
    // Panic?
    else if ((Event_Get(kEventAbeOhm) || Event_Get(kEventAlarm)) && !Event_Get(kEventResetting) && field_218_tlv_data.field_2E_panic_timeout)
    {
        ToPanicYelling_4BCBA0();
    }
    else
    {
        // If some fool is trying to shoot us then shoot back
        auto pShootingSlig = static_cast<BaseAliveGameObject*>(Event_Get(kEventShooting));
        if (pShootingSlig && pShootingSlig->field_CC_sprite_scale == field_CC_sprite_scale && gMap.Is_Point_In_Current_Camera_4810D0(pShootingSlig->field_C2_lvl_number, pShootingSlig->field_C0_path_number, pShootingSlig->mBaseAnimatedWithPhysicsGameObject_XPos, pShootingSlig->mBaseAnimatedWithPhysicsGameObject_YPos, 0) && pShootingSlig == sControlledCharacter_5C1B8C && gMap.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0) && !Event_Get(kEventResetting))
        {
            if (VIsFacingMe(pShootingSlig))
            {
                RespondToEnemyOrPatrol_4B3140();
            }
            else
            {
                ToTurn_4BE090();
            }
        }
        else
        {
            // Is a mud being noisy?
            auto pNoisyMud = static_cast<BaseAliveGameObject*>(Event_Get(kEventSuspiciousNoise));
            if (!pNoisyMud)
            {
                pNoisyMud = static_cast<BaseAliveGameObject*>(Event_Get(kEventSpeaking));
            }

            // Then kill them
            if (pNoisyMud && gMap.Is_Point_In_Current_Camera_4810D0(pNoisyMud->field_C2_lvl_number, pNoisyMud->field_C0_path_number, pNoisyMud->mBaseAnimatedWithPhysicsGameObject_XPos, pNoisyMud->mBaseAnimatedWithPhysicsGameObject_YPos, 0) && (pNoisyMud == sControlledCharacter_5C1B8C || pNoisyMud->Type() == AETypes::eMudokon_110) && VOnSameYLevel(pNoisyMud) && VIsFacingMe(pNoisyMud) && (pNoisyMud != sControlledCharacter_5C1B8C || (!sControlledCharacter_5C1B8C->mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit8_bInvisible) && gMap.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0) && !Event_Get(kEventResetting))))
            {
                ToShoot_4BF9A0();
            }
            else
            {
                // Is a slig being noisy?
                auto pNoisySlig = static_cast<BaseAliveGameObject*>(Event_Get(kEventSuspiciousNoise));
                if (!pNoisySlig)
                {
                    pNoisySlig = static_cast<BaseAliveGameObject*>(Event_Get(kEventSpeaking));
                }

                // Then say what, stop walking to respond or try to kill them.
                if (pNoisySlig && gMap.Is_Point_In_Current_Camera_4810D0(pNoisySlig->field_C2_lvl_number, pNoisySlig->field_C0_path_number, pNoisySlig->mBaseAnimatedWithPhysicsGameObject_XPos, pNoisySlig->mBaseAnimatedWithPhysicsGameObject_YPos, 0) && (pNoisySlig == sControlledCharacter_5C1B8C || pNoisySlig->Type() != AETypes::eSlig_125) && !VIsFacingMe(pNoisySlig) && gMap.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0) && !Event_Get(kEventResetting))
                {
                    TurnOrSayWhat_4BEBC0();
                }
                else
                {
                    if (field_120_timer > static_cast<s32>(sGnFrame))
                    {
                        ShouldStillBeAlive_4BBC00();
                    }
                    else
                    {
                        WaitOrWalk_4BE870();
                    }
                }
            }
        }
    }
    return 124;
}

s16 Slig::Brain_BeatingUp_24_4BF2B0()
{
    if (mCurrentMotion != eSligMotions::M_StandIdle_0_4B4EC0 || field_120_timer >= static_cast<s32>(sGnFrame))
    {
        return 129;
    }

    if (Math_NextRandom() < 100)
    {
        mNextMotion = eSligMotions::M_Beat_51_4B6C00;
        return 129;
    }

    if (Math_NextRandom() < 64)
    {
        mNextMotion = eSligMotions::M_SpeakBullshit1_25_4B5450;
        return 129;
    }

    if (Math_NextRandom() < 64)
    {
        mNextMotion = eSligMotions::M_SpeakBullshit2_27_4B5490;
        return 129;
    }

    WaitOrWalk_4BE870();
    return 129;
}

s16 Slig::Brain_DiscussionWhat_25_4BF380()
{
    if (ListenToGlukkonCommands_4B95D0())
    {
        ToUnderGlukkonCommand_4B9660();
    }

    if (mCurrentMotion == eSligMotions::M_StandIdle_0_4B4EC0 && field_120_timer == static_cast<s32>(sGnFrame))
    {
        mNextMotion = static_cast<s16>(field_294_next_gamespeak_motion);
    }

    if (field_120_timer < static_cast<s32>(sGnFrame - 5))
    {
        WaitOrWalk_4BE870();
    }

    return 128;
}

s16 Slig::Brain_Empty_26_4BF620()
{
    
    return 0;
}

s16 Slig::Brain_Empty_27_4BF600()
{
    
    return 0;
}

s16 Slig::Brain_ZShooting_28_4BFA70()
{
    if (mCurrentMotion != eSligMotions::M_ShootZ_42_4B7560 || !field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ShouldStillBeAlive_4BBC00();
        return 127;
    }

    field_158_num_times_to_shoot++;

    if (field_158_num_times_to_shoot < field_218_tlv_data.field_22_num_times_to_shoot)
    {
        return 127;
    }

    field_158_num_times_to_shoot = 0;

    if (sActiveHero->mHealth <= FP_FromInteger(0))
    {
        ToAbeDead_4B3580();
    }
    else
    {
        if (sControlledCharacter_5C1B8C->mHealth > FP_FromInteger(0))
        {
            WaitOrWalk_4BE870();
        }
        else
        {
            ToKilledAbe_4B3600();
        }
    }
    return 127;
}

s16 Slig::Brain_Shooting_29_4BF750()
{
    if (mCurrentMotion == 6 && field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_158_num_times_to_shoot++;

        if (field_158_num_times_to_shoot < field_218_tlv_data.field_22_num_times_to_shoot)
        {
            mNextMotion = eSligMotions::M_Shoot_6_4B55A0;
            return 111;
        }

        if (sActiveHero->mHealth <= FP_FromInteger(0))
        {
            ToKilledAbe_4B3600();
            return 111;
        }

        if (sControlledCharacter_5C1B8C->mHealth <= FP_FromInteger(0))
        {
            ToKilledAbe_4B3600();
            return 111;
        }
        if (!VOnSameYLevel(sControlledCharacter_5C1B8C) || !VIsFacingMe(sControlledCharacter_5C1B8C) || IsInInvisibleZone(sControlledCharacter_5C1B8C) || sControlledCharacter_5C1B8C->mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit8_bInvisible) || IsWallBetween_4BB8B0(this, sControlledCharacter_5C1B8C) || !gMap.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0) || !gMap.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0) || Event_Get(kEventResetting))
        {
            PauseALittle_4BDD00();
            return 111;
        }

        if (!VIsFacingMe(sControlledCharacter_5C1B8C))
        {
            ToTurn_4BE090();
            return 111;
        }

        if (!gMap.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0) && field_218_tlv_data.field_2A_chase_abe_when_spotted == Choice_short::eYes_1)
        {
            ToChase_4BCFF0();
            return 111;
        }
    }

    ShouldStillBeAlive_4BBC00();
    return 111;
}

s16 Slig::Brain_ZSpottedEnemy_30_4BFA30()
{
    if (field_120_timer > static_cast<s32>(sGnFrame))
    {
        return 126;
    }
    mNextMotion = eSligMotions::M_ShootZ_42_4B7560;
    SetBrain(&Slig::Brain_ZShooting_28_4BFA70);
    return 126;
}

s16 Slig::Brain_WakingUp_31_4B9390()
{
    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        WaitOrWalk_4BE870();
    }
    return 0;
}

s16 Slig::Brain_Inactive_32_4B9430()
{
    if (ListenToGlukkonCommands_4B95D0())
    {
        ToUnderGlukkonCommand_4B9660();
    }
    else if (field_120_timer > static_cast<s32>(sGnFrame))
    {
        if (sActiveHero->mHealth <= FP_FromInteger(0))
        {
            ToAbeDead_4B3580();
        }
        else if (VOnSameYLevel(sControlledCharacter_5C1B8C) && VIsFacingMe(sControlledCharacter_5C1B8C) && VIsObjNearby(ScaleToGridSize(field_CC_sprite_scale) * FP_FromInteger(1), sControlledCharacter_5C1B8C) && !IsInInvisibleZone(sControlledCharacter_5C1B8C) && !sControlledCharacter_5C1B8C->mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit8_bInvisible) && !IsWallBetween_4BB8B0(this, sControlledCharacter_5C1B8C) && !Event_Get(kEventResetting) && sControlledCharacter_5C1B8C->Type() != AETypes::eGlukkon_67)
        {
            ToShoot_4BF9A0();
        }
        else
        {
            ShouldStillBeAlive_4BBC00();
        }
    }
    else
    {
        WaitOrWalk_4BE870();
    }
    return 103;
}

s16 Slig::Brain_Paused_33_4B8DD0()
{
    if (ListenToGlukkonCommands_4B95D0())
    {
        ToUnderGlukkonCommand_4B9660();
        return 101;
    }

    if (VOnSameYLevel(sControlledCharacter_5C1B8C) && VIsFacingMe(sControlledCharacter_5C1B8C) && !IsInInvisibleZone(sControlledCharacter_5C1B8C) && !sControlledCharacter_5C1B8C->mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit8_bInvisible) && !IsWallBetween_4BB8B0(this, sControlledCharacter_5C1B8C) && gMap.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0) && (!field_15E_spotted_possessed_slig || sControlledCharacter_5C1B8C->Type() != AETypes::eSlig_125) && !Event_Get(kEventResetting) && sControlledCharacter_5C1B8C->Type() != AETypes::eGlukkon_67)
    {
        RespondToEnemyOrPatrol_4B3140();
        return 101;
    }

    if (VOnSameYLevel(sControlledCharacter_5C1B8C) && VIsFacingMe(sControlledCharacter_5C1B8C) && !IsInInvisibleZone(sControlledCharacter_5C1B8C) && !sControlledCharacter_5C1B8C->mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit8_bInvisible) && !IsWallBetween_4BB8B0(this, sControlledCharacter_5C1B8C) && gMap.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0) && !Event_Get(kEventResetting) && sControlledCharacter_5C1B8C->Type() != AETypes::eGlukkon_67)
    {
        ToShoot_4BF9A0();
    }
    else if (sActiveHero->mHealth <= FP_FromInteger(0))
    {
        ToAbeDead_4B3580();
    }
    else
    {
        auto pNoiseOrSpeaking = static_cast<BaseAliveGameObject*>(Event_Get(kEventSuspiciousNoise));
        if (!pNoiseOrSpeaking)
        {
            pNoiseOrSpeaking = static_cast<BaseAliveGameObject*>(Event_Get(kEventSpeaking));
        }

        if (pNoiseOrSpeaking && pNoiseOrSpeaking->field_CC_sprite_scale == field_CC_sprite_scale && pNoiseOrSpeaking != this && field_120_timer <= static_cast<s32>(sGnFrame) && !Event_Get(kEventResetting))
        {
            ToTurn_4BE090();
        }
        else if (sControlledCharacter_5C1B8C->field_CC_sprite_scale > field_CC_sprite_scale && VIsFacingMe(sControlledCharacter_5C1B8C) && !IsInInvisibleZone(sControlledCharacter_5C1B8C) && !sControlledCharacter_5C1B8C->mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit8_bInvisible) && gMap.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0) && !InZCover_4BB7C0(sControlledCharacter_5C1B8C) && !InZCover_4BB7C0(this) && !Event_Get(kEventResetting))
        {
            ToZShoot_4BF9E0();
        }
        else
        {
            ShouldStillBeAlive_4BBC00();
        }
    }
    return 101;
}

s16 Slig::Brain_Sleeping_34_4B9170()
{
    auto pEvent = Event_Is_Event_In_Range(kEventSuspiciousNoise, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, field_D6_scale);
    if (pEvent)
    {
        auto pNoise = Event_Is_Event_In_Range(kEventNoise, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, field_D6_scale);
        if (pNoise)
        {
            pEvent = pNoise;
        }

        const FP wakeUpDistance = ScaleToGridSize(field_CC_sprite_scale) * FP_FromInteger(field_218_tlv_data.field_4A_noise_wake_up_distance);
        if (VIsObjNearby(wakeUpDistance, pEvent) && field_120_timer <= static_cast<s32>(sGnFrame) && gMap.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0) && !Event_Get(kEventResetting))
        {
            WakeUp_4B93B0();
            return 102;
        }
    }

    if (Event_Get(kEventSpeaking) || Event_Get(kEventLoudNoise))
    {
        if (pEvent != this && field_120_timer <= static_cast<s32>(sGnFrame) && gMap.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0) && !Event_Get(kEventResetting))
        {
            WakeUp_4B93B0();
            return 102;
        }
    }

    if (Event_Get(kEventAlarm) && !Event_Get(kEventResetting))
    {
        WakeUp_4B93B0();
        return 102;
    }

    ShouldStillBeAlive_4BBC00();

    if (mBaseGameObjectFlags.Get(BaseGameObject::eDead))
    {
        const CameraPos direction = gMap.GetDirection_4811A0(field_C2_lvl_number, field_C0_path_number, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos);
        Start_Slig_sounds(direction, 0);
    }
    return 102;
}

// A leftover function from Abe's Oddysee.
s16 Slig::Brain_ChaseAndDisappear_35_4BF640()
{
    if (Event_Get(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (field_11C_brain_sub_state == Brain_35_ChaseAndDisappear::eBrain35_Summoned_0)
    {
        if (sNum_CamSwappers_5C1B66 > 0 || sActiveHero->field_1AC_flags.Get(Abe::e1AC_Bit5_shrivel))
        {
            return field_11C_brain_sub_state;
        }

        mCurrentMotion = eSligMotions::M_StandIdle_0_4B4EC0;
        field_120_timer = sGnFrame + field_218_tlv_data.field_14_pause_time;
        return Brain_35_ChaseAndDisappear::eBrain35_Running_1;
    }
    else if (field_11C_brain_sub_state == Brain_35_ChaseAndDisappear::eBrain35_Running_1)
    {
        if (static_cast<s32>(sGnFrame) < field_120_timer)
        {
            return field_11C_brain_sub_state;
        }

        mNextMotion = eSligMotions::M_Running_4_4B6000;
        return Brain_35_ChaseAndDisappear::eBrain35_ReachedDestination_2;
    }
    else
    {
        if (field_11C_brain_sub_state == Brain_35_ChaseAndDisappear::eBrain35_ReachedDestination_2 
            && gMap.Is_Point_In_Current_Camera_4810D0(
                field_C2_lvl_number,
                field_C0_path_number,
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos,
                0))
        {
            return field_11C_brain_sub_state;
        }
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        mHealth = FP_FromInteger(0);
        return field_11C_brain_sub_state;
    }
}

void Slig::Init()
{
    field_10_resources_array.SetAt(10, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kBigflashResID, 1, 0));
    field_10_resources_array.SetAt(11, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kSligBlowResID, 1, 0));
    field_10_resources_array.SetAt(12, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kShellResID, 1, 0));
    field_10_resources_array.SetAt(2, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kSlgknbkResID, 1, 0));
    field_10_resources_array.SetAt(16, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kSquibSmokeResID, 1, 0));

    if (!(field_218_tlv_data.field_48_disable_resources & 0x80))
    {
        field_10_resources_array.SetAt(6, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kSlgknfdResID, 1, 0));
    }

    if (!(field_218_tlv_data.field_48_disable_resources & 0x100))
    {
        field_10_resources_array.SetAt(3, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kSlgedgeResID, 1, 0));
    }

    if (!(field_218_tlv_data.field_48_disable_resources & 1))
    {
        field_10_resources_array.SetAt(7, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kSlgleverResID, 1, 0));
    }

    if (!(field_218_tlv_data.field_48_disable_resources & 2))
    {
        field_10_resources_array.SetAt(8, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kSlgliftResID, 1, 0));
    }

    if (!(field_218_tlv_data.field_48_disable_resources & 0x200))
    {
        field_10_resources_array.SetAt(4, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kSlgsmashResID, 1, 0));
    }

    if (!(field_218_tlv_data.field_48_disable_resources & 0x400))
    {
        field_10_resources_array.SetAt(9, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kSlgbeatResID, 1, 0));
    }

    if (!(field_218_tlv_data.field_48_disable_resources & 4))
    {
        field_10_resources_array.SetAt(5, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kSlgzshotResID, 1, 0));
    }

    if (!(field_218_tlv_data.field_48_disable_resources & 0x40))
    {
        field_10_resources_array.SetAt(1, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kSlgsleepResID, 1, 0));
    }

    if (!(field_218_tlv_data.field_48_disable_resources & 8))
    {
        field_10_resources_array.SetAt(13, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kZflashResID, 1, 0));
    }

    if (!(field_218_tlv_data.field_48_disable_resources & 0x10))
    {
        field_10_resources_array.SetAt(15, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kAbeknokzResID, 1, 0));
    }

    field_15E_spotted_possessed_slig = 0;
    field_120_timer = field_218_tlv_data.field_14_pause_time + sGnFrame;

    switch (field_218_tlv_data.field_12_start_state)
    {
        case Path_Slig::StartState::Patrol_1:
            SetBrain(&Slig::Brain_Inactive_32_4B9430);
            break;

        case Path_Slig::StartState::Sleeping_2:
            if (field_218_tlv_data.field_1_tlv_state && field_218_tlv_data.field_46_stay_awake == Choice_short::eYes_1)
            {
                SetBrain(&Slig::Brain_Inactive_32_4B9430);
            }
            else
            {
                SetBrain(&Slig::Brain_Sleeping_34_4B9170);
                mCurrentMotion = eSligMotions::M_Sleeping_32_4B89A0;
                vUpdateAnim_4B1320();
            }
            break;

        case Path_Slig::StartState::Chase_3:
            SetBrain(&Slig::Brain_StartChasing_18_4BCEB0);
            field_120_timer = sGnFrame + field_218_tlv_data.field_36_time_to_wait_before_chase;
            break;

        case Path_Slig::StartState::ChaseAndDisappear_4:
            SetBrain(&Slig::Brain_ChaseAndDisappear_35_4BF640);
            field_120_timer = sGnFrame + field_218_tlv_data.field_14_pause_time;
            break;

        case Path_Slig::StartState::Unused_5:
            break;

        case Path_Slig::StartState::ListeningToGlukkon_6:
            for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
            {
                BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
                if (!pObj)
                {
                    break;
                }

                if (pObj->Type() == AETypes::eGlukkon_67)
                {
                    auto pGlukkon = static_cast<BaseAliveGameObject*>(pObj);
                    if (gMap.Is_Point_In_Current_Camera_4810D0(
                            pGlukkon->field_C2_lvl_number,
                            pGlukkon->field_C0_path_number,
                            pGlukkon->mBaseAnimatedWithPhysicsGameObject_XPos,
                            pGlukkon->mBaseAnimatedWithPhysicsGameObject_YPos,
                            0))
                    {
                        field_208_glukkon_obj_id = pGlukkon->field_8_object_id;
                        sSligsUnderControlCount_BAF7E8++;
                        field_216_flags.Set(Flags_216::eBit1_FollowGlukkon);
                        SetBrain(&Slig::Brain_ListeningToGlukkon_4_4B9D20);
                        field_11C_brain_sub_state = Brain_ListeningToGlukkon_States::IdleListening_1;
                        break;
                    }
                }
            }

            if (!field_208_glukkon_obj_id)
            {
                SetBrain(&Slig::Brain_Inactive_32_4B9430);
            }
            break;

        default:
            SetBrain(&Slig::Brain_Paused_33_4B8DD0);
            break;
    }

    if (field_218_tlv_data.field_2C_start_direction == XDirection_short::eLeft_0)
    {
        field_20_animation.mAnimFlags.Set(AnimFlags::eBit5_FlipX);
    }

    field_290_points_count = 0;

    field_268_points[field_290_points_count].field_0_x = FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos);
    field_268_points[field_290_points_count].field_2_y = FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos);
    field_290_points_count++;

    for (s16 yCam = -3; yCam < 4; yCam++)
    {
        for (s16 xCam = -3; xCam < 4; xCam++)
        {
            Path_TLV* pTlvIter = sPath_dword_BB47C0->Get_First_TLV_For_Offsetted_Camera(xCam, yCam);
            while (pTlvIter)
            {
                bool addPoint = false;
                if (pTlvIter->field_4_type == TlvTypes::SligBoundLeft_32)
                {
                    if (static_cast<Path_SligLeftBound*>(pTlvIter)->field_10_slig_bound_id == field_218_tlv_data.field_38_slig_bound_id)
                    {
                        field_138_zone_rect.x = pTlvIter->field_8_top_left.field_0_x;
                        addPoint = true;
                    }
                }
                else if (pTlvIter->field_4_type == TlvTypes::SligBoundRight_45)
                {
                    if (static_cast<Path_SligRightBound*>(pTlvIter)->field_10_slig_bound_id == field_218_tlv_data.field_38_slig_bound_id)
                    {
                        field_138_zone_rect.w = pTlvIter->field_8_top_left.field_0_x;
                        addPoint = true;
                    }
                }
                else if (pTlvIter->field_4_type == TlvTypes::SligPersist_46)
                {
                    if (static_cast<Path_SligPersist*>(pTlvIter)->field_10_slig_bound_id == field_218_tlv_data.field_38_slig_bound_id)
                    {
                        addPoint = true;
                    }
                }

                if (addPoint)
                {
                    if (field_290_points_count < ALIVE_COUNTOF(field_268_points))
                    {
                        field_268_points[field_290_points_count].field_0_x = pTlvIter->field_8_top_left.field_0_x;
                        field_268_points[field_290_points_count].field_2_y = pTlvIter->field_8_top_left.field_2_y;
                        field_290_points_count++;
                    }
                }

                pTlvIter = Path::Next_TLV(pTlvIter);
            }
        }
    }
}

Slig::~Slig()
{
    if (sControlledCharacter_5C1B8C == this)
    {
        sControlledCharacter_5C1B8C = sActiveHero;

        MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, this, 0, 0);

        if (gMap.mLevel != LevelIds::eMenu_0)
        {
            gMap.SetActiveCam(
                field_146_level,
                field_148_path,
                field_14A_camera,
                CameraSwapEffects::eInstantChange_0,
                0,
                0);
        }
    }

    Path_TLV* pTlv = sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam(field_118_tlvInfo);
    if (pTlv)
    {
        if (pTlv->field_4_type.mType != TlvTypes::SligGetPants_104 && pTlv->field_4_type.mType != TlvTypes::SligSpawner_37)
        {
            if (mHealth <= FP_FromInteger(0))
            {
                pTlv->field_0_flags.Clear(TLV_Flags::eBit1_Created);
                pTlv->field_0_flags.Set(TLV_Flags::eBit2_Destroyed);
            }
            else
            {
                pTlv->field_0_flags.Clear(TLV_Flags::eBit1_Created);
                pTlv->field_0_flags.Clear(TLV_Flags::eBit2_Destroyed);
            }
        }
    }
}

const FP dword_5473E8[8] = {
    FP_FromInteger(4),
    FP_FromInteger(4),
    FP_FromInteger(0),
    FP_FromInteger(-4),
    FP_FromInteger(-4),
    FP_FromInteger(-4),
    FP_FromInteger(0),
    FP_FromInteger(4)};

// TODO: Size 8 ??
const FP dword_547408[22] = {
    FP_FromInteger(0),
    FP_FromInteger(-4),
    FP_FromInteger(-4),
    FP_FromInteger(-4),
    FP_FromInteger(0),
    FP_FromInteger(4),
    FP_FromInteger(4),
    FP_FromInteger(4),
    FP_FromInteger(0),
    FP_FromInteger(0),
    FP_FromInteger(0),
    FP_FromInteger(0),
    FP_FromInteger(0),
    FP_FromInteger(0),
    FP_FromInteger(0),
    FP_FromInteger(0),
    FP_FromInteger(0),
    FP_FromInteger(0),
    FP_FromInteger(0),
    FP_FromInteger(0),
    FP_FromInteger(0),
    FP_FromInteger(0)};

void Slig::VUpdate()
{
    if (mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit9_RestoredFromQuickSave))
    {
        if (BaseAliveGameObjectCollisionLineType != -1)
        {
            sCollisions_DArray_5C1128->Raycast(
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(20),
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(20),
                &BaseAliveGameObjectCollisionLine,
                &mBaseAnimatedWithPhysicsGameObject_XPos,
                &mBaseAnimatedWithPhysicsGameObject_YPos,
                1 << BaseAliveGameObjectCollisionLineType);

            if (BaseAliveGameObjectCollisionLine)
            {
                if (BaseAliveGameObjectCollisionLine->field_8_type == eLineTypes::eUnknown_32 || BaseAliveGameObjectCollisionLine->field_8_type == eLineTypes::eUnknown_36)
                {
                    PSX_RECT bRect = {};
                    VGetBoundingRect(&bRect, 1);
                    VOnCollisionWith(
                        {bRect.x, static_cast<s16>(bRect.y + 5)},
                        {bRect.w, static_cast<s16>(bRect.h + 5)},
                        ObjList_5C1B78,
                        1,
                        (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection);
                }
            }
        }
        field_208_glukkon_obj_id = BaseGameObject::RefreshId(field_208_glukkon_obj_id);
        if (BrainIs(&Slig::Brain_ListeningToGlukkon_4_4B9D20))
        {
            sSligsUnderControlCount_BAF7E8++;
        }
        mBaseAliveGameObjectFlags.Clear(Flags_114::e114_Bit9_RestoredFromQuickSave);
        BaseAliveGameObjectCollisionLineType = 0;
    }

    if (!Input_IsChanting_45F260())
    {
        field_292_prevent_depossession &= ~1u;
    }

    if (sControlledCharacter_5C1B8C == this && mHealth > FP_FromInteger(0))
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::ePossessed_9, this, 0, 0);
    }

    if (sDDCheat_FlyingEnabled_5C2C08 && sControlledCharacter_5C1B8C == this)
    {
        BaseAliveGameObjectCollisionLine = nullptr;

        // TODO: InputCommand constants
        if (sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed & 0xF)
        {
            field_C4_velx = dword_5473E8[sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_4_dir >> 5];
            field_C8_vely = dword_547408[sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_4_dir >> 5];

            if (sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed & 0x10)
            {
                field_C4_velx += dword_5473E8[sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_4_dir >> 5];
                field_C4_velx += dword_5473E8[sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_4_dir >> 5];
                field_C8_vely += dword_547408[sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_4_dir >> 5];
            }

            mBaseAnimatedWithPhysicsGameObject_XPos += field_C4_velx;
            mBaseAnimatedWithPhysicsGameObject_YPos += field_C8_vely;

            // Keep in the map bounds
            PSX_Point mapBounds = {};
            gMap.Get_map_size(&mapBounds);

            if (mBaseAnimatedWithPhysicsGameObject_XPos < FP_FromInteger(0))
            {
                mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(0);
            }

            if (mBaseAnimatedWithPhysicsGameObject_XPos >= FP_FromInteger(mapBounds.field_0_x))
            {
                mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(mapBounds.field_0_x) - FP_FromInteger(1);
            }

            if (mBaseAnimatedWithPhysicsGameObject_YPos < FP_FromInteger(0))
            {
                mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(0);
            }

            if (mBaseAnimatedWithPhysicsGameObject_YPos >= FP_FromInteger(mapBounds.field_2_y))
            {
                mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(mapBounds.field_2_y) - FP_FromInteger(1);
            }
        }
        else
        {
            field_C4_velx = FP_FromInteger(0);
            field_C8_vely = FP_FromInteger(0);
        }

        SetActiveCameraDelayedFromDir();

        BaseAliveGameObjectLastLineYPos = mBaseAnimatedWithPhysicsGameObject_YPos;
    }
    else
    {
        const auto oldMotion = mCurrentMotion;
        field_11C_brain_sub_state = (this->*field_154_brain_state)();

        if (mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit1_bShot))
        {
            vShot_4B2EA0();
        }

        if (sDDCheat_ShowAI_Info_5C1BD8)
        {
            DDCheat::DebugStr("Slig %d %d %d %d\n", field_11C_brain_sub_state, field_120_timer, mCurrentMotion, mNextMotion);
        }

        const FP oldXPos = mBaseAnimatedWithPhysicsGameObject_XPos;
        const FP oldYPos = mBaseAnimatedWithPhysicsGameObject_YPos;

        (this->*sSlig_motion_table_5604A0[mCurrentMotion])();

        if (oldXPos != mBaseAnimatedWithPhysicsGameObject_XPos || oldYPos != mBaseAnimatedWithPhysicsGameObject_YPos)
        {
            BaseAliveGameObjectPathTLV = sPath_dword_BB47C0->TLV_Get_At_4DB290(
                nullptr,
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos,
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos);
            VOn_TLV_Collision(BaseAliveGameObjectPathTLV);
        }

        if (oldMotion != mCurrentMotion || mBaseAliveGameObjectFlags.Get(Flags_114::e114_MotionChanged_Bit2))
        {
            mBaseAliveGameObjectFlags.Clear(Flags_114::e114_MotionChanged_Bit2);
            vUpdateAnim_4B1320();
        }
        else if (field_124_return_to_previous_motion)
        {
            mCurrentMotion = mPreviousMotion;
            vUpdateAnim_4B1320();
            field_20_animation.SetFrame(mBaseAliveGameObjectLastAnimFrame);
            field_124_return_to_previous_motion = 0;
        }
    }
}

void Slig::VScreenChanged()
{
    if (gMap.mCurrentLevel != gMap.mLevel || gMap.mOverlayId != gMap.GetOverlayId() || (gMap.mCurrentPath != gMap.mPath && this != sControlledCharacter_5C1B8C))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void Slig::VPossessed()
{
    mBaseAliveGameObjectFlags.Set(Flags_114::e114_Bit4_bPossesed);
    field_292_prevent_depossession |= 1u;
    if (mNextMotion != eSligMotions::M_KnockbackToStand_35_4B6A30 && mNextMotion != eSligMotions::M_Knockback_34_4B68A0)
    {
        mNextMotion = eSligMotions::M_StandIdle_0_4B4EC0;
    }
    SetBrain(&Slig::Brain_Possessed_2_4BBCF0);
    field_11C_brain_sub_state = Brain_2_Possessed::eBrain2_StartPossession_0;

    field_146_level = gMap.mCurrentLevel;
    field_148_path = gMap.mCurrentPath;
    field_14A_camera = gMap.mCurrentCamera;

    MusicController::static_PlayMusic(MusicController::MusicTypes::ePossessed_9, this, 1, 0);
}

void Slig::VUnPosses()
{
    mNextMotion = eSligMotions::M_StandIdle_0_4B4EC0;
    field_120_timer = sGnFrame + 180;
    MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, this, 0, 0);
}

void Slig::VOn_TLV_Collision(Path_TLV* pTlv)
{
    while (pTlv)
    {
        if (pTlv->field_4_type == TlvTypes::DeathDrop_4)
        {
            if (mHealth > FP_FromInteger(0))
            {
                mHealth = FP_FromInteger(0);
                field_11C_brain_sub_state = 0;
                SetBrain(&Slig::Brain_DeathDropDeath_3_4BC1E0);
                field_C8_vely = FP_FromInteger(0);
                field_C4_velx = FP_FromInteger(0);
                VSetMotion(eSligMotions::M_Falling_7_4B42D0);
                Event_Broadcast(kEventMudokonComfort, this);
            }
        }
        pTlv = sPath_dword_BB47C0->TLV_Get_At_4DB290(pTlv, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos);
    }
}

void Slig::vShot_4B2EA0()
{
    if (field_136_shot_motion != -1)
    {
        mCurrentMotion = field_136_shot_motion;
    }

    mBaseAliveGameObjectFlags.Clear(Flags_114::e114_Bit1_bShot);
    mNextMotion = -1;
    field_136_shot_motion = -1;
    SetBrain(&Slig::Brain_Death_0_4BBFB0);
    field_120_timer = sGnFrame + 60;
}

void Slig::vUpdateAnim_4B1320()
{
    u8** ppRes = ResForMotion_4B1E90(mCurrentMotion);
    if (!ppRes)
    {
        mCurrentMotion = eSligMotions::M_StandIdle_0_4B4EC0;
        ppRes = ResForMotion_4B1E90(mCurrentMotion);
    }

    const AnimRecord& animRec = AnimRec(sSligFrameTables_547318[mCurrentMotion]);
    field_20_animation.Set_Animation_Data(animRec.mFrameTableOffset, ppRes);
}

Bool32 Slig::vUnderGlukkonCommand_4B1760()
{
    return BrainIs(&Slig::Brain_ListeningToGlukkon_4_4B9D20);
}

void Slig::WakeUp_4B93B0()
{
    mNextMotion = eSligMotions::M_SleepingToStand_33_4B8C50;
    SetBrain(&Slig::Brain_WakingUp_31_4B9390);

    MusicController::static_PlayMusic(MusicController::MusicTypes::eTension_4, this, 0, 0);
    Path_TLV* pTlv = sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
        field_218_tlv_data.field_8_top_left.field_0_x,
        field_218_tlv_data.field_8_top_left.field_2_y,
        field_218_tlv_data.field_8_top_left.field_0_x,
        field_218_tlv_data.field_8_top_left.field_2_y,
        TlvTypes::Slig_15);
    if (pTlv)
    {
        pTlv->field_1_tlv_state = 1; // TODO: Keep track of these, 1 must keep slig awake ??
    }
}

void Slig::ShouldStillBeAlive_4BBC00()
{
    if (!field_15C_force_alive_state)
    {
        // Check not falling and not in the current screen
        if (mCurrentMotion != eSligMotions::M_Falling_7_4B42D0
            && mCurrentMotion != eSligMotions::M_OutToFall_38_4B4570
            && !gMap.Is_Point_In_Current_Camera_4810D0(
                field_C2_lvl_number,
                field_C0_path_number,
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos,
                0))
        {
            if (field_290_points_count <= 0)
            {
                // No patrol points, die
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            }
            else
            {
                s32 i = 0;
                while (!gMap.Is_Point_In_Current_Camera_4810D0(
                    field_C2_lvl_number,
                    field_C0_path_number,
                    FP_FromInteger(field_268_points[i].field_0_x),
                    FP_FromInteger(field_268_points[i].field_2_y),
                    0))
                {
                    if (i >= field_290_points_count)
                    {
                        // No within any out our patrol points, die
                        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                        break;
                    }
                    i++;
                }
            }
        }
    }
}

u8** Slig::ResForMotion_4B1E90(s16 motion)
{
    s16 resIdx = 0;
    if (motion < eSligMotions::M_Sleeping_32_4B89A0)
    {
        resIdx = 0;
    }
    else if (motion < eSligMotions::M_Knockback_34_4B68A0)
    {
        resIdx = 1;
    }
    else if (motion < eSligMotions::M_OutToFall_38_4B4570)
    {
        resIdx = 2;
    }
    else if (motion < eSligMotions::M_LandingFatal_41_4B4680)
    {
        resIdx = 3;
    }
    else if (motion < eSligMotions::M_ShootZ_42_4B7560)
    {
        resIdx = 4;
    }
    else if (motion < eSligMotions::M_Smash_44_4B6B90)
    {
        resIdx = 5;
    }
    else if (motion < eSligMotions::M_PullLever_45_4B8950)
    {
        resIdx = 6;
    }
    else if (motion < eSligMotions::M_LiftGrip_46_4B3700)
    {
        resIdx = 7;
    }
    else if (motion < eSligMotions::M_Beat_51_4B6C00)
    {
        resIdx = eSligMotions::M_SlidingToStand_8_4B6520;
    }
    else
    {
        resIdx = motion >= 52 ? 0 : 9;
    }

    field_134_res_idx = resIdx;

    return field_10_resources_array.ItemAt(field_134_res_idx);
}

void Slig::ToTurn_4BE090()
{
    mNextMotion = eSligMotions::M_TurnAroundStanding_5_4B6390;
    SetBrain(&Slig::Brain_Turning_19_4BDDD0);
}

void Slig::RespondWithWhat_4BF400()
{
    field_120_timer = sGnFrame + 20;
    if (VIsFacingMe(sControlledCharacter_5C1B8C))
    {
        mNextMotion = eSligMotions::M_StandIdle_0_4B4EC0;
    }
    else
    {
        mNextMotion = eSligMotions::M_TurnAroundStanding_5_4B6390;
    }
    SetBrain(&Slig::Brain_DiscussionWhat_25_4BF380);
}

void Slig::ToShoot_4BF9A0()
{
    mNextMotion = eSligMotions::M_Shoot_6_4B55A0;
    SetBrain(&Slig::Brain_Shooting_29_4BF750);
    field_158_num_times_to_shoot = 0;
    field_15A_unused = 0;
    MusicController::static_PlayMusic(MusicController::MusicTypes::eSoftChase_8, this, 0, 0);
}

void Slig::ToZShoot_4BF9E0()
{
    mNextMotion = eSligMotions::M_StandIdle_0_4B4EC0;
    field_120_timer = sGnFrame + field_218_tlv_data.field_44_Z_shoot_delay;
    SetBrain(&Slig::Brain_ZSpottedEnemy_30_4BFA30);
    MusicController::static_PlayMusic(MusicController::MusicTypes::eSoftChase_8, this, 0, 0);
}

void Slig::PauseALittle_4BDD00()
{
    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        field_120_timer = field_218_tlv_data.field_16_pause_left_min + sGnFrame;
        if (field_218_tlv_data.field_18_pause_left_max > field_218_tlv_data.field_16_pause_left_min)
        {
            field_120_timer += Math_NextRandom() % (field_218_tlv_data.field_18_pause_left_max - field_218_tlv_data.field_16_pause_left_min);
        }
    }
    else
    {
        field_120_timer = field_218_tlv_data.field_1A_pause_right_min + sGnFrame;
        if (field_218_tlv_data.field_1C_pause_right_max > field_218_tlv_data.field_1A_pause_right_min)
        {
            field_120_timer += Math_NextRandom() % (field_218_tlv_data.field_1C_pause_right_max - field_218_tlv_data.field_1A_pause_right_min);
        }
    }
    mNextMotion = eSligMotions::M_StandIdle_0_4B4EC0;
    SetBrain(&Slig::Brain_Idle_15_4BD800);
}

void Slig::ToStand_4B4A20()
{
    field_130_falling_velx_scale_factor = FP_FromInteger(0);
    field_C4_velx = FP_FromInteger(0);
    field_C8_vely = FP_FromInteger(0);
    mCurrentMotion = eSligMotions::M_StandIdle_0_4B4EC0;
    field_128_input = 0;
    field_12C_timer = Math_RandomRange(0, 60) + sGnFrame + 120;
    MapFollowMe(TRUE);
}

void Slig::BlowToGibs_4B8020()
{
    ae_new<Gibs>(
        GibType::Slig_1,
        mBaseAnimatedWithPhysicsGameObject_XPos,
        mBaseAnimatedWithPhysicsGameObject_YPos,
        field_C4_velx,
        field_C8_vely,
        field_CC_sprite_scale,
        0);

    ae_new<Blood>(
        mBaseAnimatedWithPhysicsGameObject_XPos,
        mBaseAnimatedWithPhysicsGameObject_YPos - (FP_FromInteger(30) * field_CC_sprite_scale),
        FP_FromInteger(0),
        FP_FromInteger(0),
        field_CC_sprite_scale,
        20);

    New_Smoke_Particles(
        mBaseAnimatedWithPhysicsGameObject_XPos,
        mBaseAnimatedWithPhysicsGameObject_YPos - (FP_FromInteger(30) * field_CC_sprite_scale),
        field_CC_sprite_scale,
        3,
        128,
        128,
        128);

    if (field_CC_sprite_scale == FP_FromDouble(0.5))
    {
        SFX_Play_Mono(SoundEffect::KillEffect_64, 80);
        SFX_Play_Mono(SoundEffect::FallingItemHit_47, 60);
    }
    else
    {
        SFX_Play_Mono(SoundEffect::KillEffect_64, 127);
        SFX_Play_Mono(SoundEffect::FallingItemHit_47, 90);
    }

    field_20_animation.mAnimFlags.Clear(AnimFlags::eBit3_Render);

    mCurrentMotion = eSligMotions::M_StandIdle_0_4B4EC0;
    field_C8_vely = FP_FromInteger(0);
    field_C4_velx = FP_FromInteger(0);
    mHealth = FP_FromInteger(0);
    MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, this, 0, 0);
    field_120_timer = sGnFrame + 60;
    SetUpdateDelay(40);
    SetBrain(&Slig::Brain_Death_0_4BBFB0);
    Event_Broadcast(kEventMudokonComfort, this);
}

s16 Slig::MainMovement_4B4720()
{
    if (sControlledCharacter_5C1B8C == this && mHealth > FP_FromInteger(0))
    {
        return HandlePlayerControlled_4B7800();
    }

    if (mNextMotion == eSligMotions::M_TurnAroundStanding_5_4B6390)
    {
        mCurrentMotion = eSligMotions::M_TurnAroundStanding_5_4B6390;
        mNextMotion = -1;
        return 1;
    }

    if (mNextMotion != eSligMotions::M_Walking_2_4B5BC0)
    {
        if (mNextMotion == eSligMotions::M_Running_4_4B6000)
        {
            if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
            {
                field_C4_velx = -(ScaleToGridSize(field_CC_sprite_scale) / FP_FromInteger(4));
            }
            else
            {
                field_C4_velx = (ScaleToGridSize(field_CC_sprite_scale) / FP_FromInteger(4));
            }
            mCurrentMotion = eSligMotions::M_StandToRun_3_4B62F0;
            mNextMotion = -1;
            return 1;
        }

        if (mNextMotion == eSligMotions::M_Shoot_6_4B55A0 || mNextMotion == eSligMotions::M_ShootZ_42_4B7560)
        {
            mCurrentMotion = mNextMotion;
            mNextMotion = -1;
            field_12C_timer = sGnFrame + 60;
            field_130_falling_velx_scale_factor = FP_FromInteger(0);
            field_C4_velx = FP_FromInteger(0);
            field_C8_vely = FP_FromInteger(0);
        }
        else
        {
            if (mNextMotion < eSligMotions::M_SpeakHereBoy_20_4B5330 || mNextMotion > eSligMotions::M_Blurgh_31_4B5510)
            {
                if (mNextMotion == eSligMotions::M_ReloadGun_12_4B5530 || mNextMotion == eSligMotions::M_PullLever_45_4B8950 || mNextMotion == eSligMotions::M_SteppingToStand_14_4B8480 || mNextMotion == eSligMotions::M_LiftGrip_46_4B3700 || mNextMotion == eSligMotions::M_Beat_51_4B6C00)
                {
                    mCurrentMotion = mNextMotion;
                    mNextMotion = -1;
                    return 1;
                }
                else
                {
                    ToStand_4B4A20();
                    return 0;
                }
            }

            mCurrentMotion = GetNextMotionIncGameSpeak_4B5080(0);
            if (mCurrentMotion == -1)
            {
                ToStand_4B4A20();
                return 0;
            }

            if (!BrainIs(&Slig::Brain_ListeningToGlukkon_4_4B9D20))
            {
                Event_Broadcast(kEventSpeaking, this);
                return 1;
            }
        }
        return 1;
    }

    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        if (WallHit(field_CC_sprite_scale * FP_FromInteger(45), -ScaleToGridSize(field_CC_sprite_scale)))
        {
            return 0;
        }
        field_C4_velx = -(ScaleToGridSize(field_CC_sprite_scale) / FP_FromInteger(9));
    }
    else
    {
        if (WallHit(field_CC_sprite_scale * FP_FromInteger(45), ScaleToGridSize(field_CC_sprite_scale)))
        {
            return 0;
        }
        field_C4_velx = (ScaleToGridSize(field_CC_sprite_scale) / FP_FromInteger(9));
    }

    mCurrentMotion = eSligMotions::M_StandToWalk_1_4B5F70;
    mNextMotion = -1;

    return 1;
}

s16 Slig::LeftRigtMovement(MovementDirection direction)
{
    FP offX1 = {};
    FP offX2 = {};
    FP StandToRunVelX = {};
    FP StandingToStepVelX = {};
    FP SteppingToStandVelX = {};

    switch (direction)
    {
        case MovementDirection::eLeft:
            StandToRunVelX = -(ScaleToGridSize(field_CC_sprite_scale) / FP_FromInteger(4));
            StandingToStepVelX = -(ScaleToGridSize(field_CC_sprite_scale) / FP_FromInteger(9));
            SteppingToStandVelX = -(ScaleToGridSize(field_CC_sprite_scale) / FP_FromInteger(6));
            offX1 = -(ScaleToGridSize(field_CC_sprite_scale) * FP_FromInteger(2));
            offX2 = -(ScaleToGridSize(field_CC_sprite_scale));
            break;

        case MovementDirection::eRight:
            StandToRunVelX = (ScaleToGridSize(field_CC_sprite_scale) / FP_FromInteger(4));
            StandingToStepVelX = (ScaleToGridSize(field_CC_sprite_scale) / FP_FromInteger(9));
            SteppingToStandVelX = (ScaleToGridSize(field_CC_sprite_scale) / FP_FromInteger(6));
            offX1 = (ScaleToGridSize(field_CC_sprite_scale) * FP_FromInteger(2));
            offX2 = (ScaleToGridSize(field_CC_sprite_scale));
            break;

        default:
            break;
    }

    if ((direction == MovementDirection::eLeft && !field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX)) || (direction == MovementDirection::eRight && field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX)))
    {
        mCurrentMotion = eSligMotions::M_TurnAroundStanding_5_4B6390;
        return 1;
    }

    if (sInputObject_5BD4E0.isPressed(sInputKey_Run_5550E8))
    {
        field_C4_velx = StandToRunVelX;
        mCurrentMotion = eSligMotions::M_StandToRun_3_4B62F0;
        return 1;
    }

    if (!WallHit(field_CC_sprite_scale * FP_FromInteger(45), offX1))
    {
        field_C4_velx = StandingToStepVelX;
        mCurrentMotion = eSligMotions::M_StandingToStep_15_4B83B0;
        return 1;
    }

    if (WallHit(field_CC_sprite_scale * FP_FromInteger(45), offX2))
    {
        return 0;
    }

    field_C4_velx = SteppingToStandVelX;
    mCurrentMotion = eSligMotions::M_SteppingToStand_14_4B8480;
    return 1;
}

void Slig::PullLever()
{
    FP switchXPos = {};
    FP switchYPos = {};
    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        switchXPos = mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5);
        switchYPos = mBaseAnimatedWithPhysicsGameObject_XPos - ScaleToGridSize(field_CC_sprite_scale);
    }
    else
    {
        switchXPos = mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5);
        switchYPos = ScaleToGridSize(field_CC_sprite_scale) + mBaseAnimatedWithPhysicsGameObject_XPos;
    }

    auto pSwitch = static_cast<Lever*>(FindObjectOfType(AETypes::eLever_139, switchYPos, switchXPos));
    if (pSwitch && !mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit10_Teleporting))
    {
        pSwitch->VPull(mBaseAnimatedWithPhysicsGameObject_XPos < pSwitch->mBaseAnimatedWithPhysicsGameObject_XPos);
        mCurrentMotion = eSligMotions::M_PullLever_45_4B8950;
        return;
    }

    Slig_GameSpeak_SFX_4C04F0(SligSpeak::eWhat_9, 0, field_11E_pitch_min, this);
    mCurrentMotion = eSligMotions::M_SpeakWhat_29_4B54D0;
}

s16 Slig::ToShootZ()
{
    if (sInputObject_5BD4E0.isPressed(sInputKey_ThrowItem_5550F4) && field_CC_sprite_scale == FP_FromDouble(0.5) && !mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit10_Teleporting))
    {
        mCurrentMotion = eSligMotions::M_ShootZ_42_4B7560;
        mNextMotion = -1;
        field_12C_timer = sGnFrame + 60;
        return 1;
    }
    return 0;
}

void Slig::ShootOrShootZ()
{
    if (!sInputObject_5BD4E0.isPressed(sInputKey_Down_5550DC) || field_CC_sprite_scale != FP_FromDouble(0.5) || mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit10_Teleporting))
    {
        mCurrentMotion = eSligMotions::M_Shoot_6_4B55A0;
    }
    else
    {
        mCurrentMotion = eSligMotions::M_ShootZ_42_4B7560;
    }

    mNextMotion = -1;
    field_12C_timer = sGnFrame + 60;
}

s16 Slig::GrabNearbyLift()
{
    auto pLiftPoint = static_cast<LiftPoint*>(sObjectIds.Find_Impl(BaseAliveGameObjectId));
    if (pLiftPoint && pLiftPoint->Type() == AETypes::eLiftPoint_78)
    {
        const FP halfGrid = ScaleToGridSize(field_CC_sprite_scale) / FP_FromInteger(2);
        const FP midSwitchX = FP_FromInteger((BaseAliveGameObjectCollisionLine->field_0_rect.x + BaseAliveGameObjectCollisionLine->field_0_rect.w) / 2);
        const FP distToSwitchMid = (mBaseAnimatedWithPhysicsGameObject_XPos - midSwitchX) >= FP_FromInteger(0) ? mBaseAnimatedWithPhysicsGameObject_XPos - midSwitchX : midSwitchX - mBaseAnimatedWithPhysicsGameObject_XPos;
        if (distToSwitchMid < halfGrid)
        {
            mCurrentMotion = eSligMotions::M_LiftGrip_46_4B3700;
            return 1;
        }
    }
    return 0;
}

s16 Slig::HandlePlayerControlled_4B7800()
{
    if (sInputObject_5BD4E0.isPressed(sInputKey_Right_5550D0))
    {
        return LeftRigtMovement(MovementDirection::eRight);
    }

    if (sInputObject_5BD4E0.isPressed(sInputKey_Left_5550D4))
    {
        return LeftRigtMovement(MovementDirection::eLeft);
    }

    if (sInputObject_5BD4E0.isHeld(sInputKey_DoAction_5550E4))
    {
        PullLever();
        return 1;
    }

    if (sInputObject_5BD4E0.isPressed(sInputKey_ThrowItem_5550F4))
    {
        ShootOrShootZ();
        return 1;
    }

    if (sInputObject_5BD4E0.isPressed(sInputKey_FartRoll_5550F0))
    {
        mCurrentMotion = eSligMotions::M_Beat_51_4B6C00;
        field_12C_timer = sGnFrame + 60;
        return 1;
    }

    if (sInputObject_5BD4E0.isPressed(sInputKey_Down_5550DC))
    {
        const auto ShouldGrabLift = GrabNearbyLift();
        if (ShouldGrabLift)
        {
            return ShouldGrabLift;
        }

        const auto ShouldShootZ = ToShootZ();
        if (ShouldShootZ)
        {
            return ShouldShootZ;
        }
    }
    else if (sInputObject_5BD4E0.isPressed(sInputKey_Up_5550D8))
    {
        const auto GrabLift = GrabNearbyLift();
        if (GrabLift)
        {
            return GrabLift;
        }

        if (sInputObject_5BD4E0.isHeld(sInputKey_Up_5550D8))
        {
            Slig_GameSpeak_SFX_4C04F0(SligSpeak::eWhat_9, 0, field_11E_pitch_min, this);
            mCurrentMotion = eSligMotions::M_SpeakWhat_29_4B54D0;
            return 1;
        }
    }
    else if (sInputObject_5BD4E0.isHeld(sInputKey_FartRoll_5550F0 | sInputKey_Hop_5550E0))
    {
        Slig_GameSpeak_SFX_4C04F0(SligSpeak::eBlurgh_11, 0, field_11E_pitch_min, this);
        mCurrentMotion = eSligMotions::M_Blurgh_31_4B5510;
        return 1;
    }
    ToStand_4B4A20();
    return 0;
}

s16 Slig::GetNextMotionIncGameSpeak_4B5080(s32 input)
{
    if (sControlledCharacter_5C1B8C == this && mHealth > FP_FromInteger(0))
    {
        if (Input_IsChanting_45F260())
        {
            if (field_292_prevent_depossession & 1)
            {
                return -1;
            }
            field_12C_timer = sGnFrame + 30;
            SFX_Play_Mono(SoundEffect::PossessEffect_17, 0);
            return eSligMotions::M_Depossessing_36_4B7F30;
        }

        if (sInputKey_GameSpeak2_5550F8 & input)
        {
            mNextMotion = eSligMotions::M_SpeakHereBoy_20_4B5330;
        }
        else if (input & sInputKey_GameSpeak1_555104)
        {
            mNextMotion = eSligMotions::M_SpeakHi_21_4B53D0;
        }
        else if (input & sInputKey_GameSpeak3_555100)
        {
            if (sInputObject_5BD4E0.Is_Demo_Playing_45F220() != 0)
            {
                mNextMotion = eSligMotions::M_SpeakGetHim_23_4B5410;
            }
            else
            {
                mNextMotion = eSligMotions::M_SpeakFreeze_22_4B53F0;
            }
        }
        else if (input & sInputKey_GameSpeak4_5550FC)
        {
            if (sInputObject_5BD4E0.Is_Demo_Playing_45F220() != 0)
            {
                mNextMotion = eSligMotions::M_SpeakFreeze_22_4B53F0;
            }
            else
            {
                mNextMotion = eSligMotions::M_SpeakGetHim_23_4B5410;
            }
        }
        else if (input & sInputKey_GameSpeak8_555110)
        {
            mNextMotion = eSligMotions::M_SpeakLaugh_24_4B5430;
        }
        else if (input & sInputKey_GameSpeak6_555108)
        {
            mNextMotion = eSligMotions::M_SpeakBullshit1_25_4B5450;
        }
        else if (input & sInputKey_GameSpeak7_555114)
        {
            mNextMotion = eSligMotions::M_SpeakLookOut_26_4B5470;
        }
        else if (input & sInputKey_GameSpeak5_55510C)
        {
            mNextMotion = eSligMotions::M_SpeakBullshit2_27_4B5490;
        }
        else
        {
            return -1;
        }

        GameSpeakEvents event = GameSpeakEvents::eNone_m1;
        switch (mNextMotion)
        {
            case eSligMotions::M_SpeakHereBoy_20_4B5330:
                event = GameSpeakEvents::Slig_HereBoy_28;
                break;
            case eSligMotions::M_SpeakHi_21_4B53D0:
                event = GameSpeakEvents::Slig_Hi_27;
                break;
            case eSligMotions::M_SpeakFreeze_22_4B53F0:
                event = GameSpeakEvents::Slig_Freeze_31;
                break;
            case eSligMotions::M_SpeakGetHim_23_4B5410:
                event = GameSpeakEvents::Slig_GetEm_29;
                break;
            case eSligMotions::M_SpeakLaugh_24_4B5430:
                event = GameSpeakEvents::Slig_Laugh_8;
                break;
            case eSligMotions::M_SpeakBullshit1_25_4B5450:
                event = GameSpeakEvents::Slig_BS_5;
                break;
            case eSligMotions::M_SpeakLookOut_26_4B5470:
                event = GameSpeakEvents::Slig_LookOut_6;
                break;
            case eSligMotions::M_SpeakBullshit2_27_4B5490:
                event = GameSpeakEvents::Slig_BS2_7;
                break;
        }

        pEventSystem_5BC11C->PushEvent(event);
    }

    if (mNextMotion >= eSligMotions::M_SpeakHereBoy_20_4B5330 && mNextMotion <= eSligMotions::M_Blurgh_31_4B5510)
    {
        SligSpeak speak = SligSpeak::eNone;
        switch (mNextMotion)
        {
            case eSligMotions::M_SpeakHereBoy_20_4B5330:
                speak = SligSpeak::eHereBoy_1;
                break;
            case eSligMotions::M_SpeakHi_21_4B53D0:
                speak = SligSpeak::eHi_0;
                break;
            case eSligMotions::M_SpeakFreeze_22_4B53F0:
                speak = SligSpeak::eFreeze_8;
                break;
            case eSligMotions::M_SpeakGetHim_23_4B5410:
                speak = SligSpeak::eGetHim_2;
                break;
            case eSligMotions::M_SpeakLaugh_24_4B5430:
                speak = SligSpeak::eLaugh_3;
                break;
            case eSligMotions::M_SpeakBullshit1_25_4B5450:
                speak = SligSpeak::eBullshit_5;
                break;
            case eSligMotions::M_SpeakLookOut_26_4B5470:
                speak = SligSpeak::eLookOut_6;
                break;
            case eSligMotions::M_SpeakBullshit2_27_4B5490:
                speak = SligSpeak::eBullshit2_7;
                break;
            case eSligMotions::M_SpeakPanic_28_4B54B0:
                speak = SligSpeak::eHelp_10;
                break;
            case eSligMotions::M_SpeakWhat_29_4B54D0:
                speak = SligSpeak::eWhat_9;
                break;
            case eSligMotions::M_SpeakAIFreeze_30_4B54F0:
                speak = SligSpeak::eFreeze_8;
                break;
            case eSligMotions::M_Blurgh_31_4B5510:
                speak = SligSpeak::eBlurgh_11;
                break;
        }

        Slig_GameSpeak_SFX_4C04F0(speak, 0, field_11E_pitch_min, this);
        mCurrentMotion = mNextMotion;
        mNextMotion = -1;
        return mCurrentMotion;
    }

    return -1;
}

void Slig::WaitOrWalk_4BE870()
{
    mNextMotion = eSligMotions::M_Walking_2_4B5BC0;
    SetBrain(&Slig::Brain_Walking_21_4BE0C0);

    // Right rect bound
    if (!IsFacingEffectiveLeft_4BB780(this) && (ScaleToGridSize(field_CC_sprite_scale) * FP_FromInteger(2)) + mBaseAnimatedWithPhysicsGameObject_XPos >= FP_FromInteger(field_138_zone_rect.w))
    {
        PauseALittle_4BDD00();
        MusicController::static_PlayMusic(MusicController::MusicTypes::eTension_4, this, 0, 0);
        return;
    }

    // Left rect bound
    if (IsFacingEffectiveLeft_4BB780(this) && (mBaseAnimatedWithPhysicsGameObject_XPos - (ScaleToGridSize(field_CC_sprite_scale) * FP_FromInteger(2))) <= FP_FromInteger(field_138_zone_rect.x))
    {
        PauseALittle_4BDD00();
        MusicController::static_PlayMusic(MusicController::MusicTypes::eTension_4, this, 0, 0);
        return;
    }

    Brain_Walking_21_4BE0C0();
    MusicController::static_PlayMusic(MusicController::MusicTypes::eTension_4, this, 0, 0);
}

void Slig::ToAbeDead_4B3580()
{
    mNextMotion = eSligMotions::M_SpeakLaugh_24_4B5430;
    SetBrain(&Slig::Brain_EnemyDead_10_4B3460);
    field_120_timer = sGnFrame + 45;
}

void Slig::ToUnderGlukkonCommand_4B9660()
{
    SetBrain(&Slig::Brain_ListeningToGlukkon_4_4B9D20);
    field_11C_brain_sub_state = Brain_ListeningToGlukkon_States::GettingAttention_0;
}

void Slig::ToKilledAbe_4B3600()
{
    mNextMotion = eSligMotions::M_StandIdle_0_4B4EC0;
    SetBrain(&Slig::Brain_KilledEnemy_10_4B35C0);
    field_120_timer = sGnFrame + 15;
}

Bool32 Slig::IsWallBetween_4BB8B0(BaseAliveGameObject* pLeft, BaseAliveGameObject* pRight)
{
    PSX_RECT thisBRect = {};
    VGetBoundingRect(&thisBRect, 1);

    PSX_RECT rightBRect = {};
    pRight->VGetBoundingRect(&rightBRect, 1);

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    return sCollisions_DArray_5C1128->Raycast(
               pLeft->mBaseAnimatedWithPhysicsGameObject_XPos,
               FP_FromInteger(thisBRect.h - 25),
               pRight->mBaseAnimatedWithPhysicsGameObject_XPos,
               FP_FromInteger(rightBRect.h - 25),
               &pLine, &hitX, &hitY, pLeft->field_D6_scale != 0 ? 6 : 0x60)
        == 1;
}

GameSpeakEvents Slig::LastGlukkonSpeak_4B3090()
{
    if (!gMap.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 1))
    {
        return GameSpeakEvents::eNone_m1;
    }

    const s32 last_event_idx = pEventSystem_5BC11C->field_28_last_event_index;
    if (field_160_last_event_index == last_event_idx)
    {
        if (pEventSystem_5BC11C->field_20_last_event == GameSpeakEvents::eNone_m1)
        {
            return GameSpeakEvents::eNone_m1;
        }
        else
        {
            return GameSpeakEvents::eSameAsLast_m2;
        }
    }

    field_160_last_event_index = last_event_idx;

    return pEventSystem_5BC11C->field_20_last_event;
}

s16 Slig::ListenToGlukkonCommands_4B95D0()
{
    const GameSpeakEvents glukkonSpeak = LastGlukkonSpeak_4B3090();
    if (sControlledCharacter_5C1B8C->field_CC_sprite_scale != field_CC_sprite_scale)
    {
        return 0;
    }

    if (glukkonSpeak == GameSpeakEvents::Glukkon_Hey_36)
    {
        if (!HeardGlukkonToListenTo_4B9690(GameSpeakEvents::Glukkon_Hey_36))
        {
            return 0;
        }
    }
    else if (glukkonSpeak != GameSpeakEvents::Glukkon_AllOYa_40)
    {
        return 0;
    }

    sSligsUnderControlCount_BAF7E8++;

    field_208_glukkon_obj_id = sControlledCharacter_5C1B8C->field_8_object_id;

    if (glukkonSpeak == GameSpeakEvents::Glukkon_AllOYa_40)
    {
        field_216_flags.Set(Flags_216::eBit3_GlukkonCalledAllOYa);
    }

    return 1;
}

void Slig::PlatformCollide_4B4E00()
{
    PSX_Point xy;
    xy.field_0_x = FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos - FP_FromInteger(5));
    xy.field_2_y = FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5));

    PSX_Point wh;
    wh.field_0_x = FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos + FP_FromInteger(5));
    wh.field_2_y = FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(5));

    VOnCollisionWith(
        xy,
        wh,
        ObjList_5C1B78,
        1,
        (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection);
}

void Slig::FallKnockBackOrSmash_4B4A90()
{
    Environment_SFX_457A40(EnvironmentSfx::eKnockback_13, 0, 0x7FFF, this);

    if (field_C4_velx > FP_FromInteger(0))
    {
        mBaseAnimatedWithPhysicsGameObject_XPos -= field_C4_velx;
    }

    if (BaseAliveGameObjectCollisionLine)
    {
        if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
        {
            if (!WallHit((field_CC_sprite_scale * FP_FromInteger(45)), (ScaleToGridSize(field_CC_sprite_scale) * FP_FromInteger(2))))
            {
                field_C4_velx = (ScaleToGridSize(field_CC_sprite_scale) / FP_FromInteger(4));
            }
            else
            {
                field_C4_velx = FP_FromInteger(0);
            }
        }
        else
        {
            if (!WallHit((field_CC_sprite_scale * FP_FromInteger(45)), (-ScaleToGridSize(field_CC_sprite_scale) * FP_FromInteger(2))))
            {
                field_C4_velx = -(ScaleToGridSize(field_CC_sprite_scale) / FP_FromInteger(4));
            }
            else
            {
                field_C4_velx = FP_FromInteger(0);
            }
        }
    }

    if (mNextMotion == eSligMotions::M_Smash_44_4B6B90)
    {
        field_C4_velx = -field_C4_velx;
    }

    MapFollowMe(TRUE);

    if (field_C8_vely < FP_FromInteger(0))
    {
        field_C8_vely = FP_FromInteger(0);
    }

    mCurrentMotion = eSligMotions::M_Knockback_34_4B68A0;
    field_12C_timer = sGnFrame + 10;
}

void Slig::TurnOrSayWhat_4BEBC0()
{
    if (Event_Get(kEventSpeaking) && !IsInInvisibleZone(sControlledCharacter_5C1B8C) && !sControlledCharacter_5C1B8C->mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit8_bInvisible))
    {
        GameSpeakResponse_4BF470();
        RespondWithWhat_4BF400();
    }
    else
    {
        mNextMotion = eSligMotions::M_TurnAroundStanding_5_4B6390;
        SetBrain(&Slig::Brain_GetAlertedTurn_22_4BE990);
    }
}

void Slig::GameSpeakResponse_4BF470()
{
    const s32 lastIdx = pEventSystem_5BC11C->field_28_last_event_index;

    GameSpeakEvents speak = GameSpeakEvents::eNone_m1;

    if (field_160_last_event_index == lastIdx)
    {
        if (pEventSystem_5BC11C->field_20_last_event == GameSpeakEvents::eNone_m1)
        {
            speak = GameSpeakEvents::eNone_m1;
        }
        else
        {
            speak = GameSpeakEvents::eSameAsLast_m2;
        }
    }
    else
    {
        field_160_last_event_index = lastIdx;
        speak = pEventSystem_5BC11C->field_20_last_event;
    }

    switch (speak)
    {
        case GameSpeakEvents::eUnknown_1:
        case GameSpeakEvents::eUnknown_2:
            if (!(Math_NextRandom() & 4))
            {
                field_294_next_gamespeak_motion = eSligMotions::M_SpeakBullshit1_25_4B5450;
            }
            else
            {
                field_294_next_gamespeak_motion = eSligMotions::M_SpeakBullshit2_27_4B5490;
            }
            break;

        case GameSpeakEvents::eFart_3:
        case GameSpeakEvents::eUnknown_4:
        case GameSpeakEvents::Slig_Laugh_8:
        case GameSpeakEvents::eUnknown_14:
        case GameSpeakEvents::eUnknown_15:
        case GameSpeakEvents::Slig_GetEm_29:
        case GameSpeakEvents::eUnknown_34:
            field_294_next_gamespeak_motion = eSligMotions::M_Blurgh_31_4B5510;
            return;

        case GameSpeakEvents::Slig_BS_5:
            field_294_next_gamespeak_motion = eSligMotions::M_SpeakBullshit1_25_4B5450;
            break;

        case GameSpeakEvents::Slig_LookOut_6:
            if (!(Math_NextRandom() & 4))
            {
                field_294_next_gamespeak_motion = eSligMotions::M_SpeakWhat_29_4B54D0;
            }
            else
            {
                field_294_next_gamespeak_motion = eSligMotions::M_SpeakPanic_28_4B54B0;
            }
            return;

        case GameSpeakEvents::Slig_BS2_7:
            field_294_next_gamespeak_motion = eSligMotions::M_SpeakBullshit2_27_4B5490;
            break;

        case GameSpeakEvents::eHello_9:
        case GameSpeakEvents::Slig_Hi_27:
            field_294_next_gamespeak_motion = eSligMotions::M_SpeakHi_21_4B53D0;
            break;

        case GameSpeakEvents::eFollowMe_10:
        case GameSpeakEvents::eWait_12:
            if (Math_NextRandom() & 8)
            {
                field_294_next_gamespeak_motion = eSligMotions::M_SpeakBullshit1_25_4B5450;
            }
            else
            {
                field_294_next_gamespeak_motion = eSligMotions::M_SpeakBullshit2_27_4B5490;
            }
            break;

        case GameSpeakEvents::eAnger_11:
        case GameSpeakEvents::eUnknown_13:
        case GameSpeakEvents::eUnknown_16:
        case GameSpeakEvents::eUnknown_17:
        case GameSpeakEvents::eUnknown_18:
        case GameSpeakEvents::eUnknown_19:
        case GameSpeakEvents::eUnknown_20:
        case GameSpeakEvents::eUnknown_25:
        case GameSpeakEvents::eUnknown_26:
        case GameSpeakEvents::eUnknown_30:
        case GameSpeakEvents::Slig_Freeze_31:
        case GameSpeakEvents::eUnknown_32:
        case GameSpeakEvents::eUnknown_35:
            field_294_next_gamespeak_motion = eSligMotions::M_SpeakLaugh_24_4B5430;
            break;

        default:
            field_294_next_gamespeak_motion = eSligMotions::M_SpeakWhat_29_4B54D0;
            break;
    }
}

void Slig::GoAlertedOrSayWhat_4BF140()
{
    if (Event_Get(kEventSpeaking)
        && !IsInInvisibleZone(sControlledCharacter_5C1B8C)
        && !sControlledCharacter_5C1B8C->mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit8_bInvisible))
    {
        GameSpeakResponse_4BF470();
        RespondWithWhat_4BF400();
    }
    else
    {
        mNextMotion = eSligMotions::M_StandIdle_0_4B4EC0;
        field_294_next_gamespeak_motion = eSligMotions::M_SpeakWhat_29_4B54D0;
        SetBrain(&Slig::Brain_GetAlerted_23_4BEC40);
        field_120_timer = sGnFrame + field_218_tlv_data.field_3A_alerted_listen_time;
    }
}

s32 Slig::IsFacingEffectiveLeft_4BB780(Slig* pSlig)
{
    s32 flipX = pSlig->field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX);
    if (pSlig->mCurrentMotion == eSligMotions::M_TurnAroundStanding_5_4B6390 && pSlig->field_20_animation.field_92_current_frame > 4)
    {
        // Flip the result
        return flipX == 0;
    }
    else
    {
        return flipX;
    }
}

void Slig::MoveOnLine_4B4C40()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObjectId));

    const FP xpos = mBaseAnimatedWithPhysicsGameObject_XPos;
    if (BaseAliveGameObjectCollisionLine)
    {
        BaseAliveGameObjectCollisionLine = BaseAliveGameObjectCollisionLine->MoveOnLine(
            &mBaseAnimatedWithPhysicsGameObject_XPos,
            &mBaseAnimatedWithPhysicsGameObject_YPos,
            field_C4_velx);
        if (BaseAliveGameObjectCollisionLine)
        {
            if (pPlatform)
            {
                if (BaseAliveGameObjectCollisionLine->field_8_type != 32 && BaseAliveGameObjectCollisionLine->field_8_type != 36)
                {
                    const auto oldMotion = mCurrentMotion;
                    VOnTrapDoorOpen();
                    mCurrentMotion = oldMotion;
                }
            }
            else if (BaseAliveGameObjectCollisionLine->field_8_type == eLineTypes::eUnknown_32 || BaseAliveGameObjectCollisionLine->field_8_type == eLineTypes::eUnknown_36)
            {
                PlatformCollide_4B4E00();
            }
        }
        else
        {
            VOnTrapDoorOpen();
            mBaseAnimatedWithPhysicsGameObject_XPos = xpos + field_C4_velx;
            BaseAliveGameObjectLastLineYPos = mBaseAnimatedWithPhysicsGameObject_YPos;
            mCurrentMotion = eSligMotions::M_FallingInitiate_39_4B4640;
            field_130_falling_velx_scale_factor = FP_FromDouble(0.3);

            if (field_C4_velx > (FP_FromInteger(6) * field_CC_sprite_scale))
            {
                field_C4_velx = (FP_FromInteger(6) * field_CC_sprite_scale);
            }

            if (field_C4_velx < (-FP_FromInteger(6) * field_CC_sprite_scale))
            {
                field_C4_velx = (-FP_FromInteger(6) * field_CC_sprite_scale);
            }
        }
    }
    else
    {
        VOnTrapDoorOpen();
        mCurrentMotion = eSligMotions::M_OutToFall_38_4B4570;
        BaseAliveGameObjectLastLineYPos = mBaseAnimatedWithPhysicsGameObject_YPos;
    }
}

void Slig::PlayerControlStopWalkingIfRequired_4B8540()
{
    // Pressing other direction to facing or not pressing a direction at all?
    if ((field_C4_velx > FP_FromInteger(0) && sInputObject_5BD4E0.isPressed(sInputKey_Left_5550D4)) || (field_C4_velx < FP_FromInteger(0) && sInputObject_5BD4E0.isPressed(sInputKey_Right_5550D0)) || !sInputObject_5BD4E0.isPressed(sInputKey_Left_5550D4 | sInputKey_Right_5550D0))
    {
        // Then go to standing
        mCurrentMotion = eSligMotions::M_WalkToStand_18_4B5FC0;
    }
    field_128_input = 0;
}

void Slig::CheckPlatformVanished_4B3640()
{
    BaseGameObject* pLiftPoint = sObjectIds.Find_Impl(BaseAliveGameObjectId);
    if (pLiftPoint)
    {
        if (pLiftPoint->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
        {
            // Platform is somehow gone, fall.
            const auto savedMotion = mCurrentMotion;
            VOnTrapDoorOpen();
            mCurrentMotion = savedMotion;
        }
        SetActiveCameraDelayedFromDir();
    }
}

void Slig::VOnTrapDoorOpen()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObjectId));
    if (pPlatform)
    {
        pPlatform->VRemove(this);
        BaseAliveGameObjectId = -1;
        BaseAliveGameObjectLastLineYPos = mBaseAnimatedWithPhysicsGameObject_YPos;
        VSetMotion(eSligMotions::M_OutToFall_38_4B4570);
    }
}

s16 Slig::MoveLift_4B3990(FP ySpeed)
{
    auto pLiftPoint = static_cast<LiftPoint*>(sObjectIds.Find_Impl(BaseAliveGameObjectId));
    if (!pLiftPoint)
    {
        return eSligMotions::M_LiftGripping_48_4B3850;
    }

    pLiftPoint->vMove_4626A0(FP_FromInteger(0), ySpeed, 0);
    CheckPlatformVanished_4B3640();
    field_C8_vely = pLiftPoint->field_C8_vely;

    if (!field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame) && field_20_animation.field_92_current_frame != 5)
    {
        return mCurrentMotion;
    }

    if (ySpeed < FP_FromInteger(0))
    {
        if (pLiftPoint->vOnTopFloor())
        {
            return eSligMotions::M_LiftGripping_48_4B3850;
        }

        if (sControlledCharacter_5C1B8C != this)
        {
            return mCurrentMotion;
        }

        if (sInputObject_5BD4E0.isPressed(sInputKey_Up_5550D8))
        {
            return eSligMotions::M_LiftUp_49_4B3930;
        }

        if (sInputObject_5BD4E0.isPressed(sInputKey_Down_5550DC))
        {
            return eSligMotions::M_LiftDown_50_4B3960;
        }
    }
    else if (ySpeed > FP_FromInteger(0))
    {
        if (pLiftPoint->vOnBottomFloor())
        {
            return eSligMotions::M_LiftGripping_48_4B3850;
        }

        if (sControlledCharacter_5C1B8C != this)
        {
            return mCurrentMotion;
        }

        if (sInputObject_5BD4E0.isPressed(sInputKey_Down_5550DC))
        {
            return eSligMotions::M_LiftDown_50_4B3960;
        }

        if (sInputObject_5BD4E0.isPressed(sInputKey_Up_5550D8))
        {
            return eSligMotions::M_LiftUp_49_4B3930;
        }
    }

    // Strange how this isn't "if nothing pressed and on a floor then let go ??"
    if (sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed && pLiftPoint->vOnAnyFloor())
    {
        return eSligMotions::M_LiftUngrip_47_4B3820;
    }

    pLiftPoint->vMove_4626A0(FP_FromInteger(0), FP_FromInteger(0), 0);
    return eSligMotions::M_LiftGripping_48_4B3850;
}

void Slig::SlowDown_4B6450(FP speed)
{
    if (FP_GetExponent(field_C4_velx))
    {
        MoveOnLine_4B4C40();

        if (field_C4_velx <= FP_FromInteger(0))
        {
            if (field_C4_velx < FP_FromInteger(0))
            {
                field_C4_velx = (field_CC_sprite_scale * speed) + field_C4_velx;
                if (field_C4_velx > FP_FromInteger(0))
                {
                    field_C4_velx = FP_FromInteger(0);
                }
            }
        }
        else
        {
            field_C4_velx = field_C4_velx - (field_CC_sprite_scale * speed);
            if (field_C4_velx < FP_FromInteger(0))
            {
                field_C4_velx = FP_FromInteger(0);
            }
        }
    }
}

void Slig::ToChase_4BCFF0()
{
    field_120_timer = sGnFrame + field_218_tlv_data.field_36_time_to_wait_before_chase;

    if (!VIsFacingMe(sControlledCharacter_5C1B8C))
    {
        field_20_animation.mAnimFlags.Toggle(AnimFlags::eBit5_FlipX);
    }

    mNextMotion = eSligMotions::M_StandIdle_0_4B4EC0;
    SetBrain(&Slig::Brain_StartChasing_18_4BCEB0);
    MusicController::static_PlayMusic(MusicController::MusicTypes::eSoftChase_8, this, 0, 0);
}

s16 Slig::HandleEnemyStopper_4BBA00(s32 gridBlocks)
{
    s32 directedGirdBlocks = gridBlocks;
    const auto bFacingLeft = IsFacingEffectiveLeft_4BB780(this);
    if (bFacingLeft)
    {
        directedGirdBlocks = -gridBlocks;
    }

    const FP width = ScaleToGridSize(field_CC_sprite_scale) * FP_FromInteger(directedGirdBlocks) + mBaseAnimatedWithPhysicsGameObject_XPos;
    auto pTlv = static_cast<Path_EnemyStopper*>(sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
        FP_GetExponent(width),
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos - ScaleToGridSize(field_CC_sprite_scale)),
        TlvTypes::EnemyStopper_47));

    if (!pTlv)
    {
        return 0;
    }

    if (!SwitchStates_Get(pTlv->field_12_switch_id))
    {
        return 0;
    }

    if (pTlv->field_10_stop_direction == Path_EnemyStopper::StopDirection::Both_2)
    {
        return 1;
    }

    if (bFacingLeft && pTlv->field_10_stop_direction == Path_EnemyStopper::StopDirection::Left_0)
    {
        return 1;
    }

    if (!bFacingLeft && pTlv->field_10_stop_direction == Path_EnemyStopper::StopDirection::Right_1)
    {
        return 1;
    }

    return 0;
}

void Slig::ToPanicYelling_4BCBA0()
{
    mNextMotion = eSligMotions::M_SpeakPanic_28_4B54B0;
    SetBrain(&Slig::Brain_PanicYelling_14_4BCA70);
}

void Slig::ToPanicTurn_4BC750()
{
    mNextMotion = eSligMotions::M_TurnAroundStanding_5_4B6390;
    SetBrain(&Slig::Brain_PanicTurning_12_4BC490);
}

ALIVE_VAR(1, 0xBAF7EC, s16, sDelayIdx_BAF7EC, 0);
const s16 sDelayTable_56075C[6] = {0, 6, 10, 8, 4, 12};


s16 Slig::SligStableDelay_4B99B0()
{
    // Similar to the mudokon delay stuff

    if (sSligsUnderControlCount_BAF7E8 <= 1)
    {
        return 0;
    }

    if (sDelayIdx_BAF7EC >= ALIVE_COUNTOF(sDelayTable_56075C))
    {
        sDelayIdx_BAF7EC = 0;
    }

    return sDelayTable_56075C[sDelayIdx_BAF7EC++];
}

void Slig::PlayerControlRunningSlideStopOrTurnFrame12_4B8790()
{
    const FP curVelX = field_C4_velx;

    if (curVelX > FP_FromInteger(0) && sInputObject_5BD4E0.isPressed(sInputKey_Left_5550D4))
    {
        field_C4_velx = field_CC_sprite_scale * FP_FromDouble(13.2);
        mCurrentMotion = eSligMotions::M_SlidingTurn_9_4B6680;
        field_128_input = 0;
        return;
    }

    if (curVelX < FP_FromInteger(0) && sInputObject_5BD4E0.isPressed(sInputKey_Right_5550D0))
    {
        field_C4_velx = (field_CC_sprite_scale * -FP_FromDouble(13.2));
        mCurrentMotion = eSligMotions::M_SlidingTurn_9_4B6680;
        field_128_input = 0;
        return;
    }

    if (!sInputObject_5BD4E0.isPressed(sInputKey_Left_5550D4 | sInputKey_Right_5550D0))
    {
        if (curVelX >= FP_FromInteger(0))
        {
            field_C4_velx = field_CC_sprite_scale * FP_FromDouble(13.2);
        }
        else
        {
            field_C4_velx = (field_CC_sprite_scale * -FP_FromDouble(13.2));
        }
        mCurrentMotion = eSligMotions::M_SlidingToStand_8_4B6520;
        field_128_input = 0;
        return;
    }

    if (sInputObject_5BD4E0.isPressed(sInputKey_Run_5550E8))
    {
        field_128_input = 0;
        return;
    }

    mPreviousMotion = eSligMotions::M_Walking_2_4B5BC0;
    mBaseAliveGameObjectLastAnimFrame = 15;
    field_124_return_to_previous_motion = 1;

    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        field_128_input = 0;
        field_C4_velx = -(ScaleToGridSize(field_CC_sprite_scale) / FP_FromInteger(9));
    }
    else
    {
        field_C4_velx = (ScaleToGridSize(field_CC_sprite_scale) / FP_FromInteger(9));
        field_128_input = 0;
    }
}

void Slig::PlayerControlRunningSlideStopOrTurnFrame4_4B85D0()
{
    const FP curVelX = field_C4_velx;
    if (curVelX > FP_FromInteger(0) && sInputObject_5BD4E0.isPressed(sInputKey_Left_5550D4))
    {
        field_C4_velx = (field_CC_sprite_scale * FP_FromDouble(13.2));
        mCurrentMotion = eSligMotions::M_SlidingTurn_9_4B6680;
        field_128_input = 0;
        return;
    }

    if (curVelX < FP_FromInteger(0) && sInputObject_5BD4E0.isPressed(sInputKey_Right_5550D0))
    {
        field_C4_velx = (field_CC_sprite_scale * -FP_FromDouble(13.2));
        mCurrentMotion = eSligMotions::M_SlidingTurn_9_4B6680;
        field_128_input = 0;
        return;
    }

    if (!sInputObject_5BD4E0.isPressed(sInputKey_Left_5550D4 | sInputKey_Right_5550D0))
    {
        if (curVelX >= FP_FromInteger(0))
        {
            field_C4_velx = field_CC_sprite_scale * FP_FromDouble(13.2);
        }
        else
        {
            field_C4_velx = (field_CC_sprite_scale * -FP_FromDouble(13.2));
        }
        mCurrentMotion = eSligMotions::M_SlidingToStand_8_4B6520;
        field_128_input = 0;
        return;
    }

    if (sInputObject_5BD4E0.isPressed(sInputKey_Run_5550E8))
    {
        field_128_input = 0;
        return;
    }

    mPreviousMotion = eSligMotions::M_Walking_2_4B5BC0;
    mBaseAliveGameObjectLastAnimFrame = 6;
    field_124_return_to_previous_motion = 1;

    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        field_128_input = 0;
        field_C4_velx = -(ScaleToGridSize(field_CC_sprite_scale) / FP_FromInteger(9));
    }
    else
    {
        field_128_input = 0;
        field_C4_velx = (ScaleToGridSize(field_CC_sprite_scale) / FP_FromInteger(9));
    }
}

BaseAliveGameObject* Slig::FindBeatTarget_4BD070(s32 /*typeToFind*/, s32 gridBlocks)
{
    const FP kGridSize = ScaleToGridSize(field_CC_sprite_scale);
    const FP k2Scaled = FP_FromInteger(2) * kGridSize;
    const FP kGridBlocksScaled = FP_FromInteger(gridBlocks) * kGridSize;

    const FP xAndW = field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX) ? mBaseAnimatedWithPhysicsGameObject_XPos - kGridBlocksScaled : mBaseAnimatedWithPhysicsGameObject_XPos + kGridBlocksScaled;

    PSX_RECT hitRect = MakeMinMaxRect(
        xAndW,
        mBaseAnimatedWithPhysicsGameObject_YPos,
        xAndW,
        mBaseAnimatedWithPhysicsGameObject_YPos - k2Scaled,
        true);

    for (s32 i = 0; i < gBaseAliveGameObjects_5C1B7C->Size(); i++)
    {
        BaseAliveGameObject* pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj != this && pObj->Type() == AETypes::eMudokon_110)
        {
            PSX_RECT bRect = {};
            pObj->VGetBoundingRect(&bRect, 1);

            if (hitRect.w <= bRect.w && hitRect.x >= bRect.x && hitRect.y >= bRect.y && hitRect.h <= bRect.h && pObj->field_D6_scale == field_D6_scale && !IsInInvisibleZone(pObj) && !pObj->mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit8_bInvisible) && pObj->mHealth > FP_FromInteger(0))
            {
                return pObj;
            }
        }
    }
    return nullptr;
}

void Slig::TurnOrWalk_4BD6A0(s32 a2)
{
    if (a2 == 1)
    {
        if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
        {
            if (mBaseAnimatedWithPhysicsGameObject_XPos > FP_FromInteger(field_138_zone_rect.x) + (ScaleToGridSize(field_CC_sprite_scale) * FP_FromInteger(4)))
            {
                ToTurn_4BE090();
                return;
            }
        }
        else
        {
            if (mBaseAnimatedWithPhysicsGameObject_XPos < FP_FromInteger(field_138_zone_rect.w) - (ScaleToGridSize(field_CC_sprite_scale) * FP_FromInteger(4)))
            {
                ToTurn_4BE090();
                return;
            }
        }
    }

    if (!(field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX)))
    {
        if (mBaseAnimatedWithPhysicsGameObject_XPos > FP_FromInteger(field_138_zone_rect.w) - (ScaleToGridSize(field_CC_sprite_scale) * FP_FromInteger(4)))
        {
            ToTurn_4BE090();
            return;
        }
    }
    else
    {
        if (mBaseAnimatedWithPhysicsGameObject_XPos < FP_FromInteger(field_138_zone_rect.x) + (ScaleToGridSize(field_CC_sprite_scale) * FP_FromInteger(4)))
        {
            ToTurn_4BE090();
            return;
        }
    }

    WaitOrWalk_4BE870();
}

void Slig::ToPanicRunning_4BCA30()
{
    mNextMotion = eSligMotions::M_Running_4_4B6000;
    SetBrain(&Slig::Brain_PanicRunning_13_4BC780);
    Brain_PanicRunning_13_4BC780();
    MusicController::static_PlayMusic(MusicController::MusicTypes::eSoftChase_8, this, 0, 0);
}

void Slig::RespondToEnemyOrPatrol_4B3140()
{
    field_144_unused = 0;

    if (field_218_tlv_data.field_20_shoot_on_sight_delay || sControlledCharacter_5C1B8C->mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit8_bInvisible))
    {
        if (sControlledCharacter_5C1B8C->Type() != AETypes::eSlig_125 ||
            field_218_tlv_data.field_1E_shoot_possessed_sligs != Path_Slig::ShootPossessedSligs::eNo_3) // keeping it this way because OG levels might use a different value like 2 etc
        {
            SetBrain(&Slig::Brain_SpottedEnemy_7_4B3240);
            mNextMotion = eSligMotions::M_SpeakAIFreeze_30_4B54F0;
            field_120_timer = sGnFrame + field_218_tlv_data.field_20_shoot_on_sight_delay;
        }
        else
        {
            field_15E_spotted_possessed_slig = 1;
            TurnOrWalk_4BD6A0(0);
        }
    }
    else if (VIsFacingMe(sControlledCharacter_5C1B8C))
    {
        if (field_CC_sprite_scale == sControlledCharacter_5C1B8C->field_CC_sprite_scale)
        {
            ToShoot_4BF9A0();
        }
        else
        {
            ToZShoot_4BF9E0();
        }
    }
    else
    {
        ToTurn_4BE090();
    }
}

s16 Slig::IsAbeEnteringDoor_4BB990(BaseAliveGameObject* pThis)
{
    if (((pThis->Type() == AETypes::eAbe_69) && (pThis->mCurrentMotion == eAbeMotions::Motion_114_DoorEnter_459470 && pThis->field_20_animation.field_92_current_frame > 7)) || (pThis->mCurrentMotion == eAbeMotions::Motion_115_DoorExit_459A40 && pThis->field_20_animation.field_92_current_frame < 4))
    {
        return 1;
    }
    return 0;
}

s16 Slig::FindSwitch_4B9A50()
{
    const s16 yPos = FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5));
    if (sPath_dword_BB47C0->TLV_Get_At_4DB4B0(FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos), yPos, FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos), yPos, TlvTypes::Lever_17))
    {
        return 0;
    }

    FP xOff = ScaleToGridSize(field_CC_sprite_scale);
    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        xOff = -xOff;
    }

    return sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
               FP_GetExponent(FP_Abs(mBaseAnimatedWithPhysicsGameObject_XPos) + xOff),
               yPos,
               FP_GetExponent(FP_Abs(mBaseAnimatedWithPhysicsGameObject_XPos) + xOff),
               yPos,
               TlvTypes::Lever_17)
        != 0;
}

s16 Slig::NearOrFacingActiveChar_4B9930(BaseAliveGameObject* pObj)
{
    if (pObj->VIsObjNearby(ScaleToGridSize(field_CC_sprite_scale) * FP_FromDouble(0.5), sControlledCharacter_5C1B8C) || sControlledCharacter_5C1B8C->VIsFacingMe(pObj))
    {
        return 1;
    }

    return 0;
}

static s16 IsInZCover(Path_TLV* pTlv, PSX_RECT* pRect)
{
    if (pTlv->field_4_type == TlvTypes::ZSligCover_50)
    {
        if (pRect->x >= pTlv->field_8_top_left.field_0_x && pRect->x <= pTlv->field_C_bottom_right.field_0_x &&

            pRect->y >= pTlv->field_8_top_left.field_2_y && pRect->y <= pTlv->field_C_bottom_right.field_2_y &&

            pRect->w >= pTlv->field_8_top_left.field_0_x && pRect->w <= pTlv->field_C_bottom_right.field_0_x &&

            pRect->h >= pTlv->field_8_top_left.field_2_y && pRect->h <= pTlv->field_C_bottom_right.field_2_y)
        {
            return TRUE;
        }
    }
    return FALSE;
}

s16 Slig::InZCover_4BB7C0(BaseAliveGameObject* pObj)
{
    PSX_RECT bRect = {};
    pObj->VGetBoundingRect(&bRect, 1);

    return Bullet::InZBulletCover(pObj->mBaseAnimatedWithPhysicsGameObject_XPos, FP_FromInteger(bRect.y), bRect);
}

Bool32 Slig::RenderLayerIs_4BBBC0(BaseAliveGameObject* pThis)
{
    return pThis->field_20_animation.mRenderLayer == Layer::eLayer_BeforeWell_Half_3 || pThis->field_20_animation.mRenderLayer == Layer::eLayer_BeforeWell_22;
}

void Slig::NextCommand_4B9A00(s16 speakTableIndex, s16 responseState)
{
    field_212_next_command_arg1 = speakTableIndex;
    field_214_cmd_idx = 0;
    field_120_timer = sGnFrame + SligStableDelay_4B99B0();
    field_20C_state_after_speak = responseState;
}

s16 Slig::HeardGlukkonToListenTo_4B9690(GameSpeakEvents glukkonSpeak)
{
    const s32 distToPlayer = Math_Distance(
        FP_GetExponent(sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_XPos),
        FP_GetExponent(sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_YPos),
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos));

    if (!NearOrFacingActiveChar_4B9930(this))
    {
        return 0;
    }

    for (s32 i = 0; i < gBaseAliveGameObjects_5C1B7C->Size(); i++)
    {
        BaseAliveGameObject* pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj != this && pObj->Type() == AETypes::eSlig_125)
        {
            auto* pOtherSlig = static_cast<Slig*>(pObj);
            if (pOtherSlig->field_CC_sprite_scale == sControlledCharacter_5C1B8C->field_CC_sprite_scale && gMap.Is_Point_In_Current_Camera_4810D0(pOtherSlig->field_C2_lvl_number, pOtherSlig->field_C0_path_number, pOtherSlig->mBaseAnimatedWithPhysicsGameObject_XPos, pOtherSlig->mBaseAnimatedWithPhysicsGameObject_YPos, 0) && NearOrFacingActiveChar_4B9930(pOtherSlig) && (glukkonSpeak == GameSpeakEvents::Glukkon_Hey_36 || pOtherSlig->BrainIs(&Slig::Brain_ListeningToGlukkon_4_4B9D20)))
            {
                if (VIsObjNearby(ScaleToGridSize(field_CC_sprite_scale) * FP_FromDouble(0.5), pOtherSlig))
                {
                    if ((pOtherSlig->VIsFacingMe(sControlledCharacter_5C1B8C) && !VIsFacingMe(sControlledCharacter_5C1B8C)) || pOtherSlig->field_216_flags.Get(Flags_216::eBit4_HeardGlukkon))
                    {
                        return 0;
                    }
                }
                else if (distToPlayer > Math_Distance(
                             FP_GetExponent(sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_XPos),
                             FP_GetExponent(sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_YPos),
                             FP_GetExponent(pOtherSlig->mBaseAnimatedWithPhysicsGameObject_XPos),
                             FP_GetExponent(pOtherSlig->mBaseAnimatedWithPhysicsGameObject_YPos)))
                {
                    return 0;
                }
            }
        }
    }

    field_216_flags.Set(Flags_216::eBit4_HeardGlukkon);
    return 1;
}

s16 Slig::VTakeDamage(BaseGameObject* pFrom)
{
    if (mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit10_Teleporting))
    {
        return 0;
    }

    switch (pFrom->Type())
    {
        case AETypes::eBullet_15:
        {
            if (mHealth <= FP_FromInteger(0) && sControlledCharacter_5C1B8C != this)
            {
                return 0;
            }

            auto pBullet = static_cast<Bullet*>(pFrom);
            switch (pBullet->field_20_type)
            {
                case BulletType::eSligPossessedOrUnderGlukkonCommand_0:
                case BulletType::eNormalBullet_2:
                {
                    {
                        const FP yOff = FP_FromInteger(Math_NextRandom() % 16) - FP_FromInteger(8);
                        const FP xOff = ((pBullet->field_30_x_distance <= FP_FromInteger(0) ? FP_FromInteger(-1) : FP_FromInteger(1)) * FP_FromInteger(Math_NextRandom() & 15)) + FP_FromInteger(16);
                        const FP yPos = mBaseAnimatedWithPhysicsGameObject_YPos - (FP_FromInteger(25) * field_CC_sprite_scale);
                        const FP xPos = field_CC_sprite_scale * (pBullet->field_30_x_distance <= FP_FromInteger(0) ? FP_FromInteger(-6) : FP_FromInteger(6));
                        ae_new<Blood>(xPos + mBaseAnimatedWithPhysicsGameObject_XPos, yPos, xOff, yOff, field_CC_sprite_scale, 12);
                    }

                    {
                        const FP xOff = pBullet->field_30_x_distance <= FP_FromInteger(0) ? FP_FromInteger(-6) : FP_FromInteger(6);
                        const FP yPos = mBaseAnimatedWithPhysicsGameObject_YPos - (FP_FromInteger(25) * field_CC_sprite_scale);
                        const FP xPos = field_CC_sprite_scale * (pBullet->field_30_x_distance <= FP_FromInteger(0) ? FP_FromInteger(-12) : FP_FromInteger(12));
                        ae_new<Blood>(xPos + mBaseAnimatedWithPhysicsGameObject_XPos, yPos, xOff, FP_FromInteger(0), field_CC_sprite_scale, 8);
                    }
                    break;
                }

                case BulletType::ePossessedSligZBullet_1:
                case BulletType::eZBullet_3:
                {
                    PSX_RECT myRect = {};
                    VGetBoundingRect(&myRect, 1);
                    const FP rectY = FP_FromInteger(myRect.y);

                    Path_TLV* pTlvIter = nullptr;
                    for (;;)
                    {
                        pTlvIter = sPath_dword_BB47C0->TLV_Get_At_4DB290(pTlvIter, mBaseAnimatedWithPhysicsGameObject_XPos, rectY, mBaseAnimatedWithPhysicsGameObject_XPos, rectY);
                        if (!pTlvIter)
                        {
                            break;
                        }

                        if (IsInZCover(pTlvIter, &myRect))
                        {
                            mBaseAliveGameObjectFlags.Clear(Flags_114::e114_Bit1_bShot);
                            mHealth = FP_FromInteger(1);
                            return 0;
                        }
                    }

                    if (!gMap.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, mBaseAnimatedWithPhysicsGameObject_XPos, rectY, 0))
                    {
                        mBaseAliveGameObjectFlags.Clear(Flags_114::e114_Bit1_bShot);
                        mHealth = FP_FromInteger(1);
                        return 0;
                    }

                    ae_new<Blood>(mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos - (FP_FromInteger(25) * field_CC_sprite_scale), FP_FromInteger(0), FP_FromInteger(0), field_CC_sprite_scale, 25);
                }

                default:
                    break;
            }

            mBaseAliveGameObjectFlags.Set(Flags_114::e114_Bit1_bShot);
            SetBrain(&Slig::Brain_Death_0_4BBFB0);

            field_14C_death_by_being_shot_timer = sGnFrame + 5;
            if (mCurrentMotion != eSligMotions::M_Possess_37_4B72C0)
            {
                if (field_150_explode_timer > static_cast<s32>(sGnFrame) + 20)
                {
                    field_150_explode_timer = sGnFrame + 20;
                }

                if (mCurrentMotion != eSligMotions::M_Possess_37_4B72C0 && mCurrentMotion != eSligMotions::M_Smash_44_4B6B90 && mCurrentMotion != eSligMotions::M_Knockback_34_4B68A0)
                {
                    mNextMotion = eSligMotions::M_Possess_37_4B72C0;
                    field_136_shot_motion = eSligMotions::M_Possess_37_4B72C0;
                    field_150_explode_timer = sGnFrame + 20;
                    vShot_4B2EA0();
                    mBaseAliveGameObjectFlags.Set(Flags_114::e114_MotionChanged_Bit2);
                    if (static_cast<Bullet*>(pFrom)->field_30_x_distance < FP_FromInteger(0))
                    {
                        field_C4_velx = FP_FromDouble(-0.001);
                        mHealth = FP_FromInteger(0);
                        Event_Broadcast(kEventMudokonComfort, this);
                        return 1;
                    }
                    field_C4_velx = FP_FromDouble(0.001);
                }
            }

            mHealth = FP_FromInteger(0);
            Event_Broadcast(kEventMudokonComfort, this);
            return 1;
        }

        case AETypes::eDrill_30:
        case AETypes::eBaseBomb_46:
        case AETypes::eExplosion_109:
        {
            if (!field_20_animation.mAnimFlags.Get(AnimFlags::eBit3_Render))
            {
                return 1;
            }

            ae_new<Gibs>(GibType::Slig_1, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, field_C4_velx, field_C8_vely, field_CC_sprite_scale, 0);
            mHealth = FP_FromInteger(0);
            SFX_Play_Mono(SoundEffect::FallingItemHit_47, 90);
            field_20_animation.mAnimFlags.Clear(AnimFlags::eBit2_Animate);
            field_20_animation.mAnimFlags.Clear(AnimFlags::eBit3_Render);
            SetUpdateDelay(40);
            SetBrain(&Slig::Brain_ReturnControlToAbeAndDie_1_4BC410);
            mCurrentMotion = eSligMotions::M_StandIdle_0_4B4EC0;
            vUpdateAnim_4B1320();
            Event_Broadcast(kEventMudokonComfort, this);
            return 1;
        }

        case AETypes::eElectricWall_39:
            Slig_GameSpeak_SFX_4C04F0(SligSpeak::eHelp_10, 0, field_11E_pitch_min, this);
            return 1;

        case AETypes::eAbe_69:
        {
            if (mHealth <= FP_FromInteger(0))
            {
                return 1;
            }

            if (mCurrentMotion == eSligMotions::M_Knockback_34_4B68A0 || mCurrentMotion == eSligMotions::M_Smash_44_4B6B90 || mCurrentMotion == eSligMotions::M_KnockbackToStand_35_4B6A30)
            {
                return 1;
            }

            if (!VIsFacingMe(sActiveHero) || IsInInvisibleZone(sActiveHero) || sActiveHero->mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit8_bInvisible) || IsAbeEnteringDoor_4BB990(sControlledCharacter_5C1B8C))
            {
                GoAlertedOrSayWhat_4BF140();
            }
            else
            {
                RespondToEnemyOrPatrol_4B3140();
            }

            mBaseAliveGameObjectFlags.Set(Flags_114::e114_MotionChanged_Bit2);

            if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
            {
                mBaseAnimatedWithPhysicsGameObject_XPos = ScaleToGridSize(field_CC_sprite_scale) + mBaseAnimatedWithPhysicsGameObject_XPos;
            }
            else
            {
                mBaseAnimatedWithPhysicsGameObject_XPos = mBaseAnimatedWithPhysicsGameObject_XPos - ScaleToGridSize(field_CC_sprite_scale);
            }
            FallKnockBackOrSmash_4B4A90();
            field_C4_velx = FP_FromInteger(0);
            return 1;
        }

        case AETypes::eAbilityRing_104:
            return 1;

        case AETypes::eSlog_126:
        {
            if (mHealth <= FP_FromInteger(0) && (mCurrentMotion == eSligMotions::M_Knockback_34_4B68A0 || mCurrentMotion == eSligMotions::M_Smash_44_4B6B90))
            {
                return 1;
            }

            mBaseAliveGameObjectFlags.Set(Flags_114::e114_Bit1_bShot);
            mHealth = FP_FromInteger(0);

            SetBrain(&Slig::Brain_Death_0_4BBFB0);

            Environment_SFX_457A40(EnvironmentSfx::eKnockback_13, 0, 0x7FFF, this);

            Event_Broadcast(kEventMudokonComfort, this);

            if (VIsFacingMe(static_cast<BaseAliveGameObject*>(pFrom)))
            {
                if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
                {
                    field_C4_velx = (ScaleToGridSize(field_CC_sprite_scale) / FP_FromInteger(4));
                }
                else
                {
                    field_C4_velx = -(ScaleToGridSize(field_CC_sprite_scale) / FP_FromInteger(4));
                }

                mBaseAliveGameObjectFlags.Set(Flags_114::e114_MotionChanged_Bit2);
                field_12C_timer = sGnFrame + 10;
                mCurrentMotion = eSligMotions::M_Knockback_34_4B68A0;
                mNextMotion = eSligMotions::M_Knockback_34_4B68A0;
                field_136_shot_motion = eSligMotions::M_Knockback_34_4B68A0;
            }
            else
            {
                mNextMotion = eSligMotions::M_Smash_44_4B6B90;
                field_136_shot_motion = eSligMotions::M_Smash_44_4B6B90;
            }
            return 1;
        }

        case AETypes::eElectrocute_150:
            if (mHealth <= FP_FromInteger(0))
            {
                return 1;
            }
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            mHealth = FP_FromInteger(0);
            Event_Broadcast(kEventMudokonComfort, this);
            return 1;

        case AETypes::eNeverSet_8:
            break;

        default:
            if (mHealth <= FP_FromInteger(0))
            {
                return 1;
            }
            SFX_Play_Mono(SoundEffect::KillEffect_64, 127);
            SFX_Play_Mono(SoundEffect::FallingItemHit_47, 90);
            break;
    }

    if (mHealth <= FP_FromInteger(0))
    {
        return 1;
    }

    mBaseAliveGameObjectFlags.Set(Flags_114::e114_Bit1_bShot);
    mHealth = FP_FromInteger(0);
    mNextMotion = eSligMotions::M_Smash_44_4B6B90;
    field_136_shot_motion = eSligMotions::M_Smash_44_4B6B90;
    Event_Broadcast(kEventMudokonComfort, this);
    return 1;
}

s16 Slig::vIsFacingMe_4B23D0(BaseAnimatedWithPhysicsGameObject* pWho)
{
    if (mCurrentMotion != eSligMotions::M_TurnAroundStanding_5_4B6390 || field_20_animation.field_92_current_frame < 6)
    {
        if (pWho->mBaseAnimatedWithPhysicsGameObject_XPos <= mBaseAnimatedWithPhysicsGameObject_XPos && field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
        {
            return 1;
        }

        if (pWho->mBaseAnimatedWithPhysicsGameObject_XPos >= mBaseAnimatedWithPhysicsGameObject_XPos && !field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
        {
            return 1;
        }
    }
    else
    {
        if (pWho->mBaseAnimatedWithPhysicsGameObject_XPos <= mBaseAnimatedWithPhysicsGameObject_XPos && !field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
        {
            return 1;
        }

        if (pWho->mBaseAnimatedWithPhysicsGameObject_XPos >= mBaseAnimatedWithPhysicsGameObject_XPos && field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
        {
            return 1;
        }
    }
    return 0;
}

s16 Slig::vOnSameYLevel_4BB6C0(BaseAnimatedWithPhysicsGameObject* pOther)
{
    PSX_RECT bOurRect = {};
    VGetBoundingRect(&bOurRect, 1);

    PSX_RECT bTheirRect = {};
    pOther->VGetBoundingRect(&bTheirRect, 1);

    return ((bTheirRect.h + bTheirRect.y) / 2) <= bOurRect.h && bTheirRect.h >= bOurRect.y && field_D6_scale == pOther->field_D6_scale;
}

s16 Slig::FindLiftPoint_4B9B40()
{
    const FP k2Scaled = ScaleToGridSize(field_CC_sprite_scale) * FP_FromInteger(2);

    PlatformBase* pLift = nullptr;
    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        pLift = static_cast<PlatformBase*>(FindObjectOfType(AETypes::eLiftPoint_78, mBaseAnimatedWithPhysicsGameObject_XPos - k2Scaled, mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(5)));
    }
    else
    {
        pLift = static_cast<PlatformBase*>(FindObjectOfType(AETypes::eLiftPoint_78, k2Scaled + mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(5)));
    }

    if (pLift)
    {
        PSX_RECT rect = {};
        pLift->VGetBoundingRect(&rect, 1);

        const FP liftMidXPos = FP_FromInteger((rect.x + rect.w) / 2);
        return FP_Abs(mBaseAnimatedWithPhysicsGameObject_XPos - liftMidXPos) < k2Scaled;
    }

    return FALSE;
}
