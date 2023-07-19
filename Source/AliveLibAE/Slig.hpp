#pragma once

#include "BaseAliveGameObject.hpp"
#include "../relive_lib/MapWrapper.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"
#include "../relive_lib/SaveStateBase.hpp"

#define SLIG_MOTIONS_ENUM_AE(ENTRY)         \
    ENTRY(Motion_0_StandIdle)          \
    ENTRY(Motion_1_StandToWalk)        \
    ENTRY(Motion_2_Walking)            \
    ENTRY(Motion_3_StandToRun)         \
    ENTRY(Motion_4_Running)            \
    ENTRY(Motion_5_TurnAroundStanding) \
    ENTRY(Motion_6_Shoot)              \
    ENTRY(Motion_7_Falling)            \
    ENTRY(Motion_8_SlidingToStand)     \
    ENTRY(Motion_9_SlidingTurn)        \
    ENTRY(Motion_10_SlidingTurnToWalk) \
    ENTRY(Motion_11_SlidingTurnToRun)  \
    ENTRY(Motion_12_ReloadGun)         \
    ENTRY(Motion_13_ShootToStand)      \
    ENTRY(Motion_14_SteppingToStand)   \
    ENTRY(Motion_15_StandingToStep)    \
    ENTRY(Motion_16_DepossessingAbort) \
    ENTRY(Motion_17_GameSpeak)         \
    ENTRY(Motion_18_WalkToStand)       \
    ENTRY(Motion_19_Recoil)            \
    ENTRY(Motion_20_SpeakHereBoy)      \
    ENTRY(Motion_21_SpeakHi)           \
    ENTRY(Motion_22_SpeakFreeze)       \
    ENTRY(Motion_23_SpeakGetHim)        \
    ENTRY(Motion_24_SpeakLaugh)        \
    ENTRY(Motion_25_SpeakBullshit1)    \
    ENTRY(Motion_26_SpeakLookOut)      \
    ENTRY(Motion_27_SpeakBullshit2)    \
    ENTRY(Motion_28_SpeakPanic)        \
    ENTRY(Motion_29_SpeakWhat)         \
    ENTRY(Motion_30_SpeakAIFreeze)     \
    ENTRY(Motion_31_Blurgh)            \
    ENTRY(Motion_32_Sleeping)          \
    ENTRY(Motion_33_SleepingToStand)   \
    ENTRY(Motion_34_Knockback)         \
    ENTRY(Motion_35_KnockbackToStand)  \
    ENTRY(Motion_36_Depossessing)      \
    ENTRY(Motion_37_Possess)           \
    ENTRY(Motion_38_OutToFall)         \
    ENTRY(Motion_39_FallingInitiate)   \
    ENTRY(Motion_40_LandingSoft)       \
    ENTRY(Motion_41_LandingFatal)      \
    ENTRY(Motion_42_ShootZ)            \
    ENTRY(Motion_43_ShootZtoStand)     \
    ENTRY(Motion_44_Smash)             \
    ENTRY(Motion_45_PullLever)         \
    ENTRY(Motion_46_LiftGrip)          \
    ENTRY(Motion_47_LiftUngrip)        \
    ENTRY(Motion_48_LiftGripping)      \
    ENTRY(Motion_49_LiftUp)            \
    ENTRY(Motion_50_LiftDown)          \
    ENTRY(Motion_51_Beat)

#define MAKE_ENUM(VAR) VAR,
enum class eSligMotions : s32
{
    eNone_m1 = -1,
    SLIG_MOTIONS_ENUM_AE(MAKE_ENUM)
};

enum class SligSfx : s16
{
    // Normal Slig
    eToStand_0 = 0,
    eStandingTurn_1 = 1,
    eWalkingStep_2 = 2,
    eRunningStep_3 = 3,
    eSnooze2_4 = 4,
    eSnooze1_5 = 5,
    eReload1_6 = 6,
    eReload2_7 = 7,

    // Flying Slig
    eThrowGrenade_8 = 8,
    ePropeller1_9 = 9,
    ePropeller2_10 = 10,
    ePropeller3_11 = 11, // Apparently unused
    eCollideWithWall1_12 = 12,
    eCollideWithWall2_13 = 13,
};

enum class LevelIds : s16;
enum eLineTypes : s16;

class Slig;
using TSligBrainFn = s16 (Slig::*)();
using TSligMotionFn = void (Slig::*)();

