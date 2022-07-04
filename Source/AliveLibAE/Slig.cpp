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
#include "../relive_lib/BaseGameObject.hpp"
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
#include "AnimationCallBacks.hpp"

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
            FP sndDistance = FP_FromInteger(abs(Math_Distance(0, 0, FP_GetExponent(pObj->mBaseAnimatedWithPhysicsGameObject_VelX), FP_GetExponent(pObj->mBaseAnimatedWithPhysicsGameObject_VelY))));
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

s32 Animation_OnFrame_Slig_4C0600(BaseGameObject* pObj, s16* pData)
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

    const FP xOff = (pSlig->mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(pPoints->x));
    const FP yOff = (pSlig->mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(pPoints->y));

    Bullet* pBullet = nullptr;

    FP bullet_xDist = FP_FromInteger(0);
    s8 fireDirection = 0;
    FP fireXpos = FP_FromInteger(0);
    s16 shellDirection = 0;
    if (pSlig->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
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

    pBullet = relive_new Bullet(pSlig, bulletType, pSlig->mBaseAnimatedWithPhysicsGameObject_XPos, yOff + pSlig->mBaseAnimatedWithPhysicsGameObject_YPos, bullet_xDist, 0, pSlig->mBaseAnimatedWithPhysicsGameObject_SpriteScale, 0);
    if (pBullet)
    {
        pBullet->SetUpdateDelay(1);
    }

    New_ShootingFire_Particle(fireXpos, yOff + pSlig->mBaseAnimatedWithPhysicsGameObject_YPos, fireDirection, pSlig->mBaseAnimatedWithPhysicsGameObject_SpriteScale);

    if (pSlig->mBaseAnimatedWithPhysicsGameObject_SpriteScale == FP_FromDouble(0.5))
    {
        SfxPlayMono(SoundEffect::SligShoot_5, 85);
    }
    else
    {
        relive_new BulletShell(pSlig->mBaseAnimatedWithPhysicsGameObject_XPos, yOff + pSlig->mBaseAnimatedWithPhysicsGameObject_YPos, shellDirection, pSlig->mBaseAnimatedWithPhysicsGameObject_SpriteScale);
        SfxPlayMono(SoundEffect::SligShoot_5, 0);
    }

    EventBroadcast(kEventShooting, pSlig);
    EventBroadcast(kEventLoudNoise, pSlig);

    Dove::All_FlyAway(0);
    return 1;
}

TintEntry sSligTint_560570[15] = {
    {EReliveLevelIds::eMines, 102u, 127u, 118u},
    {EReliveLevelIds::eNecrum, 102u, 127u, 118u},
    {EReliveLevelIds::eMudomoVault, 102u, 127u, 118u},
    {EReliveLevelIds::eMudancheeVault, 102u, 127u, 118u},
    {EReliveLevelIds::eFeeCoDepot, 102u, 127u, 118u},
    {EReliveLevelIds::eBarracks, 102u, 127u, 118u},
    {EReliveLevelIds::eMudancheeVault_Ender, 102u, 127u, 118u},
    {EReliveLevelIds::eBonewerkz, 102u, 127u, 118u},
    {EReliveLevelIds::eBrewery, 102u, 127u, 118u},
    {EReliveLevelIds::eBrewery_Ender, 102u, 127u, 118u},
    {EReliveLevelIds::eMudomoVault_Ender, 102u, 127u, 118u},
    {EReliveLevelIds::eFeeCoDepot_Ender, 102u, 127u, 118u},
    {EReliveLevelIds::eBarracks_Ender, 102u, 127u, 118u},
    {EReliveLevelIds::eBonewerkz_Ender, 102u, 127u, 118u},
    {EReliveLevelIds::eNone, 102u, 127u, 118u}};

ALIVE_VAR(1, 0xBAF7E4, s32, unused_BAF7E4, 0);
ALIVE_VAR(1, 0xBAF7E8, s16, sSligsUnderControlCount_BAF7E8, 0);

const AnimId sSligAnimIdTable[52] = {
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

const TSligBrainFn sSligBrainTable[36] = {
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

void Slig::SetBrain(TSligBrainFn fn)
{
    field_154_brain_state = fn;
}

bool Slig::BrainIs(TSligBrainFn fn)
{
    return field_154_brain_state == fn;
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

    field_10_resources_array.SetAt(0, ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kSlgbasicResID, 1, 0));

    const AnimRecord& rec = AnimRec(AnimId::Slig_Idle);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(AnimId::Slig_Idle, ppRes);

    SetType(ReliveTypes::eSlig);

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

    BaseAliveGameObject_PlatformId = -1;

    mCurrentMotion = eSligMotions::M_Falling_7_4B42D0;

    field_124_return_to_previous_motion = 0;
    mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(pTlv->mTopLeft.x);
    mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(pTlv->mTopLeft.y);
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

    mBaseAnimatedWithPhysicsGameObject_Anim.mFnPtrArray = kSlig_Anim_Frame_Fns_55EFAC;

    if (pTlv->field_10_scale != Scale_short::eFull_0)
    {
        if (pTlv->field_10_scale == Scale_short::eHalf_1)
        {
            mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromDouble(0.5);
            mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_SligGreeterFartsBat_Half_14;
            mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Bg;
        }
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromInteger(1);
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_SligGreeterFartsBats_33;
        mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Fg;
    }

    SetBaseAnimPaletteTint(&sSligTint_560570[0], gMap.mCurrentLevel, 412);

    FP hitX = {};
    FP hitY = {};
    if (sCollisions->Raycast(
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(24),
            &BaseAliveGameObjectCollisionLine,
            &hitX,
            &hitY,
            mBaseAnimatedWithPhysicsGameObject_Scale == Scale::Fg ? kFgFloor : kBgFloor)
        == 1)
    {
        mCurrentMotion = eSligMotions::M_StandIdle_0_4B4EC0;
        mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
    }

    MapFollowMe(TRUE);

    Init();

    VStackOnObjectsOfType(ReliveTypes::eSlig);

    if (gMap.mCurrentLevel == EReliveLevelIds::eBonewerkz && gMap.mCurrentPath == 2 && gMap.mCurrentCamera == 5)
    {
        mBaseAnimatedWithPhysicsGameObject_XOffset = 0;
    }

    mVisualFlags.Set(VisualFlags::eDoPurpleLightEffect);
    field_142_unused = 0;
    field_140_unused = 0;

    mShadow = relive_new Shadow();
}

void renderWithGlowingEyes(PrimHeader** ot, BaseAliveGameObject* actor, s16* pPalAlloc, s16 palSize, PSX_RECT* palRect,
                           s16& r, s16& g, s16& b,
                           const s16* eyeColourIndices, s16 eyeColourIndicesSize)
{
    if (actor->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit3_Render))
    {
        if (gMap.mCurrentPath == actor->mBaseAnimatedWithPhysicsGameObject_PathNumber && gMap.mCurrentLevel == actor->mBaseAnimatedWithPhysicsGameObject_LvlNumber && actor->Is_In_Current_Camera() == CameraPos::eCamCurrent_0)
        {
            actor->mBaseAnimatedWithPhysicsGameObject_Anim.field_14_scale = actor->mBaseAnimatedWithPhysicsGameObject_SpriteScale;

            const PSX_RECT boundingRect = actor->VGetBoundingRect();
            s16 rMod = actor->mBaseAnimatedWithPhysicsGameObject_RGB.r;
            s16 gMod = actor->mBaseAnimatedWithPhysicsGameObject_RGB.g;
            s16 bMod = actor->mBaseAnimatedWithPhysicsGameObject_RGB.b;
            ShadowZone::ShadowZones_Calculate_Colour(
                FP_GetExponent(actor->mBaseAnimatedWithPhysicsGameObject_XPos),
                (boundingRect.h + boundingRect.y) / 2,
                actor->mBaseAnimatedWithPhysicsGameObject_Scale,
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

                    const FrameInfoHeader* pFrameInfoHeader = actor->mBaseAnimatedWithPhysicsGameObject_Anim.Get_FrameHeader(0);
                    const u8* pAnimData = *actor->mBaseAnimatedWithPhysicsGameObject_Anim.field_20_ppBlock;
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
                    Pal_Set(
                        actor->mBaseAnimatedWithPhysicsGameObject_Anim.mPalVramXY,
                        actor->mBaseAnimatedWithPhysicsGameObject_Anim.mPalDepth,
                        reinterpret_cast<const u8*>(pPalAlloc),
                        palRect);
                }
                actor->mBaseAnimatedWithPhysicsGameObject_Anim.mRed = 127;
                actor->mBaseAnimatedWithPhysicsGameObject_Anim.mGreen = 127;
                actor->mBaseAnimatedWithPhysicsGameObject_Anim.mBlue = 127;
            }

            actor->mBaseAnimatedWithPhysicsGameObject_Anim.VRender(
                FP_GetExponent(FP_FromInteger(actor->mBaseAnimatedWithPhysicsGameObject_XOffset) + actor->mBaseAnimatedWithPhysicsGameObject_XPos - pScreenManager->CamXPos()),
                FP_GetExponent(FP_FromInteger(actor->mBaseAnimatedWithPhysicsGameObject_YOffset) + actor->mBaseAnimatedWithPhysicsGameObject_YPos - pScreenManager->CamYPos()),
                ot,
                0,
                0);

            PSX_RECT rectToInvalidate = {};
            actor->mBaseAnimatedWithPhysicsGameObject_Anim.Get_Frame_Rect(&rectToInvalidate);
            pScreenManager->InvalidateRectCurrentIdx(
                rectToInvalidate.x,
                rectToInvalidate.y,
                rectToInvalidate.w,
                rectToInvalidate.h);

            if (actor->mShadow)
            {
                actor->mShadow->Calculate_Position(actor->mBaseAnimatedWithPhysicsGameObject_XPos, actor->mBaseAnimatedWithPhysicsGameObject_YPos, &rectToInvalidate, actor->mBaseAnimatedWithPhysicsGameObject_SpriteScale, actor->mBaseAnimatedWithPhysicsGameObject_Scale);
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

    pState->field_C_velx = mBaseAnimatedWithPhysicsGameObject_VelX;
    pState->field_10_vely = mBaseAnimatedWithPhysicsGameObject_VelY;

    pState->field_58_falling_velx_scale_factor = field_130_falling_velx_scale_factor;

    pState->field_14_path_number = mBaseAnimatedWithPhysicsGameObject_PathNumber;
    pState->field_16_lvl_number = MapWrapper::ToAE(mBaseAnimatedWithPhysicsGameObject_LvlNumber);
    pState->field_18_sprite_scale = mBaseAnimatedWithPhysicsGameObject_SpriteScale;

    pState->field_1C_scale = mBaseAnimatedWithPhysicsGameObject_Scale;

    pState->field_1E_r = mBaseAnimatedWithPhysicsGameObject_RGB.r;
    pState->field_20_g = mBaseAnimatedWithPhysicsGameObject_RGB.g;
    pState->field_22_b = mBaseAnimatedWithPhysicsGameObject_RGB.b;

    if (mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit11_Electrocuting))
    {
        for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
        {
            BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
            if (!pObj)
            {
                break;
            }

            if (pObj->Type() == ReliveTypes::eElectrocute)
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
    pState->field_24_bFlipX = mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX);
    pState->field_26_current_motion = mCurrentMotion;
    pState->field_28_current_frame = mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame;
    pState->field_2A_frame_change_counter = mBaseAnimatedWithPhysicsGameObject_Anim.mFrameChangeCounter;
    pState->field_2D_bDrawable = mBaseGameObjectFlags.Get(BaseGameObject::eDrawable_Bit4);
    pState->field_2C_bRender = mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit3_Render);
    pState->field_30_health = mHealth;
    pState->field_34_current_motion = mCurrentMotion;
    pState->field_36_next_motion = mNextMotion;
    pState->field_38_last_line_ypos = FP_GetExponent(BaseAliveGameObjectLastLineYPos);
    pState->field_3A_collision_line_type = -1;

    if (BaseAliveGameObjectCollisionLine)
    {
        pState->field_3A_collision_line_type = BaseAliveGameObjectCollisionLine->mLineType;
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
    pState->field_72_return_level = MapWrapper::ToAE(field_146_return_level);
    pState->field_74_return_path = field_148_return_path;
    pState->field_76_return_camera = field_14A_return_camera;
    pState->field_78_death_by_being_shot_timer = field_14C_death_by_being_shot_timer;
    pState->field_7C_explode_timer = field_150_explode_timer;
    pState->field_80_brain_state_idx = 0;

    s32 idx = 0;
    for (const auto& fn : sSligBrainTable)
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

    if (!(disabledResources & 1) && !ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kSlgleverResID, 0, 0))
    {
        ResourceManager::LoadResourceFile_49C170("SLGLEVER.BAN", nullptr);
    }

    if (!(disabledResources & 2) && !ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kSlgliftResID, 0, 0))
    {
        ResourceManager::LoadResourceFile_49C170("SLGLIFT.BAN", nullptr);
    }

    if (!(disabledResources & 0x40) && !ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kSlgsleepResID, 0, 0))
    {
        ResourceManager::LoadResourceFile_49C170("SLGSLEEP.BAN", nullptr);
    }

    if ((disabledResources & 0x80u) == 0 && !ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kSlgknfdResID, 0, 0))
    {
        ResourceManager::LoadResourceFile_49C170("SLGKNFD.BAN", nullptr);
    }

    if (!(disabledResources & 0x100) && !ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kSlgedgeResID, 0, 0))
    {
        ResourceManager::LoadResourceFile_49C170("SLGEDGE.BAN", nullptr);
    }

    if (!(disabledResources & 0x200) && !ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kSlgsmashResID, 0, 0))
    {
        ResourceManager::LoadResourceFile_49C170("SLGSMASH.BAN", nullptr);
    }

    if (!(disabledResources & 0x400) && !ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kSlgbeatResID, 0, 0))
    {
        ResourceManager::LoadResourceFile_49C170("SLGBEAT.BAN", nullptr);
    }

    if (!(disabledResources & 4) && !ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kSlgzshotResID, 0, 0))
    {
        ResourceManager::LoadResourceFile_49C170("SLIGZ.BND", nullptr);
    }

    if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kSlgbasicResID, 0, 0))
    {
        ResourceManager::LoadResourceFile_49C170("SLIG.BND", nullptr);
    }

    if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kSligBlowResID, 0, 0))
    {
        ResourceManager::LoadResourceFile_49C170("SLGBLOW.BAN", nullptr);
    }

    if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kObjectShadowResID, 0, 0))
    {
        ResourceManager::LoadResourceFile_49C170("SHADOW.BAN", nullptr);
    }

    auto pSlig = relive_new Slig(pTlv, pState->field_5C_tlvInfo);
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
        pSlig->mBaseAnimatedWithPhysicsGameObject_VelX = pState->field_C_velx;
        pSlig->mBaseAnimatedWithPhysicsGameObject_VelY = pState->field_10_vely;
        pSlig->mBaseAnimatedWithPhysicsGameObject_PathNumber = pState->field_14_path_number;
        pSlig->mBaseAnimatedWithPhysicsGameObject_LvlNumber = MapWrapper::FromAE(pState->field_16_lvl_number);
        pSlig->mBaseAnimatedWithPhysicsGameObject_SpriteScale = pState->field_18_sprite_scale;

        if (pSlig->mBaseAnimatedWithPhysicsGameObject_SpriteScale == FP_FromInteger(1))
        {
            pSlig->mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_SligGreeterFartsBats_33;
        }
        else
        {
            pSlig->mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_SligGreeterFartsBat_Half_14;
        }

        pSlig->mBaseAnimatedWithPhysicsGameObject_Scale = pState->field_1C_scale;

        pSlig->field_200_red = pState->field_1E_r;
        pSlig->field_202_green = pState->field_20_g;
        pSlig->field_204_blue = pState->field_22_b;

        pSlig->mBaseAnimatedWithPhysicsGameObject_RGB.SetRGB(pState->field_1E_r, pState->field_20_g, pState->field_22_b);

        pSlig->mCurrentMotion = pState->field_26_current_motion;
        u8** ppRes = pSlig->ResForMotion_4B1E90(pSlig->mCurrentMotion);
        pSlig->mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(sSligAnimIdTable[pState->field_26_current_motion], ppRes);


        pSlig->mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame = pState->field_28_current_frame;
        pSlig->mBaseAnimatedWithPhysicsGameObject_Anim.mFrameChangeCounter = pState->field_2A_frame_change_counter;

        pSlig->mBaseGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4, pState->field_2D_bDrawable & 1);

        pSlig->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit5_FlipX, pState->field_24_bFlipX & 1);
        pSlig->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit3_Render, pState->field_2C_bRender & 1);

        if (IsLastFrame(&pSlig->mBaseAnimatedWithPhysicsGameObject_Anim))
        {
            pSlig->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit18_IsLastFrame);
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

        pSlig->field_146_return_level = pSlig->mBaseAnimatedWithPhysicsGameObject_LvlNumber; // always the same but set to junk in OG saves when using path skip cheat
        //pSlig->field_146_return_level = MapWrapper::FromAE(pState->field_72_return_level);
        pSlig->field_148_return_path = pState->field_74_return_path;
        pSlig->field_14A_return_camera = pState->field_76_return_camera;

        pSlig->field_14C_death_by_being_shot_timer = pState->field_78_death_by_being_shot_timer;
        pSlig->field_150_explode_timer = pState->field_7C_explode_timer;

        pSlig->SetBrain(sSligBrainTable[pState->field_80_brain_state_idx]);

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
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        xOff = FP_FromInteger(10) * mBaseAnimatedWithPhysicsGameObject_SpriteScale;
    }
    else
    {
        xOff = FP_FromInteger(-10) * mBaseAnimatedWithPhysicsGameObject_SpriteScale;
    }

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    if (!sCollisions->Raycast(
            xOff + mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(10),
            xOff + mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(10),
            &pLine,
            &hitX,
            &hitY,
            mBaseAnimatedWithPhysicsGameObject_Scale == Scale::Fg ? kFgFloor : kBgFloor))
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
                    SfxPlayMono(SoundEffect::PossessEffect_17, 0);
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
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eSligMotions::M_Walking_2_4B5BC0;
    }
    MoveOnLine_4B4C40();
}

