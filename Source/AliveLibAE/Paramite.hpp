#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "BaseAliveGameObject.hpp"
#include "Path.hpp"

#define PARAMITE_MOTIONS_ENUM(ENTRY)        \
    ENTRY(M_Idle_0_489FB0)                  \
    ENTRY(M_WalkBegin_1_48A7B0)             \
    ENTRY(M_Walking_2_48A2D0)               \
    ENTRY(M_Running_3_48AA00)               \
    ENTRY(M_Turn_4_48B180)                  \
    ENTRY(M_Hop_5_48B5B0)                   \
    ENTRY(M_Unused_6_48A930)                \
    ENTRY(M_WalkRunTransition_7_48B0C0)     \
    ENTRY(M_WalkEnd_8_48A870)               \
    ENTRY(M_RunBegin_9_48AF10)              \
    ENTRY(M_RunEnd_10_48B000)               \
    ENTRY(M_Falling_11_48B200)              \
    ENTRY(M_JumpUpBegin_12_48BE40)          \
    ENTRY(M_JumpUpMidair_13_48BAF0)         \
    ENTRY(M_JumpUpLand_14_48BF00)           \
    ENTRY(M_RopePull_15_48D930)             \
    ENTRY(M_CloseAttack_16_48DDA0)          \
    ENTRY(M_Landing_17_48B590)              \
    ENTRY(M_Unused_18_48DF60)               \
    ENTRY(M_Knockback_19_48BF50)            \
    ENTRY(M_GameSpeakBegin_20_48C010)       \
    ENTRY(M_PreHiss_21_48C180)              \
    ENTRY(M_Hiss1_22_48C3E0)                \
    ENTRY(M_Hiss2_23_48C600)                \
    ENTRY(M_Empty_24_48C680)                \
    ENTRY(M_AllOYaGameSpeakBegin_25_48C6A0) \
    ENTRY(M_Hiss3_26_48C6F0)                \
    ENTRY(M_PostHiss_27_48C780)             \
    ENTRY(M_GameSpeakEnd_28_48C8B0)         \
    ENTRY(M_GetDepossessedBegin_29_48D9D0)  \
    ENTRY(M_GetDepossessedEnd_30_48DB50)    \
    ENTRY(M_RunningAttack_31_48C9E0)        \
    ENTRY(M_Empty_32_48D740)                \
    ENTRY(M_SurpriseWeb_33_48D760)          \
    ENTRY(M_WebLeaveDown_34_48D870)         \
    ENTRY(M_WebIdle_35_48D400)              \
    ENTRY(M_WebGoingUp_36_48D000)           \
    ENTRY(M_WebGoingDown_37_48CC60)         \
    ENTRY(M_WebGrab_38_48D6C0)              \
    ENTRY(M_WebLeaveUp_39_48D8C0)           \
    ENTRY(M_Eating_40_48A0F0)               \
    ENTRY(M_Death_41_48D8E0)                \
    ENTRY(M_Squawk_42_48D900)               \
    ENTRY(M_Attack_43_48DB70)

#define MAKE_ENUM(VAR) VAR,
enum eParamiteMotions : s32
{
    PARAMITE_MOTIONS_ENUM(MAKE_ENUM)
};

class Meat;

struct Path_Paramite final : public Path_TLV
{
    Scale_short field_10_scale;
    enum class EntranceType : s16
    {
        eNormalPatrol_0 = 0,
        eNormalSurpriseWeb_1 = 1,
        eSlightlyHigherSpawnSurpriseWeb_2 = 2,
        eUnused_ScaleToLeftGridSize_3 = 3,
        eUnused_ScaleToRightGridSize_4 = 4,
    };
    EntranceType field_12_entrace_type;
    s16 field_14_alone_chase_delay;
    s16 field_16_surprise_web_delay_timer;
    s16 field_18_meat_eating_time;
    u16 field_1A_group_chase_delay;
    s16 field_1C_disabled_resources;
    s16 field_1E_id;
    Choice_short field_20_hiss_before_attack;
    Choice_short field_22_delete_when_out_of_sight;
    Choice_short field_24_bAttack_fleeches;
    s16 field_26_padding;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Paramite, 0x28);

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

