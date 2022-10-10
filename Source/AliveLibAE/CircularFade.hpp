#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"

class CircularFade final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    CircularFade(FP xpos, FP ypos, FP scale, s16 direction, s8 destroyOnDone);
    ~CircularFade();

    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;

    // New virtuals
    virtual s32 VFadeIn(s16 direction, s8 destroyOnDone); // TODO: Likely no return
    virtual s32 VDone();

private:
    enum Flags
    {
        eBit1_FadeIn = 0x1,
        eBit2_Done = 0x2,
        eBit3_DestroyOnDone = 0x4,
        eBit4_NeverSet = 0x8
    };
    BitField16<Flags> field_F4_flags = {};
    Prim_Tile field_F8_tile1[2] = {};
    Prim_Tile field_120_tile2[2] = {};
    Prim_Tile field_148_tile3[2] = {};
    Prim_Tile field_170_tile4[2] = {};
    Prim_SetTPage field_198_tPages[2] = {};
    s16 field_1B8_fade_colour = 0;
    s16 field_1BA_speed = 0;
};

CircularFade* Make_Circular_Fade_4CE8C0(FP xpos, FP ypos, FP scale, s16 direction, s8 destroyOnDone, s8 surviveDeathReset);
