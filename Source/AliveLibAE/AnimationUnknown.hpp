#pragma once

#include "../relive_lib/Animation.hpp"

// TODO: Figure out how this differs from the standard Animation.
class AnimationUnknown final : public AnimationBase
{
public:
    virtual void VDecode() override;
    virtual void VRender(s32 xpos, s32 ypos, PrimHeader** ppOt, s16 /*width*/, s32 /*height*/) override;
    virtual void VCleanUp() override;

    void GetRenderedSize(PSX_RECT* pRect);

public:
    Poly_FT4 field_10_polys[2] = {};
    Animation* field_68_anim_ptr = nullptr;
    FP field_6C_scale = {};
};
ALIVE_ASSERT_SIZEOF(AnimationUnknown, 0x70);
