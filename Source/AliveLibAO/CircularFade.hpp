#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"
#include "../relive_lib/Primitives.hpp"

namespace AO {

class CircularFade final : public BaseAnimatedWithPhysicsGameObject
{
public:
    CircularFade(FP xpos, FP ypos, FP scale, s16 direction, s8 destroyOnDone);

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;

    // New virtuals
    virtual s8 VFadeIn(u8 direction, s8 destroyOnDone);
    virtual s32 VDone();

    s32 field_D4[4] = {};
    enum Flags
    {
        eBit1_FadeIn = 0x1,
        eBit2_Done = 0x2,
        eBit3_DestroyOnDone = 0x4,
        eBit4_NeverSet = 0x8
    };
    BitField16<Flags> field_E4_flags = {};
    Prim_Tile field_E8[2] = {};
    Prim_Tile field_110[2] = {};
    Prim_Tile field_138[2] = {};
    Prim_Tile field_160[2] = {};
    Prim_SetTPage field_188_tPage[2] = {};
    s16 field_1A8_fade_colour = 0;
    s16 field_1AA_speed = 0;
};
ALIVE_ASSERT_SIZEOF(CircularFade, 0x1AC);

CircularFade* Make_Circular_Fade(FP xpos, FP ypos, FP scale, s16 direction, s8 destroyOnDone);

} // namespace AO
