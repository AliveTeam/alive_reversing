#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "EffectBase.hpp"
#include "Layer.hpp"

class Flash final : public EffectBase
{
public:
    Flash(Layer layer, u8 r, u8 g, u8 b, s32 /*not_used*/, TPageAbr abr, s32 time);

    virtual void VUpdate() override;
private:
    s32 field_78_flash_time;
};
ALIVE_ASSERT_SIZEOF(Flash, 0x7C);
