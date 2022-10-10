#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"

class PathLine;
namespace AO {

class BulletShell final : public BaseAnimatedWithPhysicsGameObject
{
public:
    BulletShell(FP xpos, FP ypos, s32 direction, FP scale);

    virtual void VUpdate() override;

    PathLine* mLine = nullptr;
    s16 mFloorBounceCount = 0;
    FP mSpeed = {};
};

} // namespace AO
