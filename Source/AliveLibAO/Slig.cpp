#include "stdafx_ao.h"
#include "AmbientSound.hpp"
#include "Function.hpp"
#include "Slig.hpp"
#include "Lever.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "../relive_lib/Shadow.hpp"
#include "../relive_lib/Collisions.hpp"
#include "Math.hpp"
#include "Abe.hpp"
#include "Elum.hpp"
#include "../relive_lib/Events.hpp"
#include "MusicController.hpp"
#include "Game.hpp"
#include "DDCheat.hpp"
#include "Input.hpp"
#include "../relive_lib/Particle.hpp"
#include "Blood.hpp"
#include "Gibs.hpp"
#include "Sfx.hpp"
#include "CameraSwapper.hpp"
#include "PlatformBase.hpp"
#include "LiftPoint.hpp"
#include "Dove.hpp"
#include "Bullet.hpp"
#include "../relive_lib/Particle.hpp"
#include "Midi.hpp"
#include "GameEnderController.hpp"
#include "SnoozeParticle.hpp"
#include "GameSpeak.hpp"
#include "ScreenShake.hpp"
#include "SwitchStates.hpp"
#include <algorithm>
#include "Psx_common.hpp"
#include "AnimationCallBacks.hpp"
#include "Grid.hpp"
#include "../AliveLibAE/Sound/Midi.hpp"
#include "BaseGameAutoPlayer.hpp"

// TODO: fix
#undef max
#undef min

namespace AO {

s32 unused_9F11BC = 0;
s32 unused_9F11C0 = 0;

const TintEntry kSligTints_4CFB10[3] = {
    {EReliveLevelIds::eStockYards, 127u, 127u, 127u},
    {EReliveLevelIds::eStockYardsReturn, 127u, 127u, 127u},
    {EReliveLevelIds::eNone, 102u, 127u, 118u}};

using TSligStateFunction = decltype(&Slig::Motion_0_StandIdle);

const TSligStateFunction sSligMotionTable[] = {
    &Slig::Motion_0_StandIdle,
    &Slig::Motion_1_StandToWalk,
    &Slig::Motion_2_Walking,
    &Slig::Motion_3_StandToRun,
    &Slig::Motion_4_Running,
    &Slig::Motion_5_TurnAroundStanding,
    &Slig::Motion_6_Shoot,
    &Slig::Motion_7_Falling,
    &Slig::Motion_8_Unknown,
    &Slig::Motion_9_SlidingToStand,
    &Slig::Motion_10_SlidingTurn,
    &Slig::Motion_11_SlidingTurnToWalk,
    &Slig::Motion_12_SlidingTurnToRun,
    &Slig::Motion_13_ReloadGun,
    &Slig::Motion_14_ShootToStand,
    &Slig::Motion_15_SteppingToStand,
    &Slig::Motion_16_StandingToStep,
    &Slig::Motion_17_DepossessingAbort,
    &Slig::Motion_18_GameSpeak,
    &Slig::Motion_19_WalkToStand,
    &Slig::Motion_20_Recoil,
    &Slig::Motion_21_SpeakHereBoy,
    &Slig::Motion_22_SpeakHi,
    &Slig::Motion_23_SpeakFreeze,
    &Slig::Motion_24_SpeakGetHim,
    &Slig::Motion_25_SpeakLaugh,
    &Slig::Motion_26_SpeakBullshit1,
    &Slig::Motion_27_SpeakLookOut,
    &Slig::Motion_28_SpeakBullshit2,
    &Slig::Motion_29_SpeakPanic,
    &Slig::Motion_30_SpeakWhat,
    &Slig::Motion_31_SpeakAIFreeze,
    &Slig::Motion_32_Blurgh,
    &Slig::Motion_33_Sleeping,
    &Slig::Motion_34_SleepingToStand,
    &Slig::Motion_35_Knockback,
    &Slig::Motion_36_KnockbackToStand,
    &Slig::Motion_37_Depossessing,
    &Slig::Motion_38_Possess,
    &Slig::Motion_39_OutToFall,
    &Slig::Motion_40_FallingInitiate,
    &Slig::Motion_41_LandingSoft,
    &Slig::Motion_42_LandingFatal,
    &Slig::Motion_43_ShootZ,
    &Slig::Motion_44_ShootZtoStand,
    &Slig::Motion_45_Smash,
    &Slig::Motion_46_ToIdle,
    &Slig::Motion_47_LiftUp,
    &Slig::Motion_48_LiftDown,
    &Slig::Motion_49_LiftGrip,
    &Slig::Motion_50_LiftUngrip,
    &Slig::Motion_51_LiftGripping,
    &Slig::Motion_52_Beat,
};

const AnimId sSligMotionAnimIds[] = {
    AnimId::Slig_Idle,
    AnimId::Slig_StandToWalk,
    AnimId::Slig_Walking,
    AnimId::Slig_StandToRun,
    AnimId::Slig_Running,
    AnimId::Slig_TurnAroundStanding,
    AnimId::Slig_Shoot,
    AnimId::Slig_Idle,
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
    AnimId::Slig_LiftUp,
    AnimId::Slig_LiftDown,
    AnimId::Slig_LiftGrip,
    AnimId::Slig_LiftUngrip,
    AnimId::Slig_LiftGripping,
    AnimId::Slig_Beat,
    AnimId::None};

const relive::SfxDefinition sSligSounds_4CFB30[] = {
    {0, 1, 58, 40, -256, -256},
    {0, 1, 58, 50, 0, 0},
    {0, 1, 59, 60, 0, 0},
    {0, 1, 59, 60, 127, 127},
    {0, 1, 60, 60, 0, 0},
    {0, 1, 61, 60, 0, 0}};

const relive::SfxDefinition sSligSounds2[] = {
    {0, 62, 60, 127, 0, 0},
    {0, 62, 62, 127, 0, 0},
    {0, 62, 61, 127, 0, 0},
    {0, 62, 63, 127, 0, 0},
    {0, 63, 60, 127, 0, 0},
    {0, 63, 37, 127, 0, 0},
    {0, 63, 61, 127, 0, 0},
    {0, 63, 38, 127, 0, 0},
    {0, 64, 61, 127, 0, 0},
    {0, 64, 62, 127, 0, 0},
    {0, 64, 63, 127, 0, 0},
    {0, 64, 39, 127, 0, 0},
    {0, 64, 36, 127, 0, 0}};

void Slig::Slig_SoundEffect(SligSfx sfxIdx)
{
    PSX_RECT worldRect;
    s32 volRight = 0;
    s32 volLeft = 0;

    auto dir = gMap.GetDirection(
        mCurrentLevel,
        mCurrentPath,
        mXPos,
        mYPos);

    auto sfxIdxInt = static_cast<s32>(sfxIdx);
    if (GetSpriteScale() == FP_FromInteger(1))
    {
        volRight = sSligSounds_4CFB30[sfxIdxInt].field_C_default_volume;
    }
    else
    {
        volRight = sSligSounds_4CFB30[sfxIdxInt].field_C_default_volume / 2;
    }
    gMap.Get_Camera_World_Rect(dir, &worldRect);
    switch (dir)
    {
        case CameraPos::eCamCurrent_0:
        {
            volLeft = volRight;
            break;
        }
        case CameraPos::eCamTop_1:
        case CameraPos::eCamBottom_2:
        {
            volLeft = sSligSounds_4CFB30[sfxIdxInt].field_C_default_volume * 2 / 3;
            volRight = volLeft;
            break;
        }
        case CameraPos::eCamLeft_3:
        {
            FP percentHowFar = (FP_FromInteger(worldRect.w) - mXPos) / FP_FromInteger(640);
            volLeft = volRight - FP_GetExponent(percentHowFar * FP_FromInteger(volRight - (volRight / 3)));
            volRight -= FP_GetExponent(percentHowFar * FP_FromInteger(volRight));
            break;
        }
        case CameraPos::eCamRight_4:
        {
            FP percentHowFar = (mXPos - FP_FromInteger(worldRect.x)) / FP_FromInteger(640);
            volLeft = volRight - FP_GetExponent(percentHowFar * FP_FromInteger(volRight));
            volRight -= FP_GetExponent(percentHowFar * FP_FromInteger(volRight - (volRight / 3)));
            break;
        }
        default:
            return;
    }
    if (GetSpriteScale() == FP_FromDouble(0.5))
    {
        volLeft = volLeft * 2 / 3;
        volRight = volRight * 2 / 3;
    }
    auto pitch = Math_RandomRange(
        sSligSounds_4CFB30[sfxIdxInt].field_E_pitch_min,
        sSligSounds_4CFB30[sfxIdxInt].field_E_pitch_min);
    SFX_SfxDefinition_Play_477330(sSligSounds_4CFB30[sfxIdxInt], static_cast<s16>(volLeft), static_cast<s16>(volRight), pitch, pitch);
}

void Slig::LoadAnimations()
{
    for (auto& animId : sSligMotionAnimIds)
    {
        if (animId != AnimId::None)
        {
            mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(animId));
        }
    }
}

Slig::Slig(relive::Path_Slig* pTlv, const Guid& tlvId)
    : BaseAliveGameObject()
{
    field_15C_last_event_index = -1;

    field_210_resources = {};

    LoadAnimations();
    Animation_Init(GetAnimRes(AnimId::Slig_Idle));

    mBaseAliveGameObjectFlags.Clear(Flags_10A::e10A_Bit2_bPossesed);
    mBaseAliveGameObjectFlags.Set(Flags_10A::e10A_Bit1_Can_Be_Possessed);
    mBaseAliveGameObjectFlags.Set(Flags_10A::e10A_Bit4_SetOffExplosives);

    SetType(ReliveTypes::eSlig);

    field_114_timer = 0;
    mBrainSubState = 0;
    mNextMotion = 0;
    field_EC_bBeesCanChase = 3;
    mExplodeTimer = 0;
    field_154_death_by_being_shot_timer = 0;
    mLiftPoint = nullptr;
    mCurrentMotion = eSligMotions::Motion_7_Falling;
    mReturnToPreviousMotion = 0;
    field_12C_falling_velx_scale_factor = FP_FromInteger(0);

    mGameSpeakPitchMin = 45 * ((Math_NextRandom() % 5) - 2);

    BaseAliveGameObjectPathTLV = pTlv;
    field_174_tlv = pTlv;
    mXPos = FP_FromInteger(pTlv->mTopLeftX);
    mYPos = FP_FromInteger(pTlv->mTopLeftY);
    field_134_tlvInfo = tlvId;
    mPreventDepossession &= ~7u;
    field_126_input = 0;
    field_130_game_ender_pause_time = 100;
    unused_9F11BC = 0;
    unused_9F11C0 = 0;
    mShootCount = 0;
    field_20C_force_alive_state = 0;
    field_13A_shot_motion = -1;
    field_138_res_idx = 0;

    GetAnimation().SetFnPtrArray(kSlig_Anim_Frame_Fns_4CEBF0);

    if (pTlv->mData.mScale == relive::reliveScale::eFull)
    {
        SetSpriteScale(FP_FromInteger(1));
        GetAnimation().SetRenderLayer(Layer::eLayer_SligGreeterFartsBats_33);
        SetScale(Scale::Fg);
    }
    else
    {
        SetSpriteScale(FP_FromDouble(0.5));
        GetAnimation().SetRenderLayer(Layer::eLayer_SligGreeterFartsBat_Half_14);
        SetScale(Scale::Bg);
    }

    SetBaseAnimPaletteTint(
        &kSligTints_4CFB10[0],
        gMap.mCurrentLevel,
        PalId::StockYardsSlig);

    FP hitX = {};
    FP hitY = {};
    if (sCollisions->Raycast(
            mXPos,
            mYPos,
            mXPos,
            mYPos + FP_FromInteger(24),
            &BaseAliveGameObjectCollisionLine,
            &hitX,
            &hitY,
            GetSpriteScale() != FP_FromDouble(0.5) ? kFgFloor : kBgFloor)
        == 1)
    {
        mCurrentMotion = 0;
        mYPos = hitY;
    }

    MapFollowMe(TRUE);

    Init();

    VStackOnObjectsOfType(ReliveTypes::eSlig);

    mBaseAliveGameObjectFlags.Set(Flags_10A::e10A_Bit6);

    CreateShadow();
}

Slig::~Slig()
{
    if (sControlledCharacter == this)
    {
        sControlledCharacter = sActiveHero;
        MusicController::static_PlayMusic(MusicController::MusicTypes::eType0, this, 0, 0);

        if (gMap.mNextLevel != EReliveLevelIds::eMenu && gMap.mNextLevel != EReliveLevelIds::eNone)
            gMap.SetActiveCam(
                mAbeLevel,
                mAbePath,
                mAbeCamera,
                CameraSwapEffects::eInstantChange_0,
                0,
                0);
    }

    auto pTlv = gMap.TLV_Get_At(
        field_174_tlv->mTopLeftX,
        field_174_tlv->mTopLeftY,
        field_174_tlv->mTopLeftX,
        field_174_tlv->mTopLeftY,
        field_174_tlv->mTlvType);

    if (mHealth <= FP_FromInteger(0))
    {
        if (pTlv)
        {
            pTlv->mTlvFlags.Clear(relive::TlvFlags::eBit1_Created);
            pTlv->mTlvFlags.Set(relive::TlvFlags::eBit2_Destroyed);
        }
    }
    else if (pTlv)
    {
        pTlv->mTlvFlags.Clear(relive::TlvFlags::eBit1_Created);
        pTlv->mTlvFlags.Clear(relive::TlvFlags::eBit2_Destroyed);
    }

    /*
    for (u8**& ppRes : field_210_resources.res)
    {
        if (ppRes && mAnim.field_20_ppBlock != ppRes)
        {
            ResourceManager::FreeResource_455550(ppRes);
            ppRes = nullptr;
        }
    }*/

    
    MusicController::ClearObject(this);
}

