#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "Primitives.hpp"
#include "BaseAliveGameObject.hpp"
#include "Path.hpp"

enum class FleechSound : u8
{
    PatrolCry_0 = 0,
    Burp_1 = 1,
    Digesting_2 = 2,
    SleepingInhale_3 = 3,
    SleepingExhale_4 = 4,
    Unknown_5 = 5, // unused? sounds like a whipping sound
    WakeUp_6 = 6,
    Scared_7 = 7,
    Devour_8 = 8,
    LandOnFloor_9 = 9,
    LickTarget_10 = 10,
    LedgeHoist_11 = 11,
    DeathByHeight_12 = 12,
    Dismember_13 = 13,
    CrawlRNG1_14 = 14,
    CrawlRNG2_15 = 15,
    CrawlRNG3_16 = 16,
};

struct Path_Fleech final : public Path_TLV
{
    Scale_short field_10_scale;
    XDirection_short field_12_direction;
    Choice_short field_14_asleep;
    s16 field_16_wake_up;
    s16 field_18_not_used;
    s16 field_1A_attack_anger_increaser;
    s16 field_1C_attack_delay;
    s16 field_1E_wake_up_id1;
    Choice_short field_20_hanging;
    s16 field_22_lost_target_timeout;
    Choice_short field_24_goes_to_sleep;
    s16 field_26_patrol_range_in_grids;
    s16 field_28_wake_up_switch_anger_value;
    s16 field_2A_can_wake_up_id;
    Choice_short field_2C_persistant;
    s16 field_2E_padding;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Fleech, 0x30);

struct Fleech_State final
{
    AETypes field_0_type;
    s16 field_2;
    s32 field_4_obj_id;
    FP field_8_xpos;
    FP field_C_ypos;
    FP field_10_velx;
    FP field_14_vely;
    s16 field_18_path_number;
    LevelIds field_1A_lvl_number;
    FP field_1C_sprite_scale;
    s16 field_20_r;
    s16 field_22_g;
    s16 field_24_b;
    s16 field_26_bFlipX;
    s16 field_28_current_motion;
    s16 field_2A_anim_current_frame;
    s16 field_2C_frame_change_counter;
    s8 field_2E_bRender;
    s8 field_2F_bDrawable;
    FP field_30_health;
    s16 field_34_current_motion;
    s16 field_36_next_motion;
    s16 field_38_lastLineYPos;
    s16 field_3A_line_type;
    s32 field_3C_id;
    s32 field_40_tlvInfo;
    s32 field_44_obj_id;
    s16 field_48_unused; //TODO: Saves and sets another unused field, field_120 -- Nemin (7/5/2020)
    s16 field_4A_save_tongue_state;
    s16 field_4C_save_tongue_sub_state;
    s16 field_4E;
    s16 field_50;
    s16 field_52_tongue_x;
    s16 field_54_tongue_y;
    s16 field_56_target_x;
    s16 field_58_target_y;
    s16 field_5A;
    s8 field_5C;
    s8 field_5D;
    s16 field_5E_brain_state;
    s16 field_60_state;
    s16 field_62;
    s32 field_64;
    s8 field_68_fleech_random_idx;
    s8 field_69;
    s16 field_6A;
    s32 field_6C;
    FP field_70;
    s16 field_74;
    s16 field_76_current_anger;
    s16 field_78_max_anger;
    s16 field_7A_attack_anger;
    s16 field_7C_wakeup_id;
    s16 field_7E_wake_up_switch_anger_value;
    s16 field_80_wake_up_switch_value;
    s16 field_82_can_wake_up_id;
    s16 field_84;
    s16 field_86;
    s16 field_88_patrol_range;
    s16 field_8A;
    s16 field_8C;
    s16 field_8E;
    s16 field_90_chase_delay;
    s16 field_92_chase_timer;
    s16 field_94_lost_target_timeout;
    s16 field_96;
    s16 field_98;
    s16 field_9A;
    s16 field_9C;
    s8 field_9E_angle;
    s8 field_9F;
    FP field_A0;
    FP field_A4;
    s32 field_A8;
    s32 field_AC_obj_id;

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
    s16 field_B2;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Fleech_State, 0xB4);

class Fleech;
using TFleechBrainFn = s16 (Fleech::*)();
using TFleechMotionFn = void (Fleech::*)();

class Fleech final : public BaseAliveGameObject
{
public:
    EXPORT Fleech* ctor_429DC0(Path_Fleech* pTlv, s32 tlvInfo);

    virtual BaseGameObject* VDestructor(s32 flags) override;

    virtual void VUpdate() override;

    virtual void VRender(PrimHeader** ppOt) override;

    virtual void VScreenChanged() override;

    virtual void VOn_TLV_Collision_4087F0(Path_TLV* pTlv) override;

    virtual s16 VTakeDamage_408730(BaseGameObject* pFrom) override;

    virtual void VOnTrapDoorOpen() override;

    virtual void VOnThrowableHit(BaseGameObject* pFrom) override;

    virtual s32 VGetSaveState(u8* pSaveBuffer) override;

