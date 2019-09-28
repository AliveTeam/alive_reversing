#pragma once

#include "FunctionFwd.hpp"
#include "BaseAliveGameObject.hpp"
#include "Path.hpp"

class Bone;

struct Path_Slog : public Path_TLV
{
    __int16 field_10_scale;
    __int16 field_12_direction;
    __int16 field_14_asleep;
    __int16 field_16_wake_up_anger;
    __int16 field_18_bark_anger;
    __int16 field_1A_chase_anger;
    __int16 field_1C_jump_delay;
    __int16 field_1E_disabled_resources;
    __int16 field_20_angry_id;
    __int16 field_22_bone_eating_time;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Slog, 0x24);

enum class SlogSound : int
{
    Unknown_1 = 1,
    IdleWoof_2 = 2,
    IdleGrrr_3 = 3,
    IdleGrrAlt_4 = 4,
    CautiousWoof_5 = 5,
    JumpBite_6 = 6,
    Unknown_7 = 7,
    AttackGrowl_8 = 8,
    DeathWhine_9 = 9,
    YawnStart_10 = 10,
    YawnEnd_11 = 11,
    Unknown_12 = 12,
    Unknown_13 = 13,
    Unknown_14 = 14,
    Unknown_15 = 15,
    Unknown_16 = 16,
    Unknown_17 = 17, //Bark cutoff?
    Unknown_18 = 18
};

struct Slog_State
{
    Types field_0_type;
    __int16 field_2_pad;
    int field_4;
    FP field_8_xpos;
    FP field_C_ypos;
    FP field_10_velx;
    FP field_14_vely;
    __int16 field_18_path_number;
    LevelIds field_1A_lvl_number;
    FP field_1C_sprite_scale;
    __int16 field_20_r;
    __int16 field_22_g;
    __int16 field_24_b;
    __int16 field_26_bAnimFlipX;
    __int16 field_28_current_motion;
    __int16 field_2A_anim_cur_frame;
    __int16 field_2C;
    char field_2E_bRender;
    char field_2F_bDrawable;
    FP field_30;
    __int16 field_34;
    __int16 field_36;
    __int16 field_38;
    __int16 field_3A_line_type;
    int field_3C_id;
    int field_40_tlvInfo;
    int field_44_obj_id;
    __int16 field_48_state_idx;
    __int16 field_4A_brain_state_result;
    int field_4C_timer;
    FP field_50;
    int field_54_obj_id;
    __int16 field_58;
    __int16 field_5A;
    __int16 field_5C;
    __int16 field_5E;
    __int16 field_60;
    __int16 field_62;
    int field_64;
    int field_68_timer;
    int field_6C;
    __int16 field_70_jump_delay;
    BYTE field_72;
    BYTE field_73_pad;

    enum Flags_74
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
        eBit10_bListenToSligs = 0x200,
    };
    BitField16<Flags_74> field_74_flags;
    __int16 field_76;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Slog_State, 0x78);

class Slog;
using TSlogAIFn = __int16 (Slog::*)();
using TSlogMotionFn = void (Slog::*)();

class Slog : public BaseAliveGameObject
{
public:
    EXPORT Slog* ctor_4C4540(FP xpos, FP ypos, FP scale, __int16 bListenToSligs, __int16 jumpDelay);


    EXPORT Slog* ctor_4C42E0(Path_Slog* pTlv, int tlvInfo);

    virtual BaseGameObject* VDestructor(signed int flags) override;

    virtual void VUpdate() override;

    virtual void VOnTrapDoorOpen() override;

    virtual void VOn_TLV_Collision_4087F0(Path_TLV* pTlv) override;

    virtual __int16 VTakeDamage_408730(BaseGameObject* pFrom) override;

    virtual void VOnThrowableHit(BaseGameObject* pFrom) override;

    virtual int VGetSaveState(BYTE* pSaveBuffer) override;

    EXPORT static int CC CreateFromSaveState_4C54F0(const BYTE* pBuffer);

private:
    EXPORT int vGetSaveState_4C78F0(Slog_State* pState);


public:
    EXPORT void M_Idle_0_4C5F90();
    EXPORT void M_Walk_1_4C60C0();
    EXPORT void M_Run_2_4C6340();
    EXPORT void M_TurnAround_3_4C65C0();
    EXPORT void M_Fall_4_4C6930();
    EXPORT void M_MoveHeadUpwards_5_4C5F20();
    EXPORT void M_StopRunning_6_4C66C0();
    EXPORT void M_SlideTurn_7_4C6790();
    EXPORT void M_StartWalking_8_4C62E0();
    EXPORT void M_EndWalking_9_4C6310();
    EXPORT void M_Land_10_4C7820();
    EXPORT void M_Unknown_11_4C7860();
    EXPORT void M_StartFastBarking_12_4C7880();
    EXPORT void M_EndFastBarking_13_4C78D0();
    EXPORT void M_Unknown_14_4C6CF0();
    EXPORT void M_Sleeping_15_4C6D60();
    EXPORT void M_MoveHeadDownwards_16_4C70D0();
    EXPORT void M_Bark_17_4C7000();
    EXPORT void M_JumpForwards_18_4C7210();
    EXPORT void M_JumpUpwards_19_4C7470();
    EXPORT void M_Eating_20_4C75F0();
    EXPORT void M_Unknown_21_4C77F0();
    EXPORT void M_Scratch_22_4C7120();
    EXPORT void M_Growl_23_4C7170();

public:
    EXPORT __int16 AI_ListeningToSlig_0_4C3790();