struct Paramite_State final
{
    AETypes field_0_type;
    s16 field_2_pad;
    FP field_4_xpos;
    FP field_8_ypos;
    FP field_C_velx;
    FP field_10_vely;
    s16 field_14_path_number;
    LevelIds field_16_lvl_number;
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
    s32 field_3C_tlvInfo;
    s32 field_40_meat_id;
    s32 field_44_web_id;
    s32 field_48_obj_id;
    s32 field_4C_pull_ring_rope_id;
    s32 field_50_brain_idx;
    s16 field_54_padding;
    s16 field_56_padding;
    s16 field_58_brain_ret;
    s16 field_5A_padding;
    s32 field_5C_timer;
    s32 field_60_depossession_timer;
    FP field_64_velx_offset;
    s32 field_68_timer;
    LevelIds field_6C_return_level;
    s16 field_6E_return_path;
    s16 field_70_return_camera;
    s16 field_72_input;
    s16 field_74_next_brain_ret;

    enum Flags_76 : s16
    {
        eBit1_unused = 0x1,
        eBit2_running = 0x2,
        eBit3_hissed_or_left_screen = 0x4,
        eBit4_prevent_depossession = 0x8,
        eBit5_spawned = 0x10,
        eBit6_alerted = 0x20,
        eBit7_can_be_possessed = 0x40,
    };

    BitField16<Flags_76> field_76_flags;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Paramite_State, 0x78);

class Paramite final : public BaseAliveGameObject
{
public:
    Paramite(Path_Paramite* pTlv, s32 tlvInfo);
    ~Paramite();

    virtual void VUpdate() override;

    virtual void VRender(PrimHeader** ppOt) override
    {
        vRender_488220(ppOt);
    }

    virtual s16 vOnSameYLevel_425520(BaseAnimatedWithPhysicsGameObject* pOther) override
    {
        return vOnSameYLevel_488A40(pOther);
    }

    virtual void VUnPosses_408F90() override
    {
        vUnPosses_488BE0();
    }

    virtual void VPossessed_408F70() override
    {
        vPossessed_488B60();
    }

    virtual s16 VTakeDamage_408730(BaseGameObject* pFrom) override
    {
        return vTakeDamage_488250(pFrom);
    }

    virtual void VOn_TLV_Collision_4087F0(Path_TLV* pTlv) override
    {
        return vOn_TLV_Collision_488640(pTlv);
    }

    virtual void VOnTrapDoorOpen() override
    {
        vOnTrapDoorOpen_489F60();
    }

    virtual s32 VGetSaveState(u8* pSaveBuffer) override
    {
        return vGetSaveState_48F220(reinterpret_cast<Paramite_State*>(pSaveBuffer));
    }


    EXPORT static s32 CC CreateFromSaveState_4855A0(const u8* pBuffer);

private:
    EXPORT s32 vGetSaveState_48F220(Paramite_State* pState);


public:
    EXPORT s16 Brain_0_Patrol_4835B0();
	s16 Brain_Patrol_State_1_IdleForAbe(BaseAliveGameObject* pObj);
	s16 Brain_Patrol_State_2_FearingAbe(BaseAliveGameObject* pObj);
	s16 Brain_Patrol_State_3_RunningFromAbe(BaseAliveGameObject* pObj);
	s16 Brain_Patrol_State_4_ApproachingAbe(BaseAliveGameObject* pObj);
	s16 Brain_Patrol_State_5_StopApproachingAbe(BaseAliveGameObject* pObj);
	s16 Brain_Patrol_State_8_StuckToWall(BaseAliveGameObject* pObj);
    s16 Brain_Patrol_State_12_Idle(BaseAliveGameObject* pObj);

    EXPORT s16 Brain_1_Death_484CD0();

    EXPORT s16 Brain_2_ChasingAbe_4859D0();
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

