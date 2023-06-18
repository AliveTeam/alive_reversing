#include "stdafx.h"
#include "Slig.hpp"
#include "../relive_lib/Function.hpp"
#include "../relive_lib/Collisions.hpp"
#include "../relive_lib/Shadow.hpp"
#include "../relive_lib/GameObjects/ShadowZone.hpp"
#include "stdlib.hpp"
#include "Map.hpp"
#include "Abe.hpp"
#include "MusicController.hpp"
#include "DDCheat.hpp"
#include "../relive_lib/GameObjects/BaseGameObject.hpp"
#include "CrawlingSlig.hpp" // TODO: SFX playing only
#include "SnoozeParticle.hpp"
#include "../relive_lib/Events.hpp"
#include "Sfx.hpp"
#include "Blood.hpp"
#include "Gibs.hpp"
#include "../relive_lib/GameObjects/Particle.hpp"
#include "../relive_lib/Sound/Midi.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "PlatformBase.hpp"
#include "ScreenShake.hpp"
#include "../relive_lib/GameObjects/ScreenManager.hpp"
#include "LiftPoint.hpp"
#include "Lever.hpp"
#include "Bullet.hpp"
#include "Dove.hpp"
#include "BulletShell.hpp"
#include "GameSpeak.hpp"
#include "AmbientSound.hpp"
#include "Electrocute.hpp"
#include "Grid.hpp"
#include "AnimationCallBacks.hpp"
#include "../relive_lib/FatalError.hpp"
#include <algorithm>
#include "../relive_lib/FixedPoint.hpp"
#include "Input.hpp"
#include "Path.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "Game.hpp"
#include "QuikSave.hpp"

static const relive::SfxDefinition kSfxInfoTable[17] = {
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

void Slig_SoundEffect(SligSfx effect, BaseAliveGameObject* pObj)
{
    const relive::SfxDefinition& pEffect = kSfxInfoTable[static_cast<s32>(effect)];
    s16 vLeft = 0;
    s16 vRight = 0;
    if (Calc_Slig_Sound_Direction(pObj, 0, pEffect, &vLeft, &vRight))
    {
        s16 pitch = 0;
        if (effect == SligSfx::ePropeller1_9 || effect == SligSfx::ePropeller2_10 || effect == SligSfx::ePropeller3_11)
        {
            FP sndDistance = FP_FromInteger(abs(Math_Distance(0, 0, FP_GetExponent(pObj->mVelX), FP_GetExponent(pObj->mVelY))));
            if (sndDistance > FP_FromInteger(8))
            {
                sndDistance = FP_FromInteger(8);
            }

            pitch = Math_RandomRange(-127, 127) + FP_GetExponent((sndDistance / FP_FromInteger(8)) * FP_FromInteger(768)) + 512;
        }
        else
        {
            pitch = Math_RandomRange(pEffect.mPitchMin, pEffect.mPitchMax);
        }
        SFX_SfxDefinition_Play_Stereo(pEffect, vLeft, vRight, pitch, pitch);
    }
}

void Animation_OnFrame_Slig(BaseGameObject* pObj, u32&, const IndexedPoint& point)
{
    auto pSlig = reinterpret_cast<Slig*>(pObj);

    if (pSlig->UpdateDelay())
    {
        return;
    }

    BulletType bulletType = BulletType::eSligPossessedOrUnderGlukkonCommand_0;
    if (pSlig->GetPossessed() || pSlig->vUnderGlukkonCommand_4B1760())
    {
        bulletType = BulletType::eSligPossessedOrUnderGlukkonCommand_0;
    }
    else
    {
        bulletType = BulletType::eNormalBullet_2;
    }

    const FP xOff = (pSlig->GetSpriteScale() * FP_FromInteger(point.mPoint.x));
    const FP yOff = (pSlig->GetSpriteScale() * FP_FromInteger(point.mPoint.y));

    Bullet* pBullet = nullptr;

    FP bullet_xDist = FP_FromInteger(0);
    s8 fireDirection = 0;
    FP fireXpos = FP_FromInteger(0);
    s16 shellDirection = 0;
    if (pSlig->GetAnimation().GetFlipX())
    {
        bullet_xDist = FP_FromInteger(-640);
        fireDirection = 1;
        fireXpos = pSlig->mXPos - xOff;
        shellDirection = 0;
    }
    else
    {
        bullet_xDist = FP_FromInteger(640);
        fireDirection = 0;
        fireXpos = xOff + pSlig->mXPos;
        shellDirection = 1;
    }

    pBullet = relive_new Bullet(pSlig, bulletType, pSlig->mXPos, yOff + pSlig->mYPos, bullet_xDist, pSlig->GetSpriteScale(), 0);
    if (pBullet)
    {
        pBullet->SetUpdateDelay(1);
    }

    New_ShootingFire_Particle(fireXpos, yOff + pSlig->mYPos, fireDirection, pSlig->GetSpriteScale());

    if (pSlig->GetSpriteScale() == FP_FromDouble(0.5))
    {
        SfxPlayMono(relive::SoundEffects::SligShoot, 85);
    }
    else
    {
        relive_new BulletShell(pSlig->mXPos, yOff + pSlig->mYPos, shellDirection, pSlig->GetSpriteScale());
        SfxPlayMono(relive::SoundEffects::SligShoot, 0);
    }

    EventBroadcast(kEventShooting, pSlig);
    EventBroadcast(kEventLoudNoise, pSlig);

    Dove::All_FlyAway(false);
    return;
}

static const TintEntry sSligTint[16] = {
    {EReliveLevelIds::eMenu, 102u, 127u, 118u},
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
    {EReliveLevelIds::eCredits, 102u, 127u, 118u}};

s16 sSligsUnderControlCount_BAF7E8 = 0;

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

const TSligMotionFn sSligMotionTable[52] = {
    &Slig::Motion_0_StandIdle,
    &Slig::Motion_1_StandToWalk,
    &Slig::Motion_2_Walking,
    &Slig::Motion_3_StandToRun,
    &Slig::Motion_4_Running,
    &Slig::Motion_5_TurnAroundStanding,
    &Slig::Motion_6_Shoot,
    &Slig::Motion_7_Falling,
    &Slig::Motion_8_SlidingToStand,
    &Slig::Motion_9_SlidingTurn,
    &Slig::Motion_10_SlidingTurnToWalk,
    &Slig::Motion_11_SlidingTurnToRun,
    &Slig::Motion_12_ReloadGun,
    &Slig::Motion_13_ShootToStand,
    &Slig::Motion_14_SteppingToStand,
    &Slig::Motion_15_StandingToStep,
    &Slig::Motion_16_DepossessingAbort,
    &Slig::Motion_17_GameSpeak,
    &Slig::Motion_18_WalkToStand,
    &Slig::Motion_19_Recoil,
    &Slig::Motion_20_SpeakHereBoy,
    &Slig::Motion_21_SpeakHi,
    &Slig::Motion_22_SpeakFreeze,
    &Slig::Motion_23_SpeakGetHim,
    &Slig::Motion_24_SpeakLaugh,
    &Slig::Motion_25_SpeakBullshit1,
    &Slig::Motion_26_SpeakLookOut,
    &Slig::Motion_27_SpeakBullshit2,
    &Slig::Motion_28_SpeakPanic,
    &Slig::Motion_29_SpeakWhat,
    &Slig::Motion_30_SpeakAIFreeze,
    &Slig::Motion_31_Blurgh,
    &Slig::Motion_32_Sleeping,
    &Slig::Motion_33_SleepingToStand,
    &Slig::Motion_34_Knockback,
    &Slig::Motion_35_KnockbackToStand,
    &Slig::Motion_36_Depossessing,
    &Slig::Motion_37_Possess,
    &Slig::Motion_38_OutToFall,
    &Slig::Motion_39_FallingInitiate,
    &Slig::Motion_40_LandingSoft,
    &Slig::Motion_41_LandingFatal,
    &Slig::Motion_42_ShootZ,
    &Slig::Motion_43_ShootZtoStand,
    &Slig::Motion_44_Smash,
    &Slig::Motion_45_PullLever,
    &Slig::Motion_46_LiftGrip,
    &Slig::Motion_47_LiftUngrip,
    &Slig::Motion_48_LiftGripping,
    &Slig::Motion_49_LiftUp,
    &Slig::Motion_50_LiftDown,
    &Slig::Motion_51_Beat};

const TSligBrainFn sSligBrainTable[36] = {
    &Slig::Brain_0_Death,
    &Slig::Brain_1_ReturnControlToAbeAndDie,
    &Slig::Brain_2_Possessed,
    &Slig::Brain_3_DeathDropDeath,
    &Slig::Brain_4_ListeningToGlukkon,
    &Slig::Brain_Empty,
    &Slig::Brain_Empty,
    &Slig::Brain_7_SpottedEnemy,
    &Slig::Brain_Empty,
    &Slig::Brain_Empty,
    &Slig::Brain_10_EnemyDead,
    &Slig::Brain_11_KilledEnemy,
    &Slig::Brain_12_PanicTurning,
    &Slig::Brain_13_PanicRunning,
    &Slig::Brain_14_PanicYelling,
    &Slig::Brain_15_Idle,
    &Slig::Brain_16_StopChasing,
    &Slig::Brain_17_Chasing,
    &Slig::Brain_18_StartChasing,
    &Slig::Brain_19_Turning,
    &Slig::Brain_20_StoppingNextToMudokon,
    &Slig::Brain_21_Walking,
    &Slig::Brain_22_GetAlertedTurn,
    &Slig::Brain_23_GetAlerted,
    &Slig::Brain_24_BeatingUp,
    &Slig::Brain_25_DiscussionWhat,
    &Slig::Brain_Empty,
    &Slig::Brain_Empty,
    &Slig::Brain_28_ZShooting,
    &Slig::Brain_29_Shooting,
    &Slig::Brain_30_ZSpottedEnemy,
    &Slig::Brain_31_WakingUp,
    &Slig::Brain_32_Inactive,
    &Slig::Brain_33_Paused,
    &Slig::Brain_34_Sleeping,
    &Slig::Brain_35_ChaseAndDisappear};

AnimId Slig::MotionToAnimId(eSligMotions motion)
{
    const u32 motionU32 = static_cast<u32>(motion);
    if (motionU32 > ALIVE_COUNTOF(sSligAnimIdTable))
    {
        ALIVE_FATAL("Motion out of bounds");
    }

    if (motion == eSligMotions::Motion_44_Smash && mSligTlv.mData.mDeathMode == relive::Path_Slig_Data::DeathMode::StandIdle)
    {
        SetCurrentMotion(eSligMotions::Motion_0_StandIdle);
        return AnimId::Slig_Idle;
    }
    else if (motion == eSligMotions::Motion_51_Beat && !mSligTlv.mData.mCanBeat)
    {
        SetCurrentMotion(eSligMotions::Motion_0_StandIdle);
        return AnimId::Slig_Idle;
    }
    else if (motion == eSligMotions::Motion_42_ShootZ && !mSligTlv.mData.mCanZShoot)
    {
        SetCurrentMotion(eSligMotions::Motion_0_StandIdle);
        return AnimId::Slig_Idle;
    }

    return sSligAnimIdTable[motionU32];
}

void Slig::SetBrain(TSligBrainFn fn)
{
    mBrainState = fn;
}

bool Slig::BrainIs(TSligBrainFn fn)
{
    return mBrainState == fn;
}

void Slig::LoadAnimations()
{
    for (auto& animId : sSligAnimIdTable)
    {
        mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(animId));
    }
}

Slig::Slig(relive::Path_Slig* pTlv, const Guid& tlvId)
    : BaseAliveGameObject(17)
{
    field_160_last_event_index = -1;

    if (tlvId != Guid{})
    {
        mBaseGameObjectTlvInfo = tlvId;
    }

    LoadAnimations();
    Animation_Init(GetAnimRes(AnimId::Slig_Idle));

    field_178_mPal = std::make_shared<AnimationPal>();

    SetType(ReliveTypes::eSlig);

    SetPossessed(false);
    SetRestoredFromQuickSave(false);
    SetCanBePossessed(true);
    SetCanSetOffExplosives(true);

    mPreventDepossession &= ~1u;

    mFollowGlukkon = false;
    mGlukkonCalledAllOYa = false;
    mHeardGlukkon = false;


    field_120_timer = 0;
    field_11E_pitch_min = 45 * (Math_NextRandom() % 5 - 2);
    SetNextMotion(eSligMotions::Motion_0_StandIdle);
    mReturnToPreviousMotion = false;
    field_150_explode_timer = 0;
    field_14C_death_by_being_shot_timer = 0;
    BaseAliveGameObjectPathTLV = pTlv;

    mSligTlv = *pTlv;

    BaseAliveGameObject_PlatformId = Guid{};

    SetCurrentMotion(eSligMotions::Motion_7_Falling);

    mXPos = FP_FromInteger(pTlv->mTopLeftX);
    mYPos = FP_FromInteger(pTlv->mTopLeftY);
    mFallingVelxScaleFactor = FP_FromInteger(0);
    field_118_tlvInfo = tlvId;
    mInput = 0;
    field_158_num_times_to_shoot = 0;
    field_15C_force_alive_state = 0;
    field_136_shot_motion = eSligMotions::eNone_m1;
    field_208_glukkon_obj_id = Guid{};
    field_20C_state_after_speak = -1;
    field_20E_attention_timeout = 0;
    
    GetAnimation().SetFnPtrArray(gSlig_Anim_Frame_Fns);

    if (pTlv->mData.mScale != relive::reliveScale::eFull)
    {
        if (pTlv->mData.mScale == relive::reliveScale::eHalf)
        {
            SetSpriteScale(FP_FromDouble(0.5));
            GetAnimation().SetRenderLayer(Layer::eLayer_SligGreeterFartsBat_Half_14);
            SetScale(Scale::Bg);
        }
    }
    else
    {
        SetSpriteScale(FP_FromInteger(1));
        GetAnimation().SetRenderLayer(Layer::eLayer_SligGreeterFartsBats_33);
        SetScale(Scale::Fg);
    }

    SetBaseAnimPaletteTint(&sSligTint[0], gMap.mCurrentLevel, PalId::Default); // Note: seemingly pointless pal override removed

    FP hitX = {};
    FP hitY = {};
    if (gCollisions->Raycast(
            mXPos,
            mYPos,
            mXPos,
            mYPos + FP_FromInteger(24),
            &BaseAliveGameObjectCollisionLine,
            &hitX,
            &hitY,
            GetScale() == Scale::Fg ? kFgFloor : kBgFloor)
        == 1)
    {
        SetCurrentMotion(eSligMotions::Motion_0_StandIdle);
        mYPos = hitY;
    }

    MapFollowMe(true);

    Init();

    VStackOnObjectsOfType(ReliveTypes::eSlig);

    if (gMap.mCurrentLevel == EReliveLevelIds::eBonewerkz && gMap.mCurrentPath == 2 && gMap.mCurrentCamera == 5)
    {
        mXOffset = 0;
    }

    SetDoPurpleLightEffect(true);

    CreateShadow();
}

void renderWithGlowingEyes(PrimHeader** ot, BaseAliveGameObject* actor, std::shared_ptr<AnimationPal>& pal, s16 palSize, s16& r, s16& g, s16& b,
                           const s16* eyeColourIndices, s16 eyeColourIndicesSize)
{
    if (actor->GetAnimation().GetRender())
    {
        if (gMap.mCurrentPath == actor->mCurrentPath && gMap.mCurrentLevel == actor->mCurrentLevel && actor->Is_In_Current_Camera() == CameraPos::eCamCurrent_0)
        {
            actor->GetAnimation().SetSpriteScale(actor->GetSpriteScale());
            const PSX_RECT boundingRect = actor->VGetBoundingRect();
            s16 rShade = actor->mRGB.r;
            s16 gShade = actor->mRGB.g;
            s16 bShade = actor->mRGB.b;
            ShadowZone::ShadowZones_Calculate_Colour(
                FP_GetExponent(actor->mXPos),
                (boundingRect.h + boundingRect.y) / 2,
                actor->GetScale(),
                &rShade,
                &gShade,
                &bShade);
            if (!actor->GetElectrocuted())
            {
                if (rShade != r || rShade != g || rShade != b)
                {
                    r = rShade;
                    g = rShade;
                    b = rShade;

                    for (s32 i = 0; i < palSize; i++)
                    {
                        u32 auxPalValue = actor->GetAnimation().mAnimRes.mPngPtr->mPal->mPal[i].r;
                        u8 resultR = static_cast<u8>((auxPalValue * r) >> 7);

                        auxPalValue = actor->GetAnimation().mAnimRes.mPngPtr->mPal->mPal[i].g;
                        u8 resultG = static_cast<u8>((auxPalValue * g) >> 7);

                        auxPalValue = actor->GetAnimation().mAnimRes.mPngPtr->mPal->mPal[i].b;
                        u8 resultB = static_cast<u8>((auxPalValue * b) >> 7);

                        pal->mPal[i].a = actor->GetAnimation().mAnimRes.mPngPtr->mPal->mPal[i].a;
                        pal->mPal[i].r = resultR;
                        pal->mPal[i].g = resultG;
                        pal->mPal[i].b = resultB;

                        // We need to check the result here in case it ended
                        // up as transparent (RGB 0 STP 0)
                        //
                        // If the colour in the palette was not transparent
                        // then we bump the colour slightly to avoid this
                        // problem
                        if (
                            pal->mPal[i].IsTransparent() &&
                            !actor->GetAnimation().mAnimRes.mPngPtr->mPal->mPal[i].IsTransparent()
                        )
                        {
                            pal->mPal[i].r = 1;
                        }
                    }
                    for (s32 i = 0; i < eyeColourIndicesSize; i++)
                    {
                        pal->mPal[eyeColourIndices[i]] = actor->GetAnimation().mAnimRes.mPngPtr->mPal->mPal[eyeColourIndices[i]];
                    }

                    actor->GetAnimation().LoadPal(pal);
                }

                actor->GetAnimation().SetRGB(127, 127, 127);
            }

            actor->GetAnimation().VRender(
                FP_GetExponent(FP_FromInteger(actor->mXOffset) + actor->mXPos - gScreenManager->CamXPos()),
                FP_GetExponent(FP_FromInteger(actor->mYOffset) + actor->mYPos - gScreenManager->CamYPos()),
                ot,
                0,
                0);

            PSX_RECT rectToInvalidate = {};
            actor->GetAnimation().Get_Frame_Rect(&rectToInvalidate);


            if (actor->GetShadow())
            {
                actor->GetShadow()->Calculate_Position(actor->mXPos, actor->mYPos, &rectToInvalidate, actor->GetSpriteScale(), actor->GetScale());
                actor->GetShadow()->Render(ot);
            }
        }
    }
}

void Slig::VRender(PrimHeader** ot)
{
    const s16 eyeIndices[] = {61, 62};

    renderWithGlowingEyes(ot, this, field_178_mPal, 64, field_200_red, field_202_green, field_204_blue, &eyeIndices[0], ALIVE_COUNTOF(eyeIndices));
}

void Slig::VGetSaveState(SerializedObjectData& pSaveBuffer)
{
    if (GetElectrocuted())
    {
        return;
    }

    SligSaveState data = {};

    data.mType = ReliveTypes::eSlig;

    data.field_4_xpos = mXPos;
    data.field_8_ypos = mYPos;

    data.field_C_velx = mVelX;
    data.field_10_vely = mVelY;

    data.mFallingVelxScaleFactor = mFallingVelxScaleFactor;

    data.field_14_path_number = mCurrentPath;
    data.field_16_lvl_number = mCurrentLevel;
    data.field_18_sprite_scale = GetSpriteScale();

    data.field_1C_scale = GetScale();

    data.field_1E_r = mRGB.r;
    data.field_20_g = mRGB.g;
    data.field_22_b = mRGB.b;

    if (GetElectrocuting())
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
                if (pElectrocute->field_20_target_obj_id == mBaseGameObjectId)
                {
                    data.field_1E_r = pElectrocute->field_24_r;
                    data.field_20_g = pElectrocute->field_26_g;
                    data.field_22_b = pElectrocute->field_28_b;
                    break;
                }
            }
        }
    }
    data.field_24_bFlipX = GetAnimation().GetFlipX();
    data.field_26_current_motion = GetCurrentMotion();
    data.field_28_current_frame = static_cast<s16>(GetAnimation().GetCurrentFrame());
    data.field_2A_frame_change_counter = static_cast<s16>(GetAnimation().GetFrameChangeCounter());
    data.field_2D_bDrawable = GetDrawable();
    data.field_2C_bRender = GetAnimation().GetRender();
    data.field_30_health = mHealth;
    data.field_34_current_motion = GetCurrentMotion();
    data.field_36_next_motion = GetNextMotion();
    data.field_38_last_line_ypos = FP_GetExponent(BaseAliveGameObjectLastLineYPos);
    data.field_3A_collision_line_type = eLineTypes::eNone_m1;

    if (BaseAliveGameObjectCollisionLine)
    {
        data.field_3A_collision_line_type = BaseAliveGameObjectCollisionLine->mLineType;
    }

    data.field_40_bActiveChar = this == sControlledCharacter;
    data.field_42_brain_sub_state = mBrainSubState;
    data.field_44_pitch_min = field_11E_pitch_min;
    data.field_48_timer = field_120_timer;
    data.mReturnToPreviousMotion = mReturnToPreviousMotion;
    data.mCheckedIfOffScreen = mCheckedIfOffScreen;
    data.field_50_input = InputObject::KeyboardInputToPsxButtons_45EF70(mInput);
    data.field_54_timer = field_12C_timer;
    data.mFallingVelxScaleFactor = mFallingVelxScaleFactor;
    data.field_5C_tlvInfo = field_118_tlvInfo;
    //data.field_60_res_idx = field_134_res_idx;
    data.field_62_shot_motion = field_136_shot_motion;
    data.field_64_zone_rect = field_138_zone_rect;
    data.field_72_return_level = mAbeLevel;
    data.field_74_return_path = mAbePath;
    data.field_76_return_camera = mAbeCamera;
    data.field_78_death_by_being_shot_timer = field_14C_death_by_being_shot_timer;
    data.field_7C_explode_timer = field_150_explode_timer;
    data.field_80_brain_state_idx = 0;

    s32 idx = 0;
    for (const auto& fn : sSligBrainTable)
    {
        if (BrainIs(fn))
        {
            data.field_80_brain_state_idx = idx;
        }
        idx++;
    }

    data.field_8C_num_times_to_shoot = field_158_num_times_to_shoot;
    data.field_90_force_alive_state = field_15C_force_alive_state;
    data.field_92_spotted_possessed_slig = field_15E_spotted_possessed_slig;
    data.field_94_glukkon_id = Guid{};

    if (field_208_glukkon_obj_id != Guid{})
    {
        BaseGameObject* pGlukkon = sObjectIds.Find_Impl(field_208_glukkon_obj_id);
        if (pGlukkon)
        {
            data.field_94_glukkon_id = pGlukkon->mBaseGameObjectTlvInfo;
        }
    }
    data.field_98_state_after_speak = field_20C_state_after_speak;
    data.field_9A_attention_timeout = field_20E_attention_timeout;

    data.field_9E_next_command_arg1 = field_212_next_command_arg1;
    data.field_A0_cmd_idx = field_214_cmd_idx;

    data.mFollowGlukkon = mFollowGlukkon;
    data.mStoppedForLeverOrLift = mStoppedForLeverOrLift;
    data.mGlukkonCalledAllOYa = mGlukkonCalledAllOYa;
    data.mHeardGlukkon = mHeardGlukkon;

    pSaveBuffer.Write(data);
}

void Slig::CreateFromSaveState(SerializedObjectData& pBuffer)
{
    const auto pState = pBuffer.ReadTmpPtr<SligSaveState>();
    auto pTlv = static_cast<relive::Path_Slig*>(gPathInfo->TLV_From_Offset_Lvl_Cam(pState->field_5C_tlvInfo));

    auto pSlig = relive_new Slig(pTlv, pState->field_5C_tlvInfo);
    if (pSlig)
    {
        if (pState->field_40_bActiveChar)
        {
            sControlledCharacter = pSlig;
            pSlig->SetPossessed(true);
        }

        pSlig->BaseAliveGameObjectPathTLV = nullptr;
        pSlig->BaseAliveGameObjectCollisionLine = nullptr;
        pSlig->mXPos = pState->field_4_xpos;
        pSlig->mYPos = pState->field_8_ypos;
        pSlig->mVelX = pState->field_C_velx;
        pSlig->mVelY = pState->field_10_vely;
        pSlig->mCurrentPath = pState->field_14_path_number;
        pSlig->mCurrentLevel = pState->field_16_lvl_number;
        pSlig->SetSpriteScale(pState->field_18_sprite_scale);

        if (pSlig->GetSpriteScale() == FP_FromInteger(1))
        {
            pSlig->GetAnimation().SetRenderLayer(Layer::eLayer_SligGreeterFartsBats_33);
        }
        else
        {
            pSlig->GetAnimation().SetRenderLayer(Layer::eLayer_SligGreeterFartsBat_Half_14);
        }

        pSlig->SetScale(pState->field_1C_scale);

        pSlig->field_200_red = pState->field_1E_r;
        pSlig->field_202_green = pState->field_20_g;
        pSlig->field_204_blue = pState->field_22_b;

        pSlig->mRGB.SetRGB(pState->field_1E_r, pState->field_20_g, pState->field_22_b);

        pSlig->SetCurrentMotion(pState->field_26_current_motion);
        pSlig->GetAnimation().Set_Animation_Data(pSlig->GetAnimRes(pSlig->MotionToAnimId(pState->field_26_current_motion)));


        pSlig->GetAnimation().SetCurrentFrame(pState->field_28_current_frame);
        pSlig->GetAnimation().SetFrameChangeCounter(pState->field_2A_frame_change_counter);

        pSlig->SetDrawable(pState->field_2D_bDrawable & 1);

        pSlig->GetAnimation().SetFlipX(pState->field_24_bFlipX & 1);
        pSlig->GetAnimation().SetRender(pState->field_2C_bRender & 1);

        if (IsLastFrame(&pSlig->GetAnimation()))
        {
            pSlig->GetAnimation().SetIsLastFrame(true);
        }

        pSlig->mHealth = pState->field_30_health;
        pSlig->SetCurrentMotion(pState->field_34_current_motion);
        pSlig->SetNextMotion(pState->field_36_next_motion);
        pSlig->BaseAliveGameObjectLastLineYPos = FP_FromInteger(pState->field_38_last_line_ypos);

        pSlig->SetRestoredFromQuickSave(true);

        pSlig->BaseAliveGameObjectCollisionLineType = pState->field_3A_collision_line_type;
        pSlig->mBrainSubState = pState->field_42_brain_sub_state;
        pSlig->field_11E_pitch_min = pState->field_44_pitch_min;
        pSlig->field_120_timer = pState->field_48_timer;
        pSlig->mReturnToPreviousMotion = pState->mReturnToPreviousMotion;
        pSlig->mCheckedIfOffScreen = pState->mCheckedIfOffScreen;

        pSlig->mInput = InputObject::PsxButtonsToKeyboardInput_45EE40(pState->field_50_input);

        pSlig->field_12C_timer = pState->field_54_timer;
        pSlig->mFallingVelxScaleFactor = pState->mFallingVelxScaleFactor;
        pSlig->field_118_tlvInfo = pState->field_5C_tlvInfo;
        //pSlig->field_134_res_idx = pState->field_60_res_idx;
        pSlig->field_136_shot_motion = pState->field_62_shot_motion;

        pSlig->field_138_zone_rect = pState->field_64_zone_rect;

        pSlig->mAbeLevel = pState->field_72_return_level;
        pSlig->mAbePath = pState->field_74_return_path;
        pSlig->mAbeCamera = pState->field_76_return_camera;

        pSlig->field_14C_death_by_being_shot_timer = pState->field_78_death_by_being_shot_timer;
        pSlig->field_150_explode_timer = pState->field_7C_explode_timer;

        pSlig->SetBrain(sSligBrainTable[pState->field_80_brain_state_idx]);

        pSlig->field_158_num_times_to_shoot = static_cast<s16>(pState->field_8C_num_times_to_shoot); // TODO: Wrong type ??
        pSlig->field_15C_force_alive_state = pState->field_90_force_alive_state;
        pSlig->field_15E_spotted_possessed_slig = pState->field_92_spotted_possessed_slig;
        pSlig->field_208_glukkon_obj_id = pState->field_94_glukkon_id;
        pSlig->field_20C_state_after_speak = pState->field_98_state_after_speak;
        pSlig->field_20E_attention_timeout = pState->field_9A_attention_timeout;

        pSlig->field_212_next_command_arg1 = pState->field_9E_next_command_arg1;

        pSlig->field_214_cmd_idx = pState->field_A0_cmd_idx;

        pSlig->mFollowGlukkon = pState->mFollowGlukkon;
        pSlig->mStoppedForLeverOrLift = pState->mStoppedForLeverOrLift;
        pSlig->mGlukkonCalledAllOYa = pState->mGlukkonCalledAllOYa;
        pSlig->mHeardGlukkon = pState->mHeardGlukkon;

        sSligsUnderControlCount_BAF7E8 = 0;
    }
}

