#pragma once

#include "FunctionFwd.hpp"
#include "BaseAliveGameObject.hpp"
#include "Factory.hpp"
#include "Map.hpp"

namespace AO {

#define ELUM_STATES_ENUM(ENTRY)                \
    ENTRY(State_0_Respawn_414C60)              \
    ENTRY(State_1_Idle_412990)                 \
    ENTRY(State_2_Unknown_412C30)              \
    ENTRY(State_3_WalkLoop_412C90)             \
    ENTRY(State_4_Turn_4140F0)                 \
    ENTRY(State_5_WalkToIdle_4132D0)           \
    ENTRY(State_6_MidWalkToIdle_4133F0)        \
    ENTRY(State_7_IdleToWalk1_413200)          \
    ENTRY(State_8_IdleToWalk2_413270)          \
    ENTRY(State_9_ToYell_415890)               \
    ENTRY(State_10_Yell_4158E0)                \
    ENTRY(State_11_Unknown_4159A0)             \
    ENTRY(State_12_RunTurn_414520)             \
    ENTRY(State_13_RunTurnToWalk_4147C0)       \
    ENTRY(State_14_Speak_414860)               \
    ENTRY(State_15_Speak_4148F0)               \
    ENTRY(State_16_Speak_414980)               \
    ENTRY(State_17_WalkToHop_413620)           \
    ENTRY(State_18_MidWalkToHop_4136A0)        \
    ENTRY(State_19_Dead_415F90)                \
    ENTRY(State_20_Fall_415F70)                \
    ENTRY(State_21_Land_414A20)                \
    ENTRY(State_22_RunOffEdge_415810)          \
    ENTRY(State_23_WalkOffEdge_415E90)         \
    ENTRY(State_24_JumpToFall_415ED0)          \
    ENTRY(State_25_LickingHoney_415B50)        \
    ENTRY(State_26_LickingToStruggling_415AC0) \
    ENTRY(State_27_AbeMountingEnd_415CA0)      \
    ENTRY(State_28_AbeUnmountingEnd_415D60)    \
    ENTRY(State_29_BeesStruggling_412A90)      \
    ENTRY(State_30_HopBegin_414E30)            \
    ENTRY(State_31_HopMid_414C70)              \
    ENTRY(State_32_HopLand_415140)             \
    ENTRY(State_33_RunJumpBegin_415400)        \
    ENTRY(State_34_RunJumpMid_415240)          \
    ENTRY(State_35_RunJumpLand_415580)         \
    ENTRY(State_36_RunLoop_413720)             \
    ENTRY(State_37_RunSlideStop_4142E0)        \
    ENTRY(State_38_RunTurnToRun_414810)        \
    ENTRY(State_39_IdleToRun_413B00)           \
    ENTRY(State_40_WalkToRun_4134B0)           \
    ENTRY(State_41_MidWalkToRun_413560)        \
    ENTRY(State_42_RunToWalk_413B60)           \
    ENTRY(State_43_MidRunToWalk_413E20)        \
    ENTRY(State_44_ScratchBegin_412730)        \
    ENTRY(State_45_ScratchLoop_4127B0)         \
    ENTRY(State_46_ScratchEnd_412800)          \
    ENTRY(State_47_Unknown_415A30)             \
    ENTRY(State_48_AbeMoutingBegin_415C40)     \
    ENTRY(State_49_AbeUnmountingBegin_415D00)  \
    ENTRY(State_50_Knockback_415DC0)

#define MAKE_ENUM(VAR) VAR,
enum eElumStates : s32
{
    ELUM_STATES_ENUM(MAKE_ENUM)
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

struct Path_ElumPathTrans : public Path_TLV
{
    LevelIds field_18_level;
    s16 field_1A_path;
    s16 field_1C_camera;
    s16 field_1E_pad;
};
ALIVE_ASSERT_SIZEOF(Path_ElumPathTrans, 0x20);



class Elum : public BaseAliveGameObject
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
    struct anythingForTheTimeBeing
    {
        s16 field_0;
        s16 field_2;
    };

    EXPORT Elum* Vdtor_411710(s32 flags);
    EXPORT void VUpdate_4102A0();
    EXPORT void VRender_410E40(PrimHeader** ppOt);
    EXPORT void vScreenChange_411340();

