#pragma once

#include "FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Primitives.hpp"

namespace AO {

class CircularFade : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT CircularFade* ctor_479E20(FP xpos, FP ypos, FP scale, s16 direction, s8 destroyOnDone);

    virtual void VScreenChanged() override;

    virtual BaseGameObject* VDestructor(s32 flags) override;

    virtual void VUpdate() override;

    EXPORT void VUpdate_47A030();

    virtual void VRender(PrimHeader** ppOt) override;

    EXPORT void VRender_47A080(PrimHeader** ppOt);

    // New virtuals
    virtual EXPORT s8 VFadeIn_479FE0(u8 direction, s8 destroyOnDone);

    virtual EXPORT s32 VDone_47A4C0();

    s32 field_D4[4];
    enum Flags
    {
        eBit1_FadeIn = 0x1,
        eBit2_Done = 0x2,
        eBit3_DestroyOnDone = 0x4,
        eBit4_NeverSet = 0x8
    };
    BitField16<Flags> field_E4_flags;
    s16 field_E6_pad;
    Prim_Tile field_E8[2];
    Prim_Tile field_110[2];
    Prim_Tile field_138[2];
    Prim_Tile field_160[2];
    Prim_SetTPage field_188_tPage[2];
    s16 field_1A8_fade_colour;
    s16 field_1AA_speed;
};
ALIVE_ASSERT_SIZEOF(CircularFade, 0x1AC);

EXPORT CircularFade* CC Make_Circular_Fade_447640(FP xpos, FP ypos, FP scale, s16 direction, s8 destroyOnDone);

} // namespace AO