void Slig::Motion_0_StandIdle()
{
    FP xOff = {};
    if (GetAnimation().GetFlipX())
    {
        xOff = FP_FromInteger(10) * GetSpriteScale();
    }
    else
    {
        xOff = FP_FromInteger(-10) * GetSpriteScale();
    }

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    if (!gCollisions->Raycast(
            xOff + mXPos,
            mYPos - FP_FromInteger(10),
            xOff + mXPos,
            mYPos + FP_FromInteger(10),
            &pLine,
            &hitX,
            &hitY,
            GetScale() == Scale::Fg ? kFgFloor : kBgFloor))
    {
        VOnTrapDoorOpen();
        SetCurrentMotion(eSligMotions::Motion_39_FallingInitiate);
        BaseAliveGameObjectLastLineYPos = mYPos;
        mFallingVelxScaleFactor = FP_FromDouble(0.3);
        return;
    }

    if (!MainMovement())
    {
        if (sControlledCharacter == this && mHealth > FP_FromInteger(0))
        {
            if (Input_IsChanting())
            {
                if (!(mPreventDepossession & 1))
                {
                    SetCurrentMotion(eSligMotions::Motion_36_Depossessing);
                    field_12C_timer = MakeTimer(30);
                    SfxPlayMono(relive::SoundEffects::PossessEffect, 0);
                    return;
                }
            }
            else
            {
                const auto pressed = Input().GetPressed();
                const auto gameSpeakCmds = InputCommands::eChant | InputCommands::eGameSpeak8 | InputCommands::eGameSpeak7 | InputCommands::eGameSpeak6 | InputCommands::eGameSpeak5 | InputCommands::eGameSpeak4 | InputCommands::eGameSpeak3 | InputCommands::eGameSpeak2 | InputCommands::eGameSpeak1;

                if (pressed & gameSpeakCmds)
                {
                    mInput = pressed;
                    SetCurrentMotion(eSligMotions::Motion_17_GameSpeak);
                    return;
                }
            }
        }

        if (static_cast<s32>(sGnFrame) >= field_12C_timer)
        {
            SetCurrentMotion(eSligMotions::Motion_12_ReloadGun);
        }
    }
}

void Slig::Motion_1_StandToWalk()
{
    mInput |= Input().GetPressed();
    if (GetAnimation().GetIsLastFrame())
    {
        SetCurrentMotion(eSligMotions::Motion_2_Walking);
    }
    MoveOnLine();
}

void Slig::Motion_2_Walking()
{
    if (gMap.GetDirection(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos)
            >= CameraPos::eCamCurrent_0
        && MusicController::static_GetMusicType(0, 0, 0) <= MusicController::MusicTypes::eTension_4)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eTension_4, this, 0, 0);
    }

    mInput |= Input().mPads[sCurrentControllerIndex].mPressed;

    if (GetAnimation().GetFlipX())
    {
        mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(9));
    }
    else
    {
        mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(9));
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(45), mVelX * FP_FromInteger(2)))
    {
        ToStand();
        return;
    }

    MoveOnLine();

    if (GetCurrentMotion() == eSligMotions::Motion_2_Walking)
    {
        if (GetAnimation().GetCurrentFrame() == 5 || GetAnimation().GetCurrentFrame() == 14)
        {
            Slig_SoundEffect(SligSfx::eWalkingStep_2, this);

            if (!mCheckedIfOffScreen)
            {
                mCheckedIfOffScreen = true;
                MapFollowMe(true);
            }

            if (sControlledCharacter == this && mHealth > FP_FromInteger(0))
            {
                if (Input().IsAnyHeld(InputCommands::eRun))
                {
                    mReturnToPreviousMotion = true;
                    SetPreviousMotion(eSligMotions::Motion_4_Running);
                    mBaseAliveGameObjectLastAnimFrame = (GetAnimation().GetCurrentFrame() - 5) != 0 ? 13 : 5;
                    if (GetAnimation().GetFlipX())
                    {
                        mInput = 0;
                        mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
                        return;
                    }
                    mVelX = ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4);
                }
                mInput = 0;
            }
        }
        else if (GetAnimation().GetCurrentFrame() == 11)
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

            if (WallHit(GetSpriteScale() * FP_FromInteger(45), gridSize * FP_FromDouble(2.5)))
            {
                SetCurrentMotion(eSligMotions::Motion_18_WalkToStand);
            }
            else if (sControlledCharacter != this || mHealth <= FP_FromInteger(0))
            {
                if (GetNextMotion() == eSligMotions::Motion_6_Shoot)
                {
                    SetCurrentMotion(eSligMotions::Motion_18_WalkToStand);
                    field_12C_timer = MakeTimer(60);
                }
                else if (mNextMotion != -1)
                {
                    SetCurrentMotion(eSligMotions::Motion_18_WalkToStand);
                }
            }
            else
            {
                PlayerControlStopWalkingIfRequired();
            }
        }
        else
        {
            mCheckedIfOffScreen = false;
        }
    }
}

void Slig::Motion_3_StandToRun()
{
    mInput |= Input().mPads[sCurrentControllerIndex].mPressed;
    if (GetAnimation().GetIsLastFrame())
    {
        SetCurrentMotion(eSligMotions::Motion_4_Running);
    }

    EventBroadcast(kEventNoise, this);

    if (WallHit(GetSpriteScale() * FP_FromInteger(45), mVelX))
    {
        ToKnockBack();
    }
    else
    {
        MoveOnLine();
    }
}

void Slig::Motion_4_Running()
{
    if (GetAnimation().GetFlipX())
    {
        mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
    }
    else
    {
        mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
    }

    if (gMap.GetDirection(mCurrentLevel, mCurrentPath, mXPos, mYPos) >= CameraPos::eCamCurrent_0 && MusicController::static_GetMusicType(0, 0, 0) != MusicController::MusicTypes::ePossessed_9)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eSoftChase_8, this, 0, 0);
    }

    mInput |= Input().mPads[sCurrentControllerIndex].mPressed;

    EventBroadcast(kEventNoise, this);

    if (WallHit(GetSpriteScale() * FP_FromInteger(45), mVelX))
    {
        ToKnockBack();
    }
    else
    {
        MoveOnLine();

        if (GetCurrentMotion() == eSligMotions::Motion_4_Running)
        {
            if (GetAnimation().GetCurrentFrame() == 4 || GetAnimation().GetCurrentFrame() == 12)
            {
                Slig_SoundEffect(SligSfx::eRunningStep_3, this);

                if (!mCheckedIfOffScreen)
                {
                    mCheckedIfOffScreen = true;
                    MapFollowMe(true);
                }

                if (sControlledCharacter == this && mHealth > FP_FromInteger(0))
                {
                    if (GetAnimation().GetCurrentFrame() == 4)
                    {
                        PlayerControlRunningSlideStopOrTurnFrame4();
                    }
                    else
                    {
                        PlayerControlRunningSlideStopOrTurnFrame12();
                    }
                }
                else
                {
                    if (GetNextMotion() == eSligMotions::Motion_2_Walking)
                    {
                        SetCurrentMotion(eSligMotions::Motion_2_Walking);
                        mNextMotion = -1;
                        if (GetAnimation().GetFlipX())
                        {
                            mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(9));
                        }
                        else
                        {
                            mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(9));
                        }
                    }
                    else if (GetNextMotion() == eSligMotions::Motion_6_Shoot)
                    {
                        SetCurrentMotion(eSligMotions::Motion_6_Shoot);
                        mNextMotion = -1;
                        field_12C_timer = MakeTimer(60);
                    }
                    else if (mNextMotion != -1)
                    {
                        if (mVelX >= FP_FromInteger(0))
                        {
                            mVelX = (GetSpriteScale() * FP_FromDouble(13.2));
                        }
                        else
                        {
                            mVelX = (GetSpriteScale() * FP_FromDouble(-13.2));
                        }

                        SetCurrentMotion(eSligMotions::Motion_8_SlidingToStand);
                    }
                }
            }
            else
            {
                mCheckedIfOffScreen = false;
            }
        }
    }
}

void Slig::Motion_5_TurnAroundStanding()
{
    if (gMap.GetDirection(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos)
            >= CameraPos::eCamCurrent_0
        && MusicController::static_GetMusicType(0, 0, 0) <= MusicController::MusicTypes::eTension_4)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eTension_4, this, 0, 0);
    }

    Slig_SoundEffect(SligSfx::eStandingTurn_1, this);

    if (GetAnimation().GetIsLastFrame())
    {
        GetAnimation().ToggleFlipX();
        ToStand();
    }
}

void Slig::Motion_6_Shoot()
{
    if (GetAnimation().GetIsLastFrame())
    {
        if (sControlledCharacter == this && mHealth > FP_FromInteger(0))
        {
            if (Input().IsAnyHeld(InputCommands::eThrowItem))
            {
                const FP k45Scaled = GetSpriteScale() * FP_FromInteger(45);
                const FP kGridSize = ScaleToGridSize(GetSpriteScale());
                const FP k8 = FP_FromInteger(8);

                // Recoil right
                if (GetAnimation().GetFlipX() && Input().IsAnyHeld(InputCommands::eRight))
                {
                    PathLine* pLine = nullptr;
                    FP hitX = {};
                    FP hitY = {};
                    if (gCollisions->Raycast(
                            mXPos,
                            mYPos,
                            mXPos + (k8 * (kGridSize / k8)),
                            mYPos,
                            &pLine,
                            &hitX,
                            &hitY,
                            GetScale() == Scale::Fg ? kFgWalls : kBgWalls)
                        || gCollisions->Raycast(
                            mXPos,
                            mYPos - k45Scaled,
                            mXPos + (k8 * (kGridSize / k8)),
                            mYPos - k45Scaled,
                            &pLine,
                            &hitX,
                            &hitY,
                            GetScale() == Scale::Fg ? kFgWalls : kBgWalls))
                    {
                        return;
                    }
                    mVelX = (kGridSize / k8);
                    SetCurrentMotion(eSligMotions::Motion_19_Recoil);
                    return;
                }

                // Recoil left
                if (!GetAnimation().GetFlipX() && Input().IsAnyHeld(InputCommands::eLeft))
                {
                    PathLine* pLine = nullptr;
                    FP hitX = {};
                    FP hitY = {};
                    if (gCollisions->Raycast(
                            mXPos,
                            mYPos,
                            mXPos - (k8 * (kGridSize / k8)),
                            mYPos,
                            &pLine,
                            &hitX,
                            &hitY,
                            GetScale() == Scale::Fg ? kFgWalls : kBgWalls)
                        || gCollisions->Raycast(
                            mXPos,
                            mYPos - k45Scaled,
                            mXPos - (k8 * (kGridSize / k8)),
                            mYPos - k45Scaled,
                            &pLine,
                            &hitX,
                            &hitY,
                            GetScale() == Scale::Fg ? kFgWalls : kBgWalls))
                    {
                        return;
                    }
                    mVelX = -(kGridSize / k8);
                    SetCurrentMotion(eSligMotions::Motion_19_Recoil);
                    return;
                }

                // General recoil
                if (!Input().IsAnyHeld(InputCommands::eDown) || GetSpriteScale() != FP_FromDouble(0.5))
                {
                    if (field_12C_timer > static_cast<s32>(sGnFrame))
                    {
                        return;
                    }

                    if (GetAnimation().GetFlipX())
                    {
                        mVelX = (kGridSize / k8);
                    }
                    else
                    {
                        mVelX = -(kGridSize / k8);
                    }

                    PathLine* pLine = nullptr;
                    FP hitX = {};
                    FP hitY = {};
                    if (gCollisions->Raycast(
                            mXPos,
                            mYPos,
                            mXPos + (k8 * mVelX),
                            mYPos,
                            &pLine,
                            &hitX,
                            &hitY,
                            GetScale() == Scale::Fg ? kFgWalls : kBgWalls)
                        || gCollisions->Raycast(
                            mXPos, mYPos - k45Scaled,
                            mXPos + (k8 * mVelX),
                            mYPos - k45Scaled,
                            &pLine,
                            &hitX,
                            &hitY,
                            GetScale() == Scale::Fg ? kFgWalls : kBgWalls))
                    {
                        mVelX = FP_FromInteger(0);
                    }
                    else
                    {
                        SetCurrentMotion(eSligMotions::Motion_19_Recoil);
                    }
                    return;
                }
            }
            else
            {
                SND_SEQ_PlaySeq(SeqId::HitBottomOfDeathPit_9, 1, 1);
                SetCurrentMotion(eSligMotions::Motion_13_ShootToStand);
                return;
            }
        }

        if (GetNextMotion() == eSligMotions::Motion_0_StandIdle)
        {
            SND_SEQ_PlaySeq(SeqId::HitBottomOfDeathPit_9, 1, 1);
            SetCurrentMotion(eSligMotions::Motion_13_ShootToStand);
            mNextMotion = -1;
            return;
        }

        if (mNextMotion != -1 && GetNextMotion() != eSligMotions::Motion_6_Shoot)
        {
            SND_SEQ_PlaySeq(SeqId::HitBottomOfDeathPit_9, 1, 1);
            SetCurrentMotion(eSligMotions::Motion_13_ShootToStand);
            return;
        }
    }
}

void Slig::Motion_7_Falling()
{
    if (mVelX <= FP_FromInteger(0))
    {
        if (mVelX < FP_FromInteger(0))
        {
            mVelX += (GetSpriteScale() * mFallingVelxScaleFactor);
            if (mVelX < (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4)))
            {
                mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
            }

            if (mVelX > FP_FromInteger(0))
            {
                mVelX = FP_FromInteger(0);
            }
        }
    }
    else
    {
        mVelX -= (GetSpriteScale() * mFallingVelxScaleFactor);
        if (mVelX > (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4)))
        {
            mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
        }

        if (mVelX < FP_FromInteger(0))
        {
            mVelX = FP_FromInteger(0);
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
            case eLineTypes::eDynamicCollision_32:
            case eLineTypes::eBackgroundDynamicCollision_36:
                BaseAliveGameObjectCollisionLine = ppPathLine;
                mYPos = hitY;
                mXPos = hitX;
                MapFollowMe(true);
                ToStand();
                PlatformCollide();
                SetCurrentMotion(eSligMotions::Motion_40_LandingSoft);
                break;
            case eLineTypes::eWallLeft_1:
            case eLineTypes::eWallRight_2:
            case eLineTypes::eBackgroundWallLeft_5:
            case eLineTypes::eBackgroundWallRight_6:
                mXPos = hitX;
                mYPos = hitY;
                ToKnockBack();
                break;
            default:
                return;
        }
    }
}

void Slig::Motion_8_SlidingToStand()
{
    EventBroadcast(kEventNoise, this);
    if (WallHit(GetSpriteScale() * FP_FromInteger(45), mVelX))
    {
        ToKnockBack();
    }
    else
    {
        SlowDown(FP_FromDouble(2.125));
        if (GetCurrentMotion() == eSligMotions::Motion_8_SlidingToStand)
        {
            if (GetAnimation().GetCurrentFrame() < 6 && sControlledCharacter == this && mHealth > FP_FromInteger(0))
            {
                if ((GetAnimation().GetFlipX() && Input().IsAnyHeld(InputCommands::eRight)) || ((!(GetAnimation().GetFlipX()) && Input().IsAnyHeld(InputCommands::eLeft))))
                {
                    mBaseAliveGameObjectLastAnimFrame = GetAnimation().GetCurrentFrame();
                    SetPreviousMotion(eSligMotions::Motion_9_SlidingTurn);
                    mReturnToPreviousMotion = true;
                }
            }
            else if (GetAnimation().GetIsLastFrame())
            {
                Environment_SFX(EnvironmentSfx::eSlideStop_0, 0, 0x7FFF, this);
                MapFollowMe(true);
                MainMovement();
            }
        }
    }
}

void Slig::Motion_9_SlidingTurn()
{
    EventBroadcast(kEventNoise, this);

    if (WallHit(GetSpriteScale() * FP_FromInteger(45), mVelX))
    {
        ToKnockBack();
    }
    else
    {
        SlowDown(FP_FromDouble(2.125));

        if (GetCurrentMotion() == eSligMotions::Motion_9_SlidingTurn)
        {
            if (GetAnimation().GetIsLastFrame())
            {
                MapFollowMe(true);

                if (sControlledCharacter == this && mHealth > FP_FromInteger(0))
                {
                    if (Input().IsAnyHeld(InputCommands::eRun))
                    {
                        SetNextMotion(eSligMotions::Motion_4_Running);
                    }
                    else
                    {
                        SetNextMotion(eSligMotions::Motion_2_Walking);
                    }
                }

                if (GetNextMotion() == eSligMotions::Motion_4_Running)
                {
                    mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
                    SetCurrentMotion(eSligMotions::Motion_11_SlidingTurnToRun);
                }
                else
                {
                    mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(9));
                    SetCurrentMotion(eSligMotions::Motion_10_SlidingTurnToWalk);
                }

                if (GetAnimation().GetFlipX())
                {
                    mVelX = -mVelX;
                }
            }
        }
    }
}

void Slig::Motion_10_SlidingTurnToWalk()
{
    EventBroadcast(kEventNoise, this);
    MoveOnLine();

    if (GetAnimation().GetIsLastFrame())
    {
        GetAnimation().ToggleFlipX();
        SetCurrentMotion(eSligMotions::Motion_2_Walking);
    }
}

void Slig::Motion_11_SlidingTurnToRun()
{
    EventBroadcast(kEventNoise, this);
    MoveOnLine();

    if (GetAnimation().GetIsLastFrame())
    {
        GetAnimation().ToggleFlipX();
        SetCurrentMotion(eSligMotions::Motion_4_Running);
    }
}

void Slig::Motion_12_ReloadGun()
{
    if (GetAnimation().GetCurrentFrame() == 1)
    {
        Slig_SoundEffect(SligSfx::eReload1_6, this);
    }
    else if (GetAnimation().GetCurrentFrame() == 4)
    {
        Slig_SoundEffect(SligSfx::eReload2_7, this);
    }

    if (GetAnimation().GetIsLastFrame())
    {
        ToStand();
    }
}

void Slig::Motion_13_ShootToStand()
{
    if (GetAnimation().GetIsLastFrame())
    {
        MainMovement();
    }
}

void Slig::Motion_14_SteppingToStand()
{
    if (GetAnimation().GetCurrentFrame() == 0)
    {
        Slig_SoundEffect(SligSfx::eWalkingStep_2, this);
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(45), mVelX))
    {
        ToKnockBack();
    }
    else
    {
        MoveOnLine();
        if (GetCurrentMotion() == eSligMotions::Motion_14_SteppingToStand)
        {
            if (GetAnimation().GetIsLastFrame())
            {
                MapFollowMe(true);
                ToStand();
            }
        }
    }
}

void Slig::Motion_15_StandingToStep()
{
    if (GetAnimation().GetIsLastFrame())
    {
        if (Input().IsAnyHeld(InputCommands::eLeft | InputCommands::eRight))
        {
            SetCurrentMotion(eSligMotions::Motion_1_StandToWalk);
        }
        else if (GetAnimation().GetFlipX())
        {
            SetCurrentMotion(eSligMotions::Motion_14_SteppingToStand);
            mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(6));
        }
        else
        {
            mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(6));
            SetCurrentMotion(eSligMotions::Motion_14_SteppingToStand);
        }
    }
}

void Slig::Motion_16_DepossessingAbort()
{
    if (GetAnimation().GetIsLastFrame())
    {
        ToStand();
    }
}

void Slig::Motion_17_GameSpeak()
{
    mInput |= Input().mPads[sCurrentControllerIndex].mPressed;
    if (GetAnimation().GetIsLastFrame())
    {
        SetCurrentMotion(GetNextMotionIncGameSpeak(mInput));
        if (GetCurrentMotion() == eSligMotions::eNone_m1)
        {
            ToStand();
            mInput = 0;
        }
        else
        {
            if (!BrainIs(&Slig::Brain_4_ListeningToGlukkon))
            {
                EventBroadcast(kEventSpeaking, this);
            }
            mInput = 0;
        }
    }
}

void Slig::Motion_18_WalkToStand()
{
    MoveOnLine();

    if (GetAnimation().GetIsLastFrame())
    {
        MapFollowMe(true);
        if (!Slig::MainMovement())
        {
            ToStand();
        }
    }
}

void Slig::Motion_19_Recoil()
{
    MoveOnLine();

    if (GetCurrentMotion() == eSligMotions::Motion_19_Recoil)
    {
        if (GetAnimation().GetIsLastFrame())
        {
            MapFollowMe(true);
            if (sControlledCharacter == this && mHealth > FP_FromInteger(0))
            {
                if (Input().IsAnyHeld(InputCommands::eThrowItem))
                {
                    mVelX = FP_FromInteger(0);
                    SetPreviousMotion(eSligMotions::Motion_6_Shoot);
                    field_12C_timer = MakeTimer(60);
                    mBaseAliveGameObjectLastAnimFrame = 2;
                    mReturnToPreviousMotion = true;
                }
                else
                {
                    SND_SEQ_PlaySeq(SeqId::HitBottomOfDeathPit_9, 1, 1);
                    SetCurrentMotion(eSligMotions::Motion_13_ShootToStand);
                }
            }
            else if (mNextMotion != -1)
            {
                SND_SEQ_PlaySeq(SeqId::HitBottomOfDeathPit_9, 1, 1);
                SetCurrentMotion(eSligMotions::Motion_13_ShootToStand);
                mNextMotion = -1;
            }
        }
    }
    else if (GetSpriteScale() == FP_FromDouble(0.5))
    {
        SfxPlayMono(relive::SoundEffects::SligShoot, 85);
    }
    else
    {
        SfxPlayMono(relive::SoundEffects::SligShoot, 0);
    }
}

void Slig::Motion_20_SpeakHereBoy()
{
    mInput |= Input().mPads[sCurrentControllerIndex].mPressed;
    if (GetAnimation().GetIsLastFrame())
    {
        SetCurrentMotion(GetNextMotionIncGameSpeak(mInput));
        if (GetCurrentMotion() == eSligMotions::eNone_m1)
        {
            ToStand();
            mInput = 0;
        }
        else
        {
            if (!BrainIs(&Slig::Brain_4_ListeningToGlukkon))
            {
                EventBroadcast(kEventSpeaking, this);
            }
            mInput = 0;
        }
    }
}

void Slig::Motion_21_SpeakHi()
{
    Motion_20_SpeakHereBoy();
}

void Slig::Motion_22_SpeakFreeze()
{
    Motion_20_SpeakHereBoy();
}

void Slig::Motion_23_SpeakGetHim()
{
    Motion_20_SpeakHereBoy();
}

void Slig::Motion_24_SpeakLaugh()
{
    Motion_20_SpeakHereBoy();
}

void Slig::Motion_25_SpeakBullshit1()
{
    Motion_20_SpeakHereBoy();
}

void Slig::Motion_26_SpeakLookOut()
{
    Motion_20_SpeakHereBoy();
}

void Slig::Motion_27_SpeakBullshit2()
{
    Motion_20_SpeakHereBoy();
}

void Slig::Motion_28_SpeakPanic()
{
    Motion_20_SpeakHereBoy();
}

void Slig::Motion_29_SpeakWhat()
{
    Motion_20_SpeakHereBoy();
}

void Slig::Motion_30_SpeakAIFreeze()
{
    Motion_20_SpeakHereBoy();
}

void Slig::Motion_31_Blurgh()
{
    Motion_20_SpeakHereBoy();
}