void Slig::VScreenChanged()
{
    if (gMap.LevelChanged()
        || (gMap.PathChanged() && this != sControlledCharacter))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void Slig::Init()
{
    mSpottedPossessedSlig = 0;
    field_114_timer = sGnFrame + field_174_tlv->mData.mPauseTime;

    switch (field_174_tlv->mData.mStartState)
    {
        case relive::Path_Slig_Data::StartState::Patrol:
        {
            SetBrain(&Slig::Brain_Inactive);
            break;
        }
        case relive::Path_Slig_Data::StartState::Sleeping:
        {
            if (field_174_tlv->mTlvSpecificMeaning && field_174_tlv->mData.mStayAwake == relive::reliveChoice::eYes)
            {
                SetBrain(&Slig::Brain_Inactive);
            }
            else
            {
                SetBrain(&Slig::Brain_Sleeping);
                mCurrentMotion = eAbeMotions::Motion_33_RunJumpMid;
                VUpdateAnimData();
            }
            break;
        }
        case relive::Path_Slig_Data::StartState::Chase:
        {
            SetBrain(&Slig::Brain_StartChasing);
            field_114_timer = sGnFrame + field_174_tlv->mData.mTimeToWaitBeforeChase;
            break;
        }
        case relive::Path_Slig_Data::StartState::ChaseAndDisappear:
        {
            field_114_timer = sGnFrame + field_174_tlv->mData.mPauseTime;
            SetBrain(&Slig::Brain_ChaseAndDisappear);
            field_130_game_ender_pause_time = field_174_tlv->mData.mPauseTime;
            break;
        }
        case relive::Path_Slig_Data::StartState::eFallingToChase:
        {
            SetBrain(&Slig::Brain_Paused);
            break;
        }
        default:
        {
            SetBrain(&Slig::Brain_Unknown);
            break;
        }
    }

    if (field_174_tlv->mData.mFacing == relive::reliveXDirection::eLeft)
    {
        GetAnimation().mFlags.Set(AnimFlags::eFlipX);
    }
    field_1F4_points_count = 0;

    field_1CC_points[field_1F4_points_count].x = FP_GetExponent(mXPos);
    field_1CC_points[field_1F4_points_count].y = FP_GetExponent(mYPos);
    field_1F4_points_count++;

    field_13C_zone_rect = {};
    bool zoneRectSet = false;

    for (s16 yCam = -2; yCam < 3; yCam++)
    {
        for (s16 xCam = -2; xCam < 3; xCam++)
        {
            auto pTlvIter = gMap.Get_First_TLV_For_Offsetted_Camera(xCam, yCam);
            while (pTlvIter)
            {
                bool addPoint = false;
                if (pTlvIter->mTlvType == ReliveTypes::eSligBoundLeft)
                {
                    if (static_cast<relive::Path_SligBoundLeft*>(pTlvIter)->mSligBoundId == field_174_tlv->mData.mSligBoundId)
                    {
                        field_13C_zone_rect.x = pTlvIter->mTopLeftX;
                        addPoint = true;
                        zoneRectSet = true;
                    }
                }
                else if (pTlvIter->mTlvType == ReliveTypes::eSligBoundRight)
                {
                    if (static_cast<relive::Path_SligBoundRight*>(pTlvIter)->mSligBoundId == field_174_tlv->mData.mSligBoundId)
                    {
                        field_13C_zone_rect.w = pTlvIter->mTopLeftX;
                        addPoint = true;
                        zoneRectSet = true;
                    }
                }
                else if (pTlvIter->mTlvType == ReliveTypes::eSligPersist)
                {
                    if (static_cast<relive::Path_SligPersist*>(pTlvIter)->mSligBoundId == field_174_tlv->mData.mSligBoundId)
                    {
                        addPoint = true;
                    }
                }

                if (addPoint)
                {
                    if (field_1F4_points_count < ALIVE_COUNTOF(field_1CC_points))
                    {
                        field_1CC_points[field_1F4_points_count].x = pTlvIter->mTopLeftX;
                        field_1CC_points[field_1F4_points_count].y = pTlvIter->mTopLeftY;
                        field_1F4_points_count++;
                    }
                }

                pTlvIter = Path_TLV::Next_446460(pTlvIter);
            }
        }
    }

    // HACK FIX: Some slig spawners don't set a slig id that matches any left/right bound.
    // In the OG this leaves the left/right zone bounds set as random values, here we fix
    // to some of these seen random values that gives the slig some what predictable patrol zones.
    if (!zoneRectSet)
    {
        LOG_INFO("Hack fix the slig walking zone (if you see this in a custom map please give your spwaned slig a left/right bound that matches the spawn id!)");
        field_13C_zone_rect.x = 12809;
        field_13C_zone_rect.w = 6405;
    }
}

const u32 sSligVelXTable_4BCA30[] = {262144, 262144, 0, 4294705152, 4294705152, 4294705152, 0, 262144};
const u32 sSligVelYTable_4BCA50[] = {0, 4294705152, 4294705152, 4294705152, 0, 262144, 262144, 262144};


void Slig::VUpdate()
{
    if (!Input_IsChanting())
    {
        mPreventDepossession &= ~4u;
    }

    if (sControlledCharacter == this && mHealth > FP_FromInteger(0))
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::ePossessed_6, this, 1, 0);
    }

    if (sDDCheat_FlyingEnabled_50771C && sControlledCharacter == this)
    {
        BaseAliveGameObjectCollisionLine = nullptr;
        if (Input().IsAnyPressed(0xF000))
        {
            const s32 dir = Input().Dir();
            mVelX = FP_FromRaw(sSligVelXTable_4BCA30[dir]);
            mVelY = FP_FromRaw(sSligVelYTable_4BCA50[dir]);

            if (Input().IsAnyPressed(0x20))
            {
                const FP velX = FP_FromRaw(sSligVelXTable_4BCA30[dir]);
                const FP velY = FP_FromRaw(sSligVelYTable_4BCA50[dir]);
                mVelX += velX;
                mVelX += velX;
                mVelY += velY;
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
    else
    {
        const auto old_motion = mCurrentMotion;

        const auto oldBrain = mBrainState;

        mBrainSubState = (this->*mBrainState)();

        if (mBrainState != oldBrain)
        {
            //LOG_INFO("Brain changed from " << GetOriginalFn(oldBrain, sSligBrainTable).fnName << " to " << GetOriginalFn(mBrainState, sSligBrainTable).fnName);
        }

        if (field_106_shot)
        {
            Vshot();
        }

        if (showDebugCreatureInfo_5076E0)
        {
            DDCheat::DebugStr(
                "Slig %d %d %d %d\n",
                mBrainSubState,
                field_114_timer,
                mCurrentMotion,
                mNextMotion);
        }

        const FP new_x = mXPos;
        const FP new_y = mYPos;

        (this->*sSligMotionTable[mCurrentMotion])();

        if (new_x != mXPos || new_y != mYPos)
        {
            BaseAliveGameObjectPathTLV = gMap.TLV_Get_At(
                nullptr,
                mXPos,
                mYPos,
                mXPos,
                mYPos);

            VOnTlvCollision(BaseAliveGameObjectPathTLV);
        }

        if (old_motion != mCurrentMotion || field_108_bMotionChanged)
        {
            field_108_bMotionChanged = FALSE;
            VUpdateAnimData();

            if (VIs8_465630(old_motion))
            {
                GetAnimation().SetFrame(mBaseAliveGameObjectLastAnimFrame);
            }
        }
        else if (mReturnToPreviousMotion)
        {
            mCurrentMotion = mPreviousMotion;

            VUpdateAnimData();

            GetAnimation().SetFrame(mBaseAliveGameObjectLastAnimFrame);
            mReturnToPreviousMotion = 0;
        }
    }
}

void Slig::VRender(PrimHeader** ppOt)
{
    if (UpdateDelay() == 0)
    {
        BaseAnimatedWithPhysicsGameObject::VRender(ppOt);
    }
}

void Slig::VOnTrapDoorOpen()
{
    if (mLiftPoint)
    {
        mLiftPoint->VRemove(this);
        mLiftPoint->mBaseGameObjectRefCount--;
        mLiftPoint = nullptr;
        BaseAliveGameObjectLastLineYPos = mYPos;
        VSetMotion(eSligMotions::Motion_39_OutToFall);
    }
}

void Slig::VUnPosses()
{
    mNextMotion = eSligMotions::Motion_0_StandIdle;
    field_114_timer = sGnFrame + 180;
    MusicController::static_PlayMusic(MusicController::MusicTypes::eType0, this, 0, 0);
}

enum Brain_Possessed
{
    eStartPossession_0 = 0,
    ePossessionShaking_1 = 1,
    ePossessionInactive_2 = 2,
    ePossessionSpeak_3 = 3,
    eControlledByPlayer_4 = 4
};

void Slig::VPossessed()
{
    mBaseAliveGameObjectFlags.Set(Flags_10A::e10A_Bit2_bPossesed);
    mPreventDepossession |= 4u;
    if (mNextMotion != eSligMotions::Motion_36_KnockbackToStand && mNextMotion != eSligMotions::Motion_35_Knockback)
    {
        mNextMotion = eSligMotions::Motion_0_StandIdle;
    }

    SetBrain(&Slig::Brain_Possessed);
    mBrainSubState = Brain_Possessed::eStartPossession_0;

    mAbeLevel = gMap.mCurrentLevel;
    mAbePath = gMap.mCurrentPath;
    mAbeCamera = gMap.mCurrentCamera;

    MusicController::static_PlayMusic(MusicController::MusicTypes::ePossessed_6, this, 1, 0);
}

s16 Slig::VTakeDamage(BaseGameObject* pFrom)
{
    switch (pFrom->Type())
    {
        case ReliveTypes::eBullet:
        {
            auto pBullet = static_cast<Bullet*>(pFrom);
            if (GetAnimation().mFlags.Get(AnimFlags::eRender))
            {
                {
                    const FP yOff = FP_FromInteger(Math_NextRandom() % 16) - FP_FromInteger(8);
                    const FP xOff = (pBullet->mXDistance <= FP_FromInteger(0) ? FP_FromInteger(-1) : FP_FromInteger(1) * FP_FromInteger(Math_NextRandom() & 15) + FP_FromInteger(16));
                    const FP xPos = (GetSpriteScale() * pBullet->mXDistance <= FP_FromInteger(0) ? FP_FromInteger(-6) : FP_FromInteger(6));

                    relive_new Blood(
                        xPos + mXPos,
                        pBullet->mYPos,
                        xOff,
                        yOff,
                        GetSpriteScale(),
                        12);
                }

                {
                    const FP xPos = (GetSpriteScale() * pBullet->mXDistance <= FP_FromInteger(0) ? FP_FromInteger(-12) : FP_FromInteger(12));
                    const FP xOff = pBullet->mXDistance <= FP_FromInteger(0) ? FP_FromInteger(-6) : FP_FromInteger(6);

                    relive_new Blood(
                        xOff + mXPos,
                        pBullet->mYPos,
                        xPos,
                        FP_FromInteger(0),
                        GetSpriteScale(),
                        8);
                }
            }

            field_106_shot = TRUE;
            SetBrain(&Slig::Brain_Death_46C3A0);
            field_154_death_by_being_shot_timer = sGnFrame + 5;

            if (mCurrentMotion != eSligMotions::Motion_38_Possess && mCurrentMotion != eSligMotions::Motion_45_Smash && mCurrentMotion != eSligMotions::Motion_35_Knockback)
            {
                mExplodeTimer = sGnFrame + 20;
                mNextMotion = eSligMotions::Motion_38_Possess;
                field_13A_shot_motion = eSligMotions::Motion_38_Possess;
                Vshot();
                field_108_bMotionChanged = TRUE;
                if (pBullet->mXDistance >= FP_FromInteger(0))
                {
                    mVelX = FP_FromDouble(0.001);
                }
                else
                {
                    mVelX = FP_FromDouble(-0.001);
                }
            }

            if (mHealth <= FP_FromInteger(0))
            {
                return GetAnimation().mFlags.Get(AnimFlags::eRender) ? 1 : 0;
            }

            if (mCurrentMotion != eSligMotions::Motion_45_Smash && mCurrentMotion != eSligMotions::Motion_35_Knockback)
            {
                mNextMotion = eSligMotions::Motion_38_Possess;
                field_13A_shot_motion = eSligMotions::Motion_38_Possess;
            }
            mHealth = FP_FromInteger(0);
            EventBroadcast(kEventMudokonComfort, sActiveHero);
            return 1;
        }

        case ReliveTypes::eElectricWall:
            Slig_GameSpeak_SFX(SligSpeak::eHelp_10, 0, mGameSpeakPitchMin, this);
            return 1;

        case ReliveTypes::eGroundExplosion:
        case ReliveTypes::eMeatSaw:
        case ReliveTypes::eAirExplosion:
            if (GetAnimation().mFlags.Get(AnimFlags::eRender))
            {
                relive_new Gibs(
                    GibType::Slig_1,
                    mXPos,
                    mYPos,
                    mVelX,
                    mVelY,
                    GetSpriteScale());

                mHealth = FP_FromInteger(0);
                SfxPlayMono(relive::SoundEffects::FallingItemHit, 90);
                SetUpdateDelay(40);
                GetAnimation().mFlags.Clear(AnimFlags::eRender);
                GetAnimation().mFlags.Clear(AnimFlags::eAnimate);
                SetBrain(&Slig::Brain_ReturnControlToAbeAndDie);
                mCurrentMotion = eSligMotions::Motion_0_StandIdle;
                VUpdateAnimData();
                EventBroadcast(kEventMudokonComfort, sActiveHero);
            }
            return 1;

        case ReliveTypes::eAbilityRing:
            return 1;

        case ReliveTypes::eSlog:
            if (mHealth <= FP_FromInteger(0)
                && (mCurrentMotion == eSligMotions::Motion_35_Knockback || mCurrentMotion == eSligMotions::Motion_45_Smash))
            {
                return 1;
            }
            mHealth = FP_FromInteger(0);
            SetBrain(&Slig::Brain_Death_46C3A0);
            field_106_shot = 1;
            Environment_SFX_42A220(EnvironmentSfx::eKnockback_13, 0, 0x7FFF, this);
            if (VIsFacingMe(static_cast<BaseAnimatedWithPhysicsGameObject*>(pFrom)))
            {
                if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
                {
                    mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
                }
                else
                {
                    mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
                }
                field_108_bMotionChanged = TRUE;
                field_128_timer = sGnFrame + 10;
                mCurrentMotion = eSligMotions::Motion_35_Knockback;
                mNextMotion = eSligMotions::Motion_35_Knockback;
                field_13A_shot_motion = eSligMotions::Motion_35_Knockback;
            }
            else
            {
                mNextMotion = eSligMotions::Motion_45_Smash;
                field_13A_shot_motion = eSligMotions::Motion_45_Smash;
            }
            return 1;

        case ReliveTypes::eBeeSwarm:
            if (mHealth > FP_FromInteger(0))
            {
                mHealth -= FP_FromDouble(0.34);
                Slig_GameSpeak_SFX(SligSpeak::eHelp_10, 0, mGameSpeakPitchMin, this);
                if (mHealth <= FP_FromInteger(0))
                {
                    mHealth = FP_FromInteger(0);
                    field_106_shot = 1;
                    mNextMotion = eSligMotions::Motion_35_Knockback;
                    field_13A_shot_motion = eSligMotions::Motion_35_Knockback;
                }
            }
            return 1;

        case ReliveTypes::eElectrocute:
            if (mHealth > FP_FromInteger(0))
            {
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                mHealth = FP_FromInteger(0);
                EventBroadcast(kEventMudokonComfort, sActiveHero);
            }
            return 1;

        case ReliveTypes::eBat:
            break;

        default:
            if (mHealth > FP_FromInteger(0))
            {
                SfxPlayMono(relive::SoundEffects::KillEffect, 127);
                SfxPlayMono(relive::SoundEffects::FallingItemHit, 90);
            }
            break;
    }

    if (mHealth > FP_FromInteger(0))
    {
        mHealth = FP_FromInteger(0);
        mNextMotion = eSligMotions::Motion_45_Smash;
        field_13A_shot_motion = eSligMotions::Motion_45_Smash;
        field_106_shot = 1;
    }
    return 1;
}

enum Brain_DeathDropDeath
{
    eSayHelpOnce_0 = 0,
    eSayHelpAndDie_1 = 1,
    eSwitchCamToAbe_2 = 2
};

void Slig::VOnTlvCollision(relive::Path_TLV* pTlv)
{
    while (pTlv)
    {
        if (pTlv->mTlvType == ReliveTypes::eDeathDrop)
        {
            if (mHealth > FP_FromInteger(0))
            {
                mHealth = FP_FromInteger(0);
                mBrainSubState = Brain_DeathDropDeath::eSayHelpOnce_0;
                SetBrain(&Slig::Brain_DeathDropDeath);
                mVelY = FP_FromInteger(0);
                mVelX = FP_FromInteger(0);
                VSetMotion(eSligMotions::Motion_7_Falling);
                EventBroadcast(kEventMudokonComfort, sActiveHero);
            }
        }

        pTlv = gMap.TLV_Get_At(
            pTlv,
            mXPos,
            mYPos,
            mXPos,
            mYPos);
    }
}

s16 Slig::VOnSameYLevel(BaseAnimatedWithPhysicsGameObject* pOther)
{
    PSX_RECT ourRect = VGetBoundingRect();
    PSX_RECT objRect = pOther->VGetBoundingRect();
    return ((objRect.y + objRect.h) / 2) <= ourRect.h && objRect.h >= ourRect.y;
}

s16 Slig::VIsFacingMe(BaseAnimatedWithPhysicsGameObject* pWho)
{
    if (mCurrentMotion != eSligMotions::Motion_5_TurnAroundStanding
        || GetAnimation().GetCurrentFrame() < 6)
    {
        if (pWho->mXPos <= mXPos && GetAnimation().mFlags.Get(AnimFlags::eFlipX))
        {
            return 1;
        }

        if (pWho->mXPos >= mXPos && !GetAnimation().mFlags.Get(AnimFlags::eFlipX))
        {
            return 1;
        }
    }
    else
    {
        if (pWho->mXPos <= mXPos && !GetAnimation().mFlags.Get(AnimFlags::eFlipX))
        {
            return 1;
        }

        if (pWho->mXPos >= mXPos && GetAnimation().mFlags.Get(AnimFlags::eFlipX))
        {
            return 1;
        }
    }
    return 0;
}

void Slig::VUpdateAnimData()
{
    // note: OG was falling back to eSligMotions::Motion_0_StandIdle if 
    // ResBlockForMotion didnt return a resource
    GetAnimation().Set_Animation_Data(GetAnimRes(sSligMotionAnimIds[mCurrentMotion]));
}

void Slig::Vshot()
{
    if (field_13A_shot_motion != -1)
    {
        mCurrentMotion = field_13A_shot_motion;
    }

    mNextMotion = -1;
    field_13A_shot_motion = -1;
    field_106_shot = FALSE;
    field_114_timer = sGnFrame + 60;
    SetBrain(&Slig::Brain_Death_46C3A0);
}

u8** Slig::ResBlockForMotion(s16 motion)
{
    s16 new_idx = 0;
    if (motion < eSligMotions::Motion_33_Sleeping)
    {
        new_idx = 0;
    }
    else if (motion < eSligMotions::Motion_35_Knockback)
    {
        new_idx = 1;
    }
    else if (motion < eSligMotions::Motion_39_OutToFall)
    {
        new_idx = 2;
    }
    else if (motion < eSligMotions::Motion_42_LandingFatal)
    {
        new_idx = 3;
    }
    else if (motion < eSligMotions::Motion_43_ShootZ)
    {
        new_idx = 4;
    }
    else if (motion < eSligMotions::Motion_45_Smash)
    {
        new_idx = 5;
    }
    else if (motion < eSligMotions::State_46_PullLever_46A590)
    {
        new_idx = 6;
    }
    else if (motion < eSligMotions::Motion_47_LiftUp)
    {
        new_idx = 7;
    }
    else if (motion < eSligMotions::Motion_52_Beat)
    {
        new_idx = 8;
    }
    else
    {
        new_idx = motion >= 53 ? 0 : 9;
    }

    if (new_idx == field_138_res_idx)
    {
        return field_210_resources.res[field_138_res_idx];
    }

    field_138_res_idx = new_idx;
    return field_210_resources.res[field_138_res_idx];
}

void Slig::ToShoot()
{
    mNextMotion = eSligMotions::Motion_6_Shoot;
    SetBrain(&Slig::Brain_Shooting);
    mShootCount = 0;
    MusicController::static_PlayMusic(MusicController::MusicTypes::eSlogChase_5, this, 0, 0);
}

void Slig::ToZShoot()
{
    mNextMotion = eSligMotions::Motion_0_StandIdle;
    field_114_timer = sGnFrame + field_174_tlv->mData.mZShootDelay;
    SetBrain(&Slig::Brain_ZSpottedEnemy);

    // don't desync playbacks - this saved the result but never used it
    // TODO: remove when we have a new AO recording without this math call
    if (GetGameAutoPlayer().IsPlaying())
    {
        Math_RandomRange(3, 5);
    }

    MusicController::static_PlayMusic(MusicController::MusicTypes::eSlogChase_5, this, 0, 0);
}

void Slig::ShouldStillBeAlive()
{
    if (!field_20C_force_alive_state)
    {
        // Check not falling and not in the current screen
        if (mCurrentMotion != eSligMotions::Motion_7_Falling && mCurrentMotion != eSligMotions::Motion_39_OutToFall)
        {
            if (!gMap.Is_Point_In_Current_Camera(
                    mCurrentLevel,
                    mCurrentPath,
                    mXPos,
                    mYPos,
                    0))
            {
                if (field_1F4_points_count <= 0)
                {
                    // No patrol points, die
                    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                }
                else
                {
                    s32 i = 0;
                    while (!gMap.Is_Point_In_Current_Camera(
                        mCurrentLevel,
                        mCurrentPath,
                        FP_FromInteger(field_1CC_points[i].x),
                        FP_FromInteger(field_1CC_points[i].y),
                        0))
                    {
                        if (i >= field_1F4_points_count)
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
}

void Slig::ToAbeDead()
{
    mNextMotion = eSligMotions::Motion_25_SpeakLaugh;
    SetBrain(&Slig::Brain_EnemyDead);
    field_114_timer = sGnFrame + 45;
}

void Slig::WaitOrWalk()
{
    mNextMotion = eSligMotions::Motion_2_Walking;
    SetBrain(&Slig::Brain_Walking);

    // Right rect bound
    if (!IsFacingEffectiveLeft(this) && (ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(2)) + mXPos >= FP_FromInteger(field_13C_zone_rect.w))
    {
        PauseALittle();
        MusicController::static_PlayMusic(MusicController::MusicTypes::eChase_4, this, 0, 0);
        return;
    }

    // Left rect bound
    if (IsFacingEffectiveLeft(this) && (mXPos - (ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(2))) <= FP_FromInteger(field_13C_zone_rect.x))
    {
        PauseALittle();
        MusicController::static_PlayMusic(MusicController::MusicTypes::eChase_4, this, 0, 0);
        return;
    }

    Brain_Walking();
    MusicController::static_PlayMusic(MusicController::MusicTypes::eChase_4, this, 0, 0);
}

s32 Slig::IsFacingEffectiveLeft(Slig* pSlig)
{
    s32 bFlipX = pSlig->GetAnimation().mFlags.Get(AnimFlags::eFlipX);
    if (pSlig->mCurrentMotion == eSligMotions::Motion_5_TurnAroundStanding
        && pSlig->GetAnimation().GetCurrentFrame() > 4)
    {
        // Flip the result
        return bFlipX == 0;
    }
    return bFlipX;
}

void Slig::PauseALittle()
{
    if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
    {
        field_114_timer = sGnFrame + field_174_tlv->mData.mPauseLeftMin;
        if (field_174_tlv->mData.mPauseLeftMax > field_174_tlv->mData.mPauseLeftMin)
        {
            field_114_timer += Math_NextRandom() % (field_174_tlv->mData.mPauseLeftMax - field_174_tlv->mData.mPauseLeftMin);
        }
    }
    else
    {
        field_114_timer = sGnFrame + field_174_tlv->mData.mPauseRightMin;
        if (field_174_tlv->mData.mPauseRightMax > field_174_tlv->mData.mPauseRightMin)
        {
            field_114_timer += Math_NextRandom() % (field_174_tlv->mData.mPauseRightMax - field_174_tlv->mData.mPauseRightMin);
        }
    }

    mNextMotion = eSligMotions::Motion_0_StandIdle;
    SetBrain(&Slig::Brain_Idle);
}

void Slig::ToTurn()
{
    mNextMotion = eSligMotions::Motion_5_TurnAroundStanding;
    SetBrain(&Slig::Brain_Turning);
}

void Slig::ToPanicRunning()
{
    mNextMotion = eSligMotions::Motion_4_Running;
    SetBrain(&Slig::Brain_PanicRunning);
    Brain_PanicRunning();
    MusicController::static_PlayMusic(MusicController::MusicTypes::eSlogChase_5, this, 0, 0);
}

void Slig::ToPanic()
{
    mNextMotion = eSligMotions::Motion_29_SpeakPanic;
    SetBrain(&Slig::Brain_PanicYelling);
}

void Slig::ToChase()
{
    field_114_timer = sGnFrame + field_174_tlv->mData.mTimeToWaitBeforeChase;

    if (!VIsFacingMe(sControlledCharacter))
    {
        GetAnimation().mFlags.Toggle(AnimFlags::eFlipX);
    }
    mNextMotion = eSligMotions::Motion_0_StandIdle;
    SetBrain(&Slig::Brain_StartChasing);
    MusicController::static_PlayMusic(MusicController::MusicTypes::eSlogChase_5, this, 0, 0);
}

void Slig::ToKilledAbe()
{
    mNextMotion = eSligMotions::Motion_0_StandIdle;
    SetBrain(&Slig::Brain_KilledEnemy);
    field_114_timer = sGnFrame + 15;
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

s16 Slig::FindBeatTarget(s32 /*typeToFind*/, s32 gridBlocks)
{
    const FP kGridSize = ScaleToGridSize(GetSpriteScale());
    const FP k2Scaled = FP_FromInteger(2) * kGridSize;
    const FP kGridBlocksScaled = FP_FromInteger(gridBlocks) * kGridSize;

    const FP xAndW = GetAnimation().mFlags.Get(AnimFlags::eFlipX) ? mXPos - kGridBlocksScaled : mXPos + kGridBlocksScaled;

    PSX_RECT hitRect = MakeMinMaxRect(
        xAndW,
        mYPos,
        xAndW,
        mYPos - k2Scaled,
        true);

    for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
    {
        auto pTargetObj = gBaseAliveGameObjects->ItemAt(i);
        if (!pTargetObj)
        {
            break;
        }
        if (pTargetObj != this && pTargetObj->Type() == ReliveTypes::eMudokon)
        {
            const PSX_RECT bRect = pTargetObj->VGetBoundingRect();
            if (hitRect.w <= bRect.w && hitRect.x >= bRect.x && hitRect.y >= bRect.y && hitRect.h <= bRect.h && !Slig::IsInInvisibleZone(pTargetObj))
            {
                return 1;
            }
        }
    }
    return 0;
}

s16 Slig::HandleEnemyStopper(s32 gridBlocks)
{
    s32 directedGirdBlocks = gridBlocks;
    auto bFacingLeft = IsFacingEffectiveLeft(this);
    if (bFacingLeft)
    {
        directedGirdBlocks = -gridBlocks;
    }

    const auto dirScaled = ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(directedGirdBlocks) + mXPos;
    auto pStopper = static_cast<relive::Path_EnemyStopper*>(gMap.TLV_Get_At(
        FP_GetExponent(mXPos),
        FP_GetExponent(mYPos),
        FP_GetExponent(dirScaled),
        FP_GetExponent(mYPos),
        ReliveTypes::eEnemyStopper));

    if (!pStopper)
    {
        return 0;
    }

    //TODO different from Exoddus (inverted) - watch out!
    if (SwitchStates_Get(pStopper->mSwitchId))
    {
        return 0;
    }

    if (pStopper->mStopDirection == relive::Path_EnemyStopper::StopDirection::Both)
    {
        return 1;
    }

    if (bFacingLeft && pStopper->mStopDirection == relive::Path_EnemyStopper::StopDirection::Left)
    {
        return 1;
    }

    if (!bFacingLeft && pStopper->mStopDirection == relive::Path_EnemyStopper::StopDirection::Right)
    {
        return 1;
    }

    return 0;
}

void Slig::RespondToEnemyOrPatrol()
{
    if (field_174_tlv->mData.mShootOnSightDelay)
    {
        if (sControlledCharacter->Type() != ReliveTypes::eSlig
            || field_174_tlv->mData.mShootPossessedSligs != relive::reliveChoice::eNo)
        {
            SetBrain(&Slig::Brain_SpottedEnemy);
            mNextMotion = eSligMotions::Motion_31_SpeakAIFreeze;
            field_114_timer = field_174_tlv->mData.mShootOnSightDelay + sGnFrame;
        }
        else
        {
            mSpottedPossessedSlig = 1;
            TurnOrWalk(0);
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

void Slig::TurnOrWalk(s32 a2)
{
    if (a2 == 1)
    {
        if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
        {
            if (mXPos > FP_FromInteger(field_13C_zone_rect.x) + (ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(4)))
            {
                ToTurn();
                return;
            }
        }
        else
        {
            if (mXPos < FP_FromInteger(field_13C_zone_rect.w) - (ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(4)))
            {
                ToTurn();
                return;
            }
        }
    }

    if (!GetAnimation().mFlags.Get(AnimFlags::eFlipX))
    {
        if (mXPos > FP_FromInteger(field_13C_zone_rect.w) - (ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(4)))
        {
            ToTurn();
            return;
        }
    }
    else
    {
        if (mXPos < FP_FromInteger(field_13C_zone_rect.x) + (ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(4)))
        {
            ToTurn();
            return;
        }
    }

    WaitOrWalk();
}

void Slig::ToPanicTurn()
{
    mNextMotion = eSligMotions::Motion_5_TurnAroundStanding;
    SetBrain(&Slig::Brain_PanicTurning);
}

s16 Slig::GetNextMotionIncGameSpeak(u16 input)
{
    if (sControlledCharacter != this || mHealth <= FP_FromInteger(0))
    {
        switch (mNextMotion)
        {
            case eSligMotions::Motion_21_SpeakHereBoy:
                mNextMotion = -1;
                Slig_GameSpeak_SFX(SligSpeak::eHereBoy_1, 0, mGameSpeakPitchMin, this);
                return eSligMotions::Motion_21_SpeakHereBoy;

            case eSligMotions::Motion_22_SpeakHi:
                mNextMotion = -1;
                Slig_GameSpeak_SFX(SligSpeak::eHi_0, 0, mGameSpeakPitchMin, this);
                return eSligMotions::Motion_22_SpeakHi;

            case eSligMotions::Motion_23_SpeakFreeze:
                mNextMotion = -1;
                Slig_GameSpeak_SFX(SligSpeak::eStay_3, 0, mGameSpeakPitchMin, this);
                return eSligMotions::Motion_23_SpeakFreeze;

            case eSligMotions::Motion_24_SpeakGetHim:
                mNextMotion = -1;
                Slig_GameSpeak_SFX(SligSpeak::eGetHim_2, 0, mGameSpeakPitchMin, this);
                return eSligMotions::Motion_24_SpeakGetHim;

            case eSligMotions::Motion_25_SpeakLaugh:
                mNextMotion = -1;
                Slig_GameSpeak_SFX(SligSpeak::eLaugh_7, 0, mGameSpeakPitchMin, this);
                return eSligMotions::Motion_25_SpeakLaugh;

            case eSligMotions::Motion_26_SpeakBullshit1:
                mNextMotion = -1;
                Slig_GameSpeak_SFX(SligSpeak::eBullshit_4, 0, mGameSpeakPitchMin, this);
                return eSligMotions::Motion_26_SpeakBullshit1;

            case eSligMotions::Motion_27_SpeakLookOut:
                mNextMotion = -1;
                Slig_GameSpeak_SFX(SligSpeak::eLookOut_5, 0, mGameSpeakPitchMin, this);
                return eSligMotions::Motion_27_SpeakLookOut;

            case eSligMotions::Motion_28_SpeakBullshit2:
                mNextMotion = -1;
                Slig_GameSpeak_SFX(SligSpeak::eBullshit2_6, 0, mGameSpeakPitchMin, this);
                return eSligMotions::Motion_28_SpeakBullshit2;

            case eSligMotions::Motion_29_SpeakPanic:
                mNextMotion = -1;
                Slig_GameSpeak_SFX(SligSpeak::eHelp_10, 0, mGameSpeakPitchMin, this);
                return eSligMotions::Motion_29_SpeakPanic;

            case eSligMotions::Motion_30_SpeakWhat:
                mNextMotion = -1;
                Slig_GameSpeak_SFX(SligSpeak::eWhat_9, 0, mGameSpeakPitchMin, this);
                return eSligMotions::Motion_30_SpeakWhat;

            case eSligMotions::Motion_31_SpeakAIFreeze:
                mNextMotion = -1;
                Slig_GameSpeak_SFX(SligSpeak::eFreeze_8, 0, mGameSpeakPitchMin, this);
                return eSligMotions::Motion_31_SpeakAIFreeze;

            case eSligMotions::Motion_32_Blurgh:
                mNextMotion = -1;
                Slig_GameSpeak_SFX(SligSpeak::eBlurgh_11, 0, mGameSpeakPitchMin, this);
                return eSligMotions::Motion_32_Blurgh;

            default:
                return -1;
        }
    }

    if (Input_IsChanting())
    {
        if (mPreventDepossession & 4)
        {
            return -1;
        }

        field_128_timer = sGnFrame + 30;
        SfxPlayMono(relive::SoundEffects::PossessEffect, 0);
        return eSligMotions::Motion_37_Depossessing;
    }

    if (Input().IsAnyPressed(sInputKey_LeftGameSpeakEnabler))
    {
        if (input & sInputKey_GameSpeak2)
        {
            gEventSystem->VPushEvent(GameSpeakEvents::Slig_HereBoy_24);
            Slig_GameSpeak_SFX(SligSpeak::eHereBoy_1, 0, mGameSpeakPitchMin, this);
            return eSligMotions::Motion_21_SpeakHereBoy;
        }
        if (input & sInputKey_GameSpeak1)
        {
            gEventSystem->VPushEvent(GameSpeakEvents::Slig_Hi_23);
            Slig_GameSpeak_SFX(SligSpeak::eHi_0, 0, mGameSpeakPitchMin, this);
            return eSligMotions::Motion_22_SpeakHi;
        }

        if (input & sInputKey_GameSpeak3)
        {
            gEventSystem->VPushEvent(GameSpeakEvents::eUnknown_27);
            Slig_GameSpeak_SFX(SligSpeak::eFreeze_8, 0, mGameSpeakPitchMin, this);
            return eSligMotions::Motion_23_SpeakFreeze;
        }

        if (input & sInputKey_GameSpeak4)
        {
            gEventSystem->VPushEvent(GameSpeakEvents::eUnknown_25);
            Slig_GameSpeak_SFX(SligSpeak::eGetHim_2, 0, mGameSpeakPitchMin, this);
            return eSligMotions::Motion_24_SpeakGetHim;
        }
        return -1;
    }
    else if (Input().IsAnyPressed(sInputKey_RightGameSpeakEnabler))
    {
        if (input & sInputKey_GameSpeak8)
        {
            gEventSystem->VPushEvent(GameSpeakEvents::eUnknown_8);
            Slig_GameSpeak_SFX(SligSpeak::eLaugh_7, 0, mGameSpeakPitchMin, this);
            return eSligMotions::Motion_25_SpeakLaugh;
        }

        if (input & sInputKey_GameSpeak6)
        {
            gEventSystem->VPushEvent(GameSpeakEvents::eUnknown_5);
            Slig_GameSpeak_SFX(SligSpeak::eBullshit_4, 0, mGameSpeakPitchMin, this);
            return eSligMotions::Motion_26_SpeakBullshit1;
        }

        if (input & sInputKey_GameSpeak7)
        {
            gEventSystem->VPushEvent(GameSpeakEvents::Slig_LookOut_6);
            Slig_GameSpeak_SFX(SligSpeak::eLookOut_5, 0, mGameSpeakPitchMin, this);
            return eSligMotions::Motion_27_SpeakLookOut;
        }

        if (input & sInputKey_GameSpeak5)
        {
            gEventSystem->VPushEvent(GameSpeakEvents::eUnknown_7);
            Slig_GameSpeak_SFX(SligSpeak::eBullshit2_6, 0, mGameSpeakPitchMin, this);
            return eSligMotions::Motion_28_SpeakBullshit2;
        }
    }
    return -1;
}

bool Slig::InAnyWellRenderLayer(BaseAliveGameObject* pThis)
{
    return pThis->GetAnimation().GetRenderLayer() == Layer::eLayer_BeforeWell_Half_3 || pThis->GetAnimation().GetRenderLayer() == Layer::eLayer_BeforeWell_22;
}

s16 Slig::IsAbeEnteringDoor(BaseAliveGameObject* pThis)
{
    if (((pThis->Type() == ReliveTypes::eAbe) && (pThis->mCurrentMotion == eAbeMotions::Motion_156_DoorEnter && pThis->GetAnimation().GetCurrentFrame() > 7)) || (pThis->mCurrentMotion == eAbeMotions::Motion_157_DoorExit && pThis->GetAnimation().GetCurrentFrame() < 4))
    {
        return 1;
    }
    return 0;
}

s16 Slig::IsWallBetween(Slig* pLeft, BaseAliveGameObject* pRight)
{
    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    return sCollisions->Raycast(
               pLeft->mXPos,
               pLeft->mYPos - FP_FromInteger(25),
               pRight->mXPos,
               pRight->mYPos - FP_FromInteger(25),
               &pLine,
               &hitX,
               &hitY,
               pLeft->GetSpriteScale() != FP_FromDouble(0.5) ? kFgWalls : kBgWalls)
        == 1;
}

void Slig::Slig_GameSpeak_SFX(SligSpeak effectId, s32 defaultVol, s32 pitch_min, BaseAliveGameObject* pObj)
{
    s32 volume = defaultVol;
    if (defaultVol == 0)
    {
        volume = sSligSounds2[static_cast<s32>(effectId)].field_C_default_volume;
    }
    if (pObj)
    {
        if (pObj->GetSpriteScale() == FP_FromDouble(0.5))
        {
            volume = FP_GetExponent(FP_FromInteger(volume * 2) / FP_FromInteger(3));
        }
    }
    SFX_SfxDefinition_Play_Mono(sSligSounds2[static_cast<s32>(effectId)], volume, pitch_min, pitch_min);
}

s16 Slig::IsInInvisibleZone(BaseAnimatedWithPhysicsGameObject* pObj)
{
    /* TODO: Not used in AE but might be possible to activate in AO
    if (gAbeInvisibleCheat)
    {
        return TRUE;
    }
    */

    if (EventGet(kEventAbeOhm))
    {
        return FALSE;
    }

    const PSX_RECT rect = pObj->VGetBoundingRect();

    relive::Path_TLV* pTlv = gMap.TLV_Get_At(rect.x, rect.y, rect.w, rect.h, ReliveTypes::eInvisibleZone);
    if (pTlv)
    {
        if (rect.x >= pTlv->mTopLeftX && rect.x <= pTlv->mBottomRightX && rect.y >= pTlv->mTopLeftY)
        {
            if (rect.y <= pTlv->mBottomRightY && rect.w >= pTlv->mTopLeftX && rect.w <= pTlv->mBottomRightX && rect.h >= pTlv->mTopLeftY && rect.h <= pTlv->mBottomRightY)
            {
                return TRUE;
            }
        }
    }

    return FALSE;
}

void Slig::ToStand()
{
    field_12C_falling_velx_scale_factor = FP_FromInteger(0);
    mVelX = FP_FromInteger(0);
    mVelY = FP_FromInteger(0);
    mCurrentMotion = eSligMotions::Motion_0_StandIdle;
    field_126_input = 0;
    field_128_timer = Math_RandomRange(0, 60) + sGnFrame + 120;
    MapFollowMe(TRUE);
}

s16 Slig::IsInZCover(BaseAnimatedWithPhysicsGameObject* pObj)
{
    const PSX_RECT bRect = pObj->VGetBoundingRect();
    return Bullet::InZBulletCover(FP_FromInteger(bRect.x), FP_FromInteger(bRect.y), bRect);
}

void Slig::CheckPlatformVanished()
{
    if (mLiftPoint)
    {
        if (mLiftPoint->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
        {
            // Platform is somehow gone, fall.
            const auto oldMotion = mCurrentMotion;
            VOnTrapDoorOpen();
            mCurrentMotion = oldMotion;
        }
        SetActiveCameraDelayedFromDir();
    }
}


s16 Slig::MoveLift(FP ySpeed)
{
    auto pLiftPoint = static_cast<LiftPoint*>(mLiftPoint);

    pLiftPoint->Move(FP_FromInteger(0), ySpeed, 0);
    CheckPlatformVanished();
    mVelY = pLiftPoint->mVelY;

    if (sControlledCharacter == this
        && !(GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
        && GetAnimation().GetCurrentFrame() != 5)
    {
        return mCurrentMotion;
    }

    if (ySpeed < FP_FromInteger(0))
    {
        if (pLiftPoint->OnTopFloor())
        {
            return eSligMotions::Motion_51_LiftGripping;
        }

        if (Input().IsAnyPressed(sInputKey_Up))
        {
            return eSligMotions::Motion_47_LiftUp;
        }

        if (Input().IsAnyPressed(sInputKey_Down))
        {
            return eSligMotions::Motion_48_LiftDown;
        }
    }
    else if (ySpeed > FP_FromInteger(0))
    {
        if (pLiftPoint->OnBottomFloor())
        {
            return eSligMotions::Motion_51_LiftGripping;
        }

        if (Input().IsAnyPressed(sInputKey_Down))
        {
            return eSligMotions::Motion_48_LiftDown;
        }

        if (Input().IsAnyPressed(sInputKey_Up))
        {
            return eSligMotions::Motion_47_LiftUp;
        }
    }

    // Strange how this isn't "if nothing pressed and on a floor then let go ??"
    if (Input().Pressed() && pLiftPoint->OnAnyFloor())
    {
        return eSligMotions::Motion_50_LiftUngrip;
    }

    pLiftPoint->Move(FP_FromInteger(0), FP_FromInteger(0), 0);
    return eSligMotions::Motion_51_LiftGripping;
}

void Slig::GameSpeakResponse()
{
    GameSpeakEvents speak = GameSpeakEvents::eNone_m1;

    const s32 lastIdx = gEventSystem->field_18_last_event_index;
    if (field_15C_last_event_index == lastIdx)
    {
        if (gEventSystem->field_10_last_event == GameSpeakEvents::eNone_m1)
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
        field_15C_last_event_index = lastIdx;
        speak = gEventSystem->field_10_last_event;
    }

    switch (speak)
    {
        case GameSpeakEvents::eWhistleHigh_1:
        case GameSpeakEvents::eWhistleLow_2:
            if (!(Math_NextRandom() & 4))
            {
                field_258_next_gamespeak_motion = eSligMotions::Motion_26_SpeakBullshit1;
            }
            else
            {
                field_258_next_gamespeak_motion = eSligMotions::Motion_28_SpeakBullshit2;
            }
            break;

        case GameSpeakEvents::eFart_3:
        case GameSpeakEvents::eLaugh_4:
        case GameSpeakEvents::eUnknown_8:
        case GameSpeakEvents::eUnknown_14:
        case GameSpeakEvents::eUnknown_15:
        case GameSpeakEvents::eUnknown_25:
        case GameSpeakEvents::eUnknown_30:
            field_258_next_gamespeak_motion = eSligMotions::Motion_32_Blurgh;
            return;

        case GameSpeakEvents::eUnknown_5:
            field_258_next_gamespeak_motion = eSligMotions::Motion_26_SpeakBullshit1;
            break;

        case GameSpeakEvents::Slig_LookOut_6:
            if (!(Math_NextRandom() & 4))
            {
                field_258_next_gamespeak_motion = eSligMotions::Motion_30_SpeakWhat;
            }
            else
            {
                field_258_next_gamespeak_motion = eSligMotions::Motion_29_SpeakPanic;
            }
            break;

        case GameSpeakEvents::eUnknown_7:
            field_258_next_gamespeak_motion = eSligMotions::Motion_28_SpeakBullshit2;
            break;

        case GameSpeakEvents::eHello_9:
        case GameSpeakEvents::Slig_Hi_23:
            field_258_next_gamespeak_motion = eSligMotions::Motion_22_SpeakHi;
            break;

        case GameSpeakEvents::eFollowMe_10:
        case GameSpeakEvents::eWait_12:
            if (Math_NextRandom() & 8)
            {
                field_258_next_gamespeak_motion = eSligMotions::Motion_26_SpeakBullshit1;
            }
            else
            {
                field_258_next_gamespeak_motion = eSligMotions::Motion_28_SpeakBullshit2;
            }
            break;

        case GameSpeakEvents::eAnger_11:
        case GameSpeakEvents::eUnknown_13:
        case GameSpeakEvents::eUnknown_16:
        case GameSpeakEvents::eUnknown_17:
        case GameSpeakEvents::eUnknown_18:
        case GameSpeakEvents::eUnknown_19:
        case GameSpeakEvents::eUnknown_20:
        case GameSpeakEvents::eUnknown_21:
        case GameSpeakEvents::eUnknown_22:
        case GameSpeakEvents::eUnknown_26:
        case GameSpeakEvents::eUnknown_27:
        case GameSpeakEvents::eUnknown_28:
        case GameSpeakEvents::eUnknown_31:
            field_258_next_gamespeak_motion = eSligMotions::Motion_25_SpeakLaugh;
            break;

        default:
            field_258_next_gamespeak_motion = eSligMotions::Motion_30_SpeakWhat;
            break;
    }
}

bool Slig::VIs8_465630(s16 motion)
{
    return motion == eSligMotions::Motion_8_Unknown;
}

void Slig::MoveOnLine()
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
                    // TODO: Inlined
                    PSX_RECT bRect = VGetBoundingRect();
                    bRect.y += 5;
                    bRect.h += 5;

                    OnCollisionWith(
                        {bRect.x, bRect.y},
                        {bRect.w, bRect.h},
                        gPlatformsArray);
                }
            }
        }
        else
        {
            VOnTrapDoorOpen();
            BaseAliveGameObjectLastLineYPos = mYPos;
            if (mCurrentMotion == eSligMotions::Motion_9_SlidingToStand || mCurrentMotion == eSligMotions::Motion_10_SlidingTurn)
            {
                mVelX = (mVelX * FP_FromDouble(0.5));
            }
            mCurrentMotion = eSligMotions::Motion_40_FallingInitiate;
            mXPos = mVelX + oldX;
        }
    }
    else
    {
        VOnTrapDoorOpen();
        mCurrentMotion = eSligMotions::Motion_39_OutToFall;
        BaseAliveGameObjectLastLineYPos = mYPos;
    }
}

void Slig::OperateLift()
{
    if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
    {
        mCurrentMotion = eSligMotions::Motion_5_TurnAroundStanding;
        mNextMotion = eSligMotions::Motion_49_LiftGrip;

        const auto camPos = gMap.GetDirection(mCurrentLevel, mCurrentPath, mXPos, mYPos);
        if ((camPos != CameraPos::eCamCurrent_0 && camPos != CameraPos::eCamInvalid_m1)
            && MusicController::GetAbmientAndMusicInfo(nullptr, nullptr, nullptr) <= MusicController::MusicTypes::eChase_4)
        {
            MusicController::static_PlayMusic(MusicController::MusicTypes::eChase_4, this, 0, 0);
        }
        Slig::Slig_SoundEffect(SligSfx::eStandingTurn_1);
        if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
        {
            GetAnimation().mFlags.Clear(AnimFlags::eFlipX);
            ToStand();
        }
    }
    else
    {
        mCurrentMotion = eSligMotions::Motion_49_LiftGrip;
    }
}

s16 Slig::HandlePlayerControlled()
{
    FP kScaleGrid = ScaleToGridSize(GetSpriteScale());

    const auto pressed = Input().Pressed();
    if (pressed & sInputKey_Right)
    {
        if (!GetAnimation().mFlags.Get(AnimFlags::eFlipX))
        {
            if (pressed & sInputKey_Run)
            {
                mVelX = kScaleGrid / FP_FromInteger(4);
                mCurrentMotion = eSligMotions::Motion_3_StandToRun;
                return 1;
            }

            if (!WallHit(GetSpriteScale() * FP_FromInteger(35), kScaleGrid * FP_FromInteger(2)))
            {
                mVelX = kScaleGrid / FP_FromInteger(9);
                mCurrentMotion = eSligMotions::Motion_16_StandingToStep;
                return 1;
            }

            if (WallHit(GetSpriteScale() * FP_FromInteger(35), kScaleGrid))
            {
                return 0;
            }
            else
            {
                mVelX = kScaleGrid / FP_FromInteger(6);
                mCurrentMotion = eSligMotions::Motion_15_SteppingToStand;
                return 1;
            }
        }
        else
        {
            mCurrentMotion = eSligMotions::Motion_5_TurnAroundStanding;
            return 1;
        }
    }

    if (pressed & sInputKey_Left)
    {
        if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
        {
            if (pressed & sInputKey_Run)
            {
                mCurrentMotion = eSligMotions::Motion_3_StandToRun;
                mVelX = -(kScaleGrid / FP_FromInteger(4));
                return 1;
            }
            if (!WallHit(GetSpriteScale() * FP_FromInteger(35), -kScaleGrid * FP_FromInteger(2)))
            {
                mCurrentMotion = eSligMotions::Motion_16_StandingToStep;
                mVelX = -(kScaleGrid / FP_FromInteger(9));
                return 1;
            }
            if (WallHit(GetSpriteScale() * FP_FromInteger(35), -kScaleGrid))
            {
                return 0;
            }
            else
            {
                mCurrentMotion = eSligMotions::Motion_15_SteppingToStand;
                mVelX = -(kScaleGrid / FP_FromInteger(6));
                return 1;
            }
        }
        else
        {
            mCurrentMotion = eSligMotions::Motion_5_TurnAroundStanding;
            return 1;
        }
    }

    if (Input().IsAnyHeld(sInputKey_DoAction)
        && !(pressed & (sInputKey_LeftGameSpeakEnabler | sInputKey_RightGameSpeakEnabler)))
    {
        if (!Input_IsChanting())
        {
            relive::Path_Lever* pTlv = static_cast<relive::Path_Lever*>(gMap.TLV_Get_At(
                FP_GetExponent(mXPos),
                FP_GetExponent(mYPos),
                FP_GetExponent(mXPos),
                FP_GetExponent(mYPos),
                ReliveTypes::eLever));
            BaseAliveGameObjectPathTLV = pTlv;
            if (pTlv)
            {
                if (FP_FromInteger(FP_GetExponent(mXPos) - pTlv->mTopLeftX) < kScaleGrid)
                {
                    if (!(GetAnimation().mFlags.Get(AnimFlags::eFlipX)))
                    {
                        auto pSwitch = static_cast<Lever*>(FindObjectOfType(
                            ReliveTypes::eLever,
                            mXPos + kScaleGrid,
                            mYPos - FP_FromInteger(5)));
                        if (pSwitch)
                        {
                            pSwitch->VPull(
                                mXPos < pSwitch->mXPos);
                            mCurrentMotion = eSligMotions::State_46_PullLever_46A590;
                            return 1;
                        }
                        mCurrentMotion = eSligMotions::State_46_PullLever_46A590;
                        return 1;
                    }
                    mNextMotion = eSligMotions::State_46_PullLever_46A590;
                    mCurrentMotion = eSligMotions::Motion_5_TurnAroundStanding;
                    return 1;
                }
                if (FP_FromInteger(pTlv->mBottomRightX - FP_GetExponent(mXPos)) < kScaleGrid)
                {
                    if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
                    {
                        auto pSwitch = static_cast<Lever*>(FindObjectOfType(ReliveTypes::eLever, mXPos - kScaleGrid, mYPos - FP_FromInteger(5)));
                        if (pSwitch)
                        {
                            pSwitch->VPull(
                                mXPos < pSwitch->mXPos);
                        }
                        mCurrentMotion = eSligMotions::State_46_PullLever_46A590;
                        return 1;
                    }
                    mNextMotion = eSligMotions::State_46_PullLever_46A590;
                    mCurrentMotion = eSligMotions::Motion_5_TurnAroundStanding;
                    return 1;
                }
                Slig::Slig_GameSpeak_SFX(SligSpeak::eWhat_9, 0, mGameSpeakPitchMin, this);
            }
            else
            {
                Slig::Slig_GameSpeak_SFX(SligSpeak::eWhat_9, 0, mGameSpeakPitchMin, this);
            }
            mCurrentMotion = eSligMotions::Motion_30_SpeakWhat;
            return 1;
        }
    }
    if (pressed & sInputKey_ThrowItem
        && !(pressed & (sInputKey_LeftGameSpeakEnabler | sInputKey_RightGameSpeakEnabler)))
    {
        if (!Input_IsChanting())
        {
            mCurrentMotion = eSligMotions::Motion_6_Shoot;
            field_128_timer = sGnFrame + 60;
            return 1;
        }
    }
    if (pressed & sInputKey_FartRoll
        && !(pressed & (sInputKey_LeftGameSpeakEnabler | sInputKey_RightGameSpeakEnabler)))
    {
        if (!Input_IsChanting())
        {
            mCurrentMotion = eSligMotions::Motion_52_Beat;
            field_128_timer = sGnFrame + 60;
            return 1;
        }
    }

    if (pressed & sInputKey_Down)
    {
        if (mLiftPoint)
        {
            if (mLiftPoint && mLiftPoint->Type() == ReliveTypes::eLiftPoint)
            {
                if (FP_Abs(mXPos - FP_FromInteger((BaseAliveGameObjectCollisionLine->mRect.x + BaseAliveGameObjectCollisionLine->mRect.w) / 2)) < kScaleGrid / FP_FromInteger(2))
                {
                    OperateLift();
                    return 1;
                }
            }
        }
        else
        {
            if (Input().IsAnyHeld(sInputKey_Down))
            {
                Slig::Slig_GameSpeak_SFX(SligSpeak::eWhat_9, 0, mGameSpeakPitchMin, this);
                mCurrentMotion = eSligMotions::Motion_30_SpeakWhat;
                return 1;
            }
            ToStand();
            return 0;
        }
    }
    if (pressed & sInputKey_Up)
    {
        if (mLiftPoint)
        {
            if (mLiftPoint->field_10C == 1)
            {
                if (FP_Abs(mXPos - FP_FromInteger((BaseAliveGameObjectCollisionLine->mRect.x + BaseAliveGameObjectCollisionLine->mRect.w) / 2)) < kScaleGrid / FP_FromInteger(2))
                {
                    OperateLift();
                    return 1;
                }
            }
        }
        else
        {
            if (Input().IsAnyHeld(sInputKey_Up))
            {
                Slig::Slig_GameSpeak_SFX(SligSpeak::eWhat_9, 0, mGameSpeakPitchMin, this);
                mCurrentMotion = eSligMotions::Motion_30_SpeakWhat;
                return 1;
            }
            ToStand();
            return 0;
        }
    }

    if (Input().IsAnyHeld(sInputKey_Hop | sInputKey_FartRoll) && !(Input().IsAnyPressed(sInputKey_LeftGameSpeakEnabler | sInputKey_RightGameSpeakEnabler)))
    {
        Slig::Slig_GameSpeak_SFX(SligSpeak::eBlurgh_11, 0, mGameSpeakPitchMin, this);
        mCurrentMotion = eSligMotions::Motion_32_Blurgh;
        return 1;
    }
    ToStand();
    return 0;
}

void Slig::PlayerControlRunningSlideStopOrTurn(s16 last_anim_frame)
{
    if (mVelX > FP_FromInteger(0) && Input().IsAnyPressed(sInputKey_Left))
    {
        mVelX = GetSpriteScale() * FP_FromDouble(13.2);
        mCurrentMotion = eSligMotions::Motion_10_SlidingTurn;
        field_126_input = 0;
        return;
    }

    if (mVelX < FP_FromInteger(0) && Input().IsAnyPressed(sInputKey_Right))
    {
        mVelX = GetSpriteScale() * FP_FromDouble(-13.2);
        mCurrentMotion = eSligMotions::Motion_10_SlidingTurn;
        field_126_input = 0;
        return;
    }

    if (!Input().IsAnyPressed(sInputKey_Right | sInputKey_Left))
    {
        if (mVelX >= FP_FromInteger(0))
        {
            mVelX = GetSpriteScale() * FP_FromDouble(13.2);
        }
        else
        {
            mVelX = GetSpriteScale() * FP_FromDouble(-13.2);
        }

        mCurrentMotion = eSligMotions::Motion_9_SlidingToStand;
        field_126_input = 0;
        return;
    }

    if (Input().IsAnyPressed(sInputKey_Run))
    {
        field_126_input = 0;
        return;
    }

    mPreviousMotion = 2;
    mBaseAliveGameObjectLastAnimFrame = last_anim_frame;
    mReturnToPreviousMotion = 1;

    if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
    {
        mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(9));
    }
    else
    {
        mVelX = ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(9);
    }
    field_126_input = 0;
}

void Slig::PlayerControlRunningSlideStopOrTurnFrame4()
{
    PlayerControlRunningSlideStopOrTurn(6);
}

void Slig::PlayerControlRunningSlideStopOrTurnFrame12()
{
    PlayerControlRunningSlideStopOrTurn(15);
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
                mVelX += (GetSpriteScale() * speed);
                if (mVelX > FP_FromInteger(0))
                {
                    mVelX = FP_FromInteger(0);
                }
            }
        }
        else
        {
            mVelX -= (GetSpriteScale() * speed);
            if (mVelX < FP_FromInteger(0))
            {
                mVelX = FP_FromInteger(0);
            }
        }
    }
}

s16 Slig::MainMovement()
{
    if (sControlledCharacter == this && mHealth > FP_FromInteger(0))
    {
        return HandlePlayerControlled();
    }

    switch (mNextMotion)
    {
        case eSligMotions::Motion_5_TurnAroundStanding:

            mCurrentMotion = eSligMotions::Motion_5_TurnAroundStanding;
            mNextMotion = -1;
            break;
        case eSligMotions::Motion_2_Walking:

            if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
            {
                if (WallHit(GetSpriteScale() * FP_FromInteger(35), -ScaleToGridSize(GetSpriteScale())))
                {
                    return 0;
                }
                mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(9));
            }
            else
            {
                if (WallHit(GetSpriteScale() * FP_FromInteger(35), ScaleToGridSize(GetSpriteScale())))
                {
                    return 0;
                }
                mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(9));
            }

            mCurrentMotion = eSligMotions::Motion_1_StandToWalk;
            mNextMotion = -1;
            break;
        case eSligMotions::Motion_4_Running:

            if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
            {
                mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
            }
            else
            {
                mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
            }

            mCurrentMotion = eSligMotions::Motion_3_StandToRun;
            mNextMotion = -1;
            break;
        case eSligMotions::Motion_6_Shoot:
        case eSligMotions::Motion_43_ShootZ:
            field_12C_falling_velx_scale_factor = FP_FromInteger(0);
            mVelX = FP_FromInteger(0);
            mVelY = FP_FromInteger(0);
            field_128_timer = sGnFrame + 60;
            mCurrentMotion = mNextMotion;
            mNextMotion = -1;
            break;
        default:
            if (mNextMotion < eSligMotions::Motion_21_SpeakHereBoy || mNextMotion > eSligMotions::Motion_32_Blurgh)
            {
                if (mNextMotion == eSligMotions::Motion_13_ReloadGun
                    || mNextMotion == eSligMotions::State_46_PullLever_46A590
                    || mNextMotion == eSligMotions::Motion_52_Beat)
                {
                    mCurrentMotion = mNextMotion;
                    mNextMotion = -1;
                    break;
                }
            }
            else
            {
                mCurrentMotion = GetNextMotionIncGameSpeak(0);
                if (mCurrentMotion != -1)
                {
                    EventBroadcast(kEventSpeaking, this);
                    break;
                }
            }

            ToStand();
            return 0;
    }

    return 1;
}

void Slig::ToKnockBack()
{
    Environment_SFX_42A220(EnvironmentSfx::eKnockback_13, 0, 0x7FFF, this);

    mXPos -= mVelX;

    if (BaseAliveGameObjectCollisionLine)
    {
        if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
        {
            mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
        }
        else
        {
            mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
        }
    }
    else
    {
        mVelX = FP_FromInteger(0);
    }

    if (mNextMotion == eSligMotions::Motion_45_Smash)
    {
        mVelX = -mVelX;
    }

    MapFollowMe(1);

    if (mVelY < FP_FromInteger(0))
    {
        mVelY = FP_FromInteger(0);
    }

    mCurrentMotion = eSligMotions::Motion_35_Knockback;
    field_128_timer = sGnFrame + 10;
}

void Slig::Motion_0_StandIdle()
{
    if (!MainMovement())
    {
        if (sControlledCharacter == this && mHealth > FP_FromInteger(0))
        {
            if (Input_IsChanting())
            {
                if (!(mPreventDepossession & 4))
                {
                    mCurrentMotion = eSligMotions::Motion_37_Depossessing;
                    field_128_timer = sGnFrame + 30;
                    SfxPlayMono(relive::SoundEffects::PossessEffect, 0);
                    return;
                }
            }
            else if (Input().IsAnyPressed(0x04 | 0x01))
            {
                if (Input().IsAnyHeld(0xF0))
                {
                    field_126_input = Input().Held();
                    mCurrentMotion = eSligMotions::Motion_18_GameSpeak;
                    return;
                }
            }
        }

        if (static_cast<s32>(sGnFrame) >= field_128_timer)
        {
            mCurrentMotion = eSligMotions::Motion_13_ReloadGun;
        }
    }
}

void Slig::Motion_1_StandToWalk()
{
    field_126_input |= Input().Held();

    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        mCurrentMotion = eSligMotions::Motion_2_Walking;
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
        && MusicController::GetAbmientAndMusicInfo(nullptr, nullptr, nullptr) <= MusicController::MusicTypes::eChase_4)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eChase_4, this, 0, 0);
    }

    field_126_input |= Input().Held();

    if (WallHit(GetSpriteScale() * FP_FromInteger(35), mVelX * FP_FromInteger(2)))
    {
        Motion_46_ToIdle();
        return;
    }

    MoveOnLine();

    if (mCurrentMotion == eSligMotions::Motion_2_Walking)
    {
        FP v12 = {};
        switch (GetAnimation().GetCurrentFrame())
        {
            case 5:

                Slig_SoundEffect(SligSfx::eWalkingStep_2);
                if (!field_120_checked_if_off_screen)
                {
                    field_120_checked_if_off_screen = 1;
                    MapFollowMe(1);
                }
                if (sControlledCharacter == this && mHealth > FP_FromInteger(0))
                {
                    if (Input().IsAnyPressed(sInputKey_Run))
                    {
                        mPreviousMotion = eSligMotions::Motion_4_Running;
                        mBaseAliveGameObjectLastAnimFrame = 5;
                        mReturnToPreviousMotion = 1;

                        if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
                        {
                            field_126_input = 0;
                            mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
                            return;
                        }

                        mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
                        field_126_input = 0;
                        return;
                    }
                    field_126_input = 0;
                    return;
                }
                break;
            case 11:

                if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
                {
                    v12 = -(ScaleToGridSize(GetSpriteScale()));
                }
                else
                {
                    v12 = (ScaleToGridSize(GetSpriteScale()));
                }

                if (WallHit(GetSpriteScale() * FP_FromInteger(35), v12 * FP_FromDouble(2.5)))
                {
                    mCurrentMotion = eSligMotions::Motion_19_WalkToStand;
                }
                else if (sControlledCharacter != this || mHealth <= FP_FromInteger(0))
                {
                    if (mNextMotion == eSligMotions::Motion_6_Shoot)
                    {
                        mCurrentMotion = eSligMotions::Motion_19_WalkToStand;
                        field_128_timer = sGnFrame + 60;
                    }
                    else if (mNextMotion != -1)
                    {
                        mCurrentMotion = eSligMotions::Motion_19_WalkToStand;
                    }
                }
                else
                {
                    if ((mVelX > FP_FromInteger(0) && Input().IsAnyPressed(sInputKey_Left))
                        || (mVelX < FP_FromInteger(0) && Input().IsAnyPressed(sInputKey_Right))
                        || !(Input().IsAnyPressed(sInputKey_Right | sInputKey_Left)))
                    {
                        mCurrentMotion = eSligMotions::Motion_19_WalkToStand;
                    }
                    field_126_input = 0;
                }
                break;
            case 14:

                Slig_SoundEffect(SligSfx::eWalkingStep_2);

                if (!field_120_checked_if_off_screen)
                {
                    field_120_checked_if_off_screen = 1;
                    MapFollowMe(1);
                }

                if (sControlledCharacter == this && mHealth > FP_FromInteger(0))
                {
                    if (Input().IsAnyPressed(sInputKey_Run))
                    {
                        mPreviousMotion = 4;
                        mBaseAliveGameObjectLastAnimFrame = 13;
                        mReturnToPreviousMotion = 1;

                        if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
                        {
                            field_126_input = 0;
                            mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
                            return;
                        }

                        mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
                        field_126_input = 0;
                        return;
                    }
                    field_126_input = 0;
                    return;
                }
                break;
            default:

                field_120_checked_if_off_screen = 0;
                break;
        }
    }
}

