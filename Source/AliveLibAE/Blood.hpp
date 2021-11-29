#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "../AliveLibCommon/FunctionFwd.hpp"
#include "Layer.hpp"

struct BloodParticle final
{
    FP field_0_x;
    FP field_4_y;
    FP field_8_offx;
    FP field_C_offy;
    Prim_Sprt field_10_prims[2];
};
ALIVE_ASSERT_SIZEOF(BloodParticle, 0x40);

class Blood final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    virtual BaseGameObject* VDestructor(s32 flags) override;
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;

    EXPORT Blood* ctor_40F0B0(FP xpos, FP ypos, FP xOff, FP yOff, FP scale, s16 count);

private:
    EXPORT void dtor_40F5D0();

    EXPORT BaseGameObject* vDtor_40F5A0(s32 /*flags*/);
    EXPORT void vUpdate_40F650();
    EXPORT void vRender_40F780(PrimHeader** ppOt);
    EXPORT void vScreenChanged_40FAD0();

private:
    u8** field_F4_ppResBuf;
    BloodParticle* field_F8_pResBuf;
    Prim_SetTPage field_FC_tPages[2];
    TPageMode field_11C_texture_mode;
    // pad
    s16 field_11E_xpos;
    s16 field_120_ypos;
    s16 field_122_to_render_count;
    u8 field_124_rand_seed;
    // pad
    s16 field_126_total_count;
    s32 field_128_timer;
    Layer field_12C_render_layer;
    //s16 field_12E_pad;
};
ALIVE_ASSERT_SIZEOF(Blood, 0x130);