void Slig::Motion_32_Sleeping()
{
    if (GetNextMotion() == eSligMotions::Motion_33_SleepingToStand)
    {
        // Out of sleep
        SetCurrentMotion(eSligMotions::Motion_33_SleepingToStand);
        mNextMotion = -1;
    }
    else if (static_cast<s32>(sGnFrame % 60))
    {
        if (!(static_cast<s32>(sGnFrame - 20) % 60))
        {
            Slig_SoundEffect(SligSfx::eSnooze1_5, this);
            if (gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0))
            {
                FP xOff = {};
                if (GetAnimation().GetFlipX())
                {
                    xOff = -(GetSpriteScale() * FP_FromInteger(-20));
                }
                else
                {
                    xOff = (GetSpriteScale() * FP_FromInteger(-20));
                }
                relive_new SnoozeParticle(
                    xOff + mXPos,
                    mYPos + (GetSpriteScale() * FP_FromInteger(-10)),
                    GetAnimation().GetRenderLayer(),
                    GetAnimation().GetSpriteScale());
            }
        }
    }
    else
    {
        Slig_SoundEffect(SligSfx::eSnooze2_4, this);

        if (gMap.Is_Point_In_Current_Camera(
                mCurrentLevel,
                mCurrentPath,
                mXPos,
                mYPos,
                0))
        {
            FP xOff = {};
            if (GetAnimation().GetFlipX())
            {
                xOff = -(GetSpriteScale() * FP_FromInteger(-20));
            }
            else
            {
                xOff = (GetSpriteScale() * FP_FromInteger(-20));
            }

            relive_new SnoozeParticle(
                xOff + mXPos,
                mYPos + (GetSpriteScale() * FP_FromInteger(-10)),
                GetAnimation().GetRenderLayer(),
                GetAnimation().GetSpriteScale());
        }
    }
}

void Slig::Motion_33_SleepingToStand()
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
            static_cast<SnoozeParticle*>(pObj)->mState = SnoozeParticle::SnoozeParticleState::eBlowingUp_2;
        }
    }

    if (GetAnimation().GetCurrentFrame() >= 2 && GetAnimation().GetCurrentFrame() <= 10)
    {
        Slig_SoundEffect(SligSfx::eToStand_0, this);
    }

    if (GetAnimation().GetCurrentFrame() == 9)
    {
        Slig_SoundEffect(SligSfx::eWalkingStep_2, this);
    }

    switch (GetAnimation().GetCurrentFrame())
    {
        case 9:
            if (GetAnimation().GetFlipX())
            {
                mXPos -= (GetSpriteScale() * FP_FromInteger(7));
                return;
            }
            else
            {
                mXPos += (GetSpriteScale() * FP_FromInteger(7));
                return;
            }
            break;

        case 10:
            if (GetAnimation().GetFlipX())
            {
                mXPos -= (GetSpriteScale() * FP_FromInteger(12));
                return;
            }
            else
            {
                mXPos += (GetSpriteScale() * FP_FromInteger(12));
                return;
            }
            break;

        case 11:
            if (GetAnimation().GetFlipX())
            {
                mXPos -= (GetSpriteScale() * FP_FromInteger(6));
                return;
            }
            else
            {
                mXPos += (GetSpriteScale() * FP_FromInteger(6));
                return;
            }
            break;

        default:
            if (GetAnimation().GetIsLastFrame())
            {
                MusicController::static_PlayMusic(MusicController::MusicTypes::eTension_4, this, 0, 0);
                MainMovement();
            }
            break;
    }
}

void Slig::Motion_34_Knockback()
{
    if (BaseAliveGameObjectCollisionLine)
    {
        SlowDown(FP_FromDouble(0.39));
    }
    else
    {
        Motion_7_Falling();

        if (GetCurrentMotion() == eSligMotions::Motion_40_LandingSoft)
        {
            SetCurrentMotion(eSligMotions::Motion_34_Knockback);
            field_12C_timer = MakeTimer(10);
            if (mYPos - BaseAliveGameObjectLastLineYPos > FP_FromInteger(180)
                && !gPathInfo->TLV_Get_At(
                    FP_GetExponent(mXPos),
                    FP_GetExponent(mYPos),
                    FP_GetExponent(mXPos),
                    FP_GetExponent(mYPos),
                    ReliveTypes::eSoftLanding))
            {
                SetCurrentMotion(eSligMotions::Motion_41_LandingFatal);
                field_12C_timer = MakeTimer(30);
                mHealth = FP_FromInteger(0);
                EventBroadcast(kEventMudokonComfort, this);
            }
        }
    }

    EventBroadcast(kEventNoise, this);
    if (GetAnimation().GetCurrentFrame() == 4)
    {
        Environment_SFX(EnvironmentSfx::eHitGroundSoft_6, 80, -200, this);
    }

    if (GetAnimation().GetForwardLoopCompleted())
    {
        if (BaseAliveGameObjectCollisionLine)
        {
            mVelX = FP_FromInteger(0);

            if (mHealth > FP_FromInteger(0) && field_12C_timer <= static_cast<s32>(sGnFrame) && sActiveHero->mHealth > FP_FromInteger(0))
            {
                SetCurrentMotion(eSligMotions::Motion_35_KnockbackToStand);
            }
        }
    }
}

void Slig::Motion_35_KnockbackToStand()
{
    EventBroadcast(kEventNoise, this);

    if (GetAnimation().GetCurrentFrame() >= 2 && GetAnimation().GetCurrentFrame() <= 10)
    {
        Slig_SoundEffect(SligSfx::eToStand_0, this);
    }

    if (GetAnimation().GetCurrentFrame() == 9)
    {
        Slig_SoundEffect(SligSfx::eWalkingStep_2, this);
    }

    FP gridSize = {};
    if (GetAnimation().GetFlipX())
    {
        gridSize = -ScaleToGridSize(GetSpriteScale());
    }
    else
    {
        gridSize = ScaleToGridSize(GetSpriteScale());
    }

    if (!WallHit(GetSpriteScale() * FP_FromInteger(45), gridSize))
    {
        s32 xOff = 0;
        switch (GetAnimation().GetCurrentFrame())
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

        if (GetAnimation().GetFlipX())
        {
            xOff = -xOff;
        }

        mXPos += (GetSpriteScale() * GetSpriteScale()) * FP_FromInteger(xOff);
    }

    if (GetAnimation().GetIsLastFrame())
    {
        ToStand();
    }
}

void Slig::Motion_36_Depossessing()
{
    if (sControlledCharacter == this)
    {
        if (GetAnimation().GetIsLastFrame())
        {
            if (!Input_IsChanting())
            {
                SetCurrentMotion(eSligMotions::Motion_16_DepossessingAbort);
            }
        }

        if (!(static_cast<s32>(sGnFrame) % 4))
        {
            const FP xRand = FP_FromInteger(Math_RandomRange(-20, 20));
            const FP yRand = FP_FromInteger(Math_RandomRange(20, 50));
            New_TintChant_Particle(
                (GetSpriteScale() * xRand) + mXPos,
                mYPos - (GetSpriteScale() * yRand),
                GetSpriteScale(),
                Layer::eLayer_0);
        }

        if (static_cast<s32>(sGnFrame) > field_12C_timer)
        {
            BlowToGibs();
        }
    }
}

void Slig::Motion_37_Possess()
{
    if (!BaseAliveGameObjectCollisionLine)
    {
        Motion_7_Falling();
        if (GetCurrentMotion() != eSligMotions::Motion_37_Possess)
        {
            SetCurrentMotion(eSligMotions::Motion_37_Possess);
        }
    }

    if (static_cast<s32>(sGnFrame) >= field_150_explode_timer)
    {
        if (GetAnimation().GetRender())
        {
            FP xOff = (FP_FromInteger(20) * GetSpriteScale());
            if (mVelX < FP_FromInteger(0))
            {
                xOff = -xOff;
            }

            relive_new Gibs(
                GibType::Slig_1,
                mXPos,
                mYPos,
                xOff,
                FP_FromInteger(0),
                GetSpriteScale(),
                0);

            New_Smoke_Particles(
                mXPos,
                mYPos - (FP_FromInteger(30) * GetSpriteScale()),
                GetSpriteScale(),
                3,
                RGB16{128, 128, 128});

            if (GetSpriteScale() == FP_FromDouble(0.5))
            {
                SfxPlayMono(relive::SoundEffects::KillEffect, 80);
                SfxPlayMono(relive::SoundEffects::FallingItemHit, 60);
            }
            else
            {
                SfxPlayMono(relive::SoundEffects::KillEffect, 127);
                SfxPlayMono(relive::SoundEffects::FallingItemHit, 90);
            }

            GetAnimation().SetRender(false);
            SetCurrentMotion(eSligMotions::Motion_0_StandIdle);
            mVelY = FP_FromInteger(0);
            mVelX = FP_FromInteger(0);
            mHealth = FP_FromInteger(0);
            MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, this, 0, 0);
            SetUpdateDelay(40);
            field_120_timer = MakeTimer(60);
            SetBrain(&Slig::Brain_0_Death);
        }
    }

    if (static_cast<s32>(sGnFrame) > field_14C_death_by_being_shot_timer)
    {
        if (mVelX >= FP_FromInteger(0))
        {
            if (GetAnimation().GetFlipX())
            {
                SetCurrentMotion(eSligMotions::Motion_34_Knockback);
            }
        }
        else if (!GetAnimation().GetFlipX())
        {
            SetCurrentMotion(eSligMotions::Motion_34_Knockback);
        }
        SetCurrentMotion(eSligMotions::Motion_44_Smash);
    }
}

// When slig is "air walking" in a death fall
void Slig::Motion_38_OutToFall()
{
    Motion_7_Falling();

    const FP fallDepth = mYPos - BaseAliveGameObjectLastLineYPos;
    if (fallDepth > FP_FromInteger(16 * 640))
    {
        SetDead(true);
    }

    if (GetCurrentMotion() == eSligMotions::Motion_40_LandingSoft && fallDepth > FP_FromInteger(180) && !gPathInfo->TLV_Get_At(FP_GetExponent(mXPos), FP_GetExponent(mYPos), FP_GetExponent(mXPos), FP_GetExponent(mYPos), ReliveTypes::eSoftLanding))
    {
        SetCurrentMotion(eSligMotions::Motion_41_LandingFatal);
        field_12C_timer = MakeTimer(30);
        mHealth = FP_FromInteger(0);
        EventBroadcast(kEventMudokonComfort, this);
    }
}

void Slig::Motion_39_FallingInitiate()
{
    EventBroadcast(kEventNoise, this);

    if (GetAnimation().GetIsLastFrame())
    {
        SetCurrentMotion(eSligMotions::Motion_38_OutToFall);
    }

    Motion_7_Falling();
}

void Slig::Motion_40_LandingSoft()
{
    if (GetAnimation().GetCurrentFrame() == 0)
    {
        Environment_SFX(EnvironmentSfx::eHitGroundSoft_6, 0, 0x7FFF, 0);
    }

    if (GetAnimation().GetIsLastFrame())
    {
        ToStand();
    }
}

void Slig::Motion_41_LandingFatal()
{
    if (GetAnimation().GetCurrentFrame() == 0)
    {
        SND_SEQ_Play(SeqId::HitBottomOfDeathPit_9, 1, 65, 65);
        SfxPlayMono(relive::SoundEffects::KillEffect, 80);
    }

    if (GetAnimation().GetIsLastFrame())
    {
        if (static_cast<s32>(sGnFrame) >= field_12C_timer && !BrainIs(&Slig::Brain_0_Death))
        {
            mHealth = FP_FromInteger(0);
            SetBrain(&Slig::Brain_0_Death);
            field_120_timer = MakeTimer(60);
        }
    }
}

void Slig::Motion_42_ShootZ()
{
    if (GetAnimation().GetIsLastFrame())
    {
        // Controlled by player and no longer pressing down or shoot
        if (sControlledCharacter == this && (!Input().IsAnyHeld(InputCommands::eThrowItem) || !Input().IsAnyHeld(InputCommands::eDown)))
        {
            SND_SEQ_PlaySeq(SeqId::HitBottomOfDeathPit_9, 1, 1);
            SetCurrentMotion(eSligMotions::Motion_43_ShootZtoStand);
        }
        // Controlled by AI and need to leave Z-shoot motion
        else if (mNextMotion != -1)
        {
            SND_SEQ_PlaySeq(SeqId::HitBottomOfDeathPit_9, 1, 1);
            mNextMotion = -1;
            SetCurrentMotion(eSligMotions::Motion_43_ShootZtoStand);
        }
    }
    else if (GetAnimation().GetCurrentFrame() == 7)
    {
        BulletType bulletType = BulletType::eSligPossessedOrUnderGlukkonCommand_0;
        if (sControlledCharacter == this)
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
            mXPos,
            mYPos - FP_FromInteger(12),
            FP_FromInteger(640),
            GetSpriteScale(),
            mSligTlv.mData.mNumTimesToShoot - field_158_num_times_to_shoot - 1);

        New_ShootingZFire_Particle(mXPos, mYPos - FP_FromInteger(12), GetSpriteScale());

        if (GetSpriteScale() == FP_FromDouble(0.5))
        {
            SfxPlayMono(relive::SoundEffects::SligShoot, 85);
        }
        else
        {
            SfxPlayMono(relive::SoundEffects::SligShoot, 0);
        }

        // The doves don't like bullets
        Dove::All_FlyAway(false);

        EventBroadcast(kEventShooting, this);
        EventBroadcast(kEventLoudNoise, this);
    }
}

void Slig::Motion_43_ShootZtoStand()
{
    if (GetAnimation().GetIsLastFrame())
    {
        ToStand();
    }
}

void Slig::Motion_44_Smash()
{
    if (BaseAliveGameObjectCollisionLine)
    {
        if (GetAnimation().GetCurrentFrame() == 4)
        {
            Environment_SFX(EnvironmentSfx::eHitGroundSoft_6, 80, -200, this);
        }
    }
    else
    {
        Motion_7_Falling();

        if (GetCurrentMotion() != eSligMotions::Motion_44_Smash)
        {
            SetCurrentMotion(eSligMotions::Motion_44_Smash);
        }
    }
}

void Slig::Motion_45_PullLever()
{
    if (GetAnimation().GetIsLastFrame())
    {
        if (GetTeleporting())
        {
            Slig_GameSpeak_SFX(SligSpeak::eBlurgh_11, 0, field_11E_pitch_min, this);
            SetCurrentMotion(eSligMotions::Motion_31_Blurgh);
        }
        else
        {
            ToStand();
        }
    }
}

void Slig::Motion_46_LiftGrip()
{
    auto pLiftPoint = static_cast<LiftPoint*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    if (pLiftPoint)
    {
        pLiftPoint->Move(FP_FromInteger(0), FP_FromInteger(0));
        mVelY = FP_FromInteger(0);
        if (GetAnimation().GetIsLastFrame())
        {
            if (sControlledCharacter == this)
            {
                if (Input().IsAnyHeld(InputCommands::eUp))
                {
                    if (pLiftPoint->vOnTopFloor())
                    {
                        SetCurrentMotion(eSligMotions::Motion_48_LiftGripping);
                    }
                    else
                    {
                        SetCurrentMotion(eSligMotions::Motion_49_LiftUp);
                    }
                    return;
                }

                if (Input().IsAnyHeld(InputCommands::eDown))
                {
                    if (pLiftPoint->vOnBottomFloor())
                    {
                        SetCurrentMotion(eSligMotions::Motion_48_LiftGripping);
                    }
                    else
                    {
                        SetCurrentMotion(eSligMotions::Motion_50_LiftDown);
                    }
                    return;
                }

                SetCurrentMotion(eSligMotions::Motion_48_LiftGripping);
            }
            else
            {
                if (pLiftPoint->vOnTopFloor())
                {
                    SetCurrentMotion(eSligMotions::Motion_50_LiftDown);
                }

                if (pLiftPoint->vOnBottomFloor())
                {
                    SetCurrentMotion(eSligMotions::Motion_49_LiftUp);
                }
            }
        }
    }
}

void Slig::Motion_47_LiftUngrip()
{
    mVelY = FP_FromInteger(0);

    if (GetAnimation().GetIsLastFrame())
    {
        ToStand();
    }
}

void Slig::Motion_48_LiftGripping()
{
    auto pLiftPoint = static_cast<LiftPoint*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    if (pLiftPoint)
    {
        CheckPlatformVanished();

        pLiftPoint->Move(FP_FromInteger(0), FP_FromInteger(0));
        mVelY = FP_FromInteger(0);

        if (Input().IsAnyHeld(InputCommands::eUp))
        {
            if (!pLiftPoint->vOnTopFloor())
            {
                SetCurrentMotion(eSligMotions::Motion_49_LiftUp);
            }
        }
        else if (Input().IsAnyHeld(InputCommands::eDown))
        {
            if (!pLiftPoint->vOnBottomFloor())
            {
                SetCurrentMotion(eSligMotions::Motion_50_LiftDown);
            }
        }
        else if (pLiftPoint->vOnAnyFloor())
        {
            SetCurrentMotion(eSligMotions::Motion_47_LiftUngrip);
        }
    }
}

void Slig::Motion_49_LiftUp()
{
    SetCurrentMotion(MoveLift(FP_FromInteger(-4)));
}

void Slig::Motion_50_LiftDown()
{
    SetCurrentMotion(MoveLift(FP_FromInteger(4)));
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

void Slig::Motion_51_Beat()
{
    if (GetAnimation().GetCurrentFrame() == 5)
    {
        SFX_Play_Pitch(relive::SoundEffects::AirStream, 90, -300);
    }

    if (GetAnimation().GetCurrentFrame() == 8)
    {
        const FP kGridSize = ScaleToGridSize(GetSpriteScale());
        const FP k2Scaled = FP_FromInteger(2) * kGridSize;

        PSX_RECT hitRect = {};
        hitRect = MakeMinMaxRect(
            GetAnimation().GetFlipX() ? mXPos - kGridSize : mXPos + kGridSize,
            mYPos,
            mXPos,
            mYPos - k2Scaled);

        for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
        {
            IBaseAliveGameObject* pObj = gBaseAliveGameObjects->ItemAt(i);
            if (!pObj)
            {
                break;
            }

            if (pObj != this)
            {
                if (pObj->Type() == ReliveTypes::eMudokon || pObj->Type() == ReliveTypes::eCrawlingSlig)
                {
                    const PSX_RECT bRect = pObj->VGetBoundingRect();

                    if (pObj->mHealth > FP_FromInteger(0) && pObj->GetScale() == GetScale() && PSX_Rects_overlap_no_adjustment(&hitRect, &bRect))
                    {
                        pObj->VTakeDamage(this);
                        EventBroadcast(kEventNoise, this);
                        SfxPlayMono(relive::SoundEffects::FallingItemHit, 60);
                        return;
                    }
                }
            }
        }
    }

    if (GetAnimation().GetIsLastFrame())
    {
        SetCurrentMotion(eSligMotions::Motion_0_StandIdle);
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


s16 Slig::Brain_Empty()
{
    return 0;
}

s16 Slig::Brain_0_Death()
{
    if (GetAnimation().GetRender() && field_120_timer < (static_cast<s32>(sGnFrame) + 30))
    {
        SetSpriteScale(GetSpriteScale() - FP_FromDouble(0.01));
        mRGB.r -= 2;
        mRGB.g -= 2;
        mRGB.b -= 2;

        if (GetSpriteScale() >= FP_FromDouble(0.3))
        {
            DeathSmokeEffect(true);
        }
    }
    else if (!GetAnimation().GetRender())
    {
        SetDead(true);
    }

    if (sControlledCharacter == this)
    {
        if (field_120_timer < static_cast<s32>(sGnFrame))
        {
            sControlledCharacter = sActiveHero;
            MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, this, 0, 0);
            gMap.SetActiveCam(mAbeLevel, mAbePath, mAbeCamera, CameraSwapEffects::eInstantChange_0, 0, 0);
        }
    }

    if (sControlledCharacter != this)
    {
        if (!gMap.Is_Point_In_Current_Camera(
                mCurrentLevel,
                mCurrentPath,
                mXPos,
                mYPos,
                0))
        {
            SetDead(true);
        }
    }

    if (GetSpriteScale() < FP_FromInteger(0))
    {
        SetDead(true);
    }

    return 116;
}

s16 Slig::Brain_1_ReturnControlToAbeAndDie()
{
    if (sControlledCharacter == this)
    {
        sControlledCharacter = sActiveHero;
        MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, this, 0, 0);
        gMap.SetActiveCam(mAbeLevel, mAbePath, mAbeCamera, CameraSwapEffects::eInstantChange_0, 0, 0);
    }

    SetDead(true);
    return 117;
}

s16 Slig::Brain_2_Possessed()
{
    switch (mBrainSubState)
    {
        case Brain_2_Possessed::eBrain2_StartPossession_0:
            Slig_GameSpeak_SFX(SligSpeak::eHelp_10, 0, field_11E_pitch_min, this);
            mBrainSubState = 1;
            mHealth = FP_FromInteger(0);
            SetCurrentMotion(eSligMotions::Motion_37_Possess);
            field_120_timer = MakeTimer(30);
            field_150_explode_timer = MakeTimer(1000);
            if (field_14C_death_by_being_shot_timer == 0)
            {
                field_14C_death_by_being_shot_timer = MakeTimer(1000);
            }
            return mBrainSubState;

        case Brain_2_Possessed::eBrain2_PossessionShaking_1:
            if (static_cast<s32>(sGnFrame) >= field_120_timer)
            {
                mBrainSubState = Brain_2_Possessed::eBrain2_PossessionInactive_2;
                field_120_timer = MakeTimer(20);
                SetCurrentMotion(eSligMotions::Motion_0_StandIdle);
                return mBrainSubState;
            }
            break;

        case Brain_2_Possessed::eBrain2_PossessionInactive_2:
            if (static_cast<s32>(sGnFrame) >= field_120_timer)
            {
                if (Math_NextRandom() & 1)
                {
                    field_120_timer = MakeTimer(20);
                    SetCurrentMotion(eSligMotions::Motion_21_SpeakHi);
                    Slig_GameSpeak_SFX(SligSpeak::eHi_0, 0, field_11E_pitch_min, this);
                }
                else
                {
                    field_120_timer = MakeTimer(20);
                    SetCurrentMotion(eSligMotions::Motion_24_SpeakLaugh);
                    Slig_GameSpeak_SFX(SligSpeak::eLaugh_3, 0, field_11E_pitch_min, this);
                }
                mBrainSubState = Brain_2_Possessed::eBrain2_PossessionSpeak_3;
                return mBrainSubState;
            }
            break;

        case Brain_2_Possessed::eBrain2_PossessionSpeak_3:
            if (EventGet(kEventDeathReset))
            {
                if (sControlledCharacter != this)
                {
                    SetDead(true);
                }
            }

            if (static_cast<s32>(sGnFrame) >= field_120_timer)
            {
                mBrainSubState = Brain_2_Possessed::eBrain2_ControlledByPlayer_4;
                mHealth = FP_FromInteger(1);
                return mBrainSubState;
            }
            break;

        case Brain_2_Possessed::eBrain2_ControlledByPlayer_4:
            if (EventGet(kEventDeathReset))
            {
                if (sControlledCharacter != this)
                {
                    SetDead(true);
                }
            }
            if (mHealth <= FP_FromInteger(0))
            {
                SetBrain(&Slig::Brain_0_Death);
                field_120_timer = MakeTimer(60);
                return mBrainSubState;
            }
            break;

        default:
            return mBrainSubState;
    }

    if (static_cast<s32>(sGnFrame) < field_120_timer)
    {
        if (mbGotShot)
        {
            BlowToGibs();
        }
    }

    return mBrainSubState;
}

enum Brain_3_DeathDropDeath
{
    eBrain3_SayHelpOnce_0 = 0,
    eBrain3_SayHelpAndDie_1 = 1,
    eBrain3_SwitchCamToAbe_2 = 2
};

s16 Slig::Brain_3_DeathDropDeath()
{
    switch (mBrainSubState)
    {
        case Brain_3_DeathDropDeath::eBrain3_SayHelpOnce_0:
            Slig_GameSpeak_SFX(SligSpeak::eHelp_10, 0, field_11E_pitch_min, this);
            field_120_timer = MakeTimer(60);
            return Brain_3_DeathDropDeath::eBrain3_SayHelpAndDie_1;

        case Brain_3_DeathDropDeath::eBrain3_SayHelpAndDie_1:
        {
            if (static_cast<s32>(sGnFrame) < field_120_timer)
            {
                if (!((field_120_timer - sGnFrame) % 15))
                {
                    Slig_GameSpeak_SFX(
                        SligSpeak::eHelp_10,
                        static_cast<s16>(2 * ((field_120_timer & 0xFFFF) - sGnFrame)),
                        field_11E_pitch_min,
                        this);
                }

                if (static_cast<s32>(sGnFrame) == field_120_timer - 6)
                {
                    SND_SEQ_Play(SeqId::HitBottomOfDeathPit_9, 1, 65, 65);
                }

                return mBrainSubState;
            }

            Environment_SFX(EnvironmentSfx::eFallingDeathScreamHitGround_15, 0, 0x7FFF, this);

            relive_new ScreenShake(0, 0);

            field_120_timer = MakeTimer(30);
            return Brain_3_DeathDropDeath::eBrain3_SwitchCamToAbe_2;
        }

        case Brain_3_DeathDropDeath::eBrain3_SwitchCamToAbe_2:
            if (static_cast<s32>(sGnFrame) > field_120_timer)
            {
                if (sControlledCharacter == this)
                {
                    MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, this, 0, 0);
                    sControlledCharacter = sActiveHero;
                    gMap.SetActiveCam(mAbeLevel, mAbePath, mAbeCamera, CameraSwapEffects::eInstantChange_0, 0, 0);
                }
                SetDead(true);
            }
            return mBrainSubState;

        default:
            return mBrainSubState;
    }
}

static const eSligMotions sGlukkonResponseTable_560768[8][6] = {
    {eSligMotions::Motion_0_StandIdle, eSligMotions::Motion_21_SpeakHi, eSligMotions::eNone_m1, eSligMotions::Motion_0_StandIdle, eSligMotions::Motion_0_StandIdle, eSligMotions::Motion_0_StandIdle},
    {eSligMotions::Motion_0_StandIdle, eSligMotions::Motion_29_SpeakWhat, eSligMotions::eNone_m1, eSligMotions::Motion_0_StandIdle, eSligMotions::Motion_0_StandIdle, eSligMotions::Motion_0_StandIdle},
    {eSligMotions::Motion_0_StandIdle, eSligMotions::Motion_25_SpeakBullshit1, eSligMotions::eNone_m1, eSligMotions::Motion_0_StandIdle, eSligMotions::Motion_0_StandIdle, eSligMotions::Motion_0_StandIdle},
    {eSligMotions::Motion_0_StandIdle, eSligMotions::Motion_27_SpeakBullshit2, eSligMotions::eNone_m1, eSligMotions::Motion_0_StandIdle, eSligMotions::Motion_0_StandIdle, eSligMotions::Motion_0_StandIdle},
    {eSligMotions::Motion_2_Walking, eSligMotions::eNone_m1, eSligMotions::Motion_0_StandIdle, eSligMotions::Motion_0_StandIdle, eSligMotions::Motion_0_StandIdle, eSligMotions::Motion_0_StandIdle},
    {eSligMotions::Motion_4_Running, eSligMotions::eNone_m1, eSligMotions::Motion_0_StandIdle, eSligMotions::Motion_0_StandIdle, eSligMotions::Motion_0_StandIdle, eSligMotions::Motion_0_StandIdle},
    {eSligMotions::Motion_0_StandIdle, eSligMotions::Motion_24_SpeakLaugh, eSligMotions::eNone_m1, eSligMotions::Motion_0_StandIdle, eSligMotions::Motion_0_StandIdle, eSligMotions::Motion_0_StandIdle},
    {eSligMotions::Motion_0_StandIdle, eSligMotions::Motion_12_ReloadGun, eSligMotions::Motion_12_ReloadGun, eSligMotions::Motion_12_ReloadGun, eSligMotions::Motion_12_ReloadGun, eSligMotions::eNone_m1}};


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