void Slig::Motion_3_StandToRun()
{
    field_126_input |= Input().Held();

    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        mCurrentMotion = eSligMotions::Motion_4_Running;
    }

    EventBroadcast(kEventNoise, this);

    if (WallHit(GetSpriteScale() * FP_FromInteger(35), mVelX))
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
    if (gMap.GetDirection(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos)
            >= CameraPos::eCamCurrent_0
        && MusicController::GetAbmientAndMusicInfo(nullptr, nullptr, nullptr) != MusicController::MusicTypes::ePossessed_6)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eSlogChase_5, this, 0, 0);
    }

    field_126_input |= Input().Held();

    EventBroadcast(kEventNoise, this);

    if (WallHit(GetSpriteScale() * FP_FromInteger(35), mVelX))
    {
        ToKnockBack();
    }
    else
    {
        MoveOnLine();

        if (mCurrentMotion == eSligMotions::Motion_4_Running)
        {
            if (GetAnimation().GetCurrentFrame() == 4 || GetAnimation().GetCurrentFrame() == 12)
            {
                Slig_SoundEffect(SligSfx::eRunningStep_3);

                if (field_120_checked_if_off_screen == 0)
                {
                    field_120_checked_if_off_screen = 1;
                    MapFollowMe(TRUE);
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
                    if (mNextMotion == eSligMotions::Motion_2_Walking)
                    {
                        mCurrentMotion = eSligMotions::Motion_2_Walking;
                        mNextMotion = -1;

                        if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
                        {
                            mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(9));
                        }
                        else
                        {
                            mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(9));
                        }
                    }
                    else if (mNextMotion == eSligMotions::Motion_6_Shoot)
                    {
                        mCurrentMotion = eSligMotions::Motion_6_Shoot;
                        mNextMotion = -1;
                        field_128_timer = sGnFrame + 60;
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

                        mCurrentMotion = eSligMotions::Motion_9_SlidingToStand;
                    }
                }
            }
            else
            {
                field_120_checked_if_off_screen = 0;
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
        && MusicController::GetAbmientAndMusicInfo(nullptr, nullptr, nullptr) <= MusicController::MusicTypes::eChase_4)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eChase_4, this, 0, 0);
    }

    Slig_SoundEffect(SligSfx::eStandingTurn_1);

    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        ToStand();
        GetAnimation().mFlags.Toggle(AnimFlags::eFlipX);
    }
}

