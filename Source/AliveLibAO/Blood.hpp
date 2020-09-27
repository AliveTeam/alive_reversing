#pragma once

#include "FunctionFwd.hpp"
#include "Primitives.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "FixedPoint.hpp"

START_NS_AO

void Blood_ForceLink();

struct BloodParticle
{
    FP field_0_x;
    FP field_4_y;
    FP field_8_offx;
    FP field_C_offy;
    Prim_Sprt field_10_prims[2];
};
ALIVE_ASSERT_SIZEOF(BloodParticle, 0x40);

class Blood : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT Blood* ctor_4072B0(FP xpos, FP ypos, FP xOff, FP yOff, FP scale, __int16 count);

    EXPORT BaseGameObject* dtor_4076F0();

    virtual void VUpdate() override;

    EXPORT void VUpdate_407750();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT BaseGameObject* Vdtor_407AC0(unsigned int flags);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_407AB0();

    int field_D4_not_used[4];
    BYTE** field_E4_ppResBuf;
    BloodParticle* field_E8_pResBuf;
    Prim_SetTPage field_EC_tPages[2];
    __int16 field_10C_texture_mode;
    __int16 field_10E_xpos;
    __int16 field_110_ypos;
    __int16 field_112_to_render_count;
    BYTE field_114_rand_seed;
    char field_115_pad;
    __int16 field_116_total_count;
    int field_118_timer;
    __int16 field_11C_render_layer;
    __int16 field_11E_pad;
};
ALIVE_ASSERT_SIZEOF(Blood, 0x120);

END_NS_AO

