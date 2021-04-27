#pragma once

#include "FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

namespace AO {

class AnimationUnknown;

class Rope : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT Rope* ctor_458520(u16 left, s16 top, u16 bottom, FP scale);

    EXPORT BaseGameObject* dtor_458720();

    virtual BaseGameObject* VDestructor(s32 flags) override;

    EXPORT Rope* Vdtor_458B60(s32 flags);

    virtual void VUpdate() override;

    virtual void VRender(PrimHeader** ppOt) override;

    EXPORT void VRender_458780(PrimHeader** ppOt);

    s32 field_D4[4];
    s16 field_E4_rope_segment_count;
    s16 field_E6_rope_length;
    AnimationUnknown* field_E8_pRopeRes;
    s16 field_EC_left;
    s16 field_EE_top;
    s16 field_F0_right;
    s16 field_F2_bottom;
};
ALIVE_ASSERT_SIZEOF(Rope, 0xF4);

EXPORT void CC ClipPoly_Vertically_4584B0(Poly_FT4* pPoly, s32 minY, s32 maxY);

} // namespace AO