void Slig::Motion_6_Shoot()
{
    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        if (sControlledCharacter == this && mHealth > FP_FromInteger(0))
        {
            if (Input().IsAnyPressed(sInputKey_ThrowItem))
            {
                const FP k35Scaled = GetSpriteScale() * FP_FromInteger(35);
                const FP kGridSize = ScaleToGridSize(GetSpriteScale());
                const FP k8 = FP_FromInteger(8);

                // Recoil right
                if (GetAnimation().mFlags.Get(AnimFlags::eFlipX) && Input().IsAnyPressed(sInputKey_Right))
                {
                    PathLine* pLine = nullptr;
                    FP hitX = {};
                    FP hitY = {};
                    if (sCollisions->Raycast(
                            mXPos,
                            mYPos,
                            mXPos + (k8 * (kGridSize / k8)),
                            mYPos,
                            &pLine,
                            &hitX,
                            &hitY,
                            GetSpriteScale() != FP_FromDouble(0.5) ? kFgWalls : kBgWalls)
                        || sCollisions->Raycast(
                            mXPos,
                            mYPos - k35Scaled,
                            mXPos + (k8 * (kGridSize / k8)),
                            mYPos - k35Scaled,
                            &pLine,
                            &hitX,
                            &hitY,
                            GetSpriteScale() != FP_FromDouble(0.5) ? kFgWalls : kBgWalls))
                    {
                        return;
                    }
                    mVelX = (kGridSize / k8);
                    mCurrentMotion = eSligMotions::Motion_20_Recoil;
                    return;
                }

                // Recoil left
                if (!GetAnimation().mFlags.Get(AnimFlags::eFlipX) && Input().IsAnyPressed(sInputKey_Left))
                {
                    PathLine* pLine = nullptr;
                    FP hitX = {};
                    FP hitY = {};
                    if (sCollisions->Raycast(
                            mXPos,
                            mYPos,
                            mXPos - (k8 * (kGridSize / k8)),
                            mYPos,
                            &pLine,
                            &hitX,
                            &hitY,
                            GetSpriteScale() != FP_FromDouble(0.5) ? kFgWalls : kBgWalls)
                        || sCollisions->Raycast(
                            mXPos,
                            mYPos - k35Scaled,
                            mXPos - (k8 * (kGridSize / k8)),
                            mYPos - k35Scaled,
                            &pLine,
                            &hitX,
                            &hitY,
                            GetSpriteScale() != FP_FromDouble(0.5) ? kFgWalls : kBgWalls))
                    {
                        return;
                    }
                    mVelX = -(kGridSize / k8);

                    mCurrentMotion = eSligMotions::Motion_20_Recoil;
                    return;
                }

                // General recoil
                // TODO: @ LABEL_27 is there a missing condition here, AE is checking
                // for the down key?
                if (field_128_timer > static_cast<s32>(sGnFrame))
                {
                    return;
                }

                if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
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
                if (sCollisions->Raycast(
                        mXPos,
                        mYPos,
                        mXPos + (k8 * mVelX),
                        mYPos,
                        &pLine,
                        &hitX,
                        &hitY,
                        GetSpriteScale() != FP_FromDouble(0.5) ? kFgWalls : kBgWalls)
                    || sCollisions->Raycast(
                        mXPos,
                        mYPos - k35Scaled,
                        mXPos + (k8 * mVelX),
                        mYPos - k35Scaled,
                        &pLine,
                        &hitX,
                        &hitY,
                        GetSpriteScale() != FP_FromDouble(0.5) ? kFgWalls : kBgWalls))
                {
                    mVelX = FP_FromInteger(0);
                }
                else
                {
                    mCurrentMotion = eSligMotions::Motion_20_Recoil;
                }
                return;
            }
            else
            {
                SND_SEQ_PlaySeq_4775A0(SeqId::eHitBottomOfDeathPit_10, 1, 1);
                mCurrentMotion = eSligMotions::Motion_14_ShootToStand;
                return;
            }
        }

        if (mNextMotion == eSligMotions::Motion_0_StandIdle)
        {
            SND_SEQ_PlaySeq_4775A0(SeqId::eHitBottomOfDeathPit_10, 1, 1);
            mCurrentMotion = eSligMotions::Motion_14_ShootToStand;
            mNextMotion = -1;
            return;
        }

        if (mNextMotion != -1 && mNextMotion != eSligMotions::Motion_6_Shoot)
        {
            SND_SEQ_PlaySeq_4775A0(SeqId::eHitBottomOfDeathPit_10, 1, 1);
            mCurrentMotion = eSligMotions::Motion_14_ShootToStand;
            return;
        }
    }
}

