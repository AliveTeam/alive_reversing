#pragma once

#include "BaseAliveGameObject.hpp"
#include "../relive_lib/FatalError.hpp"

namespace AO {

enum class EnvironmentSfx : u8;

#define ELUM_MOTIONS_ENUM(ENTRY)                \
    ENTRY(Motion_0_Respawn)              \
    ENTRY(Motion_1_Idle)                 \
    ENTRY(Motion_2_Unknown)              \
    ENTRY(Motion_3_WalkLoop)             \
    ENTRY(Motion_4_Turn)                 \
    ENTRY(Motion_5_WalkToIdle)           \
    ENTRY(Motion_6_MidWalkToIdle)        \
    ENTRY(Motion_7_IdleToWalk1)          \
    ENTRY(Motion_8_IdleToWalk2)          \
    ENTRY(Motion_9_ToYell)               \
    ENTRY(Motion_10_Yell)                \
    ENTRY(Motion_11_Unknown)             \
    ENTRY(Motion_12_RunTurn)             \
    ENTRY(Motion_13_RunTurnToWalk)       \
    ENTRY(Motion_14_Speak)               \
    ENTRY(Motion_15_Speak)               \
    ENTRY(Motion_16_Speak)               \
    ENTRY(Motion_17_WalkToHop)           \
    ENTRY(Motion_18_MidWalkToHop)        \
    ENTRY(Motion_19_Dead)                \
    ENTRY(Motion_20_Fall)                \
    ENTRY(Motion_21_Land)                \
    ENTRY(Motion_22_RunOffEdge)          \
    ENTRY(Motion_23_WalkOffEdge)         \
    ENTRY(Motion_24_JumpToFall)          \
    ENTRY(Motion_25_LickingHoney)        \
    ENTRY(Motion_26_LickingToStruggling) \
    ENTRY(Motion_27_AbeMountingEnd)      \
    ENTRY(Motion_28_AbeUnmountingEnd)    \
    ENTRY(Motion_29_BeesStruggling)      \
    ENTRY(Motion_30_HopBegin)            \
    ENTRY(Motion_31_HopMid)              \
    ENTRY(Motion_32_HopLand)             \
    ENTRY(Motion_33_RunJumpBegin)        \
    ENTRY(Motion_34_RunJumpMid)          \
    ENTRY(Motion_35_RunJumpLand)         \
    ENTRY(Motion_36_RunLoop)             \
    ENTRY(Motion_37_RunSlideStop)        \
    ENTRY(Motion_38_RunTurnToRun)        \
    ENTRY(Motion_39_IdleToRun)           \
    ENTRY(Motion_40_WalkToRun)           \
    ENTRY(Motion_41_MidWalkToRun)        \
    ENTRY(Motion_42_RunToWalk)           \
    ENTRY(Motion_43_MidRunToWalk)        \
    ENTRY(Motion_44_ScratchBegin)        \
    ENTRY(Motion_45_ScratchLoop)         \
    ENTRY(Motion_46_ScratchEnd)          \
    ENTRY(Motion_47_Unknown)             \
    ENTRY(Motion_48_AbeMoutingBegin)     \
    ENTRY(Motion_49_AbeUnmountingBegin)  \
    ENTRY(Motion_50_Knockback)

#define MAKE_ENUM(VAR) VAR,
enum class eElumMotions
{
    None_m1 = -1,
    ELUM_MOTIONS_ENUM(MAKE_ENUM)
};

enum class ElumSounds : u8
{
    eWalkingFootstep_0 = 0,
    eRunningFootstep_1 = 1,
    eHowl_2 = 2,
    eBeesStruggle_3 = 3,
    eHitGroundSoft_4 = 4,
    eRunSlide_5 = 5,
    eSpottedHoney_6 = 6,
    eExploding_7 = 7,
    eLickingHoney_8 = 8
};

class Elum final : public BaseAliveGameObject
{
public:
    static constexpr AnimId gElumMotionAnimIds[52] = {
        AnimId::Elum_Idle,
        AnimId::Elum_Idle,
        AnimId::Elum_Idle,
        AnimId::Elum_WalkLoop,
        AnimId::Elum_Turn,
        AnimId::Elum_WalkToIdle,
        AnimId::Elum_MidWalkToIdle,
        AnimId::Elum_IdleToWalk2,
        AnimId::Elum_IdleToWalk1,
        AnimId::Elum_ToYell,
        AnimId::Elum_Yell,
        AnimId::Elum_Unknown1,
        AnimId::Elum_RunTurn,
        AnimId::Elum_RunTurnToWalk,
        AnimId::Elum_Speak,
        AnimId::Elum_Speak,
        AnimId::Elum_Speak,
        AnimId::Elum_WalkToIdle,
        AnimId::Elum_MidWalkToIdle,
        AnimId::Elum_Idle,
        AnimId::Elum_Idle,
        AnimId::Elum_Land,
        AnimId::Elum_RunOffEdge,
        AnimId::Elum_WalkOffEdge,
        AnimId::Elum_JumpToFall,
        AnimId::Elum_LickingHoney,
        AnimId::Elum_LickingToStruggle,
        AnimId::Elum_AbeMountingEnd,
        AnimId::Elum_AbeUnmountingEnd,
        AnimId::Elum_BeesStruggling,
        AnimId::Elum_HopBegin,
        AnimId::Elum_HopMid,
        AnimId::Elum_HopLand,
        AnimId::Elum_RunJumpBegin,
        AnimId::Elum_RunJumpMid,
        AnimId::Elum_RunJumpLand,
        AnimId::Elum_RunLoop,
        AnimId::Elum_RunSlideStop,
        AnimId::Elum_RunTurnToRun,
        AnimId::Elum_IdleToRun,
        AnimId::Elum_WalkToRun,
        AnimId::Elum_MidWalkToRun,
        AnimId::Elum_RunToWalk,
        AnimId::Elum_MidRunToWalk,
        AnimId::Elum_ScratchBegin,
        AnimId::Elum_ScratchLoop,
        AnimId::Elum_ScratchEnd,
        AnimId::Elum_ScratchUnknown,
        AnimId::Elum_MountUnmountBegin,
        AnimId::Elum_MountUnmountBegin,
        AnimId::Elum_Knockback,
        AnimId::None};

