#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseAliveGameObject.hpp"
#include "Factory.hpp"
#include "Map.hpp"

namespace AO {

#define ELUM_MOTIONS_ENUM(ENTRY)                \
    ENTRY(Motion_0_Respawn)              \
    ENTRY(Motion_1_Idle)                 \
    ENTRY(Motion_2_Unknown)              \
    ENTRY(Motion_3_WalkLoop)             \
    ENTRY(Motion_4_Turn)                 \
    ENTRY(Motion_5_WalkToIdle)           \
    ENTRY(Motion_6_MidWalkToIdle)        \
    ENTRY(Motion_7_IdleToWalk1_413200)          \
    ENTRY(Motion_8_IdleToWalk2_413270)          \
    ENTRY(Motion_9_ToYell)               \
    ENTRY(Motion_10_Yell)                \
    ENTRY(Motion_11_Unknown)             \
    ENTRY(Motion_12_RunTurn)             \
    ENTRY(Motion_13_RunTurnToWalk)       \
    ENTRY(Motion_14_Speak)               \
    ENTRY(Motion_15_Speak)               \
    ENTRY(Motion_16_Speak)               \
    ENTRY(Motion_17_WalkToHop_413620)           \
    ENTRY(Motion_18_MidWalkToHop_4136A0)        \
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
enum class eElumMotions : s32
{
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
private:
    eElumMotions GetNextMotion() const
    {
        return static_cast<eElumMotions>(mNextMotion);
    }

public:
    eElumMotions GetCurrentMotion() const
    {
        return static_cast<eElumMotions>(mCurrentMotion);
    }

    enum Flags_170
    {
        eStrugglingWithBees_Bit1 = 1,
        eStungByBees_Bit2 = 2,
        eFalling_Bit3 = 4, //falling straight down?
        eFoundHoney_Bit4 = 8,
        eChangedPathNotMounted_Bit5 = 16,
        eCanSpeak_Bit6 = 32,
        eChangedPathMounted_Bit7 = 64,
    };

    static void Spawn(const Guid& tlvInfo);

    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;

    Elum(const Guid& a6);
    ~Elum();

    void LoadAnimations();
    virtual void VOnTlvCollision(relive::Path_TLV* pTlv) override;
    virtual s16 VTakeDamage(BaseGameObject* pFrom) override;
    virtual void VOnTrapDoorOpen() override;

    void Vsub_416120();
    void ToKnockback();
    void MidWalkToNextMotion_412FA0();
    void WalkToNextMotion_4130D0();
    void SlowOnX_414210(FP amount);
    void CheckLiftPointGoneAndSetCamera();
    void MoveOnLine(s16 xLookAhead);
    void SetAbeAsPlayer(s16 abeMotion);
    s16 ToNextMotion_4120F0();
    s16 ToNextMotionAbeControlled_411E40();
    void HandleElumPathTrans_411460();
    static void Elum_SFX_416E10(ElumSounds soundId, BaseAliveGameObject* pObj);
    void FindHoney_411600();
    s16 NearHoney_411DA0();

    // Brains
    s16 Brain_0_WithoutAbe_416190();
    s16 Brain_1_HoneyAddiction_411730();

    enum class MidType
    {
        eHopMid = 0,
        eRunJumpMid = 1
    };

    void RunJumpMidAndHopMid(MidType midType);

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

    s16 field_10C_bFootStep2 = 0;
    s16 field_10E_pressed = 0;
    s32 field_110_timer = 0;
    s32 field_114_respond_timer = 0;
    FP field_118_jump_velx = {};
    s16 field_120_bUnknown = 0;
    s16 mDontFollowAbe = 0;
    s16 field_124_bShould_IdleToWalk1 = 0;
    //s16 field_126_res_idx = 0;
    s16 mBrainIdx = 0;
    s16 mBrainSubState = 0;
    s16 field_12C_honey_xpos = 0;
    s16 field_12E_honey_ypos = 0;
    PSX_RECT mContinueRect = {};
    s16 mPreviousContinueZoneNumber = 0;
    s16 mAbeZoneNumber = 0;
    s16 mRespawnOnDead = 0;
    s16 field_146_honey_ypos = 0;
    s16 mContinuePath = 0;
    EReliveLevelIds mContinueLevel = EReliveLevelIds::eNone;
    s16 mContinueCamera = 0;
    FP mContinueSpriteScale = {};
    s16 field_154_bAbeForcedDownFromElum = 0;
    s32 field_158_last_event_idx = 0;
    BitField16<Flags_170> field_170_flags = {};
    Guid mTlvId; // never read
};

extern Elum* gElum;

} // namespace AO
