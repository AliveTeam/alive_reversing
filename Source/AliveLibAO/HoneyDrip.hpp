#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Path.hpp"

namespace AO {

struct Path_HoneyDripTarget final : public Path_TLV
{
    // No fields
};

class HoneyDrip final : public BaseAnimatedWithPhysicsGameObject
{
public:
    HoneyDrip(FP xpos, FP ypos);

    virtual void VUpdate() override;

    s32 field_D4[4] = {};
    s16 field_E4_bSoundPlaying = 0;
    s16 field_E6 = 0;
};
ALIVE_ASSERT_SIZEOF(HoneyDrip, 0xE8);

} // namespace AO