void Slig::M_Walking_2_4B5BC0()
{
    if (gMap.GetDirection(
            mBaseAnimatedWithPhysicsGameObject_LvlNumber,
            mBaseAnimatedWithPhysicsGameObject_PathNumber,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos)
            >= CameraPos::eCamCurrent_0
        && MusicController::static_GetMusicType(0, 0, 0) <= MusicController::MusicTypes::eTension_4)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eTension_4, this, 0, 0);
    }

    field_128_input |= sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_C_held;

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        mBaseAnimatedWithPhysicsGameObject_VelX = -(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(9));
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_VelX = (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(9));
    }

    if (WallHit(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(45), mBaseAnimatedWithPhysicsGameObject_VelX * FP_FromInteger(2)))
    {
        ToStand_4B4A20();
        return;
    }

    MoveOnLine_4B4C40();

    if (mCurrentMotion == eSligMotions::M_Walking_2_4B5BC0)
    {
        if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 5 || mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 14)
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
                    mBaseAliveGameObjectLastAnimFrame = (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame - 5) != 0 ? 13 : 5;
                    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
                    {
                        field_128_input = 0;
                        mBaseAnimatedWithPhysicsGameObject_VelX = -(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(4));
                        return;
                    }
                    mBaseAnimatedWithPhysicsGameObject_VelX = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(4);
                }
                field_128_input = 0;
            }
        }
        else if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 11)
        {
            FP gridSize = {};
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
            {
                gridSize = -ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
            }
            else
            {
                gridSize = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
            }

            if (WallHit(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(45), gridSize * FP_FromDouble(2.5)))
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
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eSligMotions::M_Running_4_4B6000;
    }

    EventBroadcast(kEventNoise, this);

    if (WallHit(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(45), mBaseAnimatedWithPhysicsGameObject_VelX))
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
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        mBaseAnimatedWithPhysicsGameObject_VelX = -(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(4));
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_VelX = (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(4));
    }

    if (gMap.GetDirection(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos) >= CameraPos::eCamCurrent_0 && MusicController::static_GetMusicType(0, 0, 0) != MusicController::MusicTypes::ePossessed_9)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eSoftChase_8, this, 0, 0);
    }

    field_128_input |= sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_C_held;

    EventBroadcast(kEventNoise, this);

    if (WallHit(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(45), mBaseAnimatedWithPhysicsGameObject_VelX))
    {
        FallKnockBackOrSmash_4B4A90();
    }
    else
    {
        MoveOnLine_4B4C40();

        if (mCurrentMotion == eSligMotions::M_Running_4_4B6000)
        {
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 4 || mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 12)
            {
                Slig_SoundEffect_4BFFE0(SligSfx::eRunningStep_3, this);

                if (field_126_checked_if_off_screen == 0)
                {
                    field_126_checked_if_off_screen = 1;
                    MapFollowMe(TRUE);
                }

                if (sControlledCharacter_5C1B8C == this && mHealth > FP_FromInteger(0))
                {
                    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 4)
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
                        if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
                        {
                            mBaseAnimatedWithPhysicsGameObject_VelX = -(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(9));
                        }
                        else
                        {
                            mBaseAnimatedWithPhysicsGameObject_VelX = (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(9));
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
                        if (mBaseAnimatedWithPhysicsGameObject_VelX >= FP_FromInteger(0))
                        {
                            mBaseAnimatedWithPhysicsGameObject_VelX = (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromDouble(13.2));
                        }
                        else
                        {
                            mBaseAnimatedWithPhysicsGameObject_VelX = (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromDouble(-13.2));
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
    if (gMap.GetDirection(
            mBaseAnimatedWithPhysicsGameObject_LvlNumber,
            mBaseAnimatedWithPhysicsGameObject_PathNumber,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos)
            >= CameraPos::eCamCurrent_0
        && MusicController::static_GetMusicType(0, 0, 0) <= MusicController::MusicTypes::eTension_4)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eTension_4, this, 0, 0);
    }

    Slig_SoundEffect_4BFFE0(SligSfx::eStandingTurn_1, this);

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Toggle(AnimFlags::eBit5_FlipX);
        ToStand_4B4A20();
    }
}

void Slig::M_Shoot_6_4B55A0()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (sControlledCharacter_5C1B8C == this && mHealth > FP_FromInteger(0))
        {
            if (sInputObject_5BD4E0.isPressed(sInputKey_ThrowItem_5550F4))
            {
                const FP k45Scaled = mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(45);
                const FP kGridSize = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
                const FP k8 = FP_FromInteger(8);

                // Recoil right
                if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX) && sInputObject_5BD4E0.isPressed(sInputKey_Right_5550D0))
                {
                    PathLine* pLine = nullptr;
                    FP hitX = {};
                    FP hitY = {};
                    if (sCollisions->Raycast(
                            mBaseAnimatedWithPhysicsGameObject_XPos,
                            mBaseAnimatedWithPhysicsGameObject_YPos,
                            mBaseAnimatedWithPhysicsGameObject_XPos + (k8 * (kGridSize / k8)),
                            mBaseAnimatedWithPhysicsGameObject_YPos,
                            &pLine,
                            &hitX,
                            &hitY,
                            mBaseAnimatedWithPhysicsGameObject_Scale == Scale::Fg ? kFgWalls : kBgWalls)
                        || sCollisions->Raycast(
                            mBaseAnimatedWithPhysicsGameObject_XPos,
                            mBaseAnimatedWithPhysicsGameObject_YPos - k45Scaled,
                            mBaseAnimatedWithPhysicsGameObject_XPos + (k8 * (kGridSize / k8)),
                            mBaseAnimatedWithPhysicsGameObject_YPos - k45Scaled,
                            &pLine,
                            &hitX,
                            &hitY,
                            mBaseAnimatedWithPhysicsGameObject_Scale == Scale::Fg ? kFgWalls : kBgWalls))
                    {
                        return;
                    }
                    mBaseAnimatedWithPhysicsGameObject_VelX = (kGridSize / k8);
                    mCurrentMotion = eSligMotions::M_Recoil_19_4B8270;
                    return;
                }

                // Recoil left
                if (!mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX) && sInputObject_5BD4E0.isPressed(sInputKey_Left_5550D4))
                {
                    PathLine* pLine = nullptr;
                    FP hitX = {};
                    FP hitY = {};
                    if (sCollisions->Raycast(
                            mBaseAnimatedWithPhysicsGameObject_XPos,
                            mBaseAnimatedWithPhysicsGameObject_YPos,
                            mBaseAnimatedWithPhysicsGameObject_XPos - (k8 * (kGridSize / k8)),
                            mBaseAnimatedWithPhysicsGameObject_YPos,
                            &pLine,
                            &hitX,
                            &hitY,
                            mBaseAnimatedWithPhysicsGameObject_Scale == Scale::Fg ? kFgWalls : kBgWalls)
                        || sCollisions->Raycast(
                            mBaseAnimatedWithPhysicsGameObject_XPos,
                            mBaseAnimatedWithPhysicsGameObject_YPos - k45Scaled,
                            mBaseAnimatedWithPhysicsGameObject_XPos - (k8 * (kGridSize / k8)),
                            mBaseAnimatedWithPhysicsGameObject_YPos - k45Scaled,
                            &pLine,
                            &hitX,
                            &hitY,
                            mBaseAnimatedWithPhysicsGameObject_Scale == Scale::Fg ? kFgWalls : kBgWalls))
                    {
                        return;
                    }
                    mBaseAnimatedWithPhysicsGameObject_VelX = -(kGridSize / k8);
                    mCurrentMotion = eSligMotions::M_Recoil_19_4B8270;
                    return;
                }

                // General recoil
                if (!sInputObject_5BD4E0.isPressed(sInputKey_Down_5550DC) || mBaseAnimatedWithPhysicsGameObject_SpriteScale != FP_FromDouble(0.5))
                {
                    if (field_12C_timer > static_cast<s32>(sGnFrame))
                    {
                        return;
                    }

                    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
                    {
                        mBaseAnimatedWithPhysicsGameObject_VelX = (kGridSize / k8);
                    }
                    else
                    {
                        mBaseAnimatedWithPhysicsGameObject_VelX = -(kGridSize / k8);
                    }

                    PathLine* pLine = nullptr;
                    FP hitX = {};
                    FP hitY = {};
                    if (sCollisions->Raycast(
                            mBaseAnimatedWithPhysicsGameObject_XPos,
                            mBaseAnimatedWithPhysicsGameObject_YPos,
                            mBaseAnimatedWithPhysicsGameObject_XPos + (k8 * mBaseAnimatedWithPhysicsGameObject_VelX),
                            mBaseAnimatedWithPhysicsGameObject_YPos,
                            &pLine,
                            &hitX,
                            &hitY,
                            mBaseAnimatedWithPhysicsGameObject_Scale == Scale::Fg ? kFgWalls : kBgWalls)
                        || sCollisions->Raycast(
                            mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos - k45Scaled,
                            mBaseAnimatedWithPhysicsGameObject_XPos + (k8 * mBaseAnimatedWithPhysicsGameObject_VelX),
                            mBaseAnimatedWithPhysicsGameObject_YPos - k45Scaled,
                            &pLine,
                            &hitX,
                            &hitY,
                            mBaseAnimatedWithPhysicsGameObject_Scale == Scale::Fg ? kFgWalls : kBgWalls))
                    {
                        mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
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
    if (mBaseAnimatedWithPhysicsGameObject_VelX <= FP_FromInteger(0))
    {
        if (mBaseAnimatedWithPhysicsGameObject_VelX < FP_FromInteger(0))
        {
            mBaseAnimatedWithPhysicsGameObject_VelX += (mBaseAnimatedWithPhysicsGameObject_SpriteScale * field_130_falling_velx_scale_factor);
            if (mBaseAnimatedWithPhysicsGameObject_VelX < (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(4)))
            {
                mBaseAnimatedWithPhysicsGameObject_VelX = -(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(4));
            }

            if (mBaseAnimatedWithPhysicsGameObject_VelX > FP_FromInteger(0))
            {
                mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
            }
        }
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_VelX -= (mBaseAnimatedWithPhysicsGameObject_SpriteScale * field_130_falling_velx_scale_factor);
        if (mBaseAnimatedWithPhysicsGameObject_VelX > (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(4)))
        {
            mBaseAnimatedWithPhysicsGameObject_VelX = (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(4));
        }

        if (mBaseAnimatedWithPhysicsGameObject_VelX < FP_FromInteger(0))
        {
            mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
        }
    }

    PathLine* ppPathLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    const auto bCollision = InAirCollision(&ppPathLine, &hitX, &hitY, FP_FromDouble(1.8));
    SetActiveCameraDelayedFromDir();
    if (bCollision)
    {
        switch (ppPathLine->mLineType)
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
    EventBroadcast(kEventNoise, this);
    if (WallHit(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(45), mBaseAnimatedWithPhysicsGameObject_VelX))
    {
        FallKnockBackOrSmash_4B4A90();
    }
    else
    {
        SlowDown_4B6450(FP_FromDouble(2.125));
        if (mCurrentMotion == eSligMotions::M_SlidingToStand_8_4B6520)
        {
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame < 6 && sControlledCharacter_5C1B8C == this && mHealth > FP_FromInteger(0))
            {
                if ((mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX) && sInputObject_5BD4E0.isPressed(sInputKey_Right_5550D0)) || ((!(mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX)) && sInputObject_5BD4E0.isPressed(sInputKey_Left_5550D4))))
                {
                    mBaseAliveGameObjectLastAnimFrame = mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame;
                    mPreviousMotion = eSligMotions::M_SlidingTurn_9_4B6680;
                    field_124_return_to_previous_motion = 1;
                }
            }
            else if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
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
    EventBroadcast(kEventNoise, this);

    if (WallHit(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(45), mBaseAnimatedWithPhysicsGameObject_VelX))
    {
        FallKnockBackOrSmash_4B4A90();
    }
    else
    {
        SlowDown_4B6450(FP_FromDouble(2.125));

        if (mCurrentMotion == eSligMotions::M_SlidingTurn_9_4B6680)
        {
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
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
                    mBaseAnimatedWithPhysicsGameObject_VelX = -(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(4));
                    mCurrentMotion = eSligMotions::M_SlidingTurnToRun_11_4B6850;
                }
                else
                {
                    mBaseAnimatedWithPhysicsGameObject_VelX = -(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(9));
                    mCurrentMotion = eSligMotions::M_SlidingTurnToWalk_10_4B6800;
                }

                if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
                {
                    mBaseAnimatedWithPhysicsGameObject_VelX = -mBaseAnimatedWithPhysicsGameObject_VelX;
                }
            }
        }
    }
}

void Slig::M_SlidingTurnToWalk_10_4B6800()
{
    EventBroadcast(kEventNoise, this);
    MoveOnLine_4B4C40();

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Toggle(AnimFlags::eBit5_FlipX);
        mCurrentMotion = eSligMotions::M_Walking_2_4B5BC0;
    }
}

void Slig::M_SlidingTurnToRun_11_4B6850()
{
    EventBroadcast(kEventNoise, this);
    MoveOnLine_4B4C40();

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Toggle(AnimFlags::eBit5_FlipX);
        mCurrentMotion = eSligMotions::M_Running_4_4B6000;
    }
}

void Slig::M_ReloadGun_12_4B5530()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 1)
    {
        Slig_SoundEffect_4BFFE0(SligSfx::eReload1_6, this);
    }
    else if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 4)
    {
        Slig_SoundEffect_4BFFE0(SligSfx::eReload2_7, this);
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToStand_4B4A20();
    }
}

void Slig::M_ShootToStand_13_4B5580()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        MainMovement_4B4720();
    }
}