s16 Slig::Brain_4_ListeningToGlukkon()
{
    auto pGlukkonObj = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(field_208_glukkon_obj_id));
    auto pPlatformObj = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));

    mHeardGlukkon = false;

    if (!pGlukkonObj || !pGlukkonObj->GetDrawable())
    {
        PauseALittle();
        SetNextMotion(eSligMotions::Motion_24_SpeakLaugh);
        mFollowGlukkon = false;
        --sSligsUnderControlCount_BAF7E8;
        return Brain_ListeningToGlukkon_States::GettingAttention_0;
    }

    switch (mBrainSubState)
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
            mBrainSubState = Brain_ListeningToGlukkon_States::GettingAttention_0;
            return mBrainSubState;
    }
}

s16 Slig::Brain_ListeningToGlukkon_LostAttention()
{
    SetBrain(&Slig::Brain_33_Paused);
    return mBrainSubState;
}

s16 Slig::Brain_ListeningToGlukkon_Shooting()
{
    LastGlukkonSpeak();
    if (static_cast<s32>(sGnFrame) == field_120_timer - 15)
    {
        SetNextMotion(eSligMotions::Motion_6_Shoot);
    }

    if (static_cast<s32>(sGnFrame) <= field_120_timer)
    {
        return mBrainSubState;
    }
    NextGlukkonCommand(Brain_ListeningToGlukkon_GlukkonCommands::AfterShootOrLaugh_6, Brain_ListeningToGlukkon_States::IdleListening_1);
    return Brain_ListeningToGlukkon_States::Speaking_4;
}

s16 Slig::Brain_ListeningToGlukkon_StoppingOnLift(PlatformBase* pPlatformObj)
{
    const FP gridSize = ScaleToGridSize(GetSpriteScale());

    if (GetCurrentMotion() == eSligMotions::Motion_34_Knockback)
    {
        SetNextMotion(eSligMotions::Motion_0_StandIdle);
        return Brain_ListeningToGlukkon_States::IdleListening_1;
    }

    if (GetCurrentMotion() != eSligMotions::Motion_0_StandIdle)
    {
        return mBrainSubState;
    }

    if (GetAnimation().GetFlipX())
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
        const FP v84 = mXPos - gridSize - FP_FromInteger((BaseAliveGameObjectCollisionLine->mRect.x + BaseAliveGameObjectCollisionLine->mRect.w) / 2) >= FP_FromInteger(0) ? mXPos - gridSize - FP_FromInteger((BaseAliveGameObjectCollisionLine->mRect.x + BaseAliveGameObjectCollisionLine->mRect.w) / 2) : FP_FromInteger((BaseAliveGameObjectCollisionLine->mRect.x + BaseAliveGameObjectCollisionLine->mRect.w) / 2) - (mXPos - gridSize);
        if (v84 >= gridSizeHalved)
        {
            return Brain_ListeningToGlukkon_States::IdleListening_1;
        }
        SetNextMotion(eSligMotions::Motion_14_SteppingToStand);
        mVelX = -(gridSize / FP_FromInteger(6));
        return mBrainSubState;
    }

    if (!pPlatformObj)
    {
        return Brain_ListeningToGlukkon_States::IdleListening_1;
    }

    if (pPlatformObj->Type() != ReliveTypes::eLiftPoint)
    {
        return Brain_ListeningToGlukkon_States::IdleListening_1;
    }

    const FP v90 = mXPos + gridSize - FP_FromInteger((BaseAliveGameObjectCollisionLine->mRect.x + BaseAliveGameObjectCollisionLine->mRect.w) / 2) >= FP_FromInteger(0) ? mXPos + gridSize
                                                                                                                                                                                   - FP_FromInteger((BaseAliveGameObjectCollisionLine->mRect.x + BaseAliveGameObjectCollisionLine->mRect.w) / 2)
                                                                                                                                                                             : FP_FromInteger((BaseAliveGameObjectCollisionLine->mRect.x + BaseAliveGameObjectCollisionLine->mRect.w) / 2)
                                                                                                                                                                                   - (mXPos + gridSize);
    if (v90 >= gridSize / FP_FromInteger(2))
    {
        return Brain_ListeningToGlukkon_States::IdleListening_1;
    }
    mVelX = (gridSize / FP_FromInteger(6));
    SetNextMotion(eSligMotions::Motion_14_SteppingToStand);
    return mBrainSubState;
}

s16 Slig::Brain_ListeningToGlukkon_PullingLever()
{
    const FP gridSize = ScaleToGridSize(GetSpriteScale());

    Lever* pLever = nullptr;
    if (GetAnimation().GetFlipX())
    {
        pLever = static_cast<Lever*>(FindObjectOfType(ReliveTypes::eLever, mXPos - gridSize, mYPos - FP_FromInteger(5)));
    }
    else
    {
        pLever = static_cast<Lever*>(FindObjectOfType(ReliveTypes::eLever, mXPos + gridSize, mYPos - FP_FromInteger(5)));
    }

    if (pLever)
    {
        if (GetCurrentMotion() == eSligMotions::Motion_45_PullLever)
        {
            pLever->VPull(mXPos < pLever->mXPos);
            SetNextMotion(eSligMotions::Motion_0_StandIdle);
            return Brain_ListeningToGlukkon_States::IdleListening_1;
        }
        else
        {
            SetNextMotion(eSligMotions::Motion_45_PullLever);
            return mBrainSubState;
        }
    }
    else
    {
        SetNextMotion(eSligMotions::Motion_5_TurnAroundStanding);
        return mBrainSubState;
    }
}

s16 Slig::Brain_ListenToGlukkon_StoppingNextToLever()
{
    const FP gridSize = ScaleToGridSize(GetSpriteScale());

    if (GetCurrentMotion() == eSligMotions::Motion_34_Knockback)
    {
        SetNextMotion(eSligMotions::Motion_0_StandIdle);
        return Brain_ListeningToGlukkon_States::IdleListening_1;
    }

    if (GetCurrentMotion() != eSligMotions::Motion_0_StandIdle)
    {
        return mBrainSubState;
    }

    if (!FindObjectOfType(ReliveTypes::eLever, gridSize + mXPos, mYPos - FP_FromInteger(5)))
    {
        if (!FindObjectOfType(ReliveTypes::eLever, mXPos - gridSize, mYPos - FP_FromInteger(5)))
        {
            if (GetAnimation().GetFlipX())
            {
                SetNextMotion(eSligMotions::Motion_14_SteppingToStand);
                mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(6));
                return mBrainSubState;
            }
            mVelX = (gridSize / FP_FromInteger(6));
            SetNextMotion(eSligMotions::Motion_14_SteppingToStand);
            return mBrainSubState;
        }
    }

    if ((!FindObjectOfType(ReliveTypes::eLever, gridSize + mXPos, mYPos - FP_FromInteger(5)) || !(GetAnimation().GetFlipX())) && (!FindObjectOfType(ReliveTypes::eLever, mXPos - gridSize, mYPos - FP_FromInteger(5)) || GetAnimation().GetFlipX()))
    {
        return Brain_ListeningToGlukkon_States::IdleListening_1;
    }

    SetNextMotion(eSligMotions::Motion_5_TurnAroundStanding);
    return mBrainSubState;
}

s16 Slig::Brain_ListenToGlukkon_Speaking()
{
    if (static_cast<s32>(sGnFrame) <= field_120_timer)
    {
        return mBrainSubState;
    }

    if (GetNextMotion() == eSligMotions::Motion_5_TurnAroundStanding)
    {
        return mBrainSubState;
    }

    field_120_timer = MakeTimer(8);

    SetNextMotion(sGlukkonResponseTable_560768[field_212_next_command_arg1][field_214_cmd_idx]);

    // keeps going till it hits -1
    field_214_cmd_idx++;

    if (sGlukkonResponseTable_560768[field_212_next_command_arg1][field_214_cmd_idx] != eSligMotions::eNone_m1)
    {
        return mBrainSubState;
    }

    return field_20C_state_after_speak;
}

s16 Slig::Brain_ListenToGlukkon_NextLever(BaseAliveGameObject* pGlukkonObj)
{
    if (!VIsFacingMe(pGlukkonObj))
    {
        SetNextMotion(eSligMotions::Motion_5_TurnAroundStanding);
        return mBrainSubState;
    }

    if (GetCurrentMotion() != eSligMotions::Motion_0_StandIdle)
    {
        return mBrainSubState;
    }

    if (GetAnimation().GetFlipX())
    {
        mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(6));
    }
    else
    {
        mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(6));
    }

    SetNextMotion(eSligMotions::Motion_14_SteppingToStand);
    return Brain_ListeningToGlukkon_States::IdleListening_1;
}

s16 Slig::Brain_ListenToGlukkon_Moving(BaseAliveGameObject* pGlukkonObj)
{
    FP xOffset = (ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(GetCurrentMotion() != eSligMotions::Motion_4_Running ? 1 : 3));
    if (GetAnimation().GetFlipX())
    {
        xOffset = -xOffset;
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(45), xOffset))
    {
        SetNextMotion(eSligMotions::Motion_0_StandIdle);
        return Brain_ListeningToGlukkon_States::IdleListening_1;
    }

    if (GetCurrentMotion() == eSligMotions::Motion_0_StandIdle)
    {
        if (GetNextMotion() != eSligMotions::Motion_2_Walking && GetNextMotion() != eSligMotions::Motion_4_Running)
        {
            return Brain_ListeningToGlukkon_States::IdleListening_1;
        }
    }

    if (GetCurrentMotion() == eSligMotions::Motion_34_Knockback)
    {
        SetNextMotion(eSligMotions::Motion_0_StandIdle);
        return Brain_ListeningToGlukkon_States::IdleListening_1;
    }

    if (GetCurrentMotion() != eSligMotions::Motion_2_Walking && GetCurrentMotion() != eSligMotions::Motion_4_Running)
    {
        return mBrainSubState;
    }

    if (GetCurrentMotion() != eSligMotions::Motion_4_Running && FindLever())
    {
        SetNextMotion(eSligMotions::Motion_0_StandIdle);
        mFollowGlukkon = false;
        mStoppedForLeverOrLift = true;
        return Brain_ListeningToGlukkon_States::StoppingNextToLever_5;
    }

    if (GetCurrentMotion() != eSligMotions::Motion_4_Running && FindLiftPoint())
    {
        SetNextMotion(eSligMotions::Motion_0_StandIdle);
        mFollowGlukkon = false;
        mStoppedForLeverOrLift = true;
        return Brain_ListeningToGlukkon_States::StoppingOnLift_7;
    }

    if (!VIsFacingMe(pGlukkonObj))
    {
        SetNextMotion(eSligMotions::Motion_0_StandIdle);
        return Brain_ListeningToGlukkon_States::IdleListening_1;
    }

    if (VIsObjNearby(ScaleToGridSize(GetSpriteScale()), pGlukkonObj))
    {
        SetNextMotion(eSligMotions::Motion_0_StandIdle);
        return Brain_ListeningToGlukkon_States::IdleListening_1;
    }

    if (field_160_last_event_index != gEventSystem->mLastEventIndex)
    {
        field_160_last_event_index = gEventSystem->mLastEventIndex;
        if (gEventSystem->mLastEvent == GameSpeakEvents::eGlukkon_StayHere_38 && gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0))
        {
            mFollowGlukkon = false;
            NextGlukkonCommand(Brain_ListeningToGlukkon_GlukkonCommands::Stay_2, Brain_ListeningToGlukkon_States::IdleListening_1);
            return Brain_ListeningToGlukkon_States::Speaking_4;
        }
    }

    if (!VIsObjNearby(ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(5), pGlukkonObj))
    {
        if (GetCurrentMotion() == eSligMotions::Motion_2_Walking)
        {
            SetNextMotion(eSligMotions::Motion_4_Running);
            return Brain_ListeningToGlukkon_States::Moving_2;
        }
    }

    if (!VIsObjNearby(ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(5), pGlukkonObj) || GetCurrentMotion() != eSligMotions::Motion_4_Running)
    {
        return mBrainSubState;
    }

    SetNextMotion(eSligMotions::Motion_2_Walking);
    return Brain_ListeningToGlukkon_States::Moving_2;
}

s16 Slig::Brain_ListenToGlukkon_IdleListen(BaseAliveGameObject* pGlukkonObj, PlatformBase* pPlatformObj)
{
    if (GetCurrentMotion() != eSligMotions::Motion_0_StandIdle)
    {
        field_20E_attention_timeout = 0;
        return mBrainSubState;
    }

    if (mFollowGlukkon)
    {
        field_20E_attention_timeout++;
    }

    mStoppedForLeverOrLift = false;

    FP directedGridSize = {};
    if (GetAnimation().GetFlipX())
    {
        directedGridSize = -ScaleToGridSize(GetSpriteScale());
    }
    else
    {
        directedGridSize = ScaleToGridSize(GetSpriteScale());
    }

    const FP gridSize = ScaleToGridSize(GetSpriteScale());

    const s32 bWallHit = WallHit(GetSpriteScale() * FP_FromInteger(45), directedGridSize);
    if (field_20E_attention_timeout <= 450 && (gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 1) || mFollowGlukkon))
    {
        if (mFollowGlukkon)
        {
            if (!VIsObjNearby(gridSize, pGlukkonObj))
            {
                if (!VIsFacingMe(pGlukkonObj))
                {
                    SetNextMotion(eSligMotions::Motion_5_TurnAroundStanding);
                    return mBrainSubState;
                }

                if (VIsObjNearby(gridSize * FP_FromInteger(5), pGlukkonObj) && pGlukkonObj->mCurrentMotion != 4) // TODO: glukkon motion enum
                {
                    if (!bWallHit)
                    {
                        if (FindLiftPoint())
                        {
                            mFollowGlukkon = false;
                            return Brain_ListeningToGlukkon_States::StoppingOnLift_7;
                        }
                        else
                        {
                            NextGlukkonCommand(Brain_ListeningToGlukkon_GlukkonCommands::Walk_4, Brain_ListeningToGlukkon_States::Moving_2);
                            return Brain_ListeningToGlukkon_States::Speaking_4;
                        }
                    }
                }
                else
                {
                    if (GetAnimation().GetFlipX())
                    {
                        directedGridSize = -ScaleToGridSize(GetSpriteScale());
                    }
                    else
                    {
                        directedGridSize = ScaleToGridSize(GetSpriteScale());
                    }

                    if (!WallHit(GetSpriteScale() * FP_FromInteger(45), directedGridSize * FP_FromInteger(3)))
                    {
                        NextGlukkonCommand(Brain_ListeningToGlukkon_GlukkonCommands::Run_5, Brain_ListeningToGlukkon_States::Moving_2);
                        return Brain_ListeningToGlukkon_States::Speaking_4;
                    }
                }
            }

            if (sSligsUnderControlCount_BAF7E8 > 1)
            {
                auto pOtherSlig = static_cast<Slig*>(FindObjectOfType(ReliveTypes::eSlig, mXPos, mYPos - FP_FromInteger(5)));
                if (pOtherSlig)
                {
                    if (pOtherSlig != this && !VIsFacingMe(pOtherSlig) && pOtherSlig->GetCurrentMotion() != eSligMotions::Motion_5_TurnAroundStanding)
                    {
                        SetNextMotion(eSligMotions::Motion_5_TurnAroundStanding);
                        return mBrainSubState;
                    }
                }
            }
        }

        const GameSpeakEvents glukkonSpeak = LastGlukkonSpeak();
        if (glukkonSpeak == GameSpeakEvents::eNone_m1 || glukkonSpeak == GameSpeakEvents::eSameAsLast_m2)
        {
            return mBrainSubState;
        }

        if (glukkonSpeak == GameSpeakEvents::eGlukkon_AllOYa_40)
        {
            NextGlukkonCommand(Brain_ListeningToGlukkon_GlukkonCommands::HeyOrAllYa_0, Brain_ListeningToGlukkon_States::IdleListening_1);
        }
        else
        {
            if (glukkonSpeak == GameSpeakEvents::eGlukkon_Laugh_43)
            {
                NextGlukkonCommand(Brain_ListeningToGlukkon_GlukkonCommands::AfterShootOrLaugh_6, Brain_ListeningToGlukkon_States::IdleListening_1);
            }
            else
            {
                if (!mGlukkonCalledAllOYa || glukkonSpeak == GameSpeakEvents::eGlukkon_Hey_36)
                {
                    if (!HeardGlukkonToListenTo(glukkonSpeak))
                    {
                        mGlukkonCalledAllOYa = glukkonSpeak == GameSpeakEvents::eGlukkon_AllOYa_40 ? true : false;
                        return mBrainSubState;
                    }
                }

                switch (glukkonSpeak)
                {
                    case GameSpeakEvents::eGlukkon_Hey_36:
                        if (!VIsFacingMe(pGlukkonObj))
                        {
                            SetNextMotion(eSligMotions::Motion_5_TurnAroundStanding);
                        }
                        NextGlukkonCommand(Brain_ListeningToGlukkon_GlukkonCommands::HeyOrAllYa_0, Brain_ListeningToGlukkon_States::IdleListening_1);
                        break;

                    case GameSpeakEvents::eGlukkon_DoIt_37:
                        if (pPlatformObj && pPlatformObj->Type() == ReliveTypes::eLiftPoint)
                        {
                            const FP scaled_2 = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(2));
                            const FP lineMidX = FP_FromInteger((BaseAliveGameObjectCollisionLine->mRect.x + BaseAliveGameObjectCollisionLine->mRect.w) / 2);
                            const FP v33 = mXPos - lineMidX >= FP_FromInteger(0) ? mXPos - lineMidX : lineMidX - mXPos;
                            if (v33 < scaled_2 && (static_cast<LiftPoint*>(pPlatformObj)->vOnTopFloor() || static_cast<LiftPoint*>(pPlatformObj)->vOnBottomFloor()))
                            {
                                SetNextMotion(eSligMotions::Motion_46_LiftGrip);

                                for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
                                {
                                    IBaseAliveGameObject* pFoundSlig = gBaseAliveGameObjects->ItemAt(i);
                                    if (!pFoundSlig)
                                    {
                                        break;
                                    }

                                    if (pFoundSlig->Type() == ReliveTypes::eSlig && pFoundSlig != this && static_cast<Slig*>(pFoundSlig)->GetNextMotion() == eSligMotions::Motion_46_LiftGrip)
                                    {
                                        SetNextMotion(eSligMotions::Motion_0_StandIdle);
                                    }
                                }

                                mBrainSubState = Brain_ListeningToGlukkon_States::IdleListening_1;
                                mGlukkonCalledAllOYa = glukkonSpeak == GameSpeakEvents::eGlukkon_AllOYa_40 ? true : false;
                                return mBrainSubState;
                            }
                        }

                        if (FindObjectOfType(ReliveTypes::eLever, ScaleToGridSize(GetSpriteScale()) + mXPos, mYPos - FP_FromInteger(5)) || FindObjectOfType(ReliveTypes::eLever, mXPos - ScaleToGridSize(GetSpriteScale()), mYPos - FP_FromInteger(5)))
                        {
                            mBrainSubState = Brain_ListeningToGlukkon_States::PullingLever_6;
                            mGlukkonCalledAllOYa = glukkonSpeak == GameSpeakEvents::eGlukkon_AllOYa_40 ? true : false;
                            return mBrainSubState;
                        }

                        if (!FindObjectOfType(ReliveTypes::eMudokon, mXPos, mYPos - FP_FromInteger(5)))
                        {
                            const FP scaled = GetAnimation().GetFlipX() ? -ScaleToGridSize(GetSpriteScale()) : ScaleToGridSize(GetSpriteScale());

                            if (!FindObjectOfType(ReliveTypes::eMudokon, scaled + mXPos, mYPos - FP_FromInteger(5)))
                            {
                                NextGlukkonCommand(Brain_ListeningToGlukkon_GlukkonCommands::DoitGunReload_7, Brain_ListeningToGlukkon_States::LostAttention_9);
                                mBrainSubState = Brain_ListeningToGlukkon_States::Speaking_4;
                                --sSligsUnderControlCount_BAF7E8;
                                mGlukkonCalledAllOYa = glukkonSpeak == GameSpeakEvents::eGlukkon_AllOYa_40 ? true : false;
                                return mBrainSubState;
                            }
                        }

                        SetNextMotion(eSligMotions::Motion_51_Beat);

                        mBrainSubState = Brain_ListeningToGlukkon_States::IdleListening_1;
                        mGlukkonCalledAllOYa = glukkonSpeak == GameSpeakEvents::eGlukkon_AllOYa_40 ? true : false;
                        return mBrainSubState;

                    case GameSpeakEvents::eGlukkon_StayHere_38:
                        mFollowGlukkon = false;
                        NextGlukkonCommand(Brain_ListeningToGlukkon_GlukkonCommands::Stay_2, Brain_ListeningToGlukkon_States::IdleListening_1);
                        break;

                    case GameSpeakEvents::eGlukkon_Commere_39:
                        mFollowGlukkon = true;
                        if (VIsObjNearby(gridSize, pGlukkonObj))
                        {
                            if (!VIsObjNearby(gridSize * FP_FromDouble(0.5), pGlukkonObj))
                            {
                                mBrainSubState = Brain_ListeningToGlukkon_States::NextToLeverThroughComeHere_3;
                                mGlukkonCalledAllOYa = glukkonSpeak == GameSpeakEvents::eGlukkon_AllOYa_40 ? true : false;
                                return mBrainSubState;
                            }
                        }

                        if (FindLiftPoint())
                        {
                            mFollowGlukkon = false;
                            mBrainSubState = Brain_ListeningToGlukkon_States::StoppingOnLift_7;
                            mGlukkonCalledAllOYa = glukkonSpeak == GameSpeakEvents::eGlukkon_AllOYa_40 ? true : false;
                            return mBrainSubState;
                        }

                        if (bWallHit)
                        {
                            NextGlukkonCommand(Brain_ListeningToGlukkon_GlukkonCommands::Bs_3, Brain_ListeningToGlukkon_States::IdleListening_1);
                        }
                        else
                        {
                            if (!VIsFacingMe(pGlukkonObj))
                            {
                                SetNextMotion(eSligMotions::Motion_5_TurnAroundStanding);
                            }
                            NextGlukkonCommand(Brain_ListeningToGlukkon_GlukkonCommands::Stay_2, Brain_ListeningToGlukkon_States::IdleListening_1);
                        }
                        break;

                    case GameSpeakEvents::eGlukkon_KillEm_44:
                        mBrainSubState = Brain_ListeningToGlukkon_States::Shooting_8;
                        field_120_timer = MakeTimer(25);
                        mGlukkonCalledAllOYa = glukkonSpeak == GameSpeakEvents::eGlukkon_AllOYa_40 ? true : false;
                        return mBrainSubState;

                    default:
                        mGlukkonCalledAllOYa = glukkonSpeak == GameSpeakEvents::eGlukkon_AllOYa_40 ? true : false;
                        return mBrainSubState;
                }
            }
        }
        mBrainSubState = Brain_ListeningToGlukkon_States::Speaking_4;
        mGlukkonCalledAllOYa = glukkonSpeak == GameSpeakEvents::eGlukkon_AllOYa_40 ? true : false;
        return mBrainSubState;
    }

    PauseALittle();
    mFollowGlukkon = false;
    --sSligsUnderControlCount_BAF7E8;
    return Brain_ListeningToGlukkon_States::GettingAttention_0;
}

s16 Slig::Brain_ListenToGlukkon_GettingAttention(BaseAliveGameObject* pGlukkonObj)
{
    SetNextMotion(eSligMotions::Motion_0_StandIdle);
    field_20E_attention_timeout = 0;
    mFollowGlukkon = false;
    mStoppedForLeverOrLift = false;
    if (!VIsFacingMe(pGlukkonObj))
    {
        SetNextMotion(eSligMotions::Motion_5_TurnAroundStanding);
    }
    NextGlukkonCommand(Brain_ListeningToGlukkon_GlukkonCommands::HeyOrAllYa_0, Brain_ListeningToGlukkon_States::IdleListening_1);
    return Brain_ListeningToGlukkon_States::Speaking_4;
}

s16 Slig::Brain_7_SpottedEnemy()
{
    if (gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0) || mSligTlv.mData.mChaseAbeWhenSpotted == relive::reliveChoice::eNo)
    {
        if (VOnSameYLevel(sControlledCharacter) && VIsObj_GettingNear_On_X(sControlledCharacter) && VIsObjNearby(ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(3), sControlledCharacter) && !EventGet(kEventResetting) && !sControlledCharacter->GetInvisible())
        {
            ToShoot();
        }
        else if (
            VOnSameYLevel(sControlledCharacter) && EventGet(kEventAbeOhm) && VIsFacingMe(sControlledCharacter))
        {
            ToShoot();
        }
        else if (field_120_timer > static_cast<s32>(sGnFrame))
        {
            if (sActiveHero->mHealth <= FP_FromInteger(0))
            {
                ToAbeDead();
            }
            else
            {
                ShouldStillBeAlive();
            }
        }
        else if (VIsFacingMe(sControlledCharacter) && !sControlledCharacter->GetInvisible())
        {
            ToShoot();
        }
        else
        {
            ToTurn();
        }
    }
    else
    {
        ToChase();
    }

    return 302;
}

s16 Slig::Brain_10_EnemyDead()
{
    if (EventGet(kEventDeathReset) && !gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0))
    {
        SetDead(true);
        return 113;
    }

    if (field_120_timer >= static_cast<s32>(sGnFrame))
    {
        ShouldStillBeAlive();
    }
    else
    {
        const auto rnd = Math_NextRandom();

        // Say this often
        if (rnd < 25)
        {
            SetNextMotion(eSligMotions::Motion_25_SpeakBullshit1);
            field_120_timer = MakeTimer(15);
            return 113;
        }

        // Say this less often
        if (rnd < 50)
        {
            SetNextMotion(eSligMotions::Motion_27_SpeakBullshit2);
            field_120_timer = MakeTimer(15);
            return 113;
        }

        // And turn even less often
        if (sActiveHero->mHealth > FP_FromInteger(0))
        {
            TurnOrWalk();
        }
    }
    return 113;
}