struct SligSaveState final : public SaveStateBase
{
    SligSaveState()
        : SaveStateBase(ReliveTypes::eSlig, sizeof(*this))
    { }
    FP field_4_xpos;
    FP field_8_ypos;
    FP field_C_velx;
    FP field_10_vely;
    s16 field_14_path_number;
    EReliveLevelIds field_16_lvl_number;
    FP field_18_sprite_scale;
    Scale field_1C_scale;
    s16 field_1E_r;
    s16 field_20_g;
    s16 field_22_b;
    s16 field_24_bFlipX;
    eSligMotions field_26_current_motion;
    s16 field_28_current_frame;
    s16 field_2A_frame_change_counter;
    s8 field_2C_bRender;
    s8 field_2D_bDrawable;
    FP field_30_health;
    eSligMotions field_34_current_motion;
    eSligMotions field_36_next_motion;
    s16 field_38_last_line_ypos;
    eLineTypes field_3A_collision_line_type;
    s8 field_40_bActiveChar;
    s16 field_42_brain_sub_state;
    s16 field_44_pitch_min;
    s32 field_48_timer;
    bool mReturnToPreviousMotion;
    bool mCheckedIfOffScreen;
    s16 field_50_input;
    s32 field_54_timer;
    FP mFallingVelxScaleFactor;
    Guid field_5C_tlvInfo;
    s16 field_60_res_idx;
    eSligMotions field_62_shot_motion;
    PSX_RECT field_64_zone_rect;
    EReliveLevelIds field_72_return_level;
    s16 field_74_return_path;
    s16 field_76_return_camera;
    s32 field_78_death_by_being_shot_timer;
    s32 field_7C_explode_timer;
    s32 field_80_brain_state_idx;
    s32 field_8C_num_times_to_shoot;
    s16 field_90_force_alive_state;
    s16 field_92_spotted_possessed_slig;
    Guid field_94_glukkon_id;
    s16 field_98_state_after_speak;
    s16 field_9A_attention_timeout;
    s16 field_9E_next_command_arg1;
    s16 field_A0_cmd_idx;
    bool mFollowGlukkon;
    bool mStoppedForLeverOrLift;
    bool mGlukkonCalledAllOYa;
    bool mHeardGlukkon;
};

enum class GameSpeakEvents : s16;

class LiftPoint;

