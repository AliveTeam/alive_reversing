#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"
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

    bool mSoundPlaying = false;
};
ALIVE_ASSERT_SIZEOF(HoneyDrip, 0xE8);

} // namespace AO
