#pragma once

#include "../relive_lib/GameObjects/BaseAnimatedWithPhysicsGameObject.hpp"

namespace AO {

class BirdPortalTerminator final : public BaseAnimatedWithPhysicsGameObject
{
public:
    BirdPortalTerminator(FP xpos, FP ypos, FP scale, relive::Path_BirdPortal::PortalType portalType);

    void LoadAnimations();

    virtual void VScreenChanged() override;

    void Fadeout();
};

} // namespace AO
