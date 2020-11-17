#pragma once

#include "FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

namespace AO {

struct GibPart
{
    FP field_0_x;
    FP field_4_y;
    FP field_8_z;
    FP field_C_dx;
    FP field_10_dy;
    FP field_14_dz;
    Animation field_18_anim;
};
ALIVE_ASSERT_SIZEOF(GibPart, 0xB0);

struct Gib_Data
{
    int field_0_head;
    int field_4_arm;
    int field_8_body;
    int field_C_max_w;
    int field_10_max_h;
    int field_14_resource_id;
};
ALIVE_ASSERT_SIZEOF(Gib_Data, 0x18);

class Gibs : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT Gibs* ctor_407B20(int gibType, FP xpos, FP ypos, FP xOff, FP yOff, FP scale);

    EXPORT BaseGameObject* dtor_408040();

    virtual void VUpdate() override;

    EXPORT void VUpdate_4080C0();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT BaseGameObject* Vdtor_4083D0(unsigned int flags);

    virtual void VRender(int** pOrderingTable) override;

    EXPORT void VRender_408200(int** ppOt);

    int field_D4[4];
    const Gib_Data* field_E4_pGibData;
    FP field_E8_z;
    FP field_EC_dz;
    int field_F0_timer;
    GibPart field_F4_parts[7];
    __int16 field_5C4_parts_used_count;
    __int16 field_5C6_pad;
};
ALIVE_ASSERT_SIZEOF(Gibs, 0x5C8);

}
