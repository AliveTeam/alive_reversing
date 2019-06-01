#pragma once

#include "FunctionFwd.hpp"
#include "BaseAliveGameObject.hpp"
#include "Path.hpp"

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

class Paramite : public BaseAliveGameObject
{
public:
    EXPORT Paramite* ctor_4879B0(Path_Paramite* pTlv, int tlvInfo);

    virtual BaseGameObject* VDestructor(signed int flags) override;

public:
    EXPORT __int16 AI_0_4835B0();
    EXPORT __int16 AI_1_484CD0();
    EXPORT __int16 AI_2_4859D0();
    EXPORT __int16 AI_3_4851B0();
    EXPORT __int16 AI_4_48F8F0();
    EXPORT __int16 AI_5_486880();
    EXPORT __int16 AI_6_484BC0();
    EXPORT __int16 AI_7_484FF0();
    EXPORT __int16 AI_8_48DFC0();
    EXPORT __int16 AI_9_48ED80();
private:
    EXPORT void sub_489EA0();

    void SetBrain(TParamiteAIFn fn);

private:
    EXPORT void dtor_487FC0();
    EXPORT Paramite* vdtor_487F90(signed int flags);


private:

    __int16 field_116;
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
    int field_13C;
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