    static void Spawn(const Guid& tlvInfo);

    virtual void VUpdate() override;
    virtual void VRender(OrderingTable& ot) override;
    virtual void VScreenChanged() override;

    Elum(const Guid& tlvInfo);
    ~Elum();

    void LoadAnimations();
    virtual void VOnTlvCollision(relive::Path_TLV* pTlv) override;
    virtual bool VTakeDamage(BaseGameObject* pFrom) override;
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

    void Vsub_416120();

    // Motions
    void Motion_0_Respawn();
    void Motion_1_Idle();
    void Motion_2_Unknown();
    void Motion_3_WalkLoop();
    void Motion_4_Turn();
    void Motion_5_WalkToIdle();
    void Motion_6_MidWalkToIdle();
    void Motion_7_IdleToWalk2();
    void Motion_8_IdleToWalk1();
    void Motion_9_ToYell();
    void Motion_10_Yell();
    void Motion_11_Unknown();
    void Motion_12_RunTurn();
    void Motion_13_RunTurnToWalk();
    void Speak(EnvironmentSfx speak, bool setYellMotion);
    void Motion_14_Speak();
    void Motion_15_Speak();
    void Motion_16_Speak();
    void Motion_17_Unknown();
    void Motion_18_Unknown();
    void Motion_19_Dead();
    void Motion_20_Fall();
    void Motion_21_Land();
    void Motion_22_RunOffEdge();
    void Motion_23_WalkOffEdge();
    void Motion_24_JumpToFall();
    void Motion_25_LickingHoney();
    void Motion_26_LickingToStruggling();
    void Motion_27_AbeMountingEnd();
    void Motion_28_AbeUnmountingEnd();
    void Motion_29_BeesStruggling();
    void Motion_30_HopBegin();
    void Motion_31_HopMid();
    void Motion_32_HopLand();
    void Motion_33_RunJumpBegin();
    void Motion_34_RunJumpMid();
    void Motion_35_RunJumpLand();
    void Motion_36_RunLoop();
    void RunSlideStopKnockback();
    void Motion_37_RunSlideStop();
    void Motion_38_RunTurnToRun();
    void Motion_39_IdleToRun();
    void Motion_40_WalkToRun();
    void Motion_41_MidWalkToRun();
    void Motion_42_RunToWalk();
    void Motion_43_MidRunToWalk();
    void Motion_44_ScratchBegin();
    void Motion_45_ScratchLoop();
    void Motion_46_ScratchEnd();

    void ToIdle();

    void Motion_47_Unknown();
    void Motion_48_AbeMoutingBegin();
    void Motion_49_AbeUnmountingBegin();
    void Motion_50_Knockback();

    // Brains
    s16 Brain_0_WithoutAbe();
    s16 Brain_1_HoneyAddiction();

private:
    void ToKnockback();
    void MidWalkToNextMotion();
    void WalkToNextMotion();
    void SlowOnX(FP amount);
    void CheckLiftPointGoneAndSetCamera();
    void MoveOnLine(s16 xLookAhead);
    void KnockAbeOffElum();
    bool ToNextMotion();
    bool ToNextMotionAbeControlled();
    void HandleElumPathTrans();
    static void Elum_SFX(ElumSounds soundId, BaseAliveGameObject* pObj);
    void FindHoney();
    bool NearHoney();

    enum class MidType
    {
        eHopMid = 0,
        eRunJumpMid = 1
    };

    void RunJumpMidAndHopMid(MidType midType);

public:

    s16 field_154_bAbeForcedDownFromElum = 0;
    s16 mBrainIdx = 0;
    bool mStrugglingWithBees = false;
    s16 mRespawnOnDead = 0;
    PSX_RECT mContinueRect = {};
    s16 mPreviousContinueZoneNumber = 0;
    s16 mAbeZoneNumber = 0;
    EReliveLevelIds mContinueLevel = EReliveLevelIds::eNone;
    s16 mContinuePath = 0;
    s16 mContinueCamera = 0;
    s16 mHoneyXPos = 0;
    s16 mDontFollowAbe = 0;
    bool mStungByBees = false;
    bool mFoundHoney = false;
    s16 field_120_bUnknown = 0;
    s16 mBrainSubState = 0;
    FP mContinueSpriteScale = {};
    bool mFalling = false; // falling straight down?
    s16 mHoneyCamera = 0;
    eElumMotions mPreviousMotion = eElumMotions::Motion_0_Respawn;
    eElumMotions mCurrentMotion = eElumMotions::Motion_0_Respawn;
    eElumMotions mNextMotion = eElumMotions::Motion_0_Respawn;

private:
    s16 field_10C_bFootStep2 = 0;
    s16 field_10E_pressed = 0;
    s32 field_110_timer = 0;
    s32 field_114_respond_timer = 0;
    FP field_118_jump_velx = {};
    s16 field_124_bShould_IdleToWalk1 = 0;
    //s16 field_126_res_idx = 0;
    s16 mHoneyYPos = 0;
    s32 field_158_last_event_idx = 0;
    bool mChangedPathNotMounted = false;
    bool mCanSpeak = false;
    bool mChangedPathMounted = false;
    Guid mTlvId; // never read
};

extern Elum* gElum;

} // namespace AO
