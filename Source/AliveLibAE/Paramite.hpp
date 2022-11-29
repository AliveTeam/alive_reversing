#pragma once

#include "BaseAliveGameObject.hpp"

namespace relive
{
    struct Path_Paramite;
}

#define PARAMITE_MOTIONS_ENUM_AE(ENTRY)        \
    ENTRY(Motion_0_Idle)                  \
    ENTRY(Motion_1_WalkBegin)             \
    ENTRY(Motion_2_Walking)               \
    ENTRY(Motion_3_Running)               \
    ENTRY(Motion_4_Turn)                  \
    ENTRY(Motion_5_Hop)                   \
    ENTRY(Motion_6_Unused)                \
    ENTRY(Motion_7_WalkRunTransition)     \
    ENTRY(Motion_8_WalkEnd)               \
    ENTRY(Motion_9_RunBegin)              \
    ENTRY(Motion_10_RunEnd)               \
    ENTRY(Motion_11_Falling)              \
    ENTRY(Motion_12_JumpUpBegin)          \
    ENTRY(Motion_13_JumpUpMidair)         \
    ENTRY(Motion_14_JumpUpLand)           \
    ENTRY(Motion_15_RopePull)             \
    ENTRY(Motion_16_CloseAttack)          \
    ENTRY(Motion_17_Landing)              \
    ENTRY(Motion_18_Unused)               \
    ENTRY(Motion_19_Knockback)            \
    ENTRY(Motion_20_GameSpeakBegin)       \
    ENTRY(Motion_21_PreHiss)              \
    ENTRY(Motion_22_Hiss1)                \
    ENTRY(Motion_23_Hiss2)                \
    ENTRY(Motion_24_Empty)                \
    ENTRY(Motion_25_AllOYaGameSpeakBegin) \
    ENTRY(Motion_26_Hiss3)                \
    ENTRY(Motion_27_PostHiss)             \
    ENTRY(Motion_28_GameSpeakEnd)         \
    ENTRY(Motion_29_GetDepossessedBegin)  \
    ENTRY(Motion_30_GetDepossessedEnd)    \
    ENTRY(Motion_31_RunningAttack)        \
    ENTRY(Motion_32_Empty)                \
    ENTRY(Motion_33_SurpriseWeb)          \
    ENTRY(Motion_34_WebLeaveDown)         \
    ENTRY(Motion_35_WebIdle)              \
    ENTRY(Motion_36_WebGoingUp)           \
    ENTRY(Motion_37_WebGoingDown)         \
    ENTRY(Motion_38_WebGrab)              \
    ENTRY(Motion_39_WebLeaveUp)           \
    ENTRY(Motion_40_Eating)               \
    ENTRY(Motion_41_Death)                \
    ENTRY(Motion_42_Squawk)               \
    ENTRY(Motion_43_Attack)

#define MAKE_ENUM(VAR) VAR,
enum class eParamiteMotions : s32
{
    PARAMITE_MOTIONS_ENUM_AE(MAKE_ENUM)
};

class Meat;
class Paramite;
using TParamiteBrainFn = s16 (Paramite::*)();
using TParamiteMotionFn = void (Paramite::*)();

enum class LevelIds : s16;
enum class GameSpeakEvents : s16;

class PullRingRope;


enum class ParamiteSpeak : s8
{
    CMon_or_Attack_0 = 0,
    Stay_1 = 1,
    DoIt_2 = 2,
    LoudStep_3 = 3,
    SlightStep_4 = 4,
    Howdy_5 = 5,
    ClimbingWeb_6 = 6,
    DetectedMeat_7 = 7,
    None_8 = 8,
    AllYa_9 = 9,
    None_10 = 10,
    None_11 = 11,
};

struct ParamiteSaveState final
{
    ReliveTypes field_0_type;
    s16 field_2_pad;
    FP field_4_xpos;
    FP field_8_ypos;
    FP field_C_velx;
    FP field_10_vely;
    s16 field_14_path_number;
    EReliveLevelIds field_16_lvl_number;
    FP field_18_sprite_scale;
    s16 field_1C_r;
    s16 field_1E_g;
    s16 field_20_b;
    s16 field_22_flip_x;
    s16 field_24_current_motion;
    s16 field_26_anim_current_frame;
    s16 field_28_frame_change_counter;
    s8 field_2A_render;
    s8 field_2B_drawable;
    FP field_2C_health;
    s16 field_30_current_motion;
    s16 field_32_next_motion;
    s16 field_34_last_line_ypos;
    s16 field_36_line_type;
    s16 field_38_padding;
    s16 field_3A_padding;
    Guid field_3C_tlvInfo;
    Guid field_40_meat_id;
    Guid field_44_web_id;
    Guid field_48_obj_id;
    Guid field_4C_pull_ring_rope_id;
    s32 field_50_brain_idx;
    s16 field_54_padding;
    s16 field_56_padding;
    s16 field_58_brain_ret;
    s16 field_5A_padding;
    s32 field_5C_timer;
    s32 field_60_depossession_timer;
    FP field_64_velx_offset;
    s32 field_68_timer;
    EReliveLevelIds field_6C_return_level;
    s16 field_6E_return_path;
    s16 field_70_return_camera;
    s16 field_72_input;
    s16 field_74_next_brain_ret;
    bool mControlled;
    bool mRunning;
    bool mHissedOrLeftScreen;
    bool mPreventDepossession;
    bool mSpawned;
    bool mAlerted;
    bool mCanBePossessed;
};

