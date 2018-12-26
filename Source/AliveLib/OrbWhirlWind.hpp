#pragma once

#include "Function.hpp"
#include "BaseGameObject.hpp"
#include "FixedPoint.hpp"

class Class_5480D4; // TODO

class OrbWhirlWind : public BaseGameObject
{
public:
    virtual BaseGameObject* VDestructor(signed int flags) override;

    // TODO: Virtuals.. pretty much everything else..
    EXPORT OrbWhirlWind* ctor_4E3C90(FP xpos, FP ypos, FP scale, __int16 bUnknown);
    EXPORT void sub_4E3FD0(FP xpos, FP ypos, FP scale, BaseGameObject* pObj);
    EXPORT void sub_4E4050();
private:
    EXPORT void dtor_4E3D80();
    EXPORT OrbWhirlWind* vdtor_4E3D50(signed int flags);
private:
    int field_20;
    __int16 field_24;
    __int16 field_26_bUnknown;
    __int16 field_28_obj_array_idx;
    //__int16 field_2A; // padding ?
    Class_5480D4 *field_2C_objArray[16];
    FP field_6C_xpos;
    FP field_70_ypos;
    FP field_74_scale;
};
ALIVE_ASSERT_SIZEOF(OrbWhirlWind, 0x78);
