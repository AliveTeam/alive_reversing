#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "EffectBase.hpp"
#include "../relive_lib/Layer.hpp"

class Flash final : public EffectBase
{
public:
    Flash(Layer layer, s32 r, s32 g, s32 b, TPageAbr abr = TPageAbr::eBlend_3, s32 time = 1);

    virtual void VUpdate() override;
private:
    s32 mFlashTime = 0;
};