s16 Slig::Brain_11_KilledEnemy()
{
    if (field_120_timer > static_cast<s32>(sGnFrame))
    {
        ShouldStillBeAlive();
    }
    else
    {
        ToAbeDead();
    }
    return 114;
}

s16 Slig::Brain_12_PanicTurning()
{
    if (EventGet(kEventDeathReset) && !gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0))
    {
        SetDead(true);
        return 107;
    }

    if (ListenToGlukkonCommands())
    {
        ToUnderGlukkonCommand();
        return 107;
    }

    bool checkTurn = false;
    if (GetCurrentMotion() != eSligMotions::Motion_5_TurnAroundStanding)
    {
        if (GetAnimation().GetIsLastFrame())
        {
            SetNextMotion(eSligMotions::Motion_5_TurnAroundStanding);
            return 107;
        }
        checkTurn = true;
    }
    else if (GetCurrentMotion() == eSligMotions::Motion_5_TurnAroundStanding)
    {
        if (!GetAnimation().GetIsLastFrame())
        {
            checkTurn = true;
        }
    }

    if (checkTurn)
    {
        if (GetAnimation().GetCurrentFrame() == 4)
        {
            if (GetAnimation().GetFlipX())
            {
                if (sControlledCharacter->mVelX >= FP_FromInteger(0) && (sControlledCharacter->mVelX != FP_FromInteger(0) || sControlledCharacter->mXPos >= mXPos))
                {
                    ShouldStillBeAlive();
                    return 107;
                }
            }
            else
            {
                if (sControlledCharacter->mVelX <= FP_FromInteger(0) && (sControlledCharacter->mVelX != FP_FromInteger(0) || sControlledCharacter->mXPos <= mXPos))
                {
                    ShouldStillBeAlive();
                    return 107;
                }
            }

            const PSX_RECT bRect = VGetBoundingRect();
            const PSX_RECT charRect = sControlledCharacter->VGetBoundingRect();

            if (sControlledCharacter->Type() != ReliveTypes::eGlukkon && sControlledCharacter->Type() != ReliveTypes::eSlig && !IsInInvisibleZone(sControlledCharacter) && !sControlledCharacter->GetInvisible())
            {
                if (charRect.x <= bRect.w && charRect.w >= bRect.x && charRect.h >= bRect.y && charRect.y <= bRect.h)
                {
                    GetAnimation().ToggleFlipX();
                    return 107;
                }
            }
        }
        ShouldStillBeAlive();
        return 107;
    }

    if (field_120_timer > static_cast<s32>(sGnFrame))
    {
        ToPanicRunning();
    }
    else
    {
        WaitOrWalk();
    }
    return 107;
}

s16 Slig::Brain_13_PanicRunning()
{
    if (mVelX > FP_FromInteger(4) && ((ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(4)) + mXPos) > FP_FromInteger(field_138_zone_rect.w))
    {
        ToPanicTurn();
    }
    else if (mVelX < FP_FromInteger(-4) && (mXPos - (ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(4))) < FP_FromInteger(field_138_zone_rect.x))
    {
        ToPanicTurn();
    }
    else if (HandleEnemyStopper(4))
    {
        ToPanicTurn();
    }
    else if (ListenToGlukkonCommands())
    {
        ToUnderGlukkonCommand();
    }
    else if (!mCurrentMotion && mNextMotion == -1)
    {
        ToPanicRunning();
    }
    else if (VOnSameYLevel(sControlledCharacter) && sControlledCharacter->Type() != ReliveTypes::eGlukkon && VIsFacingMe(sControlledCharacter) && !IsInInvisibleZone(sControlledCharacter) && !sControlledCharacter->GetInvisible() && !IsWallBetween(this, sControlledCharacter) && gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0) && gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0) && !EventGet(kEventResetting))
    {
        ToShoot();
    }
    else if (sActiveHero->mHealth <= FP_FromInteger(0))
    {
        ToAbeDead();
    }
    else
    {
        if (field_120_timer <= static_cast<s32>(sGnFrame))
        {
            WaitOrWalk();
        }
        else
        {
            ShouldStillBeAlive();
        }
    }
    return 109;
}

s16 Slig::Brain_14_PanicYelling()
{
    if (ListenToGlukkonCommands())
    {
        ToUnderGlukkonCommand();
    }

    if (GetCurrentMotion() != eSligMotions::Motion_28_SpeakPanic)
    {
        if (GetNextMotion() != eSligMotions::Motion_28_SpeakPanic)
        {
            SetNextMotion(eSligMotions::Motion_28_SpeakPanic);
        }

        if (GetCurrentMotion() != eSligMotions::Motion_28_SpeakPanic)
        {
            ShouldStillBeAlive();
            return 115;
        }
    }

    if (GetAnimation().GetIsLastFrame())
    {
        EventBroadcast(kEventAlarm, this);

        const bool kFlipX = GetAnimation().GetFlipX();
        field_120_timer = sGnFrame + mSligTlv.mData.mPanicTimeout;
        if ((!kFlipX || mXPos >= FP_FromInteger((field_138_zone_rect.x + field_138_zone_rect.w) / 2)) && (kFlipX || mXPos <= FP_FromInteger((field_138_zone_rect.x + field_138_zone_rect.w) / 2)))
        {
            ToPanicRunning();
        }
        else
        {
            ToPanicTurn();
        }
    }
    else
    {
        ShouldStillBeAlive();
    }
    return 115;
}

s16 Slig::Brain_15_Idle()
{
    if ((EventGet(kEventAbeOhm) || EventGet(kEventAlarm)) && !EventGet(kEventResetting) && mSligTlv.mData.mPanicTimeout)
    {
        ToPanic();
        return 104;
    }

    if (ListenToGlukkonCommands())
    {
        ToUnderGlukkonCommand();
        return 104;
    }

    if (VOnSameYLevel(sControlledCharacter) && 
        VIsFacingMe(sControlledCharacter) && 
        !IsInInvisibleZone(sControlledCharacter) &&
        !sControlledCharacter->GetInvisible() &&
        !IsWallBetween(this, sControlledCharacter) && (!field_15E_spotted_possessed_slig || sControlledCharacter->Type() != ReliveTypes::eSlig) &&
        !IsAbeEnteringDoor(sControlledCharacter) &&
        !EventGet(kEventResetting) &&
        gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0) && sControlledCharacter->Type() != ReliveTypes::eGlukkon)
    {
        RespondToEnemyOrPatrol();
        return 104;
    }

    if (sActiveHero->mHealth <= FP_FromInteger(0))
    {
        ToAbeDead();
        return 104;
    }

    auto pShooter = static_cast<BaseAnimatedWithPhysicsGameObject*>(EventGet(kEventShooting));
    if (pShooter && pShooter->GetSpriteScale() == GetSpriteScale() && gMap.Is_Point_In_Current_Camera(pShooter->mCurrentLevel, pShooter->mCurrentPath, pShooter->mXPos, pShooter->mYPos, 0) && pShooter == sControlledCharacter && gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0) && !EventGet(kEventResetting))
    {
        if (VIsFacingMe(pShooter))
        {
            RespondToEnemyOrPatrol();
            return 104;
        }
        else
        {
            ToTurn();
            return 104;
        }
    }

    auto pNoiseOrSpeaking = static_cast<BaseAnimatedWithPhysicsGameObject*>(EventGet(kEventSuspiciousNoise));
    if (!pNoiseOrSpeaking)
    {
        pNoiseOrSpeaking = static_cast<BaseAnimatedWithPhysicsGameObject*>(EventGet(kEventSpeaking));
    }

    if (pNoiseOrSpeaking && pNoiseOrSpeaking->GetSpriteScale() == GetSpriteScale() && gMap.Is_Point_In_Current_Camera(pNoiseOrSpeaking->mCurrentLevel, pNoiseOrSpeaking->mCurrentPath, pNoiseOrSpeaking->mXPos, pNoiseOrSpeaking->mYPos, 0) && pNoiseOrSpeaking != this && gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0) && !EventGet(kEventResetting))
    {
        if (pNoiseOrSpeaking != sControlledCharacter)
        {
            if (Math_NextRandom() >= 192u)
            {
                return 104;
            }
        }

        if (VIsFacingMe(sControlledCharacter))
        {
            GoAlertedOrSayWhat();
        }
        else
        {
            TurnOrSayWhat();
        }
        return 104;
    }
    else
    {
        if (sControlledCharacter->GetSpriteScale() > GetSpriteScale() && VIsFacingMe(sControlledCharacter) && !IsInInvisibleZone(sControlledCharacter) && !sControlledCharacter->GetInvisible() && gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0) && !IsInZCover(sControlledCharacter) && !IsInZCover(this) && !EventGet(kEventResetting))
        {
            ToZShoot();
            return 104;
        }

        if (GetCurrentMotion() == eSligMotions::Motion_0_StandIdle && field_120_timer <= static_cast<s32>(sGnFrame))
        {
            ToTurn();
            return 104;
        }
        else
        {
            ShouldStillBeAlive();
            return 104;
        }
    }
}

s16 Slig::Brain_16_StopChasing()
{
    if (field_120_timer > static_cast<s32>(sGnFrame))
    {
        if (HandleEnemyStopper(4))
        {
            SetNextMotion(eSligMotions::Motion_0_StandIdle);
            SetBrain(&Slig::Brain_15_Idle);
            field_120_timer = MakeTimer(1);
        }
    }
    else
    {
        field_15C_force_alive_state = 0;
        ToTurn();
    }
    return 119;
}

s16 Slig::Brain_17_Chasing()
{
    if (gMap.Is_Point_In_Current_Camera(
        mCurrentLevel,
        mCurrentPath,
        mXPos, mYPos,
        0) 
        && VOnSameYLevel(sControlledCharacter) 
        && VIsFacingMe(sControlledCharacter) 
        && !IsInInvisibleZone(sControlledCharacter) 
        && !sControlledCharacter->GetInvisible() 
        && !IsWallBetween(this, sControlledCharacter) 
        && !InAnyWellRenderLayer(sControlledCharacter) 
        && !EventGet(kEventResetting) 
        && sControlledCharacter->Type() != ReliveTypes::eGlukkon)
    {
        field_15C_force_alive_state = 0;
        RespondToEnemyOrPatrol();
        return 118;
    }

    if (HandleEnemyStopper(4))
    {
        SetNextMotion(eSligMotions::Motion_0_StandIdle);
        SetBrain(&Slig::Brain_15_Idle);
        field_120_timer = MakeTimer(1);
        return 118;
    }

    if (mCurrentPath != gMap.mCurrentPath || mCurrentLevel != gMap.mCurrentLevel || (EventGet(kEventDeathReset) && !gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0)))
    {
        SetDead(true);
    }
    else if (gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0))
    {
        SetBrain(&Slig::Brain_16_StopChasing);
        field_120_timer = sGnFrame + mSligTlv.mData.mStopChaseDelay;
        return 118;
    }
    return 118;
}

s16 Slig::Brain_18_StartChasing()
{
    if (field_120_timer > static_cast<s32>(sGnFrame))
    {
        if (gMap.Is_Point_In_Current_Camera(
                mCurrentLevel,
                mCurrentPath,
                mXPos,
                mYPos,
                0)
            && !sControlledCharacter->GetInvisible() && sControlledCharacter->Type() != ReliveTypes::eGlukkon)
        {
            field_15C_force_alive_state = 0;
            ToShoot();
        }
    }
    else
    {
        if (mCurrentPath != gMap.mCurrentPath || mCurrentLevel != gMap.mCurrentLevel)
        {
            SetDead(true);
        }

        if (!VIsFacingMe(sControlledCharacter))
        {
            GetAnimation().ToggleFlipX();
        }

        field_15C_force_alive_state = 1;
        SetNextMotion(eSligMotions::Motion_4_Running);
        SetBrain(&Slig::Brain_17_Chasing);
        field_120_timer = mSligTlv.mData.mPauseTime;
    }
    return 122;
}

s16 Slig::Brain_19_Turning()
{
    if (EventGet(kEventDeathReset) && !gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0))
    {
        SetDead(true);
        return 106;
    }

    if (ListenToGlukkonCommands())
    {
        ToUnderGlukkonCommand();
        return 106;
    }

    if ((GetCurrentMotion() == eSligMotions::Motion_5_TurnAroundStanding && GetAnimation().GetIsLastFrame()) || (GetCurrentMotion() == eSligMotions::Motion_0_StandIdle && mNextMotion == -1))
    {
        WaitOrWalk();
        return 106;
    }

    if (GetAnimation().GetCurrentFrame() != 4)
    {
        ShouldStillBeAlive();
        return 106;
    }

    if (GetAnimation().GetFlipX())
    {
        if (sControlledCharacter->mVelX >= FP_FromInteger(0) && (sControlledCharacter->mVelX > FP_FromInteger(0) || sControlledCharacter->mXPos >= mXPos))
        {
            ShouldStillBeAlive();
            return 106;
        }
    }
    else
    {
        if (sControlledCharacter->mVelX <= FP_FromInteger(0) && (sControlledCharacter->mVelX != FP_FromInteger(0) || sControlledCharacter->mXPos <= mXPos))
        {
            ShouldStillBeAlive();
            return 106;
        }
    }

    const PSX_RECT bRect = VGetBoundingRect();
    const PSX_RECT charRect = sControlledCharacter->VGetBoundingRect();


    if (sControlledCharacter->Type() != ReliveTypes::eGlukkon && sControlledCharacter->GetScale() == GetScale() && !IsInInvisibleZone(sControlledCharacter) && !sControlledCharacter->GetInvisible() && !IsWallBetween(this, sControlledCharacter) && PSX_Rects_overlap_no_adjustment(&charRect, &bRect) && sControlledCharacter->Type() != ReliveTypes::eSlig)
    {
        GetAnimation().ToggleFlipX();
        return 106;
    }

    ShouldStillBeAlive();
    return 106;
}

s16 Slig::Brain_20_StoppingNextToMudokon()
{
    if (GetCurrentMotion() != eSligMotions::Motion_0_StandIdle)
    {
        return 128;
    }

    IBaseAliveGameObject* pBeatTarget = FindBeatTarget(1);
    if (!pBeatTarget || pBeatTarget->mHealth <= FP_FromInteger(0))
    {
        WaitOrWalk();
        return 128;
    }

    if (Math_NextRandom() >= 100)
    {
        if (Math_NextRandom() & 1)
        {
            SetNextMotion(eSligMotions::Motion_25_SpeakBullshit1);
        }
        else
        {
            SetNextMotion(eSligMotions::Motion_27_SpeakBullshit2);
        }
    }
    else
    {
        SetNextMotion(eSligMotions::Motion_51_Beat);
        SetBrain(&Slig::Brain_24_BeatingUp);
    }

    return 128;
}

s16 Slig::Brain_21_Walking()
{
    if (EventGet(kEventAlarm) && mSligTlv.mData.mPanicTimeout)
    {
        ToPanic();
        return 108;
    }

    if (mVelX > FP_FromInteger(0))
    {
        if ((ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(2) + mXPos) >= FP_FromInteger(field_138_zone_rect.w))
        {
            PauseALittle();
            return 108;
        }
    }

    if (mVelX < FP_FromInteger(0))
    {
        if ((mXPos - (ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(2))) <= FP_FromInteger(field_138_zone_rect.x))
        {
            PauseALittle();
            return 108;
        }
    }

    if (mVelX == FP_FromInteger(0) && GetCurrentMotion() == eSligMotions::Motion_0_StandIdle && GetNextMotion() != eSligMotions::Motion_2_Walking)
    {
        PauseALittle();
    }
    else if (HandleEnemyStopper(2))
    {
        PauseALittle();
    }
    else if (ListenToGlukkonCommands())
    {
        ToUnderGlukkonCommand();
    }
    else if (VOnSameYLevel(sControlledCharacter) &&
        VIsFacingMe(sControlledCharacter) &&
        !IsInInvisibleZone(sControlledCharacter) &&
        !sControlledCharacter->GetInvisible() &&
        !IsWallBetween(this, sControlledCharacter) &&
        gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0) &&
        gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0) && (!field_15E_spotted_possessed_slig || sControlledCharacter->Type() != ReliveTypes::eSlig) &&
        !IsAbeEnteringDoor(sControlledCharacter) && !EventGet(kEventResetting) && sControlledCharacter->Type() != ReliveTypes::eGlukkon)
    {
        RespondToEnemyOrPatrol();
    }
    else
    {
        auto pShooter = static_cast<BaseAnimatedWithPhysicsGameObject*>(EventGet(kEventShooting));
        if (pShooter && pShooter->GetSpriteScale() == GetSpriteScale() && gMap.Is_Point_In_Current_Camera(pShooter->mCurrentLevel, pShooter->mCurrentPath, pShooter->mXPos, pShooter->mYPos, 0) && pShooter == sControlledCharacter && gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0) && !EventGet(kEventResetting))
        {
            if (VIsFacingMe(pShooter))
            {
                RespondToEnemyOrPatrol();
            }
            else
            {
                ToTurn();
            }
        }
        else if (VOnSameYLevel(sControlledCharacter) && VIsFacingMe(sControlledCharacter) && !IsWallBetween(this, sControlledCharacter) && EventGet(kEventAbeOhm) && gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0) && gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0) && !EventGet(kEventResetting))
        {
            ToShoot();
        }
        else if (EventGet(kEventAbeOhm) && !EventGet(kEventResetting) && mSligTlv.mData.mPanicTimeout)
        {
            ToPanic();
        }
        else if (sActiveHero->mHealth <= FP_FromInteger(0))
        {
            ToAbeDead();
        }
        else
        {
            auto pNoiseOrSpeaker = static_cast<BaseAnimatedWithPhysicsGameObject*>(EventGet(kEventSuspiciousNoise));
            if (!pNoiseOrSpeaker)
            {
                pNoiseOrSpeaker = static_cast<BaseAnimatedWithPhysicsGameObject*>(EventGet(kEventSpeaking));
            }

            if (pNoiseOrSpeaker && pNoiseOrSpeaker->GetSpriteScale() == GetSpriteScale() && gMap.Is_Point_In_Current_Camera(pNoiseOrSpeaker->mCurrentLevel, pNoiseOrSpeaker->mCurrentPath, pNoiseOrSpeaker->mXPos, pNoiseOrSpeaker->mYPos, 0) && pNoiseOrSpeaker != this && gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0) && !EventGet(kEventResetting))
            {
                if (VIsFacingMe(sControlledCharacter))
                {
                    GoAlertedOrSayWhat();
                }
                else
                {
                    TurnOrSayWhat();
                }
            }
            else if (sControlledCharacter->GetSpriteScale() > GetSpriteScale() && VIsFacingMe(sControlledCharacter) && !IsInInvisibleZone(sControlledCharacter) && !sControlledCharacter->GetInvisible() && gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0) && !IsInZCover(sControlledCharacter) && !IsInZCover(this) && !EventGet(kEventResetting))
            {
                ToZShoot();
            }
            else
            {
                if (Math_NextRandom() < mSligTlv.mData.mPercentBeatMud && FindBeatTarget(2) && mCurrentMotion)
                {
                    SetNextMotion(eSligMotions::Motion_0_StandIdle);
                    SetBrain(&Slig::Brain_20_StoppingNextToMudokon);
                    field_120_timer = MakeTimer(30);
                }
                else
                {
                    ShouldStillBeAlive();
                }
            }
        }
    }
    return 108;
}

s16 Slig::Brain_22_GetAlertedTurn()
{
    if (GetCurrentMotion() == eSligMotions::Motion_5_TurnAroundStanding && GetAnimation().GetIsLastFrame())
    {
        GoAlertedOrSayWhat();
        return 123;
    }

    if (ListenToGlukkonCommands())
    {
        ToUnderGlukkonCommand();
        return 123;
    }

    if (GetAnimation().GetCurrentFrame() == 4)
    {
        bool tryTurningToPlayer = true;
        if (GetAnimation().GetFlipX())
        {
            if (sControlledCharacter->mVelX >= FP_FromInteger(0) && (sControlledCharacter->mVelX > FP_FromInteger(0) || sControlledCharacter->mXPos >= mXPos))
            {
                tryTurningToPlayer = false;
            }
        }
        else if (sControlledCharacter->mVelX <= FP_FromInteger(0) && (sControlledCharacter->mVelX > FP_FromInteger(0) || sControlledCharacter->mXPos <= mXPos))
        {
            tryTurningToPlayer = false;
        }

        if (tryTurningToPlayer)
        {
            if (sControlledCharacter->Type() != ReliveTypes::eGlukkon && !IsInInvisibleZone(sControlledCharacter) && !sControlledCharacter->GetInvisible())
            {
                const PSX_RECT bRect = VGetBoundingRect();
                const PSX_RECT bRectChar = sControlledCharacter->VGetBoundingRect();

                if (PSX_Rects_overlap_no_adjustment(&bRectChar, &bRect))
                {
                    GetAnimation().ToggleFlipX();
                    return 123;
                }
            }
        }
    }

    if (GetCurrentMotion() != eSligMotions::Motion_0_StandIdle)
    {
        ShouldStillBeAlive();
    }
    else
    {
        SetNextMotion(eSligMotions::Motion_5_TurnAroundStanding);
    }
    return 123;
}

s16 Slig::Brain_23_GetAlerted()
{
    if (field_120_timer != (mSligTlv.mData.mAlertedListenTime + static_cast<s32>(sGnFrame) - 2) || Math_RandomRange(0, 100) >= mSligTlv.mData.mPercentSayWhat)
    {
        if (ListenToGlukkonCommands())
        {
            ToUnderGlukkonCommand();
        }
    }
    else
    {
        SetNextMotion(eSligMotions::Motion_29_SpeakWhat);
    }

    // Check if we should be killing Abe
    if (VOnSameYLevel(sControlledCharacter) &&
        VIsFacingMe(sControlledCharacter) &&
        !IsInInvisibleZone(sControlledCharacter) &&
        !sControlledCharacter->GetInvisible() &&
        !IsWallBetween(this, sControlledCharacter) &&
        gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0) && (sControlledCharacter->Type() != ReliveTypes::eSlig && !field_15E_spotted_possessed_slig) &&
        !IsAbeEnteringDoor(sControlledCharacter) && !EventGet(kEventResetting) && sControlledCharacter->Type() != ReliveTypes::eGlukkon)
    {
        RespondToEnemyOrPatrol();
    }
    // Panic?
    else if ((EventGet(kEventAbeOhm) || EventGet(kEventAlarm)) && !EventGet(kEventResetting) && mSligTlv.mData.mPanicTimeout)
    {
        ToPanic();
    }
    else
    {
        // If some fool is trying to shoot us then shoot back
        auto pShootingSlig = static_cast<BaseAnimatedWithPhysicsGameObject*>(EventGet(kEventShooting));
        if (pShootingSlig && pShootingSlig->GetSpriteScale() == GetSpriteScale() && gMap.Is_Point_In_Current_Camera(pShootingSlig->mCurrentLevel, pShootingSlig->mCurrentPath, pShootingSlig->mXPos, pShootingSlig->mYPos, 0) && pShootingSlig == sControlledCharacter && gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0) && !EventGet(kEventResetting))
        {
            if (VIsFacingMe(pShootingSlig))
            {
                RespondToEnemyOrPatrol();
            }
            else
            {
                ToTurn();
            }
        }
        else
        {
            // Is a mud being noisy?
            auto pNoisyMud = static_cast<BaseAnimatedWithPhysicsGameObject*>(EventGet(kEventSuspiciousNoise));
            if (!pNoisyMud)
            {
                pNoisyMud = static_cast<BaseAnimatedWithPhysicsGameObject*>(EventGet(kEventSpeaking));
            }

            // Then kill them
            if (pNoisyMud && gMap.Is_Point_In_Current_Camera(pNoisyMud->mCurrentLevel, pNoisyMud->mCurrentPath, pNoisyMud->mXPos, pNoisyMud->mYPos, 0) && (pNoisyMud == sControlledCharacter || pNoisyMud->Type() == ReliveTypes::eMudokon) && VOnSameYLevel(pNoisyMud) && VIsFacingMe(pNoisyMud) && (pNoisyMud != sControlledCharacter || (!sControlledCharacter->GetInvisible() && gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0) && !EventGet(kEventResetting))))
            {
                ToShoot();
            }
            else
            {
                // Is a slig being noisy?
                auto pNoisySlig = static_cast<BaseAnimatedWithPhysicsGameObject*>(EventGet(kEventSuspiciousNoise));
                if (!pNoisySlig)
                {
                    pNoisySlig = static_cast<BaseAnimatedWithPhysicsGameObject*>(EventGet(kEventSpeaking));
                }

                // Then mSay what, stop walking to respond or try to kill them.
                if (pNoisySlig && gMap.Is_Point_In_Current_Camera(pNoisySlig->mCurrentLevel, pNoisySlig->mCurrentPath, pNoisySlig->mXPos, pNoisySlig->mYPos, 0) && (pNoisySlig == sControlledCharacter || pNoisySlig->Type() != ReliveTypes::eSlig) && !VIsFacingMe(pNoisySlig) && gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0) && !EventGet(kEventResetting))
                {
                    TurnOrSayWhat();
                }
                else
                {
                    if (field_120_timer > static_cast<s32>(sGnFrame))
                    {
                        ShouldStillBeAlive();
                    }
                    else
                    {
                        WaitOrWalk();
                    }
                }
            }
        }
    }
    return 124;
}

s16 Slig::Brain_24_BeatingUp()
{
    if (GetCurrentMotion() != eSligMotions::Motion_0_StandIdle || field_120_timer >= static_cast<s32>(sGnFrame))
    {
        return 129;
    }

    if (Math_NextRandom() < 100)
    {
        SetNextMotion(eSligMotions::Motion_51_Beat);
        return 129;
    }

    if (Math_NextRandom() < 64)
    {
        SetNextMotion(eSligMotions::Motion_25_SpeakBullshit1);
        return 129;
    }

    if (Math_NextRandom() < 64)
    {
        SetNextMotion(eSligMotions::Motion_27_SpeakBullshit2);
        return 129;
    }

    WaitOrWalk();
    return 129;
}

s16 Slig::Brain_25_DiscussionWhat()
{
    if (ListenToGlukkonCommands())
    {
        ToUnderGlukkonCommand();
    }

    if (GetCurrentMotion() == eSligMotions::Motion_0_StandIdle && field_120_timer == static_cast<s32>(sGnFrame))
    {
        mNextMotion = static_cast<s16>(field_294_next_gamespeak_motion);
    }

    if (field_120_timer < static_cast<s32>(sGnFrame - 5))
    {
        WaitOrWalk();
    }

    return 128;
}