    EXPORT s16 Brain_3_SurpriseWeb_4851B0();
    EXPORT s16 Brain_4_Unused_48F8F0();

    EXPORT s16 Brain_5_SpottedMeat_486880();
	s16 Brain_SpottedMeat_State_0_Idle(Meat* pMeat);
	s16 Brain_SpottedMeat_State_1_Running(Meat* pMeat);
	s16 Brain_SpottedMeat_State_2_Walking(Meat* pMeat);
	s16 Brain_SpottedMeat_State_3_Jumping();
	s16 Brain_SpottedMeat_State_4_Turning(Meat* pMeat);
	s16 Brain_SpottedMeat_State_5_AttentiveToMeat(Meat* pMeat);
    s16 Brain_SpottedMeat_State_6_Eating(Meat* pMeat);

    EXPORT s16 Brain_6_Possessed_484BC0();
    EXPORT s16 Brain_7_DeathDrop_484FF0();
    EXPORT s16 Brain_8_ControlledByGameSpeak_48DFC0();
    EXPORT s16 Brain_9_ParamiteSpawn_48ED80();

public:
    EXPORT void M_Idle_0_489FB0();
    EXPORT void M_WalkBegin_1_48A7B0();
    EXPORT void M_Walking_2_48A2D0();
    EXPORT void M_Running_3_48AA00();
    EXPORT void M_Turn_4_48B180();
    EXPORT void M_Hop_5_48B5B0();
    EXPORT void M_Unused_6_48A930();
    EXPORT void M_WalkRunTransition_7_48B0C0();
    EXPORT void M_WalkEnd_8_48A870();
    EXPORT void M_RunBegin_9_48AF10();
    EXPORT void M_RunEnd_10_48B000();
    EXPORT void M_Falling_11_48B200();
    EXPORT void M_JumpUpBegin_12_48BE40();
    EXPORT void M_JumpUpMidair_13_48BAF0();
    EXPORT void M_JumpUpLand_14_48BF00();
    EXPORT void M_RopePull_15_48D930();
    EXPORT void M_CloseAttack_16_48DDA0();
    EXPORT void M_Landing_17_48B590();
    EXPORT void M_Unused_18_48DF60();
    EXPORT void M_Knockback_19_48BF50();
    EXPORT void M_GameSpeakBegin_20_48C010();
    EXPORT void M_PreHiss_21_48C180();
    EXPORT void M_Hiss1_22_48C3E0();
    EXPORT void M_Hiss2_23_48C600();
    EXPORT void M_Empty_24_48C680();
    EXPORT void M_AllOYaGameSpeakBegin_25_48C6A0();
    EXPORT void M_Hiss3_26_48C6F0();
    EXPORT void M_PostHiss_27_48C780();
    EXPORT void M_GameSpeakEnd_28_48C8B0();
    EXPORT void M_GetDepossessedBegin_29_48D9D0();
    EXPORT void M_GetDepossessedEnd_30_48DB50();
    EXPORT void M_RunningAttack_31_48C9E0();
    EXPORT void M_Empty_32_48D740();
    EXPORT void M_SurpriseWeb_33_48D760();
    EXPORT void M_WebLeaveDown_34_48D870();
    EXPORT void M_WebIdle_35_48D400();
    EXPORT void M_WebGoingUp_36_48D000();
    EXPORT void M_WebGoingDown_37_48CC60();
    EXPORT void M_WebGrab_38_48D6C0();
    EXPORT void M_WebLeaveUp_39_48D8C0();
    EXPORT void M_Eating_40_48A0F0();
    EXPORT void M_Death_41_48D8E0();
    EXPORT void M_Squawk_42_48D900();
    EXPORT void M_Attack_43_48DB70();

private:
    void SetBrain(TParamiteBrainFn fn);
    bool BrainIs(TParamiteBrainFn fn);

private:

    EXPORT void vUpdate_4871B0();

    EXPORT s16 Find_Paramite_488810();

    EXPORT void vUpdateAnim_487170();

    EXPORT Meat* FindMeat_488930();

