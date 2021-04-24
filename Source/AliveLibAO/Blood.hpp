#pragma once

#include "FunctionFwd.hpp"
#include "Primitives.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "FixedPoint.hpp"
#include "Layer.hpp"

namespace AO {

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
    EXPORT Blood* ctor_4072B0(FP xpos, FP ypos, FP xOff, FP yOff, FP scale, s16 count);

    EXPORT BaseGameObject* dtor_4076F0();

    virtual void VUpdate() override;

    EXPORT void VUpdate_407750();

    virtual BaseGameObject* VDestructor(s32 flags) override;

    EXPORT BaseGameObject* Vdtor_407AC0(u32 flags);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_407AB0();

    virtual void VRender(PrimHeader** ppOt) override;

    EXPORT void VRender_407810(PrimHeader** ppOt);

    int field_D4_not_used[4];
    BYTE** field_E4_ppResBuf;
    BloodParticle* field_E8_pResBuf;
    Prim_SetTPage field_EC_tPages[2];
    TPageMode field_10C_texture_mode;
    // pad
    s16 field_10E_xpos;
    s16 field_110_ypos;
    s16 field_112_to_render_count;
    BYTE field_114_rand_seed;
    char field_115_pad;
    s16 field_116_total_count;
    int field_118_timer;
    Layer field_11C_render_layer;
    s16 field_11E_pad;
};
ALIVE_ASSERT_SIZEOF(Blood, 0x120);

}

