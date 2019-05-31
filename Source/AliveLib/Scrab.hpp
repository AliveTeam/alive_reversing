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

class Scrab : public BaseAliveGameObject
{
public:
    EXPORT Scrab* ctor_4A3C40(Path_Scrab* pTlv, int tlvInfo, __int16 spawnedScale);
    virtual BaseGameObject* VDestructor(signed int flags) override;
private:
    EXPORT Scrab* vdtor_4A41B0(signed int flags);
    EXPORT void dtor_4A42B0();

    EXPORT void vOnTrapDoorOpen_4A7ED0();


    EXPORT void vUpdateAnim_4A34F0();
    EXPORT __int16 sub_4A41E0();
    EXPORT void sub_4AA600();
private:
    __int16 field_116;
    int field_118_brain_state;
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
    int field_13C;
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
    int field_198;
    int field_19C;
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
