#pragma once

#include "Function.hpp"
#include "BaseGameObject.hpp"

class Class_5480D4; // TODO

class OrbWhirlWind : public BaseGameObject
{
public:
    // TODO: Virtuals.. pretty much everything else..

    EXPORT void sub_4E4050();

    int field_20;
    __int16 field_24;
    __int16 field_26_bUnknown;
    __int16 field_28_obj_array_idx;
    //__int16 field_2A; // padding ?
    Class_5480D4 *field_2C_objArray[16];
    int field_6C_xpos;
    int field_70_ypos;
    int field_74_scale;
};
ALIVE_ASSERT_SIZEOF(OrbWhirlWind, 0x78);
