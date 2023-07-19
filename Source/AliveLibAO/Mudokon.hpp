#pragma once

#include "BaseAliveGameObject.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"

namespace AO {

#define MUD_STATES_ENUM(ENTRY)                   \
    ENTRY(Motion_0_Idle)                   \
    ENTRY(Motion_1_WalkLoop)               \
    ENTRY(Motion_2_StandingTurn)           \
    ENTRY(Motion_3_Speak)                  \
    ENTRY(Motion_4_Speak)                  \
    ENTRY(Motion_5_Speak)                  \
    ENTRY(Motion_6_Speak)                  \
    ENTRY(Motion_7_WalkBegin)              \
    ENTRY(Motion_8_WalkToIdle)             \
    ENTRY(Motion_9_MidWalkToIdle)          \
    ENTRY(Motion_10_Unused)                \
    ENTRY(Motion_11_Null)                  \
    ENTRY(Motion_12_LiftUse)               \
    ENTRY(Motion_13_LiftGrabBegin)         \
    ENTRY(Motion_14_LiftGrabEnd)           \
    ENTRY(Motion_15_LeverUse)              \
    ENTRY(Motion_16_StandScrubLoop)        \
    ENTRY(Motion_17_StandScrubLoopToPause) \
    ENTRY(Motion_18_StandScrubPauseToLoop) \
    ENTRY(Motion_19_StandScrubPause)       \
    ENTRY(Motion_20_IdleToStandScrub)      \
    ENTRY(Motion_21_StandScrubToIdle)      \
    ENTRY(Motion_22_CrouchScrub)           \
    ENTRY(Motion_23_CrouchIdle)            \
    ENTRY(Motion_24_CrouchTurn)            \
    ENTRY(Motion_25_StandToCrouch)         \
    ENTRY(Motion_26_CrouchToStand)         \
    ENTRY(Motion_27_WalkToRun)             \
    ENTRY(Motion_28_MidWalkToRun)          \
    ENTRY(Motion_29_RunLoop)               \
    ENTRY(Motion_30_RunToWalk)             \
    ENTRY(Motion_31_MidRunToWalk)          \
    ENTRY(Motion_32_RunSlideStop)          \
    ENTRY(Motion_33_RunSlideTurn)          \
    ENTRY(Motion_34_RunTurnToRun)          \
    ENTRY(Motion_35_SneakLoop)             \
    ENTRY(Motion_36_MidWalkToSneak)           \
    ENTRY(Motion_37_SneakToWalk)           \
    ENTRY(Motion_38_WalkToSneak)        \
    ENTRY(Motion_39_MidSneakToWalk)        \
    ENTRY(Motion_40_SneakBegin)            \
    ENTRY(Motion_41_SneakToIdle)           \
    ENTRY(Motion_42_MidSneakToIdle)        \
    ENTRY(Motion_43_RunJumpBegin)             \
    ENTRY(Motion_44_RunJumpMid)               \
    ENTRY(Motion_45_StandToRun)         \
    ENTRY(Motion_46_FallLandDie)           \
    ENTRY(Motion_47_Knockback)             \
    ENTRY(Motion_48_KnockbackGetUp)        \
    ENTRY(Motion_49_WalkOffEdge)            \
    ENTRY(Motion_50_LandSoft)              \
    ENTRY(Motion_51_Fall)                  \
    ENTRY(Motion_52_Chant)                 \
    ENTRY(Motion_53_ChantEnd)              \
    ENTRY(Motion_54_ToDuck)                \
    ENTRY(Motion_55_Duck)                  \
    ENTRY(Motion_56_DuckToCrouch)          \
    ENTRY(Motion_57_Struggle)              \
    ENTRY(Motion_58_StruggleToCrouchChant) \
    ENTRY(Motion_59_CrouchChant)           \
    ENTRY(Motion_60_CrouchChantToStruggle)  \
    ENTRY(Motion_61_DuckKnockback)         \
    ENTRY(Motion_62_PoisonGasDeath)

#define MAKE_ENUM(VAR) VAR,
enum class eMudMotions : s32
{
    MUD_STATES_ENUM(MAKE_ENUM)
};

class LiftPoint;
class BirdPortal;

struct Mudokon_Resources final
{
    u8** res[15];
};

enum class GameSpeakEvents : s16;

class Mudokon final : public BaseAliveGameObject
{
public:
    static constexpr AnimId sMudMotionAnimIds[64] = {
        AnimId::Mudokon_Idle,
        AnimId::Mudokon_Walk,
        AnimId::Mudokon_StandingTurn,
        AnimId::Mudokon_Speak1,
        AnimId::Mudokon_Speak2,
        AnimId::Mudokon_Speak3,
        AnimId::Mudokon_SpeakFart,
        AnimId::Mudokon_WalkBegin,
        AnimId::Mudokon_WalkToIdle,
        AnimId::Mudokon_MidWalkToIdle,
        AnimId::Mudokon_Idle,
        AnimId::Mudokon_LiftUseUp,
        AnimId::Mudokon_LiftUseDown,
        AnimId::Mudokon_LiftGrabBegin,
        AnimId::Mudokon_LiftGrabEnd,
        AnimId::Mudokon_LeverUse,
        AnimId::Mudokon_StandScrubLoop,
        AnimId::Mudokon_StandScrubLoopToPause,
        AnimId::Mudokon_StandScrubPauseToLoop,
        AnimId::Mudokon_StandScrubPause,
        AnimId::Mudokon_IdleToStandScrub,
        AnimId::Mudokon_StandScrubToIdle,
        AnimId::Mudokon_CrouchScrub,
        AnimId::Mudokon_CrouchIdle,
        AnimId::Mudokon_CrouchTurn,
        AnimId::Mudokon_StandToCrouch,
        AnimId::Mudokon_CrouchToStand,
        AnimId::Mudokon_WalkToRun,
        AnimId::Mudokon_MidWalkToRun,
        AnimId::Mudokon_Run,
        AnimId::Mudokon_RunToWalk,
        AnimId::Mudokon_MidRunToWalk,
        AnimId::Mudokon_RunSlideStop,
        AnimId::Mudokon_RunSlideTurn,
        AnimId::Mudokon_RunTurnToRun,
        AnimId::Mudokon_Sneak,
        AnimId::Mudokon_WalkToSneak,
        AnimId::Mudokon_SneakToWalk,
        AnimId::Mudokon_MidWalkToSneak,
        AnimId::Mudokon_MidSneakToWalk,
        AnimId::Mudokon_SneakBegin,
        AnimId::Mudokon_SneakToIdle,
        AnimId::Mudokon_MidSneakToIdle,
        AnimId::Mudokon_RunJumpBegin,
        AnimId::Mudokon_RunJumpMid,
        AnimId::Mudokon_StandToRun,
        AnimId::Mudokon_KnockForward,
        AnimId::Mudokon_Knockback,
        AnimId::Mudokon_KnockbackGetUp,
        AnimId::Mudokon_WalkOffEdge,
        AnimId::Mudokon_AO_LandSoft_Long,
        AnimId::Mudokon_FallingFromGrab, // check/rename
        AnimId::Mudokon_Chant,
        AnimId::Mudokon_ChantEnd,
        AnimId::Mudokon_ToDuck,
        AnimId::Mudokon_Duck,
        AnimId::Mudokon_DuckToCrouch,
        AnimId::Mudokon_Struggle,
        AnimId::Mudokon_StruggleToCrouchChant,
        AnimId::Mudokon_CrouchChant,
        AnimId::Mudokon_CrouchChantToStruggle,
        AnimId::Mudokon_DuckKnockback,
        AnimId::Mudokon_PoisonGasDeath,
        AnimId::None};