void Slig::M_SteppingToStand_14_4B8480()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 0)
    {
        Slig_SoundEffect_4BFFE0(SligSfx::eWalkingStep_2, this);
    }

    if (WallHit(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(45), mBaseAnimatedWithPhysicsGameObject_VelX))
    {
        FallKnockBackOrSmash_4B4A90();
    }
    else
    {
        MoveOnLine_4B4C40();
        if (mCurrentMotion == eSligMotions::M_SteppingToStand_14_4B8480)
        {
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                MapFollowMe(TRUE);
                ToStand_4B4A20();
            }
        }
    }
}

void Slig::M_StandingToStep_15_4B83B0()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (sInputObject_5BD4E0.isPressed(sInputKey_Left_5550D4 | sInputKey_Right_5550D0))
        {
            mCurrentMotion = eSligMotions::M_StandToWalk_1_4B5F70;
        }
        else if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
        {
            mCurrentMotion = eSligMotions::M_SteppingToStand_14_4B8480;
            mBaseAnimatedWithPhysicsGameObject_VelX = -(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(6));
        }
        else
        {
            mBaseAnimatedWithPhysicsGameObject_VelX = (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(6));
            mCurrentMotion = eSligMotions::M_SteppingToStand_14_4B8480;
        }
    }
}

void Slig::M_DepossessingAbort_16_4B8250()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToStand_4B4A20();
    }
}

void Slig::M_GameSpeak_17_4B5290()
{
    field_128_input |= sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_C_held;
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
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
                EventBroadcast(kEventSpeaking, this);
            }
            field_128_input = 0;
        }
    }
}

void Slig::M_WalkToStand_18_4B5FC0()
{
    MoveOnLine_4B4C40();

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
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
        if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            MapFollowMe(TRUE);
            if (sControlledCharacter_5C1B8C == this && mHealth > FP_FromInteger(0))
            {
                if (sInputObject_5BD4E0.isPressed(sInputKey_ThrowItem_5550F4))
                {
                    mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
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
    else if (mBaseAnimatedWithPhysicsGameObject_SpriteScale == FP_FromDouble(0.5))
    {
        SfxPlayMono(SoundEffect::SligShoot_5, 85);
    }
    else
    {
        SfxPlayMono(SoundEffect::SligShoot_5, 0);
    }
}

void Slig::M_SpeakHereBoy_20_4B5330()
{
    field_128_input |= sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_C_held;
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
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
                EventBroadcast(kEventSpeaking, this);
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
            if (gMap.Is_Point_In_Current_Camera(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0))
            {
                FP xOff = {};
                if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
                {
                    xOff = -(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(-20));
                }
                else
                {
                    xOff = (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(-20));
                }
                relive_new SnoozeParticle(
                    xOff + mBaseAnimatedWithPhysicsGameObject_XPos,
                    mBaseAnimatedWithPhysicsGameObject_YPos + (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(-10)),
                    mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer,
                    mBaseAnimatedWithPhysicsGameObject_Anim.field_14_scale);
            }
        }
    }
    else
    {
        Slig_SoundEffect_4BFFE0(SligSfx::eSnooze2_4, this);

        if (gMap.Is_Point_In_Current_Camera(
                mBaseAnimatedWithPhysicsGameObject_LvlNumber,
                mBaseAnimatedWithPhysicsGameObject_PathNumber,
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos,
                0))
        {
            FP xOff = {};
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
            {
                xOff = -(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(-20));
            }
            else
            {
                xOff = (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(-20));
            }

            relive_new SnoozeParticle(
                xOff + mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos + (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(-10)),
                mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer,
                mBaseAnimatedWithPhysicsGameObject_Anim.field_14_scale);
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

        if (pObj->Type() == ReliveTypes::eSnoozeParticle)
        {
            static_cast<SnoozeParticle*>(pObj)->field_1E4_state = SnoozeParticle::SnoozeParticleState::eBlowingUp_2;
        }
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame >= 2 && mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame <= 10)
    {
        Slig_SoundEffect_4BFFE0(SligSfx::eToStand_0, this);
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 9)
    {
        Slig_SoundEffect_4BFFE0(SligSfx::eWalkingStep_2, this);
    }

    switch (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame)
    {
        case 9:
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
            {
                mBaseAnimatedWithPhysicsGameObject_XPos -= (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(7));
                return;
            }
            else
            {
                mBaseAnimatedWithPhysicsGameObject_XPos += (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(7));
                return;
            }
            break;

        case 10:
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
            {
                mBaseAnimatedWithPhysicsGameObject_XPos -= (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(12));
                return;
            }
            else
            {
                mBaseAnimatedWithPhysicsGameObject_XPos += (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(12));
                return;
            }
            break;

        case 11:
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
            {
                mBaseAnimatedWithPhysicsGameObject_XPos -= (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(6));
                return;
            }
            else
            {
                mBaseAnimatedWithPhysicsGameObject_XPos += (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(6));
                return;
            }
            break;

        default:
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
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
                EventBroadcast(kEventMudokonComfort, this);
            }
        }
    }

    EventBroadcast(kEventNoise, this);
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 4)
    {
        Environment_SFX_457A40(EnvironmentSfx::eHitGroundSoft_6, 80, -200, this);
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
    {
        if (BaseAliveGameObjectCollisionLine)
        {
            mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);

            if (mHealth > FP_FromInteger(0) && field_12C_timer <= static_cast<s32>(sGnFrame) && sActiveHero->mHealth > FP_FromInteger(0))
            {
                mCurrentMotion = eSligMotions::M_KnockbackToStand_35_4B6A30;
            }
        }
    }
}

void Slig::M_KnockbackToStand_35_4B6A30()
{
    EventBroadcast(kEventNoise, this);

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame >= 2 && mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame <= 10)
    {
        Slig_SoundEffect_4BFFE0(SligSfx::eToStand_0, this);
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 9)
    {
        Slig_SoundEffect_4BFFE0(SligSfx::eWalkingStep_2, this);
    }

    FP gridSize = {};
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        gridSize = -ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
    }
    else
    {
        gridSize = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
    }

    if (!WallHit(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(45), gridSize))
    {
        s32 xOff = 0;
        switch (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame)
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

        if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
        {
            xOff = -xOff;
        }

        mBaseAnimatedWithPhysicsGameObject_XPos += (mBaseAnimatedWithPhysicsGameObject_SpriteScale * mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(xOff);
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToStand_4B4A20();
    }
}

void Slig::M_Depossessing_36_4B7F30()
{
    if (sControlledCharacter_5C1B8C == this)
    {
        if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
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
                (mBaseAnimatedWithPhysicsGameObject_SpriteScale * xRand) + mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos - (mBaseAnimatedWithPhysicsGameObject_SpriteScale * yRand),
                mBaseAnimatedWithPhysicsGameObject_SpriteScale,
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
        if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit3_Render))
        {
            FP xOff = (FP_FromInteger(20) * mBaseAnimatedWithPhysicsGameObject_SpriteScale);
            if (mBaseAnimatedWithPhysicsGameObject_VelX < FP_FromInteger(0))
            {
                xOff = -xOff;
            }

            relive_new Gibs(
                GibType::Slig_1,
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos,
                xOff,
                FP_FromInteger(0),
                mBaseAnimatedWithPhysicsGameObject_SpriteScale,
                0);

            New_Smoke_Particles(
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos - (FP_FromInteger(30) * mBaseAnimatedWithPhysicsGameObject_SpriteScale),
                mBaseAnimatedWithPhysicsGameObject_SpriteScale,
                3,
                0x80u,
                0x80u,
                0x80u);

            if (mBaseAnimatedWithPhysicsGameObject_SpriteScale == FP_FromDouble(0.5))
            {
                SfxPlayMono(SoundEffect::KillEffect_64, 80);
                SfxPlayMono(SoundEffect::FallingItemHit_47, 60);
            }
            else
            {
                SfxPlayMono(SoundEffect::KillEffect_64, 127);
                SfxPlayMono(SoundEffect::FallingItemHit_47, 90);
            }

            mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit3_Render);
            mCurrentMotion = eSligMotions::M_StandIdle_0_4B4EC0;
            mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);
            mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
            mHealth = FP_FromInteger(0);
            MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, this, 0, 0);
            SetUpdateDelay(40);
            field_120_timer = sGnFrame + 60;
            SetBrain(&Slig::Brain_Death_0_4BBFB0);
        }
    }

    if (static_cast<s32>(sGnFrame) > field_14C_death_by_being_shot_timer)
    {
        if (mBaseAnimatedWithPhysicsGameObject_VelX >= FP_FromInteger(0))
        {
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
            {
                mCurrentMotion = eSligMotions::M_Knockback_34_4B68A0;
            }
        }
        else if (!mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
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
        EventBroadcast(kEventMudokonComfort, this);
    }
}

void Slig::M_FallingInitiate_39_4B4640()
{
    EventBroadcast(kEventNoise, this);

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eSligMotions::M_OutToFall_38_4B4570;
    }

    M_Falling_7_4B42D0();
}

void Slig::M_LandingSoft_40_4B4530()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 0)
    {
        Environment_SFX_457A40(EnvironmentSfx::eHitGroundSoft_6, 0, 0x7FFF, 0);
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToStand_4B4A20();
    }
}

void Slig::M_LandingFatal_41_4B4680()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 0)
    {
        SND_SEQ_Play(SeqId::HitBottomOfDeathPit_9, 1, 65, 65);
        SfxPlayMono(SoundEffect::KillEffect_64, 80);
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
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
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
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
    else if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 7)
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

        relive_new Bullet(
            this,
            bulletType,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(12),
            FP_FromInteger(640),
            0,
            mBaseAnimatedWithPhysicsGameObject_SpriteScale,
            field_218_tlv_data.field_22_num_times_to_shoot - field_158_num_times_to_shoot - 1);

        New_ShootingZFire_Particle(mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(12), mBaseAnimatedWithPhysicsGameObject_SpriteScale);

        if (mBaseAnimatedWithPhysicsGameObject_SpriteScale == FP_FromDouble(0.5))
        {
            SfxPlayMono(SoundEffect::SligShoot_5, 85);
        }
        else
        {
            SfxPlayMono(SoundEffect::SligShoot_5, 0);
        }

        // The doves don't like bullets
        Dove::All_FlyAway(0);

        EventBroadcast(kEventShooting, this);
        EventBroadcast(kEventLoudNoise, this);
    }
}

void Slig::M_ShootZtoStand_43_4B77E0()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToStand_4B4A20();
    }
}

void Slig::M_Smash_44_4B6B90()
{
    if (BaseAliveGameObjectCollisionLine)
    {
        if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 4)
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
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
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
    auto pLiftPoint = static_cast<LiftPoint*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    if (pLiftPoint)
    {
        pLiftPoint->vMove_4626A0(FP_FromInteger(0), FP_FromInteger(0), 0);
        mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);
        if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
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
    mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToStand_4B4A20();
    }
}

void Slig::M_LiftGripping_48_4B3850()
{
    auto pLiftPoint = static_cast<LiftPoint*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    if (pLiftPoint)
    {
        CheckPlatformVanished_4B3640();

        pLiftPoint->vMove_4626A0(FP_FromInteger(0), FP_FromInteger(0), 0);
        mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);

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
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 5)
    {
        SFX_Play_Pitch(SoundEffect::AirStream_23, 90, -300);
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 8)
    {
        const FP kGridSize = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
        const FP k2Scaled = FP_FromInteger(2) * kGridSize;

        PSX_RECT hitRect = {};
        hitRect = MakeMinMaxRect(
            mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX) ? mBaseAnimatedWithPhysicsGameObject_XPos - kGridSize : mBaseAnimatedWithPhysicsGameObject_XPos + kGridSize,
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
                if (pObj->Type() == ReliveTypes::eMudokon || pObj->Type() == ReliveTypes::eCrawlingSlig)
                {
                    const PSX_RECT bRect = pObj->VGetBoundingRect();

                    if (pObj->mHealth > FP_FromInteger(0) && pObj->mBaseAnimatedWithPhysicsGameObject_Scale == mBaseAnimatedWithPhysicsGameObject_Scale && PSX_Rects_overlap_no_adjustment(&hitRect, &bRect))
                    {
                        pObj->VTakeDamage(this);
                        EventBroadcast(kEventNoise, this);
                        SfxPlayMono(SoundEffect::FallingItemHit_47, 60);
                        return;
                    }
                }
            }
        }
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
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
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit3_Render) && field_120_timer < (static_cast<s32>(sGnFrame) + 30))
    {
        mBaseAnimatedWithPhysicsGameObject_SpriteScale -= FP_FromDouble(0.01);
        mBaseAnimatedWithPhysicsGameObject_RGB.r -= 2;
        mBaseAnimatedWithPhysicsGameObject_RGB.g -= 2;
        mBaseAnimatedWithPhysicsGameObject_RGB.b -= 2;

        if (mBaseAnimatedWithPhysicsGameObject_SpriteScale >= FP_FromDouble(0.3))
        {
            DeathSmokeEffect(true);
        }
    }
    else if (!mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit3_Render))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (sControlledCharacter_5C1B8C == this)
    {
        if (field_120_timer < static_cast<s32>(sGnFrame))
        {
            sControlledCharacter_5C1B8C = sActiveHero;
            MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, this, 0, 0);
            gMap.SetActiveCam(field_146_return_level, field_148_return_path, field_14A_return_camera, CameraSwapEffects::eInstantChange_0, 0, 0);
        }
    }

    if (sControlledCharacter_5C1B8C != this)
    {
        if (!gMap.Is_Point_In_Current_Camera(
                mBaseAnimatedWithPhysicsGameObject_LvlNumber,
                mBaseAnimatedWithPhysicsGameObject_PathNumber,
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos,
                0))
        {
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        }
    }

    if (mBaseAnimatedWithPhysicsGameObject_SpriteScale < FP_FromInteger(0))
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
        gMap.SetActiveCam(field_146_return_level, field_148_return_path, field_14A_return_camera, CameraSwapEffects::eInstantChange_0, 0, 0);
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
            if (EventGet(kEventDeathReset))
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
            if (EventGet(kEventDeathReset))
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

            relive_new ScreenShake(0, 0);

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
                    gMap.SetActiveCam(field_146_return_level, field_148_return_path, field_14A_return_camera, CameraSwapEffects::eInstantChange_0, 0, 0);
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
    auto pPlatformObj = static_cast<LiftPoint*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));

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
    const FP gridSize = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);

    if (mCurrentMotion == eSligMotions::M_Knockback_34_4B68A0)
    {
        mNextMotion = eSligMotions::M_StandIdle_0_4B4EC0;
        return Brain_ListeningToGlukkon_States::IdleListening_1;
    }

    if (mCurrentMotion != eSligMotions::M_StandIdle_0_4B4EC0)
    {
        return field_11C_brain_sub_state;
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        if (!pPlatformObj)
        {
            return Brain_ListeningToGlukkon_States::IdleListening_1;
        }

        if (pPlatformObj->Type() != ReliveTypes::eLiftPoint)
        {
            return Brain_ListeningToGlukkon_States::IdleListening_1;
        }

        const FP gridSizeHalved = (gridSize / FP_FromInteger(2));
        const FP v84 = mBaseAnimatedWithPhysicsGameObject_XPos - gridSize - FP_FromInteger((BaseAliveGameObjectCollisionLine->mRect.x + BaseAliveGameObjectCollisionLine->mRect.w) / 2) >= FP_FromInteger(0) ? mBaseAnimatedWithPhysicsGameObject_XPos - gridSize - FP_FromInteger((BaseAliveGameObjectCollisionLine->mRect.x + BaseAliveGameObjectCollisionLine->mRect.w) / 2) : FP_FromInteger((BaseAliveGameObjectCollisionLine->mRect.x + BaseAliveGameObjectCollisionLine->mRect.w) / 2) - (mBaseAnimatedWithPhysicsGameObject_XPos - gridSize);
        if (v84 >= gridSizeHalved)
        {
            return Brain_ListeningToGlukkon_States::IdleListening_1;
        }
        mNextMotion = eSligMotions::M_SteppingToStand_14_4B8480;
        mBaseAnimatedWithPhysicsGameObject_VelX = -(gridSize / FP_FromInteger(6));
        return field_11C_brain_sub_state;
    }

    if (!pPlatformObj)
    {
        return Brain_ListeningToGlukkon_States::IdleListening_1;
    }

    if (pPlatformObj->Type() != ReliveTypes::eLiftPoint)
    {
        return Brain_ListeningToGlukkon_States::IdleListening_1;
    }

    const FP v90 = mBaseAnimatedWithPhysicsGameObject_XPos + gridSize - FP_FromInteger((BaseAliveGameObjectCollisionLine->mRect.x + BaseAliveGameObjectCollisionLine->mRect.w) / 2) >= FP_FromInteger(0) ? mBaseAnimatedWithPhysicsGameObject_XPos + gridSize
                                                                                                                                                                                   - FP_FromInteger((BaseAliveGameObjectCollisionLine->mRect.x + BaseAliveGameObjectCollisionLine->mRect.w) / 2)
                                                                                                                                                                             : FP_FromInteger((BaseAliveGameObjectCollisionLine->mRect.x + BaseAliveGameObjectCollisionLine->mRect.w) / 2)
                                                                                                                                                                                   - (mBaseAnimatedWithPhysicsGameObject_XPos + gridSize);
    if (v90 >= gridSize / FP_FromInteger(2))
    {
        return Brain_ListeningToGlukkon_States::IdleListening_1;
    }
    mBaseAnimatedWithPhysicsGameObject_VelX = (gridSize / FP_FromInteger(6));
    mNextMotion = eSligMotions::M_SteppingToStand_14_4B8480;
    return field_11C_brain_sub_state;
}

