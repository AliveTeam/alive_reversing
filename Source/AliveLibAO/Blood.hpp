#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/Primitives.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "FixedPoint.hpp"
#include "../relive_lib/Layer.hpp"

namespace AO {

void Blood_ForceLink();

struct BloodParticle final
{
    FP x;
    FP y;
    FP field_8_offx;
    FP field_C_offy;
    Prim_Sprt field_10_prims[2];
};
ALIVE_ASSERT_SIZEOF(BloodParticle, 0x40);

class Blood final : public BaseAnimatedWithPhysicsGameObject
{
public:
    Blood(FP xpos, FP ypos, FP xOff, FP yOff, FP scale, s32 count);
    ~Blood();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual void VRender(PrimHeader** ppOt) override;

    u8** field_E4_ppResBuf = nullptr;
    BloodParticle* field_E8_pResBuf = nullptr;
    Prim_SetTPage field_EC_tPages[2] = {};
    TPageMode field_10C_texture_mode = TPageMode::e4Bit_0;
    s16 field_10E_xpos = 0;
    s16 field_110_ypos = 0;
    s16 field_112_to_render_count = 0;
    u8 field_114_rand_seed = 0;
    s16 field_116_total_count = 0;
    s32 field_118_timer = 0;
    Layer field_11C_render_layer = Layer::eLayer_0;
};
ALIVE_ASSERT_SIZEOF(Blood, 0x120);

} // namespace AO
