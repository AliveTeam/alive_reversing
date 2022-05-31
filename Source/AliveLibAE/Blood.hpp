#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "../AliveLibCommon/Function.hpp"
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
    
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;

    Blood(FP xpos, FP ypos, FP xOff, FP yOff, FP scale, s32 count);
    ~Blood();

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
