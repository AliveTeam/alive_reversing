#pragma once

#include "BaseAliveGameObject.hpp"
#include "GameSpeak.hpp"
#include "Path.hpp"
#include "../relive_lib/SaveStateBase.hpp"
#include "../relive_lib/FatalError.hpp"

#define SCRAB_MOTIONS_ENUM_AE(ENTRY)          \
    ENTRY(Motion_0_Stand)                \
    ENTRY(Motion_1_Walk)                 \
    ENTRY(Motion_2_Run)                  \
    ENTRY(Motion_3_Turn)                 \
    ENTRY(Motion_4_RunToStand)           \
    ENTRY(Motion_5_HopBegin)             \
    ENTRY(Motion_6_HopMidair)            \
    ENTRY(Motion_7_HopLand)              \
    ENTRY(Motion_8_JumpToFall)           \
    ENTRY(Motion_9_StandToWalk)          \
    ENTRY(Motion_10_StandToRun)          \
    ENTRY(Motion_11_WalkToStand)         \
    ENTRY(Motion_12_RunJumpBegin)        \
    ENTRY(Motion_13_RunJumpEnd)          \
    ENTRY(Motion_14_WalkToFall)          \
    ENTRY(Motion_15_RunToFall)           \
    ENTRY(Motion_16_WalkToRun)           \
    ENTRY(Motion_17_RunToWalk)           \
    ENTRY(Motion_18_Knockback)           \
    ENTRY(Motion_19_GetEaten)            \
    ENTRY(Motion_20_Fall)                \
    ENTRY(Motion_21_Stamp)               \
    ENTRY(Motion_22_GetPossessed)        \
    ENTRY(Motion_23_Empty)               \
    ENTRY(Motion_24_DeathEnd)            \
    ENTRY(Motion_25_Empty)               \
    ENTRY(Motion_26_HowlBegin)           \
    ENTRY(Motion_27_HowlEnd)             \
    ENTRY(Motion_28_GetDepossessedBegin) \
    ENTRY(Motion_29_GetDepossessedEnd)   \
    ENTRY(Motion_30_Shriek)              \
    ENTRY(Motion_31_ScrabBattleAnim)     \
    ENTRY(Motion_32_AttackSpin)          \
    ENTRY(Motion_33_FeedToGulp)          \
    ENTRY(Motion_34_GulpToStand)         \
    ENTRY(Motion_35_StandToFeed)         \
    ENTRY(Motion_36_Feed)                \
    ENTRY(Motion_37_AttackLunge)         \
    ENTRY(Motion_38_LegKick)             \
    ENTRY(Motion_39_DeathBegin)

#define MAKE_ENUM(VAR) VAR,
enum class eScrabMotions : s32
{
    eNone_m1 = -1,
    SCRAB_MOTIONS_ENUM_AE(MAKE_ENUM)
};

enum class ScrabSounds : u8
{
    eHowl_0 = 0,
    eDeathHowl_1 = 1,
    eEmpty_2 = 2,
    eEmpty_3 = 3,
    eHitCollision_4 = 4,
    eShredding_5 = 5,
    eWalk1_6 = 6,
    eWalk2_7 = 7,
    eYell_8 = 8,
};

enum class LevelIds : s16;
enum class GameSpeakEvents : s16;
enum eLineTypes : s16;

struct ScrabSaveState final : public SaveStateBase
{
    ScrabSaveState()
        : SaveStateBase(ReliveTypes::eScrab, sizeof(*this))
    { }
    Guid field_4_obj_id;
    FP mXPos;
    FP mYPos;
    FP mVelX;
    FP mVelY;
    s16 mCurrentPath;
    EReliveLevelIds mCurrentLevel;
    FP mSpriteScale;
    s16 mRingRed;
    s16 mRingGreen;
    s16 mRingBlue;
    s16 mFlipX;
    eScrabMotions field_28_current_motion;
    s16 mCurrentFrame;
    s16 mFrameChangeCounter;
    s8 mAnimRender;
    s8 mDrawable;
    FP mHealth;
    eScrabMotions field_34_current_motion;
    eScrabMotions field_36_next_motion;
    s16 field_38_last_line_ypos;
    eLineTypes mLineType;
    s8 mIsControlled;
    Guid field_44_tlvInfo;
    s32 field_48_brain_idx;
    s16 mBrainSubState;
    Guid field_54_obj_id;
    Guid field_58_target_obj_id;
    s32 field_5C_timer;
    s32 field_60_depossession_timer;
    FP field_64_falling_velx_scale_factor;
    s16 field_68_motion_resource_block_index;
    s32 field_6C_spotting_abe_timer;
    s32 field_70_attack_delay_timer;
    s32 field_74_movement_timer;
    s32 field_78_sfx_bitmask;
    s16 mPreventDepossession;
    EReliveLevelIds mAbeLevel;
    s16 mAbePath;
    s16 mAbeCamera;
    s16 field_84_input;
    s16 field_8C_shred_power_active;
    GameSpeakEvents field_8E_speak;
    FP field_90_max_xpos;
    FP field_94_max_ypos;
    s16 field_98_speak_counter;
    bool mAttacking;
    bool mForceUpdateAnimation;
    bool mRoarRandomly;
    bool mPersistant;
};