s16 Slig::Brain_ListeningToGlukkon_PullingLever()
{
    const FP gridSize = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);

    Lever* pLever = nullptr;
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        pLever = static_cast<Lever*>(FindObjectOfType(ReliveTypes::eLever, mBaseAnimatedWithPhysicsGameObject_XPos - gridSize, mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5)));
    }
    else
    {
        pLever = static_cast<Lever*>(FindObjectOfType(ReliveTypes::eLever, mBaseAnimatedWithPhysicsGameObject_XPos + gridSize, mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5)));
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
    const FP gridSize = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);

    if (mCurrentMotion == eSligMotions::M_Knockback_34_4B68A0)
    {
        mNextMotion = eSligMotions::M_StandIdle_0_4B4EC0;
        return Brain_ListeningToGlukkon_States::IdleListening_1;
    }

    if (mCurrentMotion != eSligMotions::M_StandIdle_0_4B4EC0)
    {
        return field_11C_brain_sub_state;
    }

    if (!FindObjectOfType(ReliveTypes::eLever, gridSize + mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5)))
    {
        if (!FindObjectOfType(ReliveTypes::eLever, mBaseAnimatedWithPhysicsGameObject_XPos - gridSize, mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5)))
        {
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
            {
                mNextMotion = eSligMotions::M_SteppingToStand_14_4B8480;
                mBaseAnimatedWithPhysicsGameObject_VelX = -(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(6));
                return field_11C_brain_sub_state;
            }
            mBaseAnimatedWithPhysicsGameObject_VelX = (gridSize / FP_FromInteger(6));
            mNextMotion = eSligMotions::M_SteppingToStand_14_4B8480;
            return field_11C_brain_sub_state;
        }
    }

    if ((!FindObjectOfType(ReliveTypes::eLever, gridSize + mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5)) || !(mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))) && (!FindObjectOfType(ReliveTypes::eLever, mBaseAnimatedWithPhysicsGameObject_XPos - gridSize, mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5)) || mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX)))
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

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        mBaseAnimatedWithPhysicsGameObject_VelX = -(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(6));
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_VelX = (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(6));
    }

    mNextMotion = eSligMotions::M_SteppingToStand_14_4B8480;
    return Brain_ListeningToGlukkon_States::IdleListening_1;
}

