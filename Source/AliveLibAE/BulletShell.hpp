#pragma once

#include "../relive_lib/GameObjects/BaseAnimatedWithPhysicsGameObject.hpp"

class PathLine;

class BulletShell final : public BaseAnimatedWithPhysicsGameObject
{
public:
    BulletShell(FP xpos, FP ypos, s16 direction, FP scale);
    ~BulletShell();

    virtual void VUpdate() override;

private:
    PathLine* mLine = nullptr;
    s16 mFloorBounceCount = 0;
    FP mSpeed = FP_FromInteger(1);
};
