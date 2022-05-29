#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

namespace AO {

class HoneyDrip final : public BaseAnimatedWithPhysicsGameObject
{
public:
    HoneyDrip(FP xpos, FP ypos);

    virtual void VUpdate() override;

    EXPORT void VUpdate_431DC0();

    s32 field_D4[4];
    s16 field_E4_bSoundPlaying;
    s16 field_E6;
};
ALIVE_ASSERT_SIZEOF(HoneyDrip, 0xE8);

} // namespace AO
