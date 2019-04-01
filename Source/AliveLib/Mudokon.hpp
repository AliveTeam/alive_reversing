#pragma once

#include "FunctionFwd.hpp"
#include "BaseAliveGameObject.hpp"
#include "Path.hpp"

enum class TLV_Scale : __int16
{
    Full_0 = 0,
    Half_1 = 1,
};

enum class Mud_State : __int16
{
    eChisle_0 = 0,
    eScrub_1 = 1,
    eAngryWorker_2 = 2,
    eDamageRingGiver_3 = 3,
    eHealthRingGiver_4 = 4,
};

enum class Mud_Direction : __int16
{
    eLeft_0 = 0,
    eRight_1 = 1,
};

enum class Mud_Emotion : __int16
{
    eNormal_0 = 0,
    eAngry_1 = 1,
    eSad_2 = 2,
    eWired_3 = 3,
    eSick_4 = 4
};

struct Path_Mudokon : public Path_TLV
{
    TLV_Scale field_10_scale;
    Mud_State field_12_state;
    Mud_Direction field_14_direction;
    __int16 field_16_voice_pitch;
    __int16 field_18_rescue_id;
    __int16 field_1A_bDeaf;
    __int16 field_1C_disabled_resources;
    __int16 field_1E_save_state;
    Mud_Emotion field_20_emotion;
    __int16 field_22_bBlind;
    __int16 field_24_angry_trigger;
    __int16 field_26_stop_trigger;
    __int16 field_28_bGets_depressed;
    __int16 field_2A_ring_timeout;
    __int16 field_2C_bInstant_power_up;
    __int16 field_2E_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Mudokon, 0x30);

class Mudokon : public BaseAliveGameObject
{
public:
    EXPORT Mudokon* ctor_474F30(Path_Mudokon* pTlv, int tlvInfo);

private:
    EXPORT void vUpdate_4757A0();

    EXPORT void SetPal_4772D0(__int16 palType);

private:
    __int16 field_116;
    int field_118;
    int field_11C;
    __int16 field_120_angry_trigger;
    __int16 field_122;
    int field_124;
    int field_128;
    int field_12C;
    __int16 field_130;
    __int16 field_132;
    int field_134;
    int field_138;
    __int16 field_13C_voice_pitch;
    __int16 field_13E;
    int field_140;
    int field_144;
    __int16 field_148;
    __int16 field_14A;
    __int16 field_14C;
    __int16 field_14E;
    __int16 field_150;
    __int16 field_152;
    __int16 field_154;
    __int16 field_156;
    int field_158_obj_id;
    __int16 field_15C;
    __int16 field_15E;
    __int16 field_160_delayed_speak;
    __int16 field_162;
    int field_164_ring_timeout;
    __int16 field_168;

    enum Flags
    {
        eBit1 = 0x1,
        eBit2_save_state = 0x2,
        eBit3 = 0x4,
        eBit4_blind = 0x8,
        eBit5 = 0x10,
        eBit6 = 0x20,
        eBit7 = 0x40,
        eBit8 = 0x80,
        eBit9 = 0x100,
        eBit10_stop_trigger = 0x200,
        eBit11_get_depressed = 0x400,
        eBit12 = 0x800,
        eBit13 = 0x1000,
        eBit14 = 0x2000,
        eBit15_ring_timeout = 0x4000,
        eBit16_instant_power_up = 0x8000,
        eBit17 = 0x10000,
        eBit18 = 0x20000,
        eBit19 = 0x40000,
        eBit20 = 0x80000,
        eBit21 = 0x100000,
        eBit22 = 0x200000,
        eBit23 = 0x400000,
        eBit24 = 0x800000,
        eBit25 = 0x1000000,
    };
    BitField16<Flags> field_16A_flags;
    __int16 field_16C;
    __int16 field_16E;
    __int16 field_170;
    __int16 field_172;
    __int16 field_174;
    __int16 field_176;
    __int16 field_178;
    __int16 field_17A_rescue_id;
    __int16 field_17C;
    __int16 field_17E;
    __int16 field_180_emo_tbl;
    __int16 field_182;
    __int16 field_184;
    __int16 field_186;
    WORD* field_188;
    __int16 field_18C;
    unsigned __int16 field_18E_fns1_idx;
    unsigned __int16 field_190;
    __int16 field_192;
    int field_194;
    int field_198;
};
ALIVE_ASSERT_SIZEOF(Mudokon, 0x19C);