class Paramite final : public BaseAliveGameObject
{
public:
    Paramite(relive::Path_Paramite* pTlv, const Guid& tlvId);
    ~Paramite();

    void LoadAnimations();
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual s16 VOnSameYLevel(BaseAnimatedWithPhysicsGameObject* pOther) override;

    virtual void VUnPosses() override;
    virtual void VPossessed() override;
    virtual s16 VTakeDamage(BaseGameObject* pFrom) override;
    virtual void VOnTlvCollision(relive::Path_TLV* pTlv) override;
    virtual void VOnTrapDoorOpen() override;
    virtual s32 VGetSaveState(u8* pSaveBuffer) override;

    static s32 CreateFromSaveState(const u8* pBuffer);

    void HandleDDCheat();
    void HandleBrainsAndMotions();

    s16 Brain_0_Patrol();
	s16 Brain_Patrol_State_1_IdleForAbe(BaseAliveGameObject* pObj);
	s16 Brain_Patrol_State_2_FearingAbe(BaseAliveGameObject* pObj);
	s16 Brain_Patrol_State_3_RunningFromAbe(BaseAliveGameObject* pObj);
	s16 Brain_Patrol_State_4_ApproachingAbe(BaseAliveGameObject* pObj);
	s16 Brain_Patrol_State_5_StopApproachingAbe(BaseAliveGameObject* pObj);
	s16 Brain_Patrol_State_8_StuckToWall(BaseAliveGameObject* pObj);
    s16 Brain_Patrol_State_12_Idle(BaseAliveGameObject* pObj);

    s16 Brain_1_Death();

    s16 Brain_2_ChasingAbe();
	s16 Brain_ChasingAbe_State_0_Inactive(BaseAliveGameObject* pObj);
	s16 Brain_ChasingAbe_State_1_Attacking(BaseAliveGameObject* pObj);
	s16 Brain_ChasingAbe_State_2_ToWarning();
	s16 Brain_ChasingAbe_State_3_Warning();
	s16 Brain_ChasingAbe_State_4_CloseAttack();
	s16 Brain_ChasingAbe_State_5_ToChasing(BaseAliveGameObject* pObj);
	s16 Brain_ChasingAbe_State_6_QuickAttack();
	s16 Brain_ChasingAbe_State_7_Chasing(BaseAliveGameObject* pObj);
	s16 Brain_ChasingAbe_State_8_Jumping();
	s16 Brain_ChasingAbe_State_9_TurningWhileChasing();
	s16 Brain_ChasingAbe_State_10_Turning(BaseAliveGameObject* pObj);
	s16 Brain_ChasingAbe_State_11_Walking(BaseAliveGameObject* pObj);
	s16 Brain_ChasingAbe_State_12_WalkingToHop(BaseAliveGameObject* pObj);
	s16 Brain_ChasingAbe_State_13_Eating();
	s16 Brain_ChasingAbe_State_14_Idle();
    s16 Brain_ChasingAbe_State_15_Panic();

    s16 Brain_3_SurpriseWeb();
    s16 Brain_4_Unused();

    s16 Brain_5_SpottedMeat();
	s16 Brain_SpottedMeat_State_0_Idle(Meat* pMeat);
	s16 Brain_SpottedMeat_State_1_Running(Meat* pMeat);
	s16 Brain_SpottedMeat_State_2_Walking(Meat* pMeat);
	s16 Brain_SpottedMeat_State_3_Jumping();
	s16 Brain_SpottedMeat_State_4_Turning(Meat* pMeat);
	s16 Brain_SpottedMeat_State_5_AttentiveToMeat(Meat* pMeat);
    s16 Brain_SpottedMeat_State_6_Eating(Meat* pMeat);

