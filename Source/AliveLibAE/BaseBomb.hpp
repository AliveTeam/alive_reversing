#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "PlatformBase.hpp"
#include "Path.hpp"

class BaseBomb final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    virtual void VUpdate() override;

    BaseBomb(FP x, FP y, s32 unused, FP scale);

private:
    FP field_f4_scale;
};
ALIVE_ASSERT_SIZEOF(BaseBomb, 0xF8);