s16 Slig::Brain_28_ZShooting()
{
    if (GetCurrentMotion() != eSligMotions::Motion_42_ShootZ || !GetAnimation().GetIsLastFrame())
    {
        ShouldStillBeAlive();
        return 127;
    }

    field_158_num_times_to_shoot++;

    if (field_158_num_times_to_shoot < mSligTlv.mData.mNumTimesToShoot)
    {
        return 127;
    }

    field_158_num_times_to_shoot = 0;

    if (sActiveHero->mHealth <= FP_FromInteger(0))
    {
        ToAbeDead();
    }
    else
    {
        if (sControlledCharacter->mHealth > FP_FromInteger(0))
        {
            WaitOrWalk();
        }
        else
        {
            ToKilledAbe();
        }
    }
    return 127;
}

s16 Slig::Brain_29_Shooting()
{
    if (mCurrentMotion == 6 && GetAnimation().GetIsLastFrame())
    {
        field_158_num_times_to_shoot++;

        if (field_158_num_times_to_shoot < mSligTlv.mData.mNumTimesToShoot)
        {
            SetNextMotion(eSligMotions::Motion_6_Shoot);
            return 111;
        }

        if (sActiveHero->mHealth <= FP_FromInteger(0))
        {
            ToKilledAbe();
            return 111;
        }

        if (sControlledCharacter->mHealth <= FP_FromInteger(0))
        {
            ToKilledAbe();
            return 111;
        }
        if (!VOnSameYLevel(sControlledCharacter) || !VIsFacingMe(sControlledCharacter) || IsInInvisibleZone(sControlledCharacter) || sControlledCharacter->GetInvisible() || IsWallBetween(this, sControlledCharacter) || !gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0) || !gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0) || EventGet(kEventResetting))
        {
            PauseALittle();
            return 111;
        }

        if (!VIsFacingMe(sControlledCharacter))
        {
            ToTurn();
            return 111;
        }

        if (!gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0) && mSligTlv.mData.mChaseAbeWhenSpotted == relive::reliveChoice::eYes)
        {
            ToChase();
            return 111;
        }
    }

    ShouldStillBeAlive();
    return 111;
}

s16 Slig::Brain_30_ZSpottedEnemy()
{
    if (field_120_timer > static_cast<s32>(sGnFrame))
    {
        return 126;
    }
    SetNextMotion(eSligMotions::Motion_42_ShootZ);
    SetBrain(&Slig::Brain_28_ZShooting);
    return 126;
}

s16 Slig::Brain_31_WakingUp()
{
    if (GetAnimation().GetIsLastFrame())
    {
        WaitOrWalk();
    }
    return 0;
}

s16 Slig::Brain_32_Inactive()
{
    if (ListenToGlukkonCommands())
    {
        ToUnderGlukkonCommand();
    }
    else if (field_120_timer > static_cast<s32>(sGnFrame))
    {
        if (sActiveHero->mHealth <= FP_FromInteger(0))
        {
            ToAbeDead();
        }
        else if (VOnSameYLevel(sControlledCharacter) && VIsFacingMe(sControlledCharacter) && VIsObjNearby(ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(1), sControlledCharacter) && !IsInInvisibleZone(sControlledCharacter) && !sControlledCharacter->GetInvisible() && !IsWallBetween(this, sControlledCharacter) && !EventGet(kEventResetting) && sControlledCharacter->Type() != ReliveTypes::eGlukkon)
        {
            ToShoot();
        }
        else
        {
            ShouldStillBeAlive();
        }
    }
    else
    {
        WaitOrWalk();
    }
    return 103;
}

s16 Slig::Brain_33_Paused()
{
    if (ListenToGlukkonCommands())
    {
        ToUnderGlukkonCommand();
        return 101;
    }

    if (VOnSameYLevel(sControlledCharacter) && VIsFacingMe(sControlledCharacter) && !IsInInvisibleZone(sControlledCharacter) && !sControlledCharacter->GetInvisible() && !IsWallBetween(this, sControlledCharacter) && gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0) && (!field_15E_spotted_possessed_slig || sControlledCharacter->Type() != ReliveTypes::eSlig) && !EventGet(kEventResetting) && sControlledCharacter->Type() != ReliveTypes::eGlukkon)
    {
        RespondToEnemyOrPatrol();
        return 101;
    }

    if (VOnSameYLevel(sControlledCharacter) && VIsFacingMe(sControlledCharacter) && !IsInInvisibleZone(sControlledCharacter) && !sControlledCharacter->GetInvisible() && !IsWallBetween(this, sControlledCharacter) && gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0) && !EventGet(kEventResetting) && sControlledCharacter->Type() != ReliveTypes::eGlukkon)
    {
        ToShoot();
    }
    else if (sActiveHero->mHealth <= FP_FromInteger(0))
    {
        ToAbeDead();
    }
    else
    {
        auto pNoiseOrSpeaking = static_cast<BaseAnimatedWithPhysicsGameObject*>(EventGet(kEventSuspiciousNoise));
        if (!pNoiseOrSpeaking)
        {
            pNoiseOrSpeaking = static_cast<BaseAnimatedWithPhysicsGameObject*>(EventGet(kEventSpeaking));
        }

        if (pNoiseOrSpeaking && pNoiseOrSpeaking->GetSpriteScale() == GetSpriteScale() && pNoiseOrSpeaking != this && field_120_timer <= static_cast<s32>(sGnFrame) && !EventGet(kEventResetting))
        {
            ToTurn();
        }
        else if (sControlledCharacter->GetSpriteScale() > GetSpriteScale() && VIsFacingMe(sControlledCharacter) && !IsInInvisibleZone(sControlledCharacter) && !sControlledCharacter->GetInvisible() && gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0) && !IsInZCover(sControlledCharacter) && !IsInZCover(this) && !EventGet(kEventResetting))
        {
            ToZShoot();
        }
        else
        {
            ShouldStillBeAlive();
        }
    }
    return 101;
}

s16 Slig::Brain_34_Sleeping()
{
    auto pEvent = IsEventInRange(kEventSuspiciousNoise, mXPos, mYPos, AsEventScale(GetScale()));
    if (pEvent)
    {
        auto pNoise = IsEventInRange(kEventNoise, mXPos, mYPos, AsEventScale(GetScale()));
        if (pNoise)
        {
            pEvent = pNoise;
        }

        const FP wakeUpDistance = ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(mSligTlv.mData.mNoiseWakeUpDistance);
        if (VIsObjNearby(wakeUpDistance, static_cast<BaseAnimatedWithPhysicsGameObject*>(pEvent)) && field_120_timer <= static_cast<s32>(sGnFrame) && gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0) && !EventGet(kEventResetting))
        {
            WakeUp();
            return 102;
        }
    }

    if (EventGet(kEventSpeaking) || EventGet(kEventLoudNoise))
    {
        if (pEvent != this && field_120_timer <= static_cast<s32>(sGnFrame) && gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0) && !EventGet(kEventResetting))
        {
            WakeUp();
            return 102;
        }
    }

    if (EventGet(kEventAlarm) && !EventGet(kEventResetting))
    {
        WakeUp();
        return 102;
    }

    ShouldStillBeAlive();

    if (GetDead())
    {
        const CameraPos direction = gMap.GetDirection(mCurrentLevel, mCurrentPath, mXPos, mYPos);
        Start_Slig_sounds(direction, 0);
    }
    return 102;
}

// A leftover function from Abe's Oddysee.
s16 Slig::Brain_35_ChaseAndDisappear()
{
    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }

    if (mBrainSubState == Brain_35_ChaseAndDisappear::eBrain35_Summoned_0)
    {
        if (gNumCamSwappers > 0 || sActiveHero->mShrivel)
        {
            return mBrainSubState;
        }

        SetCurrentMotion(eSligMotions::Motion_0_StandIdle);
        field_120_timer = sGnFrame + mSligTlv.mData.mPauseTime;
        return Brain_35_ChaseAndDisappear::eBrain35_Running_1;
    }
    else if (mBrainSubState == Brain_35_ChaseAndDisappear::eBrain35_Running_1)
    {
        if (static_cast<s32>(sGnFrame) < field_120_timer)
        {
            return mBrainSubState;
        }

        SetNextMotion(eSligMotions::Motion_4_Running);
        return Brain_35_ChaseAndDisappear::eBrain35_ReachedDestination_2;
    }
    else
    {
        if (mBrainSubState == Brain_35_ChaseAndDisappear::eBrain35_ReachedDestination_2 
            && gMap.Is_Point_In_Current_Camera(
                mCurrentLevel,
                mCurrentPath,
                mXPos,
                mYPos,
                0))
        {
            return mBrainSubState;
        }
        SetDead(true);
        mHealth = FP_FromInteger(0);
        return mBrainSubState;
    }
}

void Slig::Init()
{
    field_15E_spotted_possessed_slig = 0;
    field_120_timer = mSligTlv.mData.mPauseTime + sGnFrame;

    switch (mSligTlv.mData.mStartState)
    {
        case relive::Path_Slig_Data::StartState::Patrol:
            SetBrain(&Slig::Brain_32_Inactive);
            break;

        case relive::Path_Slig_Data::StartState::Sleeping:
            if (mSligTlv.mTlvSpecificMeaning && mSligTlv.mData.mStayAwake == relive::reliveChoice::eYes)
            {
                SetBrain(&Slig::Brain_32_Inactive);
            }
            else
            {
                SetBrain(&Slig::Brain_34_Sleeping);
                SetCurrentMotion(eSligMotions::Motion_32_Sleeping);
                VUpdateAnimData();
            }
            break;

        case relive::Path_Slig_Data::StartState::Chase:
            SetBrain(&Slig::Brain_18_StartChasing);
            field_120_timer = sGnFrame + mSligTlv.mData.mTimeToWaitBeforeChase;
            break;

        case relive::Path_Slig_Data::StartState::ChaseAndDisappear:
            SetBrain(&Slig::Brain_35_ChaseAndDisappear);
            field_120_timer = sGnFrame + mSligTlv.mData.mPauseTime;
            break;;

        case relive::Path_Slig_Data::StartState::ListeningToGlukkon:
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
                            pGlukkon->mCurrentLevel,
                            pGlukkon->mCurrentPath,
                            pGlukkon->mXPos,
                            pGlukkon->mYPos,
                            0))
                    {
                        field_208_glukkon_obj_id = pGlukkon->mBaseGameObjectId;
                        sSligsUnderControlCount_BAF7E8++;
                        mFollowGlukkon = true;
                        SetBrain(&Slig::Brain_4_ListeningToGlukkon);
                        mBrainSubState = Brain_ListeningToGlukkon_States::IdleListening_1;
                        break;
                    }
                }
            }

            if (field_208_glukkon_obj_id == Guid{})
            {
                SetBrain(&Slig::Brain_32_Inactive);
            }
            break;

        default:
            SetBrain(&Slig::Brain_33_Paused);
            break;
    }

    if (mSligTlv.mData.mFacing == relive::reliveXDirection::eLeft)
    {
        GetAnimation().SetFlipX(true);
    }

    field_290_points_count = 0;

    field_268_points[field_290_points_count].x = FP_GetExponent(mXPos);
    field_268_points[field_290_points_count].y = FP_GetExponent(mYPos);
    field_290_points_count++;

    for (s16 yCam = -3; yCam < 4; yCam++)
    {
        for (s16 xCam = -3; xCam < 4; xCam++)
        {
            relive::Path_TLV* pTlvIter = gPathInfo->Get_First_TLV_For_Offsetted_Camera(xCam, yCam);
            while (pTlvIter)
            {
                bool addPoint = false;
                if (pTlvIter->mTlvType == ReliveTypes::eSligBoundLeft)
                {
                    if (static_cast<relive::Path_SligBoundLeft*>(pTlvIter)->mSligBoundId == mSligTlv.mData.mSligBoundId)
                    {
                        field_138_zone_rect.x = pTlvIter->mTopLeftX;
                        addPoint = true;
                    }
                }
                else if (pTlvIter->mTlvType == ReliveTypes::eSligBoundRight)
                {
                    if (static_cast<relive::Path_SligBoundRight*>(pTlvIter)->mSligBoundId == mSligTlv.mData.mSligBoundId)
                    {
                        field_138_zone_rect.w = pTlvIter->mTopLeftX;
                        addPoint = true;
                    }
                }
                else if (pTlvIter->mTlvType == ReliveTypes::eSligPersist)
                {
                    if (static_cast<relive::Path_SligPersist*>(pTlvIter)->mSligBoundId == mSligTlv.mData.mSligBoundId)
                    {
                        addPoint = true;
                    }
                }

                if (addPoint)
                {
                    if (field_290_points_count < ALIVE_COUNTOF(field_268_points))
                    {
                        field_268_points[field_290_points_count].x = pTlvIter->mTopLeftX;
                        field_268_points[field_290_points_count].y = pTlvIter->mTopLeftY;
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
    if (sControlledCharacter == this)
    {
        sControlledCharacter = sActiveHero;

        MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, this, 0, 0);

        if (gMap.mNextLevel != EReliveLevelIds::eMenu)
        {
            gMap.SetActiveCam(
                mAbeLevel,
                mAbePath,
                mAbeCamera,
                CameraSwapEffects::eInstantChange_0,
                0,
                0);
        }
    }

    relive::Path_TLV* pTlv = gPathInfo->TLV_From_Offset_Lvl_Cam(field_118_tlvInfo);
    if (pTlv)
    {
        if (pTlv->mTlvType != ReliveTypes::eSligGetPants && pTlv->mTlvType != ReliveTypes::eSligSpawner)
        {
            if (mHealth <= FP_FromInteger(0))
            {
                pTlv->mTlvFlags.Clear(relive::TlvFlags::eBit1_Created);
                pTlv->mTlvFlags.Set(relive::TlvFlags::eBit2_Destroyed);
            }
            else
            {
                pTlv->mTlvFlags.Clear(relive::TlvFlags::eBit1_Created);
                pTlv->mTlvFlags.Clear(relive::TlvFlags::eBit2_Destroyed);
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

void Slig::HandleDDCheat()
{
    BaseAliveGameObjectCollisionLine = nullptr;

    if (Input().IsAnyHeld(
        InputCommands::eUp |
        InputCommands::eDown |
        InputCommands::eLeft |
        InputCommands::eRight
    ))
    {
        mVelX = dword_5473E8[Input().mPads[sCurrentControllerIndex].mDir >> 5];
        mVelY = dword_547408[Input().mPads[sCurrentControllerIndex].mDir >> 5];

        if (Input().IsAnyHeld(InputCommands::eRun))
        {
            mVelX += dword_5473E8[Input().mPads[sCurrentControllerIndex].mDir >> 5];
            mVelX += dword_5473E8[Input().mPads[sCurrentControllerIndex].mDir >> 5];
            mVelY += dword_547408[Input().mPads[sCurrentControllerIndex].mDir >> 5];
        }

        mXPos += mVelX;
        mYPos += mVelY;

        // Keep in the map bounds
        PSX_Point mapBounds = {};
        gMap.Get_map_size(&mapBounds);

        if (mXPos < FP_FromInteger(0))
        {
            mXPos = FP_FromInteger(0);
        }

        if (mXPos >= FP_FromInteger(mapBounds.x))
        {
            mXPos = FP_FromInteger(mapBounds.x) - FP_FromInteger(1);
        }

        if (mYPos < FP_FromInteger(0))
        {
            mYPos = FP_FromInteger(0);
        }

        if (mYPos >= FP_FromInteger(mapBounds.y))
        {
            mYPos = FP_FromInteger(mapBounds.y) - FP_FromInteger(1);
        }
    }
    else
    {
        mVelX = FP_FromInteger(0);
        mVelY = FP_FromInteger(0);
    }

    SetActiveCameraDelayedFromDir();

    BaseAliveGameObjectLastLineYPos = mYPos;
}

void Slig::VUpdate()
{
    if (GetRestoredFromQuickSave())
    {
        if (BaseAliveGameObjectCollisionLineType != -1)
        {
            gCollisions->Raycast(
                mXPos,
                mYPos - FP_FromInteger(20),
                mXPos,
                mYPos + FP_FromInteger(20),
                &BaseAliveGameObjectCollisionLine,
                &mXPos,
                &mYPos,
                CollisionMask(static_cast<eLineTypes>(BaseAliveGameObjectCollisionLineType)));

            if (BaseAliveGameObjectCollisionLine)
            {
                if (BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eDynamicCollision_32 || BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eBackgroundDynamicCollision_36)
                {
                    const PSX_RECT bRect = VGetBoundingRect();
                    OnCollisionWith(
                        {bRect.x, static_cast<s16>(bRect.y + 5)},
                        {bRect.w, static_cast<s16>(bRect.h + 5)},
                        gPlatformsArray);
                }
            }
        }
        field_208_glukkon_obj_id = BaseGameObject::RefreshId(field_208_glukkon_obj_id);
        if (BrainIs(&Slig::Brain_4_ListeningToGlukkon))
        {
            sSligsUnderControlCount_BAF7E8++;
        }
        SetRestoredFromQuickSave(false);
        BaseAliveGameObjectCollisionLineType = 0;
    }

    if (!Input_IsChanting())
    {
        mPreventDepossession &= ~1u;
    }

    if (sControlledCharacter == this && mHealth > FP_FromInteger(0))
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::ePossessed_9, this, 0, 0);
    }

    if (gDDCheat_FlyingEnabled && sControlledCharacter == this)
    {
        HandleDDCheat();
    }
    else
    {
        const auto oldMotion = mCurrentMotion;
        mBrainSubState = (this->*mBrainState)();

        if (mbGotShot)
        {
            VShot();
        }

        if (gDDCheat_ShowAI_Info)
        {
            DDCheat::DebugStr("Slig %d %d %d %d\n", mBrainSubState, field_120_timer, mCurrentMotion, mNextMotion);
        }

        const FP oldXPos = mXPos;
        const FP oldYPos = mYPos;

        InvokeMemberFunction(this, sSligMotionTable, mCurrentMotion);

        if (oldXPos != mXPos || oldYPos != mYPos)
        {
            BaseAliveGameObjectPathTLV = gPathInfo->TlvGetAt(
                nullptr,
                mXPos,
                mYPos,
                mXPos,
                mYPos);
            VOnTlvCollision(BaseAliveGameObjectPathTLV);
        }

        if (oldMotion != mCurrentMotion || mbMotionChanged)
        {
            mbMotionChanged = false;
            VUpdateAnimData();
        }
        else if (mReturnToPreviousMotion)
        {
            mCurrentMotion = mPreviousMotion;
            VUpdateAnimData();
            GetAnimation().SetFrame(mBaseAliveGameObjectLastAnimFrame);
            mReturnToPreviousMotion = false;
        }
    }
}

void Slig::VScreenChanged()
{
    if (gMap.LevelChanged() || (gMap.PathChanged() && this != sControlledCharacter))
    {
        SetDead(true);
    }
}

void Slig::VPossessed()
{
    SetPossessed(true);
    mPreventDepossession |= 1u;
    if (GetNextMotion() != eSligMotions::Motion_35_KnockbackToStand && GetNextMotion() != eSligMotions::Motion_34_Knockback)
    {
        SetNextMotion(eSligMotions::Motion_0_StandIdle);
    }
    SetBrain(&Slig::Brain_2_Possessed);
    mBrainSubState = Brain_2_Possessed::eBrain2_StartPossession_0;

    mAbeLevel = gMap.mCurrentLevel;
    mAbePath = gMap.mCurrentPath;
    mAbeCamera = gMap.mCurrentCamera;

    MusicController::static_PlayMusic(MusicController::MusicTypes::ePossessed_9, this, 1, 0);
}

void Slig::VUnPosses()
{
    SetNextMotion(eSligMotions::Motion_0_StandIdle);
    field_120_timer = MakeTimer(180);
    MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, this, 0, 0);
}

void Slig::VOnTlvCollision(relive::Path_TLV* pTlv)
{
    while (pTlv)
    {
        if (pTlv->mTlvType == ReliveTypes::eDeathDrop)
        {
            if (mHealth > FP_FromInteger(0))
            {
                mHealth = FP_FromInteger(0);
                mBrainSubState = 0;
                SetBrain(&Slig::Brain_3_DeathDropDeath);
                mVelY = FP_FromInteger(0);
                mVelX = FP_FromInteger(0);
                VSetMotion(static_cast<s16>(eSligMotions::Motion_7_Falling));
                EventBroadcast(kEventMudokonComfort, this);
            }
        }
        pTlv = gPathInfo->TlvGetAt(pTlv, mXPos, mYPos, mXPos, mYPos);
    }
}

void Slig::VShot()
{
    if (field_136_shot_motion != eSligMotions::eNone_m1)
    {
        SetCurrentMotion(field_136_shot_motion);
    }

    mbGotShot = false;
    SetNextMotion(eSligMotions::eNone_m1);
    field_136_shot_motion = eSligMotions::eNone_m1;
    SetBrain(&Slig::Brain_0_Death);
    field_120_timer = MakeTimer(60);
}

void Slig::VUpdateAnimData()
{
    // note: OG was falling back to eSligMotions::Motion_0_StandIdle if 
    // ResForMotion_4B1E90 didnt return a resource (which can happen depending on disabled resources value)
    GetAnimation().Set_Animation_Data(GetAnimRes(MotionToAnimId(GetCurrentMotion())));
}

bool Slig::vUnderGlukkonCommand_4B1760()
{
    return BrainIs(&Slig::Brain_4_ListeningToGlukkon);
}

void Slig::WakeUp()
{
    SetNextMotion(eSligMotions::Motion_33_SleepingToStand);
    SetBrain(&Slig::Brain_31_WakingUp);

    MusicController::static_PlayMusic(MusicController::MusicTypes::eTension_4, this, 0, 0);
    relive::Path_TLV* pTlv = gPathInfo->TLV_Get_At(
        mSligTlv.mTopLeftX,
        mSligTlv.mTopLeftY,
        mSligTlv.mTopLeftX,
        mSligTlv.mTopLeftY,
        ReliveTypes::eSlig);
    if (pTlv)
    {
        pTlv->mTlvSpecificMeaning = 1; // TODO: Keep track of these, 1 must keep slig awake ??
    }
}

void Slig::ShouldStillBeAlive()
{
    if (!field_15C_force_alive_state)
    {
        // Check not falling and not in the current screen
        if (GetCurrentMotion() != eSligMotions::Motion_7_Falling
            && GetCurrentMotion() != eSligMotions::Motion_38_OutToFall
            && !gMap.Is_Point_In_Current_Camera(
                mCurrentLevel,
                mCurrentPath,
                mXPos,
                mYPos,
                0))
        {
            if (field_290_points_count <= 0)
            {
                // No patrol points, die
                SetDead(true);
            }
            else
            {
                bool anyPointInCamera = false;
                for (s32 i = 0; i < field_290_points_count; i++)
                {
                    if (gMap.Is_Point_In_Current_Camera(
                            mCurrentLevel,
                            mCurrentPath,
                            FP_FromInteger(field_268_points[i].x),
                            FP_FromInteger(field_268_points[i].y),
                            0))

                    {
                        anyPointInCamera = true;
                        break;
                    }
                }

                if (!anyPointInCamera)
                {
                    // TODO: Remove me when making a new recording
                    if (gMap.Is_Point_In_Current_Camera(
                            mCurrentLevel,
                            mCurrentPath,
                            FP_FromInteger(field_290_points_count),
                            FP_FromInteger(mPreventDepossession),
                            0))

                    {
                        anyPointInCamera = true;
                    }

                    if (!anyPointInCamera)
                    {
                        // No patrol points in current camera
                        SetDead(true);
                    }
                }
            }
        }
    }
}

void Slig::ToTurn()
{
    SetNextMotion(eSligMotions::Motion_5_TurnAroundStanding);
    SetBrain(&Slig::Brain_19_Turning);
}

void Slig::RespondWithWhat()
{
    field_120_timer = MakeTimer(20);
    if (VIsFacingMe(sControlledCharacter))
    {
        SetNextMotion(eSligMotions::Motion_0_StandIdle);
    }
    else
    {
        SetNextMotion(eSligMotions::Motion_5_TurnAroundStanding);
    }
    SetBrain(&Slig::Brain_25_DiscussionWhat);
}

void Slig::ToShoot()
{
    SetNextMotion(eSligMotions::Motion_6_Shoot);
    SetBrain(&Slig::Brain_29_Shooting);
    field_158_num_times_to_shoot = 0;
    MusicController::static_PlayMusic(MusicController::MusicTypes::eSoftChase_8, this, 0, 0);
}

void Slig::ToZShoot()
{
    SetNextMotion(eSligMotions::Motion_0_StandIdle);
    field_120_timer = sGnFrame + mSligTlv.mData.mZShootDelay;
    SetBrain(&Slig::Brain_30_ZSpottedEnemy);
    MusicController::static_PlayMusic(MusicController::MusicTypes::eSoftChase_8, this, 0, 0);
}

void Slig::PauseALittle()
{
    if (GetAnimation().GetFlipX())
    {
        field_120_timer = mSligTlv.mData.mPauseLeftMin + sGnFrame;
        if (mSligTlv.mData.mPauseLeftMax > mSligTlv.mData.mPauseLeftMin)
        {
            field_120_timer += Math_NextRandom() % (mSligTlv.mData.mPauseLeftMax - mSligTlv.mData.mPauseLeftMin);
        }
    }
    else
    {
        field_120_timer = mSligTlv.mData.mPauseRightMin + sGnFrame;
        if (mSligTlv.mData.mPauseRightMax > mSligTlv.mData.mPauseRightMin)
        {
            field_120_timer += Math_NextRandom() % (mSligTlv.mData.mPauseRightMax - mSligTlv.mData.mPauseRightMin);
        }
    }
    SetNextMotion(eSligMotions::Motion_0_StandIdle);
    SetBrain(&Slig::Brain_15_Idle);
}

void Slig::ToStand()
{
    mFallingVelxScaleFactor = FP_FromInteger(0);
    mVelX = FP_FromInteger(0);
    mVelY = FP_FromInteger(0);
    SetCurrentMotion(eSligMotions::Motion_0_StandIdle);
    mInput = 0;
    field_12C_timer = Math_RandomRange(0, 60) + MakeTimer(120);
    MapFollowMe(true);
}

void Slig::BlowToGibs()
{
    relive_new Gibs(
        GibType::Slig_1,
        mXPos,
        mYPos,
        mVelX,
        mVelY,
        GetSpriteScale(),
        0);

    relive_new Blood(
        mXPos,
        mYPos - (FP_FromInteger(30) * GetSpriteScale()),
        FP_FromInteger(0),
        FP_FromInteger(0),
        GetSpriteScale(),
        20);

    New_Smoke_Particles(
        mXPos,
        mYPos - (FP_FromInteger(30) * GetSpriteScale()),
        GetSpriteScale(),
        3,
        RGB16{128, 128, 128});

    if (GetSpriteScale() == FP_FromDouble(0.5))
    {
        SfxPlayMono(relive::SoundEffects::KillEffect, 80);
        SfxPlayMono(relive::SoundEffects::FallingItemHit, 60);
    }
    else
    {
        SfxPlayMono(relive::SoundEffects::KillEffect, 127);
        SfxPlayMono(relive::SoundEffects::FallingItemHit, 90);
    }

    GetAnimation().SetRender(false);

    SetCurrentMotion(eSligMotions::Motion_0_StandIdle);
    mVelY = FP_FromInteger(0);
    mVelX = FP_FromInteger(0);
    mHealth = FP_FromInteger(0);
    MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, this, 0, 0);
    field_120_timer = MakeTimer(60);
    SetUpdateDelay(40);
    SetBrain(&Slig::Brain_0_Death);
    EventBroadcast(kEventMudokonComfort, this);
}

s16 Slig::MainMovement()
{
    if (sControlledCharacter == this && mHealth > FP_FromInteger(0))
    {
        return HandlePlayerControlled();
    }

    if (GetNextMotion() == eSligMotions::Motion_5_TurnAroundStanding)
    {
        SetCurrentMotion(eSligMotions::Motion_5_TurnAroundStanding);
        mNextMotion = -1;
        return 1;
    }

    if (GetNextMotion() != eSligMotions::Motion_2_Walking)
    {
        if (GetNextMotion() == eSligMotions::Motion_4_Running)
        {
            if (GetAnimation().GetFlipX())
            {
                mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
            }
            else
            {
                mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
            }
            SetCurrentMotion(eSligMotions::Motion_3_StandToRun);
            mNextMotion = -1;
            return 1;
        }

        if (GetNextMotion() == eSligMotions::Motion_6_Shoot || GetNextMotion() == eSligMotions::Motion_42_ShootZ)
        {
            mCurrentMotion = mNextMotion;
            mNextMotion = -1;
            field_12C_timer = MakeTimer(60);
            mFallingVelxScaleFactor = FP_FromInteger(0);
            mVelX = FP_FromInteger(0);
            mVelY = FP_FromInteger(0);
        }
        else
        {
            if (GetNextMotion() < eSligMotions::Motion_20_SpeakHereBoy || GetNextMotion() > eSligMotions::Motion_31_Blurgh)
            {
                if (GetNextMotion() == eSligMotions::Motion_12_ReloadGun || GetNextMotion() == eSligMotions::Motion_45_PullLever || GetNextMotion() == eSligMotions::Motion_14_SteppingToStand || GetNextMotion() == eSligMotions::Motion_46_LiftGrip || GetNextMotion() == eSligMotions::Motion_51_Beat)
                {
                    mCurrentMotion = mNextMotion;
                    SetNextMotion(eSligMotions::eNone_m1);
                    return 1;
                }
                else
                {
                    ToStand();
                    return 0;
                }
            }

            SetCurrentMotion(GetNextMotionIncGameSpeak(0));
            if (GetCurrentMotion() == eSligMotions::eNone_m1)
            {
                ToStand();
                return 0;
            }

            if (!BrainIs(&Slig::Brain_4_ListeningToGlukkon))
            {
                EventBroadcast(kEventSpeaking, this);
                return 1;
            }
        }
        return 1;
    }

    if (GetAnimation().GetFlipX())
    {
        if (WallHit(GetSpriteScale() * FP_FromInteger(45), -ScaleToGridSize(GetSpriteScale())))
        {
            return 0;
        }
        mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(9));
    }
    else
    {
        if (WallHit(GetSpriteScale() * FP_FromInteger(45), ScaleToGridSize(GetSpriteScale())))
        {
            return 0;
        }
        mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(9));
    }

    SetCurrentMotion(eSligMotions::Motion_1_StandToWalk);
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
            StandToRunVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
            StandingToStepVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(9));
            SteppingToStandVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(6));
            offX1 = -(ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(2));
            offX2 = -(ScaleToGridSize(GetSpriteScale()));
            break;

        case MovementDirection::eRight:
            StandToRunVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
            StandingToStepVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(9));
            SteppingToStandVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(6));
            offX1 = (ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(2));
            offX2 = (ScaleToGridSize(GetSpriteScale()));
            break;

        default:
            break;
    }

    if ((direction == MovementDirection::eLeft && !GetAnimation().GetFlipX()) || (direction == MovementDirection::eRight && GetAnimation().GetFlipX()))
    {
        SetCurrentMotion(eSligMotions::Motion_5_TurnAroundStanding);
        return 1;
    }

    if (Input().IsAnyHeld(InputCommands::eRun))
    {
        mVelX = StandToRunVelX;
        SetCurrentMotion(eSligMotions::Motion_3_StandToRun);
        return 1;
    }

    if (!WallHit(GetSpriteScale() * FP_FromInteger(45), offX1))
    {
        mVelX = StandingToStepVelX;
        SetCurrentMotion(eSligMotions::Motion_15_StandingToStep);
        return 1;
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(45), offX2))
    {
        return 0;
    }

    mVelX = SteppingToStandVelX;
    SetCurrentMotion(eSligMotions::Motion_14_SteppingToStand);
    return 1;
}