    EXPORT static s32 CC CreateFromSaveState_42DD50(const u8* pBuffer);

private:
    EXPORT s32 vGetSaveState_42FF80(Fleech_State* pState);


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
    EXPORT s16 Brain_0_Patrol_430BA0();
    s16 Brain_Patrol_State_0();
    s16 Brain_Patrol_State_1();
    s16 Brain_Patrol_State_2();
    s16 Brain_Patrol_State_3();
    s16 Brain_Patrol_State_4(BaseAliveGameObject* pTarget);
    s16 Brain_Patrol_State_5();
    s16 Brain_Patrol_State_6();
    s16 Brain_Patrol_State_7();
    s16 Brain_Patrol_State_8(BaseAliveGameObject* pTarget);
    s16 Brain_Patrol_State_9();
    s16 Brain_Patrol_State_10();

    EXPORT s16 Brain_1_ChasingAbe_428760();
	s16 Brain_ChasingAbe_State_0(BaseAliveGameObject* pObj);

	EXPORT s16 Brain_ChasingAbe_State_1(BaseAliveGameObject* pObj);
	s16 Brain_ChasingAbe_State1_Helper(BaseAliveGameObject* pObj);

	s16 Brain_ChasingAbe_State_2(BaseAliveGameObject* pObj);
    s16 Brain_ChasingAbe_State_9(BaseAliveGameObject* pObj);

    EXPORT s16 Brain_2_Scared_42D310();
    EXPORT s16 Brain_3_Death_42D1E0();

private:
    EXPORT void dtor_42A3A0();

    EXPORT Fleech* vdtor_42A140(s32 flags);

    EXPORT void vUpdate_42AB20();

    EXPORT void vRender_42A550(PrimHeader** ot);

    EXPORT void RenderEx_42C5A0(PrimHeader** ot);

    EXPORT void vScreenChanged_42A4C0();

    EXPORT void vOn_Tlv_Collision_42AAB0(Path_TLV* pTlv);

private:
    EXPORT s16 IsScrabOrParamiteNear_42B440(FP radius);


    EXPORT void Init_42A170();
    EXPORT void InitTonguePolys_42B6E0();
    EXPORT void SetAnim_429D80();
    EXPORT void ResetTarget_42CF70();

    EXPORT s16 GotNoTarget_42CFA0();

    EXPORT void SetTarget_42CF50();

    EXPORT void TongueHangingFromWall_42B9A0(s16 a2, s16 a3);

    EXPORT void TongueUpdate_42BD30();

    EXPORT void ToIdle_42E520();

    EXPORT s32 Sound_430520(FleechSound soundId);

    EXPORT u8** ResBlockForMotion_42A530(s32 motion);

    EXPORT s16 CanMove_42E3E0();

    EXPORT s16 HandleEnemyStopperOrSlamDoor_42ADC0(s32 velX);

    EXPORT s32 UpdateWakeUpSwitchValue_4308B0();

    EXPORT s16 vTakeDamage_42A5C0(BaseGameObject* pFrom);

    EXPORT void sub_42B8C0();

    EXPORT void vOnTrapDoorOpen_42E5C0();

    EXPORT void SetTongueState1_42B9F0();

    EXPORT void IncreaseAnger_430920();

    EXPORT s16 InRange_4307C0(BaseAliveGameObject* pObj);

    EXPORT s32 TongueActive_42B8A0();

    EXPORT void PullTargetIn_42BAF0();

    EXPORT void sub_42BA10();

    EXPORT void SetTongueState5_42BAD0();

    EXPORT BaseAliveGameObject* FindMudOrAbe_42CFD0();

    EXPORT void MoveAlongFloor_42E600();

    EXPORT s16 IsNear_428670(BaseAliveGameObject* pObj);

    EXPORT void vOnThrowableHit_42A590(BaseGameObject* pFrom);

    EXPORT Bool32 Collision_42B290(s16 alwaysOne);

    EXPORT Path_Hoist* TryGetHoist_42AFD0(s32 xDistance, s16 bIgnoreDirection);


public:
    EXPORT void vOnFrame_42BC50(s16* pData);

private:
    s32 field_118_tlvInfo;
    s32 field_11C_obj_id;
    s16 field_120_unused;
    s16 field_122;
    s16 field_124_brain_state;
    u16 field_126_state;
    s16 field_128;
    s16 field_12A;
    s32 field_12C;
    s16 field_130;
    s16 field_132;
    s32 field_134;
    FP field_138;
    s16 field_13C;
    s16 field_13E_current_anger;
    u16 field_140_max_anger;
    s16 field_142_attack_anger_increaser;
    s16 field_144_wake_up_id;
    s16 field_146_wake_up_switch_anger_value;
    s16 field_148_wake_up_switch_value;
    s16 field_14A_can_wake_up_id;
    s16 field_14C;
    s16 field_14E;
    s16 field_150_patrol_range;
    s16 field_152;
    s16 field_154;
    s16 field_156;
    s16 field_158_chase_delay;
    s16 field_15A_chase_timer;
    s16 field_15C_lost_target_timeout;
    s16 field_15E;
    s16 field_160_hoistX;
    s16 field_162_hoistY;
    s16 field_164;
    s8 field_166_angle;
    s8 field_167;
    FP field_168;
    FP field_16C;
    s32 field_170_danger_obj;
    enum Flags_174 : s16
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
    s16 field_176;
    s16 field_178_tongue_state;
    s16 field_17A_tongue_sub_state;
    s16 field_17C;
    s16 field_17E;
    s16 field_180_tongue_x;
    s16 field_182_tongue_y;
    s16 field_184_target_x;
    s16 field_186_target_y;
    s16 field_188;

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

EXPORT s32 CC Animation_OnFrame_Fleech_449A60(void* pObj, s16* pData);