s16 Slig::Brain_ListenToGlukkon_Moving(BaseAliveGameObject* pGlukkonObj)
{
    FP xOffset = (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(mCurrentMotion != eSligMotions::M_Running_4_4B6000 ? 1 : 3));
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        xOffset = -xOffset;
    }

    if (WallHit(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(45), xOffset))
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

    if (VIsObjNearby(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale), pGlukkonObj))
    {
        mNextMotion = eSligMotions::M_StandIdle_0_4B4EC0;
        return Brain_ListeningToGlukkon_States::IdleListening_1;
    }

    if (field_160_last_event_index != pEventSystem_5BC11C->field_28_last_event_index)
    {
        field_160_last_event_index = pEventSystem_5BC11C->field_28_last_event_index;
        if (pEventSystem_5BC11C->field_20_last_event == GameSpeakEvents::Glukkon_StayHere_38 && gMap.Is_Point_In_Current_Camera(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0))
        {
            field_216_flags.Clear(Flags_216::eBit1_FollowGlukkon);
            NextCommand_4B9A00(Brain_ListeningToGlukkon_GlukkonCommands::Stay_2, Brain_ListeningToGlukkon_States::IdleListening_1);
            return Brain_ListeningToGlukkon_States::Speaking_4;
        }
    }

    if (!VIsObjNearby(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(5), pGlukkonObj))
    {
        if (mCurrentMotion == eSligMotions::M_Walking_2_4B5BC0)
        {
            mNextMotion = eSligMotions::M_Running_4_4B6000;
            return Brain_ListeningToGlukkon_States::Moving_2;
        }
    }

    if (!VIsObjNearby(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(5), pGlukkonObj) || mCurrentMotion != eSligMotions::M_Running_4_4B6000)
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
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        directedGridSize = -ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
    }
    else
    {
        directedGridSize = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
    }

    const FP gridSize = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);

    const s32 bWallHit = WallHit(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(45), directedGridSize);
    if (field_20E_attention_timeout <= 450 && (gMap.Is_Point_In_Current_Camera(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 1) || field_216_flags.Get(Flags_216::eBit1_FollowGlukkon)))
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
                    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
                    {
                        directedGridSize = -ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
                    }
                    else
                    {
                        directedGridSize = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
                    }

                    if (!WallHit(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(45), directedGridSize * FP_FromInteger(3)))
                    {
                        NextCommand_4B9A00(Brain_ListeningToGlukkon_GlukkonCommands::Run_5, Brain_ListeningToGlukkon_States::Moving_2);
                        return Brain_ListeningToGlukkon_States::Speaking_4;
                    }
                }
            }

            if (sSligsUnderControlCount_BAF7E8 > 1)
            {
                auto pOtherSlig = static_cast<Slig*>(FindObjectOfType(ReliveTypes::eSlig, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5)));
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
                        if (pPlatformObj && pPlatformObj->Type() == ReliveTypes::eLiftPoint)
                        {
                            const FP scaled_2 = (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(2));
                            const FP lineMidX = FP_FromInteger((BaseAliveGameObjectCollisionLine->mRect.x + BaseAliveGameObjectCollisionLine->mRect.w) / 2);
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

                                    if (pFoundSlig->Type() == ReliveTypes::eSlig && pFoundSlig != this && pFoundSlig->mNextMotion == eSligMotions::M_LiftGrip_46_4B3700)
                                    {
                                        mNextMotion = eSligMotions::M_StandIdle_0_4B4EC0;
                                    }
                                }

                                field_11C_brain_sub_state = Brain_ListeningToGlukkon_States::IdleListening_1;
                                field_216_flags.Set(Flags_216::eBit3_GlukkonCalledAllOYa, glukkonSpeak == GameSpeakEvents::Glukkon_AllOYa_40);
                                return field_11C_brain_sub_state;
                            }
                        }

                        if (FindObjectOfType(ReliveTypes::eLever, ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) + mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5)) || FindObjectOfType(ReliveTypes::eLever, mBaseAnimatedWithPhysicsGameObject_XPos - ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale), mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5)))
                        {
                            field_11C_brain_sub_state = Brain_ListeningToGlukkon_States::PullingLever_6;
                            field_216_flags.Set(Flags_216::eBit3_GlukkonCalledAllOYa, glukkonSpeak == GameSpeakEvents::Glukkon_AllOYa_40);
                            return field_11C_brain_sub_state;
                        }

                        if (!FindObjectOfType(ReliveTypes::eMudokon, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5)))
                        {
                            const FP scaled = mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX) ? -ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) : ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);

                            if (!FindObjectOfType(ReliveTypes::eMudokon, scaled + mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5)))
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
    if (gMap.Is_Point_In_Current_Camera(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0) || field_218_tlv_data.field_2A_chase_abe_when_spotted == Choice_short::eNo_0)
    {
        if (VOnSameYLevel(sControlledCharacter_5C1B8C) && VIsObj_GettingNear_On_X(sControlledCharacter_5C1B8C) && VIsObjNearby(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(3), sControlledCharacter_5C1B8C) && !EventGet(kEventResetting) && !sControlledCharacter_5C1B8C->mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit8_bInvisible))
        {
            ToShoot_4BF9A0();
        }
        else if (
            VOnSameYLevel(sControlledCharacter_5C1B8C) && EventGet(kEventAbeOhm) && VIsFacingMe(sControlledCharacter_5C1B8C))
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
    if (EventGet(kEventDeathReset) && !gMap.Is_Point_In_Current_Camera(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0))
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
    if (EventGet(kEventDeathReset) && !gMap.Is_Point_In_Current_Camera(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0))
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
        if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            mNextMotion = eSligMotions::M_TurnAroundStanding_5_4B6390;
            return 107;
        }
        checkTurn = true;
    }
    else if (mCurrentMotion == eSligMotions::M_TurnAroundStanding_5_4B6390)
    {
        if (!mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            checkTurn = true;
        }
    }

    if (checkTurn)
    {
        if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 4)
        {
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
            {
                if (sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_VelX >= FP_FromInteger(0) && (sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_VelX != FP_FromInteger(0) || sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_XPos >= mBaseAnimatedWithPhysicsGameObject_XPos))
                {
                    ShouldStillBeAlive_4BBC00();
                    return 107;
                }
            }
            else
            {
                if (sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_VelX <= FP_FromInteger(0) && (sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_VelX != FP_FromInteger(0) || sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_XPos <= mBaseAnimatedWithPhysicsGameObject_XPos))
                {
                    ShouldStillBeAlive_4BBC00();
                    return 107;
                }
            }

            const PSX_RECT bRect = VGetBoundingRect();
            const PSX_RECT charRect = sControlledCharacter_5C1B8C->VGetBoundingRect();

            if (sControlledCharacter_5C1B8C->Type() != ReliveTypes::eGlukkon && sControlledCharacter_5C1B8C->Type() != ReliveTypes::eSlig && !IsInInvisibleZone(sControlledCharacter_5C1B8C) && !sControlledCharacter_5C1B8C->mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit8_bInvisible))
            {
                if (charRect.x <= bRect.w && charRect.w >= bRect.x && charRect.h >= bRect.y && charRect.y <= bRect.h)
                {
                    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Toggle(AnimFlags::eBit5_FlipX);
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
    if (mBaseAnimatedWithPhysicsGameObject_VelX > FP_FromInteger(4) && ((ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(4)) + mBaseAnimatedWithPhysicsGameObject_XPos) > FP_FromInteger(field_138_zone_rect.w))
    {
        ToPanicTurn_4BC750();
    }
    else if (mBaseAnimatedWithPhysicsGameObject_VelX < FP_FromInteger(-4) && (mBaseAnimatedWithPhysicsGameObject_XPos - (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(4))) < FP_FromInteger(field_138_zone_rect.x))
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
    else if (VOnSameYLevel(sControlledCharacter_5C1B8C) && sControlledCharacter_5C1B8C->Type() != ReliveTypes::eGlukkon && VIsFacingMe(sControlledCharacter_5C1B8C) && !IsInInvisibleZone(sControlledCharacter_5C1B8C) && !sControlledCharacter_5C1B8C->mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit8_bInvisible) && !IsWallBetween_4BB8B0(this, sControlledCharacter_5C1B8C) && gMap.Is_Point_In_Current_Camera(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0) && gMap.Is_Point_In_Current_Camera(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0) && !EventGet(kEventResetting))
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

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        EventBroadcast(kEventAlarm, this);

        const bool kFlipX = mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX);
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
    if ((EventGet(kEventAbeOhm) || EventGet(kEventAlarm)) && !EventGet(kEventResetting) && field_218_tlv_data.field_2E_panic_timeout)
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
        !IsWallBetween_4BB8B0(this, sControlledCharacter_5C1B8C) && (!field_15E_spotted_possessed_slig || sControlledCharacter_5C1B8C->Type() != ReliveTypes::eSlig) &&
        !IsAbeEnteringDoor_4BB990(sControlledCharacter_5C1B8C) &&
        !EventGet(kEventResetting) &&
        gMap.Is_Point_In_Current_Camera(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0) && sControlledCharacter_5C1B8C->Type() != ReliveTypes::eGlukkon)
    {
        RespondToEnemyOrPatrol_4B3140();
        return 104;
    }

    if (sActiveHero->mHealth <= FP_FromInteger(0))
    {
        ToAbeDead_4B3580();
        return 104;
    }

    auto pShooter = static_cast<BaseAliveGameObject*>(EventGet(kEventShooting));
    if (pShooter && pShooter->mBaseAnimatedWithPhysicsGameObject_SpriteScale == mBaseAnimatedWithPhysicsGameObject_SpriteScale && gMap.Is_Point_In_Current_Camera(pShooter->mBaseAnimatedWithPhysicsGameObject_LvlNumber, pShooter->mBaseAnimatedWithPhysicsGameObject_PathNumber, pShooter->mBaseAnimatedWithPhysicsGameObject_XPos, pShooter->mBaseAnimatedWithPhysicsGameObject_YPos, 0) && pShooter == sControlledCharacter_5C1B8C && gMap.Is_Point_In_Current_Camera(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0) && !EventGet(kEventResetting))
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

    auto pNoiseOrSpeaking = static_cast<BaseAliveGameObject*>(EventGet(kEventSuspiciousNoise));
    if (!pNoiseOrSpeaking)
    {
        pNoiseOrSpeaking = static_cast<BaseAliveGameObject*>(EventGet(kEventSpeaking));
    }

    if (pNoiseOrSpeaking && pNoiseOrSpeaking->mBaseAnimatedWithPhysicsGameObject_SpriteScale == mBaseAnimatedWithPhysicsGameObject_SpriteScale && gMap.Is_Point_In_Current_Camera(pNoiseOrSpeaking->mBaseAnimatedWithPhysicsGameObject_LvlNumber, pNoiseOrSpeaking->mBaseAnimatedWithPhysicsGameObject_PathNumber, pNoiseOrSpeaking->mBaseAnimatedWithPhysicsGameObject_XPos, pNoiseOrSpeaking->mBaseAnimatedWithPhysicsGameObject_YPos, 0) && pNoiseOrSpeaking != this && gMap.Is_Point_In_Current_Camera(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0) && !EventGet(kEventResetting))
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
        if (sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_SpriteScale > mBaseAnimatedWithPhysicsGameObject_SpriteScale && VIsFacingMe(sControlledCharacter_5C1B8C) && !IsInInvisibleZone(sControlledCharacter_5C1B8C) && !sControlledCharacter_5C1B8C->mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit8_bInvisible) && gMap.Is_Point_In_Current_Camera(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0) && !InZCover_4BB7C0(sControlledCharacter_5C1B8C) && !InZCover_4BB7C0(this) && !EventGet(kEventResetting))
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
    if (gMap.Is_Point_In_Current_Camera(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0) && VOnSameYLevel(sControlledCharacter_5C1B8C) && VIsFacingMe(sControlledCharacter_5C1B8C) && !IsInInvisibleZone(sControlledCharacter_5C1B8C) && !sControlledCharacter_5C1B8C->mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit8_bInvisible) && !IsWallBetween_4BB8B0(this, sControlledCharacter_5C1B8C) && !RenderLayerIs_4BBBC0(sControlledCharacter_5C1B8C) && !EventGet(kEventResetting) && sControlledCharacter_5C1B8C->Type() != ReliveTypes::eGlukkon)
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

    if (mBaseAnimatedWithPhysicsGameObject_PathNumber != gMap.mCurrentPath || mBaseAnimatedWithPhysicsGameObject_LvlNumber != gMap.mCurrentLevel || (EventGet(kEventDeathReset) && !gMap.Is_Point_In_Current_Camera(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0)))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
    else if (gMap.Is_Point_In_Current_Camera(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0))
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
        if (gMap.Is_Point_In_Current_Camera(
                mBaseAnimatedWithPhysicsGameObject_LvlNumber,
                mBaseAnimatedWithPhysicsGameObject_PathNumber,
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos,
                0)
            && !sControlledCharacter_5C1B8C->mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit8_bInvisible) && sControlledCharacter_5C1B8C->Type() != ReliveTypes::eGlukkon)
        {
            field_15C_force_alive_state = 0;
            ToShoot_4BF9A0();
        }
    }
    else
    {
        if (mBaseAnimatedWithPhysicsGameObject_PathNumber != gMap.mCurrentPath || mBaseAnimatedWithPhysicsGameObject_LvlNumber != gMap.mCurrentLevel)
        {
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        }

        if (!VIsFacingMe(sControlledCharacter_5C1B8C))
        {
            mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Toggle(AnimFlags::eBit5_FlipX);
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
    if (EventGet(kEventDeathReset) && !gMap.Is_Point_In_Current_Camera(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        return 106;
    }

    if (ListenToGlukkonCommands_4B95D0())
    {
        ToUnderGlukkonCommand_4B9660();
        return 106;
    }

    if ((mCurrentMotion == eSligMotions::M_TurnAroundStanding_5_4B6390 && mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame)) || (mCurrentMotion == eSligMotions::M_StandIdle_0_4B4EC0 && mNextMotion == -1))
    {
        WaitOrWalk_4BE870();
        return 106;
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame != 4)
    {
        ShouldStillBeAlive_4BBC00();
        return 106;
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        if (sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_VelX >= FP_FromInteger(0) && (sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_VelX > FP_FromInteger(0) || sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_XPos >= mBaseAnimatedWithPhysicsGameObject_XPos))
        {
            ShouldStillBeAlive_4BBC00();
            return 106;
        }
    }
    else
    {
        if (sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_VelX <= FP_FromInteger(0) && (sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_VelX != FP_FromInteger(0) || sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_XPos <= mBaseAnimatedWithPhysicsGameObject_XPos))
        {
            ShouldStillBeAlive_4BBC00();
            return 106;
        }
    }

    const PSX_RECT bRect = VGetBoundingRect();
    const PSX_RECT charRect = sControlledCharacter_5C1B8C->VGetBoundingRect();


    if (sControlledCharacter_5C1B8C->Type() != ReliveTypes::eGlukkon && sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_Scale == mBaseAnimatedWithPhysicsGameObject_Scale && !IsInInvisibleZone(sControlledCharacter_5C1B8C) && !sControlledCharacter_5C1B8C->mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit8_bInvisible) && !IsWallBetween_4BB8B0(this, sControlledCharacter_5C1B8C) && PSX_Rects_overlap_no_adjustment(&charRect, &bRect) && sControlledCharacter_5C1B8C->Type() != ReliveTypes::eSlig)
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Toggle(AnimFlags::eBit5_FlipX);
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

    BaseAliveGameObject* pBeatTarget = FindBeatTarget_4BD070(ReliveTypes::eRingOrLiftMud, 1);
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

    if (EventGet(kEventAlarm) && field_218_tlv_data.field_2E_panic_timeout)
    {
        ToPanicYelling_4BCBA0();
        return 108;
    }

    if (mBaseAnimatedWithPhysicsGameObject_VelX > FP_FromInteger(0))
    {
        if ((ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(2) + mBaseAnimatedWithPhysicsGameObject_XPos) >= FP_FromInteger(field_138_zone_rect.w))
        {
            PauseALittle_4BDD00();
            return 108;
        }
    }

    if (mBaseAnimatedWithPhysicsGameObject_VelX < FP_FromInteger(0))
    {
        if ((mBaseAnimatedWithPhysicsGameObject_XPos - (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(2))) <= FP_FromInteger(field_138_zone_rect.x))
        {
            PauseALittle_4BDD00();
            return 108;
        }
    }

    if (mBaseAnimatedWithPhysicsGameObject_VelX == FP_FromInteger(0) && mCurrentMotion == eSligMotions::M_StandIdle_0_4B4EC0 && mNextMotion != eSligMotions::M_Walking_2_4B5BC0)
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
        gMap.Is_Point_In_Current_Camera(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0) &&
        gMap.Is_Point_In_Current_Camera(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0) && (!field_15E_spotted_possessed_slig || sControlledCharacter_5C1B8C->Type() != ReliveTypes::eSlig) &&
        !IsAbeEnteringDoor_4BB990(sControlledCharacter_5C1B8C) && !EventGet(kEventResetting) && sControlledCharacter_5C1B8C->Type() != ReliveTypes::eGlukkon)
    {
        RespondToEnemyOrPatrol_4B3140();
    }
    else
    {
        auto pShooter = static_cast<BaseAliveGameObject*>(EventGet(kEventShooting));
        if (pShooter && pShooter->mBaseAnimatedWithPhysicsGameObject_SpriteScale == mBaseAnimatedWithPhysicsGameObject_SpriteScale && gMap.Is_Point_In_Current_Camera(pShooter->mBaseAnimatedWithPhysicsGameObject_LvlNumber, pShooter->mBaseAnimatedWithPhysicsGameObject_PathNumber, pShooter->mBaseAnimatedWithPhysicsGameObject_XPos, pShooter->mBaseAnimatedWithPhysicsGameObject_YPos, 0) && pShooter == sControlledCharacter_5C1B8C && gMap.Is_Point_In_Current_Camera(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0) && !EventGet(kEventResetting))
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
        else if (VOnSameYLevel(sControlledCharacter_5C1B8C) && VIsFacingMe(sControlledCharacter_5C1B8C) && !IsWallBetween_4BB8B0(this, sControlledCharacter_5C1B8C) && EventGet(kEventAbeOhm) && gMap.Is_Point_In_Current_Camera(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0) && gMap.Is_Point_In_Current_Camera(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0) && !EventGet(kEventResetting))
        {
            ToShoot_4BF9A0();
        }
        else if (EventGet(kEventAbeOhm) && !EventGet(kEventResetting) && field_218_tlv_data.field_2E_panic_timeout)
        {
            ToPanicYelling_4BCBA0();
        }
        else if (sActiveHero->mHealth <= FP_FromInteger(0))
        {
            ToAbeDead_4B3580();
        }
        else
        {
            auto pNoiseOrSpeaker = static_cast<BaseAliveGameObject*>(EventGet(kEventSuspiciousNoise));
            if (!pNoiseOrSpeaker)
            {
                pNoiseOrSpeaker = static_cast<BaseAliveGameObject*>(EventGet(kEventSpeaking));
            }

            if (pNoiseOrSpeaker && pNoiseOrSpeaker->mBaseAnimatedWithPhysicsGameObject_SpriteScale == mBaseAnimatedWithPhysicsGameObject_SpriteScale && gMap.Is_Point_In_Current_Camera(pNoiseOrSpeaker->mBaseAnimatedWithPhysicsGameObject_LvlNumber, pNoiseOrSpeaker->mBaseAnimatedWithPhysicsGameObject_PathNumber, pNoiseOrSpeaker->mBaseAnimatedWithPhysicsGameObject_XPos, pNoiseOrSpeaker->mBaseAnimatedWithPhysicsGameObject_YPos, 0) && pNoiseOrSpeaker != this && gMap.Is_Point_In_Current_Camera(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0) && !EventGet(kEventResetting))
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
            else if (sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_SpriteScale > mBaseAnimatedWithPhysicsGameObject_SpriteScale && VIsFacingMe(sControlledCharacter_5C1B8C) && !IsInInvisibleZone(sControlledCharacter_5C1B8C) && !sControlledCharacter_5C1B8C->mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit8_bInvisible) && gMap.Is_Point_In_Current_Camera(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0) && !InZCover_4BB7C0(sControlledCharacter_5C1B8C) && !InZCover_4BB7C0(this) && !EventGet(kEventResetting))
            {
                ToZShoot_4BF9E0();
            }
            else
            {
                if (Math_NextRandom() < field_218_tlv_data.field_3E_percent_beat_mud && FindBeatTarget_4BD070(ReliveTypes::eRingOrLiftMud, 2) && mCurrentMotion)
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
    if (mCurrentMotion == eSligMotions::M_TurnAroundStanding_5_4B6390 && mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        GoAlertedOrSayWhat_4BF140();
        return 123;
    }

    if (ListenToGlukkonCommands_4B95D0())
    {
        ToUnderGlukkonCommand_4B9660();
        return 123;
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 4)
    {
        bool tryTurningToPlayer = true;
        if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
        {
            if (sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_VelX >= FP_FromInteger(0) && (sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_VelX > FP_FromInteger(0) || sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_XPos >= mBaseAnimatedWithPhysicsGameObject_XPos))
            {
                tryTurningToPlayer = false;
            }
        }
        else if (sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_VelX <= FP_FromInteger(0) && (sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_VelX > FP_FromInteger(0) || sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_XPos <= mBaseAnimatedWithPhysicsGameObject_XPos))
        {
            tryTurningToPlayer = false;
        }

        if (tryTurningToPlayer)
        {
            if (sControlledCharacter_5C1B8C->Type() != ReliveTypes::eGlukkon && !IsInInvisibleZone(sControlledCharacter_5C1B8C) && !sControlledCharacter_5C1B8C->mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit8_bInvisible))
            {
                const PSX_RECT bRect = VGetBoundingRect();
                const PSX_RECT bRectChar = sControlledCharacter_5C1B8C->VGetBoundingRect();

                if (PSX_Rects_overlap_no_adjustment(&bRectChar, &bRect))
                {
                    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Toggle(AnimFlags::eBit5_FlipX);
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
        gMap.Is_Point_In_Current_Camera(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0) && (sControlledCharacter_5C1B8C->Type() != ReliveTypes::eSlig && !field_15E_spotted_possessed_slig) &&
        !IsAbeEnteringDoor_4BB990(sControlledCharacter_5C1B8C) && !EventGet(kEventResetting) && sControlledCharacter_5C1B8C->Type() != ReliveTypes::eGlukkon)
    {
        RespondToEnemyOrPatrol_4B3140();
    }
    // Panic?
    else if ((EventGet(kEventAbeOhm) || EventGet(kEventAlarm)) && !EventGet(kEventResetting) && field_218_tlv_data.field_2E_panic_timeout)
    {
        ToPanicYelling_4BCBA0();
    }
    else
    {
        // If some fool is trying to shoot us then shoot back
        auto pShootingSlig = static_cast<BaseAliveGameObject*>(EventGet(kEventShooting));
        if (pShootingSlig && pShootingSlig->mBaseAnimatedWithPhysicsGameObject_SpriteScale == mBaseAnimatedWithPhysicsGameObject_SpriteScale && gMap.Is_Point_In_Current_Camera(pShootingSlig->mBaseAnimatedWithPhysicsGameObject_LvlNumber, pShootingSlig->mBaseAnimatedWithPhysicsGameObject_PathNumber, pShootingSlig->mBaseAnimatedWithPhysicsGameObject_XPos, pShootingSlig->mBaseAnimatedWithPhysicsGameObject_YPos, 0) && pShootingSlig == sControlledCharacter_5C1B8C && gMap.Is_Point_In_Current_Camera(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0) && !EventGet(kEventResetting))
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
            auto pNoisyMud = static_cast<BaseAliveGameObject*>(EventGet(kEventSuspiciousNoise));
            if (!pNoisyMud)
            {
                pNoisyMud = static_cast<BaseAliveGameObject*>(EventGet(kEventSpeaking));
            }

            // Then kill them
            if (pNoisyMud && gMap.Is_Point_In_Current_Camera(pNoisyMud->mBaseAnimatedWithPhysicsGameObject_LvlNumber, pNoisyMud->mBaseAnimatedWithPhysicsGameObject_PathNumber, pNoisyMud->mBaseAnimatedWithPhysicsGameObject_XPos, pNoisyMud->mBaseAnimatedWithPhysicsGameObject_YPos, 0) && (pNoisyMud == sControlledCharacter_5C1B8C || pNoisyMud->Type() == ReliveTypes::eMudokon) && VOnSameYLevel(pNoisyMud) && VIsFacingMe(pNoisyMud) && (pNoisyMud != sControlledCharacter_5C1B8C || (!sControlledCharacter_5C1B8C->mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit8_bInvisible) && gMap.Is_Point_In_Current_Camera(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0) && !EventGet(kEventResetting))))
            {
                ToShoot_4BF9A0();
            }
            else
            {
                // Is a slig being noisy?
                auto pNoisySlig = static_cast<BaseAliveGameObject*>(EventGet(kEventSuspiciousNoise));
                if (!pNoisySlig)
                {
                    pNoisySlig = static_cast<BaseAliveGameObject*>(EventGet(kEventSpeaking));
                }

                // Then say what, stop walking to respond or try to kill them.
                if (pNoisySlig && gMap.Is_Point_In_Current_Camera(pNoisySlig->mBaseAnimatedWithPhysicsGameObject_LvlNumber, pNoisySlig->mBaseAnimatedWithPhysicsGameObject_PathNumber, pNoisySlig->mBaseAnimatedWithPhysicsGameObject_XPos, pNoisySlig->mBaseAnimatedWithPhysicsGameObject_YPos, 0) && (pNoisySlig == sControlledCharacter_5C1B8C || pNoisySlig->Type() != ReliveTypes::eSlig) && !VIsFacingMe(pNoisySlig) && gMap.Is_Point_In_Current_Camera(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0) && !EventGet(kEventResetting))
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
    if (mCurrentMotion != eSligMotions::M_ShootZ_42_4B7560 || !mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
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
    if (mCurrentMotion == 6 && mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
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
        if (!VOnSameYLevel(sControlledCharacter_5C1B8C) || !VIsFacingMe(sControlledCharacter_5C1B8C) || IsInInvisibleZone(sControlledCharacter_5C1B8C) || sControlledCharacter_5C1B8C->mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit8_bInvisible) || IsWallBetween_4BB8B0(this, sControlledCharacter_5C1B8C) || !gMap.Is_Point_In_Current_Camera(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0) || !gMap.Is_Point_In_Current_Camera(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0) || EventGet(kEventResetting))
        {
            PauseALittle_4BDD00();
            return 111;
        }

        if (!VIsFacingMe(sControlledCharacter_5C1B8C))
        {
            ToTurn_4BE090();
            return 111;
        }

        if (!gMap.Is_Point_In_Current_Camera(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0) && field_218_tlv_data.field_2A_chase_abe_when_spotted == Choice_short::eYes_1)
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
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
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
        else if (VOnSameYLevel(sControlledCharacter_5C1B8C) && VIsFacingMe(sControlledCharacter_5C1B8C) && VIsObjNearby(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(1), sControlledCharacter_5C1B8C) && !IsInInvisibleZone(sControlledCharacter_5C1B8C) && !sControlledCharacter_5C1B8C->mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit8_bInvisible) && !IsWallBetween_4BB8B0(this, sControlledCharacter_5C1B8C) && !EventGet(kEventResetting) && sControlledCharacter_5C1B8C->Type() != ReliveTypes::eGlukkon)
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

    if (VOnSameYLevel(sControlledCharacter_5C1B8C) && VIsFacingMe(sControlledCharacter_5C1B8C) && !IsInInvisibleZone(sControlledCharacter_5C1B8C) && !sControlledCharacter_5C1B8C->mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit8_bInvisible) && !IsWallBetween_4BB8B0(this, sControlledCharacter_5C1B8C) && gMap.Is_Point_In_Current_Camera(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0) && (!field_15E_spotted_possessed_slig || sControlledCharacter_5C1B8C->Type() != ReliveTypes::eSlig) && !EventGet(kEventResetting) && sControlledCharacter_5C1B8C->Type() != ReliveTypes::eGlukkon)
    {
        RespondToEnemyOrPatrol_4B3140();
        return 101;
    }

    if (VOnSameYLevel(sControlledCharacter_5C1B8C) && VIsFacingMe(sControlledCharacter_5C1B8C) && !IsInInvisibleZone(sControlledCharacter_5C1B8C) && !sControlledCharacter_5C1B8C->mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit8_bInvisible) && !IsWallBetween_4BB8B0(this, sControlledCharacter_5C1B8C) && gMap.Is_Point_In_Current_Camera(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0) && !EventGet(kEventResetting) && sControlledCharacter_5C1B8C->Type() != ReliveTypes::eGlukkon)
    {
        ToShoot_4BF9A0();
    }
    else if (sActiveHero->mHealth <= FP_FromInteger(0))
    {
        ToAbeDead_4B3580();
    }
    else
    {
        auto pNoiseOrSpeaking = static_cast<BaseAliveGameObject*>(EventGet(kEventSuspiciousNoise));
        if (!pNoiseOrSpeaking)
        {
            pNoiseOrSpeaking = static_cast<BaseAliveGameObject*>(EventGet(kEventSpeaking));
        }

        if (pNoiseOrSpeaking && pNoiseOrSpeaking->mBaseAnimatedWithPhysicsGameObject_SpriteScale == mBaseAnimatedWithPhysicsGameObject_SpriteScale && pNoiseOrSpeaking != this && field_120_timer <= static_cast<s32>(sGnFrame) && !EventGet(kEventResetting))
        {
            ToTurn_4BE090();
        }
        else if (sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_SpriteScale > mBaseAnimatedWithPhysicsGameObject_SpriteScale && VIsFacingMe(sControlledCharacter_5C1B8C) && !IsInInvisibleZone(sControlledCharacter_5C1B8C) && !sControlledCharacter_5C1B8C->mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit8_bInvisible) && gMap.Is_Point_In_Current_Camera(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0) && !InZCover_4BB7C0(sControlledCharacter_5C1B8C) && !InZCover_4BB7C0(this) && !EventGet(kEventResetting))
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
    auto pEvent = IsEventInRange(kEventSuspiciousNoise, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, AsEventScale(mBaseAnimatedWithPhysicsGameObject_Scale));
    if (pEvent)
    {
        auto pNoise = IsEventInRange(kEventNoise, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, AsEventScale(mBaseAnimatedWithPhysicsGameObject_Scale));
        if (pNoise)
        {
            pEvent = pNoise;
        }

        const FP wakeUpDistance = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(field_218_tlv_data.field_4A_noise_wake_up_distance);
        if (VIsObjNearby(wakeUpDistance, static_cast<BaseAnimatedWithPhysicsGameObject*>(pEvent)) && field_120_timer <= static_cast<s32>(sGnFrame) && gMap.Is_Point_In_Current_Camera(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0) && !EventGet(kEventResetting))
        {
            WakeUp_4B93B0();
            return 102;
        }
    }

    if (EventGet(kEventSpeaking) || EventGet(kEventLoudNoise))
    {
        if (pEvent != this && field_120_timer <= static_cast<s32>(sGnFrame) && gMap.Is_Point_In_Current_Camera(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0) && !EventGet(kEventResetting))
        {
            WakeUp_4B93B0();
            return 102;
        }
    }

    if (EventGet(kEventAlarm) && !EventGet(kEventResetting))
    {
        WakeUp_4B93B0();
        return 102;
    }

    ShouldStillBeAlive_4BBC00();

    if (mBaseGameObjectFlags.Get(BaseGameObject::eDead))
    {
        const CameraPos direction = gMap.GetDirection(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos);
        Start_Slig_sounds(direction, 0);
    }
    return 102;
}

// A leftover function from Abe's Oddysee.
s16 Slig::Brain_ChaseAndDisappear_35_4BF640()
{
    if (EventGet(kEventDeathReset))
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
            && gMap.Is_Point_In_Current_Camera(
                mBaseAnimatedWithPhysicsGameObject_LvlNumber,
                mBaseAnimatedWithPhysicsGameObject_PathNumber,
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
    field_10_resources_array.SetAt(10, ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kBigflashResID, 1, 0));
    field_10_resources_array.SetAt(11, ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kSligBlowResID, 1, 0));
    field_10_resources_array.SetAt(12, ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kShellResID, 1, 0));
    field_10_resources_array.SetAt(2, ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kSlgknbkResID, 1, 0));
    field_10_resources_array.SetAt(16, ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kSquibSmokeResID, 1, 0));

    if (!(field_218_tlv_data.field_48_disable_resources & 0x80))
    {
        field_10_resources_array.SetAt(6, ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kSlgknfdResID, 1, 0));
    }

    if (!(field_218_tlv_data.field_48_disable_resources & 0x100))
    {
        field_10_resources_array.SetAt(3, ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kSlgedgeResID, 1, 0));
    }

    if (!(field_218_tlv_data.field_48_disable_resources & 1))
    {
        field_10_resources_array.SetAt(7, ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kSlgleverResID, 1, 0));
    }

    if (!(field_218_tlv_data.field_48_disable_resources & 2))
    {
        field_10_resources_array.SetAt(8, ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kSlgliftResID, 1, 0));
    }

    if (!(field_218_tlv_data.field_48_disable_resources & 0x200))
    {
        field_10_resources_array.SetAt(4, ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kSlgsmashResID, 1, 0));
    }

    if (!(field_218_tlv_data.field_48_disable_resources & 0x400))
    {
        field_10_resources_array.SetAt(9, ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kSlgbeatResID, 1, 0));
    }

    if (!(field_218_tlv_data.field_48_disable_resources & 4))
    {
        field_10_resources_array.SetAt(5, ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kSlgzshotResID, 1, 0));
    }

    if (!(field_218_tlv_data.field_48_disable_resources & 0x40))
    {
        field_10_resources_array.SetAt(1, ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kSlgsleepResID, 1, 0));
    }

    if (!(field_218_tlv_data.field_48_disable_resources & 8))
    {
        field_10_resources_array.SetAt(13, ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kZflashResID, 1, 0));
    }

    if (!(field_218_tlv_data.field_48_disable_resources & 0x10))
    {
        field_10_resources_array.SetAt(15, ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kAbeknokzResID, 1, 0));
    }

    field_15E_spotted_possessed_slig = 0;
    field_120_timer = field_218_tlv_data.field_14_pause_time + sGnFrame;

    switch (field_218_tlv_data.field_12_start_state)
    {
        case Path_Slig::StartState::Patrol_1:
            SetBrain(&Slig::Brain_Inactive_32_4B9430);
            break;

        case Path_Slig::StartState::Sleeping_2:
            if (field_218_tlv_data.mTlvState && field_218_tlv_data.field_46_stay_awake == Choice_short::eYes_1)
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

                if (pObj->Type() == ReliveTypes::eGlukkon)
                {
                    auto pGlukkon = static_cast<BaseAliveGameObject*>(pObj);
                    if (gMap.Is_Point_In_Current_Camera(
                            pGlukkon->mBaseAnimatedWithPhysicsGameObject_LvlNumber,
                            pGlukkon->mBaseAnimatedWithPhysicsGameObject_PathNumber,
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
        mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit5_FlipX);
    }

    field_290_points_count = 0;

    field_268_points[field_290_points_count].x = FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos);
    field_268_points[field_290_points_count].y = FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos);
    field_290_points_count++;

    for (s16 yCam = -3; yCam < 4; yCam++)
    {
        for (s16 xCam = -3; xCam < 4; xCam++)
        {
            Path_TLV* pTlvIter = sPath_dword_BB47C0->Get_First_TLV_For_Offsetted_Camera(xCam, yCam);
            while (pTlvIter)
            {
                bool addPoint = false;
                if (pTlvIter->mTlvType32 == TlvTypes::SligBoundLeft_32)
                {
                    if (static_cast<Path_SligLeftBound*>(pTlvIter)->field_10_slig_bound_id == field_218_tlv_data.field_38_slig_bound_id)
                    {
                        field_138_zone_rect.x = pTlvIter->mTopLeft.x;
                        addPoint = true;
                    }
                }
                else if (pTlvIter->mTlvType32 == TlvTypes::SligBoundRight_45)
                {
                    if (static_cast<Path_SligRightBound*>(pTlvIter)->field_10_slig_bound_id == field_218_tlv_data.field_38_slig_bound_id)
                    {
                        field_138_zone_rect.w = pTlvIter->mTopLeft.x;
                        addPoint = true;
                    }
                }
                else if (pTlvIter->mTlvType32 == TlvTypes::SligPersist_46)
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
                        field_268_points[field_290_points_count].x = pTlvIter->mTopLeft.x;
                        field_268_points[field_290_points_count].y = pTlvIter->mTopLeft.y;
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

        if (gMap.mLevel != EReliveLevelIds::eMenu)
        {
            gMap.SetActiveCam(
                field_146_return_level,
                field_148_return_path,
                field_14A_return_camera,
                CameraSwapEffects::eInstantChange_0,
                0,
                0);
        }
    }

    Path_TLV* pTlv = sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam(field_118_tlvInfo);
    if (pTlv)
    {
        if (pTlv->mTlvType32.mType != TlvTypes::SligGetPants_104 && pTlv->mTlvType32.mType != TlvTypes::SligSpawner_37)
        {
            if (mHealth <= FP_FromInteger(0))
            {
                pTlv->mTlvFlags.Clear(TlvFlags::eBit1_Created);
                pTlv->mTlvFlags.Set(TlvFlags::eBit2_Destroyed);
            }
            else
            {
                pTlv->mTlvFlags.Clear(TlvFlags::eBit1_Created);
                pTlv->mTlvFlags.Clear(TlvFlags::eBit2_Destroyed);
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
            sCollisions->Raycast(
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(20),
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(20),
                &BaseAliveGameObjectCollisionLine,
                &mBaseAnimatedWithPhysicsGameObject_XPos,
                &mBaseAnimatedWithPhysicsGameObject_YPos,
                CollisionMask(static_cast<eLineTypes>(BaseAliveGameObjectCollisionLineType)));

            if (BaseAliveGameObjectCollisionLine)
            {
                if (BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eDynamicCollision_32 || BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eBackgroundDynamicCollision_36)
                {
                    const PSX_RECT bRect = VGetBoundingRect();
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
            mBaseAnimatedWithPhysicsGameObject_VelX = dword_5473E8[sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_4_dir >> 5];
            mBaseAnimatedWithPhysicsGameObject_VelY = dword_547408[sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_4_dir >> 5];

            if (sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed & 0x10)
            {
                mBaseAnimatedWithPhysicsGameObject_VelX += dword_5473E8[sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_4_dir >> 5];
                mBaseAnimatedWithPhysicsGameObject_VelX += dword_5473E8[sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_4_dir >> 5];
                mBaseAnimatedWithPhysicsGameObject_VelY += dword_547408[sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_4_dir >> 5];
            }

            mBaseAnimatedWithPhysicsGameObject_XPos += mBaseAnimatedWithPhysicsGameObject_VelX;
            mBaseAnimatedWithPhysicsGameObject_YPos += mBaseAnimatedWithPhysicsGameObject_VelY;

            // Keep in the map bounds
            PSX_Point mapBounds = {};
            gMap.Get_map_size(&mapBounds);

            if (mBaseAnimatedWithPhysicsGameObject_XPos < FP_FromInteger(0))
            {
                mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(0);
            }

            if (mBaseAnimatedWithPhysicsGameObject_XPos >= FP_FromInteger(mapBounds.x))
            {
                mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(mapBounds.x) - FP_FromInteger(1);
            }

            if (mBaseAnimatedWithPhysicsGameObject_YPos < FP_FromInteger(0))
            {
                mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(0);
            }

            if (mBaseAnimatedWithPhysicsGameObject_YPos >= FP_FromInteger(mapBounds.y))
            {
                mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(mapBounds.y) - FP_FromInteger(1);
            }
        }
        else
        {
            mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
            mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);
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
            BaseAliveGameObjectPathTLV = sPath_dword_BB47C0->TlvGetAt(
                nullptr,
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos,
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos);
            VOnTlvCollision(BaseAliveGameObjectPathTLV);
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
            mBaseAnimatedWithPhysicsGameObject_Anim.SetFrame(mBaseAliveGameObjectLastAnimFrame);
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

    field_146_return_level = gMap.mCurrentLevel;
    field_148_return_path = gMap.mCurrentPath;
    field_14A_return_camera = gMap.mCurrentCamera;

    MusicController::static_PlayMusic(MusicController::MusicTypes::ePossessed_9, this, 1, 0);
}

void Slig::VUnPosses()
{
    mNextMotion = eSligMotions::M_StandIdle_0_4B4EC0;
    field_120_timer = sGnFrame + 180;
    MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, this, 0, 0);
}

void Slig::VOnTlvCollision(Path_TLV* pTlv)
{
    while (pTlv)
    {
        if (pTlv->mTlvType32 == TlvTypes::DeathDrop_4)
        {
            if (mHealth > FP_FromInteger(0))
            {
                mHealth = FP_FromInteger(0);
                field_11C_brain_sub_state = 0;
                SetBrain(&Slig::Brain_DeathDropDeath_3_4BC1E0);
                mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);
                mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
                VSetMotion(eSligMotions::M_Falling_7_4B42D0);
                EventBroadcast(kEventMudokonComfort, this);
            }
        }
        pTlv = sPath_dword_BB47C0->TlvGetAt(pTlv, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos);
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

    mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(sSligAnimIdTable[mCurrentMotion], ppRes);
}

bool Slig::vUnderGlukkonCommand_4B1760()
{
    return BrainIs(&Slig::Brain_ListeningToGlukkon_4_4B9D20);
}

void Slig::WakeUp_4B93B0()
{
    mNextMotion = eSligMotions::M_SleepingToStand_33_4B8C50;
    SetBrain(&Slig::Brain_WakingUp_31_4B9390);

    MusicController::static_PlayMusic(MusicController::MusicTypes::eTension_4, this, 0, 0);
    Path_TLV* pTlv = sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
        field_218_tlv_data.mTopLeft.x,
        field_218_tlv_data.mTopLeft.y,
        field_218_tlv_data.mTopLeft.x,
        field_218_tlv_data.mTopLeft.y,
        TlvTypes::Slig_15);
    if (pTlv)
    {
        pTlv->mTlvState = 1; // TODO: Keep track of these, 1 must keep slig awake ??
    }
}

void Slig::ShouldStillBeAlive_4BBC00()
{
    if (!field_15C_force_alive_state)
    {
        // Check not falling and not in the current screen
        if (mCurrentMotion != eSligMotions::M_Falling_7_4B42D0
            && mCurrentMotion != eSligMotions::M_OutToFall_38_4B4570
            && !gMap.Is_Point_In_Current_Camera(
                mBaseAnimatedWithPhysicsGameObject_LvlNumber,
                mBaseAnimatedWithPhysicsGameObject_PathNumber,
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
                while (!gMap.Is_Point_In_Current_Camera(
                    mBaseAnimatedWithPhysicsGameObject_LvlNumber,
                    mBaseAnimatedWithPhysicsGameObject_PathNumber,
                    FP_FromInteger(field_268_points[i].x),
                    FP_FromInteger(field_268_points[i].y),
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
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
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
    mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
    mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);
    mCurrentMotion = eSligMotions::M_StandIdle_0_4B4EC0;
    field_128_input = 0;
    field_12C_timer = Math_RandomRange(0, 60) + sGnFrame + 120;
    MapFollowMe(TRUE);
}

void Slig::BlowToGibs_4B8020()
{
    relive_new Gibs(
        GibType::Slig_1,
        mBaseAnimatedWithPhysicsGameObject_XPos,
        mBaseAnimatedWithPhysicsGameObject_YPos,
        mBaseAnimatedWithPhysicsGameObject_VelX,
        mBaseAnimatedWithPhysicsGameObject_VelY,
        mBaseAnimatedWithPhysicsGameObject_SpriteScale,
        0);

    relive_new Blood(
        mBaseAnimatedWithPhysicsGameObject_XPos,
        mBaseAnimatedWithPhysicsGameObject_YPos - (FP_FromInteger(30) * mBaseAnimatedWithPhysicsGameObject_SpriteScale),
        FP_FromInteger(0),
        FP_FromInteger(0),
        mBaseAnimatedWithPhysicsGameObject_SpriteScale,
        20);

    New_Smoke_Particles(
        mBaseAnimatedWithPhysicsGameObject_XPos,
        mBaseAnimatedWithPhysicsGameObject_YPos - (FP_FromInteger(30) * mBaseAnimatedWithPhysicsGameObject_SpriteScale),
        mBaseAnimatedWithPhysicsGameObject_SpriteScale,
        3,
        128,
        128,
        128);

    if (mBaseAnimatedWithPhysicsGameObject_SpriteScale == FP_FromDouble(0.5))
    {
        SfxPlayMono(SoundEffect::KillEffect_64, 80);
        SfxPlayMono(SoundEffect::FallingItemHit_47, 60);
    }
    else
    {
        SfxPlayMono(SoundEffect::KillEffect_64, 127);
        SfxPlayMono(SoundEffect::FallingItemHit_47, 90);
    }

    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit3_Render);

    mCurrentMotion = eSligMotions::M_StandIdle_0_4B4EC0;
    mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);
    mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
    mHealth = FP_FromInteger(0);
    MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, this, 0, 0);
    field_120_timer = sGnFrame + 60;
    SetUpdateDelay(40);
    SetBrain(&Slig::Brain_Death_0_4BBFB0);
    EventBroadcast(kEventMudokonComfort, this);
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
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
            {
                mBaseAnimatedWithPhysicsGameObject_VelX = -(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(4));
            }
            else
            {
                mBaseAnimatedWithPhysicsGameObject_VelX = (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(4));
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
            mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
            mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);
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
                EventBroadcast(kEventSpeaking, this);
                return 1;
            }
        }
        return 1;
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        if (WallHit(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(45), -ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale)))
        {
            return 0;
        }
        mBaseAnimatedWithPhysicsGameObject_VelX = -(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(9));
    }
    else
    {
        if (WallHit(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(45), ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale)))
        {
            return 0;
        }
        mBaseAnimatedWithPhysicsGameObject_VelX = (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(9));
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
            StandToRunVelX = -(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(4));
            StandingToStepVelX = -(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(9));
            SteppingToStandVelX = -(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(6));
            offX1 = -(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(2));
            offX2 = -(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale));
            break;

        case MovementDirection::eRight:
            StandToRunVelX = (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(4));
            StandingToStepVelX = (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(9));
            SteppingToStandVelX = (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(6));
            offX1 = (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(2));
            offX2 = (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale));
            break;

        default:
            break;
    }

    if ((direction == MovementDirection::eLeft && !mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX)) || (direction == MovementDirection::eRight && mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX)))
    {
        mCurrentMotion = eSligMotions::M_TurnAroundStanding_5_4B6390;
        return 1;
    }

    if (sInputObject_5BD4E0.isPressed(sInputKey_Run_5550E8))
    {
        mBaseAnimatedWithPhysicsGameObject_VelX = StandToRunVelX;
        mCurrentMotion = eSligMotions::M_StandToRun_3_4B62F0;
        return 1;
    }

    if (!WallHit(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(45), offX1))
    {
        mBaseAnimatedWithPhysicsGameObject_VelX = StandingToStepVelX;
        mCurrentMotion = eSligMotions::M_StandingToStep_15_4B83B0;
        return 1;
    }

    if (WallHit(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(45), offX2))
    {
        return 0;
    }

    mBaseAnimatedWithPhysicsGameObject_VelX = SteppingToStandVelX;
    mCurrentMotion = eSligMotions::M_SteppingToStand_14_4B8480;
    return 1;
}

