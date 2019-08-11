#pragma once

#include "FunctionFwd.hpp"
#include "BaseAliveGameObject.hpp"
#include "Path.hpp"

#define PARAMITE_MOTIONS_ENUM(ENTRY) \
    ENTRY(M_Idle_0_489FB0) \
    ENTRY(M_WalkBegin_1_48A7B0) \
    ENTRY(M_Walking_2_48A2D0) \
    ENTRY(M_Running_3_48AA00) \
    ENTRY(M_Turn_4_48B180) \
    ENTRY(M_Hop_5_48B5B0) \
    ENTRY(M_UNKNOWN_6_48A930) \
    ENTRY(M_WalkRunTransition_7_48B0C0) \
    ENTRY(M_WalkEnd_8_48A870) \
    ENTRY(M_RunBegin_9_48AF10) \
    ENTRY(M_RunEnd_10_48B000) \
    ENTRY(M_Falling_11_48B200) \
    ENTRY(M_JumpUpBegin_12_48BE40) \
    ENTRY(M_JumpUpMidair_13_48BAF0) \
    ENTRY(M_JumpUpLand_14_48BF00) \
    ENTRY(M_RopePull_15_48D930) \
    ENTRY(M_CloseAttack_16_48DDA0) \
    ENTRY(M_Landing_17_48B590) \
    ENTRY(M_UNKNOWN_18_48DF60) \
    ENTRY(M_Knockback_19_48BF50) \
    ENTRY(M_GameSpeakBegin_20_48C010) \
    ENTRY(M_PreHiss_21_48C180) \
    ENTRY(M_Hiss1_22_48C3E0) \
    ENTRY(M_Hiss2_23_48C600) \
    ENTRY(M_Empty_24_48C680) \
    ENTRY(M_AllOYaGameSpeakBegin_25_48C6A0) \
    ENTRY(M_Hiss3_26_48C6F0) \
    ENTRY(M_PostHiss_27_48C780) \
    ENTRY(M_GameSpeakEnd_28_48C8B0) \
    ENTRY(M_GetDepossessedBegin_29_48D9D0) \
    ENTRY(M_GetDepossessedEnd_30_48DB50) \
    ENTRY(M_RunningAttack_31_48C9E0) \
    ENTRY(M_Empty_32_48D740) \
    ENTRY(M_SurpriseWeb_33_48D760) \
    ENTRY(M_WebLeaveDown_34_48D870) \
    ENTRY(M_WebIdle_35_48D400) \
    ENTRY(M_WebGoingUp_36_48D000) \
    ENTRY(M_WebGoingDown_37_48CC60) \
    ENTRY(M_WebGrab_38_48D6C0) \
    ENTRY(M_WebLeaveUp_39_48D8C0) \
    ENTRY(M_Eating_40_48A0F0) \
    ENTRY(M_Death_41_48D8E0) \
    ENTRY(M_UNKNOWN_42_48D900) \
    ENTRY(M_Attack_43_48DB70)

#define MAKE_ENUM(VAR) VAR,
enum eParamiteMotions : int
{
    PARAMITE_MOTIONS_ENUM(MAKE_ENUM)
};

class Meat;

struct Path_Paramite : public Path_TLV
{
    __int16 field_10_scale;
    __int16 field_12_entrace_type;
    __int16 field_14_attack_delay;
    __int16 field_16_drop_delay;
    __int16 field_18_meat_eating_time;
    unsigned __int16 field_1A_attack_duration;
    __int16 field_1C_disabled_resources;
    __int16 field_1E_id;
    __int16 field_20_hiss_before_attack;
    __int16 field_22_delete_when_far_away;
    __int16 field_24_deadly_scratch;
    __int16 field_26;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Paramite, 0x28);

class Paramite;
using TParamiteAIFn = __int16 (Paramite::*)();
using TParamiteMotionFn = void (Paramite::*)();

enum class LevelIds : __int16;
enum class GameSpeakEvents : __int16;

class PullRingRope;


enum class ParamiteSpeak : __int8
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