    s16 Brain_6_Possessed();
    s16 Brain_7_DeathDrop();
    s16 Brain_8_ControlledByGameSpeak();
    s16 Brain_9_ParamiteSpawn();

public:
    void Motion_0_Idle();
    void Motion_1_WalkBegin();
    void Motion_2_Walking();
    void Motion_3_Running();
    void Motion_4_Turn();
    void Motion_5_Hop();
    void Motion_6_Unused();
    void Motion_7_WalkRunTransition();
    void Motion_8_WalkEnd();
    void Motion_9_RunBegin();
    void Motion_10_RunEnd();
    void Motion_11_Falling();
    void Motion_12_JumpUpBegin();
    void Motion_13_JumpUpMidair();
    void Motion_14_JumpUpLand();
    void Motion_15_RopePull();
    void Motion_16_CloseAttack();
    void Motion_17_Landing();
    void Motion_18_Unused();
    void Motion_19_Knockback();
    void Motion_20_GameSpeakBegin();
    void Motion_21_PreHiss();
    void Motion_22_Hiss1();
    void Motion_23_Hiss2();
    void Motion_24_Empty();
    void Motion_25_AllOYaGameSpeakBegin();
    void Motion_26_Hiss3();
    void Motion_27_PostHiss();
    void Motion_28_GameSpeakEnd();
    void Motion_29_GetDepossessedBegin();
    void Motion_30_GetDepossessedEnd();
    void Motion_31_RunningAttack();
    void Motion_32_Empty();
    void Motion_33_SurpriseWeb();
    void Motion_34_WebLeaveDown();
    void Motion_35_WebIdle();
    void Motion_36_WebGoingUp();
    void Motion_37_WebGoingDown();
    void Motion_38_WebGrab();
    void Motion_39_WebLeaveUp();
    void Motion_40_Eating();
    void Motion_41_Death();
    void Motion_42_Squawk();
    void Motion_43_Attack();

private:
    void SetBrain(TParamiteBrainFn fn);
    bool BrainIs(TParamiteBrainFn fn);

    template<class T>
    inline void SetGamespeakNextMotion(T motion)
    {
        mParamiteGamespeakNextMotion = static_cast<s16>(motion);
    }

    eParamiteMotions GetNextMotion() const
    {
        return static_cast<eParamiteMotions>(mNextMotion);
    }
    eParamiteMotions GetCurrentMotion() const
    {
        return static_cast<eParamiteMotions>(mCurrentMotion);
    }

private:

    s16 Find_Paramite();
    void vUpdateAnim();
    Meat* FindMeat();
    s16 IsNear(Paramite* pOther);
    u8** ResBlockForMotion(s16 motion);
    s16 AnotherParamiteNear();
    PathLine* WebCollision(FP yOff, FP xOff);
    void ToHop();
    s16 CanIAcceptAGameSpeakCommand();
    s16 HandleEnemyStopper(s16 numGridBlocks);
    PullRingRope* FindPullRope();
    s16 NextPlayerInputMotion();
    s16 FindTarget();
    s16 ToNextMotion();
    void ToIdle();
    void ToKnockBack();
    void MoveOnLine();
    void CheckForPlatform();
    void HandleStopWalking();
    void HandleInputRunning();
    static s16 StableDelay();
    void Sound(ParamiteSpeak soundId, s16 pitch_min);
    void UpdateSlurgWatchPoints();
    GameSpeakEvents LastSpeak();


private:
    Guid mMeatGuid;
    Guid mWebGuid;
    Guid mTargetGuid;
    Guid mPullRingRopeGuid;
    TParamiteBrainFn mBrainState = nullptr;
    s16 mBrainSubState = 0;
    s16 mSurpriseWebDelayTimer = 0;
    s32 field_130_timer = 0;
    s16 mMeatEatingTime = 0;
    s16 mAloneChaseDelay = 0;
    s32 field_138_depossession_timer = 0;
    FP field_13C_velx_offset = {};
    Guid field_140_tlvInfo;
    s32 mGroupChaseDelay = 0;
    s32 field_148_timer = 0;
    s16 mSurpriseWebSwitchId = 0;
    EReliveLevelIds mAbeLevel = EReliveLevelIds::eNone;
    s16 mAbePath = 0;
    s16 mAbeCamera = 0;
    s32 field_154_input = 0;
    s16 field_158_next_brain_ret = 0;
    s16 mParamiteGamespeakNextMotion = 0;
    s16 field_15C_paramite_xOffset = 0;
    s32 field_160_last_event_index = 0;
    bool mHissBeforeAttack = false;
    bool mControlled = false;
    bool mRunning = false;
    bool mHissedOrLeftScreen = false;
    bool mPreventDepossession = false;
    bool mSpawned = false;
    bool mAlerted = false;
    bool mCanBePossessed = false;
    bool mAttackFleeches = false;
    bool mOutOfSight = false;
};