class Slig final : public BaseAliveGameObject
{
public:
    static constexpr AnimId sSligAnimIdTable[52] = {
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

    Slig(relive::Path_Slig* pTlv, const Guid& tlvId);
    ~Slig();

    void LoadAnimations();
    virtual void VUpdate() override;

    virtual void VRender(OrderingTable& ot) override;

    virtual void VScreenChanged() override;
    virtual void VPossessed() override;
    virtual void VUnPosses() override;
    virtual void VOnTlvCollision(relive::Path_TLV* pTlv) override;
    virtual void VOnTrapDoorOpen() override;
    virtual bool VTakeDamage(BaseGameObject* pFrom) override;

    virtual bool VIsFacingMe(BaseAnimatedWithPhysicsGameObject* pOther) override;
    virtual bool VOnSameYLevel(BaseAnimatedWithPhysicsGameObject* pOther) override;
    virtual void VGetSaveState(SerializedObjectData& pSaveBuffer) override;

    static void CreateFromSaveState(SerializedObjectData& pBuffer);
    static s16 IsAbeEnteringDoor(IBaseAliveGameObject* pThis);

    eSligMotions GetNextMotion() const
    {
        return static_cast<eSligMotions>(mNextMotion);
    }
    eSligMotions GetCurrentMotion() const
    {
        return static_cast<eSligMotions>(mCurrentMotion);
    }

    bool vUnderGlukkonCommand_4B1760();
    void SetBrain(TSligBrainFn fn);
    bool BrainIs(TSligBrainFn fn);

    void Motion_0_StandIdle();
    void Motion_1_StandToWalk();
    void Motion_2_Walking();
    void Motion_3_StandToRun();
    void Motion_4_Running();
    void Motion_5_TurnAroundStanding();
    void Motion_6_Shoot();
    void Motion_7_Falling();
    void Motion_8_SlidingToStand();
    void Motion_9_SlidingTurn();
    void Motion_10_SlidingTurnToWalk();
    void Motion_11_SlidingTurnToRun();
    void Motion_12_ReloadGun();
    void Motion_13_ShootToStand();
    void Motion_14_SteppingToStand();
    void Motion_15_StandingToStep();
    void Motion_16_DepossessingAbort();
    void Motion_17_GameSpeak();
    void Motion_18_WalkToStand();
    void Motion_19_Recoil();
    void Motion_20_SpeakHereBoy();
    void Motion_21_SpeakHi();
    void Motion_22_SpeakFreeze();
    void Motion_23_SpeakGetHim();
    void Motion_24_SpeakLaugh();
    void Motion_25_SpeakBullshit1();
    void Motion_26_SpeakLookOut();
    void Motion_27_SpeakBullshit2();
    void Motion_28_SpeakPanic();
    void Motion_29_SpeakWhat();
    void Motion_30_SpeakAIFreeze();
    void Motion_31_Blurgh();
    void Motion_32_Sleeping();
    void Motion_33_SleepingToStand();
    void Motion_34_Knockback();
    void Motion_35_KnockbackToStand();
    void Motion_36_Depossessing();
    void Motion_37_Possess();
    void Motion_38_OutToFall();
    void Motion_39_FallingInitiate();
    void Motion_40_LandingSoft();
    void Motion_41_LandingFatal();
    void Motion_42_ShootZ();
    void Motion_43_ShootZtoStand();
    void Motion_44_Smash();
    void Motion_45_PullLever();
    void Motion_46_LiftGrip();
    void Motion_47_LiftUngrip();
    void Motion_48_LiftGripping();
    void Motion_49_LiftUp();
    void Motion_50_LiftDown();
    void Motion_51_Beat();

    s16 Brain_Empty();
    s16 Brain_0_Death();
    s16 Brain_1_ReturnControlToAbeAndDie();
    s16 Brain_2_Possessed();
    s16 Brain_3_DeathDropDeath();

    s16 Brain_4_ListeningToGlukkon();
    s16 Brain_ListeningToGlukkon_LostAttention();
    s16 Brain_ListeningToGlukkon_Shooting();
    s16 Brain_ListeningToGlukkon_StoppingOnLift(PlatformBase* pPlatformObj);
    s16 Brain_ListeningToGlukkon_PullingLever();
    s16 Brain_ListenToGlukkon_StoppingNextToLever();
    s16 Brain_ListenToGlukkon_Speaking();
    s16 Brain_ListenToGlukkon_NextLever(BaseAliveGameObject* pGlukkonObj);
    s16 Brain_ListenToGlukkon_Moving(BaseAliveGameObject* pGlukkonObj);
    s16 Brain_ListenToGlukkon_IdleListen(BaseAliveGameObject* pGlukkonObj, PlatformBase* pPlatformObj);
    s16 Brain_ListenToGlukkon_GettingAttention(BaseAliveGameObject* pGlukkonObj);

    s16 Brain_7_SpottedEnemy();
    s16 Brain_10_EnemyDead();
    s16 Brain_11_KilledEnemy();
    s16 Brain_12_PanicTurning();
    s16 Brain_13_PanicRunning();
    s16 Brain_14_PanicYelling();
    s16 Brain_15_Idle();
    s16 Brain_16_StopChasing();
    s16 Brain_17_Chasing();
    s16 Brain_18_StartChasing();
    s16 Brain_19_Turning();
    s16 Brain_20_StoppingNextToMudokon();
    s16 Brain_21_Walking();
    s16 Brain_22_GetAlertedTurn();
    s16 Brain_23_GetAlerted();
    s16 Brain_24_BeatingUp();
    s16 Brain_25_DiscussionWhat();
    s16 Brain_28_ZShooting();
    s16 Brain_29_Shooting();
    s16 Brain_30_ZSpottedEnemy();
    s16 Brain_31_WakingUp();
    s16 Brain_32_Inactive();
    s16 Brain_33_Paused();
    s16 Brain_34_Sleeping();
    s16 Brain_35_ChaseAndDisappear();

private:
    void Init();
    void VShot();
    void VUpdateAnimData();
    void HandleDDCheat();
    void WakeUp();
    void ShouldStillBeAlive();
    void ToTurn();
    void RespondWithWhat();
    void ToShoot();
    void ToZShoot();
    void PauseALittle();
    void ToStand();
    void BlowToGibs();
    s16 MainMovement();

    enum class MovementDirection
    {
        eLeft = 0,
        eRight = 1
    };

    void PullLever();
    void ShootOrShootZ();
    s16 ToShootZ();
    s16 LeftRigtMovement(MovementDirection direction);
    s16 GrabNearbyLift();
    s16 HandlePlayerControlled();
    eSligMotions GetNextMotionIncGameSpeak(s32 input);
    void WaitOrWalk();
    void ToAbeDead();
    void ToUnderGlukkonCommand();
    void ToKilledAbe();
    bool IsWallBetween(IBaseAliveGameObject* pLeft, IBaseAliveGameObject* pRight);
    GameSpeakEvents LastGlukkonSpeak();
    s16 ListenToGlukkonCommands();
    void PlatformCollide();
    void ToKnockBack();
    void TurnOrSayWhat();
    void GameSpeakResponse();
    void GoAlertedOrSayWhat();
    static s32 IsFacingEffectiveLeft(Slig* pSlig);
    void MoveOnLine();
    void PlayerControlStopWalkingIfRequired();
    void CheckPlatformVanished();
    eSligMotions MoveLift(FP ySpeed);
    void SlowDown(FP speed);
    void ToChase();
    s16 HandleEnemyStopper(s32 gridBlocks);
    void ToPanic();
    void ToPanicTurn();
    static s16 SligStableDelay();

    // TODO: AO has a helper function
    void PlayerControlRunningSlideStopOrTurnFrame12();
    void PlayerControlRunningSlideStopOrTurnFrame4();

    IBaseAliveGameObject* FindBeatTarget(s32 gridBlocks);
    void TurnOrWalk();
    void ToPanicRunning();
    void RespondToEnemyOrPatrol();
    s16 FindLever();
    s16 NearOrFacingActiveChar(IBaseAliveGameObject* pObj);
    static s16 IsInZCover(IBaseAliveGameObject* pObj);
    static bool InAnyWellRenderLayer(IBaseAliveGameObject* pThis);
    void NextGlukkonCommand(s16 speakTableIndex, s16 responseState);
    s16 HeardGlukkonToListenTo(GameSpeakEvents glukkonSpeak);
    s16 FindLiftPoint();
    AnimId MotionToAnimId(eSligMotions motion);

public:
    EReliveLevelIds mAbeLevel = EReliveLevelIds::eNone;
    s16 mAbePath = 0;
    s16 mAbeCamera = 0;
    s16 mBrainSubState = 0;

private:
    Guid field_118_tlvInfo;
    s16 field_11E_pitch_min = 0;
    s32 field_120_timer = 0; // this timer has multiple meanings
    bool mReturnToPreviousMotion = false;
    bool mCheckedIfOffScreen = false;
    s32 mInput = 0;
    s32 field_12C_timer = 0; // this timer has multiple meanings
    FP mFallingVelxScaleFactor = {};
    eSligMotions field_136_shot_motion = eSligMotions::Motion_0_StandIdle;
    PSX_RECT field_138_zone_rect = {};
    s32 field_14C_death_by_being_shot_timer = 0;
    s32 field_150_explode_timer = 0;
    TSligBrainFn mBrainState = nullptr;
    s16 field_158_num_times_to_shoot = 0;
    s16 field_15C_force_alive_state = 0;
    s16 field_15E_spotted_possessed_slig = 0;
    s32 field_160_last_event_index = 0;
    std::shared_ptr<AnimationPal> field_178_mPal;
    s16 field_200_red = 0;
    s16 field_202_green = 0;
    s16 field_204_blue = 0;
    Guid field_208_glukkon_obj_id;
    s16 field_20C_state_after_speak = 0;
    s16 field_20E_attention_timeout = 0;
    s16 field_212_next_command_arg1 = 0;
    s16 field_214_cmd_idx = 0;
    bool mFollowGlukkon = false;
    bool mStoppedForLeverOrLift = false;
    bool mGlukkonCalledAllOYa = false;
    bool mHeardGlukkon = false;
    relive::Path_Slig mSligTlv = {};
    PSX_Point field_268_points[10] = {};
    s16 field_290_points_count = 0;
    s16 mPreventDepossession = 0;
    eSligMotions field_294_next_gamespeak_motion = eSligMotions::Motion_0_StandIdle;
};

void Animation_OnFrame_Slig(BaseGameObject* pObj, u32&, const IndexedPoint& point);
void Slig_SoundEffect(SligSfx effect, BaseAliveGameObject* pObj);

void renderWithGlowingEyes(OrderingTable& ot, BaseAliveGameObject* actor, std::shared_ptr<AnimationPal>& pal, s16 palSize, s16& r, s16& g, s16& b,
                           const s16* eyeColourIndices, s16 eyeColourIndicesSize);
