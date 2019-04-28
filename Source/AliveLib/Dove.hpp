#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "FunctionFwd.hpp"

EXPORT void CC Static_init_dynamic_array_41F3A0();

class Dove : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT Dove* ctor_41F660(int frameTableOffset, int maxW, __int16 maxH, int resourceID, FP xpos, FP ypos, FP scale);
    EXPORT void vRender_4200B0(int** ot);
    EXPORT void dtor_41F870();
    EXPORT Dove* vdtor_41F630(signed int flags);

    EXPORT void sub_41FA20(FP xpos, FP ypos, BYTE angle);

    EXPORT void sub_41F980(FP xpos, FP ypos, BYTE angle);

    EXPORT void sub_41F940(FP xpos, FP ypos);

    EXPORT void sub_420020(__int16 a2);

private:
    int field_E4[4]; // never used
    __int16 field_F4_counter;
    __int16 field_F6;
    int field_F8_tlvInfo;
    __int16 field_FC;
    __int16 field_FE_state;
    FP field_100;
    FP field_104;
    int field_108;
    char field_10C;
    // 3 byte pad
    FP field_110;
    FP field_114;
};
ALIVE_ASSERT_SIZEOF(Dove, 0x118);
