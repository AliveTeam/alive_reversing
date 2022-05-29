#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

class CircularFade final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;

    // New virtuals
    virtual s32 VFadeIn_4CE300(s16 direction, s8 destroyOnDone);
    virtual s32 VDone_4CE0B0();

    CircularFade(FP xpos, FP ypos, FP scale, s16 direction, s8 destroyOnDone);
    ~CircularFade();

private:
    EXPORT void vRender_4CE3F0(PrimHeader** ppOt);
    EXPORT void vUpdate_4CE380();
    EXPORT s32 vFadeIn_4CE300(s16 direction, s8 destroyOnDone); // TODO: Likely no return
    EXPORT s32 vDone_4CE0B0();

private:
    enum Flags
    {
        eBit1_FadeIn = 0x1,
        eBit2_Done = 0x2,
        eBit3_DestroyOnDone = 0x4,
        eBit4_NeverSet = 0x8
    };
    BitField16<Flags> field_F4_flags;
    s16 field_F6_padding;
    Prim_Tile field_F8_tile1[2];
    Prim_Tile field_120_tile2[2];
    Prim_Tile field_148_tile3[2];
    Prim_Tile field_170_tile4[2];
    Prim_SetTPage field_198_tPages[2];
    s16 field_1B8_fade_colour;
    s16 field_1BA_speed;
};
ALIVE_ASSERT_SIZEOF(CircularFade, 0x1BC);

EXPORT CircularFade* CC Make_Circular_Fade_4CE8C0(FP xpos, FP ypos, FP scale, s16 direction, s8 destroyOnDone, s8 surviveDeathReset);
