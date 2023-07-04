#pragma once

#include "Animation.hpp"

// TODO: Figure out how this differs from the standard Animation.
class AnimationUnknown final : public AnimationBase
{
public:
    virtual void VDecode() override;
    virtual void VRender(s32 xpos, s32 ypos, BasePrimitive** ppOt, s16 /*width*/, s32 /*height*/) override;
    virtual void VCleanUp() override;

    void GetRenderedSize(PSX_RECT* pRect);

public:
    Poly_FT4 mPolys[2] = {};
    Animation* mAnimPtr = nullptr;
    FP mSpriteScale = {};
};
