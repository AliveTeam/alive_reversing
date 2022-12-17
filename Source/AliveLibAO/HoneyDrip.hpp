#pragma once

#include "../relive_lib/GameObjects/BaseAnimatedWithPhysicsGameObject.hpp"

namespace AO {

class HoneyDrip final : public BaseAnimatedWithPhysicsGameObject
{
public:
    HoneyDrip(FP xpos, FP ypos);

    virtual void VUpdate() override;

    bool mSoundPlaying = false;
};

} // namespace AO
