#pragma once

#include "AnimationBase.hpp"
#include "Primitives.hpp"

namespace AO {

class Animation;

// TODO: Figure out how this differs from the standard Animation
class AnimationUnknown final : public AnimationBase
{
public:
    virtual void VDecode() override;

    virtual void VRender(s32 xpos, s32 ypos, PrimHeader** ppOt, s16 width, s32 height) override;

    virtual void VCleanUp() override;

    virtual void VRender2(s32 xpos, s32 ypos, PrimHeader** ppOt);

    void GetRenderedSize(PSX_RECT* pRect);

private:
    void VCleanUp2_404280();

    // New virtual
    void VRender2_403FD0(s32 xpos, s32 ypos, PrimHeader** ppOt);

public:
    Poly_FT4 field_10_polys[2];
    Animation* field_68_anim_ptr;
    FP field_6C_scale;
};
ALIVE_ASSERT_SIZEOF(AnimationUnknown, 0x70);
} // namespace AO
