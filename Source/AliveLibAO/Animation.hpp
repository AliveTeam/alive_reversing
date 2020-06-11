#pragma once

#include "FunctionFwd.hpp"
#include "DynamicArray.hpp"

START_NS_AO

class AnimationBase
{
public:
    static EXPORT void CC AnimateAll_4034F0(DynamicArrayT<AnimationBase>* /*pAnimList*/)
    {
        NOT_IMPLEMENTED();
    }
};

class Animation : public AnimationBase
{
public:

    virtual void VDummy() {}

    __int16 field_4_flags;
    __int16 field_6;
    int field_8;
    __int16 field_C_layer;
    __int16 field_E;
    int field_10;
    int field_14;
    int field_18;
    int field_1C_fn_ptrs;
    int field_20;
    int field_24;
    int field_28;
    int field_2C;
    int field_30;
    int field_34;
    int field_38;
    int field_3C;
    int field_40;
    int field_44;
    int field_48;
    int field_4C;
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
    __int16 field_84;
    __int16 field_86;
    __int16 field_88;
    __int16 field_8A;
    __int16 field_8C_vram_x_pal;
    __int16 field_8E_vram_y_pal;
    __int16 field_90_pal_depth;
    __int16 field_92;
    int field_94;
};
ALIVE_ASSERT_SIZEOF(Animation, 0x98);

END_NS_AO
