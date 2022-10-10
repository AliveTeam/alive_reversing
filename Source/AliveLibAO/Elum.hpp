#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseAliveGameObject.hpp"
#include "Factory.hpp"
#include "Map.hpp"

namespace AO {

#define ELUM_MOTIONS_ENUM(ENTRY)                \
    ENTRY(Motion_0_Respawn_414C60)              \
    ENTRY(Motion_1_Idle_412990)                 \
    ENTRY(Motion_2_Unknown_412C30)              \
    ENTRY(Motion_3_WalkLoop_412C90)             \
    ENTRY(Motion_4_Turn_4140F0)                 \
    ENTRY(Motion_5_WalkToIdle_4132D0)           \
    ENTRY(Motion_6_MidWalkToIdle_4133F0)        \
    ENTRY(Motion_7_IdleToWalk1_413200)          \
    ENTRY(Motion_8_IdleToWalk2_413270)          \
    ENTRY(Motion_9_ToYell_415890)               \
    ENTRY(Motion_10_Yell_4158E0)                \
    ENTRY(Motion_11_Unknown_4159A0)             \
    ENTRY(Motion_12_RunTurn_414520)             \
    ENTRY(Motion_13_RunTurnToWalk_4147C0)       \
    ENTRY(Motion_14_Speak_414860)               \
    ENTRY(Motion_15_Speak_4148F0)               \
    ENTRY(Motion_16_Speak_414980)               \
    ENTRY(Motion_17_WalkToHop_413620)           \
    ENTRY(Motion_18_MidWalkToHop_4136A0)        \
    ENTRY(Motion_19_Dead_415F90)                \
    ENTRY(Motion_20_Fall_415F70)                \
    ENTRY(Motion_21_Land_414A20)                \
    ENTRY(Motion_22_RunOffEdge_415810)          \
    ENTRY(Motion_23_WalkOffEdge_415E90)         \
    ENTRY(Motion_24_JumpToFall_415ED0)          \
    ENTRY(Motion_25_LickingHoney_415B50)        \
    ENTRY(Motion_26_LickingToStruggling_415AC0) \
    ENTRY(Motion_27_AbeMountingEnd_415CA0)      \
    ENTRY(Motion_28_AbeUnmountingEnd_415D60)    \
    ENTRY(Motion_29_BeesStruggling_412A90)      \
    ENTRY(Motion_30_HopBegin_414E30)            \
    ENTRY(Motion_31_HopMid_414C70)              \
    ENTRY(Motion_32_HopLand_415140)             \
    ENTRY(Motion_33_RunJumpBegin_415400)        \
    ENTRY(Motion_34_RunJumpMid_415240)          \
    ENTRY(Motion_35_RunJumpLand_415580)         \
    ENTRY(Motion_36_RunLoop_413720)             \
    ENTRY(Motion_37_RunSlideStop_4142E0)        \
    ENTRY(Motion_38_RunTurnToRun_414810)        \
    ENTRY(Motion_39_IdleToRun_413B00)           \
    ENTRY(Motion_40_WalkToRun_4134B0)           \
    ENTRY(Motion_41_MidWalkToRun_413560)        \
    ENTRY(Motion_42_RunToWalk_413B60)           \
    ENTRY(Motion_43_MidRunToWalk_413E20)        \
    ENTRY(Motion_44_ScratchBegin_412730)        \
    ENTRY(Motion_45_ScratchLoop_4127B0)         \
    ENTRY(Motion_46_ScratchEnd_412800)          \
    ENTRY(Motion_47_Unknown_415A30)             \
    ENTRY(Motion_48_AbeMoutingBegin_415C40)     \
    ENTRY(Motion_49_AbeUnmountingBegin_415D00)  \
    ENTRY(Motion_50_Knockback_415DC0)

#define MAKE_ENUM(VAR) VAR,
enum eElumMotions : s32
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
public:
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
    void MoveOnLine_412580(s16 xLookAhead);
    void SetAbeAsPlayer_412520(s16 abeMotion);
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
    void Motion_0_Respawn_414C60();
    void Motion_1_Idle_412990();
    void Motion_2_Unknown_412C30();
    void Motion_3_WalkLoop_412C90();
    void Motion_4_Turn_4140F0();
    void Motion_5_WalkToIdle_4132D0();
    void Motion_6_MidWalkToIdle_4133F0();
    void Motion_7_IdleToWalk2_413200();
    void Motion_8_IdleToWalk1_413270();
    void Motion_9_ToYell_415890();
    void Motion_10_Yell_4158E0();
    void Motion_11_Unknown_4159A0();
    void Motion_12_RunTurn_414520();
    void Motion_13_RunTurnToWalk_4147C0();
    void Motion_14_Speak_414860();
    void Motion_15_Speak_4148F0();
    void Motion_16_Speak_414980();
    void Motion_17_Unknown_413620();
    void Motion_18_Unknown_4136A0();
    void Motion_19_Dead_415F90();
    void Motion_20_Fall_415F70();
    void Motion_21_Land_414A20();
    void Motion_22_RunOffEdge_415810();
    void Motion_23_WalkOffEdge_415E90();
    void Motion_24_JumpToFall_415ED0();
    void Motion_25_LickingHoney_415B50();
    void Motion_26_LickingToStruggling_415AC0();
    void Motion_27_AbeMountingEnd_415CA0();
    void Motion_28_AbeUnmountingEnd_415D60();
    void Motion_29_BeesStruggling_412A90();
    void Motion_30_HopBegin_414E30();
    void Motion_31_HopMid_414C70();
    void Motion_32_HopLand_415140();
    void Motion_33_RunJumpBegin_415400();
    void Motion_34_RunJumpMid_415240();
    void Motion_35_RunJumpLand_415580();
    void Motion_36_RunLoop_413720();
    void RunSlideStopKnockback();
    void Motion_37_RunSlideStop_4142E0();
    void Motion_38_RunTurnToRun_414810();
    void Motion_39_IdleToRun_413B00();
    void Motion_40_WalkToRun_4134B0();
    void Motion_41_MidWalkToRun_413560();
    void Motion_42_RunToWalk_413B60();
    void Motion_43_MidRunToWalk_413E20();
    void Motion_44_ScratchBegin_412730();
    void Motion_45_ScratchLoop_4127B0();
    void Motion_46_ScratchEnd_412800();

