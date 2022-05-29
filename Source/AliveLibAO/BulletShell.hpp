#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

namespace AO {

class PathLine;

class BulletShell final : public BaseAnimatedWithPhysicsGameObject
{
public:
    BulletShell(FP xpos, FP ypos, s16 direction, FP scale);

    virtual void VUpdate() override;

    EXPORT void VUpdate_4628C0();

    s32 field_D4[4];
    PathLine* field_E4_pLine;
    s32 field_E8;
    s16 field_EC_hitCount;
    s16 field_EE;
    FP field_F0_speed;
};
ALIVE_ASSERT_SIZEOF(BulletShell, 0xF4);

} // namespace AO
