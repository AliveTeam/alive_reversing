#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

namespace AO {

class AnimationUnknown;

class Rope final : public BaseAnimatedWithPhysicsGameObject
{
public:
    Rope(s32 left, s32 top, s32 bottom, FP scale);
    ~Rope();

    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;

    s32 field_D4_padding[4];
    s16 field_E4_rope_segment_count;
    s16 field_E6_rope_length;
    AnimationUnknown* field_E8_pRopeRes;
    s16 field_EC_left;
    s16 field_EE_top;
    s16 field_F0_right;
    s16 field_F2_bottom;
};
ALIVE_ASSERT_SIZEOF(Rope, 0xF4);

void ClipPoly_Vertically_4584B0(Poly_FT4* pPoly, s32 minY, s32 maxY);

} // namespace AO