struct Paramite_State
{
    Types field_0_type;
    __int16 field_2_pad;
    FP field_4_xpos;
    FP field_8_ypos;
    FP field_C_velx;
    FP field_10_vely;
    __int16 field_14_path_number;
    LevelIds field_16_lvl_number;
    FP field_18_sprite_scale;
    __int16 field_1C_r;
    __int16 field_1E_g;
    __int16 field_20_b;
    __int16 field_22_flip_x;
    __int16 field_24_current_motion;
    __int16 field_26_anim_current_frame;
    __int16 field_28;
    char field_2A_anim_bit4;
    char field_2B_bit8;
    FP field_2C_health;
    __int16 field_30_current_motion;
    __int16 field_32_next_motion;
    __int16 field_34_last_line_ypos;
    __int16 field_36_line_type;
    __int16 field_38;
    __int16 field_3A;
    int field_3C_tlvInfo;
    int field_40_obj_id;
    int field_44_obj_id;
    int field_48_obj_id;
    int field_4C_obj_id;
    int field_50_ai_idx;
    __int16 field_54;
    __int16 field_56;
    __int16 field_58_brain_ret;
    __int16 field_5A;
    int field_5C_timer;
    int field_60;
    FP field_64;
    int field_68_timer;
    LevelIds field_6C_return_level;
    __int16 field_6E_return_path;
    __int16 field_70_return_camera;
    __int16 field_72_input;
    __int16 field_74;

    enum Flags_76 : __int16
    {
        eBit1 = 0x1,
        eBit2 = 0x2,
        eBit3 = 0x4,
        eBit4 = 0x8,
        eBit5 = 0x10,
        eBit6 = 0x20,
        eBit7 = 0x40,
    };

    BitField16<Flags_76> field_76_flags;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Paramite_State, 0x78);

class Paramite : public BaseAliveGameObject
{
public:
    EXPORT Paramite* ctor_4879B0(Path_Paramite* pTlv, int tlvInfo);

    virtual BaseGameObject* VDestructor(signed int flags) override;

    virtual void VUpdate() override;

    virtual void VRender(int** pOrderingTable) override
    {
        vRender_488220(pOrderingTable);
    }

    virtual __int16 vOnSameYLevel_425520(BaseAnimatedWithPhysicsGameObject* pOther) override
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

    virtual __int16 VTakeDamage_408730(BaseGameObject* pFrom) override
    {
        return vTakeDamage_488250(pFrom);
    }

    virtual void VOn_TLV_Collision_4087F0(Path_TLV* pTlv) override
    {
        return vOn_TLV_Collision_488640(pTlv);
    }

    virtual void VOnTrapDoorOpen()
    {
        vOnTrapDoorOpen_489F60();
    }

    virtual int VGetSaveState(BYTE* pSaveBuffer) override
    {
        return vGetSaveState_48F220(reinterpret_cast<Paramite_State*>(pSaveBuffer));
    }

    EXPORT static int CC CreateFromSaveState_4855A0(const BYTE* pBuffer);

private:
    EXPORT int vGetSaveState_48F220(Paramite_State* pState);


public:
    EXPORT __int16 AI_Patrol_0_4835B0();
    EXPORT __int16 AI_Death_1_484CD0();
    EXPORT __int16 AI_ChasingAbe_2_4859D0();
    EXPORT __int16 AI_SurpriseWeb_3_4851B0();
    EXPORT __int16 AI_UNKNOWN_4_48F8F0();
    EXPORT __int16 AI_SpottedMeat_5_486880();
    EXPORT __int16 AI_Possessed_6_484BC0();
    EXPORT __int16 AI_DeathDrop_7_484FF0();
    EXPORT __int16 AI_ControlledByGameSpeak_8_48DFC0();
    EXPORT __int16 AI_ParamiteSpawn_9_48ED80();

public:
    EXPORT void M_Idle_0_489FB0();
    EXPORT void M_WalkBegin_1_48A7B0();
    EXPORT void M_Walking_2_48A2D0();
    EXPORT void M_Running_3_48AA00();
    EXPORT void M_Turn_4_48B180();
    EXPORT void M_Hop_5_48B5B0();
    EXPORT void M_UNKNOWN_6_48A930();
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
    EXPORT void M_UNKNOWN_18_48DF60();
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
    EXPORT void M_UNKNOWN_42_48D900();
    EXPORT void M_Attack_43_48DB70();

private:

    void SetBrain(TParamiteAIFn fn);
    bool BrainIs(TParamiteAIFn fn);

private:
    EXPORT void dtor_487FC0();
    EXPORT Paramite* vdtor_487F90(signed int flags);

    EXPORT void vUpdate_4871B0();

    EXPORT __int16 Find_Paramite_488810();

    EXPORT void vUpdateAnim_487170();

    EXPORT Meat* FindMeat_488930();

    EXPORT __int16 IsNear_488B10(Paramite* pOther);

    EXPORT __int16 vOnSameYLevel_488A40(BaseAnimatedWithPhysicsGameObject* pOther);

    EXPORT void vUnPosses_488BE0();

    EXPORT void vPossessed_488B60();

    EXPORT __int16 vTakeDamage_488250(BaseGameObject* pFrom);

    EXPORT BYTE** ResBlockForMotion_488130(__int16 motion);

    EXPORT void vOn_TLV_Collision_488640(Path_TLV* pTlv);

    EXPORT __int16 AnotherParamiteNear_4886E0();

    EXPORT PathLine* WebCollision_4888A0(FP yOff, FP xOff);

    EXPORT void vRender_488220(int** ot);

    EXPORT void vOnTrapDoorOpen_489F60();

    EXPORT void ToHop_489C20();

    EXPORT __int16 CanIAcceptAGameSpeakCommand_489160();

    EXPORT __int16 HandleEnemyStopper_4893B0(__int16 numGridBlocks);

    EXPORT PullRingRope* FindPullRope_488F20();

    EXPORT __int16 NextPlayerInputMotion_4894C0();

    EXPORT __int16 FindTarget_488C30();

    EXPORT __int16 ToNextMotion_4898A0();

    EXPORT void ToIdle_489B70();

    EXPORT void ToKnockBack_489BB0();

    EXPORT void MoveOnLine_489CA0();

    EXPORT void CheckForPlatform_489EA0();

    EXPORT void HandleStopWalking_48A720();

    EXPORT void HandleInputRunning_48ADB0();

    EXPORT static __int16 CC StableDelay_48DF80();

    EXPORT void Sound_48F600(ParamiteSpeak soundId, __int16 pitch_min);

    EXPORT void UpdateSlurgWatchPoints_4890D0();

    EXPORT GameSpeakEvents LastSpeak_489040();


private:
    int field_118;
    int field_11C_obj_id;
    int field_120_obj_id;
    int field_124;
    TParamiteAIFn field_128_fn_brainState;
    __int16 field_12C_brain_ret;
    __int16 field_12E_drop_delay;
    int field_130_timer;
    __int16 field_134_meat_eating_time;
    __int16 field_136_attack_delay;
    int field_138;
    FP field_13C;
    int field_140_tlvInfo;
    int field_144_attack_duration;
    int field_148_timer;
    __int16 field_14C_id;
    LevelIds field_14E_return_level;
    __int16 field_150_return_path;
    __int16 field_152_return_camera;
    int field_154;
    __int16 field_158;
    __int16 field_15A;
    __int16 field_15C;
    __int16 field_15E;
    int field_160;
    __int16 field_164;
    __int16 field_166;
    __int16 field_168;
    __int16 field_16A;
    __int16 field_16C;
    __int16 field_16E;
    __int16 field_170;
    __int16 field_172;
    __int16 field_174;
    __int16 field_176;
    enum Flags_178 : __int16
    {
        eBit1 = 0x1,
        eBit2 = 0x2,
        eBit3 = 0x4,
        eBit4 = 0x8,
        eBit5 = 0x10,
        eBit6 = 0x20,
        eBit7 = 0x40,
        eBit8 = 0x80,
        eBit9 = 0x100,
        eBit10 = 0x200,
        eBit11 = 0x400,
        eBit12 = 0x800,
        eBit13 = 0x1000,
        eBit14 = 0x2000,
        eBit15 = 0x4000,
    };
    BitField16<Flags_178> field_178_flags;
    __int16 field_17A;
};
ALIVE_ASSERT_SIZEOF(Paramite, 0x17C);
