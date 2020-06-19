#pragma once

#include "FunctionFwd.hpp"
#include "stdafx_ao.h"
#include "BaseAliveGameObject.hpp"

START_NS_AO;

class DeathFadeOut;
class OrbWhirlWind;

class Abe : public BaseAliveGameObject
{
public:
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
    char field_19C;
    char field_19D;
    __int16 field_19E;
    int field_1A0;
    BYTE** field_1A4_resources[65];
    __int16 field_2A8_flags;
    __int16 field_2AA;
    int field_2AC;
    int field_2B0;


    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VRender(int** pOrderingTable) override;
    virtual void VScreenChanged() override;

    // Virtual impls
    EXPORT BaseGameObject* vdtor_422A70(signed int flags);
    EXPORT void vUpdate_41FDB0();
    EXPORT void vRender_420F30(int** pOrderingTable);
    EXPORT void vScreenChanged_422640();

    EXPORT Abe* ctor_420770(int frameTableOffset, int a3, int a4, int a5);
};

ALIVE_VAR_EXTERN(Abe*, sActiveHero_5C1B68);

END_NS_AO;