void Slig::Motion_7_Falling()
{
    if (mVelX > FP_FromInteger(0))
    {
        mVelX -= GetSpriteScale() * field_12C_falling_velx_scale_factor;
        if (mVelX < FP_FromInteger(0))
        {
            mVelX = FP_FromInteger(0);
        }
    }
    else if (mVelX < FP_FromInteger(0))
    {
        mVelX += GetSpriteScale() * field_12C_falling_velx_scale_factor;
        if (mVelX > FP_FromInteger(0))
        {
            mVelX = FP_FromInteger(0);
        }
    }

    FP hitX = {};
    FP hitY = {};
    PathLine* pLine = nullptr;
    const s16 bHit = InAirCollision(&pLine, &hitX, &hitY, FP_FromDouble(1.8));
    SetActiveCameraDelayedFromDir();
    if (bHit)
    {
        switch (pLine->mLineType)
        {
            case 0:
            case 4:
            case 32:
            case 36:
            {
                ToStand();

                BaseAliveGameObjectCollisionLine = pLine;

                PSX_RECT bRect = VGetBoundingRect();
                bRect.y += 5;
                bRect.h += 5;

                OnCollisionWith(
                    {bRect.x, bRect.y},
                    {bRect.w, bRect.h},
                    gPlatformsArray);

                mXPos = hitX;
                mCurrentMotion = eSligMotions::Motion_41_LandingSoft;
                mYPos = hitY;
                MapFollowMe(TRUE);
                break;
            }

            case 1:
            case 2:
            case 5:
            case 6:
                mYPos = hitY;
                mXPos = hitX;
                ToKnockBack();
                break;

            default:
                break;
        }
    }
}

// uses slig idle anim
void Slig::Motion_8_Unknown()
{
    if (gNumCamSwappers <= 0)
    {
        if (sControlledCharacter != this || mHealth <= FP_FromInteger(0))
        {
            mCurrentMotion = mPreviousMotion;
            if (mLiftPoint)
            {
                mXPos = FP_FromInteger((BaseAliveGameObjectCollisionLine->mRect.x + BaseAliveGameObjectCollisionLine->mRect.w) / 2);
                mYPos = FP_FromInteger(BaseAliveGameObjectCollisionLine->mRect.y);
            }
        }
        else
        {
            mCurrentMotion = mPreviousMotion;
            if (mLiftPoint)
            {
                static_cast<LiftPoint*>(mLiftPoint)->field_12C_bMoving |= 1u;
            }
        }

        mCurrentLevel = gMap.mCurrentLevel;
        mCurrentPath = gMap.mCurrentPath;
    }
}

void Slig::Motion_9_SlidingToStand()
{
    EventBroadcast(kEventNoise, this);

    if (WallHit(GetSpriteScale() * FP_FromInteger(35), mVelX))
    {
        ToKnockBack();
    }
    else
    {
        SlowDown(FP_FromDouble(2.125));
        if (mCurrentMotion == eSligMotions::Motion_9_SlidingToStand)
        {
            if (GetAnimation().GetCurrentFrame() >= 6 || sControlledCharacter != this || mHealth <= FP_FromInteger(0))
            {
                if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
                {
                    Environment_SFX_42A220(EnvironmentSfx::eSlideStop_0, 0, 0x7FFF, this);
                    MapFollowMe(1);
                    MainMovement();
                }
            }
            else if ((GetAnimation().mFlags.Get(AnimFlags::eFlipX) && Input().IsAnyPressed(sInputKey_Right))
                     || (!(GetAnimation().mFlags.Get(AnimFlags::eFlipX)) && Input().IsAnyPressed(sInputKey_Left)))
            {
                mPreviousMotion = eSligMotions::Motion_10_SlidingTurn;
                mBaseAliveGameObjectLastAnimFrame = GetAnimation().GetCurrentFrame();
                mReturnToPreviousMotion = 1;
            }
        }
    }
}

void Slig::Motion_10_SlidingTurn()
{
    EventBroadcast(kEventNoise, this);
    if (WallHit(GetSpriteScale() * FP_FromInteger(35), mVelX))
    {
        ToKnockBack();
        return;
    }

    SlowDown(FP_FromDouble(2.125));

    if (mCurrentMotion == eSligMotions::Motion_10_SlidingTurn && GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        MapFollowMe(1);

        if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
        {
            if (sControlledCharacter != this || mHealth <= FP_FromInteger(0))
            {
                if (mNextMotion == 4)
                {
                    mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
                    mCurrentMotion = eSligMotions::Motion_12_SlidingTurnToRun;
                    return;
                }
            }
            else
            {
                if (Input().IsAnyPressed(sInputKey_Run))
                {
                    mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
                    mCurrentMotion = eSligMotions::Motion_12_SlidingTurnToRun;
                    return;
                }
            }
            mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(9));
            mCurrentMotion = eSligMotions::Motion_11_SlidingTurnToWalk;
        }
        else
        {
            if (sControlledCharacter != this || mHealth <= FP_FromInteger(0))
            {
                if (mNextMotion == eSligMotions::Motion_4_Running)
                {
                    mCurrentMotion = eSligMotions::Motion_12_SlidingTurnToRun;
                    mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
                    return;
                }
            }
            else
            {
                if (Input().IsAnyPressed(sInputKey_Run))
                {
                    mCurrentMotion = eSligMotions::Motion_12_SlidingTurnToRun;
                    mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
                    return;
                }
            }
            mCurrentMotion = eSligMotions::Motion_11_SlidingTurnToWalk;
            mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(9));
        }
    }
}

void Slig::Motion_11_SlidingTurnToWalk()
{
    EventBroadcast(kEventNoise, this);
    MoveOnLine();

    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        GetAnimation().mFlags.Toggle(AnimFlags::eFlipX);
        mCurrentMotion = eSligMotions::Motion_2_Walking;
    }
}

void Slig::Motion_12_SlidingTurnToRun()
{
    EventBroadcast(kEventNoise, this);
    MoveOnLine();

    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        GetAnimation().mFlags.Toggle(AnimFlags::eFlipX);
        mCurrentMotion = eSligMotions::Motion_4_Running;
    }
}

void Slig::Motion_13_ReloadGun()
{
    Motion_46_ToIdle();
}

void Slig::Motion_14_ShootToStand()
{
    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        MainMovement();
    }
}

void Slig::Motion_15_SteppingToStand()
{
    if (!GetAnimation().GetCurrentFrame())
    {
        Slig_SoundEffect(SligSfx::eWalkingStep_2);
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(35), mVelX))
    {
        ToKnockBack();
    }
    else
    {
        MoveOnLine();
        if (mCurrentMotion == eSligMotions::Motion_15_SteppingToStand)
        {
            if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
            {
                MapFollowMe(1);
                Motion_46_ToIdle();
            }
        }
    }
}

void Slig::Motion_16_StandingToStep()
{
    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        if (Input().IsAnyPressed(sInputKey_Left | sInputKey_Right))
        {
            mCurrentMotion = eSligMotions::Motion_1_StandToWalk;
        }
        else if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
        {
            mCurrentMotion = eSligMotions::Motion_15_SteppingToStand;
            mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(6));
        }
        else
        {
            mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(6));
            mCurrentMotion = eSligMotions::Motion_15_SteppingToStand;
        }
    }
}

void Slig::Motion_17_DepossessingAbort()
{
    Motion_46_ToIdle();
}

void Slig::Motion_18_GameSpeak()
{
    field_126_input |= Input().Held();

    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        mCurrentMotion = GetNextMotionIncGameSpeak(field_126_input);
        if (mCurrentMotion == -1)
        {
            ToStand();
        }
        else
        {
            EventBroadcast(kEventSpeaking, this);
        }
        field_126_input = 0;
    }
}

void Slig::Motion_19_WalkToStand()
{
    MoveOnLine();

    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        MapFollowMe(1);
        if (!MainMovement())
        {
            Motion_46_ToIdle();
        }
    }
}