    EXPORT static void Spawn_410E90(TlvItemInfoUnion tlvInfo);

    virtual BaseGameObject* VDestructor(s32 flags) override;
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;

    EXPORT Elum* ctor_410870(s32 a2, anythingForTheTimeBeing a3, anythingForTheTimeBeing a4, s32 a5, TlvItemInfoUnion a6);
    EXPORT BaseGameObject* dtor_410BC0();

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

    EXPORT BaseGameObject* dtor_base_416FE0();

    EXPORT u8** GetResBlock_410D00(s16 currentMotion);

    EXPORT void MidWalkToNextMotion_412FA0();

    EXPORT void WalkToNextMotion_4130D0();

    EXPORT void SlowOnX_414210(FP amount);

    void CheckLiftPointGoneAndSetCamera();

    EXPORT void MoveOnLine_412580(s16 xLookAhead);

    EXPORT void SetAbeAsPlayer_412520(s16 abeMotion);

    EXPORT s16 ToNextState_4120F0();

    EXPORT s16 ToNextStateAbeControlled_411E40();

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

    // States
    EXPORT void State_0_Respawn_414C60();
    EXPORT void State_1_Idle_412990();
    EXPORT void State_2_Unknown_412C30();
    EXPORT void State_3_WalkLoop_412C90();
    EXPORT void State_4_Turn_4140F0();
    EXPORT void State_5_WalkToIdle_4132D0();
    EXPORT void State_6_MidWalkToIdle_4133F0();
    EXPORT void State_7_IdleToWalk2_413200();
    EXPORT void State_8_IdleToWalk1_413270();
    EXPORT void State_9_ToYell_415890();
    EXPORT void State_10_Yell_4158E0();
    EXPORT void State_11_Unknown_4159A0();
    EXPORT void State_12_RunTurn_414520();
    EXPORT void State_13_RunTurnToWalk_4147C0();
    EXPORT void State_14_Speak_414860();
    EXPORT void State_15_Speak_4148F0();
    EXPORT void State_16_Speak_414980();
    EXPORT void State_17_Unknown_413620();
    EXPORT void State_18_Unknown_4136A0();
    EXPORT void State_19_Dead_415F90();
    EXPORT void State_20_Fall_415F70();
    EXPORT void State_21_Land_414A20();
    EXPORT void State_22_RunOffEdge_415810();
    EXPORT void State_23_WalkOffEdge_415E90();
    EXPORT void State_24_JumpToFall_415ED0();
    EXPORT void State_25_LickingHoney_415B50();
    EXPORT void State_26_LickingToStruggling_415AC0();
    EXPORT void State_27_AbeMountingEnd_415CA0();
    EXPORT void State_28_AbeUnmountingEnd_415D60();
    EXPORT void State_29_BeesStruggling_412A90();
    EXPORT void State_30_HopBegin_414E30();
    EXPORT void State_31_HopMid_414C70();
    EXPORT void State_32_HopLand_415140();
    EXPORT void State_33_RunJumpBegin_415400();
    EXPORT void State_34_RunJumpMid_415240();
    EXPORT void State_35_RunJumpLand_415580();
    EXPORT void State_36_RunLoop_413720();
    void RunSlideStopKnockback();
    EXPORT void State_37_RunSlideStop_4142E0();
    EXPORT void State_38_RunTurnToRun_414810();
    EXPORT void State_39_IdleToRun_413B00();
    EXPORT void State_40_WalkToRun_4134B0();
    EXPORT void State_41_MidWalkToRun_413560();
    EXPORT void State_42_RunToWalk_413B60();
    EXPORT void State_43_MidRunToWalk_413E20();
    EXPORT void State_44_ScratchBegin_412730();
    EXPORT void State_45_ScratchLoop_4127B0();
    EXPORT void State_46_ScratchEnd_412800();

    void ToIdle();

    EXPORT void State_47_Unknown_415A30();
    EXPORT void State_48_AbeMoutingBegin_415C40();
    EXPORT void State_49_AbeUnmountingBegin_415D00();
    EXPORT void State_50_Knockback_415DC0();

    struct ElumResources
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
    s16 field_12A_brain_state;
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
