#pragma once

#include "FunctionFwd.hpp"
#include "stdafx_ao.h"
#include "BaseAliveGameObject.hpp"

START_NS_AO;

class DeathFadeOut;
class OrbWhirlWind;

enum Flags_2A8
{
    e2A8_Bit1 = 0x1,
    e2A8_Bit2 = 0x2,
    e2A8_Bit3_Fall_To_Well = 0x4, //todo verify
    e2A8_Bit4 = 0x8,
    e2A8_Bit5 = 0x10,
    e2A8_Bit6_bShrivel = 0x20,
    e2A8_Bit7 = 0x40,
    e2A8_Bit8 = 0x80,
    e2A8_Bit9_bLaughAtChantEnd = 0x100, //todo verify
    e2A8_Bit10 = 0x200,
    e2A8_Bit11 = 0x400,
    e2A8_Bit12 = 0x800,
    e2A8_eBit13 = 0x1000,
    e2A8_eBit14 = 0x2000,
};

class Abe : public BaseAliveGameObject
{
public:
    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VRender(int** pOrderingTable) override;
    virtual void VScreenChanged() override;

    // Virtual impls
    EXPORT BaseGameObject* vdtor_422A70(signed int flags);
    EXPORT void vUpdate_41FDB0();
    EXPORT void vRender_420F30(int** pOrderingTable);
    EXPORT void vScreenChanged_422640();

    EXPORT void sub_42F4C0();

    __int16 field_10C;
    __int16 field_10E;
    __int16 field_110_state;
    __int16 field_112;
    int field_114;
    int field_118;
    int field_11C;
    int field_120;
    int field_124;
    __int16 field_128;
    __int16 field_12A;
    int field_12C;
    __int16 field_130;
    __int16 field_132;
    int field_134;
    __int16 field_138;
    __int16 field_13A;
    __int16 field_13C;
    __int16 field_13E;
    __int16 field_140;
    __int16 field_142;
    __int16 field_144;
    __int16 field_146;
    __int16 field_148;
    __int16 field_14A;
    int field_14C;
    int field_150;
    __int16 field_154;
    __int16 field_156;
    DeathFadeOut* field_158_pDeathFadeout;
    int field_15C;
    int field_160_pRope;
    int field_164;
    int field_168;
    __int16 field_16C;
    __int16 field_16E;
    int field_170_tlv_type;
    __int16 field_174_0x14SizeTlv;
    __int16 field_176;
    __int16 field_178;
    __int16 field_17A;
    int field_17C;
    int field_180;
    int field_184;
    OrbWhirlWind* field_188_pOrbWhirlWind;
    int field_18C;
    __int16 field_190_level;
    __int16 field_192_path;
    __int16 field_194_camera;
    __int16 field_196_door_id;
    int field_198;
    char field_19C_throwable_count;
    char field_19D;
    __int16 field_19E;
    int field_1A0;
    BYTE** field_1A4_resources[65];
    BitField16<Flags_2A8> field_2A8_flags;
    __int16 field_2AA;
    int field_2AC;
    int field_2B0;

    EXPORT Abe* ctor_420770(int frameTableOffset, int a3, int a4, int a5);
};

ALIVE_VAR_EXTERN(Abe*, sActiveHero_5C1B68);

END_NS_AO;
