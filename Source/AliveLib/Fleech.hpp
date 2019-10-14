#pragma once

#include "FunctionFwd.hpp"
#include "Primitives.hpp"
#include "BaseAliveGameObject.hpp"
#include "Path.hpp"

enum class FleechSound : unsigned __int8
{
    Unknown_0 = 0,
    Unknown_1 = 1,
    Unknown_2 = 2,
    Unknown_3 = 3,
    Unknown_4 = 4,
    Unknown_5 = 5,
    Unknown_6 = 6,
    Unknown_7 = 7,
    Unknown_8 = 8,
    Unknown_9 = 9,
    Unknown_10 = 10,
    Unknown_11 = 11,
    Unknown_12 = 12,
    Unknown_13 = 13,
    Unknown_14 = 14,
    Unknown_15 = 15,
    Unknown_16 = 16,
    Unknown_17 = 17,
    Unknown_18 = 18,
    Unknown_19 = 19,
};

struct Path_Fleech : public Path_TLV
{
    __int16 field_10_scale;
    __int16 field_12_direction;
    __int16 field_14_asleep;
    __int16 field_16_wake_up;
    __int16 field_18_not_used;
    __int16 field_1A_attack_anger;
    __int16 field_1C_attack_delay;
    __int16 field_1E_wake_up_id;
    __int16 field_20_hanging;
    __int16 field_22_lost_target_timeout;
    __int16 field_24_goes_to_sleep;
    __int16 field_26_patrol_range_in_grids;
    __int16 field_28_unused;
    __int16 field_2A_allow_wake_up_id;
    __int16 field_2C_persistant;
    __int16 field_2E;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Fleech, 0x30);

struct Fleech_State
{
    Types field_0_type;
    __int16 field_2;
    int field_4_obj_id;
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
    __int16 field_26_bFlipX;
    __int16 field_28_current_motion;
    __int16 field_2A_anim_current_frame;
    __int16 field_2C_frame_change_counter;
    char field_2E_bRender;
    char field_2F_bDrawable;
    FP field_30_health;
    __int16 field_34_current_motion;
    __int16 field_36_next_motion;
    __int16 field_38_lastLineYPos;
    __int16 field_3A_line_type;
    int field_3C_id;
    int field_40_tlvInfo;
    int field_44_obj_id;
    __int16 field_48;
    __int16 field_4A;
    __int16 field_4C;
    __int16 field_4E;
    __int16 field_50;
    __int16 field_52;
    __int16 field_54;
    __int16 field_56;
    __int16 field_58;
    __int16 field_5A;
    char field_5C;
    char field_5D;
    __int16 field_5E_brain_state;
    __int16 field_60_state;
    __int16 field_62;
    int field_64;
    char field_68_fleech_random_idx;
    char field_69;
    __int16 field_6A;
    int field_6C;
    FP field_70;
    __int16 field_74;
    __int16 field_76_anger;
    __int16 field_78;
    __int16 field_7A_attack_anger;
    __int16 field_7C_wakeup_id;
    __int16 field_7E_tlv;
    __int16 field_80_wake_up_switch_value;
    __int16 field_82_allow_wake_up_id;
    __int16 field_84;
    __int16 field_86;
    __int16 field_88_patrol_range;
    __int16 field_8A;
    __int16 field_8C;
    __int16 field_8E;
    __int16 field_90;
    __int16 field_92;
    __int16 field_94_lost_target_timeout;
    __int16 field_96;
    __int16 field_98;
    __int16 field_9A;
    __int16 field_9C;
    char field_9E_angle;
    char field_9F;
    FP field_A0;
    FP field_A4;
    int field_A8;
    int field_AC_obj_id;

    enum Flags_B0
    {
        eBit1 = 0x1,
        eBit2 = 0x2,
        eBit3 = 0x4,
        eBit4 = 0x8,
        eBit5 = 0x10,
        eBit6 = 0x20,
        eBit7 = 0x40,
    };

    BitField16<Flags_B0> field_B0;
    __int16 field_B2;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Fleech_State, 0xB4);

class Fleech;
using TFleechAIFn = __int16 (Fleech::*)();
using TFleechMotionFn = void (Fleech::*)();

class Fleech : public BaseAliveGameObject
{
public:
    EXPORT Fleech* ctor_429DC0(Path_Fleech* pTlv, int tlvInfo);

    virtual BaseGameObject* VDestructor(signed int flags) override;