void Slig::Motion_20_Recoil()
{
    MoveOnLine();

    if (mCurrentMotion == eSligMotions::Motion_20_Recoil)
    {
        if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
        {
            MapFollowMe(TRUE);

            if (sControlledCharacter != this || mHealth <= FP_FromInteger(0))
            {
                if (mNextMotion != -1)
                {
                    SND_SEQ_PlaySeq_4775A0(SeqId::eHitBottomOfDeathPit_10, 1, 1);
                    mCurrentMotion = eSligMotions::Motion_14_ShootToStand;
                    mNextMotion = -1;
                }
            }
            else if (Input().IsAnyPressed(sInputKey_ThrowItem))
            {
                mVelX = FP_FromInteger(0);
                mPreviousMotion = eSligMotions::Motion_6_Shoot;
                field_128_timer = sGnFrame + 60;
                mBaseAliveGameObjectLastAnimFrame = 2;
                mReturnToPreviousMotion = 1;
            }
            else
            {
                SND_SEQ_PlaySeq_4775A0(SeqId::eHitBottomOfDeathPit_10, 1, 1);
                mCurrentMotion = eSligMotions::Motion_14_ShootToStand;
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

void Slig::Motion_21_SpeakHereBoy()
{
    Motion_18_GameSpeak();
}

void Slig::Motion_22_SpeakHi()
{
    Motion_18_GameSpeak();
}

void Slig::Motion_23_SpeakFreeze()
{
    Motion_18_GameSpeak();
}

void Slig::Motion_24_SpeakGetHim()
{
    Motion_18_GameSpeak();
}

void Slig::Motion_25_SpeakLaugh()
{
    Motion_18_GameSpeak();
}

void Slig::Motion_26_SpeakBullshit1()
{
    Motion_18_GameSpeak();
}

void Slig::Motion_27_SpeakLookOut()
{
    Motion_18_GameSpeak();
}

void Slig::Motion_28_SpeakBullshit2()
{
    Motion_18_GameSpeak();
}

void Slig::Motion_29_SpeakPanic()
{
    Motion_18_GameSpeak();
}

void Slig::Motion_30_SpeakWhat()
{
    Motion_18_GameSpeak();
}

void Slig::Motion_31_SpeakAIFreeze()
{
    Motion_18_GameSpeak();
}

void Slig::Motion_32_Blurgh()
{
    Motion_18_GameSpeak();
}

void Slig::Motion_33_Sleeping()
{
    if (mNextMotion == eSligMotions::Motion_34_SleepingToStand)
    {
        mCurrentMotion = eSligMotions::Motion_34_SleepingToStand;
        mNextMotion = -1;
        return;
    }

    if (sGnFrame % 60)
    {
        if (!((sGnFrame - 20) % 60))
        {
            Slig_SoundEffect(SligSfx::eSnooze2_5);

            if (gMap.Is_Point_In_Current_Camera(
                    mCurrentLevel,
                    mCurrentPath,
                    mXPos,
                    mYPos,
                    0))
            {
                relive_new SnoozeParticle(
                    mXPos
                        + ((GetAnimation().mFlags.Get(AnimFlags::eFlipX)) != 0 ? FP_FromInteger(20) : FP_FromInteger(-20)),
                    mYPos - FP_FromInteger(10),
                    GetAnimation().GetRenderLayer(),
                    GetAnimation().GetSpriteScale());
                return;
            }
        }
    }
    else
    {
        Slig_SoundEffect(SligSfx::eSnooze1_4);

        if (gMap.Is_Point_In_Current_Camera(
                mCurrentLevel,
                mCurrentPath,
                mXPos,
                mYPos,
                0))
        {
            relive_new SnoozeParticle(
                mXPos
                    + ((GetAnimation().mFlags.Get(AnimFlags::eFlipX)) != 0 ? FP_FromInteger(20) : FP_FromInteger(-20)),
                mYPos - FP_FromInteger(10),
                GetAnimation().GetRenderLayer(),
                GetAnimation().GetSpriteScale());
            return;
        }
    }
}

void Slig::Motion_34_SleepingToStand()
{
    // OWI hack - kill all particles, even if they're not ours!
    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        auto pObj = gBaseGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->Type() == ReliveTypes::eSnoozParticle)
        {
            static_cast<SnoozeParticle*>(pObj)->mState = SnoozeParticle::SnoozeParticleState::eBlowingUp_2;
        }
    }

    if (GetAnimation().GetCurrentFrame() >= 2 && GetAnimation().GetCurrentFrame() <= 10)
    {
        Slig_SoundEffect(SligSfx::eToStand_0);
    }

    if (GetAnimation().GetCurrentFrame() == 9)
    {
        Slig_SoundEffect(SligSfx::eWalkingStep_2);
    }

    switch (GetAnimation().GetCurrentFrame())
    {
        case 9:
            if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
            {
                mXPos -= (GetSpriteScale() * FP_FromInteger(7));
            }
            else
            {
                mXPos += (GetSpriteScale() * FP_FromInteger(7));
            }
            break;

        case 10:
            if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
            {
                mXPos -= (GetSpriteScale() * FP_FromInteger(12));
            }
            else
            {
                mXPos += (GetSpriteScale() * FP_FromInteger(12));
            }
            break;

        case 11:
            if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
            {
                mXPos -= (GetSpriteScale() * FP_FromInteger(6));
            }
            else
            {
                mXPos += (GetSpriteScale() * FP_FromInteger(6));
            }
            break;

        default:
            if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
            {
                MusicController::static_PlayMusic(MusicController::MusicTypes::eChase_4, this, 0, 0);
                MainMovement();
            }
            break;
    }
}

void Slig::Motion_35_Knockback()
{
    if (BaseAliveGameObjectCollisionLine)
    {
        SlowDown(FP_FromDouble(0.39));
    }
    else
    {
        Motion_7_Falling();

        if (mCurrentMotion == eSligMotions::Motion_41_LandingSoft)
        {
            mCurrentMotion = eSligMotions::Motion_35_Knockback;
            field_128_timer = sGnFrame + 10;
        }
    }

    EventBroadcast(kEventNoise, this);

    if ((gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarms
         || gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarmsReturn
         || gMap.mCurrentLevel == EReliveLevelIds::eBoardRoom)
        && GetAnimation().GetCurrentFrame() == 4)
    {
        Environment_SFX_42A220(EnvironmentSfx::eHitGroundSoft_6, 80, -200, this);
    }

    if (GetAnimation().mFlags.Get(AnimFlags::eForwardLoopCompleted))
    {
        if (BaseAliveGameObjectCollisionLine)
        {
            mVelX = FP_FromInteger(0);

            if (mHealth > FP_FromInteger(0) && field_128_timer <= static_cast<s32>(sGnFrame) && sActiveHero->mHealth > FP_FromInteger(0))
            {
                mCurrentMotion = eSligMotions::Motion_36_KnockbackToStand;
            }
        }
    }
}

void Slig::Motion_36_KnockbackToStand()
{
    EventBroadcast(kEventNoise, this);

    if (GetAnimation().GetCurrentFrame() >= 2 && GetAnimation().GetCurrentFrame() <= 10)
    {
        Slig_SoundEffect(SligSfx::eToStand_0);
    }

    if (GetAnimation().GetCurrentFrame() == 9)
    {
        Slig_SoundEffect(SligSfx::eWalkingStep_2);
    }

    FP gridSize = {};
    if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
    {
        gridSize = -ScaleToGridSize(GetSpriteScale());
    }
    else
    {
        gridSize = ScaleToGridSize(GetSpriteScale());
    }

    if (!WallHit(GetSpriteScale() * FP_FromInteger(35), gridSize))
    {
        switch (GetAnimation().GetCurrentFrame())
        {
            case 9:
                if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
                {
                    mXPos += (GetSpriteScale() * GetSpriteScale()) * FP_FromInteger(-3);
                }
                else
                {
                    mXPos += (GetSpriteScale() * GetSpriteScale()) * FP_FromInteger(3);
                }
                break;

            case 10:
                if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
                {
                    mXPos += (GetSpriteScale() * GetSpriteScale()) * FP_FromInteger(-13);
                }
                else
                {
                    mXPos += (GetSpriteScale() * GetSpriteScale()) * FP_FromInteger(13);
                }

                break;

            case 11:
                if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
                {
                    mXPos += ((GetSpriteScale() * GetSpriteScale()) * FP_FromInteger(-9));
                }
                else
                {
                    mXPos += ((GetSpriteScale() * GetSpriteScale()) * FP_FromInteger(9));
                }
                break;

            default:
                break;
        }
    }

    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        ToStand();
    }
}

void Slig::Motion_37_Depossessing()
{
    if (sControlledCharacter == this)
    {
        if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
        {
            if (!Input_IsChanting())
            {
                mCurrentMotion = eSligMotions::Motion_17_DepossessingAbort;
            }
        }

        if (!(sGnFrame % 4))
        {
            const FP xRand = FP_FromInteger(Math_RandomRange(-20, 20));
            const FP yRand = FP_FromInteger(Math_RandomRange(20, 50));
            New_TintChant_Particle(
                mXPos + (GetSpriteScale() * xRand),
                mYPos - (GetSpriteScale() * yRand),
                GetSpriteScale(),
                Layer::eLayer_0);
        }

        if (static_cast<s32>(sGnFrame) > field_128_timer)
        {
            BlowToGibs();
        }
    }
}

void Slig::Motion_38_Possess()
{
    if (!BaseAliveGameObjectCollisionLine)
    {
        Motion_7_Falling();
        if (mCurrentMotion != eSligMotions::Motion_38_Possess)
        {
            mCurrentMotion = eSligMotions::Motion_38_Possess;
        }
    }

    if (static_cast<s32>(sGnFrame) >= mExplodeTimer)
    {
        if (GetAnimation().mFlags.Get(AnimFlags::eRender))
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
                GetSpriteScale());

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

            GetAnimation().mFlags.Clear(AnimFlags::eRender);
            mCurrentMotion = eSligMotions::Motion_0_StandIdle;
            mVelY = FP_FromInteger(0);
            mVelX = FP_FromInteger(0);
            mHealth = FP_FromInteger(0);
            MusicController::static_PlayMusic(MusicController::MusicTypes::eType0, this, 0, 0);
            SetUpdateDelay(40);
            field_114_timer = sGnFrame + 60;
            SetBrain(&Slig::Brain_Death_46C3A0);
        }
    }

    if (static_cast<s32>(sGnFrame) > field_154_death_by_being_shot_timer)
    {
        if (mVelX >= FP_FromInteger(0))
        {
            if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
            {
                mCurrentMotion = eSligMotions::Motion_35_Knockback;
                return;
            }
        }
        else if (!GetAnimation().mFlags.Get(AnimFlags::eFlipX))
        {
            mCurrentMotion = eSligMotions::Motion_35_Knockback;
            return;
        }
        mCurrentMotion = eSligMotions::Motion_45_Smash;
    }
}

void Slig::Motion_39_OutToFall()
{
    Motion_7_Falling();

    const FP fallDepth = mYPos - BaseAliveGameObjectLastLineYPos;
    if (fallDepth > FP_FromInteger(16 * 640))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (mCurrentMotion == eSligMotions::Motion_41_LandingSoft)
    {
        if (fallDepth > FP_FromInteger(240))
        {
            BaseAliveGameObjectLastLineYPos += FP_FromInteger(240);
        }

        if (mYPos - BaseAliveGameObjectLastLineYPos > FP_FromInteger(180))
        {
            mCurrentMotion = eSligMotions::Motion_42_LandingFatal;
            field_128_timer = sGnFrame + 30;
            mHealth = FP_FromInteger(0);
            EventBroadcast(kEventMudokonComfort, sActiveHero);
        }
    }
}

void Slig::Motion_40_FallingInitiate()
{
    EventBroadcast(kEventNoise, this);

    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        mCurrentMotion = eSligMotions::Motion_39_OutToFall;
    }

    Motion_7_Falling();
}

void Slig::Motion_41_LandingSoft()
{
    if (!GetAnimation().GetCurrentFrame())
    {
        Environment_SFX_42A220(EnvironmentSfx::eHitGroundSoft_6, 0, 0x7FFF, 0);
    }

    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        Motion_46_ToIdle();
    }
}

void Slig::Motion_42_LandingFatal()
{
    if (GetAnimation().GetCurrentFrame() == 0)
    {
        SND_SEQ_Play_477760(SeqId::eHitBottomOfDeathPit_10, 1, 65, 65);
        SfxPlayMono(relive::SoundEffects::KillEffect, 80);
    }

    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        if (static_cast<s32>(sGnFrame) >= field_128_timer && !BrainIs(&Slig::Brain_Death_46C3A0))
        {
            mHealth = FP_FromInteger(0);
            field_114_timer = sGnFrame + 60;
            SetBrain(&Slig::Brain_Death_46C3A0);
        }
    }
}

void Slig::Motion_43_ShootZ()
{
    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        if (mNextMotion != -1)
        {
            SND_SEQ_PlaySeq_4775A0(SeqId::eHitBottomOfDeathPit_10, 1, 1);
            mCurrentMotion = eSligMotions::Motion_44_ShootZtoStand;
            mNextMotion = -1;
        }
    }
    else if (GetAnimation().GetCurrentFrame() == 7)
    {
        relive_new Bullet(
            this,
            BulletType::eZBullet_2,
            mXPos,
            mYPos - FP_FromInteger(12),
            FP_FromInteger(640),
            GetSpriteScale(),
            field_174_tlv->mData.mNumTimesToShoot - mShootCount - 1);

        New_ShootingZFire_Particle(
            mXPos,
            mYPos - FP_FromInteger(12),
            GetSpriteScale());

        if (GetSpriteScale() == FP_FromDouble(0.5))
        {
            SfxPlayMono(relive::SoundEffects::SligShoot, 85);
        }
        else
        {
            SfxPlayMono(relive::SoundEffects::SligShoot, 0);
        }

        // The doves don't like bullets
        Dove::All_FlyAway();

        EventBroadcast(kEventShooting, this);
        EventBroadcast(kEventLoudNoise, this);
    }
}

void Slig::Motion_44_ShootZtoStand()
{
    Motion_46_ToIdle();
}

void Slig::Motion_45_Smash()
{
    if (BaseAliveGameObjectCollisionLine)
    {
        if (GetAnimation().GetCurrentFrame() == 4)
        {
            Environment_SFX_42A220(EnvironmentSfx::eHitGroundSoft_6, 80, -200, this);
        }
    }
    else
    {
        Motion_7_Falling();

        if (mCurrentMotion != eSligMotions::Motion_45_Smash)
        {
            mCurrentMotion = eSligMotions::Motion_45_Smash;
        }
    }
}

void Slig::Motion_46_ToIdle()
{
    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        ToStand();
    }
}

void Slig::Motion_47_LiftUp()
{
    mCurrentMotion = MoveLift(FP_FromInteger(-4));
}

void Slig::Motion_48_LiftDown()
{
    mCurrentMotion = MoveLift(FP_FromInteger(4));
}

void Slig::Motion_49_LiftGrip()
{
    auto pLiftPoint = static_cast<LiftPoint*>(mLiftPoint);

    pLiftPoint->Move(FP_FromInteger(0), FP_FromInteger(0), 0);
    mVelY = FP_FromInteger(0);

    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        if (Input().IsAnyPressed(sInputKey_Up))
        {
            if (pLiftPoint->OnTopFloor())
            {
                mCurrentMotion = eSligMotions::Motion_51_LiftGripping;
            }
            else
            {
                mCurrentMotion = eSligMotions::Motion_47_LiftUp;
            }
            return;
        }

        if (Input().IsAnyPressed(sInputKey_Down))
        {
            if (pLiftPoint->OnBottomFloor())
            {
                mCurrentMotion = eSligMotions::Motion_51_LiftGripping;
            }
            else
            {
                mCurrentMotion = eSligMotions::Motion_48_LiftDown;
            }
            return;
        }

        mCurrentMotion = eSligMotions::Motion_51_LiftGripping;
    }
}

void Slig::Motion_50_LiftUngrip()
{
    mVelY = FP_FromInteger(0);

    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        ToStand();
    }
}

void Slig::Motion_51_LiftGripping()
{
    CheckPlatformVanished();

    auto pLift = static_cast<LiftPoint*>(mLiftPoint);

    pLift->Move(FP_FromInteger(0), FP_FromInteger(0), 0);
    mVelY = FP_FromInteger(0);

    if (Input().IsAnyPressed(sInputKey_Up))
    {
        if (!pLift->OnTopFloor())
        {
            mCurrentMotion = eSligMotions::Motion_47_LiftUp;
        }
    }
    else
    {
        if (Input().IsAnyPressed(sInputKey_Down))
        {
            if (!pLift->OnBottomFloor())
            {
                mCurrentMotion = eSligMotions::Motion_48_LiftDown;
            }
        }
        else if (pLift->OnAnyFloor())
        {
            mCurrentMotion = eSligMotions::Motion_50_LiftUngrip;
        }
    }
}

void Slig::Motion_52_Beat()
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
            GetAnimation().mFlags.Get(AnimFlags::eFlipX) ? mXPos - kGridSize : mXPos + kGridSize,
            mYPos,
            mXPos,
            mYPos - k2Scaled);

        for (s32 idx = 0; idx < gBaseAliveGameObjects->Size(); idx++)
        {
            auto pObjIter = gBaseAliveGameObjects->ItemAt(idx);

            if (!pObjIter)
            {
                break;
            }
            if (pObjIter != this && pObjIter->Type() == ReliveTypes::eMudokon)
            {
                const PSX_RECT bRect = pObjIter->VGetBoundingRect();

                if (pObjIter->mHealth > FP_FromInteger(0)
                    && PSX_Rects_overlap_no_adjustment(&hitRect, &bRect))
                {
                    pObjIter->VTakeDamage(this);
                    EventBroadcast(kEventNoise, this);
                    SfxPlayMono(relive::SoundEffects::FallingItemHit, 60);
                    return;
                }
            }
        }
    }

    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        mCurrentMotion = eSligMotions::Motion_0_StandIdle;
    }
}

s16 Slig::Brain_SpottedEnemy()
{
    if (gMap.Is_Point_In_Current_Camera(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos,
            0)
        || field_174_tlv->mData.mChaseAbeWhenSpotted == relive::reliveChoice::eNo)
    {
        if (VOnSameYLevel(sControlledCharacter)
            && VIsObj_GettingNear_On_X(sControlledCharacter)
            && VIsObjNearby(ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(3), sControlledCharacter) && !EventGet(kEventResetting))
        {
            ToShoot();
        }
        else if (VOnSameYLevel(sControlledCharacter)
                 && EventGet(kEventAbeOhm)
                 && VIsFacingMe(sControlledCharacter))
        {
            ToShoot();
        }
        else if (field_114_timer > static_cast<s32>(sGnFrame))
        {
            if (sActiveHero->mHealth > FP_FromInteger(0))
            {
                ShouldStillBeAlive();
            }
            else
            {
                ToAbeDead();
            }
        }
        else
        {
            if (VIsFacingMe(sControlledCharacter))
            {
                ToShoot();
            }
            else
            {
                ToTurn();
            }
        }
    }
    else
    {
        ToChase();
    }
    return 302;
}

enum Brain_Paused
{
    eSearchGameEnderController_0 = 0,
    eSayFreezeOrTurnAround_1 = 1,
    eWaitForTurnAround_2 = 2,
    eStartRunning_3 = 3,
    eSetDead_4 = 4
};

s16 Slig::Brain_Paused()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(Options::eDead);
    }

    switch (mBrainSubState)
    {
        case Brain_Paused::eSearchGameEnderController_0:
            break;

        case Brain_Paused::eSayFreezeOrTurnAround_1:
            if (VIsFacingMe(sActiveHero))
            {
                if (mCurrentMotion != eSligMotions::Motion_0_StandIdle)
                {
                    return mBrainSubState;
                }
                mNextMotion = eSligMotions::Motion_31_SpeakAIFreeze;
                return Brain_Paused::eStartRunning_3;
            }
            else
            {
                mNextMotion = eSligMotions::Motion_5_TurnAroundStanding;
                return Brain_Paused::eWaitForTurnAround_2;
            }
            break;

        case Brain_Paused::eWaitForTurnAround_2:
            if (mCurrentMotion != eSligMotions::Motion_5_TurnAroundStanding
                || !GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
            {
                return mBrainSubState;
            }
            return Brain_Paused::eSayFreezeOrTurnAround_1;

        case Brain_Paused::eStartRunning_3:
            if (mCurrentMotion != eSligMotions::Motion_31_SpeakAIFreeze
                || !GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
            {
                return mBrainSubState;
            }
            mNextMotion = eSligMotions::Motion_4_Running;
            return Brain_Paused::eSetDead_4;

        case Brain_Paused::eSetDead_4:
            if (!gMap.Is_Point_In_Current_Camera(
                    mCurrentLevel,
                    mCurrentPath,
                    mXPos,
                    mYPos,
                    0))
            {
                mBaseGameObjectFlags.Set(Options::eDead);
            }
            return mBrainSubState;

        default:
            return mBrainSubState;
    }

    bool found = false;
    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        BaseGameObject* pObjIter = gBaseGameObjects->ItemAt(i);
        if (!pObjIter)
        {
            break;
        }

        if (pObjIter->Type() == ReliveTypes::eGameEnderController)
        {
            found = true;
            break;
        }
    }

    if (!found)
    {
        GameEnderController::CreateGameEnderController();
    }

    mCurrentMotion = eSligMotions::Motion_7_Falling;
    mNextMotion = eSligMotions::Motion_0_StandIdle;
    return Brain_Paused::eSayFreezeOrTurnAround_1;
}

s16 Slig::Brain_EnemyDead()
{
    if (EventGet(kEventDeathReset)
        && !gMap.Is_Point_In_Current_Camera(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos,
            0))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        return 113;
    }

    if (field_114_timer >= static_cast<s32>(sGnFrame))
    {
        ShouldStillBeAlive();
    }
    else
    {
        const auto rnd = Math_NextRandom();

        // Say this often
        if (rnd < 25u)
        {
            mNextMotion = eSligMotions::Motion_26_SpeakBullshit1;
            field_114_timer = sGnFrame + 15;
            return 113;
        }

        // Say this less often
        if (rnd < 50u)
        {
            mNextMotion = eSligMotions::Motion_28_SpeakBullshit2;
            field_114_timer = sGnFrame + 15;
            return 113;
        }

        // And turn even less often
        if (sActiveHero->mHealth > FP_FromInteger(0))
        {
            TurnOrWalk(0);
            return 113;
        }
    }
    return 113;
}

s16 Slig::Brain_KilledEnemy()
{
    if (field_114_timer > static_cast<s32>(sGnFrame))
    {
        ShouldStillBeAlive();
    }
    else
    {
        mNextMotion = eSligMotions::Motion_25_SpeakLaugh;
        field_114_timer = sGnFrame + 45;
        SetBrain(&Slig::Brain_EnemyDead);
    }
    return 114;
}

s16 Slig::Brain_Unknown()
{
    if (!VOnSameYLevel(sControlledCharacter)
        || !VIsFacingMe(sControlledCharacter)
        || IsInInvisibleZone(sControlledCharacter)
        || IsWallBetween(this, sControlledCharacter)
        || !gMap.Is_Point_In_Current_Camera(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos,
            0)
        || (mSpottedPossessedSlig
            && sControlledCharacter->Type() == ReliveTypes::eSlig)
        || EventGet(kEventResetting))
    {
        if (!VOnSameYLevel(
                sControlledCharacter)
            || !VIsFacingMe(sControlledCharacter)
            || IsInInvisibleZone(sControlledCharacter)
            || IsWallBetween(this, sControlledCharacter)
            || !gMap.Is_Point_In_Current_Camera(
                mCurrentLevel,
                mCurrentPath,
                mXPos,
                mYPos,
                0)
            || EventGet(kEventResetting))
        {
            if (sActiveHero->mHealth > FP_FromInteger(0))
            {
                BaseAliveGameObject* pEvent = static_cast<BaseAliveGameObject*>(EventGet(kEventSuspiciousNoise));
                if (!pEvent)
                {
                    pEvent = static_cast<BaseAliveGameObject*>(EventGet(kEventSpeaking));
                }

                if (pEvent && pEvent->GetSpriteScale() == GetSpriteScale() && pEvent != this && field_114_timer <= static_cast<s32>(sGnFrame) && !EventGet(kEventResetting))
                {
                    ToTurn();
                }
                else if (sControlledCharacter->GetSpriteScale() <= GetSpriteScale()
                         || (sControlledCharacter != sActiveHero
                             && sControlledCharacter != gElum)
                         || !VIsFacingMe(sControlledCharacter)
                         || IsInInvisibleZone(sControlledCharacter)
                         || !gMap.Is_Point_In_Current_Camera(
                             mCurrentLevel,
                             mCurrentPath,
                             mXPos,
                             mYPos,
                             0)
                         || IsInZCover(sControlledCharacter)
                         || IsInZCover(this)
                         || EventGet(kEventResetting))
                {
                    ShouldStillBeAlive();
                }
                else
                {
                    ToZShoot();
                }
            }
            else
            {
                ToAbeDead();
            }
        }
        else
        {
            ToShoot();
        }
    }
    else
    {
        RespondToEnemyOrPatrol();
    }
    return 101;
}