class Scrab;
using TScrabBrainFn = s16 (Scrab::*)();
using TScrabMotionFn = void (Scrab::*)();

class Scrab final : public BaseAliveGameObject
{
public:
    static constexpr AnimId sScrabMotionAnimIds[40] = {
        AnimId::Scrab_Idle,
        AnimId::Scrab_Walk,
        AnimId::Scrab_Run,
        AnimId::Scrab_Turn,
        AnimId::Scrab_RunToStand,
        AnimId::Scrab_HopBegin,
        AnimId::Scrab_Jump,
        AnimId::Scrab_Landing,
        AnimId::Scrab_JumpAndRunToFall,
        AnimId::Scrab_StandToWalk,
        AnimId::Scrab_StandToRun,
        AnimId::Scrab_WalkToStand,
        AnimId::Scrab_Jump,
        AnimId::Scrab_Landing,
        AnimId::Scrab_WalkToFall,
        AnimId::Scrab_JumpAndRunToFall,
        AnimId::Scrab_WalkToRun,
        AnimId::Scrab_RunToWalk,
        AnimId::Scrab_Knockback,
        AnimId::Scrab_GetEaten,
        AnimId::Scrab_Landing,
        AnimId::Scrab_Stamp,
        AnimId::Scrab_Stamp,
        AnimId::Scrab_DeathEnd,
        AnimId::Scrab_DeathEnd,
        AnimId::Scrab_Empty,
        AnimId::Scrab_HowlBegin,
        AnimId::Scrab_HowlEnd,
        AnimId::Scrab_HowlBegin,
        AnimId::Scrab_HowlEnd,
        AnimId::Scrab_Shriek,
        AnimId::Scrab_ScrabBattleAnim,
        AnimId::Scrab_AttackSpin,
        AnimId::Scrab_FeedToGulp,
        AnimId::Scrab_GulpToStand,
        AnimId::Scrab_StandToFeed,
        AnimId::Scrab_Feed,
        AnimId::Scrab_AttackLunge,
        AnimId::Scrab_LegKick,
        AnimId::Scrab_DeathBegin};

    Scrab(relive::Path_Scrab* pTlv, const Guid& tlvId, relive::Path_ScrabSpawner::SpawnDirection spawnDirection);
    ~Scrab();

    void LoadAnimations();
    virtual void VUpdate() override;
    virtual void VRender(OrderingTable& ot) override;
    virtual void VPossessed() override;
    virtual void VScreenChanged() override;
    virtual bool VTakeDamage(BaseGameObject* pFrom) override;
    virtual void VOnTlvCollision(relive::Path_TLV* pTlv) override;

    virtual void VGetSaveState(SerializedObjectData& pSaveBuffer) override;
    virtual bool VOnSameYLevel(BaseAnimatedWithPhysicsGameObject* pOther) override;
    virtual void VOnTrapDoorOpen() override;
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

    static void CreateFromSaveState(SerializedObjectData& pBuffer);

private:
    void HandleDDCheat();
    void vUpdateAnim();
    s16 OnFloor();
    void Update_Slurg_Step_Watch_Points();

public:
    s16 Brain_0_Patrol();

    s16 Brain_ChasingEnemy_State_2_Running(BaseAliveGameObject* pObj);

    s16 Brain_1_ChasingEnemy();
    s16 Brain_2_Fighting();
    s16 Brain_3_Death();
    s16 Brain_4_ShrinkDeath();
    s16 Brain_5_Possessed();

