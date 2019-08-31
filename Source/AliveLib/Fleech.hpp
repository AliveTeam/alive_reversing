#pragma once

#include "FunctionFwd.hpp"
#include "Primitives.hpp"
#include "BaseAliveGameObject.hpp"
#include "Path.hpp"

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

class Fleech;
using TFleechAIFn = __int16 (Fleech::*)();
using TFleechMotionFn = void (Fleech::*)();

class Fleech : public BaseAliveGameObject
{
public:
    EXPORT Fleech* ctor_429DC0(Path_Fleech* pTlv, int tlvInfo);

    virtual BaseGameObject* VDestructor(signed int flags) override;

    virtual void VUpdate() override;

    virtual void VScreenChanged() override;

    virtual void VOn_TLV_Collision_4087F0(Path_TLV* pTlv) override;

    virtual __int16 VTakeDamage_408730(BaseGameObject* pFrom) override;

    virtual void VOnTrapDoorOpen() override;

    virtual void VOnThrowableHit(BaseGameObject* pFrom) override;

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
    EXPORT __int16 AI_Patrol_0_430BA0();
    EXPORT __int16 AI_ChasingAbe_1_428760();
    EXPORT __int16 AI_Scared_2_42D310();
    EXPORT __int16 AI_Death_3_42D1E0();

private:
    EXPORT void dtor_42A3A0();

    EXPORT Fleech* vdtor_42A140(signed int flags);

    EXPORT void vUpdate_42AB20();

    EXPORT void vScreenChanged_42A4C0();

    EXPORT void vOn_Tlv_Collision_42AAB0(Path_TLV* pTlv);

private:
    EXPORT void Init_42A170();
    EXPORT void InitPolys_42B6E0();
    EXPORT void SetAnim_429D80();
    EXPORT void sub_42CF70();

    EXPORT void sub_42B9A0(__int16 a2, __int16 a3);

    EXPORT void sub_42BD30();

    EXPORT void ToIdle_42E520();

    EXPORT int Sound_430520(unsigned __int8 sfx);

    EXPORT BYTE** ResBlockForMotion_42A530(int motion);

    EXPORT __int16 CanMove_42E3E0();

    EXPORT __int16 HandleEnemyStopperOrSlamDoor_42ADC0(int a2);

    EXPORT int UpdateWakeUpSwitchValue_4308B0();

    EXPORT __int16 vTakeDamage_42A5C0(BaseGameObject* pFrom);

    EXPORT void sub_42B8C0();

    EXPORT void vOnTrapDoorOpen_42E5C0();

    EXPORT void sub_42B9F0();

    EXPORT void IncreaseAnger_430920();

    EXPORT __int16 InRange_4307C0(BaseAliveGameObject* pObj);

    EXPORT int sub_42B8A0();

    EXPORT void PullTargetIn_42BAF0();

    EXPORT void sub_42BA10();

    EXPORT void sub_42BAD0();

    EXPORT BaseAliveGameObject* FindMudOrAbe_42CFD0();

    EXPORT void MoveAlongFloor_42E600();

    EXPORT __int16 IsNear_428670(BaseAliveGameObject* pObj);

    EXPORT void vOnThrowableHit_42A590(BaseGameObject* pFrom);


private:
    int field_118_tlvInfo;
    int field_11C_obj_id;
    int field_120;
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
    __int16 field_160;
    __int16 field_162;
    __int16 field_164;
    char field_166_angle;
    char field_167;
    int field_168;
    int field_16C;
    int field_170;
    enum Flags_174 : __int16
    {
        eBit1 = 0x1,
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
    __int16 field_178;
    __int16 field_17A;
    __int16 field_17C;
    __int16 field_17E;
    __int16 field_180;
    __int16 field_182;
    __int16 field_184;
    __int16 field_186;
    __int16 field_188;
    __int16 field_18A;
    Poly_G4 field_18C_mPoly_G4s[8];
    int field_2CC;
    int field_2D0;
    int field_2D4;
    int field_2D8;
    int field_2DC;
    int field_2E0;
    int field_2E4;
    int field_2E8;
    int field_2EC;
    int field_2F0;
    int field_2F4;
    int field_2F8;
    int field_2FC;
    int field_300;
    int field_304;
    int field_308;
    int field_30C;
    int field_310;
    int field_314;
    int field_318;
    int field_31C;
    int field_320;
    int field_324;
    int field_328;
    int field_32C;
    int field_330;
    int field_334;
    int field_338;
    int field_33C;
    int field_340;
    int field_344;
    int field_348;
    int field_34C;
    int field_350;
    int field_354;
    int field_358;
    int field_35C;
    int field_360;
    int field_364;
    int field_368;
    int field_36C;
    int field_370;
    int field_374;
    int field_378;
    int field_37C;
    int field_380;
    int field_384;
    int field_388;
    int field_38C;
    int field_390;
    int field_394;
    int field_398;
    int field_39C;
    int field_3A0;
    int field_3A4;
    int field_3A8;
    int field_3AC;
    int field_3B0;
    int field_3B4;
    int field_3B8;
    int field_3BC;
    int field_3C0;
    int field_3C4;
    int field_3C8;
    int field_3CC;
    int field_3D0;
    int field_3D4;
    int field_3D8;
    int field_3DC;
    int field_3E0;
    int field_3E4;
    int field_3E8;
    int field_3EC;
    int field_3F0;
    int field_3F4;
    int field_3F8;
    int field_3FC;
    int field_400;
    int field_404;
    int field_408;
    int field_40C;
    int field_410;
    int field_414;
    int field_418;
    int field_41C;
    int field_420;
    int field_424;
    int field_428;
};
ALIVE_ASSERT_SIZEOF(Fleech, 0x42C);
