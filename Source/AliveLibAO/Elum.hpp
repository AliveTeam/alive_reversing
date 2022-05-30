#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
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

struct Path_ElumPathTrans final : public Path_TLV
{
    LevelIds field_18_level;
    s16 field_1A_path;
    s16 field_1C_camera;
    s16 field_1E_pad;
};
ALIVE_ASSERT_SIZEOF(Path_ElumPathTrans, 0x20);



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
    struct anythingForTheTimeBeing final
    {
        s16 field_0;
        s16 field_2;
    };

    static void Spawn(TlvItemInfoUnion tlvInfo);

    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;

    Elum(s32 a2, anythingForTheTimeBeing a3, anythingForTheTimeBeing a4, s32 a5, TlvItemInfoUnion a6);
    ~Elum();

    virtual void VOn_TLV_Collision(Path_TLV* pTlv) override;

    EXPORT void VOn_TLV_Collision_410F10(Path_TLV* pTlv);

    virtual s16 VTakeDamage(BaseGameObject* pFrom) override;

    EXPORT s16 VTakeDamage_411020(BaseGameObject* pFrom);

    virtual void VOnTrapDoorOpen() override;

    EXPORT void VOnTrapDoorOpen_412700();

    // TODO: Add new virtuals
    EXPORT void VLoadUnmountedResources_411260();

    EXPORT void VFreeMountedResources_411200();

    EXPORT void Vsub_416120();

    EXPORT void VLoadMountedResources_411300();

    EXPORT void VFreeUnmountedResources_4112B0();

    void ToKnockback();

    EXPORT u8** GetResBlock_410D00(s16 currentMotion);

    EXPORT void MidWalkToNextMotion_412FA0();

    EXPORT void WalkToNextMotion_4130D0();

    EXPORT void SlowOnX_414210(FP amount);

    void CheckLiftPointGoneAndSetCamera();

    EXPORT void MoveOnLine_412580(s16 xLookAhead);

    EXPORT void SetAbeAsPlayer_412520(s16 abeMotion);

    EXPORT s16 ToNextMotion_4120F0();

    EXPORT s16 ToNextMotionAbeControlled_411E40();

    EXPORT void HandleElumPathTrans_411460();

    EXPORT static void CC Elum_SFX_416E10(ElumSounds soundId, BaseAliveGameObject* pObj);

    EXPORT void FindHoney_411600();

    EXPORT s16 NearHoney_411DA0();

    // Brains
    EXPORT s16 Brain_0_WithoutAbe_416190();
    EXPORT s16 Brain_1_HoneyAddiction_411730();

    enum class MidType
    {
        eHopMid = 0,
        eRunJumpMid = 1
    };

    void RunJumpMidAndHopMid(MidType midType);

    // Motions
    EXPORT void Motion_0_Respawn_414C60();
    EXPORT void Motion_1_Idle_412990();
    EXPORT void Motion_2_Unknown_412C30();
    EXPORT void Motion_3_WalkLoop_412C90();
    EXPORT void Motion_4_Turn_4140F0();
    EXPORT void Motion_5_WalkToIdle_4132D0();
    EXPORT void Motion_6_MidWalkToIdle_4133F0();
    EXPORT void Motion_7_IdleToWalk2_413200();
    EXPORT void Motion_8_IdleToWalk1_413270();
    EXPORT void Motion_9_ToYell_415890();
    EXPORT void Motion_10_Yell_4158E0();
    EXPORT void Motion_11_Unknown_4159A0();
    EXPORT void Motion_12_RunTurn_414520();
    EXPORT void Motion_13_RunTurnToWalk_4147C0();
    EXPORT void Motion_14_Speak_414860();
    EXPORT void Motion_15_Speak_4148F0();
    EXPORT void Motion_16_Speak_414980();
    EXPORT void Motion_17_Unknown_413620();
    EXPORT void Motion_18_Unknown_4136A0();
    EXPORT void Motion_19_Dead_415F90();
    EXPORT void Motion_20_Fall_415F70();
    EXPORT void Motion_21_Land_414A20();
    EXPORT void Motion_22_RunOffEdge_415810();
    EXPORT void Motion_23_WalkOffEdge_415E90();
    EXPORT void Motion_24_JumpToFall_415ED0();
    EXPORT void Motion_25_LickingHoney_415B50();
    EXPORT void Motion_26_LickingToStruggling_415AC0();
    EXPORT void Motion_27_AbeMountingEnd_415CA0();
    EXPORT void Motion_28_AbeUnmountingEnd_415D60();
    EXPORT void Motion_29_BeesStruggling_412A90();
    EXPORT void Motion_30_HopBegin_414E30();
    EXPORT void Motion_31_HopMid_414C70();
    EXPORT void Motion_32_HopLand_415140();
    EXPORT void Motion_33_RunJumpBegin_415400();
    EXPORT void Motion_34_RunJumpMid_415240();
    EXPORT void Motion_35_RunJumpLand_415580();
    EXPORT void Motion_36_RunLoop_413720();
    void RunSlideStopKnockback();
    EXPORT void Motion_37_RunSlideStop_4142E0();
    EXPORT void Motion_38_RunTurnToRun_414810();
    EXPORT void Motion_39_IdleToRun_413B00();
    EXPORT void Motion_40_WalkToRun_4134B0();
    EXPORT void Motion_41_MidWalkToRun_413560();
    EXPORT void Motion_42_RunToWalk_413B60();
    EXPORT void Motion_43_MidRunToWalk_413E20();
    EXPORT void Motion_44_ScratchBegin_412730();
    EXPORT void Motion_45_ScratchLoop_4127B0();
    EXPORT void Motion_46_ScratchEnd_412800();

    void ToIdle();

    EXPORT void Motion_47_Unknown_415A30();
    EXPORT void Motion_48_AbeMoutingBegin_415C40();
    EXPORT void Motion_49_AbeUnmountingBegin_415D00();
    EXPORT void Motion_50_Knockback_415DC0();

    struct ElumResources final
    {
        u8** res[31];
    };

    s16 field_10C_bFootStep2;
    s16 field_10E_pressed;
    s32 field_110_timer;
    s32 field_114_respond_timer;
    FP field_118_jump_velx;
    s32 field_11C_padding;
    s16 field_120_bUnknown;
    s16 field_122_bDontFollowAbe;
    s16 field_124_bShould_IdleToWalk1;
    s16 field_126_res_idx;
    s16 field_128_brain_idx;
    s16 field_12A_brain_sub_state;
    s16 field_12C_honey_xpos;
    s16 field_12E_honey_ypos;
    s32 field_130_unused;
    s32 field_134_padding;
    PSX_RECT field_138_continue_rect;
    s16 field_140_continue_zone_number;
    s16 field_142_zone_number;
    s16 field_144_bRespawnOnDead;
    s16 field_146_honey_ypos;
    s16 field_148_continue_path;
    LevelIds field_14A_continue_level;
    s16 field_14C_continue_camera;
    s16 field_14E_padding;
    FP field_150_continue_sprite_scale;
    s16 field_154_bAbeForcedDownFromElum;
    s16 field_156_padding;
    s32 field_158_last_event_idx;
    s32 field_15C_padding;
    s32 field_160_padding;
    s32 field_164_padding;
    s32 field_168_padding;
    s16 field_16C_never_read;
    s16 field_16E_never_read;
    BitField16<Flags_170> field_170_flags;
    s16 field_172_padding;
    ElumResources field_174_resources;
    s32 field_1F0_tlvInfo;
};
ALIVE_ASSERT_SIZEOF(Elum, 0x1F4);

ALIVE_VAR_EXTERN(Elum*, gElum_507680);

} // namespace AO
