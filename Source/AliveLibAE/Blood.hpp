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
    u8** field_F4_ppResBuf = nullptr;
    BloodParticle* field_F8_pResBuf = nullptr;
    Prim_SetTPage field_FC_tPages[2] = {};
    TPageMode field_11C_texture_mode = TPageMode::e4Bit_0;
    s16 field_11E_xpos = 0;
    s16 field_120_ypos = 0;
    s16 field_122_to_render_count = 0;
    u8 field_124_rand_seed = 0;
    s16 field_126_total_count = 0;
    s32 field_128_timer = 0;
    Layer field_12C_render_layer = Layer::eLayer_0;
};
ALIVE_ASSERT_SIZEOF(Blood, 0x130);