void Slig::WakeUp()
{
    mNextMotion = eSligMotions::Motion_34_SleepingToStand;
    SetBrain(&Slig::Brain_WakingUp);
    MusicController::static_PlayMusic(MusicController::MusicTypes::eChase_4, this, 0, 0);
    auto pTlv = static_cast<relive::Path_Slig*>(gMap.TLV_Get_At(
        field_174_tlv->mTopLeftX,
        field_174_tlv->mTopLeftY,
        field_174_tlv->mTopLeftX,
        field_174_tlv->mTopLeftY,
        ReliveTypes::eSlig));
    if (pTlv)
    {
        pTlv->mTlvSpecificMeaning = 1;
    }
}

void Start_Slig_Sounds_Helper(Sound_Ambiance_Array array, CameraPos camPos, u8 ambianceId)
{
    if (!array.mArray[ambianceId].mScopedSeq)
    {
        array.mArray[ambianceId].mScopedSeq = relive_new ScopedSeq(ambianceId, camPos);
    }
}

s16 Slig::Brain_Sleeping()
{
    const auto pEvent = static_cast<BaseAliveGameObject*>(EventGet(kEventNoise));
    if (pEvent)
    {
        if (pEvent->GetSpriteScale() == GetSpriteScale())
        {
            const auto kScaleGrid = ScaleToGridSize(GetSpriteScale());
            const auto wake_up_dist_scaled = kScaleGrid * FP_FromInteger(field_174_tlv->mData.mNoiseWakeUpDistance);
            if (VIsObjNearby(wake_up_dist_scaled, pEvent) && field_114_timer <= static_cast<s32>(sGnFrame) && gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0) && !EventGet(kEventResetting))
            {
                WakeUp();
                return 102;
            }
        }
    }

    if (EventGet(kEventSpeaking) || EventGet(kEventAlarm) || EventGet(kEventLoudNoise))
    {
        if (pEvent != this && field_114_timer <= static_cast<s32>(sGnFrame) && gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0) && !EventGet(kEventResetting))
        {
            WakeUp();
            return 102;
        }
    }

    if (SwitchStates_Get(120) && field_114_timer <= static_cast<s32>(sGnFrame) && gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0) && !EventGet(kEventResetting))
    {
        LOG_WARNING("if this is a custom level consider changing all switch id's with the value 120"
                    " to something else. sleeping sligs will always wake up if this switch id is set.");
        WakeUp();
        return 102;
    }

    ShouldStillBeAlive();

    if (mBaseGameObjectFlags.Get(Options::eDead))
    {
        Start_Slig_sounds(gMap.GetDirection(
                                     mCurrentLevel,
                                     mCurrentPath,
                                     mXPos,
                                     mYPos),
                                 0);
    }
    return 102;
}

s16 Slig::Brain_WakingUp()
{
    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        WaitOrWalk();
    }
    return 0;
}

s16 Slig::Brain_Inactive()
{
    if (field_114_timer > static_cast<s32>(sGnFrame))
    {
        if (sActiveHero->mHealth > FP_FromInteger(0))
        {
            if (!VOnSameYLevel(sControlledCharacter)
                || !VIsFacingMe(sControlledCharacter)
                || !VIsObjNearby(ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(1), sControlledCharacter)
                || IsInInvisibleZone(sControlledCharacter)
                || IsWallBetween(this, sControlledCharacter)
                || EventGet(kEventResetting))
            {
                ShouldStillBeAlive();
            }
            else
            {
                ToShoot();
            }
        }
        else
        {
            ToAbeDead();
        }
    }
    else
    {
        WaitOrWalk();
    }
    return 103;
}

s16 Slig::Brain_Possessed()
{
    switch (mBrainSubState)
    {
        case Brain_Possessed::eStartPossession_0:
            Slig_GameSpeak_SFX(SligSpeak::eHelp_10, 0, mGameSpeakPitchMin, this);
            mBrainSubState = Brain_Possessed::ePossessionShaking_1;
            mHealth = FP_FromInteger(0);
            mCurrentMotion = eSligMotions::Motion_38_Possess;
            field_114_timer = sGnFrame + 30;
            mExplodeTimer = sGnFrame + 1000;
            field_154_death_by_being_shot_timer = sGnFrame + 1000;
            return mBrainSubState;

        case Brain_Possessed::ePossessionShaking_1:
            if (static_cast<s32>(sGnFrame) >= field_114_timer)
            {
                mBrainSubState = Brain_Possessed::ePossessionInactive_2;
                field_114_timer = sGnFrame + 20;
                mCurrentMotion = eSligMotions::Motion_0_StandIdle;
                return mBrainSubState;
            }
            break;

        case Brain_Possessed::ePossessionInactive_2:
            if (static_cast<s32>(sGnFrame) >= field_114_timer)
            {
                if (Math_NextRandom() & 1)
                {
                    field_114_timer = sGnFrame + 20;
                    mCurrentMotion = eSligMotions::Motion_22_SpeakHi;
                    Slig_GameSpeak_SFX(SligSpeak::eHi_0, 0, mGameSpeakPitchMin, this);
                }
                else
                {
                    field_114_timer = sGnFrame + 20;
                    mCurrentMotion = eSligMotions::Motion_25_SpeakLaugh;
                    Slig_GameSpeak_SFX(SligSpeak::eLaugh_7, 0, mGameSpeakPitchMin, this);
                }
                mBrainSubState = Brain_Possessed::ePossessionSpeak_3;
                return mBrainSubState;
            }
            break;

        case Brain_Possessed::ePossessionSpeak_3:
            if (EventGet(kEventDeathReset))
            {
                if (sControlledCharacter != this)
                {
                    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                }
            }

            if (static_cast<s32>(sGnFrame) >= field_114_timer)
            {
                mBrainSubState = Brain_Possessed::eControlledByPlayer_4;
                mHealth = FP_FromInteger(1);
                return mBrainSubState;
            }
            break;

        case Brain_Possessed::eControlledByPlayer_4:
            if (EventGet(kEventDeathReset))
            {
                if (sControlledCharacter != this)
                {
                    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                }
            }

            if (mHealth <= FP_FromInteger(0))
            {
                SetBrain(&Slig::Brain_Death_46C3A0);
                field_114_timer = sGnFrame + 60;
                return mBrainSubState;
            }
            break;

        default:
            return mBrainSubState;
    }

    if (field_106_shot)
    {
        BlowToGibs();
    }

    return mBrainSubState;
}

s16 Slig::Brain_Death_46C3A0()
{
    if (GetAnimation().mFlags.Get(AnimFlags::eRender) && field_114_timer < static_cast<s32>(sGnFrame) + 30)
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
    else if (!GetAnimation().mFlags.Get(AnimFlags::eRender))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (sControlledCharacter == this)
    {
        if (field_114_timer < static_cast<s32>(sGnFrame))
        {
            sControlledCharacter = sActiveHero;
            MusicController::static_PlayMusic(MusicController::MusicTypes::eType0, this, 0, 0);
            gMap.SetActiveCam(
                mAbeLevel,
                mAbePath,
                mAbeCamera,
                CameraSwapEffects::eInstantChange_0,
                0,
                0);
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
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        }
    }

    if (GetSpriteScale() < FP_FromInteger(0))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    return 116;
}

s16 Slig::Brain_DeathDropDeath()
{
    switch (mBrainSubState)
    {
        case Brain_DeathDropDeath::eSayHelpOnce_0:
            Slig_GameSpeak_SFX(SligSpeak::eHelp_10, 0, mGameSpeakPitchMin, this);
            field_114_timer = sGnFrame + 60;
            return Brain_DeathDropDeath::eSayHelpAndDie_1;

        case Brain_DeathDropDeath::eSayHelpAndDie_1:
        {
            if (static_cast<s32>(sGnFrame) < field_114_timer)
            {
                if (!((field_114_timer - sGnFrame) % 15))
                {
                    Slig_GameSpeak_SFX(
                        SligSpeak::eHelp_10,
                        static_cast<s16>(2 * ((field_114_timer & 0xFFFF) - sGnFrame)),
                        mGameSpeakPitchMin,
                        this);
                }

                if (static_cast<s32>(sGnFrame) == (field_114_timer - 6))
                {
                    SND_SEQ_Play_477760(SeqId::eHitBottomOfDeathPit_10, 1, 65, 65);
                }
                return mBrainSubState;
            }

            Environment_SFX_42A220(EnvironmentSfx::eFallingDeathScreamHitGround_15, 0, 32767, this);

            relive_new ScreenShake(0);
            field_114_timer = sGnFrame + 30;
            return Brain_DeathDropDeath::eSwitchCamToAbe_2;
        }

        case Brain_DeathDropDeath::eSwitchCamToAbe_2:
            if (static_cast<s32>(sGnFrame) > field_114_timer)
            {
                if (sControlledCharacter == this)
                {
                    MusicController::static_PlayMusic(MusicController::MusicTypes::eType0, this, 0, 0);
                    sControlledCharacter = sActiveHero;
                    gMap.SetActiveCam(mAbeLevel, mAbePath, mAbeCamera, CameraSwapEffects::eInstantChange_0, 0, 0);
                }
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            }
            return mBrainSubState;

        default:
            return mBrainSubState;
    }
}

s16 Slig::Brain_ReturnControlToAbeAndDie()
{
    if (sControlledCharacter == this)
    {
        sControlledCharacter = sActiveHero;
        MusicController::static_PlayMusic(MusicController::MusicTypes::eType0, this, 0, 0);
        gMap.SetActiveCam(
            mAbeLevel,
            mAbePath,
            mAbeCamera,
            CameraSwapEffects::eInstantChange_0,
            0,
            0);
    }
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    return 117;
}

s16 Slig::Brain_PanicTurning()
{
    if (EventGet(kEventDeathReset)
        && !gMap.Is_Point_In_Current_Camera(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos,
            0))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        return 107;
    }
    if (mCurrentMotion != eSligMotions::Motion_5_TurnAroundStanding
        || !(GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame)))
    {
        if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
        {
            mNextMotion = eSligMotions::Motion_5_TurnAroundStanding;
            return 107;
        }
        if (GetAnimation().GetCurrentFrame() != 4)
        {
            ShouldStillBeAlive();
            return 107;
        }
        if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
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

        if (sControlledCharacter->Type() != ReliveTypes::eSlig && !IsInInvisibleZone(sControlledCharacter))
        {
            if (charRect.x <= bRect.w && charRect.w >= bRect.x && charRect.h >= bRect.y && charRect.y <= bRect.h)
            {
                GetAnimation().mFlags.Toggle(AnimFlags::eFlipX);
                return 107;
            }
        }
        ShouldStillBeAlive();
        return 107;
    }

    if (field_114_timer > static_cast<s32>(sGnFrame))
    {
        mNextMotion = eSligMotions::Motion_4_Running;
        SetBrain(&Slig::Brain_PanicRunning);
        Brain_PanicRunning();
        MusicController::static_PlayMusic(MusicController::MusicTypes::eSlogChase_5, this, 0, 0);
    }
    else
    {
        WaitOrWalk();
    }
    return 107;
}

s16 Slig::Brain_PanicRunning()
{
    if ((mVelX > FP_FromInteger(4) && (ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(4)) + mXPos > FP_FromInteger(field_13C_zone_rect.w)) || (mVelX < FP_FromInteger(-4) && (mXPos - (ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(4))) < FP_FromInteger(field_13C_zone_rect.x))
        || HandleEnemyStopper(4))
    {
        mNextMotion = eSligMotions::Motion_5_TurnAroundStanding;
        SetBrain(&Slig::Brain_PanicTurning);
    }
    else if (mCurrentMotion || mNextMotion != -1)
    {
        if (VOnSameYLevel(sControlledCharacter)
            && VIsFacingMe(sControlledCharacter)
            && !IsInInvisibleZone(sControlledCharacter)
            && !IsWallBetween(this, sControlledCharacter)
            && gMap.Is_Point_In_Current_Camera(
                mCurrentLevel,
                mCurrentPath,
                mXPos,
                mYPos,
                0)
            && gMap.Is_Point_In_Current_Camera(
                mCurrentLevel,
                mCurrentPath,
                mXPos,
                mYPos,
                0)
            && !EventGet(kEventResetting))
        {
            ToShoot();
        }
        else if (sActiveHero->mHealth > FP_FromInteger(0))
        {
            if (field_114_timer > static_cast<s32>(sGnFrame))
            {
                ShouldStillBeAlive();
            }
            else
            {
                WaitOrWalk();
            }
        }
        else
        {
            ToAbeDead();
        }
    }
    else
    {
        mNextMotion = eSligMotions::Motion_4_Running;
        SetBrain(&Slig::Brain_PanicRunning);
        Brain_PanicRunning();
        MusicController::static_PlayMusic(MusicController::MusicTypes::eSlogChase_5, this, 0, 0);
    }
    return 109;
}

s16 Slig::Brain_PanicYelling()
{
    if (mCurrentMotion == eSligMotions::Motion_29_SpeakPanic
        && GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        EventBroadcast(kEventAlarm, this);

        const bool bFlipX = GetAnimation().mFlags.Get(AnimFlags::eFlipX);
        field_114_timer = sGnFrame + field_174_tlv->mData.mPanicTimeout;
        if ((!bFlipX
             || mXPos >= FP_FromInteger((field_13C_zone_rect.x + field_13C_zone_rect.w)
                                                / 2))
            && (bFlipX
                || mXPos <= FP_FromInteger((field_13C_zone_rect.x + field_13C_zone_rect.w)
                                                   / 2)))
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

s16 Slig::Brain_Chasing()
{
    if (gMap.Is_Point_In_Current_Camera(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos,
            0)
        && VOnSameYLevel(sControlledCharacter)
        && VIsFacingMe(sControlledCharacter)
        && !IsInInvisibleZone(sControlledCharacter)
        && !IsWallBetween(this, sControlledCharacter)
        && !InAnyWellRenderLayer(sControlledCharacter)
        && !EventGet(kEventResetting))
    {
        field_20C_force_alive_state = 0;
        RespondToEnemyOrPatrol();
        return 118;
    }

    if (HandleEnemyStopper(4))
    {
        mNextMotion = eSligMotions::Motion_0_StandIdle;
        SetBrain(&Slig::Brain_Idle);
        field_114_timer = sGnFrame + 1;
        return 118;
    }

    if (mCurrentPath != gMap.mCurrentPath
        || mCurrentLevel != gMap.mCurrentLevel
        || (EventGet(kEventDeathReset)
            && !gMap.Is_Point_In_Current_Camera(
                mCurrentLevel,
                mCurrentPath,
                mXPos,
                mYPos,
                0)))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
    else if (gMap.Is_Point_In_Current_Camera(
                 mCurrentLevel,
                 mCurrentPath,
                 mXPos,
                 mYPos,
                 0))
    {
        SetBrain(&Slig::Brain_StopChasing);
        field_114_timer = sGnFrame + field_174_tlv->mData.mStopChaseDelay;
    }
    return 118;
}

s16 Slig::Brain_StopChasing()
{
    if (field_114_timer > static_cast<s32>(sGnFrame))
    {
        if (HandleEnemyStopper(4))
        {
            mNextMotion = eSligMotions::Motion_0_StandIdle;
            SetBrain(&Slig::Brain_Idle);
            field_114_timer = sGnFrame + 1;
        }
    }
    else
    {
        field_20C_force_alive_state = 0;
        ToTurn();
    }
    return 119;
}

s16 Slig::Brain_StartChasing()
{
    if (field_114_timer > static_cast<s32>(sGnFrame))
    {
        if (gMap.Is_Point_In_Current_Camera(
                mCurrentLevel,
                mCurrentPath,
                mXPos,
                mYPos,
                0))
        {
            field_20C_force_alive_state = 0;
            ToShoot();
        }
    }
    else
    {
        if (mCurrentPath != gMap.mCurrentPath
            || mCurrentLevel != gMap.mCurrentLevel)
        {
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        }

        if (!VIsFacingMe(sControlledCharacter))
        {
            GetAnimation().mFlags.Toggle(AnimFlags::eFlipX);
        }

        field_20C_force_alive_state = 1;
        mNextMotion = eSligMotions::Motion_4_Running;
        SetBrain(&Slig::Brain_Chasing);
        field_114_timer = field_174_tlv->mData.mPauseTime;
    }
    return 122;
}

s16 Slig::Brain_Idle()
{
    if (EventGet(kEventAbeOhm) && !EventGet(kEventResetting))
    {
        ToPanic();
        return 104;
    }

    if (VOnSameYLevel(sControlledCharacter)
        && VIsFacingMe(sControlledCharacter)
        && !IsInInvisibleZone(sControlledCharacter)
        && !IsWallBetween(this, sControlledCharacter)
        && (!mSpottedPossessedSlig
            || sControlledCharacter->Type() != ReliveTypes::eSlig)
        && !IsAbeEnteringDoor(sControlledCharacter)
        && !EventGet(kEventResetting)
        && gMap.Is_Point_In_Current_Camera(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos,
            0))
    {
        Slig::RespondToEnemyOrPatrol();
        return 104;
    }
    if (sActiveHero->mHealth <= FP_FromInteger(0))
    {
        ToAbeDead();
        return 104;
    }
    auto pEvent = static_cast<BaseAliveGameObject*>(EventGet(kEventSuspiciousNoise));

    if (!pEvent)
    {
        pEvent = static_cast<BaseAliveGameObject*>(EventGet(kEventSpeaking));
    }

    if (!pEvent || pEvent->GetSpriteScale() != GetSpriteScale() || pEvent == this || !gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0) || EventGet(kEventResetting))
    {
        if (sControlledCharacter->GetSpriteScale() > GetSpriteScale()
            && (sControlledCharacter == sActiveHero
                || sControlledCharacter == gElum))
        {
            if (VIsFacingMe(sControlledCharacter) && !IsInInvisibleZone(sControlledCharacter) && gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0) && !IsInZCover(static_cast<Slig*>(sControlledCharacter)) && !IsInZCover(this) && !EventGet(kEventResetting))
            {
                ToZShoot();
                return 104;
            }
        }
        if (sControlledCharacter->Type() == ReliveTypes::eSlig)
        {
            if (mCurrentMotion)
            {
                ShouldStillBeAlive();
                return 104;
            }
            if (gEventSystem->field_10_last_event == GameSpeakEvents::eUnknown_29)
            {
                auto pTlv = static_cast<relive::Path_Lever*>(gMap.TLV_Get_At(
                    FP_GetExponent(mXPos),
                    FP_GetExponent(mYPos),
                    FP_GetExponent(mXPos),
                    FP_GetExponent(mYPos),
                    ReliveTypes::eLever));
                if (pTlv)
                {
                    FP kScaleGrid = ScaleToGridSize(GetSpriteScale());
                    if ((FP_FromInteger(FP_GetExponent(mXPos) - pTlv->mTopLeftX) < kScaleGrid && !(GetAnimation().mFlags.Get(AnimFlags::eFlipX))) || (FP_FromInteger(pTlv->mBottomRightX - FP_GetExponent(mXPos)) < kScaleGrid && GetAnimation().mFlags.Get(AnimFlags::eFlipX)))
                    {
                        auto pSwitch = static_cast<Lever*>(FindObjectOfType(
                            ReliveTypes::eLever,
                            mXPos + kScaleGrid,
                            mYPos - FP_FromInteger(5)));
                        if (pSwitch)
                        {
                            pSwitch->VPull(mXPos < pSwitch->mXPos ? TRUE : FALSE);
                        }
                        mNextMotion = eSligMotions::State_46_PullLever_46A590;
                        return 104;
                    }
                }
            }
        }
        if (mCurrentMotion == eSligMotions::Motion_0_StandIdle && field_114_timer <= static_cast<s32>(sGnFrame))
        {
            mNextMotion = eSligMotions::Motion_5_TurnAroundStanding;
            SetBrain(&Slig::Brain_Turning);
            return 104;
        }
        ShouldStillBeAlive();
        return 104;
    }
    if (pEvent != sControlledCharacter)
    {
        if (Math_NextRandom() >= 192u)
        {
            return 104;
        }
    }
    if (VIsFacingMe(sControlledCharacter))
    {
        if (!EventGet(kEventSpeaking) || IsInInvisibleZone(sControlledCharacter))
        {
            mNextMotion = eSligMotions::Motion_0_StandIdle;
            field_258_next_gamespeak_motion = eSligMotions::Motion_30_SpeakWhat;
            SetBrain(&Slig::Brain_GetAlerted);
            field_114_timer = static_cast<s32>(sGnFrame) + field_174_tlv->mData.mAlertedListenTime;
            return 104;
        }
        GameSpeakResponse();
        field_114_timer = static_cast<s32>(sGnFrame) + 20;
    }
    else
    {
        if (!EventGet(kEventSpeaking) || IsInInvisibleZone(sControlledCharacter))
        {
            mNextMotion = eSligMotions::Motion_5_TurnAroundStanding;
            SetBrain(&Slig::Brain_GetAlertedTurn);
            return 104;
        }
        GameSpeakResponse();
        field_114_timer = static_cast<s32>(sGnFrame) + 20;
    }
    if (!VIsFacingMe(sControlledCharacter))
    {
        mNextMotion = eSligMotions::Motion_5_TurnAroundStanding;
    }
    SetBrain(&Slig::Brain_Discussion);
    return 104;
}