    virtual void VUpdate() override;

    virtual void VRender(int** pOt) override;

    virtual void VScreenChanged() override;

    virtual void VOn_TLV_Collision_4087F0(Path_TLV* pTlv) override;

    virtual __int16 VTakeDamage_408730(BaseGameObject* pFrom) override;

    virtual void VOnTrapDoorOpen() override;

    virtual void VOnThrowableHit(BaseGameObject* pFrom) override;

    virtual int VGetSaveState(BYTE* pSaveBuffer) override;

    EXPORT static int CC CreateFromSaveState_42DD50(const BYTE* pBuffer);

private:
    EXPORT int vGetSaveState_42FF80(Fleech_State* pState);


public:

    EXPORT void M_Sleeping_0_42F0B0();
    EXPORT void M_WakingUp_1_42F270();
    EXPORT void M_Unknown_2_42F2F0();
    EXPORT void M_Idle_3_42E850();
    EXPORT void M_Crawl_4_42E960();
    EXPORT void M_PatrolCry_5_42E810();
    EXPORT void M_Knockback_6_42EAF0();
    EXPORT void M_StopCrawling_7_42EBB0();
    EXPORT void M_StopMidCrawlCycle_8_42EB20();
    EXPORT void M_Fall_9_42ECD0();
    EXPORT void M_Land_10_42F330();
    EXPORT void M_RaiseHead_11_42F590();
    EXPORT void M_Climb_12_42F7F0();
    EXPORT void M_SettleOnGround_13_42FB00();
    EXPORT void M_ExtendTongueFromEnemy_14_42FBD0();
    EXPORT void M_RetractTongueFromEnemey_15_42FC40();
    EXPORT void M_DeathByFalling_16_42FCE0();
    EXPORT void M_SleepingWithTongue_17_42F370();
    EXPORT void M_Consume_18_42FDF0();

public:
    EXPORT __int16 AI_Patrol_0_Real_430BA0();
    EXPORT __int16 AI_Patrol_0_430BA0();

    __int16 AI_Patrol_State_0();

    __int16 AI_Patrol_State_1();

    __int16 AI_Patrol_State_2();

    __int16 AI_Patrol_State_3();

    __int16 AI_Patrol_State_4(BaseAliveGameObject* pTarget);

    __int16 AI_Patrol_State_5();

    __int16 AI_Patrol_State_6();

    __int16 AI_Patrol_State_7();

    __int16 AI_Patrol_State_8(BaseAliveGameObject* pTarget);

    __int16 AI_Patrol_State_9();

    __int16 AI_Patrol_State_10();

    EXPORT __int16 AI_ChasingAbe_1_428760();

    __int16 AI_ChasingAbe_State_9(BaseAliveGameObject* pObj);

    __int16 AI_ChasingAbe_State_2(BaseAliveGameObject* pObj);

    __int16 AI_ChasingAbe_State_0(BaseAliveGameObject* pObj);

    EXPORT __int16 AI_ChasingAbe_State_1(BaseAliveGameObject* pObj);

    __int16 AI_ChasingAbe_State1_Helper(BaseAliveGameObject* pObj);

    EXPORT __int16 AI_Scared_2_42D310();
    EXPORT __int16 AI_Death_3_42D1E0();

private:
    EXPORT void dtor_42A3A0();

    EXPORT Fleech* vdtor_42A140(signed int flags);

    EXPORT void vUpdate_42AB20();

    EXPORT void vRender_42A550(int** ot);

    EXPORT void RenderEx_42C5A0(int** ot);

    EXPORT void vScreenChanged_42A4C0();

    EXPORT void vOn_Tlv_Collision_42AAB0(Path_TLV* pTlv);

private:
    EXPORT __int16 IsScrabOrParamiteNear_42B440(FP radius);


    EXPORT void Init_42A170();
    EXPORT void InitTonguePolys_42B6E0();
    EXPORT void SetAnim_429D80();
    EXPORT void sub_42CF70();

    EXPORT __int16 sub_42CFA0();

    EXPORT void sub_42CF50();

    EXPORT void sub_42B9A0(__int16 a2, __int16 a3);

    EXPORT void TongueUpdate_42BD30();

    EXPORT void ToIdle_42E520();

    EXPORT int Sound_430520(FleechSound soundId);

    EXPORT BYTE** ResBlockForMotion_42A530(int motion);

