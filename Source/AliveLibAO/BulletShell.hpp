#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

class PathLine;
namespace AO {

class BulletShell final : public BaseAnimatedWithPhysicsGameObject
{
public:
    BulletShell(FP xpos, FP ypos, s32 direction, FP scale);

    virtual void VUpdate() override;

    s32 field_D4[4] = {};
    PathLine* field_E4_pLine = nullptr;
    s32 field_E8 = 0;
    s16 field_EC_hitCount = 0;
    s16 field_EE = 0;
    FP field_F0_speed = {};
};
ALIVE_ASSERT_SIZEOF(BulletShell, 0xF4);

} // namespace AO