void Slig::PullLever()
{
    FP switchXPos = {};
    FP switchYPos = {};
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        switchXPos = mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5);
        switchYPos = mBaseAnimatedWithPhysicsGameObject_XPos - ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
    }
    else
    {
        switchXPos = mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5);
        switchYPos = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) + mBaseAnimatedWithPhysicsGameObject_XPos;
    }

    auto pSwitch = static_cast<Lever*>(FindObjectOfType(ReliveTypes::eLever, switchYPos, switchXPos));
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
    if (sInputObject_5BD4E0.isPressed(sInputKey_ThrowItem_5550F4) && mBaseAnimatedWithPhysicsGameObject_SpriteScale == FP_FromDouble(0.5) && !mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit10_Teleporting))
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
    if (!sInputObject_5BD4E0.isPressed(sInputKey_Down_5550DC) || mBaseAnimatedWithPhysicsGameObject_SpriteScale != FP_FromDouble(0.5) || mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit10_Teleporting))
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
    auto pLiftPoint = static_cast<LiftPoint*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    if (pLiftPoint && pLiftPoint->Type() == ReliveTypes::eLiftPoint)
    {
        const FP halfGrid = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(2);
        const FP midSwitchX = FP_FromInteger((BaseAliveGameObjectCollisionLine->mRect.x + BaseAliveGameObjectCollisionLine->mRect.w) / 2);
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
            SfxPlayMono(SoundEffect::PossessEffect_17, 0);
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
    if (!IsFacingEffectiveLeft_4BB780(this) && (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(2)) + mBaseAnimatedWithPhysicsGameObject_XPos >= FP_FromInteger(field_138_zone_rect.w))
    {
        PauseALittle_4BDD00();
        MusicController::static_PlayMusic(MusicController::MusicTypes::eTension_4, this, 0, 0);
        return;
    }

    // Left rect bound
    if (IsFacingEffectiveLeft_4BB780(this) && (mBaseAnimatedWithPhysicsGameObject_XPos - (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(2))) <= FP_FromInteger(field_138_zone_rect.x))
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

bool Slig::IsWallBetween_4BB8B0(BaseAliveGameObject* pLeft, BaseAliveGameObject* pRight)
{
    const PSX_RECT thisBRect =  VGetBoundingRect();
    const PSX_RECT rightBRect = pRight->VGetBoundingRect();

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    return sCollisions->Raycast(
               pLeft->mBaseAnimatedWithPhysicsGameObject_XPos,
               FP_FromInteger(thisBRect.h - 25),
               pRight->mBaseAnimatedWithPhysicsGameObject_XPos,
               FP_FromInteger(rightBRect.h - 25),
               &pLine, &hitX, &hitY, pLeft->mBaseAnimatedWithPhysicsGameObject_Scale == Scale::Fg ? kFgWalls : kBgWalls)
        == 1;
}

GameSpeakEvents Slig::LastGlukkonSpeak_4B3090()
{
    if (!gMap.Is_Point_In_Current_Camera(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 1))
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
    if (sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_SpriteScale != mBaseAnimatedWithPhysicsGameObject_SpriteScale)
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
    xy.x = FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos - FP_FromInteger(5));
    xy.y = FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5));

    PSX_Point wh;
    wh.x = FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos + FP_FromInteger(5));
    wh.y = FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(5));

    VOnCollisionWith(
        xy,
        wh,
        ObjList_5C1B78,
        1,
        (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection);
}