    EXPORT __int16 CanMove_42E3E0();

    EXPORT __int16 HandleEnemyStopperOrSlamDoor_42ADC0(int velX);

    EXPORT int UpdateWakeUpSwitchValue_4308B0();

    EXPORT __int16 vTakeDamage_42A5C0(BaseGameObject* pFrom);

    EXPORT void sub_42B8C0();

    EXPORT void vOnTrapDoorOpen_42E5C0();

    EXPORT void sub_42B9F0();

    EXPORT void IncreaseAnger_430920();

    EXPORT __int16 InRange_4307C0(BaseAliveGameObject* pObj);

    EXPORT int TongueActive_42B8A0();

    EXPORT void PullTargetIn_42BAF0();

    EXPORT void sub_42BA10();

    EXPORT void sub_42BAD0();

    EXPORT BaseAliveGameObject* FindMudOrAbe_42CFD0();

    EXPORT void MoveAlongFloor_42E600();

    EXPORT __int16 IsNear_428670(BaseAliveGameObject* pObj);

    EXPORT void vOnThrowableHit_42A590(BaseGameObject* pFrom);

    EXPORT BOOL Collision_42B290(__int16 alwaysOne);

    EXPORT Path_Hoist* TryGetHoist_42AFD0(int xDistance, __int16 bIgnoreDirection);


public:
    EXPORT void vOnFrame_42BC50(signed __int16* pData);

private:
    int field_118_tlvInfo;
    int field_11C_obj_id;
    __int16 field_120;
    __int16 field_122;
    __int16 field_124_brain_state;
    unsigned __int16 field_126_state;
    __int16 field_128;
    __int16 field_12A;
    int field_12C;
    __int16 field_130;
    __int16 field_132;
    int field_134;
    FP field_138;
    __int16 field_13C;
    __int16 field_13E_anger;
    unsigned __int16 field_140;
    __int16 field_142_attack_anger;
    __int16 field_144_wake_up_id;
    __int16 field_146_tlv_28;
    __int16 field_148_wake_up_switch_value;
    __int16 field_14A_allow_wake_up_id;
    __int16 field_14C;
    __int16 field_14E;
    __int16 field_150_patrol_range;
    __int16 field_152;
    __int16 field_154;
    __int16 field_156;
    __int16 field_158;
    __int16 field_15A;
    __int16 field_15C_lost_target_timeout;
    __int16 field_15E;
    __int16 field_160_hoistX;
    __int16 field_162_hoistY;
    __int16 field_164;
    char field_166_angle;
    char field_167;
    FP field_168;
    FP field_16C;
    int field_170_danger_obj;
    enum Flags_174 : __int16
    {
        eBit1_bHoistDone = 0x1,
        eBit2 = 0x2,
        eBit3 = 0x4,
        eBit4 = 0x8,
        eBit5_asleep = 0x10,
        eBit6_goes_to_sleep = 0x20,
        eBit7_persistant = 0x40,
        eBit8 = 0x80,
        eBit9 = 0x100,
        eBit10 = 0x200,
        eBit11 = 0x400,
        eBit12 = 0x800,
        eBit13 = 0x1000,
        eBit14 = 0x2000,
        eBit15 = 0x4000,
    };
    BitField16<Flags_174> field_174_flags;
    __int16 field_176;
    __int16 field_178_tongue_state;
    __int16 field_17A_tongue_sub_state;
    __int16 field_17C;
    __int16 field_17E;
    __int16 field_180_tongue_x;
    __int16 field_182_tongue_y;
    __int16 field_184_target_x;
    __int16 field_186_target_y;
    __int16 field_188;

    enum Flags_18A
    {
        e18A_TongueActive_Bit1 = 0x1,
        e18A_Render_Bit2 = 0x2,
        e18A_Bit3 = 0x4,
        e18A_Bit4 = 0x8,
        e18A_Bit5 = 0x10,
        e18A_Bit6 = 0x20,
        e18A_Bit7 = 0x40,
    };
    BitField16<Flags_18A> field_18A;
    Poly_G4 field_18C_tongue_polys1[4][2];
    Poly_G4 field_2CC_tongue_polys2[4][2];
    Prim_SetTPage field_40C[2];
};
ALIVE_ASSERT_SIZEOF(Fleech, 0x42C);

EXPORT int CC Animation_OnFrame_Fleech_449A60(void* pObj, signed __int16* pData);