    Mudokon(relive::Path_TLV* pTlv, const Guid& tlvId);
    ~Mudokon();

    void LoadAnimations();

    void KillLiftPoint_194();

    virtual void VUpdate() override;
    virtual void VOnTrapDoorOpen() override;
    virtual void VOnTlvCollision(relive::Path_TLV* pTlv) override;
    virtual void VScreenChanged() override;
    virtual bool VTakeDamage(BaseGameObject* pFrom) override;

    virtual void VUpdateResBlock();

    eMudMotions GetNextMotion() const
    {
        return static_cast<eMudMotions>(mNextMotion);
    }
    eMudMotions GetCurrentMotion() const
    {
        return static_cast<eMudMotions>(mCurrentMotion);
    }
    eMudMotions GetPreviousMotion() const
    {
        return static_cast<eMudMotions>(mPreviousMotion);
    }

    bool DoSmashDamage();
    void KillBirdPortal();
    u8** GetResBlockForMotion(s16 motion);
    void DoPathTrans();
    void ToStand();
    void CheckFloorGone();
    static s16 CanAbeSneak(Mudokon* pMud); // TODO: unused parameter
    void ToKnockback();
    void ReduceXVelocityBy(FP amount);
    void MoveOnLine();
    s16 FindBirdPortal();
    s16 FacingBirdPortal(BirdPortal* pTarget);
    GameSpeakEvents LastGameSpeak();
    void AddAlerted();
    void RemoveAlerted();
    s16 IAmNearestToAbe();

    // Motions
    void Motion_0_Idle();
    void Motion_1_WalkLoop();
    void Motion_2_StandingTurn();
    void Motion_3_Speak();

    // Not exported as same func as 3
    void Motion_4_Speak();
    void Motion_5_Speak();
    void Motion_6_Speak();

