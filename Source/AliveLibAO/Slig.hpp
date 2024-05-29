#pragma once

#include "../relive_lib/GameObjects/BaseAliveGameObject.hpp"
#include "../relive_lib/FatalError.hpp"
#include "GameSpeak.hpp"

namespace relive
{
    struct Path_Slig;
}

namespace AO {

#define SLIG_MOTIONS_ENUM_AO(ENTRY)              \
    ENTRY(Motion_0_StandIdle)          \
    ENTRY(Motion_1_StandToWalk)        \
    ENTRY(Motion_2_Walking)            \
    ENTRY(Motion_3_StandToRun)         \
    ENTRY(Motion_4_Running)            \
    ENTRY(Motion_5_TurnAroundStanding) \
    ENTRY(Motion_6_Shoot)              \
    ENTRY(Motion_7_Falling)            \
    ENTRY(Motion_8_Unknown)            \
    ENTRY(Motion_9_SlidingToStand)     \
    ENTRY(Motion_10_SlidingTurn)       \
    ENTRY(Motion_11_SlidingTurnToWalk) \
    ENTRY(Motion_12_SlidingTurnToRun)  \
    ENTRY(Motion_13_ReloadGun)            \
    ENTRY(Motion_14_ShootToStand)      \
    ENTRY(Motion_15_SteppingToStand)   \
    ENTRY(Motion_16_StandingToStep)    \
    ENTRY(Motion_17_DepossessingAbort) \
    ENTRY(Motion_18_GameSpeak)         \
    ENTRY(Motion_19_WalkToStand)       \
    ENTRY(Motion_20_Recoil)            \
    ENTRY(Motion_21_SpeakHereBoy)      \
    ENTRY(Motion_22_SpeakHi)           \
    ENTRY(Motion_23_SpeakFreeze)       \
    ENTRY(Motion_24_SpeakGetHim)       \
    ENTRY(Motion_25_SpeakLaugh)        \
    ENTRY(Motion_26_SpeakBullshit1)    \
    ENTRY(Motion_27_SpeakLookOut)      \
    ENTRY(Motion_28_SpeakBullshit2)    \
    ENTRY(Motion_29_SpeakPanic)        \
    ENTRY(Motion_30_SpeakWhat)         \
    ENTRY(Motion_31_SpeakAIFreeze)     \
    ENTRY(Motion_32_Blurgh)            \
    ENTRY(Motion_33_Sleeping)          \
    ENTRY(Motion_34_SleepingToStand)   \
    ENTRY(Motion_35_Knockback)         \
    ENTRY(Motion_36_KnockbackToStand)  \
    ENTRY(Motion_37_Depossessing)      \
    ENTRY(Motion_38_Possess)           \
    ENTRY(Motion_39_OutToFall)         \
    ENTRY(Motion_40_FallingInitiate)   \
    ENTRY(Motion_41_LandingSoft)       \
    ENTRY(Motion_42_LandingFatal)      \
    ENTRY(Motion_43_ShootZ)            \
    ENTRY(Motion_44_ShootZtoStand)     \
    ENTRY(Motion_45_Smash)             \
    ENTRY(State_46_PullLever_46A590)         \
    ENTRY(Motion_47_LiftUp)            \
    ENTRY(Motion_48_LiftDown)          \
    ENTRY(Motion_49_LiftGrip)          \
    ENTRY(Motion_50_LiftUngrip)        \
    ENTRY(Motion_51_LiftGripping)      \
    ENTRY(Motion_52_Beat)

#define MAKE_ENUM(VAR) VAR,
enum eSligMotions
{
    None_m1 = -1,
    SLIG_MOTIONS_ENUM_AO(MAKE_ENUM)
};

struct SligResources final
{
    u8** res[17];
};

enum class SligSpeak : u8
{
    eHi_0 = 0,
    eHereBoy_1 = 1,
    eGetHim_2 = 2,
    eStay_3 = 3,
    eBullshit_4 = 4,
    eLookOut_5 = 5,
    eBullshit2_6 = 6,
    eLaugh_7 = 7,
    eFreeze_8 = 8,
    eWhat_9 = 9,
    eHelp_10 = 10,
    eBlurgh_11 = 11
};

enum class SligSfx : s8
{
    eToStand_0 = 0,
    eStandingTurn_1 = 1,
    eWalkingStep_2 = 2,
    eRunningStep_3 = 3,
    eSnooze1_4 = 4,
    eSnooze2_5 = 5,

