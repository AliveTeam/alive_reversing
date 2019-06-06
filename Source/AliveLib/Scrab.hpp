#pragma once

#include "FunctionFwd.hpp"
#include "BaseAliveGameObject.hpp"
#include "Path.hpp"

struct Path_Scrab : public Path_TLV
{
    __int16 field_10_scale;
    __int16 field_12_attack_delay;
    __int16 field_14_patrol_type;
    __int16 field_16_left_min_delay;
    __int16 field_18_left_max_delay;
    __int16 field_1A_right_min_delay;
    __int16 field_1C_right_max_delay;
    unsigned __int16 field_1E_attack_duration;
    __int16 field_20_disabled_resources;
    __int16 field_22_roar_randomly;
    __int16 field_24_persistant;
    __int16 field_26_whirl_attack_duration;
    __int16 field_28_whirl_attack_recharge;
    __int16 field_2A_kill_close_fleech;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Scrab, 0x2C);

enum class LevelIds : __int16;

class Scrab;
using TScrabAIFn = __int16 (Scrab::*)();
using TScrabMotionFn = void (Scrab::*)();

class Scrab : public BaseAliveGameObject
{
public:
    EXPORT Scrab* ctor_4A3C40(Path_Scrab* pTlv, int tlvInfo, __int16 spawnedScale);
    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
private:
    EXPORT Scrab* vdtor_4A41B0(signed int flags);
    EXPORT void dtor_4A42B0();

    EXPORT void vOnTrapDoorOpen_4A7ED0();


    EXPORT void vUpdateAnim_4A34F0();
    EXPORT __int16 sub_4A41E0();
    EXPORT void sub_4AA600();

    EXPORT void vUpdate_4A3530();

    EXPORT void Update_Slurg_Step_Watch_Points_4A5780();

public:

public:
    EXPORT void M_Stand_0_4A8220();
    EXPORT void M_Walk_1_4A84D0();
    EXPORT void M_Run_2_4A89C0();
    EXPORT void M_Turn_3_4A91A0();
    EXPORT void M_RunToStand_4_4A90C0();
    EXPORT void M_HopBegin_5_4A96C0();
    EXPORT void M_HopMidair_6_4A9490();
    EXPORT void M_HopLand_7_4A9890();
    EXPORT void M_JumpToFall_8_4A9220();
    EXPORT void M_StandToWalk_9_4A8450();
    EXPORT void M_StandToRun_10_4A8900();
    EXPORT void M_WalkToStand_11_4A8880();
    EXPORT void M_RunJumpBegin_12_4A99C0();
    EXPORT void M_RunJumpEnd_13_4A9BE0();
    EXPORT void M_WalkToFall_14_4A9460();
    EXPORT void M_RunToFall_15_4A9430();
    EXPORT void M_WalkToRun_16_4A8D60();
    EXPORT void M_RunToWalk_17_4A8D90();
    EXPORT void M_Knockback_18_4AA490();
    EXPORT void M_GetEaten_19_4AA3E0();
    EXPORT void M_Fall_20_4A93E0();
    EXPORT void M_Stamp_21_4A9CC0();
    EXPORT void M_PossessedGet_22_4AA420();
    EXPORT void M_UNKNOWN_23_4A9D80();
    EXPORT void M_DeathEnd_24_4AA140();
    EXPORT void M_Empty_25_4A34D0();
    EXPORT void M_HowlBegin_26_4A9DA0();
    EXPORT void M_HowlEnd_27_4A9E60();
    EXPORT void M_PossessedUngetBegin_28_4AA200();
    EXPORT void M_PossessedUngetEnd_29_4AA3C0();
    EXPORT void M_Shriek_30_4A9EA0();
    EXPORT void M_UNKNOWN_ScrabBattleAnimMaybe_31_4A9F30();
    EXPORT void M_AttackSpin_32_4A8DC0();
    EXPORT void M_FeedToGulp_33_4A9FA0();
    EXPORT void M_GulpToStand_34_4A9FF0();
    EXPORT void M_StandToFeed_35_4AA010();
    EXPORT void M_Feed_36_4AA030();
    EXPORT void M_AttackLunge_37_4AA0B0();
    EXPORT void M_LegKick_38_4AA120();
    EXPORT void M_DeathBegin_39_4AA190();

private:
    __int16 field_116;
    TScrabAIFn field_118_brain_state;
    __int16 field_11C_sub_state;
    __int16 field_11E;
    int field_120_obj_id;
    int field_124_fight_target_obj_id;
    __int16 field_128_attack_delay;
    __int16 field_12A_patrol_type;
    int field_12C;
    int field_130;
    int field_134;
    int field_138;
    FP field_13C;
    __int16 field_140;
    __int16 field_142;
    int field_144_tlvInfo;
    int field_148_attack_duration;
    int field_14C;
    int field_150;
    int field_154;
    __int16 field_158_left_min_delay;
    __int16 field_15A_left_max_delay;
    __int16 field_15C;
    __int16 field_15E_right_max_delay;
    int field_160;
    __int16 field_164;
    LevelIds field_166_level;
    __int16 field_168_path;
    __int16 field_16A_camera;
    int field_16C;
    int field_170;
    __int16 field_174_whirl_attack_duration;
    __int16 field_176_whirl_attack_recharge;
    __int16 field_178;
    __int16 field_17A;
    int field_17C_last_event;
    __int16 field_180;
    __int16 field_182;
    __int16 field_184;
    __int16 field_186;
    __int16 field_188;
    __int16 field_18A;
    __int16 field_18C;
    __int16 field_18E;
    __int16 field_190;
    __int16 field_192;
    __int16 field_194;
    __int16 field_196;
    FP field_198;
    FP field_19C;
    __int16 field_1A0;
    __int16 field_1A2;
    __int16 field_1A4;
    __int16 field_1A6;
    __int16 field_1A8_kill_close_fleech;

    enum Flags_1AA : __int16
    {
        eBit1 = 0x1,
        eBit2 = 0x2,
        eBit3 = 0x4,
        eBit4 = 0x8,
        eBit5_roar_randomly = 0x10,
        eBit6_persistant = 0x20,
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
    BitField16<Flags_1AA> field_1AA_flags;
};
ALIVE_ASSERT_SIZEOF(Scrab, 0x1AC);