// TODO: rename to ToKnockBack() to match AO
void Slig::FallKnockBackOrSmash_4B4A90()
{
    Environment_SFX_457A40(EnvironmentSfx::eKnockback_13, 0, 0x7FFF, this);

    mBaseAnimatedWithPhysicsGameObject_XPos -= mBaseAnimatedWithPhysicsGameObject_VelX;

    if (BaseAliveGameObjectCollisionLine)
    {
        if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
        {
            if (!WallHit((mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(45)), (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(2))))
            {
                mBaseAnimatedWithPhysicsGameObject_VelX = (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(4));
            }
            else
            {
                mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
            }
        }
        else
        {
            if (!WallHit((mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(45)), (-ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(2))))
            {
                mBaseAnimatedWithPhysicsGameObject_VelX = -(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(4));
            }
            else
            {
                mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
            }
        }
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
    }

    if (mNextMotion == eSligMotions::M_Smash_44_4B6B90)
    {
        mBaseAnimatedWithPhysicsGameObject_VelX = -mBaseAnimatedWithPhysicsGameObject_VelX;
    }

    MapFollowMe(TRUE);

    if (mBaseAnimatedWithPhysicsGameObject_VelY < FP_FromInteger(0))
    {
        mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);
    }

    mCurrentMotion = eSligMotions::M_Knockback_34_4B68A0;
    field_12C_timer = sGnFrame + 10;
}

void Slig::TurnOrSayWhat_4BEBC0()
{
    if (EventGet(kEventSpeaking) && !IsInInvisibleZone(sControlledCharacter_5C1B8C) && !sControlledCharacter_5C1B8C->mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit8_bInvisible))
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
        case GameSpeakEvents::eDynamicCollision_32:
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
    if (EventGet(kEventSpeaking)
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
    s32 flipX = pSlig->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX);
    if (pSlig->mCurrentMotion == eSligMotions::M_TurnAroundStanding_5_4B6390 && pSlig->mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame > 4)
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
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));

    const FP xpos = mBaseAnimatedWithPhysicsGameObject_XPos;
    if (BaseAliveGameObjectCollisionLine)
    {
        BaseAliveGameObjectCollisionLine = BaseAliveGameObjectCollisionLine->MoveOnLine(
            &mBaseAnimatedWithPhysicsGameObject_XPos,
            &mBaseAnimatedWithPhysicsGameObject_YPos,
            mBaseAnimatedWithPhysicsGameObject_VelX);
        if (BaseAliveGameObjectCollisionLine)
        {
            if (pPlatform)
            {
                if (BaseAliveGameObjectCollisionLine->mLineType != eLineTypes::eDynamicCollision_32 && BaseAliveGameObjectCollisionLine->mLineType != eLineTypes::eBackgroundDynamicCollision_36)
                {
                    const auto oldMotion = mCurrentMotion;
                    VOnTrapDoorOpen();
                    mCurrentMotion = oldMotion;
                }
            }
            else if (BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eDynamicCollision_32 || BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eBackgroundDynamicCollision_36)
            {
                PlatformCollide_4B4E00();
            }
        }
        else
        {
            VOnTrapDoorOpen();
            mBaseAnimatedWithPhysicsGameObject_XPos = xpos + mBaseAnimatedWithPhysicsGameObject_VelX;
            BaseAliveGameObjectLastLineYPos = mBaseAnimatedWithPhysicsGameObject_YPos;
            mCurrentMotion = eSligMotions::M_FallingInitiate_39_4B4640;
            field_130_falling_velx_scale_factor = FP_FromDouble(0.3);

            if (mBaseAnimatedWithPhysicsGameObject_VelX > (FP_FromInteger(6) * mBaseAnimatedWithPhysicsGameObject_SpriteScale))
            {
                mBaseAnimatedWithPhysicsGameObject_VelX = (FP_FromInteger(6) * mBaseAnimatedWithPhysicsGameObject_SpriteScale);
            }

            if (mBaseAnimatedWithPhysicsGameObject_VelX < (-FP_FromInteger(6) * mBaseAnimatedWithPhysicsGameObject_SpriteScale))
            {
                mBaseAnimatedWithPhysicsGameObject_VelX = (-FP_FromInteger(6) * mBaseAnimatedWithPhysicsGameObject_SpriteScale);
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
    if ((mBaseAnimatedWithPhysicsGameObject_VelX > FP_FromInteger(0) && sInputObject_5BD4E0.isPressed(sInputKey_Left_5550D4)) || (mBaseAnimatedWithPhysicsGameObject_VelX < FP_FromInteger(0) && sInputObject_5BD4E0.isPressed(sInputKey_Right_5550D0)) || !sInputObject_5BD4E0.isPressed(sInputKey_Left_5550D4 | sInputKey_Right_5550D0))
    {
        // Then go to standing
        mCurrentMotion = eSligMotions::M_WalkToStand_18_4B5FC0;
    }
    field_128_input = 0;
}

void Slig::CheckPlatformVanished_4B3640()
{
    BaseGameObject* pLiftPoint = sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId);
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
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    if (pPlatform)
    {
        pPlatform->VRemove(this);
        BaseAliveGameObject_PlatformId = -1;
        BaseAliveGameObjectLastLineYPos = mBaseAnimatedWithPhysicsGameObject_YPos;
        VSetMotion(eSligMotions::M_OutToFall_38_4B4570);
    }
}

s16 Slig::MoveLift_4B3990(FP ySpeed)
{
    auto pLiftPoint = static_cast<LiftPoint*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    if (!pLiftPoint)
    {
        return eSligMotions::M_LiftGripping_48_4B3850;
    }

    pLiftPoint->vMove_4626A0(FP_FromInteger(0), ySpeed, 0);
    CheckPlatformVanished_4B3640();
    mBaseAnimatedWithPhysicsGameObject_VelY = pLiftPoint->mBaseAnimatedWithPhysicsGameObject_VelY;

    if (!mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame) && mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame != 5)
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
    if (FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_VelX))
    {
        MoveOnLine_4B4C40();

        if (mBaseAnimatedWithPhysicsGameObject_VelX <= FP_FromInteger(0))
        {
            if (mBaseAnimatedWithPhysicsGameObject_VelX < FP_FromInteger(0))
            {
                mBaseAnimatedWithPhysicsGameObject_VelX = (mBaseAnimatedWithPhysicsGameObject_SpriteScale * speed) + mBaseAnimatedWithPhysicsGameObject_VelX;
                if (mBaseAnimatedWithPhysicsGameObject_VelX > FP_FromInteger(0))
                {
                    mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
                }
            }
        }
        else
        {
            mBaseAnimatedWithPhysicsGameObject_VelX = mBaseAnimatedWithPhysicsGameObject_VelX - (mBaseAnimatedWithPhysicsGameObject_SpriteScale * speed);
            if (mBaseAnimatedWithPhysicsGameObject_VelX < FP_FromInteger(0))
            {
                mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
            }
        }
    }
}

void Slig::ToChase_4BCFF0()
{
    field_120_timer = sGnFrame + field_218_tlv_data.field_36_time_to_wait_before_chase;

    if (!VIsFacingMe(sControlledCharacter_5C1B8C))
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Toggle(AnimFlags::eBit5_FlipX);
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

    const FP width = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(directedGirdBlocks) + mBaseAnimatedWithPhysicsGameObject_XPos;
    auto pTlv = static_cast<Path_EnemyStopper*>(sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
        FP_GetExponent(width),
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos - ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale)),
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
    const FP curVelX = mBaseAnimatedWithPhysicsGameObject_VelX;

    if (curVelX > FP_FromInteger(0) && sInputObject_5BD4E0.isPressed(sInputKey_Left_5550D4))
    {
        mBaseAnimatedWithPhysicsGameObject_VelX = mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromDouble(13.2);
        mCurrentMotion = eSligMotions::M_SlidingTurn_9_4B6680;
        field_128_input = 0;
        return;
    }

    if (curVelX < FP_FromInteger(0) && sInputObject_5BD4E0.isPressed(sInputKey_Right_5550D0))
    {
        mBaseAnimatedWithPhysicsGameObject_VelX = (mBaseAnimatedWithPhysicsGameObject_SpriteScale * -FP_FromDouble(13.2));
        mCurrentMotion = eSligMotions::M_SlidingTurn_9_4B6680;
        field_128_input = 0;
        return;
    }

    if (!sInputObject_5BD4E0.isPressed(sInputKey_Left_5550D4 | sInputKey_Right_5550D0))
    {
        if (curVelX >= FP_FromInteger(0))
        {
            mBaseAnimatedWithPhysicsGameObject_VelX = mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromDouble(13.2);
        }
        else
        {
            mBaseAnimatedWithPhysicsGameObject_VelX = (mBaseAnimatedWithPhysicsGameObject_SpriteScale * -FP_FromDouble(13.2));
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

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        field_128_input = 0;
        mBaseAnimatedWithPhysicsGameObject_VelX = -(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(9));
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_VelX = (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(9));
        field_128_input = 0;
    }
}