void Slig::PullLever()
{
    FP switchXPos = {};
    FP switchYPos = {};
    if (GetAnimation().GetFlipX())
    {
        switchXPos = mYPos - FP_FromInteger(5);
        switchYPos = mXPos - ScaleToGridSize(GetSpriteScale());
    }
    else
    {
        switchXPos = mYPos - FP_FromInteger(5);
        switchYPos = ScaleToGridSize(GetSpriteScale()) + mXPos;
    }

    auto pSwitch = static_cast<Lever*>(FindObjectOfType(ReliveTypes::eLever, switchYPos, switchXPos));
    if (pSwitch && !GetTeleporting())
    {
        pSwitch->VPull(mXPos < pSwitch->mXPos);
        SetCurrentMotion(eSligMotions::Motion_45_PullLever);
        return;
    }

    Slig_GameSpeak_SFX(SligSpeak::eWhat_9, 0, field_11E_pitch_min, this);
    SetCurrentMotion(eSligMotions::Motion_29_SpeakWhat);
}

s16 Slig::ToShootZ()
{
    if (Input().IsAnyHeld(InputCommands::eThrowItem) && GetSpriteScale() == FP_FromDouble(0.5) && !GetTeleporting())
    {
        SetCurrentMotion(eSligMotions::Motion_42_ShootZ);
        mNextMotion = -1;
        field_12C_timer = MakeTimer(60);
        return 1;
    }
    return 0;
}

void Slig::ShootOrShootZ()
{
    if (!Input().IsAnyHeld(InputCommands::eDown) || GetSpriteScale() != FP_FromDouble(0.5) || GetTeleporting())
    {
        SetCurrentMotion(eSligMotions::Motion_6_Shoot);
    }
    else
    {
        SetCurrentMotion(eSligMotions::Motion_42_ShootZ);
    }

    mNextMotion = -1;
    field_12C_timer = MakeTimer(60);
}

s16 Slig::GrabNearbyLift()
{
    auto pLiftPoint = static_cast<LiftPoint*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    if (pLiftPoint && pLiftPoint->Type() == ReliveTypes::eLiftPoint)
    {
        const FP halfGrid = ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(2);
        const FP midSwitchX = FP_FromInteger((BaseAliveGameObjectCollisionLine->mRect.x + BaseAliveGameObjectCollisionLine->mRect.w) / 2);
        const FP distToSwitchMid = (mXPos - midSwitchX) >= FP_FromInteger(0) ? mXPos - midSwitchX : midSwitchX - mXPos;
        if (distToSwitchMid < halfGrid)
        {
            SetCurrentMotion(eSligMotions::Motion_46_LiftGrip);
            return 1;
        }
    }
    return 0;
}

s16 Slig::HandlePlayerControlled()
{
    if (Input().IsAnyHeld(InputCommands::eRight))
    {
        return LeftRigtMovement(MovementDirection::eRight);
    }

    if (Input().IsAnyHeld(InputCommands::eLeft))
    {
        return LeftRigtMovement(MovementDirection::eLeft);
    }

    if (Input().IsAnyPressed(InputCommands::eDoAction))
    {
        PullLever();
        return 1;
    }

    if (Input().IsAnyHeld(InputCommands::eThrowItem))
    {
        ShootOrShootZ();
        return 1;
    }

    if (Input().IsAnyHeld(InputCommands::eFartOrRoll))
    {
        SetCurrentMotion(eSligMotions::Motion_51_Beat);
        field_12C_timer = MakeTimer(60);
        return 1;
    }

    if (Input().IsAnyHeld(InputCommands::eDown))
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
    else if (Input().IsAnyHeld(InputCommands::eUp))
    {
        const auto GrabLift = GrabNearbyLift();
        if (GrabLift)
        {
            return GrabLift;
        }

        if (Input().IsAnyPressed(InputCommands::eUp))
        {
            Slig_GameSpeak_SFX(SligSpeak::eWhat_9, 0, field_11E_pitch_min, this);
            SetCurrentMotion(eSligMotions::Motion_29_SpeakWhat);
            return 1;
        }
    }
    else if (Input().IsAnyPressed(InputCommands::eFartOrRoll | InputCommands::eHop))
    {
        Slig_GameSpeak_SFX(SligSpeak::eBlurgh_11, 0, field_11E_pitch_min, this);
        SetCurrentMotion(eSligMotions::Motion_31_Blurgh);
        return 1;
    }
    ToStand();
    return 0;
}

eSligMotions Slig::GetNextMotionIncGameSpeak(s32 input)
{
    if (sControlledCharacter == this && mHealth > FP_FromInteger(0))
    {
        if (Input_IsChanting())
        {
            if (mPreventDepossession & 1)
            {
                return eSligMotions::eNone_m1;
            }
            field_12C_timer = MakeTimer(30);
            SfxPlayMono(relive::SoundEffects::PossessEffect, 0);
            return eSligMotions::Motion_36_Depossessing;
        }

        if (InputCommands::eGameSpeak2 & input)
        {
            SetNextMotion(eSligMotions::Motion_20_SpeakHereBoy);
        }
        else if (input & InputCommands::eGameSpeak1)
        {
            SetNextMotion(eSligMotions::Motion_21_SpeakHi);
        }
        else if (input & InputCommands::eGameSpeak3)
        {
            if (Input().Is_Demo_Playing_45F220() != 0)
            {
                SetNextMotion(eSligMotions::Motion_23_SpeakGetHim);
            }
            else
            {
                SetNextMotion(eSligMotions::Motion_22_SpeakFreeze);
            }
        }
        else if (input & InputCommands::eGameSpeak4)
        {
            if (Input().Is_Demo_Playing_45F220() != 0)
            {
                SetNextMotion(eSligMotions::Motion_22_SpeakFreeze);
            }
            else
            {
                SetNextMotion(eSligMotions::Motion_23_SpeakGetHim);
            }
        }
        else if (input & InputCommands::eGameSpeak8)
        {
            SetNextMotion(eSligMotions::Motion_24_SpeakLaugh);
        }
        else if (input & InputCommands::eGameSpeak6)
        {
            SetNextMotion(eSligMotions::Motion_25_SpeakBullshit1);
        }
        else if (input & InputCommands::eGameSpeak7)
        {
            SetNextMotion(eSligMotions::Motion_26_SpeakLookOut);
        }
        else if (input & InputCommands::eGameSpeak5)
        {
            SetNextMotion(eSligMotions::Motion_27_SpeakBullshit2);
        }
        else
        {
            return eSligMotions::eNone_m1;
        }

        GameSpeakEvents event = GameSpeakEvents::eNone_m1;
        switch (GetNextMotion())
        {
            case eSligMotions::Motion_20_SpeakHereBoy:
                event = GameSpeakEvents::eSlig_HereBoy_28;
                break;
            case eSligMotions::Motion_21_SpeakHi:
                event = GameSpeakEvents::eSlig_Hi_27;
                break;
            case eSligMotions::Motion_22_SpeakFreeze:
                event = GameSpeakEvents::eSlig_Freeze_31;
                break;
            case eSligMotions::Motion_23_SpeakGetHim:
                event = GameSpeakEvents::eSlig_GetEm_29;
                break;
            case eSligMotions::Motion_24_SpeakLaugh:
                event = GameSpeakEvents::Slig_Laugh_8;
                break;
            case eSligMotions::Motion_25_SpeakBullshit1:
                event = GameSpeakEvents::Slig_BS_5;
                break;
            case eSligMotions::Motion_26_SpeakLookOut:
                event = GameSpeakEvents::Slig_LookOut_6;
                break;
            case eSligMotions::Motion_27_SpeakBullshit2:
                event = GameSpeakEvents::Slig_BS2_7;
                break;
        }

        gEventSystem->PushEvent(event);
    }

    // TODO: don't use >= and <= for the motion checks
    if (GetNextMotion() >= eSligMotions::Motion_20_SpeakHereBoy && GetNextMotion() <= eSligMotions::Motion_31_Blurgh)
    {
        SligSpeak speak = SligSpeak::eNone;
        switch (GetNextMotion())
        {
            case eSligMotions::Motion_20_SpeakHereBoy:
                speak = SligSpeak::eHereBoy_1;
                break;
            case eSligMotions::Motion_21_SpeakHi:
                speak = SligSpeak::eHi_0;
                break;
            case eSligMotions::Motion_22_SpeakFreeze:
                speak = SligSpeak::eFreeze_8;
                break;
            case eSligMotions::Motion_23_SpeakGetHim:
                speak = SligSpeak::eGetHim_2;
                break;
            case eSligMotions::Motion_24_SpeakLaugh:
                speak = SligSpeak::eLaugh_3;
                break;
            case eSligMotions::Motion_25_SpeakBullshit1:
                speak = SligSpeak::eBullshit_5;
                break;
            case eSligMotions::Motion_26_SpeakLookOut:
                speak = SligSpeak::eLookOut_6;
                break;
            case eSligMotions::Motion_27_SpeakBullshit2:
                speak = SligSpeak::eBullshit2_7;
                break;
            case eSligMotions::Motion_28_SpeakPanic:
                speak = SligSpeak::eHelp_10;
                break;
            case eSligMotions::Motion_29_SpeakWhat:
                speak = SligSpeak::eWhat_9;
                break;
            case eSligMotions::Motion_30_SpeakAIFreeze:
                speak = SligSpeak::eFreeze_8;
                break;
            case eSligMotions::Motion_31_Blurgh:
                speak = SligSpeak::eBlurgh_11;
                break;
        }

        Slig_GameSpeak_SFX(speak, 0, field_11E_pitch_min, this);
        mCurrentMotion = mNextMotion;
        SetNextMotion(eSligMotions::eNone_m1);
        return GetCurrentMotion();
    }

    return eSligMotions::eNone_m1;
}

void Slig::WaitOrWalk()
{
    SetNextMotion(eSligMotions::Motion_2_Walking);
    SetBrain(&Slig::Brain_21_Walking);

    // Right rect bound
    if (!IsFacingEffectiveLeft(this) && (ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(2)) + mXPos >= FP_FromInteger(field_138_zone_rect.w))
    {
        PauseALittle();
        MusicController::static_PlayMusic(MusicController::MusicTypes::eTension_4, this, 0, 0);
        return;
    }

    // Left rect bound
    if (IsFacingEffectiveLeft(this) && (mXPos - (ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(2))) <= FP_FromInteger(field_138_zone_rect.x))
    {
        PauseALittle();
        MusicController::static_PlayMusic(MusicController::MusicTypes::eTension_4, this, 0, 0);
        return;
    }

    Brain_21_Walking();
    MusicController::static_PlayMusic(MusicController::MusicTypes::eTension_4, this, 0, 0);
}

void Slig::ToAbeDead()
{
    SetNextMotion(eSligMotions::Motion_24_SpeakLaugh);
    SetBrain(&Slig::Brain_10_EnemyDead);
    field_120_timer = MakeTimer(45);
}

void Slig::ToUnderGlukkonCommand()
{
    SetBrain(&Slig::Brain_4_ListeningToGlukkon);
    mBrainSubState = Brain_ListeningToGlukkon_States::GettingAttention_0;
}

void Slig::ToKilledAbe()
{
    SetNextMotion(eSligMotions::Motion_0_StandIdle);
    SetBrain(&Slig::Brain_11_KilledEnemy);
    field_120_timer = MakeTimer(15);
}

bool Slig::IsWallBetween(IBaseAliveGameObject* pLeft, IBaseAliveGameObject* pRight)
{
    const PSX_RECT thisBRect =  VGetBoundingRect();
    const PSX_RECT rightBRect = pRight->VGetBoundingRect();

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    return gCollisions->Raycast(
               pLeft->mXPos,
               FP_FromInteger(thisBRect.h - 25),
               pRight->mXPos,
               FP_FromInteger(rightBRect.h - 25),
               &pLine, &hitX, &hitY, pLeft->GetScale() == Scale::Fg ? kFgWalls : kBgWalls)
        == 1;
}

GameSpeakEvents Slig::LastGlukkonSpeak()
{
    if (!gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 1))
    {
        return GameSpeakEvents::eNone_m1;
    }

    const s32 last_event_idx = gEventSystem->mLastEventIndex;
    if (field_160_last_event_index == last_event_idx)
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

    field_160_last_event_index = last_event_idx;

    return gEventSystem->mLastEvent;
}

s16 Slig::ListenToGlukkonCommands()
{
    const GameSpeakEvents glukkonSpeak = LastGlukkonSpeak();
    if (sControlledCharacter->GetSpriteScale() != GetSpriteScale())
    {
        return 0;
    }

    if (glukkonSpeak == GameSpeakEvents::eGlukkon_Hey_36)
    {
        if (!HeardGlukkonToListenTo(GameSpeakEvents::eGlukkon_Hey_36))
        {
            return 0;
        }
    }
    else if (glukkonSpeak != GameSpeakEvents::eGlukkon_AllOYa_40)
    {
        return 0;
    }

    sSligsUnderControlCount_BAF7E8++;

    field_208_glukkon_obj_id = sControlledCharacter->mBaseGameObjectId;

    if (glukkonSpeak == GameSpeakEvents::eGlukkon_AllOYa_40)
    {
        mGlukkonCalledAllOYa = true;
    }

    return 1;
}

void Slig::PlatformCollide()
{
    PSX_Point xy;
    xy.x = FP_GetExponent(mXPos - FP_FromInteger(5));
    xy.y = FP_GetExponent(mYPos - FP_FromInteger(5));

    PSX_Point wh;
    wh.x = FP_GetExponent(mXPos + FP_FromInteger(5));
    wh.y = FP_GetExponent(mYPos + FP_FromInteger(5));

    OnCollisionWith(xy, wh, gPlatformsArray);
}

void Slig::ToKnockBack()
{
    Environment_SFX(EnvironmentSfx::eKnockback_13, 0, 0x7FFF, this);

    mXPos -= mVelX;

    if (BaseAliveGameObjectCollisionLine)
    {
        if (GetAnimation().GetFlipX())
        {
            if (!WallHit((GetSpriteScale() * FP_FromInteger(45)), (ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(2))))
            {
                mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
            }
            else
            {
                mVelX = FP_FromInteger(0);
            }
        }
        else
        {
            if (!WallHit((GetSpriteScale() * FP_FromInteger(45)), (-ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(2))))
            {
                mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
            }
            else
            {
                mVelX = FP_FromInteger(0);
            }
        }
    }
    else
    {
        mVelX = FP_FromInteger(0);
    }

    if (GetNextMotion() == eSligMotions::Motion_44_Smash)
    {
        mVelX = -mVelX;
    }

    MapFollowMe(true);

    if (mVelY < FP_FromInteger(0))
    {
        mVelY = FP_FromInteger(0);
    }

    SetCurrentMotion(eSligMotions::Motion_34_Knockback);
    field_12C_timer = MakeTimer(10);
}

void Slig::TurnOrSayWhat()
{
    if (EventGet(kEventSpeaking) && !IsInInvisibleZone(sControlledCharacter) && !sControlledCharacter->GetInvisible())
    {
        GameSpeakResponse();
        RespondWithWhat();
    }
    else
    {
        SetNextMotion(eSligMotions::Motion_5_TurnAroundStanding);
        SetBrain(&Slig::Brain_22_GetAlertedTurn);
    }
}

void Slig::GameSpeakResponse()
{
    const s32 lastIdx = gEventSystem->mLastEventIndex;

    GameSpeakEvents speak = GameSpeakEvents::eNone_m1;

    if (field_160_last_event_index == lastIdx)
    {
        if (gEventSystem->mLastEvent == GameSpeakEvents::eNone_m1)
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
        speak = gEventSystem->mLastEvent;
    }

    switch (speak)
    {
        case GameSpeakEvents::eUnknown_1:
            if (!(Math_NextRandom() & 4))
            {
                field_294_next_gamespeak_motion = eSligMotions::Motion_25_SpeakBullshit1;
            }
            else
            {
                field_294_next_gamespeak_motion = eSligMotions::Motion_27_SpeakBullshit2;
            }
            break;

        case GameSpeakEvents::eAbe_Fart_3:
        case GameSpeakEvents::Slig_Laugh_8:
        case GameSpeakEvents::eSlig_GetEm_29:
            field_294_next_gamespeak_motion = eSligMotions::Motion_31_Blurgh;
            return;

        case GameSpeakEvents::Slig_BS_5:
            field_294_next_gamespeak_motion = eSligMotions::Motion_25_SpeakBullshit1;
            break;

        case GameSpeakEvents::Slig_LookOut_6:
            if (!(Math_NextRandom() & 4))
            {
                field_294_next_gamespeak_motion = eSligMotions::Motion_29_SpeakWhat;
            }
            else
            {
                field_294_next_gamespeak_motion = eSligMotions::Motion_28_SpeakPanic;
            }
            return;

        case GameSpeakEvents::Slig_BS2_7:
            field_294_next_gamespeak_motion = eSligMotions::Motion_27_SpeakBullshit2;
            break;

        case GameSpeakEvents::eAbe_Hello_9:
        case GameSpeakEvents::eSlig_Hi_27:
            field_294_next_gamespeak_motion = eSligMotions::Motion_21_SpeakHi;
            break;

        case GameSpeakEvents::eAbe_FollowMe_10:
        case GameSpeakEvents::eAbe_Wait_12:
            if (Math_NextRandom() & 8)
            {
                field_294_next_gamespeak_motion = eSligMotions::Motion_25_SpeakBullshit1;
            }
            else
            {
                field_294_next_gamespeak_motion = eSligMotions::Motion_27_SpeakBullshit2;
            }
            break;

        case GameSpeakEvents::eAbe_Anger_11:
        case GameSpeakEvents::eSlig_Freeze_31:
        case GameSpeakEvents::eGlukkon_None_35:
            field_294_next_gamespeak_motion = eSligMotions::Motion_24_SpeakLaugh;
            break;

        default:
            field_294_next_gamespeak_motion = eSligMotions::Motion_29_SpeakWhat;
            break;
    }
}

void Slig::GoAlertedOrSayWhat()
{
    if (EventGet(kEventSpeaking)
        && !IsInInvisibleZone(sControlledCharacter)
        && !sControlledCharacter->GetInvisible())
    {
        GameSpeakResponse();
        RespondWithWhat();
    }
    else
    {
        SetNextMotion(eSligMotions::Motion_0_StandIdle);
        field_294_next_gamespeak_motion = eSligMotions::Motion_29_SpeakWhat;
        SetBrain(&Slig::Brain_23_GetAlerted);
        field_120_timer = sGnFrame + mSligTlv.mData.mAlertedListenTime;
    }
}

s32 Slig::IsFacingEffectiveLeft(Slig* pSlig)
{
    s32 flipX = pSlig->GetAnimation().GetFlipX();
    if (pSlig->GetCurrentMotion() == eSligMotions::Motion_5_TurnAroundStanding && pSlig->GetAnimation().GetCurrentFrame() > 4)
    {
        // Flip the result
        return flipX == 0;
    }
    else
    {
        return flipX;
    }
}

void Slig::MoveOnLine()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));

    const FP xpos = mXPos;
    if (BaseAliveGameObjectCollisionLine)
    {
        BaseAliveGameObjectCollisionLine = BaseAliveGameObjectCollisionLine->MoveOnLine(
            &mXPos,
            &mYPos,
            mVelX);
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
                PlatformCollide();
            }
        }
        else
        {
            VOnTrapDoorOpen();
            mXPos = xpos + mVelX;
            BaseAliveGameObjectLastLineYPos = mYPos;
            SetCurrentMotion(eSligMotions::Motion_39_FallingInitiate);
            mFallingVelxScaleFactor = FP_FromDouble(0.3);

            if (mVelX > (FP_FromInteger(6) * GetSpriteScale()))
            {
                mVelX = (FP_FromInteger(6) * GetSpriteScale());
            }

            if (mVelX < (-FP_FromInteger(6) * GetSpriteScale()))
            {
                mVelX = (-FP_FromInteger(6) * GetSpriteScale());
            }
        }
    }
    else
    {
        VOnTrapDoorOpen();
        SetCurrentMotion(eSligMotions::Motion_38_OutToFall);
        BaseAliveGameObjectLastLineYPos = mYPos;
    }
}

void Slig::PlayerControlStopWalkingIfRequired()
{
    // Pressing other direction to facing or not pressing a direction at all?
    if ((mVelX > FP_FromInteger(0) && Input().IsAnyHeld(InputCommands::eLeft)) || (mVelX < FP_FromInteger(0) && Input().IsAnyHeld(InputCommands::eRight)) || !Input().IsAnyHeld(InputCommands::eLeft | InputCommands::eRight))
    {
        // Then go to standing
        SetCurrentMotion(eSligMotions::Motion_18_WalkToStand);
    }
    mInput = 0;
}

void Slig::CheckPlatformVanished()
{
    BaseGameObject* pLiftPoint = sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId);
    if (pLiftPoint)
    {
        if (pLiftPoint->GetDead())
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
        BaseAliveGameObject_PlatformId = Guid{};
        BaseAliveGameObjectLastLineYPos = mYPos;
        VSetMotion(static_cast<s16>(eSligMotions::Motion_38_OutToFall));
    }
}

