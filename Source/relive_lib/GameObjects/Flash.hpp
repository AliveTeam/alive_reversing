#pragma once

#include "EffectBase.hpp"
#include "../Layer.hpp"

class Flash final : public EffectBase
{
public:
    Flash(Layer layer, s32 r, s32 g, s32 b, relive::TBlendModes abr = relive::TBlendModes::eBlend_3, s32 time = 1);

    virtual void VUpdate() override;
private:
    s32 mFlashTime = 0;
};