    // These are all unused
    eHi_6 = 6,
    eHereBoy_7 = 7,
    eGetHim_8 = 8,
    eStay_9 = 9,
    eBullshit_10 = 10,
    eLookOut_11 = 11,
    eBullshit2_12 = 12,
    eLaugh_13 = 13,
    eFreeze_14 = 14,
    eWhat_15 = 15,
    eHelp_16 = 16,
    eBlurgh_17 = 17,
};

class Slig final : public ::BaseAliveGameObject
{
public:
    static constexpr AnimId sSligMotionAnimIds[54] = {
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

    Slig(relive::Path_Slig* pTlv, const Guid& tlvId);
    ~Slig();

    void LoadAnimations();
    void Init();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;
    virtual void VRender(OrderingTable& ot) override;
    virtual void VOnTrapDoorOpen() override;
    virtual void VUnPosses() override;
    virtual void VPossessed() override;
    virtual bool VTakeDamage(BaseGameObject* pFrom) override;
    virtual void VOnTlvCollision(relive::Path_TLV* pTlv) override;
    virtual bool VIsFacingMe(BaseAnimatedWithPhysicsGameObject* pOther) override;
    virtual bool VOnSameYLevel(BaseAnimatedWithPhysicsGameObject* pOther) override;
    virtual s16 VGetMotion(eMotionType motionType) override
    {
        switch (motionType)
        {
            case eMotionType::ePreviousMotion:
                return static_cast<s16>(mPreviousMotion);
            case eMotionType::eCurrentMotion:
                return static_cast<s16>(mCurrentMotion);
            case eMotionType::eNextMotion:
                return static_cast<s16>(mNextMotion);
            default:
                ALIVE_FATAL("Invalid motion type %d", static_cast<s32>(motionType));
        }
    }

    void VUpdateAnimData();
    void VShot();
    void MoveOnLine();
    void ToKnockBack();
    void PlayerControlRunningSlideStopOrTurnFrame4();
    void PlayerControlRunningSlideStopOrTurnFrame12();
    void SlowDown(FP speed);
    s16 HandlePlayerControlled();
    void OperateLift();
    s16 MainMovement();
    void Slig_SoundEffect(SligSfx sfxIdx);
    u8** ResBlockForMotion(s16 motion);
    bool VIs8_465630(eSligMotions motion);
    void ToShoot();
    void ToZShoot();
    void ShouldStillBeAlive();
    void ToAbeDead();
    void WaitOrWalk();
    s32 IsFacingEffectiveLeft(Slig* pSlig);
    void PauseALittle();
    void ToTurn();
    void ToPanicRunning();
    void ToPanic();
    void ToChase();
    void ToKilledAbe();
    s16 FindBeatTarget(s32 gridBlocks);
    s16 HandleEnemyStopper(s32 gridBlocks);
    void RespondToEnemyOrPatrol();
    void TurnOrWalk();
    void ToPanicTurn();
    void PlayerControlRunningSlideStopOrTurn(s16 last_anim_frame);
    eSligMotions GetNextMotionIncGameSpeak(u16 input);
    static bool InAnyWellRenderLayer(BaseAliveGameObject* pThis);
    static s16 IsAbeEnteringDoor(BaseAliveGameObject* pThis);
    static s16 IsWallBetween(Slig* pLeft, BaseAliveGameObject* pRight);
    static void Slig_GameSpeak_SFX(SligSpeak effectId, s32 defaultVol, s32 pitch_min, BaseAliveGameObject* pObj);
    void ToStand();
    static s16 IsInZCover(BaseAnimatedWithPhysicsGameObject* pObj);
    void CheckPlatformVanished();
    eSligMotions MoveLift(FP ySpeed);
    void GameSpeakResponse();


    // Motions
    void Motion_0_StandIdle();
    void Motion_1_StandToWalk();
    void Motion_2_Walking();
    void Motion_3_StandToRun();
    void Motion_4_Running();
    void Motion_5_TurnAroundStanding();
    void Motion_6_Shoot();
    void Motion_7_Falling();
    void Motion_8_Unknown();
    void Motion_9_SlidingToStand();
    void Motion_10_SlidingTurn();
    void Motion_11_SlidingTurnToWalk();
    void Motion_12_SlidingTurnToRun();
    void Motion_13_ReloadGun();
    void Motion_14_ShootToStand();
    void Motion_15_SteppingToStand();
    void Motion_16_StandingToStep();
    void Motion_17_DepossessingAbort();
    void Motion_18_GameSpeak();
    void Motion_19_WalkToStand();
    void Motion_20_Recoil();
    void Motion_21_SpeakHereBoy();
    void Motion_22_SpeakHi();
    void Motion_23_SpeakFreeze();
    void Motion_24_SpeakGetHim();
    void Motion_25_SpeakLaugh();
    void Motion_26_SpeakBullshit1();
    void Motion_27_SpeakLookOut();
    void Motion_28_SpeakBullshit2();
    void Motion_29_SpeakPanic();
    void Motion_30_SpeakWhat();
    void Motion_31_SpeakAIFreeze();
    void Motion_32_Blurgh();
    void Motion_33_Sleeping();
    void Motion_34_SleepingToStand();
    void Motion_35_Knockback();
    void Motion_36_KnockbackToStand();
    void Motion_37_Depossessing();
    void Motion_38_Possess();
    void Motion_39_OutToFall();
    void Motion_40_FallingInitiate();
    void Motion_41_LandingSoft();
    void Motion_42_LandingFatal();
    void Motion_43_ShootZ();
    void Motion_44_ShootZtoStand();
    void Motion_45_Smash();
    void Motion_46_ToIdle();
    void Motion_47_LiftUp();
    void Motion_48_LiftDown();
    void Motion_49_LiftGrip();
    void Motion_50_LiftUngrip();
    void Motion_51_LiftGripping();
    void Motion_52_Beat();

    // Brains
    s16 Brain_SpottedEnemy();
    s16 Brain_Paused();
    s16 Brain_EnemyDead();
    s16 Brain_KilledEnemy();
    s16 Brain_Unknown();
    s16 Brain_Sleeping();
    void WakeUp();
    s16 Brain_WakingUp();
    s16 Brain_Inactive();
    s16 Brain_Possessed();
    s16 Brain_Death();
    s16 Brain_DeathDropDeath();
    s16 Brain_ReturnControlToAbeAndDie(); // just unpossess?
    s16 Brain_PanicTurning();
    s16 Brain_PanicRunning();
    s16 Brain_PanicYelling();
    s16 Brain_Chasing();
    s16 Brain_StopChasing();
    s16 Brain_StartChasing();
    s16 Brain_Idle();
    s16 Brain_Turning();
    s16 Brain_Walking();
    s16 Brain_GetAlertedTurn();
    s16 Brain_GetAlerted();
    s16 Brain_StoppingNextToMudokon();
    s16 Brain_BeatingUp();
    s16 Brain_Discussion();
    s16 Brain_ChaseAndDisappear();
    s16 Brain_Shooting();
    s16 Brain_ZSpottedEnemy();
    s16 Brain_ZShooting();

    void BlowToGibs();

    using TBrainFn = decltype(&Slig::Brain_ZShooting);

    void SetBrain(TBrainFn fn);

    bool BrainIs(TBrainFn fn);

    s16 mBrainSubState = 0;
    s16 mGameSpeakPitchMin = 0;
    s32 field_114_timer = 0; // this timer has multiple meanings
    bool mReturnToPreviousMotion = false;
    bool mCheckedIfOffScreen = false;
    s16 mInput = 0;
    s32 field_128_timer = 0; // this timer has multiple meanings
    FP mFallingVelxScaleFactor = {};
    s16 mGameEnderPauseTime = 0;
    Guid field_134_tlvInfo;
    s16 field_138_res_idx = 0;
    eSligMotions field_13A_shot_motion = eSligMotions::Motion_0_StandIdle;
    PSX_RECT field_13C_zone_rect = {};
    EReliveLevelIds mAbeLevel = EReliveLevelIds::eNone;
    s16 mAbePath = 0;
    s16 mAbeCamera = 0;
    s32 field_154_death_by_being_shot_timer = 0;
    s32 mExplodeTimer = 0;
    GameSpeakListener mListener;
    relive::Path_Slig* field_174_tlv = {};
    PSX_Point field_1CC_points[10] = {};
    s16 field_1F4_points_count = 0;
    TBrainFn mBrainState = nullptr;
    s16 mShootCount = 0;
    s16 field_20C_force_alive_state = 0;
    s16 mSpottedPossessedSlig = 0;
    SligResources field_210_resources = {};
    s16 mPreventDepossession = 0;
    eSligMotions field_258_next_gamespeak_motion = eSligMotions::Motion_0_StandIdle;
    eSligMotions mPreviousMotion = eSligMotions::Motion_0_StandIdle;
    eSligMotions mCurrentMotion = eSligMotions::Motion_0_StandIdle;
    eSligMotions mNextMotion = eSligMotions::Motion_0_StandIdle;
    bool mbMotionChanged = false;
};

} // namespace AO
