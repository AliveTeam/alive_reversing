#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"

class PathLine;

class BulletShell final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    BulletShell(FP xpos, FP ypos, s16 direction, FP scale);
    ~BulletShell();

    virtual void VUpdate() override;

private:
    PathLine* BaseAliveGameObjectCollisionLine = nullptr;
    s16 field_FC_hitCount = 0;
    FP field_100_speed = {};
};
ALIVE_ASSERT_SIZEOF(BulletShell, 0x104);