    void SetBrain(TScrabBrainFn fn);
    bool BrainIs(TScrabBrainFn fn);

public:
    void Motion_0_Stand();
    void Motion_1_Walk();
    void Motion_2_Run();
    void Motion_3_Turn();
    void Motion_4_RunToStand();
    void Motion_5_HopBegin();
    void Motion_6_HopMidair();
    void Motion_7_HopLand();
    void Motion_8_JumpToFall();
    void Motion_9_StandToWalk();
    void Motion_10_StandToRun();
    void Motion_11_WalkToStand();
    void Motion_12_RunJumpBegin();
    void Motion_13_RunJumpEnd();
    void Motion_14_WalkToFall();
    void Motion_15_RunToFall();
    void Motion_16_WalkToRun();
    void Motion_17_RunToWalk();
    void Motion_18_Knockback();
    void Motion_19_GetEaten();
    void Motion_20_Fall();
    void Motion_21_Stamp();
    void Motion_22_GetPossessed();
    void Motion_23_Empty();
    void Motion_24_DeathEnd();
    void Motion_25_Empty();
    void Motion_26_HowlBegin();
    void Motion_27_HowlEnd();
    void Motion_28_GetDepossessedBegin();
    void Motion_29_GetDepossessedEnd();
    void Motion_30_Shriek();
    void Motion_31_ScrabBattleAnim();
    void Motion_32_AttackSpin();
    void Motion_33_FeedToGulp();
    void Motion_34_GulpToStand();
    void Motion_35_StandToFeed();
    void Motion_36_Feed();
    void Motion_37_AttackLunge();
    void Motion_38_LegKick();
    void Motion_39_DeathBegin();


private:

    void ToPatrol();
    void ToStand();
    void MoveOnLine();
    void PlatformCollide();
    BaseAliveGameObject* Find_Fleech();
    u8** ResBlockForMotion(s16 motion);
    void TryMoveOrStand();
    s16 ToNextMotion();
    s16 PlayerControlled();
    void ToJump();
    void KnockBack();
    s32 Scrab_SFX(ScrabSounds soundId, s32 vol, s32 pitch, s16 applyDirection);
    void KillTarget(BaseAliveGameObject* pTarget);
    s16 FindAbeOrMud();
    s16 CanSeeAbe(BaseAliveGameObject* pObj);
    static bool LineOfSightTo(Scrab* pThis, BaseAliveGameObject* pObj);
    Scrab* FindScrabToFight();
    s16 Handle_SlamDoor_or_EnemyStopper(FP velX, s16 bCheckLeftRightBounds);
    GameSpeakEvents LastSpeak();

    // TODO: remove these later
    void SetPreviousMotion(eScrabMotions motion)
    {
        mPreviousMotion = motion;
    }

    void SetCurrentMotion(eScrabMotions motion)
    {
        mCurrentMotion = motion;
    }

    void SetNextMotion(eScrabMotions motion)
    {
        mNextMotion = motion;
    }

private:
    TScrabBrainFn mBrainState = nullptr;
    s16 mBrainSubState = 0;
    s16 field_11E_return_to_previous_motion = 0;
    Guid mTargetGuid;
    Guid mFightTargetId;
    s16 mAttackDelay = 0;
    s16 mPatrolTypeRunOrWalkChance = 0;
    s32 field_12C_timer = 0;
    s32 field_130_depossession_timer = 0;
    FP field_134_falling_velx_scale_factor = {};
    FP field_13C_last_ypos = {};
    s16 field_140_motion_resource_block_index = 0;
    Guid field_144_tlvInfo;
    s32 mPauseAfterChaseTime = 0;
    s32 field_14C_pause_after_chase_timer = 0;
    s32 field_150_attack_delay_timer = 0;
    s32 field_154_movement_timer = 0;
    s16 mPauseLeftMin = 0;
    s16 mPauseLeftMax = 0;
    s16 mPauseRightMin = 0;
    s16 mPauseRightMax = 0;
    s32 field_160_sfx_bitmask = 0;
    s16 mPreventDepossession = 0;
    EReliveLevelIds mAbeLevel = EReliveLevelIds::eNone;
    s16 mAbePath = 0;
    s16 mAbeCamera = 0;
    s32 field_16C_input = 0;
    s16 mPossessedMaxWhirlAttackDuration = 0;
    s16 mShredPowerActive = 0;
    s32 field_17C_last_event = 0;
    GameSpeakEvents field_194_speak = GameSpeakEvents::eUnknown_0;
    FP field_198_max_xpos = {};
    FP field_19C_max_ypos = {};
    s16 field_1A0_speak_max = 0;
    s16 field_1A2_speak_counter = 0;
    Choice_short mKillEnemy = Choice_short::eNo_0;
    bool mAttacking = false;
    bool mForceUpdateAnimation = false;
    bool mRoarRandomly = false;
    bool mPersistant = false;

    eScrabMotions mPreviousMotion = eScrabMotions::Motion_0_Stand;
    eScrabMotions mCurrentMotion = eScrabMotions::Motion_0_Stand;
    eScrabMotions mNextMotion = eScrabMotions::Motion_0_Stand;
    bool mbMotionChanged = false;
};