    EXPORT s16 IsNear_488B10(Paramite* pOther);

    EXPORT s16 vOnSameYLevel_488A40(BaseAnimatedWithPhysicsGameObject* pOther);

    EXPORT void vUnPosses_488BE0();

    EXPORT void vPossessed_488B60();

    EXPORT s16 vTakeDamage_488250(BaseGameObject* pFrom);

    EXPORT u8** ResBlockForMotion_488130(s16 motion);

    EXPORT void vOn_TLV_Collision_488640(Path_TLV* pTlv);

    EXPORT s16 AnotherParamiteNear_4886E0();

    EXPORT PathLine* WebCollision_4888A0(FP yOff, FP xOff);

    EXPORT void vRender_488220(PrimHeader** ppOt);

    EXPORT void vOnTrapDoorOpen_489F60();

    EXPORT void ToHop_489C20();

    EXPORT s16 CanIAcceptAGameSpeakCommand_489160();

    EXPORT s16 HandleEnemyStopper_4893B0(s16 numGridBlocks);

    EXPORT PullRingRope* FindPullRope_488F20();

    EXPORT s16 NextPlayerInputMotion_4894C0();

    EXPORT s16 FindTarget_488C30();

    EXPORT s16 ToNextMotion_4898A0();

    EXPORT void ToIdle_489B70();

    EXPORT void ToKnockBack_489BB0();

    EXPORT void MoveOnLine_489CA0();

    EXPORT void CheckForPlatform_489EA0();

    EXPORT void HandleStopWalking_48A720();

    EXPORT void HandleInputRunning_48ADB0();

    EXPORT static s16 CC StableDelay_48DF80();

    EXPORT void Sound_48F600(ParamiteSpeak soundId, s16 pitch_min);

    EXPORT void UpdateSlurgWatchPoints_4890D0();

    EXPORT GameSpeakEvents LastSpeak_489040();


private:
    s32 field_118_meat_id;
    s32 field_11C_web_id;
    s32 field_120_obj_id;
    s32 field_124_pull_ring_rope_id;
    TParamiteBrainFn field_128_fn_brainState;
    s16 field_12C_brain_ret;
    s16 field_12E_surprise_web_delay_timer;
    s32 field_130_timer;
    s16 field_134_meat_eating_time;
    s16 field_136_alone_chase_delay;
    s32 field_138_depossession_timer;
    FP field_13C_velx_offset;
    s32 field_140_tlvInfo;
    s32 field_144_group_chase_delay;
    s32 field_148_timer;
    s16 field_14C_id;
    LevelIds field_14E_return_level;
    s16 field_150_return_path;
    s16 field_152_return_camera;
    s32 field_154_input;
    s16 field_158_next_brain_ret;
    s16 field_15A_paramite_next_motion;
    s16 field_15C_paramite_xOffset;
    s16 field_15E_padding;
    s32 field_160_last_event_index;
    s16 field_164_padding;
    s16 field_166_padding;
    s16 field_168_padding;
    s16 field_16A_padding;
    s16 field_16C_padding;
    s16 field_16E_padding;
    s16 field_170_padding;
    s16 field_172_padding;
    s16 field_174_unused;
    s16 field_176_unused;
    enum Flags_178 : s16
    {
        eBit1_hiss_before_attack = 0x1,
        eBit2_running = 0x2,
        eBit3_hissed_or_left_screen = 0x4,
        eBit4_out_of_sight = 0x8,
        eBit5_prevent_depossession = 0x10,
        eBit6_spawned = 0x20,
        eBit7_alerted = 0x40,
        eBit8_bAttack_fleeches = 0x80,
        eBit9_padding = 0x100,
        eBit10_padding = 0x200,
        eBit11_padding = 0x400,
        eBit12_padding = 0x800,
        eBit13_padding = 0x1000,
        eBit14_padding = 0x2000,
        eBit15_padding = 0x4000,
    };
    BitField16<Flags_178> field_178_flags;
    s16 field_17A_padding;
};
ALIVE_ASSERT_SIZEOF(Paramite, 0x17C);