s16 Slig::Brain_Turning()
{
    if (EventGet(kEventDeathReset)
        && !gMap.Is_Point_In_Current_Camera(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos,
            0))
    {
        mBaseGameObjectFlags.Set(Options::eDead);
        return 106;
    }
    if (mCurrentMotion == eSligMotions::Motion_5_TurnAroundStanding
        && GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        WaitOrWalk();
        return 106;
    }
    if (GetAnimation().GetCurrentFrame() == 4)
    {
        if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
        {
            if (sControlledCharacter->mVelX >= FP_FromInteger(0) && (sControlledCharacter->mVelX != FP_FromInteger(0) || sControlledCharacter->mXPos >= mXPos))
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

        const PSX_RECT hitRect = VGetBoundingRect();

        if (!Slig::IsInInvisibleZone(sControlledCharacter)
            && !Slig::IsWallBetween(this, sControlledCharacter))
        {
            const PSX_RECT bRect = sControlledCharacter->VGetBoundingRect();

            if (sControlledCharacter->mHealth > FP_FromInteger(0) && PSX_Rects_overlap_no_adjustment(&hitRect, &bRect) && sControlledCharacter->Type() != ReliveTypes::eSlig)
            {
                GetAnimation().mFlags.Toggle(AnimFlags::eFlipX);
                return 106;
            }
        }
    }

    ShouldStillBeAlive();
    return 106;
}

s16 Slig::Brain_Walking()
{
    if (unused_9F11BC) // NOTE: This is never set to true, so this branch is never executed.
    {
        ToPanicRunning();
        return 108;
    }

    FP kScaleGrid = ScaleToGridSize(GetSpriteScale());
    FP kScaleGrid2 = kScaleGrid * FP_FromInteger(2);
    if (mVelX > FP_FromInteger(0))
    {
        if ((kScaleGrid2 + mXPos) >= FP_FromInteger(field_13C_zone_rect.w))
        {
            PauseALittle();
            return 108;
        }
    }
    if (mVelX < FP_FromInteger(0))
    {
        if ((mXPos - kScaleGrid2) <= FP_FromInteger(field_13C_zone_rect.x))
        {
            PauseALittle();
            return 108;
        }
    }
    if (mVelX == FP_FromInteger(0) && mCurrentMotion == eSligMotions::Motion_0_StandIdle && mNextMotion != eSligMotions::Motion_2_Walking)
    {
        PauseALittle();
        return 108;
    }
    if (HandleEnemyStopper(2))
    {
        PauseALittle();
        return 108;
    }

    if (VOnSameYLevel(sControlledCharacter) && VIsFacingMe(sControlledCharacter) && !IsInInvisibleZone(sControlledCharacter) && !IsWallBetween(this, sControlledCharacter) && gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0))
    {
        if (!mSpottedPossessedSlig
            || sControlledCharacter->Type() != ReliveTypes::eSlig)
        {
            if (!IsAbeEnteringDoor(sControlledCharacter) && !EventGet(kEventResetting))
            {
                RespondToEnemyOrPatrol();
                return 108;
            }
        }
    }

    if (VOnSameYLevel(sControlledCharacter) && VIsFacingMe(sControlledCharacter) && !IsWallBetween(this, sControlledCharacter) && EventGet(kEventAbeOhm) && gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0) && !EventGet(kEventResetting))
    {
        ToShoot();
        return 108;
    }
    if (EventGet(kEventAbeOhm) && !EventGet(kEventResetting))
    {
        ToPanic();
        return 108;
    }
    if (sActiveHero->mHealth <= FP_FromInteger(0))
    {
        ToAbeDead();
        return 108;
    }
    auto pEvent = static_cast<BaseAliveGameObject*>(EventGet(kEventSuspiciousNoise));
    if (!pEvent)
    {
        pEvent = static_cast<BaseAliveGameObject*>(EventGet(kEventSpeaking));
    }
    if (pEvent && pEvent->GetSpriteScale() == GetSpriteScale() && pEvent != this && gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0) && !EventGet(kEventResetting))
    {
        if (VIsFacingMe(sControlledCharacter))
        {
            if (!EventGet(kEventSpeaking) || IsInInvisibleZone(sControlledCharacter))
            {
                mNextMotion = eSligMotions::Motion_0_StandIdle;
                field_258_next_gamespeak_motion = eSligMotions::Motion_30_SpeakWhat;
                SetBrain(&Slig::Brain_GetAlerted);
                field_114_timer = static_cast<s32>(sGnFrame) + field_174_tlv->mData.mAlertedListenTime;
                return 108;
            }
        }
        else
        {
            if (!EventGet(kEventSpeaking) || IsInInvisibleZone(sControlledCharacter))
            {
                mNextMotion = eSligMotions::Motion_5_TurnAroundStanding;
                SetBrain(&Slig::Brain_GetAlertedTurn);
                return 108;
            }
        }
        GameSpeakResponse();
        field_114_timer = sGnFrame + 20;
        if (!VIsFacingMe(sControlledCharacter))
        {
            mNextMotion = eSligMotions::Motion_5_TurnAroundStanding;
        }
        SetBrain(&Slig::Brain_Discussion);
        return 108;
    }
    else if (sControlledCharacter->GetSpriteScale() <= GetSpriteScale()
             || (sControlledCharacter != sActiveHero
                 && sControlledCharacter != gElum)
             || !VIsFacingMe(sControlledCharacter)
             || IsInInvisibleZone(sControlledCharacter)
             || !gMap.Is_Point_In_Current_Camera(
                 mCurrentLevel,
                 mCurrentPath,
                 mXPos,
                 mYPos,
                 0)
             || IsInZCover(sControlledCharacter)
             || IsInZCover(this)
             || EventGet(kEventResetting))
    {
        if (Math_NextRandom() < field_174_tlv->mData.mPercentBeatMud
            && FindBeatTarget(52, 2))
        {
            mNextMotion = eSligMotions::Motion_0_StandIdle;
            SetBrain(&Slig::Brain_StoppingNextToMudokon);
            field_114_timer = static_cast<s32>(sGnFrame) + 30;
            return 108;
        }
        else
        {
            ShouldStillBeAlive();
            return 108;
        }
    }
    else
    {
        ToZShoot();
        return 108;
    }
}

s16 Slig::Brain_GetAlertedTurn()
{
    if (mCurrentMotion == eSligMotions::Motion_5_TurnAroundStanding && GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        if (!EventGet(kEventSpeaking) || IsInInvisibleZone(sControlledCharacter))
        {
            mNextMotion = eSligMotions::Motion_0_StandIdle;
            field_258_next_gamespeak_motion = eSligMotions::Motion_30_SpeakWhat;
            SetBrain(&Slig::Brain_GetAlerted);
            field_114_timer = sGnFrame + field_174_tlv->mData.mAlertedListenTime;
        }
        else
        {
            GameSpeakResponse();
            field_114_timer = sGnFrame + 20;

            if (!VIsFacingMe(sControlledCharacter))
            {
                mNextMotion = eSligMotions::Motion_5_TurnAroundStanding;
            }

            SetBrain(&Slig::Brain_Discussion);
        }

        return 123;
    }

    if (GetAnimation().GetCurrentFrame() == 4)
    {
        bool tryTurningToPlayer = true;
        if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
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
            if (!IsInInvisibleZone(sControlledCharacter))
            {
                const PSX_RECT bRect = VGetBoundingRect();
                const PSX_RECT bRectChar = sControlledCharacter->VGetBoundingRect();

                if (PSX_Rects_overlap_no_adjustment(&bRectChar, &bRect))
                {
                    GetAnimation().mFlags.Toggle(AnimFlags::eFlipX);
                    return 123;
                }
            }
        }
    }

    if (mCurrentMotion != eSligMotions::Motion_0_StandIdle)
    {
        ShouldStillBeAlive();
    }
    else
    {
        mNextMotion = eSligMotions::Motion_5_TurnAroundStanding;
    }
    return 123;
}

s16 Slig::Brain_GetAlerted()
{
    if (field_114_timer == field_174_tlv->mData.mAlertedListenTime + static_cast<s32>(sGnFrame) - 2 && Math_RandomRange(0, 100) < field_174_tlv->mData.mPercentSayWhat)
    {
        mNextMotion = eSligMotions::Motion_30_SpeakWhat;
    }

    if (!VOnSameYLevel(sControlledCharacter)
        || !VIsFacingMe(sControlledCharacter)
        || IsInInvisibleZone(sControlledCharacter)
        || IsWallBetween(this, sControlledCharacter)
        || !gMap.Is_Point_In_Current_Camera(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos,
            0)
        || (sControlledCharacter->Type() == ReliveTypes::eSlig && mSpottedPossessedSlig)
        || IsAbeEnteringDoor(sControlledCharacter)
        || EventGet(kEventResetting))
    {
        if (!EventGet(kEventAbeOhm) || EventGet(kEventResetting))
        {
            BaseAliveGameObject* pEvent = static_cast<BaseAliveGameObject*>(EventGet(kEventSuspiciousNoise));
            if (!pEvent)
            {
                pEvent = static_cast<BaseAliveGameObject*>(EventGet(kEventSpeaking));
            }

            if (pEvent && (pEvent == sControlledCharacter || pEvent->Type() == ReliveTypes::eMudokon)
                && VOnSameYLevel(pEvent)
                && VIsFacingMe(pEvent)
                && gMap.Is_Point_In_Current_Camera(
                    mCurrentLevel,
                    mCurrentPath,
                    mXPos,
                    mYPos,
                    0)
                && !EventGet(kEventResetting))
            {
                ToShoot();
            }
            else
            {
                if (pEvent && (pEvent == sControlledCharacter || pEvent->Type() != ReliveTypes::eSlig)
                    && !VIsFacingMe(pEvent)
                    && gMap.Is_Point_In_Current_Camera(
                        mCurrentLevel,
                        mCurrentPath,
                        mXPos,
                        mYPos,
                        0)
                    && !EventGet(kEventResetting))
                {
                    if (!EventGet(kEventSpeaking) || IsInInvisibleZone(sControlledCharacter))
                    {
                        mNextMotion = eSligMotions::Motion_5_TurnAroundStanding;
                        SetBrain(&Slig::Brain_GetAlertedTurn);
                    }
                    else
                    {
                        GameSpeakResponse();
                        field_114_timer = sGnFrame + 20;
                        if (!VIsFacingMe(sControlledCharacter))
                        {
                            mNextMotion = eSligMotions::Motion_5_TurnAroundStanding;
                        }

                        SetBrain(&Slig::Brain_Discussion);
                    }
                }
                else
                {
                    if (field_114_timer > static_cast<s32>(sGnFrame))
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
        else
        {
            ToPanic();
        }
    }
    else
    {
        RespondToEnemyOrPatrol();
    }
    return 124;
}

s16 Slig::Brain_StoppingNextToMudokon()
{
    if (mCurrentMotion != eSligMotions::Motion_0_StandIdle)
    {
        return 128;
    }

    //TODO OG BUG: Sligs beat up dead muds, fix from AE:
    //BaseAliveGameObject* pBeatTarget = FindBeatTarget_4BD070(static_cast<s32>(AOTypes::eRingOrLiftMud_81), 1);
    //if (!pBeatTarget || pBeatTarget->mHealth <= FP_FromInteger(0))
    if (!FindBeatTarget(static_cast<s32>(AOTypes::eRingOrLiftMud_52), 1))
    {
        WaitOrWalk();
        return 128;
    }

    if (Math_NextRandom() >= 100u)
    {
        if (Math_NextRandom() & 1)
        {
            mNextMotion = eSligMotions::Motion_26_SpeakBullshit1;
        }
        else
        {
            mNextMotion = eSligMotions::Motion_28_SpeakBullshit2;
        }
    }
    else
    {
        mNextMotion = eSligMotions::Motion_52_Beat;
        SetBrain(&Slig::Brain_BeatingUp);
    }

    return 128;
}

s16 Slig::Brain_BeatingUp()
{
    if (mCurrentMotion != eSligMotions::Motion_0_StandIdle || field_114_timer >= static_cast<s32>(sGnFrame))
    {
        return 129;
    }

    if (Math_NextRandom() < 100u)
    {
        mNextMotion = eSligMotions::Motion_52_Beat;
        return 129;
    }

    if (Math_NextRandom() < 64u)
    {
        mNextMotion = eSligMotions::Motion_26_SpeakBullshit1;
        return 129;
    }

    if (Math_NextRandom() < 64u)
    {
        mNextMotion = eSligMotions::Motion_28_SpeakBullshit2;
        return 129;
    }

    WaitOrWalk();
    return 129;
}

s16 Slig::Brain_Discussion()
{
    if (mCurrentMotion == eSligMotions::Motion_0_StandIdle && field_114_timer == static_cast<s32>(sGnFrame))
    {
        mNextMotion = static_cast<s16>(field_258_next_gamespeak_motion);
    }

    if (field_114_timer < static_cast<s32>(sGnFrame) - 5)
    {
        WaitOrWalk();
    }

    return 128;
}

enum Brain_ChaseAndDisappear
{
    eSummoned_0 = 0,
    eRunning_1 = 1,
    eReachedDestination_2 = 2
};

s16 Slig::Brain_ChaseAndDisappear()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (mBrainSubState == Brain_ChaseAndDisappear::eSummoned_0)
    {
        if (gNumCamSwappers > 0 || sActiveHero->field_2A8_flags.Get(Flags_2A8::e2A8_Bit6_bShrivel))
        {
            return mBrainSubState;
        }
        mCurrentMotion = eSligMotions::Motion_0_StandIdle;
        field_114_timer = sGnFrame + field_130_game_ender_pause_time;
        return Brain_ChaseAndDisappear::eRunning_1;
    }
    else if (mBrainSubState == Brain_ChaseAndDisappear::eRunning_1)
    {
        if (static_cast<s32>(sGnFrame) < field_114_timer)
        {
            return mBrainSubState;
        }
        mNextMotion = eSligMotions::Motion_4_Running;
        return Brain_ChaseAndDisappear::eReachedDestination_2;
    }
    else
    {
        if (mBrainSubState == Brain_ChaseAndDisappear::eReachedDestination_2
            && gMap.Is_Point_In_Current_Camera(
                mCurrentLevel,
                mCurrentPath,
                mXPos,
                mYPos,
                0))
        {
            return mBrainSubState;
        }
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        mHealth = FP_FromInteger(0);
        return mBrainSubState;
    }
}

s16 Slig::Brain_Shooting()
{
    if (mCurrentMotion == eSligMotions::Motion_6_Shoot
        && GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        mShootCount++;

        if (mShootCount < field_174_tlv->mData.mNumTimesToShoot
            || sActiveHero->mCurrentMotion == eAbeMotions::Motion_137_ElumUnmountBegin
            || sActiveHero->mCurrentMotion == eAbeMotions::Motion_139_ElumMountBegin
            || sActiveHero->mCurrentMotion == eAbeMotions::Motion_138_ElumUnmountEnd
            || sActiveHero->mCurrentMotion == eAbeMotions::Motion_136_ElumMountEnd)
        {
            mNextMotion = eSligMotions::Motion_6_Shoot;
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

        if (!VOnSameYLevel(sControlledCharacter)
            || !VIsFacingMe(sControlledCharacter)
            || IsInInvisibleZone(sControlledCharacter)
            || IsWallBetween(this, sControlledCharacter)
            || !gMap.Is_Point_In_Current_Camera(
                mCurrentLevel,
                mCurrentPath,
                mXPos,
                mYPos,
                0)
            || !gMap.Is_Point_In_Current_Camera(
                mCurrentLevel,
                mCurrentPath,
                mXPos,
                mYPos,
                0)
            || EventGet(kEventResetting))
        {
            PauseALittle();
            return 111;
        }

        if (!VIsFacingMe(sControlledCharacter))
        {
            ToTurn();
            return 111;
        }

        if (!gMap.Is_Point_In_Current_Camera(
                mCurrentLevel,
                mCurrentPath,
                mXPos,
                mYPos,
                0)
            && field_174_tlv->mData.mChaseAbeWhenSpotted == relive::reliveChoice::eYes)
        {
            ToChase();
            return 111;
        }
    }

    ShouldStillBeAlive();
    return 111;
}

s16 Slig::Brain_ZSpottedEnemy()
{
    if (field_114_timer > static_cast<s32>(sGnFrame))
    {
        return 126;
    }
    mNextMotion = eSligMotions::Motion_43_ShootZ;
    SetBrain(&Slig::Brain_ZShooting);
    return 126;
}


s16 Slig::Brain_ZShooting()
{
    if (mCurrentMotion != eSligMotions::Motion_43_ShootZ || !GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        ShouldStillBeAlive();
        return 127;
    }

    mShootCount++;

    if (mShootCount < field_174_tlv->mData.mNumTimesToShoot)
    {
        return 127;
    }

    if (sActiveHero->mCurrentMotion == eAbeMotions::Motion_138_ElumUnmountEnd
        || sActiveHero->mCurrentMotion == eAbeMotions::Motion_136_ElumMountEnd
        || sActiveHero->mCurrentMotion == eAbeMotions::Motion_137_ElumUnmountBegin
        || sActiveHero->mCurrentMotion == eAbeMotions::Motion_139_ElumMountBegin)
    {
        return 127;
    }

    mShootCount = 0;

    if (sActiveHero->mHealth > FP_FromInteger(0))
    {
        WaitOrWalk();
    }
    else
    {
        ToAbeDead();
    }
    return 127;
}

void Slig::BlowToGibs()
{
    relive_new Gibs(
        GibType::Slig_1,
        mXPos,
        mYPos,
        mVelX,
        mVelY,
        GetSpriteScale());

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

    GetAnimation().mFlags.Clear(AnimFlags::eRender);
    mCurrentMotion = eSligMotions::Motion_0_StandIdle;
    mVelY = FP_FromInteger(0);
    mVelX = FP_FromInteger(0);
    mHealth = FP_FromInteger(0);
    MusicController::static_PlayMusic(MusicController::MusicTypes::eType0, this, 0, 0);
    field_114_timer = sGnFrame + 60;
    SetUpdateDelay(40);
    SetBrain(&Slig::Brain_Death_46C3A0);
    EventBroadcast(kEventMudokonComfort, sActiveHero);
}

void Slig::SetBrain(Slig::TBrainFn fn)
{
    mBrainState = fn;
}

bool Slig::BrainIs(Slig::TBrainFn fn)
{
    return mBrainState == fn;
}

} // namespace AO
