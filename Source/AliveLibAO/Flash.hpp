#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "EffectBase.hpp"
#include "Layer.hpp"

namespace AO {

class Flash final : public EffectBase
{
public:
    Flash(Layer layer, s32 r, s32 g, s32 b, s32 not_used = 1, TPageAbr abr = TPageAbr::eBlend_3, s32 time = 1);

    virtual void VUpdate() override;

    s32 field_68_flash_time;
};
ALIVE_ASSERT_SIZEOF(Flash, 0x6C);

} // namespace AO