eSligMotions Slig::MoveLift(FP ySpeed)
{
    auto pLiftPoint = static_cast<LiftPoint*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    if (!pLiftPoint)
    {
        return eSligMotions::Motion_48_LiftGripping;
    }

    pLiftPoint->Move(FP_FromInteger(0), ySpeed);
    CheckPlatformVanished();
    mVelY = pLiftPoint->mVelY;

    if (!GetAnimation().GetIsLastFrame() && GetAnimation().GetCurrentFrame() != 5)
    {
        return GetCurrentMotion();
    }

    if (ySpeed < FP_FromInteger(0))
    {
        if (pLiftPoint->vOnTopFloor())
        {
            return eSligMotions::Motion_48_LiftGripping;
        }

        if (sControlledCharacter != this)
        {
            return GetCurrentMotion();
        }

        if (Input().IsAnyHeld(InputCommands::eUp))
        {
            return eSligMotions::Motion_49_LiftUp;
        }

        if (Input().IsAnyHeld(InputCommands::eDown))
        {
            return eSligMotions::Motion_50_LiftDown;
        }
    }
    else if (ySpeed > FP_FromInteger(0))
    {
        if (pLiftPoint->vOnBottomFloor())
        {
            return eSligMotions::Motion_48_LiftGripping;
        }

        if (sControlledCharacter != this)
        {
            return GetCurrentMotion();
        }

        if (Input().IsAnyHeld(InputCommands::eDown))
        {
            return eSligMotions::Motion_50_LiftDown;
        }

        if (Input().IsAnyHeld(InputCommands::eUp))
        {
            return eSligMotions::Motion_49_LiftUp;
        }
    }

    // Strange how this isn't "if nothing pressed and on a floor then let go ??"
    if (Input().mPads[sCurrentControllerIndex].mRawInput && pLiftPoint->vOnAnyFloor())
    {
        return eSligMotions::Motion_47_LiftUngrip;
    }

    pLiftPoint->Move(FP_FromInteger(0), FP_FromInteger(0));
    return eSligMotions::Motion_48_LiftGripping;
}

void Slig::SlowDown(FP speed)
{
    if (FP_GetExponent(mVelX))
    {
        MoveOnLine();

        if (mVelX <= FP_FromInteger(0))
        {
            if (mVelX < FP_FromInteger(0))
            {
                mVelX = (GetSpriteScale() * speed) + mVelX;
                if (mVelX > FP_FromInteger(0))
                {
                    mVelX = FP_FromInteger(0);
                }
            }
        }
        else
        {
            mVelX = mVelX - (GetSpriteScale() * speed);
            if (mVelX < FP_FromInteger(0))
            {
                mVelX = FP_FromInteger(0);
            }
        }
    }
}

void Slig::ToChase()
{
    field_120_timer = sGnFrame + mSligTlv.mData.mTimeToWaitBeforeChase;

    if (!VIsFacingMe(sControlledCharacter))
    {
        GetAnimation().ToggleFlipX();
    }

    SetNextMotion(eSligMotions::Motion_0_StandIdle);
    SetBrain(&Slig::Brain_18_StartChasing);
    MusicController::static_PlayMusic(MusicController::MusicTypes::eSoftChase_8, this, 0, 0);
}

s16 Slig::HandleEnemyStopper(s32 gridBlocks)
{
    s32 directedGirdBlocks = gridBlocks;
    const auto bFacingLeft = IsFacingEffectiveLeft(this);
    if (bFacingLeft)
    {
        directedGirdBlocks = -gridBlocks;
    }

    const FP width = ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(directedGirdBlocks) + mXPos;
    auto pTlv = static_cast<relive::Path_EnemyStopper*>(gPathInfo->TLV_Get_At(
        FP_GetExponent(mXPos),
        FP_GetExponent(mYPos),
        FP_GetExponent(width),
        FP_GetExponent(mYPos - ScaleToGridSize(GetSpriteScale())),
        ReliveTypes::eEnemyStopper));

    if (!pTlv)
    {
        return 0;
    }

    if (!SwitchStates_Get(pTlv->mSwitchId))
    {
        return 0;
    }

    if (pTlv->mStopDirection == relive::Path_EnemyStopper::StopDirection::Both)
    {
        return 1;
    }

    if (bFacingLeft && pTlv->mStopDirection == relive::Path_EnemyStopper::StopDirection::Left)
    {
        return 1;
    }

    if (!bFacingLeft && pTlv->mStopDirection == relive::Path_EnemyStopper::StopDirection::Right)
    {
        return 1;
    }

    return 0;
}

void Slig::ToPanic()
{
    SetNextMotion(eSligMotions::Motion_28_SpeakPanic);
    SetBrain(&Slig::Brain_14_PanicYelling);
}

void Slig::ToPanicTurn()
{
    SetNextMotion(eSligMotions::Motion_5_TurnAroundStanding);
    SetBrain(&Slig::Brain_12_PanicTurning);
}

static s16 sSligDelayIdx = 0;
static const s16 sSligDelayTable[6] = {0, 6, 10, 8, 4, 12};

s16 Slig::SligStableDelay()
{
    // Similar to the mudokon delay stuff

    if (sSligsUnderControlCount_BAF7E8 <= 1)
    {
        return 0;
    }

    if (sSligDelayIdx >= ALIVE_COUNTOF(sSligDelayTable))
    {
        sSligDelayIdx = 0;
    }

    return sSligDelayTable[sSligDelayIdx++];
}

void Slig::PlayerControlRunningSlideStopOrTurnFrame12()
{
    const FP curVelX = mVelX;

    if (curVelX > FP_FromInteger(0) && Input().IsAnyHeld(InputCommands::eLeft))
    {
        mVelX = GetSpriteScale() * FP_FromDouble(13.2);
        SetCurrentMotion(eSligMotions::Motion_9_SlidingTurn);
        mInput = 0;
        return;
    }

    if (curVelX < FP_FromInteger(0) && Input().IsAnyHeld(InputCommands::eRight))
    {
        mVelX = (GetSpriteScale() * -FP_FromDouble(13.2));
        SetCurrentMotion(eSligMotions::Motion_9_SlidingTurn);
        mInput = 0;
        return;
    }

    if (!Input().IsAnyHeld(InputCommands::eLeft | InputCommands::eRight))
    {
        if (curVelX >= FP_FromInteger(0))
        {
            mVelX = GetSpriteScale() * FP_FromDouble(13.2);
        }
        else
        {
            mVelX = (GetSpriteScale() * -FP_FromDouble(13.2));
        }
        SetCurrentMotion(eSligMotions::Motion_8_SlidingToStand);
        mInput = 0;
        return;
    }

    if (Input().IsAnyHeld(InputCommands::eRun))
    {
        mInput = 0;
        return;
    }

    SetPreviousMotion(eSligMotions::Motion_2_Walking);
    mBaseAliveGameObjectLastAnimFrame = 15;
    mReturnToPreviousMotion = true;

    if (GetAnimation().GetFlipX())
    {
        mInput = 0;
        mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(9));
    }
    else
    {
        mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(9));
        mInput = 0;
    }
}

void Slig::PlayerControlRunningSlideStopOrTurnFrame4()
{
    const FP curVelX = mVelX;
    if (curVelX > FP_FromInteger(0) && Input().IsAnyHeld(InputCommands::eLeft))
    {
        mVelX = (GetSpriteScale() * FP_FromDouble(13.2));
        SetCurrentMotion(eSligMotions::Motion_9_SlidingTurn);
        mInput = 0;
        return;
    }

    if (curVelX < FP_FromInteger(0) && Input().IsAnyHeld(InputCommands::eRight))
    {
        mVelX = (GetSpriteScale() * -FP_FromDouble(13.2));
        SetCurrentMotion(eSligMotions::Motion_9_SlidingTurn);
        mInput = 0;
        return;
    }

    if (!Input().IsAnyHeld(InputCommands::eLeft | InputCommands::eRight))
    {
        if (curVelX >= FP_FromInteger(0))
        {
            mVelX = GetSpriteScale() * FP_FromDouble(13.2);
        }
        else
        {
            mVelX = (GetSpriteScale() * -FP_FromDouble(13.2));
        }
        SetCurrentMotion(eSligMotions::Motion_8_SlidingToStand);
        mInput = 0;
        return;
    }

    if (Input().IsAnyHeld(InputCommands::eRun))
    {
        mInput = 0;
        return;
    }

    SetPreviousMotion(eSligMotions::Motion_2_Walking);
    mBaseAliveGameObjectLastAnimFrame = 6;
    mReturnToPreviousMotion = true;

    if (GetAnimation().GetFlipX())
    {
        mInput = 0;
        mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(9));
    }
    else
    {
        mInput = 0;
        mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(9));
    }
}

IBaseAliveGameObject* Slig::FindBeatTarget(s32 gridBlocks)
{
    const FP kGridSize = ScaleToGridSize(GetSpriteScale());
    const FP k2Scaled = FP_FromInteger(2) * kGridSize;
    const FP kGridBlocksScaled = FP_FromInteger(gridBlocks) * kGridSize;

    const FP xAndW = GetAnimation().GetFlipX() ? mXPos - kGridBlocksScaled : mXPos + kGridBlocksScaled;

    PSX_RECT hitRect = MakeMinMaxRect(
        xAndW,
        mYPos,
        xAndW,
        mYPos - k2Scaled,
        true);

    for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
    {
        IBaseAliveGameObject* pObj = gBaseAliveGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj != this && pObj->Type() == ReliveTypes::eMudokon)
        {
            const PSX_RECT bRect = pObj->VGetBoundingRect();

            if (hitRect.w <= bRect.w && hitRect.x >= bRect.x && hitRect.y >= bRect.y && hitRect.h <= bRect.h && pObj->GetScale() == GetScale() && !IsInInvisibleZone(pObj) && !pObj->GetInvisible() && pObj->mHealth > FP_FromInteger(0))
            {
                return pObj;
            }
        }
    }
    return nullptr;
}

void Slig::TurnOrWalk()
{
    if (!GetAnimation().GetFlipX())
    {
        if (mXPos > FP_FromInteger(field_138_zone_rect.w) - (ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(4)))
        {
            ToTurn();
            return;
        }
    }
    else
    {
        if (mXPos < FP_FromInteger(field_138_zone_rect.x) + (ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(4)))
        {
            ToTurn();
            return;
        }
    }

    WaitOrWalk();
}

void Slig::ToPanicRunning()
{
    SetNextMotion(eSligMotions::Motion_4_Running);
    SetBrain(&Slig::Brain_13_PanicRunning);
    Brain_13_PanicRunning();
    MusicController::static_PlayMusic(MusicController::MusicTypes::eSoftChase_8, this, 0, 0);
}

void Slig::RespondToEnemyOrPatrol()
{
    if (mSligTlv.mData.mShootOnSightDelay || sControlledCharacter->GetInvisible())
    {
        if (sControlledCharacter->Type() != ReliveTypes::eSlig ||
            mSligTlv.mData.mShootPossessedSligs != relive::reliveChoice::eNo)
        {
            SetBrain(&Slig::Brain_7_SpottedEnemy);
            SetNextMotion(eSligMotions::Motion_30_SpeakAIFreeze);
            field_120_timer = sGnFrame + mSligTlv.mData.mShootOnSightDelay;
        }
        else
        {
            field_15E_spotted_possessed_slig = 1;
            TurnOrWalk();
        }
    }
    else if (VIsFacingMe(sControlledCharacter))
    {
        if (GetSpriteScale() == sControlledCharacter->GetSpriteScale())
        {
            ToShoot();
        }
        else
        {
            ToZShoot();
        }
    }
    else
    {
        ToTurn();
    }
}

s16 Slig::IsAbeEnteringDoor(IBaseAliveGameObject* pThis)
{
    if (((pThis->Type() == ReliveTypes::eAbe) && (pThis->mCurrentMotion == eAbeMotions::Motion_114_DoorEnter && pThis->GetAnimation().GetCurrentFrame() > 7)) || (pThis->mCurrentMotion == eAbeMotions::Motion_115_DoorExit && pThis->GetAnimation().GetCurrentFrame() < 4))
    {
        return 1;
    }
    return 0;
}

s16 Slig::FindLever()
{
    const s16 yPos = FP_GetExponent(mYPos - FP_FromInteger(5));
    if (gPathInfo->TLV_Get_At(FP_GetExponent(mXPos), yPos, FP_GetExponent(mXPos), yPos, ReliveTypes::eLever))
    {
        return 0;
    }

    FP xOff = ScaleToGridSize(GetSpriteScale());
    if (GetAnimation().GetFlipX())
    {
        xOff = -xOff;
    }

    return gPathInfo->TLV_Get_At(
               FP_GetExponent(FP_Abs(mXPos) + xOff),
               yPos,
               FP_GetExponent(FP_Abs(mXPos) + xOff),
               yPos,
               ReliveTypes::eLever)
        != 0;
}

s16 Slig::NearOrFacingActiveChar(IBaseAliveGameObject* pObj)
{
    if (pObj->VIsObjNearby(ScaleToGridSize(GetSpriteScale()) * FP_FromDouble(0.5), sControlledCharacter) || sControlledCharacter->VIsFacingMe(pObj))
    {
        return 1;
    }

    return 0;
}

// TODO: name conflicts
static s16 IsInZCover(relive::Path_TLV* pTlv, const PSX_RECT* pRect)
{
    if (pTlv->mTlvType == ReliveTypes::eZSligCover)
    {
        if (pRect->x >= pTlv->mTopLeftX && pRect->x <= pTlv->mBottomRightX &&

            pRect->y >= pTlv->mTopLeftY && pRect->y <= pTlv->mBottomRightY &&

            pRect->w >= pTlv->mTopLeftX && pRect->w <= pTlv->mBottomRightX &&

            pRect->h >= pTlv->mTopLeftY && pRect->h <= pTlv->mBottomRightY)
        {
            return true;
        }
    }
    return false;
}

s16 Slig::IsInZCover(IBaseAliveGameObject* pObj)
{
    const PSX_RECT bRect = pObj->VGetBoundingRect();
    return Bullet::InZBulletCover(pObj->mXPos, FP_FromInteger(bRect.y), bRect);
}

bool Slig::InAnyWellRenderLayer(IBaseAliveGameObject* pThis)
{
    return pThis->GetAnimation().GetRenderLayer() == Layer::eLayer_BeforeWell_Half_3 || pThis->GetAnimation().GetRenderLayer() == Layer::eLayer_BeforeWell_22;
}

void Slig::NextGlukkonCommand(s16 speakTableIndex, s16 responseState)
{
    field_212_next_command_arg1 = speakTableIndex;
    field_214_cmd_idx = 0;
    field_120_timer = MakeTimer(SligStableDelay());
    field_20C_state_after_speak = responseState;
}

s16 Slig::HeardGlukkonToListenTo(GameSpeakEvents glukkonSpeak)
{
    const s32 distToPlayer = Math_Distance(
        FP_GetExponent(sControlledCharacter->mXPos),
        FP_GetExponent(sControlledCharacter->mYPos),
        FP_GetExponent(mXPos),
        FP_GetExponent(mYPos));

    if (!NearOrFacingActiveChar(this))
    {
        return 0;
    }

    for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
    {
        IBaseAliveGameObject* pObj = gBaseAliveGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj != this && pObj->Type() == ReliveTypes::eSlig)
        {
            auto* pOtherSlig = static_cast<Slig*>(pObj);
            if (pOtherSlig->GetSpriteScale() == sControlledCharacter->GetSpriteScale() && gMap.Is_Point_In_Current_Camera(pOtherSlig->mCurrentLevel, pOtherSlig->mCurrentPath, pOtherSlig->mXPos, pOtherSlig->mYPos, 0) && NearOrFacingActiveChar(pOtherSlig) && (glukkonSpeak == GameSpeakEvents::eGlukkon_Hey_36 || pOtherSlig->BrainIs(&Slig::Brain_4_ListeningToGlukkon)))
            {
                if (VIsObjNearby(ScaleToGridSize(GetSpriteScale()) * FP_FromDouble(0.5), pOtherSlig))
                {
                    if ((pOtherSlig->VIsFacingMe(sControlledCharacter) && !VIsFacingMe(sControlledCharacter)) || pOtherSlig->mHeardGlukkon)
                    {
                        return 0;
                    }
                }
                else if (distToPlayer > Math_Distance(
                             FP_GetExponent(sControlledCharacter->mXPos),
                             FP_GetExponent(sControlledCharacter->mYPos),
                             FP_GetExponent(pOtherSlig->mXPos),
                             FP_GetExponent(pOtherSlig->mYPos)))
                {
                    return 0;
                }
            }
        }
    }

    mHeardGlukkon = true;
    return 1;
}

bool Slig::VTakeDamage(BaseGameObject* pFrom)
{
    if (GetTeleporting())
    {
        return false;
    }

    switch (pFrom->Type())
    {
        case ReliveTypes::eBullet:
        {
            if (mHealth <= FP_FromInteger(0) && sControlledCharacter != this)
            {
                return false;
            }

            auto pBullet = static_cast<Bullet*>(pFrom);
            switch (pBullet->mBulletType)
            {
                case BulletType::eSligPossessedOrUnderGlukkonCommand_0:
                case BulletType::eNormalBullet_2:
                {
                    {
                        const FP yOff = FP_FromInteger(Math_NextRandom() % 16) - FP_FromInteger(8);
                        const FP xOff = ((pBullet->XDistance() <= FP_FromInteger(0) ? FP_FromInteger(-1) : FP_FromInteger(1)) * FP_FromInteger(Math_NextRandom() & 15)) + FP_FromInteger(16);
                        const FP yPos = mYPos - (FP_FromInteger(25) * GetSpriteScale());
                        const FP xPos = GetSpriteScale() * (pBullet->XDistance() <= FP_FromInteger(0) ? FP_FromInteger(-6) : FP_FromInteger(6));
                        relive_new Blood(xPos + mXPos, yPos, xOff, yOff, GetSpriteScale(), 12);
                    }

                    {
                        const FP xOff = pBullet->XDistance() <= FP_FromInteger(0) ? FP_FromInteger(-6) : FP_FromInteger(6);
                        const FP yPos = mYPos - (FP_FromInteger(25) * GetSpriteScale());
                        const FP xPos = GetSpriteScale() * (pBullet->XDistance() <= FP_FromInteger(0) ? FP_FromInteger(-12) : FP_FromInteger(12));
                        relive_new Blood(xPos + mXPos, yPos, xOff, FP_FromInteger(0), GetSpriteScale(), 8);
                    }
                    break;
                }

                case BulletType::ePossessedSligZBullet_1:
                case BulletType::eZBullet_3:
                {
                    const PSX_RECT myRect = VGetBoundingRect();
                    const FP rectY = FP_FromInteger(myRect.y);

                    relive::Path_TLV* pTlvIter = nullptr;
                    for (;;)
                    {
                        pTlvIter = gPathInfo->TlvGetAt(pTlvIter, mXPos, rectY, mXPos, rectY);
                        if (!pTlvIter)
                        {
                            break;
                        }

                        if (::IsInZCover(pTlvIter, &myRect))
                        {
                            mbGotShot = false;
                            mHealth = FP_FromInteger(1);
                            return false;
                        }
                    }

                    if (!gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, rectY, 0))
                    {
                        mbGotShot = false;
                        mHealth = FP_FromInteger(1);
                        return false;
                    }

                    relive_new Blood(mXPos, mYPos - (FP_FromInteger(25) * GetSpriteScale()), FP_FromInteger(0), FP_FromInteger(0), GetSpriteScale(), 25);
                    break;
                }

                default:
                    break;
            }

            mbGotShot = true;
            SetBrain(&Slig::Brain_0_Death);

            field_14C_death_by_being_shot_timer = MakeTimer(5);
            if (GetCurrentMotion() != eSligMotions::Motion_37_Possess)
            {
                if (field_150_explode_timer > static_cast<s32>(sGnFrame) + 20)
                {
                    field_150_explode_timer = MakeTimer(20);
                }

                if (GetCurrentMotion() != eSligMotions::Motion_37_Possess && GetCurrentMotion() != eSligMotions::Motion_44_Smash && GetCurrentMotion() != eSligMotions::Motion_34_Knockback)
                {
                    SetNextMotion(eSligMotions::Motion_37_Possess);
                    field_136_shot_motion = eSligMotions::Motion_37_Possess;
                    field_150_explode_timer = MakeTimer(20);
                    VShot();
                    mbMotionChanged = true;
                    if (static_cast<Bullet*>(pFrom)->XDistance() < FP_FromInteger(0))
                    {
                        mVelX = FP_FromDouble(-0.001);
                        mHealth = FP_FromInteger(0);
                        EventBroadcast(kEventMudokonComfort, this);
                        return true;
                    }
                    mVelX = FP_FromDouble(0.001);
                }
            }

            mHealth = FP_FromInteger(0);
            EventBroadcast(kEventMudokonComfort, this);
            return true;
        }

        case ReliveTypes::eDrill:
        case ReliveTypes::eGroundExplosion:
        case ReliveTypes::eAirExplosion:
        {
            if (!GetAnimation().GetRender())
            {
                return true;
            }

            relive_new Gibs(GibType::Slig_1, mXPos, mYPos, mVelX, mVelY, GetSpriteScale(), 0);
            mHealth = FP_FromInteger(0);
            SfxPlayMono(relive::SoundEffects::FallingItemHit, 90);
            GetAnimation().SetAnimate(false);
            GetAnimation().SetRender(false);
            SetUpdateDelay(40);
            SetBrain(&Slig::Brain_1_ReturnControlToAbeAndDie);
            SetCurrentMotion(eSligMotions::Motion_0_StandIdle);
            VUpdateAnimData();
            EventBroadcast(kEventMudokonComfort, this);
            return true;
        }

        case ReliveTypes::eElectricWall:
            Slig_GameSpeak_SFX(SligSpeak::eHelp_10, 0, field_11E_pitch_min, this);
            return true;

        case ReliveTypes::eAbe:
        {
            if (mHealth <= FP_FromInteger(0))
            {
                return true;
            }

            if (GetCurrentMotion() == eSligMotions::Motion_34_Knockback || GetCurrentMotion() == eSligMotions::Motion_44_Smash || GetCurrentMotion() == eSligMotions::Motion_35_KnockbackToStand)
            {
                return true;
            }

            if (!VIsFacingMe(sActiveHero) || IsInInvisibleZone(sActiveHero) || sActiveHero->GetInvisible() || IsAbeEnteringDoor(sControlledCharacter))
            {
                GoAlertedOrSayWhat();
            }
            else
            {
                RespondToEnemyOrPatrol();
            }

            mbMotionChanged = true;

            if (GetAnimation().GetFlipX())
            {
                mXPos = ScaleToGridSize(GetSpriteScale()) + mXPos;
            }
            else
            {
                mXPos = mXPos - ScaleToGridSize(GetSpriteScale());
            }
            ToKnockBack();
            mVelX = FP_FromInteger(0);
            return true;
        }

        case ReliveTypes::eAbilityRing:
            return true;

        case ReliveTypes::eSlog:
        {
            if (mHealth <= FP_FromInteger(0) && (GetCurrentMotion() == eSligMotions::Motion_34_Knockback || GetCurrentMotion() == eSligMotions::Motion_44_Smash))
            {
                return true;
            }

            mbGotShot = true;
            mHealth = FP_FromInteger(0);

            SetBrain(&Slig::Brain_0_Death);

            Environment_SFX(EnvironmentSfx::eKnockback_13, 0, 0x7FFF, this);

            EventBroadcast(kEventMudokonComfort, this);

            if (VIsFacingMe(static_cast<BaseAliveGameObject*>(pFrom)))
            {
                if (GetAnimation().GetFlipX())
                {
                    mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
                }
                else
                {
                    mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
                }

                mbMotionChanged = true;
                field_12C_timer = MakeTimer(10);
                SetCurrentMotion(eSligMotions::Motion_34_Knockback);
                SetNextMotion(eSligMotions::Motion_34_Knockback);
                field_136_shot_motion = eSligMotions::Motion_34_Knockback;
            }
            else
            {
                SetNextMotion(eSligMotions::Motion_44_Smash);
                field_136_shot_motion = eSligMotions::Motion_44_Smash;
            }
            return true;
        }

        case ReliveTypes::eElectrocute:
            if (mHealth <= FP_FromInteger(0))
            {
                return true;
            }
            SetDead(true);
            mHealth = FP_FromInteger(0);
            EventBroadcast(kEventMudokonComfort, this);
            return true;

        default:
            if (mHealth <= FP_FromInteger(0))
            {
                return true;
            }
            SfxPlayMono(relive::SoundEffects::KillEffect, 127);
            SfxPlayMono(relive::SoundEffects::FallingItemHit, 90);
            break;
    }

    if (mHealth <= FP_FromInteger(0))
    {
        return true;
    }

    mbGotShot = true;
    mHealth = FP_FromInteger(0);
    SetNextMotion(eSligMotions::Motion_44_Smash);
    field_136_shot_motion = eSligMotions::Motion_44_Smash;
    EventBroadcast(kEventMudokonComfort, this);
    return true;
}

bool Slig::VIsFacingMe(BaseAnimatedWithPhysicsGameObject* pWho)
{
    if (GetCurrentMotion() != eSligMotions::Motion_5_TurnAroundStanding || GetAnimation().GetCurrentFrame() < 6)
    {
        if (pWho->mXPos <= mXPos && GetAnimation().GetFlipX())
        {
            return true;
        }

        if (pWho->mXPos >= mXPos && !GetAnimation().GetFlipX())
        {
            return true;
        }
    }
    else
    {
        if (pWho->mXPos <= mXPos && !GetAnimation().GetFlipX())
        {
            return true;
        }

        if (pWho->mXPos >= mXPos && GetAnimation().GetFlipX())
        {
            return true;
        }
    }
    return false;
}

bool Slig::VOnSameYLevel(BaseAnimatedWithPhysicsGameObject* pOther)
{
    const PSX_RECT bOurRect = VGetBoundingRect();
    const PSX_RECT bTheirRect = pOther->VGetBoundingRect();

    return ((bTheirRect.h + bTheirRect.y) / 2) <= bOurRect.h && bTheirRect.h >= bOurRect.y && GetScale() == pOther->GetScale();
}

s16 Slig::FindLiftPoint()
{
    const FP k2Scaled = ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(2);

    PlatformBase* pLift = nullptr;
    if (GetAnimation().GetFlipX())
    {
        pLift = static_cast<PlatformBase*>(FindObjectOfType(ReliveTypes::eLiftPoint, mXPos - k2Scaled, mYPos + FP_FromInteger(5)));
    }
    else
    {
        pLift = static_cast<PlatformBase*>(FindObjectOfType(ReliveTypes::eLiftPoint, k2Scaled + mXPos, mYPos + FP_FromInteger(5)));
    }

    if (pLift)
    {
        const PSX_RECT rect = pLift->VGetBoundingRect();

        const FP liftMidXPos = FP_FromInteger((rect.x + rect.w) / 2);
        return FP_Abs(mXPos - liftMidXPos) < k2Scaled;
    }

    return false;
}
