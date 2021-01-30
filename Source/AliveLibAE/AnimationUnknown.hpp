#pragma once

#include "Animation.hpp"

// TODO: Figure out how this differs from the standard Animation.
class AnimationUnknown : public AnimationBase
{
public:
    virtual void vDecode_40AC90() override;
    virtual void vRender_40B820(int xpos, int ypos, PrimHeader** ppOt, __int16 width, signed int height) override;
    virtual void vCleanUp_40C630() override;

    EXPORT void GetRenderedSize_40C980(PSX_RECT* pRect);

private:
    EXPORT void vRender_40C690(int xpos, int ypos, PrimHeader** ppOt, int width, int height);
    EXPORT void vCleanUp_40C9C0();

public:
    Poly_FT4 field_10_polys[2];
    Animation* field_68_anim_ptr;
    FP field_6C_scale;
};
ALIVE_ASSERT_SIZEOF(AnimationUnknown, 0x70);