    __int16 AI_ListeningToSlig_State_6();

    __int16 AI_ListeningToSlig_State_5();

    __int16 AI_ListeningToSlig_State_4(const FP xpos1GridAHead);

    __int16 AI_ListeningToSlig_State_3(const FP xpos1GridAHead);

    __int16 AI_ListeningToSlig_State_2(const FP xpos1GridAHead, BaseAliveGameObject* pObj);

    __int16 AI_ListeningToSlig_State_1(const FP xpos1GridAHead);

    __int16 AI_ListeningToSlig_State_0();

    EXPORT __int16 AI_Idle_1_4C2830();
    EXPORT __int16 AI_ChasingAbe_2_4C0A00();

    __int16 AI_ChasingAbe_State_19(BaseAliveGameObject* pTarget);

    __int16 AI_ChasingAbe_State_18(BaseAliveGameObject* pTarget);

    __int16 AI_ChasingAbe_State_17(BaseAliveGameObject* pTarget);

    __int16 AI_ChasingAbe_State_16();

    __int16 AI_ChasingAbe_State_15(BaseAliveGameObject* pTarget);

    __int16 AI_ChasingAbe_State_14();

    __int16 AI_ChasingAbe_State_13();

    __int16 AI_ChasingAbe_State_12();

    __int16 AI_ChasingAbe_State_11();

    __int16 AI_ChasingAbe_State_20(BaseAliveGameObject* pTarget);

    __int16 AI_ChasingAbe_State_10();

    __int16 AI_ChasingAbe_State_9();

    __int16 AI_ChasingAbe_State_8();

    __int16 AI_ChasingAbe_State_7(BaseAliveGameObject* pTarget);

    __int16 AI_ChasingAbe_State_4(BaseAliveGameObject* pTarget);

    __int16 AI_ChasingAbe_State_3(BaseAliveGameObject* pTarget);

    __int16 AI_ChasingAbe_State_2(BaseAliveGameObject* pTarget);

    __int16 AI_ChasingAbe_State_1();

    __int16 AI_ChasingAbe_State_0();

    EXPORT __int16 AI_Death_3_4C3250();

private:
    EXPORT BYTE** ResBlockForMotion_4C4A80(__int16 motion);


    EXPORT void SetAnimFrame_4C42A0();

    EXPORT void Init_4C46A0();

    EXPORT void vUpdate_4C50D0();

    EXPORT void dtor_4C49A0();

    EXPORT Slog* vdtor_4C4510(signed int flags);


private:
    EXPORT void ToIdle_4C5C10();

    EXPORT void Sfx_4C7D30(SlogSound effectId);

    EXPORT void ToJump_4C5C60();

    EXPORT __int16 ToNextMotion_4C5A30();

    EXPORT BOOL CollisionCheck_4C5480(FP hitY, FP hitX);

    EXPORT void MoveOnLine_4C5DA0();

    EXPORT Bone* FindBone_4C25B0();

    EXPORT BaseAliveGameObject* FindTarget_4C33C0(__int16 /*a2*/, __int16 /*a3*/);

    EXPORT void vOn_TrapDoor_Open_4C5D50();

    EXPORT void vOn_Tlv_Collision_4C5060(Path_TLV* pTlv);

    EXPORT __int16 vTakeDamage_4C4B80(BaseGameObject* pFrom);

    EXPORT void vOnThrowableHit_4C4B50(BaseGameObject* pFrom);

    EXPORT __int16 PlayerOrNakedSligNear_4C26A0();

    EXPORT void DelayedResponse_4C3750(__int16 responseIdx);

    EXPORT __int16 HandleEnemyStopper_4C5340();

    EXPORT __int16 Facing_4C4020(FP xpos);

public:
    int field_118_target_id;
    __int16 field_11C;
private:
    __int16 field_11E;
    unsigned __int16 field_120_brain_state_idx;
    __int16 field_122_brain_state_result;
    int field_124_timer;
    FP field_128;
    int field_12C_tlvInfo;
    __int16 field_130;
    __int16 field_132;
    int field_134;
    int field_138;
    __int16 field_13C;
    __int16 field_13E_response_index;
    __int16 field_140_response_part;
    __int16 field_142_anger_level;
    __int16 field_144;
    __int16 field_146_total_anger;
    __int16 field_148_chase_anger;
    __int16 field_14A;
    int field_14C;
    int field_150_timer;
    __int16 field_154_angry_id;
    __int16 field_156_bone_eating_time;
    __int16 field_158_jump_delay;
    __int16 field_15A;
    int field_15C_bone_id;
    enum Flags_160 : __int16
    {
        eBit1 = 0x1,
        eBit2_ListenToSligs = 0x2,
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
    BitField16<Flags_160> field_160_flags;
    __int16 field_162;
};
ALIVE_ASSERT_SIZEOF(Slog, 0x164);

ALIVE_VAR_EXTERN(short, sSlogCount_BAF7F2);