    void Motion_7_WalkBegin();
    void Motion_8_WalkToIdle();
    void Motion_9_MidWalkToIdle();
    void Motion_10_Unused();
    void Motion_11_Null();
    void Motion_12_LiftUse();
    void Motion_13_LiftGrabBegin();
    void Motion_14_LiftGrabEnd();
    void Motion_15_LeverUse();
    void Motion_16_StandScrubLoop();
    void Motion_17_StandScrubLoopToPause();
    void Motion_18_StandScrubPauseToLoop();
    void Motion_19_StandScrubPause();
    void Motion_20_IdleToStandScrub();
    void Motion_21_StandScrubToIdle();
    void Motion_22_CrouchScrub();
    void Motion_23_CrouchIdle();
    void Motion_24_CrouchTurn();
    void Motion_25_StandToCrouch();
    void Motion_26_CrouchToStand();
    void Motion_27_WalkToRun();
    void Motion_28_MidWalkToRun();
    void Motion_29_RunLoop();
    void Motion_30_RunToWalk();
    void Motion_31_MidRunToWalk();
    void Motion_32_RunSlideStop();
    void Motion_33_RunSlideTurn();
    void Motion_34_RunTurnToRun();
	void Motion_35_SneakLoop();
	void Motion_36_MidWalkToSneak();
	void Motion_37_SneakToWalk();
	void Motion_38_WalkToSneak();
	void Motion_39_MidSneakToWalk();
	void Motion_40_SneakBegin();
	void Motion_41_SneakToIdle();
    void Motion_42_MidSneakToIdle();
    void Motion_43_RunJumpBegin();
    void Motion_44_RunJumpMid();
    void Motion_45_StandToRun();
    void Motion_46_FallLandDie();
    void Motion_47_Knockback();
    void Motion_48_KnockbackGetUp();
    void Motion_49_WalkOffEdge();
    void Motion_50_LandSoft();
    void Motion_51_Fall();
    void Motion_52_Chant();
    void Motion_53_ChantEnd();
    void Motion_54_ToDuck();
    void Motion_55_Duck();
    void Motion_56_DuckToCrouch();
    void Motion_57_Struggle();
    void Motion_58_StruggleToCrouchChant();
    void Motion_59_CrouchChant();
    void Motion_60_CrouchChantToStruggle();
    void Motion_61_DuckKnockback();
    void Motion_62_PoisonGasDeath();

    // Brains
    s16 Brain_0_ComingIn();
    s16 Brain_1_ComingOut();
    s16 Brain_2_SingSequenceIdle();
    s16 Brain_3_SingSequenceSing();
    s16 Brain_4_SingSequencePassword();
    s16 Brain_5_LiftUse();
    s16 Brain_6_LeverUse();
    s16 Brain_7_GiveRings();
    s16 Brain_8_StandScrub();
    s16 Brain_9_CrouchScrub();
    s16 Brain_10_ListeningToAbe();
    s16 Brain_11_ShrivelDeath();
    s16 Brain_12_Escape();
    s16 Brain_13_FallAndSmackDeath();
    s16 Brain_14_Chant();
    s16 Brain_15_Choke();

    Guid mTlvId;
    s16 field_110_lift_switch_id = 0;
    s32 field_114 = 0;
    s32 field_118 = 0;
    FP field_11C = {};
    s16 field_124_voice_pitch = 0;
    s16 field_126_input = 0;
    s32 field_128 = 0;
    u8 field_12C[16] = {};
    s16 field_13C = 0;
    s16 field_13E = 0;
    bool mBit2_Unknown = false;
    bool mSnapToGrid = false;
    bool mPersist = false;
    bool mBit7_Unknown = false;
    bool mBit8_Unknown = false;
    bool mAlerted = false;
    bool mGiveRingWithoutPassword = false;
    bool mDeaf = false;
    Mudokon_Resources field_148_res_array = {};
    s16 field_184 = 0;
    relive::reliveChoice mGivePassword = relive::reliveChoice::eNo;
    s16 field_188 = 0;
    FP field_18C_how_far_to_walk = {};
    FP field_190 = {};
    LiftPoint* field_194_pLiftPoint = nullptr;
    s16 field_198_abe_must_face_mud = 0;
    s16 field_19E_code_idx = 0;
    s16 field_1A0 = 0;
    s32 field_1A4_code_converted = 0;
    s16 field_1A8_code_length = 0;
    u16 field_1AA_ring_timeout = 0;
    BirdPortal* field_1AC_pBirdPortal = nullptr;
    s16 field_1B0 = 0;
    s16 field_1B2_rescue_switch_id = 0;
    s16 field_1B4_idle_time = 0;
    s16 field_1B6 = 0;
    s16 field_1B8_brain_state = 0;
    s16 field_1BA_brain_sub_state = 0;
    s16 field_1BC = 0;
    s32 field_1C0_timer = 0;
    s16 field_1C4_bDoPathTrans = 0;
};

} // namespace AO