void Slig::PlayerControlRunningSlideStopOrTurnFrame4_4B85D0()
{
    const FP curVelX = mBaseAnimatedWithPhysicsGameObject_VelX;
    if (curVelX > FP_FromInteger(0) && sInputObject_5BD4E0.isPressed(sInputKey_Left_5550D4))
    {
        mBaseAnimatedWithPhysicsGameObject_VelX = (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromDouble(13.2));
        mCurrentMotion = eSligMotions::M_SlidingTurn_9_4B6680;
        field_128_input = 0;
        return;
    }

    if (curVelX < FP_FromInteger(0) && sInputObject_5BD4E0.isPressed(sInputKey_Right_5550D0))
    {
        mBaseAnimatedWithPhysicsGameObject_VelX = (mBaseAnimatedWithPhysicsGameObject_SpriteScale * -FP_FromDouble(13.2));
        mCurrentMotion = eSligMotions::M_SlidingTurn_9_4B6680;
        field_128_input = 0;
        return;
    }

    if (!sInputObject_5BD4E0.isPressed(sInputKey_Left_5550D4 | sInputKey_Right_5550D0))
    {
        if (curVelX >= FP_FromInteger(0))
        {
            mBaseAnimatedWithPhysicsGameObject_VelX = mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromDouble(13.2);
        }
        else
        {
            mBaseAnimatedWithPhysicsGameObject_VelX = (mBaseAnimatedWithPhysicsGameObject_SpriteScale * -FP_FromDouble(13.2));
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

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        field_128_input = 0;
        mBaseAnimatedWithPhysicsGameObject_VelX = -(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(9));
    }
    else
    {
        field_128_input = 0;
        mBaseAnimatedWithPhysicsGameObject_VelX = (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(9));
    }
}

BaseAliveGameObject* Slig::FindBeatTarget_4BD070(ReliveTypes /*typeToFind*/, s32 gridBlocks)
{
    const FP kGridSize = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
    const FP k2Scaled = FP_FromInteger(2) * kGridSize;
    const FP kGridBlocksScaled = FP_FromInteger(gridBlocks) * kGridSize;

    const FP xAndW = mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX) ? mBaseAnimatedWithPhysicsGameObject_XPos - kGridBlocksScaled : mBaseAnimatedWithPhysicsGameObject_XPos + kGridBlocksScaled;

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

        if (pObj != this && pObj->Type() == ReliveTypes::eMudokon)
        {
            const PSX_RECT bRect = pObj->VGetBoundingRect();

            if (hitRect.w <= bRect.w && hitRect.x >= bRect.x && hitRect.y >= bRect.y && hitRect.h <= bRect.h && pObj->mBaseAnimatedWithPhysicsGameObject_Scale == mBaseAnimatedWithPhysicsGameObject_Scale && !IsInInvisibleZone(pObj) && !pObj->mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit8_bInvisible) && pObj->mHealth > FP_FromInteger(0))
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
        if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
        {
            if (mBaseAnimatedWithPhysicsGameObject_XPos > FP_FromInteger(field_138_zone_rect.x) + (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(4)))
            {
                ToTurn_4BE090();
                return;
            }
        }
        else
        {
            if (mBaseAnimatedWithPhysicsGameObject_XPos < FP_FromInteger(field_138_zone_rect.w) - (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(4)))
            {
                ToTurn_4BE090();
                return;
            }
        }
    }

    if (!(mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX)))
    {
        if (mBaseAnimatedWithPhysicsGameObject_XPos > FP_FromInteger(field_138_zone_rect.w) - (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(4)))
        {
            ToTurn_4BE090();
            return;
        }
    }
    else
    {
        if (mBaseAnimatedWithPhysicsGameObject_XPos < FP_FromInteger(field_138_zone_rect.x) + (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(4)))
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
        if (sControlledCharacter_5C1B8C->Type() != ReliveTypes::eSlig ||
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
        if (mBaseAnimatedWithPhysicsGameObject_SpriteScale == sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_SpriteScale)
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
    if (((pThis->Type() == ReliveTypes::eAbe) && (pThis->mCurrentMotion == eAbeMotions::Motion_114_DoorEnter_459470 && pThis->mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame > 7)) || (pThis->mCurrentMotion == eAbeMotions::Motion_115_DoorExit_459A40 && pThis->mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame < 4))
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

    FP xOff = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
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
    if (pObj->VIsObjNearby(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromDouble(0.5), sControlledCharacter_5C1B8C) || sControlledCharacter_5C1B8C->VIsFacingMe(pObj))
    {
        return 1;
    }

    return 0;
}

static s16 IsInZCover(Path_TLV* pTlv, const PSX_RECT* pRect)
{
    if (pTlv->mTlvType32 == TlvTypes::ZSligCover_50)
    {
        if (pRect->x >= pTlv->mTopLeft.x && pRect->x <= pTlv->mBottomRight.x &&

            pRect->y >= pTlv->mTopLeft.y && pRect->y <= pTlv->mBottomRight.y &&

            pRect->w >= pTlv->mTopLeft.x && pRect->w <= pTlv->mBottomRight.x &&

            pRect->h >= pTlv->mTopLeft.y && pRect->h <= pTlv->mBottomRight.y)
        {
            return TRUE;
        }
    }
    return FALSE;
}

s16 Slig::InZCover_4BB7C0(BaseAliveGameObject* pObj)
{
    const PSX_RECT bRect = pObj->VGetBoundingRect();
    return Bullet::InZBulletCover(pObj->mBaseAnimatedWithPhysicsGameObject_XPos, FP_FromInteger(bRect.y), bRect);
}

bool Slig::RenderLayerIs_4BBBC0(BaseAliveGameObject* pThis)
{
    return pThis->mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer == Layer::eLayer_BeforeWell_Half_3 || pThis->mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer == Layer::eLayer_BeforeWell_22;
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

        if (pObj != this && pObj->Type() == ReliveTypes::eSlig)
        {
            auto* pOtherSlig = static_cast<Slig*>(pObj);
            if (pOtherSlig->mBaseAnimatedWithPhysicsGameObject_SpriteScale == sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_SpriteScale && gMap.Is_Point_In_Current_Camera(pOtherSlig->mBaseAnimatedWithPhysicsGameObject_LvlNumber, pOtherSlig->mBaseAnimatedWithPhysicsGameObject_PathNumber, pOtherSlig->mBaseAnimatedWithPhysicsGameObject_XPos, pOtherSlig->mBaseAnimatedWithPhysicsGameObject_YPos, 0) && NearOrFacingActiveChar_4B9930(pOtherSlig) && (glukkonSpeak == GameSpeakEvents::Glukkon_Hey_36 || pOtherSlig->BrainIs(&Slig::Brain_ListeningToGlukkon_4_4B9D20)))
            {
                if (VIsObjNearby(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromDouble(0.5), pOtherSlig))
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
        case ReliveTypes::eBullet:
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
                        const FP yPos = mBaseAnimatedWithPhysicsGameObject_YPos - (FP_FromInteger(25) * mBaseAnimatedWithPhysicsGameObject_SpriteScale);
                        const FP xPos = mBaseAnimatedWithPhysicsGameObject_SpriteScale * (pBullet->field_30_x_distance <= FP_FromInteger(0) ? FP_FromInteger(-6) : FP_FromInteger(6));
                        relive_new Blood(xPos + mBaseAnimatedWithPhysicsGameObject_XPos, yPos, xOff, yOff, mBaseAnimatedWithPhysicsGameObject_SpriteScale, 12);
                    }

                    {
                        const FP xOff = pBullet->field_30_x_distance <= FP_FromInteger(0) ? FP_FromInteger(-6) : FP_FromInteger(6);
                        const FP yPos = mBaseAnimatedWithPhysicsGameObject_YPos - (FP_FromInteger(25) * mBaseAnimatedWithPhysicsGameObject_SpriteScale);
                        const FP xPos = mBaseAnimatedWithPhysicsGameObject_SpriteScale * (pBullet->field_30_x_distance <= FP_FromInteger(0) ? FP_FromInteger(-12) : FP_FromInteger(12));
                        relive_new Blood(xPos + mBaseAnimatedWithPhysicsGameObject_XPos, yPos, xOff, FP_FromInteger(0), mBaseAnimatedWithPhysicsGameObject_SpriteScale, 8);
                    }
                    break;
                }

                case BulletType::ePossessedSligZBullet_1:
                case BulletType::eZBullet_3:
                {
                    const PSX_RECT myRect = VGetBoundingRect();
                    const FP rectY = FP_FromInteger(myRect.y);

                    Path_TLV* pTlvIter = nullptr;
                    for (;;)
                    {
                        pTlvIter = sPath_dword_BB47C0->TlvGetAt(pTlvIter, mBaseAnimatedWithPhysicsGameObject_XPos, rectY, mBaseAnimatedWithPhysicsGameObject_XPos, rectY);
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

                    if (!gMap.Is_Point_In_Current_Camera(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, rectY, 0))
                    {
                        mBaseAliveGameObjectFlags.Clear(Flags_114::e114_Bit1_bShot);
                        mHealth = FP_FromInteger(1);
                        return 0;
                    }

                    relive_new Blood(mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos - (FP_FromInteger(25) * mBaseAnimatedWithPhysicsGameObject_SpriteScale), FP_FromInteger(0), FP_FromInteger(0), mBaseAnimatedWithPhysicsGameObject_SpriteScale, 25);
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
                        mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromDouble(-0.001);
                        mHealth = FP_FromInteger(0);
                        EventBroadcast(kEventMudokonComfort, this);
                        return 1;
                    }
                    mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromDouble(0.001);
                }
            }

            mHealth = FP_FromInteger(0);
            EventBroadcast(kEventMudokonComfort, this);
            return 1;
        }

        case ReliveTypes::eDrill:
        case ReliveTypes::eBaseBomb:
        case ReliveTypes::eExplosion:
        {
            if (!mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit3_Render))
            {
                return 1;
            }

            relive_new Gibs(GibType::Slig_1, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, mBaseAnimatedWithPhysicsGameObject_VelX, mBaseAnimatedWithPhysicsGameObject_VelY, mBaseAnimatedWithPhysicsGameObject_SpriteScale, 0);
            mHealth = FP_FromInteger(0);
            SfxPlayMono(SoundEffect::FallingItemHit_47, 90);
            mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit2_Animate);
            mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit3_Render);
            SetUpdateDelay(40);
            SetBrain(&Slig::Brain_ReturnControlToAbeAndDie_1_4BC410);
            mCurrentMotion = eSligMotions::M_StandIdle_0_4B4EC0;
            vUpdateAnim_4B1320();
            EventBroadcast(kEventMudokonComfort, this);
            return 1;
        }

        case ReliveTypes::eElectricWall:
            Slig_GameSpeak_SFX_4C04F0(SligSpeak::eHelp_10, 0, field_11E_pitch_min, this);
            return 1;

        case ReliveTypes::eAbe:
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

            if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
            {
                mBaseAnimatedWithPhysicsGameObject_XPos = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) + mBaseAnimatedWithPhysicsGameObject_XPos;
            }
            else
            {
                mBaseAnimatedWithPhysicsGameObject_XPos = mBaseAnimatedWithPhysicsGameObject_XPos - ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
            }
            FallKnockBackOrSmash_4B4A90();
            mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
            return 1;
        }

        case ReliveTypes::eAbilityRing:
            return 1;

        case ReliveTypes::eSlog:
        {
            if (mHealth <= FP_FromInteger(0) && (mCurrentMotion == eSligMotions::M_Knockback_34_4B68A0 || mCurrentMotion == eSligMotions::M_Smash_44_4B6B90))
            {
                return 1;
            }

            mBaseAliveGameObjectFlags.Set(Flags_114::e114_Bit1_bShot);
            mHealth = FP_FromInteger(0);

            SetBrain(&Slig::Brain_Death_0_4BBFB0);

            Environment_SFX_457A40(EnvironmentSfx::eKnockback_13, 0, 0x7FFF, this);

            EventBroadcast(kEventMudokonComfort, this);

            if (VIsFacingMe(static_cast<BaseAliveGameObject*>(pFrom)))
            {
                if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
                {
                    mBaseAnimatedWithPhysicsGameObject_VelX = (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(4));
                }
                else
                {
                    mBaseAnimatedWithPhysicsGameObject_VelX = -(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(4));
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

        case ReliveTypes::eElectrocute:
            if (mHealth <= FP_FromInteger(0))
            {
                return 1;
            }
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            mHealth = FP_FromInteger(0);
            EventBroadcast(kEventMudokonComfort, this);
            return 1;

        default:
            if (mHealth <= FP_FromInteger(0))
            {
                return 1;
            }
            SfxPlayMono(SoundEffect::KillEffect_64, 127);
            SfxPlayMono(SoundEffect::FallingItemHit_47, 90);
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
    EventBroadcast(kEventMudokonComfort, this);
    return 1;
}

s16 Slig::vIsFacingMe_4B23D0(BaseAnimatedWithPhysicsGameObject* pWho)
{
    if (mCurrentMotion != eSligMotions::M_TurnAroundStanding_5_4B6390 || mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame < 6)
    {
        if (pWho->mBaseAnimatedWithPhysicsGameObject_XPos <= mBaseAnimatedWithPhysicsGameObject_XPos && mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
        {
            return 1;
        }

        if (pWho->mBaseAnimatedWithPhysicsGameObject_XPos >= mBaseAnimatedWithPhysicsGameObject_XPos && !mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
        {
            return 1;
        }
    }
    else
    {
        if (pWho->mBaseAnimatedWithPhysicsGameObject_XPos <= mBaseAnimatedWithPhysicsGameObject_XPos && !mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
        {
            return 1;
        }

        if (pWho->mBaseAnimatedWithPhysicsGameObject_XPos >= mBaseAnimatedWithPhysicsGameObject_XPos && mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
        {
            return 1;
        }
    }
    return 0;
}

s16 Slig::vOnSameYLevel_4BB6C0(BaseAnimatedWithPhysicsGameObject* pOther)
{
    const PSX_RECT bOurRect = VGetBoundingRect();
    const PSX_RECT bTheirRect = pOther->VGetBoundingRect();

    return ((bTheirRect.h + bTheirRect.y) / 2) <= bOurRect.h && bTheirRect.h >= bOurRect.y && mBaseAnimatedWithPhysicsGameObject_Scale == pOther->mBaseAnimatedWithPhysicsGameObject_Scale;
}

s16 Slig::FindLiftPoint_4B9B40()
{
    const FP k2Scaled = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(2);

    PlatformBase* pLift = nullptr;
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        pLift = static_cast<PlatformBase*>(FindObjectOfType(ReliveTypes::eLiftPoint, mBaseAnimatedWithPhysicsGameObject_XPos - k2Scaled, mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(5)));
    }
    else
    {
        pLift = static_cast<PlatformBase*>(FindObjectOfType(ReliveTypes::eLiftPoint, k2Scaled + mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(5)));
    }

    if (pLift)
    {
        const PSX_RECT rect = pLift->VGetBoundingRect();

        const FP liftMidXPos = FP_FromInteger((rect.x + rect.w) / 2);
        return FP_Abs(mBaseAnimatedWithPhysicsGameObject_XPos - liftMidXPos) < k2Scaled;
    }

    return FALSE;
}
