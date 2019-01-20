#pragma once

#include "FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

struct GibPart
{
    FP field_0_x;
    FP field_4_y;
    FP field_8_z;
    FP field_C_dx;
    FP field_10_dy;
    FP field_14_dz;
    AnimationEx field_18_anim;
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
    EXPORT Gibs* ctor_40FB40(int gibType, FP xpos, FP ypos, FP xOff, FP yOff, FP scale, __int16 bMakeSmaller);
    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VRender(int** pOrderingTable) override;
private:
    EXPORT void dtor_410170();
    EXPORT Gibs* vdtor_410100(signed int flags);
    EXPORT void vUpdate_410210();
    EXPORT void vRender_4103A0(int** ot);
private:
    int field_E4_not_used[4];
    const Gib_Data* field_F4_pGibData;
    FP field_F8_z;
    FP field_FC_dz;
    int field_100_timer;
    GibPart field_104_parts[7];
    __int16 field_5D4_parts_used_count;
    __int16 field_5D6_bMakeSmaller;
};
ALIVE_ASSERT_SIZEOF(Gibs, 0x5D8);