    void ToIdle();

    void Motion_47_Unknown_415A30();
    void Motion_48_AbeMoutingBegin_415C40();
    void Motion_49_AbeUnmountingBegin_415D00();
    void Motion_50_Knockback_415DC0();

    s16 field_10C_bFootStep2 = 0;
    s16 field_10E_pressed = 0;
    s32 field_110_timer = 0;
    s32 field_114_respond_timer = 0;
    FP field_118_jump_velx = {};
    s16 field_120_bUnknown = 0;
    s16 mDontFollowAbe = 0;
    s16 field_124_bShould_IdleToWalk1 = 0;
    //s16 field_126_res_idx = 0;
    s16 field_128_brain_idx = 0;
    s16 field_12A_brain_sub_state = 0;
    s16 field_12C_honey_xpos = 0;
    s16 field_12E_honey_ypos = 0;
    s32 field_130_unused = 0;
    PSX_RECT mContinueRect = {};
    s16 mContinueZoneNumber = 0;
    s16 field_142_zone_number = 0;
    s16 mRespawnOnDead = 0;
    s16 field_146_honey_ypos = 0;
    s16 mContinuePath = 0;
    EReliveLevelIds mContinueLevel = EReliveLevelIds::eNone;
    s16 mContinueCamera = 0;
    FP mContinueSpriteScale = {};
    s16 field_154_bAbeForcedDownFromElum = 0;
    s32 field_158_last_event_idx = 0;
    s16 field_16C_never_read = 0;
    s16 field_16E_never_read = 0;
    BitField16<Flags_170> field_170_flags = {};
    Guid field_1F0_tlvInfo;
};
ALIVE_ASSERT_SIZEOF(Elum, 0x1F4);

extern Elum* gElum;

} // namespace AO
