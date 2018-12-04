#pragma once

#include "FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

struct GibPart
{
    int field_0;
    int field_4;
    int field_8;
    int field_C;
    int field_10;
    int field_14;
    AnimationEx field_18_anim;
};
ALIVE_ASSERT_SIZEOF(GibPart, 0xB0);

class Gibs : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT Gibs* ctor_40FB40(int /*a2*/, FP /*xpos*/, FP /*ypos*/, int /*a5*/, int /*a6*/, FP /*scale*/, __int16 /*a8*/);

    int field_E4;
    int field_E8;
    int field_EC;
    int field_F0;
    int field_F4;
    int field_F8;
    int field_FC;
    int field_100;
    GibPart field_104_parts[7];
    __int16 field_5D4_parts_used_count;
    __int16 field_5D6_bUnknown;
};
ALIVE_ASSERT_SIZEOF(Gibs, 0x5D8);
