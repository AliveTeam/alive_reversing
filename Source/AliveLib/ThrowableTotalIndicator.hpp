#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "FixedPoint.hpp"

class ThrowableTotalIndicator : public BaseGameObject
{
public:
    EXPORT ThrowableTotalIndicator* ctor_431CB0(FP /*xpos*/, FP /*ypos*/, __int16 /*layer*/, FP /*scale*/, __int16 /*count*/, __int16 /*bUnknown*/);

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT void dtor_431E10();
    EXPORT BaseGameObject* vdtor_431DE0(signed int flags);

private:
    int field_20_xpos1;
    int field_24_ypos1;
    int field_28_xpos2;
    int field_2C_ypos2;
    int field_30;
    int field_34;
    int field_38;
    int field_3C;
    __int16 field_40_layer;
    __int16 field_42;
    __int16 field_44;
    __int16 field_46;
    __int16 field_48_count;
    __int16 field_4A;
    int field_4C_prims;
    int field_50;
    int field_54;
    int field_58;
    int field_5C;
    int field_60;
    int field_64;
    int field_68;
    int field_6C;
    int field_70;
    int field_74;
    int field_78;
    int field_7C;
    int field_80;
    int field_84;
    int field_88;
    int field_8C;
    int field_90;
    int field_94;
    int field_98;
    int field_9C;
    int field_A0;
    int field_A4;
    int field_A8;
    int field_AC;
    int field_B0;
    int field_B4;
    int field_B8;
    int field_BC;
    int field_C0;
    int field_C4;
    int field_C8;
    int field_CC;
    int field_D0;
    int field_D4;
    int field_D8;
    int field_DC;
    int field_E0;
    int field_E4;
    int field_E8;
    int field_EC;
    int field_F0;
    int field_F4;
    int field_F8;
    int field_FC;
    int field_100;
    int field_104;
    int field_108;
    int field_10C;
    int field_110;
    int field_114;
    int field_118;
    int field_11C;
    int field_120;
    int field_124;
    int field_128;
    int field_12C;
    int field_130;
    int field_134;
    int field_138;
    int field_13C;
    int field_140;
    int field_144;
    int field_148;
    int field_14C;
    int field_150;
    int field_154;
    int field_158;
    int field_15C;
    int field_160;
    int field_164;
    int field_168;
    int field_16C;
    int field_170;
    int field_174;
    int field_178;
    int field_17C;
    int field_180;
    int field_184;
    int field_188;
    __int16 field_18C;
    __int16 field_18E_bUnknown;
};
ALIVE_ASSERT_SIZEOF(ThrowableTotalIndicator, 0x190);
